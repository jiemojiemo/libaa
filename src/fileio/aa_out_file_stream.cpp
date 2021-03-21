
//
// Created by William.Hua on 2021/3/21.
//

#include "libaa/fileio/aa_out_file_stream.h"

namespace libaa
{
OFileStream::OFileStream(const std::string& path)
{
    open(path);
}

OFileStream::~OFileStream()
{
    close();
}

bool OFileStream::isOpen() const
{
    return fp_ != nullptr;
}

int OFileStream::open(const std::string& path)
{
    fp_ = fopen(path.c_str(), "wb");
    if(!fp_){
        return -1;
    }
    return 0;
}

void OFileStream::close()
{
    if(fp_)
    {
        fclose(fp_);
        fp_ = nullptr;
    }
}

int64_t OFileStream::write(const uint8_t* source_buf, int64_t size)
{
    if(!isOpen()){
        return -1;
    }

    return fwrite(source_buf, sizeof(uint8_t), size, fp_);
}

int64_t OFileStream::tellp() const
{
    if(!isOpen()){
        return 0;
    }

    return ftell(fp_);
}

int OFileStream::seekp(int64_t pos, int mode)
{
    if(!isOpen()){
        return -1;
    }
    return fseek(fp_, pos, mode);
}

int64_t OFileStream::length()
{
    if(!isOpen()){
        return 0;
    }else
    {
        auto cur_pos = tellp();
        seekp(0, SEEK_END);
        auto length = tellp();
        seekp(cur_pos, SEEK_SET);

        return length;
    }

}
}