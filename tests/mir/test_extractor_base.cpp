//
// Created by user on 6/28/22.
//
#include "libaa/mir/aa_extractor_base.h"
#include <gmock/gmock.h>

using namespace testing;
using namespace libaa;
using namespace libaa::MIR;

class MockExtractorBase : public ExtractorBase {
public:
    MockExtractorBase() {
        parameters_.pushIntParameter("Int", 0, -10, 10);
        parameters_.pushFloatParameter("Float", 0, -10, 10);
        parameters_.pushBoolParameter("Bool", true);
        parameters_.pushChoiceParameter("Choice", 0, {"choice0", "choice1"});

        ON_CALL(*this, getName).WillByDefault(testing::Return("MockExtractorBase"));
        ON_CALL(*this, getRemainingFeatures).WillByDefault(testing::Return(FeatureSet{}));
    }
    ~MockExtractorBase() override = default;

    void configure() override {
    }

    bool initializeBuffers(size_t input_channels, size_t step_size, size_t block_size) override {
        return ExtractorBase::initializeBuffers(input_channels, step_size, block_size);
    }

    const auto &getUnwrapBuffers() const {
        return unwrap_buffers_;
    }

    const auto &getProcessingContext() const {
        return context_;
    }

    const auto &getRingBuffers() const {
        return ring_buffers_;
    }

    MOCK_METHOD(std::string, getName, (), (override, const));
    MOCK_METHOD(FeatureSet, getFrameFeatures, (), (override));
    MOCK_METHOD(FeatureSet, getRemainingFeatures, (), (override));
    MOCK_METHOD(void, processBlock, (std::vector<std::vector<float>> & input_buffers, libaa::ProcessingContext &context));
};

class AExtractorBase : public Test {
public:
    NiceMock<MockExtractorBase> ext;

    float sample_rate = 44100;
    size_t input_channels = 2;
    size_t step_size = 256;
    size_t block_size = 2048;
};

TEST_F(AExtractorBase, CanGetParameterByName) {
    ASSERT_THAT(ext.getParameter("Int").getPlainValue(), Eq(0));
}

TEST_F(AExtractorBase, GetParameterThrowsIfCannotFindIt) {
    ASSERT_ANY_THROW(ext.getParameter("XXX"));
}

TEST_F(AExtractorBase, CanSetNumericParameters) {
    ext.setParameter("Int", 5);
    ext.setParameter("Float", 5);
    ext.setParameter("Bool", static_cast<float>(false));

    ASSERT_THAT(ext.getParameter("Int").getInt(), Eq(5));
    ASSERT_THAT(ext.getParameter("Float").getPlainValue(), Eq(5.0f));
    ASSERT_FALSE(ext.getParameter("Bool").getBool());
}

TEST_F(AExtractorBase, CanSetStringParameter) {
    ext.setParameter("Int", "5");
    ext.setParameter("Float", "5");
    ext.setParameter("Bool", "false");
    ext.setParameter("Choice", "choice1");

    ASSERT_THAT(ext.getParameter("Int").getPlainValueString(), Eq("5"));
    ASSERT_THAT(ext.getParameter("Float").getPlainValueString(), Eq(std::to_string(5.0f)));
    ASSERT_THAT(ext.getParameter("Bool").getPlainValueString(), Eq("false"));
    ASSERT_THAT(ext.getParameter("Choice").getPlainValueString(), Eq("choice1"));
}

TEST_F(AExtractorBase, InitializeInternalProcessingContext) {
    ext.initialize(sample_rate, input_channels);

    auto &context = ext.getProcessingContext();
    ASSERT_THAT(context.sample_rate, Eq(sample_rate));
    ASSERT_THAT(context.play_head_sample_index, Eq(0));
}

TEST_F(AExtractorBase, InitializeBuffersWillCreateInternalUnwrapBuffer) {
    ext.initializeBuffers(input_channels, step_size, block_size);

    ASSERT_THAT(ext.getUnwrapBuffers().size(), Eq(input_channels));
    ASSERT_THAT(ext.getUnwrapBuffers()[0].size(), Eq(block_size));
}

TEST_F(AExtractorBase, InitializeUpdateInternalStepSize) {
    ASSERT_THAT(ext.getStepSize(), Eq(0));

    ext.initializeBuffers(input_channels, step_size, block_size);

    ASSERT_THAT(ext.getStepSize(), Eq(step_size));
}

TEST_F(AExtractorBase, InitializeUpdateInternalBlockSize) {
    ASSERT_THAT(ext.getBlockSize(), Eq(0));

    ext.initializeBuffers(input_channels, step_size, block_size);

    ASSERT_THAT(ext.getBlockSize(), Eq(block_size));
}

