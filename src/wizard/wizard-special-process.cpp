﻿/*!
 * @brief ウィザードモードの処理(特別処理中心) / Wizard commands
 * @date 2014/09/07
 * @author
 * Copyright (c) 1997 Ben Harrison, and others<br>
 * This software may be copied and distributed for educational, research,
 * and not for profit purposes provided that this copyright and statement
 * are included in all such copies.  Other copyrights may also apply.<br>
 * 2014 Deskull rearranged comment for Doxygen.<br>
 */

#include "wizard/wizard-special-process.h"
#include "artifact/fixed-art-generator.h"
#include "birth/inventory-initializer.h"
#include "cmd-io/cmd-dump.h"
#include "cmd-io/cmd-help.h"
#include "cmd-io/cmd-save.h"
#include "cmd-visual/cmd-draw.h"
#include "core/asking-player.h"
#include "core/player-redraw-types.h"
#include "core/player-update-types.h"
#include "core/stuff-handler.h"
#include "core/window-redrawer.h"
#include "dungeon/dungeon.h"
#include "dungeon/quest.h"
#include "flavor/object-flavor.h"
#include "floor/floor-leaver.h"
#include "floor/floor-mode-changer.h"
#include "floor/floor-object.h"
#include "game-option/birth-options.h"
#include "game-option/option-types-table.h"
#include "game-option/play-record-options.h"
#include "game-option/special-options.h"
#include "grid/feature.h"
#include "grid/grid.h"
#include "info-reader/fixed-map-parser.h"
#include "inventory/inventory-object.h"
#include "inventory/inventory-slot-types.h"
#include "io/files-util.h"
#include "io/input-key-requester.h"
#include "io/write-diary.h"
#include "market/arena.h"
#include "monster-floor/monster-remover.h"
#include "monster-floor/monster-summon.h"
#include "monster/monster-describer.h"
#include "monster/monster-description-types.h"
#include "monster/monster-info.h"
#include "monster/monster-status.h"
#include "monster/smart-learn-types.h"
#include "mutation/mutation-investor-remover.h"
#include "object-enchant/apply-magic.h"
#include "object-enchant/item-apply-magic.h"
#include "object-enchant/trc-types.h"
#include "object-enchant/trg-types.h"
#include "object/object-kind.h"
#include "perception/object-perception.h"
#include "player-info/class-info.h"
#include "player-info/race-info.h"
#include "player-info/race-types.h"
#include "player-info/self-info.h"
#include "player-status/player-energy.h"
#include "player/digestion-processor.h"
#include "player/patron.h"
#include "player/player-skill.h"
#include "player/player-status-table.h"
#include "player/player-status.h"
#include "player/race-info-table.h"
#include "spell-kind/spells-detection.h"
#include "spell-kind/spells-sight.h"
#include "spell-kind/spells-teleport.h"
#include "spell-kind/spells-world.h"
#include "spell/spells-object.h"
#include "spell/spells-status.h"
#include "spell/spells-summon.h"
#include "status/experience.h"
#include "system/angband-version.h"
#include "system/artifact-type-definition.h"
#include "system/floor-type-definition.h"
#include "system/grid-type-definition.h"
#include "system/monster-type-definition.h"
#include "system/player-type-definition.h"
#include "target/grid-selector.h"
#include "term/screen-processor.h"
#include "util/angband-files.h"
#include "util/bit-flags-calculator.h"
#include "util/enum-converter.h"
#include "util/int-char-converter.h"
#include "view/display-messages.h"
#include "wizard/tval-descriptions-table.h"
#include "wizard/wizard-spells.h"
#include "wizard/wizard-spoiler.h"
#include "world/world.h"
#include <tuple>

#define NUM_O_SET 8
#define NUM_O_BIT 32

/*!
 * @brief プレイヤーを完全回復する /
 * Cure everything instantly
 */
void wiz_cure_all(player_type *player_ptr)
{
    (void)life_stream(player_ptr, false, false);
    (void)restore_mana(player_ptr, true);
    (void)set_food(player_ptr, PY_FOOD_MAX - 1);
}

