//
// Created by admin on 2020/8/5.
//

#include "audio_effect/aa_robot.h"
#include "dsp/aa_fft.h"
#include <numeric>
#include <iostream>
using namespace std;
namespace libaa
{
class Robotisation::Impl
{
public:
    Impl(Robotisation* parent):
        parent_(parent)
    {
    }

    void prepareToPlay(double sample_rate, int max_block_size)
    {
        fft_ = std::make_unique<FFT>(parent_->fft_size);

        fft_time_domain_.resize(parent_->fft_size, 0);
        fft_freq_domain_.resize(parent_->fft_size, {0,0});

        window_ = Window::getWindow(parent_->window_type, parent_->fft_size);
        auto window_sum = std::accumulate(std::begin(window_), std::end(window_), 0.0f);
        fft_scale_ = 1.0f / static_cast<float>(parent_->fft_size) * ( window_sum / static_cast<float>(parent_->hop_size));

        cout << fft_scale_ << endl;

        input_buffer_.setSize(2, parent_->fft_size);
        output_buffer_.setSize(2, parent_->fft_size);
    }

    void processBlock(AudioBuffer<float> &buffer)
    {
        const auto num_channels = buffer.getNumChannels();
        const auto num_samples = buffer.getNumFrames();

        auto samplesincefft = 0u;
        auto inwritepos = 0u;
        auto outwritepos = 0u;
        auto outreadpos = 0u;

        const auto input_buffer_length = input_buffer_.getNumFrames();
        const auto output_buffer_length = output_buffer_.getNumFrames();

        for(auto c = 0u; c < num_channels; ++c)
        {
            auto* channel_data = buffer.getWritePointer(c);
            auto* input_buffer_data = input_buffer_.getWritePointer(c);
            auto* output_buffer_data = output_buffer_.getWritePointer(c);

            samplesincefft = samples_since_lastfft_;
            inwritepos = input_buffer_write_pos_;
            outwritepos = output_buffer_write_pos_;

            for(auto i = 0u; i < num_samples; ++i)
            {
                const float in = channel_data[i];
                // get result from output buffer
                channel_data[i] = output_buffer_data[outreadpos];
                output_buffer_data[outreadpos] = 0.0f;
                if(++outreadpos >= output_buffer_.getNumFrames())
                {
                    outreadpos = 0;
                }

                // store input sample into input buffer
                input_buffer_data[inwritepos] = in;
                if(++inwritepos >= parent_->fft_size) { inwritepos = 0; }

                // if reach the hop size, perform pv processing
                if(++samplesincefft >= parent_->hop_size)
                {
                    // clear sample since fft
                    samplesincefft = 0;

                    auto input_buffer_start_pos = (inwritepos + input_buffer_length - parent_->fft_size) % input_buffer_length;
                    auto input_buffer_index = input_buffer_start_pos;
                    for(auto fft_index = 0u; fft_index < parent_->fft_size; ++fft_index)
                    {
                        fft_time_domain_[fft_index] = input_buffer_data[input_buffer_index] * window_[fft_index];

                        if(++input_buffer_index >= input_buffer_length) { input_buffer_index = 0; }
                    }

                    // perform fft
                    fft_->forward(fft_time_domain_.data(), fft_freq_domain_.data());

//                    // set the phase to 0 but overall amplitude is the same as before
//                    for(auto bin = 0u; bin < fft_freq_domain_.size(); ++bin)
//                    {
//                        const float amplitude = sqrt(fft_freq_domain_[i].real() * fft_freq_domain_[i].real()
//                                                        + fft_freq_domain_[i].imag() * fft_freq_domain_[i].imag());
//
//                        fft_freq_domain_[i].real(amplitude);
//                        fft_freq_domain_[i].imag(0.0f);
//                    }

                    // perform ifft
                    fft_->inverse(fft_freq_domain_.data(), fft_time_domain_.data());


                    // save result to output buffer
                    auto output_buffer_index = outwritepos;
                    for(auto fft_index = 0u; fft_index < parent_->fft_size; ++fft_index)
                    {
//                        output_buffer_data[output_buffer_index] += window_[fft_index] * fft_time_domain_[output_buffer_index] * fft_scale_;
                        output_buffer_data[output_buffer_index] += fft_time_domain_[output_buffer_index] * 1.0f/parent_->fft_size;

                        if(++output_buffer_index >= output_buffer_length){ output_buffer_index = 0; }
                    }

                    outwritepos = (outwritepos + parent_->hop_size) % output_buffer_length;
                }

            }
        }

        samples_since_lastfft_ = samplesincefft;
        input_buffer_write_pos_ = inwritepos;
        output_buffer_write_pos_ = outwritepos;
        output_buffer_read_pos_ = outreadpos;
    }

    std::unique_ptr<FFT> fft_;

    AudioBuffer<float> input_buffer_;
    AudioBuffer<float> output_buffer_;

    vector<float> fft_time_domain_;
    vector<std::complex<float>> fft_freq_domain_;
    vector<float> window_;

    float fft_scale_;

    size_t samples_since_lastfft_ = 0;
    size_t input_buffer_write_pos_ = 0;
    size_t output_buffer_write_pos_ = 0;
    size_t output_buffer_read_pos_ = 0;
    Robotisation* parent_;
};

Robotisation::Robotisation():
    impl_(std::make_shared<Impl>(this))
{

}
void Robotisation::prepareToPlay(double sample_rate, int max_block_size)
{
    impl_->prepareToPlay(sample_rate, max_block_size);
}
void Robotisation::reset()
{
    fft_size            = {512};
    hop_size            = {256};
    window_type  = {WindowType::kRectangle};
}
void Robotisation::releaseResources()
{

}
void Robotisation::processBlock(AudioBuffer<float> &buffer)
{
    impl_->processBlock(buffer);
}

}
