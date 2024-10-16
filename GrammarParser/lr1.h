/*
 * @Author: modnarshen
 * @Date: 2024.10.13 15:46:15
 * @Note: Copyrights (c) 2024 modnarshen. All rights reserved.
 */
#pragma once

#include "lr.h"

namespace mcc {

class LR1Item {
    friend class LR1Parser;

public:
    LR1Item(std::shared_ptr<const LR0Item> item, const std::set<std::string> &lookahead)
        : lr0Item_(item), lookahead_(lookahead) { }

    bool operator<(const LR1Item &rhs) const {
        return std::tie(*lr0Item_, lookahead_) < std::tie(*rhs.lr0Item_, rhs.lookahead_);
    }

    std::string ToString() const;

public:
    bool HasNextSymbol() const { return lr0Item_->HasNextSymbol(); }

    const std::string &NextSymbol() const { return lr0Item_->NextSymbol(); }

    bool CanReduce() const { return lr0Item_->CanReduce(); }

    std::size_t DotPos() const { return lr0Item_->DotPos(); }

    const std::vector<std::string> &Right() const { return lr0Item_->Right(); }

private:
    std::shared_ptr<const LR0Item> lr0Item_ = nullptr;
    std::set<std::string> lookahead_;
};

class LR1Parser : public mcc::LRParser {
public:
    using LRParser::LRParser;

public:
    virtual int Parse() override;

public:
    std::shared_ptr<const ItemSet<LR1Item>> CLOSURE(std::shared_ptr<ItemSet<LR1Item>> itemSet);

    std::shared_ptr<const ItemSet<LR1Item>> GOTO(std::shared_ptr<const ItemSet<LR1Item>> itemSet,
                                                 const std::string &shiftSymbol);

    void ShowDetails(std::ostream &os) const override;

private:
    std::shared_ptr<const LR1Item> newLr1Item(std::shared_ptr<const LR0Item> lr0It,
                                              const std::set<std::string> &lookahead) {
        const auto iter = items_.find({lr0It, lookahead});
        if (iter != items_.end()) {
            return iter->second;
        }
        auto newItem = std::make_shared<const LR1Item>(lr0It, lookahead);
        items_.insert({{lr0It, lookahead}, newItem});
        return newItem;
    }

    std::set<std::string> computeLookahead(std::shared_ptr<const LR1Item> item) const;

    std::map<std::string, std::shared_ptr<ItemSet<LR1Item>>> computeGOTO(
        std::shared_ptr<const ItemSet<LR1Item>> itemSet);

private:
    std::map<std::pair<std::shared_ptr<const LR0Item>, std::set<std::string>>, std::shared_ptr<const LR1Item>> items_;
    std::map<std::size_t, std::shared_ptr<const ItemSet<LR1Item>>> closures_;
    std::size_t closureNum_ = 0UL;
};

}  // namespace mcc
