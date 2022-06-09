//
// Created by user on 6/9/22.
//

#ifndef LIBAA_AE_CLIP_UTILITIES_H
#define LIBAA_AE_CLIP_UTILITIES_H

#pragma once
#include <nlohmann/json.hpp>
namespace libaa {
class Clip;
namespace ClipUtilities {
nlohmann::json serialize(const Clip *clip);
} // namespace ClipUtilities
} // namespace libaa

#endif // LIBAA_AE_CLIP_UTILITIES_H
