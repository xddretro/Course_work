#ifndef EVENT_H
#define EVENT_H

#include "CustomDate.h"
#include <string>

class Person;
class FamilyManager;

class Event { // абстрактный класс, будет использоваться паттерн "команда"
protected:
    CustomDate eventDate_;
    std::string description_;
    FamilyManager* familyManagerContext_;

public:
    Event(const CustomDate& date, FamilyManager* context, const std::string& description = "");
    virtual ~Event() = default;
    virtual void execute() = 0; // чисто виртуальный метод для наследников
    CustomDate getEventDate() const;
    std::string getDescription() const;
};

class BirthEvent : public Event {
private:
    Person* personBorn_;

public:
    BirthEvent(const CustomDate& date, FamilyManager* context, Person* personBorn, const std::string& description = "Рождение");
    void execute() override;
    Person* getPersonBorn() const;
};

class DeathEvent : public Event {
private:
    Person* deceasedPerson_;

public:
    DeathEvent(const CustomDate& date, FamilyManager* context, Person* deceasedPerson, const std::string& description = "Смерть");
    void execute() override;
    Person* getDeceasedPerson() const;
};

class MarriageEvent : public Event {
private:
    Person* spouse1_;
    Person* spouse2_;

public:
    MarriageEvent(const CustomDate& date, FamilyManager* context, Person* spouse1, Person* spouse2, const std::string& description = "Брак");
    void execute() override;
    Person* getSpouse1() const;
    Person* getSpouse2() const;
};

#endif // EVENT_H