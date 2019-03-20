#include <utility>

#include <iostream>
#include <vector>
#include <exception>
#include <sstream>
#include <queue>


enum EColor{
    RED = 1, BLACK = 2, NONE = 3
};

typedef std::vector<std::pair<EColor, std::vector<int>>> t_graph;

class GraphFormatException {
public:
    explicit GraphFormatException(std::string message) {
        this->message = std::move(message);
    }

    std::string getMessage() {
        return this->message;
    }

private:
    std::string message;
};


class BicolorGraph {
public:
    explicit BicolorGraph() {
        isBicolorGraph = finish = false;
        nodes = edges = 0;
    }

    void readGraph() {
        isBicolorGraph = true;
        finish = false;
        if (scanf("%d", &nodes) != 1) {
            throw GraphFormatException("Error: read nodes count");
        }
        if (!nodes) {
            finish = true;
            return;
        }
        if (scanf("%d", &edges) != 1) {
            throw GraphFormatException("Error: read edges count");
        }
        graph.clear();
        for (int i = 0; i < nodes; i++) {
            std::vector<int> v;
            graph.push_back(std::make_pair(EColor::NONE, v));
        }
        for (int i = 0; i < edges; i++) {
            int from, to;
            if (scanf("%d%d", &from, &to) != 2) {
                std::ostringstream ss;
                ss << i + 1;
                throw GraphFormatException("Error: read edge " +  ss.str());
            }
            graph[from].second.push_back(to);
            graph[to].second.push_back(from);
        }

    }

    void run() {
        for (int i = 0; i < nodes && isBicolorGraph; i++) {
            if (graph[i].first == EColor::NONE) {
                bfs(i);
            }
        }
    }

    bool isEnd() {
        return finish;
    }

    bool isBicolor() {
        return isBicolorGraph;
    }

private:
    t_graph graph;
    bool finish;
    bool isBicolorGraph;
    int nodes;
    int edges;

    void bfs(int iterator) {
        std::queue<int> fifo;
        graph[iterator].first = EColor::RED;
        fifo.push(iterator);
        while (!fifo.empty()) {
            auto it = fifo.front();
            fifo.pop();
            for (auto nodes : graph[it].second) {
                if (graph[nodes].first == EColor::NONE) {
                    graph[nodes].first = graph[it].first == EColor::RED ? EColor::BLACK : EColor::RED;
                    fifo.push(nodes);
                } else if (graph[nodes].first == graph[it].first) {
                    isBicolorGraph = false;
                    return;
                }
            }
        }
    }
};

std::ostream & operator<<(std::ostream & os, BicolorGraph & graph) {
    os << (graph.isBicolor() ? "YES" : "NO");
    return os;
}


int main() {
    BicolorGraph graph;
    while(true) {
        try {
            graph.readGraph();
        } catch (GraphFormatException & ex) {
            std::cout << ex.getMessage() << std::endl;
            return 1;
        }
        if (graph.isEnd()) {
            return 0;
        }
        graph.run();
        std::cout << graph << std::endl;
    }
}