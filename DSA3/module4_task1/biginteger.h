#include <iostream>
#include <string>
#include <vector>
using namespace std;

class BigInteger {
public:
    BigInteger();
    ~BigInteger();
    BigInteger(const string& value);
    BigInteger(int value);

    // строковое представление числа
    string toString() const;

    // Логика и порядок.
    bool operator==(const BigInteger& other) const;
    bool operator!=(const BigInteger& other) const;
    bool operator<(const BigInteger& other) const;
    bool operator<=(const BigInteger& other) const;
    bool operator>(const BigInteger& other) const;
    bool operator>=(const BigInteger& other) const;

    //	унарный оператор
    BigInteger operator-() const;

    //	pre/post increment/decrement
    BigInteger& operator++();
    BigInteger& operator--();

    BigInteger operator++(int);
    BigInteger operator--(int);

    //	бинарные операторы
    friend BigInteger operator+(const BigInteger& left, const BigInteger& right);
    friend BigInteger operator-(const BigInteger& left, const BigInteger& right);
    friend BigInteger operator*(const BigInteger& left, const BigInteger& right);
    friend BigInteger operator/(const BigInteger& left, const BigInteger& right);
    friend BigInteger operator%(const BigInteger& left, const BigInteger& right);

    //	составные операторы
    BigInteger& operator+=(const BigInteger& other);
    BigInteger& operator-=(const BigInteger& other);
    BigInteger& operator*=(const BigInteger& other);
    BigInteger& operator/=(const BigInteger& other);
    BigInteger& operator%=(const BigInteger& other);

    //	преобразование типов
    explicit operator bool() const;

    // ввод и вывод
    friend ostream& operator<<(ostream &stream, const BigInteger &value);
    friend istream& operator>>(istream &stream, BigInteger &value);

private:
    vector <int> num;  // разряды числа
    int sign;  // sign == 1 <=> число > 0, 0 <=> число == 0, -1 <=> число < 0

    // сумма положительных чисел
    friend BigInteger positive_sum(const BigInteger& left, const BigInteger& right);
    // разность положительных чисел (left >= right)
    friend BigInteger positive_sub(const BigInteger& left, const BigInteger& right);
    // умножение на цифру
    friend BigInteger mul_on_num(const BigInteger& left, const int num);
    // умножение на степень 10
    friend BigInteger mul_on_grade_ten(const BigInteger& left, const int deg);
    // модуль числа
    friend BigInteger abs(const BigInteger& number);

};

BigInteger::BigInteger() {
    num.push_back(0);
    sign = 0;
}

BigInteger::~BigInteger() {
    num.clear();
}

BigInteger::BigInteger(const string &value) {
    if (value == "-0") {
        sign = 0;
    } else {
        if (value.length() == 1 && value[0] == '0') {
            sign = 0;
        } else {
            sign = (value[0] == '-') ? -1 : 1;
        }
    }

    for (int i = value.length() - 1; i >= 0; i--) {
        if (value[i] != '-') {
            num.push_back(int(value[i]) - 48);
        }
    }
}

BigInteger::BigInteger(int value) {
    sign = (value == 0) ? 0 : 1;
    if (value < 0) {
        sign = -1;
        value *= -1;
    }

    if (value == 0) {
        num.push_back(0);
    } else {
        while (value != 0) {
            num.push_back(value % 10);
            value /= 10;
        }
    }
}

string BigInteger::toString() const {
    string s;
    if (sign == 0) {
        return "0";
    }

    if (sign < 0) {
        s += '-';
    }
    for (int i = num.size() - 1; i >= 0; i--) {
        s += char(num[i] + 48);
    }

    return s;
}

bool BigInteger::operator==(const BigInteger &other) const {
    if (other.sign == 0 && this->sign == 0) {
        return true;
    }

    if (sign != other.sign || num.size() != other.num.size()) {
        return false;
    }
    for (size_t i = 0; i < num.size(); i++) {
        if (num[i] != other.num[i]) {
            return false;
        }
    }
    return true;
}

