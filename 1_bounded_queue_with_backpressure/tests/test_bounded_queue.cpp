#include <stdexcept>
#include <cassert>
#include <iostream>
#include <memory>
#include <optional>

#include "bounded_queue.hpp"

using kata01::BoundedQueue;

static void test_basic_push_pop_fifo() {
  BoundedQueue<int> q(3);

  assert(q.empty());
  assert(q.size() == 0);
  assert(q.capacity() == 3);

  assert(q.push(1));
  assert(q.push(2));
  assert(q.push(3));
  assert(q.full());

  // Backpressure
  assert(!q.push(4));

  auto a = q.pop(); assert(a.has_value() && *a == 1);
  auto b = q.pop(); assert(b.has_value() && *b == 2);
  auto c = q.pop(); assert(c.has_value() && *c == 3);

  assert(q.empty());
  assert(!q.full());

  // Underflow
  auto d = q.pop();
  assert(!d.has_value());
}

static void test_move_only_type() {
  BoundedQueue<std::unique_ptr<int>> q(2);

  assert(q.push(std::make_unique<int>(7)));
  assert(q.push(std::make_unique<int>(8)));
  assert(q.full());
  assert(!q.push(std::make_unique<int>(9)));

  auto a = q.pop(); assert(a && *a && **a == 7);
  auto b = q.pop(); assert(b && *b && **b == 8);
  auto c = q.pop(); assert(!c);
}

static void test_invalid_capacity() {
  bool threw = false;
  try {
    BoundedQueue<int> q(0);
  } catch (const std::invalid_argument&) {
    threw = true;
  }
  assert(threw && "Expected invalid_argument for capacity 0");
}

int main() {
  test_invalid_capacity();
  test_basic_push_pop_fifo();
  test_move_only_type();

  std::cout << "[kata01] All tests passed.\n";
  return 0;
}