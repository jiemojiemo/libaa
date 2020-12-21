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
#include "audio_effect/aa_biquad_filter.h"
#include "audio_effect/aa_iir_filter.h"
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

TEST_F(AudioEffectTest, BiquadFilter)
{
    BiquadFilter processor;
    processor.setRateAndBufferSizeDetails(sample_rate, block_size);
    processor.prepareToPlay(sample_rate, block_size);

    processor.processBlock(block);
    processor.reset();
    processor.releaseResources();
}

TEST_F(AudioEffectTest, IIRFilter)
{
    IIRFilter processor;
    processor.setRateAndBufferSizeDetails(sample_rate, block_size);
    processor.prepareToPlay(sample_rate, block_size);

    processor.processBlock(block);
    processor.reset();
    processor.releaseResources();
}