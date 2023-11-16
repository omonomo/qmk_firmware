#pragma once

#include QMK_KEYBOARD_H

// MODキー(x_register_code16用) #############################################
#define MOD_REGISTER_KEY KC_LCTL, KC_LSFT, KC_LALT, KC_LGUI

// 関数置き替え #############################################
#define RF_REPEAT_KEY(code, times) return repeat_key(code, times)

#define REGISTER_CODE(code) register_code(code)
#define UNREGISTER_CODE(code) unregister_code(code)

#define REGISTER_CODE16(code) x_register_code16(code)
#define UNREGISTER_CODE16(code) x_unregister_code16(code)

#define TAP_CODE(code) tap_code(code)
#define RF_TAP_CODE(code) TAP_CODE(code); return false

#define TAP_CODE_UJ(codeU, codeJ) TAP_CODE(!IS_JIS ? codeU : codeJ)
#define RF_TAP_CODE_UJ(codeU, codeJ) TAP_CODE_UJ(codeU, codeJ); return false

#define TAP_CODE16(code) x_tap_code16(code)
#define RF_TAP_CODE16(code) TAP_CODE16(code); return false

#define TAP_CODE16_UJ(codeU, codeJ) TAP_CODE16(!IS_JIS ? codeU : codeJ)
#define RF_TAP_CODE16_UJ(codeU, codeJ) TAP_CODE16_UJ(codeU, codeJ); return false

// 関数プロトタイプ宣言 #############################################
bool repeat_key(uint8_t tap_key, uint8_t times);

void x_register_code16(uint16_t reg_key);
void x_unregister_code16(uint16_t reg_key);
void x_tap_code16(uint16_t reg_key);

// End |||||||||||||||||||||||||||||||||||||||||||||
