//
// Created by william on 2020/2/21.
//

#include "audio_effect/aa_delay_line.h"
#include "fileio/aa_audio_file.h"
#include "audio_basics/aa_audio_buffer.h"
#include <iostream>
using namespace std;
using namespace libaa;

class AudoEffectProcessor
{
public:
    virtual string getName() const  = 0;
    virtual void prepareToPlay(double sample_rate, int samplers_per_block) = 0;
    virtual void reset() = 0;
    virtual void releaseResources() = 0;
    virtual void processBlock(AudioBuffer<float>& buffer) = 0;
};

class DelayEffect : public AudoEffectProcessor
{
public:
    DelayEffect()= default;

    string getName() const override
    {
        return "delay";
    }

    void prepareToPlay(double sample_rate, int samplers_per_block) override
    {
        int num_new_channel = 2;

        dlines_.resize(num_new_channel);
        delay_length_in_sample_.resize(num_new_channel, static_cast<size_t>(delay_length_ * sample_rate));

        for(int i = 0; i < num_new_channel; ++i)
        {
            dlines_[i].resize(static_cast<size_t>(2.0 * sample_rate));
        }

    }

    void reset() override
    {
        for(auto& d : dlines_){ d.clear(); }
    }

    void releaseResources() override
    {
        for(auto& d : dlines_){ d.clear(); }
    }

    void processBlock(AudioBuffer<float> &buffer) override
    {
        const int num_channels = buffer.getNumChannels();
        const int num_samples = buffer.getNumSamples();

        for(int c = 0; c < num_channels; ++c)
        {
            float* channel_data = buffer.getWritePointer(c);
            auto& dline = dlines_[c];
            size_t delay_samples = delay_length_in_sample_[c];

            for(int i = 0; i < num_samples; ++i)
            {
                const float in = channel_data[i];
                const float delay_val = dline.get(delay_samples);
                float out = 0.0f;

                out = (dry_mix_ * in + wet_mix_ * delay_val);

                dline.push( in + feedback_ * delay_val);

                channel_data[i] = out;
            }
        }
    }

    float delay_length_ = 1;
    float feedback_ = 0.8;
    float dry_mix_ = 1.0;
    float wet_mix_ = 0.2;

private:
    std::vector<DelayLine<float>> dlines_;
    std::vector<size_t> delay_length_in_sample_;
};

int main(int argc, char* argv[])
{
    const string input_filename = "../../res/wav/english_voice_test.wav";
    string output_filename = "audio_effect_result.wav";

    AudioFile audio_file;
    int failed = audio_file.load(input_filename);
    if(failed)
    {
        cerr << "load file failed\n";
        return 0;
    }

    const int num_frames = audio_file.getNumFrames();
    const int num_channels = audio_file.getNumChannels();
    const int predefine_block_size = 512;
    int sample_index = 0;
    int acctual_block_size = 0;

    DelayEffect processor;
    processor.prepareToPlay(audio_file.getSampleRate(), predefine_block_size);

    for(;sample_index < num_frames;)
    {
        acctual_block_size = (sample_index + predefine_block_size >= num_frames) ?
                             (num_frames - sample_index) : (predefine_block_size);

        vector<float*> data_refer_to(num_channels, nullptr);
        for(int c = 0; c < num_channels; ++c)
        {
            data_refer_to[c] = audio_file.samples[c].data() + sample_index;
        }

        AudioBuffer<float> block(data_refer_to.data(), num_channels, 0, acctual_block_size);

        processor.processBlock(block);

        sample_index += acctual_block_size;
    }

    audio_file.saveToWave(output_filename);

    return 0;
}