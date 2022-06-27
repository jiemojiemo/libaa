//
// Created by user on 6/27/22.
//

#ifndef LIBAA_AA_MIR_TYPES_H
#define LIBAA_AA_MIR_TYPES_H

#pragma once
#include <stdexcept>
#include <typeinfo>

namespace libaa::MIR {
class TypeProxy {
public:
    virtual ~TypeProxy() = default;

    void checkType(const std::type_info &lhs, const std::type_info &rhs) const {
        if (lhs != rhs) {
            throw std::runtime_error("Error when checking types");
        }
    }

    template <class T>
    void checkType() const {
        checkType(typeid(T), getTypeInfo());
    }

    virtual const std::type_info &getTypeInfo() const = 0;
};
} // namespace libaa::MIR

#endif // LIBAA_AA_MIR_TYPES_H
