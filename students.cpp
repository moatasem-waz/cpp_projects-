#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

struct Student {
    int id;
    std::string name;
    double grade;
};

const std::string DATA_FILE = "students.txt";

std::vector<Student> loadStudents() {
    std::vector<Student> students;
    std::ifstream file(DATA_FILE);
    std::string line;

    while (std::getline(file, line)) {
        std::istringstream row(line);
        Student student{};

        if (row >> student.id >> student.grade) {
            row.ignore(1, '\t');
            std::getline(row, student.name);

            if (!student.name.empty()) {
                students.push_back(student);
            }
        }
    }

    return students;
}

bool saveStudents(const std::vector<Student>& students) {
    std::ofstream file(DATA_FILE);

    if (!file) {
        std::cerr << "تعذر حفظ ملف الطلاب.\n";
        return false;
    }

    for (const Student& student : students) {
        // نستبدل tab حتى لا يفسد تنسيق ملف البيانات.
        std::string safeName = student.name;
        for (char& ch : safeName) {
            if (ch == '\t' || ch == '\n' || ch == '\r') ch = ' ';
        }

        file << student.id << '\t'
             << student.grade << '\t'
             << safeName << '\n';
    }

    return static_cast<bool>(file);
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

double readGrade() {
    while (true) {
        std::cout << "الدرجة من 0 إلى 100: ";
        std::string line;
        std::getline(std::cin, line);

        try {
            std::size_t used = 0;
            double grade = std::stod(line, &used);

            while (used < line.size() &&
                   (line[used] == ' ' || line[used] == '\t')) {
                ++used;
            }

            if (used == line.size() && grade >= 0 && grade <= 100) {
                return grade;
            }
        } catch (...) {
        }

        std::cout << "أدخل درجة صحيحة بين 0 و100.\n";
    }
}

int nextId(const std::vector<Student>& students) {
    int largest = 0;
    for (const Student& student : students) {
        if (student.id > largest) largest = student.id;
    }
    return largest + 1;
}

void showStudents(const std::vector<Student>& students) {
    if (students.empty()) {
        std::cout << "لا توجد بيانات طلاب.\n";
        return;
    }

    std::cout << "\n--- قائمة الطلاب ---\n";
    for (const Student& student : students) {
        std::cout << "الرقم: " << student.id
                  << " | الاسم: " << student.name
                  << " | الدرجة: " << std::fixed << std::setprecision(2)
                  << student.grade << '\n';
    }
}

void addStudent(std::vector<Student>& students) {
    std::cout << "اسم الطالب: ";
    std::string name;
    std::getline(std::cin, name);

    if (name.empty()) {
        std::cout << "لم تتم الإضافة؛ الاسم فارغ.\n";
        return;
    }

    for (char& ch : name) {
        if (ch == '\t' || ch == '\n' || ch == '\r') ch = ' ';
    }

    const double grade = readGrade();
    students.push_back({nextId(students), name, grade});

    if (saveStudents(students)) {
        std::cout << "تمت إضافة الطالب وحفظ البيانات.\n";
    }
}

void searchStudent(const std::vector<Student>& students) {
    const int id = readInt("أدخل رقم الطالب: ");

    for (const Student& student : students) {
        if (student.id == id) {
            std::cout << "الاسم: " << student.name
                      << " | الدرجة: " << std::fixed << std::setprecision(2)
                      << student.grade << '\n';
            return;
        }
    }

    std::cout << "لم يتم العثور على طالب بهذا الرقم.\n";
}

void showAverage(const std::vector<Student>& students) {
    if (students.empty()) {
        std::cout << "لا توجد درجات لحساب المعدل.\n";
        return;
    }

    double total = 0;
    for (const Student& student : students) {
        total += student.grade;
    }

    std::cout << "معدل درجات الطلاب: "
              << std::fixed << std::setprecision(2)
              << total / students.size() << '\n';
}

int main() {
    std::vector<Student> students = loadStudents();

    while (true) {
        std::cout << "\n=== نظام إدارة الطلاب ===\n"
                  << "1. عرض الطلاب\n"
                  << "2. إضافة طالب\n"
                  << "3. البحث برقم الطالب\n"
                  << "4. حساب معدل الدرجات\n"
                  << "0. خروج\n";

        const int choice = readInt("اختر: ");

        switch (choice) {
            case 1:
                showStudents(students);
                break;
            case 2:
                addStudent(students);
                break;
            case 3:
                searchStudent(students);
                break;
            case 4:
                showAverage(students);
                break;
            case 0:
                saveStudents(students);
                std::cout << "تم حفظ البيانات. إلى اللقاء!\n";
                return 0;
            default:
                std::cout << "خيار غير صحيح.\n";
        }
    }
}
