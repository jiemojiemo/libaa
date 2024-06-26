//
// Created by user on 4/28/22.
//

#include "libaa/core/aa_json_utilities.h"
#include "libaa/graph/aa_audio_processor_node.h"
#include "libaa/graph/aa_transport_context.h"
#include "libaa/processor/aa_gain_processor.h"
#include "libaa_testing/aa_mock_processor.h"
#include <gmock/gmock.h>
#include <nlohmann/json.hpp>

using namespace testing;
using namespace libaa;

class AProcessorNode : public Test {
public:
    void SetUp() override {
        proc = std::make_shared<NiceMock<MockProcessor>>();
        up_proc = std::make_shared<NiceMock<MockProcessor>>();
        upstream_node = std::make_shared<ProcessorNode>(up_proc);
        gain_proc = std::make_shared<GainProcessor>();

        params.pushFloatParameter("A", 0, 0, 1.0);
        params.pushFloatParameter("B", 0, 0, 1.0);
        params.pushFloatParameter("C", 0, 0, 1.0);
        params.pushFloatParameter("D", 0, 0, 1.0);
    }

    auto makeMockProcessorState() const {
        GainProcessor gain;
        return gain.getState();
    }

    std::shared_ptr<MockProcessor> proc;
    std::shared_ptr<MockProcessor> up_proc;
    std::shared_ptr<ProcessorNode> upstream_node;
    std::shared_ptr<GainProcessor> gain_proc;
    AudioBlock block{{{1, 1, 1}, {2, 2, 2}}};

    AudioProcessorParameters params;
    const float sr = 44100;
    const int max_block_size = 128;
};

TEST_F(AProcessorNode, CanInitWithInputsAndOutputAndProcessor) {
    ProcessorNode node(proc, {2}, {2});

    ASSERT_THAT(proc.use_count(), Eq(2));
}

TEST_F(AProcessorNode, InitWithChannelsReallocateInternalBuffers) {
    auto input_channels = {2};
    auto output_channels = {2, 2};
    ProcessorNode node(proc, input_channels, output_channels);

    ASSERT_THAT(node.getInputBlock()->buffer.getNumberChannels(), Eq(2));
    ASSERT_THAT(node.getOutputBlock()->buffer.getNumberChannels(), Eq(4));
}

TEST_F(AProcessorNode, CanInitWithProcessor) {
    ProcessorNode node(proc);

    ASSERT_THAT(proc.use_count(), Eq(2));
}

TEST_F(AProcessorNode, InitWithProcessorHasDefaultInputAndOutputChannels) {
    ProcessorNode node(proc);

    ASSERT_THAT(node.getInputBlock()->buffer.getNumberChannels(), Eq(2));
    ASSERT_THAT(node.getOutputBlock()->buffer.getNumberChannels(), Eq(2));
}

TEST_F(AProcessorNode, InitWithProcessReallocateParameterChanges) {

    auto num_params = params.size();

    EXPECT_CALL(*proc, getParameters).WillRepeatedly(Return(&params));

    ProcessorNode node(proc);
    ASSERT_THAT(node.getInputBlock()->param_changes.getNumParameters(),
                Eq(node.getDefaultNumberParametersInBlock()));
}

TEST_F(AProcessorNode, HasDefalutOneStereoInputAudioPort) {
    ProcessorNode node(proc);

    ASSERT_THAT(node.getAudioInputPortSize(), Eq(1));
    ASSERT_THAT(node.getAudioInputPortChannels(0), Eq(2));
}

TEST_F(AProcessorNode, HasDefalutOneStereoOutputAudioPort) {
    ProcessorNode node(proc);

    ASSERT_THAT(node.getAudioOutputPortSize(), Eq(1));
    ASSERT_THAT(node.getAudioOutputPortChannels(0), Eq(2));
}

TEST_F(AProcessorNode, HasDefaultOneInputParameterChangePort) {
    ProcessorNode node(proc);

    ASSERT_THAT(node.getParameterChangeInputPortSize(), Eq(1));
}

TEST_F(AProcessorNode, HasDefaultOneOutputParameterChangePort) {
    ProcessorNode node(proc);

    ASSERT_THAT(node.getParameterChangeOutputPortSize(), Eq(1));
}

