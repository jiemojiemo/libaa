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
     * Pulls data from update stream and then process the input data, finally
     * returns the output audio port.
     * @param output_audio_port , the output port index
     */
    virtual AudioPort &pullAudioPort(int output_audio_port) = 0;

    /**
     * Returns true if last block processed
     */
    virtual bool hasProcessed() const = 0;

    /**
     * Prepare for next block, call this before `pull` and reset hasProcessed()
     * state
     */
    virtual void prepareForNextBlock() = 0;

    /**
     * Returns all input ports
     */
    virtual const std::vector<AudioPort> &getAudioInputPorts() const = 0;

    /**
     * Returns all output ports
     */
    virtual const std::vector<AudioPort> &getAudioOutputPorts() const = 0;
};
} // namespace libaa

#endif // LIBAA_AA_I_NODE_H