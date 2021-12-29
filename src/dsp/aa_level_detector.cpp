//
// Created by hw on 2022/1/15.
//

#include "libaa/dsp/aa_level_detector.h"

namespace libaa
{
float LevelDetector::process(float xn)
{
    float input = fabs(xn);

    float current_level = 0.0f;
    if(param_.mode == LevelDetectorMode::kLevelDetectorMode_MS ||
       param_.mode == LevelDetectorMode::kLevelDetectorMode_RMS)
    {
        input *= input;
        current_level = kMSAlpha*(last_level_*last_level_) + (1-kMSAlpha)*(input);
    }else // peak mode
    {
        // smooth, decoupled peak detector should be used(from: Digital Dynamic Range Compressor Design— A Tutorial and Analysis)
        float t = release_alpha_*last_y1_ + (1-release_alpha_)*input;
        float y1 = fmax(input, t);
        current_level = attack_alpha_ * last_level_ + (1-attack_alpha_)*y1;

        last_y1_ = y1;
    }

    if(param_.clamp_to_unity_max){
        current_level = fmin(current_level, 1.0f);
    }

    last_level_ = current_level;

    if(param_.mode == LevelDetectorMode::kLevelDetectorMode_RMS){
        current_level = powf(current_level, 0.5f);
    }

    if(!param_.detect_db){
        return current_level;
    }

    if(current_level <= 0){
        return -96.0f;
    }

    return 20 * log10(current_level);
}
}
