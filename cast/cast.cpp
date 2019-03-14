#include <iostream>
#include <queue>



enum Attribute {
    DOT, ASTERISK, TARGET
};

struct Map {
public:
    explicit Map(int x, int y) : xSize(x), ySize(y) {
        field = new Attribute*[ySize + 2];
        for (int i = 0; i < ySize + 2; i++) {
            field[i] = new Attribute[xSize + 2];
        }
        for (int i = 0; i < xSize + 2; i++) {
            field[0][i] = field[ySize + 1][i] = Attribute::DOT;
        }
        for (int i = 0; i < ySize + 2; i++) {
            field[i][0] = field[i][xSize + 1] = Attribute::DOT;
        }
    }

    ~Map() {
        for (int i = 0; i <= ySize; i++) {
            delete[] field[i];
        }
        delete[] field;
    }

    int         xSize;
    int         ySize;
    Attribute **field;
};

std::ostream & operator<< (std::ostream & os, Map & map) {
    os << "size is " << map.ySize << "x" << map.xSize << "\n";
    for (int i = 1; i <= map.ySize; i++) {
        for (int j = 1; j <= map.xSize; j++) {
            os << (map.field[i][j] == Attribute::DOT ? "." : map.field[i][j] == Attribute::ASTERISK ? "*" : "X");
        }
        os << "\n";
    }
    return os;
}

class Reader {
public:
    explicit Reader() {
        map = nullptr;
        end = false;
    }

    ~Reader() {
        destroyMap();
    }

    void readMap() {
        int y, x;
        if (scanf("%d%d", &y, &x) != 2) {
            std::cout << "read error" << std::endl;
            return;
        }
        if (x == 0 && y == 0) {
            end = true;
            return;
        }
        map = new Map(x, y);
        char c;
        if (scanf("%c", &c) != 1) {
            std::cout << "read error" << std::endl;
            return;
        }
        for (int i = 1; i <= y; i++) {
            for (int j = 1; j < x + 2; j++) {
                if (scanf("%c", &c) != 1) {
                    std::cout << "read error" << std::endl;
                    return;
                }
                switch (c) {
                    case '.': {
                        map->field[i][j] = Attribute::DOT;
                        break;
                    }
                    case '*': {
                        map->field[i][j] = Attribute::ASTERISK;
                        break;
                    }
                    case 'X': {
                        map->field[i][j] = Attribute::TARGET;
                        break;
                    }
                    default: {
                        if (c == '\n' && j == x + 1) {
                            break;
                        } else {
                            std::cout << "read error" << std::endl;
                            return;
                        }
                    }
                }
            }
        }
    }

    Map * getMap() {
        return map;
    }

    bool isEnd() const {
        return this->end && map == nullptr;
    }

private:
    Map * map;
    bool end;

    void destroyMap() {
        if (!map)
            return;
        delete map;
    }
};

class Parser {
public:
    explicit Parser(Map * map) : map(map) {
        counts = new int[6];
        counts[0] = counts[1] = counts[2] = counts[3] = counts[4] = counts[5] = 0;
    }

    ~Parser() {
        delete[] counts;
    }

    void run() {
        for (int i = 1; i < map->ySize + 1; i++) {
            for (int j = 1; j < map->xSize + 1; j++) {
                if (map->field[i][j] != Attribute::DOT) {
                    int count = sectionBFS(j, i);
                    if (count != 0) {
                        counts[count - 1]++;
                    }
                }
            }
        }
    }

private:
    Map * map;
    int * counts;
    friend std::ostream & operator<< (std::ostream & os, Parser & parser);

    int sectionBFS(int x, int y) {
        std::queue<std::pair<int, int>> fifo;
        int count = 0;
        if (map->field[y][x] == Attribute::TARGET) {
            count++;
            targetBFS(x, y);
        }
        map->field[y][x] = Attribute::DOT;
        fifo.push(std::make_pair(x, y));
        while (!fifo.empty()) {
            if (count == 6) {
                break;
            }
            std::pair<int,int> item = fifo.front();
            fifo.pop();
            if (map->field[item.second - 1][item.first] != Attribute::DOT) {
                if (map->field[item.second - 1][item.first] == Attribute::TARGET){
                    count++;
                    targetBFS(item.first, item.second - 1);
                }
                map->field[item.second - 1][item.first] = Attribute::DOT;
                fifo.push(std::make_pair(item.first, item.second - 1));
            }
            if (map->field[item.second + 1][item.first] != Attribute::DOT) {
                if (map->field[item.second + 1][item.first] == Attribute::TARGET){
                    count++;
                    targetBFS(item.first, item.second + 1);
                }
                map->field[item.second + 1][item.first] = Attribute::DOT;
                fifo.push(std::make_pair(item.first, item.second + 1));
            }
            if (map->field[item.second][item.first - 1] != Attribute::DOT) {
                if (map->field[item.second][item.first - 1] == Attribute::TARGET){
                    count++;
                    targetBFS(item.first - 1, item.second);
                }
                map->field[item.second][item.first - 1] = Attribute::DOT;
                fifo.push(std::make_pair(item.first - 1, item.second));
            }
            if (map->field[item.second][item.first + 1] != Attribute::DOT) {
                if (map->field[item.second][item.first + 1] == Attribute::TARGET){
                    count++;
                    targetBFS(item.first + 1, item.second);
                }
                map->field[item.second][item.first + 1] = Attribute::DOT;
                fifo.push(std::make_pair(item.first + 1, item.second));
            }
        }
        return count;
    }

    void targetBFS(int x, int y) {
        std::queue<std::pair<int, int>> fifo;
        map->field[y][x] = Attribute::ASTERISK;
        fifo.push(std::make_pair(x, y));
        while (!fifo.empty()) {
            std::pair<int,int> item = fifo.front();
            fifo.pop();
            if (map->field[item.second - 1][item.first] == Attribute::TARGET) {
                map->field[item.second - 1][item.first] = Attribute::ASTERISK;
                fifo.push(std::make_pair(item.first, item.second - 1));
            }
            if (map->field[item.second + 1][item.first] == Attribute::TARGET) {
                map->field[item.second + 1][item.first] = Attribute::ASTERISK;
                fifo.push(std::make_pair(item.first, item.second + 1));
            }
            if (map->field[item.second][item.first - 1] == Attribute::TARGET) {
                map->field[item.second][item.first - 1] = Attribute::ASTERISK;
                fifo.push(std::make_pair(item.first - 1, item.second));
            }
            if (map->field[item.second][item.first + 1] == Attribute::TARGET) {
                map->field[item.second][item.first + 1] = Attribute::ASTERISK;
                fifo.push(std::make_pair(item.first + 1, item.second));
            }
        }
    }

};

std::ostream & operator<< (std::ostream & os, Parser & parser) {
    os << "Throw:";
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < parser.counts[i]; j++) {
            os << " " << i + 1;
        }
    }
    os << "\n";
    return os;
}

int main() {
    while (true) {
        Reader reader;
        reader.readMap();
        if (reader.isEnd()) {
            return 0;
        }
        //std::cout << *reader.getMap();
        Parser parser = Parser(reader.getMap());
        parser.run();
        std::cout << parser;
    }
}



/*
15 30
..............................
................*.............
...*****.......****...........
...*X***......**X***..........
...*****.....***X**...........
...***X*......****............
...*****........*.............
..............................
..............................
.....***.........******.......
....**X****......*X**X*.......
...*******.......******.......
..****X**........*X**X*.......
.....***.........******.......
..............................
3 3
...
XXX
...
0 0

 */