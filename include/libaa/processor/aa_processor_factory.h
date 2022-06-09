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
#include "libaa/processor/aa_vibrato_processor.h"

namespace libaa {
class IAudioProcessor;
class ResourcePathFinder;
namespace ProcessorFactory {
/**
 * Create processor by name
 * @param name The processor's name
 * @return processor or null if no such processor name
 */
std::unique_ptr<IAudioProcessor> create(const std::string &name);

/**
 * Create processor with name and processor state
 * @param name              The processor's name
 * @param proc_state_str    The processor's state json string
 * @param finder            The resource finder, finder help some processors to load other file
 * @return processor or null if no such processor name
 */
std::unique_ptr<IAudioProcessor> create(const std::string &name,
                                        const std::string &proc_state_str,
                                        const ResourcePathFinder *finder = nullptr);

} // namespace ProcessorFactory
} // namespace libaa

#endif // LIBAA_AA_PROCESSOR_FACTORY_H
