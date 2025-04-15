#include "chain_rules_deleter.h"
#include <algorithm>

#include "../left_recursion_deleter/left_recursion_deleter.h"

void ChainRulesDeleter::removeChainRules(GrammarInfo& grammar)
{
    LRDeleter::removeEpsilonRules(grammar);
    std::map<std::string, std::set<std::string>> chainClosure;

    for (auto [nt, _] : grammar.s_rights)
    {
        std::set<std::string> closure = {nt};
        std::vector<std::string> stack = {nt};

        while(!stack.empty())
        {
            std::string current = stack.back();
            stack.pop_back();

            for (const auto &prod : grammar.s_rights[current])
            {
                if (prod.size() == 1 && std::isupper(prod[0][0]))
                {
                    std::string B = prod[0];
                    if (closure.insert(B).second)
                    {
                        stack.push_back(B);
                    }
                }
            }
        }
        chainClosure[nt] = closure;
    }

    grammar.s_rights = formingNewGrammar(grammar, chainClosure);

}

std::map<std::string, std::vector<std::vector<std::string>>> ChainRulesDeleter::formingNewGrammar(GrammarInfo& grammar, std::map<std::string, std::set<std::string>>& chainClosure)
{
    std::map<std::string, std::vector<std::vector<std::string>>> newGrammar;

    for (const auto& [nt, closureSet] : chainClosure)
    {
        for (const auto &B : closureSet)
        {
            for (const auto& prod : grammar.s_rights[B])
            {
                if (!(prod.size() == 1 && std::isupper(prod[0][0])))
                {
                    bool exists = std::any_of(newGrammar[nt].begin(), newGrammar[nt].end(), [&](const std::vector<std::string>& p)
                    {
                        return p == prod;
                    });
                    if (!exists)
                    {
                        newGrammar[nt].push_back(prod);
                    }
                }
            }
        }
    }
    return newGrammar;
}
