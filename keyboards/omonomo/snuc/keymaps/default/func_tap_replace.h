#pragma once

#include QMK_KEYBOARD_H
#include "main_keymap.h"

// 関数置き替え #############################################
#define SET_MOD_TAP set_mod_tap(keycode)
#define RST_MOD_TAP set_mod_tap(0)

#define IS_TAP is_tap(keycode)

#define REPLACE_MOD(mod) replace_mod(mod, record)
#define RF_REPLACE_MOD(mod) REPLACE_MOD(mod); return false

#define REPLACE_KEY16(code) replace_key16(code, record)
#define RF_REPLACE_KEY16(code) REPLACE_KEY16(code); return false
#define RST_REPLACE_KEY16 REPLACE_KEY16(0)

#define REPLACE_KEY16_UJ(codeU, codeJ) REPLACE_KEY16(!IS_JIS ? codeU : codeJ)
#define RF_REPLACE_KEY16_UJ(codeU, codeJ) REPLACE_KEY16_UJ(codeU, codeJ); return false

#define UP_MOD_REPLACE_KEY16(code, flag1, flag2) up_mod_replace_key16(code, flag1, flag2, record, global)
#define R_UP_MOD_REPLACE_KEY16(code, flag1, flag2) return UP_MOD_REPLACE_KEY16(code, flag1, flag2)
#define RF_UP_MOD_REPLACE_KEY16(code, flag1, flag2) UP_MOD_REPLACE_KEY16(code, flag1, flag2); return false
#define RST_UP_MOD_REPLACE_KEY16 UP_MOD_REPLACE_KEY16(0, 0, 0)

#define UP_MET_REPLACE_KEY16(code) UP_MOD_REPLACE_KEY16(code, MOD_MASK(_M), MOD_MASK(_C _T _G))
#define R_UP_MET_REPLACE_KEY16(code) return UP_MET_REPLACE_KEY16(code)

#define REPLACE_MOD_TAP_KEY16(code, mod, flag1, flag2) replace_mod_tap_key16(code, mod, flag1, flag2, keycode, record, global)
#define RST_REPLACE_MOD_TAP_KEY16 REPLACE_MOD_TAP_KEY16(0, 0, 0, 0)

#define MOD_TAP16(code, mod) mod_tap16(code, mod, keycode, record, global)
#define RF_MOD_TAP16(code, mod) MOD_TAP16(code, mod); return false

#define DOUBLE_MOD_TAP16(code, mod) double_mod_tap16(code, mod, keycode, record, global)
#define RF_DOUBLE_MOD_TAP16(code, mod) DOUBLE_MOD_TAP16(code, mod); return false

#define DOUBLE_KEY_TAP16(code, flag1, flag2) double_key_tap16(code, flag1, flag2, keycode, record, global)
#define RF_DOUBLE_KEY_TAP16(code, flag1, flag2) DOUBLE_KEY_TAP16(code, flag1, flag2); return false

#define LAYER_TAP16(code, layer) layer_tap16(code, layer, keycode, record)
#define RF_LAYER_TAP16(code, layer) LAYER_TAP16(code, layer); return false

// 関数プロトタイプ宣言 #############################################
void set_mod_tap(uint16_t mod_key);
bool is_tap(uint16_t keycode);

bool replace_mod(uint8_t rep_mod, keyrecord_t *record);
bool replace_key16(uint16_t rep_key, keyrecord_t *record);
bool up_mod_replace_key16(uint16_t rep_key, uint16_t rp_mod_mask, uint16_t reg_mod_mask, keyrecord_t *record, global_s *global);
bool replace_mod_tap_key16(uint16_t tap_key, uint8_t rep_mod, uint16_t rp_mod_mask, uint16_t np_mod_mask, uint16_t keycode, keyrecord_t *record, global_s *global);
bool mod_tap16(uint16_t tap_key, uint8_t hold_mod, uint16_t keycode, keyrecord_t *record, global_s *global);
bool double_mod_tap16(uint16_t tap_key, uint8_t hold_mod, uint16_t keycode, keyrecord_t *record, global_s *global);
bool double_key_tap16(uint16_t tap_key, uint16_t rp_mod_mask, uint16_t np_mod_mask, uint16_t keycode, keyrecord_t *record, global_s *global);
bool layer_tap16(uint16_t tap_key, uint8_t hold_layer, uint16_t keycode, keyrecord_t *record);

// End |||||||||||||||||||||||||||||||||||||||||||||
