#pragma once

//
// These constants are checked every frame
//

//Amount of energy to restore while not shooting
const float ENERGY_RESTORE_PASSIVE = 1;

// Amount of energy to drain per "shot" (for example, when all cannons start firing)
const float ENERGY_DRAIN_SPINUP = 0.15F;

// Amount of energy to drain per bullet
const float ENERGY_DRAIN_PER_BULLET = 0.1F;

// Amount of energy to drain while moving
const float ENERGY_DRAIN_THRUSTERS = 0.15F;

// Chance for an enemy to shoot a missile
const float ENEMY_SHOOT_CHANCE = 0.02F;

// How often to shoot missiles.  For example, if this number is "2", shoot a missile every other frame.  "3" means every third frame.
const int MISSILE_SPARSITY = 3;

//
// These constants do not apply every frame
//

// Minimum amount of energy the player can have
const float ENERGY_MINIMUM = 2.5F;

//
// Fun stuff/cheats
//

// Moving and shooting doesn't drain energy
#define NODRAIN false