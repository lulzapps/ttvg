#pragma once

#include <SFML/Graphics.hpp>

#include "../Background.h"
#include "../Player.h"

#include "Scene.h"

namespace tt
{

class StatusBar : public Scene
{
    std::shared_ptr<sf::RectangleShape> _background;

public: 
    StatusBar(ResourceManager& resmgr, sf::RenderTarget& target)
        : Scene(resmgr, target)
    {
        _background = std::make_shared<sf::RectangleShape>();
        _background->setFillColor(sf::Color{ 0, 0, 0, 104 });
        _background->setPosition(5.f, 2.f);
        _background->setSize(sf::Vector2f{ _window.getSize().x - 10.f, 40.f });
        addDrawable(_background);
    }
};

class DebugWindow : public Scene
{
    sf::Font    _debugFont;

    std::shared_ptr<sf::RectangleShape> _background;
    std::shared_ptr<sf::Text>           _debugText;

public:
    DebugWindow(ResourceManager& resmgr, sf::RenderTarget& target)
        : Scene(resmgr, target)
    {
        if (auto temp = _resources.load<sf::Font>("fonts/mono_bold.ttf");
                !temp.has_value())
        {
            throw std::runtime_error("could not load resource 'fonts/mono_bold.ttf'");
        }
        else
        {
            _debugFont = *(temp);
        }

        _background = std::make_shared<sf::RectangleShape>();
        _background->setFillColor(sf::Color{ 0, 0, 0, 104 });
        _background->setPosition(0.f, _window.getSize().y - 40.f);
        _background->setSize(sf::Vector2f{ static_cast<float>(_window.getSize().x), 40.f });
        addDrawable(_background);

        _debugText = std::make_shared<sf::Text>("", _debugFont);
        _debugText->setFillColor(sf::Color::White);
        _debugText->setPosition(10.f, _window.getSize().y - 35.f);
        _debugText->setCharacterSize(20);
        addDrawable(_debugText);
    }

    void setText(const std::string& text)
    {
        _debugText->setString(text);
    }
};

class Opening : public Scene
{

public:
    Opening(ResourceManager& resmgr, sf::RenderTarget& target);

    std::uint16_t poll(const sf::Event& e) override;
    std::uint16_t timestep() override;
    void draw() override;

    sf::Vector2u getPlayerTile() const;

private:

    void adjustView();
    void animeCallback();

    StatusBar                           _statusBar;
    DebugWindow                         _debugWindow;

    BackgroundSharedPtr                 _background;
    PlayerPtr                           _player;
};

} // namespace tt