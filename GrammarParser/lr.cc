/*
 * @Author: modnarshen
 * @Date: 2024.09.25 11:00:50
 * @Note: Copyrights (c) 2024 modnarshen. All rights reserved.
 */
#include <algorithm>
#include <iostream>
#include <queue>
#include <sstream>
#include <string>

#include "grammar.h"
#include "lr.h"
#include "util.h"

namespace {

static const std::string _DotMark = "·";

}

namespace mcc {

std::string LR0Item::ToString() const {
    std::stringstream ss;
    ss << production_->Left() << " -> ";
    const auto &right = production_->Right();
    for (std::size_t i = 0UL; i <= right.size(); ++i) {
        if (i == dotPos_) {
            ss << _DotMark << "";
        }
        if (i < right.size()) {
            ss << right[i];
        }
        if (i + 1UL < right.size()) {
            ss << " ";
        }
    }
    return ss.str();
}

}  // namespace mcc
