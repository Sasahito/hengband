/*!
 * @brief プレイヤーのステータス表示メインルーチン群
 * @date 2020/02/25
 * @author Hourier
 * @details
 * ここにこれ以上関数を引っ越してくるのは禁止
 */

#include "view/display-player.h"
#include "dungeon/quest.h"
#include "floor/floor-util.h"
#include "game-option/text-display-options.h"
#include "info-reader/fixed-map-parser.h"
#include "inventory/inventory-slot-types.h"
#include "knowledge/knowledge-mutations.h"
#include "mind/mind-elementalist.h"
#include "mutation/mutation-flag-types.h"
#include "object/object-info.h"
#include "object/object-kind.h"
#include "player-base/player-class.h"
#include "player-info/alignment.h"
#include "player-info/class-info.h"
#include "player-info/mimic-info-table.h"
#include "player/patron.h"
#include "player/player-personality.h"
#include "player/player-sex.h"
#include "player/player-status-flags.h"
#include "player/player-status-table.h"
#include "player/player-status.h"
#include "realm/realm-names-table.h"
#include "status-first-page.h"
#include "system/floor-type-definition.h"
#include "system/object-type-definition.h"
#include "system/player-type-definition.h"
#include "term/screen-processor.h"
#include "term/term-color-types.h"
#include "util/buffer-shaper.h"
#include "view/display-characteristic.h"
#include "view/display-player-middle.h"
#include "view/display-player-misc-info.h"
#include "view/display-player-stat-info.h"
#include "view/display-util.h"
#include "world/world.h"
#include <string>

/*!
 * @brief
 * @param player_ptr プレイヤーへの参照ポインタ
 * @param mode ステータス表示モード
 * @return どれかの処理をこなしたらTRUE、何もしなかったらFALSE
 */
static bool display_player_info(PlayerType *player_ptr, int mode)
{
    if (mode == 2) {
        display_player_misc_info(player_ptr);
        display_player_stat_info(player_ptr);
        display_player_flag_info_1(player_ptr, display_player_equippy);
        return true;
    }

    if (mode == 3) {
        display_player_flag_info_2(player_ptr, display_player_equippy);
        return true;
    }

    if (mode == 4) {
        display_player_flag_info_3(player_ptr, display_player_equippy);
        return true;
    }

    if (mode == 5) {
        do_cmd_knowledge_mutations(player_ptr);
        return true;
    }

    return false;
}

/*!
 * @brief 名前、性別、種族、職業を表示する
 * @param player_ptr プレイヤーへの参照ポインタ
 */
static void display_player_basic_info(PlayerType *player_ptr)
{
    char tmp[64];
#ifdef JP
    sprintf(tmp, "%s%s%s", ap_ptr->title, ap_ptr->no == 1 ? "の" : "", player_ptr->name);
#else
    sprintf(tmp, "%s %s", ap_ptr->title, player_ptr->name);
#endif

    display_player_one_line(ENTRY_NAME, tmp, TERM_L_BLUE);
    display_player_one_line(ENTRY_SEX, sp_ptr->title, TERM_L_BLUE);
    display_player_one_line(ENTRY_RACE, (player_ptr->mimic_form ? mimic_info[player_ptr->mimic_form].title : rp_ptr->title), TERM_L_BLUE);
    display_player_one_line(ENTRY_CLASS, cp_ptr->title, TERM_L_BLUE);
}

/*!
 * @brief 魔法領域を表示する
 * @param player_ptr プレイヤーへの参照ポインタ
 */
static void display_magic_realms(PlayerType *player_ptr)
{
    if (player_ptr->realm1 == REALM_NONE && player_ptr->element == REALM_NONE)
        return;

    char tmp[64];
    if (PlayerClass(player_ptr).equals(PlayerClassType::ELEMENTALIST))
        sprintf(tmp, "%s", get_element_title(player_ptr->element));
    else if (player_ptr->realm2)
        sprintf(tmp, "%s, %s", realm_names[player_ptr->realm1], realm_names[player_ptr->realm2]);
    else
        strcpy(tmp, realm_names[player_ptr->realm1]);

    display_player_one_line(ENTRY_REALM, tmp, TERM_L_BLUE);
}

