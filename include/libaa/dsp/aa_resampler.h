//
// Created by user on 5/16/22.
//

#ifndef LIBAA_AA_RESAMPLER_H
#define LIBAA_AA_RESAMPLER_H

#pragma once
#include <memory>

namespace libaa
{
class Resampler {
public:
    Resampler();
    void prepare(float sample_rate, int num_channels, int max_block_size);
    /**
     * Resample the given multi-channel buffers.
     * @param in is the input buffer
     * @param in_num_frames is the number of frame the input buffer
     * @param out is the output buffer
     * @param out_num_frames is the number of frame the output buffer.
     *        Generally you want out_num_frames to be at least
     * ceil(in_num_frames * ratio)
     * @param ratio the resample ratio, >1 will give more samples.
     * @param is_final is final processing or not
     * @return Returns the number of frames written to the output buffers. This
     * may be smaller than outspace even where the ratio suggests otherwise,
     * particularly at the start of processing where there may be a filter tail
     * to allow for.
     */
    int resample(float **in, int in_num_frames, float **out, int out_num_frames,
                 float ratio, bool is_final);

    /**
     * Resample the given interleaved buffer.
     * @param in is the input buffer
     * @param in_num_frames is the number of frame the input buffer(i.e it has
     * in_num_frames * getNumberChannels() samples)
     * @param out is the output buffer
     * @param out_num_frames is the number of frame the output buffer.(i.e it
     * has out_num_frames * getNumberChannels() samples) Generally you want
     * out_num_frames to be at least ceil(in_num_frames * ratio)
     * @param ratio the resample ratio, >1 will give more samples.
     * @param is_final is final processing or not
     * @return Returns the number of frames written to the output buffers. This
     * may be smaller than outspace even where the ratio suggests otherwise,
     * particularly at the start of processing where there may be a filter tail
     * to allow for.
     */
    int resampleInterleaved(float *in, int in_num_frames, float *out,
                            int out_num_frames, float ratio, bool is_final);

    int getNumberChannels() const;

    // for testing
    void *getResampleImpl() const;

private:
    class Impl;
    std::shared_ptr<Impl> impl_{nullptr};
};

}

#endif // LIBAA_AA_RESAMPLER_H
