/**
 * Program Name: EECS 348 Assignment 4 - Sudoku Solver
 * Description: An object-oriented, recursive depth-first search program with backtracking 
 *              to solve partially completed Sudoku puzzles from text files.
 * Inputs: puzzle1.txt, puzzle2.txt, puzzle3.txt, puzzle4.txt, puzzle5.txt
 * Output: Prints the puzzle filename, initial puzzle grid, and the solved grid 
 *         (or "No solution found" if no valid configuration exists).
 * Author: Kaleb Justice Blakemore
 * Creation Date: October 15, 2026
 * Collaborators/Sources: Baseline structural approaches reviewed via GenAI (ChatGPT/Copilot); 
 *                        fully implemented, structured, and commented independently.
 */

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

// Class representing an object-oriented Sudoku board and solver
class SudokuBoard {
private:
    int grid[9][9]; // 9x9 two-dimensional array storing the Sudoku board numbers

public:
    // Constructor to initialize all cells of the board to zero
    SudokuBoard() {
        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) {
                grid[i][j] = 0;
            }
        }
    }

    // Method to load a Sudoku puzzle from a specified text file where '_' represents blanks
    bool loadPuzzle(const string& filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            return false; // Return false if file cannot be opened
        }
        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) {
                char ch;
                if (file >> ch) {
                    if (ch == '_') {
                        grid[i][j] = 0; // Store blank spaces as 0
                    } else {
                        grid[i][j] = ch - '0'; // Convert character digit to integer value
                    }
                }
            }
        }
        file.close();
        return true;
    }

    // Method to print the Sudoku board grid to the console
    void printBoard() const {
        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) {
                if (grid[i][j] == 0) {
                    cout << "_ "; // Print underscore for empty cells
                } else {
                    cout << grid[i][j] << " "; // Print the digit
                }
            }
            cout << endl;
        }
    }

    // Method to check if placing a number at a specific row and col violates Sudoku rules
    bool isValid(int row, int col, int num) const {
        // Check row and column constraints for duplicate values
        for (int x = 0; x < 9; x++) {
            if (grid[row][x] == num || grid[x][col] == num) {
                return false;
            }
        }

        // Determine the top-left corner coordinates of the 3x3 sub-grid (block)
        int startRow = row - row % 3;
        int startCol = col - col % 3;

        // Check 3x3 block constraints for duplicate values
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (grid[i + startRow][j + startCol] == num) {
                    return false;
                }
            }
        }

        return true; // The placement is valid against all constraints
    }

    // Recursive backtracking method to solve the Sudoku puzzle cell by cell
    bool solveRecursive(int row, int col) {
        // Base case: If row reaches 9, we have successfully traversed and filled the board
        if (row == 9) {
            return true;
        }

        // Transition to the next row if the current column index reaches the end (9)
        if (col == 9) {
            return solveRecursive(row + 1, 0);
        }

        // Skip cells that are pre-filled in the initial puzzle configuration
        if (grid[row][col] != 0) {
            return solveRecursive(row, col + 1);
        }

        // Iterate through candidate digits 1 through 9
        for (int num = 1; num <= 9; num++) {
            if (isValid(row, col, num)) {
                grid[row][col] = num; // Tentatively place candidate number

                // Recursively call solver for the next cell
                if (solveRecursive(row, col + 1)) {
                    return true; // Solution found down this branch
                }

                grid[row][col] = 0; // Backtrack: reset cell to 0 if candidate fails downstream
            }
        }

        return false; // Trigger backtracking if no candidate values work for this cell
    }

    // Public wrapper method to initiate the recursive solving process from the top-left cell
    bool solve() {
        return solveRecursive(0, 0);
    }
};

int main() {
    // Array containing all five required puzzle file names
    string puzzleFiles[5] = {"puzzle1.txt", "puzzle2.txt", "puzzle3.txt", "puzzle4.txt", "puzzle5.txt"};

    // Loop through each puzzle file to load, print, and solve it sequentially
    for (int i = 0; i < 5; i++) {
        cout << "Puzzle File: " << puzzleFiles[i] << endl;
        cout << "-----------------------------------" << endl;

        SudokuBoard board; // Instantiate an object for the current puzzle
        if (!board.loadPuzzle(puzzleFiles[i])) {
            cout << "Error: Could not open file " << puzzleFiles[i] << endl << endl;
            continue;
        }

        cout << "Initial Puzzle:" << endl;
        board.printBoard();
        cout << endl;

        cout << "Solution:" << endl;
        if (board.solve()) {
            board.printBoard(); // Print solved board if a solution is found
        } else {
            cout << "No solution found" << endl; // Output required message if no solution exists
        }
        cout << "\n===================================\n" << endl;
    }

    return 0;
}