
#include <iostream>
#include <vector>
#include <string>
using namespace std;

struct Triple {
    string op, arg1, arg2;
};

int main() {
    vector<Triple> triples;
    int n;
    cout << "Enter number of triples: ";
    cin >> n;
    cin.ignore(); // clear newline

    for(int i = 0; i < n; i++) {
        Triple t;
        cout << "\nTriple " << i << ":\n";
        cout << "Operator: ";
        getline(cin, t.op);
        cout << "Argument 1: ";
        getline(cin, t.arg1);
        cout << "Argument 2: ";
        getline(cin, t.arg2);
        triples.push_back(t);
    }

    cout << "\nGenerated Triples:\n";
    cout << "Index\tOp\tArg1\tArg2\n";
    cout << "--------------------------\n";
    for(int i = 0; i < triples.size(); i++)
        cout << i << "\t" << triples[i].op << "\t" << triples[i].arg1 << "\t" << triples[i].arg2 << endl;

    return 0;
}
