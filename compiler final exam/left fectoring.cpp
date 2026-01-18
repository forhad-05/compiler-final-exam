
#include <iostream>
#include <vector>
#include <sstream>
using namespace std;

int main() {
    string NT, input, prod;
    cout << "Enter non-terminal: ";
    cin >> NT;
    cin.ignore();

    cout << "Enter productions (use |): ";
    getline(cin, input);

    vector<string> P;
    stringstream ss(input);

    // Split productions
    while (getline(ss, prod, '|')) {
        while (!prod.empty() && prod[0] == ' ')
            prod.erase(0, 1);
        P.push_back(prod);
    }

    // Find common prefix
    string prefix = P[0];
    for (int i = 1; i < P.size(); i++) {
        int j = 0;
        while (j < prefix.size() && j < P[i].size() && prefix[j] == P[i][j])
            j++;
        prefix = prefix.substr(0, j);
    }

    // No left factoring
    if (prefix.empty()) {
        cout << "No left factoring needed\n";
        return 0;
    }

    // Output
    cout << "\nAfter left factoring:\n";
    cout << NT << " -> " << prefix << NT << "'\n";
    cout << NT << "' -> ";

    for (int i = 0; i < P.size(); i++) {
        string rem = P[i].substr(prefix.size());
        cout << (rem.empty() ? "ε" : rem);
        if (i != P.size() - 1) cout << " | ";
    }

    return 0;
}
