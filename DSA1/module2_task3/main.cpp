// Даны неотрицательные целые числа n,k и массив целых чисел из [0..10^9] размера n.
// Требуется найти k-ю порядковую статистику. т.е. напечатать число,
// которое бы стояло на позиции с индексом k (0..n-1) в отсортированном массиве.
// Напишите нерекурсивный алгоритм.
// Требования к дополнительной памяти: O(n). Требуемое среднее время работы: O(n).
// Реализуйте стратегию выбора опорного элемента “медиана трёх”.
// Функцию Partition реализуйте методом прохода двумя итераторами от начала массива к концу

#include <iostream>
#include <cmath>
#include <cstdio>
#include <cstdlib>
using namespace std;

// функция для выбора пивота "медиана трёх"
void MedianOfThree(int* arr, int left, int right)
{
    if( arr[left] > arr[right] ) swap(arr[left], arr[right]);
    if( arr[left] > arr[(right + left) / 2] ) swap(arr[left], arr[(right + left) / 2]);
    if( arr[right] > arr[(right + left) / 2] ) swap(arr[right], arr[(right + left) / 2]);
}

// разделение массива с возвращением позиции пивота
int Partition( int* arr, int left, int right )
{
    MedianOfThree(arr, left, right);
    const int pivot = arr[right];

    int i = left - 1;
    int j = left;

    for( int j = left; j < right; j++ ) {
        if( arr[j] <= pivot ) {
            i++;
            swap( arr[i], arr[j] );
        }
    }

    swap( arr[i + 1], arr[right] );

    return i + 1;
}

// нерекурсивный поиск к-ой порядковой статистики
int KStatDC( int* arr, int left, int right, int k )
{
    int part = Partition( arr, left, right );
    while( part != k ) {
        if( part > k ) {
            right = part - 1;
            part = Partition(arr, left, right );
        } else {
            left = part + 1;
            part = Partition(arr, left, right );
        }
    }

    return arr[part];
}

int main() {
    int n;
    int k;
    scanf("%d", &n);
    scanf("%d", &k);

    int* arr = new int[n];
    for( int i = 0; i < n; i++ ) {
        scanf("%d", &arr[i]);
    }

    cout << KStatDC(arr, 0, n - 1, k);

    delete[] arr;
}
