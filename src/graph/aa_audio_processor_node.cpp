//
// Created by user on 5/6/22.
//

#include "libaa/graph/aa_audio_processor_node.h"
#include "libaa/graph/aa_node_serialization_utilities.h"
#include "libaa/graph/aa_parameter_change_port.h"
#include "libaa/processor/aa_processor_factory.h"
#include <nlohmann/json.hpp>
namespace libaa {

auto getTotalChannels(const std::vector<int> &input_channels) {
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

auto getChannelsFromPorts(const std::vector<AudioPort> &audio_ports) {
    std::vector<int> channels;
    for (const auto &audio_port : audio_ports) {
        channels.push_back(audio_port.getNumberChannels());
    }
    return channels;
}

auto jsonToBinaryData(const nlohmann::json &j) {
    return NodeSerializationUtilities::jsonToBinaryData(j);
}

auto createProcessorWithState(const nlohmann::json &node_state_json) {
    auto processor_name = node_state_json["processor_state"]["processor_name"];
    auto proc = ProcessorFactory::create(processor_name);
    auto processor_state = jsonToBinaryData(node_state_json["processor_state"]);
    proc->setState(processor_state.data(), processor_state.size());

    return proc;
}

ProcessorNode::ProcessorNode(std::shared_ptr<IAudioProcessor> proc)
    : ProcessorNode(std::move(proc), {2}, {2}) {}

ProcessorNode::ProcessorNode(std::shared_ptr<IAudioProcessor> proc,
                             const std::initializer_list<int> &input_channels,
                             const std::initializer_list<int> &output_channels)
    : proc_(std::move(proc)) {
    initBlocksAndPorts(input_channels, output_channels);
}

void ProcessorNode::prepareToPlay(float sample_rate, int max_block_size) {
    input_block_->buffer.resizeFrames(max_block_size);
    output_block_->buffer.resizeFrames(max_block_size);

    proc_->prepareToPlay(sample_rate, max_block_size);
}

void ProcessorNode::addUpstreamAudioConnection(const AudioConnection &c) {
    checkAudioConnectionValidAndThrow(c);

    audio_connections_.push_back(c);
}

void ProcessorNode::addUpstreamParameterChangeConnection(const ParameterChangeConnection &c) {
    checkParameterConnectionValidAndThrow(c);

    param_change_connections_.push_back(c);
}

void ProcessorNode::pullUpstreamDataAndProcess() {
    if (!hasProcessed()) {
        pullUpstreamAudio();
        pullUpstreamParameterChange();

        proc_->processBlock(input_block_.get(), output_block_.get());

        has_processed_.store(true);
    }
}

void ProcessorNode::pullUpstreamAudio() {
    for (auto i = 0u; i < audio_connections_.size(); ++i) {
        const AudioPort &upstream_audio_port = audio_connections_[i].pull();

        // write upstream audio buffer to input_block
        auto downstream_port_index =
            audio_connections_[i].downstream_port_index;
        auto &downstream_port = input_audio_ports_[downstream_port_index];

        downstream_port.copyFrom(&upstream_audio_port);
    }
}
void ProcessorNode::pullUpstreamParameterChange() {
    for (auto i = 0u; i < param_change_connections_.size(); ++i) {
        ParameterChangePort &upstream_pc_port = param_change_connections_[i].pull();
        auto downstream_port_index = param_change_connections_[i].downstream_port_index;
        auto &downstream_port = input_pc_ports_[downstream_port_index];

        size_t num_params = upstream_pc_port.getParameterChanges().getNumParameters();
        ParameterChangePoint pc_point{};
        for (size_t ii = 0; ii < num_params; ++ii) {
            bool ok = upstream_pc_port.getParameterChanges().pop(ii, pc_point);
            if (ok) {
                downstream_port.getParameterChanges().push(ii, pc_point);
            }
        }
    }
}

AudioPort &ProcessorNode::pullAudioPort(int output_audio_port) {
    pullUpstreamDataAndProcess();

    return output_audio_ports_.at(output_audio_port);
}

ParameterChangePort &ProcessorNode::pullParameterChangePort(int output_pc_port) {
    pullUpstreamDataAndProcess();

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

const AudioBlock *ProcessorNode::getInputBlock() const {
    return input_block_.get();
}

const AudioBlock *ProcessorNode::getOutputBlock() const {
    return output_block_.get();
}

void ProcessorNode::initBlocksAndPorts(
    const std::vector<int> &input_channels,
    const std::vector<int> &output_channels) {
    auto total_input_channels = getTotalChannels(input_channels);
    auto total_output_channels = getTotalChannels(output_channels);
    auto num_params = getNumberOfParameters(proc_);

    const int init_num_frames = 0;
    input_block_ = std::make_shared<AudioBlock>(total_input_channels,
                                                init_num_frames, num_params);
    output_block_ = std::make_shared<AudioBlock>(total_output_channels,
                                                 init_num_frames, num_params);

    initAudioPorts(input_channels, output_channels);

    input_pc_ports_ = {ParameterChangePort{input_block_}};
    output_pc_ports_ = {ParameterChangePort{output_block_}};
}

void ProcessorNode::initAudioPorts(
    const std::vector<int> &input_channels,
    const std::vector<int> &output_channels) {
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

void ProcessorNode::checkAudioConnectionValidAndThrow(
    const AudioConnection &c) const {

    auto upstream_output_port_size = c.upstream_node->getAudioOutputPortSize();
    if (c.upstream_port_index >=
        static_cast<int>(upstream_output_port_size)) {
        throw std::invalid_argument(
            "invalid connection: upstream port output of range");
    }

    auto downstream_input_port_size = getAudioInputPortSize();
    if (c.downstream_port_index >=
        static_cast<int>(downstream_input_port_size)) {
        throw std::invalid_argument(
            "invalid connection: downstream port output of range");
    }

    auto upstream_port_num_channels =
        c.upstream_node->getAudioOutputPortChannels(
            c.upstream_port_index);
    auto downstream_port_num_channels =
        getAudioInputPortChannels(c.downstream_port_index);
    if (upstream_port_num_channels != downstream_port_num_channels) {
        throw std::invalid_argument(
            "invalid connection: number of channels mismatch");
    }
}

void ProcessorNode::checkParameterConnectionValidAndThrow(const ParameterChangeConnection &c) const {
    if (c.upstream_port_index >= c.upstream_node->getParameterChangeOutputPortSize()) {
        throw std::invalid_argument(
            "invalid parameter change connection: upstream port output of range");
    }

    if (c.downstream_port_index >= getParameterChangeInputPortSize()) {
        throw std::invalid_argument(
            "invalid parameter change connection: downstream port output of range");
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
int ProcessorNode::getParameterChangeInputPortSize() const {
    return input_pc_ports_.size();
}
int ProcessorNode::getParameterChangeOutputPortSize() const {
    return output_pc_ports_.size();
}
void ProcessorNode::setState(uint8_t *state, size_t size) {
    auto state_json = nlohmann::json::parse(state, state + size);

    BaseNode::setNodeID(state_json["node_id"].get<std::string>());

    proc_ = createProcessorWithState(state_json);

    // rebuild audio ports
    input_audio_ports_.clear();
    output_audio_ports_.clear();
    auto input_channels = state_json["input_channels"].get<std::vector<int>>();
    auto output_channels = state_json["output_channels"].get<std::vector<int>>();
    initBlocksAndPorts(input_channels, output_channels);
}
std::vector<uint8_t> ProcessorNode::getState() const {
    nlohmann::json state_json;
    state_json["node_id"] = getNodeID();
    state_json["node_type"] = "processor_node";
    state_json["input_channels"] = nlohmann::json(getChannelsFromPorts(input_audio_ports_));
    state_json["output_channels"] = nlohmann::json(getChannelsFromPorts(output_audio_ports_));
    state_json["processor_state"] = nlohmann::json::parse(proc_->getState());

    return NodeSerializationUtilities::jsonToBinaryData(state_json);
}

const IAudioProcessor *ProcessorNode::getProcessor() const {
    return (proc_ != nullptr) ? (proc_.get()) : (nullptr);
}

} // namespace libaa
