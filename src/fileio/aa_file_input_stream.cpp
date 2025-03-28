
//
// Created by William.Hua on 2021/3/19.
//

#include "libaa/fileio/aa_file_input_stream.h"
#include <iostream>

namespace libaa {
FileInputStream::FileInputStream(const std::string &path) {
    open(path);
}

FileInputStream::~FileInputStream() {
    close();
}

bool FileInputStream::isOpen() const {
    return fp_ != nullptr;
}

int FileInputStream::open(const std::string &path) {
    // close first if open a file already.
    if (fp_) {
        close();
    }

    fp_ = fopen(path.c_str(), "rb");
    if (!fp_) {
        return -1;
    }

    // get file length
    fseek(fp_, 0L, SEEK_END);
    length_ = ftell(fp_);
    fseek(fp_, 0L, SEEK_SET);

    return 0;
}

void FileInputStream::close() {
    if (fp_) {
        fclose(fp_);
        fp_ = nullptr;
        length_ = 0;
    }
}

int64_t FileInputStream::read(uint8_t *dst_buf, int64_t size) {
    if (!isOpen()) {
        std::cerr << "cannot read in close status\n";
        return -1;
    }

    return fread(dst_buf, sizeof(uint8_t), size, fp_);
}
int64_t FileInputStream::tellg() {
    if (!isOpen()) {
        return 0;
    }

    return ftell(fp_);
}
int FileInputStream::seekg(int64_t pos, int mode) {
    if (!isOpen()) {
        return -1;
    }

    return fseek(fp_, pos, mode);
}
int64_t FileInputStream::length() const {
    return length_;
}
} // namespace libaa
