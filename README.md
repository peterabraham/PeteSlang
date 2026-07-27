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
PeteSlang past "interpreter" into a real compiler with LLVM and GCC-based code generation backends — see
[Roadmap](#roadmap) below.

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
SLANG_FOR_DOT_NET_STEP7.pdf   source material / reference spec
```

AST ownership is `std::unique_ptr`-based throughout (child expressions/statements, statement-block vectors,
declared variables' `SymbolInfo`), and the core enums (`Token`, `Operator`, `RelationalOperator`, `TypeInfo`)
are scoped `enum class`es. See `CLAUDE.md` for the full architectural writeup.

## Roadmap

PeteSlang is being developed in phases, moving from "toy interpreter" toward a real multi-backend compiler:

1. **Modernize the base** *(done)* — `unique_ptr`-owned AST, `enum class` enums, `std::variant`-backed
   `SymbolInfo`, and the current folder-per-concern layout (`AST/`, `Frontend/`, `Runtime/`).
2. **Visitor pattern refactor** — replace the current per-node `evaluate()`/`typeCheck()` virtuals with a proper
   `accept(Visitor&)` dispatch, so each future backend (interpreter, LLVM, C transpiler) is a self-contained
   `Visitor` implementation instead of another method bolted onto every AST node.
3. **STEP 7 — user-defined functions** — `FUNCTION`/`RETURN`, function-call expressions, a module-of-functions
   program shape, and real call frames so recursion (Fibonacci, factorial) works.
4. **LLVM backend** — an `LLVMCodegen` visitor that emits LLVM IR per AST node, with both a JIT mode and an
   ahead-of-time mode (`.ll` → `llc` → native executable).
5. **C-transpile / GCC backend** — a `CTranspiler` visitor that emits plain C source from the AST and hands it
   to `gcc` to compile — a second, independent code-generation path alongside LLVM.
6. **Polish** — a unified CLI (`--interpret` / `--emit-llvm` / `--emit-c`), a test harness running every
   `Examples/*.sl` script through all three backends, and updated docs.
