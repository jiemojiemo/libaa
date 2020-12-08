//
// Created by william on 2020/2/22.
//

#pragma once
#include <vector>

using std::vector;

namespace libaa
{
template <typename T>
class AudioBuffer
{
public:
    AudioBuffer()
        : num_channels_(0),
          size_(0),
          channels_(32, nullptr)
    {

    }

    explicit AudioBuffer(int num_channels, int num_samples)
        : num_channels_(num_channels),
          size_(num_samples)
    {
        assert(num_channels >= 0);
        assert(num_samples >= 0);

        allocateData();
    }

    AudioBuffer(T* const* data_refer_to,
                int num_channel_to_use,
                int start_sample,
                int num_samples)
        : num_channels_(num_channel_to_use),
          size_(num_samples)
    {
        assert(data_refer_to != nullptr);
        assert(num_channel_to_use >= 0);
        assert(start_sample >= 0);
        assert(num_samples >= 0);

        allocateChannels(data_refer_to, start_sample);
    }

    /**
     * Copies another buffer.
     *
     * This buffer will make its own copy of other's data, unless the buffer was created using
     * an external data buffer
     */
    AudioBuffer(const AudioBuffer& other)
        : num_channels_(other.num_channels_),
          size_(other.size_),
          allocated_size_(other.allocated_size_)
    {
        if(allocated_size_ == 0)
        {
            channels_ = other.channels_;
        } else
        {
            allocateData();
            copyChannels(other.channels_);
        }
    }

    /**
     * Copies another buffer onto this one
     *
     * This buffer will make its own copy of other's data, unless the buffer was created using
     * an external data buffer
     */
    AudioBuffer& operator=(const AudioBuffer& other)
    {
        if(this != &other)
        {
            num_channels_ = other.num_channels_;
            size_ = other.size_;
            allocated_size_ = other.allocated_size_;

            if(allocated_size_ == 0)
            {
                channels_ = other.channels_;
            } else
            {
                allocateData();
                copyChannels(other.channels_);
            }
        }

        return *this;
    }

    /**
     * Move constructor
     */
    AudioBuffer(AudioBuffer&& other) noexcept
        : num_channels_(other.num_channels_),
          size_(other.size_),
          allocated_size_(other.allocated_size_),
          channels_(std::move(other.channels_)),
          allocated_data_(std::move(other.allocated_data_))
    {
        other.num_channels_ = 0;
        other.size_ = 0;
        other.allocated_size_ = 0;
    }

    AudioBuffer& operator=(AudioBuffer&& other) noexcept
    {
        num_channels_ = other.num_channels_;
        size_ = other.size_;
        allocated_size_ = other.allocated_size_;
        channels_ = std::move(other.channels_);
        allocated_data_ = std::move(other.allocated_data_);

        other.num_channels_ = 0;
        other.size_ = 0;
        other.allocated_size_ = 0;

        return *this;
    }


    void copyFrom(const AudioBuffer& other)
    {
        num_channels_ = other.num_channels_;
        size_ = other.size_;

        allocateData();
        copyChannels(other.channels_);
    }

    void allocateChannels(T* const* data_refer_to,
                          int offset)
    {
        channels_.resize(num_channels_ + 1);
        channels_[num_channels_] = nullptr;

        for(size_t i = 0; i < num_channels_; ++i)
        {
            channels_[i] = data_refer_to[i] + offset;
        }
    }

    void copyChannels(const std::vector<T*>& other_channels)
    {
        for(size_t i = 0; i < num_channels_; ++i)
        {
            auto first_iter = other_channels[i];
            auto last_iter = first_iter + size_;
            auto output_iter = channels_[i];
            std::copy(first_iter, last_iter, output_iter);
        }
    }


    size_t getNumFrames() const {return size_;}
    size_t getNumChannels() const {return num_channels_;}

    T* getWritePointer(size_t channel_number) const noexcept
    {
        assert(channel_number < num_channels_);
        assert(channel_number < channels_.size());

        return channels_[channel_number];
    }

    const T* getReadPointer(size_t channel_number) const noexcept
    {
        assert(channel_number < num_channels_);

        return channels_[channel_number];
    }

    void clear() const noexcept
    {
        for(size_t i = 0; i < num_channels_; ++i)
        {
            std::fill(channels_[i], channels_[i]+size_, 0);
        }
    }

    void setSize(int new_num_channels, int new_num_samples) noexcept
    {
        if(new_num_channels != static_cast<int>(num_channels_) || new_num_samples != static_cast<int>(size_))
        {
            num_channels_ = new_num_channels;
            size_ = new_num_samples;

            allocateData();
            clear();
        }
    }

private:
    void allocateData()
    {
        allocated_size_ = size_ * num_channels_;

        allocated_data_.resize(allocated_size_);
        channels_.resize(num_channels_ + 1);
        for(size_t i = 0; i < num_channels_; ++i)
        {
            channels_[i] = (allocated_data_.data() + i*size_);
        }
        channels_[num_channels_] = nullptr;
    }

    size_t num_channels_={0};
    size_t size_ = {0};
    size_t allocated_size_ = {0};
    std::vector<T*> channels_;
    std::vector<T> allocated_data_;
};
}
