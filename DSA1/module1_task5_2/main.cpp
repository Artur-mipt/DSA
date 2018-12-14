#include <iostream>

using namespace std;

class CStack {
public:
    CStack();
    ~CStack();

    void reSize(int newSize);
    void push(char elem);
    void pop();
    bool isEmpty();
    char top();

private:
    char* buffer;
    int head;
    int bufferSize;
    int realSize;
};

CStack::CStack()
{
    buffer = new char[100];
    bufferSize = 100;
    realSize = 0;
    head = -1;
}

CStack::~CStack()
{
    delete[] buffer;
}

void CStack::reSize(int newSize)
{
    char* newBuffer = new char[newSize];
    for (int i = 0; i < realSize; i++) {
        newBuffer[i] = buffer[i];
    }
    buffer = newBuffer;
}

void CStack::push(char elem)
{
    head++;
    buffer[head] = elem;
    realSize++;
    if (realSize == bufferSize) {
        reSize(bufferSize * 2);
    }
}

void CStack::pop()
{
    head--;
    realSize--;
    if (realSize * 4 < bufferSize) {
        reSize(realSize / 2);
    }
}

bool CStack::isEmpty()
{
    if (realSize == 0) {
        return true;
    }
    else {
        return false;
    }
}

char CStack::top()
{
    return buffer[head];
}


int main() {
    string s1, s2;
    cin >> s1 >> s2;
    int length = s1.length();
    int i1 = 0;
    int i2 = 0;
    CStack s;

    while (i1 < length && i2 < length) {
        bool flag = true; //чтобы не писать большие конструкции if - else
        if (s2[i2] == s1[i1] && flag) {
            i1++;
            i2++;
            flag = false;
        }
        if (s2[i2] != s1[i1] && s.isEmpty() && flag) {
            s.push(s1[i1]);
            i1++;
            flag = false;
        }
        if (s2[i2] != s1[i1] && !s.isEmpty() && flag) {
            if (s2[i2] == s.top()) {
                s.pop();
                i2++;
            }
            else {
                s.push(s1[i1]);
                i1++;
            }
        }
    }

    for (int i = i2; i < length; i++) {
        if (s2[i] == s.top()) {
            s.pop();
        }
        else {
            cout << "NO" ;
            return 0;
        }
    }

    if (s.isEmpty()) {
        cout << "YES" ;
    }
    else {
        cout << "NO" ;
    }

}
