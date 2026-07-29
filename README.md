# PeteSlang

PeteSlang is a C++ port of **SLANGFOR.net** ("SLANG" — Simple LANGuage), a toy scripting language originally
written in C# to teach the fundamentals of compiler construction. The reference material is
[`SLANG_FOR_DOT_NET_STEP7.pdf`](./SLANG_FOR_DOT_NET_STEP7.pdf) — *"The Art of Compiler Construction using C#"*
by Praseed Pai K.T. (originally hosted at slangfordotnet.codeplex.com) — which builds up the interpreter
incrementally in a series of numbered "steps." PeteSlang mirrors that design closely (an `Expression`/`Statement`
AST, a `RuntimeContext`/`CompilationContext` split, a `SymbolTable`) while modernizing the implementation to
current C++.

Architecturally it's a classic hand-rolled recursive-descent interpreter: a combined lexer/parser reads the
source text and builds an AST directly (there's no separate parse tree), and the AST nodes evaluate/execute
themselves via virtual dispatch (a tree-walking interpreter, not a bytecode VM). The long-term goal is to grow
PeteSlang past "interpreter" into a real compiler with a C++ source-generation backend (compiled through a
configurable host compiler) and, later, LLVM — see [Roadmap](#roadmap) below.

The ebook is treated as inspiration rather than a strict compatibility target: PeteSlang is free to make its
own language and architecture decisions where they diverge. See [`SPEC.md`](./SPEC.md) for the full
specification, roadmap, and a running log of known defects.

## Language at a glance

```
FUNCTION-free, statement-oriented, case-insensitive keywords, semicolon-terminated statements.

NUMERIC d;
d = 0;
WHILE ( d <= 100 )
    PRINTLINE d;
    d = d + 1;
WEND
```

- **Types**: `NUMERIC` (IEEE double), `STRING`, `BOOLEAN`.
- **Control flow**: `IF ( cond ) THEN ... [ELSE ...] ENDIF`, `WHILE ( cond ) ... WEND` — no braces, closing
  keywords delimit blocks.
- **Output**: `PRINT expr;` and `PRINTLINE expr;`.
- **Operators**: arithmetic `+ - * /`, relational `== <> > >= < <=`, logical `&& || !`. String `+` concatenates.
- **Comments**: `//` to end of line.

More sample scripts covering the full supported syntax live in [`Examples/*.sl`](./Examples).

## Currently implemented

Arithmetic expressions, `PRINT`/`PRINTLINE` statements, typed variable declarations and assignment,
relational/logical operators, and `IF`/`WHILE` control flow. User-defined functions and recursion are not yet
implemented (see Roadmap).

## Build

CMake is the only build system for this repo — no dependencies beyond the C++17 standard library. Builds with
GCC, Clang, and MSVC.

```bash
cmake -S . -B build
cmake --build build
```

The binary is produced at `build/peteslang`.

## Running

The interpreter takes a script file path as its only argument:

```bash
build/peteslang Examples/onetohundred.sl
```

Swap in any other file under `Examples/` (or your own `.sl` script) as the argument.

## Testing

GoogleTest + CTest, wired through the normal CMake build:

```bash
cmake -S . -B build -DBUILD_TESTING=ON
cmake --build build
ctest --test-dir build --output-on-failure
```

The first configure needs internet access — `Tests/CMakeLists.txt` fetches GoogleTest from GitHub via CMake's
`FetchContent`. It's cached under `build/_deps` afterward, so later builds don't re-fetch it.

If you have a `build/` directory from before the CMake restructuring (single `peteslang` target with no
`peteslang_core`/`Tests` split), delete it first — `rm -rf build` — so CMake isn't working from a stale cache
of the old layout.

Only a smoke test exists so far, proving the harness itself works end to end. Real lexer/parser/interpreter
and `Examples/*.sl` regression suites are tracked in [`SPEC.md`](./SPEC.md)'s roadmap.

## Project structure

```
PeteSlang/
  Common.h/cpp          shared enums, SymbolInfo, safe_delete helpers
  AST/
    Expressions.h/cpp    Expression node hierarchy (constants, operators, Variable)
    Statements.h/cpp     Statement node hierarchy (PRINT, IF, WHILE, assignment, decl)
  Frontend/
    Lexer.h/cpp          tokenizer
    Parser.h/cpp         recursive-descent parser
    Builder.h/cpp        ExpressionBuilder (single-expression helper)
  Runtime/
    MemoryContext.h/cpp  RuntimeContext / CompilationContext
    SymbolTable.h/cpp    variable name -> SymbolInfo lookup
  main.cpp
Examples/                sample .sl scripts
Tests/                   GoogleTest suite (CMake FetchContent), run via CTest
SLANG_FOR_DOT_NET_STEP7.pdf   source material / reference spec
SPEC.md                  full specification, roadmap, and known-defects log
```

AST ownership is `std::unique_ptr`-based throughout (child expressions/statements, statement-block vectors,
declared variables' `SymbolInfo`), and the core enums (`Token`, `Operator`, `RelationalOperator`, `TypeInfo`)
are scoped `enum class`es. See `CLAUDE.md` for the full architectural writeup.

## Roadmap

The detailed roadmap, task list, and known-defects log live in [`SPEC.md`](./SPEC.md), which is the project's
source of truth for scope and sequencing. In short: stabilize and test the current interpreter, refactor to a
visitor-based AST, add user-defined functions (the ebook's STEP 7), then — as an optional stretch goal — a
C++ code-generation backend (compiled via a configurable host compiler) and, further out, LLVM.

## License

MIT — see [`LICENSE`](./LICENSE).
