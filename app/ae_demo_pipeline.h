//
// Created by William on 3/28/22.
//

#ifndef AUDIOEFFECT_AE_DEMO_PIPELINE_H
#define AUDIOEFFECT_AE_DEMO_PIPELINE_H

#pragma once
#include "ae_demo_value.h"
class DemoPipeline {
public:
    ~DemoPipeline() {
        for (size_t i = 0; i < values.size(); ++i) {
            delete values[i];
        }
    }
    virtual void addLast(DemoValue *new_v) {
        if (!values.empty()) {
            DemoValue *last_value = values.back();
            last_value->setNext(new_v);
        }

        values.push_back(new_v);
    }

    virtual void start(DemoContext &context) {
        if (!values.empty()) {
            values.front()->invoke(context);
        }
    }

    std::vector<DemoValue *> values;
};

#endif // AUDIOEFFECT_AE_DEMO_PIPELINE_H
