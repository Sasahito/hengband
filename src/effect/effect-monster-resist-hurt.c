﻿#include "angband.h"
#include "effect/effect-monster-util.h"
#include "effect/effect-monster-resist-hurt.h"
#include "monster-status.h"

gf_switch_result effect_monster_void(effect_monster_type *em_ptr)
{
	if (em_ptr->seen) em_ptr->obvious = TRUE;

	return GF_SWITCH_CONTINUE;
}

gf_switch_result effect_monster_acid(player_type *caster_ptr, effect_monster_type *em_ptr)
{
	if (em_ptr->seen) em_ptr->obvious = TRUE;
	if ((em_ptr->r_ptr->flagsr & RFR_IM_ACID) == 0) return GF_SWITCH_CONTINUE;

	em_ptr->note = _("にはかなり耐性がある！", " resists a lot.");
	em_ptr->dam /= 9;
	if (is_original_ap_and_seen(caster_ptr, em_ptr->m_ptr))
		em_ptr->r_ptr->r_flagsr |= (RFR_IM_ACID);

	return GF_SWITCH_CONTINUE;
}


gf_switch_result effect_monster_elec(player_type *caster_ptr, effect_monster_type *em_ptr)
{
	if (em_ptr->seen) em_ptr->obvious = TRUE;
	if ((em_ptr->r_ptr->flagsr & RFR_IM_ELEC) == 0) return GF_SWITCH_CONTINUE;

	em_ptr->note = _("にはかなり耐性がある！", " resists a lot.");
	em_ptr->dam /= 9;
	if (is_original_ap_and_seen(caster_ptr, em_ptr->m_ptr))
		em_ptr->r_ptr->r_flagsr |= (RFR_IM_ELEC);

	return GF_SWITCH_CONTINUE;
}


gf_switch_result effect_monster_fire(player_type *caster_ptr, effect_monster_type *em_ptr)
{
	if (em_ptr->seen) em_ptr->obvious = TRUE;
	if (em_ptr->r_ptr->flagsr & RFR_IM_FIRE)
	{
		em_ptr->note = _("にはかなり耐性がある！", " resists a lot.");
		em_ptr->dam /= 9;
		if (is_original_ap_and_seen(caster_ptr, em_ptr->m_ptr))
			em_ptr->r_ptr->r_flagsr |= (RFR_IM_FIRE);

		return GF_SWITCH_CONTINUE;
	}

	if ((em_ptr->r_ptr->flags3 & (RF3_HURT_FIRE)) == 0) return GF_SWITCH_CONTINUE;

	em_ptr->note = _("はひどい痛手をうけた。", " is hit hard.");
	em_ptr->dam *= 2;
	if (is_original_ap_and_seen(caster_ptr, em_ptr->m_ptr))
		em_ptr->r_ptr->r_flags3 |= (RF3_HURT_FIRE);

	return GF_SWITCH_CONTINUE;
}


gf_switch_result effect_monster_cold(player_type *caster_ptr, effect_monster_type *em_ptr)
{
	if (em_ptr->seen) em_ptr->obvious = TRUE;
	if (em_ptr->r_ptr->flagsr & RFR_IM_COLD)
	{
		em_ptr->note = _("にはかなり耐性がある！", " resists a lot.");
		em_ptr->dam /= 9;
		if (is_original_ap_and_seen(caster_ptr, em_ptr->m_ptr))
			em_ptr->r_ptr->r_flagsr |= (RFR_IM_COLD);

		return GF_SWITCH_CONTINUE;
	}

	if ((em_ptr->r_ptr->flags3 & (RF3_HURT_COLD)) == 0) return GF_SWITCH_CONTINUE;

	em_ptr->note = _("はひどい痛手をうけた。", " is hit hard.");
	em_ptr->dam *= 2;
	if (is_original_ap_and_seen(caster_ptr, em_ptr->m_ptr))
		em_ptr->r_ptr->r_flags3 |= (RF3_HURT_COLD);

	return GF_SWITCH_CONTINUE;
}


