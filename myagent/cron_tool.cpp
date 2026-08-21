#include <iostream>
#include "myframe/common.h"
#include "myagent/common.h"
#include "myagent/cron_tool.h"

namespace myagent {

const static std::string CRON_DESC = R"(
  Schedule reminders and recurring tasks. Actions: add, list, update, remove.
)";

CronTool::CronTool()
  : forgeant::Tool(
    R"(cron)",
    CRON_DESC,
    Schema(),
    std::bind(&CronTool::Execute, this, std::placeholders::_1))
{}

CronTool::~CronTool() {}

forgeant::Json CronTool::Schema() {
  return forgeant::Schema::object()
    .property("action", forgeant::Schema::string()
      .enum_values({"add", "list", "update", "remove"})
      .description("Action to perform")
      .build())
    .property("message", forgeant::Schema::string()
      .description("Reminder message (for add/update)")
      .build())
    .property("name", forgeant::Schema::string()
      .description("Job display name (for add/update)")
      .build())
    .property("every_seconds", forgeant::Schema::integer()
      .description("Interval in seconds (for recurring tasks)")
      .build())
    .property("cron_expr", forgeant::Schema::string()
      .description("Cron expression like '0 9 * * *'")
      .build())
    .property("tz", forgeant::Schema::string()
      .description("IANA timezone for cron expressions")
      .build())
    .property("at", forgeant::Schema::string()
      .description("ISO datetime for one-time execution")
      .build())
    .property("job_id", forgeant::Schema::string()
      .description("Job ID (for update/remove)")
      .build())
    .required({"action"})
    .build();
}

forgeant::Json CronTool::Execute(const forgeant::Json& input) {
  std::cout << "[myagent.TOOL] cron recv:" << input.dump() << std::endl;
  forgeant::Json response;
  // TODO: execute cron
  response["error"] = "OK";

  std::cout << "[myagent.TOOL] cron result: " << response.dump() << std::endl;
  return response;
}

}  // namespace myagent
