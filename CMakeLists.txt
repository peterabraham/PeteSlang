cmake_minimum_required(VERSION 3.20)
project(PeteSlang CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF) # no GNU extensions are actually used; keep the build portable (MSVC has no gnu++ dialect)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

# Production code lives in peteslang_core so Tests/ can link against it
# directly, without pulling in main.cpp and fighting over a second main().
set(PETESLANG_CORE_SOURCES
    PeteSlang/AST/Expressions.cpp
    PeteSlang/AST/Statements.cpp
    PeteSlang/Frontend/Builder.cpp
    PeteSlang/Common.cpp
    PeteSlang/Frontend/Lexer.cpp
    PeteSlang/Runtime/MemoryContext.cpp
    PeteSlang/Frontend/Parser.cpp
    PeteSlang/Runtime/SymbolTable.cpp
)

add_library(peteslang_core ${PETESLANG_CORE_SOURCES})

# Headers use project-root-relative includes (e.g. "AST/Expressions.h",
# "Runtime/MemoryContext.h") regardless of which subfolder is doing the
# including, so every subfolder resolves through this single include path.
# PUBLIC so peteslang and peteslang_tests inherit it too.
target_include_directories(peteslang_core PUBLIC PeteSlang)

add_executable(peteslang PeteSlang/main.cpp)
target_link_libraries(peteslang PRIVATE peteslang_core)

include(CTest)
if(BUILD_TESTING)
    add_subdirectory(Tests)
endif()
