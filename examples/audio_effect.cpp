//
// Created by william on 2020/2/21.
//

#include "libaa/audio_effect/aa_biquad_filter.h"
#include "libaa/audio_effect/aa_chorus.h"
#include "libaa/audio_effect/aa_delay_effect.h"
#include "libaa/audio_effect/aa_distortion.h"
#include "libaa/audio_effect/aa_flanger.h"
#include "libaa/audio_effect/aa_robot.h"
#include "libaa/audio_effect/aa_vibrato_effect.h"
#include "libaa/core/aa_audio_buffer.h"
#include "libaa/core/aa_delay_line.h"
#include "libaa/dsp/aa_comb_filter.h"
#include "libaa/dsp/aa_delay_APF.h"
#include "libaa/dsp/aa_nested_delay_APF.h"
#include "libaa/dsp/aa_two_band_shelving_filter.h"
#include "libaa/fileio/aa_audio_file.h"
#include "libaa/fileio/aa_file_input_stream.h"
#include "libaa/fileio/aa_file_output_stream.h"
#include "libaa/fileio/aa_wav_audio_format_reader.h"
#include "libaa/fileio/aa_wav_audio_format_writer.h"
#include <cassert>
#include <iomanip> // std::setprecision
#include <iostream>
using namespace std;
using namespace libaa;

int main() {
    const string input_filename = "/Users/user/Downloads/test_sin440.wav";
    string output_filename = "xxx.wav";

    auto in_stream = std::make_unique<FileInputStream>(input_filename);
    if (!in_stream->isOpen()) {
        cerr << "cannot open " << input_filename << endl;
        return -1;
    }

    auto out_stream = std::make_unique<FileOutputStream>(output_filename);
    if (!out_stream->isOpen()) {
        cerr << "cannot open " << output_filename << endl;
        return -1;
    }

    WaveAudioFormatReader reader(std::move(in_stream));

    int sample_rate = reader.sample_rate;
    int num_channels = reader.num_channels;
    int sample_index = 0;
    int acctual_block_size = 0;
    int predefine_block_size = 512;
    int num_frames = reader.length_in_samples;
    assert(num_channels == 1);

    std::vector<float> sample_buffer(predefine_block_size, 0.0f);
    float *dest_chan[1] = {sample_buffer.data()};

    WavFormatWriter writer(std::move(out_stream), reader.sample_rate,
                           reader.num_channels, 32);

    if (!writer.isOpen()) {
        cerr << "cannot open write" << endl;
        return -1;
    }

    //    CombFilter::CombFilterParameters parameters;
    //    parameters.lpf_g = 0.8;
    //    parameters.rt_60_ms = 1000;
    //    parameters.enable_LPF = false;
    //    parameters.delay_params.delay_ms = 20;
    //    CombFilter f;
    //    f.prepare(audio_file.getSampleRate(), 2 * audio_file.getSampleRate());
    //    f.updateParameters(parameters);

    NestedDelayAPF::NestedDelayAPFParameters parameters;
    parameters.delay_ms = 8.89185047;
    parameters.apf_g = 0.5;
    parameters.enable_LPF = false;
    parameters.lpf_g = 0.5;
    parameters.enable_LFO = true;
    parameters.lfo_depth = 1.0f;
    parameters.lfo_max_modulation_ms = 0.3;
    parameters.lfo_rate_hz = 0.15f;
    parameters.inner_apf_delay_ms = 24.4876518;
    parameters.inner_apf_g = -0.5;
    NestedDelayAPF f;
    f.prepare(sample_rate, 8192);
    f.updateParameters(parameters);

    //    SimpleDelay::SimpleDelayParameters parameters;
    //    parameters.delay_ms = 81;
    //    SimpleDelay f;
    //    f.prepare(sample_rate, 4410);
    //    f.updateParameters(parameters);

    //    SimpleLPF::SimpleLPFParameters parameters;
    //    parameters.lpf_g = 0.3;
    //    SimpleLPF f;
    //    f.updateParameters(parameters);

    //    TwoBandShelvingFilter f;
    //    f.prepare(sample_rate);
    //
    //    TwoBandShelvingFilter::TwoBandShelvingFilterParameters parameters;
    //    parameters.low_shelf_fc = 500;
    //    parameters.low_shelf_boost_db = 0;
    //    parameters.high_shelf_fc = 6000;
    //    parameters.high_shelf_boost_db = 15;
    //    f.updateParameters(parameters);

    int processed_index = 0;
    for (; sample_index < num_frames;) {
        acctual_block_size = (sample_index + predefine_block_size >= num_frames)
                                 ? (num_frames - sample_index)
                                 : (predefine_block_size);

        reader.readSamples(dest_chan, num_channels, 0, 0, acctual_block_size);

        for (int i = 0; i < acctual_block_size; ++i) {
            float in = sample_buffer[i];
            float output = 0.0f;
            if (processed_index == 394) {
                output = f.processSample(in);
                sample_buffer[i] = output;
            } else {
                output = f.processSample(in);
                sample_buffer[i] = output;
            }

            if (processed_index >= 0 && processed_index <= 5000) {
                cout << std::setprecision(15) << processed_index << ","
                     << output << endl;
            }

            ++processed_index;
        }

        writer.writePlanar(reinterpret_cast<const float **>(&dest_chan),
                           acctual_block_size);
        sample_index += acctual_block_size;
    }

    writer.flush();
    writer.close();

    return 0;
}