
//
// Created by William.Hua on 2021/8/3.
//

#ifndef LIBAA_INCLUDE_LIBAA_CORE_AA_PARAMETER_CHANGES_H
#define LIBAA_INCLUDE_LIBAA_CORE_AA_PARAMETER_CHANGES_H
#pragma once
#include "libaa/core/aa_parameter_change_ringbuffer.h"
#include <memory>
#include <vector>

namespace libaa {
class ParameterChanges {
public:
    explicit ParameterChanges(size_t num_params, size_t params_fifo_size);

    ParameterChanges(const ParameterChanges &) = delete;

    size_t getNumParameters() const;

    void resize(size_t num_params, size_t params_fifo_size);

    bool push(size_t param_index, const ParameterChangePoint &result);

    bool pop(size_t param_index, ParameterChangePoint &result);

private:
    std::vector<std::shared_ptr<ParameterChange>> param_changes_;
};
} // namespace libaa

#endif // LIBAA_INCLUDE_LIBAA_CORE_AA_PARAMETER_CHANGES_H
