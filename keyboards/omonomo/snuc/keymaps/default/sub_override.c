#include "sub_override.h"

#include "ini_config.h"
#include "func_key_output.h"
#include "func_mod_output.h"
#include "sub_counter.h"
#include "main_code_layer.h"
#include "main_keymap.h"

// 関数置き替え #############################################
#define RST_SFT rst_sft(global)

#define SET_SFT_UP_OVERRIDE(code) set_sft_up_override(code)
#define RST_SFT_UP_OVERRIDE set_sft_up_override(0)

#define RST_OVERRIDE rst_override(&override_keycode, global)

// フラグ・変数 #############################################
static uint16_t sft_up_keycode = 0; // シフトアップ時キーコード

// 関数プロトタイプ宣言 #############################################
static void rst_sft(global_s *global);
static void rst_override(uint16_t *override_keycode, global_s *global);
static void set_sft_up_override(uint16_t reg_key);

// 補助関数 |||||||||||||||||||||||||||||||||||||||||||||

// SFTリセット関数 #############################################
static void rst_sft(global_s *global) {
	UNREGISTER_MOD_CODE_IF_ALONE(KC_RSFT, IS_ANY_OTHER_MOD_PRESS); // SFTを元に戻す
	UNREGISTER_MOD_CODE_IF_ALONE(KC_LSFT, IS_ANY_OTHER_MOD_PRESS);
	REGISTER_OTHER_MODS_IF_PRESS(_S _E);
	return;
}

// オーバーライドリセット関数 #############################################
static void rst_override(uint16_t *override_keycode, global_s *global) {
	UNREGISTER_CODE(*override_keycode);
	*override_keycode = 0;
	RST_SFT;
}

// シフトアップキーコードセット関数 #############################################
// 引数: シフトアップ後のオーバーライドキーコード
static void set_sft_up_override(uint16_t reg_key) {
	sft_up_keycode = reg_key;
	if (0 < reg_key) {
		SET_REPEAT_COUNT(CHANGE_OVERRIDE_DELAY); // reg_keyが0以外でオンディレイ値を設定
	}
	return;
}

// サブルーチン関数 |||||||||||||||||||||||||||||||||||||||||||||

// キーオーバーライド シフトアップ #############################################
void ms_key_override(void) {
	if (sft_up_keycode == 0) return;

	if (REPEAT_COUNT == 1) { // SFTを離した後REPEAT_COUNTが0になるまでキーを押していればsft_up_keycodeをレジスト
		REGISTER_CODE(sft_up_keycode);
		sft_up_keycode = 0;
	}
	return;
}

