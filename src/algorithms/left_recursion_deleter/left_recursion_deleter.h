#pragma once
#include "../../grammar/grammar_info.h"
class LRDeleter {
public:
    static void printGrammar(const GrammarInfo& grammar);
    static void removeLeftRecursion(GrammarInfo& grammar);
    static bool isLeftRecursion(const std::string& nonterm, const std::vector<std::string>& prod);
};
