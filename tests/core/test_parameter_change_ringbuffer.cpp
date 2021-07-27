
//
// Created by William.Hua on 2021/7/26.
//

#include <gmock/gmock.h>
#include "aa_test_helper.h"
#include "libaa/core/aa_parameter_change_ringbuffer.h"
using namespace testing;
using namespace libaa;

class AParameterChangeRingBuffer : public Test
{
public:
    constexpr static int size = 8;
};


TEST_F(AParameterChangeRingBuffer, ConstructWithSize)
{
    ParameterChangeRingbuffer<size> events;
}

TEST_F(AParameterChangeRingBuffer, NoAvailableWhenConstruct)
{
    ParameterChangeRingbuffer<size> events;

    ASSERT_THAT(events.getReadAvailableSize(), Eq(0));
}

TEST_F(AParameterChangeRingBuffer, DefaultIndexIsNegOne)
{
    ParameterChangeRingbuffer<size> events;

    ASSERT_THAT(events.getParameterIndex(), Eq(-1));
}

TEST_F(AParameterChangeRingBuffer, CanSetAndGetParameterIndex)
{
    ParameterChangeRingbuffer<size> events;

    events.setParameterIndex(0);
    ASSERT_THAT(events.getParameterIndex(), Eq(0));
}

TEST_F(AParameterChangeRingBuffer, InsertWillIncreaseAvailableSize)
{
    ParameterChangeRingbuffer<size> events;

    ParameterChangePoint event{0, 0.5};

    events.insert(event);
    ASSERT_THAT(events.getReadAvailableSize(), Eq(1));
}

TEST_F(AParameterChangeRingBuffer, CanGetItemByIndex)
{
    ParameterChangeRingbuffer<size> events;

    events.insert({0, 0.1});
    events.insert({0, 0.2});

    ASSERT_THAT(*(events.at(0)), Eq(ParameterChangePoint{0, 0.1}));
    ASSERT_THAT(*(events.at(1)), Eq(ParameterChangePoint{0, 0.2}));
}

TEST_F(AParameterChangeRingBuffer, GetNullIfIndexOutofRange)
{
    ParameterChangeRingbuffer<size> events;

    events.insert({0, 0.1});

    int index = 2;
    ASSERT_THAT(index, Gt(events.getReadAvailableSize()));
    ASSERT_THAT(events.at(index), IsNull());
}

TEST_F(AParameterChangeRingBuffer, InsertOverwriteTheOldestDataIfItIsFull)
{
    ParameterChangeRingbuffer<size> events;
    for(auto i = 0; i < size; ++i){
        events.insert({0, 0.1});
    }
    ASSERT_TRUE(events.isFull());

    events.insert({0, 0.2});
    ASSERT_THAT(events.back(), Pointee(ParameterChangePoint{0, 0.2}));
}
