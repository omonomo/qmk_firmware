#pragma once

#include QMK_KEYBOARD_H
#include "main_keymap.h"

// レイヤーマスク #############################################
#define  _MLD | (1 << _MIZU_LD)
#define  _MLS | (1 << _MIZU_LS)
#define  _MRS | (1 << _MIZU_RS)
#define  _MRD | (1 << _MIZU_RD)

#define LAYER_MASK(flag)      (0 flag) // 指定したレイヤー
#define LAYER_MASK_EX(flag) (~(0 flag)) //指定したレイヤー以外

// 関数プロトタイプ宣言 #############################################
layer_state_t ls_change(layer_state_t state, global_s *global);

bool pr_layer_one_shot(uint16_t keycode, keyrecord_t *record, global_s *global);
bool pr_layer_tap(uint16_t keycode, keyrecord_t *record);
void pr_layer_change(global_s *global);

// End |||||||||||||||||||||||||||||||||||||||||||||
