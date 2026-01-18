
#include <iostream>
#include <string>
#include <map>
#include <cctype>
#include <sstream>
using namespace std;

map<string, string> symbolTable;  // Stores variable types

// Function to check if string is a number
bool isNumber(const string& s) {
    bool dot = false;
    for (char c : s) {
        if (c == '.') {
            if (dot) return false; // multiple dots not allowed
            dot = true;
        }
        else if (!isdigit(c)) return false;
    }
    return !s.empty();
}

// Function to check if value matches variable type
bool typeCheck(const string& var, const string& val) {
    if (symbolTable.find(var) == symbolTable.end()) return false; // not declared
    string type = symbolTable[var];

    if (type == "int") return isNumber(val) && val.find('.') == string::npos;
    if (type == "float" || type == "double") return isNumber(val);
    if (type == "string") return true;

    return false;
}

int main() {
    cout << "Type Checker (declare <type> <var>, assign <var> <value>, exit)\n";
    string input;

    while (true) {
        cout << "> ";
        getline(cin, input);
        if (input == "exit") break;

        string cmd, var, type, val;
        istringstream iss(input);
        iss >> cmd;

        if (cmd == "declare") {
            iss >> type >> var;
            symbolTable[var] = type;
            cout << "Declared " << var << " as " << type << endl;
        }
        else if (cmd == "assign") {
            iss >> var >> val;
            if (typeCheck(var, val))
                cout << "Type check passed: " << var << " = " << val << endl;
            else
                cout << "Type check failed: " << var << " = " << val << endl;
        }
        else cout << "Unknown command" << endl;
    }

    return 0;
}
