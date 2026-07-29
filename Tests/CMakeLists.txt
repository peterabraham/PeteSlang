include(FetchContent)

FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.17.0.tar.gz
)

# Prevent GoogleTest from overriding the parent project's runtime
# library selection when building with MSVC.
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

# Start small: only a smoke test exists today, proving the GoogleTest +
# CTest wiring works end to end and actually links against
# peteslang_core. LexerTests.cpp / ParserTests.cpp / InterpreterTests.cpp /
# EndToEndTests.cpp get added incrementally per SPEC.md's "Now" roadmap
# phase, rather than declared here ahead of having real content.
add_executable(
    peteslang_tests
    SmokeTests.cpp
)

target_link_libraries(
    peteslang_tests
    PRIVATE
        peteslang_core
        GTest::gtest_main
)

include(GoogleTest)

gtest_discover_tests(
    peteslang_tests
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)
