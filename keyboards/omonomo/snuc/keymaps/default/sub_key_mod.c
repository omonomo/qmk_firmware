#include "sub_key_mod.h"

#include "ini_config.h"
#include "func_key_output.h"
#include "func_mod_output.h"
#include "func_edit_text.h"
#include "func_tap_replace.h"
#include "sub_counter.h"
#include "main_code_layer.h"
#include "main_keymap.h"

// フラグ・変数 #############################################
static uint8_t active_macro = 0; // 繰り返しマクロ(キー)コード
static bool second_prefix = false; // 2次プレフィクス

// 補助関数 |||||||||||||||||||||||||||||||||||||||||||||

// 繰り返しマクロセット関数 #############################################
// 引数: 有効にするマクロキーコード
void set_macro(uint8_t reg_key) {
	active_macro = reg_key;
	return;
}

// サブルーチン関数 |||||||||||||||||||||||||||||||||||||||||||||

// 繰り返しマクロ実行 #############################################
void ms_repeat_macro(global_s *global) {
	if (active_macro == 0) return;
	if (REPEAT_COUNT != 0) return;

	switch (active_macro) { // REPEAT_COUNT == 0 で実行とカウントリセット
		case KC_R:
			repeat_key(KC_UP, IS_MOD_PRESS(_T) ? CURSOR_MVS : CURSOR_MVL);
		break;

		case KC_C:
			repeat_key(KC_DOWN, IS_MOD_PRESS(_T) ? CURSOR_MVS : CURSOR_MVL);
		break;

		default:
		break;
	}

	SET_REPEAT_COUNT(MACRO_TH); // MACRO_THの間隔でマクロ実行
	return;
}

// モッドタップ等解除 #############################################
void pr_cancel(uint16_t keycode, keyrecord_t *record, global_s *global) {
	if (!IS_KEY_PRESS) return;

	if (IS_MOD_DIFFERENT) { // 押しているMODキーとレジストされているMODが異なる場合に元に戻す
		switch (keycode) {
			case KC_BSPC: case KC_TAB: // ただしAPP+テンキー、MODキー等の場合はAPPをレジストしない
			case KC_INS ... KC_PDOT:
			case KC_LALT: case KC_RALT:
			case CK_MET:
			case MT_LSFT_SPC ... LT_MODE_PENT:
				if (!IS_MOD_PRESS(_P)) {
					REGISTER_OTHER_MODS_IF_PRESS(_ALL);
				}
			break;

			default:
				if (IS_MOD_PRESS_COMP(_P) || !IS_MOD_PRESS(_P)) {
					REGISTER_OTHER_MODS_IF_PRESS(_ALL);
				}
			break;
		}
		MOD_DIFFERENT_OFF;
	} // IS_MOD_DIFFERENT

	switch (keycode) {
		case CK_MET: // タップを優先
		case MT_LSFT_SPC ... MT_RCTL_ENT: // 連続タップ機能のため、モッドタップ状態を維持
		case DT_LCTL_HYPR ... DT_RSFT_HYPR: // ダブルタップキーはモッドタップ状態を維持
			return;
		break;

		case KC_Q: // GUIのみを押している場合はモッドタップ状態を維持
			if (IS_MOD_PRESS_ONLY(_G)) return;
		break;

		default:
		break;
	}

	RST_MOD_TAP; // 上記以外の条件の場合は解除
	return;
}

