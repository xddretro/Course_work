#ifndef CUSTOMDATE_H
#define CUSTOMDATE_H

#include <string>
#include <iostream>

class CustomDate {
private:
    int day;
    int month;
    int year;

    bool isValidDate(int d, int m, int y) const;

public:
    CustomDate();
    CustomDate(int d, int m, int y);

    int getDay() const;
    int getMonth() const;
    int getYear() const;

    bool setDate(int d, int m, int y);

    std::string toString() const;

    bool operator<(const CustomDate& other) const;
    bool operator==(const CustomDate& other) const;
    bool operator!=(const CustomDate& other) const;

    friend std::ostream& operator<<(std::ostream& os, const CustomDate& date);
};

#endif