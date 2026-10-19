#include <iostream>
#include <cstdlib>
#include <ctime>
#include <limits>

void playGuessingGame() {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    int choice = 0;
    int maxRange = 100;
    int maxAttempts = 0;

    // اختيار مستوى الصعوبة
    std::cout << "\n========================================\n";
    std::cout << "      مرحباً بك في لعبة التخمين المفتورة!     \n";
    std::cout << "========================================\n";
    std::cout << "اختر مستوى الصعوبة:\n";
    std::cout << "1. سهل (الرقم بين 1 و 50 - محاولات غير محدودة)\n";
    std::cout << "2. متوسط (الرقم بين 1 و 100 - 7 محاولات)\n";
    std::cout << "3. صعب (الرقم بين 1 و 200 - 6 محاولات)\n";
    std::cout << "ادخل رقم الخيار (1-3): ";
    
    std::cin >> choice;

    if (choice == 1) {
        maxRange = 50;
        maxAttempts = 999; // مفتوح تقريباً
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

    std::cout << "\nتم اختيار رقم بين 1 و " << maxRange << ". ابدأ التخمين!\n";
    if (maxAttempts < 100) {
        std::cout << "لديك " << maxAttempts << " محاولات فقط للفوز!\n\n";
    }

    while (attempts < maxAttempts || maxAttempts >= 100) {
        std::cout << "المحاولة [" << (attempts + 1) << (maxAttempts < 100 ? "/" + std::to_string(maxAttempts) : "") << "] - ادخل تخمينك: ";
        
        if (!(std::cin >> guess)) {
            std::cout << "خطأ: يرجى إدخال رقم صحيح.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        attempts++;

        if (guess > secretNumber) {
            std::cout << "رقم كبير جداً! 🔽\n\n";
        } else if (guess < secretNumber) {
            std::cout << "رقم صغير جداً! 🔼\n\n";
        } else {
            hasWon = true;
            break;
        }
    }

    if (hasWon) {
        std::cout << "\n مبروك! لقد عثرت على الرقم الصحيح (" << secretNumber << ") في " << attempts << " محاولات!\n";
        if (attempts <= 3) {
            std::cout << "تقييمك: عبقري! أداء اسطوري 🌟\n";
        } else if (attempts <= 6) {
            std::cout << "تقييمك: ممتاز! ذكاء حاد 👏\n";
        } else {
            std::cout << "تقييمك: جيد، حاول تحسين رقم محاولاتك في المرة القادمة 👍\n";
        }
    } else {
        std::cout << "\n انتهت المحاولات! لقد خسرت. الرقم الصحيح كان: " << secretNumber << " ❌\n";
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

    std::cout << "\nشكراً لاستخدامك اللعبة! نتمنى لك وقتاً ممتعاً.\n";
    return 0;
}