// MOD置き替え #############################################
bool pr_mod_replace(uint16_t keycode, keyrecord_t *record, global_s *global) {
	if (IS_GAME_MODE) return true;

	switch (keycode) {
// キーダブルタップ ---------------------------------------------
		case KC_Q: // ダブルタップでQuit(条件不一致の場合ruturn trueでKC_Qをレジスト)
			if (IS_IOS) return true;
			return DOUBLE_KEY_TAP16((IS_WIN ? A(KC_F4) : KC_Q), MOD_MASK(_G), MOD_MASK_EX(_G));
		break;

// GUI関係 ---------------------------------------------
		case KC_TAB: // WindowsにてGUI(CTL)+TAB→ALT+TAB
			if (!IS_WIN) return true;
			if (!REPLACE_MOD_TAP_KEY16(KC_TAB, KC_LALT, MOD_MASK(_G), MOD_MASK_EX(_S _E _G))) return false;
		break;

		case KC_GRV: // WindowsにてGUI(CTL)+GRV→ALT+ESC
			if (!IS_WIN) return true;
			if (!REPLACE_MOD_TAP_KEY16(KC_ESC, KC_LALT, MOD_MASK(_G), MOD_MASK_EX(_S _E _G))) return false;
		break;

		case MT_LGUI_EISU: // ウインドウスイッチャー(Windows ALT+TAB)、アプリケーションスイッチャー(macOS・iOS GUI+TAB)
			if (!REPLACE_MOD_TAP_KEY16(S(KC_TAB), (IS_WIN ? KC_LALT :  KC_LGUI), MOD_MASK(_RG), MOD_MASK_EX(_G))) return false;
		break;

		case MT_RGUI_KANA:
			if (!REPLACE_MOD_TAP_KEY16(  KC_TAB , (IS_WIN ? KC_LALT : KC_LGUI), MOD_MASK(_LG), MOD_MASK_EX(_G))) return false;
		break;

		default:
		break;
	}

if (IS_MOD_PRESS(_G)) return true; // スイッチャー起動中に誤動作を起こさせないため
// GUI以外 ---------------------------------------------
	switch (keycode) {
		case DT_LSFT_HYPR: // タブ移動(CTL+TAB)
			if (!REPLACE_MOD_TAP_KEY16(S(KC_TAB), KC_LCTL, MOD_MASK(_RS), MOD_MASK_EX(_S))) return false;
		break;

		case DT_RSFT_HYPR:
			if (!REPLACE_MOD_TAP_KEY16(  KC_TAB,  KC_LCTL, MOD_MASK(_LS), MOD_MASK_EX(_S))) return false;
		break;

		case KC_LALT: // ウインドウ切り替え(ALT+ESC or GUI+GRV)
			if (!REPLACE_MOD_TAP_KEY16((IS_WIN ? S(KC_ESC) : S(KC_GRV)), (IS_WIN ? KC_LALT : KC_LGUI), MOD_MASK(_RA), MOD_MASK_EX(_A))) return false;
		break;

		case KC_RALT:
			if (!REPLACE_MOD_TAP_KEY16((IS_WIN ?   KC_ESC  :   KC_GRV ), (IS_WIN ? KC_LALT : KC_LGUI), MOD_MASK(_LA), MOD_MASK_EX(_A))) return false;
		break;

		default:
		break;
	}

	if (IS_KEY_PRESS) {
		RST_REPLACE_MOD_TAP_KEY16; // 上記以外の条件でキーを押した場合はMODを元に戻す
	} // IS_KEY_PRESS
	return true;
}

