#pragma once
#include <forgeant/forgeant.hpp>
#include "export.h"

namespace myagent {

class MYAGENT_EXPORT WebSearchTool {
public:
  WebSearchTool();
  virtual ~WebSearchTool();

  forgeant::Json Run(const forgeant::Json&);
};

}  // namespace myagent
