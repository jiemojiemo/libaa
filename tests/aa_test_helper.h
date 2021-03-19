//
// Created by william on 2019/12/9.
//

#pragma once
#include <gmock/gmock.h>
using namespace testing;

MATCHER_P(FloatNearPointwise, tol, "Out of range") {
    return (std::get<0>(arg)>std::get<1>(arg)-tol && std::get<0>(arg)<std::get<1>(arg)+tol) ;
}

namespace libaa
{
MATCHER_P(NearWithPrecision, ferr, "")
{
    return abs(get<0>(arg) - get<1>(arg)) < ferr;
}

MATCHER_P2(ComplexNearEqual, ferr, rhs, "")
{
    return abs(arg - rhs) < ferr;
}

class ScopeFile
{
public:
    explicit ScopeFile(std::string output_path);
    virtual ~ScopeFile();
protected:
    std::string output_path_;
};

class ScopeWaveFile : public ScopeFile
{
public:
    ScopeWaveFile(std::string output_path,
                  size_t sample_rate,
                  size_t num_channel,
                  size_t num_frames,
                  size_t num_bits,
                  float fill_val);

};

class ScopeTextFile : public ScopeFile
{
public:
    ScopeTextFile(std::string output_path,
                  const std::string& txt);
};

}
