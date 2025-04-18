#include "chain_rules_deleter.h"
#include <algorithm>
#include <queue>

#include "../left_recursion_deleter/left_recursion_deleter.h"

void ChainRulesDeleter::removeChainRules(GrammarInfo& grammar)
{

    LRDeleter::removeEpsilonRules(grammar);

    std::map<std::string, std::set<std::string>> chainClosure;

    for (const auto& [nt, _] : grammar.s_rights)
    {
        std::set<std::string> closure = {nt};
        std::vector<std::string> stack = {nt};

        while (!stack.empty())
        {
            std::string current = stack.back();
            stack.pop_back();

            for (const auto& prod : grammar.s_rights[current])
            {
                if (prod.size() == 1 && std::isupper(prod[0][0])) // A → B
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


    std::map<std::string, std::vector<std::vector<std::string>>> newGrammar;

    for (const auto& [A, reachable] : chainClosure)
    {
        for (const auto& B : reachable)
        {
            for (const auto& prod : grammar.s_rights[B])
            {
                // Пропускаем цепные правила B → C
                if (prod.size() == 1 && std::isupper(prod[0][0]))
                    continue;

                if (std::find(newGrammar[A].begin(), newGrammar[A].end(), prod) == newGrammar[A].end())
                {
                    newGrammar[A].push_back(prod);
                }
            }
        }
    }

    grammar.s_rights = std::move(newGrammar);

    removeUnreachableSymbols(grammar);

    std::set<std::string> newNonterms;
    std::set<std::string> newTerms;

    for (const auto& [lhs, productions] : grammar.s_rights)
    {
        newNonterms.insert(lhs);
        for (const auto& prod : productions)
        {
            for (const auto& sym : prod)
            {
                if (std::isupper(sym[0]))  // если заглавная буква — нетерминал
                    newNonterms.insert(sym);
                else
                    newTerms.insert(sym);
            }
        }
    }

    grammar.s_nonterm = std::vector<std::string>(newNonterms.begin(), newNonterms.end());
    grammar.s_term = std::vector<std::string>(newTerms.begin(), newTerms.end());

}

void ChainRulesDeleter::removeUnreachableSymbols(GrammarInfo& grammar)
{
    std::set<std::string> reachable;
    std::queue<std::string> queue;

    reachable.insert(grammar.start_nonterm);
    queue.push(grammar.start_nonterm);

    while (!queue.empty())
    {
        std::string current = queue.front();
        queue.pop();

        auto it = grammar.s_rights.find(current);
        if (it == grammar.s_rights.end()) continue;

        for (const auto& production : it->second)
        {
            for (const auto& symbol : production)
            {
                if (reachable.insert(symbol).second)
                {
                    queue.push(symbol);
                }
            }
        }
    }

    grammar.s_nonterm.erase(
        std::remove_if(grammar.s_nonterm.begin(), grammar.s_nonterm.end(),
            [&](const std::string& nt) {
                return !reachable.count(nt);
            }),
        grammar.s_nonterm.end()
    );


    grammar.s_term.erase(
        std::remove_if(grammar.s_term.begin(), grammar.s_term.end(),
            [&](const std::string& term) {
                return !reachable.count(term);
            }),
        grammar.s_term.end()
    );

    for (auto it = grammar.s_rights.begin(); it != grammar.s_rights.end(); )
    {
        const std::string& lhs = it->first;
        if (!reachable.count(lhs))
        {
            it = grammar.s_rights.erase(it);
            continue;
        }

        auto& productions = it->second;
        productions.erase(
            std::remove_if(productions.begin(), productions.end(),
                [&](const std::vector<std::string>& prod) {
                    return !std::all_of(prod.begin(), prod.end(),
                        [&](const std::string& sym) {
                            return reachable.count(sym);
                        });
                }),
            productions.end()
        );

        if (productions.empty())
            it = grammar.s_rights.erase(it);
        else
            ++it;
    }
}


