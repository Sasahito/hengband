﻿#include "market/arena-info-table.h"
#include "monster-race/race-indice-types.h"
#include "object/tval-types.h"
#include "sv-definition/sv-amulet-types.h"
#include "sv-definition/sv-food-types.h"
#include "sv-definition/sv-potion-types.h"
#include "sv-definition/sv-ring-types.h"
#include "sv-definition/sv-rod-types.h"
#include "sv-definition/sv-scroll-types.h"
#include "sv-definition/sv-staff-types.h"
#include "sv-definition/sv-wand-types.h"
#include "system/building-type-definition.h"

/*!
 * @brief 闘技場のモンスターID及び報酬アイテムテーブル
 */
const arena_type arena_info[MAX_ARENA_MONS + 2] = {
    { MON_NOBORTA, TV_AMULET, SV_AMULET_ADORNMENT },
    { MON_MORI_TROLL, TV_FOOD, SV_FOOD_PINT_OF_WINE },
    { MON_IMP, TV_POTION, SV_POTION_SPEED },
    { MON_LION_HEART, 0, 0 },
    { MON_MASTER_YEEK, TV_POTION, SV_POTION_CURING },
    { MON_SABRE_TIGER, TV_WAND, SV_WAND_STONE_TO_MUD },
    { MON_LIZARD_KING, TV_WAND, SV_WAND_TELEPORT_AWAY },
    { MON_WYVERN, TV_POTION, SV_POTION_HEALING },
    { MON_ARCH_VILE, TV_POTION, SV_POTION_RESISTANCE },
    { MON_ELF_LORD, TV_POTION, SV_POTION_ENLIGHTENMENT },
    { MON_GHOUL_KING, TV_FOOD, SV_FOOD_RESTORING },
    { MON_COLBRAN, TV_RING, SV_RING_ELEC },
    { MON_BICLOPS, TV_WAND, SV_WAND_ACID_BALL },
    { MON_M_MINDCRAFTER, TV_POTION, SV_POTION_SELF_KNOWLEDGE },
    { MON_GROO, TV_SCROLL, SV_SCROLL_ACQUIREMENT },
    { MON_RAAL, TV_SCROLL, SV_SCROLL_STAR_DESTRUCTION },
    { MON_DREADMASTER, TV_WAND, SV_WAND_HYPODYNAMIA },
    { MON_ULTRA_PALADIN, TV_STAFF, SV_STAFF_DISPEL_EVIL },
    { MON_BARNEY, TV_RING, SV_RING_RES_CHAOS },
    { MON_TROLL_KING, TV_SCROLL, SV_SCROLL_MASS_GENOCIDE },
    { MON_BARON_HELL, TV_POTION, SV_SCROLL_RUNE_OF_PROTECTION },
    { MON_FALLEN_ANGEL, TV_SCROLL, SV_POTION_AUGMENTATION },
    { MON_ANCIENT_CRISTAL_DRAGON, TV_WAND, SV_WAND_DRAGON_FIRE },
    { MON_BRONZE_LICH, TV_STAFF, SV_STAFF_DESTRUCTION },
    { MON_DROLEM, TV_POTION, SV_POTION_STAR_HEALING },
    { MON_G_TITAN, TV_WAND, SV_WAND_GENOCIDE },
    { MON_G_BALROG, TV_POTION, SV_POTION_EXPERIENCE },
    { MON_ELDER_VAMPIRE, TV_RING, SV_RING_SUSTAIN },
    { MON_NIGHTWALKER, TV_WAND, SV_WAND_STRIKING },
    { MON_S_TYRANNO, TV_SCROLL, SV_SCROLL_STAR_ACQUIREMENT },
    { MON_ACTING_MASTER_MYSTIC, TV_ROD, SV_ROD_IDENTIFY },
    { MON_LORD_CHAOS, TV_POTION, SV_POTION_LIFE },
    { MON_SHADOWLORD, TV_POTION, SV_POTION_STAR_ENLIGHTENMENT },
    { MON_ULT_BEHOLDER, TV_AMULET, SV_AMULET_REFLECTION },
    { MON_JABBERWOCK, TV_ROD, SV_ROD_HEALING },
    { MON_LOCKE_CLONE, TV_WAND, SV_WAND_DISINTEGRATE },
    { MON_WYRM_SPACE, TV_ROD, SV_ROD_RESTORATION },
    { MON_SHAMBLER, TV_SCROLL, SV_SCROLL_STAR_ACQUIREMENT },
    { MON_BLACK_REAVER, TV_RING, SV_RING_LORDLY },
    { MON_FENGHUANG, TV_STAFF, SV_STAFF_THE_MAGI },
    { MON_WYRM_POWER, TV_SCROLL, SV_SCROLL_ARTIFACT },
    { 0, 0, 0 }, /* Victory prizing */
    { MON_HAGURE, TV_SCROLL, SV_SCROLL_ARTIFACT },
};
