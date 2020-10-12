#pragma once
#include <functional>
#include <optional>

#include <boost/signals2.hpp>

#include <SFML/Graphics.hpp>
#include <nlohmann/json.hpp>

#include "GameTypes.h"
#include "IUpdateable.h"

namespace nl = nlohmann;

namespace tt
{

class GameObject;
using GameObjectPtr = std::shared_ptr<GameObject>;

struct GameObjectInfo;
void from_json(const nl::json& j, GameObjectInfo& i);

struct GameObjectState;
using GameObjectStates = std::map<std::string, GameObjectState>;
void from_json(const nl::json& j, GameObjectState& i);

struct GameObjectState
{
    std::string     id;
    sf::Vector2i    source;

    std::optional<std::uint32_t>    count;
};

struct GameObjectInfo
{
    std::string     name;
    std::string     description;

    std::optional<sf::Vector2u>     size;
    std::optional<sf::Vector2f>     scale;
    std::optional<std::uint32_t>    count;

    std::optional<GameObjectStates> states;
};

class GameObject :
    public sf::Drawable,
    public sf::Transformable,
    public IUpdateable
{

public:

    GameObject(const GameObjectInfo& info, const sf::Texture& texture);
    GameObject(const sf::Texture& texture, const sf::Vector2i& size);

    void setState(const std::string& statename);
    
    void setHighlighted(bool h);
    bool highlighted() const { return _highlight.getSize().x != 0; }
    
    sf::RectangleShape& highlight() { return _highlight; }

    sf::FloatRect getGlobalBounds() const;

    std::uint16_t timestep() override;

    bool animated() const { return _animated; }
    void setAnimated(bool v);

public: // signals
    
    boost::signals2::signal<void(void)> onFrameChange;

protected:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override final;

    sf::Vector2u    _size; // fixed cell size of each frame within the sprite 
    sf::Clock       _timer;

    // some sprite sheets have different frames per row
    // so this allows us to adjust how many frames get
    // animated in a particular row
    std::uint32_t   _maxFramesPerRow = 0;
    sf::Vector2i    _source;

    sf::Sprite          _sprite;
    sf::RectangleShape  _highlight;

    GameObjectStates    _states;
    bool                _animated = false;
};

} // namespace tt