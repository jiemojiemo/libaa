//
// Created by user on 6/27/22.
//

#include "libaa/mir/aa_windowing.h"

#include <Eigen/Dense>
namespace libaa {
namespace MIR {
Windowing::Windowing() {
    parameters_.pushChoiceParameter("type", 0, {"hann", "hamming", "triangular", "rectangle", "blackmanharris"});
    parameters_.pushIntParameter("size", 1024, 2, std::numeric_limits<int>::max());
    parameters_.pushBoolParameter("normalized", true);
    parameters_.pushBoolParameter("zeroPhase", true);

    defineInput(in_frame_, "frame");
    defineOutput(out_frame_, "frame");
}
std::string Windowing::getName() const {
    return "Windowing";
}

void Windowing::configure() {
    createWindow();

    auto normalized = getParameter("normalized").getBool();
    if (normalized) {
        normalize();
    }
}

void Windowing::compute() {
    const std::vector<float> &in_frame = in_frame_.get();
    std::vector<float> &out_frame = out_frame_.get();

    if (window_.size() != in_frame.size()) {
        throw std::invalid_argument("Windowing: input size not match window size");
    }

    out_frame.resize(in_frame.size());

    Eigen::Map<const Eigen::ArrayXf> in(in_frame.data(), in_frame.size());
    Eigen::Map<Eigen::ArrayXf> out(out_frame.data(), out_frame.size());
    Eigen::Map<Eigen::ArrayXf> w(window_.data(), window_.size());

    bool is_zero_phase = getParameter("zeroPhase").getBool();
    if (is_zero_phase) {
        auto half_size = in.size() / 2;
        out.head(half_size) = in.tail(half_size) * w.tail(half_size);
        out.tail(half_size) = in.head(half_size) * w.head(half_size);
    } else {
        out = w * in;
    }
}

void Windowing::createWindow() {
    auto type = getParameter("type").getChoiceString();
    auto size = getParameter("size").getInt();
    if (type == "hann") {
        window_ = Window::getWindow(WindowType::kHann, size);
    } else if (type == "hamming") {
        window_ = Window::getWindow(WindowType::kHamming, size);
    } else if (type == "triangular") {
        window_ = Window::getWindow(WindowType::kTriangular, size);
    } else if (type == "rectangle") {
        window_ = Window::getWindow(WindowType::kRectangle, size);
    } else if (type == "blackmanharris") {
        window_ = Window::getWindow(WindowType::kBlackmanHarris, size);
    }
}

void Windowing::normalize() {
    Eigen::Map<Eigen::ArrayXf> w(window_.data(), window_.size());
    float sum = w.sum();
    float scale = 2.0f / sum;
    w *= scale;
}

} // namespace MIR
}