TEST_F(AProcessorNode, GetAudioInputPortChannelsThrowsIfOutofSize) {
    ProcessorNode node(proc);

    ASSERT_ANY_THROW(node.getAudioOutputPortChannels(1));
}

TEST_F(AProcessorNode, GetAudioOutputPortChannelsThrowsIfOutofSize) {
    ProcessorNode node(proc);

    ASSERT_ANY_THROW(node.getAudioOutputPortChannels(1));
}

TEST_F(AProcessorNode, InitWithChannelsHasMultipleInputAudioPort) {
    auto input_channels = {2, 2};
    auto output_channels = {1};
    ProcessorNode node(proc, input_channels, output_channels);

    ASSERT_THAT(node.getAudioInputPortSize(), Eq(input_channels.size()));
    ASSERT_THAT(node.getAudioInputPortChannels(0), Eq(2));
    ASSERT_THAT(node.getAudioInputPortChannels(1), Eq(2));

    ASSERT_THAT(node.getAudioOutputPortSize(), Eq(output_channels.size()));
    ASSERT_THAT(node.getAudioOutputPortChannels(0), Eq(1));
}

TEST_F(AProcessorNode, CanGetAudioInputPortSize) {
    ProcessorNode node(proc);

    ASSERT_THAT(node.getAudioInputPortSize(), Eq(1));
}

TEST_F(AProcessorNode, AddInputPortWillReallocateInputBuffer) {
    ProcessorNode node(proc);
    ASSERT_THAT(node.getInputBlock()->buffer.getNumberChannels(), Eq(2));

    node.addAudioInputPort(2);
    ASSERT_THAT(node.getInputBlock()->buffer.getNumberChannels(), Eq(4));
}

TEST_F(AProcessorNode, AddInputPortIncreaseThePortSize) {
    ProcessorNode node(proc);
    ASSERT_THAT(node.getAudioInputPortSize(), Eq(1));

    node.addAudioInputPort(2);

    ASSERT_THAT(node.getAudioInputPortSize(), Eq(2));
}

TEST_F(AProcessorNode, AddAudioOutputPortWillReallocateOutputBuffer) {
    ProcessorNode node(proc);
    ASSERT_THAT(node.getOutputBlock()->buffer.getNumberChannels(), Eq(2));

    node.addAudioOutputPort(2);

    ASSERT_THAT(node.getOutputBlock()->buffer.getNumberChannels(), Eq(4));
}

TEST_F(AProcessorNode, AddOutputPortIncreaseThePortSize) {
    ProcessorNode node(proc);
    ASSERT_THAT(node.getAudioOutputPortSize(), Eq(1));

    node.addAudioOutputPort(2);

    ASSERT_THAT(node.getAudioOutputPortSize(), Eq(2));
}

TEST_F(AProcessorNode, DefaultIDIsEmpty) {
    ProcessorNode node(proc);

    ASSERT_THAT(node.getNodeID(), Eq(""));
}

TEST_F(AProcessorNode, CanSetNodeId) {
    auto id = "ABC";
    ProcessorNode node(proc);

    node.setNodeID(id);

    ASSERT_THAT(node.getNodeID(), Eq(id));
}

TEST_F(AProcessorNode, PrepareToPlayWillReallocateInputAudioBuffer) {
    ProcessorNode node(proc);
    node.prepareToPlay(sr, max_block_size);

    ASSERT_THAT(node.getInputBlock()->buffer.getNumberFrames(),
                Eq(max_block_size));
}

TEST_F(AProcessorNode, PrepareToPlayWillReallocateOutputAudioBuffer) {
    ProcessorNode node(proc);
    node.prepareToPlay(sr, max_block_size);

    ASSERT_THAT(node.getOutputBlock()->buffer.getNumberFrames(),
                Eq(max_block_size));
}

TEST_F(AProcessorNode, PrepareToPlayWillPrepareTheProcessor) {
    ProcessorNode node(proc);

    EXPECT_CALL(*proc, prepareToPlay(sr, max_block_size)).Times(1);

    node.prepareToPlay(sr, max_block_size);
}

TEST_F(AProcessorNode, HasNoAudioConnectionWhenInit) {
    ProcessorNode node(proc);

    ASSERT_THAT(node.getUpstreamAudioConnections().size(), Eq(0));
}

