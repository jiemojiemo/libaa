//
// Created by william on 2020/3/3.
//
#include "libaa/audio_basics/aa_delay_lines.h"
#include <gmock/gmock.h>
using namespace testing;
using namespace libaa;

class ADelayLines : public Test {
public:
    ADelayLines() : dlines(num_lines, line_size) {}
    int num_lines = 2;
    int line_size = 128;

    DelayLines<float> dlines;
};

TEST_F(ADelayLines, InitWithNumberDelayLineAndSize) {
    ASSERT_THAT(dlines.getNumLines(), Eq(num_lines));
    ASSERT_THAT(dlines.getSize(), Eq(line_size));
}

TEST_F(ADelayLines, GetDelayLine) {
    DelayLine<float> *dline = dlines.getDelayLine(0);
    dline->push(1.0f);

    ASSERT_THAT(dline, NotNull());
}

TEST_F(ADelayLines, ReturnsNullIfDelayLineIndexOutofSize) {
    DelayLine<float> *dline = dlines.getDelayLine(10);

    ASSERT_THAT(dline, IsNull());
}

TEST_F(ADelayLines, SetZeroToAllDelayLineAfterClear) {
    auto *dline = dlines.getDelayLine(0);
    dline->push(1.0f);

    dlines.clear();

    ASSERT_THAT(dline->back(), Eq(0.0f));
}

TEST_F(ADelayLines, ResizeAllDelayLines) {
    size_t new_size = 10;
    dlines.resize(new_size);

    auto *dline = dlines.getDelayLine(0);

    ASSERT_THAT(dline->size(), Eq(findNextPower2Number(new_size)));
}

TEST_F(ADelayLines, AllocateNewLines) {
    size_t new_num_lines = 10;
    size_t new_size = 10;
    dlines.allocateDelayLines(new_num_lines, new_size);

    ASSERT_THAT(dlines.getNumLines(), Eq(new_num_lines));
    ASSERT_THAT(dlines.getSize(), Eq(findNextPower2Number(new_size)));
}