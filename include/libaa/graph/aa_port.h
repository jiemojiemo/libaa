//
// Created by user on 5/11/22.
//

#ifndef LIBAA_AA_PORT_H
#define LIBAA_AA_PORT_H

#pragma once
namespace libaa {
enum class PortDirection {
    kInput = 0,
    kOutput
};

enum class PortType {
    kAudio = 0,
    kParameterChange
};

} // namespace libaa

#endif // LIBAA_AA_PORT_H
