//
// Created by user on 5/6/22.
//

#include "libaa/graph/aa_audio_processor_node.h"

namespace libaa {

auto getTotalChannels(const std::initializer_list<int> &input_channels) {
    return std::accumulate(input_channels.begin(), input_channels.end(), 0);
}

ProcessorNode::ProcessorNode(std::shared_ptr<IAudioProcessor> proc)
    : ProcessorNode(std::move(proc), {2}, {2}) {}

ProcessorNode::ProcessorNode(std::shared_ptr<IAudioProcessor> proc,
                             const std::initializer_list<int> &input_channels,
                             const std::initializer_list<int> &output_channels)
    : proc_(std::move(proc)) {
    initInputAndOutputBlock(input_channels, output_channels);
}

void ProcessorNode::setNodeID(std::string node_id) {
    node_id_ = std::move(node_id);
}
std::string ProcessorNode::getNodeID() const {
    return node_id_;
}

void ProcessorNode::prepareToPlay(float sample_rate, int max_block_size) {
    input_block_->buffer.resizeFrames(max_block_size);
    output_block_->buffer.resizeFrames(max_block_size);

    proc_->prepareToPlay(sample_rate, max_block_size);
}

void ProcessorNode::addUpstreamAudioConnection(const AudioConnection &c) {
    checkConnectionValidAndThrow(c);

    audio_connections_.push_back(c);
}
AudioPort &ProcessorNode::pullAudioPort(int output_audio_port) {
    if (!hasProcessed()) {
        has_processed_.store(true);

        // pull upstream port
        for (auto i = 0u; i < audio_connections_.size(); ++i) {
            const AudioPort &upstream_audio_port = audio_connections_[i].pull();

            // write upstream audio buffer to input_block
            auto downstream_port_index =
                audio_connections_[i].downstream_audio_port_index;
            auto &downstream_port = input_audio_ports_[downstream_port_index];

            downstream_port.copyFrom(&upstream_audio_port);
        }

        // processor process input_block to output_block
        proc_->processBlock(input_block_.get(), output_block_.get());
    }

    return output_audio_ports_.at(output_audio_port);
}

bool ProcessorNode::hasProcessed() const {
    return has_processed_.load();
}
void ProcessorNode::prepareForNextBlock() {
    has_processed_.store(false);
}

void ProcessorNode::addAudioInputPort(int num_in_channel) {
    auto old_channel_size = input_block_->buffer.getNumberChannels();
    auto new_channel_size = old_channel_size + num_in_channel;
    input_block_->buffer.resize(new_channel_size, 0);
    input_audio_ports_.emplace_back(input_block_, num_in_channel,
                                    old_channel_size);
}

const std::vector<AudioPort> &ProcessorNode::getAudioInputPorts() const {
    return input_audio_ports_;
}

const std::vector<AudioPort> &ProcessorNode::getAudioOutputPorts() const {
    return output_audio_ports_;
}

const std::vector<AudioConnection> &
ProcessorNode::getUpstreamAudioConnections() const {
    return audio_connections_;
}

const AudioBlock *ProcessorNode::getInputBlock() const {
    return input_block_.get();
}

const AudioBlock *ProcessorNode::getOutputBlock() const {
    return output_block_.get();
}

void ProcessorNode::initInputAndOutputBlock(
    const std::initializer_list<int> &input_channels,
    const std::initializer_list<int> &output_channels) {
    auto total_input_channels = getTotalChannels(input_channels);
    auto total_output_channels = getTotalChannels(output_channels);
    auto num_params = (proc_->getParameters() == nullptr)
                          ? 0
                          : proc_->getParameters()->size();

    const int init_num_frames = 0;
    input_block_ = std::make_shared<AudioBlock>(total_input_channels,
                                                init_num_frames, num_params);
    output_block_ = std::make_shared<AudioBlock>(total_output_channels,
                                                 init_num_frames, num_params);

    initAudioPorts(input_channels, output_channels);
}

void ProcessorNode::initAudioPorts(
    const std::initializer_list<int> &input_channels,
    const std::initializer_list<int> &output_channels) {
    size_t input_channel_offset = 0;
    for (auto c : input_channels) {
        input_audio_ports_.emplace_back(input_block_, c, input_channel_offset);
        input_channel_offset += c;
    }

    size_t output_channel_offset = 0;
    for (auto c : output_channels) {
        output_audio_ports_.emplace_back(output_block_, c,
                                         output_channel_offset);
        output_channel_offset += c;
    }
}

void ProcessorNode::checkConnectionValidAndThrow(
    const AudioConnection &c) const {
    const auto &upstream_audio_output_ports =
        c.upstream_node->getAudioOutputPorts();
    const auto &downstream_audio_input_ports = getAudioInputPorts();

    auto upstream_output_port_size = upstream_audio_output_ports.size();
    if (c.upstream_audio_port_index >=
        static_cast<int>(upstream_output_port_size)) {
        throw std::invalid_argument(
            "invalid connection: upstream port output of range");
    }

    auto downstream_input_port_size = downstream_audio_input_ports.size();
    if (c.downstream_audio_port_index >=
        static_cast<int>(downstream_input_port_size)) {
        throw std::invalid_argument(
            "invalid connection: downstream port output of range");
    }

    auto upstream_port_num_channels =
        upstream_audio_output_ports[c.upstream_audio_port_index]
            .getNumberChannels();
    auto downstream_port_num_channels =
        downstream_audio_input_ports[c.downstream_audio_port_index]
            .getNumberChannels();
    if (upstream_port_num_channels != downstream_port_num_channels) {
        throw std::invalid_argument(
            "invalid connection: number of channels mismatch");
    }
}

} // namespace libaa