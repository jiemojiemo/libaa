//
// Created by william on 2020/2/21.
//

#pragma once
#include "libaa/core/aa_math_tools.h"
#include <cmath>
#include <memory>
#include <vector>
namespace libaa {
template <typename T> class DelayLine {
public:
    void clear() noexcept {
        std::fill(&raw_data_[0], (&raw_data_[0] + size()), T(0));
    }

    /**
     * return the size of delay line
     */
    size_t size() const noexcept {
        return buffer_size_;
    }

    /**
     * resize the delay line
     *
     * @note resize will clears the data in delay line
     */
    void resize(size_t size) noexcept {
        buffer_size_ = findNextPower2Number(size);
        mask_ = buffer_size_ - 1;
        raw_data_.reset(new T[buffer_size_]);
        write_index_ = 0;

        clear();
    }

    /**
     * push a value to delay line
     */
    void push(T value) noexcept {
        raw_data_[write_index_] = value;
        ++write_index_;
        write_index_ &= mask_;
    }

    /**
     * returns the last value
     */
    T back() const noexcept {
        size_t read_index = (write_index_ - 1) & mask_;
        return raw_data_[read_index];
    }

    /**
     * returns value with delay
     */
    T get(size_t delay_in_samples) const noexcept {
        int read_index = write_index_ - 1 - delay_in_samples;
        read_index &= mask_;
        return raw_data_[read_index];
    }

    /**
     * Returns interpolation value
     */
    T getInterpolation(float delay) const noexcept {
        int previous_sample = static_cast<int>(floorf(delay));
        int next_sample = static_cast<int>(ceilf(delay));
        float fraction = static_cast<float>(next_sample) - delay;

        return fraction * get(previous_sample) +
               (1.0f - fraction) * get(next_sample);
    }

    /**
     * set value in specific delay
     */
    void set(size_t delay_in_samples, T new_val) noexcept {
        size_t pos = (write_index_ - delay_in_samples) & mask_;
        raw_data_[pos] = new_val;
    }

private:
    size_t write_index_{0};
    size_t buffer_size_{0};
    size_t mask_;
    std::unique_ptr<T[]> raw_data_ = nullptr;
};
} // namespace libaa
