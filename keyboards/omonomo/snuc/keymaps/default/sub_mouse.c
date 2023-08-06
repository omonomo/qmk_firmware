#include "sub_mouse.h"

#include "ini_config.h"
#include "func_mod_output.h"
#include "func_tap_replace.h"
#include "sub_counter.h"
#include "main_code_layer.h"
#include "main_keymap.h"

// フラグ・変数 #############################################
static mouse_e mouse_state = STOP; // マウスキー(停止/オフディレイ中/操作中/操作開始)
static uint8_t now_mouse_interval = 0; // 現在のマウス移動インターバル
static uint8_t now_mouse_ac = 0; // 現在のマウス加速度
static int8_t mouse_xyv[] = {
	0, // MOUSE_X マウスポインタ
	0, // MOUSE_Y マウスポインタ
	0  // MOUSE_V ホイール
}; // マウス各座標の移動量

// サブルーチン関数 |||||||||||||||||||||||||||||||||||||||||||||

// マウス移動実行 #############################################
void ms_mouse_key(global_s *global) {
	if (mouse_state == STOP) return;
	if (REPEAT_COUNT != 0) return;

	if (WHEEL_MOVE <= mouse_state) { // マウス操作中の場合、REPEAT_COUNT == 0 で実行とカウント数変更
		report_mouse_t current_report = pointing_device_get_report(); // マウス状態取得
		current_report.x = mouse_xyv[MOUSE_X];
		current_report.y = mouse_xyv[MOUSE_Y];
		current_report.v = mouse_xyv[MOUSE_V];
		pointing_device_set_report(current_report);
		pointing_device_send(); // 変更状態出力

		if (WHEEL_START <= mouse_state) { // 操作開始(_START)時は加減速処理なしで_MOVEに移行
			SET_REPEAT_COUNT(now_mouse_interval);
			mouse_state -= MOUSE_KIND_NUM;
		} else { // WHEEL、POINTER操作中の場合
			const uint8_t THM = (mouse_state == WHEEL_MOVE ? WHEEL_THM : POINTER_THM); // 速度計算の定数設定
			const uint8_t AC  = (mouse_state == WHEEL_MOVE ? WHEEL_AC  : POINTER_AC);
			const uint8_t THH = (mouse_state == WHEEL_MOVE ? WHEEL_THH : POINTER_THH);
			const uint8_t AD  = (mouse_state == WHEEL_MOVE ? WHEEL_AD  : POINTER_AD);

			if (IS_MOD_PRESS(_F _M)) { // METキーかFNキーを押している時は常に一定
				SET_REPEAT_COUNT(now_mouse_interval = THM);
				now_mouse_ac = AC;
			} else { // IS_MOD_PRESS
				now_mouse_interval -= (now_mouse_ac <= now_mouse_interval ? now_mouse_ac : now_mouse_interval); // インターバルを加速度分短くする
				SET_REPEAT_COUNT(THH < now_mouse_interval ? now_mouse_interval : THH); // 高速設定を越えないように調整
				now_mouse_ac -= (AD < now_mouse_ac ? AD : (now_mouse_ac - 1)); // 加速度も加速減少度分落としていく
			} // IS_MOD_PRESS
		} // WHEEL_START

	} else if (WHEEL_OFF <= mouse_state) { // WHEEL_MOVE
		mouse_state = STOP; //  キーを離した後、REPEAT_COUNT == 0 でSTOP
	} // WHEEL_OFF
	return;
}

