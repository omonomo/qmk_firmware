#include "func_tap_replace.h"

#include "ini_config.h"
#include "func_key_output.h"
#include "func_mod_output.h"
#include "sub_counter.h"
#include "main_keymap.h"

// フラグ・変数 #############################################
static uint16_t mod_tap_keycode = 0; // モッドタップキーコード

// 補助関数 |||||||||||||||||||||||||||||||||||||||||||||

// モッドタップキーコードセット関数 #############################################
// 引数: モッドタップキーコード
void set_mod_tap(uint16_t mod_key) {
	mod_tap_keycode = mod_key;
	SET_TERM_COUNT(mod_key == 0 ? 0 : MOD_TAP_TH); // mod_key == 0 でリセット
	return;
}

// モッドタップキー成立判定関数 #############################################
// 引数: モッドタップキーコード
// 戻り値: true-成立、false-不成立
bool is_tap(uint16_t keycode) {
	return (mod_tap_keycode == keycode && 0 < TERM_COUNT);
}

// パーツ関数 |||||||||||||||||||||||||||||||||||||||||||||

// 代替MOD出力関数(MOD重複判定なし) #############################################
// 引数: 置き替えるMODキーコード
// 戻り値: true-離した時、false-押した時
bool replace_mod(uint8_t rep_mod, keyrecord_t *record) {
	if (IS_KEY_PRESS) {
		register_code(rep_mod);
		return false;
	} else { // IS_KEY_PRESS
		unregister_code(rep_mod);
	} // IS_KEY_PRESS
	return true;
}

// 代替キー出力関数 #############################################
// 引数: 置き替えるキーコード
// 戻り値: true-リセット、離した時、false-押した時
bool replace_key16(uint16_t rep_key, keyrecord_t *record) {
	static uint16_t replaced_keycode = 0; // 代替キーコード

	if (rep_key == 0) { // 置き替えるキーコードが0でリセット
		UNREGISTER_CODE16(replaced_keycode);
		replaced_keycode = 0;
		return true;
	} // rep_key

	if (IS_KEY_PRESS) {
		UNREGISTER_CODE16(replaced_keycode); // 別のキーを押した場合、前のキーをアンレジスト(入りっぱなし防止)
		REGISTER_CODE16(replaced_keycode = rep_key);
		return false;
	} else { // IS_KEY_PRESS
		RST_REPLACE_KEY16;
	} // IS_KEY_PRESS
	return true;
}

// 代替キー出力関数(MODキャンセルあり) #############################################
// 引数: 置き替えるキーコード、必須MOD押下フラグ、押していればレジストするMOD押下フラグ
// 戻り値: replace_key16の結果
bool up_mod_replace_key16(uint16_t rep_key, uint16_t rp_mod_mask, uint16_t reg_mod_mask, keyrecord_t *record, global_s *global) {
	const uint16_t IS_MOD_PRESS_RP = IS_ANY_MOD_PRESS &  rp_mod_mask;
	const uint16_t IS_MOD_PRESS_XP = IS_ANY_MOD_PRESS & ~rp_mod_mask;
	const uint16_t IS_MOD_PRESS_RG = IS_ANY_MOD_PRESS & reg_mod_mask;

	if (rep_key != 0) { // rep_key == 0の処理はreplace_key16で実施
		if (IS_KEY_PRESS) {
			if (!IS_MOD_PRESS_RP) return true;
			if (!(rp_mod_mask & reg_mod_mask)) { // 押していればレジストするMODに必須モッドが含まれていない場合
				UNREGISTER_P_MOD_IF_ALONE(P_MOD_FLAG_TO_CODE(IS_MOD_PRESS_RP), IS_MOD_PRESS_XP); // 必須MODと同じキーコードのキーを押していなければアンレジスト
			} // mod_up
			REGISTER_P_MODS(IS_MOD_PRESS_RG);
		} // IS_KEY_PRESS
	} // rep_key
	return REPLACE_KEY16(rep_key);
}

