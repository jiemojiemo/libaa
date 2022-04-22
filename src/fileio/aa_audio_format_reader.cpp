
//
// Created by William.Hua on 2021/2/10.
//

#include "libaa/fileio/aa_audio_format_reader.h"

namespace libaa {
bool AudioFormatReader::isOpenOk() {
    return false;
}
int64_t AudioFormatReader::getPosition() const {
    return pos_;
}
} // namespace libaa