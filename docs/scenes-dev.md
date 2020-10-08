 # Scenes

 ## Intro

The game is divided into multiple *screens* that represent different stages in the interaction of the program. For example there is the splash screen, a menu screen and, of course, the game screen. All screens are derived from the `Screen` class, and the `TooterEngine` class is reponsible for loading and unloading screens. Screens are loaded on an as-needed basis.

Game play itself is further divided into mutiple *scenes*. Scenes represent different maps or areas within the game. The `GameScreen` class contains all of the scene objects and is responsible for controlling transitions between scenes. All scenes are loaded when control of the game is given to the `GameScreen` class.

## Scenes

Scenes are either a class derived from the `Scene` class or an instantiated `Screen` object. The default behavior of `Scene` supports:

* player movement
* HUD display
* zone notifications and transitions
* item pickup and inventory

The `GameScreen` creates all of the necessary scene objects in its constructor. Loading all the scenes when the game starts can be time consuming, but doing so allows for smoother transition between scenes during gameplay. 

## Adding a New Scene

Assume we wish to add a new scene that is the interior of a police station that we will aptly call "PoliceStation". A breakdown of the process to add this scene is as follows:

* Create a settings file for the scene in the *resources/maps* folder (i.e. *resource/maps/PoliceStation.json*). 
* Configure the scene as needed (see **Scene Configuration** below).
* Create a PNG background image for the scene (i.e. *resources/maps/PoliceStation.png*)
* Add the scene to `GameScreen`'s constructor like so:

```c++
_scenes.emplace("Tucson", std::make_shared<Tucson>(resmgr, target, _player));
_scenes.emplace("EuclidHouse", std::make_shared<Scene>("EuclidHouse", resmgr, target, _player));
_scenes.emplace("PoliceStation", std::make_shared<Scene>("PoliceStation", resmgr, target, _player)); // new scene!
```
* Create a transition point from an existing (source) scene to the new scene. For example, we would add a transition in the *tucson.json* file in order to allow the player to enter the police station. See **Transitions** below.
* Create a transition from the new scene to the source. For example, we want to the player to be able to exit the police station and return to the map of Tucson.

After these steps are completed, the new scene should be navigable within the game. 

## Scene Configuration

When a `Scene` object is constructed, a corresponding JSON configuration file is loaded. The configuration filename is scene's name with the ".json" extension in the `/resources/maps` folder. For example, the "PoliceStation" scene in our example above has a configuation file of `resources/maps/PoliceStation.json`. 

The configuration file defines several things including:

* background image scaling, and tilesize
* player object attributes for the screen 
* zone information

### Background Image

The background image used for a scene is defined by the scene's name. All background images must have the ".png" extension and exist in the "resources/maps" folder. For example, our "PoliceStation" scene has a background image named `resources/maps/PoliceStation.png`.

The background for each scene can have a custom scaling. It is also with the background settings that the tilesize for the scene is defined. (Note: even though custom scaling is supported, 16x16 is used nearly exclusively.)

Example:

```json
"background":
{
    "position": { "x": 0, "y": 0 },
    "scale": { "x": 2.25, "y": 2.25 },
    "tiles": { "x": 16, "y": 16 },
    "camera": "fixed"
}
```

* `position` (optional) - This defines where the upper left corner of the background image should be in terms of global coordinates. Default is `(0, 0)`.
* `scale` (optional) - Defines the scaling for the background image. Default is `(1,1)`.
* `tiles` - The tilesize for the map. For compatibility purposes, this should match up with the tilesize defined by in the PNG's corresponding TMX file.
* `camera` (optional) - Defines the behavior of the viewport. Default is `fixed`.
    * `fixed` - The viewport will never move. This means that if the image is bigger than the screen then the player can walk off the edge of the screen. Usually "fixed" should be used with a `scale` or `auto`.
    * `follow` - The viewport will follow the player as the avatar moves about the screen if the background is bigger than the window. 

### Player Information

The same player object is shared among all scenes, however each scene can customize how the player is drawn. When a scene is entered (not loaded), the scene's `Scene::enter()` method is called (likewise there is a corresponding `Scene::exit()` method). This allows the scene to change any attributes about the player object. 

A consequence of each scene having its own settings for the player means that **every scene must define these settings**, else settings might propagate across scenes which might result in things looking weird.

```json
"player":
{
    "start": { "x": 1660, "y": 2855 },
    "scale": { "x": 1.5, "y": 1.5 },
    "source": { "x": 0, "y": 10 },
    "origin": { "x": 0, "y": 0 },
    "stepsize" : 16.0
}
```

* `start` - This defines the starting position of the player **the first time the player enteres the scene**. When a player leaves a scene, the exact coordinates of their leaving position are stored and then restored in subsequent visits to the scene. These coordinates are defined in terms of global coordinates. 
* `scale` - The scaling of the player image. Larger maps may want to make the player avatar appear smaller (e.g. the `tucson` scene), whereas smaller maps might want to make the player avatar larger (e.g. the `EuclidHouse` scene). This setting can also be the word "auto" which means the image will scale automatically to the size of the game's window.
* `source` - These coordinates define the starting sprite image of the player when entering the scene. These coordinates are in terms of row and column of the player's sprite sheet (`resources/images/tommy-1.png`). 
* `origin` - **Should always be (0,0) for now** 
* `stepsize` - This defines how "fast" the player walks and is defined in terms of "pixels per game loop". 

