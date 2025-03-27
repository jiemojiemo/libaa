//
// Created by user on 5/12/22.
//
#include "../aa_testing_helper.h"
#include "libaa/core/aa_clip.h"
#include "libaa/processor/aa_channel_vocoder_processor.h"
#include "libaa/processor/aa_processor_factory.h"
#include <gmock/gmock.h>

using namespace testing;
using namespace libaa;
class AProcessorFactory : public Test {
public:
};

TEST_F(AProcessorFactory, ReturnsNullIfNotSuchProcessor) {
    ASSERT_THAT(ProcessorFactory::create("XXXX"), IsNull());
}

TEST_F(AProcessorFactory, CanCreateGainProcessor) {
    auto name = "Gain";
    ASSERT_THAT(ProcessorFactory::create(name)->getName(), Eq(name));
}

TEST_F(AProcessorFactory, CanCreateDelayProcessor) {
    auto name = "Delay";
    ASSERT_THAT(ProcessorFactory::create(name)->getName(), Eq(name));
}

TEST_F(AProcessorFactory, CanCreatePitchShiftProcessor) {
    auto name = "PitchShifter";
    ASSERT_THAT(ProcessorFactory::create(name)->getName(), Eq(name));
}

TEST_F(AProcessorFactory, CanCreateReverbTankProcessor) {
    auto name = "ReverbTank";
    ASSERT_THAT(ProcessorFactory::create(name)->getName(), Eq(name));
}

TEST_F(AProcessorFactory, CanCreateSourceProcessor) {
    auto name = "Source";
    ASSERT_THAT(ProcessorFactory::create(name)->getName(), Eq(name));
}

TEST_F(AProcessorFactory, CanCreateSourceCallbackProcessor) {
    auto name = "Source Callback";
    ASSERT_THAT(ProcessorFactory::create(name)->getName(), Eq(name));
}

TEST_F(AProcessorFactory, CanCreateVibratoProcessor) {
    auto name = "Vibrato";
    ASSERT_THAT(ProcessorFactory::create(name)->getName(), Eq(name));
}

TEST_F(AProcessorFactory, CanCreateCompressorProcessor) {
    auto name = "Compressor";
    ASSERT_THAT(ProcessorFactory::create(name)->getName(), Eq(name));
}

TEST_F(AProcessorFactory, CanCreateLimiterProcessor) {
    auto name = "Limiter";
    ASSERT_THAT(ProcessorFactory::create(name)->getName(), Eq(name));
}

TEST_F(AProcessorFactory, CanCreateResamplerProcessor) {
    auto name = "Resampler";
    ASSERT_THAT(ProcessorFactory::create(name)->getName(), Eq(name));
}

TEST_F(AProcessorFactory, CanCreateFilterProcessor) {
    auto name = "IIRFilter";
    ASSERT_THAT(ProcessorFactory::create(name)->getName(), Eq(name));
}

TEST_F(AProcessorFactory, CanCreateProcessorWithParametersState) {
    auto name = "Gain";
    auto param_state = R"(
    {
        "parameters":
        {
            "Gain dB" : 10
        }
    }
    )";
    auto proc = ProcessorFactory::create(name, param_state);
    ASSERT_THAT(proc->getParameters()->get(0).getPlainValue(), FloatNear(10.0, 1e-5));
}

TEST_F(AProcessorFactory, CreateWithStateReturnsNullIfNotSuchProcessor) {
    auto name = "XXXX";
    auto param_state = R"(
    {
        "parameters":
        {
            "Gain dB" : 10
        }
    }
    )";
    auto proc = ProcessorFactory::create(name, param_state);

    ASSERT_THAT(proc, IsNull());
}

TEST_F(AProcessorFactory, CanCreateChannelProcessorWithFinder) {
    auto mock_audio_file = "processor_factory_test.wav";
    auto scope_file = makeScopeWavFile(mock_audio_file);
    auto name = "ChannelVocoder";
    auto param_state = R"(
    {
        "clip":{
            "audio_file_uri":"processor_factory_test.wav",
            "looping":true
        },
        "parameters":{
            "Number Band":32
        }
    }
    )";
    ResourcePathFinder finder;
    finder.append("./");

    auto proc = ProcessorFactory::create(name, param_state, &finder);
    auto *channel_proc = dynamic_cast<ChannelVocoderProcessor *>(proc.get());

    ASSERT_THAT(channel_proc->getName(), Eq(name));
    ASSERT_THAT(channel_proc->getCarrierClip()->getAudioFileUri(), Eq(mock_audio_file));
}

TEST_F(AProcessorFactory, CreateChannelVocoderThrowsIfCannotOpenCarrierFile) {
    auto name = "ChannelVocoder";
    auto param_state = R"(
    {
        "clip":{
            "audio_file_uri":"not_exist_file.wav",
            "looping":true
        },
        "parameters":{
            "Number Band":32
        }
    }
    )";
    ResourcePathFinder finder;
    finder.append("./");

    ASSERT_ANY_THROW(ProcessorFactory::create(name, param_state, &finder));
}