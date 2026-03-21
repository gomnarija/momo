# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

**momo** is a custom Lisp-like interpreter written in C++17. It uses square-bracket prefix notation (`[operator arg1 arg2 ...]`) and is intended to eventually support an ECS (Entity Component System) style of functional programming, likely for interactive fiction (see `svasta.txt` for design sketches).

## Build

Requires CMake 3.11+ and a C++17 compiler. The `plog` logging library is fetched automatically via CMake's `FetchContent`.

```bash
# Configure (from repo root)
cmake -S . -B build

# Build — output goes to out/
cmake --build build

# Run
./out/momo.exe
```

The executable reads `staznam.momo` from the working directory (hardcoded in `main()`).

## Architecture

The pipeline is: **source text → Tokenizer → Reader → AST (`moVal` tree) → Eval → result**

### `src/tokenizer.h/.cpp`
Streaming tokenizer. Maintains a `TokenizerState` enum (`WHITESPACE`, `SYMBOL`, `NUMBER`, `STRING`, `L_PARAN`, `R_PARAN`, `SEMICOLON`, `END`). Call `next()` to advance, `curr()` to read current token, `getState()` for its type.

### `src/reader.h/.cpp`
Converts token stream into `moVal` AST nodes. `readString()` parses a single expression from a string. `readFile()` reads a `.momo` file line-by-line and returns a `moList` of all top-level expressions. Lists can span multiple lines — `read_list()` calls `getline()` to continue when it hits `END` mid-list.

### `src/momo.h` — Core types
All values inherit from `moVal` and are held via `moValPtr` (`shared_ptr<moVal>`):

| Type | C++ struct | Notes |
|---|---|---|
| `MO_NIL` | `_moNil` | Singleton `NIL`; `print()` returns `"Ništa"` |
| `MO_NUMBER` | `moNumber` | Stores `double` |
| `MO_STRING` | `moString` | Stores `std::string`, no surrounding quotes |
| `MO_SYMBOL` | `moSymbol` | Identifier; looked up in `moEnv` at eval time |
| `MO_LIST` | `moList` | `vector<moValPtr>`; used for both code and data |
| `MO_FUNCTION` | `moFunction` | Name + args list + body list |

`moEnv` is a lexically-scoped linked list of `map<string, moValPtr>`. `getVal` returns `NIL` for missing keys; `insertVal` only adds (won't overwrite); `setVal` only updates existing keys.

### `src/eval.cpp`
Two dispatch maps:
- `operators` — args are evaluated before dispatch (e.g. `+`, `-`)
- `ne_operators` — args are passed unevaluated (intended for `if`, `set`, `function`, etc. — all currently commented out)

`eval()` dispatches on type: atoms self-evaluate except `MO_SYMBOL` (looked up via `env_search`). Lists are evaluated by `eval_list()`, which checks `ne_operators` first, then evaluates args, then checks `operators`, then tries to find a `MO_FUNCTION` in the environment.

### `src/core.cpp`
Built-in operator implementations (`mo_sum`, `mo_difference`). Add new built-ins here and register them in `eval.cpp`'s `operators` or `ne_operators` maps.

### `src/log.cpp`
Errors/warnings are written to `LOG_STREAM` (a `stringstream`) with `LOG_FILENAME` and `LOG_LINE` for context. The caller (`main`) prints `LOG_STREAM.str()` after each eval. Use `write_error()`, `write_warning()`, `write_debug()` — not plog directly.

## Key Design Notes

- See `docs/known-issues.md` for current bugs and unimplemented stubs.
- `ne_operators` now has: `&`, `|`, `označi`, `ako`, `funkcija` (anonymous and named with recursion support).
- `operators` has: `+`, `-`, `*`, `/`, `=`, `==`, `>`, `<`, `>=`, `<=`, `?`, `!`.

## Workflow Rules

- **Document all changes**: When making code changes, update the relevant docs in `docs/` to reflect the new behavior. Keep `docs/language.md`, `docs/architecture.md`, and `docs/known-issues.md` in sync with the implementation.
- **Commit often in meaningful segments**: Group related changes into logical commits (e.g. feature + tests + docs). Don't bundle unrelated work. Push after each meaningful segment.
- **Commit messages**: Elegant and minimal. No co-author lines. No attribution.
