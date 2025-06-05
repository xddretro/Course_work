#include "CustomDate.h"
#include <string>
#include <vector>  
#include <iomanip> 
#include <sstream> 

CustomDate::CustomDate() : day(1), month(1), year(1970) {
}

CustomDate::CustomDate(int d, int m, int y) {
    if (!setDate(d, m, y)) {
        day = 1;
        month = 1;
        year = 1970;
    }
}

bool CustomDate::isValidDate(int d, int m, int y) const {
    if (y < 1 || y > 2999) { // Ограничим года для разумности
        return false;
    }
    if (m < 1 || m > 12) {
        return false;
    }

    std::vector<int> daysInMonth = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

    // Проверка на високосный год для февраля
    if ((y % 4 == 0 && y % 100 != 0) || (y % 400 == 0)) {
        daysInMonth[2] = 29;
    }

    if (d < 1 || d > daysInMonth[m]) {
        return false;
    }
    return true;
}

int CustomDate::getDay() const {
    return day;
}

int CustomDate::getMonth() const {
    return month;
}

int CustomDate::getYear() const {
    return year;
}

bool CustomDate::setDate(int d, int m, int y) {
    if (isValidDate(d, m, y)) {
        day = d;
        month = m;
        year = y;
        return true;
    }
    return false;
}

std::string CustomDate::toString() const {
    std::ostringstream oss;
    oss << std::setw(2) << std::setfill('0') << day << "."
        << std::setw(2) << std::setfill('0') << month << "."
        << std::setw(4) << std::setfill('0') << year;
    return oss.str();
}

bool CustomDate::operator<(const CustomDate& other) const {
    if (year < other.year) return true;
    if (year > other.year) return false;
    if (month < other.month) return true;
    if (month > other.month) return false;
    return day < other.day;
}

bool CustomDate::operator==(const CustomDate& other) const {
    return day == other.day && month == other.month && year == other.year;
}

bool CustomDate::operator!=(const CustomDate& other) const {
    return !(*this == other);
}

// Дружественная функция для вывода в поток
std::ostream& operator<<(std::ostream& os, const CustomDate& date) {
    os << date.toString();
    return os;
}