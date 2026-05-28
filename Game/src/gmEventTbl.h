// ================================================================
/*!
  @file gmEventTbl.h
  @brief イベント情報

  @author Ishizaki
				Copyright(c) 2009 Dimps

  $Id: gmEventTbl.h 2 2011-04-11 05:21:26Z thamada $
  $Date: 2009-02-28 15:39:58 +0900#$
 */
// ================================================================
/*
 * $Log: gmEventTbl.h,v $
 *
 */

#ifndef GM_EVENT_TBL_H_
#define GM_EVENT_TBL_H_


//----- Include Files -------------------------------------------------------
#include "objObject.h"

#include "gmMain.h"
#include "gmEventMgr.h"

#if	defined(__cplusplus)
extern "C" {
#endif

//----- Definitions ---------------------------------------------------------
/// イベントID
typedef enum tag_GME_EVENT_ID {
	// 敵(ボス含む 50個)
	GMD_EVENT_ID_ENEMY_START		= 0,
//  00
	// ザコ
	GMD_EVENT_ID_ENE_HARISENBO	= GMD_EVENT_ID_ENEMY_START,	//!< エネミー ハリセンボ
	GMD_EVENT_ID_ENE_MOTORA,		//!< エネミー モトラ
	GMD_EVENT_ID_ENE_STINGER,		//!< エネミー スティンガー
	GMD_EVENT_ID_ENE_GABUCCHO,		//!< エネミー ガブッチョ
	GMD_EVENT_ID_ENE_MEREON,		//!< エネミー メレオン
	GMD_EVENT_ID_ENE_MEREON_F,		//!< エネミー メレオン 落下タイプ
	GMD_EVENT_ID_ENE_MOGRIN,		//!< エネミー モグリン
	GMD_EVENT_ID_ENE_GUARDON,		//!< エネミー ガードン
	GMD_EVENT_ID_ENE_TERUSTAR,		//!< エネミー テルスター
	GMD_EVENT_ID_ENE_KANI,			//!< エネミー カニパンチ

//  10
	GMD_EVENT_ID_ENE_HAROGEN,		//!< エネミー　ハロゲン
	GMD_EVENT_ID_ENE_UNIDESU,		//!< エネミー　ウニデス
	GMD_EVENT_ID_ENE_UNIUNI,		//!< エネミー　ウニウニ
	GMD_EVENT_ID_ENE_BUKUBUKU,		//!< エネミー　ぶくぶく
	GMD_EVENT_ID_ENE_KAMA,			//!< エネミー　カマキラー
	GMD_EVENT_ID_ENE_HARISENBO_R,	//!< エネミー　ハリセンボ強

	GMD_EVENT_ID_ENE_ZAKO_END	= 39,
	GMD_EVENT_ID_ENE_ZAKO_NUM	= GMD_EVENT_ID_ENE_ZAKO_END - GMD_EVENT_ID_ENEMY_START + 1,


	// ボス
	GMD_EVENT_ID_ENE_BOSS_TOP	= 40,				//!< ここ以降ボス

	GMD_EVENT_ID_ENE_BOSS_FINAL	= 55,
	GMD_EVENT_ID_ENE_BOSS4		= 56,
	GMD_EVENT_ID_ENE_BOSS3		= 57,
	GMD_EVENT_ID_ENE_BOSS2		= 58,
	GMD_EVENT_ID_ENE_BOSS1		= 59,

	GMD_EVENT_ID_ENE_NUM	= 60,



	// ギミック
	GMD_EVENT_ID_GIMMICK_START	= 60,
//  60(00)
	GMD_EVENT_ID_GMK_TOUCH_EARTH	= GMD_EVENT_ID_GIMMICK_START,// 000接地
	GMD_EVENT_ID_GMK_A,					// 001 A面切り替え
	GMD_EVENT_ID_GMK_B,					// 002 B面切り替え
	GMD_EVENT_ID_GMK_ITEM_HISPEED,		// 003
	GMD_EVENT_ID_GMK_ITEM_INVINCIBLE,	// 004
	GMD_EVENT_ID_GMK_ITEM_RING_10,		// 005
	GMD_EVENT_ID_GMK_ITEM_BARRIER,		// 006
	GMD_EVENT_ID_GMK_ITEM_1UP,			// 007
	GMD_EVENT_ID_GMK_ITEM_RESERVE1,		// 008
	GMD_EVENT_ID_GMK_ITEM_RESERVE2,		// 009

//  70(10)
	GMD_EVENT_ID_GMK_SPRING_U,			// 010 スプリング上
	GMD_EVENT_ID_GMK_SPRING_RU,			// 011 スプリング右上
	GMD_EVENT_ID_GMK_SPRING_R,			// 012 スプリング右
	GMD_EVENT_ID_GMK_SPRING_RD,			// 013 スプリング右下
	GMD_EVENT_ID_GMK_SPRING_D,			// 014 スプリング下
	GMD_EVENT_ID_GMK_SPRING_LD,			// 015 スプリング左下
	GMD_EVENT_ID_GMK_SPRING_L,			// 016 スプリング左
	GMD_EVENT_ID_GMK_SPRING_LU,			// 017 スプリング左上
	GMD_EVENT_ID_GMK_SPRING_RUG,		// 018 スプリング右上(地面埋まり)
	GMD_EVENT_ID_GMK_SPRING_LUG,		// 019 スプリング左上(地面埋まり)

//  80(20)
	GMD_EVENT_ID_GMK_START,				// 020 スタート位置設定
	GMD_EVENT_ID_LAND_CMN,				// 021 浮島(通常)
	GMD_EVENT_ID_LAND_BIG,				// 022 浮島(大)
	GMD_EVENT_ID_LAND_COL,				// 023 浮島(当たり)
	GMD_EVENT_ID_ROCK_CHASE,			// 024 大岩(追跡)
	GMD_EVENT_ID_ROCK_FALL,				// 025 大岩(落下)
	GMD_EVENT_ID_PULLEY_BASE,			// 026 滑車本体
	GMD_EVENT_ID_PULLEY_POLE_L,			// 027 滑車ポール（左）
	GMD_EVENT_ID_PULLEY_POLE_R,			// 028 滑車ポール（右）
	GMD_EVENT_ID_PULLEY_ROPE_F,			// 029 滑車ロープ（水平）
	
//  90(30)
	GMD_EVENT_ID_PULLEY_ROPE_TR,		// 030 滑車ロープ（斜め：右）
	GMD_EVENT_ID_NEEDLE_U,				// 031 トゲ(上)
	GMD_EVENT_ID_NEEDLE_L,				// 032 トゲ(左)
	GMD_EVENT_ID_NEEDLE_D,				// 033 トゲ(下)
	GMD_EVENT_ID_NEEDLE_R,				// 034 トゲ(右)
	GMD_EVENT_ID_PULLEY_TROPE_TL,		// 035 滑車ロープ（斜め：左）
	GMD_EVENT_ID_ROCK_RIDE,				// 036 大岩(傾斜)
	GMD_EVENT_ID_ACT_NEEDLE_U,			// 037 出入りトゲ(上)
	GMD_EVENT_ID_ACT_NEEDLE_D,			// 038 出入りトゲ(下)
	GMD_EVENT_ID_FALLDIE,				// 039 落下ミス判定

// 100(40)
	GMD_EVENT_ID_GMK_SPRING_RUG_A,		// 040 スプリング右上(地面埋まり) A面用
	GMD_EVENT_ID_GMK_SPRING_LUG_A,		// 041 スプリング左上(地面埋まり) A面用
	GMD_EVENT_ID_WATER_AREA_L,			// 042 水位変更（左から）
	GMD_EVENT_ID_WATER_AREA_R,			// 043 水位変更（右から）
	GMD_EVENT_ID_WATER_AREA_T,			// 044 水位変更（上から）
	GMD_EVENT_ID_WATER_AREA_B,			// 045 水位変更（下から）
	GMD_EVENT_ID_WATER_AREA_F,			// 046 水位変更（開始時、再開時）
	GMD_EVENT_ID_GMK_DASH_PANEL_R,		// 047 ダッシュパネル右
	GMD_EVENT_ID_GMK_DASH_PANEL_L,		// 048 ダッシュパネル左
	GMD_EVENT_ID_GMK_DASH_PANEL_VR,		// 049 ダッシュパネル縦右壁

// 110(50)
	GMD_EVENT_ID_GMK_DASH_PANEL_VL,		// 050 ダッシュパネル縦左壁
	GMD_EVENT_ID_BUBBLE,				// 051 息継ぎの泡
	GMD_EVENT_ID_TARZAN_ROPE,			// 052 ターザンロープ
	GMD_EVENT_ID_TARZAN_ROPE_L,			// 053 ターザンロープ左
	GMD_EVENT_ID_TARZAN_ROPE_R,			// 054 ターザンロープ右
	GMD_EVENT_ID_WATER_SLIDER_L,		// 055 ウォータースライダー左（真下）
	GMD_EVENT_ID_WATER_SLIDER_L_30D,	// 056 ウォータースライダー左（30度）
	GMD_EVENT_ID_WATER_SLIDER_L_45D,	// 057 ウォータースライダー左（45度）
	GMD_EVENT_ID_WATER_SLIDER_L_60D,	// 058 ウォータースライダー左（60度）
	GMD_EVENT_ID_WATER_SLIDER_R,		// 059 ウォータースライダー右（真下）

// 120(60)
	GMD_EVENT_ID_WATER_SLIDER_R_30D,	// 060 ウォータースライダー右（30度）
	GMD_EVENT_ID_WATER_SLIDER_R_45D,	// 061 ウォータースライダー右（45度）
	GMD_EVENT_ID_WATER_SLIDER_R_60D,	// 062 ウォータースライダー右（60度）

	GMD_EVENT_ID_GOALPANEL,				// 063 ゴールパネル

	GMD_EVENT_ID_BREAKLAND1_R,			// 064 崩落足場　右オーバーハング
	GMD_EVENT_ID_BREAKLAND1_L,			// 065 崩落足場　左オーバーハング

	GMD_EVENT_ID_PISTON_UP,				// 066 ピストン　上向き
	GMD_EVENT_ID_PISTON_DOWN,			// 067 ピストン　下向き

	GMD_EVENT_ID_LAND_AROUND,			// 068 浮島(回転移動)

	GMD_EVENT_ID_POINTMARKER,			// 069 ポイントマーカー

// 130(70)
	GMD_EVENT_ID_BREAKOBJ,				// 070 破壊可能オブジェ
	GMD_EVENT_ID_CHANGE_CAM_CENTER,		// 071 カメラセンター変更
	GMD_EVENT_ID_SCR_LIMIT_SET,			// 072 スクロール制限
	GMD_EVENT_ID_BELTCONVEYOR,			// 073 ベルトコンベヤー
	GMD_EVENT_ID_BREAKWALL1_L1,			// 074 破壊可能壁 端っこのデザイン
	GMD_EVENT_ID_BREAKWALL1_L2,			// 075 破壊可能壁 中間までのデザイン
	GMD_EVENT_ID_BREAKWALL1_R1,			// 076 破壊可能壁 端っこのデザイン
	GMD_EVENT_ID_BREAKWALL1_R2,			// 077 破壊可能壁 中間までのデザイン
	GMD_EVENT_ID_BREAKWALL1_C,			// 078 破壊可能壁 中間のデザイン
	GMD_EVENT_ID_S_PIPE,				// 079 Ｓ字パイプ

// 140(80)
	GMD_EVENT_ID_BREAKWALL1_C2,			// 080 破壊可能壁 中間のデザイン２
	GMD_EVENT_ID_STOPPER_NORM,			// 081 ストッパー＠ピンボール ノーマルボーナス
	GMD_EVENT_ID_STOPPER_SLOT,			// 082 ストッパー＠ピンボール スロットボーナス

	GMD_EVENT_ID_BREAKFLOOR,			// 083 破壊可能床

	GMD_EVENT_ID_UPBUMPER_L,			// 084 登るバンパー左くっつき
	GMD_EVENT_ID_UPBUMPER_R,			// 085 登るバンパー右くっつき

	GMD_EVENT_ID_BUMPER_TRI_I_TOP,		// 086 バンパー二等辺三角形（上に配置用）
	GMD_EVENT_ID_BUMPER_TRI_I_BOTTOM,	// 087 バンパー二等辺三角形（下に配置用）
	GMD_EVENT_ID_BUMPER_TRI_I_LEFT,		// 088 バンパー二等辺三角形（左に配置用）
	GMD_EVENT_ID_BUMPER_TRI_I_RIGHT,	// 089 バンパー二等辺三角形（右に配置用）
// 150(90)
	GMD_EVENT_ID_BUMPER_TRI_R_LT,		// 090 バンパー直角三角形（左上に配置用）
	GMD_EVENT_ID_BUMPER_TRI_R_LB,		// 091 バンパー直角三角形（左下に配置用）
	GMD_EVENT_ID_BUMPER_TRI_R_RT,		// 092 バンパー直角三角形（右上に配置用）
	GMD_EVENT_ID_BUMPER_TRI_R_RB,		// 093 バンパー直角三角形（右下に配置用）
	GMD_EVENT_ID_BUMPER_HEX_W,			// 094 バンパー六角形（横長）
	GMD_EVENT_ID_BUMPER_HEX_H,			// 095 バンパー六角形（縦長）
	GMD_EVENT_ID_SPEAR_U,				// 096 槍(上向↑)
	GMD_EVENT_ID_SPEAR_D,				// 097 槍(下向↓)
	GMD_EVENT_ID_SPEAR_L,				// 098 槍(左向←)
	GMD_EVENT_ID_SPEAR_R,				// 099 槍(右向→)

// 160(100)
	GMD_EVENT_ID_SCREW,					// 100 コークスクリュー
	GMD_EVENT_ID_CANNON,				// 101 大砲
	GMD_EVENT_ID_LOOP_CAMERA,			// 102 ループカメラ
	GMD_EVENT_ID_CAPSULE,				// 103 カプセル
	GMD_EVENT_ID_EN_BMPR_0,				// 104 ３耐バンパー（0度）
	GMD_EVENT_ID_EN_BMPR_45,			// 105 ３耐バンパー（45度）
	GMD_EVENT_ID_EN_BMPR_90,			// 106 ３耐バンパー（90度）
	GMD_EVENT_ID_EN_BMPR_135,			// 107 ３耐バンパー（135度）
	GMD_EVENT_ID_BOBBIN,				// 108 ボビン
	GMD_EVENT_ID_FLIPPER_UL,			// 109 フリッパー（上にはじく：左側）
// 170(110)
	GMD_EVENT_ID_FLIPPER_UR,			// 110 フリッパー（上にはじく：右側）
	GMD_EVENT_ID_FLIPPER_LR,			// 111 フリッパー（左右にはじく）
	GMD_EVENT_ID_SLOT,					// 112 スロット (G082と連動)
	GMD_EVENT_ID_SEESAW0,				// 113 シーソー
	GMD_EVENT_ID_SEESAW30,				// 114 シーソー
	GMD_EVENT_ID_SEESAW330,				// 115 シーソー
	GMD_EVENT_ID_BRIDGE,				// 116 丸太橋
	GMD_EVENT_ID_SCR_LIMIT_RELEASE,		// 117 スクロール制限解除
	GMD_EVENT_ID_SP_CTPLT_0,			// 118 スプリングカタパルト０°
	GMD_EVENT_ID_SP_CTPLT_45,			// 119 スプリングカタパルト４５°
// 180(G120～)
	GMD_EVENT_ID_SP_CTPLT_315,			// 120 スプリングカタパルト３１５°
	GMD_EVENT_ID_GMK_GEAR,				// 121 歯車
	GMD_EVENT_ID_GMK_MOVE_GEAR,			// 122 移動歯車
	GMD_EVENT_ID_GMK_MOVE_GEAR_END,		// 123 移動歯車終点
	GMD_EVENT_ID_GMK_GEAR_SWITCH,		// 124 歯車スイッチ
	GMD_EVENT_ID_FORCESPIN_A,			// 125 強制スピンモードＡ ┏
	GMD_EVENT_ID_FORCESPIN_B,			// 126 強制スピンモードＢ  ┓
	GMD_EVENT_ID_FORCESPIN_C,			// 127 強制スピンモードＣ ┗
	GMD_EVENT_ID_FORCESPIN_D,			// 128 強制スピンモードＤ  ┛
	GMD_EVENT_ID_PRESSWALL,				// 129 迫る壁
// 190(130)
	GMD_EVENT_ID_PRESSWALL_STOP,		// 130 迫る壁 停止
	GMD_EVENT_ID_PRESSWALL_CONTROL,		// 131 迫る壁 速度変更
	GMD_EVENT_ID_SS_SQUARE,				// 132 SpecialStage 四角柱
	GMD_EVENT_ID_SS_CIRCLE,				// 133 SpecialStage 丸柱
	GMD_EVENT_ID_SS_ONEWAY,				// 134 SpecialStage 丸柱（一方通行）
	GMD_EVENT_ID_SS_ONEWAY_RECT,		// 135 SpecialStage 丸柱のための矩形判定
	GMD_EVENT_ID_SS_ENDURANCE,			// 136 SpecialStage 耐久柱
	GMD_EVENT_ID_SS_GOAL,				// 137 SpecialStage ゴール
	GMD_EVENT_ID_SS_EMERALD,			// 138 SpecialStage カオスエメラルド
	GMD_EVENT_ID_SS_TIME,				// 139 SpecialStage 時間パネル
// 200
	GMD_EVENT_ID_SS_RINGGATE,			// 140 SpecialStage リングゲート
	GMD_EVENT_ID_STEAM_PIPE_GATE_R,		// 141 スチームパイプ入り口→
	GMD_EVENT_ID_STEAM_PIPE_GATE_L,		// 142 スチームパイプ入り口←
	GMD_EVENT_ID_STEAM_PIPE_A1,			// 143 スチームパイプ　パイプＡ↓
	GMD_EVENT_ID_STEAM_PIPE_A2,			// 144 スチームパイプ　パイプＡ←
	GMD_EVENT_ID_STEAM_PIPE_A3,			// 145 スチームパイプ　パイプＡ↑
	GMD_EVENT_ID_STEAM_PIPE_A4,			// 146 スチームパイプ　パイプＡ→
	GMD_EVENT_ID_STEAM_PIPE_B1,			// 147 スチームパイプ　パイプＢ↓
	GMD_EVENT_ID_STEAM_PIPE_B2,			// 148 スチームパイプ　パイプＢ←
	GMD_EVENT_ID_STEAM_PIPE_B3,			// 149 スチームパイプ　パイプＢ↑
// 210
	GMD_EVENT_ID_STEAM_PIPE_B4,			// 150 スチームパイプ　パイプＢ→
	GMD_EVENT_ID_STEAM_PIPE_J1,			// 151 スチームパイプ　ジョイント┌
	GMD_EVENT_ID_STEAM_PIPE_J2,			// 152 スチームパイプ　ジョイント┐
	GMD_EVENT_ID_STEAM_PIPE_J3,			// 153 スチームパイプ　ジョイント┘
	GMD_EVENT_ID_STEAM_PIPE_J4,			// 154 スチームパイプ　ジョイント└
	GMD_EVENT_ID_STEAM_PIPE_EXIT,		// 155 スチームパイプ　出口
	GMD_EVENT_ID_DRAIN_TANK_IN,			// 156 排液装置入口
	GMD_EVENT_ID_DRAIN_TANK_OUT,		// 157 排液装置出口
	GMD_EVENT_ID_POP_STEAM_U,			// 158 ポップスチーム↑
	GMD_EVENT_ID_POP_STEAM_R,			// 159 ポップスチーム→
// 220
	GMD_EVENT_ID_POP_STEAM_D,			// 160 ポップスチーム↓
	GMD_EVENT_ID_POP_STEAM_L,			// 161 ポップスチーム←
	GMD_EVENT_ID_GMK_TRUCK,				// 162 トロッコ
	GMD_EVENT_ID_GMK_T_GRAVITY_00_00,	// 163 トロッコ重力 平地下
	GMD_EVENT_ID_GMK_T_GRAVITY_00_40,	// 164 トロッコ重力 平地左
	GMD_EVENT_ID_GMK_T_GRAVITY_00_80,	// 165 トロッコ重力 平地上
	GMD_EVENT_ID_GMK_T_GRAVITY_00_C0,	// 166 トロッコ重力 平地右
	GMD_EVENT_ID_GMK_T_GRAVITY_30_20,	// 167 トロッコ重力 30度左下
	GMD_EVENT_ID_GMK_T_GRAVITY_30_60,	// 168 トロッコ重力 30度左上
	GMD_EVENT_ID_GMK_T_GRAVITY_30_A0,	// 169 トロッコ重力 30度右上

// 230
	GMD_EVENT_ID_GMK_T_GRAVITY_30_E0,	// 170 トロッコ重力 30度右下
	GMD_EVENT_ID_GMK_T_GRAVITY_45_20,	// 171 トロッコ重力 45度左下
	GMD_EVENT_ID_GMK_T_GRAVITY_45_60,	// 172 トロッコ重力 45度左上
	GMD_EVENT_ID_GMK_T_GRAVITY_45_A0,	// 173 トロッコ重力 45度右上
	GMD_EVENT_ID_GMK_T_GRAVITY_45_E0,	// 174 トロッコ重力 45度右下
	GMD_EVENT_ID_GMK_T_GRAVITY_60_20,	// 175 トロッコ重力 60度左下
	GMD_EVENT_ID_GMK_T_GRAVITY_60_60,	// 176 トロッコ重力 60度左上
	GMD_EVENT_ID_GMK_T_GRAVITY_60_A0,	// 177 トロッコ重力 60度右下
	GMD_EVENT_ID_GMK_T_GRAVITY_60_E0,	// 178 トロッコ重力 60度右下
	GMD_EVENT_ID_GMK_T_GRAVITY_R_20,	// 179 トロッコ重力 R左下

// 240
	GMD_EVENT_ID_GMK_T_GRAVITY_R_60,	// 180 トロッコ重力 R左上
	GMD_EVENT_ID_GMK_T_GRAVITY_R_A0,	// 181 トロッコ重力 R右上
	GMD_EVENT_ID_GMK_T_GRAVITY_R_E0,	// 182 トロッコ重力 R右下
	GMD_EVENT_ID_GMK_T_GRAVITY_RR_20,	// 183 トロッコ重力 逆R左下
	GMD_EVENT_ID_GMK_T_GRAVITY_RR_60,	// 184 トロッコ重力 逆R左上
	GMD_EVENT_ID_GMK_T_GRAVITY_RR_A0,	// 185 トロッコ重力 逆R右上
	GMD_EVENT_ID_GMK_T_GRAVITY_RR_E0,	// 186 トロッコ重力 逆R右下
	GMD_EVENT_ID_GMK_SWITCH,			// 187 スイッチ
	GMD_EVENT_ID_GMK_SW_WALL_Z3_HR,		// 188 スイッチ壁 ZONE3 横向き右出現
	GMD_EVENT_ID_GMK_SW_WALL_Z3_HL,		// 189 スイッチ壁 ZONE3 横向き左出現

// 250
	GMD_EVENT_ID_GMK_SW_WALL_Z3_VB,		// 190 スイッチ壁 ZONE3 縦向き下出現
	GMD_EVENT_ID_GMK_SW_WALL_Z3_VT,		// 191 スイッチ壁 ZONE3 縦向き上出現
	GMD_EVENT_ID_GMK_SW_WALL_Z3_HR_L8,	// 192 スイッチ壁 ZONE3 ロング 横向き右出現
	GMD_EVENT_ID_GMK_SW_WALL_Z3_HL_L8,	// 193 スイッチ壁 ZONE3 ロング 横向き左出現
	GMD_EVENT_ID_GMK_SW_WALL_Z3_VB_L8,	// 194 スイッチ壁 ZONE3 ロング 縦向き下出現
	GMD_EVENT_ID_GMK_SW_WALL_Z3_VT_L8,	// 195 スイッチ壁 ZONE3 ロング 縦向き上出現
	GMD_EVENT_ID_GMK_SW_WALL_Z4_HR,		// 196 スイッチ壁 ZONE4 横向き右出現
	GMD_EVENT_ID_GMK_SW_WALL_Z4_HL,		// 197 スイッチ壁 ZONE4 横向き左出現
	GMD_EVENT_ID_GMK_SW_WALL_Z4_VB,		// 198 スイッチ壁 ZONE4 縦向き下出現
	GMD_EVENT_ID_GMK_SW_WALL_Z4_VT,		// 199 スイッチ壁 ZONE4 縦向き上出現

// 260
	GMD_EVENT_ID_SCROLL,				// 200 スクロール
	GMD_EVENT_ID_SHUTTER_IN,			// 201 シャッター入口
	GMD_EVENT_ID_SHUTTER_OUT,			// 202 シャッター出口
	GMD_EVENT_ID_NEEDLE_NEON,			// 203 ネオン針
	GMD_EVENT_ID_GMK_T_NO_LANDING_D,	// 204 トロッコ接地不可 下
	GMD_EVENT_ID_GMK_T_NO_LANDING_L,	// 205 トロッコ接地不可 左
	GMD_EVENT_ID_GMK_T_NO_LANDING_U,	// 206 トロッコ接地不可 上
	GMD_EVENT_ID_GMK_T_NO_LANDING_R,	// 207 トロッコ接地不可 右
	GMD_EVENT_ID_GMK_T_FC_GRAVITY_D,	// 208 重力強制変換 下
	GMD_EVENT_ID_GMK_T_FC_GRAVITY_L,	// 209 重力強制変換 左

// 270
	GMD_EVENT_ID_GMK_T_FC_GRAVITY_U,	// 210 重力強制変換 上
	GMD_EVENT_ID_GMK_T_FC_GRAVITY_R,	// 211 重力強制変換 右
	GMD_EVENT_ID_BREAKWALL1_C_H,		// 212 破壊可能壁 中間のデザイン 横タイプ
	GMD_EVENT_ID_GMK_Z3LAND_PULLEY,		// 213 Zone3浮島付随滑車
	GMD_EVENT_ID_GMK_Z3LAND_ROPE_V,		// 214 Zone3浮島付随ロープ（縦）
	GMD_EVENT_ID_GMK_Z3LAND_ROPE_H,		// 215 Zone3浮島付随ロープ（横）
	GMD_EVENT_ID_END_SON_NOP,			// 216 エンディングソニック操作無効
	GMD_EVENT_ID_END_SON_BRAKE,			// 217 エンディングソニックブレーキ開始
	GMD_EVENT_ID_BOSS3_ROUTE,			// 218 ボス3経路
	GMD_EVENT_ID_BOSS3_PILLAR_MANAGER,	// 219 ボス3用柱管理

// 280
	GMD_EVENT_ID_GMK_ENDING_ANIMAL,		// 220 エンディング用動物
	GMD_EVENT_ID_GMK_BOSS5_TRIGGER,		// 221 ボスFINAL発動トリガ
	GMD_EVENT_ID_GMK_BOSS5_LAND_PLACE,	// 222 ボスFINAL用足場配置基準位置
	GMD_EVENT_ID_GMK_DATA_LOAD,			// 223 データロードギミック
	GMD_EVENT_ID_GMK_P_PILLAR_N,		// 224 迫り出す柱（下から）
	GMD_EVENT_ID_GMK_P_PILLAR_R,		// 225 迫り出す柱（上から）
	GMD_EVENT_ID_GMK_P_PILLAR_SW,		// 226 迫り出す柱（起動判定）
	GMD_EVENT_ID_GMK_DANGER_SIGN_D,		// 227 危険告知看板 下
	GMD_EVENT_ID_GMK_DANGER_SIGN_L,		// 228 危険告知看板 左
	GMD_EVENT_ID_GMK_DANGER_SIGN_U,		// 229 危険告知看板 上

// 290
	GMD_EVENT_ID_GMK_DANGER_SIGN_R,		// 230 危険告知看板 右
	GMD_EVENT_ID_GMK_SS_ARROW,			// 231 SpecialStage 矢印
	GMD_EVENT_ID_GMK_SS_OBLONG,			// 232 SpecialStage RingGate終端柱
	GMD_EVENT_ID_GMK_DECO_FRAME_MGR_WAY,	// 233 装飾フレーム管理（通路用）
	GMD_EVENT_ID_GMK_DECO_FRAME_MGR_BOSS,	// 234 装飾フレーム管理（ボス用）

	GMD_EVENT_ID_GMK_END = 299,			//	2009.11.04 さらに増やしました＠ishizaki

// 300
	// ネミッサで配置しないイベント(LocalEventBirth生成)
	GMD_EVENT_ID_NOSET_START,
	GMD_EVENT_ID_NOSET_ENEMY_START = GMD_EVENT_ID_NOSET_START,				// エネミータイプ

	GMD_EVENT_ID_NOSET_GIMMICK_START = GMD_EVENT_ID_NOSET_ENEMY_START,		// ギミックタイプ

	GMD_EVENT_ID_NOSET_ROCK_FALL = GMD_EVENT_ID_NOSET_GIMMICK_START,		// 大岩(落下)
	GMD_EVENT_ID_NOSET_CAPSULE_BODY,	// カプセル（本体）
	GMD_EVENT_ID_NOSET_SCR_LIMIT_SET,	// スクロール制限設置（プログラム呼び出し用）
	GMD_EVENT_ID_NOSET_SCR_LIMIT_REL,	// スクロール制限解除（プログラム呼び出し用）
	GMD_EVENT_ID_NOSET_SPL_RING,		// SPLリング
	GMD_EVENT_ID_NOSET_DRAIN_TANK_SPLASH,	// 排液装置（噴出す水）
	GMD_EVENT_ID_NOSET_ROCK_HOOK,		// 大岩を支える装置
	GMD_EVENT_ID_NOSET_ROCK_CHASE,		// 追跡大岩


//308?

    // ここから下はネミッサでもデバッグ配置でも配置しない系
	GMD_EVENT_ID_NOSET_NODEBUG_START,
	GMD_EVENT_ID_NOSET_NODEBUG_ENEMY_START = GMD_EVENT_ID_NOSET_NODEBUG_START,		// エネミータイプ
	GMD_EVENT_ID_T_STAR_NEEDLE = GMD_EVENT_ID_NOSET_NODEBUG_ENEMY_START,
	GMD_EVENT_ID_UNIDES_NEEDLE,
	GMD_EVENT_ID_UNIUNI_NEEDLE,
	GMD_EVENT_ID_KAMA_LEFT_HAND,
	GMD_EVENT_ID_KAMA_RIGHT_HAND,
//313
	// ボス系
	GMD_EVENT_ID_BOSS1_BODY,	//= GMD_EVENT_ID_NOSET_NODEBUG_ENEMY_START,	// ステージ1ボス 本体パーツ
	GMD_EVENT_ID_BOSS1_CHAIN,		// ステージ1ボス 鎖パーツ
	GMD_EVENT_ID_BOSS1_EGG,			// ステージ1ボス エッグマン
//316
	GMD_EVENT_ID_BOSS2_BODY,		// ステージ2ボス 本体パーツ
	GMD_EVENT_ID_BOSS2_EGG,			// ステージ2ボス エッグマン
	GMD_EVENT_ID_BOSS2_BALL,		// ステージ2ボス トゲボール
//319
	GMD_EVENT_ID_BOSS3_BODY,			// ステージ3ボス 本体パーツ
	GMD_EVENT_ID_BOSS3_EGG,				// ステージ3ボス エッグマン
//321
	GMD_EVENT_ID_BOSS4_BODY,			// ステージ4ボス 本体パーツ
	GMD_EVENT_ID_BOSS4_EGG,				// ステージ4ボス エッグマン
	GMD_EVENT_ID_BOSS4_CAP_1,			// ステージ4ボス カプセル１段階目
	GMD_EVENT_ID_BOSS4_CAP_2,			// ステージ4ボス カプセル２段階目
	GMD_EVENT_ID_BOSS4_CHIBI_1,			// ステージ4ボス ちびエッグマン
	GMD_EVENT_ID_BOSS4_CHIBI_2,			// ステージ4ボス ちびエッグマン２段階目
	GMD_EVENT_ID_BOSS4_CHIBI_2_SPD,		// ステージ4ボス ちびエッグマン２段階目(SPEED)
	GMD_EVENT_ID_BOSS4_CHIBI_2_BIG,		// ステージ4ボス ちびエッグマン２段階目(BIG)
	GMD_EVENT_ID_BOSS4_CHIBI_2_IRON,	// ステージ4ボス ちびエッグマン２段階目(IRON)
//330
	GMD_EVENT_ID_BOSS5_BODY,		// ステージファイナルボス 本体パーツ
	GMD_EVENT_ID_BOSS5_CORE,		// ステージファイナルボス 中心オブジェクトパーツ
	GMD_EVENT_ID_BOSS5_ROCKET,		// ステージファイナルボス ロケットパンチ
	GMD_EVENT_ID_BOSS5_TURRET,		// ステージファイナルボス 砲塔
	GMD_EVENT_ID_BOSS5_EGG,			// ステージファイナルボス エッグマン

//335	
	GMD_EVENT_ID_NOSET_NODEBUG_GIMMICK_START,										// ギミックタイプ

	GMD_EVENT_ID_NEEDLE_BACK = GMD_EVENT_ID_NOSET_NODEBUG_GIMMICK_START,		// ギミックトゲ(後ろ側)
	GMD_EVENT_ID_NEEDLE_STAND,		// ギミックトゲ(スタンド部)
	GMD_EVENT_ID_NEEDLE_NEON_NEEDLE,	// ギミックネオントゲ(針部)
	GMD_EVENT_ID_NEEDLE_NEON_GLAER,		// ギミックネオントゲ(グレア部)
//339	
	GMD_EVENT_ID_BOSS3_PILLAR_LEFT,		// ボス3用柱パーツ（左）
	GMD_EVENT_ID_BOSS3_PILLAR_RIGHT,	// ボス3用柱パーツ（右）
	GMD_EVENT_ID_BOSS3_PILLAR_TOP,		// ボス3用柱パーツ（上）
	GMD_EVENT_ID_BOSS3_PILLAR_BOTTOM,	// ボス3用柱パーツ（下）
	GMD_EVENT_ID_BOSS3_PILLAR_WALL,		// ボス3用壁
	GMD_EVENT_ID_BOSS5_LAND,			// ボスFINAL用足場
	GMD_EVENT_ID_BOSS5_CTPLT,			// ボスFINAL用カタパルト


	GMD_EVENT_ID_MAX

} GME_EVENT_ID;

/// 装飾ID
typedef enum tag_GME_DECORATE_ID {
	GMD_DECORATE_ID_BRI_A	= 0,		//壁
	GMD_DECORATE_ID_BRI_B,				//壁
	GMD_DECORATE_ID_FISH_R,				//壁
	GMD_DECORATE_ID_FISH_L,				//壁
	GMD_DECORATE_ID_PIL_A,				//壁
	GMD_DECORATE_ID_PIL_B,				//壁

	GMD_DECORATE_ID_WATER_SLIDER_L,		//ウォータースライダー左
	GMD_DECORATE_ID_WATER_SLIDER_L_J30,	//ウォータースライダー左
	GMD_DECORATE_ID_WATER_SLIDER_L_J45,	//ウォータースライダー左
	GMD_DECORATE_ID_WATER_SLIDER_L_J60,	//ウォータースライダー左
	GMD_DECORATE_ID_WATER_SLIDER_R,		//ウォータースライダー右
	GMD_DECORATE_ID_WATER_SLIDER_R_J30,	//ウォータースライダー右
	GMD_DECORATE_ID_WATER_SLIDER_R_J45,	//ウォータースライダー右
	GMD_DECORATE_ID_WATER_SLIDER_R_J60,	//ウォータースライダー右

	GMD_DECORATE_ID_GLARE_ORANGE,		// グレア（橙色）
	GMD_DECORATE_ID_GLARE_RED,			// グレア（赤）
	GMD_DECORATE_ID_GLARE_GREEN,		// グレア（緑）

	GMD_DECORATE_ID_SUNFLOWER,			// ひまわり
	GMD_DECORATE_ID_WALL_A,				// 壁
	GMD_DECORATE_ID_WALL_B,				// 壁

	GMD_DECORATE_ID_FALL,				// 滝

	GMD_DECORATE_ID_ZONE1_ASIHANA_A,		//アシハナ
	GMD_DECORATE_ID_ZONE1_ASIHANA_B,		//アシハナ
	GMD_DECORATE_ID_ZONE1_ASIHANA_C,		//アシハナ
	GMD_DECORATE_ID_ZONE1_HANA_A,			//花
	GMD_DECORATE_ID_ZONE1_HANA_B,			//花
	GMD_DECORATE_ID_ZONE1_WOOD_A,			//木

	GMD_DECORATE_ID_FALL_BACK,				// 滝（奥）
	
	GMD_DECORATE_ID_WALL_AA,				// 壁
	GMD_DECORATE_ID_WALL_AB,				// 壁
	GMD_DECORATE_ID_WALL_AC,				// 壁
	GMD_DECORATE_ID_WALL_AD,				// 壁
	GMD_DECORATE_ID_WALL_BA,				// 壁
	
	GMD_DECORATE_ID_WALL_A_BACK,			// 壁（奥）
	GMD_DECORATE_ID_WALL_B_BACK,			// 壁（奥）
	GMD_DECORATE_ID_WALL_AA_BACK,			// 壁（奥）
	GMD_DECORATE_ID_WALL_AB_BACK,			// 壁（奥）
	GMD_DECORATE_ID_WALL_AC_BACK,			// 壁（奥）
	GMD_DECORATE_ID_WALL_AD_BACK,			// 壁（奥）
	GMD_DECORATE_ID_WALL_BA_BACK,			// 壁（奥）
	
	GMD_DECORATE_ID_FALL_LEFT,				// 滝
	GMD_DECORATE_ID_FALL_RIGHT,				// 滝
	GMD_DECORATE_ID_FALL_ONE,				// 滝
	GMD_DECORATE_ID_FALL_A,					// 滝
	GMD_DECORATE_ID_FALL_LEFT_A,			// 滝
	GMD_DECORATE_ID_FALL_RIGHT_A,			// 滝
	GMD_DECORATE_ID_FALL_ONE_A,				// 滝
	
	GMD_DECORATE_ID_FALL_LEFT_BACK,			// 滝（奥）
	GMD_DECORATE_ID_FALL_RIGHT_BACK,		// 滝（奥）
	GMD_DECORATE_ID_FALL_ONE_BACK,			// 滝（奥）
	GMD_DECORATE_ID_FALL_A_BACK,			// 滝（奥）
	GMD_DECORATE_ID_FALL_LEFT_A_BACK,		// 滝（奥）
	GMD_DECORATE_ID_FALL_RIGHT_A_BACK,		// 滝（奥）
	GMD_DECORATE_ID_FALL_ONE_A_BACK,		// 滝（奥）

	GMD_DECORATE_ID_GLARE_SALMON,			// グレア（ピンク）

	GMD_DECORATE_ID_HAI_FR_L,				// 排液装置（左）
	GMD_DECORATE_ID_HAI_FR_EXIT_L,			// 排液装置出口（左）
	GMD_DECORATE_ID_HAI_FR_R,				// 排液装置（右）
	GMD_DECORATE_ID_HAI_FR_EXIT_R,			// 排液装置出口（右）

	GMD_DECORATE_ID_EAR_RUB_A_A,			//ZONE3岩
	GMD_DECORATE_ID_EAR_RUB_A_B,			//ZONE3岩
	GMD_DECORATE_ID_EAR_RUB_B_A,			//ZONE3岩
	GMD_DECORATE_ID_EAR_RUB_B_B,			//ZONE3岩
	GMD_DECORATE_ID_EAR_RUB_B_C,			//ZONE3岩
	GMD_DECORATE_ID_WAT_RUB_A,				//ZONE3岩
	GMD_DECORATE_ID_WAT_RUB_B,				//ZONE3岩

	GMD_DECORATE_ID_EFFECT_WATERSLIDER_UNDER,	//ZONE3 ウォータースライダー用エフェクト水面下
	GMD_DECORATE_ID_EFFECT_WATERSLIDER_SPRAY,	//ZONE3 ウォータースライダー用エフェクト水飛沫
	GMD_DECORATE_ID_EFFECT_CANDDLE,			//ZONE3 蝋燭
	GMD_DECORATE_ID_EFFECT_FACE,			//ZONE3 壁の顔

	GMD_DECORATE_ID_FOUN_A,					//ZONE3噴出す水
	GMD_DECORATE_ID_FOUN_B,					//ZONE3噴出す水
	GMD_DECORATE_ID_FOUN_A_BACK,			//ZONE3噴出す水
	GMD_DECORATE_ID_FOUN_B_BACK,			//ZONE3噴出す水

	GMD_DECORATE_ID_PLANT_A,				//ZONE3植物
	GMD_DECORATE_ID_PLANT_B,				//ZONE3植物
	GMD_DECORATE_ID_PLANT_C,				//ZONE3植物
	GMD_DECORATE_ID_PLANT_D,				//ZONE3植物
	GMD_DECORATE_ID_PLANT_A_FRONT,			//ZONE3植物
	GMD_DECORATE_ID_PLANT_C_FRONT,			//ZONE3植物

	GMD_DECORATE_ID_EAR_RUB_B_A_FRONT,	//ZONE3岩（前）
	GMD_DECORATE_ID_EAR_RUB_B_B_FRONT,	//ZONE3岩（前）
	GMD_DECORATE_ID_EAR_RUB_B_C_FRONT,	//ZONE3岩（前）
	GMD_DECORATE_ID_WAT_RUB_A_FRONT,	//ZONE3岩（前）
	GMD_DECORATE_ID_WAT_RUB_B_FRONT,	//ZONE3岩（前）
	GMD_DECORATE_ID_PLANT_B_FRONT,		//ZONE3植物（前）
	GMD_DECORATE_ID_PLANT_D_FRONT,		//ZONE3植物（前）
	GMD_DECORATE_ID_FISH_R_FRONT,		//壁（前）
	GMD_DECORATE_ID_FISH_L_FRONT,		//壁（前）

	GMD_DECORATE_ID_RAIL_EDGE_A,		//ZONE3レール角
	GMD_DECORATE_ID_RAIL_EDGE_B,		//ZONE3レール角
	GMD_DECORATE_ID_RAIL_EDGE_A_FLIP,	//ZONE3レール角（左右反転）
	GMD_DECORATE_ID_RAIL_EDGE_B_FLIP,	//ZONE3レール角（左右反転）

	GMD_DECORATE_ID_WHE_HOLD_A,			//ZONE4 歯車用
	GMD_DECORATE_ID_WHE_HOLD_B,			//ZONE4 歯車用
	GMD_DECORATE_ID_WHE_HOLD_C_L,		//ZONE4 歯車用
	GMD_DECORATE_ID_WHE_HOLD_D_T,		//ZONE4 歯車用
	GMD_DECORATE_ID_WHE_HOLD_C_R,		//ZONE4 歯車用
	GMD_DECORATE_ID_WHE_HOLD_D_B,		//ZONE4 歯車用

	GMD_DECORATE_ID_EFFECT_TAKI,			//ZONE1滝上部用エフェクト
	GMD_DECORATE_ID_EFFECT_CROSS,			//ZONE3蝋燭

	GMD_DECORATE_ID_FISH_B_R,				//ZONE3 ウォータースライダー排水溝
	GMD_DECORATE_ID_FISH_B_L,				//ZONE3 ウォータースライダー排水溝（左右反転）
	GMD_DECORATE_ID_RAIL_J_A_ENDING,		//ZONE3レールジョイント
	GMD_DECORATE_ID_RAIL_J_B_ENDING,		//ZONE3レールジョイント
	GMD_DECORATE_ID_RAIL_J_C_ENDING,		//ZONE3レールジョイント
	GMD_DECORATE_ID_RAIL_J_A_ENDING_FLIP,	//ZONE3レールジョイント（左右反転）
	GMD_DECORATE_ID_RAIL_J_B_ENDING_FLIP,	//ZONE3レールジョイント（左右反転）　
	GMD_DECORATE_ID_RAIL_J_C_ENDING_FLIP,	//ZONE3レールジョイント（左右反転）
	GMD_DECORATE_ID_GLARE_ORANGE_ENDING,	//グレア（エンディング）
	GMD_DECORATE_ID_GLARE_GREEN_ENDING,		//グレア（エンディング）
	GMD_DECORATE_ID_SUNFLOWER_ENDING,		//ひまわり（エンディング）
	GMD_DECORATE_ID_ZONE1_HANA_A_ENDING,	//花（エンディング）
	GMD_DECORATE_ID_ZONE1_HANA_B_ENDING,	//花（エンディング）
	GMD_DECORATE_ID_ZONE1_WOOD_A_ENDING,	//木（エンディング）　

	GMD_DECORATE_ID_WHEEL_PIL_A,			//ZONE4歯車用柱
	GMD_DECORATE_ID_WHEEL_PIL_B,			//ZONE4歯車用柱
	GMD_DECORATE_ID_WHEEL_PIL_C,			//ZONE4歯車用柱
	GMD_DECORATE_ID_WHEEL_PIL_A_FLIP,		//ZONE4歯車用柱（上下反転）
	GMD_DECORATE_ID_WHEEL_PIL_B_FLIP,		//ZONE4歯車用柱（上下反転）
	GMD_DECORATE_ID_WHEEL_PIL_C_FLIP,		//ZONE4歯車用柱（上下反転）
	GMD_DECORATE_ID_WHEEL_PIL_D,			//ZONE4歯車用柱
	GMD_DECORATE_ID_WHEEL_PIL_E,			//ZONE4歯車用柱
	GMD_DECORATE_ID_WHEEL_PIL_F,			//ZONE4歯車用柱
	GMD_DECORATE_ID_WHEEL_PIL_D_FLIP,		//ZONE4歯車用柱（左右反転）
	GMD_DECORATE_ID_WHEEL_PIL_E_FLIP,		//ZONE4歯車用柱（左右反転）
	GMD_DECORATE_ID_WHEEL_PIL_F_FLIP,		//ZONE4歯車用柱（左右反転）

	GMD_DECORATE_ID_WHE_HOLD_A_FRONT,		//ZONE4 歯車用（前）
	GMD_DECORATE_ID_WHE_HOLD_B_FRONT,		//ZONE4 歯車用（前）
	GMD_DECORATE_ID_WHE_HOLD_C_L_FRONT,		//ZONE4 歯車用（前）
	GMD_DECORATE_ID_WHE_HOLD_D_T_FRONT,		//ZONE4 歯車用（前）
	GMD_DECORATE_ID_WHE_HOLD_C_R_FRONT,		//ZONE4 歯車用（前）
	GMD_DECORATE_ID_WHE_HOLD_D_B_FRONT,		//ZONE4 歯車用（前）
	
	GMD_DECORATE_ID_BRACE_A,				//ZONE4 柱
	GMD_DECORATE_ID_BRACE_B,				//ZONE4 柱
	GMD_DECORATE_ID_BRACE_C,				//ZONE4 柱
	GMD_DECORATE_ID_BRACE_D,				//ZONE4 柱
	GMD_DECORATE_ID_BRACE_E,				//ZONE4 柱
	GMD_DECORATE_ID_BRACE_F,				//ZONE4 柱
	GMD_DECORATE_ID_BRACE_G,				//ZONE4 柱

	GMD_DECORATE_ID_PIL_COR,				//ZONE4 柱

	GMD_DECORATE_ID_WARNING,				//ZONE4 パトランプ
	GMD_DECORATE_ID_EFFECT_WARNING,			//ZONE4 パトランプ

	GMD_DECORATE_ID_BRACE_S_A,					//ZONE4 柱
	GMD_DECORATE_ID_BRACE_S_B,					//ZONE4 柱
	GMD_DECORATE_ID_BRACE_S_C,					//ZONE4 柱
	GMD_DECORATE_ID_BRACE_S_D,					//ZONE4 柱
	GMD_DECORATE_ID_BRACE_S_E,					//ZONE4 柱
	GMD_DECORATE_ID_BRACE_S_F,					//ZONE4 柱
	GMD_DECORATE_ID_BRACE_S_G,					//ZONE4 柱	
	GMD_DECORATE_ID_P_STESM_CO01,			//ZONE4 ポップスチーム角
	GMD_DECORATE_ID_P_STESM_CO02,			//ZONE4 ポップスチーム角
	GMD_DECORATE_ID_P_STESM_CO03,			//ZONE4 ポップスチーム角
	GMD_DECORATE_ID_P_STESM_CO04,			//ZONE4 ポップスチーム角
	GMD_DECORATE_ID_P_STESM_CO_L_01,		//ZONE4 ポップスチーム角
	GMD_DECORATE_ID_P_STESM_CO_L_02,		//ZONE4 ポップスチーム角
	GMD_DECORATE_ID_P_STESM_CO_R_01,		//ZONE4 ポップスチーム角
	GMD_DECORATE_ID_P_STESM_CO_R_02,		//ZONE4 ポップスチーム角
	GMD_DECORATE_ID_P_STESM_CO_T_01,		//ZONE4 ポップスチーム角
	GMD_DECORATE_ID_P_STESM_CO_T_02,		//ZONE4 ポップスチーム角
	GMD_DECORATE_ID_P_STESM_CO_U_01,		//ZONE4 ポップスチーム角
	GMD_DECORATE_ID_P_STESM_CO_U_02,		//ZONE4 ポップスチーム角
	GMD_DECORATE_ID_P_STESM_TUBE01,			//ZONE4 ポップスチームチューブ
	GMD_DECORATE_ID_P_STESM_TUBE02,			//ZONE4 ポップスチームチューブ
	GMD_DECORATE_ID_P_STESM_TUBE03,			//ZONE4 ポップスチームチューブ
	GMD_DECORATE_ID_P_STESM_TUBE04,			//ZONE4 ポップスチームチューブ
	GMD_DECORATE_ID_P_STESM_TUBE03_FLIP,	//ZONE4 ポップスチームチューブ
	GMD_DECORATE_ID_P_STESM_TUBE04_FLIP,	//ZONE4 ポップスチームチューブ

	GMD_DECORATE_ID_UKI_RAIL,				//ZONE4 浮島用レール
	GMD_DECORATE_ID_UKI_RAIL_FLIP,			//ZONE4 浮島用レール

	GMD_DECORATE_ID_SHUTTER_3_MOVE_CLOSE,	//ZONEF シャッター
	GMD_DECORATE_ID_SHUTTER_3_OPEN_MOVE,	//ZONEF シャッター

	GMD_DECORATE_ID_SHUTTER_3_MOVE_MOVE,	//ZONEF シャッター
	GMD_DECORATE_ID_SHUTTER_3_CLOSE_CLOSE,	//ZONEF シャッター
	GMD_DECORATE_ID_SHUTTER_3_OPEN_OPEN,	//ZONEF シャッター
	GMD_DECORATE_ID_SHUTTER_3_OPEN_CLOSE,	//ZONEF シャッター

	GMD_DECORATE_ID_SHUTTER_5_MOVE_MOVE,	//ZONEF シャッター
	GMD_DECORATE_ID_SHUTTER_5_CLOSE_CLOSE,	//ZONEF シャッター
	GMD_DECORATE_ID_SHUTTER_5_OPEN_OPEN,	//ZONEF シャッター
	GMD_DECORATE_ID_SHUTTER_5_OPEN_CLOSE,	//ZONEF シャッター

	GMD_DECORATE_ID_PLANTA,				//ZONE2 植物
	GMD_DECORATE_ID_PLANTB,				//ZONE2 植物

	GMD_DECORATE_ID_EFFECT_BOSSF_LIGHT,	//ZONEF ファイナルボス用ライト

	GMD_DECORATE_ID_SHUTTER_LOOP,		//ZONEF シャッター

	GMD_DECORATE_ID_MAX

} GME_DECORATE_ID;


//----- Macros --------------------------------------------------------------
// ==========================================================================
// GMM_EVENT_ENE_VIEW_OUT_OFST
/*!
 *	敵クリッピング範囲オフセットの取得
 *
 *	@param	eve_id	[in]	イベントID GME_EVENT_ID
 *
 *	@return
 *		クリッピング範囲オフセット(view_out_ofst)
 */
// ==========================================================================
#define GMM_EVENT_ENE_VIEW_OUT_OFST(eve_id)		((s16)(g_gm_event_size_tbl[(eve_id)] + \
					GMD_MAIN_SCR_SPD_MAX/*最大スクロールサイズ*/ + \
					GMD_EVE_BIRTH_WIDTH/*生成可能範囲*/ + GMD_MAIN_SCR_SPD_MAX/* カメラゆれ許容範囲 */ + \
					128/*leftによる領域越え対応*/))

