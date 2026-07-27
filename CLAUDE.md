    # CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## What this is

PeteSlang is a toy scripting language interpreter written in C++, a port of "SLANGFOR.net" (originally used to
teach compiler construction). It's a classic hand-rolled recursive-descent interpreter: a combined
lexer/parser builds an AST directly (no separate parse tree), and the AST nodes evaluate/execute themselves
directly (tree-walking interpreter, not bytecode).

Source material: `SLANG_FOR_DOT_NET_STEP7.pdf` (repo root) — "The Art of Compiler Construction using C#" by
Praseed Pai K.T., the original SLANG4.NET writeup (originally hosted at slangfordotnet.codeplex.com). It
walks through the interpreter in incremental "steps"; this repo's design (Expression/Statement AST,
RuntimeContext/CompilationContext split, SymbolTable, safe_delete helpers) mirrors the C# original closely.
The PDF's STEP 7 section — the latest step covered — adds user-defined `FUNCTION`s, `RETURN`, function-call
expressions, and recursion (see its Fibonacci/Factorial examples) on top of everything already ported here;
that step has not yet been ported to PeteSlang.

Currently implemented: arithmetic expressions, PRINT/PRINTLINE statements, typed variable declarations and
assignment, relational/logical operators, and IF/WHILE control flow. Not yet implemented: user-defined
functions.

## Build

CMake (`CMakeLists.txt` at the repo root) is the only build system for this repo — it compiles
`PeteSlang/*.cpp` into a single `peteslang` executable. Targets C++17, no dependencies beyond the standard
library. Builds with GCC, Clang, and MSVC.

```bash
cmake -S . -B build
cmake --build build
```

The binary is produced at `build/peteslang`.

Native IDE projects (Xcode, Visual Studio, ...) can be generated on demand with `cmake -G <generator>` and
are gitignored — don't hand-edit or commit them; edit `CMakeLists.txt` instead. There is no test suite,
linter, or CI config in this repo.

## Running

The interpreter takes a script file path as its only argument:

```bash
build/peteslang Examples/onetohundred.sl
```

Sample scripts covering the supported syntax live in `Examples/*.sl`.

## Architecture

Pipeline: `main.cpp` reads a file into a string → `Parser` (which subclasses `Lexer`) parses it directly into
a `vector<std::unique_ptr<Statement>>` AST → each `Statement` is `execute()`d against a `RuntimeContext`.

Source layout (under `PeteSlang/`), one subfolder per concern rather than flat `AST_*`-prefixed filenames:

```
PeteSlang/
  Common.h/cpp          shared enums, SymbolInfo, safe_delete helpers
  AST/
    Expressions.h/cpp    Expression node hierarchy
    Statements.h/cpp     Statement node hierarchy
  Frontend/
    Lexer.h/cpp          tokenizer
    Parser.h/cpp         recursive-descent parser
    Builder.h/cpp        ExpressionBuilder
  Runtime/
    MemoryContext.h/cpp  RuntimeContext / CompilationContext
    SymbolTable.h/cpp    variable name -> SymbolInfo lookup
  main.cpp
```

Headers use project-root-relative includes (e.g. `#include "AST/Expressions.h"`) regardless of which
subfolder does the including; `CMakeLists.txt` adds `PeteSlang/` as an include directory so this resolves
from anywhere.

- **Frontend/Lexer.h/cpp** — Tokenizer. Holds current/previous token (`myCurrentToken`/`myLastToken`) and the
  last scanned identifier/string (`myLastString`). No separate token stream; `getToken()` is pulled on demand.
- **Frontend/Parser.h/cpp** — Recursive-descent parser, inherits from `Lexer` (parsing and lexing are tightly
  coupled: `getNext()` pulls tokens as parsing proceeds). Builds AST nodes directly rather than an
  intermediate parse tree, returning `std::unique_ptr<Expression>`/`std::unique_ptr<Statement>` that the
  caller (or a parent AST node) takes ownership of. Expression grammar (lowest to highest precedence):
  `bExpression` (`&&`/`||`) → `lExpression` (relational: `== <> > >= < <=`) → `expression` (`+ -`) →
  `term` (`* /`) → `factor` (unary +/-, literals, variables, parenthesized exprs). Statement parsing
  (`getStatement`) dispatches on leading token to `parsePrintStatement`, `parsePrintLineStatement`,
  `parseVariableDeclStatement`, `parseAssignmentStatement`, `parseIfStatement`, `parseWhileStatement`.
