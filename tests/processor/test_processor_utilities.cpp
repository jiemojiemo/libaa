//
// Created by user on 5/10/22.
//

#include "libaa/aa_version.h"
#include "libaa/processor/aa_processor_utilities.h"
#include "libaa_testing/aa_mock_processor.h"
#include <gmock/gmock.h>
#include <nlohmann/json.hpp>

using namespace testing;
using namespace libaa;

class AProcessorUtilities : public Test {
public:
    NiceMock<MockProcessor> p;
};

TEST_F(AProcessorUtilities, CanSerializeProcessorWithVersion) {
    auto state_string = ProcessorUtilities::serializeProcessorToString(&p);
    auto state_json = nlohmann::json::parse(state_string);

    ASSERT_THAT(state_json["version"], Eq(LIBAA_VERSION));
}

TEST_F(AProcessorUtilities, CanSerializeProcessorWithProcessorName) {
    std::string expected_name = "ABC";
    EXPECT_CALL(p, getName()).WillOnce(Return(expected_name));
    auto state_string = ProcessorUtilities::serializeProcessorToString(&p);
    auto state_json = nlohmann::json::parse(state_string);

    ASSERT_THAT(state_json["processor_name"], Eq(expected_name));
}

TEST_F(AProcessorUtilities, CanSerializeProcessorWithFloatParameters) {
    AudioProcessorParameters parameters;
    parameters.pushFloatParameter("A", 0, 0, 1.0);
    parameters.pushFloatParameter("B", 10, 0, 100.0);

    EXPECT_CALL(p, getParameters).WillOnce(Return(&parameters));

    auto state_string = ProcessorUtilities::serializeProcessorToString(&p);
    auto expected_parameters_json = nlohmann::json::parse(R"(
        {
            "A" : 0,
            "B" : 10
        }
    )");

    auto state_json = nlohmann::json::parse(state_string);
    ASSERT_THAT(state_json["parameters"], Eq(expected_parameters_json));
}

TEST_F(AProcessorUtilities, CanUpdataFloatParameter) {
    AudioProcessorParameters parameters;
    parameters.pushFloatParameter("A", 0, 0, 1.0);
    parameters.pushFloatParameter("B", 10, 0, 100.0);

    auto new_param_json = nlohmann::json::parse(R"(
    {
        "parameters":
        {
            "A" : 0.5,
            "B" : 50
        }
    }
    )");
    auto new_param_str = nlohmann::to_string(new_param_json);

    ProcessorUtilities::updateParametersFromState((uint8_t *)(new_param_str.data()), new_param_str.size(), parameters);

    ASSERT_THAT(parameters.get(0).getPlainValue(), Eq(0.5));
    ASSERT_THAT(parameters.get(1).getPlainValue(), Eq(50));
}

TEST_F(AProcessorUtilities, CanSerializeProcessorWithBoolParameters) {
    AudioProcessorParameters parameters;
    parameters.pushBoolParameter("A", true);
    parameters.pushBoolParameter("B", false);

    EXPECT_CALL(p, getParameters).WillOnce(Return(&parameters));

    auto state_string = ProcessorUtilities::serializeProcessorToString(&p);
    auto expected_parameters_json = nlohmann::json::parse(R"(
        {
            "A" : true,
            "B" : false
        }
    )");

    auto state_json = nlohmann::json::parse(state_string);
    ASSERT_THAT(state_json["parameters"], Eq(expected_parameters_json));
}

TEST_F(AProcessorUtilities, CanUpdataBoolParameter) {
    AudioProcessorParameters parameters;
    parameters.pushBoolParameter("A", true);
    parameters.pushBoolParameter("B", false);

    auto new_param_json = nlohmann::json::parse(R"(
    {
        "parameters":
        {
            "A" : false,
            "B" : true
        }
    }
    )");
    auto new_param_str = nlohmann::to_string(new_param_json);

    ProcessorUtilities::updateParametersFromState((uint8_t *)(new_param_str.data()), new_param_str.size(), parameters);

    ASSERT_THAT(parameters.get(0).getBool(), Eq(false));
    ASSERT_THAT(parameters.get(1).getBool(), Eq(true));
}

TEST_F(AProcessorUtilities, CanSerializeProcessorWithChoiceParameters) {
    AudioProcessorParameters parameters;
    parameters.pushChoiceParameter("A", 0, {"AA", "AAA"});
    parameters.pushChoiceParameter("B", 1, {"BB", "BBB"});

    EXPECT_CALL(p, getParameters).WillOnce(Return(&parameters));

    auto state_string = ProcessorUtilities::serializeProcessorToString(&p);
    auto expected_parameters_json = nlohmann::json::parse(R"(
        {
            "A" : "AA",
            "B" : "BBB"
        }
    )");

    auto state_json = nlohmann::json::parse(state_string);
    ASSERT_THAT(state_json["parameters"], Eq(expected_parameters_json));
}

TEST_F(AProcessorUtilities, CanUpdataChoiceParameter) {
    AudioProcessorParameters parameters;
    parameters.pushChoiceParameter("A", 0, {"AA", "AAA"});
    parameters.pushChoiceParameter("B", 1, {"BB", "BBB"});

    auto new_param_json = nlohmann::json::parse(R"(
    {
        "parameters":
        {
            "A" : "AAA",
            "B" : "BB"
        }
    }
    )");
    auto new_param_str = nlohmann::to_string(new_param_json);

    ProcessorUtilities::updateParametersFromState((uint8_t *)(new_param_str.data()), new_param_str.size(), parameters);

    ASSERT_THAT(parameters.get(0).getChoiceString(), Eq("AAA"));
    ASSERT_THAT(parameters.get(1).getChoiceString(), Eq("BB"));
}