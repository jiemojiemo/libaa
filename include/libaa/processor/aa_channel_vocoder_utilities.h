//
// Created by user on 6/9/22.
//

#ifndef LIBAA_AA_CHANNEL_VOCODER_UTILITIES_H
#define LIBAA_AA_CHANNEL_VOCODER_UTILITIES_H

#pragma once
#include "dsp/FFT.h"
#include "libaa/dsp/aa_window.h"
#include <fstream>
namespace libaa {
struct PhaseVocoderSetting {
    int num_band = 32;
    int window_size = 1024;
    int hop_size = 512;
    WindowType window_type = WindowType::kHann;
};

bool operator==(const PhaseVocoderSetting &lhs, const PhaseVocoderSetting &rhs);

class ChannelVocoderUtilities {
public:
    ChannelVocoderUtilities(PhaseVocoderSetting setting);

    ~ChannelVocoderUtilities() = default;

    void process(const float *modular_data,
                 const float *carrier_data,
                 float *output_data,
                 size_t data_size);

    float processSample(float modular_in,
                        float carrier_in);

    void updateNumberBand(int num_band);

    static float average(const std::vector<float> &data, int begin, int end);

    PhaseVocoderSetting setting;
    int fft_size = 0;
    int band_width = 0;
    RubberBand::FFT fft;
    int buffer_size = 0;
    std::vector<float> input_modular_buffer;
    std::vector<float> input_carrier_buffer;
    std::vector<float> output_buffer;
    std::vector<float> window;
    float overlap_add_scale{0.0f};
    float ifft_scale{0.0f};
    int hop_counter = 0;
    int input_write_pointer = 0;
    int output_write_pointer = 0;
    int output_read_pointer = 0;

    std::vector<float> unwrapped_modular_buffer;
    std::vector<float> unwrapped_carrier_buffer;
    std::vector<float> modular_fft_mag_buffer;
    std::vector<float> carrier_fft_real_buffer;
    std::vector<float> carrier_fft_imag_buffer;
    std::vector<float> carrier_fft_mag_buffer;
    std::vector<float> ifft_buffer;

private:
    void process_fft();
    void unwrap_windowing();
    void transform();
};

} // namespace libaa

#endif // LIBAA_AA_CHANNEL_VOCODER_UTILITIES_H
