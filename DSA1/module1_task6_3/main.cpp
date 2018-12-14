#include <iostream>
#include <cmath>

using namespace std;

class PriorityQueue {
public:

    PriorityQueue(int n);
    ~PriorityQueue();

    int Size();
    int Top();
    bool IsEmpty();
    void Add(int elem);
    void Pop();
    void Heapify(int i);

private:

    int realSize;
    int heapSize;
    int* buffer;
};

PriorityQueue::PriorityQueue(int n)
{
    heapSize = n;
    buffer = new int[heapSize];
    realSize = 0;
}

PriorityQueue::~PriorityQueue()
{
    delete[] buffer;
}

int PriorityQueue::Size() {
    return realSize;
}

int PriorityQueue::Top()
{
    return buffer[0];
}

bool PriorityQueue::IsEmpty()
{
    return (realSize == 0);
}

void PriorityQueue::Add(int elem)
{
    buffer[realSize] = elem;
    int parent = (realSize - 1) / 2;
    int current = realSize;

    while (current > 0 && buffer[parent] > buffer[current]) {
        int temp = buffer[parent];
        buffer[parent] = buffer[current];
        buffer[current] = temp;

        temp = parent;
        parent = (temp - 1) / 2;
    }

    realSize++;
}


void PriorityQueue::Heapify(int i) {
    int left = i * 2 + 1;
    int right = i * 2 + 2;
    int largest = i;
    if (right < realSize && buffer[right] < buffer[largest]) {
        largest = right;
    }
    if (left < realSize && buffer[left] < buffer[largest]) {
        largest = left;
    }
    if (largest != i) {
        int temp = buffer[largest];
        buffer[largest] = buffer[i];
        buffer[i] = temp;
        Heapify(largest);
    }
}

void PriorityQueue::Pop()
{
    buffer[0] = buffer[realSize - 1];
    realSize--;
    if (realSize != 0) {
        Heapify(0);
    }
}

int main()
{
    int n;
    cin >> n;

    PriorityQueue q(n);
    int ans = 0;

    for (int i = 0; i < n; i++) {
        int intime;
        int outtime;
        cin >> intime >> outtime;
        while (!q.IsEmpty() && intime > q.Top()) {
            q.Pop();
        }
        q.Add(outtime);
        if (q.Size() > ans) {
            ans = q.Size();
        }
    }

    cout << ans;
}