static std::optional<KIND_OBJECT_IDX> wiz_select_tval()
{
    KIND_OBJECT_IDX list;
    char ch;
    for (list = 0; (list < 80) && (tvals[list].tval > TV_NONE); list++) {
        auto row = 2 + (list % 20);
        auto col = _(32, 24) * (list / 20);
        ch = listsym[list];
        prt(format("[%c] %s", ch, tvals[list].desc), row, col);
    }

    auto max_num = list;
    if (!get_com(_("アイテム種別を選んで下さい", "Get what type of object? "), &ch, false)) {
        return std::nullopt;
    }

    KIND_OBJECT_IDX selection;
    for (selection = 0; selection < max_num; selection++) {
        if (listsym[selection] == ch) {
            break;
        }
    }

    if ((selection < 0) || (selection >= max_num)) {
        return std::nullopt;
    }

    return selection;
}

static KIND_OBJECT_IDX wiz_select_sval(const tval_type tval, concptr tval_description)
{
    auto num = 0;
    KIND_OBJECT_IDX choice[80]{};
    char buf[160]{};
    char ch;
    for (const auto &k_ref : k_info) {
        if (num >= 80) {
            break;
        }

        if (k_ref.idx == 0 || k_ref.tval != tval) {
            continue;
        }

        auto row = 2 + (num % 20);
        auto col = _(30, 32) * (num / 20);
        ch = listsym[num];
        strcpy(buf, "                    ");
        strip_name(buf, k_ref.idx);
        prt(format("[%c] %s", ch, buf), row, col);
        choice[num++] = k_ref.idx;
    }

    auto max_num = num;
    if (!get_com(format(_("%s群の具体的なアイテムを選んで下さい", "What Kind of %s? "), tval_description), &ch, false)) {
        return 0;
    }

    KIND_OBJECT_IDX selection;
    for (selection = 0; selection < max_num; selection++) {
        if (listsym[selection] == ch) {
            break;
        }
    }

    if ((selection < 0) || (selection >= max_num)) {
        return 0;
    }

    return choice[selection];
}

/*!
 * @brief ベースアイテムのウィザード生成のために大項目IDと小項目IDを取得する /
 * Specify tval and sval (type and subtype of object) originally
 * @return ベースアイテムID
 * @details
 * by RAK, heavily modified by -Bernd-
 * This function returns the k_idx of an object type, or zero if failed
 * List up to 50 choices in three columns
 */
static KIND_OBJECT_IDX wiz_create_itemtype()
{
    term_clear();
    auto selection = wiz_select_tval();
    if (!selection.has_value()) {
        return 0;
    }

    tval_type tval = i2enum<tval_type>(tvals[selection.value()].tval);
    concptr tval_description = tvals[selection.value()].desc;
    term_clear();
    return wiz_select_sval(tval, tval_description);
}

/*!
 * @brief 任意のベースアイテム生成のメインルーチン /
 * Wizard routine for creating objects		-RAK-
 * @details
 * Heavily modified to allow magification and artifactification  -Bernd-
 *
 * Note that wizards cannot create objects on top of other objects.
 *
 * Hack -- this routine always makes a "dungeon object", and applies
 * magic to it, and attempts to decline cursed items.
 */
void wiz_create_item(player_type *player_ptr)
{
    screen_save();
    OBJECT_IDX k_idx = wiz_create_itemtype();
    screen_load();
    if (!k_idx)
        return;

    if (k_info[k_idx].gen_flags.has(TRG::INSTA_ART)) {
        for (const auto &a_ref : a_info) {
            if ((a_ref.idx == 0) || (a_ref.tval != k_info[k_idx].tval) || (a_ref.sval != k_info[k_idx].sval))
                continue;

            (void)create_named_art(player_ptr, a_ref.idx, player_ptr->y, player_ptr->x);
            msg_print("Allocated(INSTA_ART).");
            return;
        }
    }

    object_type forge;
    object_type *q_ptr;
    q_ptr = &forge;
    q_ptr->prep(k_idx);
    apply_magic_to_object(player_ptr, q_ptr, player_ptr->current_floor_ptr->dun_level, AM_NO_FIXED_ART);
    (void)drop_near(player_ptr, q_ptr, -1, player_ptr->y, player_ptr->x);
    msg_print("Allocated.");
}

