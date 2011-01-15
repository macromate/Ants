#include "AntMoves.hpp"
AntMoves::AntMoves(Ant* ant) : 
mAnt(ant)
{
    setEventType(ETYPE_ANT_MOVES);
}

GameObjects AntMoves::getReceivers() const {
    GameObjects tmp;
    tmp.push_back((GameObject*)mAnt);
    return tmp;
}