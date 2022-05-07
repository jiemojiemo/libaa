//
// Created by hw on 2022/1/15.
//

#ifndef LIBAA_AA_LEVEL_DETECTOR_H
#define LIBAA_AA_LEVEL_DETECTOR_H
#include <cmath>
namespace libaa {
enum class LevelDetectorMode {
    kLevelDetectorMode_Peak = 0,
    kLevelDetectorMode_MS,
    kLevelDetectorMode_RMS
};

struct LevelDetectorParameters {
    float attack_time_ms = 0.0f;
    float release_time_ms = 0.0f;
    LevelDetectorMode mode = LevelDetectorMode::kLevelDetectorMode_Peak;
    bool detect_db = false;
    bool clamp_to_unity_max = true;

    bool operator==(const LevelDetectorParameters &other) const {
        return attack_time_ms == other.attack_time_ms &&
               release_time_ms == other.release_time_ms && mode == other.mode &&
               detect_db == other.detect_db &&
               clamp_to_unity_max == other.clamp_to_unity_max;
    }
};

class LevelDetector {
public:
    LevelDetector(LevelDetectorParameters p)
        : param_(std::move(p)) {}

    void prepare(float sample_rate) {
        sample_rate_ = sample_rate;
        updateAttackAlpha(param_.attack_time_ms);
        updateReleaseAlpha(param_.release_time_ms);
    }

    float getSampleRate() const {
        return sample_rate_;
    }

    void setAttackTime(float attack_time_ms) {
        if (attack_time_ms == param_.attack_time_ms) {
            return;
        }

        param_.attack_time_ms = attack_time_ms;
        updateAttackAlpha(param_.attack_time_ms);
    }

    void setReleaseTime(float release_time_ms) {
        if (release_time_ms == param_.release_time_ms) {
            return;
        }

        param_.release_time_ms = release_time_ms;
        updateReleaseAlpha(param_.release_time_ms);
    }

    float getAttackTime() const {
        return param_.attack_time_ms;
    }

    float getReleaseTime() const {
        return param_.release_time_ms;
    }

    float getAttackAlpha() const {
        return attack_alpha_;
    }

    float getReleaseAlpha() const {
        return release_alpha_;
    }

    void setParameters(LevelDetectorParameters param) {
        param_ = std::move(param);
        updateAttackAlpha(param_.attack_time_ms);
        updateReleaseAlpha(param_.release_time_ms);
    }

    const LevelDetectorParameters &getParameters() const {
        return param_;
    }

    float process(float xn);

private:
    void updateAttackAlpha(float attack_time_ms) {
        attack_alpha_ = timeToTimeCoeff(attack_time_ms);
    }

    void updateReleaseAlpha(float release_time_ms) {
        release_alpha_ = timeToTimeCoeff(release_time_ms);
    }

    float timeToTimeCoeff(float time_ms) {
        return exp(-1.0f / (time_ms * sample_rate_ * 0.001));
    }

    LevelDetectorParameters param_;
    float sample_rate_ = 0.0f;
    float attack_alpha_ = 0.0f;
    float release_alpha_ = 0.0f;
    float last_level_ = 0.0f;
    float last_y1_ = 0.0f;
    static constexpr float kMSAlpha = 0.5f;
};

} // namespace libaa

#endif // LIBAA_AA_LEVEL_DETECTOR_H
