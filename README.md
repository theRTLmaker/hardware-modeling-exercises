# hardware-modeling-exercises

A small collection of hands-on exercises for building the kind of code you actually write as a hardware modelling engineer: queues, event loops, bandwidth limiters, contention, counters, determinism, and testing.

This repo is intentionally practical:
- Each exercise is small, focused, and test-driven
- Each exercise lives in its own folder with its own implementation and tests
- Build is simple: CMake + CTest, no heavy dependencies

## Goals

- Practice modern C++ patterns that matter for simulators (RAII, move semantics, deterministic behavior, clean APIs)
- Build intuition for modelling problems: backpressure, contention, queueing, and metrics
- Keep everything runnable, testable, and easy to extend

The root CMake project discovers exercises via `add_subdirectory()`.

# Exercises

1 | Bounded Queue with Backpressure
2 | Discrete Event Simulator

## Build and test

From the repo root:

```bash
cmake -S . -B build
cmake --build build -j
ctest --test-dir build --output-on-failure