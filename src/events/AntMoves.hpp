#ifndef ANT_MOVES_HPP
#define ANT_MOVES_HPP

#include "Definitions.hpp"
#include "Event.hpp"

class AntMoves : public Event {
public:
  AntMoves(Ant* ant);
  GameObjects getReceivers() const;
  inline Ant* getAnt() const { return mAnt; }
protected:
  Ant* mAnt;
};


#endif
