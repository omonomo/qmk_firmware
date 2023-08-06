#pragma once

#include QMK_KEYBOARD_H
#include "main_keymap.h"

// 型定義 #############################################
typedef enum { // 行
	NN_GYOU, LT_GYOU, LY_GYOU, LW_GYOU, LX_GYOU, X_GYOU, K_GYOU, S_GYOU, T_GYOU, N_GYOU, H_GYOU, M_GYOU, Y_GYOU, R_GYOU, W_GYOU,
	 V_GYOU,  G_GYOU,  Z_GYOU,  D_GYOU,  B_GYOU, P_GYOU, J_GYOU, Q_GYOU, F_GYOU, C_GYOU
} gozyuon_gyou_e;

typedef enum { // 段
	A_DAN, I_DAN, U_DAN, E_DAN, O_DAN
} gozyuon_dan_e;

// 関数プロトタイプ宣言 #############################################
bool pr_romazi_output(uint16_t keycode, keyrecord_t *record, global_s *global);
bool pr_romazi_input(uint16_t keycode, keyrecord_t *record, global_s *global);

// End |||||||||||||||||||||||||||||||||||||||||||||
