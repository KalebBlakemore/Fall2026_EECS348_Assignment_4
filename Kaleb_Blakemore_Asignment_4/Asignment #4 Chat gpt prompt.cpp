```cpp
/*
 * Program Name: EECS 348 Assignment 4 - Sudoku Solver
 * Description: Solves Sudoku puzzles using recursion and backtracking.
 * Input: puzzle1.txt through puzzle5.txt
 * Author: Kaleb Justice Blakemore
 * Date: October 15, 2026
 */

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

class SudokuBoard {
private:
    int grid[9][9];

public:
    // Initialize the board with empty cells.
    SudokuBoard() {
        for (int row = 0; row < 9; row++) {
            for (int col = 0; col < 9; col++) {
                grid[row][col] = 0;
            }
        }
    }

    // Load a puzzle from a file. Underscores represent empty cells.
    bool loadPuzzle(const string& filename) {
        ifstream file(filename);

        if (!file.is_open()) {
            return false;
        }

        for (int row = 0; row < 9; row++) {
            for (int col = 0; col < 9; col++) {
                char ch;

                if (!(file >> ch)) {
                    file.close();
                    return false;
                }

                if (ch == '_') {
                    grid[row][col] = 0;
                } else if (ch >= '1' && ch <= '9') {
                    grid[row][col] = ch - '0';
                } else {
                    file.close();
                    return false;
                }
            }
        }

        file.close();
        return true;
    }

    // Print the current Sudoku board.
    void printBoard() const {
        for (int row = 0; row < 9; row++) {
            for (int col = 0; col < 9; col++) {
                if (grid[row][col] == 0) {
                    cout << "_ ";
                } else {
                    cout << grid[row][col] << " ";
                }
            }
            cout << endl;
        }
    }

    // Check whether a number can be placed in a cell.
    bool isValid(int row, int col, int num) const {
        // Check the row and column.
        for (int i = 0; i < 9; i++) {
            if (grid[row][i] == num ||
                grid[i][col] == num) {
                return false;
            }
        }

        // Find the starting position of the 3x3 block.
        int startRow = row - row % 3;
        int startCol = col - col % 3;

        // Check the 3x3 block.
        for (int r = startRow; r < startRow + 3; r++) {
            for (int c = startCol; c < startCol + 3; c++) {
                if (grid[r][c] == num) {
                    return false;
                }
            }
        }

        return true;
    }

    // Check whether the original puzzle follows Sudoku rules.
    bool isBoardValid() const {
        for (int row = 0; row < 9; row++) {
            for (int col = 0; col < 9; col++) {
                int num = grid[row][col];

                if (num != 0) {
                    // Temporarily remove the number before checking.
                    SudokuBoard temp = *this;
                    temp.grid[row][col] = 0;

                    if (!temp.isValid(row, col, num)) {
                        return false;
                    }
                }
            }
        }

        return true;
    }

    // Solve the puzzle using recursive backtracking.
    bool solveRecursive(int row, int col) {
        // All rows have been completed.
        if (row == 9) {
            return true;
        }

        // Move to the next row when the current row is finished.
        if (col == 9) {
            return solveRecursive(row + 1, 0);
        }

        // Skip cells that already contain numbers.
        if (grid[row][col] != 0) {
            return solveRecursive(row, col + 1);
        }

        // Try numbers 1 through 9.
        for (int num = 1; num <= 9; num++) {
            if (isValid(row, col, num)) {
                grid[row][col] = num;

                // Continue solving the next cell.
                if (solveRecursive(row, col + 1)) {
                    return true;
                }

                // Undo the guess if it leads to a dead end.
                grid[row][col] = 0;
            }
        }

        // No number worked, so go back to the previous cell.
        return false;
    }

    // Start solving from the top-left corner.
    bool solve() {
        if (!isBoardValid()) {
            return false;
        }

        return solveRecursive(0, 0);
    }
};

int main() {
    string puzzleFiles[5] = {
        "puzzle1.txt",
        "puzzle2.txt",
        "puzzle3.txt",
        "puzzle4.txt",
        "puzzle5.txt"
    };

    // Process all five puzzle files in order.
    for (int i = 0; i < 5; i++) {
        cout << "Puzzle File: " << puzzleFiles[i] << endl;
        cout << "-----------------------------------" << endl;

        SudokuBoard board;

        if (!board.loadPuzzle(puzzleFiles[i])) {
            cout << "Could not load puzzle file or invalid file format."
                 << endl << endl;
            continue;
        }

        cout << "Initial Puzzle:" << endl;
        board.printBoard();
        cout << endl;

        cout << "Solution:" << endl;

        if (board.solve()) {
            board.printBoard();
        } else {
            cout << "No solution found" << endl;
        }

        cout << "\n===================================\n" << endl;
    }

    return 0;
}
```