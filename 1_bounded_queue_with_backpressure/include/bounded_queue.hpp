#pragma once

#include <cstddef>
#include <deque>
#include <optional>
#include <stdexcept>
#include <utility>

namespace kata01 {

template <typename T>
class BoundedQueue {
public:
    explicit BoundedQueue(std::size_t capacity)
        : cap_{capacity} {
        if (cap_ == 0) {
            throw std::invalid_argument("BoundedQueue capacity must be > 0");
        }
    }

    std::size_t capacity() const noexcept { return cap_; }
    std::size_t size() const noexcept { return q_.size(); }
    bool empty() const noexcept { return q_.empty(); }
    bool full() const noexcept { return q_.size() >= cap_; }

    bool push(const T& elem) { // copies
        if (full()) return false;

        q_.push_back(elem);
        return true;
    }

    bool push(T&& elem) { // moves
        if (full()) return false;

        q_.push_back(std::move(elem));
        return true;
    }
    std::optional<T> pop() { // returns empty if queue is empty
        if (empty()) return std::nullopt;

        T out = std::move(q_.front());
        q_.pop_front();

        return out;
    }
private:
    std::size_t cap_{0};
    std::deque<T> q_;
};

} // namespace kata01
