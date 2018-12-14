// Дан массив целых чисел в диапазоне [0..10^9]. Размер массива кратен 10 и ограничен сверху
// значением 2.5 * 10^6 элементов. Все значения массива являются элементами псевдо-рандомной
// последовательности. Необходимо отсортировать элементы массива за минимальное время и вывести
// каждый десятый элемент отсортированной последовательности.
// Реализуйте сортировку, основанную на QuickSort.
// Минимальный набор оптимизаций, который необходимо реализовать:
// 1. Оптимизация ввода/вывода
// 2. Оптимизация выбора опорного элемента
// 3. Оптимизация Partition
// 4. Написать без рекурсии
// 5. Оптимизация концевой рекурсии

#include <iostream>
#include <vector>
#include <fstream>
#include <stack>
using namespace std;

// сортировка вставками для оптимизации концевой рекурсии
void InsertionSort( vector <int> &v, int left, int right )
{
    if( left >= right ) {
        return;
    }

    for( int i = left + 1; i <= right; i++ ) {
        int temp = v[i];
        int j = i - 1;
        while( j >= 0 && v[j] > temp ) {
            v[j + 1] = v[j];
            j--;
        }
        v[j + 1] = temp;
    }
}


// выбор опорного элемента по стратегии "медиана трех"
void MedianOfThree( vector <int> &v, int left, int right )
{
    if( v[left] > v[right] ) swap(v[left], v[right]);
    if( v[left] > v[(right + left) / 2] ) swap(v[left], v[(right + left) / 2]);
    if( v[right] > v[(right + left) / 2] ) swap(v[right], v[(right + left) / 2]);
}

// разбиение массива относительно пивота
int Partition( vector <int> &v, int left, int right )
{
    // оптимизация концевой рекурсии
    if( right - left <= 0) {
        return left;
    }

    // оптимизация выбора опорного элемента
    MedianOfThree( v, left, right );
    const int pivot = v[right];

    int i = left;
    int j = left;
    while( j < right ) {
        if( v[j] > pivot ) {
            j++;
        } else {
            if( i != j ) {
                swap( v[i], v[j] );
            }
            i++;
            j++;
        }
    }

    swap( v[i], v[right] );

    return i;
}

// нерекурсивная реализация быстрой сортировки
void QuickSort( vector <int> &v, int left, int right ) {
    // используем стек для избавления от рекурсии
    stack <int> s;
    s.push(right);
    s.push(left);

    while( !s.empty() ) {
        int left = s.top();
        s.pop();
        int right = s.top();
        s.pop();
        // оптимизация концевой рекурсии
        // если элементов подмассива <= 11, то используем сортировку вставками
        if( right - left + 1 <= 11 ) {
            InsertionSort( v, left, right );
        } else {
            if( left < right ) {
                int part = Partition( v, left, right );
                if( part > 0 ) {
                    s.push( part - 1 );
                    s.push( left );
                }
                if( part < right ) {
                    s.push( right );
                    s.push( part + 1 );
                }
            }
        }
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(0);
    // ifstream cin( "input.txt" );
    // ofstream cout( "output.txt" );
    vector <int> v;
    int a;

    // оптимизация ввода
    while( cin >> a ) {
        v.push_back(a);
    }

    QuickSort( v, 0, v.size() - 1 );

    // оптимизация вывода
    for( int i = 9; i < v.size(); i += 10 ) {
        cout << v[i] << " ";
    }

    v.clear();
}
