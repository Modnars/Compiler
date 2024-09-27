/*
 * @Author: modnarshen
 * @Date: 2024.09.27 16:17:35
 * @Note: Copyrights (c) 2024 modnarshen. All rights reserved.
 */
#pragma once

#include <memory>
#include <set>

#include "parser.h"

namespace lalr {

class Item {
private:
    std::shared_ptr<Production> production_;
    std::size_t dot_pos_ = 0UL;
    std::set<std::string> look_ahead_;
};

} // namespace lalr
