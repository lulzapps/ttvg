#include <sstream>

#include <boost/range/adaptor/indexed.hpp>

#include <fmt/core.h>

#include "../TTUtils.h"

#include "Opening.h"

namespace tt
{

constexpr auto SCALE_PLAYER = 1.50f;
constexpr auto SCALE_BACKGROUND = 2.25f;

constexpr auto STEPSIZE = 16u;

constexpr auto PLAYER_START_X = 1616.0f;
constexpr auto PLAYER_START_Y = 2875.0f;
    
Opening::Opening(ResourceManager& resmgr, sf::RenderTarget& target)
    : Scene(resmgr, target),
      _statusBar(resmgr, target),
      _debugWindow(resmgr, target)
{
    sf::Texture temptext = *(_resources.load<sf::Texture>("maps/tucson.png"));
    _background = std::make_shared<Background>(temptext);
    _background->setScale(SCALE_BACKGROUND, SCALE_BACKGROUND);
    _background->setPosition(0.0f, 0.0f);

    auto top = (_background->texture().getSize().y * 0.7f) - _window.getSize().y;
    sf::View view(sf::FloatRect(0.f, top,
        static_cast<float>(_window.getSize().x), static_cast<float>(_window.getSize().y)));
    _window.setView(view);

    temptext = *(_resources.load<sf::Texture>("textures/tommy.png"));
    _player = std::make_shared<Player>(temptext, sf::Vector2i{ 64, 64 });
    _player->texture().setSmooth(true);
    _player->setSource(0, 2);
    _player->setScale(SCALE_PLAYER, SCALE_PLAYER);
    _player->setOrigin(0.0f, 0.0f);
    _player->setPosition(PLAYER_START_X, PLAYER_START_Y);
    _player->setAnimeCallback([this]() { this->animeCallback(); });

    addUpdateable(_player);

    // the order in which we add everything to the draw'able
    // vector is important, so we do it all at the end of
    // the function
    addDrawable(_background);
    addDrawable(_player);

    _debugWindow.setVisible(false);
}

std::uint16_t Opening::poll(const sf::Event& e)
{
    if (e.type == sf::Event::KeyPressed)
    {
        switch (e.key.code)
        {
            default:
            break;

            case sf::Keyboard::Left:
            {
                if (_player->state() == AnimatedSprite::ANIMATED
                    && _player->direction() == AnimatedSprite::LEFT)
                {
                    return 0;
                }

                _player->setSource(0, 9);
                _player->setMaxFramesPerRow(9);
                _player->setState(AnimatedSprite::ANIMATED);
                _player->setDirection(AnimatedSprite::LEFT);
            }
            break;

            case sf::Keyboard::Right:
            {
                if (_player->state() == AnimatedSprite::ANIMATED
                    && _player->direction() == AnimatedSprite::RIGHT)
                {
                    return 0;
                }

                _player->setSource(0, 11);
                _player->setMaxFramesPerRow(9);
                _player->setState(AnimatedSprite::ANIMATED);
                _player->setDirection(AnimatedSprite::RIGHT);
            }
            break;

            case sf::Keyboard::Up:
            {
                if ((_player->state() == AnimatedSprite::ANIMATED && _player->direction() == AnimatedSprite::UP)
                    || (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left)))
                {
                    return 0;
                }

                _player->setSource(0, 8);
                _player->setMaxFramesPerRow(9);
                _player->setState(AnimatedSprite::ANIMATED);
                _player->setDirection(AnimatedSprite::UP);
            }
            break;

            case sf::Keyboard::Down:
            {
                if ((_player->state() == AnimatedSprite::ANIMATED && _player->direction() == AnimatedSprite::DOWN)
                    || (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left)))
                {
                    return 0;
                }

                _player->setSource(0, 10);
                _player->setMaxFramesPerRow(9);
                _player->setState(AnimatedSprite::ANIMATED);
                _player->setDirection(AnimatedSprite::DOWN);
            }
            break;

            case sf::Keyboard::Home:
            {
                _debugWindow.setVisible(!_debugWindow.visible());
            }
            break;

            case sf::Keyboard::A: // LEFT
            {
                sf::View view = _window.getView();
                auto [x,y] = view.getCenter();
                if ((x - (view.getSize().x / 2)) > 0)
                {
                    x -= 20;
                    view.setCenter(x, y);
                    _window.setView(view);
                }
            }
            break;

            case sf::Keyboard::D: // RIGHT
            {
                sf::View view = _window.getView();
                auto [x,y] = view.getCenter();
                if (((x+20) + (view.getSize().x / 2))
                    < _background->getRightBoundary())
                {
                    x += 20;
                    view.setCenter(x, y);
                    _window.setView(view);
                }
            }
            break;

            case sf::Keyboard::W: // UP
            {
                sf::View view = _window.getView();
                auto [x,y] = view.getCenter();
                if ((y - (view.getSize().y / 2)) > 20)
                {
                    y -= 20;
                    view.setCenter(x, y);
                    _window.setView(view);
                }
            }
            break;

            case sf::Keyboard::S: // DOWN
            {
                sf::View view = _window.getView();
                auto [x,y] = view.getCenter();

                if ((y + (view.getSize().y / 2)) 
                    < _background->getBottomBoundary())
                {
                    y += 20;
                    view.setCenter(x, y);
                    _window.setView(view);
                }
            }
            break;

        }
    }

    return 0;
}

