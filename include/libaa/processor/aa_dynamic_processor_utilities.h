//
// Created by user on 5/23/22.
//

#ifndef LIBAA_AA_DYNAMIC_PROCESSOR_UTILITIES_H
#define LIBAA_AA_DYNAMIC_PROCESSOR_UTILITIES_H
#pragma once
#include "libaa/dsp/aa_db_utils.h"
#include "libaa/dsp/aa_level_detector.h"

namespace libaa {
struct DynamicProcessorParameters {
    float threshold_db{-10};
    float knee_width{5};
    float ratio{1};
    float attack_time_ms{5};
    float release_time_ms{500};
    float output_gain_db{0};
    bool soft_knee{true};
    bool hard_limiter{false};
};
class DynamicProcessorUtilities {
public:
    void updateParameters(DynamicProcessorParameters params) {
        parameters = params;

        updateLevelDetectorParameters(parameters);
    }

    const DynamicProcessorParameters &getParameters() const {
        return parameters;
    }

    void prepare(float sample_rate) {
        detector.prepare(sample_rate);
    }

    float process(float in) {
        // reference - Digital Dynamic Range Compressor Design— A Tutorial and Analysis
        // equation 23
        float in_db = DbUtils::scaleToDb(fabs(in));
        float gain_computer_out = computeGain(in_db);
        float level_detect_in_db = in_db - gain_computer_out;
        float control_db = -detector.processWithoutABS(level_detect_in_db);
        float final_control_db = control_db + parameters.output_gain_db;
        return in * DbUtils::dbToScale(final_control_db);
    }

    float computeGain(float input_gain_db) {
        float output_db{0.0f};
        float threshold = parameters.threshold_db;
        float ratio = parameters.ratio;
        float width = parameters.knee_width;
        bool soft_knee = parameters.soft_knee;
        bool limiter_mode = parameters.hard_limiter;

        if (limiter_mode && input_gain_db > threshold) {
            return threshold;
        }

        // hard knee
        if (!soft_knee) {
            if (input_gain_db <= threshold) {
                output_db = input_gain_db;
            } else {
                output_db = threshold + (input_gain_db - threshold) / ratio;
            }
        } else // soft knee
        {
            if (input_gain_db < (threshold - width / 2.0f)) {
                output_db = input_gain_db;
            } else if (input_gain_db > (threshold + width / 2.0f)) {
                output_db = threshold + (input_gain_db - threshold) / ratio;
            } else {
                float a = 1.0f / ratio - 1.0f;
                float b = input_gain_db - threshold + width / 2;
                float c = b * b;
                output_db = input_gain_db + (a * c) / (2 * width);
            }
        }

        return output_db;
    }

    DynamicProcessorParameters parameters;
    LevelDetector detector;

private:
    void updateLevelDetectorParameters(const DynamicProcessorParameters &params) {
        LevelDetectorParameters level_detector_params = detector.getParameters();
        level_detector_params.release_time_ms = params.release_time_ms;
        level_detector_params.attack_time_ms = params.attack_time_ms;

        // the input of detector is db already we don't need to transform to db again or clamp to unit max
        level_detector_params.detect_db = false;
        level_detector_params.clamp_to_unity_max = false;

        detector.setParameters(level_detector_params);
    }
};
} // namespace libaa

#endif // LIBAA_AA_DYNAMIC_PROCESSOR_UTILITIES_H
