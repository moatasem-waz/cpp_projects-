#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

struct Transaction {
    int id;
    bool income;       // true للدخل، false للمصروف
    double amount;
    std::string category;
    std::string note;
};

const std::string DATA_FILE = "expenses.txt";

std::string cleanText(std::string text) {
    for (char& ch : text) {
        if (ch == '\t' || ch == '\n' || ch == '\r') {
            ch = ' ';
        }
    }
    return text;
}

std::vector<Transaction> loadTransactions() {
    std::vector<Transaction> items;
    std::ifstream file(DATA_FILE);
    std::string line;

    while (std::getline(file, line)) {
        std::istringstream row(line);
        Transaction item{};
        int incomeValue = 0;

        if (row >> item.id >> incomeValue >> item.amount) {
            row.ignore(1, '\t');
            std::getline(row, item.category, '\t');
            std::getline(row, item.note);

            if (item.amount >= 0 && !item.category.empty()) {
                item.income = (incomeValue == 1);
                items.push_back(item);
            }
        }
    }

    return items;
}

bool saveTransactions(const std::vector<Transaction>& items) {
    std::ofstream file(DATA_FILE);

    if (!file) {
        std::cerr << "تعذر حفظ البيانات.\n";
        return false;
    }

    for (const Transaction& item : items) {
        file << item.id << '\t'
             << (item.income ? 1 : 0) << '\t'
             << item.amount << '\t'
             << cleanText(item.category) << '\t'
             << cleanText(item.note) << '\n';
    }

    return static_cast<bool>(file);
}

int nextId(const std::vector<Transaction>& items) {
    int largest = 0;
    for (const Transaction& item : items) {
        if (item.id > largest) largest = item.id;
    }
    return largest + 1;
}

int readInt(const std::string& prompt) {
    while (true) {
        std::cout << prompt;
        std::string line;
        std::getline(std::cin, line);

        try {
            std::size_t used = 0;
            int value = std::stoi(line, &used);

            while (used < line.size() &&
                   (line[used] == ' ' || line[used] == '\t')) {
                ++used;
            }

            if (used == line.size()) return value;
        } catch (...) {
        }

        std::cout << "أدخل رقمًا صحيحًا.\n";
    }
}

double readAmount() {
    while (true) {
        std::cout << "المبلغ: ";
        std::string line;
        std::getline(std::cin, line);

        try {
            std::size_t used = 0;
            double amount = std::stod(line, &used);

            while (used < line.size() &&
                   (line[used] == ' ' || line[used] == '\t')) {
                ++used;
            }

            if (used == line.size() &&
                std::isfinite(amount) &&
                amount > 0) {
                return amount;
            }
        } catch (...) {
        }

        std::cout << "أدخل مبلغًا موجبًا وصحيحًا.\n";
    }
}

void addTransaction(std::vector<Transaction>& items, bool income) {
    const double amount = readAmount();

    std::cout << "الفئة (مثل راتب، طعام، مواصلات): ";
    std::string category;
    std::getline(std::cin, category);
    category = cleanText(category);

    if (category.empty()) {
        std::cout << "الفئة مطلوبة؛ لم تُسجل العملية.\n";
        return;
    }

    std::cout << "ملاحظة (اختياري): ";
    std::string note;
    std::getline(std::cin, note);

    items.push_back({
        nextId(items),
        income,
        amount,
        category,
        cleanText(note)
    });

    if (saveTransactions(items)) {
        std::cout << "تم تسجيل العملية وحفظها.\n";
    }
}

void showTransactions(const std::vector<Transaction>& items) {
    if (items.empty()) {
        std::cout << "لا توجد عمليات مسجلة.\n";
        return;
    }

    std::cout << "\n--- العمليات المالية ---\n";
    for (const Transaction& item : items) {
        std::cout << "رقم " << item.id
                  << " | " << (item.income ? "دخل" : "مصروف")
                  << " | المبلغ: " << std::fixed << std::setprecision(2)
                  << item.amount
                  << " | الفئة: " << item.category;

        if (!item.note.empty()) {
            std::cout << " | ملاحظة: " << item.note;
        }

        std::cout << '\n';
    }
}

void showSummary(const std::vector<Transaction>& items) {
    double totalIncome = 0;
    double totalExpenses = 0;

    for (const Transaction& item : items) {
        if (item.income) {
            totalIncome += item.amount;
        } else {
            totalExpenses += item.amount;
        }
    }

    std::cout << std::fixed << std::setprecision(2)
              << "إجمالي الدخل: " << totalIncome << '\n'
              << "إجمالي المصروفات: " << totalExpenses << '\n'
              << "الرصيد: " << totalIncome - totalExpenses << '\n';
}

void deleteTransaction(std::vector<Transaction>& items) {
    const int id = readInt("أدخل رقم العملية المراد حذفها: ");

    for (auto it = items.begin(); it != items.end(); ++it) {
        if (it->id == id) {
            items.erase(it);
            if (saveTransactions(items)) {
                std::cout << "تم حذف العملية.\n";
            }
            return;
        }
    }

    std::cout << "لم يتم العثور على عملية بهذا الرقم.\n";
}

int main() {
    std::vector<Transaction> items = loadTransactions();

    while (true) {
        std::cout << "\n=== منظّم المصروفات ===\n"
                  << "1. تسجيل دخل\n"
                  << "2. تسجيل مصروف\n"
                  << "3. عرض العمليات\n"
                  << "4. عرض الملخص والرصيد\n"
                  << "5. حذف عملية\n"
                  << "0. خروج\n";

        const int choice = readInt("اختر: ");

        switch (choice) {
            case 1:
                addTransaction(items, true);
                break;
            case 2:
                addTransaction(items, false);
                break;
            case 3:
                showTransactions(items);
                break;
            case 4:
                showSummary(items);
                break;
            case 5:
                deleteTransaction(items);
                break;
            case 0:
                saveTransactions(items);
                std::cout << "تم حفظ البيانات. إلى اللقاء!\n";
                return 0;
            default:
                std::cout << "خيار غير صحيح.\n";
        }
    }
}