std::uint16_t Opening::timestep()
{
    if (_player->state() == AnimatedSprite::ANIMATED
        && !sf::Keyboard::isKeyPressed(sf::Keyboard::Left)
        && !sf::Keyboard::isKeyPressed(sf::Keyboard::Right)
        && !sf::Keyboard::isKeyPressed(sf::Keyboard::Up)
        && !sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
    {
        _player->setState(AnimatedSprite::STILL);
    }

    auto [tilex, tiley] = getPlayerTile();
    auto posText = fmt::format("LOC:({},{})\n", tilex, tiley);
    _debugWindow.setText(posText);

    Scene::timestep();
    return 0;
}

sf::Vector2u Opening::getPlayerTile() const
{
    auto pc = _player->getGlobalCenter();
    return tt::getTileXY(_player->getPosition(), { 10, 10 });
}

void Opening::draw()
{
    adjustView();
    Scene::draw();
    
    _window.setView(_window.getDefaultView());
    _statusBar.draw();
    _debugWindow.draw();
}

void Opening::adjustView()
{
    auto view = _window.getView();
    auto [xpos,ypos] = _player->getGlobalCenter();

    if (xpos < (_window.getSize().x / 2))
    {
        xpos = view.getCenter().x;
    }
    else if (auto totalWidth = _background->getGlobalBounds().width;
                xpos > (totalWidth - (_window.getSize().x / 2)))
    {
        xpos = totalWidth - (view.getSize().x / 2);
    }

    if (ypos < (_window.getSize().y / 2))
    {
        ypos = view.getCenter().y;
    }
    else if (auto totalHeight = _background->getGlobalBounds().height;
                ypos > (totalHeight - (_window.getSize().y / 2)))
    {
        ypos = totalHeight - (view.getSize().y / 2);
    }

    view.setCenter(xpos, ypos);
    _window.setView(view);
}

void Opening::animeCallback()
{
    const auto stepSize = STEPSIZE 
        + (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)
            || sf::Keyboard::isKeyPressed(sf::Keyboard::RShift) ? 20 : 0);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
    {
        auto xx = _player->getGlobalLeft();
        const auto boundaryLeft = _background->getLeftBoundary();
        auto [x, y] = _player->getPosition();
        assert(xx >= boundaryLeft);
        if (xx == boundaryLeft) return;

        xx -= stepSize;
        if (xx < boundaryLeft) xx = boundaryLeft;
        _player->setGlobalLeft(xx);
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
    {
        const auto boundaryRight = _background->getRightBoundary();

        auto x = _player->getGlobalRight();
        assert(x <= boundaryRight);
        if (x == boundaryRight) return;

        x += stepSize;
        if (x > boundaryRight) x = boundaryRight;
        _player->setGlobalRight(x);
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
    {
        const auto boundaryTop = _background->getTopBoundary();
        auto y = _player->getGlobalTop();
        assert(y >= boundaryTop);
        if (y == boundaryTop) return;

        y -= stepSize;
        if (y < boundaryTop) y = boundaryTop;
        _player->setGlobalTop(y);
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
    {
        const auto boundaryBottom = _background->getBottomBoundary();

        auto y = _player->getGlobalBottom();
        assert(y <= boundaryBottom);
        if (y == boundaryBottom) return;

        y += stepSize;
        if (y > boundaryBottom)
        {
            y = boundaryBottom;
        }
        _player->setGlobalBottom(y);
    }
}

} // namespace tt