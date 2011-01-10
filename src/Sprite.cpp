#include "Sprite.hpp"

using ants::Sprite;

Sprite::Sprite() {
    sf::Sprite::Sprite();
    SetCenter(16.0f, 16.0f); // alle bilder sind 32px groß
//	SetBlendMode(sf::Blend::None);
}
