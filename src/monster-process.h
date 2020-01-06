﻿#pragma once

/* monster-process.c */
extern void process_monsters(player_type *target_ptr);
extern void process_monster(player_type *target_ptr, MONSTER_IDX m_idx);
extern int get_mproc_idx(MONSTER_IDX m_idx, int mproc_type);
extern void mproc_init(void);
extern void process_monsters_mtimed(int mtimed_idx);
