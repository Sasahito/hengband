﻿#pragma once

#include "system/angband.h"

typedef struct autopick_type autopick_type;
struct object_type;;
struct player_type;
bool autopick_new_entry(autopick_type *entry, concptr str, bool allow_default);
void autopick_entry_from_object(player_type *player_ptr, autopick_type *entry, object_type *o_ptr);
concptr autopick_line_from_entry(autopick_type *entry);
concptr autopick_line_from_entry_kill(autopick_type *entry);
bool entry_from_choosed_object(player_type *player_ptr, autopick_type *entry);
