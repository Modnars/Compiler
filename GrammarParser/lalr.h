/*
 * @Author: modnarshen
 * @Date: 2024.09.27 16:17:35
 * @Note: Copyrights (c) 2024 modnarshen. All rights reserved.
 */
#pragma once

#include <memory>
#include <set>

#include "lr.h"

namespace mcc {

class LALRItem {
    friend class LALRParser;
    friend std::set<std::string> ComputeLookahead<LALRItem>(const Grammar &, std::shared_ptr<const LALRItem>);

public:
    LALRItem(std::shared_ptr<const LR0Item> item, const std::set<std::string> &lookahead)
        : lr0Item_(item), lookahead_(lookahead) { }

public:
    bool operator<(const LALRItem &rhs) const { return *lr0Item_ < *rhs.lr0Item_; }
    bool operator==(const LALRItem &rhs) const { return *lr0Item_ == *rhs.lr0Item_; }

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

class LALRParser : public mcc::LRParser {
public:
    using LRParser::LRParser;

    int Parse() override;

    // void Analyze(std::istream &is) const override;

public:
    std::shared_ptr<ItemSet<LALRItem>> CLOSURE(std::shared_ptr<ItemSet<LALRItem>> itemSet);

    void ShowDetails(std::ostream &os) const override;

private:
    std::map<std::string, std::shared_ptr<ItemSet<LALRItem>>> computeGOTO(
        std::shared_ptr<const ItemSet<LALRItem>> itemSet);

    std::shared_ptr<LALRItem> lalrItem(std::shared_ptr<const LR0Item> lr0It, const std::set<std::string> &lookahead) {
        const auto iter = items_.find({lr0It, lookahead});
        if (iter != items_.end()) {
            return iter->second;
        }
        auto newItem = std::make_shared<LALRItem>(lr0It, lookahead);
        items_.insert({{lr0It, lookahead}, newItem});
        return newItem;
    }

    std::set<std::string> computeLookahead(std::shared_ptr<const LALRItem> item) const {
        return ComputeLookahead(grammar_, item);
    }

    /// @return true if there is any lookahead symbol was spreaded, else false.
    bool spreadLookahead(std::shared_ptr<LALRItem> item, const std::set<std::string> &lookahead) {
        if (item->lookahead_ == lookahead) {
            return false;
        }
        auto prev = item->lookahead_;
        items_.erase({item->lr0Item_, prev});
        item->lookahead_.insert(lookahead.begin(), lookahead.end());
        items_.insert({{item->lr0Item_, item->lookahead_}, item});
        return prev != item->lookahead_;
    }

    // std::set<std::string> lookahead(std::size_t stateNo, std::shared_ptr<const LR0Item> lr0It) const {
    //     if (stateNo >= lookaheadIndexes_.size()) {
    //         return {};
    //     }
    //     if (auto iter = lookaheadIndexes_[stateNo].find(lr0It); iter != lookaheadIndexes_[stateNo].end()) {
    //         return iter->second;
    //     }
    //     return {};
    // }

    bool spreadLookahead();

private:
    std::map<std::pair<std::shared_ptr<const LR0Item>, std::set<std::string>>, std::shared_ptr<LALRItem>> items_;

    std::size_t closureNum_ = 0UL;
    std::vector<std::shared_ptr<const ItemSet<LALRItem>>> closures_;

    // vec[stateNo]map<lr0Item, set<lookahead>>
    // std::vector<std::map<std::shared_ptr<const LR0Item>, std::set<std::string>>> lookaheadIndexes_;
};

}  // namespace mcc
