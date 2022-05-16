//
// Created by user on 5/7/22.
//

#ifndef LIBAA_AA_GRAPH_NODE_H
#define LIBAA_AA_GRAPH_NODE_H

#pragma once
#include "libaa/graph/aa_audio_connection.h"
#include "libaa/graph/aa_audio_port.h"
#include "libaa/graph/aa_base_node.h"
#include "libaa/graph/aa_parameter_change_port.h"
namespace libaa {
class GraphNode : public BaseNode {
public:
    /**
     * PortNodeConnection describes the input or output ports of a graph
     *
     * eg. {{nodeA, 0}} means there is a graph's port which is nodeA's no.0 port
     * eg. {{nodeB, 1},{nodeC, 0}} means there is a graph's port which is
     * nodeB's no.1 port and nodeC's no.0 port eg. {{nodeB, 1}},{{nodeC,0}}
     * means there is two graph's port, the first port is nodeB's no.1 port, the
     * second is nodeC's no.0 port
     */
    struct PortNodeConnection {
        std::shared_ptr<INode> node{nullptr};
        int node_port_index{-1};
    };
    using InputPortNodeConnections = std::vector<std::list<PortNodeConnection>>;
    using OutputPortNodeConnections = std::vector<PortNodeConnection>;

    GraphNode(std::vector<std::shared_ptr<INode>> nodes,
              InputPortNodeConnections input_audio_port_connections,
              OutputPortNodeConnections output_audio_port_connections);

    GraphNode(std::vector<std::shared_ptr<INode>> nodes,
              InputPortNodeConnections input_audio_port_connections,
              InputPortNodeConnections input_param_change_port_connections,
              OutputPortNodeConnections output_audio_port_connections,
              OutputPortNodeConnections output_param_change_port_connections);

    ~GraphNode() override = default;

    NodeType getNodeType() const override;

    void prepareToPlay(float sample_rate, int max_block_size) override;

    void addUpstreamAudioConnection(const AudioConnection &c) override;

    void addUpstreamParameterChangeConnection(const ParameterChangeConnection &c) override;

    AudioPort &pullAudioPort(int output_audio_port) override;

    ParameterChangePort &pullParameterChangePort(int output_pc_port) override;

    bool hasProcessed() const override;

    void setProcessedState(bool is_processed) override;

    void prepareForNextBlock() override;

    const std::vector<std::shared_ptr<INode>> &getAllNodes() const;

    const InputPortNodeConnections &getInputAudioPortConnections() const;
    const InputPortNodeConnections &getInputParameterChangePortConnections() const;
    const OutputPortNodeConnections &getOutputAudioPortConnections() const;
    const OutputPortNodeConnections &getOutputParameterChangePortConnections() const;

    int getAudioInputPortSize() const override;

    int getAudioOutputPortSize() const override;

    int getParameterChangeInputPortSize() const override;

    int getParameterChangeOutputPortSize() const override;

    int getAudioInputPortChannels(int port_index) const override;

    int getAudioOutputPortChannels(int port_index) const override;

    void setState(uint8_t *state, size_t size) override;
    std::vector<uint8_t> getState() const override;

private:
    std::vector<std::shared_ptr<INode>> nodes_;
    InputPortNodeConnections input_audio_port_connections_;
    InputPortNodeConnections input_param_change_port_connections_;
    OutputPortNodeConnections output_audio_port_connections_;
    OutputPortNodeConnections output_param_change_port_connections_;
};
} // namespace libaa
#endif // LIBAA_AA_GRAPH_NODE_H
