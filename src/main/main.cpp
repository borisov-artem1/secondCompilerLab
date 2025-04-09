#include <iostream>
#include "../grammar/grammar_info.h"

#include <vector>
#include <map>
#include <string>
#include <sstream>

using namespace std;

using Production = vector<string>;
using Grammar = map<string, vector<Production>>;


bool isLeftRecursive(const string& nonterminal, const Production& prod) {
    return !prod.empty() && prod[0] == nonterminal;
}

void removeLeftRecursion(Grammar& grammar, const string& nonterminal) {
    vector<Production> alpha;
    vector<Production> beta;

    for (const auto& prod : grammar[nonterminal]) {
        if (isLeftRecursive(nonterminal, prod)) {
            Production suffix(prod.begin() + 1, prod.end());
            suffix.push_back(nonterminal + "'");
            alpha.push_back(suffix);
        } else {
            Production newProd = prod;
            newProd.push_back(nonterminal + "'");
            beta.push_back(newProd);
        }
    }

    if (!alpha.empty()) {
        grammar[nonterminal] = beta;
        for (auto& prod : alpha) {
            grammar[nonterminal + "'"].push_back(prod);
        }
        grammar[nonterminal + "'"].push_back({"ε"});  // пустая строка
    }
}

void printGrammar(const Grammar& grammar) {
    for (const auto& [nt, prods] : grammar) {
        cout << nt << " → ";
        for (size_t i = 0; i < prods.size(); ++i) {
            for (const auto& sym : prods[i]) cout << sym;
            if (i != prods.size() - 1) cout << " | ";
        }
        cout << endl;
    }
}

int main() {
    Grammar grammar;

    // Пример: A → Aa | Ab | c
    grammar["E"] = {
        {"E", "+", "T"},
        {"T"}
    };
    grammar["T"] = {
        {"T", "*", "F"},
        {"F"}
    };
    grammar["F"] = {
        {"a"},
        {"(", "E", ")"}
    };

    cout << "До удаления левой рекурсии:\n";
    printGrammar(grammar);

    removeLeftRecursion(grammar, "E");
    removeLeftRecursion(grammar, "T");
    removeLeftRecursion(grammar, "F");

    cout << "\nПосле удаления левой рекурсии:\n";
    printGrammar(grammar);

    return 0;
}