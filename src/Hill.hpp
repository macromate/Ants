#ifndef HILL_HPP
#define HILL_HPP

#include "Definitions.hpp"
#include "GameObject.hpp"

class Node;

class Hill : public GameObject {
public:
    Hill(Strain, Node*);
    ~Hill();
    inline Strain getStrain() const { return mStrain; }
    void receiveEvent(Event*);
protected:
    Strain mStrain;
};

#endif