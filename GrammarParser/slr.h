/*
 * @Author: modnarshen
 * @Date: 2024.09.27 16:17:19
 * @Note: Copyrights (c) 2024 modnarshen. All rights reserved.
 */
#pragma once

#include <map>
#include <memory>

#include "grammar.h"
#include "lr.h"

namespace mcc {

class SLRParser : public mcc::LRParser {
public:
    using LRParser::LRParser;

public:
    std::shared_ptr<const ItemSet<LR0Item>> CLOSURE(std::shared_ptr<ItemSet<LR0Item>> itemSet);

    std::shared_ptr<const ItemSet<LR0Item>> GOTO(std::shared_ptr<const ItemSet<LR0Item>> itemSet,
                                                 const std::string &shiftSymbol);

    void ShowDetails() const;

public:
    virtual int Parse() override;

private:
    std::map<std::string, std::shared_ptr<ItemSet<LR0Item>>> computeGOTO(
        std::shared_ptr<const ItemSet<LR0Item>> itemSet);

private:
    std::map<std::size_t, std::shared_ptr<const ItemSet<LR0Item>>> closures_;
    std::size_t closureNum_ = 0UL;
};

}  // namespace mcc
