//
// Created by william on 2020/6/23.
//

#include "audio_effect/aa_compressor.h"

namespace libaa
{

class Compressor::Impl
{
public:
    void prepareToPlay(double sample_rate, int max_block_size)  {
        input_buffer_.setSize(1, max_block_size);

        xg_.resize(max_block_size);
        yg_.resize(max_block_size);
        xl_.resize(max_block_size);
        yl_.resize(max_block_size);
        c_.resize(max_block_size);

        yl_prev_ = 0.0f;
        threshold_ = 0.0f;
        ratio_ = 1.0f;
        attack_time_ = 0.1f;
        release_time_ = 0.1f;
        makeup_gain_ = 0.0f;
    }

    void processBlock(AudioBuffer<float> &buffer, size_t block_size, double sample_rate) {
        const auto num_channels = buffer.getNumChannels();
        const auto num_samples = buffer.getNumFrames();
        assert(num_channels == 2);
        assert(num_samples <= static_cast<size_t>(block_size));

        input_buffer_.clear();
        for(auto i=0u; i < num_samples; ++i)
        {
            input_buffer_.getWritePointer(0)[i] += 0.5f * buffer.getWritePointer(0)[i];
            input_buffer_.getWritePointer(0)[i] += 0.5f * buffer.getWritePointer(1)[i];
        }

        float alpha_attack = exp(-1/(0.001 * attack_time_ * sample_rate));
        float alpha_release = exp(-1/(0.001 * release_time_ * sample_rate));

        for(auto i=0u; i < num_samples; ++i)
        {
            const float in = input_buffer_.getWritePointer(0)[i];
            if(abs(in) < 0.000001f)
            {
                xg_[i] = -120;
            } else
            {
                xg_[i] = 20 * log10(abs(in));
            }

            if(xg_[i] > threshold_)
            {
                yg_[i] = threshold_ + (xg_[i] - threshold_) / ratio_;
            } else
            {
                yg_[i] = xg_[i];
            }

            xl_[i] = xg_[i] - yg_[i];

            if(xl_[0] > yl_prev_)
            {
                yl_[i] = alpha_attack * yl_prev_ + (1-alpha_attack)*xl_[i];
            } else
            {
                yl_[i] = alpha_release * yl_prev_ + (1-alpha_release)*xl_[i];
            }

            c_[i] = pow(10, (makeup_gain_ - yl_[i])/20.0f);
            yl_prev_ = yl_[i];

            buffer.getWritePointer(0)[i] *= c_[i];
            buffer.getWritePointer(1)[i] *= c_[i];
        }
    }


    AudioBuffer<float> input_buffer_;
    vector<float> xg_;
    vector<float> yg_;
    vector<float> xl_;
    vector<float> yl_;
    vector<float> c_;
    float yl_prev_;
    float threshold_;
    float ratio_;
    float attack_time_;  // ms
    float release_time_; // ms
    float makeup_gain_;
};

Compressor::Compressor() :
    impl_(std::make_shared<Impl>())
{
}


void Compressor::prepareToPlay(double sample_rate, int max_block_size)  {
    impl_->prepareToPlay(sample_rate, max_block_size);
}
void Compressor::reset(){
}

void Compressor::releaseResources() {
}

void Compressor::processBlock(AudioBuffer<float> &buffer){
    impl_->processBlock(buffer, getBlockSize(), getSampleRate());
}
void Compressor::setThreshold(float v) {
    impl_->threshold_ = v;
}
void Compressor::setAttackTime(float time_ms) {
    impl_->attack_time_ = time_ms;
}
void Compressor::setReleaseTime(float time_ms) {
    impl_->release_time_ = time_ms;
}
void Compressor::setRatio(float r) {
    impl_->ratio_ = r;
}
void Compressor::setMakeupGain(float g) {
    impl_->makeup_gain_ = g;
}

}

