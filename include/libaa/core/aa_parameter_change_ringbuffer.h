
//
// Created by William.Hua on 2021/7/27.
//

#ifndef LIBAA_INCLUDE_LIBAA_CORE_AA_PARAMETER_CHANGE_RINGBUFFER_H
#define LIBAA_INCLUDE_LIBAA_CORE_AA_PARAMETER_CHANGE_RINGBUFFER_H
#pragma once

#include <memory>
namespace libaa {
struct ParameterChangePoint {
    int index;
    float time;
    float normalized_value;
};

class ParameterChange {
public:
    explicit ParameterChange(size_t size);

    ParameterChange(const ParameterChange &c) = delete;

    size_t getSize() const;

    bool push(ParameterChangePoint result);

    bool pop(ParameterChangePoint &result);

private:
    class Impl;
    std::shared_ptr<Impl> impl_;
};

bool operator==(const ParameterChangePoint &lhs,
                const ParameterChangePoint &rhs);

} // namespace libaa

#endif // LIBAA_INCLUDE_LIBAA_CORE_AA_PARAMETER_CHANGE_RINGBUFFER_H
