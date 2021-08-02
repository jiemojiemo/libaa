
//
// Created by William.Hua on 2021/8/2.
//

#include <gmock/gmock.h>
#include "libaa/core/aa_parameter_change_ringbuffer.h"
#include <memory>
using namespace testing;
using namespace libaa;

class ParameterChanges
{
public:
    using RingBuffers = std::vector<std::shared_ptr<ParameterChangeRingbuffer>>;
    typedef typename std::vector<std::shared_ptr<ParameterChangeRingbuffer>> array_type;
    typedef typename array_type::iterator iterator;
    typedef typename array_type::const_iterator const_iterator;

    explicit ParameterChanges(size_t num_param_change_ring_buffer = 0)
    {
        for(auto i = 0u; i < num_param_change_ring_buffer; ++i){
            auto* ringbuffer = new ParameterChangeRingbuffer{kMaxParameterChanges};
            ringbuffer->setParameterIndex(i);

            changes_array_.emplace_back(ringbuffer);
        }
    }

    const RingBuffers& getParameterChangesArray() const {
        return changes_array_;
    }

    inline iterator begin() noexcept { return changes_array_.begin(); }
    inline const_iterator cbegin() const noexcept { return changes_array_.cbegin(); }
    inline iterator end() noexcept { return changes_array_.end(); }
    inline const_iterator cend() const noexcept { return changes_array_.cend(); }

private:
    RingBuffers changes_array_;
    constexpr static size_t kMaxParameterChanges = 512;
};

class AParameterChanges : public Test
{
public:

};

TEST_F(AParameterChanges, CanConstructByDefaut)
{
    auto param_changes = ParameterChanges{};
}

TEST_F(AParameterChanges, DefautConstructInitWithEmptyParameterRingBuffer)
{
    auto param_changes = ParameterChanges{};

    ASSERT_TRUE(param_changes.getParameterChangesArray().empty());
}

TEST_F(AParameterChanges, CanConstuctWithSizeOfParamterChange)
{
    size_t num_param_change_buffer = 2;
    auto param_changes = ParameterChanges{num_param_change_buffer};

    ASSERT_THAT(param_changes.getParameterChangesArray().size(), Eq(num_param_change_buffer));
}

TEST_F(AParameterChanges, RingbufferHasIncreasedIndex)
{
    size_t num_param_change_buffer = 2;
    auto param_changes = ParameterChanges{num_param_change_buffer};

    const ParameterChanges::RingBuffers& ringbuffers = param_changes.getParameterChangesArray();

    ASSERT_THAT(ringbuffers[0]->getParameterIndex(), Eq(0));
    ASSERT_THAT(ringbuffers[1]->getParameterIndex(), Eq(1));
}