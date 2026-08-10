#include <iostream>

#include <forgeant/forgeant.hpp>

int main(int argc, char* argv[])
{
    auto agent = forgeant::Agent::create("openai", {
        .model = "Qwen3-8B-Q8_0",
        .base_url = "192.168.1.17:8080",
        .system_prompt = "You are a helpful assistant.",
    });

    auto result = agent->run("Hello, MyAgent!");

    std::cout << "Response: " << result.output << std::endl;
    std::cout << "Tokens: " << result.total_usage.input_tokens << " input, "
              << result.total_usage.output_tokens << " output" << std::endl;
    std::cout << "Iterations: " << result.iterations << std::endl;
    return 0;
}
