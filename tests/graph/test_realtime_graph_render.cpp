//
// Created by user on 5/14/22.
//
#include "libaa/core/aa_json_utilities.h"
#include "libaa/graph/aa_realtime_graph_render.h"
#include "libaa/processor/aa_source_callback_processor.h"
#include "libaa_testing/aa_mock_node.h"
#include <gmock/gmock.h>

using namespace testing;
using namespace libaa;
class ARealtimeGraphRender : public Test {
public:
    std::shared_ptr<INode> buildTestGraph() {
        GraphBuilder builder;
        auto source_proc = std::shared_ptr<IAudioProcessor>(new SourceProcessor({{1, 1, 1, 1, 1}, {2, 2, 2, 2, 2}}));
        std::shared_ptr<IAudioProcessor> gain_proc = ProcessorFactory::create("Gain");
        std::shared_ptr<IAudioProcessor> delay_proc = ProcessorFactory::create("Delay");
        auto source_node = std::make_shared<ProcessorNode>(source_proc);
        auto gain_node = std::make_shared<ProcessorNode>(gain_proc);
        auto delay_node = std::make_shared<ProcessorNode>(delay_proc);

        builder.insert("source_node", source_node);
        builder.insert("gain_node", gain_node);
        builder.insert("delay_node", delay_node);
        builder.addConnection(ConnectionType::kAudioConnection, {source_node, 0}, {gain_node, 0});
        builder.addConnection(ConnectionType::kAudioConnection, {gain_node, 0}, {delay_node, 0});

        builder.exposePort(PortDirection::kInput, PortType::kAudio, 0, {gain_node, 0});
        builder.exposePort(PortDirection::kInput, PortType::kAudio, 1, {delay_node, 0});

        builder.exposePort(PortDirection::kInput, PortType::kParameterChange, 0, {gain_node, 0});
        builder.exposePort(PortDirection::kInput, PortType::kParameterChange, 1, {delay_node, 0});

        builder.exposePort(PortDirection::kOutput, PortType::kAudio, 0, {gain_node, 0});

        builder.exposePort(PortDirection::kOutput, PortType::kParameterChange, 0, {gain_node, 0});

        return builder.build();
    }

    std::string getProcessorNameFromNode(const std::shared_ptr<INode> &node) {
        auto upstream_proc = dynamic_cast<ProcessorNode *>(node.get())->getProcessor();
        return upstream_proc->getName();
    }

    auto isSilence(AudioPort &port) {
        for (int c = 0; c < port.getNumberChannels(); ++c) {
            float *data = port.getChannelData(c);
            for (int i = 0; i < port.getNumberFrames(); ++i) {
                if (data[i] != 0.0f)
                    return false;
            }
        }
        return true;
    };

    auto getSourceCallbackProcessorFromNode(std::shared_ptr<INode> node) {
        auto *proc_node = dynamic_cast<ProcessorNode *>(node.get());
        return dynamic_cast<SourceCallbackProcessor *>(proc_node->getProcessor());
    };

    std::shared_ptr<INode> graph = buildTestGraph();
    float sample_rate = 44100;
    int max_block_size = 128;
};

TEST_F(ARealtimeGraphRender, InitWillCreateAnotherInternalGraphToUse) {
    RealtimeGraphRender render(graph);

    auto graph_json = JsonUtilities::binaryDataToJson(graph->getState());
    auto internal_graph_json = JsonUtilities::binaryDataToJson(render.getGraph()->getState());

    ASSERT_THAT(graph_json, Not(internal_graph_json));
}

TEST_F(ARealtimeGraphRender, InternalGraphHasSameNumberExposedAudioInputPort) {
    RealtimeGraphRender render(graph);

    ASSERT_THAT(render.getGraph()->getAudioInputPortSize(), Eq(graph->getAudioInputPortSize()));
}

TEST_F(ARealtimeGraphRender, InternalGraphHasSameNumberExposedAudioOutputPort) {
    RealtimeGraphRender render(graph);

    ASSERT_THAT(render.getGraph()->getAudioOutputPortSize(), Eq(graph->getAudioOutputPortSize()));
}

