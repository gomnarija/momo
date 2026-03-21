# momo Language Reference

momo uses square-bracket prefix notation: `[operator arg1 arg2 ...]`

Comments start with `#` and run to end of line.

## Types

| Type | Example | Description |
|------|---------|-------------|
| Number | `42`, `-3.14` | Double-precision float. Negative literals supported (`-5`). |
| String | `"hello"` | Double-quoted text. |
| Bool | `tačno`, `netačno` | Boolean true/false. |
| Nil | `Ništa` | The absence of a value. Returned on errors and missing lookups. |
| List | `[+ 1 2]` | Square-bracket delimited. Used for both code and data. |
| Function | `[funkcija [x] [+ x 1]]` | First-class, supports closures. |

## Variables

Bind a name with `označi` (or the `:` shorthand):

```
[označi x 5]
:x 5          # equivalent — colon desugars to označi
```

## Arithmetic

| Operator | Usage | Notes |
|----------|-------|-------|
| `+` | `[+ 1 2]` → `3` | Also concatenates strings: `[+ "a" "b"]` → `"ab"` |
| `-` | `[- 10 3]` → `7` | |
| `*` | `[* 4 5]` → `20` | |
| `/` | `[/ 10 2]` → `5` | Division by zero returns Nil with error. |

## Comparison

| Operator | Usage | Notes |
|----------|-------|-------|
| `=` | `[= a b]` | Value equality. |
| `==` | `[== a b]` | Strict/referential equality. |
| `>` | `[> 5 3]` → `tačno` | |
| `<` | `[< 5 3]` → `netačno` | |
| `>=` | `[>= 5 5]` → `tačno` | |
| `<=` | `[<= 3 5]` → `tačno` | |

## Logic

| Operator | Usage | Notes |
|----------|-------|-------|
| `?` | `[? x]` | Returns truthiness of x as a Bool. |
| `!` | `[! x]` | Logical NOT. |
| `&` | `[& a b]` | Logical AND (short-circuit, args not pre-evaluated). |
| `\|` | `[\| a b]` | Logical OR (short-circuit, args not pre-evaluated). |

## Conditionals

```
[ako condition then-expr]
[ako condition then-expr else-expr]
```

`ako` means "if". The condition is evaluated; if truthy, evaluates and returns `then-expr`, otherwise evaluates and returns `else-expr` (or Nil if omitted).

```
[ako [> x 0] "positive" "non-positive"]
```

## Functions

### Anonymous

```
[funkcija [args...] body...]
```

Creates a function value. The last body expression is the return value.

```
:add1 [funkcija [x] [+ x 1]]
[add1 5]   # → 6
```

### Named

```
[funkcija name [args...] body...]
```

Creates a function, binds it to `name` in the current environment, and injects itself into its own closure (enabling recursion).

```
[funkcija fakt [n]
  [ako [= n 0] 1
    [* n [fakt [- n 1]]]]]

[fakt 5]   # → 120
```

### Closures

Functions capture the environment at definition time:

```
:x 10
:f [funkcija [y] [+ x y]]
[f 5]   # → 15
```

### Multiple body expressions

All body expressions are evaluated in order; the last one is returned:

```
[funkcija f [x]
  [:a 1]
  [+ x a]]

[f 10]   # → 11
```
