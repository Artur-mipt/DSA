// 1_2. Ломаная 1.
// Задано N точек на плоскости. Указать (N-1)-звенную несамопересекающуюся незамкнутую ломаную,
// проходящую через все эти точки.
// Указание: стройте ломаную в порядке возрастания x-координаты.
// Если имеются две точки с одинаковой x-координатой,
// то расположите раньше ту точку, у которой y-координата меньше.

#include <iostream>

using namespace std;

// структура для хранения координат точек
struct CPoint {
    int x;
    int y;
};

// сортировка вставками для массива структуры CPoint
void InsertionSort( CPoint* a, int n )
{
    for( int i = 1; i < n; i++ ) {
        int tmpx = a[i].x;
        int tmpy = a[i].y;
        int j = i - 1;
        while( (j >= 0) && ( (a[j].x > tmpx) || (a[j].x == tmpx && a[j].y > tmpy) ) ) {
            a[j + 1].x = a[j].x;
            a[j + 1].y = a[j].y;
            j--;
        }
        a[j+1].x = tmpx;
        a[j+1].y = tmpy;
    }
}

int main() {
    int n;
    cin >> n;
    CPoint* a = new CPoint[n];
    for( int i = 0; i < n; i++ ) {
        cin >> a[i].x >> a[i].y;
    }
    InsertionSort( a, n );
    for( int i = 0; i < n; i++ ) {
        cout << a[i].x << " " << a[i].y << endl;
    }
    delete[] a;
}
