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

    virtual int Analyze(std::istream &is) const override;

public:
    std::shared_ptr<const ItemSet<LR1Item>> CLOSURE(std::shared_ptr<ItemSet<LR1Item>> itemSet);

    std::shared_ptr<const ItemSet<LR1Item>> GOTO(std::shared_ptr<const ItemSet<LR1Item>> itemSet,
                                                 const std::string &shiftSymbol);

    void ShowDetails() const;

private:
    void computeAndCacheItems();

    std::shared_ptr<const LR0Item> lr0Item(std::shared_ptr<const Production> p, std::size_t pos) const {
        const auto iter = lr0Items_.find(std::make_pair(p, pos));
        if (iter == lr0Items_.end()) {
            return nullptr;
        }
        return iter->second;
    }

    std::shared_ptr<const LR1Item> lr1Item(std::shared_ptr<const LR0Item> item,
                                           const std::set<std::string> &lookahead) const {
        const auto iter = items_.find(std::make_pair(item, lookahead));
        if (iter == items_.end()) {
            return nullptr;
        }
        return iter->second;
    }

    std::shared_ptr<const LR1Item> newLr1Item(std::shared_ptr<const LR0Item> lr0It,
                                              const std::set<std::string> &lookahead) {
        auto newItem = lr1Item(lr0It, lookahead);
        if (newItem != nullptr) {
            return newItem;
        }
        newItem = std::make_shared<const LR1Item>(lr0It, lookahead);
        this->items_.insert({{lr0It, lookahead}, newItem});
        return newItem;
    }

    std::shared_ptr<const LR1Item> newLr1Item(std::shared_ptr<const Production> p, std::size_t pos,
                                              const std::set<std::string> &lookahead) {
        return newLr1Item(lr0Item(p, pos), lookahead);
    }

    std::set<std::string> computeLookahead(std::shared_ptr<const LR1Item> item) const;

    std::map<std::string, std::shared_ptr<ItemSet<LR1Item>>> computeGOTO(
        std::shared_ptr<const ItemSet<LR1Item>> itemSet);

    void fillActionTable(std::size_t stateNum, const std::string &symbol, std::int64_t val);

    int searchActionTable(std::size_t stateNum, const std::string &symbol, std::int64_t &val) const;

private:
    std::map<std::pair<std::shared_ptr<const Production>, std::size_t>, std::shared_ptr<const LR0Item>> lr0Items_;
    std::map<std::pair<std::shared_ptr<const LR0Item>, std::set<std::string>>, std::shared_ptr<const LR1Item>> items_;
    std::map<std::size_t, std::shared_ptr<const ItemSet<LR1Item>>> closures_;
    std::size_t closureNum_ = 0UL;

    std::vector<std::map<std::string, std::int64_t>> actionTable_;
};

}  // namespace mcc
