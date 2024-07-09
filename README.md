# TicTacToe Multiplayer Game

Welcome to the TicTacToe Multiplayer Game. This application allows two players to connect and play TicTacToe over a network.

## What does it do?

This app brings the classic game of TicTacToe to your computer, allowing two players to compete from separate terminals. It offers the following features:

- 🎮 Connects two players over a network
- 🕹 Allows turn-based gameplay of TicTacToe
- 📋 Displays the current state of the game board after each move
- 🚫 Detects invalid moves and prompts the player to try again
- 🏆 Declares the winner or a draw at the end of the game

## Why I made it

I wanted to create a simple networked game that could be played between two people on different machines. TicTacToe was the perfect choice because it's easy to implement and fun to play. This project also gave me a chance to work with socket programming in C++.

## How to Use

To use this TicTacToe Multiplayer Game, follow these steps:

1. Make sure you have a C++ compiler installed (like g++).
2. Clone this repository.
3. Build the server and client by executing the following command in your terminal:
   ```sh
   make all_clean
   ```
4. Start the server by running:
   ```sh
   make run_server
   ```
5. In two separate terminals, start the clients by running:
   ```sh
   make run_client
   ```
6. The clients will connect to the server and the game will start. Each player will be prompted to enter their moves (row and column) in their respective terminals.
7. The game will display the current board state after each move, and will notify the players of invalid moves, wins, or draws.
