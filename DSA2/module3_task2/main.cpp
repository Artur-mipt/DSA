#include <iostream>
#include <vector>
#include <set>
#include <queue>
#include <algorithm>
using namespace std;

class Task3Solver {
public:
    void ReadData();
    // нахождение кратчайшего пути, по которому можно пустить поток
    vector <int> ShortestWay();
    int EdmondsKarp();

private:
    const int INF = 1000000;
    // кол-во вершин
    int n;
    // кол-во ребер
    int m;
    vector <vector <int> > g;
    int start;
    int finish;
    // ребра, пропускная способность которых еще не заполнена
    set <pair <int, int> > freeNetwork;
};

void Task3Solver::ReadData() {
    cin >> n >> m;
    g.assign(n + 1, vector <int>(n + 1, 0));
    for( int i = 0; i < m; i++ ) {
        int v1, v2, weight;
        cin >> v1 >> v2 >> weight;
        g[v1][v2] = weight;
        // g[v2][v1] = weight;
    }
    start = 1;
    finish = n;
}

vector <int> Task3Solver::ShortestWay() {
    queue <int> q;
    q.push(start);
    vector <bool> visited(n + 1, false);
    visited[start] = true;
    vector <int> parent(n + 1);
    parent[start] = -1;

    vector <int> answer;

    while(!q.empty()) {
        int u = q.front();
        q.pop();
        for( int v = 1; v <= n; v++ ) {
            if( g[u][v] > 0 && visited[v] == false ) {
                visited[v] = true;
                parent[v] = u;
                q.push(v);
            }
        }
    }

    if( !visited[finish] ) {
        answer.push_back(-1);
        return answer;
    } else {
        for( int i = finish; i != -1; i = parent[i]) {
            answer.push_back(i);
        }
        reverse(answer.begin(), answer.end());
        return answer;
    }
}

int Task3Solver::EdmondsKarp() {
    vector <int> path = ShortestWay();
    int maxFlow = 0;

    // пока есть кратчайший путь
    while( path[0] != -1 ) {
        int minFlow = INF;

        for( int i = 0; i < path.size() - 1; i++ ) {
            int from = path[i];
            int to = path[i + 1];
            minFlow = min(g[from][to], minFlow);
        }

        maxFlow += minFlow;
        for( int i = 0; i < path.size() - 1; i++ ) {
            int from = path[i];
            int to = path[i + 1];
            g[from][to] -= minFlow;
            g[to][from] += minFlow;
        }

        path = ShortestWay();
    }

    return maxFlow;
}

int main() {
    Task3Solver solver;
    solver.ReadData();

    cout << solver.EdmondsKarp();
}