/*!
 * @ brief 年齢、身長、体重、社会的地位を表示する
 * @param player_ptr プレイヤーへの参照ポインタ
 * @details
 * 日本語版では、身長はcmに、体重はkgに変更してある
 */
static void display_phisique(PlayerType *player_ptr)
{
#ifdef JP
    display_player_one_line(ENTRY_AGE, format("%d才", (int)player_ptr->age), TERM_L_BLUE);
    display_player_one_line(ENTRY_HEIGHT, format("%dcm", (int)((player_ptr->ht * 254) / 100)), TERM_L_BLUE);
    display_player_one_line(ENTRY_WEIGHT, format("%dkg", (int)((player_ptr->wt * 4536) / 10000)), TERM_L_BLUE);
    display_player_one_line(ENTRY_SOCIAL, format("%d  ", (int)player_ptr->sc), TERM_L_BLUE);
#else
    display_player_one_line(ENTRY_AGE, format("%d", (int)player_ptr->age), TERM_L_BLUE);
    display_player_one_line(ENTRY_HEIGHT, format("%d", (int)player_ptr->ht), TERM_L_BLUE);
    display_player_one_line(ENTRY_WEIGHT, format("%d", (int)player_ptr->wt), TERM_L_BLUE);
    display_player_one_line(ENTRY_SOCIAL, format("%d", (int)player_ptr->sc), TERM_L_BLUE);
#endif
    std::string alg = PlayerAlignment(player_ptr).get_alignment_description();
    display_player_one_line(ENTRY_ALIGN, format("%s", alg.c_str()), TERM_L_BLUE);
}

/*!
 * @brief 能力値を (減少していたら色を変えて)表示する
 * @param player_ptr プレイヤーへの参照ポインタ
 */
static void display_player_stats(PlayerType *player_ptr)
{
    char buf[80];
    for (int i = 0; i < A_MAX; i++) {
        if (player_ptr->stat_cur[i] < player_ptr->stat_max[i]) {
            put_str(stat_names_reduced[i], 3 + i, 53);
            int value = player_ptr->stat_use[i];
            cnv_stat(value, buf);
            c_put_str(TERM_YELLOW, buf, 3 + i, 60);
            value = player_ptr->stat_top[i];
            cnv_stat(value, buf);
            c_put_str(TERM_L_GREEN, buf, 3 + i, 67);
        } else {
            put_str(stat_names[i], 3 + i, 53);
            cnv_stat(player_ptr->stat_use[i], buf);
            c_put_str(TERM_L_GREEN, buf, 3 + i, 60);
        }

        if (player_ptr->stat_max[i] == player_ptr->stat_max_max[i])
            c_put_str(TERM_WHITE, "!", 3 + i, _(58, 58 - 2));
    }
}

/*!
 * @brief ゲームオーバーの原因を探る (生きていたら何もしない)
 * @param player_ptr プレイヤーへの参照ポインタ
 * @param statmsg メッセージバッファ
 * @return 生きていたらFALSE、死んでいたらTRUE
 */
