/*
 * @Author: modnarshen
 * @Date: 2024.10.08 10:13:04
 * @Note: Copyrights (c) 2024 modnarshen. All rights reserved.
 */
#include "lr0.h"

#include <sstream>

namespace lr0 {
std::string Item::ToString() const {
    std::stringstream ss;
    ss << production_->Left() << " -> ";
    const auto &right = production_->Right();
    for (std::size_t i = 0UL; i <= right.size(); ++i) {
        if (i == dotPos_) {
            ss << lr0::DotMark << "";
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
}  // namespace lr0
