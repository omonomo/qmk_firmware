#pragma once

#include QMK_KEYBOARD_H
#include "main_keymap.h"

// マウスキー用 #############################################
#define MOUSE_KIND_NUM 2 // マウスポインタとホイールの2種類、mouse_stateの計算で使用
enum mouse_coordinate {
	MOUSE_X, MOUSE_Y, MOUSE_V
}; // マウス座標配列番号

// 型定義 #############################################
typedef enum {
	STOP, // 停止
	WHEEL_OFF, POINTER_OFF, // 中断
	WHEEL_MOVE, POINTER_MOVE, // 操作中
	WHEEL_START, POINTER_START // 開始
} mouse_e; // マウスステート

// 関数プロトタイプ宣言 #############################################
void ms_mouse_key(global_s *global);

bool pr_mouse_key(uint16_t keycode, keyrecord_t *record, global_s *global);

// End |||||||||||||||||||||||||||||||||||||||||||||
