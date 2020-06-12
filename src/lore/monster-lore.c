﻿/*!
 * @brief モンスターの思い出表示に必要なフラグ類の処理
 * @date 2020/06/09
 * @author Hourier
 */

#include "lore/monster-lore.h"
#include "locale/english.h"
#include "locale/japanese.h"
#include "lore/lore-calculator.h"
#include "lore/lore-util.h"
#include "lore/magic-types-setter.h"
#include "monster-race/race-flags-ability1.h"
#include "monster-race/race-flags-ability2.h"
#include "monster-race/race-flags-resistance.h"
#include "monster-race/race-flags1.h"
#include "monster-race/race-flags2.h"
#include "monster-race/race-flags3.h"
#include "monster-race/race-flags4.h"
#include "monster-race/race-flags7.h"
#include "monster-race/race-indice-types.h"
#include "mspell/monster-spell.h"
#include "mspell/mspell-damage-calculator.h"
#include "mspell/mspell-type.h"
#include "term/term-color-types.h"
#include "util/util.h"
#include "view/display-lore-magics.h"
#include "view/display-lore.h"

/*!
 * 英語の複数系記述用マクロ / Pluralizer.  Args(count, singular, plural)
 */
#define plural(c, s, p) (((c) == 1) ? (s) : (p))

static void set_msex_flags(lore_type *lore_ptr)
{
    lore_ptr->msex = MSEX_NONE;
    if (lore_ptr->r_ptr->flags1 & RF1_FEMALE) {
        lore_ptr->msex = MSEX_FEMALE;
        return;
    }

    if (lore_ptr->r_ptr->flags1 & RF1_MALE)
        lore_ptr->msex = MSEX_MALE;
}

static void set_flags1(lore_type *lore_ptr)
{
    if (lore_ptr->r_ptr->flags1 & RF1_UNIQUE)
        lore_ptr->flags1 |= (RF1_UNIQUE);

    if (lore_ptr->r_ptr->flags1 & RF1_QUESTOR)
        lore_ptr->flags1 |= (RF1_QUESTOR);

    if (lore_ptr->r_ptr->flags1 & RF1_MALE)
        lore_ptr->flags1 |= (RF1_MALE);

    if (lore_ptr->r_ptr->flags1 & RF1_FEMALE)
        lore_ptr->flags1 |= (RF1_FEMALE);

    if (lore_ptr->r_ptr->flags1 & RF1_FRIENDS)
        lore_ptr->flags1 |= (RF1_FRIENDS);

    if (lore_ptr->r_ptr->flags1 & RF1_ESCORT)
        lore_ptr->flags1 |= (RF1_ESCORT);

    if (lore_ptr->r_ptr->flags1 & RF1_ESCORTS)
        lore_ptr->flags1 |= (RF1_ESCORTS);
}

static void set_race_flags(lore_type *lore_ptr)
{
    if (!lore_ptr->r_ptr->r_tkills && !lore_ptr->know_everything)
        return;

    if (lore_ptr->r_ptr->flags3 & RF3_ORC)
        lore_ptr->flags3 |= (RF3_ORC);

    if (lore_ptr->r_ptr->flags3 & RF3_TROLL)
        lore_ptr->flags3 |= (RF3_TROLL);

    if (lore_ptr->r_ptr->flags3 & RF3_GIANT)
        lore_ptr->flags3 |= (RF3_GIANT);

    if (lore_ptr->r_ptr->flags3 & RF3_DRAGON)
        lore_ptr->flags3 |= (RF3_DRAGON);

    if (lore_ptr->r_ptr->flags3 & RF3_DEMON)
        lore_ptr->flags3 |= (RF3_DEMON);

    if (lore_ptr->r_ptr->flags3 & RF3_UNDEAD)
        lore_ptr->flags3 |= (RF3_UNDEAD);

    if (lore_ptr->r_ptr->flags3 & RF3_EVIL)
        lore_ptr->flags3 |= (RF3_EVIL);

    if (lore_ptr->r_ptr->flags3 & RF3_GOOD)
        lore_ptr->flags3 |= (RF3_GOOD);

    if (lore_ptr->r_ptr->flags3 & RF3_ANIMAL)
        lore_ptr->flags3 |= (RF3_ANIMAL);

    if (lore_ptr->r_ptr->flags3 & RF3_AMBERITE)
        lore_ptr->flags3 |= (RF3_AMBERITE);

    if (lore_ptr->r_ptr->flags2 & RF2_HUMAN)
        lore_ptr->flags2 |= (RF2_HUMAN);

    if (lore_ptr->r_ptr->flags2 & RF2_QUANTUM)
        lore_ptr->flags2 |= (RF2_QUANTUM);

    if (lore_ptr->r_ptr->flags1 & RF1_FORCE_DEPTH)
        lore_ptr->flags1 |= (RF1_FORCE_DEPTH);

    if (lore_ptr->r_ptr->flags1 & RF1_FORCE_MAXHP)
        lore_ptr->flags1 |= (RF1_FORCE_MAXHP);
}

