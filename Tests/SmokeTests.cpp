//
//  SmokeTests.cpp
//  PeteSlang test scaffolding
//
//  Confirms the GoogleTest + CTest wiring actually works, including
//  linking against peteslang_core. Real lexer/parser/interpreter and
//  end-to-end test suites are added incrementally per SPEC.md's "Now"
//  roadmap phase -- this file intentionally stays minimal until then.
//

#include <gtest/gtest.h>

#include "Frontend/Parser.h"
#include "Runtime/MemoryContext.h"

#include <memory>
#include <sstream>

TEST(Smoke, TestHarnessIsWired) {
    EXPECT_EQ(2 + 2, 4);
}

TEST(Smoke, CanRunAScriptThroughPeteslangCore) {
    // Exercises the same Parser -> Statement::execute pipeline main.cpp
    // uses (see testFileScript there), to prove peteslang_tests actually
    // links against peteslang_core and not just the standard library.
    const std::string source = "PRINT 2 + 2;";

    auto parser = std::make_unique<Parser>(source);
    auto compileCtx = std::make_unique<CompilationContext>();
    auto statements = parser->parse(compileCtx.get());

    auto runtimeCtx = std::make_unique<RuntimeContext>();

    std::ostringstream captured;
    std::streambuf* oldBuf = std::cout.rdbuf(captured.rdbuf());
    for (auto& statement : statements) {
        statement->execute(runtimeCtx.get());
    }
    std::cout.rdbuf(oldBuf);

    EXPECT_EQ(captured.str(), "4");
}
