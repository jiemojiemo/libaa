//
// Created by user on 6/6/22.
//

#include "libaa/core/aa_clip.h"

namespace libaa {
Clip::Clip(AudioBlock source_block,
           bool is_looping,
           std::string audio_file_uri)
    : source_block_(std::move(source_block)),
      is_looping_(is_looping),
      audio_file_uri_(std::move(audio_file_uri)) {
}

bool Clip::isLooping() const {
    return is_looping_;
}

size_t Clip::getNumberChannels() const {
    return source_block_.buffer.getNumberChannels();
}

size_t Clip::getNumberFrames() const {
    return source_block_.buffer.getNumberFrames();
}

float Clip::readSample(int channel_index, int sample_index) const {
    const float *channel_data = source_block_.buffer.getReadPointer(channel_index);
    if (isLooping()) {
        sample_index = sample_index % source_block_.buffer.getNumberFrames();
    }
    return channel_data[sample_index];
}

std::string Clip::getAudioFileUri() const {
    return audio_file_uri_;
}
} // namespace libaa
