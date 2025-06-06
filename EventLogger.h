#ifndef EVENTLOGGER_H
#define EVENTLOGGER_H

#include <vector>
#include "Event.h" 

class EventLogger {
private:
    std::vector<Event*> eventLog_;

public:
    EventLogger();
    ~EventLogger();
    void recordEvent(Event* event);

    const std::vector<Event*>& getAllEvents() const;
};

#endif // EVENTLOGGER_H