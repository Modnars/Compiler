// Name   : GrammaAnalysis.hpp
// Author : Modnar
// Date   : 2019-09-23
// Copyright (C) 2019 Modnar. All rights reserved.

#ifndef __GRAMMA_ANALYSIS_HPP
#define __GRAMMA_ANALYSIS_HPP

#include <iostream>
#include <vector>
#include <string>

class Production;
class Item;
extern std::ostream &operator<<(std::ostream &, const Production &);

class Production {
public:
    Production(const std::string &left, const std::vector<std::string> &rights) :
            left(left), rights(rights) { }

    const std::string left;
    const std::vector<std::string> rights;
};

#endif /* __GRAMMA_ANALYSIS_HPP */
