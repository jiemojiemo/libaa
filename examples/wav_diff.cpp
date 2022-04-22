//
// Created by user on 4/17/22.
//

#include "libaa/fileio/aa_file_input_stream.h"
#include "libaa/fileio/aa_file_output_stream.h"
#include "libaa/fileio/aa_wav_audio_format_reader.h"
#include "libaa/fileio/aa_wav_audio_format_writer.h"
#include <cmath>
#include <iostream>
#include <string>
using namespace libaa;
using namespace std;
int main(int argc, char *argv[]) {
    if (argc != 3) {
        return -1;
    }

    const string input_filename = argv[1];
    const string input_filename1 = argv[2];

    auto in_stream = std::make_unique<FileInputStream>(input_filename);
    if (!in_stream->isOpen()) {
        cerr << "cannot open " << input_filename << endl;
        return -1;
    }

    auto in_stream1 = std::make_unique<FileInputStream>(input_filename1);
    if (!in_stream1->isOpen()) {
        cerr << "cannot open " << input_filename1 << endl;
        return -1;
    }

    WaveAudioFormatReader reader(std::move(in_stream));
    WaveAudioFormatReader reader1(std::move(in_stream1));

    if (!reader1.isOpenOk()) {
        return -2;
    }

    const auto length =
        std::min(reader.length_in_samples, reader1.length_in_samples);
    vector<float> samples(length);
    vector<float> samples1(length);
    float *d[1]{samples.data()};
    float *d1[1]{samples1.data()};

    reader.readSamples(d, 1, 0, 0, length);
    reader1.readSamples(d1, 1, 0, 0, length);

    for (int i = 0; i < length; ++i) {
        float diff = abs(samples[i] - samples1[i]);
        if (diff != 0) {
            float db = 20 * log10(diff);
            if (db > -60) {
                cout << i << "," << db << endl;
            }
        }
    }

    return 0;
}