/*!
 * @brief モンスターの思い出情報を表示するメインルーチン
 * Hack -- display monster information using "hooked_roff()"
 * @param r_idx モンスターの種族ID
 * @param mode 表示オプション
 * @return なし
 * @details
 * This function should only be called with the cursor placed at the
 * left edge of the screen, on a cleared line, in which the recall is
 * to take place.  One extra blank line is left after the recall.
 */
void process_monster_lore(player_type *player_ptr, MONRACE_IDX r_idx, BIT_FLAGS mode)
{
    lore_type tmp_lore;
    lore_type *lore_ptr = initialize_lore_type(&tmp_lore, r_idx, mode);
    for (int n = 0; n < A_MAX; n++) {
        if (lore_ptr->r_ptr->reinforce_id[n] > 0)
            lore_ptr->reinforce = TRUE;
    }

    if (cheat_know || (mode & 0x01))
        lore_ptr->know_everything = TRUE;

    set_drop_flags(lore_ptr);
    set_msex_flags(lore_ptr);
    set_flags1(lore_ptr);
    set_race_flags(lore_ptr);
    display_kill_numbers(lore_ptr);
    concptr tmp = r_text + lore_ptr->r_ptr->text;
    if (tmp[0]) {
        hooked_roff(tmp);
        hooked_roff("\n");
    }

    if (r_idx == MON_KAGE) {
        hooked_roff("\n");
        return;
    }

    if (!display_where_to_appear(lore_ptr))
        return;

    display_monster_move(lore_ptr);
    display_monster_never_move(lore_ptr);
    if (lore_ptr->old) {
        hooked_roff(_("。", ".  "));
        lore_ptr->old = FALSE;
    }

    display_lore_this(player_ptr, lore_ptr);
    display_monster_aura(lore_ptr);
    if (lore_ptr->flags2 & RF2_REFLECTING)
        hooked_roff(format(_("%^sは矢の呪文を跳ね返す。", "%^s reflects bolt spells.  "), wd_he[lore_ptr->msex]));

    display_monster_collective(lore_ptr);
    lore_ptr->vn = 0;
    if (lore_ptr->flags4 & RF4_SHRIEK) {
        lore_ptr->vp[lore_ptr->vn] = _("悲鳴で助けを求める", "shriek for help");
        lore_ptr->color[lore_ptr->vn++] = TERM_L_WHITE;
    }

    display_monster_launching(player_ptr, lore_ptr);
    if (lore_ptr->a_ability_flags2 & (RF6_SPECIAL)) {
        lore_ptr->vp[lore_ptr->vn] = _("特別な行動をする", "do something");
        lore_ptr->color[lore_ptr->vn++] = TERM_VIOLET;
    }

    display_monster_sometimes(lore_ptr);
    set_breath_types(player_ptr, lore_ptr);
    display_monster_breath(lore_ptr);

    lore_ptr->vn = 0;
    set_ball_types(player_ptr, lore_ptr);
    set_particular_types(player_ptr, lore_ptr);
    set_bolt_types(player_ptr, lore_ptr);
    set_status_types(lore_ptr);
    set_teleport_types(lore_ptr);
    set_floor_types(player_ptr, lore_ptr);
    set_summon_types(lore_ptr);
    display_monster_magic_types(lore_ptr);
    if (lore_ptr->breath || lore_ptr->magic) {
        int m = lore_ptr->r_ptr->r_cast_spell;
        int n = lore_ptr->r_ptr->freq_spell;
        if (m > 100 || lore_ptr->know_everything) {
            hooked_roff(format(_("(確率:1/%d)", "; 1 time in %d"), 100 / n));
        } else if (m) {
            n = ((n + 9) / 10) * 10;
            hooked_roff(format(_("(確率:約1/%d)", "; about 1 time in %d"), 100 / n));
        }

        hooked_roff(_("。", ".  "));
    }

    if (lore_ptr->know_everything || know_armour(r_idx)) {
        hooked_roff(format(_("%^sは AC%d の防御力と", "%^s has an armor rating of %d"), wd_he[lore_ptr->msex], lore_ptr->r_ptr->ac));

        if ((lore_ptr->flags1 & RF1_FORCE_MAXHP) || (lore_ptr->r_ptr->hside == 1)) {
            u32b hp = lore_ptr->r_ptr->hdice * (lore_ptr->nightmare ? 2 : 1) * lore_ptr->r_ptr->hside;
            hooked_roff(format(_(" %d の体力がある。", " and a life rating of %d.  "), (s16b)MIN(30000, hp)));
        } else {
            hooked_roff(format(
                _(" %dd%d の体力がある。", " and a life rating of %dd%d.  "), lore_ptr->r_ptr->hdice * (lore_ptr->nightmare ? 2 : 1), lore_ptr->r_ptr->hside));
        }
    }

    lore_ptr->vn = 0;
    if (lore_ptr->flags7 & (RF7_HAS_LITE_1 | RF7_HAS_LITE_2)) {
        lore_ptr->vp[lore_ptr->vn] = _("ダンジョンを照らす", "illuminate the dungeon");
        lore_ptr->color[lore_ptr->vn++] = TERM_WHITE;
    }
    if (lore_ptr->flags7 & (RF7_HAS_DARK_1 | RF7_HAS_DARK_2)) {
        lore_ptr->vp[lore_ptr->vn] = _("ダンジョンを暗くする", "darken the dungeon");
        lore_ptr->color[lore_ptr->vn++] = TERM_L_DARK;
    }
    if (lore_ptr->flags2 & RF2_OPEN_DOOR) {
        lore_ptr->vp[lore_ptr->vn] = _("ドアを開ける", "open doors");
        lore_ptr->color[lore_ptr->vn++] = TERM_WHITE;
    }
    if (lore_ptr->flags2 & RF2_BASH_DOOR) {
        lore_ptr->vp[lore_ptr->vn] = _("ドアを打ち破る", "bash down doors");
        lore_ptr->color[lore_ptr->vn++] = TERM_WHITE;
    }
    if (lore_ptr->flags7 & RF7_CAN_FLY) {
        lore_ptr->vp[lore_ptr->vn] = _("空を飛ぶ", "fly");
        lore_ptr->color[lore_ptr->vn++] = TERM_WHITE;
    }
    if (lore_ptr->flags7 & RF7_CAN_SWIM) {
        lore_ptr->vp[lore_ptr->vn] = _("水を渡る", "swim");
        lore_ptr->color[lore_ptr->vn++] = TERM_WHITE;
    }
    if (lore_ptr->flags2 & RF2_PASS_WALL) {
        lore_ptr->vp[lore_ptr->vn] = _("壁をすり抜ける", "pass through walls");
        lore_ptr->color[lore_ptr->vn++] = TERM_WHITE;
    }
    if (lore_ptr->flags2 & RF2_KILL_WALL) {
        lore_ptr->vp[lore_ptr->vn] = _("壁を掘り進む", "bore through walls");
        lore_ptr->color[lore_ptr->vn++] = TERM_WHITE;
    }
    if (lore_ptr->flags2 & RF2_MOVE_BODY) {
        lore_ptr->vp[lore_ptr->vn] = _("弱いモンスターを押しのける", "push past weaker monsters");
        lore_ptr->color[lore_ptr->vn++] = TERM_WHITE;
    }
    if (lore_ptr->flags2 & RF2_KILL_BODY) {
        lore_ptr->vp[lore_ptr->vn] = _("弱いモンスターを倒す", "destroy weaker monsters");
        lore_ptr->color[lore_ptr->vn++] = TERM_WHITE;
    }
    if (lore_ptr->flags2 & RF2_TAKE_ITEM) {
        lore_ptr->vp[lore_ptr->vn] = _("アイテムを拾う", "pick up objects");
        lore_ptr->color[lore_ptr->vn++] = TERM_WHITE;
    }
    if (lore_ptr->flags2 & RF2_KILL_ITEM) {
        lore_ptr->vp[lore_ptr->vn] = _("アイテムを壊す", "destroy objects");
        lore_ptr->color[lore_ptr->vn++] = TERM_WHITE;
    }

    if (lore_ptr->vn > 0) {
        hooked_roff(format(_("%^sは", "%^s"), wd_he[lore_ptr->msex]));
        for (int n = 0; n < lore_ptr->vn; n++) {
#ifdef JP
            if (n != lore_ptr->vn - 1) {
                jverb(lore_ptr->vp[n], lore_ptr->jverb_buf, JVERB_AND);
                hook_c_roff(lore_ptr->color[n], lore_ptr->jverb_buf);
                hooked_roff("、");
            } else {
                hook_c_roff(lore_ptr->color[n], lore_ptr->vp[n]);
            }
#else
            if (n == 0)
                hooked_roff(" can ");
            else if (n < lore_ptr->vn - 1)
                hooked_roff(", ");
            else
                hooked_roff(" and ");

            hook_c_roff(lore_ptr->color[n], lore_ptr->vp[n]);
#endif
        }

        hooked_roff(_("ことができる。", ".  "));
    }

    if (lore_ptr->flags7 & RF7_AQUATIC) {
        hooked_roff(format(_("%^sは水中に棲んでいる。", "%^s lives in water.  "), wd_he[lore_ptr->msex]));
    }

    if (lore_ptr->flags7 & (RF7_SELF_LITE_1 | RF7_SELF_LITE_2)) {
        hooked_roff(format(_("%^sは光っている。", "%^s is shining.  "), wd_he[lore_ptr->msex]));
    }

    if (lore_ptr->flags7 & (RF7_SELF_DARK_1 | RF7_SELF_DARK_2)) {
        hook_c_roff(TERM_L_DARK, format(_("%^sは暗黒に包まれている。", "%^s is surrounded by darkness.  "), wd_he[lore_ptr->msex]));
    }

    if (lore_ptr->flags2 & RF2_INVISIBLE) {
        hooked_roff(format(_("%^sは透明で目に見えない。", "%^s is invisible.  "), wd_he[lore_ptr->msex]));
    }

    if (lore_ptr->flags2 & RF2_COLD_BLOOD) {
        hooked_roff(format(_("%^sは冷血動物である。", "%^s is cold blooded.  "), wd_he[lore_ptr->msex]));
    }

    if (lore_ptr->flags2 & RF2_EMPTY_MIND) {
        hooked_roff(format(_("%^sはテレパシーでは感知できない。", "%^s is not detected by telepathy.  "), wd_he[lore_ptr->msex]));
    } else if (lore_ptr->flags2 & RF2_WEIRD_MIND) {
        hooked_roff(format(_("%^sはまれにテレパシーで感知できる。", "%^s is rarely detected by telepathy.  "), wd_he[lore_ptr->msex]));
    }

    if (lore_ptr->flags2 & RF2_MULTIPLY) {
        hook_c_roff(TERM_L_UMBER, format(_("%^sは爆発的に増殖する。", "%^s breeds explosively.  "), wd_he[lore_ptr->msex]));
    }

    if (lore_ptr->flags2 & RF2_REGENERATE) {
        hook_c_roff(TERM_L_WHITE, format(_("%^sは素早く体力を回復する。", "%^s regenerates quickly.  "), wd_he[lore_ptr->msex]));
    }

    if (lore_ptr->flags7 & RF7_RIDING) {
        hook_c_roff(TERM_SLATE, format(_("%^sに乗ることができる。", "%^s is suitable for riding.  "), wd_he[lore_ptr->msex]));
    }

    lore_ptr->vn = 0;
    if (lore_ptr->flags3 & RF3_HURT_ROCK) {
        lore_ptr->vp[lore_ptr->vn] = _("岩を除去するもの", "rock remover");
        lore_ptr->color[lore_ptr->vn++] = TERM_UMBER;
    }
    if (lore_ptr->flags3 & RF3_HURT_LITE) {
        lore_ptr->vp[lore_ptr->vn] = _("明るい光", "bright light");
        lore_ptr->color[lore_ptr->vn++] = TERM_YELLOW;
    }
    if (lore_ptr->flags3 & RF3_HURT_FIRE) {
        lore_ptr->vp[lore_ptr->vn] = _("炎", "fire");
        lore_ptr->color[lore_ptr->vn++] = TERM_RED;
    }
    if (lore_ptr->flags3 & RF3_HURT_COLD) {
        lore_ptr->vp[lore_ptr->vn] = _("冷気", "cold");
        lore_ptr->color[lore_ptr->vn++] = TERM_L_WHITE;
    }

    if (lore_ptr->vn > 0) {
        hooked_roff(format(_("%^sには", "%^s"), wd_he[lore_ptr->msex]));

        for (int n = 0; n < lore_ptr->vn; n++) {
#ifdef JP
            if (n != 0)
                hooked_roff("や");
#else
            if (n == 0)
                hooked_roff(" is hurt by ");
            else if (n < lore_ptr->vn - 1)
                hooked_roff(", ");
            else
                hooked_roff(" and ");
#endif
            hook_c_roff(lore_ptr->color[n], lore_ptr->vp[n]);
        }

        hooked_roff(_("でダメージを与えられる。", ".  "));
    }

    lore_ptr->vn = 0;
    if (lore_ptr->flagsr & RFR_IM_ACID) {
        lore_ptr->vp[lore_ptr->vn] = _("酸", "acid");
        lore_ptr->color[lore_ptr->vn++] = TERM_GREEN;
    }
    if (lore_ptr->flagsr & RFR_IM_ELEC) {
        lore_ptr->vp[lore_ptr->vn] = _("稲妻", "lightning");
        lore_ptr->color[lore_ptr->vn++] = TERM_BLUE;
    }
    if (lore_ptr->flagsr & RFR_IM_FIRE) {
        lore_ptr->vp[lore_ptr->vn] = _("炎", "fire");
        lore_ptr->color[lore_ptr->vn++] = TERM_RED;
    }
    if (lore_ptr->flagsr & RFR_IM_COLD) {
        lore_ptr->vp[lore_ptr->vn] = _("冷気", "cold");
        lore_ptr->color[lore_ptr->vn++] = TERM_L_WHITE;
    }
    if (lore_ptr->flagsr & RFR_IM_POIS) {
        lore_ptr->vp[lore_ptr->vn] = _("毒", "poison");
        lore_ptr->color[lore_ptr->vn++] = TERM_L_GREEN;
    }

    if (lore_ptr->flagsr & RFR_RES_LITE) {
        lore_ptr->vp[lore_ptr->vn] = _("閃光", "light");
        lore_ptr->color[lore_ptr->vn++] = TERM_YELLOW;
    }
    if (lore_ptr->flagsr & RFR_RES_DARK) {
        lore_ptr->vp[lore_ptr->vn] = _("暗黒", "dark");
        lore_ptr->color[lore_ptr->vn++] = TERM_L_DARK;
    }
    if (lore_ptr->flagsr & RFR_RES_NETH) {
        lore_ptr->vp[lore_ptr->vn] = _("地獄", "nether");
        lore_ptr->color[lore_ptr->vn++] = TERM_L_DARK;
    }
    if (lore_ptr->flagsr & RFR_RES_WATE) {
        lore_ptr->vp[lore_ptr->vn] = _("水", "water");
        lore_ptr->color[lore_ptr->vn++] = TERM_BLUE;
    }
    if (lore_ptr->flagsr & RFR_RES_PLAS) {
        lore_ptr->vp[lore_ptr->vn] = _("プラズマ", "plasma");
        lore_ptr->color[lore_ptr->vn++] = TERM_L_RED;
    }
    if (lore_ptr->flagsr & RFR_RES_SHAR) {
        lore_ptr->vp[lore_ptr->vn] = _("破片", "shards");
        lore_ptr->color[lore_ptr->vn++] = TERM_L_UMBER;
    }
    if (lore_ptr->flagsr & RFR_RES_SOUN) {
        lore_ptr->vp[lore_ptr->vn] = _("轟音", "sound");
        lore_ptr->color[lore_ptr->vn++] = TERM_ORANGE;
    }
    if (lore_ptr->flagsr & RFR_RES_CHAO) {
        lore_ptr->vp[lore_ptr->vn] = _("カオス", "chaos");
        lore_ptr->color[lore_ptr->vn++] = TERM_VIOLET;
    }
    if (lore_ptr->flagsr & RFR_RES_NEXU) {
        lore_ptr->vp[lore_ptr->vn] = _("因果混乱", "nexus");
        lore_ptr->color[lore_ptr->vn++] = TERM_VIOLET;
    }
    if (lore_ptr->flagsr & RFR_RES_DISE) {
        lore_ptr->vp[lore_ptr->vn] = _("劣化", "disenchantment");
        lore_ptr->color[lore_ptr->vn++] = TERM_VIOLET;
    }
    if (lore_ptr->flagsr & RFR_RES_WALL) {
        lore_ptr->vp[lore_ptr->vn] = _("フォース", "force");
        lore_ptr->color[lore_ptr->vn++] = TERM_UMBER;
    }
    if (lore_ptr->flagsr & RFR_RES_INER) {
        lore_ptr->vp[lore_ptr->vn] = _("遅鈍", "inertia");
        lore_ptr->color[lore_ptr->vn++] = TERM_SLATE;
    }
    if (lore_ptr->flagsr & RFR_RES_TIME) {
        lore_ptr->vp[lore_ptr->vn] = _("時間逆転", "time");
        lore_ptr->color[lore_ptr->vn++] = TERM_L_BLUE;
    }
    if (lore_ptr->flagsr & RFR_RES_GRAV) {
        lore_ptr->vp[lore_ptr->vn] = _("重力", "gravity");
        lore_ptr->color[lore_ptr->vn++] = TERM_SLATE;
    }
    if (lore_ptr->flagsr & RFR_RES_ALL) {
        lore_ptr->vp[lore_ptr->vn] = _("あらゆる攻撃", "all");
        lore_ptr->color[lore_ptr->vn++] = TERM_YELLOW;
    }
    if ((lore_ptr->flagsr & RFR_RES_TELE) && !(lore_ptr->r_ptr->flags1 & RF1_UNIQUE)) {
        lore_ptr->vp[lore_ptr->vn] = _("テレポート", "teleportation");
        lore_ptr->color[lore_ptr->vn++] = TERM_ORANGE;
    }

    if (lore_ptr->vn > 0) {
        hooked_roff(format(_("%^sは", "%^s"), wd_he[lore_ptr->msex]));
        for (int n = 0; n < lore_ptr->vn; n++) {
#ifdef JP
            if (n != 0)
                hooked_roff("と");
#else
            if (n == 0)
                hooked_roff(" resists ");
            else if (n < lore_ptr->vn - 1)
                hooked_roff(", ");
            else
                hooked_roff(" and ");
#endif
            hook_c_roff(lore_ptr->color[n], lore_ptr->vp[n]);
        }

        hooked_roff(_("の耐性を持っている。", ".  "));
    }

    if ((lore_ptr->r_ptr->r_xtra1 & MR1_SINKA) || lore_ptr->know_everything) {
        if (lore_ptr->r_ptr->next_r_idx) {
            hooked_roff(format(_("%^sは経験を積むと、", "%^s will evolve into "), wd_he[lore_ptr->msex]));
            hook_c_roff(TERM_YELLOW, format("%s", r_name + r_info[lore_ptr->r_ptr->next_r_idx].name));

            hooked_roff(_(format("に進化する。"), format(" when %s gets enough experience.  ", wd_he[lore_ptr->msex])));
        } else if (!(lore_ptr->r_ptr->flags1 & RF1_UNIQUE)) {
            hooked_roff(format(_("%sは進化しない。", "%s won't evolve.  "), wd_he[lore_ptr->msex]));
        }
    }

    lore_ptr->vn = 0;
    if (lore_ptr->flags3 & RF3_NO_STUN) {
        lore_ptr->vp[lore_ptr->vn] = _("朦朧としない", "stunned");
        lore_ptr->color[lore_ptr->vn++] = TERM_ORANGE;
    }
    if (lore_ptr->flags3 & RF3_NO_FEAR) {
        lore_ptr->vp[lore_ptr->vn] = _("恐怖を感じない", "frightened");
        lore_ptr->color[lore_ptr->vn++] = TERM_SLATE;
    }
    if (lore_ptr->flags3 & RF3_NO_CONF) {
        lore_ptr->vp[lore_ptr->vn] = _("混乱しない", "confused");
        lore_ptr->color[lore_ptr->vn++] = TERM_L_UMBER;
    }
    if (lore_ptr->flags3 & RF3_NO_SLEEP) {
        lore_ptr->vp[lore_ptr->vn] = _("眠らされない", "slept");
        lore_ptr->color[lore_ptr->vn++] = TERM_BLUE;
    }
    if ((lore_ptr->flagsr & RFR_RES_TELE) && (lore_ptr->r_ptr->flags1 & RF1_UNIQUE)) {
        lore_ptr->vp[lore_ptr->vn] = _("テレポートされない", "teleported");
        lore_ptr->color[lore_ptr->vn++] = TERM_ORANGE;
    }

    if (lore_ptr->vn > 0) {
        hooked_roff(format(_("%^sは", "%^s"), wd_he[lore_ptr->msex]));
        for (int n = 0; n < lore_ptr->vn; n++) {
#ifdef JP
            if (n != 0)
                hooked_roff("し、");
#else
            if (n == 0)
                hooked_roff(" cannot be ");
            else if (n < lore_ptr->vn - 1)
                hooked_roff(", ");
            else
                hooked_roff(" or ");
#endif
            hook_c_roff(lore_ptr->color[n], lore_ptr->vp[n]);
        }

        hooked_roff(_("。", ".  "));
    }

    if ((((int)lore_ptr->r_ptr->r_wake * (int)lore_ptr->r_ptr->r_wake) > lore_ptr->r_ptr->sleep) || (lore_ptr->r_ptr->r_ignore == MAX_UCHAR)
        || (lore_ptr->r_ptr->sleep == 0 && lore_ptr->r_ptr->r_tkills >= 10) || lore_ptr->know_everything) {
        concptr act;
        if (lore_ptr->r_ptr->sleep > 200) {
            act = _("を無視しがちであるが", "prefers to ignore");
        } else if (lore_ptr->r_ptr->sleep > 95) {
            act = _("に対してほとんど注意を払わないが", "pays very little attention to");
        } else if (lore_ptr->r_ptr->sleep > 75) {
            act = _("に対してあまり注意を払わないが", "pays little attention to");
        } else if (lore_ptr->r_ptr->sleep > 45) {
            act = _("を見過ごしがちであるが", "tends to overlook");
        } else if (lore_ptr->r_ptr->sleep > 25) {
            act = _("をほんの少しは見ており", "takes quite a while to see");
        } else if (lore_ptr->r_ptr->sleep > 10) {
            act = _("をしばらくは見ており", "takes a while to see");
        } else if (lore_ptr->r_ptr->sleep > 5) {
            act = _("を幾分注意深く見ており", "is fairly observant of");
        } else if (lore_ptr->r_ptr->sleep > 3) {
            act = _("を注意深く見ており", "is observant of");
        } else if (lore_ptr->r_ptr->sleep > 1) {
            act = _("をかなり注意深く見ており", "is very observant of");
        } else if (lore_ptr->r_ptr->sleep > 0) {
            act = _("を警戒しており", "is vigilant for");
        } else {
            act = _("をかなり警戒しており", "is ever vigilant for");
        }

        hooked_roff(_(format("%^sは侵入者%s、 %d フィート先から侵入者に気付くことがある。", wd_he[lore_ptr->msex], act, 10 * lore_ptr->r_ptr->aaf),
            format("%^s %s intruders, which %s may notice from %d feet.  ", wd_he[lore_ptr->msex], act, wd_he[lore_ptr->msex], 10 * lore_ptr->r_ptr->aaf)));
    }

    if (lore_ptr->drop_gold || lore_ptr->drop_item) {
        hooked_roff(format(_("%^sは", "%^s may carry"), wd_he[lore_ptr->msex]));
#ifdef JP
#else
        lore_ptr->sin = FALSE;
#endif

        int n = MAX(lore_ptr->drop_gold, lore_ptr->drop_item);
        if (n == 1) {
            hooked_roff(_("一つの", " a"));
#ifdef JP
#else
            lore_ptr->sin = TRUE;
#endif
        } else if (n == 2) {
            hooked_roff(_("一つか二つの", " one or two"));
        } else {
            hooked_roff(format(_(" %d 個までの", " up to %d"), n));
        }

        concptr p;
        if (lore_ptr->flags1 & RF1_DROP_GREAT) {
            p = _("特別な", " exceptional");
        } else if (lore_ptr->flags1 & RF1_DROP_GOOD) {
            p = _("上質な", " good");
#ifdef JP
#else
            lore_ptr->sin = FALSE;
#endif
        } else {
            p = NULL;
        }

        if (lore_ptr->drop_item) {
#ifdef JP
#else
            if (lore_ptr->sin)
                hooked_roff("n");

            lore_ptr->sin = FALSE;
#endif
            if (p)
                hooked_roff(p);
            hooked_roff(_("アイテム", " object"));
#ifdef JP
#else
            if (n != 1)
                hooked_roff("s");
#endif
            p = _("や", " or");
        }

        if (lore_ptr->drop_gold) {
#ifdef JP
#else
            if (!p)
                lore_ptr->sin = FALSE;

            if (lore_ptr->sin)
                hooked_roff("n");

            lore_ptr->sin = FALSE;
#endif
            if (p)
                hooked_roff(p);
            hooked_roff(_("財宝", " treasure"));
#ifdef JP
#else
            if (n != 1)
                hooked_roff("s");
#endif
        }

        hooked_roff(_("を持っていることがある。", ".  "));
    }

    const int max_attack_numbers = 4;
    int count = 0;
    for (int m = 0; m < max_attack_numbers; m++) {
        if (!lore_ptr->r_ptr->blow[m].method)
            continue;
        if (lore_ptr->r_ptr->blow[m].method == RBM_SHOOT)
            continue;

        if (lore_ptr->r_ptr->r_blows[m] || lore_ptr->know_everything)
            count++;
    }

    int attack_numbers = 0;
    for (int m = 0; m < max_attack_numbers; m++) {
        if (!lore_ptr->r_ptr->blow[m].method)
            continue;
        if (lore_ptr->r_ptr->blow[m].method == RBM_SHOOT)
            continue;
        if (!lore_ptr->r_ptr->r_blows[m] && !lore_ptr->know_everything)
            continue;

        rbm_type method = lore_ptr->r_ptr->blow[m].method;
        int effect = lore_ptr->r_ptr->blow[m].effect;
        int d1 = lore_ptr->r_ptr->blow[m].d_dice;
        int d2 = lore_ptr->r_ptr->blow[m].d_side;

        concptr p = NULL;
        switch (method) {
        case RBM_HIT:
            p = _("殴る", "hit");
            break;
        case RBM_TOUCH:
            p = _("触る", "touch");
            break;
        case RBM_PUNCH:
            p = _("パンチする", "punch");
            break;
        case RBM_KICK:
            p = _("蹴る", "kick");
            break;
        case RBM_CLAW:
            p = _("ひっかく", "claw");
            break;
        case RBM_BITE:
            p = _("噛む", "bite");
            break;
        case RBM_STING:
            p = _("刺す", "sting");
            break;
        case RBM_SLASH:
            p = _("斬る", "slash");
            break;
        case RBM_BUTT:
            p = _("角で突く", "butt");
            break;
        case RBM_CRUSH:
            p = _("体当たりする", "crush");
            break;
        case RBM_ENGULF:
            p = _("飲み込む", "engulf");
            break;
        case RBM_CHARGE:
            p = _("請求書をよこす", "charge");
            break;
        case RBM_CRAWL:
            p = _("体の上を這い回る", "crawl on you");
            break;
        case RBM_DROOL:
            p = _("よだれをたらす", "drool on you");
            break;
        case RBM_SPIT:
            p = _("つばを吐く", "spit");
            break;
        case RBM_EXPLODE:
            p = _("爆発する", "explode");
            break;
        case RBM_GAZE:
            p = _("にらむ", "gaze");
            break;
        case RBM_WAIL:
            p = _("泣き叫ぶ", "wail");
            break;
        case RBM_SPORE:
            p = _("胞子を飛ばす", "release spores");
            break;
        case RBM_XXX4:
            break;
        case RBM_BEG:
            p = _("金をせがむ", "beg");
            break;
        case RBM_INSULT:
            p = _("侮辱する", "insult");
            break;
        case RBM_MOAN:
            p = _("うめく", "moan");
            break;
        case RBM_SHOW:
            p = _("歌う", "sing");
            break;
        }

        concptr q = NULL;
        switch (effect) {
        case RBE_SUPERHURT:
            q = _("強力に攻撃する", "slaughter");
            break;
        case RBE_HURT:
            q = _("攻撃する", "attack");
            break;
        case RBE_POISON:
            q = _("毒をくらわす", "poison");
            break;
        case RBE_UN_BONUS:
            q = _("劣化させる", "disenchant");
            break;
        case RBE_UN_POWER:
            q = _("充填魔力を吸収する", "drain charges");
            break;
        case RBE_EAT_GOLD:
            q = _("金を盗む", "steal gold");
            break;
        case RBE_EAT_ITEM:
            q = _("アイテムを盗む", "steal items");
            break;
        case RBE_EAT_FOOD:
            q = _("あなたの食料を食べる", "eat your food");
            break;
        case RBE_EAT_LITE:
            q = _("明かりを吸収する", "absorb light");
            break;
        case RBE_ACID:
            q = _("酸を飛ばす", "shoot acid");
            break;
        case RBE_ELEC:
            q = _("感電させる", "electrocute");
            break;
        case RBE_FIRE:
            q = _("燃やす", "burn");
            break;
        case RBE_COLD:
            q = _("凍らせる", "freeze");
            break;
        case RBE_BLIND:
            q = _("盲目にする", "blind");
            break;
        case RBE_CONFUSE:
            q = _("混乱させる", "confuse");
            break;
        case RBE_TERRIFY:
            q = _("恐怖させる", "terrify");
            break;
        case RBE_PARALYZE:
            q = _("麻痺させる", "paralyze");
            break;
        case RBE_LOSE_STR:
            q = _("腕力を減少させる", "reduce strength");
            break;
        case RBE_LOSE_INT:
            q = _("知能を減少させる", "reduce intelligence");
            break;
        case RBE_LOSE_WIS:
            q = _("賢さを減少させる", "reduce wisdom");
            break;
        case RBE_LOSE_DEX:
            q = _("器用さを減少させる", "reduce dexterity");
            break;
        case RBE_LOSE_CON:
            q = _("耐久力を減少させる", "reduce constitution");
            break;
        case RBE_LOSE_CHR:
            q = _("魅力を減少させる", "reduce charisma");
            break;
        case RBE_LOSE_ALL:
            q = _("全ステータスを減少させる", "reduce all stats");
            break;
        case RBE_SHATTER:
            q = _("粉砕する", "shatter");
            break;
        case RBE_EXP_10:
            q = _("経験値を減少(10d6+)させる", "lower experience (by 10d6+)");
            break;
        case RBE_EXP_20:
            q = _("経験値を減少(20d6+)させる", "lower experience (by 20d6+)");
            break;
        case RBE_EXP_40:
            q = _("経験値を減少(40d6+)させる", "lower experience (by 40d6+)");
            break;
        case RBE_EXP_80:
            q = _("経験値を減少(80d6+)させる", "lower experience (by 80d6+)");
            break;
        case RBE_DISEASE:
            q = _("病気にする", "disease");
            break;
        case RBE_TIME:
            q = _("時間を逆戻りさせる", "time");
            break;
        case RBE_DR_LIFE:
            q = _("生命力を吸収する", "drain life");
            break;
        case RBE_DR_MANA:
            q = _("魔力を奪う", "drain mana force");
            break;
        case RBE_INERTIA:
            q = _("減速させる", "slow");
            break;
        case RBE_STUN:
            q = _("朦朧とさせる", "stun");
            break;
        }

#ifdef JP
        if (attack_numbers == 0) {
            hooked_roff(format("%^sは", wd_he[lore_ptr->msex]));
        }

        if (d1 && d2 && (lore_ptr->know_everything || know_damage(r_idx, m))) {
            hooked_roff(format(" %dd%d ", d1, d2));
            hooked_roff("のダメージで");
        }

        if (!p)
            p = "何か奇妙なことをする";

        /* XXしてYYし/XXしてYYする/XXし/XXする */
        if (q != NULL)
            jverb(p, lore_ptr->jverb_buf, JVERB_TO);
        else if (attack_numbers != count - 1)
            jverb(p, lore_ptr->jverb_buf, JVERB_AND);
        else
            strcpy(lore_ptr->jverb_buf, p);

        hooked_roff(lore_ptr->jverb_buf);
        if (q) {
            if (attack_numbers != count - 1)
                jverb(q, lore_ptr->jverb_buf, JVERB_AND);
            else
                strcpy(lore_ptr->jverb_buf, q);
            hooked_roff(lore_ptr->jverb_buf);
        }

        if (attack_numbers != count - 1)
            hooked_roff("、");
#else
        if (attack_numbers == 0) {
            hooked_roff(format("%^s can ", wd_he[lore_ptr->msex]));
        } else if (attack_numbers < count - 1) {
            hooked_roff(", ");
        } else {
            hooked_roff(", and ");
        }

        if (!p)
            p = "do something weird";
        hooked_roff(p);
        if (q) {
            hooked_roff(" to ");
            hooked_roff(q);
            if (d1 && d2 && (lore_ptr->know_everything || know_damage(r_idx, m))) {
                hooked_roff(" with damage");
                hooked_roff(format(" %dd%d", d1, d2));
            }
        }
#endif

        attack_numbers++;
    }

    if (attack_numbers > 0) {
        hooked_roff(_("。", ".  "));
    } else if (lore_ptr->flags1 & RF1_NEVER_BLOW) {
        hooked_roff(format(_("%^sは物理的な攻撃方法を持たない。", "%^s has no physical attacks.  "), wd_he[lore_ptr->msex]));
    } else {
        hooked_roff(format(_("%s攻撃については何も知らない。", "Nothing is known about %s attack.  "), wd_his[lore_ptr->msex]));
    }

    bool is_kingpin = (lore_ptr->flags1 & RF1_QUESTOR) != 0;
    is_kingpin &= lore_ptr->r_ptr->r_sights > 0;
    is_kingpin &= lore_ptr->r_ptr->max_num > 0;
    is_kingpin &= (r_idx == MON_OBERON) || (r_idx == MON_SERPENT);
    if (is_kingpin) {
        hook_c_roff(TERM_VIOLET, _("あなたはこのモンスターを殺したいという強い欲望を感じている...", "You feel an intense desire to kill this monster...  "));
    } else if (lore_ptr->flags7 & RF7_GUARDIAN) {
        hook_c_roff(TERM_L_RED, _("このモンスターはダンジョンの主である。", "This monster is the master of a dungeon."));
    }

    hooked_roff("\n");
}
