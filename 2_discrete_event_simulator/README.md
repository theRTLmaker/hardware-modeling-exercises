# Kata 02: Discrete-Event Simulator (Priority Queue + Callbacks)

This exercise builds the core of many performance models: a **deterministic discrete-event simulator**.
You schedule events at future timestamps, store them in a **priority queue**, and execute callbacks in time order.

If you can implement this cleanly, you can model queues, bandwidth, contention, arbitration, and pipelines on top of it.

---

## Objectives

Implement an event-driven simulation kernel with:

- A **timestamped event queue** (min-heap / priority queue)
- **Callbacks** invoked when events fire
- **Deterministic ordering** for events with the same timestamp
- Ability to **cancel** scheduled events (stretch goal)
- Small, testable, dependency-free code

---

## Definitions

- **Tick**: discrete simulation time unit (integer)
- **Event**: `(time, order, callback)`
- **Determinism**: identical inputs produce identical event order and identical results
