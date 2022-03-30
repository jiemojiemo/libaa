//
// Created by William on 3/28/22.
//
#include "ae_demo_context.h"
#include "ae_demo_pipeline.h"
#include "ae_read_wav_value.h"
#include "ae_write_wav_value.h"
#include <iostream>

#include "ae_processor_value.h"

using namespace std;
int main(int argc, char *argv[]) {
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " regression_cases.yaml\n";
        return -1;
    }

    const std::string yaml_input_path = argv[1];

    std::vector<DemoContext> contexts =
        DemoContext::loadAllFromFile(yaml_input_path);

    DemoPipeline pipeline;
    pipeline.addLast(new ReadWavValue);
    pipeline.addLast(new ProcessorValue);
    pipeline.addLast(new WriteWavValue);

    pipeline.start(contexts[0]);

    return 0;
}