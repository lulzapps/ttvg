# 0. Introduction

**This feature is a work in progress**. 

Lua support makes it easier for the community to add content to the game. This document talks about the Lua API and gives examples of how to extend the game using Lua.

Every scene has a *&lt;scene-name&gt;.lua* file which resides in `resources/lua` folder. This Lua file contains all code that is used by the scene and the scene's components. 

# 1. Scenes Events

Scenes will emit events that can be caught in Lua. The default names and will be called automatically when the event is triggered. However, the names can be overriden using the JSON configuration like so:

```json
{
    "onInit": "scene_onInit",
    "onEnter": "scene_onEnter",
    "onExit": "scene_onExit",
    "onTileupdate": "scene_onTileUpdate"
}
```

Each event is passed a `Scene` object as the first parameter (see `Scene API` below).

A *Hello World* example using the configuration above might look like:

```lua
function scene_onInit(scene)
    print('Hello from '..scene:name..'!')
end

function scene_onEnter(scene)
    print('Entering scene '..scene:name..'!')
end

function scene_onExit(scene)
    print('Now leaving scene '..scene:name..'!')
end

function scene_onTileUpdate(scene, x, y)
    print('You are currently on tile ('..x..','..y..')')
end
```
</s>

## 1.1. Event Details

There are three callback functions for every scene.
<hr/>

### `[void] onInit(scene)`

This function is invoked when the scene is first loaded. This should not be confused with when the scene is entered, which can happen multiple times during the game. `onInit` is called only once when the game starts. Note that if game ends and the user starts a new game, `onInit` **will** be called at the start of that new game.

<hr/>

### `[void] onEnter(scene)`

This function is invoked when the player first enters the scene. This callback can be called multiple times in a single game since a user can enter and leave scenes multiple times.

<hr/>

### `[void] onExit(scene)`

This function is invoked when the playe first exits the scene. This callback can be also be called multiple times.

<hr/>

### `[void] onTileUpdate(scene, x, y)`

This is called everytime the user walks onto a new tile. The first argument is the current `scene`, and the `x` and `y` arguments are the coordinates of the new tile.


<br/>

## 1.2. Scene API

### `[void] Scene.addItem(item)`

The argument `item` is an `Item` object. This function adds an existing `Item` object to the current scene, hence the object must already exist when this function is called. 

For example:

```lua
function onEnter(scene)
    local item = ItemFactor.createItem("sax")
    scene:addItem(item)
end
```
<hr/>

### `[DescriptionWindow] Scene.createModal(ModalType)`

Creates a new modal window of type `ModalType` for display.

See Modal Window for more details.

<hr/>

### `[DescriptionWindow] Scene.getDescriptionWindow()`

Returns a handle to the scene's description window. See "Description Window".

<hr/>

### `[Player] Scene.getPlayer()`

No arguments. Returns a `Player` object. **This object is only valid for the life of the scene.** 

**Example**
```lua
function onEnter(scene)
    local player = scene:getPlayer()
    local health = player:getHealth() - 2
    player:setHealth(health)
end
```

<hr/>

### `[number,number] Scene.getPlayerTile()`

No arguments. Returns the (x,y) coordinates of the player's current tile. 


**Example**
```lua
function onEnter(scene)
    local x,y = scene:getPlayerTile()
    print ("The player is at position "..x..','..y)
end
```

<hr/>

### `[string] Scene.name()`

No arguments. Returns the name of the current scene.

<hr/>


### `[void] Scene.removeItem(item)`

The argument `item` is an `Item` object. If the scene does not contain the passed in item then nothing happens.

<hr/>

### `[void] Scene.setPlayerTile(x,y)`

Sets the player's current tile to (`x`,`y`). There are no bounds checking done on coordinates passed in.

<br/>

# 2. Player

The player object is accessible through `Scene.getPlayer`. **The player object is only valid for the life of the current scene.** 

The lifetime of the player object only lasts for the lifetime of the current scene. This means that if the player object is cached, it should be cached with each call to `onEnter`. For example, this will cause a crash:

```lua
gPlayer = nil

function onEnter(scene)
    if gPlayer == nil then
        gPlayer = scene:getPlayer()
    end
end

function someOtherFunction()
    print("Health is "..gPlayer:getHealth()) -- CRASH! (when scene is entered a second time)
end
```

The first time the scene is entered this will run. However when the player leaves the scene, the data to which `gPlayer` points is invalidated but the underlying value of `gPlayer` is still non-nil. Hence when the user re-enteres the scene and `someOtherFunction` is invoked, the methos `Player.getHealth()` will be called on corrupted data.

The correct way to do this would be to just set it every time:

```lua
gPlayer = nil

function onEnter(scene)
    gPlayer = scene:getPlayer()
end

function someOtherFunction()
    print("Health is "..gPlayer:getHealth()) -- OK
end
```

This also means that the player object is **not** available during initialization (i.e. `onInit`). 

```lua
function onInit(scene)
    local player = scene:getPlayer()
    print("Health is "..gPlayer:getHealth()) -- CRASH!
end
```

## 2.1 Player API

<hr/>

### `[void] Player.addItem(item)`

Add the `item` object to `Player`'s inventory. `item` is an object and not a string value. 

Example:
```lua
-- give the player 'special-coin' when entering the scene
function scene_onEnter(scene)
    local item = ItemFactory.createItem('special-coin')
    player:addItem(item)
end
```

See more about "Items".
</span>

<hr/>

### `[float] Player.getBalance()`
No arguments. Returns the value of the player's current balance.

<hr/>

### `[float] Player.getBalance()`

Returns a float value of the player's current balance.

<hr/>

### `[integer] Player.getHealth()`

Returns an intenger value of theplayer's current health.

<hr/>

### `[float,float] Player.getPosition()`

Returns the (x,y) corrdinates of the player's position **in terms of the display coordinates**. 

See `Scene.getPlayerTile()`

<hr/>

### `[bool] Player.hasItem(item)`

The argument `item` is an `Item` object (i.e. the type returned from `ItemFactory.createItem()`). Behind the scenes, `item` holds a pointer to the allocated item and does a raw pointer comparison with all the items contained in player inventory.

<hr/>

### `[bool] Player.hasItemByName(itemid)`

The argument `itemid` is the **ID** of the item, which in most cases corresponds to the JSON filename of the item.

<hr/>

### `[void] Player.removeItem(item)`

The argument `item` is a pointer to an `Item` object, just like the argument for `Player.hasItem`. 

<hr/>

### `[bool] Player.removeItemByName(itemid)`

The argument `itemid` is the **ID** of the item, which in most cases corresponds to the JSON filename of the item.

<hr/>

### `[void] Player.setBalance(balance)`

Sets the balance of the player to `balance`. This should be a numeric value.

<hr/>

### `[void] Player.setHealth(health)`
Sets the health of the player to `health`. This should be an **integer** value.

### `[void] Player.setPosition(x,y)`

Sets the player's display position to (`x`,`y`). There are no bounds checking done on coordinates passed in.

See `Scene.getPlayerTile()`

<br/>

# 3. Items

## 3.1 Item Events

Just like scenes, items have cofigurable events. The events are specified in the JSON file where each individual item is configured. 

For example:

```json
"treasure-chest":
[
    { 
        "x": 37, "y": 31, "onSelect": "treasureChest_onSelect" 
    }
],
```

These are the item events:

<hr/>

### `[bool] onSelect(scene, item)`

`scene` is the current scene, and `item` is the item object being picked up. Returns `true` if the player was successful in picking up the item, returns `false` if the player was unsuccessful and the item is to remain in the scene. **The return value overrides the `isObtainable` attribute of the item.**

For example:

```lua
function treasureChest_onSelect(scene, item)
    if _player:hasItemByName("key") then
        local vagina = ItemFactory.createItem("magic-space-vagina")
        _player:addItem(vagina)
        _player:removeItemByName("key")
        Utils.showModal("You opened the chest!\nNow you have a Magic Space Vagina!")
        return true
    else
        Utils.showModal("You cannot open this right now")
    end
    return false
end
```

