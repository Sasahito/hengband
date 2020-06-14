﻿/*
 * @author
 * 2002/01/12 mogami
 * 2020/05/16 Hourier
 * @details
 * Copyright (c) 1997 Ben Harrison, James E. Wilson, Robert A. Koeneke
 * This software may be copied and distributed for educational, research,
 * and not for profit purposes provided that this copyright and statement
 * are included in all such copies.  Other copyrights may also apply.
 */

#include "system/angband.h"
#include "util/util.h"
#include "cmd-io/cmd-dump.h"
#include "cmd-io/cmd-menu-content-table.h"
#include "cmd-io/macro-util.h"
#include "core/asking-player.h"
#include "core/output-updater.h"
#include "core/stuff-handler.h"
#include "dungeon/quest.h"
#include "floor/floor.h"
#include "game-option/cheat-options.h"
#include "game-option/disturbance-options.h"
#include "game-option/input-options.h"
#include "game-option/map-screen-options.h"
#include "game-option/option-flags.h"
#include "game-option/special-options.h"
#include "io/files-util.h"
#include "io/input-key-acceptor.h"
#include "io/input-key-processor.h"
#include "io/input-key-requester.h"
#include "io/signal-handlers.h"
#include "io/write-diary.h"
#include "locale/japanese.h"
#include "main/music-definitions-table.h"
#include "main/sound-of-music.h"
#include "monster-race/monster-race-hook.h"
#include "player/player-class.h"
#include "system/system-variables.h"
#include "term/gameterm.h"
#include "term/screen-processor.h"
#include "term/term-color-types.h"
#include "util/quarks.h"
#include "util/string-processor.h"
#include "view/display-main-window.h"
#include "view/display-messages.h"
#include "world/world.h"

/*!
 * 10進数から16進数への変換テーブル /
 * Global array for converting numbers to uppercase hecidecimal digit
 * This array can also be used to convert a number to an octal digit
 */
const char hexsym[16] =
{
	'0', '1', '2', '3', '4', '5', '6', '7',
	'8', '9', 'A', 'B', 'C', 'D', 'E', 'F'
};

pos_list tmp_pos;

/*
 * Check a char for "vowel-hood"
 */
bool is_a_vowel(int ch)
{
	switch (ch)
	{
	case 'a':
	case 'e':
	case 'i':
	case 'o':
	case 'u':
	case 'A':
	case 'E':
	case 'I':
	case 'O':
	case 'U':
		return TRUE;
	}

	return FALSE;
}


/*
 * GH
 * Called from cmd4.c and a few other places. Just extracts
 * a direction from the keymap for ch (the last direction,
 * in fact) byte or char here? I'm thinking that keymaps should
 * generally only apply to single keys, which makes it no more
 * than 128, so a char should suffice... but keymap_act is 256...
 */
int get_keymap_dir(char ch)
{
	int d = 0;

	if (isdigit(ch))
	{
		d = D2I(ch);
	}
	else
	{
		BIT_FLAGS mode;
		if (rogue_like_commands)
		{
			mode = KEYMAP_MODE_ROGUE;
		}
		else
		{
			mode = KEYMAP_MODE_ORIG;
		}

		concptr act = keymap_act[mode][(byte)(ch)];
		if (act)
		{
			for (concptr s = act; *s; ++s)
			{
				if (isdigit(*s)) d = D2I(*s);
			}
		}
	}

	if (d == 5) d = 0;

	return (d);
}


/*
 * Array size for which InsertionSort
 * is used instead of QuickSort
 */
#define CUTOFF 4


 /*
  * Exchange two sort-entries
  * (should probably be coded inline
  * for speed increase)
  */
static void swap(tag_type *a, tag_type *b)
{
	tag_type temp;

	temp = *a;
	*a = *b;
	*b = temp;
}


/*
 * Insertion-Sort algorithm
 * (used by the Quicksort algorithm)
 */
static void InsertionSort(tag_type elements[], int number)
{
	tag_type tmp;
	for (int i = 1; i < number; i++)
	{
		tmp = elements[i];
		int j;
		for (j = i; (j > 0) && (elements[j - 1].tag > tmp.tag); j--)
			elements[j] = elements[j - 1];
		elements[j] = tmp;
	}
}


