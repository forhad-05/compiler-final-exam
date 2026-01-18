
//ll(1)
#include<bits/stdc++.h>
using namespace std;

// Helper function to display sets
void displaySet(const set<string>& s) {
    for (const string& str : s) {
        cout << str << " ";
    }
    cout << endl;
}

// Structure to hold production rules
struct Production {
    string left;
    vector<string> right;
};

// Class to represent the LL(1) Parser
class LL1Parser {
public:
    map<string, vector<Production>> grammar;  // Stores the grammar
    map<string, map<string, vector<string>>> parsingTable;  // LL(1) parsing table
    set<string> terminals;  // Set of terminal symbols
    set<string> nonTerminals;  // Set of non-terminal symbols
    map<string, set<string>> firstSets;  // FIRST sets
    map<string, set<string>> followSets;  // FOLLOW sets

    // Function to add production rules from input
    void addProduction(const string& rule) {
        stringstream ss(rule);
        string left, rightSide;
        getline(ss, left, '-');
        getline(ss, rightSide);

        // Trim spaces and handle epsilon
        left = trim(left);
        rightSide = trim(rightSide);

        nonTerminals.insert(left);
        stringstream rightStream(rightSide);
        string production;

        while (getline(rightStream, production, '|')) {
            Production prod;
            prod.left = left;
            stringstream prodStream(production);
            string symbol;
            while (getline(prodStream, symbol, ' ')) {
                symbol = trim(symbol);
                if (!symbol.empty()) {
                    prod.right.push_back(symbol);
                    if (symbol[0] >= 'a' && symbol[0] <= 'z') {
                        terminals.insert(symbol);
                    }
                }
            }
            grammar[left].push_back(prod);
        }
    }

    // Helper function to trim spaces
    string trim(const string& str) {
        size_t first = str.find_first_not_of(' ');
        if (first == string::npos) return "";
        size_t last = str.find_last_not_of(' ');
        return str.substr(first, last - first + 1);
    }

    // Function to calculate FIRST set
    void calculateFirstSet() {
        bool changed = true;
        while (changed) {
            changed = false;
            for (const auto& entry : grammar) {
                const string& nonTerminal = entry.first;
                for (const Production& prod : entry.second) {
                    for (const string& symbol : prod.right) {
                        if (terminals.count(symbol)) {
                            if (firstSets[nonTerminal].insert(symbol).second) {
                                changed = true;
                            }
                            break;
                        }
                        else {
                            set<string> tempFirstSet = firstSets[symbol];
                            tempFirstSet.erase("ε");
                            if (!tempFirstSet.empty()) {
                                for (const string& t : tempFirstSet) {
                                    if (firstSets[nonTerminal].insert(t).second) {
                                        changed = true;
                                    }
                                }
                            }
                            if (firstSets[symbol].count("ε") == 0) break;
                        }
                    }
                }
            }
        }
    }

    // Function to calculate FOLLOW set
    void calculateFollowSet() {
        followSets["E"].insert("$");  // Follow of start symbol is $
        bool changed = true;
        while (changed) {
            changed = false;
            for (const auto& entry : grammar) {
                const string& nonTerminal = entry.first;
                for (const Production& prod : entry.second) {
                    for (size_t i = 0; i < prod.right.size(); i++) {
                        const string& symbol = prod.right[i];
                        if (nonTerminals.count(symbol)) {
                            if (i + 1 < prod.right.size()) {
                                string nextSymbol = prod.right[i + 1];
                                if (terminals.count(nextSymbol)) {
                                    if (followSets[symbol].insert(nextSymbol).second) {
                                        changed = true;
                                    }
                                } else {
                                    for (const string& t : firstSets[nextSymbol]) {
                                        if (t != "ε" && followSets[symbol].insert(t).second) {
                                            changed = true;
                                        }
                                    }
                                    if (firstSets[nextSymbol].count("ε") > 0) {
                                        for (const string& t : followSets[prod.left]) {
                                            if (followSets[symbol].insert(t).second) {
                                                changed = true;
                                            }
                                        }
                                    }
                                }
                            } else {
                                for (const string& t : followSets[prod.left]) {
                                    if (followSets[symbol].insert(t).second) {
                                        changed = true;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    // Function to build the LL(1) parsing table
    void buildParsingTable() {
        for (const auto& entry : grammar) {
            const string& nonTerminal = entry.first;
            for (const Production& prod : entry.second) {
                set<string> firstSetForProd = getFirstSetForProduction(prod);
                for (const string& terminal : firstSetForProd) {
                    if (terminal != "ε") {
                        parsingTable[nonTerminal][terminal] = prod.right;
                    }
                }
                if (firstSetForProd.count("ε") > 0) {
                    for (const string& terminal : followSets[nonTerminal]) {
                        parsingTable[nonTerminal][terminal] = prod.right;
                    }
                }
            }
        }
    }

    // Function to get FIRST set of a production
    set<string> getFirstSetForProduction(const Production& prod) {
        set<string> result;
        for (const string& symbol : prod.right) {
            if (terminals.count(symbol)) {
                result.insert(symbol);
                break;
            } else {
                set<string> firstSetOfSymbol = firstSets[symbol];
                firstSetOfSymbol.erase("ε");
                result.insert(firstSetOfSymbol.begin(), firstSetOfSymbol.end());
                if (firstSets[symbol].count("ε") == 0) break;
            }
        }
        return result;
    }

    // Function to perform parsing on an input string
    void parse(const string& input) {
        stack<string> parseStack;
        parseStack.push("$");
        parseStack.push("E");

        queue<string> inputQueue;
        for (char ch : input) {
            inputQueue.push(string(1, ch));
        }
        inputQueue.push("$");

        while (!parseStack.empty()) {
            string top = parseStack.top();
            string currentInput = inputQueue.front();

            if (top == currentInput) {
                cout << "Matched: " << top << endl;
                parseStack.pop();
                inputQueue.pop();
            } else if (nonTerminals.count(top)) {
                if (parsingTable[top].count(currentInput)) {
                    cout << "Expanding: " << top << " -> ";
                    vector<string> production = parsingTable[top][currentInput];
                    parseStack.pop();
                    for (auto it = production.rbegin(); it != production.rend(); ++it) {
                        if (*it != "ε") parseStack.push(*it);
                    }
                    for (const string& symbol : production) {
                        cout << symbol << " ";
                    }
                    cout << endl;
                } else {
                    cout << "Error: No rule to apply for " << top << " with input " << currentInput << endl;
                    break;
                }
            } else {
                cout << "Error: Expected terminal " << top << ", found " << currentInput << endl;
                break;
            }
        }
    }
};

int main() {
    LL1Parser parser;

    // Input grammar (for testing purposes)
    string grammarLine;
    cout << "Enter grammar (type 'done' to stop):" << endl;

    while (true) {
        getline(cin, grammarLine);
        if (grammarLine == "done") break;  // End of input
        parser.addProduction(grammarLine);
    }

    // Build the LL(1) parsing table
    parser.buildParsingTable();

    // Test input string for parsing
    string input;
    cout << "\nEnter input string to parse (type '$' at the end): ";
    getline(cin, input);

    cout << "\nParsing input: " << input << endl;
    parser.parse(input);

    return 0;
}