Here we check the player's inventory to see if they have a `key` to the `treasure-chest`. If so, then we remove the `key` manually and give the player a `magic-space-vagina`, and we also return `true` so that the `treasure-chest` is removed from the scene. If the player does not have the required `key` then we return `false` so that the `treasure-chest` is not removed.

<br/>

## 3.2 ItemFactory

<hr/>

#### `[Item] ItemFactory.createItem(itemid)`

Parameter `itemid` is the item's unique key as determined by the filename. This function returns an `Item` object.

<br/>

## 3.3 Item

<hr/>

### `[string] Item.description()`

Returns the description of the item.

<hr/>

### `[string] Item.id()`

Returns the id of the item.

<hr/>

### `[string] Item.name()`

Returns the name of the item.

<hr/>

### `[bool] Item.obtainable()`

<hr/>

### `[void] Item.setDescription(text)`

Sets the description of the item to `text`. Can be empty.

<hr/>

### `[void] Item.setName(name)`

Sets the name of the item to `name`. Can be empty.

<hr/>

### `[bool] Item.setObtainable()`

<br/>

# 4. Description Window

The description window is the text shown in the center of the HUD. It is safe to cache this in a variable for conviennce. For example the following is safe:

```lua
local descw = nil

function onInit(scene)
    descw = scene:getDescriptionWindow()
end

function someFunction()
    descw:setText("Invoking some function!")
end
```

<hr/>

### `[string] DescriptionText.getText()`

Returns the current test of description window.

<hr/>

### `[void] ]DescriptionText.setText(text)`

Sets the text of the description window to `text`.


<br/>

# 5. Zones

Zones are configured in a scene's JSON file.

<br/>

## 5.1 Zone Events

Zone events are defined in the zone's JSON configuration like so:

For example:

```json
"zones":
[
    { 
        "name" : "Home",
        "description": "Press SPACE to enter your filthy apartment",
        "rects" :
        [ 
            "48,77,52,80", 
        ],
        "transition":
        {
            "scene": "EuclidHouse",
        },
        "onSelect" : "home_onSelect"
    }
]
```

The zone events are:

<hr/>

### `[bool] onSelect(scene, zone)`

This is event is triggered when the user presses SPACE while on a zone (see 'Zones' and 'Transitions' in scenes-dev.md). 

The event is passed a handle to the current `scene`, and a handle to the `zone` itself. 

This callback must return a boolean where **true** indicates that the transition should continue (i.e. that the new scene should be loaded) or **false** to cancel the transition. If there is no transition defined for the zone then the return value is ignored.

The following example implements a "cover charge" for entering a scene:

```lua
function club_onSelect(scene, zone)
    local player = scene:getPlayer()
    local balance = player:getBalance()
    if balance > 20 then
        player:setBalance(balance - 20)
        return true
    end

    scene:getDescriptionWindow():setText("Entry costs $20")
    return false
end
```

<br/>

## 5.1 Zone API

<hr/>

### `[string] Zone.getName()`

Returns the name of the zone.


<br/>

# 6. Modal Windows

Modal windows stop all processing in the game to display text and get input from the user. Modal windows can be used to simulate dialog from NPCs or ask the user questions.

<br/>

## 6.1 Creating Modal Windows

Modal windows are created through the `Scene.createModal()` method. There are currently four different types of modal windows.

Example:
```lua
function onEnter(scene)
    local player = scene:getPlayer()
    if player:getHealth() < 30 then
        local win = scene:createModal(ModalType.Default)
        win:setText("This is a dangerous map. Be careful")
        win:exec()
    end
end
```

In this example we displayng a warning to the player when they enter a level if their health is too low.

The four types of modal windows are:

* `ModalType.Default`
* `ModalType.Messages`
* `ModalType.Options`
* `ModalType.Inventory`

*Note*: There are also some helper functions in the `Utils` namespace to make displaying modal windows more programmer-friendly.

<br/>

## 6.2 Default Modal Windows

 This modal window is a standard window that displays a message that can be dismissed by pressing Space or Escape. 

 The API for Default Modal Windows is available for all types of modal windows. 

