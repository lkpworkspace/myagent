#pragma once

#include <forgeant/forgeant.hpp>
#include "export.h"

namespace myagent {

class MYAGENT_EXPORT LlamaProvider : public forgeant::LlmProvider {
public:
  LlamaProvider();
  virtual ~LlamaProvider() override;

  forgeant::LlmResponse chat(
    const forgeant::Conversation& conversation,
    const forgeant::ChatRequest& request = {}) override;
};

}  // namespace myagent
