//
// Created by william on 2019/12/15.
//

#include "libaa/dsp/aa_mfcc.h"
#include "libaa/dsp/aa_options.h"
#include "libaa/fileio/aa_audio_file.h"
#include <Eigen/Core>
#include <iostream>

using namespace std;
using namespace libaa;
using namespace Eigen;

int main() {
    const string filename = "../../res/wav/wav_mono_16bit_44100.wav";
    AudioFile audiofile;
    int ret = audiofile.load(filename);
    if (ret) {
        cerr << "open file failed\n";
        return -1;
    }

    const auto *left_channel = audiofile.samples[0].data();
    const auto num_frames = audiofile.getNumFrames();
    const auto sample_rate = audiofile.getSampleRate();

    Options opts;
    opts.n_mels = 88;
    opts.n_mfcc = 22;
    ArrayXXf mfccs = MFCC::mfcc(left_channel, num_frames, sample_rate, opts);
    cout << "mfccs: " << mfccs.rows() << "x" << mfccs.cols() << endl;

    return 0;
}