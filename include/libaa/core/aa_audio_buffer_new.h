
//
// Created by William.Hua on 2021/7/25.
//

#ifndef LIBAA_INCLUDE_LIBAA_CORE_AA_AUDIO_BUFFER_NEW_H
#define LIBAA_INCLUDE_LIBAA_CORE_AA_AUDIO_BUFFER_NEW_H
#pragma once
#include <algorithm>
#include <list>
#include <vector>
namespace libaa {
template <typename T>
class AudioBufferX {
public:
    explicit AudioBufferX()
        : AudioBufferX(0, 0) {}

    explicit AudioBufferX(size_t num_channels, size_t num_frames)
        : num_channels_(num_channels), num_frames_(num_frames) {
        allocateData();
    }

    explicit AudioBufferX(std::list<std::vector<T>> &&channel_data)
        : num_channels_(channel_data.size()) {
        copyFromVectors(channel_data);
    }

    size_t getNumberChannels() const {
        return num_channels_;
    }

    size_t getNumberFrames() const {
        return num_frames_;
    }

    const T *getReadPointer(size_t channel) const {
        if (channel >= getNumberChannels()) {
            return nullptr;
        }
        return &data_[channel * num_frames_];
    }

    T *getWriterPointer(size_t channel) {
        if (channel >= getNumberChannels()) {
            return nullptr;
        }
        return &data_[channel * num_frames_];
    }

    void copyFrom(AudioBufferX<T> *other) {
        copyFrom(other, other->getNumberChannels(), other->getNumberFrames(), 0, 0);
    }

    void copyFrom(AudioBufferX<T> *other, int num_channels, int num_frames,
                  int src_frame_offset, int dest_frame_offset) {
        for (auto c = 0; c < num_channels; ++c) {
            const T *s = other->getReadPointer(c) + src_frame_offset;
            T *d = getWriterPointer(c) + dest_frame_offset;
            std::copy_n(s, num_frames, d);
        }
    }

    void copyFrom(T **source, int num_channels, int num_frames,
                  int src_frame_offset, int dest_frame_offset) {
        for (auto c = 0; c < num_channels; ++c) {
            T *s = source[c] + src_frame_offset;
            T *d = getWriterPointer(c) + dest_frame_offset;
            std::copy_n(s, num_frames, d);
        }
    }

    void copyTo(T **dest, int num_channels, int num_frames,
                int src_frame_offset, int dest_frame_offset) {
        for (auto c = 0; c < num_channels; ++c) {
            T *s = getWriterPointer(c) + src_frame_offset;
            T *d = dest[c] + dest_frame_offset;
            std::copy_n(s, num_frames, d);
        }
    }

    void resize(size_t num_channels, size_t num_frames) {
        num_channels_ = num_channels;
        num_frames_ = num_frames;

        allocateData();
    }

    void resizeFrames(size_t num_frames) {
        resize(getNumberChannels(), num_frames);
    }

    void applyGain(size_t channel, size_t start_sample, size_t num_samples, float gain) {
        auto *channel_data = getWriterPointer(channel) + start_sample;

        if (gain != 1) {
            for (auto i = 0u; i < num_samples; ++i) {
                channel_data[i] *= gain;
            }
        }
    }

    void applyGain(size_t start_sample, size_t num_samples, float gain) {
        for (auto c = 0u; c < num_channels_; ++c) {
            applyGain(c, start_sample, num_samples, gain);
        }
    }

    void applyGainRamp(size_t channel, size_t start_sample, size_t num_samples, float start_gain, float end_gain) {
        if (start_gain == end_gain) {
            applyGain(channel, start_sample, num_samples, start_gain);
        } else {
            const auto inc = (end_gain - start_gain) / (float)(num_samples);
            auto *channel_data = getWriterPointer(channel) + start_sample;
            for (auto i = 0u; i < num_samples; ++i) {
                channel_data[i] *= start_gain;
                start_gain += inc;
            }
        }
    }

    void applyGainRamp(size_t start_sample, size_t num_samples, float start_gain, float end_gain) {
        for (auto c = 0u; c < num_channels_; ++c) {
            applyGainRamp(c, start_sample, num_samples, start_gain, end_gain);
        }
    }

private:
    size_t
    findTheMaxFramesSize(const std::list<std::vector<float>> &channel_data) {
        size_t max_frames_size = 0;
        for (const auto &item : channel_data) {
            if (item.size() > max_frames_size)
                max_frames_size = item.size();
        }

        return max_frames_size;
    }

    void allocateData() {
        data_.resize(num_channels_ * num_frames_, 0.0f);
    }

private:
    void copyFromVectors(const std::list<std::vector<T>> &channel_data) {
        num_frames_ = findTheMaxFramesSize(channel_data);
        allocateData();

        size_t c = 0;
        for (const auto &item : channel_data) {
            std::copy_n(item.data(), item.size(), getWriterPointer(c));
            ++c;
        }
    }

private:
    size_t num_channels_{0};
    size_t num_frames_{0};
    std::vector<T> data_;
};
} // namespace libaa

#endif // LIBAA_INCLUDE_LIBAA_CORE_AA_AUDIO_BUFFER_NEW_H
