#include "func_mod_output.h"

#include "ini_config.h"
#include "func_key_output.h"
#include "main_code_layer.h"
#include "main_keymap.h"

// 補助関数 |||||||||||||||||||||||||||||||||||||||||||||

// MOD押下フラグフィルタ関数 #############################################
// 引数: MOD押下フラグのアドレス
void p_mod_flag_filter(uint16_t *target_flag, global_s *global) {
	if (IS_MOUSE_MODE) {
		*target_flag &= MOD_MASK_EX(_E _M); // マウスモードの時はSFT_SPCとMETは対象外
	} // IS_MOUSE_MODE
	return;
}

// パーツ関数 |||||||||||||||||||||||||||||||||||||||||||||

// 押下MODキーレジスト関数(対になるMODがレジストされていたらスルー) #############################################
// 引数: 押すキーのMOD押下フラグ
void register_p_mods(uint16_t reg_flag, global_s *global) {
	if (reg_flag == 0) return;

	P_MOD_FLAG_FILTER(&reg_flag);
	for (int8_t i = 15; 0 <= i; --i) {
		uint16_t j = reg_flag & (1 << i);
		if (j) {
			uint8_t k = P_MOD_FLAG_TO_CODE(j);
			if (!(get_mods() & (1 << PAIR_CODE_TO_GET_MODS(k)))) { // 対になるMODがレジストされていなければレジスト
				REGISTER_CODE(k);
			} // get_mods
		} // j
	} // i
	return;
}

// 押下MODキーアンレジスト関数 #############################################
// 引数: 離すキーのMOD押下フラグ
void unregister_p_mods(uint16_t reg_flag, global_s *global) {
	if (reg_flag == 0) return;

	P_MOD_FLAG_FILTER(&reg_flag);
	for (int8_t i = 0; i < 16; ++i) {
		uint16_t j = reg_flag & (1 << i);
		if (j) {
			UNREGISTER_CODE(P_MOD_FLAG_TO_CODE(j));
		} // j
	} // i
	return;
}

// 押下MODキーレジスト関数(既に押されていたらキャンセル) #############################################
// 引数: 押すキーコード
void register_mod_code(uint8_t reg_mod, global_s *global) {
//	if (reg_mod < KC_LCTL || KC_RGUI < reg_mod) return;

	if (!FIND_P_MOD(reg_mod       , IS_ANY_OTHER_MOD_PRESS)
	&&  !FIND_P_MOD(reg_mod ^ 0x04, IS_ANY_OTHER_MOD_PRESS)) { // reg_modとその対になるMODが押されていなければレジスト
		REGISTER_CODE(reg_mod);
	} // FIND_P_MOD
	return;
}

// 押下MODキーアンレジスト関数(他で押されていたらキャンセル) #############################################
// 引数: 離すキーコード、検索対象MOD押下フラグ
void unregister_mod_code_if_alone(uint8_t reg_mod, uint16_t target_flag, global_s *global) {
//	if (reg_mod < KC_LCTL || KC_RGUI < reg_mod) return;

	if (!FIND_P_MOD(reg_mod, target_flag)) {
		INVALID_ONE_SHOT(reg_mod);
		UNREGISTER_CODE(reg_mod);
	} // FIND_P_MOD
	return;
}

// MODキーコード検索関数 #############################################
// 引数: 検索するキーコード、検索対象MOD押下フラグ
// 戻り値: true-検索対象に同じキーコードが含まれる、false-含まれない
bool find_p_mod(uint8_t source_mod, uint16_t target_flag, global_s *global) {
//	if (source_mod < KC_LCTL || KC_RGUI < source_mod) return false;

	P_MOD_FLAG_FILTER(&target_flag);
	for (int8_t i = 0; i < 16; ++i) {
		if (source_mod == P_MOD_FLAG_TO_CODE(target_flag & (1 << i))) return true;
	} // i
	return false;
}

// MOD押下フラグ取得関数 #############################################
// 引数: MOD押下キーコード
// 戻り値: 対応するMOD押下フラグ
uint16_t code_to_p_mod_flag(uint16_t mod_key) {
//	if (mod_key == 0) return 0;

	const uint16_t MOD_KEYCODE[] = {P_MOD_KEYCODE};
	const uint16_t MOD_FLAG[] = {P_MOD_FLAG};

	for (int8_t i = 0; i < ARRAY_NUM(MOD_KEYCODE); ++i) {
		if (mod_key == MOD_KEYCODE[i]) {
			return 1 << MOD_FLAG[i];
		} // mod_key
	} // i
	return 0;
}

// MODキーコード取得関数 #############################################
// 引数: MOD押下フラグ
// 戻り値: 対応するMODキーコード
uint8_t p_mod_flag_to_code(uint16_t mod_flag, global_s *global) {
//	if (mod_flag == 0) return 0;

	const uint8_t MOD_KC[] = {P_MOD_FLAG_KEY_C};
	const uint8_t MOD_KG[] = {P_MOD_FLAG_KEY_G};

	for (int8_t i = 0; i < 16; ++i) {
		if (mod_flag & (1 << i)) {
			return (GUI_IS_CTL ? MOD_KC[i] : MOD_KG[i]);
		} // mod_flag
	} // i
	return 0;
}

// WindowsワンショットMOD無効関数 #############################################
// 引数: MODキーコード
// 戻り値: true-タップ条件不成立時、false-タップ時
bool invalid_one_shot(uint8_t reg_mod, global_s *global) {
	if (!IS_WIN) return true;
	if (IS_KEY_PRESS_AFTER_MOD) return true; // MODを押した後にキーを押していたらキャンセル

	switch (reg_mod) {
		case KC_LALT: case KC_RALT: // メニューがハイライトされるのを防止
		case KC_LGUI: case KC_RGUI: // スタートメニューが開くのを防止
		break;

		default:
			return true;
		break;
	}

	const uint8_t GET_MODS = get_mods();

	if (GET_MODS & MOD_MASK_CTRL) {
		TAP_CODE(GET_MODS & MOD_BIT(KC_LCTL) ? KC_RCTL : KC_LCTL);
	} else { // GET_MODS
		TAP_CODE(GET_MODS & MOD_BIT(KC_LSFT) ? KC_RSFT : KC_LSFT); // わざとSFTかCTLをタップさせる
	} // GET_MODS
	wait_ms(TAP_AFTER_DELAY_S);
	KEY_PRESS_AFTER_MOD_ON;
	return false;
}

// End |||||||||||||||||||||||||||||||||||||||||||||
