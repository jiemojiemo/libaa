//
// Created by user on 6/27/22.
//

#ifndef LIBAA_AA_MAX_FILTER_H
#define LIBAA_AA_MAX_FILTER_H

#pragma once
#include "libaa/mir/aa_mir_algorithm_base.h"
#include <memory>
namespace libaa::MIR {
class MaxFilter : public MIRAlgorithmBase {
protected:
    Input<std::vector<float>> in_signal_;
    Output<std::vector<float>> out_signal_;

public:
    class SimpleRingBuffer {
    public:
        SimpleRingBuffer(size_t size, float init_val)
            : write_index(0),
              data(size, init_val) {
        }

        void push(float v) {
            data[write_index++] = v;
            if (write_index >= data.size()) {
                write_index = 0;
            }
        }

        size_t write_index;
        std::vector<float> data;
    };

    MaxFilter();
    ~MaxFilter() override = default;
    std::string getName() const override;
    void configure() override;
    void compute() override;

    // -- testing --
    const SimpleRingBuffer *getSimpleRingBuffer() const;

private:
    std::unique_ptr<SimpleRingBuffer> buffer_{nullptr};
};
} // namespace AudioEffect::MIR


#endif // LIBAA_AA_MAX_FILTER_H
