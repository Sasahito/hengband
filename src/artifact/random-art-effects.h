﻿#pragma once

#include "system/angband.h"

/*!
 * @file random-art-effects.h
 * @brief ランダムアーティファクトの発動ID定義
 */
enum random_art_activation_type : uint8_t {
    ACT_SUNLIGHT = 1,
    ACT_BO_MISS_1 = 2,
    ACT_BA_POIS_1 = 3,
    ACT_BO_ELEC_1 = 4,
    ACT_BO_ACID_1 = 5,
    ACT_BO_COLD_1 = 6,
    ACT_BO_FIRE_1 = 7,
    ACT_BA_COLD_1 = 8,
    ACT_BA_FIRE_1 = 9,
    ACT_HYPODYNAMIA_1 = 10,
    ACT_BA_COLD_2 = 11,
    ACT_BA_ELEC_2 = 12,
    ACT_HYPODYNAMIA_2 = 13,
    ACT_DRAIN_1 = 14,
    ACT_BO_MISS_2 = 15,
    ACT_BA_FIRE_3 = 16,
    ACT_BA_COLD_3 = 17,
    ACT_BA_ELEC_3 = 18,
    ACT_WHIRLWIND = 19,
    ACT_DRAIN_2 = 20,
    ACT_CALL_CHAOS = 21,
    ACT_ROCKET = 22,
    ACT_DISP_EVIL = 23,
    ACT_BA_MISS_3 = 24,
    ACT_DISP_GOOD = 25,
    ACT_BO_MANA = 26,
    ACT_BA_FIRE_2 = 27,
    ACT_BA_WATER = 28,
    ACT_BA_STAR = 29,
    ACT_BA_DARK = 30,
    ACT_BA_MANA = 31,
    ACT_PESTICIDE = 32,
    ACT_BLINDING_LIGHT = 33,
    ACT_BIZARRE = 34,
    ACT_CAST_BA_STAR = 35,
    ACT_BLADETURNER = 36,
    ACT_BA_ACID_1 = 37,
    ACT_BR_FIRE = 38,
    ACT_BR_COLD = 39,
    ACT_BR_DRAGON = 40,
    ACT_BA_FIRE_4 = 41,
    ACT_BA_NUKE_1 = 42,
    /* 43 - 48 unused */
    ACT_TREE_CREATION = 49,
    ACT_ANIM_DEAD = 50,
    ACT_CONFUSE = 51,
    ACT_SLEEP = 52,
    ACT_QUAKE = 53,
    ACT_TERROR = 54,
    ACT_TELE_AWAY = 55,
    ACT_BANISH_EVIL = 56,
    ACT_GENOCIDE = 57,
    ACT_MASS_GENO = 58,
    ACT_SCARE_AREA = 59,
    ACT_AGGRAVATE = 60,
    /* 59 - 64 unused */
    ACT_CHARM_ANIMAL = 65,
    ACT_CHARM_UNDEAD = 66,
    ACT_CHARM_OTHER = 67,
    ACT_CHARM_ANIMALS = 68,
    ACT_CHARM_OTHERS = 69,
    ACT_SUMMON_ANIMAL = 70,
    ACT_SUMMON_PHANTOM = 71,
    ACT_SUMMON_ELEMENTAL = 72,
    ACT_SUMMON_DEMON = 73,
    ACT_SUMMON_UNDEAD = 74,
    ACT_SUMMON_HOUND = 75,
    ACT_SUMMON_DAWN = 76,
    ACT_SUMMON_OCTOPUS = 77,
    /* 78 - 79 unused */
    ACT_CHOIR_SINGS = 80,
    ACT_CURE_LW = 81,
    ACT_CURE_MW = 82,
    ACT_CURE_POISON = 83,
    ACT_REST_EXP = 84,
    ACT_REST_ALL = 85,
    ACT_CURE_700 = 86,
    ACT_CURE_1000 = 87,
    ACT_CURING = 88,
    ACT_CURE_MANA_FULL = 89,
    ACT_ESP = 91,
    ACT_BERSERK = 92,
    ACT_PROT_EVIL = 93,
    ACT_RESIST_ALL = 94,
    ACT_SPEED = 95,
    ACT_XTRA_SPEED = 96,
    ACT_WRAITH = 97,
    ACT_INVULN = 98,
    ACT_HERO = 99,
    ACT_HERO_SPEED = 100,
    ACT_ACID_BALL_AND_RESISTANCE = 101,
    ACT_FIRE_BALL_AND_RESISTANCE = 102,
    ACT_COLD_BALL_AND_RESISTANCE = 103,
    ACT_ELEC_BALL_AND_RESISTANCE = 104,
    ACT_POIS_BALL_AND_RESISTANCE = 105,
    ACT_RESIST_ACID = 106,
    ACT_RESIST_FIRE = 107,
    ACT_RESIST_COLD = 108,
    ACT_RESIST_ELEC = 109,
    ACT_RESIST_POIS = 110,
    ACT_LIGHT = 111,
    ACT_MAP_LIGHT = 112,
    ACT_DETECT_ALL = 113,
    ACT_DETECT_XTRA = 114,
    ACT_ID_FULL = 115,
    ACT_ID_PLAIN = 116,
    ACT_RUNE_EXPLO = 117,
    ACT_RUNE_PROT = 118,
    ACT_SATIATE = 119,
    ACT_DEST_DOOR = 120,
    ACT_STONE_MUD = 121,
    ACT_RECHARGE = 122,
    ACT_ALCHEMY = 123,
    ACT_DIM_DOOR = 124,
    ACT_TELEPORT = 125,
    ACT_RECALL = 126,
    ACT_JUDGE = 127,
    ACT_TELEKINESIS = 128,
    ACT_DETECT_UNIQUE = 129,
    ACT_ESCAPE = 130,
    ACT_DISP_CURSE_XTRA = 131,
    ACT_BRAND_FIRE_BOLTS = 132,
    ACT_RECHARGE_XTRA = 133,
    ACT_LORE = 134,
    ACT_SHIKOFUMI = 135,
    ACT_PHASE_DOOR = 136,
    ACT_DETECT_ALL_MONS = 137,
    ACT_ULTIMATE_RESIST = 138,
    ACT_ELBERETH = 139,
    ACT_DETECT_TREASURE = 140,
    /* 141 - 245 unused */
    ACT_FALLING_STAR = 246,
    ACT_STRAIN_HASTE = 247,
    ACT_TELEPORT_LEVEL = 248,
    ACT_GRAND_CROSS = 249,
    ACT_CAST_OFF = 250,
    ACT_FISHING = 251,
    ACT_INROU = 252,
    ACT_MURAMASA = 253,
    ACT_BLOODY_MOON = 254,
    ACT_CRIMSON = 255,
    ACT_MAX = 255, /* ダミー、最大値と合わせること / Dummy. Please match the value as max value */
};
