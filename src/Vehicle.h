#pragma once
#include <vector>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "GameTypes.h"
#include "Path.hpp"
#include "GameObject.h"
#include "Intersection.h"
#include "Tiles.hpp"

namespace tt
{

class Background;
using BackgroundSharedPtr = std::shared_ptr<Background>;

class Vehicle;
using VehiclePtr = std::shared_ptr<Vehicle>;

class Vehicle : public GameObject
{

public:
    enum TimeStep
    {
        NOOP = 0,
        DELETE_VEHICLE = 1
    };

    enum State
    {
        MOVING,
        STOPPED
    };

    Vehicle(const sf::Texture& texture, const sf::Vector2i& size, BackgroundSharedPtr bg);

    std::uint16_t timestep() override;
    bool isBlocked(const sf::FloatRect& point);

    State vehicleState() const { return _state; }
    void setVehicleState(State val);

    void setPath(const Path& path);
    const Path& path() const { return _path; }
    Path& path() { return const_cast<Path&>((static_cast<const Vehicle&>(*this)).path()); }

    void setSpeed(float v) { _speed = v; }
    float speed() const { return _speed; }

    void setDamage(std::uint16_t v) { _damage = v; }
    std::uint16_t damage() const { return _damage; }

    Direction direction() const { return _direction; }

    tt::Tile currentTile() const;

    void setHornSound(sf::SoundBuffer* v) 
    { 
        _hornbuffer = v; 
        _hornsound.setBuffer(*_hornbuffer);
    }

    void playHornSound()
    {
        _hornsound.play();
    }

    void move();

private:
    void setDirection(std::uint32_t dir);

    sf::Clock                   _movementClock;

    BackgroundSharedPtr         _bg;

    Path                        _path;
    std::vector<sf::Vector2f>   _globalPoints;
    float                       _speed = 10.0f;  // Pixels per timestep
    std::uint16_t               _damage = 0;

    Direction                   _direction = DOWN;  // Current direction of the object
    State                       _state = MOVING;

    bool                _finishedPath = false;

    sf::SoundBuffer*    _hornbuffer = nullptr;
    sf::Sound           _hornsound;

};

bool isPathBlocked(const sf::FloatRect& object, const sf::FloatRect& other,
    Direction direction, float minDistance);

////////////////////////////////////////////////////////////
/// \brief Calculate the next position in the given direction
///
/// \param point        Starting point
/// \param direction    Direction of movement
/// \param speed        Speed in pixels
///
/// \return The new global coordinates 
///
////////////////////////////////////////////////////////////
template<typename V>
inline sf::Vector2<V> vehicleStepDirection(const sf::Vector2<V>& point, Direction direction, V speed, const Scale& scale)
{
    switch (direction)
    {
    default:
        break;

    case Direction::UP:
        return { point.x, point.y - (speed * scale.y) };

    case Direction::DOWN:
        return { point.x, point.y + (speed * scale.y) };

    case Direction::LEFT:
        return { point.x - (speed * scale.x), point.y };

    case Direction::RIGHT:
        return { point.x + (speed * scale.x), point.y };
    }

    return point;
}

} // namespace tt