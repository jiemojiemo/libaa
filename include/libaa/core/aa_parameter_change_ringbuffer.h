
//
// Created by William.Hua on 2021/7/27.
//

#ifndef LIBAA_INCLUDE_LIBAA_CORE_AA_PARAMETER_CHANGE_RINGBUFFER_H
#define LIBAA_INCLUDE_LIBAA_CORE_AA_PARAMETER_CHANGE_RINGBUFFER_H
#pragma once

#include "ringbuffer.hpp"
namespace libaa
{

struct ParameterChangePoint
{
    float time;
    float normalized_value;
};

template <size_t buffer_size>
class ParameterChangeRingbuffer
{
public:
    size_t getReadAvailableSize() const{
        return ringbuffer_.readAvailable();
    }

    bool insert(const ParameterChangePoint& event){
        if(isFull()){
            ringbuffer_.remove();
        }

        return ringbuffer_.insert(event);
    }

    ParameterChangePoint* at(int index){
        return ringbuffer_.at(index);
    }

    ParameterChangePoint* back(){
        if(isEmpty()){
            return nullptr;
        }else
        {
            return at(getReadAvailableSize() - 1);
        }
    }

    void setParameterIndex(int param_index){
        parameter_index_ = param_index;
    }

    int getParameterIndex() const{
        return parameter_index_;
    }

    bool isFull() const {
        return ringbuffer_.isFull();
    }

    bool isEmpty() const{
        return ringbuffer_.isEmpty();
    }

private:
    int parameter_index_ = -1;
    jnk0le::Ringbuffer<ParameterChangePoint, buffer_size> ringbuffer_;
};
}

#endif //LIBAA_INCLUDE_LIBAA_CORE_AA_PARAMETER_CHANGE_RINGBUFFER_H