//
// Created by user on 5/12/22.
//
#include "libaa/processor/aa_processor_factory.h"

namespace libaa
{
namespace ProcessorFactory
{
std::shared_ptr<IAudioProcessor> create(const std::string &name) {
    if (name == "Gain") {
        return std::shared_ptr<IAudioProcessor>(new GainProcessor);
    } else if (name == "Delay") {
        return std::shared_ptr<IAudioProcessor>(new DelayProcessor);
    } else if (name == "PitchShifter") {
        return std::shared_ptr<IAudioProcessor>(new PitchShiftProcessor);
    } else if (name == "ReverbTank") {
        return std::shared_ptr<IAudioProcessor>(new ReverbTankProcessor);
    } else if (name == "Source") {
        return std::shared_ptr<IAudioProcessor>(new SourceProcessor);
    } else if (name == "Source Callback") {
        return std::shared_ptr<IAudioProcessor>(new SourceCallbackProcessor);
    } else if (name == "Vibrato") {
        return std::shared_ptr<IAudioProcessor>(new VibratoProcessor);
    }

    return nullptr;
}
}
}