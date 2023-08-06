#pragma once

#include QMK_KEYBOARD_H
#include "main_keymap.h"

// 関数置き替え #############################################
//#define RST_SFT rst_sft(global)

//#define SET_SFT_UP_OVERRIDE(code) set_sft_up_override(code)
//#define RST_SFT_UP_OVERRIDE set_sft_up_override(0)

//#define RST_OVERRIDE rst_override(&override_keycode, global)

// 関数プロトタイプ宣言 #############################################
//void rst_sft(global_s *global);
//void rst_override(uint16_t *override_keycode, global_s *global);
//void set_sft_up_override(uint16_t reg_key);

void ms_key_override(void);

bool pr_key_override(uint16_t keycode, keyrecord_t *record, global_s *global);

// End |||||||||||||||||||||||||||||||||||||||||||||
