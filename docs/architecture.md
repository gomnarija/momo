# momo Architecture

## Pipeline

```
source text → Tokenizer → Reader → AST (moVal tree) → Eval → result
```

## Source Files

| File | Role |
|------|------|
| `src/tokenizer.h/.cpp` | Streaming tokenizer. States: WHITESPACE, SYMBOL, NUMBER, STRING, L_PARAN, R_PARAN, SEMICOLON, END. |
| `src/reader.h/.cpp` | Converts token stream into moVal AST. Handles multi-line lists, colon desugaring. |
| `src/momo.h` | Core type definitions: moVal, moNumber, moString, moSymbol, moList, moFunction, moBool, moEnv. |
| `src/types.cpp` | Type method implementations (print, isTrue, equals, etc.). |
| `src/eval.cpp` | Evaluator. Two dispatch maps: `operators` (args pre-evaluated) and `ne_operators` (args unevaluated). |
| `src/core.cpp` | Built-in operator implementations. |
| `src/log.h/.cpp` | Logging via LOG_STREAM stringstream. Use write_error/write_warning/write_debug. |
| `src/utils.cpp` | Utility helpers. |
| `src/momo.cpp` | main() — REPL + file mode. Reads `staznam.momo`. |

## Core Types

All values inherit from `moVal` and are held via `moValPtr` (`shared_ptr<moVal>`).

| MO_TYPE | C++ struct | Key fields |
|---------|------------|------------|
| MO_NIL | _moNil | Singleton `NIL`. print() → "Ništa" |
| MO_BOOL | moBool | `bool value` |
| MO_NUMBER | moNumber | `double value` |
| MO_STRING | moString | `std::string str` |
| MO_SYMBOL | moSymbol | `std::string name` — looked up in moEnv at eval |
| MO_LIST | moList | `vector<moValPtr>` — code and data |
| MO_FUNCTION | moFunction | `name`, `args` (moListPtr), `body` (moListPtr), `closure` (map) |

## Environment (moEnv)

Lexically-scoped linked list of `map<string, moValPtr>`.

| Method | Behavior |
|--------|----------|
| `getVal(key)` | Returns value or NIL if missing. |
| `insertVal(key, val)` | Add only — won't overwrite existing. |
| `bindVal(key, val)` | Create or overwrite. |
| `setVal(key, val)` | Update existing only. |
| `getAllBindings()` | Flattens scope chain into single map. |

## Eval Dispatch

1. Atoms self-evaluate (numbers, strings, bools, nil).
2. Symbols are looked up via `env_search` (walks scope chain).
3. Lists are evaluated by `eval_list()`:
   - Check `ne_operators` (unevaluated args): `&`, `|`, `označi`, `ako`, `funkcija`
   - Evaluate all args
   - Check `operators` (evaluated args): `+`, `-`, `*`, `/`, `=`, `==`, `>`, `<`, `>=`, `<=`, `?`, `!`
   - Try to find a `MO_FUNCTION` in the environment and call it

## Adding a New Built-in

1. Implement `moValPtr mo_name(moListPtr items, moEnv& env)` in `src/core.cpp`
2. Declare it in `src/core.cpp` (or a header if shared)
3. Register in `eval.cpp` — in `operators` if args should be evaluated first, `ne_operators` if not
