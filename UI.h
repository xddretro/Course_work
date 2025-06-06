#ifndef UI_H
#define UI_H

#include <string>
#include "FamilyManager.h"
#include "TreePrinter.h"

class UI {
public:
    UI();
    ~UI();

    void run();

private:
    FamilyManager familyManager_; // UI владеет экземпл€ром FamilyManager
    TreePrinter treePrinter_;     // UI владеет экземпл€ром TreePrinter

    void displayMainMenu() const;
    int getUserChoice(int minChoice, int maxChoice) const;
    std::string getStringInput(const std::string& prompt) const;
    int getIntInput(const std::string& prompt) const;
    CustomDate getDateInput(const std::string& prompt) const;

    void handleAddPerson();
    void handleAddDeathEvent();
    void handleAddMarriageEvent();
    void handleDisplayPersonInfo();
    void handleDisplayTree();
    void handleSetParentChildLink();

    // ¬спомогательный метод дл€ выбора человека ---
    Person* selectPerson(const std::string& promptMessage);
};

#endif // UI_