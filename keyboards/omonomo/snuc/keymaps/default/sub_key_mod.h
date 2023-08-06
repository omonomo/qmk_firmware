#pragma once

#include QMK_KEYBOARD_H
#include "main_keymap.h"

// 2次プレフィクス用 #############################################
#define IS_SECOND_PREFIX  (second_prefix)
#define SECOND_PREFIX_OFF second_prefix = false
#define SECOND_PREFIX_ON  second_prefix = true

// 関数置き替え #############################################
#define SET_MACRO set_macro(keycode)
#define RST_MACRO set_macro(0)

// 関数プロトタイプ宣言 #############################################
void set_macro(uint8_t reg_key);

void ms_repeat_macro(global_s *global);

void pr_cancel(uint16_t keycode, keyrecord_t *record, global_s *global);
bool pr_mod_replace(uint16_t keycode, keyrecord_t *record, global_s *global);
bool pr_second_prefix(uint16_t keycode, keyrecord_t *record, global_s *global);
bool pr_metkey(uint16_t keycode, keyrecord_t *record, global_s *global);
bool pr_mod_tap_dance(uint16_t keycode, keyrecord_t *record, global_s *global);

// End |||||||||||||||||||||||||||||||||||||||||||||
