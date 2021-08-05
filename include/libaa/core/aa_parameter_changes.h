
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

    explicit ParameterChanges(size_t num_param_change_ring_buffer = 0)
    {
        for(auto i = 0u; i < num_param_change_ring_buffer; ++i){
            auto* ringbuffer = new ParameterChangeRingbuffer{kMaxParameterChanges};
            ringbuffer->setParameterIndex(static_cast<int>(i));

            changes_array_.emplace_back(ringbuffer);
        }
    }

    const RingBuffers& getParameterChangesArray() const {
        return changes_array_;
    }

    inline iterator begin() noexcept { return changes_array_.begin(); }
    inline const_iterator cbegin() const noexcept { return changes_array_.cbegin(); }
    inline iterator end() noexcept { return changes_array_.end(); }
    inline const_iterator cend() const noexcept { return changes_array_.cend(); }

private:
    RingBuffers changes_array_;
    constexpr static size_t kMaxParameterChanges = 512;
};
}


#endif //LIBAA_INCLUDE_LIBAA_CORE_AA_PARAMETER_CHANGES_H
