// Полный ориентированный взвешенный граф задан матрицей смежности.
// Постройте матрицу кратчайших путей между его вершинами.
// Гарантируется, что в графе нет циклов отрицательного веса.

#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
using namespace std;

class Graph {
public:
    Graph() {};
    ~Graph() = default;
    void ReadData();
    void Floyd();
private:
    int n = 0;
    vector <vector <int> > g;
    vector <vector <int> > answer;
};

void Graph::ReadData() {
    cin >> n;
    g.assign(n, vector <int>(n, 0));
    for( int i = 0; i < n; i++ )
        for( int j = 0; j < n; j++ ) {
            cin >> g[i][j];
        }
}

void Graph::Floyd() {
    answer.assign(n, vector <int>(n, 0));
    for( int i = 0; i < n; i++ )
        for( int j = 0; j < n; j++ ) {
            answer[i][j] = g[i][j];
        }

    for( int k = 0; k < n; k++ )
        for( int i = 0; i < n; i++ )
            for( int j = 0; j < n; j++ ) {
                answer[i][j] = min(answer[i][j], g[i][k] + g[k][j]);
            }

    for( int k = 0; k < n; k++ )
        for( int i = 0; i < n; i++ )
            for( int j = 0; j < n; j++ ) {
                answer[i][j] = min(answer[i][j], answer[i][k] + answer[k][j]);
            }

    for( int i = 0; i < n; i++ ) {
        for( int j = 0; j < n; j++ )
            cout << answer[i][j] << " ";
        cout << endl;
    }
}

int main() {
    ifstream cin("floyd.in");
    ofstream cout("floyd.out");
    Graph g;
    g.ReadData();
    g.Floyd();
}
