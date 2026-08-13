#pragma once
#include <forgeant/forgeant.hpp>
#include "export.h"

namespace myagent {

class MYAGENT_EXPORT WeChatTool {
public:
  WeChatTool();
  virtual ~WeChatTool();

  forgeant::Json Run(const forgeant::Json&);
};

}  // namespace myagent
