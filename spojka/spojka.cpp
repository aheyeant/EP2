#include <iostream>
#include <vector>
#include <set>


using namespace std;

struct TSpoj {
    TSpoj() : price(INT64_MAX), used(false), id(0) {}

    unsigned long price;
    bool used;
    int id;
    vector<pair<int, int>> directions;
};

/*bool operator<(const TSpoj & l, const TSpoj & r) {
    return l.price < r.price;
}*/

//typedef set<TSpoj&> sortedData;


class ProblemSpojka {
public:
    ProblemSpojka() : count(0), maxPrice(0), nodes(nullptr) {}

    ~ProblemSpojka() {
        if (!count) return;
        delete[] nodes;
    }

    void read() {
        int edges;
        cin >> count >> edges;
        nodes = new TSpoj[count];
        for(int i = 0; i < count; i++) {
            nodes[i].id = i;
        }
        for (int i = 0; i < edges; i++) {
            int from, to, price;
            cin >> from >> to >> price;
            from--;
            to--;
            nodes[from].directions.emplace_back(to, price);
            nodes[to].directions.emplace_back(from, price);
        }
    }

    void run() {
        for (int i = 0; i < count; i++) {
            dejksta(i);
            findMaxPath();
            if (maxPrice == INT64_MAX) {
                break;
            }
        }
    }

private:
    int count;
    unsigned long maxPrice;
    TSpoj * nodes;

    friend ostream & operator<<(ostream & os, const ProblemSpojka & problemSpojka);

    struct TCompare {
        bool operator()(TSpoj* const& l, TSpoj* const& r) {
            if(l->price == r->price)
                return l->id < r->id;
            return l->price < r->price;};
    };

    void dejksta(int id) {
        set<TSpoj*, TCompare> data;
        //sortedData data;

        nodes[id].price = 0;

        data.insert(&nodes[id]);

        while (!data.empty()) {
            auto item = data.begin().operator*();
            item->used = true;
            data.erase(data.begin());
            for (auto line : item->directions) {
                if (!nodes[line.first].used) {
                    if (nodes[line.first].price > item->price + line.second) {
                        auto iter = data.find(&nodes[line.first]);
                        if (iter != data.end()) {
                            data.erase(iter);
                        }
                        nodes[line.first].price = item->price + line.second;
                        data.insert(&nodes[line.first]);
                    }
                }
            }
        }
    }

    void findMaxPath() {
        for (int i = 0; i < count; i++) {
            if (nodes[i].price > maxPrice) {
                maxPrice = nodes[i].price;
            }
            nodes[i].price = INT64_MAX;
            nodes[i].used = false;
        }
    }
};

ostream & operator<<(ostream & os, const ProblemSpojka & problemSpojka) {
    if (problemSpojka.maxPrice == INT64_MAX) {
        os << "Bez spojeni neni veleni!";
        return os;
    }
    os << "Nejvetsi vzdalenost je " << problemSpojka.maxPrice << ".";
    return os;
}



int main() {
    int count;
    cin >> count;
    for (int i = 0; i < count; i++) {
        ProblemSpojka problemSpojka;
        problemSpojka.read();
        problemSpojka.run();
        cout << problemSpojka << endl;
    }
    return 0;
}

/*
2
5 5
1 2 1
2 3 2
3 4 3
4 5 4
5 1 5
5 2
1 2 1
2 3 1




 */