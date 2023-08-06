#include "sub_counter.h"

#include "main_keymap.h"

// フラグ・変数 #############################################
static uint16_t ms_counter[] = {
	0, // TERM
	0  // REPEAT
};

// 補助関数 |||||||||||||||||||||||||||||||||||||||||||||

// カウンタセット関数 #############################################
// 引数: カウンタ名、セット値
void set_counter(counter_e cnt_name, uint16_t set_cnt) {
	ms_counter[cnt_name] = set_cnt;
	return;
}

// カウンタゲット関数 #############################################
// 引数: カウンタ名
// 戻り値: カウント値
uint16_t get_counter(counter_e cnt_name) {
	return ms_counter[cnt_name];
}

// サブルーチン関数 |||||||||||||||||||||||||||||||||||||||||||||

// カウントダウン #############################################
void ms_countdown(void) {
	for (int8_t i = 0; i < ARRAY_NUM(ms_counter); ++i) {
		if (0 < ms_counter[i]) { // カウントダウン
			--ms_counter[i];
		} // ms_counter
	} // i
	return;
}

// End |||||||||||||||||||||||||||||||||||||||||||||
