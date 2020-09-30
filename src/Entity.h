#pragma once

#include <SFML/Graphics.hpp>

#include "GameTypes.h"
#include "IUpdateable.h"

namespace tt
{

class EntityState
{
    std::string     id;
    sf::Vector2i    source;
    std::uint32_t   frameCount = 1;
};


class Entity : 
    public sf::Drawable,
    public sf::Transformable,
    public IUpdateable
{

};

class EntityFactory
{

};

} // namespace tt