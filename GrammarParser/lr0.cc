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
    ss << this->production_->Left << " -> ";
    for (std::size_t i = 0UL; i <= this->production_->Right.size(); ++i) {
        if (i == this->dotPos_) {
            ss << lr0::DotMark << "";
        }
        if (i < this->production_->Right.size()) {
            ss << this->production_->Right[i];
        }
        if (i + 1UL < this->production_->Right.size()) {
            ss << " ";
        }
    }
    return ss.str();
}
}  // namespace lr0
