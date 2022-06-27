//
// Created by user on 6/27/22.
//

#ifndef LIBAA_AA_WINDOWING_H
#define LIBAA_AA_WINDOWING_H

#pragma once
#include "libaa/core/aa_audio_processor_parameters.h"
#include "libaa/dsp/aa_window.h"
#include "libaa/mir/aa_mir_algorithm_base.h"
namespace libaa {
namespace MIR {
class Windowing : public MIRAlgorithmBase {
protected:
    Input<std::vector<float>> in_frame_;
    Output<std::vector<float>> out_frame_;

public:
    Windowing();

    std::string getName() const override;

    void configure() override;

    void compute() override;

    // -- testing --
    const std::vector<float> &getWindow() const {
        return window_;
    }

private:
    void createWindow();
    void normalize();

    std::vector<float> window_;
};
} // namespace MIR
} // namespace AudioEffect


#endif // LIBAA_AA_WINDOWING_H
