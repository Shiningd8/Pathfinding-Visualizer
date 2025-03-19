#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <conio.h>
#include <windows.h>
#include <limits>

const int ROWS = 20;
const int COLS = 20;
const int INF = std::numeric_limits<int>::max();

enum Algorithm {
    BFS,
    DFS,
    DIJKSTRA
};

struct Node {
    int x, y;
    bool isObstacle = false;
    bool isVisited = false;
    bool isPath = false;
    Node* parent = nullptr;
    int distance = INF;  // For Dijkstra's

    Node(int x, int y) : x(x), y(y) {}
};

std::vector<std::vector<Node>> grid(ROWS, std::vector<Node>(COLS, Node(0, 0)));
Algorithm currentAlgorithm = BFS;

void initializeGrid() {
    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLS; ++j) {
            grid[i][j] = Node(i, j);
        }
    }
}

void setCursorPosition(int x, int y) {
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos = { (short)x, (short)y };
    SetConsoleCursorPosition(handle, pos);
}

std::string getAlgorithmName() {
    switch(currentAlgorithm) {
        case BFS: return "Breadth-First Search";
        case DFS: return "Depth-First Search";
        case DIJKSTRA: return "Dijkstra's Algorithm";
        default: return "Unknown";
    }
}

void drawGrid(int cursorY, int cursorX, Node* startNode, Node* endNode, bool showPath = true) {
    setCursorPosition(0, 0);
    std::cout << "\n";
    std::cout << "  ========================================\n";
    std::cout << "  Current Algorithm: " << getAlgorithmName() << "\n";
    std::cout << "  ========================================\n\n";
    std::cout << "  Controls:\n";
    std::cout << "  - Use arrow keys to move cursor\n";
    std::cout << "  - Press Space to mark/unmark obstacles\n";
    std::cout << "  - Press S to set start point\n";
    std::cout << "  - Press E to set end point\n";
    std::cout << "  - Press F to find path\n";
    std::cout << "  - Press A to change algorithm\n";
    std::cout << "  - Press Q to quit\n\n";

    // Draw top border
    std::cout << "   ";
    for (int j = 0; j < COLS; ++j) {
        std::cout << "--";
    }
    std::cout << "\n";

    for (int i = 0; i < ROWS; ++i) {
        std::cout << "  |";
        for (int j = 0; j < COLS; ++j) {
            if (i == cursorY && j == cursorX) {
                std::cout << "X ";
            } else if (showPath && grid[i][j].isPath) {
                std::cout << "o ";
            } else if (grid[i][j].isObstacle) {
                std::cout << "# ";
            } else if (&grid[i][j] == endNode) {
                std::cout << "E ";
            } else if (grid[i][j].isVisited && &grid[i][j] == startNode) {
                std::cout << "@ ";
            } else if (grid[i][j].isVisited) {
                std::cout << ". ";
            } else {
                std::cout << ". ";
            }
        }
        std::cout << "|\n";
    }

    // Draw bottom border
    std::cout << "   ";
    for (int j = 0; j < COLS; ++j) {
        std::cout << "--";
    }
    std::cout << "\n";
    
    std::cout << "\n  Cursor at: (" << cursorY << ", " << cursorX << ")\n";
}

void resetPathfinding() {
    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLS; ++j) {
            if (!grid[i][j].isObstacle) {
                grid[i][j].isVisited = false;
                grid[i][j].isPath = false;
                grid[i][j].parent = nullptr;
                grid[i][j].distance = INF;
            }
        }
    }
}

bool bfs(Node* startNode, Node* endNode) {
    // Reset path and visited nodes
    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLS; ++j) {
            if (!grid[i][j].isObstacle) {
                grid[i][j].isVisited = false;
                grid[i][j].isPath = false;
                grid[i][j].parent = nullptr;
            }
        }
    }

    std::queue<Node*> queue;
    queue.push(startNode);
    startNode->isVisited = true;

    while (!queue.empty()) {
        Node* current = queue.front();
        queue.pop();

        if (current == endNode) {
            // Mark the path
            Node* pathNode = current;
            while (pathNode) {
                pathNode->isPath = true;
                pathNode = pathNode->parent;
            }
            return true;
        }

        // Check neighbors (up, down, left, right)
        int dx[] = {-1, 1, 0, 0};
        int dy[] = {0, 0, -1, 1};

        for (int i = 0; i < 4; ++i) {
            int newX = current->x + dx[i];
            int newY = current->y + dy[i];

            if (newX >= 0 && newX < ROWS && newY >= 0 && newY < COLS) {
                Node* neighbor = &grid[newX][newY];
                if (!neighbor->isVisited && !neighbor->isObstacle) {
                    neighbor->isVisited = true;
                    neighbor->parent = current;
                    queue.push(neighbor);
                    drawGrid(newX, newY, startNode, endNode, false);
                    Sleep(20);  // Reduced visualization delay
                }
            }
        }
    }
    return false;
}

