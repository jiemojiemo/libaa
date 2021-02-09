//
// Created by william on 2019/11/27.
//

#include "fileio/aa_wav_decoder.h"

#include "dr_wav.h"

#include <iostream>

namespace libaa
{
class WavDecoder::Impl
{
public:
    drwav wav;
};

WavDecoder::WavDecoder()
    : impl_(std::make_shared<Impl>())
{

}


int WavDecoder::open(const std::string& filename)
{
    if(!drwav_init_file(&impl_->wav, filename.c_str(), NULL))
    {
        std::cerr << "Open file "<<  filename << " failed\n";
        return -1;
    }

    num_channels_ = impl_->wav.channels;
    sample_rate_  = impl_->wav.sampleRate;
    num_bits_     = impl_->wav.bitsPerSample;
    num_frames_   = impl_->wav.totalPCMFrameCount;
    return 0;
}

int WavDecoder::read(float *buffer, size_t size)
{
    const size_t frame_to_read = size / num_channels_;

    int num_readed = drwav_read_pcm_frames_f32(&impl_->wav, frame_to_read, buffer);

    return num_readed;
}

}