// 2次プレフィクス #############################################
bool pr_second_prefix(uint16_t keycode, keyrecord_t *record, global_s *global) {
	if (!IS_SECOND_PREFIX) return true;
	if (!IS_KEY_PRESS) return true;

	switch (keycode) { //MOD条件なし
		case CS_LSFT: case MT_LSFT_SPC: // MET、SFT、SFT_SPC以外を押すと2次プレフィクス解除
		case CS_RSFT: case MT_RSFT_SPC:
		case CK_MET:
		break;

		default:
			SECOND_PREFIX_OFF;
		break;
	}

	if (IS_MOD_PRESS(_M)) {
		if (!IS_MOD_PRESS_EX(_S _E _M)) { // MET必須、SFTオプション、その他禁止
			switch (keycode) {
				case KC_E: // カーソル移動
					RF_KEY_SEQUENCE(MOVE_PARA_TOP);
				break;

				case KC_S:
				case KC_A:
					RF_KEY_SEQUENCE(MOVE_LINE_TOP);
				break;

				case KC_X:
					RF_KEY_SEQUENCE(MOVE_PARA_BTM);
				break;

				case KC_D:
				case KC_F:
					RF_KEY_SEQUENCE(MOVE_LINE_BTM);
				break;

				case KC_R:
					RF_KEY_SEQUENCE(MOVE_DOC_TOP);
				break;

				case KC_C:
					RF_KEY_SEQUENCE(MOVE_DOC_BTM);
				break;

				case KC_J: // 選択(SFTを押しながらだとキャレットが先頭側)
					if (IS_MOD_PRESS(_S _E)) {
						UNREGISTER_MODS(_S);
						MOD_DIFFERENT_ON;
						RF_KEY_SEQUENCE(SLCT_WORD_B);
					} else { // IS_MOD_PRESS
						RF_KEY_SEQUENCE(SLCT_WORD);
					} // IS_MOD_PRESS
				break;

				case KC_K:
					if (IS_MOD_PRESS(_S _E)) {
						UNREGISTER_MODS(_S);
						MOD_DIFFERENT_ON;
						RF_KEY_SEQUENCE(SLCT_PARA_B);
					} else { // IS_MOD_PRESS
						RF_KEY_SEQUENCE(SLCT_PARA);
					} // IS_MOD_PRESS
				break;

				default:
				break;
			}
		} // IS_MOD_PRESS_EX
		if (!IS_MOD_PRESS_EX(_M)) { // MET必須、その他禁止
			switch (keycode) {
				case KC_T: // 削除
					RF_KEY_SEQUENCE(SLCT_PARA_BTM DEL_TEXT);
				break;

				case KC_Y:
					RF_KEY_SEQUENCE(SLCT_PARA_TOP DEL_TEXT);
				break;

				case KC_P: // 行結合
					RF_KEY_SEQUENCE(MOVE_PARA_BTM DEL_TEXT);
				break;

				case KC_COMM: // 行挿入後カーソル移動
					RF_KEY_SEQUENCE(MOVE_PARA_TOP TAP_ENT MOVE_PARA_BTM);
				break;

				case KC_DOT:
					RF_KEY_SEQUENCE(MOVE_PARA_BTM MOVE_RGHT TAP_ENT MOVE_UP MOVE_UP MOVE_PARA_BTM);
				break;

				default:
				break;
			}
		} // IS_MOD_PRESS_EX
	} // IS_MOD_PRESS
	if (IS_MOD_PRESS(_S _E)) {
		if (!IS_MOD_PRESS_EX(_S _E _M)) { // SFT必須、METオプション、その他禁止
			switch (keycode) {
				case KC_9: // (..)
					UNREGISTER_MODS(_S);
					MOD_DIFFERENT_ON;
					RF_HOLD_TEXT16_UJ(S(KC_9), S(KC_0), S(KC_8), S(KC_9));
				break;

				case KC_LBRC: // {..}
					UNREGISTER_MODS(_S);
					MOD_DIFFERENT_ON;
					RF_HOLD_TEXT16_UJ(S(KC_LBRC), S(KC_RBRC), S(KC_RBRC), S(KC_NUHS));
				break;

				case KC_QUOT: // ".."
					UNREGISTER_MODS(_S);
					MOD_DIFFERENT_ON;
					RF_HOLD_TEXT16_UJ(S(KC_QUOT), S(KC_QUOT), S(KC_2), S(KC_2));
				break;

				case KC_COMM: // <..>
					UNREGISTER_MODS(_S);
					MOD_DIFFERENT_ON;
					RF_HOLD_TEXT16_UJ(S(KC_COMM), S(KC_DOT), S(KC_COMM), S(KC_DOT));
				break;

				default:
				break;
			}
		} // IS_MOD_PRESS_EX
	} // IS_MOD_PRESS
	if (!IS_MOD_PRESS_EX(_M)) { // METオプション、その他禁止
		switch (keycode) {
			case KC_LBRC: // [..]
				RF_HOLD_TEXT16_UJ(KC_LBRC, KC_RBRC, KC_RBRC, KC_NUHS);
			break;

			case KC_QUOT: // '..'
				RF_HOLD_TEXT16_UJ(KC_QUOT, KC_QUOT, S(KC_7), S(KC_7));
			break;

			case KC_1 ... KC_0: // テキスト複製
				RF_DUPLICATE_TEXT(keycode - 29);
			break;

			default:
			break;
		}
	} // IS_MOD_PRESS_EX
	return true;
}

