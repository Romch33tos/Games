#include <iostream>
#include <conio.h>
#include <windows.h>
#include <thread>
#include <chrono>

using namespace std;

const int width = 20;
const int height = 10;
int x, y, fruitX, fruitY, veryBigFruittX, veryBigFruittY, score, lengthOfSnake;
int tailX[100], tailY[100];
enum eDirection { STOP = 0, LEFT, RIGHT, UP, DOWN };
eDirection dir;
bool veryBigFruittActive = false;
chrono::steady_clock::time_point veryBigFruittTime;

void setColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void drawGame() {
    system("cls");
    for (int j = 0; j < width; j++) {
        cout << "-";
    }
    cout << endl;

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (j == 0 || j == width - 1) {
                cout << "|";
            } else if (i == y && j == x) {
                setColor(10);
                cout << "O";
                setColor(7);
            } else if (i == fruitY && j == fruitX) {
                setColor(12);
                cout << "*";
                setColor(7);
            } else if (veryBigFruittActive && i == veryBigFruittY && j == veryBigFruittX) {
                setColor(12);
                cout << "@";
                setColor(7);
            } else {
                bool isItTail = false;
                for (int k = 0; k < lengthOfSnake; k++) {
                    if (tailX[k] == j && tailY[k] == i) {
                        setColor(10);
                        cout << "o";
                        isItTail = true;
                        setColor(7);
                    }
                }
                if (!isItTail) cout << " ";
            }
        }
        cout << endl;
    }

    for (int j = 0; j < width; j++) {
        cout << "-";
    }
    cout << endl;

    cout << "Score: " << score << endl;
}

void gameControl() {
    if (_kbhit()) {
        switch (_getch()) {
            case 72:
                if (dir != DOWN) dir = UP;
                break;
            case 80:
                if (dir != UP) dir = DOWN;
                break;
            case 75:
                if (dir != RIGHT) dir = LEFT;
                break;
            case 77:
                if (dir != LEFT) dir = RIGHT;
                break;
        }
    }
}

bool isGameOver() {
    for (int i = 0; i < lengthOfSnake; i++) {
        if (tailX[i] == x && tailY[i] == y) {
            return true;
        }
    }
    if (x >= width - 1 || x <= 0 || y >= height || y < 0) {
        return true;
    }
    return false;
}

bool isPositionOccupied(int posX, int posY) {
    // Проверяем, занято ли место змейкой или ее хвостом
    if (posX == x && posY == y) return true;
    for (int i = 0; i < lengthOfSnake; i++) {
        if (tailX[i] == posX && tailY[i] == posY) return true;
    }
    return false;
}

void spawnFruit() {
    do {
        fruitX = rand() % (width - 2) + 1;
        fruitY = rand() % (height - 2) + 1;
    } while (isPositionOccupied(fruitX, fruitY)); // Проверяем, не занято ли место
}

void gamefield() {
    dir = STOP;
    x = width / 2;
    y = height / 2;
    score = 0;
    lengthOfSnake = 0;
    spawnFruit(); // Изначально спавним фрукт
}

void gameLogic() {
    int prevX = tailX[0];
    int prevY = tailY[0];
    int prev2X, prev2Y;

    tailX[0] = x;
    tailY[0] = y;

    for (int i = 1; i < lengthOfSnake; i++) {
        prev2X = tailX[i];
        prev2Y = tailY[i];
        tailX[i] = prevX;
        tailY[i] = prevY;
        prevX = prev2X;
        prevY = prev2Y;
    }

    switch (dir) {
        case UP: y--; break;
        case DOWN: y++; break;
        case LEFT: x--; break;
        case RIGHT: x++; break;
    }

    if (x == fruitX && y == fruitY) {
        score += 1;
        spawnFruit(); // Спавн нового фрукта
        lengthOfSnake++;

        if (score % 5 == 0) {
            do {
                veryBigFruittX = rand() % (width - 2) + 1;
                veryBigFruittY = rand() % (height - 2) + 1;
            } while (isPositionOccupied(veryBigFruittX, veryBigFruittY)); // Проверяем, не занято ли место
            veryBigFruittActive = true;
            veryBigFruittTime = chrono::steady_clock::now();
        }
    }

    if (veryBigFruittActive && x == veryBigFruittX && y == veryBigFruittY) {
        score += 10;
        veryBigFruittActive = false;
    }

    if (veryBigFruittActive && chrono::steady_clock::now() - veryBigFruittTime >= chrono::seconds(5)) {
        veryBigFruittActive = false;
    }
}

int main() {
    gamefield();
    while (true) {
        drawGame();
        gameControl();
        gameLogic();
        if (isGameOver()) {
            cout << "Game Over! :( " << endl;
            break;
        }
        Sleep(150);
    }
    return 0;
}
