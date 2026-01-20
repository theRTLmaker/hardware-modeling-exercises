#pragma once

#include <cstdint>
#include <functional>
#include <optional>
#include <vector>
#include <queue>
#include <unordered_set>

namespace kata02 {

using Tick = std::uint64_t;
using EventId = std::uint64_t;

struct Event {
    Tick time;
    EventId seq_id;
    std::function<void()> cb;

    Event(Tick time, EventId id, std::function<void()> cb) :
        time(time),
        seq_id(id),
        cb(cb){}
};

struct CompareEvent {
    bool operator()(const Event& e1, const Event& e2) const {
        return e1.time == e2.time ? e1.seq_id > e2.seq_id : e1.time > e2.time;
    }
};

class Sim {
public:
    Sim() : time_(0), cur_id_(0) {}

    // Current simulation time
    Tick now() const noexcept {
        return time_;
    }

    // True if there are any pending events
    bool empty() const noexcept {
        return pq_.empty();
    }

    // Number of pending events (including cancelled ones if you keep tombstones)
    std::size_t pending() const noexcept {
        return pq_.size();
    }

    // Schedule callback to run at absolute time t.
    // Must be deterministic for equal timestamps.
    EventId schedule_at(Tick t, std::function<void()> cb) {
        EventId id = cur_id_++;
        Event ev{t, id, cb};

        pq_.push(ev);

        return id;
    }

    // Convenience: schedule after delta  ticks from now()
    EventId schedule_after(Tick delta, std::function<void()> cb) {
        EventId id = cur_id_++;
        Event ev{time_ + delta, id, cb};

        pq_.push(ev);

        return id;
    }

    // Run until the event queue is empty
    void run() {
        while (pq_.size()) {
            Event e = pq_.top();

            time_ = e.time;

            if (!is_cancelled(e.seq_id)) {
                e.cb();
            } else {
                cancelled_.erase(e.seq_id);
            }
            pq_.pop();
        }
    }

    // Run while there are events with time <= t_limit
    void run_until(Tick t_limit){
        while (pq_.size()) {
            Event e = pq_.top();
            if (e.time > t_limit) {
                break;
            }

            time_ = e.time;

            if (!is_cancelled(e.seq_id)) {
                e.cb();
            } else {
                cancelled_.erase(e.seq_id);
            }
            pq_.pop();
        }
    }

    // Run exactly one event. Returns false if no event executed (queue empty).
    bool step() {
        if (pq_.size()) {
            Event e = pq_.top();

            time_ = e.time;

            pq_.pop();
            if (!is_cancelled(e.seq_id)) {
                e.cb();

                return true;
            } else {
                cancelled_.erase(e.seq_id);
            }
        }
        return false;
    }

    // Checks if it is cancelled
    bool is_cancelled(EventId id) {
        auto it = cancelled_.find(id);
        if (it != cancelled_.end()) {
            return true;
        }

        return false;
    }

    // Cancel an event by id. Returns true if it was cancelled, false if not found/already fired.
    bool cancel(EventId id) {
        auto it = cancelled_.find(id);
        if (it != cancelled_.end()) {
            return true;
        }

        cancelled_.insert(id);
        return false;
    }

private:
    Tick time_;
    EventId cur_id_;
    std::priority_queue<Event, std::vector<Event>, CompareEvent> pq_;

    std::unordered_set<EventId> cancelled_;
};

} // namespace kata02
