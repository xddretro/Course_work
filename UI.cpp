#include "UI.h"
#include "Person.h"      
#include "CustomDate.h"   
#include "Event.h"        
#include <iostream>
#include <string>
#include <vector>
#include <limits>      
#include <algorithm>    

UI::UI() {}

UI::~UI() {}

void UI::run() {
    int choice = 0;
    do {
        displayMainMenu();
        choice = getUserChoice(1, 7);

        switch (choice) {
        case 1:
            handleAddPerson();
            break;
        case 2:
            handleAddDeathEvent();
            break;
        case 3:
            handleAddMarriageEvent();
            break;
        case 4:
            handleDisplayPersonInfo();
            break;
        case 5:
            handleDisplayTree();
            break;
        case 6:
            handleSetParentChildLink();
            break;
        case 7:
            std::cout << "����� �� ���������..." << std::endl;
            break;
        default:
            // getUserChoice ������ �������� �������� ��������, �� �� ������ ������
            std::cout << "�������� �����. ����������, ���������� �����." << std::endl;
        }
        if (choice != 7) { // ���� �� ������ �����
            std::cout << "\n������� Enter ��� �����������...";
            std::cin.get();
        }
    } while (choice != 7);
}

void UI::displayMainMenu() const {
    std::cout << "\n===== ������� ���� =====" << std::endl;
    std::cout << "1. �������� ��������" << std::endl;
    std::cout << "2. ���������������� ������� ������" << std::endl;
    std::cout << "3. ���������������� ������� �����" << std::endl;
    std::cout << "4. �������� ���������� � ��������" << std::endl;
    std::cout << "5. �������� ��������������� ������" << std::endl;
    std::cout << "6. ���������� ����� ��������-�������" << std::endl;
    std::cout << "7. �����" << std::endl;
    std::cout << "========================" << std::endl;
}

