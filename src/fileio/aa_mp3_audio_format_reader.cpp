
//
// Created by William.Hua on 2021/2/23.
//

#include "libaa/fileio/aa_mp3_audio_format_reader.h"
#include "dr_mp3.h"
#include <memory>
#include <vector>
namespace libaa {
class Mp3AudioFormatReader::Impl {
public:
    Impl(Mp3AudioFormatReader *parent)
        : parent_(parent) {
        openMp3FromStream();
    }

    ~Impl() {
        close();
    }

    void close() {
        if (isOpenOk()) {
            drmp3_uninit(mp3_.get());

            parent_->sample_rate = -1;
            parent_->num_channels = -1;
            parent_->num_bits = -1;
        }
    }

    bool isOpenOk() const {
        return mp3_ != nullptr;
    }

    void openMp3FromStream() {
        mp3_ = std::make_unique<drmp3>();
        if (drmp3_init(mp3_.get(), readCallback, seekCallback, this, nullptr,
                       nullptr)) {
            parent_->sample_rate = mp3_->sampleRate;
            parent_->num_channels = mp3_->channels;
            parent_->length_in_samples = drmp3_get_pcm_frame_count(mp3_.get());
            parent_->num_bits = 16;
        } else {
            mp3_ = nullptr;
            close();
        }
    }

    bool readSamples(float **dest_channels, int num_dest_channels,
                     int start_offset_of_dest, int64_t start_offset_of_file,
                     int num_samples) {
        if (parent_->pos_ != start_offset_of_file) {
            if (drmp3_seek_to_pcm_frame(mp3_.get(), start_offset_of_file)) {
                parent_->pos_ = start_offset_of_file;
            }
        }

        num_dest_channels = std::min(num_dest_channels, parent_->num_channels);
        interleave_buffer_.resize(num_dest_channels * num_samples);

        auto num_read = drmp3_read_pcm_frames_f32(mp3_.get(), num_samples,
                                                  interleave_buffer_.data());

        // interleave to planar
        for (drmp3_uint64 i = start_offset_of_dest, ii = 0; i < num_read; ++i) {
            for (auto c = 0; c < num_dest_channels; ++c) {
                dest_channels[c][i] = interleave_buffer_[ii++];
            }
        }

        return true;
    }

    static size_t readCallback(void *pUserData, void *pBufferOut,
                               size_t bytesToRead) {
        auto *self = (Mp3AudioFormatReader::Impl *)(pUserData);

        return self->parent_->in_stream_->read((uint8_t *)pBufferOut,
                                               bytesToRead);
    }

    static drmp3_bool32 seekCallback(void *pUserData, int offset,
                                     drmp3_seek_origin origin) {
        auto *self = (Mp3AudioFormatReader::Impl *)(pUserData);
        if (origin == drmp3_seek_origin_current) {
            if (self->parent_->in_stream_->seekg(offset, SEEK_CUR) != 0) {
                return DRMP3_FALSE;
            }

        } else if (origin == drmp3_seek_origin_start) {
            if (self->parent_->in_stream_->seekg(offset, SEEK_SET) != 0) {
                return DRMP3_FALSE;
            }
        }

        return DRMP3_TRUE;
    }

    Mp3AudioFormatReader *parent_;
    std::unique_ptr<drmp3> mp3_{nullptr};
    std::vector<float> interleave_buffer_;
};

Mp3AudioFormatReader::Mp3AudioFormatReader(
    std::unique_ptr<InputStream> in_stream)
    : AudioFormatReader(std::move(in_stream)),
      impl_(std::make_shared<Impl>(this)) {}

bool Mp3AudioFormatReader::isOpenOk() {
    return impl_->isOpenOk();
}

bool Mp3AudioFormatReader::readSamples(float **dest_channels,
                                       int num_dest_channels,
                                       int start_offset_of_dest,
                                       int64_t start_offset_of_file,
                                       int num_samples) {
    if (!isOpenOk()) {
        return false;
    }

    return impl_->readSamples(dest_channels, num_dest_channels,
                              start_offset_of_dest, start_offset_of_file,
                              num_samples);
}

} // namespace libaa
