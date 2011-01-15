#pragma once
#ifndef SPICE_HPP
#define SPICE_HPP

#include "Definitions.hpp"
#include "GameObject.hpp"

class Spice : public GameObject {
public:
	Spice(Node*);
	virtual ~Spice();
};

#endif
