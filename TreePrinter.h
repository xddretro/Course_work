#ifndef TREEPRINTER_H
#define TREEPRINTER_H

#include <string>
#include <vector>
#include <set>    
#include "FamilyManager.h"

class TreePrinter {
public:
    TreePrinter();
    ~TreePrinter();

    void printFamilyTree(const FamilyManager& familyManager) const;

private:
    void printPersonAndDescendants(const Person* person,
        const FamilyManager& familyManager,
        const std::string& currentPersonPrefix,
        const std::string& childrenPrefixStem,
        std::set<int>& printedPersons) const;

    std::string getPersonDisplayString(const Person& person) const;

    void printSpouses(const Person* person,
        const FamilyManager& familyManager,
        const std::string& spousePrefix) const;
};

#endif // TREEPRINTER_H
