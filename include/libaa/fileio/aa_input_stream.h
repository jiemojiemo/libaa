
//
// Created by William.Hua on 2021/3/19.
//

#pragma once
#include <stdint.h>

namespace libaa
{
class InputStream
{
public:
    virtual ~InputStream() = default;
    virtual int64_t read(uint8_t* dst_buf, int64_t size) = 0;
    virtual int64_t tellg() = 0;
    virtual int seekg(int64_t pos, int mode) = 0;
    virtual int64_t length() const = 0;
};
}

