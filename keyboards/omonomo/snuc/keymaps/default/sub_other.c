#include "sub_other.h"

#include "func_edit_text.h"
#include "func_key_output.h"
#include "func_mod_output.h"
#include "func_tap_replace.h"
#include "main_code_layer.h"
#include "main_keymap.h"

// 関数置き替え #############################################
#define CHANGE_WIDTH(width) change_width(width, &letter_width, global)
#define R_CHANGE_WIDTH(width) CHANGE_WIDTH(width); return

// 関数プロトタイプ宣言 #############################################
static void change_width(letter_width_e after_width, letter_width_e *letter_width, global_s *global);

// 補助関数 |||||||||||||||||||||||||||||||||||||||||||||

// 全角・半角切り替え #############################################
// 引数: 変更する文字幅、現在の文字幅のアドレス
static void change_width(letter_width_e after_width, letter_width_e *letter_width, global_s *global) {
	switch (after_width) {
		case ROMAN:
		case HALF:
			if (IS_HALF_SYMBOL) {
				if (*letter_width == FULL || *letter_width == SYMBOL) {
					UNREGISTER_MODS(_S _A);
					TAP_CODE(KK_EISU);
					REGISTER_MODS_IF_PRESS(_S _E _A);
				} // letter_width
			} // IS_HALF_SYMBOL
		break;

		case FULL:
		case SYMBOL:
			if (IS_HALF_SYMBOL) {
				if (*letter_width == HALF || *letter_width == ROMAN) {
					UNREGISTER_MODS(_S _A);
					TAP_CODE(KK_KANA);
					REGISTER_MODS_IF_PRESS(_S _E _A);
				} // letter_width
			} // IS_HALF_SYMBOL
		break;

		default:
		break;
	}

	*letter_width = after_width;
	return;
}

// サブルーチン関数 |||||||||||||||||||||||||||||||||||||||||||||

// テンキー・記号入力時英数切り替え #############################################
void pr_change_en(uint16_t keycode, keyrecord_t *record, global_s *global) {
	if (IS_GAME_MODE) return;
	if (IS_LEGACY_EJ) return; // ALT+GRVはトグルでモード不確定のためキャンセル

	static letter_width_e letter_width = HALF; // 文字幅

	if (IS_KEY_PRESS) {
		switch (keycode) {
			case KC_PSLS ... KC_PDOT: // 全・半角切り替えモードに関係なく常に半角
				if (IS_MOD_PRESS_EX(_P _F _M)) return;
				if (LAST_KEYCODE < KC_PSLS || KC_PDOT < LAST_KEYCODE) { // 確実に半角にするため、異なる条件で判定
					TAP_CODE(KK_EISU);
				} // LAST_KEYCODE
				letter_width = HALF;
				return;
			break;

			case KC_APP: // 000を出力する場合常に半角
				if (IS_OTHER_MOD_PRESS_ONLY(_F)) {
					TAP_CODE(KK_EISU);
					letter_width = HALF;
				} // IS_OTHER_MOD_PRESS_ONLY
				return;
			break;

			default:
			break;
		}

		if (IS_ROMAZI_OFF) return; // ROMAZI_OFFでキャンセル
// 日本語入力モード時のみ ---------------------------------------------
		if (IS_WIN ? IS_OTHER_MOD_PRESS_EX(_S _E _B) : IS_OTHER_MOD_PRESS_EX(_S _E _A _B)) return; // 特定のMOD以外を押していたらそのまま
		const bool IS_SAB_PRESS = IS_WIN ? IS_OTHER_MOD_PRESS(_S _B) : IS_OTHER_MOD_PRESS(_S _A _B); // 全角出力するMODキー

		switch (keycode) {
			case RM_A ... RM_TEN: // 常に全角
				R_CHANGE_WIDTH(FULL);
			break;

			case KC_A ... KC_Z: // SFT_SPCありで半角、その後解除まで半角、それ以外全角
				if (letter_width == ROMAN) return;
				if (IS_MOD_PRESS(_E)) {
					R_CHANGE_WIDTH(ROMAN);
				} // IS_MOD_PRESS
				R_CHANGE_WIDTH(FULL);
			break;

			case KC_LBRC: case KC_RBRC: // MODなしで全角
			case KC_COMM: case KC_DOT: // MODなし、数字の後以外で全角
				if (IS_SAB_PRESS) {
					R_CHANGE_WIDTH(FULL);
				} // IS_SAB_PRESS
				if (letter_width == ROMAN) return;
				if ((keycode == KC_COMM || keycode == KC_DOT)
				&&  (KC_1 <= LAST_KEYCODE && LAST_KEYCODE <= KC_0)) return;
				if (!IS_ANY_MOD_PRESS) {
					R_CHANGE_WIDTH(FULL);
				} // IS_ANY_MOD_PRESS
			break;

			case KC_SCLN: // MODなしで全角、その後に押したキーは全角
			case KC_GRV:
				if (IS_SAB_PRESS) {
					R_CHANGE_WIDTH(FULL);
				} // IS_SAB_PRESS
				if (letter_width == ROMAN) return;
				if (!IS_ANY_MOD_PRESS) {
					R_CHANGE_WIDTH(SYMBOL);
				} // IS_ANY_MOD_PRESS
			break;

			case KC_1 ... KC_0: // ;`の後、またはMODなしで全角
			case KC_EQL:
			case KC_BSLS:
			case KC_SLSH:
			case KC_MINS: // ;`の後、またはROMAZI_INの時MODなしで全角
			case KC_QUOT: // ;`の後、またはROMAZI_NOの時MODなしで全角
				if (IS_SAB_PRESS) {
					R_CHANGE_WIDTH(FULL);
				} // IS_SAB_PRESS
				if (letter_width == ROMAN) return;
				if (!IS_ANY_MOD_PRESS) {
					if (letter_width == SYMBOL
					|| (keycode == KC_MINS && IS_ROMAZI_IN_ON)
					|| (keycode == KC_QUOT && IS_ROMAZI_NO_ON)) {
						R_CHANGE_WIDTH(FULL);
					} // keycode
				} // IS_ANY_MOD_PRESS
			break;

			case MT_LSFT_SPC:
			case MT_RSFT_SPC:
				return;
			break;

			default:
			break;
		}

		letter_width -= letter_width % 2; // いずれの条件にも当てはまらない場合 ROMAN→HALF、SYMBOL→FULL
	} else { // IS_KEY_PRESS
		switch (keycode) {
			case MT_LGUI_EISU: // TAPした時に半角設定
				if (IS_MOD_PRESS_EX(_S _E)) return;
				if (IS_TAP) {
					letter_width = HALF;
				} // IS_TAP
				return;
			break;

			case MT_RGUI_KANA: // TAPした時に全角設定
				if (IS_MOD_PRESS_EX(_S _E)) return;
				if (IS_TAP) {
					letter_width = FULL;
				} // IS_TAP
				return;
			break;

			default:
			break;
		}
	} // IS_KEY_PRESS

	return;
}

