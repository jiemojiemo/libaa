//
// Created by user on 5/14/22.
//
#include "libaa/core/aa_audio_block.h"
#include "libaa/graph/aa_audio_processor_node.h"
#include "libaa/processor/aa_processor_factory.h"
#include "libaa/processor/aa_source_callback_processor.h"

#include "libaa/graph/aa_graph_builder.h"
#include "libaa/graph/aa_node_serialization_utilities.h"
#include "libaa_testing/aa_mock_node.h"
#include <gmock/gmock.h>

using namespace testing;

namespace libaa {

using AudioCallback = std::function<void(AudioBufferX<float> *buffer)>;
using ParameterChangeCallback = std::function<void(ParameterChanges *params)>;

auto buildSourceCallbackNodeWithAudioCallback = [](const AudioCallback &callback) {
    auto source_callback_proc = std::make_shared<SourceCallbackProcessor>();
    source_callback_proc->setSourceCallback([callback](AudioBlock *block) {
        callback(&block->buffer);
    });
    return std::make_shared<ProcessorNode>(source_callback_proc);
};

auto buildSourceCallbackNodeWithPCCallback = [](const ParameterChangeCallback &callback) {
    auto source_callback_proc = std::make_shared<SourceCallbackProcessor>();
    source_callback_proc->setSourceCallback([callback](AudioBlock *block) {
        callback(&block->param_changes);
    });
    return std::make_shared<ProcessorNode>(source_callback_proc);
};

auto buildGraphWithPorts(const std::shared_ptr<INode> &graph) {
    GraphBuilder builder;
    builder.insert("process_graph", graph);

    int num_in_audio_ports = graph->getAudioInputPortSize();
    for (int i = 0; i < num_in_audio_ports; ++i) {
        builder.exposePort(PortDirection::kInput, PortType::kAudio, i, {graph, i});
    }

    int num_out_audio_ports = graph->getAudioOutputPortSize();
    for (int i = 0; i < num_out_audio_ports; ++i) {
        builder.exposePort(PortDirection::kOutput, PortType::kAudio, i, {graph, i});
    }

    int num_in_pc_ports = graph->getParameterChangeInputPortSize();
    for (int i = 0; i < num_in_pc_ports; ++i) {
        builder.exposePort(PortDirection::kInput, PortType::kParameterChange, i, {graph, i});
    }

    int num_out_pc_ports = graph->getParameterChangeOutputPortSize();
    for (int i = 0; i < num_out_pc_ports; ++i) {
        builder.exposePort(PortDirection::kOutput, PortType::kParameterChange, i, {graph, i});
    }

    return builder.build();
}

class RealtimeGraphRender {
public:
    RealtimeGraphRender(const std::shared_ptr<INode> &graph)
        : graph_(buildGraphWithPorts(graph)) {
    }

    const std::shared_ptr<INode> &getGraph() const {
        return graph_;
    }

    void setAudioCallback(int port_index, const AudioCallback &callback) {
        AudioConnection audio_con{buildSourceCallbackNodeWithAudioCallback(callback), 0, port_index};
        graph_->addUpstreamAudioConnection(audio_con);
    }

    void setParameterChangeCallback(int port_index, const ParameterChangeCallback &callback) {
        ParameterChangeConnection pc_con{buildSourceCallbackNodeWithPCCallback(callback), 0, port_index};
        graph_->addUpstreamParameterChangeConnection(pc_con);
    }

    enum PlaybackState {
        kStopped = 0,
        kPlaying
    };
    PlaybackState getPlaybackState() const {
        return playback_state_;
    }

    void play() {
        playback_state_ = PlaybackState::kPlaying;
    }

    void stop() {
        playback_state_ = PlaybackState::kStopped;
    }

private:
    std::shared_ptr<INode> graph_{nullptr};
    PlaybackState playback_state_{PlaybackState::kStopped};
};
} // namespace libaa

using namespace libaa;
class ARealtimeGraphRender : public Test {
public:
    std::shared_ptr<INode> buildTestGraph() {
        GraphBuilder builder;
        auto gain_proc = ProcessorFactory::create("Gain");
        auto delay_proc = ProcessorFactory::create("Delay");
        auto gain_node = std::make_shared<ProcessorNode>(gain_proc);
        auto delay_node = std::make_shared<ProcessorNode>(delay_proc);

        builder.insert("gain_node", gain_node);
        builder.insert("delay_node", delay_node);
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

    std::shared_ptr<INode> graph = buildTestGraph();
};

TEST_F(ARealtimeGraphRender, InitWillCreateAnotherInternalGraphToUse) {
    RealtimeGraphRender render(graph);

    auto graph_json = NodeSerializationUtilities::binaryDataToJson(graph->getState());
    auto internal_graph_json = NodeSerializationUtilities::binaryDataToJson(render.getGraph()->getState());

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

TEST_F(ARealtimeGraphRender, SetAudioCallbackWillConnectSourceStreamNodeToInternalNode) {
    auto graph = buildTestGraph();
    RealtimeGraphRender render(graph);

    auto mock_audio_callback = [](AudioBufferX<float> *buffer) {};
    render.setAudioCallback(0, mock_audio_callback);

    ASSERT_THAT(graph->getUpstreamAudioConnections().size(), Eq(1));
    ASSERT_THAT(graph->getUpstreamAudioConnections()[0].upstream_node->getNodeType(), Eq(NodeType::kProcessorNode));
    ASSERT_THAT(getProcessorNameFromNode(graph->getUpstreamAudioConnections()[0].upstream_node), Eq("Source Callback"));
}

TEST_F(ARealtimeGraphRender, SetParameterChangeCallbackWillConnectSourceStreamNodeToInternalNode) {
    auto graph = buildTestGraph();
    RealtimeGraphRender render(graph);

    auto mock_pc_callback = [](ParameterChanges *params) {};
    render.setParameterChangeCallback(0, mock_pc_callback);

    ASSERT_THAT(graph->getUpstreamParameterConnections().size(), Eq(1));
    ASSERT_THAT(graph->getUpstreamParameterConnections()[0].upstream_node->getNodeType(), Eq(NodeType::kProcessorNode));
    ASSERT_THAT(getProcessorNameFromNode(graph->getUpstreamParameterConnections()[0].upstream_node), Eq("Source Callback"));
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

// TEST_F(ARealtimeGraphRender, )

//
// TEST_F(ARealtimeGraphRender, XXX) {
//    auto graph = buildTestGraph();
//    RealtimeGraphRender render(graph);
//
//    //    auto a_json = NodeSerializationUtilities::binaryDataToJson(graph->getState());
//
//    //    std::cout << a_json << std::endl;
//
//    auto mock_audio_callback = [](AudioBufferX<float> *buffer) {};
//    render.setAudioCallback(0, mock_audio_callback);
//
//    auto b_json = NodeSerializationUtilities::binaryDataToJson(graph->getState());
//
//    std::cout << b_json << std::endl;
//
//    //    ASSERT_THAT(a_json, Eq(b_json));
//}