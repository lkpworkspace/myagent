#include <iostream>
#include <curl/curl.h>
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
  auto tavily_api_key = std::getenv("TAVILY_API_KEY");
  if (tavily_api_key == nullptr) {
    response = SearchDuckDuckGo(query);
    // return forgeant::Json::object({
    //     {"error", "error"},
    //     {"detail", "Tavily api key is empty"},
    //   });
  } else {
    response = SearchTavily(query, max_results, tavily_api_key);
  }
  std::cout << "[myagent.TOOL] web search result: " << response.dump() << std::endl;
  return response;
}

forgeant::Json WebSearchTool::SearchTavily(
    const std::string& query,
    int max_results,
    const std::string& api_key) {
  std::string url = "https://api.tavily.com/search";
  auto body = forgeant::Json::object({
    {"api_key", api_key},
    {"query", query},
    {"max_results", max_results},
    {"include_answer", true},
  });
  forgeant::HttpHeaders header = {
    {"Content-Type", "application/json"}
  };
  forgeant::CurlHttpClient client;
  forgeant::HttpResponse http_resp;
  try {
    http_resp = client.post(url, header, body.dump());
  } catch (const std::exception& e) {
    return forgeant::Json::object({
      {"error", "error"},
      {"detail", e.what()}
    });
  }
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

forgeant::Json WebSearchTool::SearchDuckDuckGo(const std::string& query) {
  // DuckDuckGo Instant Answer API
  // https://api.duckduckgo.com/?q=QUERY&format=json
  // 注意: 这是即时问答 API, 不是完整搜索, 结果可能为空
  forgeant::CurlHttpClient client;
  std::string url = "https://api.duckduckgo.com/";
  std::unordered_map<std::string, std::string> params = {
    {"q", UrlEncode(query)},
    {"format", "json"},
    {"no_html", "1"},
    {"skip_disambig", "1"},
  };
  bool is_first = true;
  for (const auto& p : params) {
    if (is_first) {
      url += "?";
      is_first = false;
    } else {
      url += "&";
    }
    url += p.first;
    url += "=";
    url += p.second;
  }
  std::cout << "[myagent.TOOL] DuckDuckGo search: " << url << std::endl;
  forgeant::HttpHeaders header = {};
  forgeant::HttpResponse http_resp;
  try {
    http_resp = client.post(url, header, "");
  } catch (const std::exception& e) {
    return forgeant::Json::object({
      {"error", "error"},
      {"detail", e.what()}
    });
  }
  auto resp = forgeant::Json::parse(http_resp.body);
  if (http_resp.status_code != 200) {
    return forgeant::Json::object({
        {"error", "DuckDuckGo API error: HTTP " + std::to_string(http_resp.status_code)}
    });
  }

  auto results = forgeant::Json::array();

  // 提取摘要文本
  if (!resp.value("AbstractText", "").empty()) {
    results.push_back(forgeant::Json::object({
      {"type", "answer"},
      {"content", resp["AbstractText"]},
      {"source", resp.value("AbstractURL", "")}
    }));
  }

  // 提取相关主题
  if (resp.contains("RelatedTopics")) {
    for (const auto& topic : resp["RelatedTopics"]) {
      if (topic.contains("Text")) {
        results.push_back(forgeant::Json::object({
          {"title", topic.value("FirstURL", "")},
          {"content", topic.value("Text", "")},
          {"url", topic.value("FirstURL", "")}
        }));
      }
    }
  }

  return forgeant::Json::object({{"query", query}, {"results", results}});
}

std::string WebSearchTool::UrlEncode(const std::string& input) {
  CURL* curl = curl_easy_init();
  char* encoded = curl_easy_escape(curl, input.c_str(), static_cast<int>(input.length()));
  std::string result(encoded);
  curl_free(encoded);
  curl_easy_cleanup(curl);
  return result;
}

}  // namespace myagent
