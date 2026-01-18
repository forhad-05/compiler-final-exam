#include <bits/stdc++.h>
using namespace std;

map<char, vector<string>> G;
map<char, set<char>> FIRST, FOLLOW;
set<char> NT;
char START;

bool isNT(char c) { return c >= 'A' && c <= 'Z'; }

// Compute FIRST set of a non-terminal
void findFirst(char A) {
    if (!FIRST[A].empty()) return; // already computed

    for (auto prod : G[A]) {
        if (prod == "#") { FIRST[A].insert('#'); continue; }

        for (char X : prod) {
            if (!isNT(X)) { FIRST[A].insert(X); break; } // terminal
            findFirst(X);
            bool eps = false;
            for (char c : FIRST[X]) {
                if (c == '#') eps = true;
                else FIRST[A].insert(c);
            }
            if (!eps) break;
        }
    }
}

// Compute FOLLOW set of all non-terminals
void findFollow() {
    FOLLOW[START].insert('$');
    bool changed;
    do {
        changed = false;
        for (auto &[LHS, prods] : G) {
            for (auto prod : prods) {
                for (int i = 0; i < prod.size(); i++) {
                    if (!isNT(prod[i])) continue;
                    char A = prod[i];
                    set<char> temp;

                    if (i + 1 < prod.size()) {
                        char next = prod[i + 1];
                        if (!isNT(next)) temp.insert(next);
                        else {
                            for (char c : FIRST[next])
                                if (c != '#') temp.insert(c);
                            if (FIRST[next].count('#'))
                                temp.insert(FOLLOW[LHS].begin(), FOLLOW[LHS].end());
                        }
                    } else {
                        temp.insert(FOLLOW[LHS].begin(), FOLLOW[LHS].end());
                    }

                    int oldSize = FOLLOW[A].size();
                    FOLLOW[A].insert(temp.begin(), temp.end());
                    if (FOLLOW[A].size() > oldSize) changed = true;
                }
            }
        }
    } while (changed);
}

int main() {
    int n;
    cout << "Enter number of productions: ";
    cin >> n;

    cout << "Enter productions (A->BC|d , # for epsilon):\n";
    for (int i = 0; i < n; i++) {
        string s; cin >> s;
        char lhs = s[0];
        NT.insert(lhs);
        if (i == 0) START = lhs;

        string rhs = s.substr(3), temp = "";
        for (char c : rhs) {
            if (c == '|') { G[lhs].push_back(temp); temp.clear(); }
            else temp += c;
        }
        G[lhs].push_back(temp);
    }

    // Compute FIRST sets
    for (char nt : NT) findFirst(nt);

    // Compute FOLLOW sets
    findFollow();

    cout << "\nFIRST Sets:\n";
    for (char nt : NT) {
        cout << "FIRST(" << nt << ") = { ";
        for (char c : FIRST[nt]) cout << c << " ";
        cout << "}\n";
    }

    cout << "\nFOLLOW Sets:\n";
    for (char nt : NT) {
        cout << "FOLLOW(" << nt << ") = { ";
        for (char c : FOLLOW[nt]) cout << c << " ";
        cout << "}\n";
    }

    return 0;
}

