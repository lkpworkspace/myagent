CMAKE_MINIMUM_REQUIRED(VERSION 3.19)

### configure file
# configure_file(${CMAKE_CURRENT_SOURCE_DIR}/launcher_config.h.in ${CMAKE_CURRENT_SOURCE_DIR}/launcher_config.h @ONLY)

### source
set(AGENT_LAUNCHER "myagent_launcher")
# get_filename_component(DIR_NAME ${CMAKE_CURRENT_LIST_DIR} NAME_WE)
aux_source_directory(. __srcs)

### bin
add_executable(${AGENT_LAUNCHER} ${__srcs})
target_link_libraries(${AGENT_LAUNCHER} PRIVATE ${PROJECT_NAME})

### install
install(TARGETS ${AGENT_LAUNCHER}
  LIBRARY DESTINATION ${MYAGENT_LIB_DIR}
  ARCHIVE DESTINATION ${MYAGENT_LIB_DIR}
  RUNTIME DESTINATION ${MYAGENT_BIN_DIR}
)
