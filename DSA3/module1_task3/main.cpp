// Шаблон поиска задан строкой длины m, в которой кроме обычных символов могут встречаться символы “?”.
// Найти позиции всех вхождений шаблона в тексте длины n.
// Каждое вхождение шаблона предполагает, что все обычные символы совпадают с соответствующими из текста, а вместо
// символа “?” в тексте встречается произвольный символ.
// Время работы - O(n + m + Z), где Z - общее -число вхождений подстрок шаблона “между вопросиками” в исходном тексте.
// (Ахо-Корасик)
// m ≤ 5000, n ≤ 2000000.


#include <iostream>
#include <vector>
using namespace std;

const int AlphabetSize = 26;

struct Node
{
    // конструктор для создания корня
    Node(bool is_leaf, int link, int parent): is_leaf(is_leaf), link(link), parent(parent),
                                              next(vector <int>(AlphabetSize, -1)),
                                              go(vector <int> (AlphabetSize, -1)) { };

    // конструктор для добавления новой вершины в автомат
    Node(bool is_leaf, int link, int parent, int parent_ch): is_leaf(is_leaf), link(link), parent(parent),
                                                             parent_ch(parent_ch),
                                                             next(vector <int>(AlphabetSize, -1)),
                                                             go(vector <int> (AlphabetSize, -1)) { };

    vector <int> next; // next[i] - вершина в которую переходим по букве i в обычном Боре
    bool is_leaf; // является ли вершина терминальной
    int parent; // номер вершины-родителя
    int parent_ch; // символ, по которому попадаем из родителя в нашу вершину
    int link; // суффиксная ссылка
    vector <int> go; // переходы по каждому из символов (в модифицированном Боре)
    vector <int> terminal_parents; // все терминальные вершины, в которые можно попасть по суфф. ссылкам
    vector <int> pattern_numbers; // номера шаблонов, которым соответствует вершина
};

// автомат, построенный по шаблонам
class Automate
{
public:
    Automate();
    void AddString(const string &s, int pattern_number);
    int Go(int vertex, int ch);
    int SuffLink(int vertex);
    vector <int> TerminalParents(int vertex);
    vector <int> PatternNumbers(int vertex);
private:
    vector <Node> trie;
};

// создаем корень автомата
Automate::Automate() {
    Node root(false, 0, -1);

    trie.push_back(root);
}

// добавление строки в автомат
void Automate::AddString(const string &s, int pattern_number) {
    // начинаем с корня
    int current_vertex = 0;

    for (auto ch: s) {
        int c = (int)(ch - 'a'); // символ, по которому хотим сделать переход
        // если нет ребра, то создаем новое
        if (trie[current_vertex].next[c] == -1) {
            size_t bor_size = trie.size();
            Node new_vertex(false, -1, current_vertex, c);

            trie.push_back(new_vertex);
            trie[current_vertex].next[c] = trie.size() - 1;
        }

        current_vertex = trie[current_vertex].next[c];
    }

    trie[current_vertex].is_leaf = true;
    trie[current_vertex].pattern_numbers.push_back(pattern_number);
}

// подсчёт суффиксной ссылки для вершины vertex
int Automate::SuffLink(int vertex) {
    if (trie[vertex].link == -1) {
        if (vertex == 0 || trie[vertex].parent == 0) {
            // для корня и его сыновей суфф. ссылка = 0
            trie[vertex].link = 0;
        } else {
            // иначе берем суфф. ссылку от предка и от нее переходим по символу parent_ch
            trie[vertex].link = Go(SuffLink(trie[vertex].parent), trie[vertex].parent_ch);
        }
    }

    return trie[vertex].link;
}

// подсчет функции перехода для вершины vertex и символа ch
int Automate::Go(int vertex, int ch) {
    if (trie[vertex].go[ch] == -1) {
        if (trie[vertex].next[ch] != -1) {
            // если есть такой переход, то сразу берём его
            trie[vertex].go[ch] = trie[vertex].next[ch];
        } else {
            // если нет перехода по букве
            if (vertex == 0) {
                // корень отправляем в корень
                trie[vertex].go[ch] = 0;
            } else {
                // иначе добавляем переход по суффиксной ссылке
                trie[vertex].go[ch] = Go(SuffLink(vertex), ch);
            }
        }
    }

    return trie[vertex].go[ch];
}

// нахождение терминальных вершин, в которые можно попасть по суффиксным ссылкам
vector <int> Automate::TerminalParents(int vertex) {
    if (trie[vertex].terminal_parents.size() > 0) {
        return trie[vertex].terminal_parents;
    }

    int current_vertex = vertex;
    while (current_vertex != 0) {
        if (trie[current_vertex].is_leaf) {
            trie[vertex].terminal_parents.push_back(current_vertex);
        }

        current_vertex = SuffLink(current_vertex);
    }

    return trie[vertex].terminal_parents;
}

// по вершине определить, какому по счёту шаблону она соответствует
vector <int> Automate::PatternNumbers(int vertex) {
    if (trie[vertex].is_leaf) {
        return trie[vertex].pattern_numbers;
    } else {
        return vector <int>(0);
    }
}

// парсинг строки с '?' и добавление новых шаблонов в бор
void BuildTrie(Automate* automate, string s, vector <int> &l, vector <int> &d) {
    string current_str = "";
    bool flag = true; // последний прочитанный символ - '?' ?

    for (size_t i = 0; i < s.length(); i++) {
        char ch = s[i];
        if (ch != '?') {
            current_str += ch;
            if (flag) {
                l.push_back(i);
            }
            flag = false;
        } else {
            if (current_str != "") {
                automate->AddString(current_str, l.size() - 1);
                d.push_back(current_str.length());
                current_str = "";
            }
            flag = true;
        }
    }

    automate->AddString(current_str, l.size() - 1);
    d.push_back(current_str.length());
}

// нахождение всех вхождений шаблонов в тексте и заполнение массива кол-ва вхождений
void FindSubstrings(Automate* automate, const string &text, const vector <int> &l,
                    const vector <int> &d, vector <int> &c) {
    int current_vertex = 0;

    for (int i = 0; i < text.length(); i++) {
        int ch = text[i] - 'a';
        current_vertex = automate->Go(current_vertex, ch);
        vector <int> term = automate->TerminalParents(current_vertex); // все достижимые терминальные вершины
        // для каждой вершины из ans найдем, какому шаблону она соответствует, а затем по номеру шаблона
        // определим, на какой позиции он находится в исходном шаблоне с "?"
        for (auto a: term) {
            vector <int> pattern_numbers = automate->PatternNumbers(a);
            for (auto pattern_number: pattern_numbers) {
                if (i - d[pattern_number] - l[pattern_number] + 1 >= 0) {
                    c[i - d[pattern_number] - l[pattern_number] + 1]++;
                }
            }
        }
    }
}

int main() {
    auto automate = new Automate;

    // считывание шаблона
    string s;
    cin >> s;

    // построение бора
    vector <int> l(0); // вхождения шаблонов без '?' в исходном шаблоне
    vector <int> d(0); // длины шаблонов без '?'
    BuildTrie(automate, s, l, d);

    // считывание текста
    string text;
    cin >> text;

    // c[i] - кол-во "подозрений", что вхождение на символе i
    vector <int> c(text.length(), 0);

    // поиск вхождений шаблона в тексте
    FindSubstrings(automate, text, l, d, c);

    // выводим индексы i: c[i] == кол-во шаблонов
    for (int i = 0; i < c.size(); i++) {
        if (c[i] == l.size() && i + s.length() <= text.length()) {
            cout << i << " ";
        }
    }

    return 0;
}