cmake_minimum_required(VERSION 3.14)
project(momo)

include(FetchContent)

FetchContent_Declare(
  plog
  GIT_REPOSITORY https://github.com/SergiusTheBest/plog
  GIT_TAG        1.1.8
)
FetchContent_MakeAvailable(plog)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/out)
set(SOURCE_DIR ${CMAKE_SOURCE_DIR}/src)

add_subdirectory(${SOURCE_DIR})

# Static library from all source except main
add_library(momo_lib STATIC ${LIB_SOURCES})
target_include_directories(momo_lib PUBLIC ${SOURCE_DIR})
target_link_libraries(momo_lib PUBLIC plog)

# Main executable
add_executable(${PROJECT_NAME} ${SOURCE_DIR}/momo.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE momo_lib)

# Tests
enable_testing()
add_subdirectory(tests)
