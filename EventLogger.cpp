#include "EventLogger.h"
#include <iostream> 

EventLogger::EventLogger() {}

EventLogger::~EventLogger() {
    for (Event* eventPtr : eventLog_) {
        delete eventPtr;
        eventPtr = nullptr;
    }
    eventLog_.clear();
}

void EventLogger::recordEvent(Event* event) {
    if (event != nullptr) {
        eventLog_.push_back(event);
    }
}

const std::vector<Event*>& EventLogger::getAllEvents() const {
    return eventLog_;
}