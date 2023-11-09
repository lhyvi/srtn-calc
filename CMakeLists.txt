cmake_minimum_required(VERSION 3.14)

include(cmake/prelude.cmake)

project(
    srtn-calc
    VERSION 0.1.0
    DESCRIPTION "A SRTN Calculator"
    LANGUAGES CXX
)

include(cmake/project-is-top-level.cmake)
include(cmake/variables.cmake)

# ---- Declare library ----

add_library(
    srtn-calc_lib OBJECT
    source/lib.cpp
)

target_include_directories(
    srtn-calc_lib ${warning_guard}
    PUBLIC
    "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/source>"
)

target_compile_features(srtn-calc_lib PUBLIC cxx_std_20)

find_package(OpenGL REQUIRED)
target_link_libraries(srtn-calc_lib PRIVATE OpenGL::GL)

find_package(imgui CONFIG REQUIRED)
target_link_libraries(srtn-calc_lib PRIVATE imgui::imgui)

find_package(SDL2 CONFIG REQUIRED)
target_link_libraries(srtn-calc_lib
	PRIVATE
	$<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
	$<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
)

# ---- Declare executable ----

add_executable(srtn-calc_exe source/main.cpp source/process.cpp source/srtn.cpp)
add_executable(srtn-calc::exe ALIAS srtn-calc_exe)

set_property(TARGET srtn-calc_exe PROPERTY OUTPUT_NAME srtn-calc)

target_compile_features(srtn-calc_exe PRIVATE cxx_std_20)

target_link_libraries(srtn-calc_exe PRIVATE OpenGL::GL)
target_link_libraries(srtn-calc_exe PRIVATE srtn-calc_lib)
target_link_libraries(srtn-calc_exe PRIVATE imgui::imgui)
target_link_libraries(srtn-calc_exe
	PRIVATE
	$<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
	$<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
)

set_property(TARGET srtn-calc_exe PROPERTY WIN32_EXECUTABLE true)
# ---- Install rules ----

if(NOT CMAKE_SKIP_INSTALL_RULES)
  include(cmake/install-rules.cmake)
endif()

# ---- Developer mode ----

if(NOT srtn-calc_DEVELOPER_MODE)
  return()
elseif(NOT PROJECT_IS_TOP_LEVEL)
  message(
      AUTHOR_WARNING
      "Developer mode is intended for developers of srtn-calc"
  )
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
include(cmake/dev-mode.cmake)
