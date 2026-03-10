FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest
  GIT_TAG        v1.14.0
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# One test executable with all test files
add_executable(momo_tests
  test_types.cpp
  test_bool.cpp
  test_tokenizer.cpp
  test_reader.cpp
  test_eval.cpp
  test_core.cpp
  test_env.cpp
  test_utils.cpp
  test_logger.cpp
)

target_link_libraries(momo_tests PRIVATE momo_lib GTest::gtest_main)
target_link_options(momo_tests PRIVATE -static)

add_test(NAME momo_tests COMMAND momo_tests)
