//
// Created by user on 6/6/22.
//

#ifndef LIBAA_AA_CLIP_FACTORY_H
#define LIBAA_AA_CLIP_FACTORY_H

#pragma once
#include "libaa/core/aa_clip.h"
#include <memory>
namespace libaa {
class ResourcePathFinder;
namespace ClipFactory {
std::unique_ptr<Clip> buildClip(const std::string &audio_file_uri,
                                bool loop_or_not,
                                ResourcePathFinder *finder = nullptr);
} // namespace ClipFactory
} // namespace libaa

#endif // LIBAA_AA_CLIP_FACTORY_H
