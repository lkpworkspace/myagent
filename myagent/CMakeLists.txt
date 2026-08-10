cmake_minimum_required(VERSION 3.19)

### config
# configure_file (${CMAKE_CURRENT_SOURCE_DIR}/config.h.in ${CMAKE_CURRENT_SOURCE_DIR}/config.h)

### source
aux_source_directory(. __srcs)

### lib
add_library(${PROJECT_NAME} ${__srcs})
target_include_directories(${PROJECT_NAME}
  PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>
  INTERFACE
  $<INSTALL_INTERFACE:include>
)
target_link_libraries(${PROJECT_NAME}
  PRIVATE
  ${CMAKE_DL_LIBS}
  PUBLIC
  Threads::Threads
  forgeant::forgeant
)

set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(${PROJECT_NAME} PROPERTIES SOVERSION ${PROJECT_VERSION_MAJOR})

# export file
include (GenerateExportHeader)
generate_export_header (${PROJECT_NAME}
  EXPORT_MACRO_NAME MYAGENT_EXPORT
  EXPORT_FILE_NAME "${CMAKE_CURRENT_SOURCE_DIR}/export.h"
)

### install
file(GLOB header_files
#   config.h
#   platform.h
  export.h
#   macros.h
)
install(FILES 
  ${header_files}
  PERMISSIONS
  OWNER_READ OWNER_WRITE
  GROUP_READ
  WORLD_READ
  DESTINATION ${MYAGENT_INC_DIR}/${PROJECT_NAME}
)
install(TARGETS ${PROJECT_NAME}
  EXPORT "${PROJECT_NAME}Targets"
  LIBRARY DESTINATION ${MYAGENT_LIB_DIR}
  ARCHIVE DESTINATION ${MYAGENT_LIB_DIR}
  RUNTIME DESTINATION ${MYAGENT_BIN_DIR}
)

include(InstallingConfigs)
