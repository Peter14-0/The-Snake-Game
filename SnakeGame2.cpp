#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <math.h>
#include <algorithm>
#include <windows.h>
#include <conio.h>
#include <thread>
#include <ctime>

using namespace std;

bool gameOver;
const int width = 40, height = 20;
int x, y, fruitX, fruitY, score;
int tailX[100], tailY[100], color[100];
int nTail;
enum eDir { STOP = 0, LEFT, RIGHT, UP, DOWN }; // ������������
eDir dir;

//bool func(pair <string, int> p1, pair <string, int> p2) {
//    return p1.second < p2.second;
//}

void Setup() { // ������� ����������� ��� ����������� ���������
    gameOver = false;
    dir = STOP;
    x = width / 2;
    y = height / 2;
    fruitX = rand() % width; // ��������� ������� ��������� ���������� ������ �� x
    fruitY = rand() % height; // ��������� ������� ��������� ���������� ������ �� y
    score = 0;
}

void Draw() {
    system("cls"); // ������� ������� �������� ��� ������ ������ �-��
    for (int i = 0; i < width + 1; i++) {
        cout << '#';
    }
    cout << endl;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width + 1; j++) {
            if (j == 0) cout << '#';
            if (j == width - 1) {
                cout << '#';
            }
            if (i == y && j == x) cout << "\033[38;2;0;255;0m" << "0" << "\033[0m";
            else if (i == fruitY && j == fruitX) cout << "\033[38;2;255;0;0m" << "F" << "\033[0m";
            else {
                bool print = false;
                for (int k = 0; k < nTail; ++k) {
                    if (tailX[k] == j && tailY[k] == i) {
                        cout << "\033[38;2;" << color[k] << ";255;0m" << "o" << "\033[0m";
                        print = true;
                    }
                }
                if (!print) cout << " ";
            }
        }
        cout << endl;
    }
    for (int i = 0; i < width + 1; i++) {
        cout << '#';
    }
    cout << endl;
    cout << endl << "Score: " << score;
    cout << endl << "Press 'l' to left the game";
    // cout << endl << "fx: " << fruitX << "   " << "fy: " << fruitY;
    // cout << endl << "sx: " << x << "   " << "sy: " << y;
    // cout << endl << dir;
}

void Input() {
    if (_kbhit()) { // ��������, ���� ������������ ����� �� �������
        switch (_getch())
        {
        case 'a':
            if (dir == RIGHT && nTail > 0) break;
            dir = LEFT;
            break;
        case 'd':
            if (dir == LEFT && nTail > 0) break;
            dir = RIGHT;
            break;
        case 'w':
            if (dir == DOWN && nTail > 0) break;
            dir = UP;
            break;
        case 's':
            if (dir == UP && nTail > 0) break;
            dir = DOWN;
            break;
        case 'l':
            gameOver = true;
            break;
        default:
            break;
        }
    }
}

void Logic() {
    int prevX = tailX[0];
    int prevY = tailY[0];
    int prevCol = color[0];
    int prev2X, prev2Y, prev2Col;
    tailX[0] = x; tailY[0] = y; prevCol = 0;
    for (int i = 1; i < nTail; i++) {
        prev2X = tailX[i];
        prev2Y = tailY[i];
        prev2Col = color[i];
        tailX[i] = prevX;
        tailY[i] = prevY;
        if (prevCol + 20 < 120) color[i] = prevCol + 20;
        else if (100 < prevCol < 240) color[i] = prevCol + 10;
        if (prevCol + 5 > 255) color[i] = 255;
        prevX = prev2X;
        prevY = prev2Y;
        prevCol = prev2Col;
    }
    switch (dir)
    {
    case LEFT:
        x--;
        break;
    case RIGHT:
        x++;
        break;
    case UP:
        y--;
        break;
    case DOWN:
        y++;
        break;
    }
    if (x == fruitX && y == fruitY) {
        score += 10;
        nTail++;
        fruitX = rand() % (width - 1);
        fruitY = rand() % height;
        for (int i = 0; i < nTail - 1; ++i) {
            if (fruitX == tailX[i] && fruitY == tailY[i]) {
                fruitX = rand() % (width - 1);
                fruitY = rand() % height;
            }
        }
    }
    for (int i = 0; i < nTail; ++i) {
        if (tailX[i] == x && tailY[i] == y) gameOver = true;
    }
    if (x == width - 1) x = 0;
    else if (x == -1) x = width - 2;
    if (y == height) y = 0;
    else if (y == -1) y = height - 1;
}

int main() {
    srand(time(NULL) % 100);
    SetConsoleCP(1251); // ��������� ��������� �� Windows 1251
    SetConsoleOutputCP(1251);
    string pl; getline(cin, pl); // ��� ������
    cout << endl;
    Setup();
    while (!gameOver) {
        Draw();
        Input();
        Logic();
        if (dir == LEFT || dir == RIGHT) this_thread::sleep_for(30ms);
        else this_thread::sleep_for(80ms);
    }
    cout << endl << "Game Over!" << endl;
    cout << pl << ", your score is " << score << endl;
    ifstream file("Leader_board.txt");
    string name; int sc = score;
    vector <pair <string, int>> board;
    while (file >> name >> sc) {
        board.push_back({ name, sc });
    }
    file.close();
    if (board.size() == 0) board.push_back({ pl, sc });
    else {
        bool found = false;
        for (int i = 0; i < board.size(); ++i) {
            if (board[i].first == pl) {
                if (board[i].second < score) board[i].second = score;
                found = true;
            }
        }
        if (!found) board.push_back({ pl, score });
    }
    sort(board.begin(), board.end(), [](auto a, auto b) { return a.second < b.second; });
    ofstream file1("Leader_board.txt");
    for (int i = board.size() - 1; i >= 0; --i) {
        file1 << board[i].first << " " << board[i].second << endl;
        cout << board[i].first << " " << board[i].second << endl;
    }
    file1.close();
    _getch();
    return 0;
}
//1 ����������� ����� ������ � ������ ����                          YES
//2 ������� � ����� �� ����� ���������� ������������ ����� �� ����  YES
//3 ������ ����� � ��������� ����                                   YES
//4 ��� ��������� ����� ������� ������� � ������
//
//���� 20
//John deer 15
//���� 5
//������ 2
//
//5 ������� ������� ������ ������� ������ ��������� �������� �����
//6 ���� ������ �� ������ �� ������ � �������� �� �������(������ �������, ��������� ����� �������
//    ������, ��������� ��� ������ � ��� �����)