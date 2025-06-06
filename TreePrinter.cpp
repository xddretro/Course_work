#include "TreePrinter.h"
#include "Person.h"      
#include "Event.h"        
#include "CustomDate.h"  
#include <iostream>       
#include <sstream>        
#include <algorithm>      

TreePrinter::TreePrinter() {}

TreePrinter::~TreePrinter() {}

std::string TreePrinter::getPersonDisplayString(const Person& person) const {
    std::ostringstream oss;
    oss << person.getFullName();
    CustomDate birthDate = person.getBirthDate();

    // Простая проверка на валидность года перед его использованием
    if (birthDate.getYear() >= 1) { // год < 1 невалиден
        oss << " (" << birthDate.getYear();
        if (person.hasDeathDateOccurred()) {
            CustomDate deathDate = person.getDeathDate();
            if (deathDate.getYear() >= 1) {
                oss << "-" << deathDate.getYear();
            }
            else {
                oss << "-?"; // Если дата смерти есть, но год невалиден
            }
        }
        else {
            oss << "-"; // Человек жив или дата смерти не указана
        }
        oss << ")";
    }
    else {
        oss << " (даты неизв.)";
    }
    return oss.str();
}

// Выводит информацию о супругах человека
void TreePrinter::printSpouses(const Person* person,
    const FamilyManager& familyManager,
    const std::string& childrenVerticalStem) const {
    if (!person) return;
    const auto& marriages = person->getMarriages(); // Получаем список событий брака
    if (marriages.empty()) return;

    bool firstSpousePrinted = false;
    for (const MarriageEvent* marriageEvent : marriages) {
        if (!marriageEvent) continue;
        Person* spouse = nullptr;
        // Определяем второго супруга в этом событии брака
        if (marriageEvent->getSpouse1() && marriageEvent->getSpouse1()->getID() != person->getID()) {
            spouse = marriageEvent->getSpouse1();
        }
        else if (marriageEvent->getSpouse2() && marriageEvent->getSpouse2()->getID() != person->getID()) {
            spouse = marriageEvent->getSpouse2();
        }

        if (spouse) {
            std::cout << childrenVerticalStem << "  +-- (супруг/а) "
                << getPersonDisplayString(*spouse)
                << " [брак: " << marriageEvent->getEventDate().toString() << "]"
                << std::endl;
            firstSpousePrinted = true;
        }
    }
}

// Рекурсивная функция для вывода человека и его потомков
void TreePrinter::printPersonAndDescendants(const Person* person,
    const FamilyManager& familyManager,
    const std::string& currentPersonPrefix,
    const std::string& childrenPrefixStem,
    std::set<int>& printedPersons) const {
    if (!person) {
        return;
    }
    // Проверка, не был ли этот человек уже выведен
    if (printedPersons.count(person->getID())) {
        std::cout << currentPersonPrefix << "*" << getPersonDisplayString(*person) << " (уже показан)" << std::endl;
        return;
    }
    printedPersons.insert(person->getID());

    // Выводим текущего человека
    std::cout << currentPersonPrefix << this->getPersonDisplayString(*person) << std::endl;

    printSpouses(person, familyManager, childrenPrefixStem);

    // Выводим детей
    const auto& children = person->getChildren();
    for (size_t i = 0; i < children.size(); ++i) {
        const Person* child = children[i];
        bool isLastChild = (i == children.size() - 1);

        std::string childLinePrefix = childrenPrefixStem + (isLastChild ? "\\-- " : "+-- ");
        std::string grandChildrenStem = childrenPrefixStem + (isLastChild ? "    " : "|   ");

        printPersonAndDescendants(child, familyManager, childLinePrefix, grandChildrenStem, printedPersons);
    }
}

// Основной публичный метод для вывода дерева
void TreePrinter::printFamilyTree(const FamilyManager& familyManager) const {
    Person* root = familyManager.findOldestPerson(); // Находим "корень" дерева

    if (!root) {
        std::cout << "В семье нет людей для отображения." << std::endl;
        return;
    }

    std::set<int> printedPersons; // Множество для отслеживания выведенных ID

    // Выводим корневого человека без префиксов ветвления
    std::cout << this->getPersonDisplayString(*root) << std::endl;
    printedPersons.insert(root->getID());
    printSpouses(root, familyManager, "  ");

    // Выводим детей корневого человека
    const auto& children = root->getChildren();
    for (size_t i = 0; i < children.size(); ++i) {
        const Person* child = children[i];
        bool isLastChild = (i == children.size() - 1); // Является ли текущий ребенок последним в списке

        std::string childLinePrefix = isLastChild ? "\\-- " : "+-- ";
        std::string grandChildrenStem = isLastChild ? "    " : "|   ";

        printPersonAndDescendants(child, familyManager, childLinePrefix, grandChildrenStem, printedPersons);
    }
}