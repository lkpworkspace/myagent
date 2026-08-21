#pragma once
#include <forgeant/forgeant.hpp>
#include "export.h"

namespace myagent {

class MYAGENT_EXPORT CronTool : public forgeant::Tool {
public:
  CronTool();
  ~CronTool();

  forgeant::Json Schema();
  forgeant::Json Execute(const forgeant::Json&);
};

}  // namespace myagent
