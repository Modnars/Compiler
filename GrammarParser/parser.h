/*
 * @Author: modnarshen
 * @Date: 2024.09.25 11:10:36
 * @Note: Copyrights (c) 2024 modnarshen. All rights reserved.
 */
#pragma once

#include <map>
#include <memory>
#include <string>
#include <vector>

namespace mcc {

class Parser {
public:
    virtual int Parse() = 0;

    virtual int Analyze(std::istream &is) const = 0;
};

}  // namespace mcc
