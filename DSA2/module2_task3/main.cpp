// «Восьминашки» – упрощенный вариант известной головоломки «Пятнашки».
// Восемь костяшек, пронумерованных от 1 до 8, расставлены по ячейкам игровой доски 3 на 3,
// одна ячейка при этом остается пустой. За один ход разрешается передвинуть одну из костяшек,
// расположенных рядом с пустой ячейкой, на свободное место.
// Цель игры – для заданной начальной конфигурации игровой доски за минимальное число ходов
// получить выигрышную конфигурацию (пустая ячейка обозначена нулем):
// 1 2 3
// 4 5 6
// 7 8 0


#include <iostream>
#include <cmath>
#include <algorithm>
#include <vector>
#include <set>
#include <map>
#include <queue>
using namespace std;

class Task3Solver {
public:
    Task3Solver(vector <int> v);
    // поиск состояний, в которые можно прийти из данного
    void findNeighbors(vector <int> &state, vector <vector <int> > &neighbors);
    // определение, каким именнно сдвигом из состояния 'from' можно получить 'to'
    char fromWhere(vector <int> &from, vector <int> &to);
    void bfs();
private:
    vector <int> start;
    // сет состояний, в которых мы уже были
    set <vector <int> > used;
    vector <int> finish = {1, 2, 3, 4, 5, 6, 7, 8, 0};
    // mapOfDist каждому состоянию сопоставляет дистанцию от него из start
    map <vector <int>, int> mapOfDist;
    // mapOfWay для каждого состояния хранит путь до него из start
    map <vector <int>, string> mapOfWay;
};

Task3Solver::Task3Solver(vector <int> v) {
    start = v;
}

void Task3Solver::findNeighbors(vector <int> &state, vector <vector <int> > &vectorNeighbors) {
    int null = 0;

    for( int i = 0; i < 9; i++ ) {
        if( state[i] == 0 ) {
            null = i;
        }
    }

    if( null != 2 && null != 5 && null != 8 ) {
        vector <int> tmpVector = state;
        swap(tmpVector[null], tmpVector[null + 1]);
        vectorNeighbors.push_back(tmpVector);
    }

    if( null != 0 && null != 3 && null != 6 ) {
        vector <int> tmpVector = state;
        swap(tmpVector[null], tmpVector[null - 1]);
        vectorNeighbors.push_back(tmpVector);
    }

    if( null != 0 && null != 1 && null != 2 ) {
        vector <int> tmpVector = state;
        swap(tmpVector[null], tmpVector[null - 3]);
        vectorNeighbors.push_back(tmpVector);
    }

    if( null != 6 && null != 7 && null != 8 ) {
        vector <int> tmpVector = state;
        swap(tmpVector[null], tmpVector[null + 3]);
        vectorNeighbors.push_back(tmpVector);
    }
}

char Task3Solver::fromWhere(vector <int> &from, vector <int> &to) {
    int nullFrom = 0;
    for( int i = 0; i < 9; i++ ) {
        if( from[i] == 0 ) {
            nullFrom = i;
        }
    }
    int nullTo = 0;
    for( int i = 0; i < 9; i++ ) {
        if( to[i] == 0 ) {
            nullTo = i;
        }
    }

    if( nullTo - nullFrom == 1 ) {
        return 'R';
    }
    if( nullTo - nullFrom == -1 ) {
        return 'L';
    }
    if( nullTo - nullFrom == 3 ) {
        return 'D';
    }
    if( nullTo - nullFrom == -3 ) {
        return 'U';
    }
}

void Task3Solver::bfs() {
    queue <vector <int> > q;
    q.push(start);
    used.insert(start);
    bool isFind = false;
    mapOfDist.insert( pair <vector <int>, int>(start, 0) );
    mapOfWay.insert( pair <vector <int>, string>(start, "") );

    while( !q.empty() && isFind == false ) {
        vector <int> current = q.front();
        q.pop();
        if( current == finish ) {
            isFind = true;
            break;
        }
        vector <vector <int> > neighbors;
        findNeighbors( current, neighbors );
        for( int i = 0; i < neighbors.size(); i++ ) {
            vector <int> to = neighbors[i];
            if( used.count(to) == 0 ) {
                used.insert(to);
                q.push(to);
                int dist = mapOfDist[current] + 1;
                char way = fromWhere(current, to);
                string path = mapOfWay[current] + way;
                mapOfDist.insert( pair <vector <int>, int>(to, dist));
                mapOfWay.insert( pair <vector <int>, string>(to, path));
            }
        }
    }

    if( isFind == false ) {
        cout << -1;
    } else {
        cout << mapOfDist[finish] << endl;
        cout << mapOfWay[finish] << endl;
    }
}

using namespace std;
int main() {
    vector <int> start(9);
    for( int i = 0; i < 9; i++ ) {
        int a;
        cin >> a;
        start[i] = a;
    }

    Task3Solver solver(start);
    solver.bfs();
}
