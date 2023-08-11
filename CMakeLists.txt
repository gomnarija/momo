cmake_minimum_required(VERSION 3.11) # FetchContent is available in 3.11+
project(momo)

include(FetchContent)

FetchContent_Declare(
  plog
  GIT_REPOSITORY https://github.com/SergiusTheBest/plog
  GIT_TAG        1.1.8
)
FetchContent_MakeAvailable(plog)




set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/out)
set(SOURCE_DIR ${CMAKE_SOURCE_DIR}/src)

add_subdirectory(${SOURCE_DIR})



set (CMAKE_CXX_FLAGS "-fsanitize=address")
set (CMAKE_CXX_FLAGS "-static-libasan")






add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_include_directories(${PROJECT_NAME} PRIVATE ${SOURCE_DIR})
target_link_libraries(momo PRIVATE plog)