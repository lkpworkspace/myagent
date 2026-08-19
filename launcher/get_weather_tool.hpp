#pragma once
#include <forgeant/forgeant.hpp>

struct WeatherParams {
  std::string city;
  std::string unit = "fahrenheit";
};

template <>
struct forgeant::ParamSchema<WeatherParams> {
  static forgeant::Json schema() {
    return forgeant::Schema::object()
      .property("city", forgeant::Schema::string()
        .description("The city name").build())
      .property("unit", forgeant::Schema::string()
        .description("Temperature unit")
        .enum_values({"fahrenheit", "celsius"})
        .default_value("fahrenheit")
        .build())
      .required({"city"})
      .build();
  }
};

void from_json(const forgeant::Json& j, WeatherParams& params) {
  j.at("city").get_to(params.city);
  if (j.contains("unit")) {
    j.at("unit").get_to(params.unit);
  }
}

void add_get_weather_tool(std::unique_ptr<forgeant::Agent>& agent) {
  agent->add_tool(
    forgeant::make_tool<WeatherParams>(
      "get_weather",
      "Get the current weather for a city",
      [](WeatherParams params) -> std::string {
          return "72°F and sunny in " + params.city;
      }));
}
