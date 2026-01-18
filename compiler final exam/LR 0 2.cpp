
#include<bits/stdc++.h>
using namespace std;

struct Item {
    string lhs;
    string rhs;
    int dot;
};

vector<Item> grammar;
vector<vector<Item>> states;
unordered_set<char> nullableNonTerminals;  // Store nullable non-terminals

bool sameItem(Item a, Item b) {
    return a.lhs == b.lhs && a.rhs == b.rhs && a.dot == b.dot;
}

bool contains(vector<Item> state, Item it) {
    for (auto x : state)
        if (sameItem(x, it))
            return true;
    return false;
}

// Check if a production can produce epsilon
bool canProduceEpsilon(char lhs) {
    for (auto g : grammar) {
        if (g.lhs == string(1, lhs) && g.rhs == "ε") {
            return true;
        }
    }
    return false;
}

vector<Item> closure(vector<Item> state) {
    bool added = true;
    while (added) {
        added = false;
        int originalSize = state.size();

        for (int i = 0; i < state.size(); i++) {
            Item it = state[i];

            if (it.dot < it.rhs.length()) {
                char sym = it.rhs[it.dot];

                // If the symbol is a non-terminal, add its productions
                if (sym >= 'A' && sym <= 'Z') {
                    for (auto g : grammar) {
                        if (g.lhs == string(1, sym)) {
                            Item newItem = {g.lhs, g.rhs, 0};
                            if (!contains(state, newItem)) {
                                state.push_back(newItem);
                                added = true;
                            }
                        }
                    }
                }
            }

            // Special case for nullable non-terminals and epsilon
            if (it.dot < it.rhs.length()) {
                char sym = it.rhs[it.dot];
                if (nullableNonTerminals.count(sym) && !contains(state, {it.lhs, it.rhs, it.dot + 1})) {
                    state.push_back({it.lhs, it.rhs, it.dot + 1});
                    added = true;
                }
            }
        }
        // If any new item was added, we go for another iteration
        if (state.size() > originalSize) {
            added = true;
        }
    }
    return state;
}

vector<Item> GOTO(vector<Item> state, char symbol) {
    vector<Item> temp;
    for (auto it : state) {
        if (it.dot < it.rhs.length() && it.rhs[it.dot] == symbol) {
            temp.push_back({it.lhs, it.rhs, it.dot + 1});
        }
    }
    return closure(temp);
}

bool sameState(vector<Item> a, vector<Item> b) {
    if (a.size() != b.size()) return false;

    for (auto x : a) {
        bool found = false;
        for (auto y : b) {
            if (sameItem(x, y)) {
                found = true;
                break;
            }
        }
        if (!found) return false;
    }
    return true;
}

// Modify the code to detect nullable non-terminals and ε productions
void findNullableNonTerminals() {
    bool changed = true;
    while (changed) {
        changed = false;
        for (auto g : grammar) {
            if (g.rhs == "ε" && !nullableNonTerminals.count(g.lhs[0])) {
                nullableNonTerminals.insert(g.lhs[0]);
                changed = true;
            }
        }
        // Adding nullable non-terminals to the set
        for (auto g : grammar) {
            if (nullableNonTerminals.count(g.lhs[0]) && g.rhs != "ε") {
                bool allNullable = true;
                for (int i = 0; i < g.rhs.length(); i++) {
                    if (nullableNonTerminals.count(g.rhs[i]) == 0) {
                        allNullable = false;
                        break;
                    }
                }
                if (allNullable && !nullableNonTerminals.count(g.lhs[0])) {
                    nullableNonTerminals.insert(g.lhs[0]);
                    changed = true;
                }
            }
        }
    }
}

int main() {
    int n;
    // cout << "Enter number of productions: ";
    cin >> n;

    // cout << "Enter productions:\n";
    for (int i = 0; i < n; i++) {
        string p;
        cin >> p;
        grammar.push_back({string(1, p[0]), p.substr(3), 0});
    }

    // Find nullable non-terminals (those that can derive ε)
    findNullableNonTerminals();

    // Augmented grammar (Add a new start symbol)
    grammar.insert(grammar.begin(),
        {grammar[0].lhs + "'", grammar[0].lhs, 0});

    vector<Item> start;
    start.push_back(grammar[0]);
    states.push_back(closure(start));

    for (int i = 0; i < states.size(); i++) {
        for (auto it : states[i]) {
            if (it.dot < it.rhs.length()) {
                char sym = it.rhs[it.dot];
                vector<Item> next = GOTO(states[i], sym);

                bool exists = false;
                for (auto s : states) {
                    if (sameState(s, next)) {
                        exists = true;
                        break;
                    }
                }
                if (!exists && !next.empty())
                    states.push_back(next);
            }
        }
    }

    // Print states
    cout << "\nLR(0) States:\n";
    for (int i = 0; i < states.size(); i++) {
        cout << "\nI" << i << ":\n";
        for (auto it : states[i]) {
            cout << it.lhs << " -> ";
            for (int j = 0; j < it.rhs.length(); j++) {
                if (j == it.dot) cout << ".";
                cout << it.rhs[j];
            }
            if (it.dot == it.rhs.length()) cout << ".";
            cout << endl;
        }
    }
    return 0;
}


/*
Enter number of productions: 4
Enter productions:
S' -> S
S -> A
S -> ε
A -> aA
A -> b


*/
