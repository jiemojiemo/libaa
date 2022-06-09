//
// Created by user on 6/6/22.
//
#include "libaa/processor/aa_channel_vocoder_utilities.h"
#include <gmock/gmock.h>
#include <numeric>

using namespace testing;
using namespace std;

using namespace libaa;
class APhaseVocoderUtilities : public Test {
public:
    PhaseVocoderSetting setting;
};

TEST_F(APhaseVocoderUtilities, InitWithSetting) {
    setting.window_size = 512;
    ChannelVocoderUtilities u(setting);

    ASSERT_THAT(u.setting, Eq(setting));
}

TEST_F(APhaseVocoderUtilities, InitWithSettingAllocateInputBufferWithWindowSize) {
    ChannelVocoderUtilities u(setting);

    ASSERT_THAT(u.input_modular_buffer.size(), Eq(setting.window_size));
}

TEST_F(APhaseVocoderUtilities, InitWithSettingAllocateOutputBufferWithWindowSize) {
    ChannelVocoderUtilities u(setting);

    ASSERT_THAT(u.output_buffer.size(), Eq(setting.window_size));
}

TEST_F(APhaseVocoderUtilities, InitWitSettingAllocateFFT) {
    ChannelVocoderUtilities u(setting);

    ASSERT_THAT(u.fft.getSize(), Eq(setting.window_size));
}

TEST_F(APhaseVocoderUtilities, InitWithSettingUpdateScale) {
    ChannelVocoderUtilities u(setting);

    ASSERT_THAT(u.overlap_add_scale, Eq(float(setting.hop_size) / setting.window_size));
}

TEST_F(APhaseVocoderUtilities, InputBufferWritePointerIsZeroWhenInit) {
    ChannelVocoderUtilities u(setting);

    ASSERT_THAT(u.input_write_pointer, Eq(0));
}

TEST_F(APhaseVocoderUtilities, OutputBufferWritePointerIsZeroWhenInit) {
    ChannelVocoderUtilities u(setting);

    ASSERT_THAT(u.output_write_pointer, Eq(0));
}

TEST_F(APhaseVocoderUtilities, OutputReadPointerIsZeroWhenInit) {
    ChannelVocoderUtilities u(setting);

    ASSERT_THAT(u.output_read_pointer, Eq(0));
}

TEST_F(APhaseVocoderUtilities, HopCounterIsNegHopSizeWhenInit) {
    ChannelVocoderUtilities u(setting);

    ASSERT_THAT(u.hop_counter, Eq(-setting.hop_size));
}

TEST_F(APhaseVocoderUtilities, FFTSizeEqualeWindowSize) {
    ChannelVocoderUtilities u(setting);

    ASSERT_THAT(u.fft_size, Eq(setting.window_size));
}

TEST_F(APhaseVocoderUtilities, BandWidthCalculatCorreclyWhenInit) {
    ChannelVocoderUtilities u(setting);

    ASSERT_THAT(u.band_width, Eq(ceil(setting.window_size / 2 / setting.num_band)));
}

TEST_F(APhaseVocoderUtilities, CanCalcAverageValue) {
    ChannelVocoderUtilities u(setting);
    vector<float> data{1, 2, 3, 4, 5, 6};

    float x = u.average(data, 0, 4);

    ASSERT_THAT(x, Eq(2.5f));
}

TEST_F(APhaseVocoderUtilities, ProcessSamplesIncreaseHopCounter) {
    ChannelVocoderUtilities u(setting);
    auto init_hop_counter = u.hop_counter;
    int num_samples = 10;
    vector<float> data(num_samples, 0);

    u.process(data.data(),
              data.data(),
              data.data(),
              data.size());

    ASSERT_THAT(u.hop_counter, Eq(init_hop_counter + num_samples));
}

TEST_F(APhaseVocoderUtilities, HopCounterModuleToHopSize) {
    setting.window_size = 8;
    setting.hop_size = 4;
    ChannelVocoderUtilities u(setting);
    auto init_hop_counter = u.hop_counter;
    int num_samples = 10;
    vector<float> data(num_samples, 0);

    u.process(data.data(),
              data.data(),
              data.data(),
              data.size());

    ASSERT_THAT(u.hop_counter, Eq((init_hop_counter + num_samples) % setting.hop_size));
}

TEST_F(APhaseVocoderUtilities, CanUpdateNumberBand) {
    ChannelVocoderUtilities u(setting);

    int num_band = 100;
    u.updateNumberBand(100);

    ASSERT_THAT(u.setting.num_band, Eq(num_band));
}

TEST_F(APhaseVocoderUtilities, UpdateNumberBandReCalcBandWidth) {
    ChannelVocoderUtilities u(setting);

    int num_band = 100;
    u.updateNumberBand(100);

    ASSERT_THAT(u.band_width, Eq(ceil(setting.window_size / 2 / num_band)));
}