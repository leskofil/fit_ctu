
#include <iostream>
#include <valarray>

using namespace std;

class Node {
public:
    Node() = default;

    Node(int x, int y, int level, Node *pred) : row(x), col(y), level(level), pred(pred) {}

    int row, col, level;
    Node *pred;
};

class CQueue {
private:

    struct CNode {
        CNode *m_Next{};

        Node data{};
    };

    CNode *begin;

    CNode *end;

public:
    CQueue() : begin(nullptr), end(nullptr) {}

    CQueue &operator=(const CQueue &s) = delete;

    ~CQueue();

    void push(const Node &value);

    void pop();

    bool empty() const;

    Node *back() const;

    Node front() const;
};

CQueue::~CQueue() {
    CNode *tmp;

    while (begin) {
        tmp = begin->m_Next;
        delete begin;
        begin = tmp;
    }
    begin = end = nullptr;
}

void CQueue::push(const Node &value) {
    auto *node = new CNode;
    node->data = value;
    node->m_Next = nullptr;

    if (begin == nullptr) {
        begin = node;
        end = node;
    } else {
        end->m_Next = node;
        end = node;
    }
}

void CQueue::pop() {
    if (empty())
        throw;
    if (begin == end) {
        delete begin;
        begin = end = nullptr;
    } else {
        CNode *node = begin;
        begin = begin->m_Next;
        delete node;
    }
}

Node CQueue::front() const {
    if (empty())
        throw;
    else
        return begin->data;
}

Node *CQueue::back() const {
    if (empty())
        throw;
    else
        return &end->data;
}

bool CQueue::empty() const {
    return begin == nullptr;
}

class CLever {
public:
    CLever() = default;

    void ReadVector(int size) {
        isUp = false;
        vector = new bool[size];
        char cell;
        for (int j = 0; j < size; ++j) {
            cin >> cell;
            if (cell == '1')
                vector[j] = true;
            else if (cell == '0')
                vector[j] = false;
            else
                throw runtime_error("Invalid read");
        }
        LeverSize = size;
    }

    ~CLever() { delete[] vector; }

    friend ostream &operator<<(ostream &os, const CLever &lever) {
        os << lever.order << " ";
        for (int i = 0; i < lever.LeverSize; i++)
            cout << lever.vector[i];
        cout << endl;
        return os;
    }

    bool isUp;
    int order;
    bool *vector;
    int LeverSize;


};

class Maze {
public:
    Maze(const int size, int numberOfLevers) : shortestPathLen(10000000), size(size), numberOfLevers(numberOfLevers),
                                               rowNum{-1, 0, 0, 1},
                                               colNum{0, -1, 1, 0},
                                               finish{0, 0, 0, nullptr} {
        int cellu;
        levers = new CLever[numberOfLevers];
        for (int i = 0; i < numberOfLevers; i++) {
            cin >> cellu;
            levers[i].order = cellu;
            levers[i].ReadVector(size);
        }

        char cell;
        maze = new bool *[size];
        visited = new bool *[size];
        tmpMaze = new bool *[size];
        for (int i = 0; i < size; ++i) {
            maze[i] = new bool[size];
            visited[i] = new bool[size];
            tmpMaze[i] = new bool[size];
            for (int j = 0; j < size; ++j) {
                maze[i][j] = false;
                visited[i][j] = false;
                tmpMaze[i][j] = false;
            }
        }

        for (int row = 0; row < size; ++row) {
            for (int col = 0; col < size; ++col) {
                cin >> cell;
                if (cell == '1') {
                    maze[row][col] = true;
                    tmpMaze[row][col] = true;
                }
                else if (cell == '0') {
                    maze[row][col] = false;
                    tmpMaze[row][col] = false;
                }
                else
                    throw runtime_error("Invalid read");
            }
        }
        cin >> finish.col >> finish.row;
        finish.col--;
        finish.row--;
        //Print();
    }

    ~Maze() {
        for (int i = 0; i < size; i++) {
            delete[] maze[i];
            delete[] visited[i];
            delete[] tmpMaze[i];
        }
        delete[] tmpMaze;
        delete[] maze;
        delete[] visited;
        delete[] levers;
        delete[] path;
    }

    void Print(bool ** mazu) const {
        for (int i = 0; i < numberOfLevers; i++) {
            cout << levers[i].isUp;
        }
        cout << "\n";
        for (int row = size - 1; row >= 0; --row) {
            for (int col = 0; col < size; ++col) {
                cout << mazu[row][col];
            }
            cout << "\n";
        }
        cout << "\n" << endl;
    }