bool BigInteger::operator!=(const BigInteger &other) const {
    return !(*this == other);
}

bool BigInteger::operator<(const BigInteger &other) const {
    if (sign < other.sign) {
        return true;
    }
    if (sign > other.sign) {
        return false;
    }

    if (sign == 0 && other.sign == 0) {
        return false;
    }

    if (num.size() < other.num.size()) {
        return sign >= 0;
    }

    if (num.size() > other.num.size()) {
        return sign < 0;
    }

    for (int i = int(num.size()) - 1; i >= 0; i--) {
        if (num[i] < other.num[i]) {
            return (sign != -1);
        }
        if (num[i] > other.num[i]) {
            return (sign == -1);
        }
    }

    return false;
}


bool BigInteger::operator<=(const BigInteger &other) const {
    return (*this < other) || (*this == other);
}

bool BigInteger::operator>(const BigInteger &other) const {
    return !(*this <= other);
}

bool BigInteger::operator>=(const BigInteger &other) const {
    return !(*this < other);
}


BigInteger positive_sum(const BigInteger &left, const BigInteger &right) {
    if (left.sign == 0) {
        return right;
    }
    if (right.sign == 0) {
        return left;
    }

    BigInteger ans;
    ans.num.resize(0);
    ans.sign = 1;

    int max_discharges = 0;  // максимальное число разрядов из left и right
    if (left.num.size() > right.num.size()) {
        max_discharges = left.num.size();
    } else {
        max_discharges = right.num.size();
    }

    int base = 0;  // перенос
    for (int i = 0; i < max_discharges; i++) {
        int right_num = (size_t(i) >= right.num.size()) ? 0 : right.num[i];
        int left_num = (size_t(i) >= left.num.size()) ? 0 : left.num[i];
        ans.num.push_back((base + right_num + left_num) % 10);
        base = (right_num + left_num + base > 9) ? 1 : 0;
    }
    if (base > 0) {
        ans.num.push_back(1);
    }

    return ans;
}

BigInteger positive_sub(const BigInteger &left, const BigInteger &right) {
    int carry = 0;  // перенос
    BigInteger ans;
    ans.num.resize(0);
    for (size_t i = 0; i < left.num.size(); i++) {
        int left_num = left.num[i];
        int right_num = (size_t(i) >= right.num.size()) ? 0 : right.num[i];
        carry += left_num - right_num + 10;
        ans.num.push_back(carry % 10);
        carry = (carry < 10) ? -1 : 0;
    }

    int ind = ans.num.size() - 1;
    while(ans.num[ind] == 0 && ind > 0) {
        ind--;
    }
    ans.num.resize(ind + 1);

    // если результат > 0
    if (!(ans.num.size() == 1 && ans.num[0] == 0)) {
        ans.sign = 1;
    }

    return ans;
}

BigInteger BigInteger::operator-() const {
    BigInteger ans(*this);
    ans.sign = -1 * this->sign;

    return ans;
}

BigInteger& BigInteger::operator++() {
    *this = *this + 1;
    return *this;
}

BigInteger& BigInteger::operator--() {
    *this = *this - 1;
    return *this;
}

BigInteger BigInteger::operator++(int) {
    *this = *this + 1;
    return *this - 1;
}

BigInteger BigInteger::operator--(int) {
    *this = *this - 1;
    return *this + 1;
}

BigInteger abs(const BigInteger &number) {
    if (number.sign >= 0) {
        return number;
    } else {
        return -number;
    }
}

BigInteger operator+(const BigInteger& left, const BigInteger &right) {
    BigInteger ans;
    if (left.sign >= 0 && right.sign >= 0) {
        ans = positive_sum(left, right);
        return ans;
    }
    if (left.sign < 0 && right.sign < 0) {
        ans = positive_sum(left, right);
        ans.sign = (ans.sign == 0) ? 0 : -1;
        return ans;
    }
    if (left.sign < 0) {
        if (right > abs(left)) {
            ans = positive_sub(right, left);
            return ans;
        } else {
            ans = positive_sub(left, right);
            ans.sign = (ans.sign == 0) ? 0 : -1;
            return ans;
        }
    } else {
        if (left > -right) {
            ans = positive_sub(left, right);
            return ans;
        } else {
            ans = positive_sub(right, left);
            ans.sign = (ans.sign == 0) ? 0 : -1;
            return ans;
        }
    }
}