// ==========================================================================
// GMM_EVENT_DECO_VIEW_OUT_OFST
/*!
 *	装飾クリッピング範囲オフセットの取得
 *
 *	@param	eve_id	[in]	イベントID GME_EVENT_ID
 *
 *	@return
 *		クリッピング範囲オフセット(view_out_ofst)
 */
// ==========================================================================
#define GMM_EVENT_DECO_VIEW_OUT_OFST(eve_id)	((s16)(g_gm_decorate_size_tbl[(eve_id)] + \
					GMD_MAIN_SCR_SPD_MAX/*最大スクロールサイズ*/ + \
					GMD_EVE_BIRTH_WIDTH/*生成可能範囲*/ + GMD_MAIN_SCR_SPD_MAX/* カメラゆれ許容範囲 */))

//----- Macros Functions ----------------------------------------------------

//----- External Variables --------------------------------------------------
/// イベント生成関数テーブル
extern OBS_OBJECT_WORK* (*const g_gm_event_tbl[GMD_EVENT_ID_MAX])(GMS_EVE_RECORD_EVENT*, fx32, fx32, u8);

/// 装飾生成関数テーブル
extern OBS_OBJECT_WORK* (*const g_gm_decorate_tbl[GMD_DECORATE_ID_MAX])(GMS_EVE_RECORD_DECORATE*, fx32, fx32, u8);

/// イベントサイズテーブル
extern u16 g_gm_event_size_tbl[GMD_EVENT_ID_MAX];

/// 装飾物サイズテーブル
extern u16  g_gm_decorate_size_tbl[GMD_DECORATE_ID_MAX];

//----- External Declarations -----------------------------------------------

#if	defined(__cplusplus)
} /* extern "C" */
#endif

#endif // GM_EVENT_TBL_H_

//----- Include Files -------------------------------------------------------
