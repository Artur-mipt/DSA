// Требуется отыскать самый выгодный маршрут между городами.
// Из города может выходить дорога, которая возвращается в этот же город.
// Требуемое время работы O((N + M)log N), где N – количество городов, M – известных дорог между ними.
// N ≤ 10000, M ≤ 250000.
// Длина каждой дороги ≤ 10000.

#include <iostream>
#include <algorithm>
#include <vector>
#include <set>
using namespace std;

class Graph {
public:
    Graph() {};
    ~Graph() = default;
    void ReadData();
    void Dijkstra();
private:
    const int INF = 1000000000;
    int n = 0; // кол-во вершин
    int m = 0; // кол-во ребер
    vector <vector <pair <int, int> > > g; // граф
    int start = 0; // начало пути
    int finish = 0; // конец пути
    vector <long long> dist;
};

void Graph::ReadData() {
    cin >> n;
    cin >> m;
    g.assign(n, vector <pair <int, int> >(0));
    for (int i = 0; i < m; i++) {
        int from, to, distance;
        cin >> from >> to >> distance;
        g[from].push_back(make_pair(distance, to));
        g[to].push_back(make_pair(distance, from));
    }
    cin >> start >> finish;
};

void Graph::Dijkstra() {
    dist.assign(n, INF);
    dist[start] = 0;
    set< pair <int, int> > unused;
    unused.insert(make_pair(0, start));
    while(!unused.empty()) {
        // берем номер вершины, до которой наименьшее расстояние
        int u = unused.begin()->second;
        unused.erase(unused.begin());
        // релаксируем ребра
        for( auto edge: g[u] ) {
            if( dist[u] + edge.first < dist[edge.second] ) {
                unused.erase(make_pair(dist[edge.second], edge.second));
                dist[edge.second] = dist[u] + edge.first;
                unused.insert(make_pair(dist[edge.second], edge.second));
            }
        }
    }

    cout << dist[finish];
}

int main() {
    Graph g;
    g.ReadData();
    g.Dijkstra();
}