// テンキー・Hyper #############################################
bool pr_numeric_hyper(uint16_t keycode, keyrecord_t *record, global_s *global) {
// キー置き換え ---------------------------------------------
	switch (keycode) {
		case KC_BSPC:
			if (IS_MOD_PRESS_COMP(_P)) {
				if (IS_KEY_PRESS) { // APP押しながらで行頭まで削除
					RF_KEY_SEQUENCE(SLCT_PARA_TOP DEL_TEXT);
				} // IS_KEY_PRESS
			} else if (IS_MOD_PRESS(_F)) { // FNを押しながらで DEL
				RF_REPLACE_KEY16(KC_DEL);
			} // IS_MOD_PRESS
		break;

		case KC_CAPS: // LFN押しながらで 英数→CAPS
			if (IS_WIN) {
				if (IS_MOD_PRESS(_LF)) {
					if (IS_KEY_PRESS) {
						RF_TAP_CODE16(S(KC_CAPS));
					} // IS_KEY_PRESS
				} // IS_MOD_PRESS
			} // IS_WIN
		break;

		case KC_DEL:
			if (IS_MOD_PRESS_COMP(_P)) {
				if (IS_KEY_PRESS) { // APP押しながらで行末まで削除
					RF_KEY_SEQUENCE(SLCT_PARA_BTM DEL_TEXT);
				} // IS_KEY_PRESS
			} else if (IS_MOD_PRESS(_F _M)) { // FNかMETを押しながらで INS or CLEAR
				RF_REPLACE_KEY16(IS_WIN ? KC_INS : KC_NUM);
			} // IS_MOD_PRESS
		break;

		case KC_APP: // 単独でタップした時のみメニューを出す
			if (IS_GAME_MODE) return true;
			if (IS_KEY_PRESS) {
				MOD_DIFFERENT_ON;
				if (IS_OTHER_MOD_PRESS_ONLY(_F)) { // FNを押しながらで 000
					KEY_PRESS_AFTER_MOD_ON;
					RF_REPEAT_KEY(KC_P0, 3);
				} // IS_OTHER_MOD_PRESS_ONLY
				return false;
			} else { // IS_KEY_PRESS
				if (!IS_KEY_PRESS_AFTER_MOD) {
					REGISTER_CODE(KC_APP);
				} // IS_KEY_PRESS_AFTER_MOD
			} // IS_KEY_PRESS
		break;

		case KC_A ... KC_Z: // FN単独で文字キーを押した場合Hyperタップ
		case KC_LBRC ... KC_SLASH:
			if (IS_MOD_PRESS_ONLY(_F)) {
				if (IS_KEY_PRESS) {
					RF_TAP_CODE16(X_HYPR(keycode));
				} // IS_KEY_PRESS
			} // IS_MOD_PRESS_ONLY
		break;

		default:
		break;
	}

	return true;
}

// End |||||||||||||||||||||||||||||||||||||||||||||
