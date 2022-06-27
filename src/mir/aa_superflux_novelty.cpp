//
// Created by user on 6/27/22.
//

#include "libaa/mir/aa_superflux_novelty.h"
#include "libaa/mir/aa_mir_algorithm_factory.h"
#include <limits>
#include <stdexcept>
namespace libaa::MIR {
SuperFluxNovelty::SuperFluxNovelty() {
    parameters_.pushIntParameter("binWidth", 3, 3, std::numeric_limits<int>::max());
    parameters_.pushIntParameter("frameWidth", 2, 0, std::numeric_limits<int>::max());

    defineInput(in_bands_, "bands");
    defineOutput(out_differences_, "differences");

    max_filter_ = MIRAlgorithmFactory::create("MaxFilter");
}
std::string SuperFluxNovelty::getName() const {
    return {"SuperFluxNovelty"};
}
void SuperFluxNovelty::configure() {
    max_filter_->setParameter("width", getParameter("binWidth").getInt());
    max_filter_->configure();

    frame_width_ = getParameter("frameWidth").getInt();
}
void SuperFluxNovelty::compute() {
    const auto &in_bands = in_bands_.get();
    float &diff = out_differences_.get();

    int num_frames = in_bands.size();
    int num_bands = in_bands[0].size();

    if (frame_width_ >= num_frames) {
        throw std::invalid_argument("SuperFluxNovelty: not enough frames to compute");
    }

    std::vector<float> maxfilter_buffer(num_bands, 0.0f);

    diff = 0.0f;
    for (int i = frame_width_; i < num_frames; ++i) {
        max_filter_->input("signal").set(in_bands[i - frame_width_]);
        max_filter_->output("signal").set(maxfilter_buffer);
        max_filter_->compute();

        float cur_diff = 0.0f;
        for (int j = 0; j < num_bands; ++j) {
            cur_diff = in_bands[i][j] - maxfilter_buffer[j];
            if (cur_diff > 0.0f) {
                diff += cur_diff;
            }
        }
    }
}

// -- testing --
const MIRAlgorithmBase *SuperFluxNovelty::getMaxFilter() const {
    return dynamic_cast<MIRAlgorithmBase *>(max_filter_.get());
}
} // namespace AudioEffect::MIR

