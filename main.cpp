// Tic-Tac-Toe - a beginner-friendly C++ project
//
// Play against a friend or against the computer. Type a number from 1 to 9 to
// pick a square. The first player to get three in a row (across, down, or
// diagonal) wins. If the board fills up with no winner, it's a draw.
//
// Ideas used here: reading and validating input, functions, loops, and a simple
// "computer brain" (win if you can, block if you must, otherwise pick a square).

#include <cstdlib> // rand, srand, exit
#include <ctime>   // time (used to seed the random number generator)
#include <iostream>
#include <limits>
#include <string>
#include <vector>

// The board is 9 characters. At the start each square shows its own number
// ('1' to '9'); when a player picks it, it becomes 'X' or 'O'.
using Board = std::vector<char>;

Board makeBoard() {
    Board board(9);
    for (int i = 0; i < 9; i++) {
        board[i] = static_cast<char>('1' + i);
    }
    return board;
}

void printBoard(const Board& b) {
    std::cout << "\n";
    std::cout << " " << b[0] << " | " << b[1] << " | " << b[2] << "\n";
    std::cout << "---+---+---\n";
    std::cout << " " << b[3] << " | " << b[4] << " | " << b[5] << "\n";
    std::cout << "---+---+---\n";
    std::cout << " " << b[6] << " | " << b[7] << " | " << b[8] << "\n\n";
}

// Returns true if the given player ('X' or 'O') has three in a row
bool hasWon(const Board& b, char player) {
    // All 8 winning lines, written as board positions (0-8)
    const int lines[8][3] = {
        {0, 1, 2}, {3, 4, 5}, {6, 7, 8}, // rows
        {0, 3, 6}, {1, 4, 7}, {2, 5, 8}, // columns
        {0, 4, 8}, {2, 4, 6}             // diagonals
    };
    for (const auto& line : lines) {
        if (b[line[0]] == player && b[line[1]] == player && b[line[2]] == player) {
            return true;
        }
    }
    return false;
}

// A square is free if it still shows its number instead of X or O
bool isFree(const Board& b, int square) {
    return b[square] != 'X' && b[square] != 'O';
}

// If the user closes the input (Ctrl+Z then Enter on Windows, Ctrl+D elsewhere),
// stop the program politely instead of looping forever.
void quitIfInputEnded() {
    if (std::cin.eof()) {
        std::cout << "\nInput ended. Goodbye!\n";
        std::exit(0);
    }
}

// Keep asking until the player types a valid, free square (1-9).
// Returns the board position (0-8).
int askForMove(const Board& b, char player) {
    while (true) {
        std::cout << "Player " << player << ", pick a square (1-9): ";
        int choice;
        if (!(std::cin >> choice)) {
            quitIfInputEnded();
            // The player typed something that isn't a number, e.g. "abc".
            std::cin.clear(); // reset the error state
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "That's not a number. Try again.\n";
            continue;
        }
        if (choice < 1 || choice > 9) {
            std::cout << "Please choose a number from 1 to 9.\n";
            continue;
        }
        if (!isFree(b, choice - 1)) {
            std::cout << "That square is already taken. Try another.\n";
            continue;
        }
        return choice - 1;
    }
}

// Look for a square where `player` could win on their very next move.
// Returns that square (0-8), or -1 if there isn't one.
int findWinningMove(Board b, char player) { // `b` is a copy, so we can experiment freely
    for (int square = 0; square < 9; square++) {
        if (isFree(b, square)) {
            b[square] = player;       // try the move...
            bool wins = hasWon(b, player);
            b[square] = static_cast<char>('1' + square); // ...and undo it
            if (wins) {
                return square;
            }
        }
    }
    return -1;
}

// The computer's brain. It tries these steps in order:
//   1. Win right now if possible.
//   2. Block the human if they are about to win.
//   3. Take the center square.
//   4. Otherwise pick a random free square.
int computerMove(const Board& b, char computer, char human) {
    int move = findWinningMove(b, computer);
    if (move != -1) {
        return move;
    }

    move = findWinningMove(b, human);
    if (move != -1) {
        return move;
    }

    if (isFree(b, 4)) {
        return 4;
    }

    std::vector<int> freeSquares;
    for (int square = 0; square < 9; square++) {
        if (isFree(b, square)) {
            freeSquares.push_back(square);
        }
    }
    return freeSquares[std::rand() % freeSquares.size()];
}

// Ask a yes/no question. Returns true for yes, false for no.
bool askYesNo(const std::string& question) {
    while (true) {
        std::cout << question << " (y/n): ";
        std::string answer;
        if (!(std::cin >> answer)) {
            quitIfInputEnded();
            std::cin.clear();
            continue;
        }
        if (answer[0] == 'y' || answer[0] == 'Y') {
            return true;
        }
        if (answer[0] == 'n' || answer[0] == 'N') {
            return false;
        }
        std::cout << "Please type y or n.\n";
    }
}

// Ask whether to play against a friend or the computer.
// Returns true if the player wants to play against the computer.
bool askVsComputer() {
    while (true) {
        std::cout << "\nChoose a mode:\n";
        std::cout << "  1) Two players\n";
        std::cout << "  2) Play against the computer\n";
        std::cout << "Your choice: ";
        int choice;
        if (!(std::cin >> choice)) {
            quitIfInputEnded();
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "That's not a number. Try again.\n";
            continue;
        }
        if (choice == 1) {
            return false;
        }
        if (choice == 2) {
            return true;
        }
        std::cout << "Please type 1 or 2.\n";
    }
}

// Play one full game. In computer mode, the human is X and the computer is O.
void playGame(bool vsComputer) {
    Board board = makeBoard();
    char player = 'X';

    // The board has 9 squares, so the game can last at most 9 turns
    for (int turn = 0; turn < 9; turn++) {
        printBoard(board);

        int move;
        if (vsComputer && player == 'O') {
            move = computerMove(board, 'O', 'X');
            std::cout << "The computer picks square " << (move + 1) << ".\n";
        } else {
            move = askForMove(board, player);
        }
        board[move] = player;

        if (hasWon(board, player)) {
            printBoard(board);
            if (vsComputer && player == 'O') {
                std::cout << "The computer wins! Better luck next time.\n";
            } else if (vsComputer) {
                std::cout << "You win! Congratulations!\n";
            } else {
                std::cout << "Player " << player << " wins! Congratulations!\n";
            }
            return;
        }

        // Switch to the other player
        player = (player == 'X') ? 'O' : 'X';
    }

    printBoard(board);
    std::cout << "It's a draw!\n";
}

int main() {
    std::srand(static_cast<unsigned>(std::time(nullptr))); // seed the random numbers

    std::cout << "=== Tic-Tac-Toe ===\n";

    do {
        bool vsComputer = askVsComputer();
        playGame(vsComputer);
    } while (askYesNo("\nPlay again?"));

    std::cout << "Thanks for playing!\n";
    return 0;
}