gf_switch_result effect_monster_pois(player_type *caster_ptr, effect_monster_type *em_ptr)
{
	if (em_ptr->seen) em_ptr->obvious = TRUE;
	if ((em_ptr->r_ptr->flagsr & RFR_IM_POIS) == 0) return GF_SWITCH_CONTINUE;

	em_ptr->note = _("にはかなり耐性がある！", " resists a lot.");
	em_ptr->dam /= 9;
	if (is_original_ap_and_seen(caster_ptr, em_ptr->m_ptr))
		em_ptr->r_ptr->r_flagsr |= (RFR_IM_POIS);

	return GF_SWITCH_CONTINUE;
}


gf_switch_result effect_monster_nuke(player_type *caster_ptr, effect_monster_type *em_ptr)
{
	if (em_ptr->seen) em_ptr->obvious = TRUE;
	if (em_ptr->r_ptr->flagsr & RFR_IM_POIS)
	{
		em_ptr->note = _("には耐性がある。", " resists.");
		em_ptr->dam *= 3; em_ptr->dam /= randint1(6) + 6;
		if (is_original_ap_and_seen(caster_ptr, em_ptr->m_ptr))
			em_ptr->r_ptr->r_flagsr |= (RFR_IM_POIS);

		return GF_SWITCH_CONTINUE;
	}

	if (one_in_(3)) em_ptr->do_polymorph = TRUE;

	return GF_SWITCH_CONTINUE;
}


gf_switch_result effect_monster_hell_fire(player_type *caster_ptr, effect_monster_type *em_ptr)
{
	if (em_ptr->seen) em_ptr->obvious = TRUE;
	if ((em_ptr->r_ptr->flags3 & RF3_GOOD) == 0) return GF_SWITCH_CONTINUE;

	em_ptr->note = _("はひどい痛手をうけた。", " is hit hard.");
	em_ptr->dam *= 2;
	if (is_original_ap_and_seen(caster_ptr, em_ptr->m_ptr))
		em_ptr->r_ptr->r_flags3 |= RF3_GOOD;

	return GF_SWITCH_CONTINUE;
}


gf_switch_result effect_monster_holy_fire(player_type *caster_ptr, effect_monster_type *em_ptr)
{
	if (em_ptr->seen) em_ptr->obvious = TRUE;
	if ((em_ptr->r_ptr->flags3 & RF3_EVIL) == 0)
	{
		em_ptr->note = _("には耐性がある。", " resists.");
		em_ptr->dam *= 3; em_ptr->dam /= randint1(6) + 6;
		return GF_SWITCH_CONTINUE;
	}

	em_ptr->dam *= 2;
	em_ptr->note = _("はひどい痛手をうけた。", " is hit hard.");
	if (is_original_ap_and_seen(caster_ptr, em_ptr->m_ptr))
		em_ptr->r_ptr->r_flags3 |= RF3_EVIL;

	return GF_SWITCH_CONTINUE;
}


gf_switch_result effect_monster_plasma(player_type *caster_ptr, effect_monster_type *em_ptr)
{
	if (em_ptr->seen) em_ptr->obvious = TRUE;
	if ((em_ptr->r_ptr->flagsr & RFR_RES_PLAS) == 0) return GF_SWITCH_CONTINUE;

	em_ptr->note = _("には耐性がある。", " resists.");
	em_ptr->dam *= 3; em_ptr->dam /= randint1(6) + 6;
	if (is_original_ap_and_seen(caster_ptr, em_ptr->m_ptr))
		em_ptr->r_ptr->r_flagsr |= (RFR_RES_PLAS);

	return GF_SWITCH_CONTINUE;
}


static bool effect_monster_nether_resist(player_type *caster_ptr, effect_monster_type *em_ptr)
{
	if ((em_ptr->r_ptr->flagsr & RFR_RES_NETH) == 0) return FALSE;

	if (em_ptr->r_ptr->flags3 & RF3_UNDEAD)
	{
		em_ptr->note = _("には完全な耐性がある！", " is immune.");
		em_ptr->dam = 0;
		if (is_original_ap_and_seen(caster_ptr, em_ptr->m_ptr))
			em_ptr->r_ptr->r_flags3 |= (RF3_UNDEAD);
	}
	else
	{
		em_ptr->note = _("には耐性がある。", " resists.");
		em_ptr->dam *= 3; em_ptr->dam /= randint1(6) + 6;
	}

	if (is_original_ap_and_seen(caster_ptr, em_ptr->m_ptr))
		em_ptr->r_ptr->r_flagsr |= (RFR_RES_NETH);

	return TRUE;
}


