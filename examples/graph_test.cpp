//
// Created by user on 5/14/22.
//

#include "libaa/fileio/aa_file_input_stream.h"
#include "libaa/fileio/aa_file_output_stream.h"
#include "libaa/fileio/aa_wav_audio_format_reader.h"
#include "libaa/fileio/aa_wav_audio_format_writer.h"
#include "libaa/graph/aa_graph_builder.h"
#include "libaa/graph/aa_realtime_graph_render.h"
#include "libaa/processor/aa_processor_factory.h"
#include "libaa/processor/aa_source_callback_processor.h"
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

    AudioBlock mock_block{0, 0, 20};

    std::shared_ptr<IAudioProcessor> vibrato_proc = ProcessorFactory::create("Vibrato");
    std::shared_ptr<IAudioProcessor> pitch_shift_proc = ProcessorFactory::create("PitchShifter");
    std::shared_ptr<IAudioProcessor> gain_proc = ProcessorFactory::create("Gain");

    auto vibrato_node = std::make_shared<ProcessorNode>(vibrato_proc);
    auto pitch_shift_node = std::make_shared<ProcessorNode>(pitch_shift_proc);

    GraphBuilder builder;
    builder.insert("vibrato", vibrato_node);
    builder.insert("pitch_shift", pitch_shift_node);
    builder.addConnection(ConnectionType::kAudioConnection, {vibrato_node, 0}, {pitch_shift_node, 0});
    builder.exposePort(PortDirection::kInput, PortType::kAudio, 0, {vibrato_node, 0});
    builder.exposePort(PortDirection::kOutput, PortType::kAudio, 0, {pitch_shift_node, 0});

    WavFormatWriter writer(std::move(out_stream), reader.sample_rate,
                           reader.num_channels, 32);

    if (!writer.isOpen()) {
        cerr << "cannot open write" << endl;
        return -1;
    }

    RealtimeGraphRender realtime_render(builder.build());
    realtime_render.setAudioCallback(0, [&reader, &num_channels](AudioBufferX<float> *buffer, ProcessingContext &context) {
        float *dest_chan[1] = {buffer->getWriterPointer(0)};
        reader.readSamples(dest_chan, num_channels, 0, 0, context.num_samples);
    });

    realtime_render.prepareToPlay(sample_rate, predefine_block_size);
    int processed_index = 0;
    for (; sample_index < num_frames;) {
        acctual_block_size = (sample_index + predefine_block_size >= num_frames)
                                 ? (num_frames - sample_index)
                                 : (predefine_block_size);

        auto output_port = realtime_render.pull(acctual_block_size);

        float *dest_chan = output_port.getChannelData(0);
        writer.writeInterleave(dest_chan, acctual_block_size);
        sample_index += acctual_block_size;
        processed_index += acctual_block_size;
    }

    writer.flush();
    writer.close();
}