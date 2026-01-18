
#include <iostream>
#include <vector>
#include <sstream>
using namespace std;

int main() {
    string nonTerminal;
    cout << "Enter non-terminal: ";
    cin >> nonTerminal;
    cin.ignore();

    string input;
    cout << "Enter productions (use |): ";
    getline(cin, input);

    vector<string> alpha, beta;
    stringstream ss(input);
    string prod;

    while (getline(ss, prod, '|')) {
        while (!prod.empty() && prod[0] == ' ')
            prod.erase(0, 1);

        if (prod.substr(0, nonTerminal.size()) == nonTerminal) {
            alpha.push_back(prod.substr(nonTerminal.size()));
        } else {
            beta.push_back(prod);
        }
    }

    if (alpha.empty()) {
        cout << "No left recursion found.\n";
        return 0;
    }

    cout << "\nAfter eliminating left recursion:\n";

    cout << nonTerminal << " -> ";
    for (int i = 0; i < beta.size(); i++) {
        cout << beta[i] << nonTerminal << "'";
        if (i != beta.size() - 1) cout << " | ";
    }
    cout << endl;

    cout << nonTerminal << "' -> ";
    for (int i = 0; i < alpha.size(); i++) {
        cout << alpha[i] << nonTerminal << "'";
        if (i != alpha.size() - 1) cout << " | ";
    }
    cout << " | ε" << endl;

    return 0;
}
