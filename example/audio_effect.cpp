//
// Created by william on 2020/2/21.
//

#include "audio_basics/aa_delay_line.h"
#include "fileio/aa_audio_file.h"
#include "audio_basics/aa_audio_buffer.h"
#include "audio_effect/aa_delay_effect.h"
#include <iostream>
using namespace std;
using namespace libaa;


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