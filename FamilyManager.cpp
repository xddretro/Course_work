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

        BirthEvent* birthCmd = new BirthEvent(birthDate, this, newPerson, "Рождение (автоматическое)");
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
    DeathEvent* deathCmd = new DeathEvent(date, this, deceasedPerson, "Смерть");
    if (deathCmd) {
        deathCmd->execute();
    }
}

void FamilyManager::createAndExecuteMarriageEvent(Person* spouse1, Person* spouse2, const CustomDate& date) {
    if (!spouse1 || !spouse2 || spouse1->getID() == spouse2->getID()) {
        return;
    }
    MarriageEvent* marriageCmd = new MarriageEvent(date, this, spouse1, spouse2, "Брак");
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
    // 1. Базовые проверки входных данных
    if (!parent || !child) {
        std::cout << "Ошибка: Не указан родитель или ребенок для установления связи." << std::endl;
        return false;
    }

    if (parent->getID() == child->getID()) {
        std::cout << "Ошибка: Человек не может быть родителем самому себе." << std::endl;
        return false;
    }

    // 2. Проверка на простейший цикл: не является ли ребенок уже родителем для указанного родителя
    if ((parent->getMother() && parent->getMother()->getID() == child->getID()) ||
        (parent->getFather() && parent->getFather()->getID() == child->getID())) {
        std::cout << "Ошибка: " << child->getFullName()
            << " уже является родителем для " << parent->getFullName()
            << ". Нельзя установить циклическую связь." << std::endl;
        return false;
    }

    // 3. Установка связи в зависимости от пола родителя
    Gender parentGender = parent->getGender();

    if (parentGender == Gender::MALE) {
        if (child->getFather() == nullptr) { // Если у ребенка еще нет отца
            child->setFather(parent);
            parent->addChild(child); // Добавляем ребенка к родителю
            std::cout << "Связь установлена: " << parent->getFullName()
                << " назначен отцом для " << child->getFullName() << "." << std::endl;
            return true;
        }
        else if (child->getFather()->getID() == parent->getID()) { // Если этот родитель уже является отцом
            std::cout << parent->getFullName() << " уже является отцом для " << child->getFullName() << "." << std::endl;
            return true; // Связь уже существует, считаем операцию успешной
        }
        else { // У ребенка уже есть другой отец
            std::cout << "Ошибка: У " << child->getFullName()
                << " уже есть другой отец (" << child->getFather()->getFullName()
                << "). Замена родителя на данном этапе не реализована." << std::endl;
            return false;
        }
    }
    else if (parentGender == Gender::FEMALE) {
        if (child->getMother() == nullptr) { // Если у ребенка еще нет матери
            child->setMother(parent);
            parent->addChild(child); // Добавляем ребенка к родителю
            std::cout << "Связь установлена: " << parent->getFullName()
                << " назначена матерью для " << child->getFullName() << "." << std::endl;
            return true;
        }
        else if (child->getMother()->getID() == parent->getID()) { // Если этот родитель уже является матерью
            std::cout << parent->getFullName() << " уже является матерью для " << child->getFullName() << "." << std::endl;
            return true; // Связь уже существует, считаем операцию успешной
        }
        else { // У ребенка уже есть другая мать
            std::cout << "Ошибка: У " << child->getFullName()
                << " уже есть другая мать (" << child->getMother()->getFullName()
                << "). Замена родителя на данном этапе не реализована." << std::endl;
            return false;
        }
    }

    else { // Пол родителя не определен как М или Ж
        std::cout << "Ошибка: Не удалось установить роль родителя для " << parent->getFullName()
            << ", так как пол не определен как Мужской или Женский." << std::endl;
        return false;
    }
}