### `ModalWindow.exec()`

This function displays the window. Without calling this function the modal window will do nothing. **This function is blocking**.

<hr/>

### `ModalWindow.setAlignment(ModalAlignment)`

There are three ways the window can be vertically aligned:

* `ModalAlignment.Bottom`
* `ModalAlignment.Center`
* `ModalAlignment.Top`

The default is `ModalAlignment.Bottom`.

<hr/>

### `ModalWindow.setHeight(height)`

Sets the height of the window to `height` while preserving the vertical alignment.

<hr/>

### `ModalWindow.setText(text)`

Sets the text to the window to `text`. There is no word-wrap functionality, but `text` can be broken into lines using `\n`.

<hr/>

### `ModalWindow.setWidth(width)`

Sets the width of the window to `width`.

<br/>

## 6.3 Messages Modal Windows

The *Messages Modal Window* allows you to display multiple windows of text within the same modal window instances. The user moves onto the next message by pressing *Space, but can also quit out early by pressing *Escape*.

### `ModalWindow.pushMessage(message)`

Pushes a message into the queue of messages to be displayed. The messages will be shown in the order they're pushed. The player can go to the next messages by pressing the Space bar. Pressing the Escape key will close the window.

Example:
```lua
local w = scene:createModal(ModalType.Messages)
w:pushMessage("This will be shown first")
w:pushMessage("This will be displayed second")
w:pushMessage("This will be printed last")
w:exec()
```
<br/>

## 6.4 Options Modal Windows

The *Options Modal Window* displays a list of choices to the user. The user can scroll through the options using the arrow keys, and an option can be selected by pressing Space or Enter. If the user closes the window using Escape then the selection is null.

Here is a full example:

```lua
function home_onSelect(scene, zone)
    local w = scene:createModal(ModalType.Options)
    w:setText("There is a $35 cover charge.\nDo you want to enter?")
    w:addOption("Yes")
    w:addOption("No")
    w:exec()
    local r = w:getSelection()
    if r == nil or r == 1 then
        return false
    end

    local player = scene:getPlayer()
    local balance = player:getBalance()
    player:setBalance(balance - 35)
    return true
end
```

<hr/>

### `[void] OptionsWindow.addOption(text)`

Adds the option text. 

<hr/>

### `[selection] OptionsWindow.getSelection()`

Returns the 0-based index of the selected option. This can also be `nil` which means the user dismissed the box with the Escape key (i.e. no selection was made)

<br/>

## 6.5 Inventory Modal Windows

The *Inventory Modal Window* shows the player's inventory. This window uses the same API of the *Options Modal Window* to get the index of the selection.


<br/>

# 6. Logging

Lua's logging mechanism hooks into the same logging mechanism as the C++ code, and therefore will use the same settings, logfile, etc.

The functions available are:

* `[void] Log.trace(text)`
* `[void] Log.debug(text)`
* `[void] Log.info(text)`
* `[void] Log.warn(text)`
* `[void] Log.error(text)`
* `[void] Log.critical(text)`

Example:
```lua
function scene_onInit(scene)
    Log.info("The scene "..scene:name().." has been initialized")
end
```

<br/>

# 6. Utilities

The `Utils` namespace offers a group of helper functions.

<br/>

### `[void] Utils.openUrl(url)`

Opens the system's default browser to the passed in `url`.

<hr/>

### `[void] Utils.showModal(scene, text)`

Shows a single message in a modal window. The `scene` argument is the current scene.

For example:

```lua
function scene_onInit(scene)
    Utils.showModal(scene, "Welcome to the game!")
end
```

is functionally equivalent to:

```lua
function scene_onInit(scene)
    local window = scene:createModal(ModalType.Default)
    window:setText("Welcome to the game!")
    window.exec()
end
```

<hr/>

### `[bool] Utils.showYesNo(scene, text)`

Shows a message in a modal window and gives the user a "Yes" or no "Option".

Example:

```lua
local res = Utils.showYesNo(scene, "Would you like to continue?")
if res then
    -- code to continue
else
    -- code not to continue
end
```