int UI::getUserChoice(int minChoice, int maxChoice) const {
    int choice = 0;
    bool validInput = false;
    do {
        std::cout << "��� ����� [" << minChoice << "-" << maxChoice << "]: ";
        std::cin >> choice;
        if (std::cin.good() && choice >= minChoice && choice <= maxChoice) {
            validInput = true;
        }
        else {
            std::cout << "������ �����. ����������, ������� ����� �� " << minChoice << " �� " << maxChoice << "." << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    } while (!validInput);
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return choice;
}

std::string UI::getStringInput(const std::string& prompt) const {
    std::string input;
    std::cout << prompt;
    std::getline(std::cin, input);
    return input;
}

int UI::getIntInput(const std::string& prompt) const {
    int input = 0;
    bool validInput = false;
    do {
        std::cout << prompt;
        std::cin >> input;
        if (std::cin.good()) {
            validInput = true;
        }
        else {
            std::cout << "������ �����. ����������, ������� ����� �����." << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    } while (!validInput);
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return input;
}

CustomDate UI::getDateInput(const std::string& promptMessage) const {
    std::cout << promptMessage << std::endl;
    int d, m, y;
    CustomDate date;
    bool dateSet = false;
    do {
        d = getIntInput("  ������� ���� (��): ");
        m = getIntInput("  ������� ����� (��): ");
        y = getIntInput("  ������� ��� (����): ");

        if (date.setDate(d, m, y)) {
            dateSet = true;
        }
        else {
            std::cout << "  ������������ ���� (" << d << "." << m << "." << y << "). ����������, ���������� �����." << std::endl;
        }
    } while (!dateSet);
    return date;
}

Person* UI::selectPerson(const std::string& promptMessage) {
    std::cout << promptMessage << std::endl;
    std::cout << "  �������� ������ ������:" << std::endl;
    std::cout << "  1. ����� �� ID" << std::endl;
    std::cout << "  2. ����� �� �����" << std::endl;
    std::cout << "  0. ������" << std::endl;

    // ���������� ����� ������������ getUserChoice ��� ��������� ������
    int choice = getUserChoice(0, 2);
    Person* selectedPerson = nullptr;

    switch (choice) {
    case 1: {
        int id = getIntInput("  ������� ID ��������: ");
        selectedPerson = familyManager_.findPersonById(id);
        if (!selectedPerson) {
            std::cout << "  ������� � ID " << id << " �� ������." << std::endl;
        }
        else {
            std::cout << "  ������: " << selectedPerson->getFullName() << " (ID: " << selectedPerson->getID() << ")" << std::endl;
        }
        break;
    }
    case 2: {
        std::string name = getStringInput("  ������� ������ ��� ��������: ");
        std::vector<Person*> foundPeople = familyManager_.findPersonByName(name);

        if (foundPeople.empty()) {
            std::cout << "  ������� � ������ '" << name << "' �� ������." << std::endl;
        }
        else if (foundPeople.size() == 1) {
            selectedPerson = foundPeople[0];
            std::cout << "  ������: " << selectedPerson->getFullName()
                << " (���� ����.: " << selectedPerson->getBirthDate().toString()
                << ", ID: " << selectedPerson->getID() << ")" << std::endl;
        }
        else {
            // ������� ��������� �������, ���������� ������������ �������
            std::cout << "  ������� ��������� ������� � ������ '" << name << "'. ����������, �������� ������:" << std::endl;
            for (size_t i = 0; i < foundPeople.size(); ++i) {
                std::cout << "    " << (i + 1) << ". " << foundPeople[i]->getFullName()
                    << " (���� ����.: " << foundPeople[i]->getBirthDate().toString()
                    << ", ID: " << foundPeople[i]->getID() << ")" << std::endl;
            }
            std::cout << "    0. ������" << std::endl;

            // �������� ����� ������������ �� ������
            int selection = getUserChoice(0, static_cast<int>(foundPeople.size()));
            if (selection > 0) { // ������������ ������ ����������� ��������
                selectedPerson = foundPeople[selection - 1];
                std::cout << "  ������: " << selectedPerson->getFullName() << " (ID: " << selectedPerson->getID() << ")" << std::endl;
            }
            else { // ������������ ������ 0 (������) ��� ���� ������������ ��������, ������������ getUserChoice
                std::cout << "  ����� ������� ��� �����������." << std::endl;
            }
        }
        break;
    }
    case 0:
        std::cout << "  ����� ������� ������ �������." << std::endl;
        break;
    }
    return selectedPerson; // ���������� ��������� �� ���������� �������� ��� nullptr
}

void UI::handleAddPerson() {
    std::cout << "\n--- ���������� ������ �������� ---" << std::endl;
    std::string fullName = getStringInput("������� ������ ���: ");

    std::cout << "�������� ���:" << std::endl;
    std::cout << "  1. �������" << std::endl;
    std::cout << "  2. �������" << std::endl;
    int genderChoice = getUserChoice(1, 2);
    Gender gender;

    switch (genderChoice) {
    case 1:
        gender = Gender::MALE;
        break;
    case 2:
        gender = Gender::FEMALE;
        break;
    }

    CustomDate birthDate = getDateInput("������� ���� ��������:");
    std::string occupation = getStringInput("������� ���������/��� ������������: ");

    Person* mother = nullptr;
    Person* father = nullptr;

    std::string choiceStr = getStringInput("������ ������� ����? (��/���): ");
    std::transform(choiceStr.begin(), choiceStr.end(), choiceStr.begin(), ::tolower);
    if (choiceStr == "��" || choiceStr == "yes" || choiceStr == "�" || choiceStr == "y") {
        mother = selectPerson("�������� ���� �� ������������ �����:");
    }

    choiceStr = getStringInput("������ ������� ����? (��/���): ");
    std::transform(choiceStr.begin(), choiceStr.end(), choiceStr.begin(), ::tolower);
    if (choiceStr == "��" || choiceStr == "yes" || choiceStr == "�" || choiceStr == "y") {
        father = selectPerson("�������� ���� �� ������������ �����:");
    }

    Person* newPerson = familyManager_.addPerson(fullName, gender, birthDate, occupation, mother, father);
    if (newPerson) {
        std::cout << "������� '" << fullName << "' (ID: " << newPerson->getID() << ") ������� ��������." << std::endl;
    }
    else {
        std::cout << "�� ������� �������� ��������." << std::endl;
    }
}

void UI::handleAddDeathEvent() {
    std::cout << "\n--- ����������� ������� ������ ---" << std::endl;
    Person* person = selectPerson("�������� ��������, ��� �������� �������������� ������:");
    if (person) {
        if (person->hasDeathDateOccurred()) {
            std::cout << "��� �������� '" << person->getFullName() << "' ��� ���������������� ���� ������." << std::endl;
            return;
        }
        CustomDate deathDate = getDateInput("������� ���� ������:");

        familyManager_.createAndExecuteDeathEvent(person, deathDate);
        std::cout << "������� ������ ��� '" << person->getFullName() << "' ����������������." << std::endl;
    }
}

void UI::handleAddMarriageEvent() {
    std::cout << "\n--- ����������� ������� ����� ---" << std::endl;
    Person* spouse1 = selectPerson("�������� ������� �������:");
    if (!spouse1) return;

    Person* spouse2 = selectPerson("�������� ������� �������:");
    if (!spouse2) return;

    if (spouse1->getID() == spouse2->getID()) {
        std::cout << "������: ������� �� ����� �������� � ���� ��� � �����." << std::endl;
        return;
    }

    CustomDate marriageDate = getDateInput("������� ���� �����:");

    familyManager_.createAndExecuteMarriageEvent(spouse1, spouse2, marriageDate);
    std::cout << "���� ����� '" << spouse1->getFullName() << "' � '" << spouse2->getFullName() << "' ���������������." << std::endl;
}

void UI::handleDisplayPersonInfo() {
    std::cout << "\n--- ���������� � �������� ---" << std::endl;
    Person* person = selectPerson("�������� �������� ��� ����������� ����������:");
    if (person) {
        std::cout << "ID: " << person->getID() << std::endl;
        std::cout << "���: " << person->getFullName() << std::endl;
        std::cout << "���: " << genderToString(person->getGender()) << std::endl;
        std::cout << "���� ��������: " << person->getBirthDate().toString() << std::endl;
        std::cout << "���������: " << person->getOccupation() << std::endl;

        if (person->hasDeathDateOccurred()) {
            std::cout << "���� ������: " << person->getDeathDate().toString() << std::endl;
        }
        else {
            std::cout << "���� ������: -" << std::endl;
        }

        Person* mother = person->getMother();
        if (mother) {
            std::cout << "����: " << mother->getFullName() << " (ID: " << mother->getID() << ")" << std::endl;
        }
        else {
            std::cout << "����: -" << std::endl;
        }

        Person* father = person->getFather();
        if (father) {
            std::cout << "����: " << father->getFullName() << " (ID: " << father->getID() << ")" << std::endl;
        }
        else {
            std::cout << "����: -" << std::endl;
        }

        std::cout << "����:" << std::endl;
        const auto& children = person->getChildren();
        if (children.empty()) {
            std::cout << "  -" << std::endl;
        }
        else {
            for (const Person* child : children) {
                std::cout << "  - " << child->getFullName() << " (ID: " << child->getID() << ")" << std::endl;
            }
        }

        std::cout << "�����:" << std::endl;
        const auto& marriages = person->getMarriages();
        if (marriages.empty()) {
            std::cout << "  -" << std::endl;
        }
        else {
            for (const MarriageEvent* marriageEvent : marriages) {
                Person* spouse = (marriageEvent->getSpouse1()->getID() == person->getID()) ? marriageEvent->getSpouse2() : marriageEvent->getSpouse1();
                std::cout << "  - � " << spouse->getFullName() << " (ID: " << spouse->getID() << ")"
                    << " ����: " << marriageEvent->getEventDate().toString();
                std::cout << std::endl;
            }
        }
    }
}

void UI::handleSetParentChildLink() {
    std::cout << "\n--- ��������� ����� ��������-������� ---" << std::endl;

    Person* parent = selectPerson("�������� �������� (��� 0 ��� ������):");
    if (!parent) {
        return;
    }

    Person* child = selectPerson("�������� ������� (��� �������� " + parent->getFullName() + ", ��� 0 ��� ������):");
    if (!child) {
        return;
    }

    if (parent->getID() == child->getID()) {
        std::cout << "������: ������� �� ����� ���� ��������� ������ ����." << std::endl;
        return;
    }

    bool success = familyManager_.establishParentChildRelationship(parent, child);

    if (success) {
        std::cout << "�������� �� ������������ ����� ���������." << std::endl;
    }
    else {
        std::cout << "�� ������� ���������� ����� (��. ��������� ����)." << std::endl;
    }
}

void UI::handleDisplayTree() {
    std::cout << "\n--- ��������������� ������ ---" << std::endl;
    treePrinter_.printFamilyTree(familyManager_);
}