bool dfs(Node* startNode, Node* endNode) {
    resetPathfinding();
    std::stack<Node*> stack;
    stack.push(startNode);
    startNode->isVisited = true;

    while (!stack.empty()) {
        Node* current = stack.top();
        stack.pop();

        if (current == endNode) {
            // Mark the path
            Node* pathNode = current;
            while (pathNode) {
                pathNode->isPath = true;
                pathNode = pathNode->parent;
            }
            return true;
        }

        // Check neighbors (up, down, left, right)
        int dx[] = {-1, 1, 0, 0};
        int dy[] = {0, 0, -1, 1};

        for (int i = 0; i < 4; ++i) {
            int newX = current->x + dx[i];
            int newY = current->y + dy[i];

            if (newX >= 0 && newX < ROWS && newY >= 0 && newY < COLS) {
                Node* neighbor = &grid[newX][newY];
                if (!neighbor->isVisited && !neighbor->isObstacle) {
                    neighbor->isVisited = true;
                    neighbor->parent = current;
                    stack.push(neighbor);
                    drawGrid(newX, newY, startNode, endNode, false);
                    Sleep(20);
                }
            }
        }
    }
    return false;
}

bool dijkstra(Node* startNode, Node* endNode) {
    resetPathfinding();
    startNode->distance = 0;

    auto compare = [](Node* a, Node* b) { return a->distance > b->distance; };
    std::priority_queue<Node*, std::vector<Node*>, decltype(compare)> pq(compare);
    pq.push(startNode);

    while (!pq.empty()) {
        Node* current = pq.top();
        pq.pop();

        if (current == endNode) {
            // Mark the path
            Node* pathNode = current;
            while (pathNode) {
                pathNode->isPath = true;
                pathNode = pathNode->parent;
            }
            return true;
        }

        if (current->isVisited) continue;
        current->isVisited = true;

        // Check neighbors (up, down, left, right)
        int dx[] = {-1, 1, 0, 0};
        int dy[] = {0, 0, -1, 1};

        for (int i = 0; i < 4; ++i) {
            int newX = current->x + dx[i];
            int newY = current->y + dy[i];

            if (newX >= 0 && newX < ROWS && newY >= 0 && newY < COLS) {
                Node* neighbor = &grid[newX][newY];
                if (!neighbor->isObstacle) {
                    int newDist = current->distance + 1;
                    if (newDist < neighbor->distance) {
                        neighbor->distance = newDist;
                        neighbor->parent = current;
                        pq.push(neighbor);
                        drawGrid(newX, newY, startNode, endNode, false);
                        Sleep(20);
                    }
                }
            }
        }
    }
    return false;
}

bool findPath(Node* startNode, Node* endNode) {
    switch(currentAlgorithm) {
        case BFS:
            return bfs(startNode, endNode);
        case DFS:
            return dfs(startNode, endNode);
        case DIJKSTRA:
            return dijkstra(startNode, endNode);
        default:
            return false;
    }
}

int main() {
    // Set up console
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    
    // Set console size - increased height to accommodate header
    SMALL_RECT windowSize = {0, 0, 80, 45};
    SetConsoleWindowInfo(hConsole, TRUE, &windowSize);
    
    // Set buffer size - increased height to match window
    COORD bufferSize = {81, 46};
    SetConsoleScreenBufferSize(hConsole, bufferSize);
    
    // Hide cursor
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = false;
    SetConsoleCursorInfo(hConsole, &cursorInfo);

    // Clear screen once at start
    system("cls");

    initializeGrid();
    Node* startNode = nullptr;
    Node* endNode = nullptr;
    int cursorX = 0, cursorY = 0;

    // Draw initial grid
    drawGrid(cursorY, cursorX, startNode, endNode);

    while (true) {
        if (_kbhit()) {
            int key = _getch();
            bool needRedraw = false;

            if (key == 224 || key == 0) { // Arrow key prefix
                key = _getch();
                switch (key) {
                    case 72: // Up
                        if (cursorY > 0) {
                            cursorY--;
                            needRedraw = true;
                        }
                        break;
                    case 80: // Down
                        if (cursorY < ROWS - 1) {
                            cursorY++;
                            needRedraw = true;
                        }
                        break;
                    case 75: // Left
                        if (cursorX > 0) {
                            cursorX--;
                            needRedraw = true;
                        }
                        break;
                    case 77: // Right
                        if (cursorX < COLS - 1) {
                            cursorX++;
                            needRedraw = true;
                        }
                        break;
                }
            } else {
                switch (key) {
                    case ' ': // Toggle obstacle
                        if (&grid[cursorY][cursorX] != startNode && 
                            &grid[cursorY][cursorX] != endNode) {
                            grid[cursorY][cursorX].isObstacle = !grid[cursorY][cursorX].isObstacle;
                            needRedraw = true;
                        }
                        break;
                    case 's':
                    case 'S':
                        if (!grid[cursorY][cursorX].isObstacle) {
                            if (startNode) startNode->isVisited = false;
                            startNode = &grid[cursorY][cursorX];
                            startNode->isVisited = true;
                            needRedraw = true;
                        }
                        break;
                    case 'e':
                    case 'E':
                        if (!grid[cursorY][cursorX].isObstacle) {
                            endNode = &grid[cursorY][cursorX];
                            needRedraw = true;
                        }
                        break;
                    case 'f':
                    case 'F':
                        if (startNode && endNode) {
                            findPath(startNode, endNode);
                            needRedraw = true;
                        }
                        break;
                    case 'a':
                    case 'A':
                        // Cycle through algorithms
                        currentAlgorithm = Algorithm((currentAlgorithm + 1) % 3);
                        needRedraw = true;
                        break;
                    case 'q':
                    case 'Q':
                        return 0;
                }
            }

            if (needRedraw) {
                drawGrid(cursorY, cursorX, startNode, endNode);
            }
        }
        Sleep(10);  // Reduced main loop delay
    }
    return 0;
}