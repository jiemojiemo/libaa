//
// Created by user on 6/9/22.
//

#include "libaa/core/aa_clip.h"
#include "libaa/core/aa_clip_utilities.h"

namespace libaa {
namespace ClipUtilities {
nlohmann::json serialize(const Clip *clip) {
    nlohmann::json j;
    j["audio_file_uri"] = clip->getAudioFileUri();
    j["looping"] = clip->isLooping();

    return j;
}
} // namespace ClipUtilities
} // namespace libaa
