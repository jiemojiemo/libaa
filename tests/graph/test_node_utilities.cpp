//
// Created by user on 6/9/22.
//
#include "libaa/graph/aa_node_utilities.h"
#include <gmock/gmock.h>

using namespace testing;
using namespace libaa;

class ANodeUtilities : public Test {
public:
};

TEST_F(ANodeUtilities, CanGetConnectionTypeFromString) {
    ASSERT_THAT(NodeUtilities::getConnectionTypeFromString("audio"), Eq(ConnectionType::kAudioConnection));
    ASSERT_THAT(NodeUtilities::getConnectionTypeFromString("parameter_change"), Eq(ConnectionType::kParameterChangeConnection));
}

TEST_F(ANodeUtilities, GetConnectionTypeThrowsIfTypeInvalid) {
    ASSERT_ANY_THROW(NodeUtilities::getConnectionTypeFromString("invalid_connection_type"));
}

TEST_F(ANodeUtilities, CanGetConnectionTypeString) {
    ASSERT_THAT(NodeUtilities::connectionTypeToString(ConnectionType::kAudioConnection), Eq("audio"));
    ASSERT_THAT(NodeUtilities::connectionTypeToString(ConnectionType::kParameterChangeConnection), Eq("parameter_change"));
}

TEST_F(ANodeUtilities, CanGetPortDirectionTypeFromString) {
    ASSERT_THAT(NodeUtilities::getPortDirectionTypeFromString("input"), Eq(PortDirection::kInput));
    ASSERT_THAT(NodeUtilities::getPortDirectionTypeFromString("output"), Eq(PortDirection::kOutput));
}

TEST_F(ANodeUtilities, GetPortDirectionTypeThrowsIfTypeInvalid) {
    ASSERT_ANY_THROW(NodeUtilities::getPortDirectionTypeFromString("invalid_port_direction"));
}

TEST_F(ANodeUtilities, CanGetPortDirectionTypeString) {
    ASSERT_THAT(NodeUtilities::portDirectionTypeToString(PortDirection::kInput), Eq("input"));
    ASSERT_THAT(NodeUtilities::portDirectionTypeToString(PortDirection::kOutput), Eq("output"));
}

TEST_F(ANodeUtilities, CanGetPortTypeFromString) {
    ASSERT_THAT(NodeUtilities::getPortTypeFromString("audio"), Eq(PortType::kAudio));
    ASSERT_THAT(NodeUtilities::getPortTypeFromString("parameter_change"), Eq(PortType::kParameterChange));
}

TEST_F(ANodeUtilities, GetPortTypeThrowsIfTypeInvalid) {
    ASSERT_ANY_THROW(NodeUtilities::getPortTypeFromString("invalid_port_type"));
}

TEST_F(ANodeUtilities, CanGetPortTypeString) {
    ASSERT_THAT(NodeUtilities::portTypeToString(PortType::kAudio), Eq("audio"));
    ASSERT_THAT(NodeUtilities::portTypeToString(PortType::kParameterChange), Eq("parameter_change"));
}