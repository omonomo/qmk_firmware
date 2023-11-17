#include "sub_flag_mode.h"

#include "ini_config.h"
#include "func_mod_output.h"
#include "func_tap_replace.h"
#include "sub_counter.h"
#include "main_code_layer.h"
#include "main_keymap.h"

// フラグ・変数 #############################################
static uint8_t press_key_num = 0; // 押している通常キーの数
static bool wakefl_mode = false; // 不眠モード

// サブルーチン関数 |||||||||||||||||||||||||||||||||||||||||||||

// 不眠モード実行 #############################################
void ms_wakeup(global_s *global) {
	if (!IS_WAKEFUL) return;

	if (REPEAT_COUNT == 0) { // 不眠させるため、定期的にLCTLをタップ
		TAP_CODE(KC_LCTL);
		SET_REPEAT_COUNT(WAKEUP_TH); // たたき起こし間隔を再セット
	} // REPEAT_COUNT
	return;
}

// MOD押下フラグ #############################################
void pr_p_mod_flag(uint16_t keycode, keyrecord_t *record, global_s *global) {
	const uint16_t CHANGE_FLAG = CODE_TO_P_MOD_FLAG(keycode);

	if (IS_KEY_PRESS) {
		PC_SLEEPY; // 不眠モード解除
		LAST2_KEYCODE = LAST_KEYCODE; // 2回前に押したキーを記憶
		LAST_KEYCODE  = NOW_KEYCODE; // 直前に押したキーを記憶
		NOW_KEYCODE   = keycode;
		ANY_OTHER_MOD_PRESS = ANY_MOD_PRESS; // 変化前のフラグを保存
		ANY_MOD_PRESS |= CHANGE_FLAG; // p_mod_flag更新
		++press_key_num; // 押しているキーの数
	} else { // IS_KEY_PRESS
		ANY_MOD_PRESS &= ~CHANGE_FLAG;
		ANY_OTHER_MOD_PRESS = ANY_MOD_PRESS; // 変化後のフラグを保存
		press_key_num -= (press_key_num == 0 ? 0 : 1); // 念のため、0から引かないようにする
		if (CHANGE_FLAG > 0) {
			NOW_KEYCODE = keycode; // ローマ字の誤出力防止(last_keycodeをアルファベット以外にする)
		} // CHANGE_FLAG
	} // IS_KEY_PRESS
	return;
}

// Windows対応 #############################################
void pr_windows(uint16_t keycode, keyrecord_t *record, global_s *global) {
	if (IS_GAME_MODE) return;

	switch (keycode) {
		case KC_LALT: // ALTを単独でタップした時のみメニューをハイライトさせる
		case KC_RALT:
			if (IS_KEY_PRESS) {
				KEY_PRESS_AFTER_MOD_OFF; // 押した時にfalse
				SET_MOD_TAP;
			} else { // IS_KEY_PRESS
				if (IS_MOD_PRESS(_F) // FNを押している、または単独で短押しならtrue
				|| (IS_TAP && press_key_num == 0)) {
					KEY_PRESS_AFTER_MOD_ON;
				} else { // IS_TAP
					KEY_PRESS_AFTER_MOD_OFF;
				} // IS_TAP
			} // IS_KEY_PRESS
		break;

		case DT_LCTL_HYPR:
		case DT_RCTL_HYPR:
			if (!GUI_IS_CTL || IS_MOD_PRESS_EX(_C)) { // GUIをレジストしない、または単独でなければtrue
				KEY_PRESS_AFTER_MOD_ON;
			} else if (IS_KEY_PRESS) { // それ以外の条件で押した時false
				KEY_PRESS_AFTER_MOD_OFF;
			} // IS_KEY_PRESS
		break;

		case MT_LGUI_EISU:
		case MT_RGUI_KANA:
			if (GUI_IS_CTL || IS_MOD_PRESS_EX(_G)) {
				KEY_PRESS_AFTER_MOD_ON;
			} else if (IS_KEY_PRESS) {
				KEY_PRESS_AFTER_MOD_OFF;
			} // IS_KEY_PRESS
		break;

		case KC_APP: // 単独でタップした時のみメニューを出す
			if (IS_KEY_PRESS) {
				if (1 < press_key_num) { // 単独でなければtrue、単独でfalse
					KEY_PRESS_AFTER_MOD_ON;
				} else { // press_key_num
					KEY_PRESS_AFTER_MOD_OFF;
				} // press_key_num
				SET_MOD_TAP;
			} else { // IS_KEY_PRESS
				if (!IS_TAP) { // 短押しでなければtrue
					KEY_PRESS_AFTER_MOD_ON;
				} // IS_TAP
			} // IS_KEY_PRESS
		break;

		default:
			KEY_PRESS_AFTER_MOD_ON; // 上記以外のキーを操作した場合はtrue
		break;
	}
	return;
}

