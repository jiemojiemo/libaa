//
// Created by user on 5/16/22.
//
#include "libaa/graph/aa_realtime_graph_render.h"

namespace libaa {

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

RealtimeGraphRender::RealtimeGraphRender(const std::shared_ptr<INode> &graph)
    : graph_(buildGraphWithPorts(graph)),
      transport_context_(std::make_shared<TransportContext>()) {
}

void RealtimeGraphRender::prepareToPlay(float sample_rate, int max_block_size) {
    transport_context_->sample_rate = sample_rate;
    processing_context_.sample_rate = sample_rate;

    graph_->setTransportContext(transport_context_);
    graph_->prepareToPlay(sample_rate, max_block_size);

    prepareCallbackNodes(sample_rate, max_block_size);
}

AudioPort &RealtimeGraphRender::pull(int num_samples) {
    transport_context_->num_samples = num_samples;
    processing_context_.num_samples = num_samples;

    callbackNodesToPrepareForNextBlock();

    graph_->prepareForNextBlock();
    return graph_->pullAudioPort(0);
}

const std::shared_ptr<INode> &RealtimeGraphRender::getGraph() const {
    return graph_;
}

void RealtimeGraphRender::setAudioCallback(int port_index, const AudioCallback &callback) {
    auto stream_source_node = buildSourceCallbackNodeWithAudioCallback(callback);
    audio_callback_node_map_[port_index] = stream_source_node;

    AudioConnection audio_con{stream_source_node, 0, port_index};
    graph_->addUpstreamAudioConnection(audio_con);
}

void RealtimeGraphRender::setParameterChangeCallback(int port_index, const ParameterChangeCallback &callback) {
    auto stream_source_node = buildSourceCallbackNodeWithPCCallback(callback);
    pc_callback_node_map_[port_index] = stream_source_node;

    ParameterChangeConnection pc_con{stream_source_node, 0, port_index};
    graph_->addUpstreamParameterChangeConnection(pc_con);
}

RealtimeGraphRender::PlaybackState RealtimeGraphRender::getPlaybackState() const {
    return playback_state_;
}

void RealtimeGraphRender::play() {
    playback_state_ = PlaybackState::kPlaying;
}

void RealtimeGraphRender::stop() {
    playback_state_ = PlaybackState::kStopped;
}

const std::map<int, std::shared_ptr<INode>> &RealtimeGraphRender::getAudioCallbackNodeMap() const {
    return audio_callback_node_map_;
}

const std::map<int, std::shared_ptr<INode>> &RealtimeGraphRender::getParameterChangeCallbackNodeMap() const {
    return pc_callback_node_map_;
}

void RealtimeGraphRender::prepareCallbackNodes(float sample_rate, int max_block_size) {
    for (auto &n : audio_callback_node_map_) {
        n.second->prepareToPlay(sample_rate, max_block_size);
        n.second->setTransportContext(transport_context_);
    }
    for (auto &n : pc_callback_node_map_) {
        n.second->prepareToPlay(sample_rate, max_block_size);
        n.second->setTransportContext(transport_context_);
    }
}

void RealtimeGraphRender::callbackNodesToPrepareForNextBlock() {
    for (auto &n : audio_callback_node_map_) {
        n.second->prepareForNextBlock();
    }
    for (auto &n : pc_callback_node_map_) {
        n.second->prepareForNextBlock();
    }
}

std::shared_ptr<INode> RealtimeGraphRender::buildSourceCallbackNodeWithAudioCallback(const AudioCallback &callback) {
    auto source_callback_proc = std::make_shared<SourceCallbackProcessor>();
    source_callback_proc->setSourceCallback([this, callback](AudioBlock *block) {
        callback(&block->buffer, processing_context_);
    });
    return std::make_shared<ProcessorNode>(source_callback_proc);
};

std::shared_ptr<INode> RealtimeGraphRender::buildSourceCallbackNodeWithPCCallback(const ParameterChangeCallback &callback) {
    auto source_callback_proc = std::make_shared<SourceCallbackProcessor>();
    source_callback_proc->setSourceCallback([this, callback](AudioBlock *block) {
        callback(&block->param_changes, processing_context_);
    });
    return std::make_shared<ProcessorNode>(source_callback_proc);
};

} // namespace libaa