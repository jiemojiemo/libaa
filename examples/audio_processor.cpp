//
// Created by user on 4/19/22.
//
#include "libaa/fileio/aa_file_input_stream.h"
#include "libaa/fileio/aa_file_output_stream.h"
#include "libaa/fileio/aa_wav_audio_format_reader.h"
#include "libaa/fileio/aa_wav_audio_format_writer.h"
#include "libaa/processor/aa_moorer_reverb_processor.h"
#include "libaa/processor/aa_reverb_tank_processor.h"
#include "libaa/processor/aa_reverb_tank_utilities.h"
#include "libaa/processor/aa_schroeder_reverb_processor.h"
#include <cassert>
#include <iostream>
using namespace std;
using namespace libaa;
int main(int argc, char *argv[]) {
    if (argc != 3) {
        return -1;
    }

    const string input_filename = argv[1];
    const string output_filename = argv[2];
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
    //    SchroederReverbProcessor proc;
    MoorerReverbProcessor proc;
    //    ReverbTankProcessor proc;
    int num_params = proc.getParameters()->size();
    AudioBlock block(num_channels, predefine_block_size, num_params);
    block.param_changes.push(1, {0, 0, 1.0f});
    //    block.param_changes.push(8, {1, 0, 1.0f});
    //    block.param_changes.push(9, {1, 0, 0.0f});

    float *dest_chan[1] = {block.buffer.getWriterPointer(0)};

    WavFormatWriter writer(std::move(out_stream), reader.sample_rate,
                           reader.num_channels, 32);

    if (!writer.isOpen()) {
        cerr << "cannot open write" << endl;
        return -1;
    }

    proc.prepareToPlay(sample_rate, predefine_block_size);
    int processed_index = 0;
    for (; sample_index < num_frames;) {
        acctual_block_size = (sample_index + predefine_block_size >= num_frames)
                                 ? (num_frames - sample_index)
                                 : (predefine_block_size);

        reader.readSamples(dest_chan, num_channels, 0, 0, acctual_block_size);

        if (processed_index == 51200) {
            proc.processBlock(&block, &block);
        } else {
            proc.processBlock(&block, &block);
        }

        writer.writePlanar(reinterpret_cast<const float **>(&dest_chan),
                           acctual_block_size);
        sample_index += acctual_block_size;
        processed_index += acctual_block_size;
    }

    writer.flush();
    writer.close();
}