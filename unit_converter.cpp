#include <iostream>
#include <limits>
#include <string>

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

int readChoice(const std::string& prompt, int min, int max) {
    int choice;

    while (true) {
        std::cout << prompt;
        if (std::cin >> choice && choice >= min && choice <= max) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return choice;
        }

        std::cout << "اختيار غير صالح.\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

void convertLength() {
    const char* units[] = {"متر", "كيلومتر", "سنتيمتر", "ميل"};
    // عوامل التحويل إلى متر.
    const double toMeters[] = {1.0, 1000.0, 0.01, 1609.344};

    std::cout << "\n1. متر\n2. كيلومتر\n3. سنتيمتر\n4. ميل\n";
    const int from = readChoice("حوّل من: ", 1, 4);
    const int to = readChoice("حوّل إلى: ", 1, 4);
    const double value = readDouble("القيمة: ");

    const double result = value * toMeters[from - 1] / toMeters[to - 1];
    std::cout << value << ' ' << units[from - 1] << " = "
              << result << ' ' << units[to - 1] << '\n';
}

void convertWeight() {
    const char* units[] = {"كيلوغرام", "غرام", "رطل"};
    // عوامل التحويل إلى كيلوغرام.
    const double toKilograms[] = {1.0, 0.001, 0.45359237};

    std::cout << "\n1. كيلوغرام\n2. غرام\n3. رطل\n";
    const int from = readChoice("حوّل من: ", 1, 3);
    const int to = readChoice("حوّل إلى: ", 1, 3);
    const double value = readDouble("القيمة: ");

    const double result = value * toKilograms[from - 1] / toKilograms[to - 1];
    std::cout << value << ' ' << units[from - 1] << " = "
              << result << ' ' << units[to - 1] << '\n';
}

void convertVolume() {
    const char* units[] = {"لتر", "مليلتر", "غالون أمريكي"};
    // عوامل التحويل إلى لتر.
    const double toLiters[] = {1.0, 0.001, 3.785411784};

    std::cout << "\n1. لتر\n2. مليلتر\n3. غالون أمريكي\n";
    const int from = readChoice("حوّل من: ", 1, 3);
    const int to = readChoice("حوّل إلى: ", 1, 3);
    const double value = readDouble("القيمة: ");

    const double result = value * toLiters[from - 1] / toLiters[to - 1];
    std::cout << value << ' ' << units[from - 1] << " = "
              << result << ' ' << units[to - 1] << '\n';
}

double toCelsius(double value, int unit) {
    if (unit == 1) return value;                  // مئوية
    if (unit == 2) return (value - 32.0) * 5.0 / 9.0; // فهرنهايت
    return value - 273.15;                       // كلفن
}

double fromCelsius(double celsius, int unit) {
    if (unit == 1) return celsius;                // مئوية
    if (unit == 2) return celsius * 9.0 / 5.0 + 32.0; // فهرنهايت
    return celsius + 273.15;                     // كلفن
}

void convertTemperature() {
    const char* units[] = {"مئوية", "فهرنهايت", "كلفن"};

    std::cout << "\n1. مئوية\n2. فهرنهايت\n3. كلفن\n";
    const int from = readChoice("حوّل من: ", 1, 3);
    const int to = readChoice("حوّل إلى: ", 1, 3);
    const double value = readDouble("القيمة: ");

    const double celsius = toCelsius(value, from);
    const double result = fromCelsius(celsius, to);

    std::cout << value << ' ' << units[from - 1] << " = "
              << result << ' ' << units[to - 1] << '\n';
}

int main() {
    while (true) {
        std::cout << "\n=== محوّل الوحدات ===\n"
                  << "1. الطول\n"
                  << "2. الوزن\n"
                  << "3. الحجم\n"
                  << "4. درجة الحرارة\n"
                  << "0. خروج\n";

        const int choice = readChoice("اختر نوع التحويل: ", 0, 4);

        switch (choice) {
            case 1: convertLength(); break;
            case 2: convertWeight(); break;
            case 3: convertVolume(); break;
            case 4: convertTemperature(); break;
            case 0:
                std::cout << "تم إغلاق البرنامج.\n";
                return 0;
        }
    }
}