    bool BFS(CLever **&activatedLevers, int activatedLeversSize) {
        Node start = {0, 0, 0, nullptr};
        if (tmpMaze[start.col][start.row])
            return false;
        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size; j++)
                visited[i][j] = false;
        }

        CQueue queue;
        visited[start.col][start.row] = true;
        CQueue q;
        q.push(start);
        while (!q.empty()) {
            Node curr = q.front();
            if (curr.level >= shortestPathLen)
                return false;
            queue.push(curr);
            if (curr.col == finish.col && curr.row == finish.row && tmpMaze[curr.row][curr.col] != 1) {
                ReconstructPath(curr, activatedLevers, activatedLeversSize);
                return true;
            }
            q.pop();

            for (int i = 0; i < 4; i++) {
                int row = curr.row + rowNum[i];
                int col = curr.col + colNum[i];

                if (isValid(row, col) && !tmpMaze[row][col] && !visited[row][col]) {
                    visited[row][col] = true;
                    q.push({row, col, curr.level + 1, queue.back()});
                }
            }
        }
        return false;
    }

    int CalculatePathLen(Node &node, CLever **&pLever, int pLeverSize) {
        int len = node.level;
        char max = 0;
        bool wasNegative = false;
        for (int i = 0; i < pLeverSize; i++) {
            if (pLever[i]->order > max)
                max = pLever[i]->order;
            if (pLever[i]->order == -1 && !wasNegative) {
                len += 2;
                wasNegative = true;
            }
        }
        len += max * 2;
        return len;
    }

    void ReconstructPath(Node &goal, CLever **&activatedLevers, int activatedLeversSize) {
        int newShortestPath = CalculatePathLen(goal, activatedLevers, activatedLeversSize);
        if (newShortestPath < shortestPathLen) {
            if (!path || path[0].level >= goal.level) {
                path = new Node[goal.level + 1];
            }
            Node *x = &goal;
            int i = 0;
            while (x) {
                path[i] = *x;
                x = x->pred;
                i++;
            }
            for (int k = 0; k < activatedLeversSize; k++)
                activatedLevers[k]->isUp = true;
            shortestPathLen = newShortestPath;
        }
    }

    void ChangeMaze(CLever **&pLever, int leverCount) {
        for (int i = 0; i < leverCount; i++) {
            //cout << *pLever[i] << endl;
            for (int j = 0; j < size; j++) {
                //pLever[i]->isUp ^= 1;
                if (i == 0) {
                    if (pLever[i]->vector[j]) {
                        for (int row = 0; row < size; row++) {
                            tmpMaze[row][j] = !maze[row][j];
                        }
                    } else {
                        for (int row = 0; row < size; row++) {
                            tmpMaze[row][j] = maze[row][j];
                        }
                    }
                } else {
                    if (pLever[i]->vector[j]) {
                        for (int row = 0; row < size; row++) {
                            tmpMaze[row][j] = !tmpMaze[row][j];
                        }
                    } else {
                        for (int row = 0; row < size; row++) {
                            tmpMaze[row][j] = tmpMaze[row][j];
                        }
                    }
                }
            }
        }
        //Print(tmpMaze);
    }

    void FindPath() {
        auto **data = new CLever *[numberOfLevers];
        unsigned int powSetSize = pow(2, numberOfLevers);
        unsigned int counter;
        int j, k = 0;
        //BFS(data, 0);
        for (counter = 0; counter < powSetSize; counter++) {
            for (j = 0; j < numberOfLevers; j++) {
                if (counter & (1 << j)) {
                    data[k] = &levers[j];
                    //cout << *data[k];
                    k++;
                }
            }
            //cout << endl;
            ChangeMaze(data, k);
            BFS(data, k);
            //CleanMaze(tmpMaze);
            k = 0;
        }
        delete[]data;
    }

    void PrintAns() {
        if (!path) {
            cout << -1;
        } else {
            cout << shortestPathLen << endl;
            for (int i = 0; i < numberOfLevers; i++) {
                cout << levers[i].isUp;
            }
            cout << "\n";
            for (int j = path[0].level; j >= 0; j--) {
                if (j == 0)
                    cout << "[" << path[j].col + 1 << ";" << path[j].row + 1 << "]";
                else
                    cout << "[" << path[j].col + 1 << ";" << path[j].row + 1 << "],";
            }
            cout << "\n";
        }
    }

private:
    bool isValid(int row, int col) const {
        return (row >= 0) && (row < size) && (col >= 0) && (col < size);
    }

    int shortestPathLen;
    int size; // 1-1000
    int numberOfLevers; // 0-10
    char rowNum[4];
    char colNum[4];
    Node *path{};
    Node finish;
    CLever *levers;
    bool **visited;
    bool **maze;
    bool **tmpMaze;
};


int main() {
    int size, levers;
    cin >> size >> levers;
    Maze maze = Maze(size, levers);

    //maze.Print();

    maze.FindPath();
    maze.PrintAns();
    return 0;
}