/*!
 * @brief 指定されたIDの固定アーティファクトを生成する / Create the artifact of the specified number
 * @param player_ptr プレイヤーへの参照ポインタ
 */
void wiz_create_named_art(player_type *player_ptr, ARTIFACT_IDX a_idx)
{
    if (a_idx <= 0) {
        int val;
        if (!get_value("ArtifactID", 1, a_info.size() - 1, &val)) {
            return;
        }
        a_idx = static_cast<ARTIFACT_IDX>(val);
    }
    (void)create_named_art(player_ptr, a_idx, player_ptr->y, player_ptr->x);
    msg_print("Allocated.");
}

/*!
 * @brief プレイヤーの現能力値を調整する / Change various "permanent" player variables.
 * @param player_ptr プレイヤーへの参照ポインタ
 */
void wiz_change_status(player_type *player_ptr)
{
    int tmp_int;
    char tmp_val[160];
    char ppp[80];
    for (int i = 0; i < A_MAX; i++) {
        sprintf(ppp, "%s (3-%d): ", stat_names[i], player_ptr->stat_max_max[i]);
        sprintf(tmp_val, "%d", player_ptr->stat_max[i]);
        if (!get_string(ppp, tmp_val, 3))
            return;

        tmp_int = atoi(tmp_val);
        if (tmp_int > player_ptr->stat_max_max[i])
            tmp_int = player_ptr->stat_max_max[i];
        else if (tmp_int < 3)
            tmp_int = 3;

        player_ptr->stat_cur[i] = player_ptr->stat_max[i] = (BASE_STATUS)tmp_int;
    }

    sprintf(tmp_val, "%d", WEAPON_EXP_MASTER);
    if (!get_string(_("熟練度: ", "Proficiency: "), tmp_val, 4))
        return;

    int16_t tmp_s16b = (int16_t)atoi(tmp_val);
    if (tmp_s16b < WEAPON_EXP_UNSKILLED)
        tmp_s16b = WEAPON_EXP_UNSKILLED;

    if (tmp_s16b > WEAPON_EXP_MASTER)
        tmp_s16b = WEAPON_EXP_MASTER;

    for (int j = 0; j <= TV_WEAPON_END - TV_WEAPON_BEGIN; j++) {
        for (int i = 0; i < 64; i++) {
            player_ptr->weapon_exp[j][i] = tmp_s16b;
            if (player_ptr->weapon_exp[j][i] > s_info[player_ptr->pclass].w_max[j][i])
                player_ptr->weapon_exp[j][i] = s_info[player_ptr->pclass].w_max[j][i];
        }
    }

    for (int j = 0; j < 10; j++) {
        player_ptr->skill_exp[j] = tmp_s16b;
        if (player_ptr->skill_exp[j] > s_info[player_ptr->pclass].s_max[j])
            player_ptr->skill_exp[j] = s_info[player_ptr->pclass].s_max[j];
    }

    int k;
    for (k = 0; k < 32; k++)
        player_ptr->spell_exp[k] = (tmp_s16b > SPELL_EXP_MASTER ? SPELL_EXP_MASTER : tmp_s16b);

    for (; k < 64; k++)
        player_ptr->spell_exp[k] = (tmp_s16b > SPELL_EXP_EXPERT ? SPELL_EXP_EXPERT : tmp_s16b);

    sprintf(tmp_val, "%ld", (long)(player_ptr->au));
    if (!get_string("Gold: ", tmp_val, 9))
        return;

    long tmp_long = atol(tmp_val);
    if (tmp_long < 0)
        tmp_long = 0L;

    player_ptr->au = tmp_long;
    sprintf(tmp_val, "%ld", (long)(player_ptr->max_exp));
    if (!get_string("Experience: ", tmp_val, 9))
        return;

    tmp_long = atol(tmp_val);
    if (tmp_long < 0)
        tmp_long = 0L;

    if (player_ptr->prace == player_race_type::ANDROID)
        return;

    player_ptr->max_exp = tmp_long;
    player_ptr->exp = tmp_long;
    check_experience(player_ptr);
    do_cmd_redraw(player_ptr);
}

