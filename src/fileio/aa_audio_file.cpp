//
// Created by william on 2019/11/27.
//

#include "libaa/fileio/aa_audio_file.h"
#include "dr_wav.h"
#include "libaa/fileio/aa_audio_decoder_factory.h"
#include "libaa/fileio/aa_wav_decoder.h"
#include <iostream>

namespace libaa {

class AudioFile::Impl {
public:
    int load(const std::string &filename) {
        std::string file_ext = filename.substr(filename.find(".") + 1);

        if (file_ext == ".wav") {
            loadWaveFile(filename);
        }

        return 0;
    }

    int loadWaveFile(const std::string &filename) {
        (void)filename;
        return 0;
    }

    std::unique_ptr<AudioDecoder> createDecoder(const std::string &filename) {
        std::string::size_type idx = filename.rfind('.');
        std::string extension;
        if (idx != std::string::npos) {
            std::string extension = filename.substr(idx + 1);
        } else {
            extension = "Unsupported";
        }

        if (extension == "wav") {
            return std::make_unique<WavDecoder>();
        }

        std::cerr << "Unsupport " << extension << " audio format\n";
        return nullptr;
    }

    std::unique_ptr<AudioDecoder> decoder;
};

AudioFile::AudioFile() : impl_(std::make_shared<Impl>()) {}

int AudioFile::load(const std::string &filename) {
    impl_->decoder = AudioDecoderFactory::createDecoder(filename);
    if (impl_->decoder == nullptr) {
        return -1;
    }

    int failed = impl_->decoder->open(filename);
    if (failed) {
        return -1;
    }

    // get audio information from decoder
    num_channels_ = impl_->decoder->getNumChannels();
    sample_rate_ = impl_->decoder->getSampleRate();
    num_bits_ = impl_->decoder->getBits();
    int num_frames = impl_->decoder->getNumFrames();

    std::vector<float> fbuffer(num_frames * num_channels_);
    auto num_decoded = impl_->decoder->read(fbuffer.data(), fbuffer.size());

    // interleave to planar
    samples.resize(num_channels_);
    for (auto &s : samples) {
        s.resize(num_frames);
    }

    for (size_t c = 0; c < num_channels_; ++c) {
        for (int i = 0; i < num_decoded; ++i) {
            samples[c][i] = fbuffer[i * num_channels_ + c];
        }
    }

    return 0;
}
int AudioFile::saveToWave(const std::string &save_path) const {
    drwav_data_format format;
    drwav wav;
    format.container =
        drwav_container_riff; // <-- drwav_container_riff = normal WAV files,
                              // drwav_container_w64 = Sony Wave64.
    format.format =
        DR_WAVE_FORMAT_PCM; // <-- Any of the DR_WAVE_FORMAT_* codes.
    format.channels = getNumChannels();
    format.sampleRate = getSampleRate();
    format.bitsPerSample = getNumBits();
    drwav_init_file_write(&wav, save_path.c_str(), &format, NULL);

    std::vector<int16_t> data_s16_interleave(getNumChannels() * getNumFrames());

    int r = 0;
    for (size_t c = 0; c < getNumChannels(); ++c) {
        for (size_t i = 0; i < getNumFrames(); ++i) {
            float x = samples[c][i];
            float xx = 0.0;
            xx = ((x < -1) ? -1 : ((x > 1) ? 1 : x));
            xx = xx + 1;
            r = (int)(xx * 32767.5f);
            r = r - 32768;
            data_s16_interleave[i * getNumChannels() + c] = (short)r;
        }
    }

    drwav_uint64 framesWritten = drwav_write_pcm_frames(
        &wav, getNumFrames(), data_s16_interleave.data());

    drwav_uninit(&wav);
    return framesWritten;
}
void AudioFile::setChannelData(size_t channel, float *data, size_t data_len) {
    if (channel >= 2) {
        std::cerr
            << "channel out of bound: only support mono(0) or stereo(2)\n";
        return;
    }

    if (samples.size() < channel) {
        samples.resize(channel);
    }

    for (auto &x : samples) {
        if (x.size() < data_len) {
            x.resize(data_len);
        }

        setNumFrames(data_len);
    }

    std::copy(data, data + data_len, samples[channel].begin());
}

} // namespace libaa