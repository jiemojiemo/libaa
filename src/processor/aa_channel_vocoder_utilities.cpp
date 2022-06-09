//
// Created by user on 6/9/22.
//

#include "libaa/processor/aa_channel_vocoder_utilities.h"
#include <numeric>

namespace libaa {
bool operator==(const PhaseVocoderSetting &lhs, const PhaseVocoderSetting &rhs) {
    return lhs.num_band == rhs.num_band &&
           lhs.window_size == rhs.window_size &&
           lhs.hop_size == rhs.hop_size &&
           lhs.window_type == rhs.window_type;
}

ChannelVocoderUtilities::ChannelVocoderUtilities(PhaseVocoderSetting setting)
    : setting(setting),
      fft_size(setting.window_size),
      band_width(ceil(fft_size / 2 / setting.num_band)),
      fft(fft_size),
      buffer_size(setting.window_size),
      input_modular_buffer(buffer_size, 0),
      input_carrier_buffer(buffer_size, 0),
      output_buffer(buffer_size, 0),
      window(Window::getWindow(setting.window_type, fft_size, true)),
      overlap_add_scale(float(setting.hop_size) / setting.window_size),
      ifft_scale(1.0f / setting.window_size),
      hop_counter(-setting.hop_size),
      unwrapped_modular_buffer(fft_size, 0),
      unwrapped_carrier_buffer(fft_size, 0),
      modular_fft_mag_buffer(fft_size / 2 + 1, 0),
      carrier_fft_real_buffer(fft_size / 2 + 1, 0),
      carrier_fft_imag_buffer(fft_size / 2 + 1, 0),
      carrier_fft_mag_buffer(fft_size / 2 + 1, 0),
      ifft_buffer(setting.window_size, 0) {
    fft.initFloat();
}

void ChannelVocoderUtilities::process(const float *modular_data,
                                      const float *carrier_data,
                                      float *output_data,
                                      size_t data_size) {
    for (auto i = 0u; i < data_size; ++i) {
        output_data[i] = processSample(modular_data[i],
                                       carrier_data[i]);
    }
}

float ChannelVocoderUtilities::processSample(float modular_in,
                                             float carrier_in) {

    // push in to input circular buffer
    // increase input write pointer
    input_modular_buffer[input_write_pointer] = modular_in;
    input_carrier_buffer[input_write_pointer] = carrier_in;
    input_write_pointer++;
    if (input_write_pointer >= buffer_size) {
        input_write_pointer = 0;
    }

    float out = output_buffer[output_read_pointer];
    // reset to 0, so it is ready for the next overlap-add
    output_buffer[output_read_pointer] = 0;

    // rescale output with overlap-add scale
    out *= overlap_add_scale;

    // increase read pointer in output circular buffer
    if (++output_read_pointer >= buffer_size) {
        output_read_pointer = 0;
    }

    if (++hop_counter >= static_cast<int64_t>(setting.hop_size)) {
        hop_counter = 0;

        process_fft();

        output_write_pointer = (output_write_pointer + setting.hop_size) % buffer_size;
    }

    return out;
}

void ChannelVocoderUtilities::updateNumberBand(int num_band) {
    setting.num_band = num_band;
    band_width = ceil(fft_size / 2 / setting.num_band);
}

float ChannelVocoderUtilities::average(const std::vector<float> &data, int begin, int end) {
    float count = static_cast<float>(end - begin);
    return std::reduce(data.data() + begin, data.data() + end) / count;
}

void ChannelVocoderUtilities::process_fft() {
    unwrap_windowing();

    fft.forwardMagnitude(unwrapped_modular_buffer.data(), modular_fft_mag_buffer.data());
    fft.forward(unwrapped_carrier_buffer.data(), carrier_fft_real_buffer.data(), carrier_fft_imag_buffer.data());
    fft.forwardMagnitude(unwrapped_carrier_buffer.data(), carrier_fft_mag_buffer.data());

    transform();

    fft.inverse(carrier_fft_real_buffer.data(), carrier_fft_imag_buffer.data(), ifft_buffer.data());

    // overlap-add
    for (auto i = 0; i < fft_size; ++i) {
        int circular_buffer_index = (output_write_pointer + i) % buffer_size;
        output_buffer[circular_buffer_index] += (ifft_buffer[i] * ifft_scale);
    }
}

void ChannelVocoderUtilities::unwrap_windowing() {
    // unwrap input_buffer to unwrapped_buffer
    // unwrap means copy
    for (auto i = 0; i < fft_size; ++i) {
        int circular_buffer_index = (input_write_pointer + i - fft_size + buffer_size) % buffer_size;
        unwrapped_modular_buffer[i] = input_modular_buffer[circular_buffer_index] * window[i];
        unwrapped_carrier_buffer[i] = input_carrier_buffer[circular_buffer_index] * window[i];
    }
}

void ChannelVocoderUtilities::transform() {
    // transform...
    const auto fft_middle_index = fft_size / 2;
    const auto band_with_cache = band_width;
    const auto num_band_cache = setting.num_band;

    for (auto b = 0; b < num_band_cache; ++b) {
        // calc modular mag mean value in this band range [band_begin, bend_end)
        auto band_begin = b * band_with_cache;
        auto band_end = band_begin + band_with_cache;
        if (band_end >= fft_middle_index) {
            band_end = fft_middle_index + 1;
        }
        float mean_modular_mag_value = average(modular_fft_mag_buffer, band_begin, band_end);

        // multiple carrier fft with mean_modular_mag_value
        // normalize with carrier mag value
        float mean_carrier_mag_value = average(carrier_fft_mag_buffer, band_begin, band_end);
        for (auto j = band_begin; j < band_end; ++j) {
            carrier_fft_real_buffer[j] *= (mean_modular_mag_value / mean_carrier_mag_value);
            carrier_fft_imag_buffer[j] *= (mean_modular_mag_value / mean_carrier_mag_value);
        }
    }
}

} // namespace libaa
