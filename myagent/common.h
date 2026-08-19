#pragma once
#include <string>
#include "export.h"

namespace myagent {

class MYAGENT_EXPORT Common {
 public:
  static std::string ExecCommand(const std::string& cmd);
};

}  // namespace myagent
