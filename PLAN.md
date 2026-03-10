# momo — Implementation Plan

## Phase 1: REPL + Logging ✓
- ~~Replace `main()` with an interactive loop: read line → eval → print result~~
- ~~Separate logging into `Logger` class (`src/log.h`, `src/log.cpp`)~~
- ~~Errors print to stderr and REPL continues (no crash)~~
- ~~Support multi-line input (bracket depth tracking)~~
- ~~File mode via CLI arg: `momo script.momo`~~

## Phase 1.5: Boolean type ✓
- ~~New `MO_BOOL` type with `moBool` struct~~
- ~~Two values: `tačno` (true) and `netačno` (false)~~
- ~~Reader recognizes `tačno` and `netačno` as boolean literals (not symbols)~~
- ~~Self-evaluating in eval~~

## Phase 1.6: Arithmetic, comparison & logic operators ✓
- ~~`*` — product, `/` — quotient (division by zero protected)~~
- ~~`?` — truthiness, `!` — logical not~~
- ~~`=` — equality, `==` — strict/referential equality~~
- ~~`>`, `<`, `>=`, `<=` — numeric comparisons~~
- ~~`&` — logical and, `|` — logical or (both short-circuit, ne_operators)~~

## Phase 2: `označi` — variable binding ✓
- ~~`[označi ime vrednost]` — ne_operator, evaluates `vrednost`, binds to `ime`~~
- ~~Overwrites in current scope, shadows parent scopes~~
- ~~Added `moEnv::bindVal` (create-or-overwrite)~~

## Phase 2.5: Tokenizer & Reader — `:` desugaring ✓
- ~~Tokenizer: `COLON` state, always length 1, terminates symbols~~
- ~~Reader: `read_colon` handles `:name value` → `[označi name value]`~~
- ~~`:muda` and `: muda` are equivalent~~
- ~~Works at top level and inside lists~~

## Phase 3: Language constructs ← NEXT
- `ako` (ne_operator) — `[ako uvjet onda]` / `[ako uvjet onda inače]`
- `funkcija` (ne_operator) — `[funkcija [args] body...]` returns a `moFunction`

## Phase 4: Dot property access
- Tokenizer reads `objekat.ime` as a single symbol
- Eval: if symbol contains `.`, split → look up `objekat` in env → get `ime` from its internal env

## Phase 5: Entity type
- New type `moEntity` — wraps a `moEnv` (properties) + a type tag string
- `stvar`/`prostor`/`delo` (standard lib, written in momo) create entities by evaluating body in a fresh env

## Phase 6: Standard lib (written in momo)
- `ispiši` — print
- `je-tip` — type check
- `stvar`, `prostor`, `delo` — prototypes using entity type

## Phase 7: Game REPL
- Separate game REPL for text adventure interaction (future)

## Also done
- Test suite (199 tests): types, bool, tokenizer, reader, eval, core, env, utils, logger, operators, označi, colon
- Fixed semicolon tokenizer fallthrough bug
- Added `isTrue()`/`equals()` to `moFunction`

## Key decisions
- `označi` overwrites if name already bound, shadows parent scopes
- Function keyword: `funkcija`
- `:key value` desugars to `[označi key value]` (reader-level transform)
- Entity type details: TBD
