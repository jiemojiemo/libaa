//
// Created by user on 6/23/22.
//
#include "libaa/mir/aa_mir_output.h"
#include <gmock/gmock.h>

using namespace testing;
using namespace libaa::MIR;

class AOutput : public Test {
public:
};

TEST_F(AOutput, CanDefineWithTemplate) {
    Output<float> float_out;
}

TEST_F(AOutput, CanGetTypeInfo) {
    Output<float> out;

    const std::type_info &expected = typeid(float);
    ASSERT_TRUE(out.getTypeInfo() == expected);
}

TEST_F(AOutput, GetThrowsIfInternalDataIsNull) {
    Output<float> out;

    ASSERT_ANY_THROW(out.get());
}

TEST_F(AOutput, CanGetRawData) {
    std::vector<float> v;
    Output<std::vector<float>> out;

    out.set(v);

    ASSERT_THAT(out.getRawData(), Eq(&v));
}

TEST_F(AOutput, CanSetData) {
    std::vector<float> v;
    Output<std::vector<float>> out;

    out.set(v);

    ASSERT_THAT(out.getRawData(), Eq(&v));
}

TEST_F(AOutput, SetThrowsIfDataTypeNotMatched) {
    Output<std::vector<float>> out;
    std::vector<int> wrong_type;

    ASSERT_ANY_THROW(out.set(wrong_type));
}