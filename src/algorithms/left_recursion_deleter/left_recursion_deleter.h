#pragma once
#include "../../grammar/grammar_info.h"
class LRDeleter {
public:
    static void printGrammar(const GrammarInfo& grammar, const std::string& nameDir, int&& counter);
    static void printToDirs();
    static void removeLeftRecursion(GrammarInfo& grammar);
    static bool isLeftRecursion(const std::string& nonterm, const std::vector<std::string>& prod);
    static void removeEpsilonRules(GrammarInfo& grammar);
    static std::set<std::string> findNullable(const GrammarInfo& grammar);
};

