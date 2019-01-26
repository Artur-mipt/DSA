#include <iostream>
#include <vector>
#include <cmath>
using namespace std;

class CTree {
public:
    CTree( int n );
    ~CTree();
    void ReadTree();
    int VectorChildren( int vertex );
    int VectorWays( int vertex );
    void VectorAns();
    void WriteTree();
private:
    int n; // количество вершин
    vector < vector <int> > g; // дерево
    vector <long long> ways; // вектор суммы путей до сыновей
    vector <int> children; // вектор количества сыновей в поддереве
    vector <int> ans; // вектор, хранящий сумму расстояний от i-й вершины до всех остальных
    vector <int> parents; // вектор, хранящий родителя для каждого узла
};

CTree::CTree( int num )
{
    n = num;
    vector < vector <int> > gNew(n);
    vector <long long> waysNew( n, -1 );
    vector <int> childrenNew( n, -1 );
    vector <int> ansNew( n, 0 );
    vector <int> parentsNew( n, -1 );
    g = gNew;
    ways = waysNew;
    children = childrenNew;
    ans = ansNew;
    parents = parentsNew;
    gNew.clear();
    waysNew.clear();
    childrenNew.clear();
    ansNew.clear();
    parentsNew.clear();
}

CTree::~CTree()
{
    g.clear();
    ways.clear();
    children.clear();
    ans.clear();
}

void CTree::ReadTree()
{
    for( int i = 0; i < n - 1; i++ ) {
        int x, y;
        cin >> x >> y;
        g[ min(x, y) ].push_back( max(x, y) );
        parents[ max(x, y) ] = min(x, y);
    }
}

// заполнение вектора количества сыновей
int CTree::VectorChildren( int vertex )
{
    if( g[vertex].size() == 0 ) {
        children[vertex] = 0;
        return 0;
    }

    children[vertex] = g[vertex].size();

    for( int i = 0; i < g[vertex].size(); i++ ) {
        if( children[ g[vertex][i] ] != -1) {
            children[vertex] += children[ g[vertex][i] ];
        } else {
            children[vertex] += VectorChildren( g[vertex][i] );
        }
    }

    return children[vertex];
}

// заполнение вектора суммы путей до всех сыновей
int CTree::VectorWays( int vertex )
{
    if( children[vertex] == 0 ) {
        ways[vertex] = 0;
        return 0;
    }

    ways[vertex] = 0;

    for( int i = 0; i < g[vertex].size(); i++ ) {
        if( ways[ g[vertex][i] ] != -1 ) {
            ways[vertex] += ways[ g[vertex][i] ] + children[ g[vertex][i] ] + 1;
        } else {
            ways[vertex] += VectorWays( g[vertex][i] ) + children[ g[vertex][i] ] + 1;
        }
    }

    return ways[vertex];
}

// заполнение вектора сумм от i-й вершины до всех остальных
void CTree::VectorAns()
{
    ans[0] = ways[0];
    for( int i = 1; i < n; i++ ) {
        ans[i] = ways[i] + ( ans[ parents[i] ] - children[i] - 1 - ways[i] ) +
                 ( n - children[i] - 1 );
    }
}

// вывод массива ans
void CTree::WriteTree()
{
    for( int i = 0; i < n; i++ ) {
        cout << ans[i] << endl;
    }
}


int main() {
    int n;
    cin >> n;
    CTree tree( n );

    // считывание дерева
    tree.ReadTree();

    // заполнение вектора количества сыновей
    tree.VectorChildren( 0 );

    // заполнение вектора суммы путей до всех сыновей
    tree.VectorWays( 0 );

    // заполнение вектора суммы расстояний от i-й вершины до всех остальных
    tree.VectorAns();

    // вывод
    tree.WriteTree();
}
