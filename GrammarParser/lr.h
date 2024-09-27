/*
 * @Author: modnarshen
 * @Date: 2024.09.25 11:00:40
 * @Note: Copyrights (c) 2024 modnarshen. All rights reserved.
 */
#pragma once

#include <memory>
#include <vector>

#include "parser.h"

namespace lr {

class Item {
    friend bool operator<(const Item &lhs, const Item &rhs);

private:
    std::shared_ptr<Production> production_;
    std::size_t dot_pos_ = 0UL;
    std::string lookahead_;
};

bool operator<(const Item &lhs, const Item &rhs);

class Parser {
private:
    std::vector<std::shared_ptr<Production>> productions_;
};

} // namespace lr
