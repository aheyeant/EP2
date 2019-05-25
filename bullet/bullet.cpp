#include <iostream>
#include <vector>
#include <iomanip>
#include "math.h"

#define EPS 1e-8
#define PI 3.14159265


struct TCircular {
    TCircular(int x, int y, int r) {
        this->x = x;
        this->y = y;
        this->r = r;
    }

    int x;
    int y;
    int r;
};

class ProblemBullet {
public:
    explicit ProblemBullet() : start_x(0), start_y(0), stop_x(0), stop_y(0), k(0), b(0), x_ray(0), len(0), dist(0) {}

    bool read() {
        int count;
        scanf("%d", &count);
        if (count == -1) return false;
        for (int i = 0; i < count; i++) {
            int x, y, r;
            scanf("%d%d%d", &x, &y, &r);
            circular.push_back(TCircular(x, y, r));
        }
        scanf("%d%d%d%d", &start_x, &start_y, &stop_x, &stop_y);
        return true;
    }

    void run() {
        calculateKB();
        for (auto item : circular) {
            calculateCircular(item);
        }
        dist = sqrt(pow(start_x - stop_x, 2) + pow(start_y - stop_y, 2));
        dist = dist - x_ray + len;
        //dist = (round(dist * 100)) / 100;
        //x_ray = (round(x_ray * 100)) / 100;
        std::cout << "Superman sees thru " << std::fixed << std::setprecision(2) << x_ray << " units, and flies " << dist << " units." << std::endl;
    }

private:
    std::vector<TCircular> circular;
    int start_x, start_y, stop_x, stop_y;
    double k, b;
    double x_ray, len, dist;

    void calculateKB() {
        k = (start_y - stop_y) / static_cast<double>(start_x - stop_x);
        b = start_y - k * start_x;
    }

    bool equals(double left, double right) {
        return fabs(left - right) < EPS;
    }

    void calculateCircular(const TCircular & circular) {
        double a_x = (k * k  + 1);
        double b_x = (2 * k * b - 2 * circular.x - 2 * circular.y * k);
        double c_x = (b * b + circular.x * circular.x + circular.y * circular.y - 2 * circular.y * b - circular.r * circular.r);
        double d_x = b_x * b_x - 4 * a_x * c_x;
        if (equals(d_x, 0)) {
            return;
        }
        if (d_x < 0) {
            return;
        }
        d_x = sqrt(d_x);
        double x_0 = (-b_x + d_x) / (2 * a_x);
        double x_1 = (-b_x - d_x) / (2 * a_x);
        double y_0 = k * x_0 + b;
        double y_1 = k * x_1 + b;
        //std::cout << "x_0: " << x_0 << " y_0: " << y_0  << std::endl << "x_1: " << x_1 << " y_1: " << y_1 << std::endl;

        if (start_x <= stop_x) {
            if (x_0 < start_x) return;
            if (x_0 > stop_x) return;
        } else {
            if (x_0 > start_x) return;
            if (x_0 < stop_x) return;
        }
        if (start_y <= stop_y) {
            if (y_0 < start_y) return;
            if (y_0 > stop_y) return;
        } else {
            if (y_0 > start_y) return;
            if (y_0 < stop_y) return;
        }


        double l_a = sqrt(pow(x_0 - x_1, 2) + pow(y_0 - y_1, 2));
        //double l_b = sqrt(pow(x_0 - circular.x, 2) + pow(y_0 - circular.y, 2));

        double l_b = circular.r;
        //double l_c = sqrt(pow(x_1 - circular.x, 2) + pow(y_1 - circular.y, 2));

        double l_c = circular.r;

        double cos_a = (l_b * l_b + l_c * l_c - l_a * l_a) / (2 * l_b * l_c);
        double acos_a = acos(cos_a) * 180.0 / PI;
        x_ray += l_a;
        if (acos_a > 180) {
            acos_a = 360 - acos_a;
        }

        //std::cout << "x_ray: " << x_ray << " len: " << (2 * PI * circular.r) / 360 * acos_a << std::endl;

        len += (2 * PI * circular.r) / 360 * acos_a;
    }
};



int main() {
    while (true) {
        ProblemBullet bullet;
        if (bullet.read()) {
            bullet.run();
        } else {
            return 0;
        }
    }
}

/**
3
0 0 100
-1000 0 100
1000 0 100
-210 0
210 0
-1

 */