TEST_F(ARealtimeGraphRender, InternalGraphHasSameNumberExposedParameterChangeInputPort) {
    RealtimeGraphRender render(graph);

    ASSERT_THAT(render.getGraph()->getParameterChangeInputPortSize(), Eq(graph->getParameterChangeInputPortSize()));
}

TEST_F(ARealtimeGraphRender, InternalGraphHasSameNumberExposedParameterChangeOutputPort) {
    RealtimeGraphRender render(graph);

    ASSERT_THAT(render.getGraph()->getParameterChangeOutputPortSize(), Eq(graph->getParameterChangeOutputPortSize()));
}

TEST_F(ARealtimeGraphRender, AudioCallbackNodeCreatesWhenInit) {
    RealtimeGraphRender render(graph);

    ASSERT_THAT(render.getAudioCallbackNodeMap().size(), Eq(graph->getAudioInputPortSize()));
}

TEST_F(ARealtimeGraphRender, ParameterChangeCallbackNodeCreatesWhenInit) {
    RealtimeGraphRender render(graph);

    ASSERT_THAT(render.getParameterChangeCallbackNodeMap().size(), Eq(graph->getParameterChangeInputPortSize()));
}

TEST_F(ARealtimeGraphRender, AudioCallbackNodeConnectionWithInternalGraph) {
    RealtimeGraphRender render(graph);

    ASSERT_THAT(graph->getUpstreamAudioConnections().size(), Eq(graph->getAudioInputPortSize()));
    ASSERT_THAT(graph->getUpstreamAudioConnections()[0].upstream_node->getNodeType(), Eq(NodeType::kProcessorNode));
    ASSERT_THAT(getProcessorNameFromNode(graph->getUpstreamAudioConnections()[0].upstream_node), Eq("Source Callback"));
}

TEST_F(ARealtimeGraphRender, ParameterChangeCallbackNodeConnectionWithInternalGraph) {
    RealtimeGraphRender render(graph);

    ASSERT_THAT(graph->getUpstreamParameterConnections().size(), Eq(graph->getParameterChangeInputPortSize()));
    ASSERT_THAT(graph->getUpstreamAudioConnections()[0].upstream_node->getNodeType(), Eq(NodeType::kProcessorNode));
    ASSERT_THAT(getProcessorNameFromNode(graph->getUpstreamAudioConnections()[0].upstream_node), Eq("Source Callback"));
}

TEST_F(ARealtimeGraphRender, CanSetAudioCallback) {
    auto graph = buildTestGraph();
    RealtimeGraphRender render(graph);

    auto mock_audio_callback = [](AudioBufferX<float> *buffer, ProcessingContext &) {};
    render.setAudioCallback(0, mock_audio_callback);

    auto *source_callback_proc = getSourceCallbackProcessorFromNode(render.getAudioCallbackNodeMap().at(0));
    ASSERT_TRUE(source_callback_proc->getSourceCallback() != nullptr);
}

TEST_F(ARealtimeGraphRender, CanSetParameterChangeCallback) {
    auto graph = buildTestGraph();
    RealtimeGraphRender render(graph);

    auto mock_pc_callback = [](ParameterChanges *params, ProcessingContext &) {};
    render.setParameterChangeCallback(0, mock_pc_callback);

    auto *source_callback_proc = getSourceCallbackProcessorFromNode(render.getParameterChangeCallbackNodeMap().at(0));
    ASSERT_TRUE(source_callback_proc->getSourceCallback() != nullptr);
}

