#include "Person.h"
#include <iostream> 

int Person::nextID_ = 1;

std::string genderToString(Gender gender) {
    switch (gender) {
    case Gender::MALE:    return "Мужской";
    case Gender::FEMALE:  return "Женский";
    case Gender::UNKNOWN: return "Неизвестно";
    default:              return "Не указан";
    }
}

Person::Person(int id, const std::string& fullName, Gender gender, const CustomDate& birthDate, const std::string& occupation)
    : personID_(id),
    fullName_(fullName),
    gender_(gender),
    birthDate_(birthDate),
    occupation_(occupation),
    hasDeathDate_(false),
    deathDate_(),
    mother_(nullptr),
    father_(nullptr),
    ownBirthEvent_(nullptr),
    ownDeathEvent_(nullptr) {
}

Person* Person::create(const std::string& fullName, Gender gender, const CustomDate& birthDate, const std::string& occupation) {
    int newPersonID = nextID_++;
    return new Person(newPersonID, fullName, gender, birthDate, occupation);
}

Person::~Person() {}

int Person::getID() const {
    return personID_;
}

std::string Person::getFullName() const {
    return fullName_;
}

Gender Person::getGender() const {
    return gender_;
}

CustomDate Person::getBirthDate() const {
    return birthDate_;
}

std::string Person::getOccupation() const {
    return occupation_;
}

bool Person::hasDeathDateOccurred() const {
    return hasDeathDate_;
}

CustomDate Person::getDeathDate() const {
    return deathDate_;
}

Person* Person::getMother() const {
    return mother_;
}

Person* Person::getFather() const {
    return father_;
}

const std::vector<Person*>& Person::getChildren() const {
    return children_;
}

const std::vector<MarriageEvent*>& Person::getMarriages() const {
    return marriages_;
}

BirthEvent* Person::getOwnBirthEvent() const {
    return ownBirthEvent_;
}

DeathEvent* Person::getOwnDeathEvent() const {
    return ownDeathEvent_;
}

void Person::setFullName(const std::string& fullName) {
    fullName_ = fullName;
}

void Person::setGender(Gender gender) {
    gender_ = gender;
}

void Person::setOccupation(const std::string& occupation) {
    occupation_ = occupation;
}

void Person::recordDeath(const CustomDate& dateOfDeath) {
    deathDate_ = dateOfDeath;
    hasDeathDate_ = true;
}

void Person::setMother(Person* mother) {
    mother_ = mother;
}

void Person::setFather(Person* father) {
    father_ = father;
}

void Person::addChild(Person* child) {
    if (child) {
        children_.push_back(child);
    }
}

void Person::addMarriageEvent(MarriageEvent* marriage) {
    if (marriage) {
        marriages_.push_back(marriage);
    }
}

void Person::setOwnBirthEvent(BirthEvent* event) {
    ownBirthEvent_ = event;
}

void Person::setOwnDeathEvent(DeathEvent* event) {
    ownDeathEvent_ = event;
}