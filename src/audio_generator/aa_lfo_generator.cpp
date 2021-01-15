
//
// Created by William.Hua on 2021/1/15.
//

#include "audio_generator/aa_lfo_generator.h"
#include "dsp/aa_dsp_utilities.h"

namespace libaa
{

class LFOGenerator::Impl
{
public:
    Impl(LFOGenerator* parent)
        : parent_(parent)
    {

    }

    void prepare(double sample_rate) {
        sample_rate_ = sample_rate;
        phase_inc_ = parent_->frequency_hz / sample_rate;

        mod_counter_ = 0.0;
        mod_counter_qp_ = 0.0;
    }

    SignalGenData renderAudioOutput(){
        checkAndWrapModulo(mod_counter_, phase_inc_);

        mod_counter_qp_ = mod_counter_;
        advanceAndCheckWrapModulo(mod_counter_qp_, 0.25);

        SignalGenData output;

        if(parent_->waveform == GeneratorWaveform::kSaw)
        {
            output.normal_output = unipolarToBipolar(mod_counter_);
            output.quad_phase_output_pos = unipolarToBipolar(mod_counter_qp_);
        }
        else if(parent_->waveform == GeneratorWaveform::kTriangle){
            output.normal_output = 2.0 * fabs(unipolarToBipolar(mod_counter_)) - 1.0;
            output.quad_phase_output_pos = 2.0 * fabs(unipolarToBipolar(mod_counter_qp_)) - 1.0;
        }
        else if(parent_->waveform == GeneratorWaveform::kSin)
        {
            auto angle = mod_counter_ * 2.0 * M_PI - M_PI;
            output.normal_output = DSPUtilities::parabolicSine(angle);

            angle = mod_counter_qp_ * 2.0f * M_PI - M_PI;
            output.quad_phase_output_pos = DSPUtilities::parabolicSine(angle);
        }

        output.quad_phase_output_neg = -output.quad_phase_output_pos;
        output.inverted_output = -output.normal_output;

        mod_counter_ += phase_inc_;

        return output;
    }

    double getPhaseIncrement() const{
        return phase_inc_;
    }


    static void checkAndWrapModulo(double& mod_counter, double phase_inc){
        if(phase_inc > 0 && mod_counter >= 1.0){
            mod_counter -= 1.0;
        }
        if(phase_inc < 0 && mod_counter <= 0){
            mod_counter += 1.0;
        }
    }

    static void advanceAndCheckWrapModulo(double& mod_counter, double phase_inc){
        mod_counter += phase_inc;
        checkAndWrapModulo(mod_counter, phase_inc);
    }

    static inline double unipolarToBipolar(double value)
    {
        return 2.0*value - 1.0;
    }

    double sample_rate_      = {0.0f};
    double phase_inc_        = {0.0f};
    double mod_counter_      = {0.0f};
    double mod_counter_qp_   = {0.0f};
    LFOGenerator* parent_;
};

LFOGenerator::LFOGenerator()
    : impl_(std::make_shared<Impl>(this))
{

}

void LFOGenerator::prepare(double sample_rate) {
    return impl_->prepare(sample_rate);
}

SignalGenData LFOGenerator::renderAudioOutput() {
    return impl_->renderAudioOutput();
}

double LFOGenerator::getPhaseIncrement() const {
    return impl_->getPhaseIncrement();
}

}