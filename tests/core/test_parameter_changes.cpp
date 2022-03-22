
//
// Created by William.Hua on 2021/8/2.
//

#include "aa_test_helper.h"
#include "libaa/core/aa_parameter_changes.h"
#include <gmock/gmock.h>
using namespace testing;
using namespace libaa;

class AParameterChanges : public Test {
public:
};

TEST_F(AParameterChanges, CanConstructByDefaut) {
    auto param_changes = ParameterChanges{};
}

TEST_F(AParameterChanges, DefautConstructInitWithEmptyParameterRingBuffer) {
    auto param_changes = ParameterChanges{};

    ASSERT_TRUE(param_changes.getParameterChangeSet().empty());
}

TEST_F(AParameterChanges, IsEmptyWhenConstructByDefault) {
    auto param_changes = ParameterChanges{};

    ASSERT_TRUE(param_changes.empty());
}

TEST_F(AParameterChanges, CanConstuctWithSizeOfParamterChange) {
    size_t num_param_change_buffer = 2;
    auto param_changes = ParameterChanges{num_param_change_buffer};

    ASSERT_THAT(param_changes.getParameterChangeSet().size(),
                Eq(num_param_change_buffer));
}

TEST_F(AParameterChanges, ConstructWithParameterChangePoints) {
    auto param_changes = ParameterChanges{{0, 0, 0.1}, {0, 0, 0.2}};
}

TEST_F(AParameterChanges, ConstructWithPointsHasSizeOfDifferentIndex) {
    auto param_changes =
        ParameterChanges{{0, 0, 0.1}, {0, 0, 0.1}, {1, 0, 0.2}, {1, 0, 0.2}};

    ASSERT_THAT(param_changes.getParameterChangeSet().size(), Eq(2));
}

TEST_F(AParameterChanges, ConstructWithPointsWillInsertPointIntoRingbuffer) {
    auto param_changes =
        ParameterChanges{{0, 0, 0.1}, {0, 0, 0.1}, {1, 0, 0.2}, {1, 0, 0.2}};

    ASSERT_THAT(param_changes.at(0)->getParameterIndex(), Eq(0));
    ASSERT_THAT(param_changes.at(0)->getReadAvailableSize(), Eq(2));
    ASSERT_THAT(param_changes.at(0)->at(0),
                Pointee(ParameterChangePoint{0, 0, 0.1}));
    ASSERT_THAT(param_changes.at(0)->at(1),
                Pointee(ParameterChangePoint{0, 0, 0.1}));

    ASSERT_THAT(param_changes.at(1)->getParameterIndex(), Eq(1));
    ASSERT_THAT(param_changes.at(1)->getReadAvailableSize(), Eq(2));
    ASSERT_THAT(param_changes.at(1)->at(0),
                Pointee(ParameterChangePoint{0, 0, 0.2}));
    ASSERT_THAT(param_changes.at(1)->at(1),
                Pointee(ParameterChangePoint{0, 0, 0.2}));
}

TEST_F(AParameterChanges, RingbufferHasIncreasedIndex) {
    size_t num_param_change_buffer = 2;
    auto param_changes = ParameterChanges{num_param_change_buffer};

    const ParameterChanges::ParameterChangeSet &ringbuffers =
        param_changes.getParameterChangeSet();

    ASSERT_THAT(ringbuffers.at(0)->getParameterIndex(), Eq(0));
    ASSERT_THAT(ringbuffers.at(1)->getParameterIndex(), Eq(1));
}

TEST_F(AParameterChanges, IsNotEmptyIfThereHasParameterChangePoint) {
    auto param_changes =
        ParameterChanges{{0, 0, 0.1}, {0, 0, 0.1}, {1, 0, 0.2}, {1, 0, 0.2}};

    ASSERT_FALSE(param_changes.empty());
}

TEST_F(AParameterChanges, InsertIncreaseTheNumberOfParameterChangePoint) {
    auto param_changes = ParameterChanges{};

    param_changes.insert({0, 0, 0.5});
    ASSERT_THAT(param_changes.getNumChanges(), Eq(1));

    param_changes.insert({0, 0, 0.5});
    ASSERT_THAT(param_changes.getNumChanges(), Eq(2));
}