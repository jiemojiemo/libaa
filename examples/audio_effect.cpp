//
// Created by william on 2020/2/21.
//

#include "libaa/audio_basics/aa_delay_line.h"
#include "libaa/fileio/aa_audio_file.h"
#include "libaa/audio_basics/aa_audio_buffer.h"
#include "libaa/audio_effect/aa_delay_effect.h"
#include "libaa/audio_effect/aa_vibrato_effect.h"
#include "libaa/audio_effect/aa_flanger.h"
#include "libaa/audio_effect/aa_chorus.h"
#include "libaa/audio_effect/aa_distortion.h"
#include "libaa/audio_effect/aa_robot.h"
#include "libaa/audio_effect/aa_biquad_filter.h"
#include <iostream>
using namespace std;
using namespace libaa;


int main()
{
    const string input_filename = "../../res/wav/es01_2ch_48k_i16.wav";
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


    BiquadFilter processor;
    processor.setRateAndBufferSizeDetails(audio_file.getSampleRate(), predefine_block_size);
    processor.prepareToPlay(audio_file.getSampleRate(), predefine_block_size);
    processor.setCoefficients({{1.0,
                                0.73,
                                1.0,
                                -0.78,
                                0.88}});
//    processor.coeff_array_[BiquadFilter::a0] = 1.0f;
//    processor.coeff_array_[BiquadFilter::a1] = 0.73f;
//    processor.coeff_array_[BiquadFilter::a2] = 0.0f;
//    processor.coeff_array_[BiquadFilter::b1] = -0.71f;
//    processor.coeff_array_[BiquadFilter::b2] = 0.0f;

//    processor.hop_size = 256;
//    processor.fft_size = 1024;

//    processor.input_gainDb = 30;
//    processor.distortion_type = Distortion::kHardClipping;

//    processor.min_delay = 0.05;
//    processor.num_voices = 4;
//    processor.sweep_width = 0.05;
//    processor.lfo_freq = 0.5;
//    processor.stereo = 1;
//    processor.lfo_freq = 1;
//    processor.feedback_ = 0.95f;

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