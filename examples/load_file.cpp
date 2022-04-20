//
// Created by william on 2019/12/1.
//
#include "libaa/fileio/aa_audio_file.h"
#include <iostream>
#include <string>
using namespace std;
using namespace libaa;

int main(int argc, char *argv[]) {
    //    if(argc < 2)
    //    {
    //        cerr << "Usage: load_file {audio-filename} [{output-filename}]" <<
    //        endl; return -1;
    //    }
    //
    //    string input_filename = argv[1];
    //    string output_filename = "load_file_result.wav";
    //    if(argc == 3)
    //    {
    //        output_filename = argv[2];
    //    }

    AudioFile audio_file;
    audio_file.load("/Users/user/Downloads/test_test_re.wav");
    
    audio_file.setSampleRate(44100);
    audio_file.setNumBits(32);
    audio_file.setNumChannles(1);
    audio_file.setNumFrames(1024);
    audio_file.samples[0][0] = 1.0f;
    //    int failed = audio_file.load(input_filename);
    //    if(failed)
    //    {
    //        cerr << "load file failed\n";
    //        return 0;
    //    }
    auto output_filename = "impulse_0.wav";
    audio_file.saveToWave(output_filename);

    return 0;
}