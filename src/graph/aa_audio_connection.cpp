//
// Created by user on 5/7/22.
//

#include "libaa/graph/aa_audio_connection.h"

namespace libaa {
bool operator==(const AudioConnection &lhs, const AudioConnection &rhs) {
    return lhs.upstream_node == rhs.upstream_node &&
           lhs.upstream_port_index == rhs.upstream_port_index &&
           lhs.downstream_port_index == rhs.downstream_port_index;
}
} // namespace libaa