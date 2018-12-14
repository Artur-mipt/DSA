#include <iostream>
#include <fstream>

using namespace std;

class CDeque {
public:

    CDeque();
    ~CDeque();

    void Resize(int newSize);

    int Front();
    void PopFront();
    void PushFront(int elem);

    int Back();
    void PopBack();
    void PushBack(int elem);

    void Write();

private:

    int* buffer;
    int bufferSize;
    int realSize;
    int head;
    int tail;

};

CDeque::CDeque()
{
    head = 0;
    tail = 0;
    bufferSize = 2;
    realSize = 0;
    buffer = new int[bufferSize];
}

CDeque::~CDeque()
{
    delete[] buffer;
}

void CDeque::Resize(int newSize)
{
    int* newBuffer = new int[newSize];

    for (int i = 0; i < realSize; i++) {
        newBuffer[i] = buffer[(head + i) % bufferSize];
    }

    buffer = newBuffer;
    bufferSize = newSize;
    head = 0;
    tail = realSize - 1;

}

int CDeque::Front()
{
    int elem = buffer[head];
    if (realSize == 0) {
        elem = -1;
    }
    return elem;
}

int CDeque::Back()
{
    int elem = buffer[tail];
    if (realSize == 0) {
        elem = -1;
    }
    return elem;
}

void CDeque::PushFront(int elem)
{
    if (bufferSize == realSize && realSize != 0) {
        Resize(bufferSize * 2);
    }

    if (realSize != 0) {
        head--;
        if (head == -1) {
            head = bufferSize - 1;
        }
        buffer[head] = elem;
        realSize++;
    }
    else {
        buffer[head] = elem;
        realSize++;
    }
}

void CDeque::PushBack(int elem)
{
    if (bufferSize == realSize && realSize != 0) {
        Resize(bufferSize * 2);
    }

    if (realSize != 0) {
        tail++;
        if (tail == bufferSize) {
            tail = 0;
        }
        buffer[tail] = elem;
        realSize++;
    }
    else {
        buffer[tail] = elem;
        realSize++;
    }
}

void CDeque::PopFront()
{
    if (realSize != 1) head++;
    if (head == bufferSize) {
        head = 0;
    }
    realSize--;

    if (realSize != 0 && realSize * 4 < bufferSize) {
        Resize(bufferSize / 2);
    }
}

void CDeque::PopBack()
{
    if (realSize != 1) tail--;
    if (tail == -1) {
        tail = bufferSize - 1;
    }
    realSize--;

    if (realSize != 0 && realSize * 4 < bufferSize) {
        Resize(bufferSize / 2);
    }
}

void CDeque::Write()
{
    for (int i = 0; i < realSize; i++) {
        cout << buffer[(head + i) % bufferSize] << " " ;
    }
    cout << endl;
    cout << buffer[head] << " " << buffer[tail] << endl;
    cout << endl;
}

int main()
{
    //ifstream cin("input.txt");
    //ofstream cout("output.txt");

    int n;
    cin >> n;
    CDeque deq;
    bool flag = false;
    for (int i = 0; i < n; i++) {
        int operation;
        int elem;
        cin >> operation >> elem;

        if (operation == 1) {
            deq.PushFront(elem);
        }
        if (operation == 2){
            int pop = deq.Front();
            if (pop != -1) {
                deq.PopFront();
            }
            if (pop != elem) {
                flag = true;
            }
        }
        if (operation == 3){
            deq.PushBack(elem);
        }
        if (operation == 4){
            int pop = deq.Back();
            if (pop != -1) {
                deq.PopBack();
            }
            if (pop != elem) {
                flag = true;
            }
        }

        //deq.Write();
    }

    if (flag) {
        cout << "NO";
    }
    else {
        cout << "YES";
    }
}
