//
// Created by user on 6/23/22.
//
#include "libaa/mir/aa_mir_input.h"
#include <gmock/gmock.h>

using namespace testing;
using namespace libaa::MIR;

class AInput : public Test {
};

TEST_F(AInput, CanDefineWithTemplate) {
    Input<float> float_in;
}

TEST_F(AInput, CanGetTypeInfo) {
    Input<float> in;

    const std::type_info &expected = typeid(float);
    ASSERT_TRUE(in.getTypeInfo() == expected);
}

TEST_F(AInput, GetThrowsIfInternalDataIsNull) {
    Input<float> in;

    ASSERT_ANY_THROW(in.get());
}

TEST_F(AInput, CanGetRawData) {
    std::vector<float> v;
    Input<std::vector<float>> input;

    input.set(v);

    ASSERT_THAT(input.getRawData(), Eq(&v));
}

TEST_F(AInput, CanSetData) {
    std::vector<float> v;
    Input<std::vector<float>> input;

    input.set(v);

    ASSERT_THAT(input.getRawData(), Eq(&v));
}

TEST_F(AInput, SetThrowsIfDataTypeNotMatched) {
    Input<std::vector<float>> input;
    std::vector<int> wrong_type;

    ASSERT_ANY_THROW(input.set(wrong_type));
}
