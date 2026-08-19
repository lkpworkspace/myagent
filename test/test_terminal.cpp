#include <iostream>
#include "forgeant/forgeant.hpp"
#include "myagent/terminal_tool.h"

int main(int argc, char* argv[]) {
  myagent::TerminalTool terminal_tool;

  auto j = forgeant::Json::object({
    {"command", R"(ls)"},
  });
  terminal_tool.Execute(j);
  return 0;
}
