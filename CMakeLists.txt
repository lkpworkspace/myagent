cmake_minimum_required(VERSION 3.19)
# Allow the user to specify the MSVC runtime
if (POLICY CMP0091) # >= 3.15
  cmake_policy(SET CMP0091 NEW)
endif()
project(myagent
  VERSION 1.0.0
  LANGUAGES CXX
)

### option

### cmake module
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

### compile option
set(CMAKE_C_VISIBILITY_PRESET hidden)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
if (CMAKE_CXX_STANDARD_REQUIRED)
  message(STATUS "Set cxx standard ${CMAKE_CXX_STANDARD}")
else()
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
  set(CMAKE_CXX_STANDARD 17)
  message(STATUS "Set default cxx standard 17")
endif()
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/source-charset:utf-8>")
if (MSVC)
  add_definitions(
		-wd4251
  )
endif (MSVC)

### install path
if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  message(STATUS "Set default install prefix ${CMAKE_INSTALL_PREFIX}")
else()
  message(STATUS "Set install prefix ${CMAKE_INSTALL_PREFIX}")
endif()
set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
set(MYAGENT_BIN_DIR "bin")
set(MYAGENT_INC_DIR "include")
set(MYAGENT_LIB_DIR "lib")
set(MYAGENT_LOG_DIR "log")
set(MYAGENT_SERVICE_DIR "service")
set(MYAGENT_CONF_DIR "conf")
set(MYAGENT_PYTHON_DIR "lib/python")

### deps libs
find_package(Threads REQUIRED)
find_package(forgeant REQUIRED)
find_package(myframe REQUIRED)

### sub directory
add_subdirectory(myagent)
add_subdirectory(launcher)
add_subdirectory(test)

### install file/dir
install(FILES
  "LICENSE"
  PERMISSIONS
  OWNER_READ OWNER_WRITE
  GROUP_READ
  WORLD_READ
  DESTINATION .
)
install(DIRECTORY resources DESTINATION .)

### package
include(Packing)
