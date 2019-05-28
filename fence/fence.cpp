#include <iostream>
#include <cmath>


struct TTree {
    TTree() : x(0), y(0), value(0), length(0) {}

    int x;
    int y;
    int value;
    int length;

    double distance(const TTree & tree) const {
        return sqrt(std::pow(this->x - tree.x, 2) + pow(this->y - tree.y, 2));
    }
};

std::istream & operator>>(std::istream & is, TTree & tree) {
    is >> tree.x >> tree.y >> tree.value >> tree.length;
    return is;
}

class ProblemFence {
public:
    ProblemFence() {
        tree = nullptr;
        counter = 0;
        lostValue = 0x0fffffff;
        maxBit = 1;
        mask = 0;
        currentLost = 0;
        currentLength = 0;
    }

    ~ProblemFence() {
        delete[] tree;
    }

    bool read() {
        std::cin >> counter;
        if (!counter) return false;
        tree = new TTree[counter];
        maxBit <<= counter;
        for (int i = 0; i < counter; i++) {
            std::cin >> tree[i];
        }
        return true;
    }

    void run() {
        if (counter == 2) {
            lostValue = std::min(tree[0].value, tree[1].value);
            return;
        }

        while (!((++mask) & maxBit)) {
            if (!calculateLostValue()) continue;
            double length = 0;
            for (int i = 0; i < counter - 1; i++) {
                if (mask & (1 << i)) continue;
                for (int j = i + 1; j < counter; j++) {
                    if (mask & (1 << j)) continue;
                    if (checkTree(tree[i], tree[j])) {
                        length += tree[i].distance(tree[j]);
                    }
                    if (checkTree(tree[j], tree[i])) {
                        length += tree[i].distance(tree[j]);
                    }
                    if (length > currentLength) {
                        i = counter;
                        break;
                    }
                }
            }
            if (length <= currentLength) lostValue = currentLost;
        }
    }

private:
    TTree * tree;
    int counter;
    int lostValue;
    int maxBit;
    int mask;
    int currentLost;
    int currentLength;

    bool checkTree(const TTree & first, const TTree & second) {
        for (int i = 0; i < counter; i++) {
            if (!((1 << i) & mask)) {
                int prod = ((tree[i].x - first.x) * (second.y - first.y)) - ((second.x - first.x) * (tree[i].y - first.y));
                if (prod > 0) continue;
                if (prod < 0) return false;
                if (tree[i].x == first.x) {
                    if ((first.y < tree[i].y && tree[i].y < second.y) || (first.y > tree[i].y && tree[i].y > second.y)) return false;
                } else {
                    if ((first.x < tree[i].x && tree[i].x < second.x) || (first.x > tree[i].x && tree[i].x > second.x)) return false;
                }
            }
        }
        return true;
    }

    bool calculateLostValue() {
        currentLost = currentLength = 0;
        for (int i = 0; i < counter; i++) {
            if (mask & (1 << i)) {
                currentLost += tree[i].value;
                currentLength += tree[i].length;
            }
        }
        return (currentLost < lostValue);
    }

    friend std::ostream &operator<<(std::ostream & os, const ProblemFence & problemFence);
};

std::ostream &operator<<(std::ostream & os, const ProblemFence & problemFence) {
    os << "The lost value is " << problemFence.lostValue << ".";
    return os;
}


int main() {
    while (true) {
        ProblemFence problemFence;
        if (!problemFence.read()) return 0;
        problemFence.run();
        std::cout << problemFence << std::endl;
    }
}


/*

6
0 0 8 3
1 4 3 2
2 1 7 1
4 1 2 3
3 5 4 6
2 3 9 8
3
3 0 10 3
5 -3 20 25
7 -3 30 32
2
100 0 5 4
0 100 4 5
5
0 0 10 10
0 1 10 10
1 0 10 10
1 1 10 10
50 50 8 4
0












 */