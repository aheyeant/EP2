#include <iostream>
#include <cmath>

class Point {
public:
    Point(double x, double y) : x(x), y(y) {}

    double getSquareDistance(const Point * point) {
        return pow(x - point->x, 2) + pow(y - point->y, 2);
    }

private:
    double x;
    double y;
};

class Heep {
public:
    explicit Heep() : count(0) {
        this->max_size = 10;
        field = new TItem[this->max_size + 1];
    }

    ~Heep() {
        delete[] this->field;
    }

    void insert(double num, int elem) {
        this->field[++this->count] = TItem(num, elem);
        resize();
        bubbleUp(this->count);
    }

    std::pair<double, int> extractMin() {
        std::pair<double, int> tmp = std::make_pair(field[1].num, field[1].elem);
        field[1] = field[this->count--];
        bubbleDown(1);
        return tmp;
    }

    bool isEmpty() {
        return count == 0;
    }

private:
    struct TItem;
    TItem * field;
    int count;
    int max_size;

    struct TItem {
        explicit TItem(double num = 0, int elem = 0) {
            this->num = num;
            this->elem = elem;
        }

        bool operator<(const TItem & item) {
            return this->num < item.num;
        }

        bool operator<=(const TItem & item) {
            return this->num <= item.num;
        }

        double num;
        int elem;
    };

    void bubbleUp(int iter) {
        if (iter == 1) return;
        int father = iter / 2;
        if (field[iter] < field[father]) {
            TItem tmp = field[iter];
            field[iter] = field[father];
            field[father] = tmp;
            bubbleUp(father);
        }
    }

    void bubbleDown(int iter) {
        int left_son = iter * 2;
        if (left_son > count) return;
        if (left_son == count) {
            if (field[left_son] < field[iter]) {
                TItem tmp = field[iter];
                field[iter] = field[left_son];
                field[left_son] = tmp;
            }
            return;
        }
        if (field[left_son] <= field[left_son + 1]) {
            if (field[left_son] < field[iter]) {
                TItem tmp = field[iter];
                field[iter] = field[left_son];
                field[left_son] = tmp;
                bubbleDown(left_son);
            }
        } else {
            if (field[left_son + 1] < field[iter]) {
                TItem tmp = field[iter];
                field[iter] = field[left_son + 1];
                field[left_son + 1] = tmp;
                bubbleDown(left_son + 1);
            }
        }
    }

    void resize() {
        if (this->count == this->max_size) {
            this->max_size *= 2;
            auto * tmp = new TItem[this->max_size + 1];
            for (int i = 0; i <= this->count; i++) {
                tmp[i] = this->field[i];
            }
            delete[] this->field;
            this->field = tmp;
        }
    }
};


class FrecklesProblem {
public:
    explicit FrecklesProblem() {
        this->points = nullptr;
        this->heep = nullptr;
        this->status = nullptr;
        this->count = 0;
        this->result = 0;
    }

    ~FrecklesProblem() {
        if (this->points) {
            for (int i = 0; i < this->count; i++) {
                delete this->points[i];
            }
        }
        delete[] this->points;
        delete[] this->status;
        delete this->heep;
    }

    void readSequence() {
        if (scanf("%d", &this->count) != 1) {
            throw "Invalid argument";
        }
        if (!this->count)
            return;
        this->points = new Point*[this->count];
        this->status = new bool[this->count];
        this->heep = new Heep();
        double x, y;
        for (int i = 0; i < this->count; i++) {
            if(scanf("%lf %lf", &x, &y) != 2) {
                throw "Invalid argument";
            }
            this->points[i] = new Point(x, y);
            this->status[i] = true;
        }
    }

    double getResult() {
        return this->result;
    }

    void run() {
        heep->insert(0, 0);
        int free_points = this->count;
        while (!heep->isEmpty() && free_points) {
            auto item = heep->extractMin();
            if (!this->status[item.second]) continue;
            this->status[item.second] = false;
            result += item.first == 0 ? 0 : sqrt(item.first);
            free_points--;
            for (int i = 0; i < this->count; i++) {
                if (this->status[i]) {
                    heep->insert(this->points[item.second]->getSquareDistance(this->points[i]), i);
                }
            }
        }
    }

private:
    Point ** points;
    Heep * heep;
    bool * status;
    int count;
    double result;
};


int main() {
    int count;
    if (scanf("%d", &count) != 1) {
        return 1;
    }
    for (int i = 0; i < count; i++) {
        FrecklesProblem frecklesProblem;
        frecklesProblem.readSequence();
        frecklesProblem.run();
        printf("%.2f\n", frecklesProblem.getResult());
        if (i != count - 1) {
            printf("\n");
        }
    }
    return 0;
}
