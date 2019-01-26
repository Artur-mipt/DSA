// В город N приехал цирк с командой атлетов.
// Они хотят удивить горожан города N — выстроить из своих тел башню максимальной высоты.
// Башня — это цепочка атлетов, первый стоит на земле, второй стоит у него на плечах, третий
// стоит на плечах у второго и т.д. Каждый атлет характеризуется силой si (kg) и массой mi (kg).
// Сила — это максимальная масса, которую атлет способен держать у себя на плечах.
// К сожалению ни один из атлетов не умеет программировать, так как всю жизнь они занимались
// физической подготовкой, и у них не было времени на изучение языков программирования.
// Помогите им, напишите программу, которая определит максимальную высоту башни, которую они могут составить.
// Известно, что если атлет тяжелее, то он и сильнее: если mi>mj, то si > sj.
// Атлеты равной массы могут иметь различную силу.


#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
    int m, s;
    vector < pair <int, int> > v;

    while( cin >> m >> s ) {
        v.push_back( make_pair( m, s ) );
    }

    sort( v.begin(), v.end() );

    // будем строить башню сверху вниз
    // currentM = общая масса атлетов, которые уже стоят в башне

    int currentM = v[0].first;
    int ans = 1;

    for( int i = 1; i < v.size(); i++ )
    {
        if( v[i].second >= currentM ) {
            ans++;
            currentM += v[i].first;
        }
    }

    cout << ans;
}
