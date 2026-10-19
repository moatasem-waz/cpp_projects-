#include <fstream>
#include <iostream>
#include <limits>
#include <string>
#include <vector>

struct Contact {
    int id;
    std::string name;
    std::string phone;
    std::string email;
};

const std::string DATA_FILE = "contacts.txt";

std::string cleanField(std::string text) {
    // استبدال الفاصل والأسطر الجديدة كي لا تتلف صيغة الملف.
    for (char& ch : text) {
        if (ch == '|') ch = '/';
        if (ch == '\n' || ch == '\r') ch = ' ';
    }
    return text;
}

std::vector<Contact> loadContacts() {
    std::vector<Contact> contacts;
    std::ifstream file(DATA_FILE);
    std::string line;

    while (std::getline(file, line)) {
        const auto first = line.find('|');
        const auto second = line.find('|', first == std::string::npos ? first : first + 1);
        const auto third = line.find('|', second == std::string::npos ? second : second + 1);

        if (first == std::string::npos ||
            second == std::string::npos ||
            third == std::string::npos) {
            continue;
        }

        try {
            Contact contact;
            contact.id = std::stoi(line.substr(0, first));
            contact.name = line.substr(first + 1, second - first - 1);
            contact.phone = line.substr(second + 1, third - second - 1);
            contact.email = line.substr(third + 1);
            contacts.push_back(contact);
        } catch (...) {
            // تجاهل السطر التالف.
        }
    }

    return contacts;
}

bool saveContacts(const std::vector<Contact>& contacts) {
    std::ofstream file(DATA_FILE);
    if (!file) {
        std::cerr << "تعذر فتح ملف الحفظ.\n";
        return false;
    }

    for (const Contact& contact : contacts) {
        file << contact.id << '|'
             << cleanField(contact.name) << '|'
             << cleanField(contact.phone) << '|'
             << cleanField(contact.email) << '\n';
    }

    return static_cast<bool>(file);
}

int nextId(const std::vector<Contact>& contacts) {
    int id = 1;
    for (const Contact& contact : contacts) {
        if (contact.id >= id) id = contact.id + 1;
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

void showContact(const Contact& contact) {
    std::cout << "المعرّف: " << contact.id
              << " | الاسم: " << contact.name
              << " | الهاتف: " << contact.phone
              << " | البريد: " << contact.email << '\n';
}

void addContact(std::vector<Contact>& contacts) {
    Contact contact;
    contact.id = nextId(contacts);
    contact.name = readText("الاسم: ");
    contact.phone = readText("رقم الهاتف: ");
    contact.email = readText("البريد الإلكتروني: ");

    if (contact.name.empty()) {
        std::cout << "لم تتم الإضافة: الاسم مطلوب.\n";
        return;
    }

    contacts.push_back(contact);
    if (saveContacts(contacts)) {
        std::cout << "تمت إضافة جهة الاتصال. المعرّف: " << contact.id << '\n';
    }
}

void listContacts(const std::vector<Contact>& contacts) {
    if (contacts.empty()) {
        std::cout << "دفتر جهات الاتصال فارغ.\n";
        return;
    }

    for (const Contact& contact : contacts) {
        showContact(contact);
    }
}

void searchContacts(const std::vector<Contact>& contacts) {
    const std::string query = readText("ابحث بالاسم أو الهاتف أو البريد: ");
    bool found = false;

    for (const Contact& contact : contacts) {
        if (contact.name.find(query) != std::string::npos ||
            contact.phone.find(query) != std::string::npos ||
            contact.email.find(query) != std::string::npos) {
            showContact(contact);
            found = true;
        }
    }

    if (!found) std::cout << "لم يتم العثور على جهة اتصال مطابقة.\n";
}

void editContact(std::vector<Contact>& contacts) {
    const int id = readInt("أدخل معرّف جهة الاتصال لتعديلها: ");

    for (Contact& contact : contacts) {
        if (contact.id == id) {
            std::cout << "اترك الحقل فارغًا للإبقاء على قيمته الحالية.\n";

            std::string value = readText("الاسم [" + contact.name + "]: ");
            if (!value.empty()) contact.name = value;

            value = readText("الهاتف [" + contact.phone + "]: ");
            if (!value.empty()) contact.phone = value;

            value = readText("البريد [" + contact.email + "]: ");
            if (!value.empty()) contact.email = value;

            if (saveContacts(contacts)) std::cout << "تم تحديث جهة الاتصال.\n";
            return;
        }
    }

    std::cout << "لم يتم العثور على هذا المعرّف.\n";
}

void deleteContact(std::vector<Contact>& contacts) {
    const int id = readInt("أدخل معرّف جهة الاتصال لحذفها: ");

    for (auto it = contacts.begin(); it != contacts.end(); ++it) {
        if (it->id == id) {
            contacts.erase(it);
            if (saveContacts(contacts)) std::cout << "تم حذف جهة الاتصال.\n";
            return;
        }
    }

    std::cout << "لم يتم العثور على هذا المعرّف.\n";
}

int main() {
    std::vector<Contact> contacts = loadContacts();

    while (true) {
        std::cout << "\n=== دفتر جهات الاتصال ===\n"
                  << "1. إضافة جهة اتصال\n"
                  << "2. عرض جميع جهات الاتصال\n"
                  << "3. البحث\n"
                  << "4. تعديل جهة اتصال\n"
                  << "5. حذف جهة اتصال\n"
                  << "0. خروج\n";

        const int choice = readInt("اختر: ");

        switch (choice) {
            case 1: addContact(contacts); break;
            case 2: listContacts(contacts); break;
            case 3: searchContacts(contacts); break;
            case 4: editContact(contacts); break;
            case 5: deleteContact(contacts); break;
            case 0:
                std::cout << "تم إغلاق البرنامج.\n";
                return 0;
            default:
                std::cout << "اختيار غير صحيح.\n";
        }
    }
}
