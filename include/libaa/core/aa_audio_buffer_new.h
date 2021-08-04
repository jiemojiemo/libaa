
//
// Created by William.Hua on 2021/7/25.
//

#ifndef LIBAA_INCLUDE_LIBAA_CORE_AA_AUDIO_BUFFER_NEW_H
#define LIBAA_INCLUDE_LIBAA_CORE_AA_AUDIO_BUFFER_NEW_H
#pragma once
#include <list>
#include <vector>
#include <algorithm>
namespace libaa
{
template <typename T>
class AudioBufferNew
{
public:
    explicit AudioBufferNew():
        AudioBufferNew(0, 0)
    {

    }
    explicit AudioBufferNew(size_t num_channels, size_t num_frames):
        num_channels_(num_channels),
        num_frames_(num_frames)
    {
        allocateData();
    }

    explicit AudioBufferNew(std::list<std::vector<T>>&& channel_data):
        num_channels_(channel_data.size())
    {
        copyFromVectors(channel_data);
    }

    size_t getNumberChannels() const{
        return num_channels_;
    }

    size_t getNumberFrames() const{
        return num_frames_;
    }

    const T* getReadPointer(size_t channel) const{
        return &data_[channel * num_frames_];
    }

    T* getWriterPointer(size_t channel){
        return &data_[channel * num_frames_];
    }

    void copyFrom(AudioBufferNew<T>* other){
        for(auto c = 0u; c < getNumberChannels(); ++c){
            std::copy_n(other->getReadPointer(c), getNumberFrames(), this->getWriterPointer(c));
        }
    }

    void resize(size_t num_channels, size_t num_frames)
    {
        num_channels_ = num_channels;
        num_frames_ = num_frames;

        allocateData();
    }

private:
    size_t findTheMaxFramesSize(const std::list<std::vector<float>>& channel_data)
    {
        size_t max_frames_size = 0;
        for(const auto& item : channel_data)
        {
            if(item.size() > max_frames_size) max_frames_size = item.size();
        }

        return max_frames_size;
    }

    void allocateData(){
        data_.resize(num_channels_ * num_frames_, 0.0f);
    }

private:
    void copyFromVectors(const std::list<std::vector<T>>& channel_data)
    {
        num_frames_ = findTheMaxFramesSize(channel_data);
        allocateData();

        size_t c = 0;
        for(const auto& item : channel_data)
        {
            std::copy_n(item.data(), item.size(), getWriterPointer(c));
            ++c;
        }
    }

private:
    size_t num_channels_{0};
    size_t num_frames_{0};
    std::vector<T> data_;
};
}

#endif //LIBAA_INCLUDE_LIBAA_CORE_AA_AUDIO_BUFFER_NEW_H