// METキーシフト #############################################
bool pr_metkey(uint16_t keycode, keyrecord_t *record, global_s *global) {
	RST_MACRO; // キーを操作すると繰り返しマクロ終了

	if (!IS_KEY_PRESS) {
		if (keycode == CK_MET) {
			UNREGISTER_P_MOD_IF_ALONE(MET_KEY, IS_ANY_OTHER_MOD_PRESS);
			REGISTER_OTHER_MODS_IF_PRESS(_C _T _G);
			return false;
		} // keycode
	} // IS_KEY_PRESS

// キー置き替え ---------------------------------------------
	const uint8_t ORI_KEY[] = { // レイヤーで実現する場合はMET-ENT等に対応する必要あり
		KC_E,    KC_S,    KC_X,    KC_D, // カーソル移動
		KC_G,    KC_H,    KC_M,    KC_U,    KC_I,    KC_V, // ENT、DEL他
		KC_MINS, KC_EQL // ファンクションキー
	};
	const uint8_t REP_KEY[] = {
		KC_UP,   KC_LEFT, KC_DOWN, KC_RGHT,
		KC_DEL,  KC_BSPC, KC_ENT,  KC_ESC,  KC_TAB,  KC_ENT,
		KC_F11,  KC_F12
	};

	for (int8_t i = 0; i < ARRAY_NUM(ORI_KEY); ++i) {
		if (keycode == ORI_KEY[i]) {
			if (i < 4) { // カーソル移動の場合
				if (IS_MOD_PRESS(_A)) {
					if (!IS_MOD_PRESS_EX(_S _E _A _M)) {
						R_UP_MOD_REPLACE_KEY16(REP_KEY[i], MOD_MASK(_M), MOD_MASK(_C _T _G _M)); // MET+ALTの場合、METをレジスト(ウインドウ移動用)
					} // IS_MOD_PRESS_EX
				} // IS_MOD_PRESS
			} // i
			R_UP_MET_REPLACE_KEY16(REP_KEY[i]); // MET必須、その他オプション
		} // keycode
	} // i

	switch (keycode) { // その他、条件付きなど
		case KC_A: // カーソル移動
			R_UP_MET_REPLACE_KEY16((IS_WIN ? C(KC_LEFT) : A(KC_LEFT)));
		break;

		case KC_F:
			R_UP_MET_REPLACE_KEY16((IS_WIN ? C(KC_RGHT) : A(KC_RGHT)));
		break;

		case KC_R:
		case KC_C:
			if (!IS_IOS) { // iOSはPageUp・Downが無いため、繰り返しマクロで代用
				if (!IS_TEXT_EDIT || !IS_MOD_PRESS(_T)) { // RCTL_ENTを押していた場合の処理が後にあるためのif文
					R_UP_MET_REPLACE_KEY16(keycode == KC_R ? KC_PGUP : KC_PGDN);
				} // IS_MOD_PRESS
			} // IS_IOS
		break;

		case KC_1 ... KC_0: // ファンクションキー(レイヤーで実現する場合はMETをアンレジストする必要あり)
			R_UP_MET_REPLACE_KEY16(keycode + 28);
		break;

		default:
		break;
	}
	if (IS_ROMAZI_OUT_ON) { // ローマ字出力時インプットメソッド標準設定対応
		if (!IS_MOD_PRESS_EX(_M)) {
			switch (keycode) {
				case KC_Q: // 文節短縮
					R_UP_MET_REPLACE_KEY16(S(KC_LEFT));
				break;

				case KC_W: // 文節伸長
					R_UP_MET_REPLACE_KEY16(S(KC_RGHT));
				break;

				case KC_J: // ひらがな変換
					R_UP_MET_REPLACE_KEY16((IS_WIN ? KC_F6 : C(KC_J)));
				break;

				case KC_K: // カタカナ変換
					R_UP_MET_REPLACE_KEY16((IS_WIN ? KC_F7 : C(KC_K)));
				break;

				case KC_L: // 半角英数変換
					R_UP_MET_REPLACE_KEY16((IS_WIN ? KC_F10 : C(KC_QUOT)));
				break;

				case KC_SCLN: // 半角変換
					R_UP_MET_REPLACE_KEY16((IS_WIN ? KC_F8 : C(KC_SCLN)));
				break;

				default:
				break;
			}
		} // IS_MOD_PRESS_EX
	} // IS_ROMAZI_OUT_ON

	if (!IS_MOD_PRESS(_M)) return true;
// マクロ実行 ---------------------------------------------
	if (IS_KEY_PRESS) {
		RST_UP_MOD_REPLACE_KEY16; // 上記以外のキーを押した場合、置き替えたキーをアンレジスト
		if (IS_TEXT_EDIT) {
			if (!IS_MOD_PRESS_EX(_S _E _M)) { // MET必須、SFTオプション、その他禁止
				switch (keycode) {
					case KC_O: // 2次プレフィクス有効
						UNREGISTER_MET;
						SECOND_PREFIX_ON;
						return false;

					case KC_R:
					case KC_C:
						if (IS_IOS) { // iOSはPageUp・Downが無いため、繰り返しマクロで代用
							UNREGISTER_MET;
							SET_MACRO;
							return false;
						} // IS_IOS
					break;

					case KC_P: // 文字入れ替え(SFTを押しながらだとキャレットが先頭側)
						UNREGISTER_MET;
						if (IS_MOD_PRESS(_S _E)) {
							UNREGISTER_MODS(_S);
							MOD_DIFFERENT_ON;
							RF_KEY_SEQUENCE(SLCT_CHAR_RGHT CUT_TEXT MOVE_LEFT PASTE_TEXT MOVE_LEFT);
						} else { // IS_MOD_PRESS
							RF_KEY_SEQUENCE(SLCT_CHAR_LEFT CUT_TEXT MOVE_RGHT PASTE_TEXT);
						} // IS_MOD_PRESS
					break;

					default:
					break;
				}
			} // IS_MOD_PRESS_EX
			if (!IS_MOD_PRESS_EX(_M)) { //MET必須、その他禁止
				switch (keycode) {
					case KC_B: // 削除
						UNREGISTER_MET;
						RF_KEY_SEQUENCE(SLCT_PARA DEL_TEXT);
					break;

					case KC_T:
						UNREGISTER_MET;
						RF_KEY_SEQUENCE(SLCT_WORD_RGHT DEL_TEXT);
					break;

					case KC_Y:
						UNREGISTER_MET;
						RF_KEY_SEQUENCE(SLCT_WORD_LEFT DEL_TEXT);
					break;

					case KC_COMM: // 行挿入後カーソル移動
						UNREGISTER_MET;
						RF_KEY_SEQUENCE(MOVE_PARA_TOP TAP_ENT MOVE_UP);
					break;

					case KC_DOT:
						UNREGISTER_MET;
						RF_KEY_SEQUENCE(MOVE_PARA_BTM MOVE_RGHT TAP_ENT MOVE_UP);
					break;

					case KC_SLSH: // 行複製
						UNREGISTER_MET;
						RF_KEY_SEQUENCE(SLCT_PARA CUT_TEXT PASTE_TEXT PASTE_TEXT MOVE_LEFT);
					break;

					case KC_LBRC: // 行移動
						UNREGISTER_MET;
						RF_KEY_SEQUENCE(SLCT_PARA CUT_TEXT MOVE_LEFT MOVE_PARA_TOP PASTE_TEXT MOVE_LEFT);
					break;

					case KC_RBRC:
						UNREGISTER_MET;
						RF_KEY_SEQUENCE(SLCT_PARA CUT_TEXT MOVE_PARA_BTM MOVE_RGHT PASTE_TEXT MOVE_LEFT);
					break;

					case KC_BSLS: // 000
						UNREGISTER_MET;
						RF_REPEAT_KEY(KC_0, 3);
					break;

					default:
					break;
				}
			} // IS_MOD_PRESS_EX
		} // IS_TEXT_EDIT

		switch (keycode) { // MET必須、その他オプション
			case KC_INS ... KC_PDOT: // テンキーはMETをレジストしない
			case LT_CRSR_TAB: // また削除(MET-G etc)後キャレット移動等での誤操作防止
			case LT_MODE_PENT:
				UNREGISTER_P_MOD_IF_ALONE(MET_KEY, IS_MOD_PRESS_EX(_M));
			break;

			case KC_R:
			case KC_C:
				if (IS_TEXT_EDIT) {
					if (IS_MOD_PRESS(_T)) { // MET、RCTL_ENT必須、SFTオプション、その他禁止
						if (!IS_MOD_PRESS_EX(_T _S _E _M)) {
							UNREGISTER_MODS(_T _M);
							SET_MACRO; // 繰り返しマクロでカーソル移動
							return false;
						} // IS_MOD_PRESS
					} // IS_MOD_PRESS
				} // IS_TEXT_EDIT
			break;

			case MT_LSFT_SPC: // 2次プレフィクスOFFで、SFT_SPCを押す前にMETが押されていればMET+SPC
			case MT_RSFT_SPC:
				if (!IS_SECOND_PREFIX) {
					REGISTER_MET;
				} // IS_SECOND_PREFIX
			break;

			case CK_MET: // MODを押している場合はMETをレジストしない(マウスモードでのMOD誤出力防止、
				if (IS_MOD_PRESS(_C _T _S _E _A _G)) { // iOSのMOD両押し対策、SPCタップ前にMETをレジストさせない等)
					UNREGISTER_P_MOD_IF_ALONE(MET_KEY, IS_ANY_OTHER_MOD_PRESS);
				} else { // IS_MOD_PRESS
					REGISTER_MET;
				} // IS_MOD_PRESS
			break;

			case CS_LCTL: case CS_RCTL: // スルーするキー
			case CS_LSFT: case CS_RSFT:
			case KC_LALT: case KC_RALT:
			case KC_LGUI: case KC_RGUI:
			break;

			default: // 上記以外のキーを押した場合(マウスモード除く)はMETをレジスト
				REGISTER_OTHER_MODS_IF_PRESS(_M);
			break;
		}
	} else { // IS_KEY_PRESS
		switch (keycode) {
			case MT_LSFT_SPC:
			case MT_RSFT_SPC:
				if (IS_TEXT_EDIT) {
					if (IS_MOD_PRESS(_S _E)) {
						if (IS_TAP) { // MET+SFT+SPCタップであればSPCを前に挿入
							RST_MOD_TAP;
							UNREGISTER_MODS(_S _M);
							MOD_DIFFERENT_ON;
							RF_KEY_SEQUENCE(TAP_SPC MOVE_LEFT);
						} // IS_TAP
					} // keycode
				} // IS_TEXT_EDIT
			break;

			default:
			break;
		}
	} // IS_KEY_PRESS
	return true;
}

