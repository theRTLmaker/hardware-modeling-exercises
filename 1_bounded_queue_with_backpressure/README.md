# Bounded Queue with Backpressure

Queues are the basic molecule of simulation and modelling.
Implement a fixed-capacity queue that supports backpressure.

## The problem

Implement a bounded queue `BoundedQueue<T>`:

- Fixed capacity set at construction
- `push(...)` fails if the queue is full (backpressure)
- `pop()` returns empty if the queue is empty
- Must support move-only types (e.g. `std::unique_ptr<int>`)
- Must not leak memory
- Must behave deterministically

## Required API (C++20)

You may add extra methods, but these must exist:

```cpp
template <typename T>
class BoundedQueue {
public:
  explicit BoundedQueue(std::size_t capacity);

  std::size_t capacity() const noexcept;
  std::size_t size() const noexcept;
  bool empty() const noexcept;
  bool full() const noexcept;

  bool push(const T& value);   // copies
  bool push(T&& value);        // moves
  std::optional<T> pop();      // returns empty if queue is empty
};