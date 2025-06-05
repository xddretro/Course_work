#include "FamilyManager.h"
#include "EventLogger.h"   
#include "Person.h"        
#include "Event.h"         
#include "CustomDate.h"   
#include <iostream>        

FamilyManager::FamilyManager() {
    eventLogger_ = new EventLogger();
}

FamilyManager::~FamilyManager() {
    for (auto const& pairElement : people_) {
        Person* personPtr = pairElement.second;
        delete personPtr;
    }
    people_.clear();

    delete eventLogger_;
    eventLogger_ = nullptr;
}

Person* FamilyManager::addPerson(const std::string& fullName, Gender gender, const CustomDate& birthDate,
    const std::string& occupation,
    Person* mother, Person* father) {

    Person* newPerson = Person::create(fullName, gender, birthDate, occupation);

    if (newPerson) {
        people_[newPerson->getID()] = newPerson;

        if (mother) {
            newPerson->setMother(mother);
            mother->addChild(newPerson);
        }
        if (father) {
            newPerson->setFather(father);
            father->addChild(newPerson);
        }

        BirthEvent* birthCmd = new BirthEvent(birthDate, this, newPerson, "�������� (��������������)");
        if (birthCmd) {
            birthCmd->execute();
        }
        return newPerson;
    }
    return nullptr;
}

Person* FamilyManager::findPersonById(int personID) const {
    auto it = people_.find(personID);
    if (it != people_.end()) {
        return it->second;
    }
    return nullptr;
}

std::vector<Person*> FamilyManager::findPersonByName(const std::string& name) const {
    std::vector<Person*> foundPeople;
    for (auto const& pairElement : people_) {
        Person* personPtr = pairElement.second;
        if (personPtr && personPtr->getFullName() == name) {
            foundPeople.push_back(personPtr);
        }
    }
    return foundPeople;
}

const std::map<int, Person*>& FamilyManager::getAllPeople() const {
    return people_;
}

void FamilyManager::createAndExecuteDeathEvent(Person* deceasedPerson, const CustomDate& date) {
    if (!deceasedPerson) {
        return;
    }
    DeathEvent* deathCmd = new DeathEvent(date, this, deceasedPerson, "������");
    if (deathCmd) {
        deathCmd->execute();
    }
}

void FamilyManager::createAndExecuteMarriageEvent(Person* spouse1, Person* spouse2, const CustomDate& date) {
    if (!spouse1 || !spouse2 || spouse1->getID() == spouse2->getID()) {
        return;
    }
    MarriageEvent* marriageCmd = new MarriageEvent(date, this, spouse1, spouse2, "����");
    if (marriageCmd) {
        marriageCmd->execute();
    }
}

void FamilyManager::recordEventInLog(Event* event) {
    if (eventLogger_ && event) {
        eventLogger_->recordEvent(event);
    }
}

EventLogger* FamilyManager::getEventLogger() const {
    return eventLogger_;
}

Person* FamilyManager::findOldestPerson() const {
    if (people_.empty()) {
        return nullptr;
    }

    Person* oldestPerson = nullptr;
    CustomDate oldestBirthDate;

    bool first = true;
    for (auto const& pairElement : people_) {
        Person* personPtr = pairElement.second;
        if (personPtr) {
            if (first) {
                oldestPerson = personPtr;
                oldestBirthDate = personPtr->getBirthDate();
                first = false;
            }
            else {
                if (personPtr->getBirthDate() < oldestBirthDate) {
                    oldestPerson = personPtr;
                    oldestBirthDate = personPtr->getBirthDate();
                }
            }
        }
    }
    return oldestPerson;
}

bool FamilyManager::establishParentChildRelationship(Person* parent, Person* child) {
    // 1. ������� �������� ������� ������
    if (!parent || !child) {
        std::cout << "������: �� ������ �������� ��� ������� ��� ������������ �����." << std::endl;
        return false;
    }

    if (parent->getID() == child->getID()) {
        std::cout << "������: ������� �� ����� ���� ��������� ������ ����." << std::endl;
        return false;
    }

    // 2. �������� �� ���������� ����: �� �������� �� ������� ��� ��������� ��� ���������� ��������
    if ((parent->getMother() && parent->getMother()->getID() == child->getID()) ||
        (parent->getFather() && parent->getFather()->getID() == child->getID())) {
        std::cout << "������: " << child->getFullName()
            << " ��� �������� ��������� ��� " << parent->getFullName()
            << ". ������ ���������� ����������� �����." << std::endl;
        return false;
    }

    // 3. ��������� ����� � ����������� �� ���� ��������
    Gender parentGender = parent->getGender();

    if (parentGender == Gender::MALE) {
        if (child->getFather() == nullptr) { // ���� � ������� ��� ��� ����
            child->setFather(parent);
            parent->addChild(child); // ��������� ������� � ��������
            std::cout << "����� �����������: " << parent->getFullName()
                << " �������� ����� ��� " << child->getFullName() << "." << std::endl;
            return true;
        }
        else if (child->getFather()->getID() == parent->getID()) { // ���� ���� �������� ��� �������� �����
            std::cout << parent->getFullName() << " ��� �������� ����� ��� " << child->getFullName() << "." << std::endl;
            return true; // ����� ��� ����������, ������� �������� ��������
        }
        else { // � ������� ��� ���� ������ ����
            std::cout << "������: � " << child->getFullName()
                << " ��� ���� ������ ���� (" << child->getFather()->getFullName()
                << "). ������ �������� �� ������ ����� �� �����������." << std::endl;
            return false;
        }
    }
    else if (parentGender == Gender::FEMALE) {
        if (child->getMother() == nullptr) { // ���� � ������� ��� ��� ������
            child->setMother(parent);
            parent->addChild(child); // ��������� ������� � ��������
            std::cout << "����� �����������: " << parent->getFullName()
                << " ��������� ������� ��� " << child->getFullName() << "." << std::endl;
            return true;
        }
        else if (child->getMother()->getID() == parent->getID()) { // ���� ���� �������� ��� �������� �������
            std::cout << parent->getFullName() << " ��� �������� ������� ��� " << child->getFullName() << "." << std::endl;
            return true; // ����� ��� ����������, ������� �������� ��������
        }
        else { // � ������� ��� ���� ������ ����
            std::cout << "������: � " << child->getFullName()
                << " ��� ���� ������ ���� (" << child->getMother()->getFullName()
                << "). ������ �������� �� ������ ����� �� �����������." << std::endl;
            return false;
        }
    }

    else { // ��� �������� �� ��������� ��� � ��� �
        std::cout << "������: �� ������� ���������� ���� �������� ��� " << parent->getFullName()
            << ", ��� ��� ��� �� ��������� ��� ������� ��� �������." << std::endl;
        return false;
    }
}