// Найдите все вхождения шаблона в строку.
// Найдите все вхождения шаблона в строку. Длина шаблона – p, длина строки – n.
// Время O(n + p), доп. память – O(p).
// p <= 30000, n <= 300000.

#include <iostream>
#include <vector>
using namespace std;

void SubstringSearch(const string &s, int pattern_length, vector <int> &answer) {
    vector <int> zf(pattern_length, 0);
    int left = 0, right = 0;

    for (int i = 1; i < s.length(); i++) {
        int current_zf = 0;
        if (i - left <= pattern_length) {
            current_zf = max(0, min(right - i, zf[i - left]));
        } else {
            current_zf = max(0, right - i);
        }

        while (i + current_zf < s.length() && s[current_zf] == s[i + current_zf]) {
            current_zf++;
        }

        if (i + current_zf - 1 > right) {
            left = i;
            right = i + current_zf - 1;
        }

        if (i < pattern_length) {
            zf[i] = current_zf;
        }

        if (current_zf == pattern_length) {
            answer.push_back(i - pattern_length - 1);
        }
    }
}

int main() {
    string pattern, text;
    cin >> pattern >> text;

    vector <int> answer;
    SubstringSearch(pattern + '#' + text, pattern.length(), answer);

    for (auto a: answer) {
        cout << a << " ";
    }
}
