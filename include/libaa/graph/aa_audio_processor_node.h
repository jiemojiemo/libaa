//
// Created by user on 5/6/22.
//

#ifndef LIBAA_AA_AUDIO_PROCESSOR_NODE_H
#define LIBAA_AA_AUDIO_PROCESSOR_NODE_H

#pragma once
#include "libaa/graph/aa_audio_connection.h"
#include "libaa/graph/aa_audio_port.h"
#include "libaa/graph/aa_base_node.h"
#include "libaa/graph/aa_parameter_change_connection.h"
#include "libaa/graph/aa_parameter_change_port.h"
#include "libaa/processor/aa_audio_processor.h"
#include <atomic>
#include <cassert>
#include <numeric>

namespace libaa {

class ProcessorNode : public BaseNode {
public:
    ~ProcessorNode() override = default;

    explicit ProcessorNode(std::shared_ptr<IAudioProcessor> proc);

    explicit ProcessorNode(std::shared_ptr<IAudioProcessor> proc,
                           const std::initializer_list<int> &input_channels,
                           const std::initializer_list<int> &output_channels);

    void prepareToPlay(float sample_rate, int max_block_size) override;

    void addUpstreamAudioConnection(const AudioConnection &c) override;

    void addUpstreamParameterChangeConnection(const ParameterChangeConnection &c) override;

    AudioPort &pullAudioPort(int output_audio_port) override;

    ParameterChangePort &pullParameterChangePort(int output_pc_port) override;

    bool hasProcessed() const override;

    void setProcessedState(bool is_processed) override;

    void prepareForNextBlock() override;

    void addAudioInputPort(int num_in_channel);

    int getAudioInputPortSize() const override;
    int getAudioOutputPortSize() const override;
    int getAudioInputPortChannels(int port_index) const override;
    int getAudioOutputPortChannels(int port_index) const override;
    int getParameterChangeInputPortSize() const override;
    int getParameterChangeOutputPortSize() const override;

    void setState(uint8_t *state, size_t size) override;
    std::vector<uint8_t> getState() const override;

    //** testing ***
    const AudioBlock *getInputBlock() const;
    const AudioBlock *getOutputBlock() const;

private:
    void
    initBlocksAndPorts(const std::initializer_list<int> &input_channels,
                       const std::initializer_list<int> &output_channels);

    void initAudioPorts(const std::initializer_list<int> &input_channels,
                        const std::initializer_list<int> &output_channels);

    void checkAudioConnectionValidAndThrow(const AudioConnection &c) const;
    void checkParameterConnectionValidAndThrow(const ParameterChangeConnection &c) const;

    void pullUpstreamDataAndProcess();
    void pullUpstreamAudio();
    void pullUpstreamParameterChange();

    std::shared_ptr<IAudioProcessor> proc_{nullptr};
    std::vector<AudioPort> input_audio_ports_;
    std::vector<AudioPort> output_audio_ports_;
    std::vector<ParameterChangePort> input_pc_ports_;
    std::vector<ParameterChangePort> output_pc_ports_;
    std::shared_ptr<AudioBlock> input_block_{nullptr};
    std::shared_ptr<AudioBlock> output_block_{nullptr};
    std::atomic<bool> has_processed_{false};
};
} // namespace libaa

#endif // LIBAA_AA_AUDIO_PROCESSOR_NODE_H
