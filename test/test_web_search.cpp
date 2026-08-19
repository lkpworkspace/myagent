#include <iostream>
#include "forgeant/forgeant.hpp"
#include "myagent/web_search_tool.h"

int main(int argc, char* argv[]) {
  myagent::WebSearchTool web_search_tool;

  auto j = forgeant::Json::object({
    {"query", R"(本地部署最强的模型有哪些?)"},
  });
  web_search_tool.Execute(j);
  return 0;
}

