#pragma once
#include <forgeant/forgeant.hpp>
#include "export.h"

namespace myagent {

class MYAGENT_EXPORT TerminalTool : public forgeant::Tool {
public:
  TerminalTool();
  ~TerminalTool();

  forgeant::Json Schema();
  forgeant::Json Execute(const forgeant::Json&);
};

}  // namespace myagent
