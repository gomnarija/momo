# Known Issues

## Bugs

- **moList::equals()** — Compares `this->at(i)` to itself instead of `list->at(i)`. Always returns true for equal-length lists.
- **moSymbol::isTrue()** — Stub returning `false`. Symbols are always falsy.
- **moSymbol::equals()** — Stub returning `false`. Symbol comparison always fails.
- **NIL singleton** — Defined as `static` in the header, creating one copy per translation unit instead of a shared singleton.

## Unimplemented

- **moEnv::fsetTal** — Declared in `momo.h` but not implemented. Intended as a "force set" bypassing insert-only restriction.
