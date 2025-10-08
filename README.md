# boundless
A C bound and memory safety checker independent of platform.

## Project Plan

### Initial Scope
- Language: a basic C subset (functions, locals/globals, ints, pointers, arrays, structs, field access, assignment, if/while/return, calls). Skip unions/varargs for now.
- Preprocessor: either assume preprocessed input (ideal early on) or token-pass `#...` lines without evaluation.
- Goals first: OOB detection and memory leaks; also catch double free and UAF naturally with the same metadata.

### Tokenizer Direction
- Keep it simple: identifiers, numbers, strings, chars, keywords, operators/punct, comments.
- Example mapping: `obj.field = obj2;` → `TOKEN_IDENT TOKEN_DOT TOKEN_IDENT TOKEN_EQUAL TOKEN_IDENT`.
- Required multi-char operators: `== != <= >= ++ -- += -= *= /= %= && || << >> ->`.
- Skip whitespace/comments but track line (and ideally column).
- Produce an explicit `TOKEN_EOF`.
- Post-classify identifiers into keywords from a small table (`int`, `return`, `if`, `else`, `while`, `struct`, `sizeof`, `void`, `char`).

- Token examples (C subset):
  - Identifiers: `TOKEN_IDENT`
  - Literals: `TOKEN_INT`, `TOKEN_CHAR`, `TOKEN_STRING` (floats later)
  - Operators/punct: `TOKEN_DOT .`, `TOKEN_ARROW ->`, `TOKEN_EQUAL =`, `TOKEN_EQ ==`, `TOKEN_PLUS +`, `TOKEN_LPAREN (`, `TOKEN_RPAREN )`, `TOKEN_LBRACE {`, `TOKEN_RBRACE }`, `TOKEN_COMMA ,`, `TOKEN_SEMI ;`, `TOKEN_LBRACK [`, `TOKEN_RBRACK ]`, etc.
  - Preprocessor (optional early): treat as raw lines or a minimal token pair (e.g., `TOKEN_HASH`, `TOKEN_PP_IDENT`).

### IR + VM (Memory Model)
- Objects: Every allocation (heap, static, stack frame objects, arrays, structs) gets an object ID with metadata: size, base address (abstract), type info, lifetime state (live/freed), allocation site (for leaks).
- Pointers: Represent as `(object_id, offset)`. Permit one-past-end pointers but trap on deref/store out of bounds. Support pointer arithmetic; preserve C rules.
- Loads/stores: Check that object is live, and that `offset + width` is within bounds. On `free`, mark object freed; subsequent use traps. Double free traps.
- Struct/array layout: Match C ABI rules for alignment/padding. `.` and `->` resolve to compile-time offsets (struct field table).
- Type policy: For early progress, enforce “compatible type” checks at load/store; allow configurable leniency for C-style type punning later (e.g., via `char*`).
- Stack: Create VM objects for locals/arrays; mark them freed at function return. Dangling use after return will trap.
- Globals: VM objects that are freed at program end.

### Compile-Time Checks (Early)
- Obvious leak patterns: within a function, if all control paths after `p = malloc(...)` do not call `free(p)` and `p` doesn’t escape, report a leak. Start path-insensitive, intraprocedural; escalate to runtime reporting for complex cases.
- Double free detection: dataflow to warn when you see a `free` on a variable already “freed” on all paths.
- Null checks: flag deref of a value that’s obviously null.
- Escape analysis (basic): if a pointer is returned/stored in a global/arg/heap, skip compile-time leak claim and rely on VM to report at exit.

### Parsing Strategy (Minimal)
- Start with declarations, assignments, expressions (field access `.` and `->`, array indexing, calls), compound statements, if/while/return, function definitions.
- Operator precedence is the usual C ladder; build just what you need for assignments and field/array access first.
- Use a simple recursive-descent for the subset; defer tricky parts (e.g., function pointer declarators) until later.

### Milestones
- M1: Tokenizer complete for the subset, with newline tracking and multi-char operators; smoke test by printing token streams.
- M2: Minimal parser for expressions/statements and function defs; build simple typed AST.
- M3: IR design (three-address or SSA-ish): ops for alloc/free, gep (field/element offset), load/store, arithmetic, call/ret, branch.
- M4: VM with object table, pointer `(obj, off)`, bounds/lifetime checks on load/store/free; run small programs; detect OOB, UAF, double free.
- M5: Intraprocedural leak checker; VM reports remaining live heap objects at program end.
