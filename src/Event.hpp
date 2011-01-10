#pragma once
#ifndef EVENT_HPP
#define EVENT_HPP

#include "Definitions.hpp"

#include <vector>

class Event {
public:
  virtual GameObjects getReceivers() const = 0;
  inline virtual EventType getEventType() const { return mEventType; }
protected:
  virtual inline void setEventType(EventType type) { mEventType = type; }
  EventType mEventType;
};

// #include "events/GameObjectArrivesInRadius.hpp"


#endif
