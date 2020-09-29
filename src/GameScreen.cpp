#include <lua/lua.hpp>

#include "Scenes/Scene.h"
#include "Scenes/Tucson.h"

#include "GameScreen.h"

namespace tt
{

GameScreen* GameScreen::l_get(lua_State * L)
{
    lua_rawgeti(L, LUA_REGISTRYINDEX, GAMESCREEN_LUA_IDX);
    int type = lua_type(L, -1);
    if (type != LUA_TLIGHTUSERDATA)
    {
        return nullptr;
    }

    GameScreen* state = static_cast<GameScreen*>(lua_touserdata(L, -1));
    lua_pop(L, 1); // -1
    if (!state || state->_luaState != L)
    {
        return nullptr;
    }
    return state;
}

GameScreen::GameScreen(ResourceManager& resmgr, sf::RenderTarget& target)
    : Screen(resmgr, target),
      _itemFactory{std::make_shared<ItemFactory>(resmgr)}
{
    _luaState = luaL_newstate();
        initLua(_luaState, *this, static_cast<void*>(_itemFactory.get()));

    // the `Player` object is shared among all the `Scene` objects
    auto textptr = _resources.cacheTexture("textures/tommy.png");
    assert(textptr);
    textptr->setSmooth(true);
    _player = std::make_shared<Player>(*textptr, sf::Vector2i{ 64, 64 });

    SceneSetup setup{ _resources, _window, _player, _luaState, _itemFactory };

    // TODO: as the game grows these constructions will take longer
    // and should probably be done in parallel and/or with a loading screen
    _scenes.emplace(Tucson::SCENE_NAME, std::make_shared<Tucson>(setup));
    _scenes.emplace("EuclidHouse", std::make_shared<Scene>("EuclidHouse", setup));
    _scenes.emplace("Hospital", std::make_shared<Scene>("Hospital", setup));
    _scenes.emplace("CourthouseInterior", std::make_shared<Scene>("CourthouseInterior", setup));
    _scenes.emplace("PoliceStationInterior", std::make_shared<Scene>("PoliceStationInterior", setup));
    _scenes.emplace("FireStationInterior", std::make_shared<Scene>("FireStationInterior", setup));

    //
    // Would be so nice if lines didn't wrap in source code.
    // I think 80 characters is a fair line size.
    //
    // I used to get into this debate with a developer at my old job, and I'll ask you what I asked him: who still uses an editor that's only 80 characters wide? What is this, 1992? 80x254lyfe?!
    //
    _scenes.emplace("DeathCampInterior", 
                    std::make_shared<Scene>("DeathCampInterior", setup));

    _currentScene = _scenes["Tucson"];

    // make sure all constructors across all scenes
    // have been run BEFORE init()'ing the scenes
    for (auto& item : _scenes)
    {
        item.second->init();
    }

    _currentScene->enter();
}

GameScreen::~GameScreen()
{
    if (!_luaState) return;

    // collect all garbage
    lua_gc(_luaState, LUA_GCCOLLECT, 0);

    // destroy the lua state
    lua_close(_luaState);
}

void GameScreen::draw()
{
    _currentScene->draw();
}

PollResult GameScreen::poll(const sf::Event& e)
{
    assert(_currentScene);
    auto result = _currentScene->poll(e);

    switch (result.action.type)
    {
        default:
        break;

        case ScreenActionType::CHANGE_SCREEN:
        break;

        case ScreenActionType::CHANGE_SCENE:
        {
            const auto name = boost::any_cast<std::string>(result.action.data);
            assert(_scenes.find(name) != _scenes.end());

            _currentScene->exit();
            _currentScene = _scenes.at(name);
            _currentScene->enter();
        }
        break;
    }

    return result;
}

ScreenAction GameScreen::timestep()
{
    assert(_currentScene);
    return _currentScene->update(_gameClock.getElapsedTime());
}

} // namespace tt
