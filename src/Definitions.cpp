#include "Definitions.hpp"

Coordinate::Coordinate(int x, int y) :
mX(x),
mY(y)
{
}

Direction directionForVector(const sf::Vector2f& vector) {
    if (vector.x == 0 && vector.y == -1) {
        return North;
    } else if (vector.x == 0 && vector.y == 1) {
        return South;
    } else if (vector.x == 1 && vector.y == 0) {
        return East;
    } else if (vector.x == -1 && vector.y == 0) {
        return West;
    } else if (vector.x == 1 && vector.y == -1) {
        return NorthEast;
    } else if (vector.x == -1 && vector.y == -1) {
        return NorthWest;
    } else if (vector.x == 1 && vector.y == 1) {
        return SouthEast;
    } else if (vector.x == -1 && vector.y == 1) {
        return SouthWest;
    }
}
