//
// Created by user on 5/16/22.
//

#ifndef LIBAA_AA_REALTIME_GRAPH_RENDER_H
#define LIBAA_AA_REALTIME_GRAPH_RENDER_H

#pragma once
#include "libaa/core/aa_audio_block.h"
#include "libaa/core/aa_processing_context.h"
#include "libaa/graph/aa_audio_processor_node.h"
#include "libaa/graph/aa_graph_builder.h"
#include "libaa/graph/aa_transport_context.h"
#include "libaa/processor/aa_processor_factory.h"
#include "libaa/processor/aa_source_callback_processor.h"

namespace libaa {

using AudioCallback = std::function<void(AudioBufferX<float> *buffer, ProcessingContext &context)>;
using ParameterChangeCallback = std::function<void(ParameterChanges *params, ProcessingContext &context)>;

class RealtimeGraphRender {
public:
    RealtimeGraphRender(const std::shared_ptr<INode> &graph);

    void prepareToPlay(float sample_rate, int max_block_size);

    AudioPort &pull(int num_samples);

    const std::shared_ptr<INode> &getGraph() const;

    void setAudioCallback(int port_index, const AudioCallback &callback);

    void setParameterChangeCallback(int port_index, const ParameterChangeCallback &callback);

    void pushParameterChange(int port_index, int param_index, float norm_val);

    enum PlaybackState {
        kStopped = 0,
        kPlaying
    };
    PlaybackState getPlaybackState() const;

    void play();

    void stop();

    const std::map<int, std::shared_ptr<INode>> &getAudioCallbackNodeMap() const;
    const std::map<int, std::shared_ptr<INode>> &getParameterChangeCallbackNodeMap() const;

private:
    void initAndConnectCallbackNodes();
    void prepareCallbackNodes(float sample_rate, int max_block_size);
    void callbackNodesToPrepareForNextBlock();
    std::shared_ptr<INode> buildSourceCallbackNodeWithAudioCallback(const AudioCallback &callback);
    std::shared_ptr<INode> buildSourceCallbackNodeWithPCCallback(const ParameterChangeCallback &callback);

    std::shared_ptr<INode> graph_{nullptr};
    std::shared_ptr<TransportContext> transport_context_{nullptr};
    ProcessingContext processing_context_;
    PlaybackState playback_state_{PlaybackState::kStopped};
    std::map<int, std::shared_ptr<INode>> audio_callback_node_map_;
    std::map<int, std::shared_ptr<INode>> pc_callback_node_map_;
};
} // namespace libaa

#endif // LIBAA_AA_REALTIME_GRAPH_RENDER_H
