#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <set>

struct TNode {
    explicit TNode(const uint8_t id) : id(id), is_touch(false), has_camera(false) {}

    void touch() {
        this->is_touch = true;
    }

    void detach() {
        this->is_touch = false;
    }

    std::vector<TNode *> ways;
    uint8_t id;
    bool is_touch;
    bool has_camera;
};

class GuideProblem {
public:
    GuideProblem() = default;

    ~GuideProblem() {
        for (auto pItem : graph) {
            delete pItem;
        }
    }

    bool read() {
        int count;
        std::cin >> count;
        if (!count) {
            return false;
        }
        for (int i = 0; i < count; i++) {
            std::string str;
            std::cin >> str;
            graph.push_back(new TNode(static_cast<const uint8_t>(graph.size())));
            decrypt.insert(std::make_pair(str, graph.size() - 1));
        }
        std::cin >> count;
        for (int i = 0; i < count; i++) {
            std::string s1, s2;
            std::cin >> s1 >> s2;
            TNode * e1 = graph[decrypt.find(s1).operator*().second];
            TNode * e2 = graph[decrypt.find(s2).operator*().second];
            e1->ways.push_back(e2);
            e2->ways.push_back(e1);
        }
        return true;
    }

    void run (const int num) {
        for (auto item : graph) {
            item->touch();
            touched++;
            dfs(item->ways.front());
            if (touched != graph.size()) {
                item->has_camera = true;
                camera_count++;
            }
            detach_all();
        }
        std::cout << "City map #" << num <<": " << camera_count << " camera(s) found" << std::endl;
        for (auto item : graph) {
            if (item->has_camera) {
                for (auto i : decrypt) {
                    if (i.second == item->id) {
                        names.insert(i.first);
                    }
                }
            }
        }
        for (const auto &item : names) {
            std::cout << item << std::endl;
        }
        std::cout << std::endl;
    }


private:
    std::set<std::string> names;
    std::map<std::string, uint8_t> decrypt;
    std::vector<TNode *> graph;
    uint8_t touched = 0;
    short camera_count = 0;


    void detach_all() {
        touched = 0;
        for (auto item : graph) {
            item->detach();
        }
    }

    void dfs(TNode * node) {
        node->touch();
        touched++;
        for (auto item : node->ways) {
            if (!item->is_touch) {
                dfs(item);
            }
        }
    }

};


int main() {
    int i = 0;
    while (true) {
        GuideProblem guideProblem;
        if (guideProblem.read()) {
            guideProblem.run(++i);
        } else {
            return 0;
        }
    }
}