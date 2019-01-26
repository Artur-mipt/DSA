#include <iostream>
#include <vector>
#include <algorithm>
#include <set>
using namespace std;

void PrimAlgorithm(vector <vector <pair <int, int> > > g) {
    const int INF = 1000000;
    int n = g.size() - 1;

    // вес минимального остова
    int answer = 0;

    // min_e[i] - минимальный вес допустимого ребра из i в уже посещенную вершину
    vector <int> min_e(n + 1, INF);
    min_e[1] = 0;

    // forVisit - сет еще не посещенных вершин
    set <pair <int, int>> forVisit;
    forVisit.insert({0, 1});

    // visited - сет вершин, уже взятых в мин. остов
    set <int> visited;

    for( int i = 0; i < n; i++ ) {
        if( forVisit.empty() ) {
            break;
        }
        // берем номер непосещенной вершины, из которой выходит минимальное ребро в посещенную
        int v = forVisit.begin()->second;
        forVisit.erase(forVisit.begin());
        visited.insert(v);

        // релаксируем метки для ее соседей
        for( int j = 0; j < g[v].size(); j++ ) {
            int to = g[v][j].first;
            int weight = g[v][j].second;
            if( weight < min_e[to] && visited.count(to) == 0 ) {
                forVisit.erase({min_e[to], to});
                min_e[to] = weight;
                forVisit.insert({min_e[to], to});
            }
        }
    }

    for( int i = 1; i < min_e.size(); i++ ) {
        answer += min_e[i];
    }

    cout << answer;
}

int main() {
    int n, m;
    cin >> n >> m;
    vector <vector <pair <int, int> > > g(n + 1);
    for( int i = 0; i < m; i++ ) {
        int b, e, w;
        cin >> b >> e >> w;
        g[b].push_back({e, w});
        g[e].push_back({b, w});
    }

    PrimAlgorithm(g);
}