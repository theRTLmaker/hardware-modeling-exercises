#include <cassert>
#include <iostream>
#include <vector>
#include <string>

#include "discrete_event_sim.hpp"  // adjust include if needed

using kata02::Sim;
using kata02::Tick;
using kata02::EventId;

static void test_step_advances_time_and_executes_one() {
  Sim sim;
  std::vector<Tick> fired;

  sim.schedule_at(10, [&]{ fired.push_back(sim.now()); });
  sim.schedule_at(20, [&]{ fired.push_back(sim.now()); });

  assert(sim.step());
  assert((fired == std::vector<Tick>{10}));
  assert(sim.now() == 10);
  assert(sim.pending() == 1);

  assert(sim.step());
  assert((fired == std::vector<Tick>{10, 20}));
  assert(sim.now() == 20);
  assert(sim.pending() == 0);

  assert(!sim.step());
}

static void test_same_tick_is_fifo_by_schedule_order() {
  Sim sim;
  std::vector<std::string> log;

  sim.schedule_at(5, [&]{ log.push_back("A"); });
  sim.schedule_at(5, [&]{ log.push_back("B"); });
  sim.schedule_at(5, [&]{ log.push_back("C"); });

  sim.run();
  assert((log == std::vector<std::string>{"A","B","C"}));
}

static void test_schedule_after_zero_runs_same_tick_after_current() {
  Sim sim;
  std::vector<std::string> log;

  sim.schedule_at(7, [&]{
    log.push_back("A");
    sim.schedule_after(0, [&]{ log.push_back("B"); });
  });

  sim.run();
  // B should run at tick 7 after A (same tick, later seq)
  assert((log == std::vector<std::string>{"A","B"}));
}

static void test_cancel_prevents_execution() {
  Sim sim;
  bool ran = false;

  EventId id = sim.schedule_at(3, [&]{ ran = true; });

  // cancel before run
  bool cancelled = sim.cancel(id);
  (void)cancelled;

  sim.run();
  assert(!ran);
}

static void test_cancel_one_of_many_same_tick() {
  Sim sim;
  std::vector<std::string> log;

  EventId a = sim.schedule_at(5, [&]{ log.push_back("A"); });
  EventId b = sim.schedule_at(5, [&]{ log.push_back("B"); });
  EventId c = sim.schedule_at(5, [&]{ log.push_back("C"); });

  sim.cancel(b);
  sim.run();

  // B should be absent, and remaining order should be stable
  assert((log == std::vector<std::string>{"A","C"}));
  (void)a; (void)c;
}

static void test_cancel_unknown_id_returns_false() {
  Sim sim;
  // No events scheduled
  bool ok = sim.cancel(12345);
  // Spec choice: cancelling unknown should return false
  assert(!ok);
}

static void test_cancel_after_event_fired_returns_false() {
  Sim sim;
  EventId id = sim.schedule_at(1, []{});
  sim.run();

  bool ok = sim.cancel(id);
  assert(!ok);
}

static void test_run_until_boundary_inclusive() {
  Sim sim;
  std::vector<Tick> fired;

  sim.schedule_at(1, [&]{ fired.push_back(sim.now()); });
  sim.schedule_at(2, [&]{ fired.push_back(sim.now()); });
  sim.schedule_at(3, [&]{ fired.push_back(sim.now()); });

  sim.run_until(2);
  assert((fired == std::vector<Tick>{1,2}));
  assert(sim.pending() == 1);
  assert(sim.now() == 2);

  sim.run();
  assert((fired == std::vector<Tick>{1,2,3}));
}

static void test_callback_can_schedule_earlier_than_future_event() {
  Sim sim;
  std::vector<Tick> fired;

  sim.schedule_at(10, [&]{ fired.push_back(sim.now()); });

  sim.schedule_at(5, [&]{
    fired.push_back(sim.now());
    sim.schedule_at(6, [&]{ fired.push_back(sim.now()); });
  });

  sim.run();
  assert((fired == std::vector<Tick>{5,6,10}));
}

int main() {
  test_step_advances_time_and_executes_one();
  test_same_tick_is_fifo_by_schedule_order();
  test_schedule_after_zero_runs_same_tick_after_current();
  test_cancel_prevents_execution();
  test_cancel_one_of_many_same_tick();
  test_cancel_unknown_id_returns_false();
  test_cancel_after_event_fired_returns_false();
  test_run_until_boundary_inclusive();
  test_callback_can_schedule_earlier_than_future_event();

  std::cout << "[kata02-extra] All tests passed.\n";
  return 0;
}