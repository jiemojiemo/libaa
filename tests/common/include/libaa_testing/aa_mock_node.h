//
// Created by user on 4/28/22.
//

#ifndef LIBAA_AA_MOCK_NODE_H
#define LIBAA_AA_MOCK_NODE_H

#pragma once
#include "libaa/graph/aa_audio_connection.h"
#include "libaa/graph/aa_audio_port.h"
#include "libaa/graph/aa_i_node.h"
#include "libaa/graph/aa_parameter_change_connection.h"
#include "libaa/graph/aa_parameter_change_port.h"
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
    MOCK_METHOD(void, addUpstreamParameterChangeConnection,
                (const libaa::ParameterChangeConnection &), (override));
    MOCK_METHOD(libaa::AudioPort &, pullAudioPort, (int), (override));
    MOCK_METHOD(libaa::ParameterChangePort &, pullParameterChangePort, (int), (override));
    MOCK_METHOD(bool, hasProcessed, (), (override, const));
    MOCK_METHOD(void, setProcessedState, (bool), (override));
    MOCK_METHOD(void, prepareForNextBlock, (), (override));
    MOCK_METHOD(int, getAudioInputPortSize, (), (override, const));
    MOCK_METHOD(int, getAudioOutputPortSize, (), (override, const));
    MOCK_METHOD(int, getAudioInputPortChannels, (int), (override, const));
    MOCK_METHOD(int, getAudioOutputPortChannels, (int), (override, const));
    MOCK_METHOD(int, getParameterChangeInputPortSize, (), (override, const));
    MOCK_METHOD(int, getParameterChangeOutputPortSize, (), (override, const));
};
} // namespace libaa

#endif // LIBAA_AA_MOCK_NODE_H
