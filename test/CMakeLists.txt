# Install GoogleTest
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.13.0.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
enable_testing()

# It does compile
add_executable(compile_test compile_test.cc)
target_link_libraries(compile_test gtest_main triforce)
add_test(NAME compile_test COMMAND compile_test)