TEST_F(AProcessorNode, HasNoParameterChangeConnectionWhenInit) {
    ProcessorNode node(proc);

    ASSERT_THAT(node.getUpstreamParameterConnections().size(), Eq(0));
}

TEST_F(AProcessorNode, CanAddUpstreamAudioConnection) {
    ProcessorNode node(proc);
    AudioConnection connection{upstream_node, 0, 0};

    node.addUpstreamAudioConnection(connection);
}

TEST_F(AProcessorNode, CanAddUpstreamParameterChangeConnection) {
    ProcessorNode node(proc);
    ParameterChangeConnection connection{upstream_node, 0, 0};

    node.addUpstreamParameterChangeConnection(connection);
}

TEST_F(AProcessorNode, AddAudioConnectionIncreasesTheConnectionSize) {
    ProcessorNode node(proc);
    AudioConnection connection{upstream_node, 0, 0};

    node.addUpstreamAudioConnection(connection);

    ASSERT_THAT(node.getUpstreamAudioConnections().size(), Eq(1));
}

TEST_F(AProcessorNode, AddParameterChangeConnectionIncreasesTheConnectionSize) {
    ProcessorNode node(proc);
    ParameterChangeConnection connection{upstream_node, 0, 0};

    node.addUpstreamParameterChangeConnection(connection);

    ASSERT_THAT(node.getUpstreamParameterConnections().size(), Eq(1));
}

TEST_F(AProcessorNode, AddAudioConnectionThrowsIfUpstreamPortIndexInvalid) {
    ProcessorNode node(proc);
    int upstream_port_index = 1;
    AudioConnection connection{upstream_node, 1, 0};

    ASSERT_THAT(upstream_port_index,
                Ge(upstream_node->getAudioOutputPortSize()));
    ASSERT_ANY_THROW(node.addUpstreamAudioConnection(connection));
}

TEST_F(AProcessorNode, AddParameterChangeConnectionThrowsIfUpstreamPortIndexInvalid) {
    ProcessorNode node(proc);
    int upstream_port_index = 1;
    ParameterChangeConnection connection{upstream_node, 1, 0};

    ASSERT_THAT(upstream_port_index,
                Ge(upstream_node->getParameterChangeOutputPortSize()));
    ASSERT_ANY_THROW(node.addUpstreamParameterChangeConnection(connection));
}

TEST_F(AProcessorNode, AddAudioConnectionThrowsIfDownstreamPortIndexInvalid) {
    ProcessorNode node(proc);
    int downstream_port_index = 1;
    AudioConnection connection{upstream_node, 0, downstream_port_index};

    ASSERT_THAT(downstream_port_index, Ge(node.getAudioInputPortSize()));
    ASSERT_ANY_THROW(node.addUpstreamAudioConnection(connection));
}

TEST_F(AProcessorNode, AddParameterChangeConnectionThrowsIfDownstreamPortIndexInvalid) {
    ProcessorNode node(proc);
    int downstream_port_index = 1;
    ParameterChangeConnection connection{upstream_node, 0, downstream_port_index};

    ASSERT_THAT(downstream_port_index, Ge(node.getAudioInputPortSize()));
    ASSERT_ANY_THROW(node.addUpstreamParameterChangeConnection(connection));
}

TEST_F(AProcessorNode, AddAudioConnectionThrowsChannelsMismatch) {
    ProcessorNode node(proc, {1}, {2});
    AudioConnection connection{upstream_node, 0, 0};

    ASSERT_THAT(upstream_node->getAudioOutputPortChannels(0),
                Not(node.getAudioInputPortChannels(0)));
    ASSERT_ANY_THROW(node.addUpstreamAudioConnection(connection));
}

TEST_F(AProcessorNode, HasProcessReturnsFalseWhenInit) {
    ProcessorNode node(proc);

    ASSERT_FALSE(node.hasProcessed());
}

TEST_F(AProcessorNode, HasProcessReturnsTrueAfterPullAudioPort) {
    ProcessorNode node(proc);

    node.pullAudioPort(0);

    ASSERT_TRUE(node.hasProcessed());
}

TEST_F(AProcessorNode, PrepareForNextBlockResetTheProcessState) {
    ProcessorNode node(proc);
    node.pullAudioPort(0);
    ASSERT_TRUE(node.hasProcessed());

    node.prepareForNextBlock();

    ASSERT_FALSE(node.hasProcessed());
}

