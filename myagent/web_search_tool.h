#pragma once
#include <forgeant/forgeant.hpp>
#include "export.h"

namespace myagent {

class MYAGENT_EXPORT WebSearchTool : public forgeant::Tool {
 public:
  WebSearchTool();
  ~WebSearchTool();

  forgeant::Json Schema();
  forgeant::Json Execute(const forgeant::Json&);

 private:
  forgeant::Json SearchTavily(const std::string& query, int max_results);
};

}  // namespace myagent
