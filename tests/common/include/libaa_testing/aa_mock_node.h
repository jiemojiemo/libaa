//
// Created by user on 4/28/22.
//

#ifndef LIBAA_AA_MOCK_NODE_H
#define LIBAA_AA_MOCK_NODE_H

#pragma once
#include "libaa/graph/aa_audio_port.h"
#include "libaa/graph/aa_i_node.h"
#include <gmock/gmock.h>

namespace libaa {
class MockNode : public libaa::INode {
public:
    ~MockNode() override = default;
    MOCK_METHOD(void, setNodeID, (std::string), (override));
    MOCK_METHOD(std::string, getNodeID, (), (override, const));
    MOCK_METHOD(void, prepareToPlay, (float, int), (override));
    MOCK_METHOD(void, addUpstreamAudioConnection,
                (const libaa::AudioConnection &), (override));
    MOCK_METHOD(libaa::AudioPort &, pullAudioPort, (int), (override));
    MOCK_METHOD(bool, hasProcessed, (), (override, const));
    MOCK_METHOD(void, prepareForNextBlock, (), (override));
    MOCK_METHOD(const std::vector<AudioPort> &, getAudioInputPorts, (),
                (override, const));
    MOCK_METHOD(const std::vector<AudioPort> &, getAudioOutputPorts, (),
                (override, const));
};
} // namespace libaa

#endif // LIBAA_AA_MOCK_NODE_H
