#include <fstream>
#include <iostream>
#include <limits>
#include <string>
#include <vector>

struct Appointment {
    int id;
    std::string name;
    std::string date;  
    std::string time;  
    std::string notes;
};

const std::string DATA_FILE = "appointments.txt";

std::string cleanField(std::string text) {
    // منع الفاصل أو الأسطر الجديدة من إفساد تنسيق الملف.
    for (char& ch : text) {
        if (ch == '|') ch = '/';
        if (ch == '\n' || ch == '\r') ch = ' ';
    }
    return text;
}

std::vector<Appointment> loadAppointments() {
    std::vector<Appointment> appointments;
    std::ifstream file(DATA_FILE);
    std::string line;

    while (std::getline(file, line)) {
        const std::size_t p1 = line.find('|');
        const std::size_t p2 = line.find('|', p1 == std::string::npos ? p1 : p1 + 1);
        const std::size_t p3 = line.find('|', p2 == std::string::npos ? p2 : p2 + 1);
        const std::size_t p4 = line.find('|', p3 == std::string::npos ? p3 : p3 + 1);

        if (p1 == std::string::npos || p2 == std::string::npos ||
            p3 == std::string::npos || p4 == std::string::npos) {
            continue;
        }

        try {
            Appointment appointment;
            appointment.id = std::stoi(line.substr(0, p1));
            appointment.name = line.substr(p1 + 1, p2 - p1 - 1);
            appointment.date = line.substr(p2 + 1, p3 - p2 - 1);
            appointment.time = line.substr(p3 + 1, p4 - p3 - 1);
            appointment.notes = line.substr(p4 + 1);
            appointments.push_back(appointment);
        } catch (...) {
            // تجاهل أي سطر تالف في الملف.
        }
    }

    return appointments;
}

bool saveAppointments(const std::vector<Appointment>& appointments) {
    std::ofstream file(DATA_FILE);
    if (!file) {
        std::cerr << "تعذر فتح ملف المواعيد للحفظ.\n";
        return false;
    }

    for (const Appointment& appointment : appointments) {
        file << appointment.id << '|'
             << cleanField(appointment.name) << '|'
             << cleanField(appointment.date) << '|'
             << cleanField(appointment.time) << '|'
             << cleanField(appointment.notes) << '\n';
    }

    if (!file) {
        std::cerr << "حدث خطأ أثناء حفظ المواعيد.\n";
        return false;
    }

    return true;
}

int getNextId(const std::vector<Appointment>& appointments) {
    int next = 1;
    for (const Appointment& appointment : appointments) {
        if (appointment.id >= next) next = appointment.id + 1;
    }
    return next;
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

void printAppointment(const Appointment& appointment) {
    std::cout << "المعرّف: " << appointment.id
              << " | الاسم: " << appointment.name
              << " | التاريخ: " << appointment.date
              << " | الوقت: " << appointment.time
              << " | التفاصيل: " << appointment.notes << '\n';
}

bool timeSlotTaken(const std::vector<Appointment>& appointments,
                   const std::string& date,
                   const std::string& time) {
    for (const Appointment& appointment : appointments) {
        if (appointment.date == date && appointment.time == time) {
            return true;
        }
    }
    return false;
}

void addAppointment(std::vector<Appointment>& appointments) {
    Appointment appointment;
    appointment.name = readText("اسم صاحب الموعد: ");
    appointment.date = readText("التاريخ (مثال 2026-10-25): ");
    appointment.time = readText("الوقت (مثال 14:30): ");
    appointment.notes = readText("تفاصيل إضافية: ");

    if (appointment.name.empty() || appointment.date.empty() ||
        appointment.time.empty()) {
        std::cout << "الاسم والتاريخ والوقت حقول مطلوبة.\n";
        return;
    }

    if (timeSlotTaken(appointments, appointment.date, appointment.time)) {
        std::cout << "هذا الموعد محجوز بالفعل في التاريخ والوقت نفسيهما.\n";
        return;
    }

    appointment.id = getNextId(appointments);
    appointments.push_back(appointment);

    if (saveAppointments(appointments)) {
        std::cout << "تم حجز الموعد. معرّفه: " << appointment.id << '\n';
    }
}

void listAppointments(const std::vector<Appointment>& appointments) {
    if (appointments.empty()) {
        std::cout << "لا توجد مواعيد مسجلة.\n";
        return;
    }

    for (const Appointment& appointment : appointments) {
        printAppointment(appointment);
    }
}

void searchAppointments(const std::vector<Appointment>& appointments) {
    const std::string query = readText("أدخل اسمًا أو تاريخًا للبحث: ");
    bool found = false;

    for (const Appointment& appointment : appointments) {
        if (appointment.name.find(query) != std::string::npos ||
            appointment.date.find(query) != std::string::npos) {
            printAppointment(appointment);
            found = true;
        }
    }

    if (!found) std::cout << "لم يتم العثور على مواعيد مطابقة.\n";
}

void cancelAppointment(std::vector<Appointment>& appointments) {
    const int id = readInt("أدخل معرّف الموعد المراد إلغاؤه: ");

    for (auto it = appointments.begin(); it != appointments.end(); ++it) {
        if (it->id == id) {
            appointments.erase(it);
            if (saveAppointments(appointments)) {
                std::cout << "تم إلغاء الموعد.\n";
            }
            return;
        }
    }

    std::cout << "لم يتم العثور على موعد بهذا المعرّف.\n";
}

int main() {
    std::vector<Appointment> appointments = loadAppointments();

    while (true) {
        std::cout << "\n=== نظام حجز المواعيد ===\n"
                  << "1. حجز موعد\n"
                  << "2. عرض جميع المواعيد\n"
                  << "3. البحث عن موعد\n"
                  << "4. إلغاء موعد\n"
                  << "0. خروج\n";

        switch (readInt("اختر: ")) {
            case 1:
                addAppointment(appointments);
                break;
            case 2:
                listAppointments(appointments);
                break;
            case 3:
                searchAppointments(appointments);
                break;
            case 4:
                cancelAppointment(appointments);
                break;
            case 0:
                std::cout << "تم إغلاق البرنامج.\n";
                return 0;
            default:
                std::cout << "اختيار غير صحيح.\n";
        }
    }
}
