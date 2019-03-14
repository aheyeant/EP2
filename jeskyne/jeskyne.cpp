#include <iostream>
#include <queue>

enum Attribute {
    FREE, WATER
};

struct TCoordinate {
    TCoordinate(int x, int y, int z) : x(x), y(y), z(z) {}

    int x;
    int y;
    int z;
};

struct Map {
public:
    explicit Map (int x, int y, int z) : xSize(x), ySize(y), zSize(z) {
        field = new Attribute**[zSize + 2];
        for (int i = 0; i < zSize + 2; i++) {
            field[i] = new Attribute*[ySize + 2];
            for (int j = 0; j < ySize + 2; j++) {
                field[i][j] = new Attribute[xSize + 2];
                for (int k = 0; k < xSize + 2; k++) {
                    field[i][j][k] = Attribute::FREE;
                }
            }
        }

    }

    ~Map() {
        for (int i = 0; i < zSize + 2; i++) {
            for (int j = 0; j < ySize + 2; j++) {
                delete[] field[i][j];
            }
            delete[] field[i];
        }
        delete[] field;
    }

    void makeFree (TCoordinate coordinate) {
        field[coordinate.z][coordinate.y][coordinate.x] = Attribute::FREE;
    }

    int xSize;
    int ySize;
    int zSize;
    Attribute *** field;
};

std::ostream & operator<< (std::ostream & os, Map & map) {
/*
    os << "size [z=" << map.zSize << ",y=" << map.ySize << ",x=" << map.xSize << "\n";
    for (int i = 1; i < map.zSize + 1; i++) {
        os << "layer " << i << "\n";
        for (int j = 1; j < map.ySize + 1; j++) {
            for (int k = 1; k < map.xSize + 1; k++) {
                os << (map.field[i][j][k] == Attribute::FREE ? "." : "*");
            }
            os << "\n";
        }
    }
    return os;
*/


    os << "size [z=" << map.zSize << ",y=" << map.ySize << ",x=" << map.xSize << "\n";
    for (int i = 0; i < map.zSize + 2; i++) {
        os << "layer " << i << "\n";
        for (int j = 0; j < map.ySize + 2; j++) {
            for (int k = 0; k < map.xSize + 2; k++) {
                os << (map.field[i][j][k] == Attribute::FREE ? "." : "*");
            }
            os << "\n";
        }
    }
    return os;
}

class Reader {
public:
    explicit Reader() {
        map = nullptr;
    }

    ~Reader() {
        delete map;
    }

    void rearMap() {
        int x, y, z;
        if (scanf("%d %d %d", &x, &y, &z) != 3) {
            std::cout << "input error" << std::endl;
            return;
        }
        map = new Map(x, y, z);
        for (int i = 1; i < map->zSize + 1; i++) {
            int n;
            if (scanf("%d", &n) != 1) {
                std::cout << "input error" << std::endl;
                return;
            }
            for (int j = 0; j < n; j++) {
                int xx, yy;
                if (scanf("%d %d", &xx, &yy) != 2) {
                    std::cout << "input error" << std::endl;
                    return;
                }
                map->field[i][yy][xx] = Attribute::WATER;
            }
        }
    }

    Map * getMap() {
        return map;
    }

private:
    Map * map;
};

class Parser {
public:
    explicit Parser(Map * map) : map(map), volume(0) {}

    void run () {
        for (int i = 1; i < map->ySize + 1; i++) {
            for (int j = 1; j < map->xSize + 1; j++) {
                if (map->field[1][i][j] == Attribute::WATER) {
                    BFS(j, i, 1);
                }
            }
        }
    }

private:
    friend std::ostream & operator<< (std::ostream & os, Parser & parser);
    Map * map;
    int volume;

    void BFS(int x, int y, int z) {
        std::queue<TCoordinate> fifo;
        fifo.push({x, y, z});
        map->makeFree({x, y, z});
        volume++;
        while(!fifo.empty()) {
            TCoordinate item = fifo.front();
            fifo.pop();
            if (map->field[item.z + 1][item.y][item.x] == Attribute::WATER) {
                fifo.push({item.x, item.y, item.z + 1});
                map->makeFree({item.x, item.y, item.z + 1});
                volume++;
            }
            if (map->field[item.z][item.y + 1][item.x] == Attribute::WATER) {
                fifo.push({item.x, item.y + 1, item.z});
                map->makeFree({item.x, item.y + 1, item.z});
                volume++;
            }
            if (map->field[item.z][item.y][item.x + 1] == Attribute::WATER) {
                fifo.push({item.x + 1, item.y, item.z});
                map->makeFree({item.x + 1, item.y, item.z});
                volume++;
            }
            if (map->field[item.z - 1][item.y][item.x] == Attribute::WATER) {
                fifo.push({item.x, item.y, item.z - 1});
                map->makeFree({item.x, item.y, item.z - 1});
                volume++;
            }
            if (map->field[item.z][item.y - 1][item.x] == Attribute::WATER) {
                fifo.push({item.x, item.y - 1, item.z});
                map->makeFree({item.x, item.y - 1, item.z});
                volume++;
            }
            if (map->field[item.z][item.y][item.x - 1] == Attribute::WATER) {
                fifo.push({item.x - 1, item.y, item.z});
                map->makeFree({item.x - 1, item.y, item.z});
                volume++;
            }
        }
    }
};

std::ostream & operator<< (std::ostream & os, Parser & parser) {
    os << "Je nutne vycerpat " << parser.volume * 1000 << " litru vody." << "\n";
    return os;
}


int main() {
    int n;
    if (scanf("%d", &n) != 1) {
        std::cout << "input error" << std::endl;
        return 1;
    }
    for (int i = 0; i < n; i++) {
        Reader reader;
        reader.rearMap();
        Parser parser = Parser(reader.getMap());
        parser.run();
        std::cout << parser;
        //std::cout << *reader.getMap() << std::endl;
    }
    return 0;
}