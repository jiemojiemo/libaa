//
// Created by user on 6/22/22.
//
#include "../aa_testing_helper.h"
#include "libaa/mir/aa_windowing.h"
#include <gmock/gmock.h>
#include <numeric>

using namespace testing;
using namespace libaa::MIR;

class AWindowing : public Test {
public:
    Windowing w;
};

TEST_F(AWindowing, CanReportCorrectName) {
    ASSERT_THAT(w.getName(), "Windowing");
}

TEST_F(AWindowing, FrameInputAsExpected) {
    const std::type_info &expected = typeid(std::vector<float>);

    ASSERT_THAT(w.input("frame").getTypeInfo(), Eq(std::ref(expected)));
}

TEST_F(AWindowing, FrameOutputAsExpected) {
    const std::type_info &expected = typeid(std::vector<float>);

    ASSERT_THAT(w.output("frame").getTypeInfo(), Eq(std::ref(expected)));
}

TEST_F(AWindowing, WindowTypeParameterAsExpected) {
    auto &p = w.getParameter("type");

    ASSERT_THAT(p.getParameterName(), Eq("type"));
    ASSERT_THAT(p.getParameterType(), Eq(libaa::ParameterType::kChoice));
    ASSERT_THAT(p.getChoiceString(), Eq("hann"));
}

TEST_F(AWindowing, SizeParameterAsExpected) {
    auto &p = w.getParameter("size");

    ASSERT_THAT(p.getParameterName(), Eq("size"));
    ASSERT_THAT(p.getParameterType(), Eq(libaa::ParameterType::kInt));
    ASSERT_THAT(p.getInt(), Eq(1024));
    ASSERT_THAT(p.getMaxPlainValue(), Eq(std::numeric_limits<int>::max()));
    ASSERT_THAT(p.getMinPlainValue(), Eq(2));
}

TEST_F(AWindowing, NormalizedParameterAsExpected) {
    auto &p = w.getParameter("normalized");

    ASSERT_THAT(p.getParameterName(), Eq("normalized"));
    ASSERT_THAT(p.getParameterType(), Eq(libaa::ParameterType::kBool));
    ASSERT_THAT(p.getBool(), Eq(true));
}

TEST_F(AWindowing, ZeroPhaseParaemterAsExpected) {
    auto &p = w.getParameter("zeroPhase");

    ASSERT_THAT(p.getParameterName(), Eq("zeroPhase"));
    ASSERT_THAT(p.getParameterType(), Eq(libaa::ParameterType::kBool));
    ASSERT_THAT(p.getBool(), Eq(true));
}

TEST_F(AWindowing, ConfigureCreateInternalWindowBuffer) {
    w.configure();

    auto window_size = w.getParameter("size").getInt();

    ASSERT_THAT(w.getWindow().size(), Eq(window_size));
}

TEST_F(AWindowing, CanCreateDifferentTypeWindow) {
    w.setParameter("normalized", false);
    w.setParameter("type", "hann");
    w.configure();
    auto size = w.getParameter("size").getInt();
    auto expected = libaa::Window::getWindow(libaa::WindowType::kHann, size);
    ASSERT_THAT(w.getWindow(), ContainerEq(expected));

    w.setParameter("type", "hamming");
    w.configure();
    expected = libaa::Window::getWindow(libaa::WindowType::kHamming, size);
    ASSERT_THAT(w.getWindow(), ContainerEq(expected));

    w.setParameter("type", "triangular");
    w.configure();
    expected = libaa::Window::getWindow(libaa::WindowType::kTriangular, size);
    ASSERT_THAT(w.getWindow(), ContainerEq(expected));

    w.setParameter("type", "rectangle");
    w.configure();
    expected = libaa::Window::getWindow(libaa::WindowType::kRectangle, size);
    ASSERT_THAT(w.getWindow(), ContainerEq(expected));

    w.setParameter("type", "blackmanharris");
    w.configure();
    expected = libaa::Window::getWindow(libaa::WindowType::kBlackmanHarris, size);
    ASSERT_THAT(w.getWindow(), ContainerEq(expected));
}

TEST_F(AWindowing, NormalizeWindowWhenConfigure) {
    w.setParameter("type", "rectangle");
    w.setParameter("normalized", true);
    auto size = w.getParameter("size").getInt();

    w.configure();

    ASSERT_THAT(w.getWindow()[0], Eq(2.0 / size));
}

TEST_F(AWindowing, ComputeThrowsIfWindowSizeNotEqualInputFrameSize) {
    std::vector<float> frame{1, 2, 3, 4};
    std::vector<float> out_frame;
    w.setParameter("size", 100);
    w.input("frame").set(frame);
    w.output("frame").set(out_frame);
    w.configure();

    ASSERT_ANY_THROW(w.compute());
}

TEST_F(AWindowing, ComputeApplyWindowing) {
    std::vector<float> frame{1, 2, 3, 4};
    std::vector<float> out_frame;
    w.setParameter("size", frame.size());
    w.setParameter("normalized", false);
    w.setParameter("zeroPhase", false);

    w.configure();

    w.input("frame").set(frame);
    w.output("frame").set(out_frame);

    auto size = w.getParameter("size").getInt();
    auto window = libaa::Window::getWindow(libaa::WindowType::kHann, size);
    auto expected = frame;
    for (int i = 0; i < size; ++i) {
        expected[i] *= window[i];
    }

    w.compute();

    ASSERT_THAT(expected, ContainerEq(out_frame));
}

TEST_F(AWindowing, CanComputeWithZeroPhase) {
    std::vector<float> frame{1, 2, 3, 4};
    std::vector<float> out_frame;
    w.setParameter("type", "triangular");
    w.setParameter("size", frame.size());
    w.setParameter("normalized", false);
    w.setParameter("zeroPhase", true);

    w.configure();

    w.input("frame").set(frame);
    w.output("frame").set(out_frame);

    auto size = w.getParameter("size").getInt();
    auto window = libaa::Window::getWindow(libaa::WindowType::kHann, size);
    std::vector<float> expected{2.25, 1, 0.25, 1.5};

    w.compute();

    ASSERT_THAT(out_frame, Pointwise(NearWithPrecision(1e-5), expected));
}