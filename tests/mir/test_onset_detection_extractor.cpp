//
// Created by user on 6/28/22.
//
#include <gmock/gmock.h>

#include <Eigen/Dense>

#include "libaa/mir/aa_mir_algorithm.h"
#include "libaa/mir/aa_mir_algorithm_base.h"
#include "libaa/mir/aa_streaming_super_flux_extractor.h"

using namespace testing;
using namespace libaa;
using namespace libaa::MIR;

class ASuperFluxExtractor : public Test {
public:
    SuperFluxExtractor e;

    float sample_rate = 44100;
    size_t num_channels = 1;
};

TEST_F(ASuperFluxExtractor, CenReportCorrectName) {
    ASSERT_THAT(e.getName(), Eq("SuperFluxExtractor"));
}

TEST_F(ASuperFluxExtractor, CombineParameterAsExpected) {
    auto &p = e.getParameter("combine");

    ASSERT_THAT(p.getParameterName(), Eq("combine"));
    ASSERT_THAT(p.getParameterType(), Eq(ParameterType::kFloat));
    ASSERT_THAT(p.getPlainValue(), Eq(20));
    ASSERT_THAT(p.getMinPlainValue(), Eq(0));
    ASSERT_THAT(p.getMaxPlainValue(), Eq(std::numeric_limits<float>::max()));
}

TEST_F(ASuperFluxExtractor, FrameSizeParameterAsExpected) {
    auto &p = e.getParameter("frameSize");

    ASSERT_THAT(p.getParameterName(), Eq("frameSize"));
    ASSERT_THAT(p.getParameterType(), Eq(ParameterType::kInt));
    ASSERT_THAT(p.getInt(), Eq(2048));
    ASSERT_THAT(p.getMinPlainValue(), Eq(0));
    ASSERT_THAT(p.getMaxPlainValue(), Eq(std::numeric_limits<int>::max()));
}

TEST_F(ASuperFluxExtractor, HopSizeParameterAsExpected) {
    auto &p = e.getParameter("hopSize");

    ASSERT_THAT(p.getParameterName(), Eq("hopSize"));
    ASSERT_THAT(p.getParameterType(), Eq(ParameterType::kInt));
    ASSERT_THAT(p.getInt(), Eq(256));
    ASSERT_THAT(p.getMinPlainValue(), Eq(0));
    ASSERT_THAT(p.getMaxPlainValue(), Eq(std::numeric_limits<int>::max()));
}

TEST_F(ASuperFluxExtractor, RatioThresholdParameterAsExpected) {
    auto &p = e.getParameter("ratioThreshold");

    ASSERT_THAT(p.getParameterName(), Eq("ratioThreshold"));
    ASSERT_THAT(p.getParameterType(), Eq(ParameterType::kFloat));
    ASSERT_THAT(p.getPlainValue(), Eq(16));
    ASSERT_THAT(p.getMinPlainValue(), Eq(0));
    ASSERT_THAT(p.getMaxPlainValue(), Eq(std::numeric_limits<float>::max()));
}

TEST_F(ASuperFluxExtractor, ThresholdParameterAsExpected) {
    auto &p = e.getParameter("threshold");

    ASSERT_THAT(p.getParameterName(), Eq("threshold"));
    ASSERT_THAT(p.getParameterType(), Eq(ParameterType::kFloat));
    ASSERT_THAT(p.getPlainValue(), FloatNear(0.05f, 1e-6));
    ASSERT_THAT(p.getMinPlainValue(), Eq(0));
    ASSERT_THAT(p.getMaxPlainValue(), Eq(std::numeric_limits<float>::max()));
}

TEST_F(ASuperFluxExtractor, ConfigureInternalWindowing) {

    e.initialize(sample_rate, num_channels);
    int expected = 512;
    e.setParameter("frameSize", expected);

    e.configure();

    ASSERT_THAT(e.getWindowing()->getParameter("size").getInt(), Eq(expected));
    ASSERT_THAT(e.getWindowing()->getParameter("type").getChoiceString(), Eq("hann"));
}

TEST_F(ASuperFluxExtractor, ConfigureInternalWindowingOutput) {
    e.initialize(sample_rate, num_channels);

    e.configure();

    ASSERT_THAT(e.getWindowing()->output("frame").getRawData(), NotNull());
}

TEST_F(ASuperFluxExtractor, ConfigureInternalSpectrum) {
    e.initialize(sample_rate, num_channels);
    int expected = 512;
    e.setParameter("frameSize", expected);

    e.configure();

    ASSERT_THAT(e.getSpectrum()->getParameter("size").getInt(), Eq(expected));
}