/*!
 * @brief 指定された地点の地形IDを変更する /
 * Create desired feature
 * @param creaturer_ptr プレイヤーへの参照ポインタ
 */
void wiz_create_feature(player_type *player_ptr)
{
    POSITION y, x;
    if (!tgt_pt(player_ptr, &x, &y))
        return;

    grid_type *g_ptr;
    g_ptr = &player_ptr->current_floor_ptr->grid_array[y][x];
    static int prev_feat = 0;
    char tmp_val[160];
    sprintf(tmp_val, "%d", prev_feat);

    if (!get_string(_("地形: ", "Feature: "), tmp_val, 3))
        return;

    FEAT_IDX tmp_feat = (FEAT_IDX)atoi(tmp_val);
    if (tmp_feat < 0)
        tmp_feat = 0;
    else if (tmp_feat >= static_cast<FEAT_IDX>(f_info.size()))
        tmp_feat = static_cast<FEAT_IDX>(f_info.size()) - 1;

    static int prev_mimic = 0;
    sprintf(tmp_val, "%d", prev_mimic);

    if (!get_string(_("地形 (mimic): ", "Feature (mimic): "), tmp_val, 3))
        return;

    FEAT_IDX tmp_mimic = (FEAT_IDX)atoi(tmp_val);
    if (tmp_mimic < 0)
        tmp_mimic = 0;
    else if (tmp_mimic >= static_cast<FEAT_IDX>(f_info.size()))
        tmp_mimic = static_cast<FEAT_IDX>(f_info.size()) - 1;

    cave_set_feat(player_ptr, y, x, tmp_feat);
    g_ptr->mimic = (int16_t)tmp_mimic;
    feature_type *f_ptr;
    f_ptr = &f_info[g_ptr->get_feat_mimic()];

    if (f_ptr->flags.has(FF::RUNE_PROTECTION) || f_ptr->flags.has(FF::RUNE_EXPLOSION))
        g_ptr->info |= CAVE_OBJECT;
    else if (f_ptr->flags.has(FF::MIRROR))
        g_ptr->info |= CAVE_GLOW | CAVE_OBJECT;

    note_spot(player_ptr, y, x);
    lite_spot(player_ptr, y, x);
    player_ptr->update |= PU_FLOW;
    prev_feat = tmp_feat;
    prev_mimic = tmp_mimic;
}

/*
 * @brief 選択したダンジョンの任意フロアを選択する
 * @param player_ptr プレイヤーへの参照ポインタ
 * @param dungeon_type ダンジョン番号
 * @return フロアを選択したらtrue、キャンセルならfalse
 * @details 0を指定すると地上に飛ぶが、元いた場所にしか飛ばない
 * @todo 可能ならダンジョンの入口 (例：ルルイエなら大洋の真ん中)へ飛べるようにしたい
 */
static bool select_debugging_floor(player_type *player_ptr, int dungeon_type)
{
    auto max_depth = d_info[dungeon_type].maxdepth;
    if ((max_depth == 0) || (dungeon_type > static_cast<int>(d_info.size()))) {
        dungeon_type = DUNGEON_ANGBAND;
    }

    auto min_depth = (int)d_info[dungeon_type].mindepth;
    while (true) {
        char ppp[80];
        char tmp_val[160];
        sprintf(ppp, "Jump to level (0, %d-%d): ", min_depth, max_depth);
        sprintf(tmp_val, "%d", (int)player_ptr->current_floor_ptr->dun_level);
        if (!get_string(ppp, tmp_val, 10)) {
            return false;
        }

        auto tmp_command_arg = (COMMAND_ARG)atoi(tmp_val);
        if (tmp_command_arg == 0) {
            command_arg = tmp_command_arg;
            break;
        }

        auto is_valid_floor = tmp_command_arg > 0;
        is_valid_floor &= tmp_command_arg >= min_depth;
        is_valid_floor &= tmp_command_arg <= max_depth;
        if (is_valid_floor) {
            command_arg = tmp_command_arg;
            break;
        }

        msg_print("Invalid floor. Please re-input.");
        continue;
    }

    return true;
}

