
//
// Created by William.Hua on 2021/6/4.
//

#include <gmock/gmock.h>

#include "libaa/audio_effect/aa_audio_effect_processor.h"

using namespace testing;
using namespace libaa;

class GainProcessor : public AudioEffectProcessor
{
public:
    GainProcessor() = default;

    std::string getName() const override {
        return std::string("Gain");
    }
    void prepareToPlay(double sample_rate, int max_block_size) override {
        (void)(sample_rate);
        (void)(max_block_size);
    }
    void reset() override {

    }
    void releaseResources() override {

    }
    void processBlock(AudioBuffer<float> &buffer) override {
        (void)(buffer);
    }


};

class AGainProcessor : public Test
{
public:
    GainProcessor gain;
};

TEST_F(AGainProcessor, CanConstructWithDefault)
{
    GainProcessor gain;
}

TEST_F(AGainProcessor, CanReportCorretName)
{
    ASSERT_THAT(gain.getName(), Eq("Gain"));
}

TEST_F(AGainProcessor, GainParameterDefaultIsExpected)
{

}