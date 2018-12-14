#include "biginteger.h"

bool test(int i, int j, BigInteger big_i, BigInteger big_j) {
    if ((i < j) != (big_i < big_j)) return false;
    if ((i <= j) != (big_i <= big_j)) return false;
    if ((i == j) != (big_i == big_j)) return false;
    if ((i > j) != (big_i > big_j)) return false;
    if ((i >= j) != (big_i >= big_j)) return false;
    if ((i != j) != (big_i != big_j)) return false;

    if (j != 0) {
        if (BigInteger(i - j) != (big_i - big_j)) return false;
        if (BigInteger(i + j) != (big_i + big_j)) return false;
        if (BigInteger(i * j) != (big_i * big_j)) return false;
        if (BigInteger(i / j) != (big_i / big_j)) return false;
        if (BigInteger(i % j) != (big_i % big_j)) return false;
    }

    if (BigInteger(i++) != big_i++) return false;
    if (BigInteger(++i) != ++big_i) return false;
    if (BigInteger(i--) != big_i--) return false;
    if (BigInteger(--i) != --big_i) return false;

    if (j != 0) {
        i += j;
        big_i += big_j;
        i *= j;
        big_i *= j;
        i %= j;
        big_i %= big_j;

        if (BigInteger(i) != big_i) return false;
    }

    return true;
}

int main() {
    bool flag = true;
    for (int i = -1000; i <= 1000; i++) {
        for (int j = -1000; j <= 1000; j++) {
            BigInteger big_i(i);
            BigInteger big_j(j);

            flag = (flag && test(i, j, big_i, big_j));
        }
    }

    cout << flag;
}