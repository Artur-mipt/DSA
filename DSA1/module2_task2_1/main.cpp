// В супермаркете решили оптимизировать показ рекламы. Известно расписание прихода и ухода покупателей
// (два целых числа). Каждому покупателю необходимо показать минимум 2 рекламы. Рекламу можно транслировать
// только в целочисленные моменты времени. Покупатель может видеть рекламу от момента прихода до момента
// ухода из магазина. В каждый момент времени может показываться только одна реклама. Считается, что реклама
// показывается мгновенно. Если реклама показывается в момент ухода или прихода, то считается, что посетитель
// успел её посмотреть.
// Требуется определить минимальное число показов рекламы.

#include <iostream>
// #include <fstream>

using namespace std;

// структура для хранения времени прихода и выхода каждого клиента
struct CClient {
    int inTime;
    int outTime;
};

inline bool operator>( const CClient& a, const CClient& b )
{
    if( a.outTime > b.outTime ) {
        return true;
    }
    if ( a.outTime == b.outTime && a.inTime < b.inTime ) {
        return true;
    }
    return false;
}

// реализуем кучу, чтобы с помощью нее сортировать клиентов
class CHeap {
public:

    CHeap(int n);
    ~CHeap();

    int Size();
    CClient Top();
    void Add(CClient elem);
    void Pop();
    void SiftDown(int i);

private:

    int realSize;
    int heapSize;
    CClient* arr;
};

// конструктор
CHeap::CHeap(int n)
{
    heapSize = n;
    arr = new CClient[heapSize];
    realSize = 0;
}

// деструктор
CHeap::~CHeap()
{
    delete[] arr;
}

// возвращает кол-во элементов в куче
int CHeap::Size()
{
    return realSize;
}

// возвращение корневого элемента
CClient CHeap::Top()
{
    return arr[0];
}

// добавление элемента с учетом проталкивания вверх (SiftUp)
void CHeap::Add(CClient elem)
{
    arr[realSize].inTime = elem.inTime;
    arr[realSize].outTime = elem.outTime;
    int parent = (realSize - 1) / 2;
    int current = realSize;

    while( current > 0 && ( arr[parent] > arr[current] ) ) {
        CClient temp = arr[parent];
        arr[parent] = arr[current];
        arr[current] = temp;

        current = parent;
        parent = (current - 1) / 2;
    }

    realSize++;
}

// проталкивание i-го элемента вниз
void CHeap::SiftDown( int i )
{
    int left = i * 2 + 1;
    int right = i * 2 + 2;

    // minn - индекс наименьшего сына
    int minn = i;
    if ( right < realSize && ( arr[minn] > arr[right] ) ) {
        minn = right;
    }

    if( left < realSize && ( arr[minn] > arr[left] ) ) {
        minn = left;
    }

    // если наименьший сын нашелся
    if( minn != i ) {
        CClient temp = arr[minn];
        arr[minn] = arr[i];
        arr[i] = temp;
        SiftDown(minn);
    }
}

// удаление корневого элемента из кучи
void CHeap::Pop()
{
    arr[0] = arr[realSize - 1];
    realSize--;
    if (realSize != 0) {
        SiftDown(0);
    }
}

int main()
{
    // ifstream cin("input.txt");
    // ofstream cout("output.txt");
    int n;
    cin >> n;

    CHeap heap(n);

    for( int i = 0; i < n; i++ ) {
        CClient a;
        cin >> a.inTime >> a.outTime;
        heap.Add(a);
    }


    // ans - кол-во показов рекламы всего, last1 и last2 - время двух последних показов
    CClient temp = heap.Top();
    heap.Pop();
    int ans = 2;
    int last1 = temp.outTime - 1;
    int last2 = temp.outTime;

    while( heap.Size() > 0 ) {
        // достаем корневой элемент
        CClient temp = heap.Top();
        heap.Pop();

        // показываем ему рекламу
        bool flag = true;
        if( temp.inTime > last2 && flag ) {
            last1 = temp.outTime - 1;
            last2 = temp.outTime;
            ans = ans + 2;
            flag = false;
        }
        if( temp.inTime <= last2 && temp.inTime > last1 && flag ) {
            last1 = last2;
            last2 = temp.outTime;
            ans = ans + 1;
            flag = false;
        }
    }

    cout << ans;
}
