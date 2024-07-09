#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <vector>
#include <sstream>

const int PORT = 8080;
const int BOARD_SIZE = 3;

class TicTacToe
{
private:
    std::vector<std::vector<char>> board;
    char currentPlayer;

public:
    TicTacToe() : board(BOARD_SIZE, std::vector<char>(BOARD_SIZE, ' ')), currentPlayer('X') {}

    std::string getBoardState() const
    {
        std::stringstream ss;
        for (const auto &row : board)
        {
            for (char cell : row)
            {
                ss << cell;
            }
        }
        return ss.str();
    }

    bool makeMove(int row, int col)
    {
        if (row < 0 || row >= BOARD_SIZE || col < 0 || col >= BOARD_SIZE || board[row][col] != ' ')
            return false;

        board[row][col] = currentPlayer;
        currentPlayer = (currentPlayer == 'X') ? 'O' : 'X';
        return true;
    }

    bool checkWin() const
    {
        for (int i = 0; i < BOARD_SIZE; ++i)
        {
            if (board[i][0] != ' ' && board[i][0] == board[i][1] && board[i][1] == board[i][2])
                return true;
            if (board[0][i] != ' ' && board[0][i] == board[1][i] && board[1][i] == board[2][i])
                return true;
        }
        if (board[0][0] != ' ' && board[0][0] == board[1][1] && board[1][1] == board[2][2])
            return true;
        if (board[0][2] != ' ' && board[0][2] == board[1][1] && board[1][1] == board[2][0])
            return true;
        return false;
    }

    bool isBoardFull() const
    {
        for (const auto &row : board)
        {
            for (char cell : row)
            {
                if (cell == ' ')
                    return false;
            }
        }
        return true;
    }

    char getCurrentPlayer() const
    {
        return currentPlayer;
    }
};

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
    return std::string(buffer, valread);
}

int main()
{
    int server_fd, client_sockets[2];
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 2) < 0)
    {
        perror("listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    std::cout << "Waiting for players to connect..." << std::endl;

    for (int i = 0; i < 2; i++)
    {
        if ((client_sockets[i] = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
        {
            perror("accept failed");
            close(server_fd);
            exit(EXIT_FAILURE);
        }
        std::cout << "Player " << i + 1 << " connected." << std::endl;
        sendMessage(client_sockets[i], "PLAYER " + std::to_string(i + 1) + "\n");
    }

    TicTacToe game;
    int current_player = 0;

    while (true)
    {
        std::string board_state = "BOARD " + game.getBoardState() + "\n";
        sendMessage(client_sockets[0], board_state);
        sendMessage(client_sockets[1], board_state);

        std::string turn_message = "TURN " + std::to_string(current_player + 1) + "\n";
        sendMessage(client_sockets[0], turn_message);
        sendMessage(client_sockets[1], turn_message);

        std::string move = receiveMessage(client_sockets[current_player]);
        if (move.empty())
        {
            std::cout << "Player " << current_player + 1 << " disconnected." << std::endl;
            break;
        }

        int row = move[0] - '0';
        int col = move[1] - '0';

        if (game.makeMove(row, col))
        {
            if (game.checkWin())
            {
                std::string win_message = "WIN " + std::to_string(current_player + 1) + "\n";
                sendMessage(client_sockets[0], win_message);
                sendMessage(client_sockets[1], win_message);
                break;
            }
            else if (game.isBoardFull())
            {
                sendMessage(client_sockets[0], "DRAW\n");
                sendMessage(client_sockets[1], "DRAW\n");
                break;
            }
            current_player = 1 - current_player;
        }
        else
        {
            std::string invalid_move = "INVALID\n";
            sendMessage(client_sockets[current_player], invalid_move);
        }
    }

    close(client_sockets[0]);
    close(client_sockets[1]);
    close(server_fd);
    return 0;
}
