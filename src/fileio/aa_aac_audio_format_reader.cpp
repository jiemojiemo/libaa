
//
// Created by William.Hua on 2021/3/24.
//

#include "libaa/fileio/aa_aac_audio_format_reader.h"
#include "libaa/fileio/aa_file_input_stream.h"
#include "aacdecoder_lib.h"
#include <numeric>
#include <iostream>
#include <vector>

namespace libaa
{
class AACAudioFormatReader::Impl
{
public:
    Impl(AACAudioFormatReader* parent):
        parent_(parent)
    {

    }

    bool isOpenOk()  {
        return handle_ != nullptr;
    }

    void seekToPCMFrame(int64_t start_offset_of_file)
    {
        (void)(start_offset_of_file);
        auto package_index = start_offset_of_file / frame_size;

        // prerolling 4 packet to make it have a reliable re-sync
        const int kNumPreroll = 4;
        int preroll_pkg_begin = package_index - kNumPreroll;
        int preroll_pkg_end = package_index - 1;

        if(preroll_pkg_begin < 0)
        {
            preroll_pkg_begin = 0;
        }

        parent_->in_stream_->seekg(packet_offset_array_[preroll_pkg_begin], SEEK_SET);

        for(;preroll_pkg_begin < preroll_pkg_end; ++preroll_pkg_begin)
        {
            readPacketAndDecode();
        }

        decode_offset_when_seek_ = start_offset_of_file - (package_index * frame_size);
    }

    bool readSamples(float **dest_channels,
                                           int num_dest_channels,
                                           int start_offset_of_dest,
                                           int64_t start_offset_of_file,
                                           int num_samples){
        /**
         * TODO: optimize this, check the start_offset_of_file whether decoded already.
         */
        if(parent_->pos_ != start_offset_of_file){
            seekToPCMFrame(start_offset_of_file);
            parent_->pos_ = start_offset_of_file;
        }

        num_dest_channels = std::min(num_dest_channels, parent_->num_channels);
        int num_fill = 0;
        for(;;)
        {
            if(readPacketAndDecode()){
                for(int i = 0; i < frame_size; ++i){
                    for(int c = 0; c < num_dest_channels; ++c){
                        dest_channels[c][start_offset_of_dest + i] = decode_buf[(i + decode_offset_when_seek_)*parent_->num_channels + c] / 32768.0;
                    }
                    ++num_fill;

                    // read all samples you want
                    if(num_fill >= num_samples){
                        return true;
                    }
                }

                decode_offset_when_seek_ = 0;
            }else{
                // read eof
                return false;
            }
        }
    }

    bool openAACDecoder()
    {
        handle_ = aacDecoder_Open(TT_MP4_ADTS, 1);
        if(!handle_){
            return false;
        }

        initBuffer();

        if(!getAudioInfo()){
            return false;
        }

        if(!getEachPacketSize()){
            return false;
        }

        return true;
    }

    bool getAudioInfo(){
        // decode one packet to get basic audio information
        if(readPacketAndDecode()){
            CStreamInfo* info = aacDecoder_GetStreamInfo(handle_);
            if (!info || info->sampleRate <= 0) {
                fprintf(stderr, "No stream info\n");
                return false;
            }

            parent_->num_channels = info->numChannels;
            parent_->sample_rate = info->sampleRate;
            parent_->length_in_samples = 0;
            parent_->num_bits = 16;

            frame_size = info->frameSize;

            return true;
        }

        return false;
    }

