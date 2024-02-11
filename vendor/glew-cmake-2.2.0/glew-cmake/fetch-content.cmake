cmake_minimum_required(VERSION 3.4)
project(fetch_content)

include(FetchContent)
FetchContent_Declare(
  glew
  GIT_REPOSITORY https://github.com/Perlmint/glew-cmake.git
  GIT_TAG        origin/master
)
FetchContent_GetProperties(glew)
if(NOT glew_POPULATED)
  FetchContent_Populate(glew)

  add_subdirectory(${glew_SOURCE_DIR} ${glew_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

add_executable(glewinfo glewinfo.c)
target_link_libraries(glewinfo PRIVATE libglew_static)
