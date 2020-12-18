
//
// Created by William.Hua on 2020/12/8.
//

#pragma once
#include "aa_audio_effect_processor.h"
#include <array>

namespace libaa
{
class BiquadFilter : public AudioEffectProcessor
{
    template<typename T>
    class Biquad
    {
    public:
        T processSample(T in){
            return processSampleWithDirectForm(in);
        }

        T processSampleWithDirectForm(T in){
            T out = coeff_array_[a0] * in +
                coeff_array_[a1] * state_array_[z0] +
                coeff_array_[a2] * state_array_[z1] -
                coeff_array_[b1] * state_array_[z2] -
                coeff_array_[b2] * state_array_[z3];

            state_array_[z1] = state_array_[z0];
            state_array_[z0] = in;

            state_array_[z3] = state_array_[z2];
            state_array_[z2] = out;

            return out;
        }

        T processSampleWithCanonicalForm(T in){
            T wn = in - coeff_array_[b1]*state_array_[z0] - coeff_array_[b2]*state_array_[z1];
            T out = coeff_array_[a0]*wn + coeff_array_[a1]*state_array_[z0] + coeff_array_[a2]*state_array_[z1];

            state_array_[z1] = state_array_[z0];
            state_array_[z0] = wn;

            return out;
        }

        T processSampleWithTransposedDirectForm(T in){
            T wn = in + state_array_[z0];
            T out = coeff_array_[a0]*wn + state_array_[z2];

            state_array_[z0] = -coeff_array_[b1] + state_array_[z1];
            state_array_[z1] = -coeff_array_[b2]*wn;

            state_array_[z2] = coeff_array_[a1]*wn + state_array_[z3];
            state_array_[z3] = coeff_array_[a2]*wn;

            return out;
        }

        T processSampleWithTransposedCanonicalForm(T in){
            T out = coeff_array_[a0]*in + state_array_[z0];

            state_array_[z0] = coeff_array_[a1]*in - coeff_array_[b1]*out + state_array_[z1];
            state_array_[z1] = coeff_array_[a2]*in - coeff_array_[b2]*out;

            return out;
        }

        T operator()(T in){
            return processSampleWithTransposedCanonicalForm(in);
        }

        void reset(){
            std::fill(coeff_array_.begin(), coeff_array_.end(), 0.0f);
            std::fill(state_array_.begin(), state_array_.end(), 0.0f);
        }

        enum filter_coeff {a0, a1, a2, b1, b2, num_coeffs};
        enum state_registers {z0, z1, z2, z3, num_states};

        std::array<T, num_coeffs> coeff_array_{{0.0f,}};
        std::array<T, num_states> state_array_{{0.0f,}};
    };
public:

    std::string getName() const override {
        return std::string("BiquadFilter");
    }
    void prepareToPlay(double sample_rate, int max_block_size) override {
    }
    void reset() override {
    }
    void releaseResources() override {
    }
    void processBlock(AudioBuffer<float> &buffer) override;

    void setCoefficients(const std::array<float, Biquad<float>::filter_coeff::num_coeffs>& coff);
private:
    constexpr static size_t kDefaultNumChannels{2};
    std::vector<Biquad<float>> biquad_filters_{kDefaultNumChannels};
};
}
