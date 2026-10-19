#include <iostream>
#include <cstdlib>
#include <ctime>
#include <limits>

void playGuessingGame() {
    std::srand(static_cast<unsigned int>(std::time(0)));

    int choice = 0;
    int maxRange = 100;
    int maxAttempts = 0;

    std::cout << "\n========================================\n";
    std::cout << "      مرحباً بك في لعبة التخمين!     \n";
    std::cout << "========================================\n";
    std::cout << "اختر مستوى الصعوبة:\n";
    std::cout << "1. سهل (الرقم بين 1 و 50)\n";
    std::cout << "2. متوسط (الرقم بين 1 و 100)\n";
    std::cout << "3. صعب (الرقم بين 1 و 200)\n";
    std::cout << "ادخل رقم الخيار (1-3): ";
    
    std::cin >> choice;

    if (choice == 1) {
        maxRange = 50;
        maxAttempts = 999;
    } else if (choice == 3) {
        maxRange = 200;
        maxAttempts = 6;
    } else {
        maxRange = 100;
        maxAttempts = 7;
    }

    int secretNumber = std::rand() % maxRange + 1;
    int guess = 0;
    int attempts = 0;
    bool hasWon = false;

    std::cout << "\nتم اختيار رقم بين 1 و " << maxRange << ". ابدأ التخمين!\n\n";

    while (attempts < maxAttempts) {
        std::cout << "المحاولة [" << (attempts + 1) << "/" << maxAttempts << "] - ادخل تخمينك: ";
        
        if (!(std::cin >> guess)) {
            std::cout << "خطأ: يرجى إدخال رقم صحيح.\n";
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            continue;
        }

        attempts++;

        if (guess > secretNumber) {
            std::cout << "رقم كبير جداً! حاول برقم أصغر.\n\n";
        } else if (guess < secretNumber) {
            std::cout << "رقم صغير جداً! حاول برقم أكبر.\n\n";
        } else {
            hasWon = true;
            break;
        }
    }

    if (hasWon) {
        std::cout << "\n مبروك! لقد عثرت على الرقم الصحيح (" << secretNumber << ") في " << attempts << " محاولات!\n";
    } else {
        std::cout << "\n انتهت المحاولات! الرقم الصحيح كان: " << secretNumber << " ❌\n";
    }
    std::cout << "----------------------------------------\n";
}

int main() {
    char playAgain;
    do {
        playGuessingGame();
        std::cout << "هل تريد اللعب مرة أخرى؟ (y/n): ";
        std::cin >> playAgain;
    } while (playAgain == 'y' || playAgain == 'Y');

    std::cout << "\nشكراً لاستخدامك اللعبة!\n";
    return 0;
}
