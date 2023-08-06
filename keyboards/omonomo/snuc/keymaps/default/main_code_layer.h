#pragma once

#include QMK_KEYBOARD_H

// キー置き替え #############################################
#define X_HYPR(code) (IS_WIN ? C(S(A(code))) : HYPR(code))

#define KK_EISU kk_eisu(global)
#define KK_KANA kk_kana(global)

#define KK_LGUI_C (GUI_IS_CTL ? KC_LCTL : KC_LGUI) // GUIで入力されるキーコード
#define KK_RGUI_C (GUI_IS_CTL ? KC_RCTL : KC_RGUI)
#define KK_LCTL_G (GUI_IS_CTL ? KC_LGUI : KC_LCTL) // CTLで入力されるキーコード
#define KK_RCTL_G (GUI_IS_CTL ? KC_RGUI : KC_RCTL)

// METキー #############################################
#define MET_KEY KC_LCTL
#define M(code) C(code)
#define UNREGISTER_MET unregister_code(MET_KEY)
#define REGISTER_MET     register_code(MET_KEY)

// switch case文用 #############################################
#define CS_LCTL KC_LCTL: case DT_LCTL_HYPR // 物理的に同じキーをまとめる
#define CS_RCTL KC_RCTL: case DT_RCTL_HYPR
#define CS_LSFT KC_LSFT: case DT_LSFT_HYPR
#define CS_RSFT KC_RSFT: case DT_RSFT_HYPR
#define CS_LGUI KC_LGUI: case MT_LGUI_EISU
#define CS_RGUI KC_RGUI: case MT_RGUI_KANA

// カスタムキーコード #############################################
#define CK_HIRA KC_INT2 // ひらがな
#define CK_HENK KC_INT4 // 変換
#define CK_MHEN KC_INT5 // 無変換

enum keycodes {
// モード変更用 =============================================
	CK_JIS = SAFE_RANGE,
	CK_US,
	CK_COL,  CK_QWTY, CK_GAME,
	CK_WIN,  CK_MAC,  CK_IPHN, CK_IPAD,
	CK_ROUT, CK_RIN,  CK_ALPH,
	CK_LEJ,  CK_MEJ,
	CK_GC,   CK_GG,
	CK_TON,  CK_TOFF,
	CK_HALF, CK_FULL,
	CK_TEL,
	CK_WKUP,
// 入力モード切り替え用 =============================================
	CK_EISU, CK_KANA,
// テンキー拡張用 =============================================
	CK_P000, // 始めP000、絶対半角終わりCK_PEQL、テンキー終わりCK_DELを崩さない
	CK_PCMM, CK_PCLN, CK_PEQL,
	CK_UNDO, CK_CUT,  CK_COPY, CK_PAST, CK_DEL,
// カスタムMOD =============================================
	CK_MET, // モッドタップの前に置く
// モッドタップキー =============================================
	MT_LSFT_SPC,  MT_RSFT_SPC,
	MT_LGUI_EISU, MT_RGUI_KANA,
	MT_RCTL_ENT,
// ダブルタップキー =============================================
	DT_LCTL_HYPR, DT_RCTL_HYPR,
	DT_LSFT_HYPR, DT_RSFT_HYPR,
// レイヤータップキー =============================================
	LT_LFNC_ESC, LT_RFNC_ESC, LT_CRSR_TAB, LT_MODE_PENT,
// ワンショットレイヤーキー =============================================
	OL_MIZU_LD, OL_MIZU_LS, OL_MIZU_RS, OL_MIZU_RD,
// マウスキー用 =============================================
	MS_LEFT, MS_UP,   MS_WDWN, MS_RGHT, MS_DOWN, MS_WUP, // 順番注意(-X,-Y,-V,+X,+Y,+Vを崩さない)
	MS_BTN1, MS_BTN2, MS_BTN3, MS_BTN4, MS_BTN5, MS_ESC,
// ローマ字出力用 =============================================
	RM_A,    RM_I,    RM_U,    RM_E,    RM_O,    RM_KA,   RM_KI,   RM_KU,   RM_KE,  RM_KO,
	RM_SA,   RM_SI,   RM_SU,   RM_SE,   RM_SO,   RM_TA,   RM_TI,   RM_TU,   RM_TE,  RM_TO,
	RM_NA,   RM_NI,   RM_NU,   RM_NE,   RM_NO,   RM_HA,   RM_HI,   RM_HU,   RM_HE,  RM_HO,
	RM_MA,   RM_MI,   RM_MU,   RM_ME,   RM_MO,   RM_YA,   RM_YU,   RM_YO,
	RM_RA,   RM_RI,   RM_RU,   RM_RE,   RM_RO,   RM_WA,   RM_WYI,  RM_WYE,  RM_WO,  RM_NN,
	RM_VU,
	RM_GA,   RM_GI,   RM_GU,   RM_GE,   RM_GO,   RM_ZA,   RM_ZI,   RM_ZU,   RM_ZE,  RM_ZO,
	RM_DA,   RM_DI,   RM_DU,   RM_DE,   RM_DO,   RM_BA,   RM_BI,   RM_BU,   RM_BE,  RM_BO,
	RM_PA,   RM_PI,   RM_PU,   RM_PE,   RM_PO,
	RM_XA,   RM_XI,   RM_XU,   RM_XE,   RM_XO,   RM_XTU,  RM_XYA,  RM_XYU,  RM_XYO, RM_XWA,
	RM_LKKK, RM_RKKK, RM_L2KK, RM_R2KK, RM_LSKK, RM_RSKK, RM_LPRN, RM_RPRN,
	RM_NAMI, RM_LEAD, RM_EXC,  RM_QUE,
	RM_H,    RM_TEN // 始めRM_A、ローマ字終わりRM_XWA、記号終わりRM_TENを崩さない
};

// レイヤー #############################################
enum layers {
	_COLEMAK,  _QWERTY,
	_MIZU, _MIZU_LD, _MIZU_LS, _MIZU_RS, _MIZU_RD,
	_GAME, _CURSOR,  _NUMERIC,
	_LFNC, _RFNC,    _MODE,    _MOUSE
};

// End |||||||||||||||||||||||||||||||||||||||||||||
