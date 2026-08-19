#include <iostream>
#include "myframe/common.h"
#include "myagent/common.h"
#include "myagent/web_search_tool.h"

namespace myagent {

const static std::string WEB_SEARCH_DESC = R"(
  Search the web for real-time information.
  Use this when you need current facts, news, or data beyond your training cutoff.
)";
WebSearchTool::WebSearchTool()
  : forgeant::Tool(
      R"(web_search)",
      WEB_SEARCH_DESC,
      Schema(),
      std::bind(&WebSearchTool::Execute, this, std::placeholders::_1))
{}

WebSearchTool::~WebSearchTool() {}

forgeant::Json WebSearchTool::Schema() {
  return forgeant::Schema::object()
    .property("query",
      forgeant::Schema::string().description(R"(The search query string)").build())
    .property("max_results",
      forgeant::Schema::integer()
        .description(R"(Maximum results to return)")
        .default_value(5)
        .build())
    .required({"query"})
    .build();
}

forgeant::Json WebSearchTool::Execute(const forgeant::Json& input) {
  forgeant::Json response;
  auto query = input.value<std::string>("query", "");
  auto max_results = input.value<int>("max_results", 5);
  if (query.empty()) {
    return forgeant::Json::object({
        {"error", "error"},
        {"detail", "Query is empty"},
      });
  }
  // execute web search
  std::cout << "[myagent.TOOL] web search query: " << query
    << ", max results:" << max_results << std::endl;
  response = SearchTavily(query, max_results);
  std::cout << "[myagent.TOOL] web search result: " << response.dump() << std::endl;
  return response;
}

forgeant::Json WebSearchTool::SearchTavily(
    const std::string& query,
    int max_results) {
  std::string tavily_api_key = std::getenv("TAVILY_API_KEY");
  std::cout << "Tavily api key: " << tavily_api_key << std::endl;
  if (tavily_api_key.empty()) {
    return forgeant::Json::object({
        {"error", "error"},
        {"detail", "Tavily api key is empty"},
      });
  }

  std::string url = "https://api.tavily.com/search";
  auto body = forgeant::Json::object({
    {"api_key", tavily_api_key},
    {"query", query},
    {"max_results", max_results},
    {"include_answer", true},
  });
  forgeant::HttpHeaders header = {
    {"Content-Type", "application/json"}
  };
  forgeant::CurlHttpClient client;
  auto http_resp = client.post(url, header, body.dump());
  /*
    struct HttpResponse {
        int status_code = 0;
        std::unordered_map<std::string, std::string> headers;
        std::string body;

        bool operator==(const HttpResponse&) const = default;
    };
  */
  auto resp = forgeant::Json::parse(http_resp.body);
  if (http_resp.status_code != 200) {
    return forgeant::Json::object({
        {"error", "Tavily API error: HTTP " + std::to_string(http_resp.status_code)},
        {"detail", http_resp.body}
    });
  }

  auto results = forgeant::Json::array();

  // 提取 AI 生成的摘要 (如果有)
  if (resp.contains("answer") && !resp["answer"].is_null()) {
    results.push_back(forgeant::Json::object({
      {"type", "answer"},
      {"content", resp["answer"]}
    }));
  }

  // 提取搜索结果列表
  if (resp.contains("results")) {
    for (const auto& item : resp["results"]) {
      results.push_back(forgeant::Json::object({
        {"title",   item.value("title", "")},
        {"url",     item.value("url", "")},
        {"content", item.value("content", "")},
        {"score",   item.value("score", 0.0)}
      }));
    }
  }

  return forgeant::Json::object({{"query", query}, {"results", results}});
}

}  // namespace myagent
