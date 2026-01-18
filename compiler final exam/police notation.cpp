
#include <iostream>
#include <stack>
#include <algorithm>
using namespace std;

// Function to return precedence of operators
int precedence(char op) {
    if (op == '+' || op == '-')
        return 1;
    if (op == '*' || op == '/')
        return 2;
    if (op == '^')
        return 3;
    return 0;
}

// Function to convert infix to prefix
string infixToPrefix(string exp) {
    // Step 1: Reverse the infix expression
    reverse(exp.begin(), exp.end());

    // Step 2: Replace '(' with ')' and vice versa
    for (int i = 0; i < exp.length(); i++) {
        if (exp[i] == '(')
            exp[i] = ')';
        else if (exp[i] == ')')
            exp[i] = '(';
    }

    stack<char> st;
    string prefix = "";

    // Step 3: Convert reversed infix to postfix
    for (int i = 0; i < exp.length(); i++) {
        char ch = exp[i];

        // If operand, add to prefix
        if (isalnum(ch)) {
            prefix += ch;
        }
        // If opening bracket
        else if (ch == '(') {
            st.push(ch);
        }
        // If closing bracket
        else if (ch == ')') {
            while (!st.empty() && st.top() != '(') {
                prefix += st.top();
                st.pop();
            }
            st.pop();
        }
        // If operator
        else {
            while (!st.empty() && precedence(ch) < precedence(st.top())) {
                prefix += st.top();
                st.pop();
            }
            st.push(ch);
        }
    }

    // Pop remaining operators
    while (!st.empty()) {
        prefix += st.top();
        st.pop();
    }

    // Step 4: Reverse postfix to get prefix
    reverse(prefix.begin(), prefix.end());
    return prefix;
}

int main() {
    string infix;
    cout << "Enter infix expression: ";
    cin >> infix;

    cout << "Prefix expression: " << infixToPrefix(infix) << endl;
    return 0;
}