    bool getEachPacketSize()
    {
        parent_->in_stream_->seekg(0, SEEK_SET);
        for(;;)
        {
            int num_read = 0;
            num_read = parent_->in_stream_->read(packet.data(), kPacketHeadSize);
            // read eof
            if (num_read != kPacketHeadSize) {
                break;
            }

            // check if ADTS packet
            if (packet[0] != 0xff || (packet[1] & 0xf0) != 0xf0) {
                return false;
            }

            UINT packet_size = ((packet[3] & 0x03) << 11) | (packet[4] << 3) | (packet[5] >> 5);
            packet_size_array_.push_back(packet_size);

            // skip the content
            parent_->in_stream_->seekg(packet_size - kPacketHeadSize, SEEK_CUR);
        }

        parent_->length_in_samples = packet_size_array_.size() * frame_size;

        packet_offset_array_.resize(packet_size_array_.size() + 1);
        packet_offset_array_[0] = 0;
        std::partial_sum(packet_size_array_.begin(), packet_size_array_.end(), packet_offset_array_.begin() + 1);

        // clear internal data
        clearInternalData();

        // reset current_packet
        current_packet_ = 0;
        parent_->in_stream_->seekg(0, SEEK_SET);

        return true;
    }

    void clearInternalData()
    {
        aacDecoder_DecodeFrame(handle_, decode_buf.data(), kDecodeBufferSize, AACDEC_INTR|AACDEC_FLUSH|AACDEC_CLRHIST);
    }

    void closeAACDecoder(){
        if(handle_){
            aacDecoder_Close(handle_);
            handle_ = nullptr;
        }
    }

    void initBuffer(){
        packet.resize(kPacketBufferSize);
        decode_buf.resize(kDecodeBufferSize);
    }

    bool readPacketAndDecode(){
        // read a packet to get input audio basic information
        AAC_DECODER_ERROR err = AAC_DEC_OK;
        auto n = parent_->in_stream_->read(packet.data(), kPacketHeadSize);
        if(n != 7){
            std::cerr << "cannot read from input stream\n";
            return false;
        }

        if(packet[0] != 0xff || (packet[1] & 0xf0) != 0xf0){
            std::cerr << "Not an ADTS packet\n";
            return false;
        }

        uint32_t packet_size = ((packet[3] & 0x03) << 11) | (packet[4] << 3) | (packet[5] >> 5);
        n = parent_->in_stream_->read(packet.data() + kPacketHeadSize, packet_size - kPacketHeadSize);
        if (n != packet_size - kPacketHeadSize) {
            fprintf(stderr, "Partial packet\n");
            return false;
        }

        uint32_t valid = packet_size;
        uint8_t* ptr = packet.data();
        err = aacDecoder_Fill(handle_, &ptr, &packet_size, &valid);
        if (err != AAC_DEC_OK) {
            fprintf(stderr, "Fill failed: %x\n", err);
            return false;
        }

        err = aacDecoder_DecodeFrame(handle_, decode_buf.data(), decode_buf.size(), 0);
        if (err == AAC_DEC_NOT_ENOUGH_BITS)
            return false;
        if (err != AAC_DEC_OK) {
            fprintf(stderr, "Decode failed: %x\n", err);
            return false;
        }

        return true;
    }

    AACAudioFormatReader* parent_;

    HANDLE_AACDECODER handle_{nullptr};
    constexpr static int kPacketHeadSize = 7;
    constexpr static int kPacketBufferSize = 10240;
    constexpr static int kDecodeBufferSize = 8 * 2048;
    std::vector<uint8_t> packet;
    std::vector<INT_PCM> decode_buf;
    std::vector<int> packet_size_array_;
    std::vector<int> packet_offset_array_;
    int current_packet_{0};
    int frame_size{0};
    int decode_offset_when_seek_{0};
};


AACAudioFormatReader::AACAudioFormatReader(std::unique_ptr<InputStream> in_stream):
    AudioFormatReader(std::move(in_stream)),
    impl_(std::make_shared<Impl>(this))
{
    auto ok = impl_->openAACDecoder();
    if(!ok){
        impl_->closeAACDecoder();
    }
}

bool AACAudioFormatReader::isOpenOk()  {
    return impl_->isOpenOk();
}

bool AACAudioFormatReader::readSamples(float **dest_channels,
                 int num_dest_channels,
                 int start_offset_of_dest,
                 int64_t start_offset_of_file,
                 int num_samples)  {
    return impl_->readSamples(dest_channels,num_dest_channels,start_offset_of_dest,
                              start_offset_of_file, num_samples);
}
}