BigInteger operator-(const BigInteger& left, const BigInteger &right) {
    return left + (-right);
}

// умножение длинного числа на цифру
BigInteger mul_on_num(const BigInteger& left, const int num) {
    int base = 0;  // перенос
    BigInteger ans;
    ans.num.resize(0);
    ans.sign = 1;
    for (size_t i = 0; i < left.num.size(); i++) {
        int tmp = left.num[i] * num + base;
        ans.num.push_back(tmp % 10);
        base = tmp / 10;
    }
    if (base != 0) {
        ans.num.push_back(base);
    }

    return ans;
}

// умножение на степень 10
BigInteger mul_on_grade_ten(const BigInteger& left, const int deg) {
    BigInteger ans(left);
    for (int i = 0; i < deg; i++) {
        ans.num.insert(ans.num.begin(), 0);
    }

    return ans;
}

BigInteger operator*(const BigInteger& left, const BigInteger &right) {
    BigInteger ans(0);
    ans.sign = 1;
    for (size_t j = 0; j < right.num.size(); j++) {
        BigInteger tmp = mul_on_num(left, right.num[j]);
        if (j != 0) {
            tmp = mul_on_grade_ten(tmp, j);
        }
        ans = ans + tmp;
    }
    ans.sign = left.sign * right.sign;

    return ans;
}

BigInteger operator/(const BigInteger& left, const BigInteger &right) {
    if (right.sign == 0) {
        return left;
    }

    if (abs(left) < abs(right)) {
        return BigInteger(0);
    }

    BigInteger copy_left(abs(left));
    BigInteger copy_right(abs(right));

    BigInteger ans(0);

    while (copy_left >= copy_right) {
        BigInteger tmp;
        tmp.num.resize(0);
        tmp.sign = 1;
        // записываем в tmp число, большее right, составленное из старших разрядов left
        for (size_t i = 0; i < copy_right.num.size(); i++) {
            tmp.num.push_back(copy_left.num[i + (copy_left.num.size() - copy_right.num.size())]);
        }
        if (tmp < copy_right) {
            tmp.num.insert(tmp.num.begin(), copy_left.num[(copy_left.num.size() - copy_right.num.size()) - 1]);
        }

        bool good_num = false;  // нашлась ли нужная цифра
        for (int i = 9; i >= 1; i--) {
            BigInteger big_i(i);
            if (copy_right * big_i <= tmp && !good_num) {
                BigInteger to_ans = mul_on_grade_ten(big_i, copy_left.num.size() - tmp.num.size());
                ans = ans + to_ans;
                copy_left = copy_left - to_ans * copy_right;
                good_num = true;
            }
        }
    }

    ans.sign = left.sign * right.sign;

    return ans;
}

BigInteger operator%(const BigInteger& left, const BigInteger &right) {
    return left - (left / abs(right)) * abs(right);
}

BigInteger& BigInteger::operator+=(const BigInteger &other) {
    *this = *this + other;
    return *this;
}

BigInteger& BigInteger::operator-=(const BigInteger &other) {
    *this = *this - other;
    return *this;
}

BigInteger& BigInteger::operator*=(const BigInteger &other) {
    *this = *this * other;
    return *this;
}

BigInteger& BigInteger::operator/=(const BigInteger &other) {
    *this = *this / other;
    return *this;
}

BigInteger& BigInteger::operator%=(const BigInteger &other) {
    *this = *this % other;
    return *this;
}

BigInteger::operator bool() const {
    return (this->sign != 0);
}

istream& operator>>(std::istream &stream, BigInteger &value) {
    string s;
    stream >> s;
    BigInteger tmp(s);
    value = tmp;
    return stream;
}

ostream& operator<<(std::ostream &stream, const BigInteger &value) {
    stream << value.toString();
    return stream;
}