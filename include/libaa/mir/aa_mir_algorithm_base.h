//
// Created by user on 6/27/22.
//

#ifndef LIBAA_AA_MIR_ALGORITHM_BASE_H
#define LIBAA_AA_MIR_ALGORITHM_BASE_H

#pragma once
#include "libaa/core/aa_audio_processor_parameters.h"
#include "libaa/mir/aa_mir_algorithm.h"
#include "libaa/mir/aa_mir_input.h"
#include "libaa/mir/aa_mir_output.h"
#include <map>
namespace libaa::MIR {
class MIRAlgorithmBase : public IMIRAlgorithm {
public:
    ~MIRAlgorithmBase() override = default;

    using InputsMap = std::map<std::string, InputBase *>;
    const InputsMap &getInputs() const;

    using OutputsMap = std::map<std::string, OutputBase *>;
    const OutputsMap &getOutputs() const;

    InputBase &input(const std::string &name) override;
    const InputBase &input(const std::string &name) const override;
    OutputBase &output(const std::string name) override;
    const OutputBase &output(const std::string name) const override;

    void setParameter(const std::string &param_name, float v) override;
    void setParameter(const std::string &param_name, const std::string &str_v) override;
    const AudioProcessorParameter &getParameter(const std::string &param_name) const override;

protected:
    void defineInput(InputBase &in, const std::string &name);
    void defineOutput(OutputBase &in, const std::string &name);
    AudioProcessorParameter &findParameter(const std::string &param_name);

    InputsMap inputs_;
    OutputsMap outputs_;
    AudioProcessorParameters parameters_;
};
} // namespace libaa::MIR

#endif // LIBAA_AA_MIR_ALGORITHM_BASE_H
