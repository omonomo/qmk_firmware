#include "func_key_output.h"

#include "ini_config.h"
#include "main_keymap.h"

// パーツ関数 |||||||||||||||||||||||||||||||||||||||||||||

// 連続出力関数 #############################################
// 引数: タップされるキーコード、回数
// 戻り値: false
bool repeat_key(uint8_t tap_key, uint8_t times) {
	for (int8_t i = 0; i < times; ++i) {
		tap_code(tap_key);
		wait_ms(TAP_AFTER_DELAY_S);
	} // i
	return false;
}

// キー押し関数(16bit) #############################################
// 引数: 押すキーコード
void x_register_code16(uint16_t reg_key) {
//	if (reg_key == 0) return;

	const uint8_t MOD_KC[] = {MOD_REGISTER_KEY};

	for (int8_t i = 11; 7 < i; --i) { // 先にMODをレジスト
		if (reg_key & (1 << i)) { // フラグ部分を処理
			register_code(MOD_KC[i - 8]);
		} // reg_key
	} // i
	register_code(reg_key); // MOD無しでレジスト
	return;
}

// キー離し関数(16bit) #############################################
// 引数: 離すキーコード
void x_unregister_code16(uint16_t reg_key) {
//	if (reg_key == 0) return;

	const uint8_t MOD_KC[] = {MOD_REGISTER_KEY};

	unregister_code(reg_key); // MOD無しでアンレジスト
	for (int8_t i = 8; i < 12; ++i) { // 後でMODをアンレジスト
		if (reg_key & (1 << i)) { // (unregister_code16だとなぜか先にMODがアンレジストされる)
			unregister_code(MOD_KC[i - 8]);
		} // mod_key
	} // i
	return;
}

// キータップ関数(16bit) #############################################
// 引数: タップするキーコード
void x_tap_code16(uint16_t reg_key) {
//	if (reg_key == 0) return;

	REGISTER_CODE16(reg_key);
	wait_ms(TAP_AFTER_DELAY_S);
	UNREGISTER_CODE16(reg_key);
	return;
}

// End |||||||||||||||||||||||||||||||||||||||||||||
