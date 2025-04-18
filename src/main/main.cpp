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
    std::string pathToLR = "../src/tests/input_LR/";
    std::string pathToChain = "../src/tests/input_chain/";
    int LRcounter = 1;
    int chainCounter = 1;

    std::vector<std::filesystem::directory_entry> files_lr;
    std::vector<std::filesystem::directory_entry> files_chain;

    for (const auto& entry : std::filesystem::directory_iterator(pathToLR))
    {
        std::string fileName = entry.path().filename().string().substr(2, std::string::npos);
        if (entry.is_regular_file() && fileName != "expected.txt")
        {
            files_lr.push_back(entry);
        }
    }

    for (const auto& entry : std::filesystem::directory_iterator(pathToChain))
    {
        std::string fileName = entry.path().filename().string().substr(2, std::string::npos);
        if (entry.is_regular_file() && fileName != "expected.txt")
        {
            files_chain.push_back(entry);
        }
    }

    std::ranges::sort(files_lr, [] (const auto& lhs, const auto& rhs)
    {
        return lhs.path().filename().string() < rhs.path().filename().string();
    });

    std::ranges::sort(files_chain, [] (const auto& lhs, const auto& rhs)
    {
        return lhs.path().filename().string() < rhs.path().filename().string();
    });

    for (const auto& entry : files_lr)
    {
        if (entry.is_regular_file())
        {
            GrammarInfo grammarInfo;
            grammarInfo.parseInputFiles(entry.path());
            LRDeleter::removeLeftRecursion(grammarInfo);
            LRDeleter::printGrammar(grammarInfo, "LRDelete", LRcounter++);
        }
    }

    for (const auto& entry : files_chain)
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