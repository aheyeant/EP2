#include <iostream>
#include <vector>
#include <exception>
#include <iomanip>
#include <math.h>

#define BASE_ZERO 3

class BigInteger {
public:
    /**
     * Constructor from int.
     * @param number
     */
    explicit BigInteger(int number = 0);

    /**
     * Copy constructor.
     * @param integer
     */
    BigInteger(const BigInteger & integer, bool changeMinus = false);

    /**
     * Calculate binomial coefficient.
     * @param A
     * @param B
     * @return (A + B)! / (A! * B!)
     */
    static const BigInteger calculateC(int A, int B);

    static const BigInteger abs(const BigInteger & integer);

    BigInteger & operator=(const BigInteger & right);

    BigInteger & operator+=(const BigInteger & right);

    BigInteger & operator-=(const BigInteger & right);

    BigInteger & operator*=(const BigInteger & right);

    BigInteger & operator/=(const BigInteger & right);

    friend const BigInteger operator+(const BigInteger & integer);

    friend const BigInteger operator-(const BigInteger & integer);

    friend const BigInteger operator+(const BigInteger & left, const BigInteger & right);

    friend const BigInteger operator-(const BigInteger & left, const BigInteger & right);

    friend const BigInteger operator*(const BigInteger & left, const BigInteger & right);

    friend const BigInteger operator/(const BigInteger & left, const BigInteger & right);

    friend const bool operator< (const BigInteger & left, const BigInteger & right);

    friend const bool operator<=(const BigInteger & left, const BigInteger & right);

    friend const bool operator> (const BigInteger & left, const BigInteger & right);

    friend const bool operator>=(const BigInteger & left, const BigInteger & right);

    friend const bool operator!=(const BigInteger & left, const BigInteger & right);

    friend const bool operator==(const BigInteger & left, const BigInteger & right);

    friend std::ostream & operator<<(std::ostream & os, const BigInteger & integer);

private:
    /**
     * Digits in vector: low digit in vector[0], high digit in vector[end].
     */
    std::vector<int> _digits;
    const int base = static_cast<int>(pow(10, BASE_ZERO));
    bool _isMinus;

    void removeForwardZeros();

    bool absLess(const BigInteger & left, const BigInteger & right) const;

    bool absEquals(const BigInteger & left, const BigInteger & right) const;
};

BigInteger::BigInteger(int number) {
    if (number < 0) {
        _isMinus = true;
        number = -number;
    } else {
        _isMinus = false;
    }
    while (number >= base) {
        _digits.push_back(number % base);
        number /= base;
    }
    if (number > 0) {
        _digits.push_back(number);
    }
    if (_digits.empty()) {
        _digits.push_back(0);
    }
}

BigInteger::BigInteger(const BigInteger &integer, bool changeMinus) {
    changeMinus ? (_isMinus = !integer._isMinus) : (_isMinus = integer._isMinus);
    for (int item : integer._digits) {
        _digits.push_back(item);
    }
}

const BigInteger BigInteger::calculateC(int A, int B) {
    if (A == 0 && B == 0) return BigInteger(1);
    if (A == 0) return BigInteger(1);
    if (B == 0) return BigInteger(1);
    int down = std::min(A, B);
    int r = A + B, l = r - down + 1;
    int d[down], u[down];
    for (int i = 0; i < down; i++) {
        u[i] = l + i;
        d[i] = i + 1;
    }
    for (int i = down - 1; i >= 0; i--) {
        if (d[i] == 1) continue;

        for (int j = down - 1; j >= 0; j--) {
            if (u[j] == 1) continue;
            if (u[j] >= d[i]) {
                if (u[j] % d[i] == 0) {
                    u[j] /= d[i];
                    d[i] = 1;
                    j = 0;
                }
            } else if (d[i] % u[j] == 0) {
                d[i] /= u[j];
                u[j] = 1;
                if (d[i] == 1) j = 0;
            }
        }
    }
    BigInteger result(1);
    for (int i = 0; i < down; i++) {
        if (u[i] > 1) {
            result *= BigInteger(u[i]);
        }
    }
    for (int i = 0; i < down; i++) {
        if (d[i] > 1) {
            result /= BigInteger(d[i]);
        }
    }
    return result;
}

void BigInteger::removeForwardZeros() {
    while (_digits.size() > 1 && _digits.back() == 0) {
        _digits.pop_back();
    }
}

const BigInteger BigInteger::abs(const BigInteger & integer) {
    return BigInteger(integer, integer._isMinus);
}

bool BigInteger::absLess(const BigInteger &left, const BigInteger &right) const {
    if (left._digits.size() < right._digits.size()) return true;
    if (left._digits.size() > right._digits.size()) return false;
    for (long int i = left._digits.size() - 1; i >= 0; i--) {
        if (left._digits[i] < right._digits[i]) return true;
        if (left._digits[i] > right._digits[i]) return false;
    }
    return false;
}

