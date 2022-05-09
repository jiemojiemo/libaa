//
// Created by user on 5/9/22.
//

#ifndef LIBAA_AA_PARAMETER_CHANGE_PORT_H
#define LIBAA_AA_PARAMETER_CHANGE_PORT_H

#pragma once
#include "libaa/core/aa_audio_block.h"
#include "libaa/core/aa_parameter_change_ringbuffer.h"
#include <cassert>
namespace libaa {
class ParameterChangePort {
public:
    ParameterChangePort(std::shared_ptr<AudioBlock> block);

    void pushParameterChange(ParameterChangePoint p);

    const ParameterChanges &getParameterChanges() const;

    ParameterChanges &getParameterChanges();

private:
    std::shared_ptr<AudioBlock> block_{nullptr};
};
} // namespace libaa

#endif // LIBAA_AA_PARAMETER_CHANGE_PORT_H
