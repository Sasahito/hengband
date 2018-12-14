﻿#include "angband.h"
#include "objectkind-hook.h"

/*!
 * @brief オブジェクトがクロークかどうかを判定する /
 * Hack -- determine if a template is Cloak
 * @param k_idx 判定したいオブジェクトのベースアイテムID
 * @return オブジェクトがクロークならばTRUEを返す
 */
bool kind_is_cloak(KIND_OBJECT_IDX k_idx)
{
	object_kind *k_ptr = &k_info[k_idx];

	/* Analyze the item type */
	if (k_ptr->tval == TV_CLOAK)
	{
		return (TRUE);
	}

	/* Assume not good */
	return (FALSE);
}


/*!
 * @brief オブジェクトが竿状武器かどうかを判定する /
 * Hack -- determine if a template is Polearm
 * @param k_idx 判定したいオブジェクトのベースアイテムID
 * @return オブジェクトが竿状武器ならばTRUEを返す
 */
bool kind_is_polearm(KIND_OBJECT_IDX k_idx)
{
	object_kind *k_ptr = &k_info[k_idx];

	/* Analyze the item type */
	if (k_ptr->tval == TV_POLEARM)
	{
		return (TRUE);
	}

	/* Assume not good */
	return (FALSE);
}


/*!
 * @brief オブジェクトが剣かどうかを判定する /
 * Hack -- determine if a template is Sword
 * @param k_idx 判定したいオブジェクトのベースアイテムID
 * @return オブジェクトが剣ならばTRUEを返す
 */
bool kind_is_sword(KIND_OBJECT_IDX k_idx)
{
	object_kind *k_ptr = &k_info[k_idx];

	/* Analyze the item type */
	if ((k_ptr->tval == TV_SWORD) && (k_ptr->sval > 2))
	{
		return (TRUE);
	}

	/* Assume not good */
	return (FALSE);
}


/*!
 * @brief オブジェクトが魔法書かどうかを判定する /
 * Hack -- determine if a template is Book
 * @param k_idx 判定したいオブジェクトのベースアイテムID
 * @return オブジェクトが魔法書ならばTRUEを返す
 */
bool kind_is_book(KIND_OBJECT_IDX k_idx)
{
	object_kind *k_ptr = &k_info[k_idx];

	/* Analyze the item type */
	if ((k_ptr->tval >= TV_LIFE_BOOK) && (k_ptr->tval <= TV_CRUSADE_BOOK))
	{
		return (TRUE);
	}

	/* Assume not good */
	return (FALSE);
}


/*!
 * @brief オブジェクトがベースアイテム時点でGOODかどうかを判定する /
 * Hack -- determine if a template is Good book
 * @param k_idx 判定したいオブジェクトのベースアイテムID
 * @return オブジェクトがベースアイテム時点でGOODなアイテムならばTRUEを返す
 */
bool kind_is_good_book(KIND_OBJECT_IDX k_idx)
{
	object_kind *k_ptr = &k_info[k_idx];

	/* Analyze the item type */
	if ((k_ptr->tval >= TV_LIFE_BOOK) && (k_ptr->tval <= TV_CRUSADE_BOOK) && (k_ptr->tval != TV_ARCANE_BOOK) && (k_ptr->sval > 1))
	{
		return (TRUE);
	}

	/* Assume not good */
	return (FALSE);
}


/*!
 * @brief オブジェクトが鎧かどうかを判定する /
 * Hack -- determine if a template is Armor
 * @param k_idx 判定したいオブジェクトのベースアイテムID
 * @return オブジェクトが鎧ならばTRUEを返す
 */
bool kind_is_armor(KIND_OBJECT_IDX k_idx)
{
	object_kind *k_ptr = &k_info[k_idx];

	/* Analyze the item type */
	if (k_ptr->tval == TV_HARD_ARMOR)
	{
		return (TRUE);
	}

	/* Assume not good */
	return (FALSE);
}


/*!
 * @brief オブジェクトが打撃武器かどうかを判定する /
 * Hack -- determine if a template is hafted weapon
 * @param k_idx 判定したいオブジェクトのベースアイテムID
 * @return オブジェクトが打撃武器ならばTRUEを返す
 */
bool kind_is_hafted(KIND_OBJECT_IDX k_idx)
{
	object_kind *k_ptr = &k_info[k_idx];

	/* Analyze the item type */
	if (k_ptr->tval == TV_HAFTED)
	{
		return (TRUE);
	}

	/* Assume not good */
	return (FALSE);
}
