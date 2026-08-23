#include <iostream>

#include <forgeant/forgeant.hpp>
#include "myagent/wechat_tool.h"
#include "myagent/terminal_tool.h"
#include "myagent/web_search_tool.h"
#include "myagent/cron_tool.h"

void print_conv(const forgeant::Conversation& conv) {
  forgeant::Json j;
  forgeant::to_json(j, conv);
  std::cout << j.dump() << std::endl;
}

void reg_all_tool(std::unique_ptr<forgeant::Agent>& agent) {
  myagent::TerminalTool terminal_tool;
  agent->add_tool(std::move(terminal_tool));
  myagent::WeChatTool wechat_tool;
  agent->add_tool(std::move(wechat_tool));
  myagent::WebSearchTool web_search_tool;
  agent->add_tool(std::move(web_search_tool));
  myagent::CronTool cron_tool;
  agent->add_tool(std::move(cron_tool));
}

int main(int argc, char* argv[]) {
  auto agent = forgeant::Agent::create("openai", {
    .model = "Qwen3-8B-Q8_0",
    .base_url = "127.0.0.1:8080",
    .system_prompt = "You are a helpful assistant, answer questions in chinese.",
  });
  reg_all_tool(agent);

  std::cout << "Enter content (type q to quit, Ctrl+D for EOF):" << std::endl;
  std::cout << "------------------------------" << std::endl;
  std::string input;
  forgeant::Conversation conv;
  bool first_chat = true;
  while (true) {
    std::cout << "> ";
    if (!std::getline(std::cin, input)) {
      break;
    }
    if (input == "q") {
      break;
    }
    forgeant::AgentResult<std::string> result;
    if (first_chat) {
      first_chat = false;
      result = agent->run(input);
    } else {
      conv.add(forgeant::Message(forgeant::Role::user, input));
      result = agent->run(conv);
    }
    std::cout << "Response: " << result.output << std::endl;
    std::cout << "Iterations: " << result.iterations << std::endl;
    conv = result.conversation;
  }
  return 0;
}
