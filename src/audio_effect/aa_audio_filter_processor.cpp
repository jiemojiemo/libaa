//
// Created by hw on 2021/12/11.
//

#include "libaa/audio_effect/aa_audio_filter_processor.h"

namespace libaa {
class AudioFilterProcessor::Impl {
public:
    Impl() {
        createParameters();
    }
    void createParameters() {
        param_.pushChoiceParameter("Filter Type", 0,
                                   {
                                       "LPF1",
                                       "HPF1",
                                       "LPF2",
                                       "HPF2",
                                       "BPF2",
                                       "BSF2",
                                       "ButterLPF2",
                                       "ButterHPF2",
                                       "ButterBPF2",
                                       "ButterBSF2",
                                       "LWRLPF2",
                                       "LWRHPF2",
                                       "APF1",
                                       "APF2",
                                       "LowShelf",
                                       "HighShelf",
                                       "NCQParaEQ",
                                       "CQParaEQ",
                                   });
        param_.pushFloatParameter("FC", 20, 20, 20500);
        param_.pushFloatParameter("Q", 0.707, 0, 20);
        param_.pushFloatParameter("Gain", 0, -35, 35);
    }

    int prepareToPlay(ProcessorPrepareConfig config) {
        sample_rate_ = config.sample_rate;

        utilities_.updateCoeffes(
            static_cast<FilterType>(param_.get(0).getPlainValue()),
            param_.get(1).getPlainValue(), param_.get(2).getPlainValue(),
            param_.get(3).getPlainValue(), sample_rate_);

        return 0;
    }

    int processBlock(AudioBlock *in_block, AudioBlock *output_block) {
        if (output_block != in_block) {
            output_block->audio_buffer.copyFrom(&in_block->audio_buffer);
        }

        if (!in_block->param_changes.empty()) {
            for (const auto &changes : in_block->param_changes) {
                int param_index = changes.first;
                float norm_val = changes.second->back()->normalized_value;
                param_.get(param_index).setNormalizedValue(norm_val);
            }

            utilities_.updateCoeffes(
                static_cast<FilterType>(param_.get(0).getPlainValue()),
                param_.get(1).getPlainValue(), param_.get(2).getPlainValue(),
                param_.get(3).getPlainValue(), sample_rate_);
        }

        for (auto c = 0u; c < output_block->audio_buffer.getNumberChannels();
             ++c) {
            float *channel = output_block->audio_buffer.getWriterPointer(c);
            Biquad &b = utilities_.biquad_array[c];
            for (auto i = 0u; i < output_block->audio_buffer.getNumberFrames();
                 ++i) {
                channel[i] = b.processSample(channel[i]);
            }
        }

        return 0;
    }

    AudioProcessorParameters param_;
    AudioFilterUtilities utilities_;
    float sample_rate_;
};

AudioFilterProcessor::AudioFilterProcessor()
    : impl_(std::make_shared<Impl>()) {}

int AudioFilterProcessor::prepareToPlay(ProcessorPrepareConfig config) {
    return impl_->prepareToPlay(config);
}

std::string AudioFilterProcessor::getName() {
    return {"Filter"};
}

std::vector<uint8_t> AudioFilterProcessor::getState() {
    return std::vector<uint8_t>();
}

int AudioFilterProcessor::setState(const uint8_t *data, int data_size) {
    (void)(data);
    (void)(data_size);
    return 0;
}

int AudioFilterProcessor::getLatencySamples() {
    return 0;
}

int AudioFilterProcessor::getTailLengthSamples() const {
    return 0;
}

void AudioFilterProcessor::reset() {}

const AudioProcessorParameters &AudioFilterProcessor::getParameters() {
    return impl_->param_;
}

int AudioFilterProcessor::processBlock(AudioBlock *in_block,
                                       AudioBlock *output_block) {
    return impl_->processBlock(in_block, output_block);
}

} // namespace libaa
