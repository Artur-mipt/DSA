#include <iostream>
#include <vector>
using namespace std;

const int AlphabetSize = 28;

struct Vertex {
    int l;
    int r;  // индекcы l, r ребра в родителя
    int parent;  // родитель
    int suff;  // суфф. ссылка

    Vertex(int l, int r, int parent, int suff): l(l), r(r), parent(parent), suff(suff) {}
};

class DoubleSuffTree {
public:
    void AddFirstString(const string& str);  // добавление первой строки
    void AddSecondString(const string& str);  // добавление второй строки + инициализация корня
    void UkkonenBuilding(); // построение дерева по строке
    void DescribeTree(); // вывести описание дерева

private:
    void Split(int ch);  // разделение ребра на два при добавлении символа ch и добавление суфф. ссылки
    void UkkonenAdding(int ch); // добавление символа в дерево
    int GetCode(char ch); // получить код символа ch
    void DFS(int v, int& counter); // запустить обход в глубину из вершины
    void DescribeVertex(int v); // вывести описание вершины v

    vector <Vertex> vertexes;  // вершины дерева

    vector <vector <int>> next; // next[v][c] - переход из вершины v по символу c

    int cur_v; // вершина текущего суффикса (если мы на ребре, то нижняя вершина)
    int cur_s; // текущий символ в строке
    int cur_pos; // позиция в строке, соотв. месту на ребре (от edges[cur_v].first до second)
    int vert_number; // кол-во вершин

    string s;
    int l1;  // длина первой строки

    // sorted_vertexes[i] - какой номер имеет вершина i при dfs обходе в лексикогр. порядке
    vector <int> sorted_vertexes;
};

void DoubleSuffTree::AddFirstString(const string& str) {
    s = str;
    l1 = str.length();
}

void DoubleSuffTree::AddSecondString(const string &str) {
    s = s + str;

    next.assign(s.length() * 2, vector <int>(AlphabetSize, -1));
    next[1].assign(s.length() * 2, 0);

    vertexes.assign(s.length() * 2, Vertex(0, s.length() - 1, 0, 0));

    sorted_vertexes.assign(s.length() * 2, -1);

    vertexes[0] = Vertex(-1, -1, 0, 1);
    vertexes[1] = Vertex(-1, -1, 0, 0);
}

void DoubleSuffTree::UkkonenBuilding() {
    vert_number = 2;
    cur_pos = 0;
    cur_v = 0;
    cur_s = 0;

    for (auto ch: s) {
        if (ch == '#') {
            UkkonenAdding(0);
            cur_s++;
            continue;
        }
        if (ch == '$') {
            UkkonenAdding(1);
            cur_s++;
            continue;
        }
        UkkonenAdding(int(ch - 'a') + 2);
        cur_s++;
    }
}

void DoubleSuffTree::Split(int ch) {
    // добавляем вершину n и пускаем ребро в cur_v
    vertexes[vert_number] = Vertex(vertexes[cur_v].l, cur_pos - 1, vertexes[cur_v].parent, 0);
    int tmp_ch = GetCode(s[cur_pos]);
    next[vert_number][tmp_ch] = cur_v;

    next[vert_number][ch] = vert_number + 1;  // добавляем вершину n + 1, в которую приходим из n
    vertexes[vert_number + 1].l = cur_s;
    vertexes[vert_number + 1].parent = vert_number;

    // обновляем значения для cur_v
    vertexes[cur_v].l = cur_pos;
    vertexes[cur_v].parent = vert_number;
    tmp_ch = GetCode(s[vertexes[vert_number].l]);
    next[vertexes[vert_number].parent][tmp_ch] = vert_number;
    vert_number += 2;

    cur_v = vertexes[vertexes[vert_number - 2].parent].suff;  // поднимаемся назад и переходим по суфф. ссылке
    cur_pos = vertexes[vert_number - 2].l;

    while (cur_pos <= vertexes[vert_number - 2].r) {  // читаем оставшийся суффикс пока можем
        tmp_ch = GetCode(s[cur_pos]);
        cur_v = next[cur_v][tmp_ch];
        cur_pos += vertexes[cur_v].r - vertexes[cur_v].l + 1;
    }

    // ставим новую суфф. ссылку
    if (cur_pos == vertexes[vert_number - 2].r + 1) {
        vertexes[vert_number - 2].suff = cur_v; // если пришли в вершину, то в нее
    } else {
        vertexes[vert_number - 2].suff = vert_number; // иначе в новую вершину в будущем
    }
    // передвигаем cur_pos и снова добавляем букву
    cur_pos = vertexes[cur_v].r - (cur_pos - vertexes[vert_number - 2].r) + 2;
}

