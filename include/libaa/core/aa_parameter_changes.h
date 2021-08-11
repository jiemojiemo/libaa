
//
// Created by William.Hua on 2021/8/3.
//

#ifndef LIBAA_INCLUDE_LIBAA_CORE_AA_PARAMETER_CHANGES_H
#define LIBAA_INCLUDE_LIBAA_CORE_AA_PARAMETER_CHANGES_H
#pragma once
#include "libaa/core/aa_parameter_change_ringbuffer.h"
#include <memory>
#include <map>

namespace libaa
{
class ParameterChanges
{
public:
    using ParameterChangeSet = std::map<int ,std::shared_ptr<ParameterChangeRingbuffer>>;
    typedef typename ParameterChangeSet::iterator iterator;
    typedef typename ParameterChangeSet::const_iterator const_iterator;

    explicit ParameterChanges(std::initializer_list<ParameterChangePoint> points)
    {
        for(const auto& p : points)
        {
            insert(p);
        }

    }

    explicit ParameterChanges(size_t num_param_change_ring_buffer = 0)
    {
        allocateRingbuffers(num_param_change_ring_buffer);
    }

    const ParameterChangeSet& getParameterChangeSet() const {
        return changes_set_;
    }

    const std::shared_ptr<ParameterChangeRingbuffer>& at(int index) const{
        return changes_set_.at(index);
    }

    std::shared_ptr<ParameterChangeRingbuffer> at(int index){
        return changes_set_.at(index);
    }

    inline iterator begin() noexcept { return changes_set_.begin(); }
    inline const_iterator cbegin() const noexcept { return changes_set_.cbegin(); }
    inline iterator end() noexcept { return changes_set_.end(); }
    inline const_iterator cend() const noexcept { return changes_set_.cend(); }

    bool empty() const{
        return getNumChanges() == 0;
    }

    size_t getNumChanges() const{
        size_t total_size = 0;
        for(const auto& item : changes_set_)
        {
            total_size += item.second->getReadAvailableSize();
        }

        return total_size;
    }

    void insert(const ParameterChangePoint& p)
    {
        auto search = changes_set_.find(p.index);
        if(search == changes_set_.end())
        {
            insertNewRingBuffer(p.index);
        }

        changes_set_.at(p.index)->insert(p);
    }


private:
    void allocateRingbuffers(size_t num_param_change_ring_buffer)
    {
        for(auto i = 0u; i < num_param_change_ring_buffer; ++i){
            auto ringbuffer = std::make_shared<ParameterChangeRingbuffer>(kMaxParameterChanges);
            ringbuffer->setParameterIndex(static_cast<int>(i));

            changes_set_.insert({i, ringbuffer});
        }
    }

    void insertNewRingBuffer(int param_index)
    {
        auto ringbuffer = std::make_shared<ParameterChangeRingbuffer>(kMaxParameterChanges);
        ringbuffer->setParameterIndex(static_cast<int>(param_index));

        changes_set_.insert({param_index, ringbuffer});
    }
private:
    ParameterChangeSet changes_set_;
    constexpr static size_t kMaxParameterChanges = 512;
};
}


#endif //LIBAA_INCLUDE_LIBAA_CORE_AA_PARAMETER_CHANGES_H
