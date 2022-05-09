//
// Created by user on 5/6/22.
//

#include "libaa/graph/aa_audio_processor_node.h"
#include "libaa/graph/aa_parameter_change_port.h"

namespace libaa {

auto getTotalChannels(const std::initializer_list<int> &input_channels) {
    return std::accumulate(input_channels.begin(), input_channels.end(), 0);
}

auto getNumberOfParameters(const std::shared_ptr<IAudioProcessor> &proc) {
    size_t num_params = 0;
    if (proc != nullptr) {
        num_params = (proc->getParameters() == nullptr)
                         ? 0
                         : proc->getParameters()->size();
    }

    return num_params;
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
ParameterChangePort &ProcessorNode::pullParameterChangePort(int output_pc_port) {
    return output_pc_ports_.at(output_pc_port);
}

bool ProcessorNode::hasProcessed() const {
    return has_processed_.load();
}

void ProcessorNode::setProcessedState(bool is_processed) {
    has_processed_.store(is_processed);
}

void ProcessorNode::prepareForNextBlock() {
    setProcessedState(false);
}

void ProcessorNode::addAudioInputPort(int num_in_channel) {
    auto old_channel_size = input_block_->buffer.getNumberChannels();
    auto new_channel_size = old_channel_size + num_in_channel;
    input_block_->buffer.resize(new_channel_size, 0);
    input_audio_ports_.emplace_back(input_block_, num_in_channel,
                                    old_channel_size);
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
    auto num_params = getNumberOfParameters(proc_);

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

    auto upstream_output_port_size = c.upstream_node->getAudioOutputPortSize();
    if (c.upstream_audio_port_index >=
        static_cast<int>(upstream_output_port_size)) {
        throw std::invalid_argument(
            "invalid connection: upstream port output of range");
    }

    auto downstream_input_port_size = getAudioInputPortSize();
    if (c.downstream_audio_port_index >=
        static_cast<int>(downstream_input_port_size)) {
        throw std::invalid_argument(
            "invalid connection: downstream port output of range");
    }

    auto upstream_port_num_channels =
        c.upstream_node->getAudioOutputPortChannels(
            c.upstream_audio_port_index);
    auto downstream_port_num_channels =
        getAudioInputPortChannels(c.downstream_audio_port_index);
    if (upstream_port_num_channels != downstream_port_num_channels) {
        throw std::invalid_argument(
            "invalid connection: number of channels mismatch");
    }
}
int ProcessorNode::getAudioInputPortSize() const {
    return input_audio_ports_.size();
}
int ProcessorNode::getAudioOutputPortSize() const {
    return output_audio_ports_.size();
}
int ProcessorNode::getAudioInputPortChannels(int port_index) const {
    return input_audio_ports_.at(port_index).getNumberChannels();
}
int ProcessorNode::getAudioOutputPortChannels(int port_index) const {
    return output_audio_ports_.at(port_index).getNumberChannels();
}

} // namespace libaa
