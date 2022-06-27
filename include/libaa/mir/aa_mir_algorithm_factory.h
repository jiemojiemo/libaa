//
// Created by user on 6/27/22.
//

#ifndef LIBAA_AA_MIR_ALGORITHM_FACTORY_H
#define LIBAA_AA_MIR_ALGORITHM_FACTORY_H

#pragma once
#include <memory>
#include <string>
namespace libaa::MIR {
class IMIRAlgorithm;

namespace MIRAlgorithmFactory {
std::unique_ptr<IMIRAlgorithm> create(const std::string &name);
} // namespace MIRAlgorithmFactory
} // namespace libaa::MIR

#endif // LIBAA_AA_MIR_ALGORITHM_FACTORY_H
