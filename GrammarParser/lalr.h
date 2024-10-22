/*
 * @Author: modnarshen
 * @Date: 2024.09.27 16:17:35
 * @Note: Copyrights (c) 2024 modnarshen. All rights reserved.
 */
#pragma once

#include <map>
#include <memory>
#include <set>

#include "lr.h"

namespace mcc {

class LALRParser : public mcc::LRParser {
public:
    using LRParser::LRParser;

    int Parse() override;

public:
    std::shared_ptr<const ItemSet<LR0Item>> CLOSURE(std::shared_ptr<const ItemSet<LR0Item>> itemSet);

    void ShowDetails(std::ostream &os) const override;

private:
    std::map<std::string, std::shared_ptr<ItemSet<LR0Item>>> computeGOTO(
        std::shared_ptr<const ItemSet<LR0Item>> itemSet);

    bool updateLookahead(std::size_t stateNo, std::shared_ptr<const LR0Item> lr0It,
                         const std::set<std::string> &lookahead) {
        if (stateNo >= itemSetLookahead_.size()) {
            return false;
        }
        auto &targetSet = itemSetLookahead_[stateNo][lr0It];
        if (targetSet == lookahead) {
            return false;
        } else {
            auto prev = targetSet;
            targetSet.insert(lookahead.begin(), lookahead.end());
            return prev != targetSet;
        }
        return false;
    }

    bool spreadInClosure(std::shared_ptr<const ItemSet<LR0Item>> coreItemSet);

    bool spreadFromState(std::size_t from, std::shared_ptr<const ItemSet<LR0Item>> toState);

    const std::set<std::string> &lookahead(std::size_t stateNo, std::shared_ptr<const LR0Item> lr0It) const {
        static const std::set<std::string> empty;
        if (stateNo < itemSetLookahead_.size()) {
            if (auto iter = itemSetLookahead_[stateNo].find(lr0It); iter != itemSetLookahead_[stateNo].end()) {
                return iter->second;
            }
        }
        return empty;
    }

    bool addLookahead(std::size_t stateNo, std::shared_ptr<const LR0Item> lr0It, const std::set<std::string> &symbols) {
        while (stateNo >= itemSetLookahead_.size()) {
            itemSetLookahead_.emplace_back(std::map<std::shared_ptr<const LR0Item>, std::set<std::string>>());
        }
        auto &target = itemSetLookahead_[stateNo][lr0It];
        if (target == symbols) {
            return false;
        }
        auto prev = target;
        target.insert(symbols.begin(), symbols.end());
        return prev != target;
    }

    void addGotoState(std::size_t from, std::shared_ptr<const ItemSet<LR0Item>> itemSet);

private:
    std::size_t coreItemSetNum_ = 0UL;
    std::vector<std::shared_ptr<const ItemSet<LR0Item>>> coreItemSets_;
    std::map<std::size_t, std::vector<std::shared_ptr<const ItemSet<LR0Item>>>> gotoStates_;

    // vec[stateNo]map<lr0Item, set<lookahead>>
    std::vector<std::map<std::shared_ptr<const LR0Item>, std::set<std::string>>> itemSetLookahead_;
};

}  // namespace mcc
