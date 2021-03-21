
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

    /**
     * write to file
     * @param source_buf the source of data
     * @param size number of size to write
     * @return number of written bytes, returns -1 if failed
     */
    virtual int64_t write(const uint8_t* source_buf, int64_t size) = 0;

    /**
     * returns the write pointer position
     */
    virtual int64_t tellp() const = 0;

    /**
     * seek the write pointer
     * @param pos the new pos
     * @param mode SEEK_SET	Beginning of file
                   SEEK_CUR	Current position of the file pointer
                   SEEK_END End of file
     * @return 0 if seek successfully, others failed
     */
    virtual int seekp(int64_t pos, int mode) = 0;

    /**
     * return the length of output stream
     */
    virtual int64_t length() = 0;
};
}
