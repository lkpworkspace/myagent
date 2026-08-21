#include <iostream>
#include "myframe/common.h"
#include "myagent/common.h"
#include "myagent/terminal_tool.h"

namespace myagent {

const static std::string TERMINAL_DESC = R"(
  MacOS终端执行工具,在本地终端执行bash/shell命令;
  禁止执行危害系统的指令,比如删除指令,修改系统配置指令。
)";

TerminalTool::TerminalTool()
  : forgeant::Tool(
      R"(Terminal)",
      TERMINAL_DESC,
      Schema(),
      std::bind(&TerminalTool::Execute, this, std::placeholders::_1))
{}

TerminalTool::~TerminalTool() {}

forgeant::Json TerminalTool::Schema() {
  return forgeant::Schema::object()
    .property("command", forgeant::Schema::string()
      .description(R"(The shell command to execute)").build())
    .required({"command"})
    .build();
}

forgeant::Json TerminalTool::Execute(const forgeant::Json& input) {
  forgeant::Json response;
  auto cmd = input.value<std::string>("command", "");
  if (cmd.empty()) {
    response["error"] = "error";
    response["detail"] = "command is empty";
    return response;
  }
  cmd += " 2>&1";
  // execute apple script
  std::cout << "[myagent.TOOL] terminal recv command:" << cmd << std::endl;
  try {
    std::string output = Common::ExecCommand(cmd);
    response["output"] = output;
  } catch (const std::exception& e) {
    response["error"] = "error";
    response["detail"] = e.what();
  }
  std::cout << "[myagent.TOOL] terminal result: " << response.dump() << std::endl;
  return response;
}

}  // namespace myagent