TEST_F(AProcessorNode, CanSetTransportContext) {
    auto trans_context = std::make_shared<TransportContext>();
    ProcessorNode node(proc);

    node.setTransportContext(trans_context);

    ASSERT_THAT(trans_context.use_count(), Eq(2));
}

TEST_F(AProcessorNode, PrepareForNextUpdateBlockContextFromTransportContext) {
    auto trans_context = std::make_shared<TransportContext>();
    ProcessorNode node(proc);
    node.setTransportContext(trans_context);

    trans_context->num_samples = 1;
    trans_context->sample_rate = 10;
    trans_context->is_playing = true;
    trans_context->is_final = false;
    trans_context->play_head_sample_index = 3;
    node.prepareForNextBlock();

    ASSERT_THAT(node.getInputBlock()->context.sample_rate, FloatEq(trans_context->sample_rate));
    ASSERT_THAT(node.getInputBlock()->context.num_samples, Eq(trans_context->num_samples.load()));
    ASSERT_THAT(node.getInputBlock()->context.is_playing, Eq(trans_context->is_playing.load()));
    ASSERT_THAT(node.getInputBlock()->context.is_final, Eq(trans_context->is_final.load()));
    ASSERT_THAT(node.getInputBlock()->context.play_head_sample_index, Eq(trans_context->play_head_sample_index.load()));

    ASSERT_THAT(node.getOutputBlock()->context.sample_rate, FloatEq(trans_context->sample_rate));
    ASSERT_THAT(node.getOutputBlock()->context.num_samples, Eq(trans_context->num_samples.load()));
    ASSERT_THAT(node.getOutputBlock()->context.is_playing, Eq(trans_context->is_playing.load()));
    ASSERT_THAT(node.getOutputBlock()->context.is_final, Eq(trans_context->is_final.load()));
    ASSERT_THAT(node.getOutputBlock()->context.play_head_sample_index, Eq(trans_context->play_head_sample_index.load()));
}

TEST_F(AProcessorNode, PullAudioPortWillPullUpstreamBlock) {
    ProcessorNode node(proc);
    AudioConnection connection{upstream_node, 0, 0};
    node.addUpstreamAudioConnection(connection);
    upstream_node->prepareToPlay(sr, max_block_size);
    node.prepareToPlay(sr, max_block_size);

    EXPECT_CALL(*up_proc, processBlock)
        .WillOnce([this](AudioBlock *input, AudioBlock *output) {
            output->buffer.copyFrom(&this->block.buffer,
                                    this->block.buffer.getNumberChannels(),
                                    this->block.buffer.getNumberFrames(), 0, 0);
        });

    EXPECT_CALL(*proc, processBlock)
        .WillOnce([](AudioBlock *input, AudioBlock *output) {
            output->buffer.copyFrom(&input->buffer);
        });

    AudioPort &port = node.pullAudioPort(0);

    ASSERT_THAT(port.getChannelData(0)[0], Eq(1));
    ASSERT_THAT(port.getChannelData(1)[0], Eq(2));
}

TEST_F(AProcessorNode, PullParameterChangePortWillPullUpstreamBlcok) {
    ProcessorNode node(proc);
    ParameterChangeConnection connection{upstream_node, 0, 0};
    node.addUpstreamParameterChangeConnection(connection);

    upstream_node->prepareToPlay(sr, max_block_size);
    node.prepareToPlay(sr, max_block_size);

    EXPECT_CALL(*up_proc, processBlock)
        .WillOnce([this](AudioBlock *input, AudioBlock *output) {
            output->buffer.copyFrom(&this->block.buffer,
                                    this->block.buffer.getNumberChannels(),
                                    this->block.buffer.getNumberFrames(), 0, 0);
            output->param_changes.push(0, {0, 1, 1});
        });

    EXPECT_CALL(*proc, processBlock)
        .WillOnce([](AudioBlock *input, AudioBlock *output) {
            output->buffer.copyFrom(&input->buffer);

            ParameterChangePoint result{};
            input->param_changes.pop(0, result);
            ASSERT_THAT(result.index, Eq(0));
            ASSERT_THAT(result.time, Eq(1));
            ASSERT_THAT(result.normalized_value, Eq(1));
        });

    ParameterChangePort &port = node.pullParameterChangePort(0);
}

