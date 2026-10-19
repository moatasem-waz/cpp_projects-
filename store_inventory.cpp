#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <string>
#include <vector>

struct Product {
    int id;
    std::string name;
    double price;
    int quantity;
};

const std::string DATA_FILE = "inventory.txt";

std::string cleanField(std::string text) {
    for (char& ch : text) {
        if (ch == '|') ch = '/';
        if (ch == '\n' || ch == '\r') ch = ' ';
    }
    return text;
}

std::vector<Product> loadProducts() {
    std::vector<Product> products;
    std::ifstream file(DATA_FILE);
    std::string line;

    while (std::getline(file, line)) {
        const std::size_t p1 = line.find('|');
        const std::size_t p2 = line.find('|', p1 == std::string::npos ? p1 : p1 + 1);
        const std::size_t p3 = line.find('|', p2 == std::string::npos ? p2 : p2 + 1);

        if (p1 == std::string::npos ||
            p2 == std::string::npos ||
            p3 == std::string::npos) {
            continue;
        }

        try {
            Product product;
            product.id = std::stoi(line.substr(0, p1));
            product.name = line.substr(p1 + 1, p2 - p1 - 1);
            product.price = std::stod(line.substr(p2 + 1, p3 - p2 - 1));
            product.quantity = std::stoi(line.substr(p3 + 1));
            products.push_back(product);
        } catch (...) {
            // تجاهل السطر التالف.
        }
    }

    return products;
}

bool saveProducts(const std::vector<Product>& products) {
    std::ofstream file(DATA_FILE);
    if (!file) {
        std::cerr << "تعذر فتح ملف المخزون للحفظ.\n";
        return false;
    }

    for (const Product& product : products) {
        file << product.id << '|'
             << cleanField(product.name) << '|'
             << product.price << '|'
             << product.quantity << '\n';
    }

    if (!file) {
        std::cerr << "حدث خطأ أثناء حفظ بيانات المخزون.\n";
        return false;
    }

    return true;
}

int nextId(const std::vector<Product>& products) {
    int id = 1;
    for (const Product& product : products) {
        if (product.id >= id) id = product.id + 1;
    }
    return id;
}

std::string readText(const std::string& prompt) {
    std::string value;
    std::cout << prompt;
    std::getline(std::cin, value);
    return cleanField(value);
}

int readInt(const std::string& prompt) {
    int value;

    while (true) {
        std::cout << prompt;
        if (std::cin >> value) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return value;
        }

        std::cout << "أدخل رقمًا صحيحًا.\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

double readDouble(const std::string& prompt) {
    double value;

    while (true) {
        std::cout << prompt;
        if (std::cin >> value) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return value;
        }

        std::cout << "أدخل رقمًا صالحًا.\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

Product* findProductById(std::vector<Product>& products, int id) {
    for (Product& product : products) {
        if (product.id == id) return &product;
    }
    return nullptr;
}

void printProduct(const Product& product) {
    std::cout << "المعرّف: " << product.id
              << " | المنتج: " << product.name
              << " | السعر: " << std::fixed << std::setprecision(2) << product.price
              << " | الكمية: " << product.quantity << '\n';
}

void addProduct(std::vector<Product>& products) {
    Product product;
    product.name = readText("اسم المنتج: ");

    if (product.name.empty()) {
        std::cout << "اسم المنتج مطلوب.\n";
        return;
    }

    product.price = readDouble("سعر الوحدة: ");
    product.quantity = readInt("الكمية المتوفرة: ");

    if (product.price < 0 || product.quantity < 0) {
        std::cout << "السعر والكمية لا يمكن أن يكونا سالبين.\n";
        return;
    }

    product.id = nextId(products);
    products.push_back(product);

    if (saveProducts(products)) {
        std::cout << "تمت إضافة المنتج. معرّفه: " << product.id << '\n';
    }
}

void listProducts(const std::vector<Product>& products) {
    if (products.empty()) {
        std::cout << "المخزون فارغ.\n";
        return;
    }

    for (const Product& product : products) {
        printProduct(product);
    }
}

void searchProduct(const std::vector<Product>& products) {
    const std::string query = readText("أدخل اسم المنتج أو جزءًا منه: ");
    bool found = false;

    for (const Product& product : products) {
        if (product.name.find(query) != std::string::npos) {
            printProduct(product);
            found = true;
        }
    }

    if (!found) std::cout << "لم يتم العثور على منتج مطابق.\n";
}

void updateQuantity(std::vector<Product>& products) {
    const int id = readInt("معرّف المنتج: ");
    Product* product = findProductById(products, id);

    if (product == nullptr) {
        std::cout << "لم يتم العثور على هذا المنتج.\n";
        return;
    }

    std::cout << "الكمية الحالية: " << product->quantity << '\n'
              << "أدخل التغيير في الكمية (مثال: 5 للإضافة أو -2 للبيع): ";

    int change;
    while (!(std::cin >> change)) {
        std::cout << "أدخل رقمًا صحيحًا: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (product->quantity + change < 0) {
        std::cout << "لا يمكن أن تصبح الكمية أقل من صفر.\n";
        return;
    }

    product->quantity += change;
    if (saveProducts(products)) {
        std::cout << "تم تحديث الكمية. الكمية الجديدة: "
                  << product->quantity << '\n';
    }
}

void deleteProduct(std::vector<Product>& products) {
    const int id = readInt("أدخل معرّف المنتج المراد حذفه: ");

    for (auto it = products.begin(); it != products.end(); ++it) {
        if (it->id == id) {
            products.erase(it);
            if (saveProducts(products)) std::cout << "تم حذف المنتج.\n";
            return;
        }
    }

    std::cout << "لم يتم العثور على هذا المنتج.\n";
}

int main() {
    std::vector<Product> products = loadProducts();

    while (true) {
        std::cout << "\n=== إدارة مخزون المتجر ===\n"
                  << "1. إضافة منتج\n"
                  << "2. عرض المخزون\n"
                  << "3. البحث عن منتج\n"
                  << "4. تحديث الكمية (إضافة أو بيع)\n"
                  << "5. حذف منتج\n"
                  << "0. خروج\n";

        const int choice = readInt("اختر: ");

        switch (choice) {
            case 1: addProduct(products); break;
            case 2: listProducts(products); break;
            case 3: searchProduct(products); break;
            case 4: updateQuantity(products); break;
            case 5: deleteProduct(products); break;
            case 0:
                std::cout << "تم إغلاق البرنامج.\n";
                return 0;
            default:
                std::cout << "اختيار غير صحيح.\n";
        }
    }
}
