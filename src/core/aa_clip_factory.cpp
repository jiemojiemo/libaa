//
// Created by user on 6/6/22.
//

#include "libaa/core/aa_clip_factory.h"
#include "libaa/core/aa_resource_path_finder.h"
#include "libaa/fileio/aa_file_input_stream.h"
#include "libaa/fileio/aa_wav_audio_format_reader.h"
namespace libaa {
namespace ClipFactory {
std::unique_ptr<FileInputStream> openFileStream(const std::string &audio_file_uri,
                                                ResourcePathFinder *finder) {
    if (finder != nullptr) {
        auto audio_file_path = finder->find(audio_file_uri);
        return std::make_unique<FileInputStream>(audio_file_path);
    } else {
        return std::make_unique<FileInputStream>(audio_file_uri);
    }
}

AudioBlock buildBlockFromReader(AudioFormatReader *reader) {
    auto num_channels = reader->num_channels;
    auto num_frames = reader->length_in_samples;

    AudioBlock block(num_channels, num_frames, 0);
    std::vector<float *> data_refer_to(num_channels);
    for (auto c = 0; c < num_channels; ++c) {
        data_refer_to[c] = block.buffer.getWriterPointer(c);
    }

    reader->readSamples(data_refer_to.data(), num_channels, 0, 0, num_frames);

    return block;
}

std::unique_ptr<Clip> buildClip(const std::string &audio_file_uri,
                                bool loop_or_not,
                                ResourcePathFinder *finder) {
    std::unique_ptr<FileInputStream> in_stream = openFileStream(audio_file_uri, finder);
    if (in_stream->isOpen()) {
        auto reader = std::make_unique<WaveAudioFormatReader>(std::move(in_stream));
        return std::make_unique<Clip>(buildBlockFromReader(reader.get()), loop_or_not, audio_file_uri);
    } else {
        return nullptr;
    }
}
} // namespace ClipFactory
} // namespace libaa
