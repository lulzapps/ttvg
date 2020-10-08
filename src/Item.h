#pragma once
#include <vector>
#include <variant>
#include <optional>

#include <SFML/Graphics.hpp>
#include <nlohmann/json.hpp>

#include "AnimatedSprite.h"

namespace nl = nlohmann;

namespace tt
{

class Item;
using ItemPtr = std::shared_ptr<Item>;

// Item callbacks can be null or non-null and empty. 
// If the callback is null, then it was not defined.
// If it is empty, then this denotes a configuration
// like: `"onSelect": ""` which might be used to 
// override a default action with an empty action
struct ItemCallbacks
{
    // used when the item is picked up from the map
    std::optional<std::string> onSelect;

    // // used when a weapon is yielded or an instrument
    // // is played
    // std::optional<std::string> onUse;

    // // used when somethin is eaten, smoked, etc
    // std::optional<std::string> onConsume;
};

enum class ItemFlags : std::uint16_t
{
    NONE = 0x0000,
    CONSUMABLE = 0x0001,
    WEAPON = 0x0002,
    INSTRUMENT = 0x0004,       // can be used for busking
};


struct ItemInfo
{
    std::string             id;

    // a null x,y means that the coordinate was not specified, 
    // and a value of -1 means it should be picked randomly
    std::optional<float>    x;
    std::optional<float>    y;
    std::optional<float>    respawn;

    ItemCallbacks           callbacks;
};

class Item : public GameObject
{

public:
    static constexpr auto CLASS_NAME = "Item";
    static const struct luaL_Reg LuaMethods[];

    Item(       const std::string&  id,
                const sf::Texture&  texture, 
                const sf::Vector2i& size );

    std::string getID() const;

    std::string getName() const;
    void        setName(const std::string& s);

    std::string getDescription() const;
    void        setDescription(const std::string& s);

    bool    isObtainable() const;
    void    setObtainable(bool b);

    ItemCallbacks   callbacks;

    void setInfo(const ItemInfo& info) { _itemInfo = info; }
    ItemInfo info() const { return _itemInfo; }

private:

    std::string     _id;
    std::string     _name;
    std::string     _description;

    std::uint32_t   _flags = 0;
    bool            _isObtainable = false;

    ItemInfo        _itemInfo;
};

void from_json(const nl::json& j, ItemCallbacks& i);
void from_json(const nl::json& j, ItemInfo& i);

} // namespace tt
