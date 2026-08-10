#include "llama_provider.h"

namespace myagent {

LlamaProvider::LlamaProvider()
{
}

LlamaProvider::~LlamaProvider() {}

forgeant::LlmResponse LlamaProvider::chat(
    const forgeant::Conversation& conversation,
    const forgeant::ChatRequest& request) {
  forgeant::LlmResponse response;
  return response;
}

}  // namespace myagent