// マウスキー #############################################
bool pr_mouse_key(uint16_t keycode, keyrecord_t *record, global_s *global) {
	static uint8_t mouse_flag = 0; // マウスキー押下フラグ

	switch (keycode) {
		case CK_MET:
			if (IS_KEY_PRESS) {
				if (IS_MOUSE_MODE) return false;
			}
		break;

		case MT_LSFT_SPC:
		case MT_RSFT_SPC:
			if (IS_KEY_PRESS) {
				if (IS_MOUSE_MODE) return false;
				if (IS_MOD_PRESS_COMP(_E)) { // 他のMODを押してない状態でSFT_SPC両押しでレイヤー変更
					layer_on(_MOUSE);
				} // IS_MOD_PRESS
			} else { // IS_KEY_PRESS
				if (IS_TAP || !IS_MOD_PRESS(_E)) { // タップ時はマウスモードにしない
					MOUSE_MODE_OFF; // SFT_SPC両離しでマウスモード解除
					layer_off(_MOUSE);
				} // IS_MOD_PRESS
			} // IS_KEY_PRESS
		break;

		case KC_ENT:
		case KC_ESC:
		case MS_LEFT ... MS_ESC:
			if (IS_KEY_PRESS) {
				if (!IS_MOUSE_MODE) {
					MOUSE_MODE_ON; // マウスキーを押してマウスモードへ
					UNREGISTER_P_MOD_IF_ALONE(KC_LSFT, IS_ANY_MOD_PRESS);
					UNREGISTER_P_MOD_IF_ALONE(KC_RSFT, IS_ANY_MOD_PRESS);
					UNREGISTER_P_MOD_IF_ALONE(MET_KEY, IS_ANY_MOD_PRESS);
				} // IS_MOUSE_MODE
			} // IS_KEY_PRESS
		break;

		default:
		break;
	}

	switch (keycode) {
		case MS_LEFT ... MS_WUP: { // ポインタ、ホイール
			const int8_t MOUSE_MV[] = {
				-POINTER_MV, -POINTER_MV, -WHEEL_MV, // LEFT UP   WDWN
				 POINTER_MV,  POINTER_MV,  WHEEL_MV  // RGHT DOWN WUP
			};
			const uint8_t MOUSE_DELTA = keycode - MS_LEFT; // マウス移動量(MOUSE_MV)の配列番号
			const uint8_t MOUSE_FLAG_BIT = 1 << MOUSE_DELTA; // マウスキー押下フラグ
			const uint8_t MOUSE_COORDINATE= MOUSE_DELTA % ARRAY_NUM(mouse_xyv); // 操作する座標(mouse_xyv)の配列番号
			const uint8_t MOUSE_PAIR_DELTA = (MOUSE_DELTA + ARRAY_NUM(mouse_xyv)) % (ARRAY_NUM(mouse_xyv) * 2); // 対になるマウス移動量の配列番号
			const uint8_t MOUSE_PAIR_FLAG_BIT = 1 << MOUSE_PAIR_DELTA; // 対になるマウスキーの押下フラグ

			if (IS_KEY_PRESS) {
				wait_ms(MOUSE_ON_DELAY); // SFT誤出力防止
				mouse_flag |= MOUSE_FLAG_BIT; // キーを押した時に
				mouse_xyv[MOUSE_COORDINATE] = MOUSE_MV[MOUSE_DELTA]; // 該当する座標に移動量を入れる
			} else { // IS_KEY_PRESS
				mouse_flag &= ~MOUSE_FLAG_BIT; // キーを離した時に
				mouse_xyv[MOUSE_COORDINATE] = (mouse_flag & MOUSE_PAIR_FLAG_BIT ? MOUSE_MV[MOUSE_PAIR_DELTA] : 0); // 対になるマウスキーが押されていたら移動量を入れる
			} // IS_KEY_PRESS

			if (mouse_xyv[MOUSE_V] != 0) { // ホイール操作中
				mouse_xyv[MOUSE_X] = mouse_xyv[MOUSE_Y] = 0; // ポインタ停止
				if (mouse_state == STOP || mouse_state % MOUSE_KIND_NUM == 0) { // マウスが静止状態か、ポインタ操作後の場合速度設定リセット
					now_mouse_interval = WHEEL_THL;
					now_mouse_ac = WHEEL_AC;
				} // mouse_state
				mouse_state = (mouse_state == WHEEL_MOVE ? WHEEL_MOVE : WHEEL_START); // 直前にホイール操作をしていなかった場合WHEEL_START
			} else if (mouse_xyv[MOUSE_X] != 0 || mouse_xyv[MOUSE_Y] != 0) { // ポインタ操作中
				if (mouse_state == STOP || mouse_state % MOUSE_KIND_NUM == 1) { // マウスが静止状態か、ホイール操作後の場合は速度設定リセット
					now_mouse_interval = POINTER_THL;
					now_mouse_ac = POINTER_AC;
				} // mouse_state
				mouse_state = (mouse_state == POINTER_MOVE ? POINTER_MOVE : POINTER_START);
			} else if (WHEEL_MOVE <= mouse_state) { // キーを離した直後にオフディレイカウント開始、速度設定は維持
				SET_REPEAT_COUNT(OFF_DELAY_TH); // オフディレイ値設定
				mouse_state -= MOUSE_KIND_NUM; // WHEEL_OFF、POINTER_OFFに移行
			} // WHEEL_MOVE
			return false;
		} // case
		break;

		case MS_BTN1 ... MS_BTN5: { // ボタン
			wait_ms(MOUSE_ON_DELAY);
			report_mouse_t current_report = pointing_device_get_report();
			const uint8_t MOUSE_BUTTON = 1 << (keycode - MS_BTN1); // 操作するボタンのフラグ

			if (IS_KEY_PRESS) {
				current_report.buttons |= MOUSE_BUTTON;
			} else { // IS_KEY_PRESS
				current_report.buttons &= ~MOUSE_BUTTON;
			} // IS_KEY_PRESS
			pointing_device_set_report(current_report);
			pointing_device_send();
			return false;
		} // case
		break;

		case MS_ESC: { // ポインタエスケープ
			if (IS_KEY_PRESS) {
				wait_ms(MOUSE_ON_DELAY);
				report_mouse_t current_report = pointing_device_get_report();
				current_report.x = current_report.y = POINTER_MVE;
				pointing_device_set_report(current_report);
				pointing_device_send();
			} // IS_KEY_PRESS
			return false;
		} // case
		break;

		default:
		break;
		}
	return true;
}

// End |||||||||||||||||||||||||||||||||||||||||||||
