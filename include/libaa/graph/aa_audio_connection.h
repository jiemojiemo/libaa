//
// Created by user on 4/28/22.
//

#ifndef LIBAA_AA_AUDIO_CONNECTION_H
#define LIBAA_AA_AUDIO_CONNECTION_H

#pragma once
#include "libaa/core/aa_audio_block.h"
#include "libaa/graph/aa_audio_port.h"
#include "libaa/graph/aa_connection.h"

namespace libaa {
class AudioConnection : public Connection {
public:
    using Connection::Connection;
    /**
     * Pull from upstream node and returns the AudioPort
     */
    const AudioPort &pull() const {
        return upstream_node->pullAudioPort(upstream_port_index);
    }
};

bool operator==(const AudioConnection &lhs, const AudioConnection &rhs);

} // namespace libaa

#endif // LIBAA_AA_AUDIO_CONNECTION_H
