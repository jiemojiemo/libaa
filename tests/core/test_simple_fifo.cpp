//
// Created by user on 11/11/22.
//
#include "libaa/core/aa_simple_fifo.h"
#include <gmock/gmock.h>

using namespace testing;
using namespace libaa;

class ASimpleFIFO : public Test {
public:
    int capacity = 4;
};

TEST_F(ASimpleFIFO, InitWithCapcity) {
    SimpleFIFO<float> fifo(capacity);

    ASSERT_THAT(fifo.capacity(), Eq(capacity));
}

TEST_F(ASimpleFIFO, InitSizeIsEmpty) {
    SimpleFIFO<float> fifo(capacity);

    ASSERT_THAT(fifo.size(), Eq(0));
}

TEST_F(ASimpleFIFO, EmptyWhenSizeIsZero) {
    SimpleFIFO<float> fifo(capacity);

    ASSERT_THAT(fifo.size(), Eq(0));
    ASSERT_TRUE(fifo.empty());
}

TEST_F(ASimpleFIFO, PushIncreaseTheSize) {
    SimpleFIFO<float> fifo(capacity);

    fifo.push(1.0);

    ASSERT_THAT(fifo.size(), Eq(1));
}

TEST_F(ASimpleFIFO, PushFailedIfIsFull) {
    SimpleFIFO<float> fifo(capacity);
    for (int i = 0; i < capacity; ++i) {
        fifo.push(i);
    }

    ASSERT_TRUE(fifo.full());
    ASSERT_FALSE(fifo.push(1.0f));
}

TEST_F(ASimpleFIFO, PopDecraseSize) {
    SimpleFIFO<float> fifo(capacity);
    fifo.push(1.0f);
    ASSERT_THAT(fifo.size(), Eq(1));

    float v;
    fifo.pop(v);

    ASSERT_THAT(fifo.size(), Eq(0));
}

TEST_F(ASimpleFIFO, PopGetValue) {
    float v;
    SimpleFIFO<float> fifo(capacity);
    fifo.push(1.0f);
    fifo.push(2.0f);
    fifo.push(3.0f);

    fifo.pop(v);
    ASSERT_THAT(v, Eq(1.0f));

    fifo.pop(v);
    ASSERT_THAT(v, Eq(2.0f));

    fifo.pop(v);
    ASSERT_THAT(v, Eq(3.0f));
}

TEST_F(ASimpleFIFO, PopFailedIfEmpty) {
    float v;
    SimpleFIFO<float> fifo(capacity);

    ASSERT_FALSE(fifo.pop(v));
}

TEST_F(ASimpleFIFO, CanSetNewCapacity) {
    SimpleFIFO<float> fifo(capacity);

    int new_capacity = 2 * capacity;
    fifo.setCapacity(new_capacity);

    ASSERT_THAT(fifo.capacity(), Eq(new_capacity));
}

TEST_F(ASimpleFIFO, SizeResetToZeroAfterSetCapacity) {
    SimpleFIFO<float> fifo(capacity);
    fifo.push(1.0f);
    ASSERT_THAT(fifo.size(), Eq(1));

    fifo.setCapacity(10);

    ASSERT_THAT(fifo.size(), Eq(0));
}

TEST_F(ASimpleFIFO, ClearWillPopAllValues) {
    SimpleFIFO<float> fifo(capacity);
    for (int i = 0; i < capacity; ++i) {
        fifo.push(i);
    }

    fifo.clear();

    ASSERT_TRUE(fifo.empty());
}