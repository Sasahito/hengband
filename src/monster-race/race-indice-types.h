﻿#pragma once

typedef enum monster_race_type {
    MON_PLAYER = 0, // Dummy.
    MON_BEGGAR = 12,
    MON_LEPER = 13,
    MON_LION_HEART = 19,
    MON_NOV_PRIEST = 45,
    MON_GRIP = 53,
    MON_WOLF = 54,
    MON_FANG = 55,
    MON_LOUSE = 69,
    MON_PIRANHA = 70,
    MON_COPPER_COINS = 85,
    MON_NOV_PALADIN = 97,
    MON_NOV_PRIEST_G = 109,
    MON_SILVER_COINS = 117,
    MON_D_ELF = 122,
    MON_MANES = 128,
    MON_NOV_PALADIN_G = 147,
    MON_PHANTOM_W = 152,
    MON_WOUNDED_BEAR = 159,
    MON_D_ELF_MAGE = 178,
    MON_D_ELF_WARRIOR = 182,
    MON_BLUE_HORROR = 189,
    MON_GOLD_COINS = 195,
    MON_MASTER_YEEK = 224,
    MON_PRIEST = 225,
    MON_D_ELF_PRIEST = 226,
    MON_MITHRIL_COINS = 239,
    MON_PINK_HORROR = 242,
    MON_IMP = 296,
    MON_LIZARD_KING = 332,
    MON_WYVERN = 334,
    MON_SABRE_TIGER = 339,
    MON_D_ELF_LORD = 348,
    MON_ARCH_VILE = 357,
    MON_JADE_MONK = 370,
    MON_D_ELF_WARLOCK = 375,
    MON_MENELDOR = 384,
    MON_PHANTOM_B = 385,
    MON_D_ELF_DRUID = 400,
    MON_GWAIHIR = 410,
    MON_ADAMANT_COINS = 423,
    MON_COLBRAN = 435,
    MON_MITHRIL_GOLEM = 464,
    MON_THORONDOR = 468,
    MON_GHOUL_KING = 483,
    MON_NINJA = 485,
    MON_BICLOPS = 490,
    MON_IVORY_MONK = 492,
    MON_GOEMON = 505,
    MON_WATER_ELEM = 512,
    MON_BLOODLETTER = 523,
    MON_RAAL = 557,
    MON_NIGHTBLADE = 564,
    MON_BARON_HELL = 609,
    MON_ANCIENT_CRISTAL_DRAGON = 646,
    MON_FALLEN_ANGEL = 652,
    MON_D_ELF_SORC = 657,
    MON_DREADMASTER = 690,
    MON_DROLEM = 691,
    MON_DAWN = 693,
    MON_NAZGUL = 696,
    MON_SMAUG = 697,
    MON_STORMBRINGER = 698,
    MON_ULTRA_PALADIN = 699,
    MON_S_TYRANNO = 705,
    MON_FAFNER = 712,
    MON_G_BALROG = 720,
    MON_BULLGATES = 732,
    MON_LORD_CHAOS = 737,
    MON_NIGHTWALKER = 768,
    MON_SHADOWLORD = 774,
    MON_JABBERWOCK = 778,
    MON_SHAMBLER = 786,
    MON_BLACK_REAVER = 798,
    MON_UNMAKER = 815,
    MON_CYBER = 816,
    MON_ANGMAR = 825,
    MON_WYRM_POWER = 847,
    MON_JORMUNGAND = 854,
    MON_SAURON = 858,
    MON_UNICORN_ORD = 859,
    MON_OBERON = 860,
    MON_MORGOTH = 861,
    MON_SERPENT = 862,
    MON_ONE_RING = 864,
    MON_EBONY_MONK = 870,
    MON_HAGURE = 871,
    MON_DIO = 878,
    MON_OHMU = 879,
    MON_WONG = 880,
    MON_ZOMBI_SERPENT = 883,
    MON_D_ELF_SHADE = 886,
    MON_TROLL_KING = 894,
    MON_ELF_LORD = 900,
    MON_TSUCHINOKO = 926,
    MON_LOCKE_CLONE = 930,
    MON_CALDARM = 931,
    MON_BANORLUPART = 932,
    MON_BANOR = 933,
    MON_LUPART = 934,
    MON_KENSHIROU = 936,
    MON_W_KNIGHT = 938,
    MON_BIKETAL = 945,
    MON_IKETA = 949,
    MON_B_DEATH_SWORD = 953,
    MON_YASE_HORSE = 955,
    MON_HORSE = 956,
    MON_BOTEI = 963,
    MON_KAGE = 964,
    MON_JAIAN = 967,
    MON_FENGHUANG = 988,
    MON_SUKE = 1001,
    MON_KAKU = 1002,
    MON_A_GOLD = 1010,
    MON_A_SILVER = 1011,
    MON_ROLENTO = 1013,
    MON_RAOU = 1018,
    MON_GRENADE = 1023,
    MON_DEBBY = 1032,
    MON_KNI_TEMPLAR = 1037,
    MON_PALADIN = 1038,
    MON_CHAMELEON = 1040,
    MON_CHAMELEON_K = 1041,
    MON_TOPAZ_MONK = 1047,
    MON_M_MINDCRAFTER = 1056,
    MON_ELDER_VAMPIRE = 1058,
    MON_NOBORTA = 1059,
    MON_MORI_TROLL = 1060,
    MON_BARNEY = 1061,
    MON_GROO = 1062,
    MON_LOUSY = 1063,
    MON_WYRM_SPACE = 1064,
    MON_JIZOTAKO = 1065,
    MON_TANUKI = 1067,
    MON_ALIEN_JURAL = 1082,
    MON_MIDDLE_AQUA_FIRST = 1152,
    MON_LARGE_AQUA_FIRST = 1153,
    MON_EXTRA_LARGE_AQUA_FIRST = 1154,
    MON_MIDDLE_AQUA_SECOND = 1156,
    MON_LARGE_AQUA_SECOND = 1157,
    MON_EXTRA_LARGE_AQUA_SECOND = 1158,
    MON_SMALL_MOAI = 1159,
    MON_TOTEM_MOAI = 1161,
    MON_VAIF = 1162,
    MON_DEMON_SLAYER_SENIOR = 1164,
    MON_DEMON_SLAYER_MEMBER = 1165,
    MON_MASTER_MYSTIC = 1178,
    MON_BRONZE_LICH = 1180,
    MON_ULT_BEHOLDER = 1185,
    MON_G_TITAN = 1187,
    MON_WYRM_COLOURS = 1198,
    MON_ALDUIN = 1209,
    MON_FOLLOWER_WARRIOR = 1210,
    MON_FOLLOWER_MAGE = 1211,
    MON_MIRAAK = 1212,
    MON_SCARAB = 1220,
    MON_IMHOTEP = 1221,
    MON_EDGE = 1245,
    MON_JOBZ = 1247,
} monster_race_type;
