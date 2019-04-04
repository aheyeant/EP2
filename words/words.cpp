#include <cstdio>
#include <iostream>
#include <stack>


typedef unsigned char byte;

struct TLetterConnections {
    TLetterConnections() {
        count = first = 0;
        end = 100;
    }

    bool isEmpty() {
        return count == 0;
    }

    byte getFirst() {
        if (!count) return 100;
        for (byte i = first; i < 26; i++) {
            if (connections[i]) {
                first = i;
                count--;
                connections[i]--;
                return i;
            }
        }
        return 100;
    }

    void add(byte iter) {
        connections[iter]++;
        count++;
    }

    byte first;
    byte end;
    int count;
    int connections[26] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
};

struct TSequenceTester {
    TSequenceTester() = default;

    void add(byte from, byte to) {
        field[from][0]++;
        field[to][1]++;
    }

    bool hasChance() {
        bool left = true;
        bool right = true;

        for (byte i = 0; i < 26; i++) {
            if (field[i][0] == field[i][1]) {
                continue;
            } else {
                if ((field[i][0] - field[i][1]) == 1) {
                    if (left) {
                        left = false;
                        start_place = i;
                    } else {
                        return false;
                    }
                } else if ((field[i][0] - field[i][1]) == -1) {
                    if (right) {
                        right = false;
                    } else {
                        return false;
                    }
                } else {
                    return false;
                }
            }
        }
        if (start_place == 100) {
            for (byte i = 0; i < 26; i++) {
                if (field[i][0]) {
                    start_place = i;
                    break;
                }
            }
        }
        return true;
    }

    int field[26][2] = {{0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0},
                        {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0},
                        {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}};
    byte start_place = 100;
};

class WordsProblem {
public:
    WordsProblem() {
        this->end = 100;
        this->result = false;
        this->count = 0;
    }

    void read() {
        scanf("%d", &this->count);
        if (this->count == 1) this->result = true;
        byte from;
        char prev, now;
        scanf("%c", &prev);
        for (int i = 0; i < this->count; i++) {
            scanf("%c", &now);
            from = static_cast<byte>(now - 'a');
            do {
                prev = now;
                scanf("%c", &now);
            } while (now != '\n' && now != EOF);
            addConnection(from, static_cast<byte>(prev - 'a'));
        }
    }

    bool run() {
        if (result) return result;
        if (!sequenceTester.hasChance()) return false;
        result = true;
        findPartOfSequence(sequenceTester.start_place);
        if (count) result = false;
        return result;
    }

    void test_structure() {
        std::cout << "count = " << this->count << std::endl;
        for (int i = 0; i < 26; i++) {
            std::cout << i << " | ";
            for (int connection : letters[i].connections) {
                std::cout << connection << " ";
            }
            std::cout << "| " << letters[i].count << std::endl;
        }
        std::cout << "chance: " << sequenceTester.hasChance() << std::endl;
    }

private:
    TSequenceTester sequenceTester;
    TLetterConnections letters[26]{};
    byte end;
    bool result;
    int count;

    void addConnection(byte from, byte to) {
        letters[from].add(to);
        sequenceTester.add(from, to);
    }

    void findPartOfSequence(byte iter) {
        std::stack<std::pair<byte, byte>> lifo;
        lifo.push(std::make_pair(iter, letters[iter].getFirst()));
        count--;
        bool find_end = false;

        while (!lifo.empty()) {
            auto next = lifo.top();
            if (letters[next.second].isEmpty()) {
                if (!find_end) {
                        letters[next.first].end = iter;
                        find_end = true;
                } else {
                    if (letters[next.second].end == iter) {
                        letters[next.first].end = iter;
                    } else {
                        result = false;
                        return;
                    }
                }
                if (lifo.size() == 1) {
                    auto tmp = lifo.top().first;
                    lifo.pop();
                    if (!letters[tmp].isEmpty()) {
                        lifo.push(std::make_pair(tmp, letters[tmp].getFirst()));
                        count--;
                    }
                } else {
                    lifo.pop();
                }
            } else {
                lifo.push(std::make_pair(next.second, letters[next.second].getFirst()));
                count--;
            }
        }
    }
};


int main() {
    int count;
    scanf("%d", &count);
    while (count) {
        WordsProblem wordsProblem;
        wordsProblem.read();
        if (wordsProblem.run()) {
            std::cout << "Ordering is possible." << std::endl;
        } else {
            std::cout << "The door cannot be opened." << std::endl;
        }
        count--;
        //if (count) std::cout << std::endl;

    }
    return 0;
}

/*
7
ab
ba
cd
dc
ak
kd
mc
*/

/*
char str[1002];
str[0] = 'a';
for (int i = 0; i < 500; i++) {
char c = static_cast<char>((rand() % 26) + 'a');
str[i * 2 + 1] = str[i * 2 + 2] = c;
}

std::cout << "1\n500\n";
for (int i = 0; i < 500; i++) {
std::cout << str[i * 2] << str[i * 2 + 1] << "\n";
}*/

/*
1
10
an
nw
wl
lr
rb
bb
bm
mq
qb
bh
hc
cd
da
ar
rz
zo
ow
wk
kk
ky
yh
hi
id
dd
dq
qs
sc
cd
dx
xr
rj
jm
mo
ow
wf
fr
rx
xs
sj
jy
yb
bl
ld
db
be
ef
fs
sa
ar
rc
cb
by
yn
ne
ec
cd
dy
yg
gg
gx
xx
xp
pk
kl
lo
or
re
el
ll
ln
nm
mp
pa
ap
pq
qf
fw
wk
kh
ho
op
pk
km
mc
co
oq
qh
hn
nw
wn
nk
ku
ue
ew
wh
hs
sq
qm
mg
gb
 */
