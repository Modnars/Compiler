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

enum class ParserType : std::size_t {
    UNKNOWN = 0UL,
    LL1 = 1UL,
    SLR1 = 2UL,
    LR1 = 3UL,
    LALR1 = 4UL,
};

class Parser {
public:
    virtual int Parse() = 0;

    virtual void Analyze(std::istream &is) const = 0;

public:
    virtual void ShowDetails(std::ostream &os) const { }

    virtual void OutputToGraphviz(std::ostream &os) const { }

    virtual void OutputToCsv(std::ostream &os) const { }

    virtual void OutputToGo(std::ostream &os) const { }
};

}  // namespace mcc
