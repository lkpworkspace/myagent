#include <iostream>
#include "myframe/common.h"
#include "myagent/common.h"
#include "myagent/wechat_tool.h"

namespace myagent {

WeChatTool::WeChatTool()
  : forgeant::Tool(
      R"(WeChat)",
      R"(微信聊天工具)",
      Schema(),
      std::bind(&WeChatTool::Execute, this, std::placeholders::_1))
{}

WeChatTool::~WeChatTool() {}

forgeant::Json WeChatTool::Schema() {
  return forgeant::Schema::object()
    .property("user", forgeant::Schema::string().description(R"(联系人)").build())
    .property("message", forgeant::Schema::string().description(R"(消息内容)").build())
    .required({"user", "message"})
    .build();
}

forgeant::Json WeChatTool::Execute(const forgeant::Json& input) {
  forgeant::Json response;
  auto user = input.value<std::string>("user", "");
  auto message = input.value<std::string>("message", "");
  if (user.empty() || message.empty()) {
    response["error"] = "error";
    response["detail"] = "user or message is empty";
    return response;
  }
  // execute apple script
  std::cout << "[myagent.TOOL] wechat recv user:" << user << ", message:" << message << std::endl;
  auto root_dir = myframe::Common::GetWorkRoot();
  std::string script_filepath = (root_dir / "resources/wechat.applescript").string();
  std::string cmd = "osascript " + script_filepath
    + " \"" + user + "\""
    + " \"" + message + "\"";
  try {
    std::string output = Common::ExecCommand(cmd);
    if (output == "OK") {
      response["error"] = "OK";
    } else {
      response["error"] = "error";
      response["detail"] = output;
    }
  } catch (const std::exception& e) {
    response["error"] = "error";
    response["detail"] = e.what();
  }
  std::cout << "[myagent.TOOL] wechat result: " << response.dump() << std::endl;
  return response;
}

}  // namespace myagent
