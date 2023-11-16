#include "sub_romazi.h"

#include "func_key_output.h"
#include "func_mod_output.h"
#include "main_code_layer.h"
#include "main_keymap.h"

// サブルーチン関数 |||||||||||||||||||||||||||||||||||||||||||||

// ローマ字出力用 #############################################
bool pr_romazi_output(uint16_t keycode, keyrecord_t *record, global_s *global) {
	if (!IS_ROMAZI_OUT_ON) return true;
	if (!IS_KEY_PRESS) return true;

// 記号 ---------------------------------------------
	switch (keycode) { // 全・半角切り換え機能に対応するため、タップコードで出力
		case RM_LKKK: // 「
			RF_TAP_CODE_UJ(KC_LBRC, KC_RBRC);
		break;

		case RM_RKKK: // 」
			RF_TAP_CODE_UJ(KC_RBRC, KC_NUHS);
		break;

		case RM_L2KK: // 『
			if (IS_WIN) {
				RF_TAP_CODE_UJ(KC_LBRC, KC_RBRC);
			} else {
				RF_TAP_CODE16_UJ(S(KC_LBRC), S(KC_RBRC));
			}
		break;

		case RM_R2KK: // 』
			if (IS_WIN) {
				RF_TAP_CODE_UJ(KC_RBRC, KC_NUHS);
			} else {
				RF_TAP_CODE16_UJ(S(KC_RBRC), S(KC_NUHS));
			}
		break;

		case RM_LSKK: // 【
			if (IS_WIN) {
				RF_TAP_CODE16_UJ(S(KC_9), S(KC_8));
			} else {
				RF_TAP_CODE16(A(KC_9));
			}
		break;

		case RM_RSKK: // 】
			if (IS_WIN) {
				RF_TAP_CODE16_UJ(S(KC_0), S(KC_9));
			} else {
				RF_TAP_CODE16(A(KC_0));
			}
		break;

		case RM_LPRN: // (
			RF_TAP_CODE16_UJ(S(KC_9), S(KC_8));
		break;

		case RM_RPRN: // )
			RF_TAP_CODE16_UJ(S(KC_0), S(KC_9));
		break;

		case RM_NAMI: // 〜
			RF_TAP_CODE16_UJ(S(KC_GRV), S(KC_EQL));
		break;

		case RM_LEAD: // …
			RF_TAP_CODE16(IS_WIN ? KC_SCLN : A(KC_SCLN));
		break;

		case RM_EXC: // ！
			RF_TAP_CODE16(S(KC_1));
		break;

		case RM_QUE: // ？
			RF_TAP_CODE16(S(KC_SLSH));
		break;

		case RM_H: // ー
			RF_TAP_CODE(KC_MINS);
		break;

		case RM_TEN: // ・
			RF_TAP_CODE(KC_SLSH);
		break;

		default:
		break;
	}

	if (IS_ANY_MOD_PRESS) return true; // MODを押しているときは以下の処理をキャンセル
// ローマ字 ---------------------------------------------
	const char ROMAZI[][4] = {
		"a",  "i",  "u",  "e",  "o",  "ka",  "ki",  "ku",  "ke",  "ko",
		"sa", "si", "su", "se", "so", "ta",  "ti",  "tu",  "te",  "to",
		"na", "ni", "nu", "ne", "no", "ha",  "hi",  "hu",  "he",  "ho",
		"ma", "mi", "mu", "me", "mo", "ya",  "yu",  "yo",
		"ra", "ri", "ru", "re", "ro", "wa",  "wyi", "wye", "wo",  "nn",
		"vu",
		"ga", "gi", "gu", "ge", "go", "za",  "zi",  "zu",  "ze",  "zo",
		"da", "di", "du", "de", "do", "ba",  "bi",  "bu",  "be",  "bo",
		"pa", "pi", "pu", "pe", "po",
		"xa", "xi", "xu", "xe", "xo", "xtu", "xya", "xyu", "xyo", "xwa"
	};

	for (uint16_t i = RM_A; i <= RM_XWA; ++i) { // ローマ字出力
		if (keycode == i) {
			send_string(ROMAZI[i - RM_A]);
			return false;
		} // keycode
	} // i
	return true;
}