bool BigInteger::absEquals(const BigInteger &left, const BigInteger &right) const {
    if (left._digits.size() != right._digits.size()) return false;
    for (long int i = left._digits.size() - 1; i >= 0; i--) {
        if (left._digits[i] != right._digits[i]) return false;
    }
    return true;
}

BigInteger & BigInteger::operator=(const BigInteger &right) {
    this->_isMinus = right._isMinus;
    this->_digits.clear();
    for (auto item : right._digits) {
        this->_digits.push_back(item);
    }
    return *this;
}

BigInteger & BigInteger::operator+=(const BigInteger & right) {
    *this = *this + right;
    return *this;
}

BigInteger & BigInteger::operator-=(const BigInteger & right) {
    *this = *this - right;
    return *this;
}

BigInteger & BigInteger::operator*=(const BigInteger & right) {
    *this = (*this) * right;
    return *this;
}

BigInteger & BigInteger::operator/=(const BigInteger & right) {
    *this = *this / right;
    return *this;
}

const BigInteger operator+(const BigInteger &left, const BigInteger &right) {
    if (left._isMinus) {
        if (right._isMinus) return -(-left + -right);
        return right - (-left);
    } else {
        if (right._isMinus) return left - (-right);
    }
    int carry = 0;
    BigInteger result;
    result._digits.clear();
    for (int i = 0; i < std::max(left._digits.size(), right._digits.size()) || carry; i++) {
        int current = (i < left._digits.size() ? left._digits[i] : 0) +
                      (i < right._digits.size() ? right._digits[i] : 0) +
                      carry;
        carry = current >= left.base;
        result._digits.push_back(current % left.base);
    }
    return result;
}

const BigInteger operator-(const BigInteger &left, const BigInteger &right) {
    if (left._isMinus) {
        if (right._isMinus) return -((-left) - (-right));
        return -((-left) + right);
    } else {
        if (right._isMinus) return left + (-right);
        if (left < right) return -(right - left);
    }
    int carry = 0;
    BigInteger result;
    result._digits.clear();
    for (long int i = 0; i < left._digits.size() || carry; i++) {
        int current = (i < left._digits.size() ? left._digits[i] : 0) -
                      (i < right._digits.size() ? right._digits[i] : 0) -
                      carry;
        carry = current < 0;
        carry ? result._digits.push_back(current + left.base) : result._digits.push_back(current);
    }
    result.removeForwardZeros();
    return result;
}

const BigInteger operator*(const BigInteger &left, const BigInteger &right) {
    BigInteger result;
    result._digits.resize(left._digits.size() + right._digits.size());
    for (long int i = 0; i < left._digits.size(); i++) {
        int carry = 0;
        for (long int j = 0; j < right._digits.size() || carry; j++) {
            long int current = result._digits[i + j] + carry + left._digits[i] * (j < right._digits.size() ? right._digits[j] : 0);
            carry = static_cast<int>(current / left.base);
            result._digits[i + j] = static_cast<int>(current % left.base);
        }
    }
    result.removeForwardZeros();
    result._isMinus = left._isMinus != right._isMinus;
    return result;
}

const BigInteger operator/(const BigInteger &left, const BigInteger &right) {
    if (left.absLess(left, right)) return BigInteger(0);
    if (left.absEquals(left, right)) return BigInteger(left._isMinus != right._isMinus ? -1 : 1);
    if (right.absEquals(right, BigInteger(1))) return BigInteger(left, right._isMinus);
    BigInteger upPart;
    upPart._digits.clear();
    BigInteger result;
    result._digits.clear();
    BigInteger div = BigInteger::abs(right);
    for (long int i = left._digits.size() - 1; i >= 0; i--) {
        upPart._digits.emplace(upPart._digits.begin(), left._digits[i]);
        upPart.removeForwardZeros();
        BigInteger tmp = div;
        int k = 0;
        while (upPart >= tmp && k < left.base) {
            k++;
            tmp = BigInteger(k + 1) * div;
        }
        upPart = upPart - (BigInteger(k) * div);
        result._digits.emplace(result._digits.begin(), k);
        result.removeForwardZeros();
    }
    result._isMinus = left._isMinus != right._isMinus;
    return result;
}

const BigInteger operator+(const BigInteger & integer) {
    return BigInteger(integer);
}

const BigInteger operator-(const BigInteger & integer) {
    return BigInteger(integer, true);
}