TEST_F(ARealtimeGraphRender, PrepareWillAlsoPrepareSourceStreamNode) {
    int port_index = 0;
    auto graph = buildTestGraph();
    RealtimeGraphRender render(graph);
    auto mock_pc_callback = [](ParameterChanges *params, ProcessingContext &) {};
    auto mock_audio_callback = [](AudioBufferX<float> *buffer, ProcessingContext &) {};
    render.setAudioCallback(port_index, mock_audio_callback);
    render.setParameterChangeCallback(port_index, mock_pc_callback);

    render.prepareToPlay(sample_rate, max_block_size);

    auto internal_audio_stream_node = dynamic_cast<ProcessorNode *>(render.getAudioCallbackNodeMap().at(port_index).get());
    auto internal_pc_stream_node = dynamic_cast<ProcessorNode *>(render.getAudioCallbackNodeMap().at(port_index).get());

    ASSERT_THAT(internal_audio_stream_node->getInputBlock()->buffer.getNumberFrames(), Eq(max_block_size));
    ASSERT_THAT(internal_audio_stream_node->getOutputBlock()->buffer.getNumberFrames(), Eq(max_block_size));
    ASSERT_THAT(internal_pc_stream_node->getInputBlock()->buffer.getNumberFrames(), Eq(max_block_size));
    ASSERT_THAT(internal_pc_stream_node->getOutputBlock()->buffer.getNumberFrames(), Eq(max_block_size));
}

TEST_F(ARealtimeGraphRender, DefaultStateIsStoped) {
    auto graph = buildTestGraph();
    RealtimeGraphRender render(graph);

    ASSERT_THAT(render.getPlaybackState(), Eq(RealtimeGraphRender::kStopped));
}

TEST_F(ARealtimeGraphRender, StateChangeToPlayingAfterCallPlay) {
    auto graph = buildTestGraph();
    RealtimeGraphRender render(graph);

    render.play();

    ASSERT_THAT(render.getPlaybackState(), Eq(RealtimeGraphRender::kPlaying));
}

TEST_F(ARealtimeGraphRender, StateChangeToStopAfterCallStop) {
    auto graph = buildTestGraph();
    RealtimeGraphRender render(graph);

    render.play();
    render.stop();

    ASSERT_THAT(render.getPlaybackState(), Eq(RealtimeGraphRender::kStopped));
}

TEST_F(ARealtimeGraphRender, PrepareWillPrepareNode) {
    auto mock_graph = std::make_shared<MockNode>();
    RealtimeGraphRender render(mock_graph);

    EXPECT_CALL(*mock_graph, prepareToPlay(sample_rate, max_block_size)).Times(1);

    render.prepareToPlay(sample_rate, max_block_size);
}

TEST_F(ARealtimeGraphRender, CanPullNumSamples) {
    int num_samples = 3;
    auto graph = buildTestGraph();
    RealtimeGraphRender render(graph);
    render.prepareToPlay(sample_rate, max_block_size);

    AudioPort &output = render.pull(num_samples);

    ASSERT_THAT(output.getNumberFrames(), Eq(num_samples));
}

TEST_F(ARealtimeGraphRender, CanPushParameterChange) {
    int num_samples = 3;
    auto graph = buildTestGraph();
    RealtimeGraphRender render(graph);

    render.prepareToPlay(sample_rate, max_block_size);
    render.pushParameterChange(0, 0, 1.0f); // "Gain dB" parameter changed

    AudioPort &output = render.pull(num_samples);
    auto *graph_node = dynamic_cast<GraphNode *>(graph.get());
    auto gain_node = dynamic_cast<ProcessorNode *>(graph_node->getAllNodes()[1].get());

    ASSERT_THAT(gain_node->getProcessor()->getParameters()->get(0).getNormalizedValue(), Eq(1.0f));
}

TEST_F(ARealtimeGraphRender, PullWillIncreaseTransportPlayHeadSample) {
    int num_samples = 3;
    auto graph = buildTestGraph();
    RealtimeGraphRender render(graph);
    render.prepareToPlay(sample_rate, max_block_size);
    ASSERT_THAT(render.getTransportContext()->play_head_sample_index.load(), Eq(0));

    render.pull(10);
    ASSERT_THAT(render.getTransportContext()->play_head_sample_index.load(), Eq(10));
}