TEST_F(AProcessorNode, PullAudioPortReturnsLastResultIfHasProcessed) {
    ProcessorNode node(proc);
    AudioConnection connection{upstream_node, 0, 0};
    node.addUpstreamAudioConnection(connection);
    upstream_node->prepareToPlay(sr, max_block_size);
    node.prepareToPlay(sr, max_block_size);

    EXPECT_CALL(*up_proc, processBlock).Times(1);
    EXPECT_CALL(*proc, processBlock).Times(1);

    AudioPort &port_first_time = node.pullAudioPort(0);
    AudioPort &port_second_time = node.pullAudioPort(0);

    ASSERT_THAT(port_first_time.getNumberChannels(),
                Eq(port_second_time.getNumberChannels()));
    ASSERT_THAT(port_first_time.getChannelData(0),
                Eq(port_second_time.getChannelData(0)));
}

TEST_F(AProcessorNode, AudioPortSynchronizeNumSamplesWhenPushPullMode) {
    ProcessorNode node(proc);
    AudioConnection connection{upstream_node, 0, 0};
    node.addUpstreamAudioConnection(connection);
    upstream_node->prepareToPlay(sr, max_block_size);
    node.prepareToPlay(sr, max_block_size);
    auto trans_context = std::make_shared<TransportContext>();
    trans_context->is_push_pull_mode = true;
    node.setTransportContext(trans_context);

    EXPECT_CALL(*up_proc, processBlock).WillOnce([this](AudioBlock *input, AudioBlock *output) {
        output->context.num_samples = 0.5 * max_block_size;
    });

    AudioPort &output_port = node.pullAudioPort(0);
    ASSERT_THAT(output_port.getNumberFrames(), Eq(0.5 * max_block_size));
}

TEST_F(AProcessorNode, PullParameterChangePortReturnsLastResultIfHasProcessed) {
    ProcessorNode node(proc);
    ParameterChangeConnection connection{upstream_node, 0, 0};
    node.addUpstreamParameterChangeConnection(connection);
    upstream_node->prepareToPlay(sr, max_block_size);
    node.prepareToPlay(sr, max_block_size);

    EXPECT_CALL(*up_proc, processBlock).Times(1);
    EXPECT_CALL(*proc, processBlock).Times(1);

    auto &port_first_time = node.pullParameterChangePort(0);
    auto &port_second_time = node.pullParameterChangePort(0);
}

TEST_F(AProcessorNode, NodeStateContainsNodeId) {
    auto node_id = "ABC";
    ProcessorNode node(proc);
    node.setNodeID(node_id);

    auto node_state = node.getState();
    auto node_json = nlohmann::json::parse(node_state);

    ASSERT_FALSE(node_json["node_id"].is_null());
    ASSERT_THAT(node_json["node_id"].get<std::string>(), Eq(node_id));
}

TEST_F(AProcessorNode, NodeStateContainsNodeTypeString) {
    ProcessorNode node(proc);

    auto node_json = JsonUtilities::binaryDataToJson(node.getState());

    ASSERT_FALSE(node_json["node_type"].is_null());
    ASSERT_THAT(node_json["node_type"].get<std::string>(), Eq("processor_node"));
}

TEST_F(AProcessorNode, NodeStateContainsInputChannels) {
    auto input_channels = {2, 1, 2};
    ProcessorNode node(proc, input_channels, {});

    auto node_state = node.getState();
    auto node_json = nlohmann::json::parse(node_state);

    auto expected_json = nlohmann::json::parse(R"(
        {
            "input_channels": [2,1,2]
        }
    )");
    ASSERT_FALSE(node_json["input_channels"].is_null());
    ASSERT_TRUE(node_json["input_channels"].is_array());
    ASSERT_THAT(node_json["input_channels"], Eq(expected_json["input_channels"]));
}

TEST_F(AProcessorNode, NodeStateContainsOutputChannels) {
    auto output_channels = {2, 1, 2};
    ProcessorNode node(proc, {}, output_channels);

    auto node_state = node.getState();
    auto node_json = nlohmann::json::parse(node_state);

    auto expected_json = nlohmann::json::parse(R"(
        {
            "output_channels": [2,1,2]
        }
    )");
    ASSERT_FALSE(node_json["output_channels"].is_null());
    ASSERT_TRUE(node_json["output_channels"].is_array());
    ASSERT_THAT(node_json["output_channels"], Eq(expected_json["output_channels"]));
}

