//
// Created by user on 3/24/22.
//
#include "libaa/core/aa_parameter_changes.h"

namespace libaa {
ParameterChanges::ParameterChanges(size_t num_params, size_t params_fifo_size)
    : param_changes_(num_params, nullptr) {

    for (size_t i = 0; i < num_params; ++i) {
        param_changes_[i] = std::make_shared<ParameterChange>(params_fifo_size);
    }
}

size_t ParameterChanges::getNumParameters() const {
    return param_changes_.size();
}

void ParameterChanges::resize(size_t num_params, size_t params_fifo_size) {
    if (num_params != getNumParameters()) {
        param_changes_.resize(num_params, nullptr);
        for (size_t i = 0; i < num_params; ++i) {
            param_changes_[i] =
                std::make_shared<ParameterChange>(params_fifo_size);
        }
    }
}

bool ParameterChanges::push(size_t param_index,
                            const ParameterChangePoint &result) {
    if (param_index >= param_changes_.size()) {
        return false;
    }
    return param_changes_[param_index]->push(result);
}

bool ParameterChanges::pop(size_t param_index, ParameterChangePoint &result) {
    if (param_index >= param_changes_.size()) {
        return false;
    }
    return param_changes_[param_index]->pop(result);
}
} // namespace libaa