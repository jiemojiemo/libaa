//
// Created by william on 2020/3/3.
//

#pragma once
#include "libaa/audio_basics/aa_delay_line.h"

namespace libaa
{
template <typename T>
class DelayLineArray
{
public:
    DelayLineArray() = default;

    DelayLineArray(size_t num_lines, size_t line_size)
    {
        allocateDelayLines(num_lines, line_size);
    }

    size_t getNumLines() const
    {
        return dlines_.size();
    }

    void allocateDelayLines(size_t num_lines, size_t size)
    {
        dlines_.resize(num_lines);

        for(auto& dline : dlines_)
        {
            dline.resize(size);
        }
    }

    size_t getSize() const
    {
        if(dlines_.empty())
            return 0;

        return dlines_[0].size();
    }

    DelayLine<T>* getDelayLine(size_t num_of_dline)
    {
        if(num_of_dline > getNumLines())
        {
            return nullptr;
        }

        return &dlines_[num_of_dline];
    }

    void clear()
    {
        for(auto& dline : dlines_)
        {
            dline.clear();
        }
    }

    void resize(size_t new_size)
    {
        for(auto& dline : dlines_)
        {
            dline.resize(new_size);
        }
    }

private:
    std::vector<DelayLine<T>> dlines_;
};
}
