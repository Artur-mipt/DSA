//даны два массива длинами n и m, найти их пересечение за O(n log k)
//где k - позиция элемента b[m-1] в массиве a

#include <iostream>
using namespace std;

//реализация бин поиска, где в кач-ве аргументов передаётся массив,
//элемент, который надо в нем искать, и границы, в которых надо искать
//функция возвращает индекс найденного элемента
int BinarySearch(const int* arr, int left, int right, int element)
{
    while(left < right) {
        int middle = (left + right) / 2;
        if (arr[middle] < element) {
            left = middle + 1;
        }
        else {
            right = middle;
        }
    }
    return left;
}

int main()
{
    int n, m;
    cin >> n >> m;
    int* a = new int[n];
    int* b = new int[m];

    for (int i = 0; i < n; i++) {
        cin >> a[i];
    }

    for (int i = 0; i < m; i++) {
        cin >> b[i];
    }

    int ind = 0;
    for (int i = 0; i < m; i++) {
        ind = BinarySearch(a, ind, n-1, b[i]);
        if (a[ind] == b[i]) cout << b[i] << " ";
    }

    delete[] a;
    delete[] b;
}
