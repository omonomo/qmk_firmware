#pragma once

#include QMK_KEYBOARD_H
#include "main_keymap.h"

// 関数置き替え #############################################
//#define CHANGE_WIDTH(width) change_width(width, &letter_width, global)
//#define R_CHANGE_WIDTH(width) CHANGE_WIDTH(width); return

// 型定義 #############################################
typedef enum {
	HALF, ROMAN, FULL, SYMBOL
} letter_width_e; // pr_change_en関数用

// 関数プロトタイプ宣言 #############################################
//void change_width(letter_width_e after_width, letter_width_e *letter_width, global_s *global);

void pr_change_en(uint16_t keycode, keyrecord_t *record, global_s *global);
bool pr_numeric_hyper(uint16_t keycode, keyrecord_t *record, global_s *global);

// End |||||||||||||||||||||||||||||||||||||||||||||
