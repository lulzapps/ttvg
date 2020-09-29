#pragma once
#include <functional>

#include <boost/signals2.hpp>

#include <SFML/Graphics.hpp>
#include <nlohmann/json.hpp>

#include "GameTypes.h"
#include "IUpdateable.h"

namespace nl = nlohmann;

namespace tt
{

class AnimatedSprite;
using AnimatedSpritePtr = std::shared_ptr<AnimatedSprite>;
void from_json(const nl::json& j, AnimatedSprite& i);

struct AnimatedState2;
using AnimatedState2Ptr = std::shared_ptr<AnimatedState2>;
using AnimatedState2Map = std::map<std::string, AnimatedState2>;
void from_json(const nl::json& j, AnimatedState2& i);

struct AnimatedState2
{
    std::string     id;
    sf::Vector2i    source;
    std::uint32_t   count;
};


class AnimatedSprite :
    public sf::Drawable,
    public sf::Transformable,
    public IUpdateable
{

public:
    friend void from_json(const nl::json& j, AnimatedSprite& i);

    AnimatedSprite(const sf::Texture& texture, const sf::Vector2i& size);

    AnimatedState state() const;
    void setState(AnimatedState state);

    Direction direction() const { return _direction; }
    void setDirection(Direction val) { _direction = val; }

    void setSource(std::uint32_t x, std::uint32_t y);
    void setMaxFramesPerRow(std::uint32_t max);
    
    void setHighlighted(bool h);
    bool highlighted() const { return _highlight.getSize().x != 0; }
    
    sf::RectangleShape& highlight() { return _highlight; }

    sf::FloatRect getGlobalBounds() const;

    std::uint16_t timestep() override;

    boost::signals2::signal<void(void)> onUpdate;

protected:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override final;

    sf::Vector2i      _size; // fixed cell size of each frame within the sprite

    AnimatedState           _state = AnimatedState::STILL;
    Direction               _direction = Direction::DOWN;
    sf::Vector2i            _source;
    sf::Clock               _timer;

    // some sprite sheets have different frames per row
    // so this allows us to adjust how many frames get
    // animated in a particular row
    std::uint32_t           _maxFramesPerRow = 0;

    sf::Sprite              _sprite;
    sf::RectangleShape      _highlight;


    AnimatedState2Map       _states;
};

} // namespace tt