### Zones

The zones define rectangles that when entered can trigger certain actions. For example, when a player enters a zone the "name" and "description" are shown in the HUD.

```json
{ 
    "name" : "Iron Horse Park",
    "rects" :
    [
        "0,76,13,105",
        "14,97,43,105"
    ]
}
```

* `name` - This is the name of the zone that is displayed in the user's HUD.
* `description` - This text is displayed just below the user's HUD. This is a good place to give the user instructions (e.g. "Press SPACE to enter")
* `rects` - This is an array of strings that are parsed into rectangles. Unlike SFML rects which are defined as `(top, left, width, height)`, these rectangles are defined as `(x1, y1, x2, y2)`. Configuration can define one or more rectangles within this array.

## Transitions

Transitions allow users to navigate from one scene to another by pressing the SPACE bar. Transitions are defined as part of a zone. 

```json
{ 
    "name" : "Home",
    "description": "Press SPACE to enter your filthy apartment",
    "rects" :
    [ 
        "48,77,52,80", 
        "53,77,54,79"
    ],
    "transition":
    {
        "scene": "EuclidHouse",
        "enabled": true
    }
}
```
* `scene` - The name of the scene to be entered.
* `enabled` - **Currently not supported**

## Items

Items can be placed anywhere on a scene/map using the JSON configuration. For example, say we want to add a pair of handcuffs inside our police station that the player can pickup. To add the "handcuffs" we:

* Add the item .png file to *resources/items* (i.e. *resources/items/handcuffs.png*)
* Create a configuration item for the item (i.e. *resources/items/handcuff.json*). 
* Add the item to the PoliceStation's configuration

```json
"items":
{
    "handcuffs":
    {
        "instances":
        [    
            { "x": 5, "y": 5 }
            { "x": 24, "y": 32 }
        ]
    }
}
```

The item name should be used as the JSON key for the item (in this case "handcuffs"). For each pair of coordinates in the array, and item will be placed at that location. The coordinates are defined in terms of tile coordinates.

### Item Defaults

Items can have default settings that apply to each instance of the item in the scene. The default settings can also be overridden by a specifc instance. For example:

```json
"items":
{
    "handcuffs": 
    {
        "onSelect": "handcuff_pickup",
        "instances":
        [    
            { "x": "random", "y": "random" },
            { "x": "random", "y": "random" },
            { "x": "random", "y": "random" },
            { "x": 24, "y": 32, "onSelect": "special_handcuffs" }
        ]
    }
}
```

In the above example, each instance of `handcuffs` will call `handcuff_pickup` in the Lua system when the item is picked up by the player, except for the last instance which will instead call `special_handcuffs`. A specific instance can also override the callback to a null action by doing `"onSelect": ""`.

### Randomizing Item Location

Items can be randomly placed on a map by using the `random` keyword in the configuration. For example:

```json
"items":
{
    "handcuffs": 
    {
        "instances":
        [    
            { "x": "random", "y": "random" }
            { "x": 24, "y": 32 }
        ]
    }
}
```

The `random` keyword can be used on both the `x` and `y` coordinates, or just one.

### Respawning Items

Items can be set to respawn a specific amount of time after the player has picked it up.

```json
"items":
{
    "handcuffs": 
    {
        "respawn-delay": 60,
        "instances":
        [    
            { "x": "random", "y": "random" }
        ]
    }
}
```

Here we have configured the set of handcuffs to respawn 60 seconds after the user has picked it up. Note that a random location for the handcuffs will be chosen every time the item spawns.

## Background Music

Background music can be specified in a scene using the `background-music` property in the scene's JSON configuration. For example:

```json
{
    "background":
    {
        "scale": "auto",
        "tiles": { "x": 16, "y": 16 },
        "camera": "fixed" 
    },
    "background-music":
    {
        "file": "music/EuclidHouse.wav",
        "volume": 80
    }
}
```

`file` is the path the song's .wav file which is played on loop. `volume` is a value 0-100 where 100 is full volume and is the default.

## Subclassing `Scene`

There are some things that must be considered when subclassing from `Scene` to create a new scene.

* Derived scenes that have their own items will have to implement their own `draw() method **that does not call the base class**. This means that each subclass needs to make sure to draw the HUD, modal windows, etc.

## TODO

These are some remaining tasks relating to scenes and their default behavior.

* Give zones definable `id` attributes. This will allow for code to do things such as enable and disable transitions, or change the name and/or description based on certain conditions.

* The `source` field in the player configuration should be supported, but first there needs to be consistency to how it's used. This field allows you to define how the object is referenced in the world. For example, `player->setPosition(1,1)` will set the position of the center of the player avatar at `(1,1)`, **however** if you do:

```
player->setSource(0,0);
player->setPosition(1,1);
```

That will then set the upper left corner of the player avatar at `(0,0)`. 

