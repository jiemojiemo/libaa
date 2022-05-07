//
// Created by user on 4/28/22.
//

#ifndef LIBAA_AA_AUDIO_CONNECTION_H
#define LIBAA_AA_AUDIO_CONNECTION_H

#pragma once
#include "libaa/core/aa_audio_block.h"
#include "libaa/graph/aa_audio_port.h"
#include "libaa/graph/aa_i_node.h"

namespace libaa {
class AudioConnection {
public:
    explicit AudioConnection(std::shared_ptr<INode> upstream_n,
                             int upstream_audio_port_inx,
                             int downstream_audio_port_inx)
        : upstream_node(std::move(upstream_n)),
          upstream_audio_port_index(upstream_audio_port_inx),
          downstream_audio_port_index(downstream_audio_port_inx) {}

    /**
     * Pull from upstream node and returns the AudioPort
     */
    const AudioPort &pull() const {
        return upstream_node->pullAudioPort(upstream_audio_port_index);
    }

    std::shared_ptr<INode> upstream_node{nullptr};
    int upstream_audio_port_index{-1};
    int downstream_audio_port_index{-1};
};

bool operator==(const AudioConnection &lhs, const AudioConnection &rhs);

} // namespace libaa

#endif // LIBAA_AA_AUDIO_CONNECTION_H
