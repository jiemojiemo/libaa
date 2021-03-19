
//
// Created by William.Hua on 2021/3/19.
//

#pragma once
#include "aa_input_stream.h"
#include <string>
namespace libaa
{

class FileStream : public InputStream
{
public:
    FileStream() = default;
    explicit FileStream(const std::string& path);

    ~FileStream();

    bool isOpen() const;

    int open(const std::string& path);

    void close();

    int64_t read(uint8_t *dst_buf, int64_t size) override;
    int64_t tellg() override ;
    int seekg(int64_t pos, int mode) override;
    int64_t length() const override ;

private:
    FILE* fp_{nullptr};
    int64_t length_{0};
};
}