// ローマ字入力用 #############################################
bool pr_romazi_input(uint16_t keycode, keyrecord_t *record, global_s *global) {
	if (!IS_ROMAZI_IN_ON) return true;
	if (IS_ANY_MOD_PRESS) return true; // MODを押していない時のみ有効
	if (!IS_KEY_PRESS) return true;

	switch (keycode) {
		case KC_ENT ... KC_SPC: // 以下の処理をキャンセルするキー
		case KC_DEL:
		case LT_MODE_PENT:
			return true;
		break;

		default:
		break;
	}
// ローマ字 ---------------------------------------------
	const uint8_t SIIN[] = {0, 0, 0, 0, KC_L, KC_X, KC_K, KC_S, KC_T, KC_N, KC_H, KC_M, KC_Y, KC_R, KC_W, // 対応する子音
																			KC_V, KC_G, KC_Z, KC_D, KC_B, KC_P, KC_J, KC_Q, KC_F, KC_C};
	const uint8_t BOIN[] = {KC_A, KC_I, KC_U, KC_E, KC_O}; // 対応する母音

	const char MIZU[][5][5] = {
		{"",     "n",    "ex",   "",     ""    }, // NN ンー
		{"",     "",     "l",    "",     ""    }, // LT ッ
		{"b",    "ei",   "x",    "sr",   "z"   }, // LY ャ行
		{"sa",   "",     "",     "",     ""    }, // LW ヮ
		{"en",   "ei",   "st",   "sr",   "eo"  }, // LX ァ行
		{"et",   "u",    "h",    "sk",   "ea"  }, // X  ア行
		{"a",    "y",    "k",    "ev",   "si"  }, // K  カ行
		{"ew",   "o",    "es",   "ep",   "se"  }, // S  サ行
		{"f",    "",     "j",    "d",    "t"   }, // T  タ行
		{"v",    "c",    "sj",   "ez",   "w"   }, // N  ナ行
		{"er",   "su",   "g",    "sl",   "ec"  }, // H  ハ行
		{"p",    "sy",   "s.",   "eb",   "ef"  }, // M  マ行
		{"sh",   "",     "s,",   "",     "eg"  }, // Y  ヤ行
		{"ed",   "so",   "m",    "sn",   "s"   }, // R  ラ行
		{"sm",   "iq",   "",     "r.",   "rn"  }, // W  ワ行
		{"rhen", "rhei", "rh",   "rhsr", "rheo"}, // V  ヴ行
		{"ia",   "ry",   "rk",   "iv",   "ri"  }, // G  ガ行
		{"iw",   "ro",   "is",   "ip",   "re"  }, // Z  ザ行
		{"if",   "r",    "rj",   "id",   "it"  }, // D  ダ行
		{"ir",   "ru",   "ig",   "rl",   "ic"  }, // B  バ行
		{"ix",   "r,",   "ib",   "rm",   "iz"  }, // P  パ行
		{"rob",  "ro",   "rox",  "rosr", "roz" }, // J  ジャ行
		{"ken",  "kei",  "k",    "ksr",  "keo" }, // Q  クァ行
		{"gen",  "gei",  "g",    "gsr",  "geo" }, // F  ファ行
		{"",     "",     "",     "",     ""    }  // C
	};

	gozyuon_gyou_e gyou1 = NN_GYOU; gozyuon_dan_e dan1 = A_DAN;
	gozyuon_gyou_e gyou2 = NN_GYOU; gozyuon_dan_e dan2 = A_DAN; // 初期値、出力なし
// 母音 ---------------------------------------------
	for (int8_t i = 0; i < ARRAY_NUM(BOIN); ++i) {
		if (keycode != BOIN[i]) continue;
		gyou1 = X_GYOU; dan1 = i; // 押されたのが母音であればまずア行i段をセット
		for (int8_t j = LX_GYOU; j < ARRAY_NUM(SIIN); ++j) {
			if (LAST_KEYCODE != SIIN[j]) continue;
			gyou1 = j; // 1つ前に押されたのが子音、半母音であればj行に上書き
			for (int8_t k = LX_GYOU; k < ARRAY_NUM(SIIN); ++k) {
				if (LAST2_KEYCODE != SIIN[k]) continue;
				switch (LAST_KEYCODE) { // 2つ前に押されたのが子音の場合
					case KC_X: // ア段
						if (LAST2_KEYCODE != KC_V) {
							gyou1 = k;       dan1 = A_DAN; // 1つ前が半母音であれば拗音を含めて行･段を上書き
							gyou2 = LX_GYOU; dan2 = i;
						} // LAST2_KEYCODE
					break;

					case KC_Y: // イ段
						if (LAST2_KEYCODE == KC_L) { // 小文字に上書き
							gyou1 = LY_GYOU;
						} else
						if (LAST2_KEYCODE != KC_Y
						&&  LAST2_KEYCODE != KC_W
						&&  LAST2_KEYCODE != KC_V) {
							gyou1 = k;       dan1 = I_DAN;
							gyou2 = LY_GYOU; dan2 = i;
						} // LAST2_KEYCODE
					break;

					case KC_W: // ウ段ァ行
						if (LAST2_KEYCODE == KC_L) {
							gyou1 = LW_GYOU;
						} else if (LAST2_KEYCODE != KC_W) {
							gyou1 = k;       dan1 = U_DAN;
							gyou2 = LX_GYOU; dan2 = i;
						} // LAST2_KEYCODE
					break;

					case KC_J: // ウ段ャ行
						if (LAST2_KEYCODE != KC_W
						&&  LAST2_KEYCODE != KC_J
						&&  keycode != KC_I
						&&  keycode != KC_E) {
							gyou1 = k;       dan1 = U_DAN;
							gyou2 = LY_GYOU; dan2 = i;
						} // LAST2_KEYCODE
					break;

					case KC_Q: // エ段
						if (LAST2_KEYCODE != KC_Y
						&&  LAST2_KEYCODE != KC_W
						&&  LAST2_KEYCODE != KC_V
						&&  LAST2_KEYCODE != KC_Q) {
							gyou1 = k;       dan1 = E_DAN;
							gyou2 = LY_GYOU; dan2 = i;
						} // LAST2_KEYCODE
					break;

					case KC_L: // オ段
						if (LAST2_KEYCODE != KC_W
						&&  LAST2_KEYCODE != KC_V
						&&  LAST2_KEYCODE != KC_L) {
							gyou1 = k;       dan1 = O_DAN;
							gyou2 = LX_GYOU; dan2 = i;
						} // LAST2_KEYCODE
					break;

					case KC_T: // ッ
						if (LAST2_KEYCODE == KC_L) {
							gyou1 = LT_GYOU;
						} // LAST2_KEYCODE
					break;

					case KC_S: // ツ (以下ヘボン式対応)
						if (LAST2_KEYCODE == KC_T) {
							gyou1 = k; dan1 = U_DAN;
							if (keycode != KC_U) {
								gyou2 = LX_GYOU; dan2 = i;
							} // keycode
						} // LAST2_KEYCODE
					break;

					case KC_H:
						switch (LAST2_KEYCODE) {
							case KC_S: // シ
								gyou1 = k; dan1 = I_DAN;
								if (keycode != KC_I) {
									gyou2 = LY_GYOU; dan2 = i;
								} // keycode
							break;

							case KC_C: // チ
								gyou1 = T_GYOU; dan1 = I_DAN;
								if (keycode != KC_I) {
									gyou2 = LY_GYOU; dan2 = i;
								} // keycode
							break;

							case KC_T: // テ
							case KC_D: // デ
								gyou1 = k; dan1 = E_DAN;
								gyou2 = LY_GYOU; dan2 = i;
							break;

							default:
							break;
						}
					break;

					default:
					break;
				}
			} // k
		} // j

		send_string(MIZU[gyou1][dan1]);
		if ((gyou1 == T_GYOU && dan1 == I_DAN) // QUOTキー対応
		||  (gyou1 == R_GYOU && dan1 == O_DAN)
		||  (gyou1 == D_GYOU && dan1 == I_DAN)) {
			TAP_CODE16_UJ(KC_QUOT, S(KC_7));
		} // gyou && dan
		send_string(MIZU[gyou2][dan2]);
		return false;
	} // i
// 子音 ---------------------------------------------
	for (int8_t j = LX_GYOU; j < ARRAY_NUM(SIIN); ++j) {
		if (LAST_KEYCODE != SIIN[j]) continue;
		switch (LAST_KEYCODE) { // 1つ前に押されたのが子音の場合、ッ・ン・ーを処理する
			case KC_T: // ッ
				switch (keycode) {
					case KC_X: case KC_Y: // 半母音に成りえるキーはスルー
					case KC_W: case KC_J:
					case KC_Q: case KC_L:
					case KC_S: case KC_H:
					break;

					default:
						gyou1 = LT_GYOU; dan1 = U_DAN; // 上記以外の子音が押された場合にセット
					break;
				}
			break;

			case KC_N: // ン
				switch (keycode) {
					case KC_X: case KC_Y:
					case KC_W: case KC_J:
					case KC_Q: case KC_L:
					break;

					default:
						gyou1 = NN_GYOU; dan1 = I_DAN;
						if (keycode == LAST_KEYCODE) { // 連続入力後の誤出力防止
							NOW_KEYCODE = KC_A;
						} // keycode
					break;
				}
			break;

			case KC_H: // ー
				switch (keycode) {
					case KC_X: case KC_Y:
					case KC_W: case KC_J:
					case KC_Q: case KC_L:
					break;

					default:
						gyou1 = NN_GYOU; dan1 = U_DAN;
						if (keycode == LAST_KEYCODE) {
							NOW_KEYCODE = KC_A;
						} // keycode
					break;
				}
			break;

			case KC_X: // スルー
			break;

			default:
				if (keycode == LAST_KEYCODE) { // 上記以外の子音2回押しの場合
					gyou1 = LT_GYOU; dan1 = U_DAN; // ッ
				} // keycode
			break;
		}

		send_string(MIZU[gyou1][dan1]);
		return false;
	} // j
// 単打 ---------------------------------------------
	if (keycode == KC_LBRC) { // 「
		TAP_CODE(KC_S);
		RF_TAP_CODE_UJ(KC_LBRC, KC_RBRC);
	}
	if (keycode == KC_RBRC) { // 」
		TAP_CODE(KC_S);
		RF_TAP_CODE_UJ(KC_RBRC, KC_NUHS);
	}
	if (KC_A <= keycode && keycode <= KC_Z) return false; // 単打でのカナ出力防止
	if (keycode == KC_QUOT && !IS_HALF_SYMBOL) return false;

	return true;
}

// End |||||||||||||||||||||||||||||||||||||||||||||
