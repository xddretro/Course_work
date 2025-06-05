#ifndef FAMILYMANAGER_H
#define FAMILYMANAGER_H

#include <string>
#include <vector>
#include <map>
#include "Person.h" 
#include "Event.h"  

class EventLogger;

class FamilyManager {
private:
    std::map<int, Person*> people_; // ’ранилище всех людей в семье, ключ - ID человека
    EventLogger* eventLogger_;        // ”казатель на логгер/реестр событий

public:
    FamilyManager();
    ~FamilyManager();

    Person* addPerson(const std::string& fullName, Gender gender, const CustomDate& birthDate,
        const std::string& occupation,
        Person* mother = nullptr, Person* father = nullptr);

    bool establishParentChildRelationship(Person* parent, Person* child);

    Person* findPersonById(int personID) const;
    std::vector<Person*> findPersonByName(const std::string& name) const;
    const std::map<int, Person*>& getAllPeople() const;

    void createAndExecuteDeathEvent(Person* deceasedPerson, const CustomDate& date);
    void createAndExecuteMarriageEvent(Person* spouse1, Person* spouse2, const CustomDate& date);

    void recordEventInLog(Event* event);

    EventLogger* getEventLogger() const;

    Person* findOldestPerson() const;
};

#endif // FAMILYMANAGER_H