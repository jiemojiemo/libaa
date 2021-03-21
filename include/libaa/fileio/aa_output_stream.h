
//
// Created by William.Hua on 2021/3/21.
//

#pragma once
#include <inttypes.h>
namespace libaa
{
class OutputStream
{
public:
    virtual ~OutputStream() = default;
    virtual int64_t write(const uint8_t* source_buf, int64_t size) = 0;
    virtual int64_t tellp() const = 0;
    virtual int seekp(int64_t pos, int mode) = 0;
    virtual int64_t length() = 0;
};
}
