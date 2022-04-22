//
// Created by william on 2019/11/28.
//

#pragma once
#include <memory>
#include <string>
#include <vector>

namespace libaa {
class AudioFile {
public:
    AudioFile();
    /**
     * open file and decode it. All decoded samples store in the member variable
     * 'samples'
     * @param filename
     * @return 0 successful, others failed.
     */
    int load(const std::string &filename);

    int saveToWave(const std::string &save_path) const;

    void setChannelData(size_t channel, float *data, size_t data_len);
    std::vector<std::vector<float>> samples;

public:
    inline size_t getNumChannels() const {
        return num_channels_;
    }
    inline size_t getSampleRate() const {
        return sample_rate_;
    }
    inline size_t getNumFrames() const {
        return samples.size() > 0 ? samples[0].size() : 0;
    }
    inline size_t getNumBits() const {
        return num_bits_;
    }

    inline void setNumChannles(size_t channels) {
        num_channels_ = channels;
        samples.resize(num_channels_);
    }
    inline void setSampleRate(size_t sample_rate) {
        sample_rate_ = sample_rate;
    }
    inline void setNumFrames(size_t num_frames) {
        for (auto &x : samples) {
            x.resize(num_frames);
        }
    }
    inline void setNumBits(size_t bits) {
        num_bits_ = bits;
    }

protected:
    size_t num_channels_{0};
    size_t sample_rate_{0};
    size_t num_bits_{0};

private:
    class Impl;
    std::shared_ptr<Impl> impl_;
};

} // namespace libaa
