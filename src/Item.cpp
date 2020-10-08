#include <iostream>

#include <lua/lua.hpp>

#include "Item.h"
#include "TTLua.h"

namespace tt
{

void from_json(const nl::json& j, ItemCallbacks& i)
{
    if (j.contains("onSelect")) 
    {
        i.onSelect = j["onSelect"].get<std::string>();
    }

    // if (j.contains("onDrop")) 
    // {
    //     j.at("onDrop").get_to(i.onDrop);
    // }

    // if (j.contains("onUse")) 
    // {
    //     j.at("onUse").get_to(i.onUse);
    // }

    // if (j.contains("onConsume")) 
    // {
    //     j.at("onConsume").get_to(i.onConsume);
    // }
}

void from_json(const nl::json& j, ItemInfo& info)
{
    if (j.contains("x"))
    {
        if (j["x"].is_number())
        {
            info.x = j["x"].get<float>();
        }
        else if (j["x"].is_string()
            && j["x"].get<std::string>() == "random")
        {
            info.x = -1.f;
        }
        else
        {
            throw std::runtime_error("invalid item coordinate");
        }
    }

    if (j.contains("y"))
    {
        if (j["y"].is_number())
        {
            info.y = j["y"].get<float>();
        }
        else if (j["y"].is_string()
            && j["y"].get<std::string>() == "random")
        {
            info.y = -1.f;
        }
        else
        {
            throw std::runtime_error("invalid item coordinate");
        }
    }

    if (j.contains("respawn-delay"))
    {
        info.respawn = j["respawn-delay"].get<float>();
    }

    info.callbacks = j.get<ItemCallbacks>();
}

namespace
{

int Item_getId(lua_State* L)
{
    auto item = tt::checkObject<Item>(L);
    lua_pushstring(L, item->getID().c_str());
    return 1;
}

int Item_getName(lua_State* L)
{
    auto item = tt::checkObject<Item>(L);
    lua_pushstring(L, item->getName().c_str());
    return 1;
}

int Item_setName(lua_State* L)
{
    auto item = tt::checkObject<Item>(L);
    const auto name = lua_tostring(L, 2);
    item->setName(name);
    return 0;
}

int Item_getDescription(lua_State* L)
{
    auto item = tt::checkObject<Item>(L);
    lua_pushstring(L, item->getDescription().c_str());
    return 1;
}

int Item_setDescription(lua_State* L)
{
    auto item = tt::checkObject<Item>(L);
    const auto name = lua_tostring(L, 2);
    item->setDescription(name);
    return 0;
}

int Item_isObtainable(lua_State* L)
{
    auto item = tt::checkObject<Item>(L);
    lua_pushboolean(L, item->isObtainable() ? 1 : 0);
    return 1;
}

int Item_setObtainable(lua_State* L)
{
    auto item = tt::checkObject<Item>(L);
    const auto val = lua_toboolean(L, 2);
    item->setObtainable(val);
    return 0;
}

}

const struct luaL_Reg Item::LuaMethods[] =
{
    {"id", Item_getId},
    {"name", Item_getName},
    {"setName", Item_setName},
    {"description", Item_getDescription},
    {"setDescription", Item_setDescription},
    {"obtainable", Item_isObtainable},
    {"setObtainable", Item_setObtainable},
    {nullptr, nullptr}
};

Item::Item( const std::string&  id,
            const sf::Texture& texture,
            const sf::Vector2i& size )
    : GameObject(texture, size),
        _id { id }
{
}

std::string Item::getID() const
{
    return _id;
}

void Item::setName(const std::string& s)
{
    _name = s;
}

std::string Item::getName() const
{
    return _name;
}

void Item::setDescription(const std::string& s)
{
    _description = s;
}

std::string Item::getDescription() const
{
    return _description;
}

void Item::setObtainable(bool b)
{
    _isObtainable = b;
}

bool Item::isObtainable() const
{
    return _isObtainable;
}
 
}
