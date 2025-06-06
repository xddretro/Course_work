#ifndef PERSON_H
#define PERSON_H

#include <string>
#include <vector>
#include "CustomDate.h"

class BirthEvent;
class DeathEvent;
class MarriageEvent;
class FamilyManager;

enum class Gender {
    MALE,
    FEMALE
};

std::string genderToString(Gender gender);

class Person {
private:
    int personID_;
    static int nextID_; // Статическая переменная для генерации уникальных ID

    std::string fullName_;
    Gender gender_;
    CustomDate birthDate_;
    std::string occupation_;

    bool hasDeathDate_;      // Флаг, указывающий, есть ли дата смерти
    CustomDate deathDate_;

    // Семейные связи
    Person* mother_;
    Person* father_;
    std::vector<Person*> children_;
    std::vector<MarriageEvent*> marriages_; // Список событий брака

    // Ссылки на собственные ключевые события
    BirthEvent* ownBirthEvent_;
    DeathEvent* ownDeathEvent_;

    Person(int id, const std::string& fullName, Gender gender, const CustomDate& birthDate,
        const std::string& occupation);

public:
    // Статический фабричный метод для создания объектов Person 
    static Person* create(const std::string& fullName, Gender gender, const CustomDate& birthDate,
        const std::string& occupation);

    friend class FamilyManager;

    ~Person();

    int getID() const;
    std::string getFullName() const;
    Gender getGender() const;
    CustomDate getBirthDate() const;
    std::string getOccupation() const;
    bool hasDeathDateOccurred() const;
    CustomDate getDeathDate() const;
    Person* getMother() const;
    Person* getFather() const;
    const std::vector<Person*>& getChildren() const;
    const std::vector<MarriageEvent*>& getMarriages() const;
    BirthEvent* getOwnBirthEvent() const;
    DeathEvent* getOwnDeathEvent() const;

    void setFullName(const std::string& fullName);
    void setGender(Gender gender);
    void setOccupation(const std::string& occupation);
    void recordDeath(const CustomDate& dateOfDeath);
    void setMother(Person* mother);
    void setFather(Person* father);
    void addChild(Person* child);
    void addMarriageEvent(MarriageEvent* marriage);
    void setOwnBirthEvent(BirthEvent* event);
    void setOwnDeathEvent(DeathEvent* event);
};

#endif // PERSON_H