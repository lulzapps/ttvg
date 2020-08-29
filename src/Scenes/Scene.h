#pragma once

#include <lua/lua.hpp>

#include <nlohmann/json.hpp>

#include "../Screen.h"
#include "../Player.h"
#include "../Background.h"
#include "../Item.h"
#include "../ItemFactory.h"

#include "Hud.h"
#include "DescriptionText.h"
#include "DebugWindow.h"

namespace tt
{

struct AvatarInfo
{
    sf::Vector2f    start;
    sf::Vector2f    scale;
    sf::Vector2f    source;
    sf::Vector2f    origin;
    float           stepsize;
};

void from_json(const nl::json& j, AvatarInfo& av);

class Scene;
using ScenePtr = std::unique_ptr<Scene>;
using SceneSharedPtr = std::shared_ptr<Scene>;

class Scene : public Screen
{

public:
    static constexpr auto CLASS_NAME = "Scene";
    static const struct luaL_Reg LuaMethods[];

    Scene(std::string_view name,
        ResourceManager& res,
        sf::RenderTarget& target,
        PlayerPtr player,
        lua_State* luaState);

    std::string name() const { return _name; }

    virtual void init();

    virtual void enter();
    virtual void exit();

    PollResult poll(const sf::Event& e) override;
    void draw() override;

    sf::Vector2f getPlayerTile() const;

protected:
    virtual void updateCurrentTile(const TileInfo& info);
    virtual sf::Vector2f animeCallback();
    virtual void adjustView();

    [[maybe_unused]] bool walkPlayer(float speed);    

    std::string             _name;
    lua_State*              _luaState;
    int                     _luaIdx = 0;

    Hud                     _hud;
    DescriptionText         _descriptionText;
    DebugWindow             _debugWindow;

    BackgroundSharedPtr     _background;

    std::weak_ptr<Player>   _weakPlayer;
    PlayerPtr               _player;
    sf::Vector2f            _lastPlayerPos;
    AvatarInfo              _playerAvatarInfo;
    TileInfo                _currentTile;

    std::vector<ItemPtr>    _items;

private:
    void createItems();
    void loadLuaFile(const std::string& luafile);
};

} // namespace tt