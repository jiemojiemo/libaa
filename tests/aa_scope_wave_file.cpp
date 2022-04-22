
//
// Created by William.Hua on 2021/2/9.
//
#include "aa_testing_helper.h"
#include "libaa/fileio/aa_audio_file.h"
#include "libaa/fileio/aa_file_output_stream.h"
#include "libaa/fileio/aa_wav_audio_format_writer.h"
#include <fstream>
using namespace std;
namespace libaa {
ScopeFile::ScopeFile(std::string output_path)
    : output_path_(std::move(output_path)) {}

ScopeFile::~ScopeFile() {
    std::remove(output_path_.c_str());
}

ScopeWaveFile::ScopeWaveFile(std::string output_path, size_t sample_rate,
                             size_t num_channel, size_t num_frames,
                             size_t num_bits, float fill_val)
    : ScopeFile(std::move(output_path)) {

    auto out_stream = std::make_unique<FileOutputStream>(output_path_);
    WavFormatWriter writer(std::move(out_stream), sample_rate, num_channel,
                           num_bits);

    assert(writer.isOpen() == true);

    vector<vector<float>> samples(num_channel);
    vector<const float *> data_refer_to(num_channel);
    for (auto c = 0u; c < num_channel; ++c) {
        samples[c].resize(num_frames);
        std::fill(samples[c].begin(), samples[c].end(), fill_val);

        data_refer_to[c] = samples[c].data();
    }

    writer.writePlanar(data_refer_to.data(), num_frames);
    writer.flush();
    writer.close();
}

ScopeTextFile::ScopeTextFile(std::string output_path, const std::string &txt)
    : ScopeFile(std::move(output_path)) {
    std::ofstream out(output_path_, std::ios::out);
    out << txt;
    out.close();
}

} // namespace libaa
