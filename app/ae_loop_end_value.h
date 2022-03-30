//
// Created by William on 3/30/22.
//

#ifndef AUDIOEFFECT_AE_LOOP_END_VALUE_H
#define AUDIOEFFECT_AE_LOOP_END_VALUE_H

#pragma once

class LoopEndValue : public DemoValue {
public:
    explicit LoopEndValue(LoopBeginValue *v) : begin_value(v) {}
    void invoke(DemoContext &context) override {
        if (context.current_round < context.num_round) {
            if (begin_value != nullptr) {
                begin_value->invoke(context);
            }
        } else {
            if (next != nullptr) {
                next->invoke(context);
            }
        }
    }

    LoopBeginValue *begin_value{nullptr};
};

#endif // AUDIOEFFECT_AE_LOOP_END_VALUE_H
