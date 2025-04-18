#pragma once
#include "../../grammar/grammar_info.h"

class ChainRulesDeleter
{
public:
    static void removeChainRules(GrammarInfo& grammar);
    static void removeUnreachableSymbols(GrammarInfo& grammar);
    static std::map<std::string, std::vector<std::vector<std::string>>> formingNewGrammar(GrammarInfo& grammar, std::map<std::string, std::set<std::string>>& chainClosure);
};