TEST_F(AProcessorNode, NodeStateContainsProcessorState) {
    ProcessorNode node(proc);
    auto proc_state = makeMockProcessorState();
    auto proc_state_json = nlohmann::json::parse(proc_state);

    EXPECT_CALL(*proc, getState).WillOnce(Return(proc_state));

    auto node_state = node.getState();
    auto node_json = nlohmann::json::parse(node_state);

    ASSERT_FALSE(node_json["processor_state"].is_null());
    ASSERT_THAT(node_json["processor_state"], Eq(proc_state_json));
}

TEST_F(AProcessorNode, SetStateUpdatesNodeId) {
    auto node_id = "gain";
    ProcessorNode gain_node(gain_proc);
    gain_node.setNodeID(node_id);

    auto gain_node_state = gain_node.getState();

    ProcessorNode node(nullptr);
    node.setState(gain_node_state.data(), gain_node_state.size());

    ASSERT_THAT(node.getNodeID(), Eq(node_id));
}

TEST_F(AProcessorNode, SetStateRebuildInternalProcessor) {
    ProcessorNode node(proc);
    ProcessorNode gain_node(gain_proc);

    auto gain_node_state = gain_node.getState();
    node.setState(gain_node_state.data(), gain_node_state.size());

    ASSERT_THAT(node.getProcessor()->getName(), Eq(gain_proc->getName()));
}

TEST_F(AProcessorNode, SetStateRebuildInternalProcessorParameters) {
    gain_proc = std::make_shared<GainProcessor>(20);
    ProcessorNode node(proc);
    ProcessorNode gain_node(gain_proc);
    auto gain_node_state = gain_node.getState();
    auto gain_node_json = nlohmann::json::parse(gain_node_state);

    node.setState(gain_node_state.data(), gain_node_state.size());

    auto node_state = node.getState();
    auto node_state_json = nlohmann::json::parse(node_state);

    ASSERT_THAT(node_state_json["processor_state"], Eq(gain_node_json["processor_state"]));
}

TEST_F(AProcessorNode, SetStateRebuildInternalAudioInputPorts) {
    auto input_channels = {2, 1};
    ProcessorNode node(proc);
    ProcessorNode gain_node(gain_proc, input_channels, {});

    auto gain_node_state = gain_node.getState();
    node.setState(gain_node_state.data(), gain_node_state.size());

    ASSERT_THAT(node.getAudioInputPortSize(), Eq(input_channels.size()));
    ASSERT_THAT(node.getAudioInputPortChannels(0), Eq(2));
    ASSERT_THAT(node.getAudioInputPortChannels(1), Eq(1));
}

TEST_F(AProcessorNode, SetStateRebuildInternalAudioOutputPorts) {
    auto output_channels = {2, 1};
    ProcessorNode node(proc);
    ProcessorNode gain_node(gain_proc, {}, output_channels);

    auto gain_node_state = gain_node.getState();
    node.setState(gain_node_state.data(), gain_node_state.size());

    ASSERT_THAT(node.getAudioOutputPortSize(), Eq(output_channels.size()));
    ASSERT_THAT(node.getAudioOutputPortChannels(0), Eq(2));
    ASSERT_THAT(node.getAudioOutputPortChannels(1), Eq(1));
}

TEST_F(AProcessorNode, SetStateReallocateInternalInputBlock) {
    auto input_channels = {2, 1};
    ProcessorNode node(proc);
    ProcessorNode gain_node(gain_proc, input_channels, {});

    auto gain_node_state = gain_node.getState();
    node.setState(gain_node_state.data(), gain_node_state.size());

    ASSERT_THAT(node.getInputBlock()->buffer.getNumberChannels(), Eq(3));
}

TEST_F(AProcessorNode, SetStateReallocateInternalOutputBlock) {
    auto output_channels = {2, 1};
    ProcessorNode node(proc);
    ProcessorNode gain_node(gain_proc, {}, output_channels);

    auto gain_node_state = gain_node.getState();
    node.setState(gain_node_state.data(), gain_node_state.size());

    ASSERT_THAT(node.getOutputBlock()->buffer.getNumberChannels(), Eq(3));
}