const bool operator< (const BigInteger & left, const BigInteger & right) {
    if (left._isMinus) {
        if (!right._isMinus) return true;
    } else {
        if (right._isMinus) return false;
    }
    if (left._digits.size() > right._digits.size()) return false;
    if (left._digits.size() < right._digits.size()) return true;
    for (long int i = left._digits.size() - 1; i >= 0; i--) {
        if (left._digits[i] > right._digits[i]) return false;
        if (left._digits[i] < right._digits[i]) return true;
    }
    return false;
}

const bool operator<=(const BigInteger & left, const BigInteger & right) {
    return !(left > right);
}

const bool operator> (const BigInteger & left, const BigInteger & right) {
    if (left._isMinus) {
        if (!right._isMinus) return false;
    } else {
        if (right._isMinus) return true;
    }
    if (left._digits.size() > right._digits.size()) return true;
    if (left._digits.size() < right._digits.size()) return false;
    for (long int i = left._digits.size() - 1; i >= 0; i--) {
        if (left._digits[i] > right._digits[i]) return true;
        if (left._digits[i] < right._digits[i]) return false;
    }
    return false;
}

const bool operator>=(const BigInteger & left, const BigInteger & right) {
    return !(left < right);
}

const bool operator!=(const BigInteger & left, const BigInteger & right) {
    if (left._isMinus != right._isMinus) return true;
    return left._digits != right._digits;
}

const bool operator==(const BigInteger & left, const BigInteger & right) {
    if (left._isMinus != right._isMinus) return false;
    return left._digits == right._digits;
}

std::ostream & operator<<(std::ostream & os, const BigInteger & integer) {
    if (integer._isMinus) {
        os << "-";
    }
    os << integer._digits.back();
    char oldFill = os.fill('0');
    for (long int i = integer._digits.size() - 2; i >= 0; i--) {
        os << std::setw(BASE_ZERO) << integer._digits[i];
    }
    os.fill(oldFill);
    return os;
}


class Parser {
public:
    explicit Parser() {
        start = finish = 0;
        integer = BigInteger(0);
        length = 0;
    }

    void read() {
        if (scanf("%d %d", &start, &finish) != 2) {
            std::cout << "read error\n";
        }
    }

    void run() {
        auto item = getAB(getXY(start), getXY(finish));
        length = item.first + item.second;
        integer = BigInteger::calculateC(item.first, item.second);
    }

    bool isEnd() {
        return start == 0 && finish == 0;
    }

private:
    friend std::ostream & operator<< (std::ostream & os, Parser const & parser);
    int start;
    int finish;
    int length;
    BigInteger integer;

    /**
     * @return <x, y>
     */
    std::pair<int, int> getXY(int num) {
        if (num == 1) {
            return std::make_pair(0, 0);
        }
        int n = 2;
        int circle = 1;
        while (num >= n + 6 * circle) {
            n += 6 * circle++;
        }
        int position = num - n;
        int y, x;

        if (position < circle) {
            x = -1 - position;
            y = -circle;
        } else {
            position -= circle;
            if (position < circle) {
                x = -circle;
                y = -circle + 1 + position;
            } else {
                position -= circle;
                if (position < circle) {
                    x = -circle + 1 + position;
                    y = position + 1;
                } else {
                    position -= circle;
                    if (position < circle) {
                        x = position + 1;
                        y = circle;
                    } else {
                        position -= circle;
                        if (position < circle) {
                            x = circle;
                            y = circle - 1 - position;
                        } else {
                            position -= circle;
                            if (position < circle) {
                                x = circle - 1 - position;
                                y = -1 - position;
                            } else {
                                std::cout << "ERROR in yy y\n";
                                return std::make_pair(0, 0);
                            }
                        }
                    }
                }
            }
        }
        return std::make_pair(x, y);
    }

    std::pair<int, int> getAB(std::pair<int, int> point1, std::pair<int, int> point2) {
        int dx = point1.first - point2.first;
        int dy = point1.second - point2.second;

        if ((dx >= 0 && dy >= 0) || (dx < 0 && dy < 0)) {
            dx = std::abs(dx);
            dy = std::abs(dy);
            return std::make_pair(std::min(dx, dy), dx + dy - 2 * std::min(dx, dy));
        }
        return std::make_pair(std::abs(dx), std::abs(dy));
    }
};

std::ostream & operator<< (std::ostream & os, Parser const & parser) {
    if (parser.integer == BigInteger(1)) {
        os << "There is ";
        os << parser.integer;
        os << " route of the shortest length ";
    } else {
        os << "There are ";
        os << parser.integer;
        os << " routes of the shortest length ";
    }
    os << parser.length;
    os << ".\n";
    return os;
}

int main() {
    Parser parser;
    while(true) {
        parser.read();
        if (parser.isEnd()) {
            return 0;
        }
        parser.run();
        std::cout << parser;
    }
}