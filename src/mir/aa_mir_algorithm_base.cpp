//
// Created by user on 6/27/22.
//

#include "libaa/mir/aa_mir_algorithm_base.h"

namespace libaa::MIR {

const MIRAlgorithmBase::InputsMap &MIRAlgorithmBase::getInputs() const {
    return inputs_;
}

const MIRAlgorithmBase::OutputsMap &MIRAlgorithmBase::getOutputs() const {
    return outputs_;
}

InputBase &MIRAlgorithmBase::input(const std::string &name) {
    return *inputs_.at(name);
}
const InputBase &MIRAlgorithmBase::input(const std::string &name) const {
    return *inputs_.at(name);
}

OutputBase &MIRAlgorithmBase::output(const std::string name) {
    return *outputs_.at(name);
}

const OutputBase &MIRAlgorithmBase::output(const std::string name) const {
    return *outputs_.at(name);
}

void MIRAlgorithmBase::setParameter(const std::string &param_name, float v) {
    findParameter(param_name).setPlainValue(v);
}
void MIRAlgorithmBase::setParameter(const std::string &param_name, const std::string &str_v) {
    findParameter(param_name).setPlainValue(str_v);
}

const AudioProcessorParameter &MIRAlgorithmBase::getParameter(const std::string &param_name) const {
    auto index = parameters_.findParameterIndexByName(param_name);
    return parameters_.get(index);
}

void MIRAlgorithmBase::defineInput(InputBase &in, const std::string &name) {
    inputs_.emplace(name, &in);
}
void MIRAlgorithmBase::defineOutput(OutputBase &in, const std::string &name) {
    outputs_.emplace(name, &in);
}

AudioProcessorParameter &MIRAlgorithmBase::findParameter(const std::string &param_name) {
    auto index = parameters_.findParameterIndexByName(param_name);
    return parameters_.get(index);
}

} // namespace libaa::MIR