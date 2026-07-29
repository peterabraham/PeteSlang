# PeteSlang — Project Specification and Roadmap

## Purpose

PeteSlang is a small, typed programming language and proof-of-concept compiler written in modern C++.

The project is inspired by *The Art of Compiler Construction using C#* and the SLANG4.NET implementation, but
it is not intended to be a strict port. The ebook provides the educational foundation; PeteSlang is free to
improve its language semantics, architecture, safety, tooling, and compilation model.

The goal is to build a compact but functional compiler that demonstrates lexical analysis and parsing,
abstract syntax tree construction, name resolution and static type checking, interpretation, functions with
local variables and recursion, a backend-independent compiler architecture, C++ source generation, native
executable generation through a host C++ compiler, and — optionally, later — LLVM IR generation as a second
backend.

Clarity and correctness matter more than language size. This is a personal/teaching project, not a product;
the roadmap below is written to be honest about what's committed versus aspirational rather than to maximize
scope.

## Project principles

- Keep the project small enough to understand end to end.
- Prefer explicit, well-owned data over shared raw pointers.
- Keep syntax, semantic analysis, interpretation, and code generation separate.
- Keep the AST independent of any particular backend.
- Treat diagnostics and tests as part of the compiler, not later polish.
- Use the ebook as guidance rather than as a compatibility requirement.
- Make intentional language decisions instead of preserving accidental legacy behavior.
- Add language features only when they can be interpreted, validated, and tested consistently.
- Avoid unnecessary frameworks and excessive architectural abstraction.
- Prefer stopping at a phase boundary over carrying unfinished work into the next one.
- Never hard-code a single toolchain where a configurable one costs little extra.

## Proof-of-concept scope

The core language will support `NUMERIC`, `BOOLEAN`, and `STRING` values; variable declarations and
assignment; arithmetic expressions; string concatenation; relational and logical expressions; `PRINT` and
`PRINTLINE`; `IF`/`ELSE`/`WHILE`; typed functions with typed parameters; function-call expressions; `RETURN`;
local variables; recursive and mutually recursive functions; and a defined program entry point.

The first complete compilation backend will generate portable C++17 source, compiled by a configurable host
C++ compiler. LLVM is a later, optional extension and is not required to establish the initial compiler POC.

## Intended compiler pipeline

```text
Source file → Lexer → Parser → AST → Semantic analysis (name resolution, type checking,
function validation) → Backend (Interpreter | C++ generator | LLVM generator, later)
```

The same analyzed AST should be usable by every backend.

## Language direction

PeteSlang does not need to reproduce every behavior from the ebook. The following must be decided explicitly
as the implementation evolves, and documented via tests once decided: operator precedence/associativity,
identifier case sensitivity, duplicate declaration behavior, variable initialization behavior, function
entry-point rules, required-return behavior, short-circuit logical operators, division-by-zero behavior,
string storage/ownership, and supported implicit/explicit conversions.

## Known current defects (verified against the code, 2026-07-29)

These are confirmed, not guessed, and are the reason "stabilize the interpreter" comes before any refactor:

1. `IfStatement::execute` (`AST/Statements.cpp`) ends with `myStatements.clear(); myStatements.clear();` —
   the if-branch's statement list is destroyed after one execution (this likely should not clear *anything*,
   since statement bodies must survive repeated execution once loops-inside-functions or recursion exist).
2. `WhileStatement::execute` has the same self-destruction pattern on its own loop body.
3. Both `IfStatement::execute` and `WhileStatement::execute` contain
   `nullptr == pRetSymbol && TYPE_BOOL != pRetSymbol->myType`, which dereferences `pRetSymbol->myType` even
   when `pRetSymbol` is null. Needs `||` or an early return before the type check.
4. `SymbolTable::get` dereferences `map::find()`'s result without checking against `end()` — looking up an
   undeclared or misspelled variable name is undefined behavior, not a diagnostic.
5. `SymbolTable::add` uses `map::insert`, which silently no-ops on a duplicate key — redeclaring a variable
   is accepted without error and quietly keeps the *first* declaration (including its type), which can cause
   confusing downstream type mismatches.
6. `ExpressionBuilder::getExpression()` (`Frontend/Builder.cpp`) always returns `nullptr` — the actual parse
   call (`parser->callExpression(...)`) is commented out. The standalone single-expression helper described
   in CLAUDE.md is currently non-functional; only the full script pipeline (`Parser::parse`) works.

## Testing strategy

GoogleTest (pinned release, via CMake `FetchContent`) is the test framework; CTest is the runner, wired
through the normal build:

```bash
cmake -S . -B build -DBUILD_TESTING=ON
cmake --build build
ctest --test-dir build --output-on-failure
```

`FetchContent` means a fresh build needs network access the first time GoogleTest isn't already cached
locally. If fully offline builds become a real requirement later, add a `PETESLANG_FETCH_GTEST` option that
tries `find_package(GTest CONFIG QUIET)` first and only fetches when nothing is installed — not needed on
day one.

