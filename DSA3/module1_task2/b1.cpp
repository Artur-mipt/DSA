// Найти лексикографически-минимальную строку, построенную по префикс-функции, в алфавите a-z.

#include <iostream>
#include <vector>
#include <unordered_set>
#include <set>
using namespace std;

string string_from_prefix(const vector <int> &pr) {
    string answer = "a";

    for (int i = 1; i < pr.size(); i++) {
        if (pr[i] != 0) {
            answer += answer[pr[i] - 1];
        } else {
            int tmp = pr[i - 1];
            set <char> s;

            while (tmp > 0) {
                s.insert(answer[tmp]);
                tmp = pr[tmp - 1];
            }

            for (char ch = 'b'; ch <= 'z'; ch++) {
                auto it = s.find(ch);
                if (it == s.end()) {
                    answer += ch;
                    s.insert(ch);
                    break;
                }
            }
        }
    }

    return answer;
}

int main() {
    vector <int> pr;

    int a;
    while (cin >> a) {
        pr.push_back(a);
    }

    cout << string_from_prefix(pr);
}