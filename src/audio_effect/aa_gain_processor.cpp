
//
// Created by William.Hua on 2021/7/21.
//

#include "libaa/audio_effect/aa_gain_processor.h"
#include "libaa/dsp/aa_db_utils.h"
#include <cmath>
namespace libaa
{
GainProcessor::GainProcessor(float gain_db):params_(buildParameters(gain_db))
{

}

std::string GainProcessor::getName() const  {
return std::string("Gain");
}

void GainProcessor::prepareToPlay(double sample_rate, int max_block_size)  {
    (void)(sample_rate);
    (void)(max_block_size);
}

void GainProcessor::reset()  {

}
void GainProcessor::releaseResources()  {

}
void GainProcessor::processBlock(AudioBuffer<float> &buffer)  {
(void)(buffer);
}

const Parameters& GainProcessor::getParameters() const{
    return params_;
}


void GainProcessor::applyGain(float* out_buffer, size_t out_size){
    float gain_db  = params_.get(0).getPlainValue();
    if(gain_db != 0.0f){
        float gain_scale = db_to_scale(gain_db);
        for(size_t i = 0; i < out_size; ++i)
        {
            out_buffer[i] *= gain_scale;
        }
    }
}

Parameters GainProcessor::buildParameters(float gain_db){
    Parameters params;

    params.pushFloatParameter("Gain dB", gain_db, -75, 35);

    return params;
}
int GainProcessor::setParameter(int param_index, float normalized_value) {
    try{
        AudioProcessorParameter& p = params_.get(param_index);
        p.setNormalizedValue(normalized_value);
        return 0;
    }catch (...){
        return -1;
    }
}
void GainProcessor::process(AudioBufferNew<float> *in_block, AudioBufferNew<float> *out_block) {
    out_block->copyFrom(in_block);

    for(auto c = 0u; c < out_block->getNumberChannels(); ++c){
        float* channel_data = out_block->getWriterPointer(c);
        applyGain(channel_data, out_block->getNumberFrames());
    }
}

void GainProcessor::process(AudioBlock *in_block, AudioBlock *out_block) {

    if(!in_block->param_changes.at(0)->isEmpty()){
        ParameterChangePoint* p = in_block->param_changes.at(0)->back();
        params_.get(0).setNormalizedValue(p->normalized_value);
    }
    process(&in_block->audio_buffer, &out_block->audio_buffer);
}

}