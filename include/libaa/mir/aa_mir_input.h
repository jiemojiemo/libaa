//
// Created by user on 6/27/22.
//

#ifndef LIBAA_AA_MIR_INPUT_H
#define LIBAA_AA_MIR_INPUT_H

#pragma once
#include "libaa/mir/aa_mir_types.h"

namespace libaa::MIR {

class InputBase : public TypeProxy {
public:
    ~InputBase() override = default;
    template <typename Type>
    void set(const Type &data) {
        checkType<Type>();
        data_ = &data;
    }

    const void *getRawData() const {
        return data_;
    }

protected:
    const void *data_{nullptr};
};

template <typename T>
class Input : public InputBase {
public:
    ~Input() override = default;

    const T &get() const {
        if (data_ == nullptr) {
            throw std::runtime_error("Input data is null");
        }
        return *(T *)data_;
    }

    const std::type_info &getTypeInfo() const override {
        return typeid(T);
    }
};

} // namespace libaa::MIR

#endif // LIBAA_AA_MIR_INPUT_H
