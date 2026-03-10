# momo — Implementation Plan

## Phase 1: REPL + Logging ✓
- ~~Replace `main()` with an interactive loop: read line → eval → print result~~
- ~~Separate logging into `Logger` class (`src/log.h`, `src/log.cpp`)~~
- ~~Errors print to stderr and REPL continues (no crash)~~
- ~~Support multi-line input (bracket depth tracking)~~
- ~~File mode via CLI arg: `momo script.momo`~~

## Phase 1.5: Boolean type ← NEXT
- New `MO_BOOL` type with `moBool` struct
- Two values: `tačno` (true) and `netačno` (false)
- `tačno` is truthy, `netačno` is falsy
- `print()` returns `"tačno"` / `"netačno"`
- Reader recognizes `tačno` and `netačno` as boolean literals (not symbols)

## Phase 1.6: Arithmetic, comparison & logic operators
- `*` — product (like `+`, works on numbers, strings convert)
- `/` — quotient (numbers only)
- `?` — truthiness check (returns `tačno`/`netačno`)
- `!` — logical not (returns `tačno`/`netačno`)
- `=` — equality via `moVal::equals` (returns `tačno`/`netačno`)
- `==` — referential/strict equality (returns `tačno`/`netačno`)
- `>` — greater than
- `<` — less than
- `>=` — greater or equal
- `<=` — less or equal
- `&` — logical and (short-circuit, ne_operator)
- `|` — logical or (short-circuit, ne_operator)

## Phase 2: Tokenizer & Reader — `:` desugaring
- Tokenizer: recognize `:symbol` as a single token (colon-prefixed)
- Reader: when it sees `:boja`, reads next value, emits `[označi boja <value>]`
- Makes `:` work anywhere, not just in entity definitions

## Phase 3: Core operators
- `označi` (ne_operator) — `[označi ime vrednost]`, binds/overwrites name in current env
- `ako` (ne_operator) — `[ako uvjet onda]` / `[ako uvjet onda inače]`
- `funkcija` (ne_operator) — `[funkcija [args] body...]` returns a `moFunction`
- Remaining commented-out operators: `*`, `/`, `=`, `>`, `<`, `?`, `!`

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

## Key decisions
- `označi` overwrites if name already bound
- Function keyword: `funkcija`
- `:key value` desugars to `[označi key value]` (reader-level transform)
- Entity type details: TBD
