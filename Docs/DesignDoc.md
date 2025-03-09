# Cursed Lands Design Document

## Vision

* A small Action-RPG Open World experience, including demos of all required systems and mechanics.
* A polished (as much as possible) 3Cs and Gameplay experience.
* Usage of Advanced Locomotion and Gameplay Camera System for better 3Cs.
* Usage of GAS (Gameplay Ability System) for the game's Attribute System and Ability System.
* Usage of State Trees for enemy AIs.

## Controls

| Action     | Mouse & Keyboard | Controller                                |
|------------|------------------|-------------------------------------------|
| Look       | Mouse XY 2D-Axis | Gamepad Right Thumbstick 2D-Axis          |
| Move Left  | A                | Gamepad Left Thumbstick X-Axis (Negative) |
| Move Right | D                | Gamepad Left Thumbstick X-Axis (Positive) |
| Move Down  | S                | Gamepad Left Thumbstick Y-Axis (Negative) |
| Move Up    | W                | Gamepad Left Thumbstick Y-Axis (Positive) |

## Features
### Player Character

TODO

### Attribute System

Vital Attributes:
* Health
* Stamina

Primary Attributes (TODO):
* ???

Secondary Attributes:
* ???

### Combat
#### Melee

TODO

#### Ranged

TODO

#### Casting



#### Spell System

Each spell has a type (think of element).\
There are 4 types of elements in the game:
* Acid
* Fire
* Ice
* Lightning

Each of the spell types is accompanied by a debuff that can affect the enemies or the player character:
* Burning (Fire) - A burning character will be receiving damage over time. (TODO: Understand how this differs from Poisoned) 
* Frozen (Ice) - A frozen character has its movement and attack speed decreased for a **medium** duration. 
* Poisoned (Acid) - A poisoned character will be receiving damage over time. (TODO: Understand how this differs from Burning)
* Stunned (Lightning) - A stunned character will not be able to move or act for a **short** duration.

### Inventory

TODO

### Open World

The Open World should include a small town, with NPCs that can provide quests.

### Quest System

TODO