gf_switch_result effect_monster_nether(player_type *caster_ptr, effect_monster_type *em_ptr)
{
	if (em_ptr->seen) em_ptr->obvious = TRUE;

	if (effect_monster_nether_resist(caster_ptr, em_ptr)) return GF_SWITCH_CONTINUE;
	if ((em_ptr->r_ptr->flags3 & RF3_EVIL) == 0) return GF_SWITCH_CONTINUE;

	em_ptr->note = _("はいくらか耐性を示した。", " resists somewhat.");
	em_ptr->dam /= 2;
	if (is_original_ap_and_seen(caster_ptr, em_ptr->m_ptr))
		em_ptr->r_ptr->r_flags3 |= (RF3_EVIL);

	return GF_SWITCH_CONTINUE;
}


gf_switch_result effect_monster_water(player_type *caster_ptr, effect_monster_type *em_ptr)
{
	if (em_ptr->seen) em_ptr->obvious = TRUE;
	if ((em_ptr->r_ptr->flagsr & RFR_RES_WATE) == 0) return GF_SWITCH_CONTINUE;

	if ((em_ptr->m_ptr->r_idx == MON_WATER_ELEM) || (em_ptr->m_ptr->r_idx == MON_UNMAKER))
	{
		em_ptr->note = _("には完全な耐性がある！", " is immune.");
		em_ptr->dam = 0;
	}
	else
	{
		em_ptr->note = _("には耐性がある。", " resists.");
		em_ptr->dam *= 3; em_ptr->dam /= randint1(6) + 6;
	}

	if (is_original_ap_and_seen(caster_ptr, em_ptr->m_ptr))
		em_ptr->r_ptr->r_flagsr |= (RFR_RES_WATE);

	return GF_SWITCH_CONTINUE;
}


gf_switch_result effect_monster_chaos(player_type *caster_ptr, effect_monster_type *em_ptr)
{
	if (em_ptr->seen) em_ptr->obvious = TRUE;
	if (em_ptr->r_ptr->flagsr & RFR_RES_CHAO)
	{
		em_ptr->note = _("には耐性がある。", " resists.");
		em_ptr->dam *= 3; em_ptr->dam /= randint1(6) + 6;
		if (is_original_ap_and_seen(caster_ptr, em_ptr->m_ptr))
			em_ptr->r_ptr->r_flagsr |= (RFR_RES_CHAO);
	}
	else if ((em_ptr->r_ptr->flags3 & RF3_DEMON) && one_in_(3))
	{
		em_ptr->note = _("はいくらか耐性を示した。", " resists somewhat.");
		em_ptr->dam *= 3; em_ptr->dam /= randint1(6) + 6;
		if (is_original_ap_and_seen(caster_ptr, em_ptr->m_ptr))
			em_ptr->r_ptr->r_flags3 |= (RF3_DEMON);
	}
	else
	{
		em_ptr->do_polymorph = TRUE;
		em_ptr->do_conf = (5 + randint1(11) + em_ptr->r) / (em_ptr->r + 1);
	}

	return GF_SWITCH_CONTINUE;
}


gf_switch_result effect_monster_shards(player_type *caster_ptr, effect_monster_type *em_ptr)
{
	if (em_ptr->seen) em_ptr->obvious = TRUE;
	if ((em_ptr->r_ptr->flagsr & RFR_RES_SHAR) == 0) return GF_SWITCH_CONTINUE;

	em_ptr->note = _("には耐性がある。", " resists.");
	em_ptr->dam *= 3; em_ptr->dam /= randint1(6) + 6;
	if (is_original_ap_and_seen(caster_ptr, em_ptr->m_ptr))
		em_ptr->r_ptr->r_flagsr |= (RFR_RES_SHAR);

	return GF_SWITCH_CONTINUE;
}


