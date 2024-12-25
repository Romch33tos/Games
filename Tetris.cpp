#include <iostream>
#include <conio.h> // _kbhit() и _getch()
#include <windows.h> // Sleep()
#include <vector>
#include <cstdlib> // rand() и srand()
#include <ctime> // time()

using namespace std;

const int gamefieldWidth = 10;
const int gamefieldHeight = 20;

class Tetris {
private:
    vector<vector<int>> board;
    int currentPiece, currentRotation;
    int currentX, currentY;
    bool gameover, fastDrop;
    int score;

    const vector<vector<vector<int>>> shapes = {
        // I
        {
            {1, 1, 1, 1},
            {0, 0, 0, 0},
            {0, 0, 0, 0},
            {0, 0, 0, 0}
        },
        {
            {0, 0, 1, 0},
            {0, 0, 1, 0},
            {0, 0, 1, 0},
            {0, 0, 1, 0}
        },
        {
            {0, 0, 0, 0},
            {1, 1, 1, 1},
            {0, 0, 0, 0},
            {0, 0, 0, 0}
        },
        {
            {0, 0, 0, 0},
            {0, 0, 1, 0},
            {0, 0, 1, 0},
            {0, 0, 1, 0}
        },

        // O
        {
            {1, 1},
            {1, 1}
        },
        {
            {1, 1},
            {1, 1}
        },
        {
            {1, 1},
            {1, 1}
        },
        {
            {1, 1},
            {1, 1}
        },

        // S
        {
            {0, 1, 1},
            {1, 1, 0},
            {0, 0, 0}
        },
        {
            {0, 1, 0},
            {0, 1, 1},
            {0, 0, 1}
        },
        {
            {0, 1, 1},
            {1, 1, 0},
            {0, 0, 0}
        },
        {
            {0, 1, 0},
            {0, 1, 1},
            {0, 0, 1}
        },

        // Z
        {
            {1, 1, 0},
            {0, 1, 1},
            {0, 0, 0}
        },
        {
            {0, 0, 1},
            {0, 1, 1},
            {0, 1, 0}
        },
        {
            {1, 1, 0},
            {0, 1, 1},
            {0, 0, 0}
        },
        {
            {0, 1, 0},
            {1, 1, 0},
            {1, 0, 0}
        },

        // L
        {
            {1, 0, 0},
            {1, 1, 1},
            {0, 0, 0}
        },
        {
            {0, 1},
            {0, 1},
            {1, 1}
        },
        {
            {0, 0, 1},
            {1, 1, 1},
            {0, 0, 0}
        },
        {
            {1, 1},
            {1, 0},
            {1, 0}
        },

        // J
        {
            {0, 0, 1},
            {1, 1, 1},
            {0, 0, 0}
        },
        {
            {1, 0},
            {1, 0},
            {1, 1}
        },
        {
            {0, 0, 0},
            {1, 1, 1},
            {0, 0, 1}
        },
        {
            {1, 1},
            {0, 1},
            {0, 1}
        },

        // T
        {
            {0, 1, 0},
            {1, 1, 1},
            {0, 0, 0}
        },
        {
            {0, 1},
            {1, 1},
            {0, 1}
        },
        {
            {0, 0, 0},
            {1, 1, 1},
            {0, 1, 0}
        },
        {
            {1, 0},
            {1, 1},
            {1, 0}
        }
    };

public:
    Tetris() {
        startGame();
    }

    void startGame() {
        board.resize(gamefieldHeight, vector<int>(gamefieldWidth, 0));
        gameover = false;
        fastDrop = false;
        score = 0;
        srand(static_cast<unsigned int>(time(NULL)));
        spawnPiece();
    }

    void spawnPiece() {
        currentPiece = rand() % (shapes.size() / 4);
        currentRotation = 0;
        currentX = gamefieldWidth / 2 - 1;
        currentY = 0;

        if (!isValidMove(0, 0)) {
            gameover = true;
        }
    }

    bool isValidMove(int moveForX, int moveForY, int rotation = -1) {
        if (rotation == -1) {
            rotation = currentRotation;
        }
        for (int y = 0; y < shapes[currentPiece * 4 + rotation].size(); y++) {
            for (int x = 0; x < shapes[currentPiece * 4 + rotation][y].size(); x++) {
                if (shapes[currentPiece * 4 + rotation][y][x] != 0) {
                    int newX = currentX + x + moveForX;
                    int newY = currentY + y + moveForY;
                    if (newX < 0 || newX >= gamefieldWidth || newY >= gamefieldHeight || (newY >= 0 && board[newY][newX])) {
                        return false;
                    }
                }
            }
        }
        return true;
    }

    void rotatePiece() {
        int newRotation = (currentRotation + 1) % 4; // 4 - количество вращений для каждой фигуры
        if (isValidMove(0, 0, newRotation)) {
            currentRotation = newRotation;
        }
    }

    void mergePiece() {
        for (int y = 0; y < shapes[currentPiece * 4 + currentRotation].size(); y++) {
            for (int x = 0; x < shapes[currentPiece * 4 + currentRotation][y].size(); x++) {
                if (shapes[currentPiece * 4 + currentRotation][y][x] != 0) {
                    board[currentY + y][currentX + x] = shapes[currentPiece * 4 + currentRotation][y][x];
                }
            }
        }
    }

    void clearLines() {
        for (int y = gamefieldHeight - 1; y >= 0; y--) {
            bool fullLine = true;
            for (int x = 0; x < gamefieldWidth; x++) {
                if (board[y][x] == 0) {
                    fullLine = false;
                    break;
                }
            }
            if (fullLine) {
                board.erase(board.begin() + y);
                board.insert(board.begin(), vector<int>(gamefieldWidth, 0));
                score += 100;
                y++;
            }
        }
    }

    void display() {
        system("CLS");
        cout << "Score: " << score << endl;
        for (int y = 0; y < gamefieldHeight; y++) {
            cout << "|";
            for (int x = 0; x < gamefieldWidth; x++) {
                if (board[y][x] != 0) {
                    cout << "#";
                } else {
                    bool isCurrentPiece = false;
                    for (int py = 0; py < shapes[currentPiece * 4 + currentRotation].size(); py++) {
                        for (int px = 0; px < shapes[currentPiece * 4 + currentRotation][py].size(); px++) {
                            if (shapes[currentPiece * 4 + currentRotation][py][px] != 0) {
                                if (currentX + px == x && currentY + py == y) {
                                    cout << "#";
                                    isCurrentPiece = true;
                                }
                            }
                        }
                    }
                    if (!isCurrentPiece) {
                        cout << " ";
                    }
                }
            }
            cout << "|" << endl;
        }
    }

    void run() {
        while (true) {
            while (!gameover) {
                display();

                if (_kbhit()) {
                    switch (_getch()) {
                        case 75: // Влево
                            if (isValidMove(-1, 0)) currentX--;
                            break;
                        case 77: // Вправо
                            if (isValidMove(1, 0)) currentX++;
                            break;
                        case 80: // Вниз
                            fastDrop = true;
                            break;
                        case 72: // Вращение
                            rotatePiece();
                            break;
                    }
                } else {
                    fastDrop = false;
                }

                if (fastDrop) {
                    while (isValidMove(0, 1)) {
                        currentY++;
                    }
                } else if (isValidMove(0, 1)) {
                    currentY++;
                } else {
                    mergePiece();
                    clearLines();
                    spawnPiece();
                }

                Sleep(100);
            }

            cout << "Game Over! Your score: " << score << endl;
            break;
        }
    }
};

int main() {
    Tetris game;
    game.run();
    return 0;
}
