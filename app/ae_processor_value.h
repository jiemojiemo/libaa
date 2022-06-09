//
// Created by William on 3/30/22.
//

#ifndef AUDIOEFFECT_AE_PROCESSOR_VALUE_H
#define AUDIOEFFECT_AE_PROCESSOR_VALUE_H

#pragma once
#include "ae_demo_value.h"
#include "libaa/core/aa_audio_block.h"
#include "libaa/processor/aa_vibrato_processor.h"
#include <vector>
using namespace libaa;
using namespace std;

class ProcessorValue : public DemoValue {
public:
    void process(DemoContext &context) override {
        // setup processor
        VibratoProcessor proc;
        proc.prepareToPlay(context.in_sample_rate, context.block_size);

        size_t num_param = (proc.getParameters() == nullptr)
                               ? (0)
                               : (proc.getParameters()->size());
        block = std::make_unique<AudioBlock>(context.in_num_channels,
                                             context.block_size, num_param);
        block->param_changes.push(0, {0, 0, 0.7});
        block->param_changes.push(1, {0, 0, 0.5});

        int index = 0;
        vector<float *> data_refer_to(context.in_samples.size());
        for (int c = 0; c < data_refer_to.size(); ++c) {
            data_refer_to[c] = context.in_samples[c].data();
        }

        for (; index < context.in_num_frames;) {
            int actual_num_frames = getNextBlockSize(
                index, context.in_num_frames, context.block_size);

            block->buffer.copyFrom(data_refer_to.data(),
                                   context.in_num_channels, actual_num_frames,
                                   index, 0);

            proc.processBlock(block.get(), block.get());

            block->buffer.copyTo(data_refer_to.data(), context.in_num_channels,
                                 actual_num_frames, 0, index);

            index += context.block_size;
        }
    }

    int getNextBlockSize(int current_index, int total_num_frames,
                         int block_size) {
        return (total_num_frames - current_index) < block_size
                   ? (total_num_frames - current_index)
                   : (block_size);
    }

    std::unique_ptr<AudioBlock> block;
};

#endif // AUDIOEFFECT_AE_PROCESSOR_VALUE_H
