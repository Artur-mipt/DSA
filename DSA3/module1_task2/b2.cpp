// Найти лексикографически-минимальную строку, построенную по z-функции, в алфавите a-z.

#include <iostream>
#include <vector>
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

void prefix_from_z(const vector <int> &zf, vector <int> &pr) {
    pr.assign(zf.size(), 0);

    for (int i = 1; i < zf.size(); i++) {
        for (int j = zf[i] - 1; j >= 0; j--) {
            if (pr[i + j] > 0) {
                break;
            }
            else
                pr[i + j] = j + 1;
        }
    }
}

int main() {
    vector <int> zf;

    int a;
    while (cin >> a) {
        zf.push_back(a);
    }

    vector <int> pr;
    prefix_from_z(zf, pr);
    cout << string_from_prefix(pr);
}