
//
// Created by William.Hua on 2021/2/10.
//

#include "libaa/fileio/aa_wav_audio_format_reader.h"
#include "dr_wav.h"
#include <vector>

namespace libaa
{
class WaveAudioFormatReader::Impl
{
public:
    Impl(WaveAudioFormatReader* parent):
        parent_(parent)
    {
        openWaveFromStream();
    }

    ~Impl(){
        drwav_uninit(&wav_);
    }

    bool readSamples(float **dest_channels,
                     int num_dest_channels,
                     int start_offset_of_dest,
                     int64_t start_offset_of_file,
                     int num_samples)
    {
        if(parent_->pos_ != start_offset_of_file){
            if(drwav_seek_to_pcm_frame(&wav_, start_offset_of_file)){
                parent_->pos_ = start_offset_of_file;
            }
        }

        num_dest_channels = std::min(num_dest_channels, parent_->num_channels);
        interleave_buffer_.resize(num_dest_channels * num_samples);

        auto num_read = drwav_read_pcm_frames_f32(&wav_, num_samples, interleave_buffer_.data());

        // interleave to planar
        for(drwav_uint64 i = start_offset_of_dest, ii = 0; i < num_read; ++i){
            for(auto c = 0; c < num_dest_channels; ++c){
                dest_channels[c][i] = interleave_buffer_[ii++];
            }
        }

        return true;
    }

    void openWaveFromStream(){
        if(drwav_init(&wav_,readCallback,seekCallback,this,nullptr))
        {
            parent_->sample_rate = wav_.sampleRate;
            parent_->num_channels = wav_.channels;
            parent_->length_in_samples = wav_.totalPCMFrameCount;
            parent_->num_bits = wav_.bitsPerSample;
        }
    }

    static size_t readCallback(void* pUserData, void* pBufferOut, size_t bytesToRead)
    {
        auto* self = (WaveAudioFormatReader::Impl*)(pUserData);
        return self->parent_->in_stream_->read((uint8_t *)pBufferOut, bytesToRead);
    }

    static drwav_bool32 seekCallback(void* pUserData, int offset, drwav_seek_origin origin)
    {
        auto* self = (WaveAudioFormatReader::Impl*)(pUserData);
        if(origin == drwav_seek_origin_current)
        {
            if(self->parent_->in_stream_->seekg(offset, SEEK_CUR) != 0){
                return DRWAV_FALSE;
            }

        }else if(origin == drwav_seek_origin_start)
        {
            if(self->parent_->in_stream_->seekg(offset, SEEK_SET) != 0){
                return DRWAV_FALSE;
            }
        }

        return DRWAV_TRUE;
    }

    WaveAudioFormatReader* parent_;
    drwav wav_;
    std::vector<float> interleave_buffer_;
};

WaveAudioFormatReader::WaveAudioFormatReader(std::unique_ptr<InputStream> in_stream)
    : AudioFormatReader(std::move(in_stream)),
      impl_(std::make_shared<Impl>(this))
{

}


bool WaveAudioFormatReader::readSamples(float **dest_channels,
                                        int num_dest_channels,
                                        int start_offset_of_dest,
                                        int64_t start_offset_of_file,
                                        int num_samples)
{
    if(!isOpenOk()){
        return false;
    }

    return impl_->readSamples(dest_channels, num_dest_channels, start_offset_of_dest, start_offset_of_file, num_samples);
}

bool WaveAudioFormatReader::isOpenOk() {
    return sample_rate > 0 && num_channels > 0 && length_in_samples > 0;
}

}