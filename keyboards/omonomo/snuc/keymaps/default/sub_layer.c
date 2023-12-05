#include "sub_layer.h"

#include "func_mod_output.h"
#include "func_tap_replace.h"
#include "main_code_layer.h"
#include "main_keymap.h"

// サブルーチン関数 |||||||||||||||||||||||||||||||||||||||||||||

// レイヤー変更 #############################################
layer_state_t ls_change(layer_state_t state, global_s *global) {
	if (IS_LAYER_OFF_STATE(state, _MIZU)) { // _MIZU ONで他のローマ字レイヤーOFF
		state &= ~((1 << _MIZU_LD) | (1 << _MIZU_LS) | (1 << _MIZU_RS) | (1 << _MIZU_RD));
	}

	if (IS_LAYER_ON_STATE(state, _RFNC)) { // _RFNC ONで_LFNCもON
		state |= 1 << _LFNC;
	} else if (!IS_MOD_PRESS(_LF)) {
		state &= ~(1 << _LFNC);
	}

	if (IS_LAYER_ON_STATE(state, _LFNC)) { // _LFNC ONで_GAMEもON
		state |= 1 << _GAME;
	} else if (!IS_GAME_MODE) {
		state &= ~(1 << _GAME);
	}

	return state;
}

// ワンショットレイヤー #############################################
bool pr_layer_one_shot(uint16_t keycode, keyrecord_t *record, global_s *global) {
	if (!IS_ROMAZI_OUT_ON) return true;

	if (IS_KEY_PRESS) {
		switch (keycode) { // レイヤーON
			case OL_MIZU_LD:
				layer_or(LAYER_MASK(_MLD));
				layer_and(LAYER_MASK_EX(_MLS _MRS _MRD)); // 他のローマ字レイヤーをOFF
				return false;
			break;

			case OL_MIZU_LS:
				layer_or(LAYER_MASK(_MLS));
				layer_and(LAYER_MASK_EX(_MLD _MRS _MRD));
				return false;
			break;

			case OL_MIZU_RS:
				layer_or(LAYER_MASK(_MRS));
				layer_and(LAYER_MASK_EX(_MLD _MLS _MRD));
				return false;
			break;

			case OL_MIZU_RD:
				layer_or(LAYER_MASK(_MRD));
				layer_and(LAYER_MASK_EX(_MLD _MLS _MRS));
				return false;
			break;

			default:
			break;
		}
	} else { // IS_KEY_PRESS
		switch (keycode) {
			case OL_MIZU_LD ... OL_MIZU_RD: // レイヤーを保持するキー(ワンショットレイヤーキー)
			break;

			default: // 上記以外のキーは離した時にレイヤーOFF(押した時に解除だとRM_H等が出力されない)
				layer_and(LAYER_MASK_EX(_MLD _MLS _MRS _MRD)); // MODキーは押した時に別処理でレイヤーOFF済
			break;
		}
	} // IS_KEY_PRESS
	return true;
}

// レイヤータップ #############################################
bool pr_layer_tap(uint16_t keycode, keyrecord_t *record) {
	switch (keycode) {
		case LT_LFNC_ESC:
			RF_LAYER_TAP16(KC_ESC, _LFNC);
		break;

		case LT_RFNC_ESC:
			RF_LAYER_TAP16(KC_ESC, _RFNC);
		break;

		case LT_MODE_PENT:
			RF_LAYER_TAP16(KC_PENT, _MODE);
		break;

		case LT_CRSR_TAB:
			RF_LAYER_TAP16(KC_TAB, _CURSOR);
		break;

		default:
		break;
	}
	return true;
}

// レイヤー切り替え #############################################
void pr_layer_change(global_s *global) {
	if (IS_ROMAZI_OUT_ON && !IS_ANY_MOD_PRESS) {// MODを押したときはローマ字出力無効
		layer_on(_MIZU);
	} else { // IS_ROMAZI_OUT_ON
		layer_off(_MIZU);
	} // IS_ROMAZI_OUT_ON

	if (IS_MOD_PRESS(_F _B) // FNかテンキーTABを押しているか、
	|| (!IS_MOD_PRESS(_M) && IS_MOD_PRESS_EX(_M _P))) {// METを押さずにAPP以外のMODを押したときは_CURSORレイヤーON
		layer_on(_CURSOR);
	} else { // IS_MOD_PRESS_EX
		layer_off(_CURSOR);
	} // IS_MOD_PRESS_EX

	if (IS_WIN ? IS_MOD_PRESS_EX(_S _E _B) : IS_MOD_PRESS_EX(_S _E _A _B)) { // 機能キーを出力するときはQWERTY配列
		layer_on(_QWERTY);
	} else if (!IS_QWERTY) { // IS_MOD_PRESS_EX
		layer_off(_QWERTY);
	} // IS_QWERTY
	return;
}

// End |||||||||||||||||||||||||||||||||||||||||||||
