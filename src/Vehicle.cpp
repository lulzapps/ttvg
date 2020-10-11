#include <iostream>
#include <cmath>

#include "TTUtils.h"
#include "Background.h"
#include "Vehicle.h"
#include "Intersection.h"

namespace tt
{

bool isPathBlocked(const sf::FloatRect& object, const sf::FloatRect& other,
    Direction direction, float minDistance)
{
    switch (direction)
    {
        default:
        return false;

        case Direction::UP:
        {
            auto temprect = object;
            temprect.top -= minDistance;
            return temprect.intersects(other);
        }

        case Direction::DOWN:
        {
            auto temprect = object;
            temprect.top += minDistance;
            return temprect.intersects(other);
        }

        case Direction::LEFT:
        {
            auto temprect = object;
            temprect.left -= minDistance;
            return temprect.intersects(other);
        }

        case Direction::RIGHT:
        {
            auto temprect = object;
            temprect.left += minDistance;
            return temprect.intersects(other);
        }
    }

    return false;
}

bool shouldTurn(const sf::Vector2f pathpoint, const sf::Vector2f& current, Direction direction)
{
    switch (direction)
    {
        default:
            return false;

        case RIGHT:
        {
            return current.x >= pathpoint.x;
        }

        case LEFT:
        {
            return current.x <= pathpoint.x;
        }

        case UP:
        {
            return current.y <= pathpoint.y;
        }

        case DOWN:
        {
            return current.y >= pathpoint.y;
        }
    }

    return false;
}

Vehicle::Vehicle(const VehicleInfo& info, const sf::Texture& texture, BackgroundSharedPtr bg)
    : GameObject{ info.object, texture },
      _bg{ bg }
{
    // TODO: this will probably be removed
    setSource(0, 0);
    setAnimated(true);
}

Vehicle::Vehicle(const sf::Texture& texture, const sf::Vector2i & size, BackgroundSharedPtr bg)
    : GameObject(texture, size),
      _bg { bg }
{
    setSource(0, 0);
    setAnimated(true);
}

std::uint16_t Vehicle::timestep()
{
    GameObject::timestep();

    if (_state == State::MOVING
        && _movementClock.getElapsedTime().asMilliseconds() > 100)
    {
        move();
        _movementClock.restart();
    }

    if (_path.finished())
    {
        return TimeStep::DELETE_VEHICLE;
    }

    return TimeStep::NOOP;
}

void Vehicle::move()
{
    auto[xpos, ypos, h, w] = getGlobalBounds();
    auto currentTile = _bg->getTileFromGlobal(sf::Vector2f{ xpos, ypos });

    if (currentTile != _path.next())
    {
        auto nextpos = vehicleStepDirection(
            sf::Vector2f{ xpos, ypos }, _direction, _speed, _bg->getScale());

        if (shouldTurn(_globalPoints.at(_path.nextIndex()), nextpos, _direction))
        {
            _path.step();
            nextpos = _globalPoints.at(_path.index());

            const auto ntile = _bg->getTileFromGlobal(nextpos);
            const auto newdirection = tt::getDirection(ntile, _path.next());
            setDirection(newdirection);
        }

        setPosition(nextpos);
        return;
    }
    else
    {
        auto globalPos = _bg->getGlobalFromTile(currentTile);
        setPosition(globalPos);
        _path.step();

        auto newdirection = tt::getDirection(currentTile, _path.next());
        setDirection(newdirection);
    }
}

tt::Tile Vehicle::currentTile() const
{
    auto bounds = getGlobalBounds();
    return _bg->getTileFromGlobal(sf::Vector2f{ bounds.left, bounds.top });
}

bool Vehicle::isBlocked(const sf::FloatRect& test)
{
    const auto minDistance = 6.f;
    return isPathBlocked(getGlobalBounds(), test, _direction, minDistance);
}

void Vehicle::setVehicleState(State val) 
{ 
    _state = val; 
}

void Vehicle::setPath(const Path& path)
{
    assert(path.points().size() > 1);

    // VECTOR COPY IN THE GAME LOOP!!!
    _path = path;
    for (const auto& p : _path.points())
    {
        auto gp = _bg->getGlobalFromTile(p);
        _globalPoints.push_back(gp);
    }

    auto& start = _path.points().at(0);
    auto& next = _path.points().at(1);

    auto direction = tt::getDirection(start, next);
    setDirection(direction);

    auto globalPos = _bg->getGlobalFromTile(_path.points().at(0));
    setPosition(globalPos);
}

void Vehicle::setDirection(std::uint32_t dir)
{
    assert(dir == 0 || exactly_one_bit_set(dir));
    _direction = static_cast<Direction>(dir);

    switch (_direction)
    {
        case NONE:
        case DOWN:
        default:
        {
            //setSource(0, 0);
            setState("down");
        }
        break;

        case UP:
        {
            //setSource(0, 3);
            setState("up");
        }
        break;

        case LEFT:
        {
            //setSource(0, 1);
            setState("left");
        }
        break;

        case RIGHT:
        {
            //setSource(0, 2);
            setState("right");
        }
        break;
    }
}

} // namespace tt