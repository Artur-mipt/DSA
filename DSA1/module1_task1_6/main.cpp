
/*
Дан массив целых чисел A[0..n). Не используя других массивов переставить элементы массива A
в обратном порядке за O(n).
≤ 10000.
*/

#include <iostream>
void CSwap(int& a, int&b)
{
    a = a + b;
    b = a - b;
    a = a - b;
}
using namespace std;
int main()
{
    int n;
    cin >> n;
    int* a = new int[n];
    for (int i = 0; i < n; i++)
        cin >> a[i];
    for (int i = 0; i < n/2; i++)
    {
        CSwap(a[i], a[n-i-1]);
    }
    for (int i = 0; i < n; i++)
        cout << a[i] << " ";
    delete[] a;
}