static std::optional<std::string> search_death_cause(PlayerType *player_ptr)
{
    auto *floor_ptr = player_ptr->current_floor_ptr;
    if (!player_ptr->is_dead) {
        return std::nullopt;
    }

    if (w_ptr->total_winner) {
        return std::string(format(_("…あなたは勝利の後%sした。", "...You %s after winning."),
            streq(player_ptr->died_from, "Seppuku") ? _("切腹", "committed seppuku") : _("引退", "retired from the adventure")));
    }

    if (!floor_ptr->dun_level) {
#ifdef JP
        return std::string(format("…あなたは%sで%sに殺された。", map_name(player_ptr), player_ptr->died_from));
#else
        return std::string(format("...You were killed by %s in %s.", player_ptr->died_from, map_name(player_ptr)));
#endif
    }

    if (floor_ptr->quest_number && quest_type::is_fixed(floor_ptr->quest_number)) {
        /* Get the quest text */
        /* Bewere that INIT_ASSIGN resets the cur_num. */
        init_flags = INIT_NAME_ONLY;
        parse_fixed_map(player_ptr, "q_info.txt", 0, 0, 0, 0);
#ifdef JP
        return std::string(format("…あなたは、クエスト「%s」で%sに殺された。", quest[floor_ptr->quest_number].name, player_ptr->died_from));
#else
        return std::string(format("...You were killed by %s in the quest '%s'.", player_ptr->died_from, quest[floor_ptr->quest_number].name));
#endif
    }

#ifdef JP
    return std::string(format("…あなたは、%sの%d階で%sに殺された。", map_name(player_ptr), (int)floor_ptr->dun_level, player_ptr->died_from));
#else
    return std::string(format("...You were killed by %s on level %d of %s.", player_ptr->died_from, floor_ptr->dun_level, map_name(player_ptr)));
#endif
}

/*!
 * @brief クエストフロアで生きている場合、クエスト名をバッファに詰める
 * @param player_ptr プレイヤーへの参照ポインタ
 * @param statmsg メッセージバッファ
 * @return クエスト内であればTRUE、いなければFALSE
 */
static std::optional<std::string> decide_death_in_quest(PlayerType *player_ptr)
{
    floor_type *floor_ptr = player_ptr->current_floor_ptr;
    if (!floor_ptr->quest_number || !quest_type::is_fixed(floor_ptr->quest_number))
        return std::nullopt;

    for (int i = 0; i < 10; i++)
        quest_text[i][0] = '\0';

    quest_text_line = 0;
    init_flags = INIT_NAME_ONLY;
    parse_fixed_map(player_ptr, "q_info.txt", 0, 0, 0, 0);
    return std::string(format(_("…あなたは現在、 クエスト「%s」を遂行中だ。", "...Now, you are in the quest '%s'."), quest[floor_ptr->quest_number].name));
}

/*!
 * @brief 現在いるフロアを、または死んでいたらどこでどう死んだかをバッファに詰める
 * @param player_ptr プレイヤーへの参照ポインタ
 * @param statmsg メッセージバッファ
 */
static std::string decide_current_floor(PlayerType *player_ptr)
{
    if (auto death_cause = search_death_cause(player_ptr);
        death_cause.has_value() || !w_ptr->character_dungeon) {
        return death_cause.value_or("");
    }

    auto *floor_ptr = player_ptr->current_floor_ptr;
    if (floor_ptr->dun_level == 0) {
        return std::string(format(_("…あなたは現在、 %s にいる。", "...Now, you are in %s."), map_name(player_ptr)));
    }

    if (auto decision = decide_death_in_quest(player_ptr); decision.has_value()) {
        return decision.value();
    }

#ifdef JP
    return std::string(format("…あなたは現在、 %s の %d 階で探索している。", map_name(player_ptr), (int)floor_ptr->dun_level));
#else
    return std::string(format("...Now, you are exploring level %d of %s.", (int)floor_ptr->dun_level, map_name(player_ptr)));
#endif
}

/*!
 * @brief 今いる、または死亡した場所を表示する
 * @param statmsg メッセージバッファ
 * @return ダンプ表示行数の補正項
 * @details v2.2までは状況に関係なく必ず2行であり、v3.0では1～4行になり得、num_linesはその行数.
 * ギリギリ見切れる場合があるので行数は僅かに多めに取る.
 */
