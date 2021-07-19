
//
// Created by William.Hua on 2021/7/19.
//

#include "libaa/core/aa_audio_processor_parameters.h"

namespace libaa
{
const AudioProcessorParameter* Parameters::get(int param_id)const{
    auto iter = findParameter(param_id);
    if(iter != parameters_list.cend()){
        return &(*iter);
    }

    return nullptr;
}

void Parameters::eraseParameterIfExist(int param_id){
    auto iter = findParameter(param_id);
    if(iter != parameters_list.end()){
        parameters_list.erase(iter);
    }
}
std::list<AudioProcessorParameter>::const_iterator Parameters::findParameter(int param_id) const{
    auto beg = parameters_list.begin();
    for(; beg != parameters_list.end(); ++beg){
        if(beg->getParameterID() == param_id){
            break;
        }
    }
    return beg;
}

void Parameters::addFloatParameter(int param_id, float default_val,float min_plain_value, float max_plain_value){
    eraseParameterIfExist(param_id);

    parameters_list.emplace_back(ParameterType::kFloat, param_id, default_val, min_plain_value, max_plain_value);
}

}