Coverage to build toward: lexer unit tests, parser/AST construction tests, semantic-analysis tests,
interpreter tests, diagnostic/failure tests, end-to-end script tests, and — once those backends exist —
C++ and LLVM conformance tests comparing output against the interpreter. `Examples/*.sl` becomes the first
batch of end-to-end regression cases with recorded expected output. Malformed-program inputs (truncated
operators, unterminated strings, missing delimiters, invalid types, duplicate declarations, unresolved
names, invalid calls, missing returns) matter as much as valid ones. AddressSanitizer/UBSan builds are worth
adding once the interpreter is stabilized, so the stabilization work itself gets sanitizer coverage.

## Roadmap

### Now — next couple of sessions

- Wire up GoogleTest (FetchContent, pinned version) + CTest; a `peteslang_tests` executable separate from
  the `peteslang` CLI target so tests don't fight over `main()`. *(Scaffolding for this landed alongside this
  spec — see `Tests/`. Only a smoke test exists so far; the rest of this list is still open.)*
- Convert `Examples/*.sl` into golden-output regression tests *before* changing any interpreter code, so the
  fixes below have a safety net and a way to prove they didn't change unrelated behavior.
- Fix the six defects listed above, each with a regression test written first.
- Add file-open validation and real process exit codes in `main.cpp` (currently a bad `argc` check exits via
  a message helper, and `main` returns `1` unconditionally on success — conventional would be `0`).

### Next — foundational, medium effort

- Introduce an owned runtime `Value` type; stop leaking a `new SymbolInfo()` on every expression evaluation.
- Separate declared symbol metadata (`VariableSymbol`) from mutable runtime values held in an
  `Environment`/`CallFrame`, so environments — not AST nodes — own variable storage.
- Visitor-pattern refactor (`accept(Visitor&)` dispatch) — **before** functions, not after, so functions and
  any future backend land on the final architecture instead of getting redone on top of it.
- Token objects with line/column tracking; structured diagnostics instead of exiting from low-level code.

### Later — the actual next language feature

- STEP 7 from the reference PDF: `FUNCTION`/`RETURN`, parameters, call expressions, forward-call resolution,
  self- and mutual recursion, per-invocation call frames. The source material already spells this out in
  detail, so once "Next" lands this is the lowest-risk feature addition on the list.
- Factorial/Fibonacci as conformance tests, plus invalid-call/missing-return diagnostic tests.

### Someday — optional stretch, revisit only after "Later" ships and it's still fun

**C++ backend.** `g++` is a reasonable initial default compiler, but the backend must not hard-code it as the
only option. Suggested compiler selection order:

1. User-provided compiler via `--cxx`.
2. CMake-configured compiler (`CMAKE_CXX_COMPILER`) or the `CXX` environment variable.
3. Platform default, tried in order: `g++`, `clang++`, MSVC `cl`.

Tasks:
- Add a `CppCodegen` visitor.
- Generate portable C++17 source.
- Generate safe, deterministic C++ identifiers.
- Map PeteSlang types to `double`, `bool`, and `std::string`.
- Emit function declarations before definitions.
- Generate expressions, assignments, branches, loops, calls, and returns.
- Generate output through `std::cout`.
- Correctly escape generated string literals.
- Add a small runtime support layer only where semantics differ from C++.
- Add `emit-cpp` to produce a `.cpp` file.
- Add `build` to invoke a configurable host C++ compiler, using the selection order above.
- Initially support `g++`, while keeping room for `clang++` and MSVC.
- Preserve generated C++ when requested.
- Capture and report compiler failures.
- Compare interpreter and compiled output through the same conformance suite.

POC completion for this phase becomes: `.sl → AST → C++17 → g++/clang++/MSVC → native executable`.

- A coherent CLI (`peteslang run/check/dump-ast/emit-cpp/build`).
- LLVM backend (JIT, then ahead-of-time, only after the C++ backend path is stable).
- CI across host compilers, sanitizers in CI, fuzz testing, static analysis, source-excerpt diagnostics.

## Initial POC completion criteria

The primary POC is complete when a source program can be lexed, parsed, and semantically validated; the
language supports typed functions and recursion; the interpreter executes the full supported language; the
C++ backend compiles the same language to a native executable via a configurable host C++ compiler
(`g++` initially, with room for `clang++`/MSVC); interpreter and compiled output agree; invalid programs fail
with useful diagnostics; and the core suite passes under CTest (including sanitizer runs) without runtime
defects. LLVM is not required for primary POC completion.

## Out of scope for the initial POC

Classes/OOP features, garbage collection, user-defined aggregate types, arrays/containers, package/module
imports, closures, optimizing passes, debug information, a linker implementation, self-hosting, and full
SLANG4.NET compatibility.
