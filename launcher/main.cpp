#include <iostream>

#include <forgeant/forgeant.hpp>

int main(int argc, char* argv[])
{
    auto agent = forgeant::Agent::create("openai", {
        .model = "Qwen3-8B-Q8_0",
        .base_url = "127.0.0.1:8080",
        .system_prompt = "You are a helpful assistant, answer questions in chinese.",
    });

    // auto result = agent->run("Hello, MyAgent!");
    // std::cout << "Response: " << result.output << std::endl;
    // std::cout << "Tokens: " << result.total_usage.input_tokens << " input, "
    //           << result.total_usage.output_tokens << " output" << std::endl;
    // std::cout << "Iterations: " << result.iterations << std::endl;

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

    return 0;
}