gf_switch_result effect_monster_rocket(player_type *caster_ptr, effect_monster_type *em_ptr)
{
	if (em_ptr->seen) em_ptr->obvious = TRUE;
	if ((em_ptr->r_ptr->flagsr & RFR_RES_SHAR) == 0) return GF_SWITCH_CONTINUE;

	em_ptr->note = _("はいくらか耐性を示した。", " resists somewhat.");
	em_ptr->dam /= 2;
	if (is_original_ap_and_seen(caster_ptr, em_ptr->m_ptr))
		em_ptr->r_ptr->r_flagsr |= (RFR_RES_SHAR);

	return GF_SWITCH_CONTINUE;
}


gf_switch_result effect_monster_sound(player_type *caster_ptr, effect_monster_type *em_ptr)
{
	if (em_ptr->seen) em_ptr->obvious = TRUE;
	if ((em_ptr->r_ptr->flagsr & RFR_RES_SOUN) == 0)
	{
		em_ptr->do_stun = (10 + randint1(15) + em_ptr->r) / (em_ptr->r + 1);
		return GF_SWITCH_CONTINUE;
	}

	em_ptr->note = _("には耐性がある。", " resists.");
	em_ptr->dam *= 2; em_ptr->dam /= randint1(6) + 6;
	if (is_original_ap_and_seen(caster_ptr, em_ptr->m_ptr))
		em_ptr->r_ptr->r_flagsr |= (RFR_RES_SOUN);

	return GF_SWITCH_CONTINUE;
}


gf_switch_result effect_monster_confusion(player_type *caster_ptr, effect_monster_type *em_ptr)
{
	if (em_ptr->seen) em_ptr->obvious = TRUE;
	if ((em_ptr->r_ptr->flags3 & RF3_NO_CONF) == 0)
	{
		em_ptr->do_conf = (10 + randint1(15) + em_ptr->r) / (em_ptr->r + 1);
		return GF_SWITCH_CONTINUE;
	}

	em_ptr->note = _("には耐性がある。", " resists.");
	em_ptr->dam *= 3; em_ptr->dam /= randint1(6) + 6;
	if (is_original_ap_and_seen(caster_ptr, em_ptr->m_ptr))
		em_ptr->r_ptr->r_flags3 |= (RF3_NO_CONF);

	return GF_SWITCH_CONTINUE;
}


gf_switch_result effect_monster_disenchant(player_type *caster_ptr, effect_monster_type *em_ptr)
{
	if (em_ptr->seen) em_ptr->obvious = TRUE;
	if ((em_ptr->r_ptr->flagsr & RFR_RES_DISE) == 0) return GF_SWITCH_CONTINUE;

	em_ptr->note = _("には耐性がある。", " resists.");
	em_ptr->dam *= 3; em_ptr->dam /= randint1(6) + 6;
	if (is_original_ap_and_seen(caster_ptr, em_ptr->m_ptr))
		em_ptr->r_ptr->r_flagsr |= (RFR_RES_DISE);

	return GF_SWITCH_CONTINUE;
}


gf_switch_result effect_monster_nexus(player_type *caster_ptr, effect_monster_type *em_ptr)
{
	if (em_ptr->seen) em_ptr->obvious = TRUE;
	if ((em_ptr->r_ptr->flagsr & RFR_RES_NEXU) == 0) return GF_SWITCH_CONTINUE;

	em_ptr->note = _("には耐性がある。", " resists.");
	em_ptr->dam *= 3; em_ptr->dam /= randint1(6) + 6;
	if (is_original_ap_and_seen(caster_ptr, em_ptr->m_ptr))
		em_ptr->r_ptr->r_flagsr |= (RFR_RES_NEXU);

	return GF_SWITCH_CONTINUE;
}