/*
 * Helper function for Quicksort
 */
static tag_type median3(tag_type elements[], int left, int right)
{
	int center = (left + right) / 2;

	if (elements[left].tag > elements[center].tag)
		swap(&elements[left], &elements[center]);
	if (elements[left].tag > elements[right].tag)
		swap(&elements[left], &elements[right]);
	if (elements[center].tag > elements[right].tag)
		swap(&elements[center], &elements[right]);

	swap(&elements[center], &elements[right - 1]);
	return (elements[right - 1]);
}


/*
 * Quicksort algorithm
 *
 * The "median of three" pivot selection eliminates
 * the bad case of already sorted input.
 *
 * We use InsertionSort for smaller sub-arrays,
 * because it is faster in this case.
 *
 * For details see: "Data Structures and Algorithm
 * Analysis in C" by Mark Allen Weiss.
 */
static void quicksort(tag_type elements[], int left, int right)
{
	tag_type pivot;
	if (left + CUTOFF <= right)
	{
		pivot = median3(elements, left, right);

		int i = left;
		int j = right - 1;

		while (TRUE)
		{
			while (elements[++i].tag < pivot.tag);
			while (elements[--j].tag > pivot.tag);

			if (i < j)
				swap(&elements[i], &elements[j]);
			else
				break;
		}

		swap(&elements[i], &elements[right - 1]);

		quicksort(elements, left, i - 1);
		quicksort(elements, i + 1, right);
	}
	else
	{
		InsertionSort(elements + left, right - left + 1);
	}
}


/*
 * Frontend for the sorting algorithm
 *
 * Sorts an array of tagged pointers
 * with <number> elements.
 */
void tag_sort(tag_type elements[], int number)
{
	quicksort(elements, 0, number - 1);
}

/*
 * Add a series of keypresses to the "queue".
 *
 * Return any errors generated by Term_keypress() in doing so, or SUCCESS
 * if there are none.
 *
 * Catch the "out of space" error before anything is printed.
 *
 * NB: The keys added here will be interpreted by any macros or keymaps.
 */
errr type_string(concptr str, uint len)
{
	errr err = 0;
	term *old = Term;
	if (!str) return -1;
	if (!len) len = strlen(str);

	Term_activate(term_screen);
	for (concptr s = str; s < str + len; s++)
	{
		if (*s == '\0') break;

		err = Term_keypress(*s);
		if (err) break;
	}

	Term_activate(old);
	return err;
}


void roff_to_buf(concptr str, int maxlen, char *tbuf, size_t bufsize)
{
	int read_pt = 0;
	int write_pt = 0;
	int line_len = 0;
	int word_punct = 0;
	char ch[3];
	ch[2] = '\0';

	while (str[read_pt])
	{
#ifdef JP
		bool kinsoku = FALSE;
		bool kanji;
#endif
		int ch_len = 1;
		ch[0] = str[read_pt];
		ch[1] = '\0';
#ifdef JP
		kanji = iskanji(ch[0]);

		if (kanji)
		{
			ch[1] = str[read_pt + 1];
			ch_len = 2;

			if (strcmp(ch, "。") == 0 ||
				strcmp(ch, "、") == 0 ||
				strcmp(ch, "ィ") == 0 ||
				strcmp(ch, "ー") == 0)
				kinsoku = TRUE;
		}
		else if (!isprint(ch[0]))
			ch[0] = ' ';
#else
		if (!isprint(ch[0]))
			ch[0] = ' ';
#endif

		if (line_len + ch_len > maxlen - 1 || str[read_pt] == '\n')
		{
			int word_len = read_pt - word_punct;
#ifdef JP
			if (kanji && !kinsoku)
				/* nothing */;
			else
#endif
				if (ch[0] == ' ' || word_len >= line_len / 2)
					read_pt++;
				else
				{
					read_pt = word_punct;
					if (str[word_punct] == ' ')
						read_pt++;
					write_pt -= word_len;
				}

			tbuf[write_pt++] = '\0';
			line_len = 0;
			word_punct = read_pt;
			continue;
		}

		if (ch[0] == ' ')
			word_punct = read_pt;

#ifdef JP
		if (!kinsoku) word_punct = read_pt;
#endif

		if ((size_t)(write_pt + 3) >= bufsize) break;

		tbuf[write_pt++] = ch[0];
		line_len++;
		read_pt++;
#ifdef JP
		if (kanji)
		{
			tbuf[write_pt++] = ch[1];
			line_len++;
			read_pt++;
		}
#endif
	}

	tbuf[write_pt] = '\0';
	tbuf[write_pt + 1] = '\0';
	return;
}


