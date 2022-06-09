//
// Created by user on 5/12/22.
//
#include "libaa/processor/aa_processor_factory.h"
#include "libaa/core/aa_clip_factory.h"
#include "libaa/core/aa_resource_path_finder.h"
#include "libaa/processor/aa_audio_processor.h"
#include "libaa/processor/aa_channel_vocoder_processor.h"
#include "libaa/processor/aa_compressor_processor.h"
#include "libaa/processor/aa_delay_processor.h"
#include "libaa/processor/aa_gain_processor.h"
#include "libaa/processor/aa_iir_filter_processor.h"
#include "libaa/processor/aa_limiter_processor.h"
#include "libaa/processor/aa_pitch_shifter_processor.h"
#include "libaa/processor/aa_processor_factory.h"
#include "libaa/processor/aa_resampler_processor.h"
#include "libaa/processor/aa_reverb_tank_processor.h"
#include "libaa/processor/aa_source_callback_processor.h"
#include "libaa/processor/aa_source_processor.h"
#include "libaa/processor/aa_vibrato_processor.h"

namespace libaa {
namespace ProcessorFactory {

namespace {
auto buildChannelVocoderProcessor(const std::string &proc_state_str,
                                  const ResourcePathFinder *finder) {
    auto proc = std::make_unique<ChannelVocoderProcessor>(nullptr, std::make_shared<ResourcePathFinder>(*finder));
    proc->setState((uint8_t *)proc_state_str.data(), proc_state_str.size());
    return proc;
}

auto buildGeneralProcessor(const std::string &name,
                           const std::string &proc_state_str) {
    auto proc = create(name);
    if (proc != nullptr) {
        proc->setState((uint8_t *)proc_state_str.data(), proc_state_str.size());
    }
    return proc;
}

} // namespace

std::unique_ptr<IAudioProcessor> create(const std::string &name) {
    if (name == "Gain") {
        return std::unique_ptr<IAudioProcessor>(new GainProcessor);
    } else if (name == "Delay") {
        return std::unique_ptr<IAudioProcessor>(new DelayProcessor);
    } else if (name == "PitchShifter") {
        return std::unique_ptr<IAudioProcessor>(new PitchShiftProcessor);
    } else if (name == "ReverbTank") {
        return std::unique_ptr<IAudioProcessor>(new ReverbTankProcessor);
    } else if (name == "Source") {
        return std::unique_ptr<IAudioProcessor>(new SourceProcessor);
    } else if (name == "Source Callback") {
        return std::unique_ptr<IAudioProcessor>(new SourceCallbackProcessor);
    } else if (name == "Vibrato") {
        return std::unique_ptr<IAudioProcessor>(new VibratoProcessor);
    } else if (name == "Compressor") {
        return std::unique_ptr<IAudioProcessor>(new Compressor);
    } else if (name == "Limiter") {
        return std::unique_ptr<IAudioProcessor>(new LimiterProcessor);
    } else if (name == "Resampler") {
        return std::unique_ptr<IAudioProcessor>(new ResamplerProcessor);
    } else if (name == "IIRFilter") {
        return std::unique_ptr<IAudioProcessor>(new IIRFilterProcessor);
    } else {
        return nullptr;
    }
}

std::unique_ptr<IAudioProcessor> create(const std::string &name,
                                        const std::string &proc_state_str,
                                        const ResourcePathFinder *finder) {
    if (name == "ChannelVocoder") {
        return buildChannelVocoderProcessor(proc_state_str, finder);
    } else {
        return buildGeneralProcessor(name, proc_state_str);
    }
}

} // namespace ProcessorFactory
} // namespace libaa