gf_switch_result effect_monster_force(player_type *caster_ptr, effect_monster_type *em_ptr)
{
	if (em_ptr->seen) em_ptr->obvious = TRUE;
	if ((em_ptr->r_ptr->flagsr & RFR_RES_WALL) == 0)
	{
		em_ptr->do_stun = (randint1(15) + em_ptr->r) / (em_ptr->r + 1);
		return GF_SWITCH_CONTINUE;
	}

	em_ptr->note = _("には耐性がある。", " resists.");
	em_ptr->dam *= 3; em_ptr->dam /= randint1(6) + 6;
	if (is_original_ap_and_seen(caster_ptr, em_ptr->m_ptr))
		em_ptr->r_ptr->r_flagsr |= (RFR_RES_WALL);

	return GF_SWITCH_CONTINUE;
}


// Powerful monsters can resists and normal monsters slow down.
gf_switch_result effect_monster_inertial(player_type *caster_ptr, effect_monster_type *em_ptr)
{
	if (em_ptr->seen) em_ptr->obvious = TRUE;
	if (em_ptr->r_ptr->flagsr & RFR_RES_INER)
	{
		em_ptr->note = _("には耐性がある。", " resists.");
		em_ptr->dam *= 3; em_ptr->dam /= randint1(6) + 6;
		if (is_original_ap_and_seen(caster_ptr, em_ptr->m_ptr))
			em_ptr->r_ptr->r_flagsr |= (RFR_RES_INER);

		return GF_SWITCH_CONTINUE;
	}

	if ((em_ptr->r_ptr->flags1 & (RF1_UNIQUE)) ||
		(em_ptr->r_ptr->level > randint1((em_ptr->dam - 10) < 1 ? 1 : (em_ptr->dam - 10)) + 10))
	{
		em_ptr->obvious = FALSE;
		return GF_SWITCH_CONTINUE;
	}

	if (set_monster_slow(caster_ptr, em_ptr->g_ptr->m_idx, MON_SLOW(em_ptr->m_ptr) + 50))
		em_ptr->note = _("の動きが遅くなった。", " starts moving slower.");

	return GF_SWITCH_CONTINUE;
}


gf_switch_result effect_monster_time(player_type *caster_ptr, effect_monster_type *em_ptr)
{
	if (em_ptr->seen) em_ptr->obvious = TRUE;
	if ((em_ptr->r_ptr->flagsr & RFR_RES_TIME) == 0)
	{
		em_ptr->do_time = (em_ptr->dam + 1) / 2;
		return GF_SWITCH_CONTINUE;
	}

	em_ptr->note = _("には耐性がある。", " resists.");
	em_ptr->dam *= 3; em_ptr->dam /= randint1(6) + 6;
	if (is_original_ap_and_seen(caster_ptr, em_ptr->m_ptr))
		em_ptr->r_ptr->r_flagsr |= (RFR_RES_TIME);

	return GF_SWITCH_CONTINUE;
}


static bool effect_monster_gravity_resist_teleport(player_type *caster_ptr, effect_monster_type *em_ptr)
{

	if (em_ptr->seen) em_ptr->obvious = TRUE;
	if ((em_ptr->r_ptr->flagsr & RFR_RES_TELE) == 0) return FALSE;

	if (em_ptr->r_ptr->flags1 & (RF1_UNIQUE))
	{
		if (is_original_ap_and_seen(caster_ptr, em_ptr->m_ptr))
			em_ptr->r_ptr->r_flagsr |= RFR_RES_TELE;

		em_ptr->note = _("には効果がなかった。", " is unaffected!");
		return TRUE;
	}

	if (em_ptr->r_ptr->level <= randint1(100)) return FALSE;

	if (is_original_ap_and_seen(caster_ptr, em_ptr->m_ptr))
		em_ptr->r_ptr->r_flagsr |= RFR_RES_TELE;

	em_ptr->note = _("には耐性がある！", " resists!");
	return TRUE;
}


