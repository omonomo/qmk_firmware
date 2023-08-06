#pragma once

#include QMK_KEYBOARD_H

// 関数置き替え #############################################
#define SET_TERM_COUNT(count) set_counter(TERM, count)
#define SET_REPEAT_COUNT(count) set_counter(REPEAT, count)

#define TERM_COUNT get_counter(TERM)
#define REPEAT_COUNT get_counter(REPEAT)

// 型定義 #############################################
typedef enum {
	TERM, REPEAT
} counter_e; // カウンタ配列番号

// 関数プロトタイプ宣言 #############################################
void set_counter(counter_e cnt_name, uint16_t set_cnt);
uint16_t get_counter(counter_e cnt_name);

void ms_countdown(void);

// End |||||||||||||||||||||||||||||||||||||||||||||
