﻿#pragma once

#include "floor/geometry.h"

/*
 * Hack -- allow use of "ASCII" and "EBCDIC" for "indexes", "digits",
 * and "Control-Characters".
 *
 * Note that all "index" values must be "lowercase letters", while
 * all "digits" must be "digits".  Control characters can be made
 * from any legal characters.
 */
#define A2I(X)	((X) - 'a')
#define I2A(X)	((char)(X) + 'a')
#define D2I(X)	((X) - '0')
#define I2D(X)	((X) + '0')
#define KTRL(X)	((X) & 0x1F)
#define ESCAPE	'\033'

#define KEYMAP_MODE_ORIG	0 /*!< オリジナルキー配置 / Mode for original keyset commands */
#define KEYMAP_MODE_ROGUE	1 /*!< ローグライクキー配置 / Mode for roguelike keyset commands */
#define KEYMAP_MODES		2 /*!< キー配置の数 / Number of keymap modes */

#define SCREEN_BUF_MAX_SIZE (4 * 65536) /*!< Max size of screen dump buffer */

/* Cheat Info Type */
#define CHEAT_OBJECT 0
#define CHEAT_MONSTER 1
#define CHEAT_DUNGEON 2
#define CHEAT_MISC 3

/*
 * Max numbers of macro trigger names
 */
#define MAX_MACRO_MOD 12
#define MAX_MACRO_TRIG 200 /*!< 登録を許すマクロ（トリガー）の最大数 */

 /*
  * Object flags
  *
  * Old variables for object flags such as flags1, flags2, and flags3
  * are obsolated.  Now single array flgs[TR_FLAG_SIZE] contains all
  * object flags.  And each flag is refered by single index number
  * instead of a bit mask.
  *
  * Therefore it's very easy to add a lot of new flags; no one need to
  * worry about in which variable a new flag should be put, nor to
  * modify a huge number of files all over the source directory at once
  * to add new flag variables such as flags4, a_ability_flags1, etc...
  *
  * All management of flags is now treated using a set of macros
  * instead of bit operations.
  * Note: These macros are using division, modulo, and bit shift
  * operations, and it seems that these operations are rather slower
  * than original bit operation.  But since index numbers are almost
  * always given as constant, such slow operations are performed in the
  * compile time.  So there is no problem on the speed.
  *
  * Exceptions of new flag management is a set of flags to control
  * object generation and the curse flags.  These are not yet rewritten
  * in new index form; maybe these have no merit of rewriting.
  */

#define have_flag(ARRAY, INDEX) !!((ARRAY)[(INDEX)/32] & (1L << ((INDEX)%32)))
#define add_flag(ARRAY, INDEX) ((ARRAY)[(INDEX)/32] |= (1L << ((INDEX)%32)))
#define remove_flag(ARRAY, INDEX) ((ARRAY)[(INDEX)/32] &= ~(1L << ((INDEX)%32)))
#define is_pval_flag(INDEX) ((TR_STR <= (INDEX) && (INDEX) <= TR_MAGIC_MASTERY) || (TR_STEALTH <= (INDEX) && (INDEX) <= TR_BLOWS))
#define have_pval_flags(ARRAY) !!((ARRAY)[0] & (0x00003f7f))

  /*
	Language selection macro
  */
#ifdef JP
#define _(JAPANESE,ENGLISH) (JAPANESE)
#else
#define _(JAPANESE,ENGLISH) (ENGLISH)
#endif

/*
 * Sort-array element
 */
typedef struct tag_type
{
	int tag;
	int index;
} tag_type;

/*
 * An entry for the object/monster allocation functions
 *
 * Pass 1 is determined from allocation information
 * Pass 2 is determined from allocation restriction
 * Pass 3 is determined from allocation calculation
 */

typedef struct alloc_entry alloc_entry;

struct alloc_entry
{
	KIND_OBJECT_IDX index;		/* The actual index */

	DEPTH level;		/* Base dungeon level */
	PROB prob1;		/* Probability, pass 1 */
	PROB prob2;		/* Probability, pass 2 */
	PROB prob3;		/* Probability, pass 3 */

	u16b total;		/* Unused for now */
};

extern pos_list tmp_pos;

/*
 * OPTION: Maximum number of messages to remember (see "io.c")
 * Default: assume maximal memorization of 2048 total messages
 */
#define MESSAGE_MAX  81920

/*
 * OPTION: Maximum space for the message text buffer (see "io.c")
 * Default: assume that each of the 2048 messages is repeated an
 * average of three times, and has an average length of 48
 */
#define MESSAGE_BUF 655360

/*
 * Hack -- The main "screen"
 */
#define term_screen     (angband_term[0])

/*
 * Hack -- conditional (or "bizarre") externs
 */

#ifdef SET_UID
extern void user_name(char *buf, int id);
#endif

#ifndef HAVE_USLEEP
extern int usleep(huge usecs);
#endif

extern const char hexsym[16];

extern bool is_a_vowel(int ch);
extern int get_keymap_dir(char ch);
extern errr type_string(concptr str, uint len);
extern void roff_to_buf(concptr str, int wlen, char *tbuf, size_t bufsize);

extern void tag_sort(tag_type elements[], int number);

extern size_t angband_strcpy(char *buf, concptr src, size_t bufsize);
extern size_t angband_strcat(char *buf, concptr src, size_t bufsize);
extern char *angband_strstr(concptr haystack, concptr needle);
extern char *angband_strchr(concptr ptr, char ch);
extern void str_tolower(char *str);