// 代替MOD & キータップ関数 #############################################
// 引数: タップされるキーコード、置き替えるMODキーコード、必須（離す）MOD押下フラグ、禁止MOD押下フラグ
// 戻り値: true-リセット、置き替えたMODと異なるキーを離した時、条件不一致で押した時
//         false-タップ時(条件一致で押した時)、置き替えたMODと同じキーを離した時
bool replace_mod_tap_key16(uint16_t tap_key, uint8_t rep_mod, uint16_t rp_mod_mask, uint16_t np_mod_mask, uint16_t keycode, keyrecord_t *record, global_s *global) {
	static uint8_t replaced_keycode = 0; // 置き替えたMODキーコード
	const uint16_t IS_MOD_PRESS_RP = IS_ANY_MOD_PRESS & rp_mod_mask;
	const uint16_t IS_MOD_PRESS_NP = IS_ANY_MOD_PRESS & np_mod_mask;

	if (tap_key == 0) { // タップされるキーコードが0でMODを元に戻す
		unregister_code(replaced_keycode);
		wait_ms(TAP_AFTER_DELAY_S);
		REGISTER_P_MODS(replaced_keycode == 0 ? 0 : IS_ANY_OTHER_MOD_PRESS);
		replaced_keycode = 0;
		return true;
	} // tap_key

	if (IS_KEY_PRESS) {
		if (!IS_MOD_PRESS_RP) return true; // MODの状態が異なる場合はキャンセル
		if (IS_MOD_PRESS_NP) return true;
		if (replaced_keycode == 0) { // 初回に押し続けるMODを変更
			if (!FIND_P_MOD(rep_mod, IS_MOD_PRESS_RP)) { // 置き替えるキーと押しているキーが異なる場合アンレジスト
				INVALID_ONE_SHOT(P_MOD_FLAG_TO_CODE(IS_MOD_PRESS_RP));
				UNREGISTER_P_MODS(IS_MOD_PRESS_RP);
			} // FIND_P_MOD
			register_code(replaced_keycode = rep_mod);
		} // replaced_keycode
		TAP_CODE16(tap_key); // MODを押しながらもう一方のキーを押した時にタップ
		return false; // タップ時にfalse
	} else { // IS_KEY_PRESS
		if (IS_MOD_PRESS_RP) { // 必須のMODが押されている場合
			KEY_PRESS_AFTER_MOD_ON; // ALT・GUI対策
			return !FIND_P_MOD(replaced_keycode, code_to_p_mod_flag(keycode)); // 置き替えたキーと離したキーが異なる場合true、同じでfalse
		} else { // IS_MOD_PRESS_RP
			unregister_code(replaced_keycode); // 全て離した時置き替えたMODをアンレジスト
			replaced_keycode = 0;
		} // IS_MOD_PRESS_RP
	} // IS_KEY_PRESS
	return true;
}

// モッドタップ関数 #############################################
// 引数: タップされるキーコード、ホールドされるキーコード
// 戻り値: true-押した時、条件不成立で離した時、false-タップ時(条件成立で離した時)
bool mod_tap16(uint16_t tap_key, uint8_t hold_mod, uint16_t keycode, keyrecord_t *record, global_s *global) {
	if (IS_KEY_PRESS) {
		if (IS_TAP) { // 連続タップ時はhold_modをレジストしない
			MOD_DIFFERENT_ON;
		} else { // IS_TAP
			REGISTER_P_MOD_IF_ALONE(hold_mod, IS_ANY_OTHER_MOD_PRESS); // hold_modとその対になるMODが押されていなければレジスト
			SET_MOD_TAP; // 押した時に初期設定
		} // IS_TAP
	} else { // IS_KEY_PRESS
		if (IS_TAP) { // 設定時間内に同じキーを離したらタップ
			SET_MOD_TAP; // 連続タップにそなえてカウンタを元に戻す
			UNREGISTER_P_MOD_IF_ALONE(hold_mod, IS_ANY_OTHER_MOD_PRESS);
			TAP_CODE16(tap_key);
			return false; // タップ時にfalse
		} else { // 設定時間を超えた場合
			UNREGISTER_P_MOD_IF_ALONE(hold_mod, IS_ANY_OTHER_MOD_PRESS);
		} // IS_TAP
	} // IS_KEY_PRESS
	return true;
}