- **AST/Expressions.h/cpp** — `Expression` node hierarchy (constants, `Variable`, binary/unary arithmetic,
  `RelationalExpression`, `LogicalExpression`, `LogicalNot`). Every node implements `evaluate()` (runtime
  value), `typeCheck()` (compile-time type inference/validation), and `getType()`. Child `Expression`s are
  owned via `std::unique_ptr`, which makes composite nodes automatically non-copyable (no more hand-written
  private copy ctor/assignment per class).
- **AST/Statements.h/cpp** — `Statement` node hierarchy (`PrintStatement`, `PrintLineStatement`,
  `AssignmentStatement`, `VariableDeclStatement`, `IfStatement`, `WhileStatement`). Each implements
  `execute(RuntimeContext*)`. Child `Expression`/`Statement` values and block vectors
  (`vector<std::unique_ptr<Statement>>`) are owned the same way.
- **Runtime/MemoryContext.h/cpp** — Two parallel context types wrapping a `SymbolTable`: `CompilationContext`
  (used during parsing/type-checking, e.g. to know a variable's declared type) and `RuntimeContext` (used
  during execution, holds actual values). Both expose the same `SymbolTable`-backed interface
  (`getInfo`/`addInfo`/`assignInfo`).
- **Runtime/SymbolTable.h/cpp** — `std::map<std::string, SymbolInfo*>` mapping variable names to `SymbolInfo`.
  These are non-owning observer pointers: the true owner of a variable's `SymbolInfo` is the
  `VariableDeclStatement` AST node that declared it (via `std::unique_ptr<SymbolInfo>`), which outlives both
  the compile-time and runtime tables that reference it.
- **Common.h/cpp** — Shared `enum class`es (`Token`, `Operator`, `RelationalOperator`, `TypeInfo` — scoped, so
  call sites must write e.g. `Token::TOK_PLUS`), the `SymbolInfo` value/type-record struct (holds its value in
  a `std::variant<double, bool, std::string>` instead of three simultaneously-live fields), and generic
  `safe_delete`/`safe_delete_array` helpers still used for the handful of remaining raw owning pointers
  (`SymbolTable*` inside the contexts, the `ValueTable**` keyword table in `Lexer`).
- **Frontend/Builder.h/cpp** — `ExpressionBuilder`, a thin standalone wrapper for evaluating a single expression
  string outside of full script parsing.

### Language notes (from `Examples/*.sl`)

- Types: `NUMERIC`/`Numeric` (IEEE double), `STRING`/`String`, `BOOLEAN`/`Boolean` — declarations and keywords
  are case-insensitive.
- Statements are `;`-terminated. Comments use `//`.
- Control flow: `IF ( cond ) THEN ... [ELSE ...] ENDIF`, `WHILE ( cond ) ... WEND` (no braces, closing
  keywords delimit blocks).
- Output: `PRINT expr;` and `PRINTLINE expr;`.
- Operators: arithmetic `+ - * /`, relational `== <> > >= < <=`, logical `&& || !`. String `+` concatenates.

### Memory management

AST ownership (`Expression`/`Statement` children, statement-block vectors, declared variables' `SymbolInfo`)
is `std::unique_ptr`-based; follow that convention for any new AST/parser code. The `safe_delete`/
`safe_delete_array`/`safe_delete_pointer_array` helpers in `Common.h` still exist and are still used, but only
for the remaining handful of raw owning pointers that weren't part of this pass (`SymbolTable*` inside
`RuntimeContext`/`CompilationContext`, and the `ValueTable**` keyword array in `Lexer`) — don't reach for
`new`/`safe_delete` for anything new; use `unique_ptr` instead.

One intentionally out-of-scope leak: transient `SymbolInfo*` results produced mid-expression-evaluation
(e.g. every arithmetic/relational/logical operator's `evaluate()` does `new SymbolInfo()` for its result) are
still raw, unowned, and never freed — same as before this pass. Constant leaf nodes (`BooleanConstant`,
`NumericConstant`, `StringLiteral`) no longer do this (they hold their one `SymbolInfo` by value), but fixing
lifetime for the transient values flowing up through binary/relational/logical operators needs a real runtime
value model (e.g. once the Visitor-pattern refactor lands) rather than a piecemeal fix.
