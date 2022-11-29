//
// Created by user on 11/29/22.
//

#include "libaa/mir/aa_extractor_base.h"

namespace libaa::MIR {
bool ExtractorBase::initializeBuffers(size_t input_channels, size_t step_size, size_t block_size) {
    allocateRingBuffers(input_channels, block_size);
    allocateUnwrapBuffers(input_channels, block_size);

    input_channels_ = input_channels;
    step_size_ = step_size;
    block_size_ = block_size;
    // we initialize this counter = block_size - step_size so that it is easier for the first block
    // assume that block_size >= step_size
    assert(block_size >= step_size);
    step_counter_ = static_cast<int>(step_size - block_size);

    return true;
}

void ExtractorBase::setParameter(const std::string &param_name, float v) {
    parameters_.get(param_name).setPlainValue(v);
}

void ExtractorBase::setParameter(const std::string &param_name, const std::string &str_v) {
    parameters_.get(param_name).setPlainValue(str_v);
}

const AudioProcessorParameter &ExtractorBase::getParameter(const std::string &param_name) const {
    return parameters_.get(param_name);
}

void ExtractorBase::process(const float *const *input_buffers, size_t samples, bool is_final) {
    if (getBlockSize() == 0 || getStepSize() == 0) {
        printf("block size or step size is ZERO, call initializeBuffers() before process");
        return;
    }

    size_t i = 0u;
    for (i = 0u; i < samples; ++i) {
        pushSampleToRingBuffers(input_buffers, i);

        ++num_remaining_;
        ++step_counter_;
        ++context_.play_head_sample_index;
        if (step_counter_ >= static_cast<int>(step_size_)) {
            step_counter_ = 0;

            unwrap(block_size_, false);
            num_remaining_ -= block_size_;
            processBlock(unwrap_buffers_, context_);
        }
    }

    if (is_final && num_remaining_ > 0) {
        unwrap(num_remaining_, true);
        processBlock(unwrap_buffers_, context_);
    }
}

void ExtractorBase::processBlock(std::vector<std::vector<float>> &input_buffers, ProcessingContext &context) {
    (void)(input_buffers);
    (void)(context);
}

size_t ExtractorBase::getStepSize() const {
    return step_size_;
}

size_t ExtractorBase::getBlockSize() const {
    return block_size_;
}

int ExtractorBase::getStepCounter() const {
    return step_counter_;
}

void ExtractorBase::allocateRingBuffers(size_t input_channels, size_t block_size) {
    ring_buffers_.allocateDelayLines(input_channels, block_size);
}

void ExtractorBase::allocateUnwrapBuffers(size_t input_channels, size_t block_size) {
    unwrap_buffers_.resize(input_channels);
    for (auto &b : unwrap_buffers_) {
        b.resize(block_size, 0.0f);
    }
}

void ExtractorBase::pushSampleToRingBuffers(const float *const *input_buffers, int i) {
    for (auto c = 0u; c < input_channels_; ++c) {
        ring_buffers_.getDelayLine(c)->push(input_buffers[c][i]);
    }
}

void ExtractorBase::unwrap(size_t size, bool fill_zero) {
    for (auto c = 0u; c < input_channels_; ++c) {
        if (fill_zero) {
            std::fill(unwrap_buffers_[c].begin(), unwrap_buffers_[c].end(), 0.0f);
        }

        // read samples from delay lines
        for (int ii = size - 1, i = 0; ii >= 0; ++i, --ii) {
            unwrap_buffers_[c][i] = ring_buffers_.getDelayLine(c)->get(ii);
        }
    }
}
void ExtractorBase::initialize(float sample_rate, size_t input_channels) {
    context_.sample_rate = sample_rate;
    input_channels_ = input_channels;
}
} // namespace AudioEffect::MIR
