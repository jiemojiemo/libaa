//
// Created by user on 4/19/22.
//

#include "libaa/processor/aa_reverb_tank_processor.h"

namespace libaa {
std::string ReverbTankProcessor::getName() const {
    return {"ReverbTank"};
}

void ReverbTankProcessor::prepareToPlay(float sample_rate, int max_block_size) {
    IAudioProcessor::prepareToPlay(sample_rate, max_block_size);

    utilities_.prepareToPlay(sample_rate, max_block_size);
}
void ReverbTankProcessor::processBlock(AudioBlock *input, AudioBlock *output) {
    output->buffer.copyFrom(&input->buffer);

    utilities_.process(output);
}
const AudioProcessorParameters *ReverbTankProcessor::getParameters() const {
    return &utilities_.params;
}
} // namespace libaa