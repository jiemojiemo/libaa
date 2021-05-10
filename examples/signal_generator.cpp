
//
// Created by William.Hua on 2021/1/15.
//

#include "libaa/fileio/aa_audio_file.h"
#include <cmath>
using namespace std;
using namespace libaa;

enum class GeneratorWaveform{
    kTriangle = 0,
    kSin,
    kSaw
};

struct SignalGenData
{
    double normal_output            = 0.0;
    double inverted_output          = 0.0;
    double quad_phase_output_pos    = 0.0;
    double quad_phase_output_neg    = 0.0;
};

class AudioSignalGenerator
{
public:
    virtual bool prepare(double sample_rate) = 0;

    virtual const SignalGenData renderAudioOutput() = 0;
};

class LFO : public AudioSignalGenerator
{
public:
    bool prepare(double sample_rate) override {
        sample_rate_ = sample_rate;
        phase_inc_ = frequency_hz / sample_rate;

        mod_counter_ = 0.0;
        mod_counter_qp_ = 0.0;

        return false;
    }
    const SignalGenData renderAudioOutput() override {
        checkAndWrapModulo(mod_counter_, phase_inc_);

        mod_counter_qp_ = mod_counter_;
        advanceAndCheckWrapModulo(mod_counter_qp_, 0.25);

        SignalGenData output;

        if(waveform == GeneratorWaveform::kSaw)
        {
            output.normal_output = unipolarToBipolar(mod_counter_);
            output.quad_phase_output_pos = unipolarToBipolar(mod_counter_qp_);
        }
        else if(waveform == GeneratorWaveform::kTriangle){
            output.normal_output = 2.0 * fabs(unipolarToBipolar(mod_counter_)) - 1.0;
            output.quad_phase_output_pos = 2.0 * fabs(unipolarToBipolar(mod_counter_qp_)) - 1.0;
        }
        else if(waveform == GeneratorWaveform::kSin)
        {
            auto angle = mod_counter_ * 2.0 * M_PI - M_PI;
            output.normal_output = parabolicSine(angle);

            angle = mod_counter_qp_ * 2.0f * M_PI - M_PI;
            output.quad_phase_output_pos = parabolicSine(angle);
        }

        output.quad_phase_output_neg = -output.quad_phase_output_pos;
        output.inverted_output = -output.normal_output;

        mod_counter_ += phase_inc_;

        return output;
    }

    double frequency_hz        = {0.0f};
    GeneratorWaveform waveform = {GeneratorWaveform::kTriangle};
private:
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


    static double parabolicSine(double angle){
        double y = B * angle + C * angle * fabs(angle);
        y = P * (y * fabs(y) - y) + y;
        return y;
    }

    static inline double unipolarToBipolar(double value)
    {
        return 2.0*value - 1.0;
    }


    static constexpr double B = 4.0 / M_PI;
    static constexpr double C = -4.0 / (M_PI*M_PI);
    static constexpr double P = 0.225;

    double sample_rate_      = {0.0f};
    double phase_inc_        = {0.0f};
    double mod_counter_      = {0.0f};
    double mod_counter_qp_   = {0.0f};
};

int main() {
    string output_filename = "audio_generator.wav";
    AudioFile audio_file;

    LFO lfo;
    lfo.waveform = GeneratorWaveform::kTriangle;
    lfo.frequency_hz = 200;

    const double sample_rate = 44100;
    lfo.prepare(sample_rate);

    audio_file.setSampleRate(sample_rate);
    audio_file.setNumChannles(2);
    audio_file.setNumBits(16);

    for(int i = 0; i < sample_rate * 10; ++i){
        auto out = lfo.renderAudioOutput();
        audio_file.samples[0].push_back(out.normal_output);
        audio_file.samples[1].push_back(out.quad_phase_output_pos);
    }

    audio_file.saveToWave(output_filename);
}