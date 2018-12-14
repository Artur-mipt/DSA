// Дана очень длинная последовательность целых чисел длины n.
// Требуется вывести в отсортированном виде её первые k элементов.
// Последовательность может не помещаться в память.
// Время работы O(n * log(k)). Доп. память O(k). Использовать слияние.

#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>
#include <fstream>
using namespace std;

// слияние двух половинок массива v, причем результат записывается в c
void Merge( vector <int> &v, int left1, int right1, int left2, int right2, vector <int> &c )
{
    int i = left1;
    int j = left2;
    while( i <= right1 && j <= right2 ) {
        if( v[i] <= v[j] ) {
            c.push_back( v[i] );
            i++;
        } else {
            c.push_back( v[j] );
            j++;
        }
    }
    if( i > right1 ) {
        for( int k = j; k <= right2; k++ ) {
            c.push_back( v[k] );
        }
    } else {
        for( int k = i; k <= right1; k++) {
            c.push_back( v[k] );
        }
    }
}

// рекурсивная реализация сортировки слиянием
void MergeSort( vector <int> &v, int left, int right ) {
    if( right - left <= 0 ) {
        return;
    }

    int middle = (left + right) / 2;
    MergeSort( v, left, middle);
    MergeSort( v, middle + 1, right );

    vector <int> c;
    Merge( v, left, middle, middle + 1, right, c);

    int indC = 0;
    for( int i = left; i <= right; i++ ) {
        v[i] = c[indC];
        indC++;
    }

    c.clear();
}

// процедура заполнения массивов v1 и v2 (используется при вводе данных)
void AddElem( vector <int> &v1, vector <int> &v2, bool &flag1, bool &flag2, int elem, int k )
{
    if( flag1 && v2.size() < k ) {
        v2.push_back( elem );
    }
    if( v2.size() == k && !flag2 ) {
        MergeSort( v2, 0, v2.size() - 1 );
        flag2 = true;
    }
    if( v1.size() < k ) {
        v1.push_back( elem );
    }
    if( v1.size() == k && !flag1 ) {
        MergeSort( v1, 0, v1.size() - 1 );
        flag1 = true;
    }
    if( flag1 && flag2 ) {
        vector <int> v;
        for( int i = 0; i < v1.size(); i++ ) {
            v.push_back( v1[i] );
        }
        for( int i = 0; i < v2.size(); i++ ) {
            v.push_back( v2[i] );
        }
        vector <int> c;
        Merge( v, 0, k-1, k, 2*k - 1, c);
        for( int i = 0; i < k; i++ ) {
            v1[i] = c[i];
        }
        c.clear();
        v2.clear();
        flag2 = false;
    }
}


int main() {
    ifstream cin("input.txt");
    ofstream cout("output.txt");

    int n, k;
    cin >> n >> k;

    // в векторе v1 хранятся отсортированные минимальные k элементов на данный момент
    // вектор v2 используется для слияния его с v1 и дальнейшего обновления v1
    vector <int> v1;
    vector <int> v2;

    // flag1 == true, если вектор v1 отсортирован и заполнен, иначе flag1 == false
    // flag2 == true, если вектор v2 отсортирован и заполнен, иначе flag2 == false
    bool flag1 = false;
    bool flag2 = false;

    // ввод данных с преобразованием векторов v1 и v2
    for( int i = 0; i < n; i++ ) {
        int a;
        cin >> a;
        AddElem( v1, v2, flag1, flag2, a, k);
    }

    // если вектор 2 остался незаполненным до конца
    if ( !v2.empty() ) {
        MergeSort( v2, 0, v2.size() - 1 );
        vector <int> v;
        for (int i = 0; i < v1.size() ; i++ ) {
            v.push_back( v1[i] );
        }
        for (int i = 0; i < v2.size(); i++ ) {
            v.push_back( v2[i] );
        }
        MergeSort( v, 0, v.size() - 1 );
        for( int i = 0; i < k; i++ ) {
            v1[i] = v[i];
        }
        v.clear();
    }

    for( int i = 0; i < v1.size(); i++ ) {
        cout << v1[i] << " ";
    }

    v1.clear();
    v2.clear();
}
