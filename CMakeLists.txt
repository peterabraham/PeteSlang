cmake_minimum_required(VERSION 3.10)
project(PeteSlang CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF) # no GNU extensions are actually used; keep the build portable (MSVC has no gnu++ dialect)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

set(PETESLANG_SOURCES
    PeteSlang/AST/Expressions.cpp
    PeteSlang/AST/Statements.cpp
    PeteSlang/Frontend/Builder.cpp
    PeteSlang/Common.cpp
    PeteSlang/Frontend/Lexer.cpp
    PeteSlang/Runtime/MemoryContext.cpp
    PeteSlang/Frontend/Parser.cpp
    PeteSlang/Runtime/SymbolTable.cpp
    PeteSlang/main.cpp
)

add_executable(peteslang ${PETESLANG_SOURCES})

# Headers use project-root-relative includes (e.g. "AST/Expressions.h",
# "Runtime/MemoryContext.h") regardless of which subfolder is doing the
# including, so every subfolder resolves through this single include path.
target_include_directories(peteslang PRIVATE PeteSlang)