TEST_F(AExtractorBase, InitializeUpdateInternalStepCouner) {
    ASSERT_THAT(ext.getStepCounter(), Eq(0));

    ext.initializeBuffers(input_channels, step_size, block_size);

    ASSERT_THAT(ext.getStepCounter(), Eq(int(step_size - block_size)));
}

TEST_F(AExtractorBase, InitializeUpdateInternalRingBuffers) {
    ext.initializeBuffers(input_channels, step_size, block_size);

    ASSERT_THAT(ext.getRingBuffers().getNumDelayLines(), Eq(input_channels));
    ASSERT_THAT(ext.getRingBuffers().getMaxDelaySamples(), Eq(block_size));
}

TEST_F(AExtractorBase, ProcessIncreaseStepCounter) {
    std::vector<std::vector<float>> inputs{
        {1, 1, 1},
        {2, 2, 2}};
    std::vector<float *> data_refer_to{inputs[0].data(), inputs[1].data()};
    ext.initializeBuffers(input_channels, step_size, block_size);

    int expected = ext.getStepCounter() + inputs[0].size();
    ext.process(data_refer_to.data(), inputs[0].size(), false);

    ASSERT_THAT(ext.getStepCounter(), Eq(expected));
}

TEST_F(AExtractorBase, ProcessBlockIfThereHasEnoughBlockSize) {
    std::vector<std::vector<float>> inputs{
        {1, 2, 3, 4, 5, 6},
        {1, 2, 3, 4, 5, 6},
    };
    std::vector<float *> data_refer_to{inputs[0].data(), inputs[1].data()};
    ext.initializeBuffers(input_channels, 2, 4);

    {
        InSequence seq;
        std::vector<std::vector<float>> block0{
            {1, 2, 3, 4},
            {1, 2, 3, 4},
        };
        EXPECT_CALL(ext, processBlock(block0, _)).Times(1);

        std::vector<std::vector<float>> block1{
            {3, 4, 5, 6},
            {3, 4, 5, 6},
        };
        EXPECT_CALL(ext, processBlock(block1, _)).Times(1);
    }

    ext.process(data_refer_to.data(), inputs[0].size(), false);
}

TEST_F(AExtractorBase, ProcessIncreaseContextPlayHeadPosition) {
    std::vector<std::vector<float>> inputs{
        {1, 2, 3, 4, 5, 6},
        {1, 2, 3, 4, 5, 6},
    };
    std::vector<float *> data_refer_to{inputs[0].data(), inputs[1].data()};
    ext.initializeBuffers(input_channels, 2, 4);

    ext.process(data_refer_to.data(), inputs[0].size(), false);

    ASSERT_THAT(ext.getProcessingContext().play_head_sample_index, Eq(inputs[0].size()));
}

TEST_F(AExtractorBase, ProcessBlockPaddingZeroIfFinalBlockNotEnough) {
    std::vector<std::vector<float>> inputs{
        {1, 2, 3, 4, 5, 6},
        {1, 2, 3, 4, 5, 6},
    };
    std::vector<float *> data_refer_to{inputs[0].data(), inputs[1].data()};
    ext.initializeBuffers(input_channels, 2, 4);

    std::vector<std::vector<float>> expected{
        {1, 2, 0, 0},
        {1, 2, 0, 0},
    };
    EXPECT_CALL(ext, processBlock(expected, _)).Times(1);

    ext.process(data_refer_to.data(), 2, true);
}

TEST_F(AExtractorBase, ProcessNothingIsBlockSizeIsZero) {
    std::vector<std::vector<float>> inputs{
        {1, 2, 3, 4, 5, 6},
        {1, 2, 3, 4, 5, 6},
    };
    std::vector<float *> data_refer_to{inputs[0].data(), inputs[1].data()};

    EXPECT_CALL(ext, processBlock(inputs, _)).Times(0);

    ext.process(data_refer_to.data(), inputs[0].size(), false);
    ASSERT_THAT(ext.getBlockSize(), Eq(0));
}

TEST_F(AExtractorBase, ProcessNothingIsStepSizeIsZero) {
    std::vector<std::vector<float>> inputs{
        {1, 2, 3, 4, 5, 6},
        {1, 2, 3, 4, 5, 6},
    };
    std::vector<float *> data_refer_to{inputs[0].data(), inputs[1].data()};

    EXPECT_CALL(ext, processBlock(inputs, _)).Times(0);

    ext.process(data_refer_to.data(), inputs[0].size(), false);
    ASSERT_THAT(ext.getStepSize(), Eq(0));
}