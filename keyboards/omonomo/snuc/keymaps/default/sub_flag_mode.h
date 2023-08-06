#pragma once

#include QMK_KEYBOARD_H
#include "main_keymap.h"

// 不眠モード用 #############################################
#define IS_WAKEFUL (wakefl_mode)
#define PC_SLEEPY  wakefl_mode = false
#define PC_WAKEFUL wakefl_mode = true

// 関数プロトタイプ宣言 #############################################
void ms_wakeup(global_s *global);

void pr_p_mod_flag(uint16_t keycode, keyrecord_t *record, global_s *global);
void pr_windows(uint16_t keycode, keyrecord_t *record, global_s *global);
bool pr_mode_change(uint16_t keycode, keyrecord_t *record, global_s *global);

// End |||||||||||||||||||||||||||||||||||||||||||||
