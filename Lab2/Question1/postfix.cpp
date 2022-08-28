#include <stdio.h>
#include <string.h>
#include <stack>
#include <cctype>
#include <string>
using namespace std;

bool isOperator(char c) {
    return c == '+' ||
        c == '-' ||
        c == '*' ||
        c == '/';
}

string solvePostfix(char *input) {
    stack<float> s;
    int len = strlen(input);

    for (int i = 0; i < len; ++i) {
        char c = input[i];

        if (isdigit(c)) {
            s.push(c - '0');
        } else if (isspace(c)) {
            continue;
        } else if (isOperator(c)) {
            if (s.size() < 2) {
                return "Invalid";
            }

            float val1 = s.top(); s.pop();
            float val2 = s.top(); s.pop();

            switch (c) {
                case '+':
                    s.push(val2 + val1);
                    break;
                case '-':
                    s.push(val2 - val1);
                    break;
                case '*':
                    s.push(val2 * val1);
                    break;
                case '/':
                    if (val2 == 0) {
                        return "Zero division";
                    }

                    s.push(val2 / val1);
                    break;
            }
        } else {
            return "Invalid";
        }
    }

    if (s.empty()) {
        return "Invalid";
    }

    return to_string(s.top());
}

