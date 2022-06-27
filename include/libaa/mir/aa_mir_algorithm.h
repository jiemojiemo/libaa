//
// Created by user on 6/27/22.
//

#ifndef LIBAA_AA_MIR_ALGORITHM_H
#define LIBAA_AA_MIR_ALGORITHM_H

#include <string>
namespace libaa {
class AudioProcessorParameter;
namespace MIR {
class InputBase;
class OutputBase;
class IMIRAlgorithm {
public:
    virtual ~IMIRAlgorithm() = default;

    /**
     * Returns the algorithm name
     */
    virtual std::string getName() const = 0;

    /**
     * Sets up for compute, this function will creat internal object or allocate new memory according parameters.
     * This functions needs to be called before compute() and after update parameter.
     */
    virtual void configure() = 0;

    /**
     * Do the actual computation once that everything is set and configured.
     */
    virtual void compute() = 0;

    /**
     * Resets internal state
     */
    virtual void reset() {}

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
     * Returns parameter, throws if cannot find parameter. @see AudioProcessorParameter
     * @param param_name The parameter name
     */
    virtual const AudioProcessorParameter &getParameter(const std::string &param_name) const = 0;

    /**
     * Returns the input wrapper associated with the given name, throws exception if cannot find input.
     */
    virtual InputBase &input(const std::string &name) = 0;
    virtual const InputBase &input(const std::string &name) const = 0;

    /**
     * Returns the output wrapper associated with the given name, throws exception if cannot find input.
     */
    virtual OutputBase &output(const std::string name) = 0;
    virtual const OutputBase &output(const std::string name) const = 0;
};
} // namespace MIR
} // namespace AudioEffect


#endif // LIBAA_AA_MIR_ALGORITHM_H
