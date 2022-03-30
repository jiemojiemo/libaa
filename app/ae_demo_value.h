//
// Created by William on 3/28/22.
//

#ifndef AUDIOEFFECT_AE_DEMO_VALUE_H
#define AUDIOEFFECT_AE_DEMO_VALUE_H

#pragma once
#include "ae_demo_context.h"

class DemoValue {
public:
    virtual void invoke(DemoContext &context) {
        process(context);
        if (next != nullptr) {
            next->invoke(context);
        }
    }

    virtual void process(DemoContext &context){};

    virtual void setNext(DemoValue *n) { next = n; }

    DemoValue *next{nullptr};
};

#endif // AUDIOEFFECT_AE_DEMO_VALUE_H
