//
// Created by William on 3/29/22.
//

#ifndef AUDIOEFFECT_AE_WRITE_WAV_VALUE_H
#define AUDIOEFFECT_AE_WRITE_WAV_VALUE_H

#pragma once

#include "ae_demo_context.h"
#include "ae_demo_value.h"
#include "ae_read_wav_value.h"
#include "libaa/fileio/aa_file_output_stream.h"
#include "libaa/fileio/aa_wav_audio_format_writer.h"
#include <cassert>
#include <filesystem>

class WriteWavValue : public DemoValue {
public:
    void process(DemoContext &context) override {
        using namespace libaa;

        auto output_full_path = std::filesystem::path(context.output_dir) /
                                context.output_file_name;
        context.output_full_path = output_full_path.string();

        auto out_stream =
            std::make_unique<FileOutputStream>(context.output_full_path);
        if (!out_stream->isOpen()) {
            std::cerr << "Cannot write to " << context.output_full_path
                      << std::endl;
            return;
        }

        WavFormatWriter writer(std::move(out_stream), context.in_sample_rate,
                               context.in_num_channels, 16);

        assert(context.in_samples.size() > 0);
        int num_frames = context.in_samples[0].size();
        std::vector<const float *> data_refer_to(context.in_num_channels);
        for (auto c = 0u; c < data_refer_to.size(); ++c) {
            data_refer_to[c] = context.in_samples[c].data();
        }

        writer.writePlanar(data_refer_to.data(), num_frames);
        writer.flush();
        writer.close();
    }
};

#endif // AUDIOEFFECT_AE_WRITE_WAV_VALUE_H
