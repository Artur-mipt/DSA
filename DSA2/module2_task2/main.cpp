// Тесты пока не готовы!
// Необходимо написать торгового советника для поиска арбитража.
// Арбитраж - это торговля по цепочке различных валют в надежде заработать на небольших различиях в коэффициентах.
// Например, есть следующие курсы валют:
// GBP/USD: 0.67
// RUB/GBP: 78.66
// USD/RUB: 0.02
// Имея 1$ и совершив цикл USD->GBP->RUB->USD, получим 1.054$. Таким образом заработав 5.4

#include <iostream>
#include <vector>
using namespace std;

class Graph {
public:
    Graph() {};
    ~Graph() = default;
    void ReadData();
    void Floyd();
private:
    int n = 0;
    vector <vector <double> > g;
};

void Graph::ReadData() {
    cin >> n;
    g.assign(n, vector <double>(n));
    for( int i = 0; i < n; i++ )
        for( int j = 0; j < n - 1; j++ ) {
            double a;
            cin >> a;
            if( i == j) {
                g[i][j] = 1;
            }
            if( j >= i ) {
                g[i][j + 1] = a;
            } else {
                g[i][j] = a;
            }
        }
}

void Graph::Floyd() {
    for( int k = 0; k < n; k++ )
        for( int i = 0; i < n; i++ )
            for( int j = 0; j < n; j++ ) {
                if( g[i][k] * g[k][j] > g[i][j] && g[i][k] != -1 && g[k][j] != -1 ) {
                    g[i][j] = g[i][k] * g[k][j];
                }
            }

    bool flag = false;
    for( int i = 0 ; i < n; i++ ) {
        if( g[i][i] > 1 ) {
            flag = true;
        }
    }

    if( flag ) {
        cout << "YES";
    } else {
        cout << "NO";
    }
}

int main() {
    Graph g;
    g.ReadData();
    g.Floyd();
    return 0;
}