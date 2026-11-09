# Tic-Tac-Toe (C++)

A two-player console Tic-Tac-Toe game, written in C++ as my Day 2 project while learning the language.

## How to play
Choose two players or play against the computer (you are X, the computer is O). Type a number from 1 to 9 to pick a square. Get three in a row (across, down, or diagonal) to win. If the board fills up with no winner, it's a draw. When a game ends you can choose to play again.

## Build and run

```bash
g++ -std=c++17 -o tictactoe main.cpp
./tictactoe          # on Windows: .\tictactoe.exe
```

You need a C++ compiler such as g++ (MinGW on Windows) or clang++.
