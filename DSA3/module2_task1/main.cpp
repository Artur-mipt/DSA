#include <iostream>
#include <vector>
using namespace std;

const int AlphabetSize = 256;

class SuffArray
{
public:
    void SuffArrBuilding(const string &s);
    void LCPBuilding(const string &s);

    int Size() const;  // размер суфф. массива
    int GetLCP(int i) const;  // i-ый элемент в векторе lcp
    int operator[](int i) const;  // suff_arr[i]
private:
    void SecondStageBuilding(vector <int>& perm,
                             vector <int>& cnt,
                             vector <int>& eq_classes,
                             const string& s,
                             int len_s,
                             int classes);

    vector <int> suff_arr;
    vector <int> lcp;
};

int SuffArray::Size() const {
    return suff_arr.size();
}

int SuffArray::GetLCP(int i) const {
    return lcp[i];
}

int SuffArray::operator[](int i) const {
    return suff_arr[i];
}

void SuffArray::SuffArrBuilding(const string &s) {
    int len_s = s.length();
    suff_arr.assign(len_s, 0);

    // первая фаза
    vector <int> perm(len_s); // перестановка индексов циклической подстроки
    vector <int> cnt(AlphabetSize); // массив для сортировки подсчетом
    vector <int> eq_classes(len_s); // классы эквивалентности

    // сортировка для подстрок длины 1
    for (int i = 0; i < s.length(); i++) {
        cnt[s[i]]++;
    }
    for (int i = 1; i < AlphabetSize; i++) {
        cnt[i] += cnt[i - 1];
    }
    for (int i = 0; i < s.length(); i++) {
        perm[ --cnt[s[i]] ] = i;
    }
    eq_classes[perm[0]] = 0;
    int classes = 1;
    for (int i = 1; i < s.length(); i++) {
        if (s[perm[i]] != s[perm[i - 1]]) {
            ++classes;
        }
        eq_classes[perm[i]] = classes - 1;
    }

    SecondStageBuilding(perm, cnt, eq_classes, s, len_s, classes);
}

void SuffArray::SecondStageBuilding(vector <int>& perm,
                                    vector <int>& cnt,
                                    vector <int>& eq_classes,
                                    const string& s,
                                    int len_s,
                                    int classes) {
    // дальнейшие сортировки, учитывая предыдущие
    vector <int> perm_tmp(s.length());  // перестановка в порядке сортировки по 2-ым эл-ам пар
    vector <int> eq_classes_tmp(s.length());  // новые номера классов эквивалентности

    for (int i = 0; (1 << i) < len_s; i++) {
        for (int j = 0; j < len_s; j++) {
            perm_tmp[j] = perm[j] - (1 << i);
            if (perm_tmp[j] < 0) {
                perm_tmp[j] += len_s;
            }
        }

        cnt.assign(classes, 0);
        for (int j = 0; j < len_s; j++) {
            cnt[ eq_classes[ perm_tmp[j] ] ]++;
        }

        for (int j = 1; j < classes; j++) {
            cnt[j] += cnt[j - 1];
        }

        for (int j = len_s - 1; j >= 0; j--) {
            perm[ --cnt[ eq_classes[perm_tmp[j]] ] ] = perm_tmp[j];
        }

        eq_classes_tmp[perm[0]] = 0;
        classes = 1;

        for (int j = 1; j < len_s; j++) {
            int mid1 = (perm[j] + (1 << i)) % len_s;
            int mid2 = (perm[j - 1] + (1 << i)) % len_s;
            if (eq_classes[perm[j]] != eq_classes[perm[j - 1]]
                || eq_classes[mid1] != eq_classes[mid2]) {
                classes++;
            }

            eq_classes_tmp[perm[j]] = classes - 1;
        }

        copy(eq_classes_tmp.begin(), eq_classes_tmp.begin() + len_s, eq_classes.begin());
    }

    copy(perm.begin(), perm.begin() + len_s, suff_arr.begin());
}

void SuffArray::LCPBuilding(const string &s) {
    int ls = s.length();

    lcp.assign(ls, 0); // массив наименьших общих префиксов
    vector <int> pos(ls); // обратный массиву suff_arr

    for (int i = 0; i < ls; i++) {
        pos[suff_arr[i]] = i;
    }

    int k = 0;

    for (int i = 0; i < ls; i++) {
        if (k > 0) {
            k--;
        }
        if (pos[i] == ls - 1) {
            lcp[ls - 1] = -1;
            k = 0;
        } else {
            int j = suff_arr[pos[i] + 1];
            while (max(i + k, j + k) < ls && s[i + k] == s[j + k]) {
                k++;
            }

            lcp[pos[i]] = k;
        }
    }
}

int GetAnswer(const SuffArray& suff_arr) {
    int len_s = suff_arr.Size() - 1;

    int answer = 0;

    for (int i = 0; i < len_s; i++) {
        answer += len_s - suff_arr[i] - suff_arr.GetLCP(i);
    }
    answer += len_s - suff_arr[len_s];

    return answer;
}

int main() {
    string s;
    cin >> s;
    s += '$';

    SuffArray suff_arr;
    suff_arr.SuffArrBuilding(s);
    suff_arr.LCPBuilding(s);
    cout << GetAnswer(suff_arr);
}