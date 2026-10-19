#include <iostream>
#include <string>

void showBoard(const char board[]) {
    std::cout << '\n';

    for (int row = 0; row < 3; ++row) {
        for (int col = 0; col < 3; ++col) {
            const int index = row * 3 + col;

            if (board[index] == ' ') {
                std::cout << ' ' << index + 1 << ' ';
            } else {
                std::cout << ' ' << board[index] << ' ';
            }

            if (col < 2) std::cout << '|';
        }

        std::cout << '\n';
        if (row < 2) std::cout << "---+---+---\n";
    }
    std::cout << '\n';
}

bool hasWon(const char board[], char mark) {
    const int wins[8][3] = {
        {0, 1, 2}, {3, 4, 5}, {6, 7, 8},
        {0, 3, 6}, {1, 4, 7}, {2, 5, 8},
        {0, 4, 8}, {2, 4, 6}
    };

    for (const auto& line : wins) {
        if (board[line[0]] == mark &&
            board[line[1]] == mark &&
            board[line[2]] == mark) {
            return true;
        }
    }

    return false;
}

int readMove(const char board[], char player) {
    while (true) {
        std::cout << "اللاعب " << player
                  << "، اختر خانة من 1 إلى 9: ";

        std::string input;
        if (!std::getline(std::cin, input)) {
            return -1;
        }

        try {
            std::size_t used = 0;
            int choice = std::stoi(input, &used);

            while (used < input.size() &&
                   (input[used] == ' ' || input[used] == '\t')) {
                ++used;
            }

            if (used != input.size() || choice < 1 || choice > 9) {
                std::cout << "أدخل رقمًا من 1 إلى 9.\n";
                continue;
            }

            if (board[choice - 1] != ' ') {
                std::cout << "هذه الخانة مأخوذة؛ اختر خانة أخرى.\n";
                continue;
            }

            return choice - 1;
        } catch (...) {
            std::cout << "إدخال غير صالح؛ أدخل رقمًا من 1 إلى 9.\n";
        }
    }
}

bool playRound() {
    char board[9] = {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '};
    char player = 'X';

    for (int turn = 0; turn < 9; ++turn) {
        showBoard(board);

        const int move = readMove(board, player);
        if (move == -1) return false;

        board[move] = player;

        if (hasWon(board, player)) {
            showBoard(board);
            std::cout << "فاز اللاعب " << player << "!\n";
            return true;
        }

        player = (player == 'X') ? 'O' : 'X';
    }

    showBoard(board);
    std::cout << "تعادل!\n";
    return true;
}

int main() {
    std::cout << "=== لعبة XO ===\n"
              << "لاختيار خانة، أدخل رقمها كما يظهر على اللوحة.\n";

    while (true) {
        if (!playRound()) {
            std::cout << "\nانتهى الإدخال؛ تم إيقاف اللعبة.\n";
            return 0;
        }

        std::cout << "\nهل تريد اللعب مرة أخرى؟ (y/n): ";
        std::string answer;

        if (!std::getline(std::cin, answer) ||
            answer == "n" || answer == "N") {
            break;
        }

        if (answer != "y" && answer != "Y") {
            std::cout << "تم إنهاء اللعبة.\n";
            break;
        }
    }

    std::cout << "شكرًا للعب!\n";
    return 0;
}
