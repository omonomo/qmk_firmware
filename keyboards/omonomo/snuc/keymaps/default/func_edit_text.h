#pragma once

#include QMK_KEYBOARD_H
#include "main_keymap.h"

// 関数置き替え #############################################
#define RF_DUPLICATE_TEXT(times) return duplicate_text(times, global)

#define HOLD_TEXT16_UJ(leftU, rightU, leftJ, rightJ) hold_text16((!IS_JIS ? leftU : leftJ), (!IS_JIS ? rightU : rightJ), global)
#define RF_HOLD_TEXT16_UJ(leftU, rightU, leftJ, rightJ) return HOLD_TEXT16_UJ(leftU, rightU, leftJ, rightJ)

#define EDIT_TEXT(select, range, direction) edit_text(select, range, direction, global)

#define KEY_SEQUENCE(string) do {string} while (0)
#define RF_KEY_SEQUENCE(string) KEY_SEQUENCE(string); return false

// KEY_SEQUENCE関数型マクロ用(セミコロン注意) =============================================
#define TAP_ENT    tap_code(KC_ENT);
#define TAP_SPC    tap_code(KC_SPC);
#define MOVE_UP    tap_code(KC_UP);
#define MOVE_RGHT  tap_code(KC_RGHT);
#define MOVE_DOWN  tap_code(KC_DOWN);
#define MOVE_LEFT  tap_code(KC_LEFT);
#define CUT_TEXT   cut_text(global);
#define PASTE_TEXT paste_text(global);
#define DEL_TEXT   delete_text(global);

#define MOVE_WORD_LEFT EDIT_TEXT(MOVE, WORD, LEFT);
#define MOVE_WORD_RGHT EDIT_TEXT(MOVE, WORD, RGHT);
#define MOVE_LINE_TOP  EDIT_TEXT(MOVE, LINE, TOP);
#define MOVE_LINE_BTM  EDIT_TEXT(MOVE, LINE, BTM);
#define MOVE_PARA_TOP  EDIT_TEXT(MOVE, PARA, TOP);
#define MOVE_PARA_BTM  EDIT_TEXT(MOVE, PARA, BTM);
#define MOVE_DOC_TOP   EDIT_TEXT(MOVE, DOC,  TOP);
#define MOVE_DOC_BTM   EDIT_TEXT(MOVE, DOC,  BTM);
#define SLCT_CHAR_LEFT EDIT_TEXT(SLCT, CHAR, LEFT);
#define SLCT_CHAR_RGHT EDIT_TEXT(SLCT, CHAR, RGHT);
#define SLCT_WORD_LEFT EDIT_TEXT(SLCT, WORD, LEFT);
#define SLCT_WORD_RGHT EDIT_TEXT(SLCT, WORD, RGHT);
#define SLCT_WORD      EDIT_TEXT(SLCT, WORD, ALL);
#define SLCT_WORD_B    EDIT_TEXT(SLCT, WORD, ALL_B);
#define SLCT_PARA_TOP  EDIT_TEXT(SLCT, PARA, TOP);
#define SLCT_PARA_BTM  EDIT_TEXT(SLCT, PARA, BTM);
#define SLCT_PARA      EDIT_TEXT(SLCT, PARA, ALL);
#define SLCT_PARA_B    EDIT_TEXT(SLCT, PARA, ALL_B);

// 型定義 #############################################
typedef enum {
	MOVE, SLCT,
	CHAR, WORD, LINE, PARA, DOC,
	LEFT, RGHT, TOP,  BTM,  ALL, ALL_B
} textedit_e; // edit_text関数用

// 関数プロトタイプ宣言 #############################################
bool duplicate_text(uint8_t times, global_s *global);
bool hold_text16(uint16_t left_letter, uint16_t right_letter, global_s *global);
void cut_text(global_s *global);
void paste_text(global_s *global);
void delete_text(global_s *global);
void edit_text(textedit_e select, textedit_e range, textedit_e direction, global_s *global);

// End |||||||||||||||||||||||||||||||||||||||||||||