/*
 * The angband_strcpy() function copies up to 'bufsize'-1 characters from 'src'
 * to 'buf' and NUL-terminates the result.  The 'buf' and 'src' strings may
 * not overlap.
 *
 * angband_strcpy() returns strlen(src).  This makes checking for truncation
 * easy.  Example: if (angband_strcpy(buf, src, sizeof(buf)) >= sizeof(buf)) ...;
 *
 * This function should be equivalent to the strlcpy() function in BSD.
 */
size_t angband_strcpy(char *buf, concptr src, size_t bufsize)
{
#ifdef JP
	char *d = buf;
	concptr s = src;
	size_t len = 0;

	if (bufsize > 0) {
		/* reserve for NUL termination */
		bufsize--;

		/* Copy as many bytes as will fit */
		while (*s && (len < bufsize))
		{
			if (iskanji(*s))
			{
				if (len + 1 >= bufsize || !*(s + 1)) break;
				*d++ = *s++;
				*d++ = *s++;
				len += 2;
			}
			else
			{
				*d++ = *s++;
				len++;
			}
		}
		*d = '\0';
	}

	while (*s++) len++;
	return len;

#else
	size_t len = strlen(src);
	size_t ret = len;
	if (bufsize == 0) return ret;

	if (len >= bufsize) len = bufsize - 1;

	(void)memcpy(buf, src, len);
	buf[len] = '\0';
	return ret;
#endif
}


/*
 * The angband_strcat() tries to append a string to an existing NUL-terminated string.
 * It never writes more characters into the buffer than indicated by 'bufsize' and
 * NUL-terminates the buffer.  The 'buf' and 'src' strings may not overlap.
 *
 * angband_strcat() returns strlen(buf) + strlen(src).  This makes checking for
 * truncation easy.  Example:
 * if (angband_strcat(buf, src, sizeof(buf)) >= sizeof(buf)) ...;
 *
 * This function should be equivalent to the strlcat() function in BSD.
 */
size_t angband_strcat(char *buf, concptr src, size_t bufsize)
{
	size_t dlen = strlen(buf);
	if (dlen < bufsize - 1)
	{
		return (dlen + angband_strcpy(buf + dlen, src, bufsize - dlen));
	}
	else
	{
		return (dlen + strlen(src));
	}
}


/*
 * A copy of ANSI strstr()
 *
 * angband_strstr() can handle Kanji strings correctly.
 */
char *angband_strstr(concptr haystack, concptr needle)
{
	int l1 = strlen(haystack);
	int l2 = strlen(needle);

	if (l1 >= l2)
	{
		for (int i = 0; i <= l1 - l2; i++)
		{
			if (!strncmp(haystack + i, needle, l2))
				return (char *)haystack + i;

#ifdef JP
			if (iskanji(*(haystack + i))) i++;
#endif
		}
	}

	return NULL;
}


/*
 * A copy of ANSI strchr()
 *
 * angband_strchr() can handle Kanji strings correctly.
 */
char *angband_strchr(concptr ptr, char ch)
{
	for (; *ptr != '\0'; ptr++)
	{
		if (*ptr == ch) return (char *)ptr;

#ifdef JP
		if (iskanji(*ptr)) ptr++;
#endif
	}

	return NULL;
}


/*
 * Convert string to lower case
 */
void str_tolower(char *str)
{
	for (; *str; str++)
	{
#ifdef JP
		if (iskanji(*str))
		{
			str++;
			continue;
		}
#endif
		*str = (char)tolower(*str);
	}
}
