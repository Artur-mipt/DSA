// Написать алгоритм для решения игры в “пятнашки”.
// Решением задачи является приведение к виду: [ 1 2 3 4 ] [ 5 6 7 8 ] [ 9 10 11 12] [ 13 14 15 0 ],
// где 0 задает пустую ячейку. Необходимо найти минимальное решение.

#include <iostream>
#include <map>
#include <set>
#include <vector>
#include <cmath>
#include <algorithm>
#include <string>
#include <cstring>
using namespace std;

class Task4Solver {
public:
    Task4Solver(vector <int> &v);
    // поиск состояний, в которые можно прийти из данного
    void findNeighbors(vector <int> &state, vector <vector <int> > &vectorNeighbors);
    // определение, каким именнно сдвигом из состояния 'from' можно получить 'to'
    char fromWhere(vector <int> &from, vector <int> &to);
    // вычисление эвристики
    double heuristic(vector <int> &state);
    bool hasSolution();
    void aStar();
    void writeAnswer();
private:
    vector <int> start;
    vector <int> finish = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 0 };
    // set с вершинами, которые следует рассмотреть
    set <pair <double, vector <int> > > forVisit;
    // set c рассмотренными вершинами
    set <vector <int> > visited;
    // map с наименьшими расстояниями
    map <vector <int>, double> dist;
    // map с предками
    map <vector <int>, vector <int> > parent;
};

Task4Solver::Task4Solver(vector <int> &v) {
    start = v;
}

void Task4Solver::findNeighbors(vector <int> &state, vector <vector <int> > &vectorNeighbors) {
    int null = 0;
    for( int i = 0; i < 16; i++ ) {
        if( state[i] == 0 ) {
            null = i;
        }
    }
    if( null != 3 && null != 7 && null != 11 && null != 15 ) {
        vector <int> tmpVector = state;
        swap(tmpVector[null], tmpVector[null + 1]);
        vectorNeighbors.push_back(tmpVector);
    }
    if( null != 0 && null != 4 && null != 8 && null != 12 ) {
        vector <int> tmpVector = state;
        swap(tmpVector[null], tmpVector[null - 1]);
        vectorNeighbors.push_back(tmpVector);
    }
    if( null > 3 ) {
        vector <int> tmpVector = state;
        swap(tmpVector[null], tmpVector[null - 4]);
        vectorNeighbors.push_back(tmpVector);
    }
    if( null < 12 ) {
        vector <int> tmpVector = state;
        swap(tmpVector[null], tmpVector[null + 4]);
        vectorNeighbors.push_back(tmpVector);
    }
}

char Task4Solver::fromWhere(vector <int> &from, vector <int> &to) {
    int nullFrom = 0;
    for( int i = 0; i < 16; i++ ) {
        if( from[i] == 0 ) {
            nullFrom = i;
        }
    }
    int nullTo = 0;
    for( int i = 0; i < 16; i++ ) {
        if( to[i] == 0 ) {
            nullTo = i;
        }
    }
    if( nullTo - nullFrom == 1 ) {
        return 'L';
    }
    if( nullTo - nullFrom == -1 ) {
        return 'R';
    }
    if( nullTo - nullFrom == 4 ) {
        return 'U';
    }
    if( nullTo - nullFrom == -4 ) {
        return 'D';
    }
}

double Task4Solver::heuristic(vector <int> &state) {
    double sum = 0;
    for( int i = 0; i < 16; i++ ) {
        if( state[i] != 0 ) {
            sum += abs( (state[i] - 1) / 4 - i / 4 ) + abs( (state[i] - 1) % 4 - i % 4 );
        }
        // else {
        //    sum += abs(i / 4) + abs(i % 4);
        // }
    }

    vector <int> row(4, 0);
    vector <int> column(4, 0);

    for( int k = 0; k < 4; k++ ) {
        int last = -1;
        bool flag = false;
        for( int i = 0; i < 4; i++ ) {
            int a = state[4*k+i];
            if( (a - 1) / 4 != (4*k + i) / 4 || a == 0 ) {
                continue;
            }
            if( last != -1 && a < last ) {
                sum += 2;
                flag = true;
                row[k] = 1;
            }
            last = a;
        }
    }

    for( int k = 0; k < 4; k++ ) {
        int last = -1;
        for( int i = 0; i < 4; i++ ) {
            int a = state[i*4 + k];
            if( (a - 1) / 4 != (i*4 + k) / 4 || a == 0 ) {
                continue;
            }
            if( last != -1 && a < last ) {
                sum += 2;
                column[k] = 1;
            }
            last = a;
        }
    }

    if( state[0] != 1 && state[1] == 2 && state[4] == 5 &&
       state[0] != 0 && state[1] != 0 && state[4] != 0 &&
       column[0] == 0 && row[0] == 0 ) {
        sum += 2;
    }

    if( state[3] != 4 && state[2] == 3 && state[7] == 8 &&
       state[3] != 0 && state[2] != 0 && state[3] != 0 &&
       column[3] == 0 && row[0] == 0 ) {
        sum += 2;
    }

    if( state[12] != 13 && state[8] == 9 && state[13] == 14 &&
       state[12] != 0 && state[8] != 0 && state[13] != 0 &&
       column[0] == 0 && row[3] == 0 ) {
        sum += 2;
    }

    return sum * 1.455;
}

bool Task4Solver::hasSolution() {
    int inv = 0;
    for( int i = 0; i < 16; ++i ) {
        if( start[i] ) {
            for( int j = 0; j < i; ++j ) {
                if( start[j] > start[i] ) {
                    ++inv;
                }
            }
		}
    }

    for( int i = 0; i < 16; ++i ) {
        if( start[i] == 0 ) {
            inv += 1 + i / 4;
        }
    }

    return !(inv & 1);
}

void Task4Solver::aStar() {
    dist.insert(pair<vector <int>, double>(start, 0));
    double f = dist[start] + heuristic(start);
    forVisit.insert(pair<double, vector <int> >(f, start));

    while( forVisit.size() != 0 ) {
        vector <int> current = forVisit.begin()->second;
        if( current == finish ) {
            break;
        }
        forVisit.erase(forVisit.begin());
        visited.insert(current);
        vector <vector <int> > neighbors;
        findNeighbors(current, neighbors);
        for( int i = 0; i < neighbors.size(); i++ ) {
            int tentativeScore = dist[current] + 1;
            vector <int> v = neighbors[i];
            if( visited.count(v) != 0 && tentativeScore >= dist[v] ) {
                continue;
            }
            if( visited.count(v) == 0 || tentativeScore < dist[v] ) {
                parent.insert(pair <vector <int>, vector <int> >(v, current));
                pair <long long, vector <int> > tmpPair = make_pair(dist[v] + heuristic(v), v);
                forVisit.erase(tmpPair);
                dist[v] = tentativeScore;
                double f = dist[v] + heuristic(v);
                forVisit.insert(pair <double, vector <int> >(f, v));
            }
        }
    }

}

void Task4Solver::writeAnswer() {
    bool flag = hasSolution();
    if( !flag ) {
        cout << -1;
    } else {
        aStar();
        string s = "";
        vector <int> current = finish;
        while( current != start ) {
            char ch = fromWhere(parent[current], current);
            s += ch;
            current = parent[current];
        }
        cout << s.length() << endl;
        for( int i = s.length() - 1; i >= 0; i-- ) {
            cout << s[i];
        }
    }
}

int main() {
    vector <int> start;
    for( int i = 0; i < 16; i++ ) {
        int a;
        cin >> a;
        start.push_back(a);
    }

    Task4Solver solver(start);
    solver.writeAnswer();
}
