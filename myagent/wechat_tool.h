#pragma once
#include <forgeant/forgeant.hpp>
#include "export.h"

namespace myagent {

class MYAGENT_EXPORT WeChatTool : public forgeant::Tool {
public:
  WeChatTool();
  ~WeChatTool();

  forgeant::Json Schema();
  forgeant::Json Execute(const forgeant::Json&);
};

}  // namespace myagent
