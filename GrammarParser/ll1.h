/*
 * @Author: modnarshen
 * @Date: 2024.10.13 16:02:32
 * @Note: Copyrights (c) 2024 modnarshen. All rights reserved.
 */
#pragma once

#include <cstddef>
#include <ostream>
#include <unordered_map>

#include "grammar.h"
#include "parser.h"

namespace mcc {

class LL1Parser : public mcc::Parser {
public:
    LL1Parser(Grammar &grammar) : grammar_(grammar) { }

public:
    virtual int Parse() override;

    virtual int Analyze(std::istream &is) const override;

public:
    void ShowDetails(std::ostream &os) const override;

protected:
    int fillPredictionTable(const std::string &nonTerminal, const std::string &terminal,
                            std::shared_ptr<const Production> production);

    std::shared_ptr<const Production> searchPredictionTable(const std::string &nonTerminal,
                                                            const std::string &terminal) const {
        auto map_iter = predictionTable_.find(nonTerminal);
        if (map_iter == predictionTable_.end()) {
            return nullptr;
        }
        auto inner_map_iter = map_iter->second.find(terminal);
        if (inner_map_iter == map_iter->second.end()) {
            return nullptr;
        }
        return inner_map_iter->second;
    }

private:
    Grammar &grammar_;

    std::unordered_map<std::string, std::unordered_map<std::string, std::shared_ptr<const Production>>>
        predictionTable_;
    bool parsedSucc_ = true;
};

}  // namespace mcc
