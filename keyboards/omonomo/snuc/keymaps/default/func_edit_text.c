#include "func_edit_text.h"

#include "ini_config.h"
#include "func_key_output.h"
#include "func_mod_output.h"
#include "main_keymap.h"

// パーツ関数 |||||||||||||||||||||||||||||||||||||||||||||

// 連続複製関数 #############################################
// 引き数: 回数
// 戻り値: false
bool duplicate_text(uint8_t times, global_s *global) {
	KEY_SEQUENCE(CUT_TEXT);
	for (int8_t i = 0; i < times; ++i) {
		KEY_SEQUENCE(PASTE_TEXT);
	} // i
	return false;
}

// 選択範囲を囲む関数 #############################################
// 引数: 左側のキーコード、右側のキーコード
// 戻り値: false
bool hold_text16(uint16_t left_letter, uint16_t right_letter, global_s *global) {
	KEY_SEQUENCE(CUT_TEXT);
	TAP_CODE16(left_letter);
	KEY_SEQUENCE(PASTE_TEXT);
	TAP_CODE16(right_letter);
	return false;
}

// カット関数 #############################################
void cut_text(global_s *global) {
	TAP_CODE16(IS_WIN ? C(KC_X) : G(KC_X));
	wait_ms(IS_IOS ? TAP_AFTER_DELAY_L : TAP_AFTER_DELAY_S); // 主にiOSで処理が間に合わないための待ち時間
	return;
}

// ペースト関数 #############################################
void paste_text(global_s *global) {
	TAP_CODE16(IS_WIN ? C(KC_V) : G(KC_V));
	wait_ms(IS_IOS ? TAP_AFTER_DELAY_L : TAP_AFTER_DELAY_S);
	return;
}

// 削除関数 #############################################
void delete_text(global_s *global) {
	TAP_CODE(IS_WIN ? KC_DEL : KC_BSPC);
	return;
}

// テキスト移動・選択関数 #############################################
// 引数: 移動/選択、範囲、方向または全体
void edit_text(textedit_e select, textedit_e range, textedit_e direction, global_s *global) {
// 全体 ---------------------------------------------
	if (direction == ALL) {
		switch (range) {
			case WORD: // 単語全体
				EDIT_TEXT(MOVE, WORD, LEFT);
				EDIT_TEXT(SLCT, WORD, RGHT);
			break;

			case LINE: // 行全体
				EDIT_TEXT(MOVE, LINE, TOP);
				EDIT_TEXT(SLCT, LINE, BTM);
				EDIT_TEXT(SLCT, CHAR, RGHT);
			break;

			case PARA: // 文全体
				EDIT_TEXT(MOVE, PARA, TOP);
				EDIT_TEXT(SLCT, PARA, BTM);
				EDIT_TEXT(SLCT, CHAR, RGHT);
			break;

			default:
			break;
		}
		return;
	} // ALL

	if (direction == ALL_B) {
		switch (range) {
			case WORD: // 単語全体(キャレット文頭側)
				EDIT_TEXT(MOVE, WORD, RGHT);
				EDIT_TEXT(SLCT, WORD, LEFT);
			break;

			case LINE: // 行全体(キャレット文頭側)
				EDIT_TEXT(MOVE, LINE, BTM);
				EDIT_TEXT(MOVE, CHAR, RGHT);
				EDIT_TEXT(SLCT, CHAR, LEFT);
				EDIT_TEXT(SLCT, LINE, TOP);
			break;

			case PARA: // 文全体(キャレット文頭側)
				EDIT_TEXT(MOVE, PARA, BTM);
				EDIT_TEXT(MOVE, CHAR, RGHT);
				EDIT_TEXT(SLCT, CHAR, LEFT);
				EDIT_TEXT(SLCT, PARA, TOP);
			break;

			default:
			break;
		}
	return;
	} // ALL_B

// 全体以外 ---------------------------------------------
	if (select == SLCT) {
		REGISTER_CODE(KC_LSFT);
	} // SLCT
	switch (range) {
		case CHAR:
			switch (direction) {
				case LEFT: // 文字左
					TAP_CODE(KC_LEFT);
				break;

				case RGHT: //文字右
					TAP_CODE(KC_RGHT);
				break;

				default:
				break;
			}
		break;

		case WORD:
			switch (direction) {
				case LEFT: // 単語先頭
					TAP_CODE16(IS_WIN ? C(KC_LEFT) : A(KC_LEFT));
				break;

				case RGHT: // 単語末尾
					TAP_CODE16(IS_WIN ? C(KC_RGHT) : A(KC_RGHT));
				break;

				default:
				break;
			}
		break;

		case LINE:
			switch (direction) {
				case TOP: // 行頭
					TAP_CODE16(IS_WIN ? KC_HOME : G(KC_LEFT));
				break;

				case BTM: // 行末
					TAP_CODE16(IS_WIN ? KC_END : G(KC_RGHT));
				break;

				default:
				break;
			}
		break;

		case PARA:
			switch (direction) {
				case TOP: // 文頭
					if (IS_WIN) {
						TAP_CODE(KC_HOME);
					} else {
						if (!IS_MOD_PRESS(_S _E) && !select) {
							TAP_CODE(KC_RGHT);
						}
						TAP_CODE16(A(KC_UP));
					}
				break;

				case BTM: // 文末
					if (IS_WIN) {
						TAP_CODE(KC_END);
					} else {
						if (!IS_MOD_PRESS(_S _E) && !select) {
							TAP_CODE(KC_LEFT);
						}
						TAP_CODE16(A(KC_DOWN));
					}
				break;

				default:
				break;
			}
		break;

		case DOC:
			switch (direction) {
				case TOP: // 文章先頭
					if (IS_WIN) {
						TAP_CODE16(C(KC_HOME));
					} else if (IS_MAC) { // IS_WIN
						TAP_CODE(KC_HOME);
					} else { // IS_MAC
						TAP_CODE16(G(KC_UP));
					}
				break;

				case BTM: // 文章末尾
					if (IS_WIN) {
						TAP_CODE16(C(KC_END));
					} else if (IS_MAC) { // IS_WIN
						TAP_CODE(KC_END);
					} else { // IS_MAC
						TAP_CODE16(G(KC_DOWN));
					}
				break;

				default:
				break;
			}
		break;

		default:
		break;
	}
	if (select == SLCT) {
		UNREGISTER_CODE(KC_LSFT);
	} // SLCT
	return;
}

// End |||||||||||||||||||||||||||||||||||||||||||||
