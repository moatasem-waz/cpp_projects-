#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

struct Book {
    int id;
    std::string title;
    std::string author;
    bool available;
};

const std::string DATA_FILE = "books.txt";

std::string cleanText(std::string text) {
    for (char& ch : text) {
        if (ch == '\t' || ch == '\n' || ch == '\r') {
            ch = ' ';
        }
    }
    return text;
}

std::vector<Book> loadBooks() {
    std::vector<Book> books;
    std::ifstream file(DATA_FILE);
    std::string line;

    while (std::getline(file, line)) {
        std::istringstream row(line);
        Book book{};
        int availability = 0;

        if (row >> book.id >> availability) {
            row.ignore(1, '\t');
            std::getline(row, book.title, '\t');
            std::getline(row, book.author);

            if (!book.title.empty()) {
                book.available = (availability == 1);
                books.push_back(book);
            }
        }
    }

    return books;
}

bool saveBooks(const std::vector<Book>& books) {
    std::ofstream file(DATA_FILE);

    if (!file) {
        std::cerr << "تعذر حفظ البيانات في الملف.\n";
        return false;
    }

    for (const Book& book : books) {
        file << book.id << '\t'
             << (book.available ? 1 : 0) << '\t'
             << cleanText(book.title) << '\t'
             << cleanText(book.author) << '\n';
    }

    return static_cast<bool>(file);
}

int nextId(const std::vector<Book>& books) {
    int largestId = 0;

    for (const Book& book : books) {
        if (book.id > largestId) {
            largestId = book.id;
        }
    }

    return largestId + 1;
}

int readInteger(const std::string& prompt) {
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

            if (used == line.size()) {
                return value;
            }
        } catch (...) {
            // الإدخال ليس رقمًا صحيحًا؛ سيُطلب من المستخدم المحاولة مجددًا.
        }

        std::cout << "أدخل رقمًا صحيحًا.\n";
    }
}

void showBooks(const std::vector<Book>& books) {
    if (books.empty()) {
        std::cout << "لا توجد كتب مسجلة في المكتبة.\n";
        return;
    }

    std::cout << "\n--- كتب المكتبة ---\n";

    for (const Book& book : books) {
        std::cout << "الرقم: " << book.id
                  << " | العنوان: " << book.title
                  << " | المؤلف: " << book.author
                  << " | الحالة: "
                  << (book.available ? "متاح" : "مُعار")
                  << '\n';
    }
}

void addBook(std::vector<Book>& books) {
    std::string title;
    std::string author;

    std::cout << "عنوان الكتاب: ";
    std::getline(std::cin, title);

    std::cout << "اسم المؤلف: ";
    std::getline(std::cin, author);

    title = cleanText(title);
    author = cleanText(author);

    if (title.empty() || author.empty()) {
        std::cout << "لم تتم إضافة الكتاب؛ العنوان واسم المؤلف مطلوبان.\n";
        return;
    }

    books.push_back({nextId(books), title, author, true});

    if (saveBooks(books)) {
        std::cout << "تمت إضافة الكتاب وحفظه.\n";
    }
}

void searchBooks(const std::vector<Book>& books) {
    std::cout << "أدخل كلمة من العنوان أو اسم المؤلف: ";
    std::string query;
    std::getline(std::cin, query);

    if (query.empty()) {
        std::cout << "عبارة البحث فارغة.\n";
        return;
    }

    bool found = false;

    for (const Book& book : books) {
        if (book.title.find(query) != std::string::npos ||
            book.author.find(query) != std::string::npos) {
            std::cout << "الرقم: " << book.id
                      << " | " << book.title
                      << " | المؤلف: " << book.author
                      << " | " << (book.available ? "متاح" : "مُعار")
                      << '\n';
            found = true;
        }
    }

    if (!found) {
        std::cout << "لم يُعثر على كتب تطابق البحث.\n";
    }
}

void changeLoanStatus(std::vector<Book>& books, bool borrow) {
    const int id = readInteger(
        borrow ? "رقم الكتاب لإعارته: " : "رقم الكتاب لإرجاعه: ");

    for (Book& book : books) {
        if (book.id == id) {
            if (borrow && !book.available) {
                std::cout << "الكتاب مُعار بالفعل.\n";
                return;
            }

            if (!borrow && book.available) {
                std::cout << "الكتاب متاح بالفعل، ولا يحتاج إلى إرجاع.\n";
                return;
            }

            book.available = !borrow;

            if (saveBooks(books)) {
                std::cout << (borrow ? "تم تسجيل إعارة الكتاب.\n"
                                    : "تم تسجيل إرجاع الكتاب.\n");
            }
            return;
        }
    }

    std::cout << "لم يتم العثور على كتاب بهذا الرقم.\n";
}

int main() {
    std::vector<Book> books = loadBooks();

    while (true) {
        std::cout << "\n=== نظام إدارة المكتبة ===\n"
                  << "1. عرض جميع الكتب\n"
                  << "2. إضافة كتاب\n"
                  << "3. البحث عن كتاب\n"
                  << "4. إعارة كتاب\n"
                  << "5. إرجاع كتاب\n"
                  << "0. خروج\n";

        const int choice = readInteger("اختر عملية: ");

        switch (choice) {
            case 1:
                showBooks(books);
                break;
            case 2:
                addBook(books);
                break;
            case 3:
                searchBooks(books);
                break;
            case 4:
                changeLoanStatus(books, true);
                break;
            case 5:
                changeLoanStatus(books, false);
                break;
            case 0:
                saveBooks(books);
                std::cout << "تم حفظ البيانات. إلى اللقاء!\n";
                return 0;
            default:
                std::cout << "خيار غير صحيح.\n";
        }
    }
}
