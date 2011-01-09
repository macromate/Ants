#pragma once
#ifndef DEFINITIONS_HPP
#define DEFINITIONS_HPP

#include <vector>

#define PATH_TO_IMAGES "assets/images/"
#define PATH_TO_SCRIPTS "assets/scripts/"

class Coordinate {
public:
  Coordinate(int x, int y);
  inline void setX(int x) { mX = x; }
  inline void setY(int y) { mY = y; }
  inline int getX() const { return mX; }
  inline int getY() const { return mY; }
protected:
  int mX, mY;
};

// converts to screen coordinates
int ConvertXCoordinate(int x);
int ConvertYCoordinate(int y);

class GameObject;
typedef std::vector<GameObject*> GameObjects;

class Ant;
typedef std::vector<Ant*> Ants;

enum EventType {
	ETYPE_ALL, 
	ETYPE_IN_RADIUS, 
	ETYPE_LEAVES_RADIUS,
	ETYPE_ANT_STARTS_ATTACK,
	ETYPE_INVENTORY_ADD
};

enum Strain {
	StrainRed = 0, 
	StrainBlue = 1, 
	StrainYellow = 3
};

#endif
