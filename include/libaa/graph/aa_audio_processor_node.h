//
// Created by user on 5/6/22.
//

#ifndef LIBAA_AA_AUDIO_PROCESSOR_NODE_H
#define LIBAA_AA_AUDIO_PROCESSOR_NODE_H

#pragma once
#include "libaa/graph/aa_audio_connection.h"
#include "libaa/graph/aa_audio_port.h"
#include "libaa/graph/aa_i_node.h"
#include "libaa/processor/aa_audio_processor.h"
#include <atomic>
#include <cassert>
#include <numeric>

namespace libaa {

class ProcessorNode : public INode {
public:
    explicit ProcessorNode(std::shared_ptr<IAudioProcessor> proc);

    explicit ProcessorNode(std::shared_ptr<IAudioProcessor> proc,
                           const std::initializer_list<int> &input_channels,
                           const std::initializer_list<int> &output_channels);

    void setNodeID(std::string node_id) override;

    std::string getNodeID() const override;

    void prepareToPlay(float sample_rate, int max_block_size) override;

    void addUpstreamAudioConnection(const AudioConnection &c) override;

    AudioPort &pullAudioPort(int output_audio_port) override;

    bool hasProcessed() const override;

    void prepareForNextBlock() override;

    void addAudioInputPort(int num_in_channel);

    const std::vector<AudioPort> &getAudioInputPorts() const override;

    const std::vector<AudioPort> &getAudioOutputPorts() const override;

    const std::vector<AudioConnection> &getUpstreamAudioConnections() const;

    //** testing ***
    const AudioBlock *getInputBlock() const;

    const AudioBlock *getOutputBlock() const;

private:
    void
    initInputAndOutputBlock(const std::initializer_list<int> &input_channels,
                            const std::initializer_list<int> &output_channels);

    void initAudioPorts(const std::initializer_list<int> &input_channels,
                        const std::initializer_list<int> &output_channels);

    void checkConnectionValidAndThrow(const AudioConnection &c) const;

    std::shared_ptr<IAudioProcessor> proc_{nullptr};
    std::vector<AudioPort> input_audio_ports_;
    std::vector<AudioPort> output_audio_ports_;
    std::vector<AudioConnection> audio_connections_;
    std::shared_ptr<AudioBlock> input_block_{nullptr};
    std::shared_ptr<AudioBlock> output_block_{nullptr};
    std::string node_id_{};
    std::atomic<bool> has_processed_{false};
};
} // namespace libaa

#endif // LIBAA_AA_AUDIO_PROCESSOR_NODE_H