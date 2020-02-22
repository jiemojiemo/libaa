//
// Created by william on 2020/2/21.
//

#pragma once

#include <vector>

namespace libaa
{
template <typename T>
class DelayLine
{
public:
    void clear() noexcept
    {
        std::fill(raw_data_.begin(), raw_data_.end(), T(0));
    }
    /**
     * return the size of delay line
     */
    size_t size() const noexcept
    {
        return raw_data_.size();
    }

    /**
     * resize the delay line
     *
     * @note resize will clears the data in delay line
     */
    void resize(size_t size) noexcept
    {
        raw_data_.resize(size);
        least_recent_index_ = 0;

        clear();
    }

    /**
     * push a value to delay line
     */
    void push(T value) noexcept
    {
        raw_data_[least_recent_index_] = value;
        least_recent_index_ = (least_recent_index_ == 0) ? (size() - 1):(least_recent_index_ - 1);
    }

    /**
     * returns the last value
     */
    T back() const noexcept
    {
        return raw_data_[(least_recent_index_ + 1) % size()];
    }

    /**
     * returns value with delay
     */
    T get(size_t delay_in_samples) const noexcept
    {
        return raw_data_[(least_recent_index_ + 1 + delay_in_samples) % size()];
    }

    /**
     * set value in specific delay
     */
    void set(size_t delay_in_samples, T new_val) noexcept
    {
        raw_data_[(least_recent_index_ + 1 + delay_in_samples) % size()] = new_val;
    }

private:
    size_t least_recent_index_{0};
    std::vector<T> raw_data_;
};
}
