#include <iomanip>

using namespace std;

class SudokuSolver {
private:
    vector<vector<int>> board;

public:
    SudokuSolver() {
        board.resize(9, vector<int>(9, 0));
    }

    bool loadPuzzle(const string& filename) {
        ifstream infile(filename);

        if (!infile) {
            cerr << "Error: Cannot open file " << filename << endl;
            return false;
        }

        for (int row = 0; row < 9; row++) {
            for (int col = 0; col < 9; col++) {
                if (!(infile >> board[row][col])) {
                    cerr << "Error: Invalid puzzle format in "
                         << filename << endl;
                    return false;
                }
            }
        }

        infile.close();
        return true;
    }

    void printBoard() const {
        for (int row = 0; row < 9; row++) {
            if (row % 3 == 0 && row != 0) {
                cout << "------+-------+------" << endl;
            }

            for (int col = 0; col < 9; col++) {
                if (col % 3 == 0 && col != 0) {
                    cout << "| ";
                }

                cout << board[row][col] << " ";
            }
            cout << endl;
        }
    }

    bool isValid(int row, int col, int num) const {
        // Check row
        for (int c = 0; c < 9; c++) {
            if (board[row][c] == num) {
                return false;
            }
        }

        // Check column
        for (int r = 0; r < 9; r++) {
            if (board[r][col] == num) {
                return false;
            }
        }

        // Check 3x3 block
        int startRow = row - row % 3;
        int startCol = col - col % 3;

        for (int r = startRow; r < startRow + 3; r++) {
            for (int c = startCol; c < startCol + 3; c++) {
                if (board[r][c] == num) {
                    return false;
                }
            }
        }

        return true;
    }

    bool findEmptyCell(int& row, int& col) const {
        for (row = 0; row < 9; row++) {
            for (col = 0; col < 9; col++) {
                if (board[row][col] == 0) {
                    return true;
                }
            }
        }
        return false;
    }

    bool solve() {
        int row, col;

        if (!findEmptyCell(row, col)) {
            return true; // Puzzle solved
        }

        for (int num = 1; num <= 9; num++) {
            if (isValid(row, col, num)) {
                board[row][col] = num;

                if (solve()) {
                    return true;
                }

                // Backtrack
                board[row][col] = 0;
            }
        }

        return false;
    }
};

int main() {
    const string puzzleFiles[] = {
        "puzzle1.txt",
        "puzzle2.txt",
        "puzzle3.txt",
        "puzzle4.txt",
        "puzzle5.txt"
    };

    for (const string& filename : puzzleFiles) {
        cout << "\n========================================\n";
        cout << "Processing: " << filename << endl;
        cout << "========================================\n";

        SudokuSolver sudoku;

        if (!sudoku.loadPuzzle(filename)) {
            cout << "Skipping file.\n";
            continue;
        }

        cout << "\nInitial Puzzle:\n";
        sudoku.printBoard();

        if (sudoku.solve()) {
            cout << "\nSolved Puzzle:\n";
            sudoku.printBoard();
        } else {
            cout << "\nNo solution found.\n";
        }

        cout << endl;
    }

    return 0;
}