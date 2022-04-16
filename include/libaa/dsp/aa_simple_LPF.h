//
// Created by user on 4/16/22.
//

#ifndef LIBAA_AA_SIMPLE_LPF_H
#define LIBAA_AA_SIMPLE_LPF_H

#pragma once

namespace libaa {
class SimpleLPF {
public:
    struct SimpleLPFParameters {
        float g;
    };
    void updateParameters(SimpleLPFParameters param) {
        param_ = param;
    }

    const SimpleLPFParameters &getParameters() const {
        return param_;
    }

    float processSample(float in) {
        float y = in * (1 - param_.g) + param_.g * state_;
        state_ = y;
        return y;
    }

private:
    SimpleLPFParameters param_;
    float state_{0.0f};
};

} // namespace libaa

#endif // LIBAA_AA_SIMPLE_LPF_H
