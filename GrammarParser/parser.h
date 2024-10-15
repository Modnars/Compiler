/*
 * @Author: modnarshen
 * @Date: 2024.09.25 11:10:36
 * @Note: Copyrights (c) 2024 modnarshen. All rights reserved.
 */
#pragma once

#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>

namespace mcc {

class Parser {
public:
    virtual int Parse() = 0;

    virtual int Analyze(std::istream &is) const = 0;

public:
    virtual void OutputToGraphviz(std::ostream &os) const { }
    virtual void OutputToCsv(std::ostream &os) const { }
};

}  // namespace mcc