TEST_F(ASuperFluxExtractor, ConfigureInternalSpectrumInputAndOutput) {
    e.initialize(sample_rate, num_channels);

    e.configure();

    ASSERT_THAT(e.getSpectrum()->input("frame").getRawData(), NotNull());
    ASSERT_THAT(e.getSpectrum()->output("spectrum").getRawData(), NotNull());
}

TEST_F(ASuperFluxExtractor, ConfigureInternalTriangluarBands) {
    e.initialize(sample_rate, num_channels);

    int frame_size = 512;
    int expected = frame_size / 2 + 1; // half frame_size
    e.setParameter("frameSize", frame_size);
    e.configure();

    ASSERT_THAT(e.getTriangularBands()->getParameter("log").getBool(), Eq(true));
    ASSERT_THAT(e.getTriangularBands()->getParameter("inputSize").getInt(), Eq(expected));
}

TEST_F(ASuperFluxExtractor, ConfigureInternalTriangluarBandsInputAndOutput) {
    e.initialize(sample_rate, num_channels);

    e.configure();

    ASSERT_THAT(e.getTriangularBands()->input("spectrum").getRawData(), NotNull());
    ASSERT_THAT(e.getTriangularBands()->output("bands").getRawData(), NotNull());
}

TEST_F(ASuperFluxExtractor, ConfigureInternalSuperFluxNovelty) {
    e.initialize(sample_rate, num_channels);

    e.configure();

    ASSERT_THAT(e.getStreamingSuperFluxNovelty()->getParameter("binWidth").getInt(), Eq(8));
    ASSERT_THAT(e.getStreamingSuperFluxNovelty()->getParameter("frameWidth").getInt(), Eq(2));
}

TEST_F(ASuperFluxExtractor, ConfigureInternalSuperFluxNoveltyInputAndOutput) {
    e.initialize(sample_rate, num_channels);

    e.configure();

    ASSERT_THAT(e.getStreamingSuperFluxNovelty()->input("bands").getRawData(), NotNull());
    ASSERT_THAT(e.getStreamingSuperFluxNovelty()->output("differences").getRawData(), NotNull());
}

TEST_F(ASuperFluxExtractor, ConfigureInternalSuperFluxPeaks) {
    e.initialize(sample_rate, num_channels);
    float expected_combine = 200;
    float expected_threshold = 0.1;
    float expected_ratio_threshold = 20;
    int hop_size = 128;
    float expected_frame_rate = sample_rate / 128;
    e.setParameter("combine", expected_combine);
    e.setParameter("threshold", expected_threshold);
    e.setParameter("ratioThreshold", expected_ratio_threshold);
    e.setParameter("hopSize", hop_size);

    e.configure();

    ASSERT_THAT(e.getStreamingSuperFluxPeaks()->getParameter("combine").getPlainValue(), Eq(expected_combine));
    ASSERT_THAT(e.getStreamingSuperFluxPeaks()->getParameter("threshold").getPlainValue(), FloatNear(expected_threshold, 1e-6));
    ASSERT_THAT(e.getStreamingSuperFluxPeaks()->getParameter("ratioThreshold").getPlainValue(), Eq(expected_ratio_threshold));
    ASSERT_THAT(e.getStreamingSuperFluxPeaks()->getParameter("frameRate").getPlainValue(), Eq(expected_frame_rate));
}

TEST_F(ASuperFluxExtractor, ConfigureInternalSuperFluxPeaksInputAndOutput) {
    e.initialize(sample_rate, num_channels);

    e.configure();

    ASSERT_THAT(e.getStreamingSuperFluxPeaks()->input("novelty").getRawData(), NotNull());
    ASSERT_THAT(e.getStreamingSuperFluxPeaks()->output("peaks").getRawData(), NotNull());
}

TEST_F(ASuperFluxExtractor, ConfigureInitInternalRingbuffersBasedOnFrameSizeAndHopSize) {
    e.initialize(sample_rate, num_channels);

    int expected_block_size = 1024;
    int expected_step_size = 128;
    e.setParameter("frameSize", expected_block_size);
    e.setParameter("hopSize", expected_step_size);
    e.configure();

    ASSERT_THAT(e.getBlockSize(), Eq(expected_block_size));
    ASSERT_THAT(e.getStepSize(), Eq(expected_step_size));
}

TEST_F(ASuperFluxExtractor, ProcessCanGetPeaksAsExpeced) {
    int num_channel = 1;
    int block_size = 8;
    int hop_size = 4;
    sample_rate = 16;
    e.initialize(sample_rate, num_channel);
    e.setParameter("frameSize", block_size);
    e.setParameter("hopSize", hop_size);
    Eigen::ArrayXf r = Eigen::ArrayXf::Random(2048);
    std::vector<float *> data_refer_to{r.data()};

    e.configure();
    e.process(data_refer_to.data(), r.size(), true);

    auto feature = e.getFrameFeatures();
    ASSERT_THAT(feature.empty(), Eq(false));
}