/*!
 * @brief デバッグ帰還のダンジョンを選ぶ
 * @param player_ptr プレイヤーへの参照ポインタ
 * @details 範囲外の値が選択されたら再入力を促す
 */
static bool select_debugging_dungeon(player_type *player_ptr, DUNGEON_IDX *dungeon_type)
{
    if (command_arg > 0) {
        return true;
    }

    while (true) {
        char ppp[80];
        char tmp_val[160];
        sprintf(ppp, "Jump which dungeon : ");
        sprintf(tmp_val, "%d", player_ptr->dungeon_idx);
        if (!get_string(ppp, tmp_val, 2)) {
            return false;
        }

        *dungeon_type = (DUNGEON_IDX)atoi(tmp_val);
        if ((*dungeon_type < DUNGEON_ANGBAND) || (*dungeon_type > DUNGEON_MAX)) {
            msg_print("Invalid dungeon. Please re-input.");
            continue;
        }

        return true;
    }
}

/*!
 * @brief 任意のダンジョン及び階層に飛ぶtための選択処理
 * Go to any level
 */
void wiz_jump_to_dungeon(player_type *player_ptr)
{
    DUNGEON_IDX dungeon_type = 1;
    if (!select_debugging_dungeon(player_ptr, &dungeon_type)) {
        return;
    }

    if (!select_debugging_floor(player_ptr, dungeon_type)) {
        return;
    }

    if (command_arg < d_info[dungeon_type].mindepth)
        command_arg = 0;

    if (command_arg > d_info[dungeon_type].maxdepth)
        command_arg = (COMMAND_ARG)d_info[dungeon_type].maxdepth;

    msg_format("You jump to dungeon level %d.", command_arg);
    if (autosave_l)
        do_cmd_save_game(player_ptr, true);

    jump_floor(player_ptr, dungeon_type, command_arg);
}

/*!
 * @brief 全ベースアイテムを鑑定済みにする /
 * Become aware of a lot of objects
 * @param player_ptr プレイヤーへの参照ポインタ
 */
void wiz_learn_items_all(player_type *player_ptr)
{
    object_type forge;
    object_type *q_ptr;
    for (const auto &k_ref : k_info) {
        if (k_ref.idx > 0 && k_ref.level <= command_arg) {
            q_ptr = &forge;
            q_ptr->prep(k_ref.idx);
            object_aware(player_ptr, q_ptr);
        }
    }
}

/*!
 * @brief プレイヤーの種族を変更する
 */
void wiz_reset_race(player_type *player_ptr)
{
    char ppp[80];
    sprintf(ppp, "Race (0-%d): ", MAX_RACES - 1);

    char tmp_val[160];
    sprintf(tmp_val, "%d", enum2i(player_ptr->prace));

    if (!get_string(ppp, tmp_val, 2))
        return;

    int tmp_int = atoi(tmp_val);
    if (tmp_int < 0 || tmp_int >= MAX_RACES)
        return;

    player_ptr->prace = i2enum<player_race_type>(tmp_int);
    rp_ptr = &race_info[enum2i(player_ptr->prace)];

    player_ptr->window_flags |= PW_PLAYER;
    player_ptr->update |= PU_BONUS | PU_HP | PU_MANA | PU_SPELLS;
    player_ptr->redraw |= PR_BASIC | PR_HP | PR_MANA | PR_STATS;
    handle_stuff(player_ptr);
}

/*!
 * @brief プレイヤーの職業を変更する
 * @todo 魔法領域の再選択などがまだ不完全、要実装。
 */
void wiz_reset_class(player_type *player_ptr)
{
    char ppp[80];
    sprintf(ppp, "Class (0-%d): ", MAX_CLASS - 1);

    char tmp_val[160];
    sprintf(tmp_val, "%d", player_ptr->pclass);

    if (!get_string(ppp, tmp_val, 2))
        return;

    int tmp_int = atoi(tmp_val);
    if (tmp_int < 0 || tmp_int >= MAX_CLASS)
        return;

    player_ptr->pclass = i2enum<player_class_type>(tmp_int);
    cp_ptr = &class_info[player_ptr->pclass];
    mp_ptr = &m_info[player_ptr->pclass];
    player_ptr->window_flags |= PW_PLAYER;
    player_ptr->update |= PU_BONUS | PU_HP | PU_MANA | PU_SPELLS;
    player_ptr->redraw |= PR_BASIC | PR_HP | PR_MANA | PR_STATS;
    handle_stuff(player_ptr);
}

