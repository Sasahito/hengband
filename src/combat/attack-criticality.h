﻿#pragma once

#include "combat/combat-options-type.h"
#include "main/sound-definitions-table.h"
#include "system/angband.h"

#include <tuple>

struct player_attack_type;
struct player_type;
std::tuple<HIT_POINT, concptr, sound_type> apply_critical_norm_damage(int k, HIT_POINT base_dam);
HIT_POINT critical_norm(player_type *player_ptr, WEIGHT weight, int plus, HIT_POINT dam, int16_t meichuu, combat_options mode, bool impact = false);
void critical_attack(player_type *player_ptr, player_attack_type *pa_ptr);