// MODダブルタップ関数 #############################################
// 引数: タップされるキーコード、ホールドされるキーコード
// 戻り値: true-離した時、条件不成立で押した時、false-ダブルタップ時(条件成立で押した時)
bool double_mod_tap16(uint16_t tap_key, uint8_t hold_mod, uint16_t keycode, keyrecord_t *record, global_s *global) {
	if (IS_KEY_PRESS) {
		if (IS_TAP) { // 設定時間内に同じキーを押したらタップ
			RST_MOD_TAP;
			if (IS_ANY_OTHER_MOD_PRESS) { // ただし他のMODが押されていたらキャンセル
				REGISTER_P_MOD_IF_ALONE(hold_mod, IS_ANY_OTHER_MOD_PRESS); // hold_modとその対になるMODが押されていなければレジスト
				return true;
			} // IS_ANY_OTHER_MOD_PRESS
			TAP_CODE16(tap_key);
			MOD_DIFFERENT_ON;
			return false; // ダブルタップ時にfalse
		} else { // IS_TAP 1回目のキーを押した時に初期設定
			REGISTER_P_MOD_IF_ALONE(hold_mod, IS_ANY_OTHER_MOD_PRESS);
			SET_MOD_TAP;
		} // IS_TAP
	} else { // IS_KEY_PRESS
		if (!IS_MOD_DIFFERENT) { // タップ不成立で離した場合
			UNREGISTER_P_MOD_IF_ALONE(hold_mod, IS_ANY_OTHER_MOD_PRESS);
		} // IS_MOD_DIFFERENT
	} // IS_KEY_PRESS
	return true;
}

// キーダブルタップ関数 #############################################
// 引数: タップされるキーコード(MOD付で必須MODキーアンレジスト)、必須MOD押下フラグ、禁止MOD押下フラグ
// 戻り値: true-条件不一致、false-条件一致で操作した時
bool double_key_tap16(uint16_t tap_key, uint16_t rp_mod_mask, uint16_t np_mod_mask, uint16_t keycode, keyrecord_t *record, global_s *global) {
	const uint16_t IS_MOD_PRESS_RP = IS_ANY_MOD_PRESS & rp_mod_mask;
	const uint16_t IS_MOD_PRESS_NP = IS_ANY_MOD_PRESS & np_mod_mask;

	if (!IS_MOD_PRESS_RP) return true; // MODの状態が異なる場合はキャンセル
	if (IS_MOD_PRESS_NP) return true;

	if (IS_KEY_PRESS) {
		if (IS_TAP) { // 設定時間内に同じキーを押したらタップ
			RST_MOD_TAP;
			if (tap_key & 0x0f00) { // tap_keyがMOD付の場合、押しているMODキーをアンレジスト
				KEY_PRESS_AFTER_MOD_OFF; // 条件成立操作時はreturn falseによりキー出力がされていないため、このタイミングでリセットをかける
				INVALID_ONE_SHOT(P_MOD_FLAG_TO_CODE(IS_MOD_PRESS_RP));
				UNREGISTER_P_MODS(IS_MOD_PRESS_RP);
			} // mod_up
			TAP_CODE16(tap_key);
			MOD_DIFFERENT_ON;
		} else { // IS_TAP 1回目のキーを押した時に初期設定
			SET_MOD_TAP;
		} // IS_TAP
	} // IS_KEY_PRESS
	return false; // ダブルタップ操作中は false
}

// レイヤータップ関数 #############################################
// 引数: タップされるキーコード、操作するレイヤー
// 戻り値: true-押した時、条件不成立で離した時、false-タップ時(条件成立で離した時)
bool layer_tap16(uint16_t tap_key, uint8_t hold_layer, uint16_t keycode, keyrecord_t *record) {
	if (IS_KEY_PRESS) {
		layer_on(hold_layer); // キーを押した時にレイヤーを変更して初期設定
		SET_MOD_TAP;
	} else { // IS_KEY_PRESS
		layer_off(hold_layer);
		if (IS_TAP) { // 設定時間内に同じキーを離したらタップ
			RST_MOD_TAP;
			TAP_CODE16(tap_key);
			return false; // タップ時にfalse
		} // IS_TAP
	} // IS_KEY_PRESS
	return true;
}

// End |||||||||||||||||||||||||||||||||||||||||||||
