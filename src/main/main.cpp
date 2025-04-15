#include <algorithm>
#include <iostream>
#include "../grammar/grammar_info.h"
#include "../algorithms/left_recursion_deleter/left_recursion_deleter.h"
#include "../algorithms/chain_rules_deleter/chain_rules_deleter.h"

#include <vector>
#include <map>
#include <string>
#include <sstream>
#include <filesystem>

void parseDir()
{
    std::string pathToDir = "../src/tests/input/";
    int LRcounter = 1;
    int chainCounter = 1;

    std::vector<std::filesystem::directory_entry> files;

    for (const auto& entry : std::filesystem::directory_iterator(pathToDir))
    {
        if (entry.is_regular_file())
        {
            files.push_back(entry);
        }
    }

    std::ranges::sort(files, [] (const auto& lhs, const auto& rhs)
    {
        return lhs.path().filename().string() < rhs.path().filename().string();
    });

    for (const auto& entry : files)
    {
        if (entry.is_regular_file())
        {
            GrammarInfo grammarInfo;
            grammarInfo.parseInputFiles(entry.path());
            LRDeleter::removeLeftRecursion(grammarInfo);
            LRDeleter::printGrammar(grammarInfo, "LRDelete", LRcounter++);
        }
    }

    for (const auto& entry : files)
    {
        if (entry.is_regular_file())
        {
            GrammarInfo grammarInfo;
            grammarInfo.parseInputFiles(entry.path());
            ChainRulesDeleter::removeChainRules(grammarInfo);
            LRDeleter::printGrammar(grammarInfo, "ChainRulesDelete", chainCounter++);
        }
    }
}

int main() {
    parseDir();
    return 0;
}