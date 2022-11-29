//
// Created by user on 11/29/22.
//

#ifndef LIBAA_AA_EXTRACTOR_BASE_H
#define LIBAA_AA_EXTRACTOR_BASE_H

#pragma once
#include "libaa/core/aa_audio_processor_parameters.h"
#include "libaa/core/aa_delay_lines.h"
#include "libaa/core/aa_processing_context.h"
#include "libaa/mir/aa_extractor.h"
namespace libaa::MIR {
class ExtractorBase : public IExtractor {
public:
    ~ExtractorBase() override = default;

    void initialize(float sample_rate, size_t input_channels) override;

    void setParameter(const std::string &param_name, float v) override;

    void setParameter(const std::string &param_name, const std::string &str_v) override;

    const AudioProcessorParameter &getParameter(const std::string &param_name) const override;

    void process(const float *const *input_buffers, size_t samples, bool is_final) override;

    void reset() override {}

    /**
     * Processes audio block with block_size and step in with step_size, @see initialize()
     *
     * If your algorithm do not need block processing, just simply override ExtractorBase::process() function
     *
     * You can call getFrameFeatures() to get the frame features
     *
     * @param input_buffers The input audio block, has block_size and input_channels size
     * @param context       The processing context
     */
    virtual void processBlock(std::vector<std::vector<float>> &input_buffers, ProcessingContext &context);

    size_t getStepSize() const;
    size_t getBlockSize() const;
    int getStepCounter() const;

protected:
    /**
     * This function will allocate internal ring buffers based on parameters.
     * This functions needs to be called when block_size or step_size changed.
     * @param sample_rate    The sample rate of input audio
     * @param input_channels The number channels of input audio
     * @param step_size      The hop size
     * @param block_size     The block size for processing
     * @return true if ok, false failed
     */
    virtual bool initializeBuffers(size_t input_channels, size_t step_size, size_t block_size);

private:
    void allocateRingBuffers(size_t input_channels, size_t block_size);
    void allocateUnwrapBuffers(size_t input_channels, size_t block_size);
    void pushSampleToRingBuffers(const float *const *input_buffers, int i);
    void unwrap(size_t size, bool fill_zero);

protected:
    size_t input_channels_{0};
    AudioProcessorParameters parameters_;
    DelayLines<float> ring_buffers_;
    std::vector<std::vector<float>> unwrap_buffers_;
    ProcessingContext context_;
    size_t step_size_{0};
    size_t block_size_{0};
    int step_counter_{0};
    int num_remaining_{0};
};
} // namespace libaa::MIR

#endif // LIBAA_AA_EXTRACTOR_BASE_H
