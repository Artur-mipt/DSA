// Вычислить количество узлов в самом широком слое декартового дерева и количество узлов
// в самом широком слое наивного дерева поиска. Вывести их разницу. Разница может быть отрицательна.

#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>
using namespace std;

// описание узла дерева (и Декартова, и двоичного)
struct CTreeNode {
    int key;
    int prior;
    CTreeNode* left;
    CTreeNode* right;
    CTreeNode* parent;
};

// "наивное" добавление узла в бинарное дерево
void BinTreeInsert( CTreeNode*& root, int value, int priority )
{
    if( root == NULL ) {
        root = new CTreeNode();
        root->key = value;
        root->prior = priority;
        return;
    }
    if( root->key > value ) {
        BinTreeInsert( root->left, value, priority );
    } else {
        BinTreeInsert( root->right, value, priority );
    }
}

// процедура разрезания Декартова дерева по ключу
void Split( CTreeNode* current, int key, CTreeNode*& left, CTreeNode*& right )
{
    if( current == NULL ) {
        left = NULL;
        right = NULL;
    } else
    if( current->key <= key ) {
        Split( current->right, key, current->right, right );
        left = current;
    } else {
        Split( current->left, key, left, current->left );
        right = current;
    }
}

// процедура добавления узла в Декартово дерево
void TreapInsert( CTreeNode*& current, int value, int priority )
{
    // если узел с меньшим приоритетом не нашёлся
    // и рекурсия дошла до конца
    if( current == NULL ) {
        current = new CTreeNode();
        current->key = value;
        current->prior = priority;
        return;
    }

    // если узел с меньшим приоритетом нашёлся
    if( current->prior < priority ) {
        // создаем узел, который надо вставить
        CTreeNode* newNode = new CTreeNode();
        newNode->key = value;
        newNode->prior = priority;

        // разрезаем найденное поддерево по ключу value
        Split( current, newNode->key, newNode->left, newNode->right );

        // вставляем новый узел на место current
        current = newNode;

        return;
    }

    // иначе
    // рекурсивно спускаемся вниз по дереву
    if( current->key >= value ) {
        TreapInsert( current->left, value, priority );
    } else {
        TreapInsert( current->right, value, priority );
    }
}

// обход в глубину, в ходе которого создается заполняется вектор layers
// layers[i] - количество узлов в i-ом слое дерева
void DFS( CTreeNode*& root, int counter, vector <int> &layers )
{
    if( root == NULL ) {
        return;
    }

    layers[counter]++;
    DFS( root->left, counter + 1, layers );
    DFS( root->right, counter + 1, layers );
}

// удаление всех узлов путем обхода в глубину
void DeleteTree( CTreeNode*& root )
{
    if( root == NULL ) {
        return;
    }
    DeleteTree( root->left );
    DeleteTree( root->right );
    delete root;
}

// нахождение кол-ва узлов в самом широком слое дерева
int WidestLayer( CTreeNode*& root, int n )
{
    // layers[i] - количество узлов в i-ом слое дерева
    vector <int> layers(n, 0);
    DFS( root, 0, layers );
    int maxx = -1;
    for( int i = 0; i < n; i++ ) {
        if( layers[i] > maxx ) {
            maxx = layers[i];
        }
    }
    layers.clear();
    return maxx;
}

int main() {

    //ifstream cin("input.txt");
    //ofstream cout("output.txt");

    // число узлов
    int n;
    cin >> n;

    // первый узел
    int value, priority;
    cin >> value >> priority;

    // добавление первого узла в Декартово дерево
    CTreeNode* rootTreap = new CTreeNode();
    rootTreap->key = value;
    rootTreap->prior = priority;

    // добавление первого узла в двоичное дерево
    CTreeNode* rootBinTree = new CTreeNode();
    rootBinTree->key = value;
    rootBinTree->prior = priority;

    for( int i = 0; i < n - 1; i++ ) {
        // считывание нового узла
        cin >> value >> priority;

        // добавление узла в Декартово дерево
        TreapInsert( rootTreap, value, priority );

        // "наивное" добавление узла в обычное двоичное дерево поиска
        BinTreeInsert( rootBinTree, value, priority );
    }

    // подсчет ширин Декартова и обычного деревьев
    int wideOfTreap = WidestLayer( rootTreap, n );
    int wideOfBinTree = WidestLayer( rootBinTree, n );

    // вывод разности ширин
    cout << wideOfTreap - wideOfBinTree << endl;

    // очищение деревьев из памяти
    DeleteTree( rootTreap );
    DeleteTree( rootBinTree );
}