static int display_current_floor(const std::string &statmsg)
{
    char temp[1000];
    constexpr auto chars_per_line = 60;
    shape_buffer(statmsg.c_str(), chars_per_line, temp, sizeof(temp));
    auto t = temp;
    auto statmsg_size = statmsg.size();
    auto fraction = statmsg_size % (chars_per_line - 1);
    auto num_lines = statmsg_size / (chars_per_line - 1);
    num_lines += fraction > 0 ? 1 : 0;
    for (auto i = 0U; i < num_lines; i++) {
        put_str(t, i + 5 + 12, 10);
        t += strlen(t) + 1;
    }

    return num_lines;
}

/*!
 * @brief プレイヤーのステータス表示メイン処理
 * Display the character on the screen (various modes)
 * @param player_ptr プレイヤーへの参照ポインタ
 * @param tmp_mode 暫定表示モード (突然変異の有無で実際のモードに切り替える)
 * @return 死亡原因となったモンスター名が複数行に亘る場合、表示に必要な行数. それ以外を表示する場合はnullopt
 * @details
 * 最初の1行と最後の2行は空行. / The top one and bottom two lines are left blank.
 * Mode 0 = standard display with skills.
 * Mode 1 = standard display with history.
 * Mode 2 = summary of various things.
 * Mode 3 = summary of various things (part 2).
 * Mode 4 = mutations.
 * Mode 5 = ??? (コード上の定義より6で割った余りは5になりうるが元のコメントに記載なし).
 */
std::optional<int> display_player(PlayerType *player_ptr, const int tmp_mode)
{
    auto has_any_mutation = (player_ptr->muta.any() || has_good_luck(player_ptr)) && display_mutations;
    auto mode = has_any_mutation ? tmp_mode % 6 : tmp_mode % 5;
    clear_from(0);
    if (display_player_info(player_ptr, mode)) {
        return std::nullopt;
    }

    display_player_basic_info(player_ptr);
    display_magic_realms(player_ptr);
    if (PlayerClass(player_ptr).equals(PlayerClassType::CHAOS_WARRIOR) || (player_ptr->muta.has(PlayerMutationType::CHAOS_GIFT))) {
        display_player_one_line(ENTRY_PATRON, patron_list[player_ptr->chaos_patron].name.c_str(), TERM_L_BLUE);
    }

    display_phisique(player_ptr);
    display_player_stats(player_ptr);
    if (mode == 0) {
        display_player_middle(player_ptr);
        display_player_various(player_ptr);
        return std::nullopt;
    }

    put_str(_("(キャラクターの生い立ち)", "(Character Background)"), 11, 25);
    for (auto i = 0; i < 4; i++) {
        put_str(player_ptr->history[i], i + 12, 10);
    }

    auto statmsg = decide_current_floor(player_ptr);
    if (statmsg == "") {
        return std::nullopt;
    }

    return display_current_floor(statmsg);
}

/*!
 * @brief プレイヤーの装備一覧をシンボルで並べる
 * Equippy chars
 * @param player_ptr プレイヤーへの参照ポインタ
 * @param y 表示するコンソールの行
 * @param x 表示するコンソールの列
 * @param mode オプション
 * @todo y = 6、x = 0、mode = 0で固定。何とかする
 */
void display_player_equippy(PlayerType *player_ptr, TERM_LEN y, TERM_LEN x, BIT_FLAGS16 mode)
{
    int max_i = (mode & DP_WP) ? INVEN_BOW + 1 : INVEN_TOTAL;
    for (int i = INVEN_MAIN_HAND; i < max_i; i++) {
        ObjectType *o_ptr;
        o_ptr = &player_ptr->inventory_list[i];

        TERM_COLOR a = object_attr(o_ptr);
        SYMBOL_CODE c = object_char(o_ptr);

        if (!equippy_chars || !o_ptr->k_idx) {
            c = ' ';
            a = TERM_DARK;
        }

        term_putch(x + i - INVEN_MAIN_HAND, y, a, c);
    }
}
