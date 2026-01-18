#pragma once

#include <cstddef>
#include <vector>
#include <optional>
#include <stdexcept>
#include <utility>

namespace kata01 {

template <typename T>
class BoundedQueue {
public:
    explicit BoundedQueue(std::size_t capacity)
        : cap_{capacity},
          q_(capacity) {
        if (cap_ == 0) {
            throw std::invalid_argument("BoundedQueue capacity must be > 0");
        }
    }

    std::size_t capacity() const noexcept { return cap_; }
    std::size_t size() const noexcept { return size_; }
    bool empty() const noexcept { return size_ == 0; }
    bool full() const noexcept { return size_ == cap_; }

    bool push(const T& elem) { // copies
        if (full()) return false;

        q_[front_] = elem;
        front_ = (front_ + 1) % cap_;
        ++size_;
        return true;
    }

    bool push(T&& elem) { // moves
        if (full()) return false;

        q_[front_] = std::move(elem);
        front_ = (front_ + 1) % cap_;
        ++size_;
        return true;
    }
    std::optional<T> pop() { // returns empty if queue is empty
        if (empty()) return std::nullopt;

        std::optional<T> out = std::move(q_[back_]);
        q_[back_].reset();
        back_ = (back_ + 1) % cap_;
        --size_;
        return out;
    }
private:
    std::size_t cap_{0};
    std::vector<std::optional<T>> q_;

    std::size_t size_{0};
    std::size_t front_{0};
    std::size_t back_{0};

};

} // namespace kata01
