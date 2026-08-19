#include <iostream>
#include "forgeant/forgeant.hpp"
#include "myagent/wechat_tool.h"

int main(int argc, char* argv[]) {
  myagent::WeChatTool wechat_tool;

  auto j = forgeant::Json::object({
    {"user", R"(老婆)"},
    {"message", R"(Hello, AI Agent auto sending)"},
  });
  wechat_tool.Execute(j);
  return 0;
}