// キーオーバーライド(US-JIS対応) #############################################
bool pr_key_override(uint16_t keycode, keyrecord_t *record, global_s *global) {
	if (!IS_JIS) return true;

	static uint16_t override_keycode = 0; // オーバーライドキーコード

	const uint8_t ORI_KEY_NS[] = {KC_GRV,  KC_EQL,  KC_QUOT}; // SFT無・有
	const uint8_t REP_KEY_NS[] = {KC_LBRC, KC_MINS, KC_7};    // {JP_GRV, JP_EQL, JP_QUOT};

	const uint8_t ORI_KEY_SN[] = {KC_2,    KC_6,   KC_SCLN}; // SFT有・無
	const uint8_t REP_KEY_SN[] = {KC_LBRC, KC_EQL, KC_QUOT}; // {JP_AT, JP_CIRC, JP_COLN};

	const uint8_t ORI_KEY_SS[] = {KC_GRV, KC_EQL,  KC_QUOT, KC_7, KC_8,    KC_9, KC_0, KC_MINS}; // SFT有・有(順番注意)
	const uint8_t REP_KEY_SS[] = {KC_EQL, KC_SCLN, KC_2,    KC_6, KC_QUOT, KC_8, KC_9, KC_INT1};
	                          // {JP_TILD, JP_AMPR, JP_ASTR, JP_LPRN, JP_RPRN, JP_UNDS, JP_PLUS, JP_DQUO};
	const uint8_t ORI_KEY_TT[] = {KC_LBRC, KC_RBRC, KC_BSLS}; // MOD不問
	const uint8_t REP_KEY_TT[] = {KC_RBRC, KC_NUHS, KC_INT3}; // {JP_LBRC, JP_RBRC, JP_YEN};

	RST_SFT_UP_OVERRIDE; // キー操作するとsft_up_keycodeをリセット
// SFT無・有 ---------------------------------------------
	for (int8_t i = 0; i < ARRAY_NUM(ORI_KEY_NS); ++i) {
		if (keycode != ORI_KEY_NS[i]) continue;
		if (IS_KEY_PRESS) {
			if (IS_MOD_PRESS_EX(_B)) break; // TAB以外のMODを押していたらループを抜ける
			UNREGISTER_CODE(override_keycode);
			REGISTER_CODE16(override_keycode = S(REP_KEY_NS[i])); // LSFT付きでレジスト
			return false;
		} else { // IS_KEY_PRESS
			if (override_keycode == 0) return true;
			if (IS_MOD_PRESS(_S _E)) break; // 有・有と区別するための判定(SFTを押している場合ループを抜ける)
			RST_OVERRIDE;
			return false;
		} // IS_KEY_PRESS
	} // i
// SFT有・無 ---------------------------------------------
	for (int8_t i = 0; i < ARRAY_NUM(ORI_KEY_SN); ++i) {
		if (keycode != ORI_KEY_SN[i]) continue;
		if (IS_KEY_PRESS) {
			if (!IS_MOD_PRESS(_S _E)) break;// SFTを押していなければループを抜ける
			if (IS_MOD_PRESS_EX(_S _E _B)) break; // SFT・TAB以外のMODを押していたらループを抜ける
			UNREGISTER_CODE(override_keycode);
			UNREGISTER_MODS(_S);
			REGISTER_CODE(override_keycode = REP_KEY_SN[i]);
			return false;
		} else { // IS_KEY_PRESS
			if (override_keycode == 0) return true;
			RST_OVERRIDE;
			return false;
		} // IS_KEY_PRESS
	} // i
// SFT有・有 ---------------------------------------------
	for (int8_t i = 0; i < ARRAY_NUM(ORI_KEY_SS); ++i) {
		if (keycode != ORI_KEY_SS[i]) continue;
		if (IS_KEY_PRESS) {
			if (!IS_MOD_PRESS(_S _E)) break; // SFTを押していなければループを抜ける
			if (IS_MOD_PRESS_EX(_S _E _B)) break; // SFT、TAB以外のMODを押していたらループを抜ける
			UNREGISTER_CODE(override_keycode);
			REGISTER_MODS_IF_PRESS(_S _E); //有・無からの移行対応
			REGISTER_CODE(override_keycode = S(REP_KEY_SS[i])); // SFTが既に押されているため、LSFT抜きでレジスト(有・無と区別のためLSFT付きで保存)
			return false;
		} else { // IS_KEY_PRESS
			if (override_keycode == 0) return true;
			RST_OVERRIDE;
			return false;
		} // IS_KEY_PRESS
	} // i
// MOD不問 ---------------------------------------------
	for (int8_t i = 0; i < ARRAY_NUM(ORI_KEY_TT); ++i) {
		if (keycode != ORI_KEY_TT[i]) continue;
		if (IS_KEY_PRESS) {
			if (IS_WIN ? IS_MOD_PRESS_EX(_S _E _B) : IS_MOD_PRESS_EX(_S _E _A _B)) break; // SFT(、ALT)、TAB以外のMODを押していたらループを抜ける
			UNREGISTER_CODE(override_keycode);
			RST_SFT;
			REGISTER_CODE(override_keycode = REP_KEY_TT[i]);
			return false;
		} else { // IS_KEY_PRESS
			if (override_keycode == 0) return true;
			RST_OVERRIDE;
			return false;
		} // IS_KEY_PRESS
	} // i


	if (override_keycode == 0) return true;
// SFTキー ---------------------------------------------
	switch (keycode) {
		case CS_LSFT: case MT_LSFT_SPC:
		case CS_RSFT: case MT_RSFT_SPC:
			if (!IS_KEY_PRESS) { // シフトを離した場合
				if (IS_OTHER_MOD_PRESS(_S _E)) return true; // 他のSFTが押されていればreturn
				for (int8_t i = 0; i < ARRAY_NUM(REP_KEY_SN); ++i) { // SFT有・無
					if (override_keycode != REP_KEY_SN[i]) continue;
					UNREGISTER_CODE(override_keycode);
					SET_SFT_UP_OVERRIDE(override_keycode = ORI_KEY_SN[i]); // SFTアンレジスト済みなのでそのままで元のキーに戻す
					return false;
				} // i
				for (int8_t i = 0; i < ARRAY_NUM(REP_KEY_SS); ++i) { // SFT有・有
					if (override_keycode != S(REP_KEY_SS[i])) continue;
					UNREGISTER_CODE(override_keycode);
					if (i < ARRAY_NUM(REP_KEY_NS)) { // 一部のキーはSFTを離した場合、無・有に移行
						SET_SFT_UP_OVERRIDE(override_keycode = S(REP_KEY_NS[i])); //SFTそのまま、LSFT付きで保存し、後でLSFT抜きでレジスト
					} else { // REP_KEY_NS
						UNREGISTER_MODS(_S); // その他のキーはSFTをアンレジストして元のキーに戻す
						SET_SFT_UP_OVERRIDE(override_keycode = ORI_KEY_SS[i]);
					} // REP_KEY_NS
					return false;
				} // i
			} // IS_KEY_PRESS
		break;

		default:
		break;
	}
// 上記以外の条件 ---------------------------------------------
	if (IS_KEY_PRESS) {
		RST_OVERRIDE; // オーバーライド解除
	} // IS_KEY_PRESS

	return true;
}

// End |||||||||||||||||||||||||||||||||||||||||||||
