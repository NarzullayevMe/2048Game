#pragma clang diagnostic push
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <vector>
#include <windows.h>

using namespace std;

class Game2048 {
private:
    static const int SIZE = 4;
    int score;
    vector<vector<int>> board;

#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-msc50-cpp"
    void Random() {
        vector<int> Empty;
        for (int i = 0; i < SIZE; ++i)
            for (int j = 0; j < SIZE; ++j)
                if (board[i][j] == 0)
                    Empty.push_back(i * SIZE + j);

        if (Empty.empty())
            return;

        int ind = Empty[rand() % Empty.size()];
        int val = (rand() % 10 == 0) ? 4 : 2;
        board[ind / SIZE][ind % SIZE] = val;
    }
#pragma clang diagnostic pop

    static void PrintColor(int val) {
        HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);
        int colCode;
        switch (val) {
            case 2:
                colCode = 11;
                break;
            case 4:
                colCode = 12;
                break;
            case 8:
                colCode = 13;
                break;
            case 16:
                colCode = 14;
                break;
            case 32:
                colCode = 15;
                break;
            case 64:
                colCode = 10;
                break;
            case 128:
                colCode = 9;
                break;
            case 256:
                colCode = 8;
                break;
            case 512:
                colCode = 7;
                break;
            case 1024:
                colCode = 6;
                break;
            case 2048:
                colCode = 5;
                break;
            default:
                colCode = 15;
                break;
        }
        SetConsoleTextAttribute(hCon, colCode);
        cout << setw(6) << val;
        SetConsoleTextAttribute(hCon, 15);
    }

    void MergeRow(vector<int> &row) {
        for (int i = 0; i < SIZE - 1; ++i) {
            for (int j = i + 1; j < SIZE; ++j) {
                if (row[j] != 0)
                    if (row[i] == 0) {
                        row[i] = row[j];
                        row[j] = 0;
                    }
                if (row[i] == row[j]) {
                    row[i] *= 2;
                    row[j] = 0;
                    score += row[i];
                    break;
                }
            }
        }
    }

    void ReverseAndMergeRow(vector<int> &row) {
        reverse(row.begin(), row.end());
        MergeRow(row);
        reverse(row.begin(), row.end());
    }

public:
    Game2048() : score(0), board(SIZE, vector<int>(SIZE, 0)) {
        init();
        init();
    }

#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-msc50-cpp"
    void init() {
        vector<int> Empty;
        for (int i = 0; i < SIZE; ++i)
            for (int j = 0; j < SIZE; ++j)
                if (board[i][j] == 0)
                    Empty.push_back(i * SIZE + j);

        if (Empty.empty())
            return;

        int ind = Empty[rand() % Empty.size()];
        int val = (rand() % 10 == 0) ? 4 : 2;
        board[ind / SIZE][ind % SIZE] = val;
    }
#pragma clang diagnostic pop

    static void ClearScreen() { system("cls"); }

    void PrintBoard() {
        ClearScreen();
        cout << "Score: " << score << "\n";
        cout << "+---------------------------+\n";
        for (int i = 0; i < SIZE; ++i) {
            for (int j = 0; j < SIZE; ++j) {
                cout << "|";
                int val = board[i][j];
                if (val != 0) {
                    PrintColor(val);
                } else {
                    cout << setw(6) << " ";
                }
            }
            cout << "|\n";
            cout << "+---------------------------+\n";
        }
    }

    [[nodiscard]] int GetScore() const { return score; }

    bool MoveLeft() {
        vector<vector<int>> old = board;
        for (int i = 0; i < SIZE; ++i)
            MergeRow(board[i]);
        return old != board;
    }

    bool MoveRight() {
        vector<vector<int>> old = board;
        for (int i = 0; i < SIZE; ++i)
            ReverseAndMergeRow(board[i]);
        return old != board;
    }

    bool MoveUp() {
        vector<vector<int>> old = board;
        for (int j = 0; j < SIZE; ++j) {
            vector<int> col;
            for (int i = 0; i < SIZE; ++i)
                col.push_back(board[i][j]);
            MergeRow(col);
            for (int i = 0; i < SIZE; ++i)
                board[i][j] = col[i];
        }
        return old != board;
    }

    bool MoveDown() {
        vector<vector<int>> old = board;
        for (int j = 0; j < SIZE; ++j) {
            vector<int> col;
            for (int i = 0; i < SIZE; ++i)
                col.push_back(board[i][j]);
            ReverseAndMergeRow(col);
            for (int i = 0; i < SIZE; ++i)
                board[i][j] = col[i];
        }
        return old != board;
    }

    [[nodiscard]] bool IsGameOver() const {
        for (int i = 0; i < SIZE; ++i)
            for (int j = 0; j < SIZE; ++j)
                if (board[i][j] == 0 ||
                    (i < SIZE - 1 && board[i][j] == board[i + 1][j]) ||
                    (j < SIZE - 1 && board[i][j] == board[i][j + 1]))
                    return false;
        return true;
    }

    bool Run(wchar_t key) {
        switch (key) {
            case L'w':
            case L'8':
                if (MoveUp()) {
                    Random();
                    PrintBoard();
                    return true;
                }
                break;
            case L's':
            case L'2':
                if (MoveDown()) {
                    Random();
                    PrintBoard();
                    return true;
                }
                break;
            case L'a':
            case L'4':
                if (MoveLeft()) {
                    Random();
                    PrintBoard();
                    return true;
                }
                break;
            case L'd':
            case L'6':
                if (MoveRight()) {
                    Random();
                    PrintBoard();
                    return true;
                }
                break;
            case L'7':
            case L'9':
                break;
            case VK_LEFT:
                if (MoveLeft()) {
                    Random();
                    PrintBoard();
                    return true;
                }
                break;
            case VK_RIGHT:
                if (MoveRight()) {
                    Random();
                    PrintBoard();
                    return true;
                }
                break;
            case VK_UP:
                if (MoveUp()) {
                    Random();
                    PrintBoard();
                    return true;
                }
                break;
            case VK_DOWN:
                if (MoveDown()) {
                    Random();
                    PrintBoard();
                    return true;
                }
                break;
            default:
                return false;
        }

        return false;
    }

    static void Delay(int ms) { Sleep(ms); }

    void PrintGameOutcome() const {
        wcout << L"O'yin tugadi! Yakuniy natija: " << GetScore() << L"\n";
    }
};

#pragma clang diagnostic push
#pragma ide diagnostic ignored "hicpp-multiway-paths-covered"
#pragma ide diagnostic ignored "cert-msc51-cpp"
int main() {
    srand(static_cast<unsigned>(time(nullptr)));
    Game2048 game;

    while (!game.IsGameOver()) {
        game.PrintBoard();
        int key = _getwch();
        wchar_t c = key;

        if (key == 0xE0 || key == 0) {
            key = _getwch();
            switch (key) {
                case 72:
                    c = VK_UP;
                    break;
                case 80:
                    c = VK_DOWN;
                    break;
                case 75:
                    c = VK_LEFT;
                    break;
                case 77:
                    c = VK_RIGHT;
                    break;
            }
        }

        if (game.Run(c)) {
            Game2048::Delay(10);
        } else if (c == L'q' || c == L'Q') {
            break;
        } else {
            wcout << L"Noto'g'ri tugma: " << c << L"\n";
        }
    }

    game.PrintGameOutcome();

    return 0;
}
#pragma clang diagnostic pop

#pragma clang diagnostic pop
