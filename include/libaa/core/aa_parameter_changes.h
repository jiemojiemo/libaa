
//
// Created by William.Hua on 2021/8/3.
//

#ifndef LIBAA_INCLUDE_LIBAA_CORE_AA_PARAMETER_CHANGES_H
#define LIBAA_INCLUDE_LIBAA_CORE_AA_PARAMETER_CHANGES_H
#pragma once
#include "libaa/core/aa_parameter_change_ringbuffer.h"
#include <memory>

namespace libaa
{
class ParameterChanges
{
public:
    using RingBuffers = std::vector<std::shared_ptr<ParameterChangeRingbuffer>>;
    typedef typename std::vector<std::shared_ptr<ParameterChangeRingbuffer>> array_type;
    typedef typename array_type::iterator iterator;
    typedef typename array_type::const_iterator const_iterator;

    explicit ParameterChanges(std::initializer_list<ParameterChangePoint> points)
    {
        int max_index = -1;
        for(const auto& p : points)
        {
            if(p.index > max_index) { max_index = p.index; }
        }
        int num_params = max_index + 1;

        allocateRingbuffers(static_cast<size_t>(num_params));

        for(const auto& p : points)
        {
            changes_array_.at(static_cast<size_t>(p.index))->insert(p);
        }
    }

    explicit ParameterChanges(size_t num_param_change_ring_buffer = 0)
    {
        allocateRingbuffers(num_param_change_ring_buffer);
    }

    const RingBuffers& getParameterChangesArray() const {
        return changes_array_;
    }

    const std::shared_ptr<ParameterChangeRingbuffer>& at(int index) const{
        return changes_array_.at(static_cast<size_t>(index));
    }

    std::shared_ptr<ParameterChangeRingbuffer> at(int index){
        return changes_array_.at(static_cast<size_t>(index));
    }

    inline iterator begin() noexcept { return changes_array_.begin(); }
    inline const_iterator cbegin() const noexcept { return changes_array_.cbegin(); }
    inline iterator end() noexcept { return changes_array_.end(); }
    inline const_iterator cend() const noexcept { return changes_array_.cend(); }

private:
    void allocateRingbuffers(size_t num_param_change_ring_buffer)
    {
        for(auto i = 0u; i < num_param_change_ring_buffer; ++i){
            auto* ringbuffer = new ParameterChangeRingbuffer{kMaxParameterChanges};
            ringbuffer->setParameterIndex(static_cast<int>(i));

            changes_array_.emplace_back(ringbuffer);
        }
    }
private:
    RingBuffers changes_array_;
    constexpr static size_t kMaxParameterChanges = 512;
};
}


#endif //LIBAA_INCLUDE_LIBAA_CORE_AA_PARAMETER_CHANGES_H