void DoubleSuffTree::UkkonenAdding(int ch) {
    bool need_new_vertex = true;
    // пока добавление символа ch, в т.ч. после перехода по суфф. ссылке, требует добавления новой вершины
    while (need_new_vertex) {
        if (vertexes[cur_v].r < cur_pos) {
            // если вышли за пределы ребра и дальше нет ребер
            if (next[cur_v][ch] == -1) {
                next[cur_v][ch] = vert_number;
                vertexes[vert_number].l = cur_s;
                vertexes[vert_number].parent = cur_v;
                vert_number++;
                cur_v = vertexes[cur_v].suff;
                cur_pos = vertexes[cur_v].r + 1;
                continue;
            }
            // иначе просто переходим к следующему ребру
            cur_v = next[cur_v][ch];
            cur_pos = vertexes[cur_v].l;
        }

        // совпадает ли ch с буквой на ребре
        bool ch_cur_pos_eq = (cur_pos == -1) ? true : (ch == GetCode(s[cur_pos]));
        if (ch_cur_pos_eq) {
            cur_pos++;  // проходим по букве, если можем
            need_new_vertex = false;
        } else {    // разделяем ребро на два
            Split(ch);
        }
    }
}

int DoubleSuffTree::GetCode(char ch) {
    if (ch == '#') {
        return 0;
    }

    if (ch =='$') {
        return 1;
    }

    return int(ch - 'a') + 2;
}

void DoubleSuffTree::DescribeTree() {
    cout << vert_number - 1 << endl; // выводим кол-во вершин
    int counter = 0;
    DFS(0, counter);
}

void DoubleSuffTree::DFS(int v, int& counter) {
    if (v != 0) {
        DescribeVertex(v);
    }

    sorted_vertexes[v] = counter;
    counter++;

    bool was_dollar = false; // прошли ли мы по ребру с $
    if (vertexes[v].l + 1 <= l1 && vertexes[v].r >= l1) {
        was_dollar = true;
    }

    if (!was_dollar) { // если еще не прошли через $
        for (int i = 0; i <= 27; i++) {
            if (next[v][i] > 0) {
                DFS(next[v][i], counter);
            }
        }
    }
}

void DoubleSuffTree::DescribeVertex(int v) {
    printf("%d", sorted_vertexes[vertexes[v].parent]); // выводим родителя

    // выводим, к какой строке относится подстрока
    int w;
    if (vertexes[v].l + 1 <= l1) {
        w = 0;
    } else {
        w = 1;
    }
    printf(" %d ", w);

    // выводим границы подстроки
    if (w == 0) {
        printf("%d ", vertexes[v].l);
        if (vertexes[v].r >= l1) {
            printf("%d", l1);
        } else {
            printf("%d", vertexes[v].r + 1);
        }
    } else {
        printf("%d %d", vertexes[v].l - l1, vertexes[v].r - l1 + 1);
    }

    printf("\n");
}

int main() {
    string s1;
    string s2;
    cin >> s1 >> s2;
    string str = s1 + s2;

    DoubleSuffTree double_suff_tree;
    double_suff_tree.AddFirstString(s1);
    double_suff_tree.AddSecondString(s2);

    double_suff_tree.UkkonenBuilding();

    double_suff_tree.DescribeTree();
}