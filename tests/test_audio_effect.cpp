//
// Created by william on 2020/2/28.
//

#include "audio_effect/aa_vibrato_effect.h"
#include "audio_effect/aa_delay_effect.h"
#include "audio_basics/aa_delay_line_array.h"
#include "audio_effect/aa_flanger.h"
#include "audio_effect/aa_chorus.h"
#include "audio_effect/aa_tremolo.h"
#include "audio_effect/aa_compressor.h"
#include "audio_effect/aa_distortion.h"
#include "audio_effect/aa_robot.h"
#include <gmock/gmock.h>

using namespace std;
using namespace libaa;
using namespace testing;

class AudioEffectTest : public Test
{
public:
    void SetUp() override
    {
        left_data.resize(sample_rate*2, 1.0f);
        right_data.resize(sample_rate*2, 1.0f);

        data_refer_to[0] = left_data.data();
        data_refer_to[1] = right_data.data();

        block = AudioBuffer<float>(data_refer_to,2,0,block_size );
    }
    int sample_rate = 44100;
    int block_size = 1024;

    vector<float> left_data;
    vector<float> right_data;
    float* data_refer_to[2];
    AudioBuffer<float> block;

};


TEST_F(AudioEffectTest, DelayTest)
{
    DelayEffect processor;
    processor.setRateAndBufferSizeDetails(sample_rate, block_size);
    processor.prepareToPlay(sample_rate, block_size);

    processor.processBlock(block);
    processor.reset();
    processor.releaseResources();
}

TEST_F(AudioEffectTest, VibratoTest)
{
    VibratoEffect processor;
    processor.setRateAndBufferSizeDetails(sample_rate, block_size);
    processor.prepareToPlay(sample_rate, block_size);

    processor.processBlock(block);
    processor.reset();
    processor.releaseResources();
}

TEST_F(AudioEffectTest, FlangerTest)
{
    Flanger processor;
    processor.setRateAndBufferSizeDetails(sample_rate, block_size);
    processor.prepareToPlay(sample_rate, block_size);

    processor.processBlock(block);
    processor.reset();
    processor.releaseResources();
}

TEST_F(AudioEffectTest, Chorus)
{
    Chorus processor;
    processor.setRateAndBufferSizeDetails(sample_rate, block_size);
    processor.prepareToPlay(sample_rate, block_size);

    processor.processBlock(block);
    processor.reset();
    processor.releaseResources();
}

TEST_F(AudioEffectTest, Tremolo)
{
    Tremolo processor;
    processor.setRateAndBufferSizeDetails(sample_rate, block_size);
    processor.prepareToPlay(sample_rate, block_size);

    processor.processBlock(block);
    processor.reset();
    processor.releaseResources();
}


TEST_F(AudioEffectTest, Compressor)
{
    Compressor processor;
    processor.setRateAndBufferSizeDetails(sample_rate, block_size);
    processor.prepareToPlay(sample_rate, block_size);

    processor.processBlock(block);
    processor.reset();
    processor.releaseResources();
}


TEST_F(AudioEffectTest, Distortion)
{
    Distortion processor;
    processor.setRateAndBufferSizeDetails(sample_rate, block_size);
    processor.prepareToPlay(sample_rate, block_size);

    processor.processBlock(block);
    processor.reset();
    processor.releaseResources();
}

TEST_F(AudioEffectTest, Robotisation)
{
    Robotisation processor;
    processor.setRateAndBufferSizeDetails(sample_rate, block_size);
    processor.prepareToPlay(sample_rate, block_size);

    processor.processBlock(block);
    processor.reset();
    processor.releaseResources();
}
#include <array>
class BiquadFilter : public AudioEffectProcessor
{
public:
    string getName() const override {
        return std::string();
    }
    void prepareToPlay(double sample_rate, int max_block_size) override {

    }
    void reset() override {
        std::fill(coeff_array_.begin(), coeff_array_.end(), 0.0f);
        std::fill(state_array_.begin(), state_array_.end(), 0.0f);
    }
    void releaseResources() override {

    }
    void processBlock(AudioBuffer<float> &buffer) override {
        const auto num_channels = buffer.getNumChannels();
        const auto num_frames = buffer.getNumFrames();

        for(auto c = 0u; c < num_channels; ++c){
            auto* channel = buffer.getWritePointer(c);
            for(auto i = 0u; i < num_frames; ++i){
                float in = channel[i];
                float out = coeff_array_[a0] * in +
                             coeff_array_[a1] * state_array_[z0] +
                             coeff_array_[a2] * state_array_[z1] -
                             coeff_array_[b1] * state_array_[z2] -
                             coeff_array_[b2] * state_array_[z3];

                state_array_[z1] = state_array_[z0];
                state_array_[z0] = in;

                state_array_[z3] = state_array_[z2];
                state_array_[z2] = out;

                channel[i] = out;
            }
        }
    }

private:
    enum filter_coeff {a0, a1, a2, b1, b2, num_coeffs};
    enum state_registers {z0, z1, z2, z3, num_states};

    std::array<float, num_coeffs> coeff_array_{0.0f};
    std::array<float, num_states> state_array_{0.0f};
};

TEST_F(AudioEffectTest, BiquadFilter)
{
    BiquadFilter processor;
    processor.setRateAndBufferSizeDetails(sample_rate, block_size);
    processor.prepareToPlay(sample_rate, block_size);

    processor.processBlock(block);
    processor.reset();
    processor.releaseResources();
}