//
// Created by user on 3/24/22.
//
#include "farbot/fifo.hpp"
#include "libaa/core/aa_math_tools.h"
#include "libaa/core/aa_parameter_change_ringbuffer.h"
#include <cfloat>
#include <cmath>
namespace libaa {

class ParameterChange::Impl {
public:
    Impl(size_t size) : size_(size), points_(size) {}

public:
    size_t size_{0};
    farbot::fifo<ParameterChangePoint,
                 farbot::fifo_options::concurrency::single,
                 farbot::fifo_options::concurrency::single,
                 farbot::fifo_options::full_empty_failure_mode::
                     return_false_on_full_or_empty,
                 farbot::fifo_options::full_empty_failure_mode::
                     overwrite_or_return_default>
        points_;
};

ParameterChange::ParameterChange(size_t size)
    : impl_(std::make_shared<Impl>(size)) {}

size_t ParameterChange::getSize() const { return impl_->size_; }

bool ParameterChange::push(ParameterChangePoint result) {
    return impl_->points_.push(std::move(result));
}

bool ParameterChange::pop(ParameterChangePoint &result) {
    return impl_->points_.pop(result);
}

bool operator==(const ParameterChangePoint &lhs,
                const ParameterChangePoint &rhs) {
    return lhs.index == rhs.index &&
           approximatelyEqual(lhs.time, rhs.time, DBL_EPSILON) &&
           approximatelyEqual(lhs.normalized_value, rhs.normalized_value,
                              DBL_EPSILON);
}

} // namespace libaa
