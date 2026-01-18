
//First and Follow Set Calculation for Context-Free Grammars
#include <bits/stdc++.h>
using namespace std;
map<char, vector<string>> G;
map<char, set<char>> FIRST, FOLLOW;
set<char> NT;
char START;
bool isNT(char c) {
    return c >= 'A' && c <= 'Z';
}
void findFirst(char A) {
    if (!FIRST[A].empty()) return;

    for (string prod : G[A]) {
        if (prod == "#") {
            FIRST[A].insert('#');
            continue;
        }

        for (char X : prod) {
            if (!isNT(X)) {
                FIRST[A].insert(X);
                break;
            }

            findFirst(X);
            bool hasEps = false;

            for (char c : FIRST[X]) {
                if (c == '#') hasEps = true;
                else FIRST[A].insert(c);
            }

            if (!hasEps) break;
        }
    }
}
void findFollow(char A) {
    if (A == START)
        FOLLOW[A].insert('$');
    for (auto &[LHS, prods] : G) {
        for (string p : prods) {
            for (int i = 0; i < p.size(); i++) {
                if (p[i] != A) continue;
                if (i + 1 < p.size()) {
                    char next = p[i + 1];
                    if (!isNT(next)) {
                        FOLLOW[A].insert(next);
                    } else {
                        bool eps = false;
                        for (char c : FIRST[next]) {
                            if (c == '#') eps = true;
                            else FOLLOW[A].insert(c);
                        }
                        if (eps)
                            FOLLOW[A].insert(FOLLOW[LHS].begin(), FOLLOW[LHS].end());
                    }
                } else {
                    FOLLOW[A].insert(FOLLOW[LHS].begin(), FOLLOW[LHS].end());
                }
            }
        }
    }
}

int main() {
    int n;
    cout << "Enter number of productions: ";
    cin >> n;

    cout << "Enter productions (A->BC|d , # for epsilon):\n";
    for (int i = 0; i < n; i++) {
        string s;
        cin >> s;

        char lhs = s[0];
        NT.insert(lhs);
        if (i == 0) START = lhs;

        string rhs = s.substr(3), temp = "";
        for (char c : rhs) {
            if (c == '|') {
                G[lhs].push_back(temp);
                temp.clear();
            } else temp += c;
        }
        G[lhs].push_back(temp);
    }

    for (char nt : NT) findFirst(nt);
    for (int i = 0; i < 2; i++)
        for (char nt : NT) findFollow(nt);

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
// 3
// E->TR
// R->+TR|#
// T->i
