# Road Map

This document discusses long term plans for the game. 

These items are in a **rough order** in which I would like to tackle them. 

**All plans are tentative and subject to change.**

<hr/>

## Combat System

The combat system will introduce NPCs and weapons into the game giving players a way to win money from fighting NPCs or possibly die. This feature will be implemented in the following stages:

### Configurable Animation

Currently only the `Player` and `Vehicle` objects supported animation in a convoluted manner. This functionality should be refactored to allow for configurable animated states of all kinds of objects.

An example configuration for the player object might look like:

```json
{
    "texture": "textures/tommy.png",
    "size": { "x": 64, "y": 64 },
    "states":
    [
        { "id": "walking-up", "x": 0, "y": 0, "count": 9 },
        { "id": "walking-left", "x": 0, "y": 1, "count": 9  },
        { "id": "walking-down", "x": 0, "y": 2, "count": 9  },
        { "id": "walking-right", "x": 0, "y": 3, "count": 9  }
    ]
}
```

### Configurable Bounds Checking

Collision bounds need to be seperated from sprite bounds in a sprite sheet. These bounds will be configurable with an offset relative to the state's rectangle, and with a rectangle representing the size of the bounds rectangle.

### Stationary NPCs

NPCs will be animated but in a stationary tile. Players will be able to interact with them and their states will change accordingly.

### Roaming NPCs

NPCs will roam the map much like vehicles currently do. They will also be able to chase the player or run from the player. 

### Combative NPCs

NPCs will have combative state. They will be capable of fighting the player or each other. This will also introduce weapons into the game.

<hr/>

## Audio System Refactor

The system to play audio needs to be abstracted away from the SFML implementation. This will allow for things like log decoration, and a null audio system.

<hr>

## Auto-Detect Collision Bounds

Logic should be added that scans the four directions of an image for the extent of its bounds. This would make it easier to add textures and give more precise collision detection.

<hr/>

## Mission API

Need to a define a mission API.

<hr/>


## Integrate GUI Library

Explore GUI/Widget libraries that support SFLM. This will be useful for things like a Settings page where widgets (e.g. checkboxes, dropdowns) would be useful.

<hr/>

## Migrate to Github Actions

The Github action system supports CI testing on Windows, macOS and Ubuntu. We should migrate away from Travis-CI and AppVeyor and use the Github Action system.

<hr/>

## AMB Badges lol

Acomplishments in the game could be rewarded with badges on AMB. This would require some kind of login in the game.

<hr/>

## Multiplayer! LOLLOL

Wouldn't it be great if multiple AMBers could wander around the same map and have hobofights?!

<hr/>

## MicroTransactions! LOLLLOLulz!

Wouldn't it also be great to send Max Power money?!
