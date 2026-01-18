#include <iostream>
#include <string>
#include <stack>
#include <map>
using namespace std;

// Node structure for DAG
struct Node {
    string op;
    string left, right;
    string name;
};

// Function to return operator precedence
int prec(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    return 0;
}

// Generate DAG-based Three-Address Code
void generateDAG(string expr) {
    stack<char> ops;
    stack<string> vals;
    map<string, string> dag;   // map to store sub-expression -> temp variable
    int temp = 1;

    for (char ch : expr) {
        if (ch == ' ') continue;
        if (isalnum(ch)) vals.push(string(1, ch));
        else { // operator
            while (!ops.empty() && prec(ch) <= prec(ops.top())) {
                char op = ops.top(); ops.pop();
                string b = vals.top(); vals.pop();
                string a = vals.top(); vals.pop();
                string key = a + op + b; // unique key for sub-expression

                if (!dag.count(key)) {
                    string t = "t" + to_string(temp++);
                    cout << t << " = " << a << " " << op << " " << b << endl;
                    dag[key] = t;
                }
                vals.push(dag[key]);
            }
            ops.push(ch);
        }
    }

    while (!ops.empty()) {
        char op = ops.top(); ops.pop();
        string b = vals.top(); vals.pop();
        string a = vals.top(); vals.pop();
        string key = a + op + b;

        if (!dag.count(key)) {
            string t = "t" + to_string(temp++);
            cout << t << " = " << a << " " << op << " " << b << endl;
            dag[key] = t;
        }
        vals.push(dag[key]);
    }
}

int main() {
    string expr;
    cout << "Enter arithmetic expression: ";
    getline(cin, expr);

    cout << "\nDAG-based Three-Address Code:\n";
    generateDAG(expr);

    return 0;
}
