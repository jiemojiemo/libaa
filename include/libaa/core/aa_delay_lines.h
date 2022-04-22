//
// Created by William on 3/24/22.
//

#ifndef AUDIOEFFECT_AE_DELAY_LINES_H
#define AUDIOEFFECT_AE_DELAY_LINES_H

#pragma once
#include "libaa/core/aa_delay_line.h"
namespace libaa {
template <typename T> class DelayLines {
public:
    DelayLines() = default;

    DelayLines(size_t num_lines, size_t line_size) {
        allocateDelayLines(num_lines, line_size);
    }

    size_t getNumDelayLines() const {
        return dlines_.size();
    }

    void allocateDelayLines(size_t num_lines, size_t size) {
        dlines_.resize(num_lines);

        for (auto &dline : dlines_) {
            dline.resize(size);
        }
    }

    size_t getMaxDelaySamples() const {
        if (dlines_.empty())
            return 0;

        return dlines_[0].size();
    }

    DelayLine<T> *getDelayLine(size_t num_of_dline) {
        if (num_of_dline > getNumDelayLines()) {
            return nullptr;
        }

        return &dlines_[num_of_dline];
    }

    void clear() {
        for (auto &dline : dlines_) {
            dline.clear();
        }
    }

    void resize(size_t new_size) {
        for (auto &dline : dlines_) {
            dline.resize(new_size);
        }
    }

private:
    std::vector<DelayLine<T>> dlines_;
};

} // namespace libaa

#endif // AUDIOEFFECT_AE_DELAY_LINES_H
