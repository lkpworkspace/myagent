#include <iostream>
#include "myagent/common.h"

namespace myagent {

std::string Common::ExecCommand(const std::string& cmd) {
  std::array<char, 128> buffer;
  std::string result;
  std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd.c_str(), "r"), pclose);
  if (!pipe) {
    throw std::runtime_error("popen() failed!");
  }
  while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
    result += buffer.data();
  }
  std::cout << "[myagent] exec command \"" << cmd << "\",result: "
    << result << std::endl;
  if (!result.empty() && result.back() == '\n') {
    result.pop_back();
  }
  return result;
}

}  // namespace myagent
