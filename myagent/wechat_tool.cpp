#include <iostream>
#include "myframe/common.h"
#include "wechat_tool.h"

namespace myagent {

std::string ExecCommand(const std::string& cmd) {
  std::array<char, 128> buffer;
  std::string result;
  std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd.c_str(), "r"), pclose);
  if (!pipe) {
    throw std::runtime_error("popen() failed!");
  }
  while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
    result += buffer.data();
  }
  std::cout << "[myagent] exec command \"" << cmd << "\",result: "
    << result << std::endl;
  if (!result.empty() && result.back() == '\n') {
    result.pop_back();
  }
  return result;
}

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
    response["state"] = "ERROR";
    response["message"] = "user or message is empty";
    return response;
  }
  // execute apple script
  std::cout << "[myagent.TOOL] wechat recv: user:" << user << ", message:" << message << std::endl;
  auto root_dir = myframe::Common::GetWorkRoot();
  std::string script_filepath = (root_dir / "resources/wechat.applescript").string();
  std::string cmd = "osascript " + script_filepath
    + " " + user
    + " \"" + message + "\"";
  try {
    std::string output = ExecCommand(cmd);
    if (output == "OK") {
      response["state"] = "OK";
    } else {
      response["state"] = "ERROR";
      response["message"] = output;
    }
  } catch (const std::exception& e) {
    response["state"] = "ERROR";
    response["message"] = e.what();
  }
  std::cout << "[myagent.TOOL] wechat result: " << response.dump() << std::endl;
  return response;
}

}  // namespace myagent
