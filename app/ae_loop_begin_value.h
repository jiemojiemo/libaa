//
// Created by William on 3/30/22.
//

#ifndef AUDIOEFFECT_AE_LOOP_BEGIN_VALUE_H
#define AUDIOEFFECT_AE_LOOP_BEGIN_VALUE_H

#pragma once
#include "ae_demo_value.h"
class LoopBeginValue : public DemoValue {
public:
    void process(DemoContext &context) override {
        context.current_round += 1;
    }
};

#endif // AUDIOEFFECT_AE_LOOP_BEGIN_VALUE_H