// モード変更 #############################################
bool pr_mode_change(uint16_t keycode, keyrecord_t *record, global_s *global) {
	if (!IS_KEY_PRESS) return true;

	switch (keycode) {
// キーボードタイプ切り替え ---------------------------------------------
		case CK_JIS:
			KEYBOARD_JIS;
		break;

		case CK_US:
			KEYBOARD_US;
		break;
// キー配列切り替え ---------------------------------------------
		case CK_COL:
			LAYOUT_COLEMAK;
			GAME_MODE_OFF;
			ROMAZI_OFF;
			TAP_CODE(KK_EISU);
		break;

		case CK_QWTY:
			LAYOUT_QWERTY;
			GAME_MODE_OFF;
			ROMAZI_OFF;
			TAP_CODE(KK_EISU);
		break;

		case CK_GAME:
			LAYOUT_QWERTY;
			GAME_MODE_ON;
			ROMAZI_OFF;
			TAP_CODE(KK_EISU);
		break;
// 各OS対応モード切り替え ---------------------------------------------
		case CK_WIN:
			TAP_CODE(KC_F21); // USBtoBT+用
			WIN_DEFAULT;
		break;

		case CK_MAC:
			TAP_CODE(KC_F22);
			MAC_DEFAULT;
		break;

		case CK_IPHN:
		case CK_IPAD:
			TAP_CODE(keycode == CK_IPAD ? KC_F23 : KC_F24);
			IOS_DEFAULT;
		break;
// ローマ字入出力切り替え ---------------------------------------------
		case CK_ROUT:
			ROMAZI_OUT_OFF;
		break;

		case CK_RIN:
			ROMAZI_IN_OFF;
		break;

		case CK_ALPH:
			ROMAZI_NO_OFF;
		break;
// IME ON/OFF方法切り替え ---------------------------------------------
		case CK_LEJ:
			LEGACY_EJ;
		break;

		case CK_MEJ:
			MODERN_EJ;
		break;
// GUItoCTL切り替え ---------------------------------------------
		case CK_GC:
			GUI_TO_CTL;
		break;

		case CK_GG:
			GUI_TO_GUI;
		break;
// テキスト編集機能切り替え ---------------------------------------------
		case CK_TON:
			TEXT_EDIT_ON;
		break;

		case CK_TOFF:
			TEXT_EDIT_OFF;
		break;

// 数字・記号半角出力機能切り替え ---------------------------------------------
		case CK_HALF:
			HALF_SYMBOL_ON;
		break;

		case CK_FULL:
			HALF_SYMBOL_OFF;
		break;

// 電話応対モード切り替え ---------------------------------------------
		case CK_TEL:
			LAYOUT_QWERTY;
			GAME_MODE_OFF;
			if (IS_IOS) {
				ROMAZI_NO_OFF;
			} else {
				ROMAZI_IN_OFF;
			}
		break;

// 不眠モードON ---------------------------------------------
		case CK_WKUP:
			PC_WAKEFUL;
			SET_REPEAT_COUNT(0);
		break;

		default:
			return true; // 上記に該当しないキーはreturn true
		break;
	}
	return false;
}

// End |||||||||||||||||||||||||||||||||||||||||||||