/*!
 * @brief プレイヤーの領域を変更する
 * @todo 存在有無などは未判定。そのうちすべき。
 */
void wiz_reset_realms(player_type *player_ptr)
{
    char ppp[80];
    char tmp_val[160];

    sprintf(ppp, "1st Realm (None=0, 1-%d): ", MAX_REALM - 1);
    sprintf(tmp_val, "%d", player_ptr->realm1);
    if (!get_string(ppp, tmp_val, 2))
        return;

    player_ptr->realm1 = static_cast<int16_t>(atoi(tmp_val));

    sprintf(ppp, "2st Realm (None=0, 1-%d): ", MAX_REALM - 1);
    sprintf(tmp_val, "%d", player_ptr->realm2);
    if (!get_string(ppp, tmp_val, 2))
        return;

    player_ptr->realm2 = static_cast<int16_t>(atoi(tmp_val));
    player_ptr->window_flags |= PW_PLAYER;
    player_ptr->update |= PU_BONUS | PU_HP | PU_MANA | PU_SPELLS;
    player_ptr->redraw |= PR_BASIC;
    handle_stuff(player_ptr);
}

/*!
 * @brief 現在のオプション設定をダンプ出力する /
 * @param player_ptr プレイヤーへの参照ポインタ
 * Hack -- Dump option bits usage
 */
void wiz_dump_options(void)
{
    char buf[1024];
    path_build(buf, sizeof(buf), ANGBAND_DIR_USER, "opt_info.txt");
    FILE *fff;
    fff = angband_fopen(buf, "a");
    if (fff == nullptr) {
        msg_format(_("ファイル %s を開けませんでした。", "Failed to open file %s."), buf);
        msg_print(nullptr);
        return;
    }

    std::vector<std::vector<int>> exist(NUM_O_SET, std::vector<int>(NUM_O_BIT));

    for (int i = 0; option_info[i].o_desc; i++) {
        const option_type *ot_ptr = &option_info[i];
        if (ot_ptr->o_var)
            exist[ot_ptr->o_set][ot_ptr->o_bit] = i + 1;
    }

    char title[200];
    put_version(title);
    fprintf(fff, "[Option bits usage on %s\n]", title);
    fputs("Set - Bit (Page) Option Name\n", fff);
    fputs("------------------------------------------------\n", fff);
    for (int i = 0; i < NUM_O_SET; i++) {
        for (int j = 0; j < NUM_O_BIT; j++) {
            if (exist[i][j]) {
                const option_type *ot_ptr = &option_info[exist[i][j] - 1];
                fprintf(fff, "  %d -  %02d (%4d) %s\n", i, j, ot_ptr->o_page, ot_ptr->o_text);
            } else {
                fprintf(fff, "  %d -  %02d\n", i, j);
            }
        }

        fputc('\n', fff);
    }

    angband_fclose(fff);
    msg_format(_("オプションbit使用状況をファイル %s に書き出しました。", "Option bits usage dump saved to file %s."), buf);
}

/*!
 * @brief プレイ日数を変更する / Set gametime.
 * @return 実際に変更を行ったらTRUEを返す
 */
void set_gametime(void)
{
    int tmp_int = 0;
    char ppp[80], tmp_val[40];
    sprintf(ppp, "Dungeon Turn (0-%ld): ", (long)w_ptr->dungeon_turn_limit);
    sprintf(tmp_val, "%ld", (long)w_ptr->dungeon_turn);
    if (!get_string(ppp, tmp_val, 10))
        return;

    tmp_int = atoi(tmp_val);
    if (tmp_int >= w_ptr->dungeon_turn_limit)
        tmp_int = w_ptr->dungeon_turn_limit - 1;
    else if (tmp_int < 0)
        tmp_int = 0;

    w_ptr->dungeon_turn = w_ptr->game_turn = tmp_int;
}