// モッドタップ関係 #############################################
bool pr_mod_tap_dance(uint16_t keycode, keyrecord_t *record, global_s *global) {
// 通常キー と 入力モード切り替えキー ---------------------------------------------
	switch (keycode) {
		case KC_LCTL:
			RF_REPLACE_MOD(KK_LCTL_G);
		break;

		case KC_RCTL:
			RF_REPLACE_MOD(KK_RCTL_G);
		break;

		case KC_LGUI:
			RF_REPLACE_MOD(KK_LGUI_C);
		break;

		case KC_RGUI:
			RF_REPLACE_MOD(KK_RGUI_C);
		break;

		case CK_EISU:
			RF_REPLACE_KEY16(KK_EISU);
		break;

		case CK_KANA:
			RF_REPLACE_KEY16(KK_KANA);
		break;

		default:
		break;
	}

	if (IS_GAME_MODE) return true;

	switch (keycode) {
		case KC_LALT:
		case KC_RALT:
			if (!IS_KEY_PRESS) {
				INVALID_ONE_SHOT(keycode);
			}
		break;

 // モッドタップ ---------------------------------------------
		case MT_RCTL_ENT:
			MOD_TAP16(KC_ENT, KC_RCTL);
			REGISTER_MODS_IF_KEY_RELEASE(_C _G);
			return false;
		break;

		case MT_LSFT_SPC:
			MOD_TAP16(KC_SPC, KC_LSFT);
			REGISTER_MODS_IF_KEY_RELEASE(_S _E); // 離した時に他のSFTが押されていたらSFTをレジスト
			return false;
		break;

		case MT_RSFT_SPC:
			MOD_TAP16(KC_SPC, KC_RSFT);
			REGISTER_MODS_IF_KEY_RELEASE(_S _E);
			return false;
		break;

		case MT_LGUI_EISU:
			if (!MOD_TAP16(KK_EISU, KK_LGUI_C)) {
				if (!IS_MOD_PRESS_EX(_S _E)) { // 単独またはSFTのみでタップした場合は入力切替あり
					ROMAZI_OFF;
				} // IS_MOD_PRESS
			} // MOD_TAP16
			REGISTER_MODS_IF_KEY_RELEASE(_C _T); // MOD置き換えで出力されてしまうため、_Gは省く
			return false;
		break;

		case MT_RGUI_KANA:
			if (!MOD_TAP16(KK_KANA, KK_RGUI_C)) {
				if (!IS_MOD_PRESS_EX(_S _E)) {
					ROMAZI_ON;
					if (IS_ROMAZI_OUT_ON) {
						layer_on(_MIZU);
					} // IS_ROMAZI_OUT_ON
				} // IS_MOD_PRESS
			} // MOD_TAP16
			REGISTER_MODS_IF_KEY_RELEASE(_C _T);
			return false;
		break;
// ダブルタップ ---------------------------------------------
		case DT_LCTL_HYPR:
			DOUBLE_MOD_TAP16(X_HYPR(KC_P2), KK_LCTL_G);
			REGISTER_MODS_IF_KEY_RELEASE(_C _T _G); // 離した時にCTL等が押されていたらCTLをレジスト
			return false;
		break;

		case DT_RCTL_HYPR:
			DOUBLE_MOD_TAP16(X_HYPR(KC_P3), KK_RCTL_G);
			REGISTER_MODS_IF_KEY_RELEASE(_C _T _G);
			return false;
		break;

		case DT_LSFT_HYPR:
			DOUBLE_MOD_TAP16(X_HYPR(KC_P0), KC_LSFT);
			REGISTER_MODS_IF_KEY_RELEASE(_E); // 離した時にSFT_SPCが押されていたらSFTをレジスト
			return false;
		break;

		case DT_RSFT_HYPR:
			DOUBLE_MOD_TAP16(X_HYPR(KC_P1), KC_RSFT);
			REGISTER_MODS_IF_KEY_RELEASE(_E);
			return false;
		break;

		default:
		break;
	}
	return true;
}

// End |||||||||||||||||||||||||||||||||||||||||||||
