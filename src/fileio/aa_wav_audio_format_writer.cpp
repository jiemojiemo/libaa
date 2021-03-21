
//
// Created by William.Hua on 2021/3/9.
//

#include "libaa/fileio/aa_wav_audio_format_writer.h"
#include "dr_wav.h"

namespace libaa
{
class WavFormatWriter::Impl
{
public:
    Impl(WavFormatWriter* parent,
         int rate,
         int channels,
         int bits):
        parent_(parent)
    {
        openOutputWaveStream(rate, channels, bits);
    }

    void openOutputWaveStream(int rate,
                              int channels,
                              int bits)
    {
        if(channels <= 0 || rate <= 0 || bits <=0){
            return;
        }

        drwav_data_format format;
        format.container = drwav_container_riff;     // <-- drwav_container_riff = normal WAV files, drwav_container_w64 = Sony Wave64.
        format.format = DR_WAVE_FORMAT_IEEE_FLOAT;          // <-- Any of the DR_WAVE_FORMAT_* codes.
        format.channels = channels;
        format.sampleRate = rate;
        format.bitsPerSample = 32; // only support float now

        auto ret = drwav_init_write(&wav_, &format, writeCallback, seekCallback, this, nullptr);
        if(ret){
            parent_->sample_rate = rate;
            parent_->num_channels = channels;
            parent_->num_bits = 32;
        }
    }

    bool writePlanar(const float **samples, int num_samples)
    {
        if(!isOpen()){
            return false;
        }

        //  planar to interleave
        buff_.resize(num_samples * parent_->num_channels);
        for(auto i = 0; i < num_samples; ++i)
        {
            for(auto c = 0; c < parent_->num_channels; ++c)
            {
                buff_[i * parent_->num_channels + c] = samples[c][i];
            }
        }

        return writeInterleave(buff_.data(), num_samples);
    }

    bool writeInterleave(const float* samples, int num_samples)
    {
        if(!isOpen()){
            return false;
        }

        auto num_written = drwav_write_pcm_frames(&wav_, num_samples, samples);

        return num_written > 0;
    }

    bool isOpen() const
    {
        return parent_->sample_rate > 0 &&
            parent_->num_channels > 0 &&
            parent_->num_bits > 0;
    }

    void flush(){
    }

    void close() {
        flush();
        drwav_uninit(&wav_);

        parent_->sample_rate = -1;
        parent_->num_channels = -1;
        parent_->num_bits = -1;
    }

    static size_t writeCallback(void* pUserData, const void* pData, size_t bytesToWrite)
    {
        auto* self = static_cast<WavFormatWriter::Impl*>(pUserData);
        if(self->parent_->out_stream_->write((uint8_t*)(pData), bytesToWrite) == -1){
            return 0;
        }

        return bytesToWrite;
    }

    static drwav_bool32 seekCallback(void* pUserData, int offset, drwav_seek_origin origin)
    {
        auto* self = static_cast<WavFormatWriter::Impl*>(pUserData);
        if(origin == drwav_seek_origin_current)
        {
            if(self->parent_->out_stream_->seekp(offset, SEEK_CUR) != 0){
                return DRWAV_FALSE;
            }

        }else if(origin == drwav_seek_origin_start)
        {
            if(self->parent_->out_stream_->seekp(offset, SEEK_SET) != 0){
                return DRWAV_FALSE;
            }
        }

        return DRWAV_TRUE;
    }

    WavFormatWriter* parent_;
    drwav wav_;
    std::vector<float> buff_;
};

WavFormatWriter::WavFormatWriter(std::unique_ptr<OutputStream> out_stream,
                                 int rate,
                                 int channels,
                                 int bits):
    AudioFormatWriter(std::move(out_stream), rate, channels, bits),
    impl_(std::make_shared<Impl>(this, rate, channels, bits))
{
}

bool WavFormatWriter::writePlanar(const float **samples, int num_samples)
{
    return impl_->writePlanar(samples, num_samples);
}

bool WavFormatWriter::writeInterleave(const float* samples, int num_samples)
{
    return impl_->writeInterleave(samples, num_samples);
}

bool WavFormatWriter::isOpen() const
{
    return impl_->isOpen();
}

void WavFormatWriter::close() {
    return impl_->close();
}
void WavFormatWriter::flush() {
    return impl_->flush();
}

}
