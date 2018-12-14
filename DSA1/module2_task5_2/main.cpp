// Дан массив неотрицательных целых 64-битных чисел. Количество чисел не больше 1000000.
// Отсортировать массив методом поразрядной сортировки LSD по байтам.

#include <iostream>
using namespace std;

// функция, возвращающая (к+1)-ый с конца байт числа
int KByte( long long a, int k )
{
    for( int i = 0; i < k; i++ ) {
        a = a >> 8;
    }
    return a % 256;
}

// сортировка подсчетом для конкретного байта
void CountingSort( long long* arr, int n, int byte )
{
    int* c = new int[256];
    for( int i = 0; i < 256; i++ ) {
        c[i] = 0;
    }

    for( int i = 0; i < n; i++ ) {
        int temp = KByte(arr[i], byte);
        c[temp]++;
    }

    for( int i = 1; i < 256; i++ ) {
        c[i] += c[i-1];
    }

    long long* b = new long long[n];
    for( int i = n - 1; i >= 0; i-- ) {
        int temp = KByte( arr[i], byte );
        c[temp]--;
        int pos = c[temp];
        b[pos] = arr[i];
    }

    for (int i = 0; i < n; i++) {
        arr[i] = b[i];
        //cout << arr[i] << " ";
    }
    //cout << endl;

    delete[] c;
    delete[] b;
}

// поразрядная сортировка LSD по байтам
void LSDSort( long long* arr, int n )
{
    for( int r = 0; r < 8; r++ ) {
        CountingSort( arr, n, r );
    }
}

int main() {
    int n;
    cin >> n;
    long long* arr = new long long[n];
    for( int i = 0; i < n; i++ ) {
        cin >> arr[i];
    }

    LSDSort(arr, n);

    for (int i = 0; i < n; i++ ) {
        cout << arr[i] << " ";
    }

    delete[] arr;
}
