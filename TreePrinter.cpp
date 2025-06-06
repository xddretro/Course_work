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

    // ������� �������� �� ���������� ���� ����� ��� ��������������
    if (birthDate.getYear() >= 1) { // ��� < 1 ���������
        oss << " (" << birthDate.getYear();
        if (person.hasDeathDateOccurred()) {
            CustomDate deathDate = person.getDeathDate();
            if (deathDate.getYear() >= 1) {
                oss << "-" << deathDate.getYear();
            }
            else {
                oss << "-?"; // ���� ���� ������ ����, �� ��� ���������
            }
        }
        else {
            oss << "-"; // ������� ��� ��� ���� ������ �� �������
        }
        oss << ")";
    }
    else {
        oss << " (���� �����.)";
    }
    return oss.str();
}

// ������� ���������� � �������� ��������
void TreePrinter::printSpouses(const Person* person,
    const FamilyManager& familyManager,
    const std::string& childrenVerticalStem) const {
    if (!person) return;
    const auto& marriages = person->getMarriages(); // �������� ������ ������� �����
    if (marriages.empty()) return;

    bool firstSpousePrinted = false;
    for (const MarriageEvent* marriageEvent : marriages) {
        if (!marriageEvent) continue;
        Person* spouse = nullptr;
        // ���������� ������� ������� � ���� ������� �����
        if (marriageEvent->getSpouse1() && marriageEvent->getSpouse1()->getID() != person->getID()) {
            spouse = marriageEvent->getSpouse1();
        }
        else if (marriageEvent->getSpouse2() && marriageEvent->getSpouse2()->getID() != person->getID()) {
            spouse = marriageEvent->getSpouse2();
        }

        if (spouse) {
            std::cout << childrenVerticalStem << "  +-- (������/�) "
                << getPersonDisplayString(*spouse)
                << " [����: " << marriageEvent->getEventDate().toString() << "]"
                << std::endl;
            firstSpousePrinted = true;
        }
    }
}

// ����������� ������� ��� ������ �������� � ��� ��������
void TreePrinter::printPersonAndDescendants(const Person* person,
    const FamilyManager& familyManager,
    const std::string& currentPersonPrefix,
    const std::string& childrenPrefixStem,
    std::set<int>& printedPersons) const {
    if (!person) {
        return;
    }
    // ��������, �� ��� �� ���� ������� ��� �������
    if (printedPersons.count(person->getID())) {
        std::cout << currentPersonPrefix << "*" << getPersonDisplayString(*person) << " (��� �������)" << std::endl;
        return;
    }
    printedPersons.insert(person->getID());

    // ������� �������� ��������
    std::cout << currentPersonPrefix << this->getPersonDisplayString(*person) << std::endl;

    printSpouses(person, familyManager, childrenPrefixStem);

    // ������� �����
    const auto& children = person->getChildren();
    for (size_t i = 0; i < children.size(); ++i) {
        const Person* child = children[i];
        bool isLastChild = (i == children.size() - 1);

        std::string childLinePrefix = childrenPrefixStem + (isLastChild ? "\\-- " : "+-- ");
        std::string grandChildrenStem = childrenPrefixStem + (isLastChild ? "    " : "|   ");

        printPersonAndDescendants(child, familyManager, childLinePrefix, grandChildrenStem, printedPersons);
    }
}

// �������� ��������� ����� ��� ������ ������
void TreePrinter::printFamilyTree(const FamilyManager& familyManager) const {
    Person* root = familyManager.findOldestPerson(); // ������� "������" ������

    if (!root) {
        std::cout << "� ����� ��� ����� ��� �����������." << std::endl;
        return;
    }

    std::set<int> printedPersons; // ��������� ��� ������������ ���������� ID

    // ������� ��������� �������� ��� ��������� ���������
    std::cout << this->getPersonDisplayString(*root) << std::endl;
    printedPersons.insert(root->getID());
    printSpouses(root, familyManager, "  ");

    // ������� ����� ��������� ��������
    const auto& children = root->getChildren();
    for (size_t i = 0; i < children.size(); ++i) {
        const Person* child = children[i];
        bool isLastChild = (i == children.size() - 1); // �������� �� ������� ������� ��������� � ������

        std::string childLinePrefix = isLastChild ? "\\-- " : "+-- ";
        std::string grandChildrenStem = isLastChild ? "    " : "|   ";

        printPersonAndDescendants(child, familyManager, childLinePrefix, grandChildrenStem, printedPersons);
    }
}