/*!
 * @brief プレイヤー近辺の全モンスターを消去する / Delete all nearby monsters
 */
void wiz_zap_surrounding_monsters(player_type *player_ptr)
{
    for (MONSTER_IDX i = 1; i < player_ptr->current_floor_ptr->m_max; i++) {
        monster_type *m_ptr = &player_ptr->current_floor_ptr->m_list[i];
        if (!monster_is_valid(m_ptr) || (i == player_ptr->riding) || (m_ptr->cdis > MAX_SIGHT))
            continue;

        if (record_named_pet && is_pet(m_ptr) && m_ptr->nickname) {
            GAME_TEXT m_name[MAX_NLEN];

            monster_desc(player_ptr, m_name, m_ptr, MD_INDEF_VISIBLE);
            exe_write_diary(player_ptr, DIARY_NAMED_PET, RECORD_NAMED_PET_WIZ_ZAP, m_name);
        }

        delete_monster_idx(player_ptr, i);
    }
}

/*!
 * @brief フロアに存在する全モンスターを消去する / Delete all monsters
 * @param player_ptr 術者の参照ポインタ
 */
void wiz_zap_floor_monsters(player_type *player_ptr)
{
    for (MONSTER_IDX i = 1; i < player_ptr->current_floor_ptr->m_max; i++) {
        monster_type *m_ptr = &player_ptr->current_floor_ptr->m_list[i];
        if (!monster_is_valid(m_ptr) || (i == player_ptr->riding))
            continue;

        if (record_named_pet && is_pet(m_ptr) && m_ptr->nickname) {
            GAME_TEXT m_name[MAX_NLEN];
            monster_desc(player_ptr, m_name, m_ptr, MD_INDEF_VISIBLE);
            exe_write_diary(player_ptr, DIARY_NAMED_PET, RECORD_NAMED_PET_WIZ_ZAP, m_name);
        }

        delete_monster_idx(player_ptr, i);
    }
}

void cheat_death(player_type *player_ptr)
{
    if (player_ptr->sc)
        player_ptr->sc = player_ptr->age = 0;
    player_ptr->age++;

    w_ptr->noscore |= 0x0001;
    msg_print(_("ウィザードモードに念を送り、死を欺いた。", "You invoke wizard mode and cheat death."));
    msg_print(nullptr);

    player_ptr->is_dead = false;
    (void)life_stream(player_ptr, false, false);
    (void)restore_mana(player_ptr, true);
    (void)recall_player(player_ptr, 0);
    reserve_alter_reality(player_ptr, 0);

    (void)strcpy(player_ptr->died_from, _("死の欺き", "Cheating death"));
    (void)set_food(player_ptr, PY_FOOD_MAX - 1);

    floor_type *floor_ptr = player_ptr->current_floor_ptr;
    floor_ptr->dun_level = 0;
    floor_ptr->inside_arena = false;
    player_ptr->phase_out = false;
    leaving_quest = 0;
    floor_ptr->inside_quest = 0;
    if (player_ptr->dungeon_idx)
        player_ptr->recall_dungeon = player_ptr->dungeon_idx;
    player_ptr->dungeon_idx = 0;
    if (lite_town || vanilla_town) {
        player_ptr->wilderness_y = 1;
        player_ptr->wilderness_x = 1;
        if (vanilla_town) {
            player_ptr->oldpy = 10;
            player_ptr->oldpx = 34;
        } else {
            player_ptr->oldpy = 33;
            player_ptr->oldpx = 131;
        }
    } else {
        player_ptr->wilderness_y = 48;
        player_ptr->wilderness_x = 5;
        player_ptr->oldpy = 33;
        player_ptr->oldpx = 131;
    }

    player_ptr->wild_mode = false;
    player_ptr->leaving = true;

    exe_write_diary(player_ptr, DIARY_DESCRIPTION, 1, _("                            しかし、生き返った。", "                            but revived."));
    leave_floor(player_ptr);
}
