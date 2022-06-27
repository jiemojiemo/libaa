//
// Created by user on 6/27/22.
//

#include "libaa/mir/aa_triangular_bands.h"

#include <Eigen/Dense>
#include <cmath>

namespace libaa::MIR {

class TriangularBands::Impl {
public:
    void createFilters() {
        const float nyquist_sr = sample_rate_ / 2.0f;

        // find max frequency that below nyquist sample rate
        auto findMaxSupportedFrequencyIndex = [this](float nyquist_sr) {
            for (int i = kFrequencies.size() - 1; i >= 0; --i) {
                if (nyquist_sr >= kFrequencies[i])
                    return i;
            }
            return 0;
        };

        auto freq_scale = (sample_rate_ / 2.0f) / (spectrum_size_ - 1);
        num_bands_ = findMaxSupportedFrequencyIndex(nyquist_sr) - 1;
        filters_ = Eigen::MatrixXf::Zero(num_bands_, spectrum_size_);

        for (int i = 0; i < num_bands_; ++i) {
            float fstep1 = kFrequencies[i + 1] - kFrequencies[i];
            float fstep2 = kFrequencies[i + 2] - kFrequencies[i + 1];

            // frequency --> bin index
            int jbegin = ceil(kFrequencies[i] / freq_scale);
            int jend = floor(kFrequencies[i + 2] / freq_scale);

            float weight = 0.0f;
            for (int j = jbegin; j <= jend; ++j) {
                float bin_freq = j * freq_scale;
                if (bin_freq < kFrequencies[i + 1]) {
                    filters_(i, j) = (bin_freq - kFrequencies[i]) / fstep1;
                } else {
                    filters_(i, j) = (kFrequencies[i + 2] - bin_freq) / fstep2;
                }
                weight += filters_(i, j);
            }

            // normalized
            if (weight != 0.0f) {
                filters_.row(i) /= weight;
            }
        }
    }

    const std::vector<float> kFrequencies{21.533203125, 43.06640625, 64.599609375, 86.1328125, 107.666015625, 129.19921875, 150.732421875, 172.265625, 193.798828125, 215.33203125, 236.865234375, 258.3984375, 279.931640625, 301.46484375, 322.998046875, 344.53125, 366.064453125, 387.59765625, 409.130859375, 430.6640625, 452.197265625, 473.73046875, 495.263671875, 516.796875, 538.330078125, 559.86328125, 581.396484375, 602.9296875, 624.462890625, 645.99609375, 667.529296875, 689.0625, 710.595703125, 732.12890625, 753.662109375, 775.1953125, 796.728515625, 839.794921875, 861.328125, 882.861328125, 904.39453125, 925.927734375, 968.994140625, 990.52734375, 1012.060546875, 1055.126953125, 1076.66015625, 1098.193359375, 1141.259765625, 1184.326171875, 1205.859375, 1248.92578125, 1270.458984375, 1313.525390625, 1356.591796875, 1399.658203125, 1442.724609375, 1485.791015625, 1528.857421875, 1571.923828125, 1614.990234375, 1658.056640625, 1701.123046875, 1765.72265625, 1808.7890625, 1873.388671875, 1916.455078125, 1981.0546875, 2024.12109375, 2088.720703125, 2153.3203125, 2217.919921875, 2282.51953125, 2347.119140625, 2411.71875, 2497.8515625, 2562.451171875, 2627.05078125, 2713.18359375, 2799.31640625, 2885.44921875, 2950.048828125, 3036.181640625, 3143.84765625, 3229.98046875, 3316.11328125, 3423.779296875, 3509.912109375, 3617.578125, 3725.244140625, 3832.91015625, 3940.576171875, 4069.775390625, 4177.44140625, 4306.640625, 4435.83984375, 4565.0390625, 4694.23828125, 4844.970703125, 4974.169921875, 5124.90234375, 5275.634765625, 5426.3671875, 5577.099609375, 5749.365234375, 5921.630859375, 6093.896484375, 6266.162109375, 6459.9609375, 6653.759765625, 6847.55859375, 7041.357421875, 7256.689453125, 7450.48828125, 7687.353515625, 7902.685546875, 8139.55078125, 8376.416015625, 8613.28125, 8871.6796875, 9130.078125, 9388.4765625, 9668.408203125, 9948.33984375, 10249.8046875, 10551.26953125, 10852.734375, 11175.732421875, 11498.73046875, 11843.26171875, 12187.79296875, 12553.857421875, 12919.921875, 13285.986328125, 13673.583984375, 14082.71484375, 14491.845703125, 14922.509765625, 15353.173828125, 15805.37109375, 16257.568359375};
    int num_bands_{0};
    int spectrum_size_{0};
    float sample_rate_{0.0f};
    bool is_log_{false};
    bool is_power_{false};
    Eigen::MatrixXf filters_;
};

TriangularBands::TriangularBands()
    : impl_(std::make_shared<Impl>()) {
    parameters_.pushIntParameter("inputSize", 1025, 1, std::numeric_limits<int>::max());
    parameters_.pushBoolParameter("log", true);
    parameters_.pushFloatParameter("sampleRate", 44100, 0, std::numeric_limits<float>::max());
    parameters_.pushChoiceParameter("type", 1, {"magnitude", "power"});

    defineInput(in_spec_, "spectrum");
    defineOutput(out_bands_, "bands");
}
std::string TriangularBands::getName() const {
    return {"TriangularBands"};
}
void TriangularBands::configure() {
    impl_->spectrum_size_ = getParameter("inputSize").getInt();
    impl_->sample_rate_ = getParameter("sampleRate").getPlainValue();
    impl_->is_log_ = getParameter("log").getBool();
    impl_->is_power_ = getParameter("type").getChoiceString() == "power";
    impl_->createFilters();
}
void TriangularBands::compute() {

    const auto &in_spec = in_spec_.get();
    auto &out_bands = out_bands_.get();

    out_bands.resize(impl_->num_bands_);

    Eigen::Map<const Eigen::ArrayXf> in_spec_vec(in_spec.data(), in_spec.size());
    Eigen::Map<Eigen::ArrayXf> out_bands_vec(out_bands.data(), out_bands.size());

    if (impl_->is_power_) {
        out_bands_vec = impl_->filters_ * in_spec_vec.pow(2).matrix();
    } else {
        out_bands_vec = impl_->filters_ * in_spec_vec.matrix();
    }

    if (impl_->is_log_) {
        // log2(1 + x) = ln(1+x) / ln(2)
        out_bands_vec = out_bands_vec.log1p() / M_LN2;
    }
}

const std::vector<float> &TriangularBands::getFrequencies() const {
    return impl_->kFrequencies;
}

int TriangularBands::getNumberBands() const {
    return impl_->num_bands_;
}

const void *TriangularBands::getFilters() const {
    return &impl_->filters_;
}
} // namespace libaa::MIR