﻿#pragma once

#include "system/angband.h"

#include "monster-race/race-ability-flags.h"
#include "util/flag-group.h"

struct bluemage_data_type {
    EnumClassFlagGroup<RF_ABILITY> learnt_blue_magics{};
    bool new_magic_learned{};
};
