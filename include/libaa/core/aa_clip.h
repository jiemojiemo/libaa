//
// Created by user on 6/6/22.
//

#ifndef LIBAA_AA_CLIP_H
#define LIBAA_AA_CLIP_H

#pragma once
#include "libaa/core/aa_audio_block.h"
#include <string>
namespace libaa {
class Clip {
public:
    Clip(AudioBlock source_block,
         bool is_looping,
         std::string audio_file_uri = "");

    bool isLooping() const;

    size_t getNumberChannels() const;
    size_t getNumberFrames() const;

    float readSample(int channel_index, int sample_index) const;

    std::string getAudioFileUri() const;

private:
    AudioBlock source_block_;
    bool is_looping_{false};
    std::string audio_file_uri_;
};
} // namespace libaa

#endif // LIBAA_AA_CLIP_H
