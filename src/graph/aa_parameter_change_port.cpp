//
// Created by user on 5/9/22.
//

#include "libaa/graph/aa_parameter_change_port.h"

namespace libaa {

ParameterChangePort::ParameterChangePort(std::shared_ptr<AudioBlock> block)
    : block_(std::move(block)) {
    assert(block_ != nullptr);
}

void ParameterChangePort::pushParameterChange(ParameterChangePoint p) {
    assert(block_ != nullptr);
    block_->param_changes.push(p.index, p);
}

const ParameterChanges &ParameterChangePort::getParameterChanges() const {
    assert(block_ != nullptr);
    return block_->param_changes;
}

ParameterChanges &ParameterChangePort::getParameterChanges() {
    assert(block_ != nullptr);
    return block_->param_changes;
}
} // namespace libaa
