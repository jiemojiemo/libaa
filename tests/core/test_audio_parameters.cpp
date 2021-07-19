
//
// Created by William.Hua on 2021/7/18.
//

#include <gmock/gmock.h>

#include <list>
#include "libaa/core/aa_audio_processor_parameter.h"
using namespace testing;

using namespace libaa;
class Parameters
{
public:
    Parameters() = default;

    int size() const
    {
        return parameters_list.size();
    }

    void addFloatParameter(int param_id, float default_val,float min_plain_value, float max_plain_value){
        eraseParameterIfExist(param_id);

        parameters_list.emplace_back(ParameterType::kFloat, param_id, default_val, min_plain_value, max_plain_value);
    }

    const AudioProcessorParameter* get(int param_id){
        for(const auto& item : parameters_list){
            if(item.getParameterID() == param_id)
            {
                return &item;
            }
        }
        return nullptr;
    }
private:
    void eraseParameterIfExist(int param_id){
        auto iter = findParameter(param_id);
        if(iter != parameters_list.end()){
            parameters_list.erase(iter);
        }
    }
    std::list<AudioProcessorParameter>::iterator findParameter(int param_id){
        auto beg = parameters_list.begin();
        for(; beg != parameters_list.end(); ++beg){
            if(beg->getParameterID() == param_id){
                break;
            }
        }
        return beg;
    }

private:
    std::list<AudioProcessorParameter> parameters_list;
};

class AParameters : public Test
{
public:
    Parameters params;

};

TEST_F(AParameters, DefaultContructWithEmpty)
{
    Parameters params;

    ASSERT_THAT(params.size(), Eq(0));
}

TEST_F(AParameters, AddParametersIncreaseTheSize)
{
    params.addFloatParameter(0, 1.0, 0, 2.0);

    ASSERT_THAT(params.size(), Eq(1));
}

TEST_F(AParameters, CanAddFloatParameter)
{
    params.addFloatParameter(0, 1.0, 0, 2.0);

    auto* p = params.get(0);
    AudioProcessorParameter expected{ParameterType::kFloat, 0, 1.0, 0, 2.0};

    ASSERT_THAT(*p, Eq(expected));
}

TEST_F(AParameters, AddParamterWithExistedIsWillOverwriteBeforeOne)
{
    int param_id = 0;
    params.addFloatParameter(param_id, 1.0, 0, 2.0);
    params.addFloatParameter(param_id, 2.0, 0, 2.0);

    AudioProcessorParameter expected{ParameterType::kFloat, param_id, 2.0, 0, 2.0};
    ASSERT_THAT(params.size(), Eq(1));
    ASSERT_THAT(params.get(param_id), Pointee(expected));
}

TEST_F(AParameters, GetNullptrIfIDNotExist)
{
    auto* p = params.get(2);

    ASSERT_THAT(p, IsNull());
}

