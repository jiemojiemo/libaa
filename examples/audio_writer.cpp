
//
// Created by William.Hua on 2021/3/9.
//
#include "libaa/fileio/aa_wav_audio_format_writer.h"
#include "libaa/fileio/aa_wav_audio_format_reader.h"
#include "libaa/fileio/aa_file_input_stream.h"
#include "libaa/fileio/aa_file_output_stream.h"
#include <memory>
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
using namespace std;
using namespace libaa;
int main()
{
    const string input_filename = "../../res/sound/es01_2ch_48k_i16.wav";
    string output_filename = "wav_writer_out.wav";

    auto in_file = std::make_unique<FileInputStream>(input_filename);
    if(!in_file->isOpen()){
        cerr << "cannot open input file\n";
        return -1;
    }

    WaveAudioFormatReader reader(std::move(in_file));
    const int sample_rate = reader.sample_rate;
    const int num_channels = reader.num_channels;
    const int num_frames = reader.length_in_samples;
    cout << "sample rate: " << sample_rate << endl;
    cout << "number of channels: " << num_channels << endl;
    cout << "number of total frames: " << num_frames << endl;
    vector<float> buffer(num_channels * num_frames);
    vector<float*> data_refer_to(num_channels);
    for(auto c = 0; c < num_channels; ++c){ data_refer_to[c] = buffer.data() + c*num_frames; }

    // read whole file
    reader.readSamples(data_refer_to.data(), num_channels, 0, 0,num_frames);

    auto out_file = std::make_unique<FileOutputStream>(output_filename);
    if(!out_file->isOpen()){
        cerr << "cannot open output file\n";
        return -1;
    }
    WavFormatWriter writer(std::move(out_file), sample_rate, num_channels, 32);
    writer.writePlanar((const float**)(data_refer_to.data()), num_frames);
    writer.close();
}