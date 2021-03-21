
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
    /**
     * read from inout stream
     * @param dst_buf destination buffer
     * @param size the read size
     * @return number of bytes actually read, 0 if reach the end, negative value if has error
     */
    virtual int64_t read(uint8_t* dst_buf, int64_t size) = 0;

    /**
     * return the read pointer position
     */
    virtual int64_t tellg() = 0;
    /**
     * seek read pointer
     * @param pos the pos you want to go
     * @param mode SEEK_SET	Beginning of file
                   SEEK_CUR	Current position of the file pointer
                   SEEK_END End of file
     * @return 0 if seek successfully, others failed
     */
    virtual int seekg(int64_t pos, int mode) = 0;

    /**
     * return the length of stream
     */
    virtual int64_t length() const = 0;
};
}

