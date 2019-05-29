#include <iostream>
#include <set>
#include <iomanip>
#include "cmath"


#define EPS 1e-15
#define Pi 3.1415926535

using namespace std;

struct TData {
    TData(double degree, int plus, int minus) : degree(degree), plus(plus), minus(minus) {}

    double degree;
    int plus;
    int minus;
};

ostream & operator<<(ostream & os, TData * right) {
    os << setprecision(14) << fixed << right->degree * 180 / Pi << " " << right->plus << " " << right->minus;
    return os;
}

struct CompareTData {
    bool operator()(TData* const & left, TData* const & right) const {
        if (fabs(left->degree - right->degree) < EPS) return false;
        return left->degree < right->degree;
    }
};

typedef set<TData *, CompareTData> sortedData;

class ProblemOil {
public:
    ProblemOil() {
        lines = nullptr;
        count = 0;
        size = 0;
        oilMax = 0;
    }

    ~ProblemOil() {
        if (!lines) return;
        for (int i = 0; i < size; i++) {
            delete[] lines[i];
        }
        delete[] lines;
    }

    bool read() {
        std::cin >> size;
        if (std::cin.eof()) return false;
        count = size;
        lines = new int*[size];
        for (int i = 0; i < size; i++) {
            lines[i] = new int[4];
        }

        for (int i = 0; i < count; i++) {
            std::cin >> lines[i][0] >> lines[i][1] >> lines[i][2];
            if (lines[i][0] > lines[i][1]) {
                lines[i][3] = lines[i][0];
                lines[i][0] = lines[i][1];
                lines[i][1] = lines[i][3];
            }
            lines[i][3] = lines[i][1] - lines[i][0];
            if (!lines[i][3]) {
                i--;
                count--;
            }
        }
        return true;
    }

    void run() {
        for (int i = 0; i < count; i++) {
            calculate(i, 0);
            calculate(i, 1);
        }
    }

    void test() {
        for (int i = 0; i < count; i++) {
            int *& a = lines[i];
            std::cout << setw(10) << a[0] << " " << setw(10) << a[1] << " " << setw(10) << a[2] << " " << setw(10) << a[3] << std::endl;
        }
    }

    void testSet(int iter, int pos) {
        sortedData s = calculateFromPoint(iter, pos);
        cout << "\nset test" << endl;
        for (auto item : s) {
            cout << item << endl;
        }
        destroySet(s);
    }


private:
    int count;
    int size;
    int oilMax;
    int ** lines;

    friend ostream & operator<<(ostream & os, const ProblemOil & problemOil);

    void calculate(int iter, int pos) {
        sortedData s = calculateFromPoint(iter, pos);
        int currentOil = lines[iter][3];
        if (currentOil > oilMax) {
            oilMax = currentOil;
        }
        for (auto item : s) {
            currentOil += item->plus;
            if (currentOil > oilMax) {
                oilMax = currentOil;
            }
            currentOil -= item->minus;
        }
        destroySet(s);
    }

    sortedData calculateFromPoint(int iter, int pos) {
        sortedData data;
        int x = lines[iter][pos];
        int y = lines[iter][2];
        for (int i = 0; i < count; i++) {
            if (lines[i][2] == y) continue;
            if (lines[i][2] > y) {
                if (lines[i][1] == x) {
                    auto * t = new TData(Pi / 2, lines[i][3], 0);
                    if (!data.insert(t).second) {
                        data.find(t).operator*()->plus += lines[i][3];
                        delete t;
                    }
                } else {
                    double tg = atan((lines[i][2] - y) / static_cast<double>(lines[i][1] - x));
                    auto * t = new TData((tg < 0 ? Pi + tg : tg), lines[i][3], 0);
                    if (!data.insert(t).second) {
                        data.find(t).operator*()->plus += lines[i][3];
                        delete t;
                    }
                }

                if (lines[i][0] == x) {
                    auto * t = new TData(Pi / 2, 0, lines[i][3]);
                    if (!data.insert(t).second) {
                        data.find(t).operator*()->minus += lines[i][3];
                        delete t;
                    }
                } else {
                    double tg = atan((lines[i][2] - y) / static_cast<double>(lines[i][0] - x));
                    auto * t = new TData((tg < 0 ? Pi + tg : tg), 0, lines[i][3]);
                    if (!data.insert(t).second) {
                        data.find(t).operator*()->minus += lines[i][3];
                        delete t;
                    }
                }
            } else {
                if (lines[i][0] == x) {
                    auto * t = new TData(Pi / 2, lines[i][3], 0);
                    if (!data.insert(t).second) {
                        data.find(t).operator*()->plus += lines[i][3];
                        delete t;
                    }
                } else {
                    double tg = atan((lines[i][2] - y) / static_cast<double>(lines[i][0] - x));
                    auto * t = new TData((tg < 0 ? Pi + tg : tg), lines[i][3], 0);
                    if (!data.insert(t).second) {
                        data.find(t).operator*()->plus += lines[i][3];
                        delete t;
                    }
                }

                if (lines[i][1] == x) {
                    auto * t = new TData(Pi / 2, 0, lines[i][3]);
                    if (!data.insert(t).second) {
                        data.find(t).operator*()->minus += lines[i][3];
                        delete t;
                    }
                } else {
                    double tg = atan((lines[i][2] - y) / static_cast<double>(lines[i][1] - x));
                    auto * t = new TData((tg < 0 ? Pi + tg : tg), 0, lines[i][3]);
                    if (!data.insert(t).second) {
                        data.find(t).operator*()->minus += lines[i][3];
                        delete t;
                    }
                }
            }
        }
        return data;
    }

    void destroySet(sortedData & data) {
        for (auto item : data) {
            delete item;
        }
        data.clear();
    }
};

ostream & operator<<(ostream & os, const ProblemOil & problemOil) {
    os << problemOil.oilMax;
    return os;
}





int main() {
    while (true) {
        ProblemOil problemOil;
        if (!problemOil.read()) return 0;
        //problemOil.test();
        problemOil.run();
        //problemOil.testSet(0, 1);
        cout << problemOil << endl;
    }

}

/*
7
-30 -10 10
-30 -10 20
-30 -10 30
20 50 40
10 30 -10
10 30 -20
10 30 -30

7
0 1 0
0 1 10000000
2 3 10000000
4 5 10000000
6 7 10000000
8 9 10000000
10 11 10000000

4
-1 0 0
-2 0 1000000
-1 0 500000
1 2 1000000
 */