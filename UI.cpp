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
            std::cout << "Выход из программы..." << std::endl;
            break;
        default:
            // getUserChoice должен отсекать неверные варианты, но на всякий случай
            std::cout << "Неверный выбор. Пожалуйста, попробуйте снова." << std::endl;
        }
        if (choice != 7) { // Если не выбран выход
            std::cout << "\nНажмите Enter для продолжения...";
            std::cin.get();
        }
    } while (choice != 7);
}

void UI::displayMainMenu() const {
    std::cout << "\n===== Главное Меню =====" << std::endl;
    std::cout << "1. Добавить человека" << std::endl;
    std::cout << "2. Зарегистрировать событие смерти" << std::endl;
    std::cout << "3. Зарегистрировать событие брака" << std::endl;
    std::cout << "4. Показать информацию о человеке" << std::endl;
    std::cout << "5. Показать генеалогическое дерево" << std::endl;
    std::cout << "6. Установить связь родитель-ребенок" << std::endl;
    std::cout << "7. Выход" << std::endl;
    std::cout << "========================" << std::endl;
}

int UI::getUserChoice(int minChoice, int maxChoice) const {
    int choice = 0;
    bool validInput = false;
    do {
        std::cout << "Ваш выбор [" << minChoice << "-" << maxChoice << "]: ";
        std::cin >> choice;
        if (std::cin.good() && choice >= minChoice && choice <= maxChoice) {
            validInput = true;
        }
        else {
            std::cout << "Ошибка ввода. Пожалуйста, введите число от " << minChoice << " до " << maxChoice << "." << std::endl;
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
            std::cout << "Ошибка ввода. Пожалуйста, введите целое число." << std::endl;
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
        d = getIntInput("  Введите день (ДД): ");
        m = getIntInput("  Введите месяц (ММ): ");
        y = getIntInput("  Введите год (ГГГГ): ");

        if (date.setDate(d, m, y)) {
            dateSet = true;
        }
        else {
            std::cout << "  Некорректная дата (" << d << "." << m << "." << y << "). Пожалуйста, попробуйте снова." << std::endl;
        }
    } while (!dateSet);
    return date;
}

Person* UI::selectPerson(const std::string& promptMessage) {
    std::cout << promptMessage << std::endl;
    std::cout << "  Выберите способ поиска:" << std::endl;
    std::cout << "  1. Поиск по ID" << std::endl;
    std::cout << "  2. Поиск по имени" << std::endl;
    std::cout << "  0. Отмена" << std::endl;

    // Используем ранее определенный getUserChoice для получения выбора
    int choice = getUserChoice(0, 2);
    Person* selectedPerson = nullptr;

    switch (choice) {
    case 1: {
        int id = getIntInput("  Введите ID человека: ");
        selectedPerson = familyManager_.findPersonById(id);
        if (!selectedPerson) {
            std::cout << "  Человек с ID " << id << " не найден." << std::endl;
        }
        else {
            std::cout << "  Выбран: " << selectedPerson->getFullName() << " (ID: " << selectedPerson->getID() << ")" << std::endl;
        }
        break;
    }
    case 2: {
        std::string name = getStringInput("  Введите полное имя человека: ");
        std::vector<Person*> foundPeople = familyManager_.findPersonByName(name);

        if (foundPeople.empty()) {
            std::cout << "  Человек с именем '" << name << "' не найден." << std::endl;
        }
        else if (foundPeople.size() == 1) {
            selectedPerson = foundPeople[0];
            std::cout << "  Найден: " << selectedPerson->getFullName()
                << " (дата рожд.: " << selectedPerson->getBirthDate().toString()
                << ", ID: " << selectedPerson->getID() << ")" << std::endl;
        }
        else {
            // Найдено несколько человек, предлагаем пользователю выбрать
            std::cout << "  Найдено несколько человек с именем '" << name << "'. Пожалуйста, выберите одного:" << std::endl;
            for (size_t i = 0; i < foundPeople.size(); ++i) {
                std::cout << "    " << (i + 1) << ". " << foundPeople[i]->getFullName()
                    << " (дата рожд.: " << foundPeople[i]->getBirthDate().toString()
                    << ", ID: " << foundPeople[i]->getID() << ")" << std::endl;
            }
            std::cout << "    0. Отмена" << std::endl;

            // Получаем выбор пользователя из списка
            int selection = getUserChoice(0, static_cast<int>(foundPeople.size()));
            if (selection > 0) { // Пользователь выбрал конкретного человека
                selectedPerson = foundPeople[selection - 1];
                std::cout << "  Выбран: " << selectedPerson->getFullName() << " (ID: " << selectedPerson->getID() << ")" << std::endl;
            }
            else { // Пользователь выбрал 0 (Отмена) или ввел некорректное значение, обработанное getUserChoice
                std::cout << "  Выбор отменен или некорректен." << std::endl;
            }
        }
        break;
    }
    case 0:
        std::cout << "  Выбор способа поиска отменен." << std::endl;
        break;
    }
    return selectedPerson; // Возвращаем указатель на выбранного человека или nullptr
}

void UI::handleAddPerson() {
    std::cout << "\n--- Добавление нового человека ---" << std::endl;
    std::string fullName = getStringInput("Введите полное имя: ");

    std::cout << "Выберите пол:" << std::endl;
    std::cout << "  1. Мужской" << std::endl;
    std::cout << "  2. Женский" << std::endl;
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

    CustomDate birthDate = getDateInput("Введите дату рождения:");
    std::string occupation = getStringInput("Введите профессию/род деятельности: ");

    Person* mother = nullptr;
    Person* father = nullptr;

    std::string choiceStr = getStringInput("Хотите указать мать? (да/нет): ");
    std::transform(choiceStr.begin(), choiceStr.end(), choiceStr.begin(), ::tolower);
    if (choiceStr == "да" || choiceStr == "yes" || choiceStr == "д" || choiceStr == "y") {
        mother = selectPerson("Выберите мать из существующих людей:");
    }

    choiceStr = getStringInput("Хотите указать отца? (да/нет): ");
    std::transform(choiceStr.begin(), choiceStr.end(), choiceStr.begin(), ::tolower);
    if (choiceStr == "да" || choiceStr == "yes" || choiceStr == "д" || choiceStr == "y") {
        father = selectPerson("Выберите отца из существующих людей:");
    }

    Person* newPerson = familyManager_.addPerson(fullName, gender, birthDate, occupation, mother, father);
    if (newPerson) {
        std::cout << "Человек '" << fullName << "' (ID: " << newPerson->getID() << ") успешно добавлен." << std::endl;
    }
    else {
        std::cout << "Не удалось добавить человека." << std::endl;
    }
}

void UI::handleAddDeathEvent() {
    std::cout << "\n--- Регистрация события смерти ---" << std::endl;
    Person* person = selectPerson("Выберите человека, для которого регистрируется смерть:");
    if (person) {
        if (person->hasDeathDateOccurred()) {
            std::cout << "Для человека '" << person->getFullName() << "' уже зарегистрирована дата смерти." << std::endl;
            return;
        }
        CustomDate deathDate = getDateInput("Введите дату смерти:");

        familyManager_.createAndExecuteDeathEvent(person, deathDate);
        std::cout << "Событие смерти для '" << person->getFullName() << "' зарегистрировано." << std::endl;
    }
}

void UI::handleAddMarriageEvent() {
    std::cout << "\n--- Регистрация события брака ---" << std::endl;
    Person* spouse1 = selectPerson("Выберите первого супруга:");
    if (!spouse1) return;

    Person* spouse2 = selectPerson("Выберите второго супруга:");
    if (!spouse2) return;

    if (spouse1->getID() == spouse2->getID()) {
        std::cout << "Ошибка: Человек не может вступить в брак сам с собой." << std::endl;
        return;
    }

    CustomDate marriageDate = getDateInput("Введите дату брака:");

    familyManager_.createAndExecuteMarriageEvent(spouse1, spouse2, marriageDate);
    std::cout << "Брак между '" << spouse1->getFullName() << "' и '" << spouse2->getFullName() << "' зарегистрирован." << std::endl;
}

void UI::handleDisplayPersonInfo() {
    std::cout << "\n--- Информация о человеке ---" << std::endl;
    Person* person = selectPerson("Выберите человека для отображения информации:");
    if (person) {
        std::cout << "ID: " << person->getID() << std::endl;
        std::cout << "Имя: " << person->getFullName() << std::endl;
        std::cout << "Пол: " << genderToString(person->getGender()) << std::endl;
        std::cout << "Дата рождения: " << person->getBirthDate().toString() << std::endl;
        std::cout << "Профессия: " << person->getOccupation() << std::endl;

        if (person->hasDeathDateOccurred()) {
            std::cout << "Дата смерти: " << person->getDeathDate().toString() << std::endl;
        }
        else {
            std::cout << "Дата смерти: -" << std::endl;
        }

        Person* mother = person->getMother();
        if (mother) {
            std::cout << "Мать: " << mother->getFullName() << " (ID: " << mother->getID() << ")" << std::endl;
        }
        else {
            std::cout << "Мать: -" << std::endl;
        }

        Person* father = person->getFather();
        if (father) {
            std::cout << "Отец: " << father->getFullName() << " (ID: " << father->getID() << ")" << std::endl;
        }
        else {
            std::cout << "Отец: -" << std::endl;
        }

        std::cout << "Дети:" << std::endl;
        const auto& children = person->getChildren();
        if (children.empty()) {
            std::cout << "  -" << std::endl;
        }
        else {
            for (const Person* child : children) {
                std::cout << "  - " << child->getFullName() << " (ID: " << child->getID() << ")" << std::endl;
            }
        }

        std::cout << "Браки:" << std::endl;
        const auto& marriages = person->getMarriages();
        if (marriages.empty()) {
            std::cout << "  -" << std::endl;
        }
        else {
            for (const MarriageEvent* marriageEvent : marriages) {
                Person* spouse = (marriageEvent->getSpouse1()->getID() == person->getID()) ? marriageEvent->getSpouse2() : marriageEvent->getSpouse1();
                std::cout << "  - с " << spouse->getFullName() << " (ID: " << spouse->getID() << ")"
                    << " дата: " << marriageEvent->getEventDate().toString();
                std::cout << std::endl;
            }
        }
    }
}

void UI::handleSetParentChildLink() {
    std::cout << "\n--- Установка связи родитель-ребенок ---" << std::endl;

    Person* parent = selectPerson("Выберите родителя (или 0 для отмены):");
    if (!parent) {
        return;
    }

    Person* child = selectPerson("Выберите ребенка (для родителя " + parent->getFullName() + ", или 0 для отмены):");
    if (!child) {
        return;
    }

    if (parent->getID() == child->getID()) {
        std::cout << "Ошибка: Человек не может быть родителем самому себе." << std::endl;
        return;
    }

    bool success = familyManager_.establishParentChildRelationship(parent, child);

    if (success) {
        std::cout << "Операция по установлению связи завершена." << std::endl;
    }
    else {
        std::cout << "Не удалось установить связь (см. сообщения выше)." << std::endl;
    }
}

void UI::handleDisplayTree() {
    std::cout << "\n--- Генеалогическое дерево ---" << std::endl;
    treePrinter_.printFamilyTree(familyManager_);
}