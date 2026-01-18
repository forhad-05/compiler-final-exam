#include <iostream>
#include <string>
#include <vector>
using namespace std;

struct Quadruple {
    string op;
    string arg1;
    string arg2;
    string res;
};

int main()
{
    vector<Quadruple> quads;
    int num;

    cout << "Enter the number of quadruples: ";
    cin >> num;
    cin.ignore(); // clear newline after number input

    for(int i = 0; i < num; i++)
    {
        Quadruple q;
        cout << "\nQuadruple " << i + 1 << ":\n";

        cout << "Enter operator: ";
        getline(cin, q.op);

        cout << "Enter argument 1: ";
        getline(cin, q.arg1);

        cout << "Enter argument 2: ";
        getline(cin, q.arg2);

        cout << "Enter result: ";
        getline(cin, q.res);

        quads.push_back(q);
    }

    // Display the quadruples
    cout << "\nGenerated Quadruples (3-Address Code):\n";
    cout << "--------------------------------------------\n";
    cout << "Index\tOp\tArg1\tArg2\tResult\n";
    cout << "--------------------------------------------\n";

    for(int i = 0; i < quads.size(); i++) {
        cout << i << "\t"
             << quads[i].op << "\t"
             << quads[i].arg1 << "\t"
             << quads[i].arg2 << "\t"
             << quads[i].res << endl;
    }

    return 0;
}