static void effect_monster_gravity_slow(player_type *caster_ptr, effect_monster_type *em_ptr)
{
	if ((em_ptr->r_ptr->flags1 & (RF1_UNIQUE)) ||
		(em_ptr->r_ptr->level > randint1((em_ptr->dam - 10) < 1 ? 1 : (em_ptr->dam - 10)) + 10))
		em_ptr->obvious = FALSE;

	if (set_monster_slow(caster_ptr, em_ptr->g_ptr->m_idx, MON_SLOW(em_ptr->m_ptr) + 50))
		em_ptr->note = _("の動きが遅くなった。", " starts moving slower.");
}


static void effect_monster_gravity_stun(effect_monster_type *em_ptr)
{
	em_ptr->do_stun = damroll((em_ptr->caster_lev / 20) + 3, (em_ptr->dam)) + 1;
	if ((em_ptr->r_ptr->flags1 & (RF1_UNIQUE)) ||
		(em_ptr->r_ptr->level > randint1((em_ptr->dam - 10) < 1 ? 1 : (em_ptr->dam - 10)) + 10))
	{
		em_ptr->do_stun = 0;
		em_ptr->note = _("には効果がなかった。", " is unaffected!");
		em_ptr->obvious = FALSE;
	}
}


/*
 * Powerful monsters can resist and normal monsters slow down
 * Furthermore, this magic can make non-unique monsters slow/stun.
 */
gf_switch_result effect_monster_gravity(player_type *caster_ptr, effect_monster_type *em_ptr)
{
	em_ptr->do_dist = effect_monster_gravity_resist_teleport(caster_ptr, em_ptr) ? 0 : 10;
	if (caster_ptr->riding && (em_ptr->g_ptr->m_idx == caster_ptr->riding)) em_ptr->do_dist = 0;

	if (em_ptr->r_ptr->flagsr & RFR_RES_GRAV)
	{
		em_ptr->note = _("には耐性がある！", " resists!");
		em_ptr->dam *= 3; em_ptr->dam /= randint1(6) + 6;
		em_ptr->do_dist = 0;
		if (is_original_ap_and_seen(caster_ptr, em_ptr->m_ptr))
			em_ptr->r_ptr->r_flagsr |= (RFR_RES_GRAV);

		return GF_SWITCH_CONTINUE;
	}

	effect_monster_gravity_slow(caster_ptr, em_ptr);
	effect_monster_gravity_stun(em_ptr);
	return GF_SWITCH_CONTINUE;
}


gf_switch_result effect_monster_disintegration(player_type *caster_ptr, effect_monster_type *em_ptr)
{
	if (em_ptr->seen) em_ptr->obvious = TRUE;
	if ((em_ptr->r_ptr->flags3 & RF3_HURT_ROCK) == 0) return GF_SWITCH_CONTINUE;

	if (is_original_ap_and_seen(caster_ptr, em_ptr->m_ptr))
		em_ptr->r_ptr->r_flags3 |= (RF3_HURT_ROCK);

	em_ptr->note = _("の皮膚がただれた！", " loses some skin!");
	em_ptr->note_dies = _("は蒸発した！", " evaporates!");
	em_ptr->dam *= 2;
	return GF_SWITCH_CONTINUE;
}

gf_switch_result effect_monster_icee_bolt(player_type *caster_ptr, effect_monster_type *em_ptr)
{
	if (em_ptr->seen) em_ptr->obvious = TRUE;

	em_ptr->do_stun = (randint1(15) + 1) / (em_ptr->r + 1);
	if (em_ptr->r_ptr->flagsr & RFR_IM_COLD)
	{
		em_ptr->note = _("にはかなり耐性がある！", " resists a lot.");
		em_ptr->dam /= 9;
		if (is_original_ap_and_seen(caster_ptr, em_ptr->m_ptr))
			em_ptr->r_ptr->r_flagsr |= (RFR_IM_COLD);
	}
	else if (em_ptr->r_ptr->flags3 & (RF3_HURT_COLD))
	{
		em_ptr->note = _("はひどい痛手をうけた。", " is hit hard.");
		em_ptr->dam *= 2;
		if (is_original_ap_and_seen(caster_ptr, em_ptr->m_ptr))
			em_ptr->r_ptr->r_flags3 |= (RF3_HURT_COLD);
	}

	return GF_SWITCH_CONTINUE;
}