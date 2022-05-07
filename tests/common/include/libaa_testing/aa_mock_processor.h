//
// Created by user on 5/7/22.
//

#ifndef LIBAA_AA_MOCK_PROCESSOR_H
#define LIBAA_AA_MOCK_PROCESSOR_H

#pragma once
#include "libaa/processor/aa_audio_processor.h"
#include <gmock/gmock.h>

namespace libaa {
class MockProcessor : public IAudioProcessor {
public:
    ~MockProcessor() override = default;
    MockProcessor() {
        ON_CALL(*this, getName).WillByDefault(testing::Return("MockProcessor"));
        ON_CALL(*this, getLatencySamples).WillByDefault(testing::Return(0));
        ON_CALL(*this, getTailLengthSamples).WillByDefault(testing::Return(0));
    }

    MOCK_METHOD(std::string, getName, (), (override, const));
    MOCK_METHOD(void, processBlock, (AudioBlock *, AudioBlock *), (override));
    MOCK_METHOD(const AudioProcessorParameters *, getParameters, (),
                (override, const));
    MOCK_METHOD(void, prepareToPlay, (float, int), (override));
    MOCK_METHOD(int, getLatencySamples, (), (override, const, noexcept));
    MOCK_METHOD(int, getTailLengthSamples, (), (override, const, noexcept));
};
} // namespace libaa

#endif // LIBAA_AA_MOCK_PROCESSOR_H
