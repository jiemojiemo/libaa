//
// Created by user on 4/28/22.
//

#ifndef LIBAA_AA_I_NODE_H
#define LIBAA_AA_I_NODE_H

#pragma once
#include <string>
namespace libaa {
class AudioConnection;
class AudioPort;
class ParameterChangePort;
class ParameterChangeConnection;

class INode {
public:
    virtual ~INode() = default;

    virtual void setNodeID(std::string node_id) = 0;

    virtual std::string getNodeID() const = 0;

    /**
     * prepare to play
     * @param sample_rate the playback sample rate
     * @param max_block_size max number of samples during play
     */
    virtual void prepareToPlay(float sample_rate, int max_block_size) = 0;

    /**
     * Adds upstream audio connection
     */
    virtual void addUpstreamAudioConnection(const AudioConnection &c) = 0;

    /**
     * Adds upstream parameter change connection
     */
    virtual void addUpstreamParameterChangeConnection(const ParameterChangeConnection &c) = 0;

    /**
     * Pulls data from upstream node into input data, and then process it, finally returns the output parameter port.
     * @param output_audio_port , the output audio port index
     */
    virtual AudioPort &pullAudioPort(int output_audio_port) = 0;

    /**
     * Pulls data from upstream node into input data, and then process it, finally returns the output parameter port.
     * @param output_pc_port, the output parameter port index
     */
    virtual ParameterChangePort &pullParameterChangePort(int output_pc_port) = 0;

    /**
     * Returns true if last block processed
     */
    virtual bool hasProcessed() const = 0;

    /**
     * Sets processed state, see aso @hasProcessed
     */
    virtual void setProcessedState(bool is_processed) = 0;

    /**
     * Prepare for next block, call this before `pull` and reset hasProcessed()
     * state
     */
    virtual void prepareForNextBlock() = 0;

    virtual int getAudioInputPortSize() const = 0;

    virtual int getAudioOutputPortSize() const = 0;

    virtual int getAudioInputPortChannels(int port_index) const = 0;

    virtual int getAudioOutputPortChannels(int port_index) const = 0;

    virtual int getParameterChangeInputPortSize() const
    {
        return 0;
    }

    virtual int getParameterChangeOutputPortSize() const{
        return 0;
    }
};
} // namespace libaa

#endif // LIBAA_AA_I_NODE_H
