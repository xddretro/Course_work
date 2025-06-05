#include "Event.h"
#include "Person.h"        
#include "FamilyManager.h" 
#include <iostream>        

Event::Event(const CustomDate& date, FamilyManager* context, const std::string& description)
    : eventDate_(date), familyManagerContext_(context), description_(description) {
}

CustomDate Event::getEventDate() const {
    return eventDate_;
}

std::string Event::getDescription() const {
    return description_;
}

BirthEvent::BirthEvent(const CustomDate& date, FamilyManager* context, Person* personBorn, const std::string& description)
    : Event(date, context, description), personBorn_(personBorn) {
}

void BirthEvent::execute() {
    if (personBorn_) {
        personBorn_->setOwnBirthEvent(this);
    }
    if (familyManagerContext_) {
        familyManagerContext_->recordEventInLog(this);
    }
}

Person* BirthEvent::getPersonBorn() const {
    return personBorn_;
}

DeathEvent::DeathEvent(const CustomDate& date, FamilyManager* context, Person* deceasedPerson, const std::string& description)
    : Event(date, context, description), deceasedPerson_(deceasedPerson) {
}

void DeathEvent::execute() {
    if (deceasedPerson_) {
        deceasedPerson_->recordDeath(this->eventDate_);
        deceasedPerson_->setOwnDeathEvent(this);
    }
    if (familyManagerContext_) {
        familyManagerContext_->recordEventInLog(this);
    }
}

Person* DeathEvent::getDeceasedPerson() const {
    return deceasedPerson_;
}

MarriageEvent::MarriageEvent(const CustomDate& date, FamilyManager* context, Person* spouse1, Person* spouse2, const std::string& description)
    : Event(date, context, description), spouse1_(spouse1), spouse2_(spouse2) {
}

void MarriageEvent::execute() {
    if (spouse1_) {
        spouse1_->addMarriageEvent(this);
    }
    if (spouse2_) {
        spouse2_->addMarriageEvent(this);
    }
    if (familyManagerContext_) {
        familyManagerContext_->recordEventInLog(this);
    }
}

Person* MarriageEvent::getSpouse1() const {
    return spouse1_;
}

Person* MarriageEvent::getSpouse2() const {
    return spouse2_;
}
