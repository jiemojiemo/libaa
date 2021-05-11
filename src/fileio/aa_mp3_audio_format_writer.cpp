
//
// Created by William.Hua on 2021/5/11.
//

#include "libaa/fileio/aa_mp3_audio_format_writer.h"
#include "lame.h"
#include <vector>
namespace libaa
{
class MP3FormatWriter::Impl
{
public:
    Impl(MP3FormatWriter* parent,int rate,
         int channels,
         int bits):
        parent_(parent)
    {
        openLAME(rate, channels, bits);
    }

    void openLAME(int rate,
                  int channels,
                  int bits)
    {
        if(channels <= 0 || rate <= 0 || bits <=0)
        {
            return;
        }

        gfp_ = initLAME(rate, channels);
        if(gfp_){
            const int kInitBufferSize = 1024 * 1.25 + 7200;
            allocateBuffer(kInitBufferSize);

            parent_->sample_rate = rate;
            parent_->num_channels = channels;
            parent_->num_bits = 16;
        }

    }

    void allocateBuffer(int buffer_size)
    {
        if(mp3_buffer_.size() < size_t(buffer_size)){
            mp3_buffer_.resize(buffer_size);
        }
    }

    lame_global_flags* initLAME(int rate,
                                int channels)
    {
        lame_global_flags* gfp = lame_init();

        int ret = 0;
        ret = lame_set_num_channels(gfp, channels);
        if(ret < 0){
            lame_close(gfp);
            return nullptr;
        }

        lame_set_out_samplerate(gfp, rate);
        lame_set_brate(gfp, 128);
        if(channels == 2){
            lame_set_mode(gfp, STEREO);
        }else if(channels == 1){
            lame_set_mode(gfp, MONO);
        }else {
            lame_set_mode(gfp, NOT_SET);
        }
        lame_set_quality(gfp, 2);

        ret = lame_init_params(gfp);
        if(ret < 0){
            lame_close(gfp);
            return nullptr;
        }

        return gfp;
    }

    bool writePlanar(const float **samples, int num_samples)
    {
        if(!isOpen()){
            return false;
        }

        int num_bytes_output = 0;
        if(parent_->num_channels == 1){
            num_bytes_output = lame_encode_buffer_ieee_float(
                gfp_, samples[0], NULL, num_samples, mp3_buffer_.data(), mp3_buffer_.size());
        }else if(parent_->num_channels == 2){
            num_bytes_output = lame_encode_buffer_ieee_float(
                gfp_, samples[0], samples[1], num_samples, mp3_buffer_.data(), mp3_buffer_.size());
        }else {
            return false;
        }

        return parent_->out_stream_->write(mp3_buffer_.data(), num_bytes_output);
    }
    bool writeInterleave(const float *samples, int num_samples)
    {
        if(!isOpen()){
            return false;
        }

        int num_bytes_output = lame_encode_buffer_interleaved_ieee_float(
            gfp_, samples, num_samples, mp3_buffer_.data(), mp3_buffer_.size());
        return parent_->out_stream_->write(mp3_buffer_.data(), num_bytes_output);
    }
    void flush()
    {
        if(isOpen()){
            int num_bytes_output = lame_encode_flush(gfp_, mp3_buffer_.data(), mp3_buffer_.size());
            parent_->out_stream_->write(mp3_buffer_.data(), num_bytes_output);
        }
    }
    void close()
    {
        if(gfp_){

            lame_close(gfp_);
            gfp_ = nullptr;

            parent_->sample_rate = -1;
            parent_->num_channels = -1;
            parent_->num_bits = -1;
        }
    }

    bool isOpen() const  {
        return gfp_ != nullptr;
    }

    MP3FormatWriter* parent_ = nullptr;
    lame_global_flags *gfp_ = nullptr;
    std::vector<unsigned char> mp3_buffer_;
};

MP3FormatWriter::MP3FormatWriter(std::unique_ptr<OutputStream> out_stream,
                                 int rate,
                                 int channels,
                                 int bits):
    AudioFormatWriter(std::move(out_stream), rate, channels, bits),
    impl_(std::make_shared<Impl>(this, rate, channels, bits))
{
}

bool MP3FormatWriter::writePlanar(const float **samples, int num_samples)
{
    return impl_->writePlanar(samples, num_samples);
}
bool MP3FormatWriter::writeInterleave(const float *samples, int num_samples)
{
    return impl_->writeInterleave(samples, num_samples);
}
void MP3FormatWriter::flush()
{
    impl_->flush();
}
void MP3FormatWriter::close()
{
    return impl_->close();
}

bool MP3FormatWriter::isOpen() const
{
    return impl_->isOpen();
}


}
