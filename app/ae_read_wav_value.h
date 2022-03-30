//
// Created by William on 3/29/22.
//

#ifndef AUDIOEFFECT_AE_READ_WAV_VALUE_H
#define AUDIOEFFECT_AE_READ_WAV_VALUE_H

#pragma once
#include "ae_demo_value.h"
#include "libaa/fileio/aa_file_input_stream.h"
#include "libaa/fileio/aa_wav_audio_format_reader.h"

class ReadWavValue : public DemoValue {
public:
    void process(DemoContext &context) override {
        using namespace libaa;

        auto full_path = context.finder.find(context.input_file_name);
        if (full_path.empty()) {
            std::cerr << "Cannot find " << context.input_file_name;
            return;
        }

        auto in_stream = std::make_unique<FileInputStream>(full_path);
        if (!in_stream->isOpen()) {
            return;
        }

        auto reader =
            std::make_unique<WaveAudioFormatReader>(std::move(in_stream));
        if (!reader->isOpenOk()) {
            return;
        }

        context.in_sample_rate = reader->sample_rate;
        context.in_num_channels = reader->num_channels;
        context.in_num_frames = reader->length_in_samples;

        // decode from wav
        context.in_samples.resize(context.in_num_channels);
        std::vector<float *> data_refer_to(context.in_num_channels);
        for (auto c = 0; c < context.in_num_channels; ++c) {
            context.in_samples[c].resize(context.in_num_frames);
            data_refer_to[c] = context.in_samples[c].data();
        }

        reader->readSamples(data_refer_to.data(), context.in_num_channels, 0, 0,
                            context.in_num_frames);
    }
};

#endif // AUDIOEFFECT_AE_READ_WAV_VALUE_H
