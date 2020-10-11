#include <iostream>

#include "GameObject.h"
#include "TTUtils.h"

namespace nl = nlohmann;

namespace tt
{

void from_json(const nl::json& j, GameObjectInfo& i)
{
    if (j.contains("name")) j["name"].get_to(i.name);
    if (j.contains("description")) j["description"].get_to(i.description);
    
    if (j.contains("size"))
    {
        i.size = j["size"].get<sf::Vector2u>();
    }

    if (j.contains("scale"))
    {
        i.scale = j["scale"].get<sf::Vector2f>();
    }

    if (j.contains("count"))
    {
        i.count = j["count"].get<std::uint32_t>();
    }

    if (j.contains("states")
        && j["states"].is_array())
    {
        i.states.emplace();

        for (const auto& item : j["states"].items())
        {
            auto state = item.value().get<GameObjectState>();
            i.states->insert(std::make_pair(state.id, std::move(state)));
        }
    }
}

void from_json(const nl::json& j, GameObjectState& state)
{
    if (j.contains("id"))
    {
        j["id"].get_to(state.id);
    }

    if (j.contains("source"))
    {
        state.source = j["source"].get<sf::Vector2i>();
    }

    if (j.contains("count"))
    {
        state.count = j["count"].get<std::uint32_t>();
    }
}

GameObject::GameObject(const GameObjectInfo& info, const sf::Texture& texture)
{
    _sprite.setTexture(texture);

    if (info.size.has_value())
    {
        _size = *info.size;
        _sprite.setTextureRect(sf::IntRect(0, 0, _size.x, _size.y));
    }
    else
    {
        _size = _sprite.getTexture()->getSize();
    }

    if (info.scale.has_value())
    {
        setScale(*(info.scale));
    }

    std::uint32_t count = 1;
    if (info.count.has_value())
    {
        count = *(info.count);
    }

    if (info.states.has_value())
    {
        // TODO: container copy!!!
        for (const auto& [id, state] : *(info.states))
        {
            GameObjectState newstate = state;
            if (!newstate.count.has_value()) newstate.count = count;
            _states.emplace(id, newstate);
        }
    }
    else
    {
        _states.insert(
            std::make_pair("default", GameObjectState{ "default", sf::Vector2i{0,0}, 1 }));
    }

    _highlight.setFillColor(sf::Color::Transparent);
    _highlight.setOutlineThickness(2);
    _highlight.setOutlineColor(sf::Color(255, 255, 255));
}

// TODO: THIS CONSTRUCTOR WILL BE REFACTORED OUT
GameObject::GameObject(const sf::Texture& texture, const sf::Vector2i& size)
    :   _size{ size }
{
    _sprite.setTexture(texture);
    _sprite.setTextureRect(sf::IntRect(0, 0, _size.x, _size.y));

    _highlight.setFillColor(sf::Color::Transparent);
    _highlight.setOutlineThickness(2);
    _highlight.setOutlineColor(sf::Color(255, 255, 255));
}

void GameObject::setSource(std::uint32_t x, std::uint32_t y)
{
    _source.x = x;
    _source.y = y;
    _sprite.setTextureRect(sf::IntRect(
        _source.x * _size.x, _source.y * _size.y, _size.x, _size.y));
}

void GameObject::setState(const std::string& statename)
{
    const auto& state = _states.at(statename);
    _source = state.source;
    _sprite.setTextureRect(sf::IntRect(
        _source.x * _size.x, _source.y * _size.y, _size.x, _size.y));
}

std::uint16_t GameObject::timestep()
{
    if (animated()
        && _timer.getElapsedTime().asMilliseconds() > 55)
    {
        auto[left, top] = _source;
        left++;

        auto textureWidth = _maxFramesPerRow > 0 ?
            _maxFramesPerRow * _size.x : _sprite.getTexture()->getSize().x;

        if (static_cast<std::uint32_t>(left * _size.x) >= textureWidth)
        {
            left = 0;
        }

        setSource(left, top);
        
        onFrameChange();

        _timer.restart();
    }

    _highlight.setPosition(getPosition());
    return 0;
}

sf::FloatRect GameObject::getGlobalBounds() const
{
    const auto textureRect = _sprite.getTextureRect();
    float width = static_cast<float>(std::abs(textureRect.width));
    float height = static_cast<float>(std::abs(textureRect.height));
    return getTransform().transformRect(sf::FloatRect(0.f, 0.f, width, height));
}

void GameObject::setHighlighted(bool h)
{
    if (h)
    {
        _highlight.setPosition(getPosition());
        auto b = getGlobalBounds();
        _highlight.setSize(sf::Vector2f{ b.width, b.height });
    }
    else
    {
        _highlight.setSize(sf::Vector2f{ 0.f, 0.f });
    }
}

void GameObject::setAnimated(bool v)
{
    _animated = v;
    _timer.restart();
}

void GameObject::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    target.draw(_sprite, states);
    target.draw(_highlight);
}

} // namespace tt