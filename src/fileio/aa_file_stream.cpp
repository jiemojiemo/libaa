
//
// Created by William.Hua on 2021/3/19.
//

#include "libaa/fileio/aa_file_stream.h"
#include <iostream>

namespace libaa
{
FileStream::FileStream(const std::string& path)
{
    open(path);
}

FileStream::~FileStream(){
    close();
}

bool FileStream::isOpen() const{
    return fp_ != nullptr;
}

int FileStream::open(const std::string& path) {
    // close first if open a file already.
    if(fp_){
        close();
    }

    fp_ = fopen(path.c_str(), "rb");
    if(!fp_){
        return -1;
    }

    // get file length
    fseek(fp_, 0L, SEEK_END);
    length_ = ftell(fp_);
    fseek(fp_, 0L, SEEK_SET);

    return 0;
}

void FileStream::close(){
    if(fp_){
        fclose(fp_);
        fp_ = nullptr;
        length_ = 0;
    }
}

int64_t FileStream::read(uint8_t *dst_buf, int64_t size)  {
    if(!isOpen()){
        std::cerr <<"cannot read in close status\n";
        return -1;
    }

    return fread(dst_buf, sizeof(uint8_t), size, fp_);
}
int64_t FileStream::tellg()  {
    if(!isOpen()){
        return 0;
    }

    return ftell(fp_);
}
int FileStream::seekg(int64_t pos, int mode)  {
    if(!isOpen()){
        return -1;
    }

    return fseek(fp_, pos, mode);
}
int64_t FileStream::length() const  {
    return length_;
}
}
