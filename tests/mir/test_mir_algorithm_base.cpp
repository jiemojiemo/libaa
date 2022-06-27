//
// Created by user on 6/23/22.
//
#include "libaa/mir/aa_mir_algorithm_base.h"
#include "libaa/mir/aa_mir_input.h"
#include "libaa/mir/aa_mir_output.h"
#include <gmock/gmock.h>

using namespace testing;
using namespace libaa::MIR;

class MockMIRAlgorithm : public MIRAlgorithmBase {
public:
    MockMIRAlgorithm() {
        parameters_.pushIntParameter("Int", 0, -10, 10);
        parameters_.pushFloatParameter("Float", 0, -10, 10);
        parameters_.pushBoolParameter("Bool", true);
        parameters_.pushChoiceParameter("Choice", 0, {"choice0", "choice1"});
    }

    void defineInput(InputBase &in, const std::string &name) {
        MIRAlgorithmBase::defineInput(in, name);
    }

    void defineOutput(OutputBase &out, const std::string &name) {
        MIRAlgorithmBase::defineOutput(out, name);
    }

    std::string getName() const override {
        return std::string();
    }
    void configure() override {
    }
    void compute() override {
    }
};

class AMockMIRAlgorithm : public Test {
public:
    MockMIRAlgorithm m;
};

TEST_F(AMockMIRAlgorithm, DefineInputInsertInputToInputMap) {
    Input<std::vector<float>> vec_in;

    m.defineInput(vec_in, "vec_in");

    ASSERT_THAT(m.getInputs().find("vec_in"), Not(m.getInputs().end()));
}

TEST_F(AMockMIRAlgorithm, DefineOutputInsertInputToOutputMap) {
    Output<std::vector<float>> vec_out;

    m.defineOutput(vec_out, "vec_out");

    ASSERT_THAT(m.getOutputs().find("vec_out"), Not(m.getOutputs().end()));
}

TEST_F(AMockMIRAlgorithm, CanSetNumericParameter) {
    m.setParameter("Int", 5);
    m.setParameter("Float", 5);
    m.setParameter("Bool", static_cast<float>(false));

    ASSERT_THAT(m.getParameter("Int").getInt(), Eq(5));
    ASSERT_THAT(m.getParameter("Float").getPlainValue(), Eq(5.0f));
    ASSERT_FALSE(m.getParameter("Bool").getBool());
}

TEST_F(AMockMIRAlgorithm, CanSetStringParameter) {
    m.setParameter("Int", "5");
    m.setParameter("Float", "5");
    m.setParameter("Bool", "false");
    m.setParameter("Choice", "choice1");

    ASSERT_THAT(m.getParameter("Int").getPlainValueString(), Eq("5"));
    ASSERT_THAT(m.getParameter("Float").getPlainValueString(), Eq(std::to_string(5.0f)));
    ASSERT_THAT(m.getParameter("Bool").getPlainValueString(), Eq("false"));
    ASSERT_THAT(m.getParameter("Choice").getPlainValueString(), Eq("choice1"));
}