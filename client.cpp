#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sstream>

const int PORT = 8080;
const int BOARD_SIZE = 3;

void printBoard(const std::string &board)
{
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            std::cout << board[i * BOARD_SIZE + j];
            if (j < BOARD_SIZE - 1)
                std::cout << "|";
        }
        std::cout << std::endl;
        if (i < BOARD_SIZE - 1)
            std::cout << "-----" << std::endl;
    }
}

void sendMessage(int socket, const std::string &message)
{
    send(socket, message.c_str(), message.length(), 0);
}

std::string receiveMessage(int socket)
{
    char buffer[1024] = {0};
    int valread = recv(socket, buffer, 1024, 0);
    if (valread <= 0)
    {
        return "";
    }
    std::string message(buffer, valread);
    return message;
}

int main()
{
    int sock = 0;
    struct sockaddr_in serv_addr;

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        std::cout << "Socket creation error" << std::endl;
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
    {
        std::cout << "Invalid address/ Address not supported" << std::endl;
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        std::cout << "Connection Failed" << std::endl;
        return -1;
    }

    std::cout << "Connected to the game server." << std::endl;

    int player_number = 0;

    while (true)
    {
        std::string message = receiveMessage(sock);
        if (message.empty())
        {
            std::cout << "Server disconnected" << std::endl;
            break;
        }

        std::istringstream iss(message);
        std::string command;
        iss >> command;

        if (command == "PLAYER")
        {
            iss >> player_number;
            std::cout << "You are Player " << player_number << std::endl;
        }
        else if (command == "BOARD")
        {
            std::string board_state;
            std::getline(iss, board_state);
            board_state = board_state.substr(1);
            std::cout << "Current board:" << std::endl;
            printBoard(board_state);
        }
        else if (command == "TURN")
        {
            int turn;
            iss >> turn;
            std::cout << "Player " << turn << "'s turn." << std::endl;
            if (turn == player_number)
            {
                int row, col;
                std::cout << "Enter your move (row and column, e.g., '1 2'): ";
                std::cin >> row >> col;
                sendMessage(sock, std::to_string(row - 1) + std::to_string(col - 1));
            }
        }
        else if (command == "INVALID")
        {
            std::cout << "Invalid move. Try again." << std::endl;
        }
        else if (command == "WIN")
        {
            int winner;
            iss >> winner;
            std::cout << "Player " << winner << " wins!" << std::endl;
            break;
        }
        else if (command == "DRAW")
        {
            std::cout << "It's a draw!" << std::endl;
            break;
        }
        else
        {
            std::cout << message;
        }
    }

    close(sock);
    return 0;
}
