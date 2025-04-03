# Cursed Lands Metrics Document

##  Attributes

### Vital:

Vital Attributes are attributes that all characters in the game will have.\
Base values are affected by actions in game.\
Max and Regen values for player are affected by attributes, while for enemies they are hardcoded.

| Name                 | Description | Affected By  | Calculation |
|----------------------|-------------|--------------|-------------|
| Health               | TODO        | None         | NA          |
| Mana                 | TODO        | None         | NA          |
| Stamina              | TODO        | None         | NA          |
| Max Health           | TODO        | Strength     | NA          |
| Max Mana             | TODO        | Intelligence | NA          |
| Max Stamina          | TODO        | Resilience   | NA          |
| Health Regeneration  | TODO        | Resilience   | NA          |
| Mana Regeneration    | TODO        | Intelligence | NA          |
| Stamina Regeneration | TODO        | Resilience   | NA          |

### Primary:

Primary Attributes are attributes that only the Player Character has and are affected by the Player investing in them when leveling up.\
Those Attributes effect the Secondary Attributes.

| Name             | Description | Affected By  | Calculation |
|------------------|-------------|--------------|-------------|
| Strength (S)     | TODO        | None         | NA          |
| Agility (A)      | TODO        | None         | NA          |
| Intelligence (I) | TODO        | None         | NA          |
| Perception (P)   | TODO        | None         | NA          |
| Resilience (R)   | TODO        | None         | NA          |

### Secondary:

Secondary Attributes are attributes that only the Player Character has and are affected by his Primary Attributes.\
Those Attributes effect the Player Character Gameplay during Combat.

| Name                 | Description | Affected By  | Calculation  |
|----------------------|-------------|--------------|--------------|
| Physical Damage      | TODO        | Strength     | NA           |
| Defense              | TODO        | Strength     | NA           |
| Avoidability         | TODO        | Agility      | NA           |
| Action Speed         | TODO        | Agility      | NA           |
| Magical Damage       | TODO        | Intelligence | NA           |
| Critical Hit Chance  | TODO        | Perception   | NA           |
| Maximum Range        | TODO        | Perception   | NA           |
| Critical Hit Damage  | TODO        | TODO         | NA           |

* Defense - Improves armor efficiency
* Avoidability - The chance for an incoming attack to be "Dodged"
* Action Speed - The speed of attack, spells and abilities

##  3Cs

### Sprint

Minimum Walk Speed: 20.0

Run Speed: 500.0

Sprint Speed: 800.0

### Fall Damage

Fall height for minimum fall damage: 800.0

Fall height for maximum fall damage: 1500.0

### Fatigue

Fatigue Duration: 2.0 Seconds