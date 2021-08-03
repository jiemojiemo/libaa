
//
// Created by William.Hua on 2021/8/2.
//

#include <gmock/gmock.h>
#include "libaa/core/aa_parameter_changes.h"
using namespace testing;
using namespace libaa;

class AParameterChanges : public Test
{
public:

};

TEST_F(AParameterChanges, CanConstructByDefaut)
{
    auto param_changes = ParameterChanges{};
}

TEST_F(AParameterChanges, DefautConstructInitWithEmptyParameterRingBuffer)
{
    auto param_changes = ParameterChanges{};

    ASSERT_TRUE(param_changes.getParameterChangesArray().empty());
}

TEST_F(AParameterChanges, CanConstuctWithSizeOfParamterChange)
{
    size_t num_param_change_buffer = 2;
    auto param_changes = ParameterChanges{num_param_change_buffer};

    ASSERT_THAT(param_changes.getParameterChangesArray().size(), Eq(num_param_change_buffer));
}

TEST_F(AParameterChanges, RingbufferHasIncreasedIndex)
{
    size_t num_param_change_buffer = 2;
    auto param_changes = ParameterChanges{num_param_change_buffer};

    const ParameterChanges::RingBuffers& ringbuffers = param_changes.getParameterChangesArray();

    ASSERT_THAT(ringbuffers[0]->getParameterIndex(), Eq(0));
    ASSERT_THAT(ringbuffers[1]->getParameterIndex(), Eq(1));
}