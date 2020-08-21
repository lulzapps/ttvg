#include "Engine.h"
#include "IntroScreen.h"
#include "GameScreen.h"
#include "GameOverScreen.h"

namespace tt
{

TooterEngine::TooterEngine(const boost::filesystem::path& respath, RenderTargetPtr render)
    : _resourceManager{ respath },
      _renderTarget { render }
{
    _currentScreen = std::make_shared<SplashScreen>(_resourceManager, *_renderTarget);
}

void TooterEngine::drawScreen()
{
    if (_currentScreen)
    {
        _currentScreen->draw();
    }
}

void TooterEngine::poll(const sf::Event& e)
{
    if (auto result = _currentScreen->poll(e); 
        result.action.type == ScreenActionType::CHANGE_SCREEN)
    {
        changeScreen(boost::any_cast<std::uint16_t>(result.action.data));
    }
}

void TooterEngine::timestep()
{
    if (auto action = _currentScreen->timestep(); 
        action.type == ScreenActionType::CHANGE_SCREEN)
    {
        changeScreen(boost::any_cast<std::uint16_t>(action.data));
    }
}

void TooterEngine::changeScreen(std::uint16_t id)
{
    switch (id)
    {
        default:
            throw std::runtime_error("invalid screen id");
        break;

        case SCREEN_INTRO:
        {
            _currentScreen->close();
            _currentScreen.reset();
            _currentScreen = std::make_shared<IntroScreen>(_resourceManager, *_renderTarget);
        }
        break;

        case SCREEN_GAME:
        {
            _currentScreen->close();
            _currentScreen.reset();
            _currentScreen = std::make_shared<GameScreen>(_resourceManager, *_renderTarget);
        }
        break;

        case SCREEN_GAMEOVER:
        {
            _currentScreen->close();
            _currentScreen.reset();
            _currentScreen = std::make_shared<GameOverScreen>(_resourceManager, *_renderTarget);
        }
        break;
    }
}

} // namespace tt