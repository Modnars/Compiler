/*
 * @Author: modnarshen
 * @Date: 2024.09.25 11:00:50
 * @Note: Copyrights (c) 2024 modnarshen. All rights reserved.
 */
#include "lr.h"

namespace lr {

bool operator<(const Item &lhs, const Item &rhs) {
    return std::tie(lhs.production_, lhs.dot_pos_, lhs.lookahead_) <
           std::tie(rhs.production_, rhs.dot_pos_, rhs.lookahead_);
}

}

