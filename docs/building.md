# Building momo

## Requirements

- CMake 3.11+
- C++17 compiler
- `plog` logging library (fetched automatically via CMake FetchContent)
- Google Test (fetched automatically for tests)

## Build

```bash
# Configure (from repo root)
cmake -S . -B build

# Build — executable goes to out/
cmake --build build
```

## Run

```bash
# Runs the interpreter, reads staznam.momo from working directory
./out/momo.exe
```

## Tests

```bash
# Run all tests
./out/momo_tests.exe

# Run specific test suite
./out/momo_tests.exe --gtest_filter="Funkcija.*"

# Run a single test
./out/momo_tests.exe --gtest_filter="Funkcija.NamedSelfRecursion"
```

## Project Layout

```
momo/
├── src/            # Source files
├── tests/          # Google Test files
├── docs/           # Documentation
├── out/            # Build output (momo.exe, momo_tests.exe)
├── build/          # CMake build directory
├── CLAUDE.md       # Instructions for Claude Code
├── PLAN.md         # Development roadmap
└── staznam.momo    # Default script (in out/)
```
