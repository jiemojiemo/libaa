//
// Created by user on 5/12/22.
//

#ifndef LIBAA_AA_PROCESSOR_FACTORY_H
#define LIBAA_AA_PROCESSOR_FACTORY_H

#pragma once
#include "libaa/processor/aa_audio_processor.h"
#include "libaa/processor/aa_delay_processor.h"
#include "libaa/processor/aa_gain_processor.h"
#include "libaa/processor/aa_pitch_shifter_processor.h"
#include "libaa/processor/aa_reverb_tank_processor.h"
#include "libaa/processor/aa_source_callback_processor.h"
#include "libaa/processor/aa_source_processor.h"
#include "libaa/processor/aa_vibrator_processor.h"

namespace libaa {
namespace ProcessorFactory {
/**
 * Create processor by name
 */
std::shared_ptr<IAudioProcessor> create(const std::string &name);
} // namespace ProcessorFactory
} // namespace libaa

#endif // LIBAA_AA_PROCESSOR_FACTORY_H
