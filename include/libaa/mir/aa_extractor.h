//
// Created by user on 11/29/22.
//

#ifndef LIBAA_AA_EXTRACTOR_H
#define LIBAA_AA_EXTRACTOR_H

#pragma once
#include <map>
#include <string>
#include <vector>
namespace libaa {
class AudioProcessorParameter;
namespace MIR {
class IExtractor {
public:
    virtual ~IExtractor() = default;

    /**
     * Returns the extractor name
     */
    virtual std::string getName() const = 0;

    /**
     * Initialize a extractor to prepare it for processing input audio.
     * @param sample_rate    The sample rate of input audio
     * @param input_channels The number channels of input audio
     */
    virtual void initialize(float sample_rate, size_t input_channels) = 0;

    /**
     * Reset internal state
     */
    virtual void reset() = 0;

    /**
     * Sets numeric parameter, throws exception if cannot find parameter. @see AudioProcessorParameter
     * @param param_name The parameter name
     * @param v The parameter numeric value
     */
    virtual void setParameter(const std::string &param_name, float v) = 0;

    /**
     * Sets string value parameter, throws exception if cannot find parameter. @see AudioProcessorParameter
     * @param param_name The parameter name
     * @param str_v The parameter string value
     */
    virtual void setParameter(const std::string &param_name, const std::string &str_v) = 0;

    /**
     * Sets up for compute, this function will creat internal object or allocate new memory according parameters.
     * This functions needs to be called before process() and after update parameter.
     */
    virtual void configure() = 0;

    /**
     * Returns parameter, throws if cannot find parameter. @see AudioProcessorParameter
     * @param param_name The parameter name
     */
    virtual const AudioProcessorParameter &getParameter(const std::string &param_name) const = 0;

    struct Feature {
        bool has_timestamp{false};
        float timestamp{0};
        bool has_duration{false};
        float duration{0};
        std::vector<float> values;
        std::string label;
    };
    using FeatureList = std::vector<Feature>;
    using FeatureSet = std::map<int, FeatureList>;

    /**
     * Process input audio, then you can call getFrameFeatures() to get features
     * @param input_buffers The input not-interleave buffer
     * @param samples The number of input samples
     */
    virtual void process(const float *const *input_buffers, size_t samples, bool is_final) = 0;

    /**
     * Returns frame features of last process, may return empty features
     */
    virtual FeatureSet getFrameFeatures() = 0;

    /**
     * After call blocks have been processed, calculate and return any remaining features derived from the complete input.
     */
    virtual FeatureSet getRemainingFeatures() = 0;
};

} // namespace MIR
} // namespace AudioEffect

#endif // LIBAA_AA_EXTRACTOR_H
