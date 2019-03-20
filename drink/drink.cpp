#include <utility>
#include <iostream>
#include <set>
#include <map>
#include <vector>

typedef std::map<std::string, int> encrypt_id;
typedef std::map<int, std::string> decrypt_id;
typedef std::vector<std::pair<int, std::set<int>>> t_drink;

class DrinkSort {
public:
    explicit DrinkSort() : number(0) {}

    bool read() {
        int drinksCount;
        if (scanf("%d", &drinksCount) != 1) {
            std::cout << "read drinks count - fail\n";
            return false;
        }
        if (drinksCount == 0) return false;
        decrypt.clear();
        encrypt.clear();
        data.clear();
        output.clear();
        for (int i = 0; i < drinksCount; i++) {
            std::string drink;
            std::cin >> drink;
            encrypt.insert(std::make_pair(drink, i));
            decrypt.insert(std::make_pair(i, drink));
            std::set<int> s;
            data.push_back(std::make_pair(i, s));
        }
        return true;
    }

    void run(int n) {
        number = n;
        int relationsCount;
        if (scanf("%d", &relationsCount) != 1) {
            std::cout << "read relations count - fail\n";
            return;
        }
        char c;
        if (scanf("%c", &c) != 1) {
            std::cout << "read \\n before relations count - fail\n";
            return;
        }
        for (int i = 0; i < relationsCount; i++) {
            int left, right;
            std::string str;
            while (true) {
                if (scanf("%c", &c) != 1) {
                    std::cout << "read char in " << i + 1 << " relation - fail\n";
                    return;
                }
                if (c == ' ') {
                    break;
                }
                if (c == '\n') goto run;
                str += c;
            }
            auto iter = encrypt.find(str);
            if (iter == encrypt.end()) {
                std::cout << "read first drink in " << i + 1 << " relation - fail\n";
                return;
            }
            left = iter.operator*().second;
            str.clear();
            while (true) {
                if (scanf("%c", &c) != 1) {
                    std::cout << "read char in " << i + 1 << " relation - fail\n";
                    return;
                }
                if (c == '\n') {
                    break;
                }
                str += c;
            }
            iter = encrypt.find(str);
            if (iter == encrypt.end()) {
                std::cout << "read second drink in " << i + 1 << " relation - fail\n";
                return;
            }
            right = iter.operator*().second;
            data[right].second.insert(left);
        }
        run:
            for (int i = 0; i < data.size(); i++) {
                int index = 0;
                for (auto &j : data) {
                    if (j.first >= 0 && j.second.empty()) {
                        index = j.first;
                        j.first = -1;
                        break;
                    }
                }
                for (auto &k : data) {
                    if (k.first != -1) {
                        k.second.erase(index);
                    }
                }
                output.push_back(index);
            }
    }

    friend std::ostream & operator<<(std::ostream & os, const DrinkSort & drinkSort);

private:
    decrypt_id decrypt;
    encrypt_id encrypt;
    t_drink data;
    std::vector<int> output;
    int number;
};

std::ostream & operator<<(std::ostream & os, const DrinkSort & drinkSort) {
    os << "Case #" << drinkSort.number << ": Dilbert should drink beverages in this order:";
    for (auto item : drinkSort.output) {
        os << " " << drinkSort.decrypt.find(item).operator*().second;
    }
    os << ".";
    return os;
}

int main() {
    DrinkSort drinkSort;
    int n = 0;
    while (true) {
        n++;
        if (drinkSort.read()) {
            drinkSort.run(n);
            std::cout << drinkSort << std::endl;
        } else {
            return 0;
        }
    }
}