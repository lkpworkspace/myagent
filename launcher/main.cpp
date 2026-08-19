#include <iostream>

#include <forgeant/forgeant.hpp>
#include "myagent/wechat_tool.h"
#include "get_weather_tool.hpp"

void print_conv(const forgeant::Conversation& conv) {
  forgeant::Json j;
  forgeant::to_json(j, conv);
  std::cout << j.dump() << std::endl;
}

void print_schema() {
  auto j = forgeant::Schema::object()
    .property("title", forgeant::Schema::string().description("Movie title").build())
    .property("rating",
              forgeant::Schema::integer().description("Rating from 1 to 10").build())
    .property("summary",
              forgeant::Schema::string().description("One sentence summary").build())
    .required({"title", "rating", "summary"})
    .build();

  auto j2 = forgeant::Schema::object()
    .property("city", forgeant::Schema::string().description("The city name").build())
    .property("unit", forgeant::Schema::string()
      .description("Temperature unit")
      .enum_values({"fahrenheit", "celsius"})
      .default_value("fahrenheit")
      .build())
    .required({"city"})
    .build();
  std::cout << j.dump() << std::endl;
  std::cout << j2.dump() << std::endl;
}

void basic_chat(std::unique_ptr<forgeant::Agent>& agent) {
  auto result = agent->run("Hello, MyAgent!");
  std::cout << "Response: " << result.output << std::endl;
  std::cout << "Tokens: " << result.total_usage.input_tokens
    << " input, " << result.total_usage.output_tokens << " output"
    << std::endl;
  std::cout << "Iterations: " << result.iterations << std::endl;
}

void mutil_turn(std::unique_ptr<forgeant::Agent>& agent) {
  std::cout << "=== Turn 1 ===" << std::endl;
  std::cout << "User: My name is Alice. Remember that." << std::endl;
  auto r1 = agent->run("My name is Alice. Remember that.");
  std::cout << "Assistant: " << r1.output << std::endl;

  std::cout << "\n=== Turn 2 ===" << std::endl;
  std::cout << "User: What is my name?" << std::endl;
  forgeant::Conversation conv = r1.conversation;
  conv.add(forgeant::Message(forgeant::Role::user, "What is my name?"));
  auto r2 = agent->run(conv);
  std::cout << "Assistant: " << r2.output << std::endl;

  std::cout << "\n=== Turn 3 ===" << std::endl;
  std::cout << "User: How many messages have we exchanged so far?" << std::endl;
  conv = r2.conversation;
  conv.add(
      forgeant::Message(forgeant::Role::user, "How many messages have we exchanged so far?"));
  auto r3 = agent->run(conv);
  std::cout << "Assistant: " << r3.output << std::endl;

  std::cout << "\nConversation has " << r3.conversation.messages().size() << " messages"
              << std::endl;
}

void get_weather_tool_usage(std::unique_ptr<forgeant::Agent>& agent) {
  add_get_weather_tool(agent);
  auto result = agent->run("What's the weather like in Denver?");
  std::cout << "Response: " << result.output << std::endl;
  std::cout << "Iterations: " << result.iterations << std::endl;
  print_conv(result.conversation);
}

void wechat_tool_usage(std::unique_ptr<forgeant::Agent>& agent) {
  myagent::WeChatTool tool;
  agent->add_tool(std::move(tool));
  auto result = agent->run(R"(用微信给"老婆"发一条七夕快乐祝福语并注明是用myagent发送的)");
  std::cout << "Response: " << result.output << std::endl;
  std::cout << "Iterations: " << result.iterations << std::endl;
  print_conv(result.conversation);
}

int main(int argc, char* argv[]) {
  auto agent = forgeant::Agent::create("openai", {
    .model = "Qwen3-8B-Q8_0",
    .base_url = "127.0.0.1:8080",
    .system_prompt = "You are a helpful assistant, answer questions in chinese.",
  });

//   get_weather_tool_usage(agent);
  wechat_tool_usage(agent);

  return 0;
}
