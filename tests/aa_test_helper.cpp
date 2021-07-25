
//
// Created by William.Hua on 2021/7/25.
//

#include "aa_test_helper.h"

namespace libaa
{
bool operator==(const AudioBufferNew<float>& lhs, const AudioBufferNew<float>& rhs)
{
    if(lhs.getNumberFrames() != rhs.getNumberFrames() ||
        lhs.getNumberChannels() != rhs.getNumberChannels()){
        return false;
    }

    for(auto c = 0u; c < lhs.getNumberChannels(); ++c)
    {
        for(auto i = 0u; i < lhs.getNumberFrames(); ++i)
        {
            if(lhs.getReadPointer(c)[i] != rhs.getReadPointer(c)[i])
                return false;
        }
    }

    return true;
}

}
