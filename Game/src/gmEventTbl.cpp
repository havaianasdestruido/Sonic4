// ==========================================================================
/*!
  @file gmEventTbl.c
  @brief 

  @author Ishizaki
				Copyright(c) 2009 Dimps

  $Id: gmEventTbl.cpp 22 2011-04-25 02:28:55Z thamada $
  $Date: 2008-11-10 15:04:41 +0900#$
 */
// ==========================================================================
/*
 * $Log: gmEventTbl.c,v $
 *
 */

//----- Include Files -------------------------------------------------------
#include "pch.h"
#include "gmEventTbl.h"
#include "gmMain.h"

// イベント関連
#include "gmGmkFlagChange.h"
#include "gmGmkSpring.h"
#include "gmGmkRock.h"
#include "gmGmkRockRide.h"
#include "gmGmkStart.h"
#include "gmGmkLand.h"
#include "gmGmkPulley.h"
#include "gmGmkNeedle.h"
#include "gmGmkWaterArea.h"
#include "gmGmkDashPanel.h"
#include "gmGmkGoalPanel.h"
#include "gmGmkBubble.h"
#include "gmGmkWaterSlider.h"
#include "gmGmkBreakLand.h"
#include "gmGmkBreakObj.h"
#include "gmGmkBreakWall.h"
#include "gmGmkTarzanRope.h"
#include "gmGmkPointMarker.h"
#include "gmGmkCamScrLim.h"
#include "gmGmkPiston.h"
#include "gmGmkBeltConveyor.h"
#include "gmGmkSpipe.h"
#include "gmGmkStopper.h"
#include "gmGmkUpBumper.h"
#include "gmGmkBumper.h"
#include "gmGmkItem.h"
#include "gmGmkSpear.h"
#include "gmGmkScrew.h"
#include "gmGmkCannon.h"
#include "gmGmkCapsule.h"
#include "gmGmkEnBmpr.h"
#include "gmGmkBobbin.h"
#include "gmGmkFlipper.h"
#include "gmGmkSlot.h"
#include "gmGmkSeesaw.h"
#include "gmGmkBridge.h"
#include "gmGmkSplRing.h"
#include "gmGmkSpCtplt.h"
#include "gmGmkForceSpin.h"
#include "gmGmkGear.h"
#include "gmGmkPressWall.h"
#include "gmGmkSsSquare.h"
#include "gmGmkSsCircle.h"
#include "gmGmkSsEndurance.h"
#include "gmGmkSsGoal.h"
#include "gmGmkSsEmerald.h"
#include "gmGmkSsTime.h"
#include "gmGmkSsRingGate.h"
#include "gmGmkSsArrow.h"
#include "gmGmkSsOblong.h"
#include "gmGmkSteamPipe.h"
#include "gmGmkDrainTank.h"
#include "gmGmkPopSteam.h"
#include "gmGmkTruck.h"
#include "gmGmkSwitch.h"
#include "gmGmkSwWall.h"
#include "gmGmkLoop.h"
#include "gmGmkShutter.h"
#include "gmGmkNeedleNeon.h"
#include "gmGmkBoss3Route.h"
#include "gmGmkBoss3Pillar.h"
#include "gmGmkAnimal.h"
#include "gmGmkBoss5Trigger.h"
#include "gmGmkBoss5LandPlace.h"
#include "gmGmkPressPillar.h"
#include "gmGmkDSign.h"
#include "gmGmkDecoFrameMgr.h"

#include "gmDeco.h"
#include "gmBoss1.h"
#include "gmBoss5.h"
#include "gmBoss5Rocket.h"
#include "gmBoss5Turret.h"
#include "gmBoss5Egg.h"
#include "gmBoss5Land.h"
#include "gmBoss5Ctplt.h"

#include "gmBoss4.h"
#include "gmBoss4Body.h"
#include "gmBoss4Eggman.h"
#include "gmBoss4Capsule.h"
#include "gmBoss4Chibi.h"

#include "gmBoss2.h"
#include "gmBoss3.h"


#include "gmEneMotora.h"
#include "gmEneHari.h"
#include "gmEneGabu.h"
#include "gmEneSting.h"
#include "gmEneMereon.h"
#include "gmEneMogurin.h"
#include "gmEneGardon.h"
#include "gmEneTeruStar.h"
#include "gmEneKaniPunch.h"
#include "gmEneHarogen.h"
#include "gmEneUnides.h"
#include "gmEneUniuni.h"
#include "gmEneBukubuku.h"
#include "gmEneKama.h"

// データヘッダ

//----- Definitions ---------------------------------------------------------

//----- Macros --------------------------------------------------------------

//----- Macros Functions ----------------------------------------------------

//----- External Declarations -----------------------------------------------

//----- Static Declarations -------------------------------------------------

//----- Global Variables ----------------------------------------------------
/// イベント生成関数テーブル
OBS_OBJECT_WORK* (*const g_gm_event_tbl[GMD_EVENT_ID_MAX])(GMS_EVE_RECORD_EVENT *eve_rec, fx32 x, fx32 y, u8 type) = {
	// 敵
//  00
	GmEneHariSenboInit,		// E000 ハリセンボ
	GmEneMotoraInit,		// E001 モトラ
	GmEneStingInit,			// E002 スティンガー
	GmEneGabuInit,			// E003 ガブッチョ
	GmEneMereonInit,		// E004 メレオン
	GmEneMereonInit,		// E005 メレオン 落下タイプ
	GmEneMoguInit,			// E006 モグリン
	GmEneGardonInit,		// E007 ガードン
	GmEneTStarInit,			// E008 テルスター
	GmEneKaniInit,			// E009 かにパンチ

//  10
	GmEneHaroInit,			// E010 ハロゲン
	GmEneUnidesInit,		// E011 ウニデス
	GmEneUniuniInit,		// E012 ウニウニ
	GmEneBukuInit,			// E013 ブクブク
	GmEneKamaInit,			// E014 カマキラー
	GmEneHariSenboInit,		// E015 ハリセンボ 赤
	NULL,		// E016
	NULL,		// E017
	NULL,		// E018
	NULL,		// E019

//  20
	NULL,		// E020
	NULL,		// E021
	NULL,		// E022
	NULL,		// E023
	NULL,		// E024
	NULL,		// E025
	NULL,		// E026
	NULL,		// E027
	NULL,		// E028
	NULL,		// E029

//  30
	NULL,		// E030
	NULL,		// E031
	NULL,		// E032
	NULL,		// E033
	NULL,		// E034
	NULL,		// E035
	NULL,		// E036
	NULL,		// E037
	NULL,		// E038
	NULL,		// E039

//  40
	NULL,		// E040
	NULL,		// E041
	NULL,		// E042
	NULL,		// E043
	NULL,		// E044
	NULL,		// E045
	NULL,		// E046
	NULL,		// E047
	NULL,		// E048
	NULL,		// E049

//  50
	NULL,		// E050
	NULL,		// E051
	NULL,		// E052
	NULL,		// E053
	NULL,		// E054
#if !defined(SONIC4_TRIAL)
	GmBoss5Init,	// E055 ボスFinal
	GmBoss4Init,	// E056 ボス4
	GmBoss3Init,	// E057 ボス3
	GmBoss2Init,	// E058 ボス2
	GmBoss1Init,	// E059 ボス1
#else
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
#endif // !defined(SONIC4_TRIAL)
	
	// ギミック
//  60
	GmGmkFlagChangeInit,	// G000 接地
	GmGmkFlagChangeInit,	// G001 A面切り替え
	GmGmkFlagChangeInit,	// G002 B面切り替え
	GmGmkItemInit,			// G003 アイテム ハイスピード
	GmGmkItemInit,			// G004 アイテム 無敵
	GmGmkItemInit,			// G005 アイテム リング10
	GmGmkItemInit,			// G006 アイテム バリア
	GmGmkItemInit,			// G007 アイテム 1UP
	NULL,		// G008 アイテム 予備
	NULL,		// G009 アイテム 予備
//  70
	GmGmkSpringInit,		// G010 スプリング上
	GmGmkSpringInit,		// G011 スプリング右上
	GmGmkSpringInit,		// G012 スプリング右
	GmGmkSpringInit,		// G013 スプリング右下
	GmGmkSpringInit,		// G014 スプリング下
	GmGmkSpringInit,		// G015 スプリング左下
	GmGmkSpringInit,		// G016 スプリング左
	GmGmkSpringInit,		// G017 スプリング左上
	GmGmkSpringInit,		// G018 スプリング右上(地面埋まり)
	GmGmkSpringInit,		// G019 スプリング左上(地面埋まり)
//  80
	GmGmkStartInit,			// G020 スタート位置設定
	GmGmkLandInit,			// G021 浮島(通常)
	GmGmkLandInit,			// G022 浮島(大)
	GmGmkLandInit,			// G023 浮島(当たり)
	GmGmkRockChaseManagerInit,	// G024 大岩(追跡管理)
	GmGmkRockFallManagerInit,	// G025 大岩(落下管理)
	GmGmkPulleyBaseInit,	// G026 滑車本体
	GmGmkPulleyPoleLInit,	// G027 滑車ポール（左）
	GmGmkPulleyPoleRInit,	// G028 滑車ポール（右）
	GmGmkPulleyRopeFInit,	// G029 滑車ロープ（水平）
//  90
	GmGmkPulleyRopeTInit,	// G030 滑車ロープ（斜め：右）
	GmGmkNeedleInit,		// G031 トゲ(上向き)
	GmGmkNeedleInit,		// G032 トゲ(左向き)
	GmGmkNeedleInit,		// G033 トゲ(下向き)
	GmGmkNeedleInit,		// G034 トゲ(右向き)
	GmGmkPulleyRopeTInit,	// G035 滑車ロープ（斜め：左）
	GmGmkRockRideInit,		// G036 大岩(傾斜)
	GmGmkActNeedleInit,		// G037 出入りトゲ(上向き)
	GmGmkActNeedleInit,		// G038 出入りトゲ(下向き)
	GmGmkFlagChangeInit,	// G039 落下ミス判定
// 100
	GmGmkSpringInit,		// G040 スプリング右上(地面埋まり) A面用
	GmGmkSpringInit,		// G041 スプリング左上(地面埋まり) A面用
	GmGmkWaterAreaInit,		// G042 水位変更（左から）
	GmGmkWaterAreaInit,		// G043 水位変更（右から）
	GmGmkWaterAreaInit,		// G044 水位変更（上から）
	GmGmkWaterAreaInit,		// G045 水位変更（下から
	GmGmkWaterAreaInit,		// G046 水位変更（開始時、再開時）
	GmGmkDashPanelInit,		// G047 ダッシュパネル右
	GmGmkDashPanelInit,		// G048 ダッシュパネル左
	GmGmkDashPanelInit,		// G049 ダッシュパネル縦右壁
// 110
	GmGmkDashPanelInit,		// G050 ダッシュパネル縦左壁
	GmGmkBubbleManagerInit,	// G051	息継ぎの泡
	GmGmkTarzanRopeInit,	// G052 ターザンロープ
	GmGmkTarzanRopeInit,	// G053 ターザンロープ
	GmGmkTarzanRopeInit,	// G054 ターザンロープ
	GmGmkWaterSliderInit,	// G055 ウォータースライダー左（真下）
	GmGmkWaterSliderInit,	// G056 ウォータースライダー左（30度）
	GmGmkWaterSliderInit,	// G057 ウォータースライダー左（45度）
	GmGmkWaterSliderInit,	// G058 ウォータースライダー左（60度）
	GmGmkWaterSliderInit,	// G059 ウォータースライダー右（真下）
// 120
	GmGmkWaterSliderInit,	// G060 ウォータースライダー右（30度）
	GmGmkWaterSliderInit,	// G061 ウォータースライダー右（45度）
	GmGmkWaterSliderInit,	// G062 ウォータースライダー右（60度）
	GmGmkGoalPanelInit,		// G063 ゴールパネル
	GmGmkBreakLandRInit,	// G064 崩落足場　右張り出し
	GmGmkBreakLandLInit,	// G065 崩落足場　左張り出し
	GmGmkPistonUpInit,		// G066 上向ピストン
	GmGmkPistonDownInit,	// G067 下向ピストン
	GmGmkLandInit,			// G068 浮島(回転移動)
	GmGmkPointMarkerInit,	// G069 ポイントマーカー
// 130
	GmGmkBreakObjInit,		// G070 破壊可能オブジェ
	GmGmkFlagChangeInit,	// G071 カメラセンター変更
	GmGmkCamScrLimitInit,	// G072 スクロール制限
	GmGmkBeltConveyorInit,	// G073 ベルトコンベヤー
	GmGmkBreakWall_L1Init,	// G074 破壊可能壁
	GmGmkBreakWall_L2Init,	// G075 破壊可能壁
	GmGmkBreakWall_R1Init,	// G076 破壊可能壁
	GmGmkBreakWall_R2Init,	// G077 破壊可能壁
	GmGmkBreakWall_C1Init,	// G078 破壊可能壁
	GmGmkSpipeInit,			// G079	Ｓ字パイプ
// 140
	GmGmkBreakWall_C2Init,	// G080 破壊可能壁
	GmGmkStopperNormInit,	// G081 ストッパー　普通
	GmGmkStopperSlotInit,	// G082 ストッパー　スロットスターター
	GmGmkBreakFloorInit,	// G083 破壊可能壁横型(床天井)
	GmGmkUpBumperLInit,		// G084 登るバンパー左くっつき
	GmGmkUpBumperRInit,		// G085 登るバンパー右くっつき
	GmGmkBumperInit,		// G086	バンパー
	GmGmkBumperInit,		// G087	バンパー
	GmGmkBumperInit,		// G088	バンパー
	GmGmkBumperInit,		// G089	バンパー
// 150
	GmGmkBumperInit,		// G090	バンパー
	GmGmkBumperInit,		// G091	バンパー
	GmGmkBumperInit,		// G092	バンパー
	GmGmkBumperInit,		// G093	バンパー
	GmGmkBumperInit,		// G094	バンパー
	GmGmkBumperInit,		// G095	バンパー
	GmGmkSpearUInit,		// G096 槍(上)
	GmGmkSpearDInit,		// G097 槍(下)
	GmGmkSpearLInit,		// G098 槍(左)
	GmGmkSpearRInit,		// G099 槍(右)
// 160
	GmGmkScrewInit,			// G100 コークスクリュー
	GmGmkCannonInit,		// G101 大砲
	GmGmkFlagChangeInit,	// G102 ループカメラ
	GmGmkCapsuleInit,		// G103 カプセル
	GmGmkEnBmprInit,		// G104	３耐バンパー（0度）
	GmGmkEnBmprInit,		// G105	３耐バンパー（45度）
	GmGmkEnBmprInit,		// G106	３耐バンパー（90度）
	GmGmkEnBmprInit,		// G107	３耐バンパー（135度）
	GmGmkBobbinInit,		// G108	ボビン
	GmGmkFlipperInit,		// G109 フリッパー（上にはじく：左側）
// 170
	GmGmkFlipperInit,		// G110 フリッパー（上にはじく：右側）
	GmGmkFlipperInit,		// G111 フリッパー（左右にはじく）
	GmGmkSlotInit,			// G112 スロット (G082と連動)
	GmGmkSeesaw0Init,		// G113 シーソー
	GmGmkSeesaw30Init,		// G114 シーソー
	GmGmkSeesaw330Init,		// G115 シーソー
	GmGmkBridgeInit,		// G116 丸太橋
	GmGmkCamScrLimitReleaseInit,// G117 スクロール制限解除
	GmGmkSpCtplt0Init,		// G118 スプリングカタパルト０°
	GmGmkSpCtplt45Init,		// G119 スプリングカタパルト４５°
// 180
	GmGmkSpCtplt315Init,	// G120 スプリングカタパルト３１５°
	GmGmkGearInit,			// G121 歯車
	GmGmkGearInit,			// G122 移動歯車
	GmGmkGearMoveEndInit,	// G123 移動歯車終点
	GmGmkGearInit,			// G124 歯車スイッチ
	GmGmkForceSpinSetInit,	// G125 強制スピンセット
	GmGmkForceSpinResetInit,// G126 強制スピン解除
	NULL,	// G127 未使用（旧：強制スピン
	NULL,	// G128 未使用（旧：強制スピン
	GmGmkPressWallInit,		// G129 迫る壁
// 190
	GmGmkPressWallStopInit,	// G130 迫る壁止める
	GmGmkPressWallControlerInit,// G131 迫る壁速度変化
	GmGmkSsSquareInit,		// G132 SpecialStage 四角柱
	GmGmkSsCircleInit,		// G133 SpecialStage 丸柱
	GmGmkSsCircleInit,		// G134 SpecialStage 丸柱（一方通行）
	GmGmkFlagChangeInit,	// G135 SpecialStage 丸柱のための矩形判定
	GmGmkSsEnduranceInit,	// G136 SpecialStage 耐久柱
	GmGmkSsGoalInit,		// G137 SpecialStage ゴール
	GmGmkSsEmeraldInit,		// G138 SpecialStage カオスエメラルド
	GmGmkSsTimeInit,		// G139 SpecialStage 時間パネル
// 200
	GmGmkSsRingGateInit,	// G140 SpecialStage リングゲート
	GmGmkSteamPipeGateRInit,	// G141 スチームパイプ入り口→
	GmGmkSteamPipeGateLInit,	// G142 スチームパイプ入り口←
	GmGmkSteamPipeA1Init,	// G143 スチームパイプ　パイプＡ↓
	GmGmkSteamPipeA2Init,	// G144 スチームパイプ　パイプＡ←
	GmGmkSteamPipeA3Init,	// G145 スチームパイプ　パイプＡ↑
	GmGmkSteamPipeA4Init,	// G146 スチームパイプ　パイプＡ→
	GmGmkSteamPipeB1Init,	// G147 スチームパイプ　パイプＢ↓
	GmGmkSteamPipeB2Init,	// G148 スチームパイプ　パイプＢ←
	GmGmkSteamPipeB3Init,	// G149 スチームパイプ　パイプＢ↑
// 210
	GmGmkSteamPipeB4Init,	// G150 スチームパイプ　パイプＢ→
	GmGmkSteamPipeJ1Init,	// G151 スチームパイプ　ジョイント┌
	GmGmkSteamPipeJ2Init,	// G152 スチームパイプ　ジョイント┐
	GmGmkSteamPipeJ3Init,	// G153 スチームパイプ　ジョイント┘
	GmGmkSteamPipeJ4Init,	// G154 スチームパイプ　ジョイント└
	GmGmkSteamPipeGateEInit,	// G155 スチームパイプ　出口
	GmGmkDrainTankInitIn,	// G156 排液装置入口
	GmGmkDrainTankInitOut,	// G157 排液装置出口
	GmGmkPopSteamUInit,		// G158 ポップスチーム
	GmGmkPopSteamRInit,		// G159 ポップスチーム
// 220
	GmGmkPopSteamDInit,		// G160 ポップスチーム
	GmGmkPopSteamLInit,		// G161 ポップスチーム
	GmGmkTruckInit,			// G162 トロッコ
	GmGmkTruckGravityInit,	// G163 トロッコ重力 平地下
	GmGmkTruckGravityInit,	// G164 トロッコ重力 平地左
	GmGmkTruckGravityInit,	// G165 トロッコ重力 平地上
	GmGmkTruckGravityInit,	// G166 トロッコ重力 平地右
	GmGmkTruckGravityInit,	// G167 トロッコ重力 30度左下
	GmGmkTruckGravityInit,	// G168 トロッコ重力 30度左上
	GmGmkTruckGravityInit,	// G169 トロッコ重力 30度右上
// 230
	GmGmkTruckGravityInit,	// G170 トロッコ重力 30度右下
	GmGmkTruckGravityInit,	// G171 トロッコ重力 45度左下
	GmGmkTruckGravityInit,	// G172 トロッコ重力 45度左上
	GmGmkTruckGravityInit,	// G173 トロッコ重力 45度右上
	GmGmkTruckGravityInit,	// G174 トロッコ重力 45度右下
	GmGmkTruckGravityInit,	// G175 トロッコ重力 60度左下
	GmGmkTruckGravityInit,	// G176 トロッコ重力 60度左上
	GmGmkTruckGravityInit,	// G177 トロッコ重力 60度右下
	GmGmkTruckGravityInit,	// G178 トロッコ重力 60度右下
	GmGmkTruckGravityInit,	// G179 トロッコ重力 R左下
// 240
	GmGmkTruckGravityInit,	// G180 トロッコ重力 R左上
	GmGmkTruckGravityInit,	// G181 トロッコ重力 R右上
	GmGmkTruckGravityInit,	// G182 トロッコ重力 R右下
	GmGmkTruckGravityInit,	// G183 トロッコ重力 逆R左下
	GmGmkTruckGravityInit,	// G184 トロッコ重力 逆R左上
	GmGmkTruckGravityInit,	// G185 トロッコ重力 逆R右上
	GmGmkTruckGravityInit,	// G186 トロッコ重力 逆R右下
	GmGmkSwitchInit,		// G187 スイッチ
	GmGmkSwWallInit,		// G188 スイッチ壁 ZONE3 横向き右出現
	GmGmkSwWallInit,		// G189 スイッチ壁 ZONE3 横向き左出現

// 250
	GmGmkSwWallInit,		// G190 スイッチ壁 ZONE3 縦向き下出現
	GmGmkSwWallInit,		// G191 スイッチ壁 ZONE3 縦向き上出現
	GmGmkSwWallInit,		// G192 スイッチ壁 ZONE3 ロング 横向き右出現
	GmGmkSwWallInit,		// G193 スイッチ壁 ZONE3 ロング 横向き左出現
	GmGmkSwWallInit,		// G194 スイッチ壁 ZONE3 ロング 縦向き下出現
	GmGmkSwWallInit,		// G195 スイッチ壁 ZONE3 ロング 縦向き上出現
	GmGmkSwWallInit,		// G196 スイッチ壁 ZONE4 横向き右出現
	GmGmkSwWallInit,		// G197 スイッチ壁 ZONE4 横向き左出現
	GmGmkSwWallInit,		// G198 スイッチ壁 ZONE4 縦向き下出現
	GmGmkSwWallInit,		// G199 スイッチ壁 ZONE4 縦向き上出現
// 260
	GmGmkLoopInit,			// G200 ループ
	GmGmkShutterInInit,		// G201 シャッター入口
	GmGmkShutterOutInit,	// G202 シャッター出口
	GmGmkNeedleNeonInitStand,	// G203 ネオン針（スタンド部）
	GmGmkTruckNoLandingInit,// G204 トロッコ接地不可 上
	GmGmkTruckNoLandingInit,// G205 トロッコ接地不可 左
	GmGmkTruckNoLandingInit,// G206 トロッコ接地不可 上
	GmGmkTruckNoLandingInit,// G207 トロッコ接地不可 右
	GmGmkTruckGravityInit,	// G208 重力強制変換 下
	GmGmkTruckGravityInit,	// G209 重力強制変換 左

// 270
	GmGmkTruckGravityInit,	// G210 重力強制変換 上
	GmGmkTruckGravityInit,	// G211 重力強制変換 右
	GmGmkBreakWall_C1_H_Init,// G212 破壊可能壁 水平タイプ
	GmGmkZ3LandPulleyInit,	// G213 Zone3浮島付随滑車
	GmGmkZ3LandRopeInit,	// G214 Zone3浮島付随ロープ（縦）
	GmGmkZ3LandRopeInit,	// G215 Zone3浮島付随ロープ（横）
	GmGmkFlagChangeInit,	// G216 エンディングソニック操作無効
	GmGmkFlagChangeInit,	// G217 エンディングソニックブレーキ開始
#if !defined(SONIC4_TRIAL)
	GmGmkBoss3RouteInit,	// G218 ボス3経路
	GmGmkBoss3PillarInitManager,	// G219 ボス3柱管理
#else
	NULL,
	NULL,
#endif // !defined(SONIC4_TRIAL)

// 280
	GmGmkEndingAnimalInit,	// G220 エンディング用動物
#if !defined(SONIC4_TRIAL)
	GmGmkBoss5TriggerInit,	// G221 ボスFINAL発動トリガ
	GmGmkBoss5LandPlaceInit,// G222 ボスFINAL足場配置基準
#else
	NULL,
	NULL,
#endif // !defined(SONIC4_TRIAL)
	GmGmkFlagChangeInit,	// G223 データロードギミック
	GmGmkPressPillarInit,	// G224 迫り出す柱（下から）
	GmGmkPressPillarInit,	// G225 迫り出す柱（上から）
	GmGmkFlagChangeInit,	// G226 迫り出す柱（起動判定）
	GmGmkDSignInit,			// G227 危険告知看板 下
	GmGmkDSignInit,			// G228 危険告知看板 左
	GmGmkDSignInit,			// G229 危険告知看板 上

// 290
	GmGmkDSignInit,			// G230 危険告知看板 右
	GmGmkSsArrowInit,		// G231 SpecialStage 矢印
	GmGmkSsOblongInit,		// G232 SpecialStage RingGate終端柱
	GmGmkDecoFrameMgrInit,	// G233 装飾フレーム管理（通路用）
	GmGmkDecoFrameMgrInit,	// G234 装飾フレーム管理（ボス用）	
	NULL,	
	NULL,	
	NULL,	
	NULL,	
	NULL,	

// 300
	// LocalEventBirth
	// ネミッサで配置しないイベント
	GmGmkRockFallInit,			//大岩（落下）
	GmGmkCapsuleBodyInit,		// カプセル（本体）
	GmGmkCamScrLimitInit,		// スクロール制限設置（プログラム呼び出し用）
	GmGmkCamScrLimitReleaseInit,// スクロール制限解除（プログラム呼び出し用）
	GmGmkSplRingInit,			// SPLリング
	GmGmkDrainTankSplashInit,	// 排液装置（噴出す水）
	GmGmkRockHookInit,			// 大岩を支える装置
	GmGmkRockChaseInit,			// 大岩（追跡）

	// ネミッサでもデバッグ配置でも配置しない系
	// エネミー
	GmEneTStarNeedleInit,	// テルスターニードル
	GmEneUnidesNeedleInit,	// ウニデスニードル

// 300
	GmEneUniuniNeedleInit,	// ウニウニニードル
	GmEneKamaLeftHandInit,		// カマキラーの左手
	GmEneKamaRightHandInit,		// カマキラーの右手

	// ボス系
#if !defined(SONIC4_TRIAL)
	GmBoss1BodyInit,	// ボス1本体
	GmBoss1ChainInit,	// ボス1鎖
	GmBoss1EggInit,		// ボス1エッグマン

	GmBoss2BodyInit,	// ボス2本体
	GmBoss2EggInit,		// ボス2エッグマン
	GmBoss2BallInit,	// ボス2トゲボール

	GmBoss3BodyInit,	// ボス3本体
	GmBoss3EggInit,		// ボス3エッグマン

	GmBoss4BodyInit,			// ボス4本体
// 310
	GmBoss4EggInit,				// ボス4エッグマン
	GmBoss4CapsuleInit1st,		// ボス4カプセル1段階目
	GmBoss4CapsuleInit2nd,		// ボス4カプセル2段階目

	GmBoss4ChibiInit1st,		// ちびエッグマン
	GmBoss4ChibiInit2nd,		// ちびエッグマン２段階目
	GmBoss4ChibiInit2ndSpeed,	// ちびエッグマン２段階目(SPEED)
	GmBoss4ChibiInit2ndBig,		// ちびエッグマン２段階目(BIG)
	GmBoss4ChibiInit2ndIron,	// ちびエッグマン２段階目(IRON)

	GmBoss5BodyInit,	// ボス5本体
	GmBoss5CoreInit,	// ボス5中心オブジェクト
// 320
	GmBoss5RocketInit,	// ボス5ロケット
	GmBoss5TurretInit,	// ボス5砲塔
	GmBoss5EggInit,		// ボス5エッグマン
#else
	NULL,
	NULL,
	NULL,
	
	NULL,
	NULL,
	NULL,
	
	NULL,
	NULL,
	
	NULL,
	// 310
	NULL,
	NULL,
	NULL,
	
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	
	NULL,
	NULL,
	// 320
	NULL,
	NULL,
	NULL,
#endif // !defined(SONIC4_TRIAL)

	GmGmkBackNeedleInit,		// 173 後ろ側トゲ
	GmGmkStandNeedleInit,		// 174 トゲ台座
	GmGmkNeedleNeonInitNeedle,	// ネオン針（針部）
	GmGmkNeedleNeonInitGlaer,	// ネオン針（グレア部）
#if !defined(SONIC4_TRIAL)
	GmGmkBoss3PillarInitParts,	// ボス3柱
	GmGmkBoss3PillarInitParts,	// ボス3柱
	GmGmkBoss3PillarInitParts,	// ボス3柱
// 330
	GmGmkBoss3PillarInitParts,	// ボス3柱
	GmGmkBoss3PillarInitWall,	// ボス3壁
	GmBoss5LandInit,			// ボス5足場
	GmBoss5CtpltInit,			// ボス5カタパルト
#else
	NULL,
	NULL,
	NULL,
// 330
	NULL,
	NULL,
	NULL,
#endif // !defined(SONIC4_TRIAL)
	//NULL,		// 
};

/// 装飾生成関数テーブル
OBS_OBJECT_WORK *(*const g_gm_decorate_tbl[GMD_DECORATE_ID_MAX])(GMS_EVE_RECORD_DECORATE *dec_rec, fx32 x, fx32 y, u8 type) = {
#if GMD_DECO_TEST
	GmDecoInitModel,			// zone3 壁
	GmDecoInitModel,			// zone3 壁
	GmDecoInitModel,			// zone3 壁
	GmDecoInitModel,			// zone3 壁
	GmDecoInitModel,			// zone3 壁
	GmDecoInitModel,			// zone3 壁
	
	GmDecoInitModelMaterial,	// zone3 ウォータースライダー左
	GmDecoInitModelMotionMaterial,	// zone3 ウォータースライダー左
	GmDecoInitModelMotionMaterial,	// zone3 ウォータースライダー左
	GmDecoInitModelMotionMaterial,	// zone3 ウォータースライダー左
	GmDecoInitModelMaterial,	// zone3 ウォータースライダー右
	GmDecoInitModelMotionMaterial,	// zone3 ウォータースライダー右
	GmDecoInitModelMotionMaterial,	// zone3 ウォータースライダー右
	GmDecoInitModelMotionMaterial,	// zone3 ウォータースライダー右

	GmDecoInitPrimitive3D,		// グレア
	GmDecoInitPrimitive3D,		// グレア
	GmDecoInitPrimitive3D,		// グレア

	GmDecoInitModelMotionTouch,	// zone1 ひまわり
	GmDecoInitModel,			// zone1 壁
	GmDecoInitModel,			// zone1 壁
	
	GmDecoInitFall,				// zone1 滝

	GmDecoInitModel,			//アシハナ
	GmDecoInitModel,			//アシハナ
	GmDecoInitModel,			//アシハナ
	GmDecoInitModel,			//花
	GmDecoInitModel,			//花
	GmDecoInitModelMotionTouch,			//木
	
	GmDecoInitFall,				// zone1 奥の滝

	GmDecoInitModel,			// zone1 壁
	GmDecoInitModel,			// zone1 壁
	GmDecoInitModel,			// zone1 壁
	GmDecoInitModel,			// zone1 壁
	GmDecoInitModel,			// zone1 壁
	
	GmDecoInitModel,			// zone1 壁（奥）
	GmDecoInitModel,			// zone1 壁（奥）
	GmDecoInitModel,			// zone1 壁（奥）
	GmDecoInitModel,			// zone1 壁（奥）
	GmDecoInitModel,			// zone1 壁（奥）
	GmDecoInitModel,			// zone1 壁（奥）
	GmDecoInitModel,			// zone1 壁（奥）
	
	GmDecoInitFall,				// zone1 滝
	GmDecoInitFall,				// zone1 滝
	GmDecoInitFall,				// zone1 滝
	GmDecoInitFall,				// zone1 滝
	GmDecoInitFall,				// zone1 滝
	GmDecoInitFall,				// zone1 滝
	GmDecoInitFall,				// zone1 滝
	
	GmDecoInitFall,				// zone1 滝（奥）
	GmDecoInitFall,				// zone1 滝（奥）
	GmDecoInitFall,				// zone1 滝（奥）
	GmDecoInitFall,				// zone1 滝（奥）
	GmDecoInitFall,				// zone1 滝（奥）
	GmDecoInitFall,				// zone1 滝（奥）
	GmDecoInitFall,				// zone1 滝（奥）

	GmDecoInitPrimitive3D,		// グレア

	GmDecoInitModel,	//排液装置（左）
	GmDecoInitModel,	//排液装置出口（左）
	GmDecoInitModel,	//排液装置（右）
	GmDecoInitModel,	//排液装置出口（右）

	GmDecoInitModel,			//ZONE3岩
	GmDecoInitModel,			//ZONE3岩
	GmDecoInitModel,			//ZONE3岩
	GmDecoInitModel,			//ZONE3岩
	GmDecoInitModel,			//ZONE3岩
	GmDecoInitModel,			//ZONE3岩
	GmDecoInitModel,			//ZONE3岩

	GmDecoInitEffect,				//ZONE3 ウォータースライダー用エフェクト水面下
	GmDecoInitEffect,				//ZONE3 ウォータースライダー用エフェクト水飛沫
	GmDecoInitEffectBlockAndNext,	//ZONE3 蝋燭
	GmDecoInitEffectBlock,			//ZONE3 壁の顔

	GmDecoInitFall,			//ZONE3噴出す水
	GmDecoInitFall,			//ZONE3噴出す水
	GmDecoInitFall,			//ZONE3噴出す水
	GmDecoInitFall,			//ZONE3噴出す水

	GmDecoInitModel,				//ZONE3植物
	GmDecoInitModel,				//ZONE3植物
	GmDecoInitModel,				//ZONE3植物
	GmDecoInitModel,				//ZONE3植物
	GmDecoInitModel,				//ZONE3植物
	GmDecoInitModel,				//ZONE3植物

	GmDecoInitModel,	//ZONE3岩（前）
	GmDecoInitModel,	//ZONE3岩（前）
	GmDecoInitModel,	//ZONE3岩（前）
	GmDecoInitModel,	//ZONE3岩（前）
	GmDecoInitModel,	//ZONE3岩（前）
	GmDecoInitModel,		//ZONE3植物（前）
	GmDecoInitModel,		//ZONE3植物（前）
	GmDecoInitModel,		//壁（前）
	GmDecoInitModel,		//壁（前）

	GmDecoInitModel,		//ZONE3レール角
	GmDecoInitModel,		//ZONE3レール角
	GmDecoInitModel,		//ZONE3レール角（左右反転）
	GmDecoInitModel,		//ZONE3レール角（左右反転）

	GmDecoInitModel,			//ZONE4 歯車用
	GmDecoInitModel,			//ZONE4 歯車用
	GmDecoInitModel,			//ZONE4 歯車用
	GmDecoInitModel,			//ZONE4 歯車用
	GmDecoInitModel,			//ZONE4 歯車用
	GmDecoInitModel,			//ZONE4 歯車用
	
	GmDecoInitEffect,			//ZONE1滝上部用エフェクト
	NULL,						//ZONE3 蝋燭（使用しなくなりました）
	
	GmDecoInitModel,			//ZONE3 ウォータースライダー排水溝
	GmDecoInitModel,			//ZONE3 ウォータースライダー排水溝（左右反転）
	GmDecoInitModel,			//ZONE3 大岩レール
	GmDecoInitModel,			//ZONE3 大岩レール
	GmDecoInitModel,			//ZONE3 大岩レール
	GmDecoInitModel,			//ZONE3 大岩レール（左右反転）
	GmDecoInitModel,			//ZONE3 大岩レール（左右反転）
	GmDecoInitModel,			//ZONE3 大岩レール（左右反転）
	GmDecoInitPrimitive3D,		// グレア
	GmDecoInitPrimitive3D,		// グレア
	GmDecoInitModelMotionTouch,	// zone1 ひまわり
	GmDecoInitModel,			//花
	GmDecoInitModel,			//花
	GmDecoInitModelMotionTouch,			//木
	
	GmDecoInitModel,			//ZONE4歯車用柱
	GmDecoInitModel,			//ZONE4歯車用柱
	GmDecoInitModel,			//ZONE4歯車用柱
	GmDecoInitModel,			//ZONE4歯車用柱（上下反転）
	GmDecoInitModel,			//ZONE4歯車用柱（上下反転）
	GmDecoInitModel,			//ZONE4歯車用柱（上下反転）
	GmDecoInitModel,			//ZONE4歯車用柱
	GmDecoInitModel,			//ZONE4歯車用柱
	GmDecoInitModel,			//ZONE4歯車用柱
	GmDecoInitModel,			//ZONE4歯車用柱（左右反転）
	GmDecoInitModel,			//ZONE4歯車用柱（左右反転）
	GmDecoInitModel,			//ZONE4歯車用柱（左右反転）

	GmDecoInitModel,			//ZONE4 歯車用（前）
	GmDecoInitModel,			//ZONE4 歯車用（前）
	GmDecoInitModel,			//ZONE4 歯車用（前）
	GmDecoInitModel,			//ZONE4 歯車用（前）
	GmDecoInitModel,			//ZONE4 歯車用（前）
	GmDecoInitModel,			//ZONE4 歯車用（前）
	
	GmDecoInitModel,				//ZONE4 柱
	GmDecoInitModel,				//ZONE4 柱
	GmDecoInitModel,				//ZONE4 柱
	GmDecoInitModel,				//ZONE4 柱
	GmDecoInitModel,				//ZONE4 柱
	GmDecoInitModel,				//ZONE4 柱
	GmDecoInitModel,				//ZONE4 柱
	
	GmDecoInitModel,				//ZONE4 柱

	GmDecoInitModelEffect,				//ZONE4 パトランプ
	NULL,				//ZONE4 パトランプ

	GmDecoInitModel,					//ZONE4 柱
	GmDecoInitModel,					//ZONE4 柱
	GmDecoInitModel,					//ZONE4 柱
	GmDecoInitModel,					//ZONE4 柱
	GmDecoInitModel,					//ZONE4 柱
	GmDecoInitModel,					//ZONE4 柱
	GmDecoInitModel,					//ZONE4 柱	
	GmDecoInitModel,			//ZONE4 ポップスチーム角
	GmDecoInitModel,			//ZONE4 ポップスチーム角
	GmDecoInitModel,			//ZONE4 ポップスチーム角
	GmDecoInitModel,			//ZONE4 ポップスチーム角
	GmDecoInitModel,		//ZONE4 ポップスチーム角
	GmDecoInitModel,		//ZONE4 ポップスチーム角
	GmDecoInitModel,		//ZONE4 ポップスチーム角
	GmDecoInitModel,		//ZONE4 ポップスチーム角
	GmDecoInitModel,		//ZONE4 ポップスチーム角
	GmDecoInitModel,		//ZONE4 ポップスチーム角
	GmDecoInitModel,		//ZONE4 ポップスチーム角
	GmDecoInitModel,		//ZONE4 ポップスチーム角
	GmDecoInitModel,			//ZONE4 ポップスチームチューブ
	GmDecoInitModel,			//ZONE4 ポップスチームチューブ
	GmDecoInitModel,			//ZONE4 ポップスチームチューブ
	GmDecoInitModel,			//ZONE4 ポップスチームチューブ
	GmDecoInitModel,	//ZONE4 ポップスチームチューブ
	GmDecoInitModel,	//ZONE4 ポップスチームチューブ

	GmDecoInitModel,			//ZONE4 浮島用レール
	GmDecoInitModel,			//ZONE4 浮島用レール

	GmDecoInitModelMotionMaterialTouch,		//ZONEF シャッター
	GmDecoInitModelMotionMaterialTouch,		//ZONEF シャッター

	GmDecoInitModelMotionMaterial,		//ZONEF シャッター
	GmDecoInitModel,					//ZONEF シャッター
	GmDecoInitModelMotionMaterial,		//ZONEF シャッター
	GmDecoInitModelMaterial,			//ZONEF シャッター

	GmDecoInitModelMotionMaterial,		//ZONEF シャッター
	GmDecoInitModel,					//ZONEF シャッター
	GmDecoInitModelMotionMaterial,		//ZONEF シャッター
	GmDecoInitModelMaterial,		//ZONEF シャッター

	GmDecoInitModel,		//ZONE2 植物
	GmDecoInitModel,		//ZONE2 植物
#if _IPHONE
	NULL,
#else
	GmDecoInitEffectBlock,	//ZONEF ファイナルボス用ライト
#endif // _IPHONE

	GmDecoInitModelLoop,			//ZONEF シャッター

	
#else
	NULL,			// zone3 壁
	NULL,			// zone3 壁
	NULL,			// zone3 壁
	NULL,			// zone3 壁
	NULL,			// zone3 壁
	NULL,			// zone3 壁
	
	NULL,	// zone3 ウォータースライダー左
	NULL,	// zone3 ウォータースライダー左
	NULL,	// zone3 ウォータースライダー左
	NULL,	// zone3 ウォータースライダー左
	NULL,	// zone3 ウォータースライダー右
	NULL,	// zone3 ウォータースライダー右
	NULL,	// zone3 ウォータースライダー右
	NULL,	// zone3 ウォータースライダー右

	NULL,		// グレア
	NULL,		// グレア
	NULL,		// グレア

	NULL,	// zone1 ひまわり
	NULL,			// zone1 壁
	NULL,			// zone1 壁
	
	NULL,			// zone1 滝

	NULL,			//アシハナ
	NULL,			//アシハナ
	NULL,			//アシハナ
	NULL,			//花
	NULL,			//花
	NULL,			//木
	
	NULL,			// zone1 奥の滝

	NULL,			// zone1 壁
	NULL,			// zone1 壁
	NULL,			// zone1 壁
	NULL,			// zone1 壁
	NULL,			// zone1 壁
	
	NULL,			// zone1 壁（奥）
	NULL,			// zone1 壁（奥）
	NULL,			// zone1 壁（奥）
	NULL,			// zone1 壁（奥）
	NULL,			// zone1 壁（奥）
	NULL,			// zone1 壁（奥）
	NULL,			// zone1 壁（奥）
	
	NULL,				// zone1 滝
	NULL,				// zone1 滝
	NULL,				// zone1 滝
	NULL,				// zone1 滝
	NULL,				// zone1 滝
	NULL,				// zone1 滝
	NULL,				// zone1 滝
	
	NULL,				// zone1 滝（奥）
	NULL,				// zone1 滝（奥）
	NULL,				// zone1 滝（奥）
	NULL,				// zone1 滝（奥）
	NULL,				// zone1 滝（奥）
	NULL,				// zone1 滝（奥）
	NULL,				// zone1 滝（奥）

	NULL,				// グレア

	NULL,	//排液装置（左）
	NULL,	//排液装置出口（左）
	NULL,	//排液装置（右）
	NULL,	//排液装置出口（右）

	NULL,			//ZONE3岩
	NULL,			//ZONE3岩
	NULL,			//ZONE3岩
	NULL,			//ZONE3岩
	NULL,			//ZONE3岩
	NULL,			//ZONE3岩
	NULL,			//ZONE3岩

	NULL,			//ZONE3 ウォータースライダー用エフェクト水面下
	NULL,			//ZONE3 ウォータースライダー用エフェクト水飛沫
	NULL,			//ZONE3 蝋燭
	NULL,			//ZONE3 壁の顔

	NULL,					//ZONE3噴出す水
	NULL,					//ZONE3噴出す水
	NULL,					//ZONE3噴出す水
	NULL,					//ZONE3噴出す水

	NULL,				//ZONE3植物
	NULL,				//ZONE3植物
	NULL,				//ZONE3植物
	NULL,				//ZONE3植物
	NULL,				//ZONE3植物
	NULL,				//ZONE3植物

	NULL,	//ZONE3岩（前）
	NULL,	//ZONE3岩（前）
	NULL,	//ZONE3岩（前）
	NULL,	//ZONE3岩（前）
	NULL,	//ZONE3岩（前）
	NULL,		//ZONE3植物（前）
	NULL,		//ZONE3植物（前）
	NULL,		//壁（前）
	NULL,		//壁（前）

	NULL,		//ZONE3レール角
	NULL,		//ZONE3レール角
	NULL,		//ZONE3レール角（左右反転）
	NULL,		//ZONE3レール角（左右反転）

	NULL,			//ZONE4 歯車用
	NULL,			//ZONE4 歯車用
	NULL,			//ZONE4 歯車用
	NULL,			//ZONE4 歯車用
	NULL,			//ZONE4 歯車用
	NULL,			//ZONE4 歯車用
	
	NULL,		//ZONE1滝上部用エフェクト
	NULL,		//ZONE3 蝋燭
	
	NULL,			//ZONE3 ウォータースライダー排水溝
	NULL,			//ZONE3 ウォータースライダー排水溝（左右反転）
	NULL,			//ZONE3 大岩レール
	NULL,			//ZONE3 大岩レール
	NULL,			//ZONE3 大岩レール
	NULL,			//ZONE3 大岩レール（左右反転）
	NULL,			//ZONE3 大岩レール（左右反転）
	NULL,			//ZONE3 大岩レール（左右反転）
	NULL,			// グレア
	NULL,			// グレア
	NULL,			// zone1 ひまわり
	NULL,			//花
	NULL,			//花
	NULL,			//木
	
	NULL,			//ZONE4歯車用柱
	NULL,			//ZONE4歯車用柱
	NULL,			//ZONE4歯車用柱
	NULL,			//ZONE4歯車用柱（上下反転）
	NULL,			//ZONE4歯車用柱（上下反転）
	NULL,			//ZONE4歯車用柱（上下反転）
	NULL,			//ZONE4歯車用柱
	NULL,			//ZONE4歯車用柱
	NULL,			//ZONE4歯車用柱
	NULL,			//ZONE4歯車用柱（左右反転）
	NULL,			//ZONE4歯車用柱（左右反転）
	NULL,			//ZONE4歯車用柱（左右反転）

	NULL,			//ZONE4 歯車用（前）
	NULL,			//ZONE4 歯車用（前）
	NULL,			//ZONE4 歯車用（前）
	NULL,			//ZONE4 歯車用（前）
	NULL,			//ZONE4 歯車用（前）
	NULL,			//ZONE4 歯車用（前）
	
	NULL,				//ZONE4 柱
	NULL,				//ZONE4 柱
	NULL,				//ZONE4 柱
	NULL,				//ZONE4 柱
	NULL,				//ZONE4 柱
	NULL,				//ZONE4 柱
	NULL,				//ZONE4 柱
	
	NULL,				//ZONE4 柱

	NULL,				//ZONE4 パトランプ
	NULL,				//ZONE4 パトランプ

	NULL,				//ZONE4 柱
	NULL,				//ZONE4 柱
	NULL,				//ZONE4 柱
	NULL,				//ZONE4 柱
	NULL,				//ZONE4 柱
	NULL,				//ZONE4 柱
	NULL,				//ZONE4 柱	
	NULL,				//ZONE4 ポップスチーム角
	NULL,				//ZONE4 ポップスチーム角
	NULL,				//ZONE4 ポップスチーム角
	NULL,				//ZONE4 ポップスチーム角
	NULL,				//ZONE4 ポップスチーム角
	NULL,				//ZONE4 ポップスチーム角
	NULL,				//ZONE4 ポップスチーム角
	NULL,				//ZONE4 ポップスチーム角
	NULL,				//ZONE4 ポップスチーム角
	NULL,				//ZONE4 ポップスチーム角
	NULL,				//ZONE4 ポップスチーム角
	NULL,				//ZONE4 ポップスチーム角
	NULL,				//ZONE4 ポップスチームチューブ
	NULL,				//ZONE4 ポップスチームチューブ
	NULL,				//ZONE4 ポップスチームチューブ
	NULL,				//ZONE4 ポップスチームチューブ
	NULL,				//ZONE4 ポップスチームチューブ
	NULL,				//ZONE4 ポップスチームチューブ

	NULL,			//ZONE4 浮島用レール
	NULL,			//ZONE4 浮島用レール

	NULL,		//ZONEF シャッター
	NULL,		//ZONEF シャッター

	NULL,		//ZONEF シャッター
	NULL,		//ZONEF シャッター
	NULL,		//ZONEF シャッター
	NULL,		//ZONEF シャッター

	NULL,		//ZONEF シャッター
	NULL,		//ZONEF シャッター
	NULL,		//ZONEF シャッター
	NULL,		//ZONEF シャッター

	NULL,		//ZONE2 植物
	NULL,		//ZONE2 植物

	NULL,	//ZONEF ファイナルボス用ライト

	NULL,			//ZONEF シャッター

#endif	//GMD_DECO_TEST
};


// 生成矩形サイズテーブル
// --------------------------------------------------------------
// P	: カメラ
// S	: イベントサイズ
// A	: S + 最大移動速度dot
// ■	: 生成範囲(最大移動速度dot以上)
// ▲	: 生成範囲外での生存可能範囲(カメラ(生成範囲)のゆれ許容範囲を含む)
// ※1  : 
//
// 通常、画面端から256ドット＋最大移動量までのブロックを生成チェックする
//
// イベント生成チェックは、画面から256ドットまでの距離
// ├        256       ┼画面サイズ(※1) ┼       256        ┤
// ┌─────────┬────────┬─────────┐┬
// │　　　　　　　　　│　　　　　　　　│　　　　　　　　　│
// │　　　　　　　　　│　　　　　　　　│　　　　　　　　　│
// │　　　　　　　　　│　　　　　　　　│　　　　　　　　　│
// │　　　┌─────┼────────┼─────┐　　　│
// │　　　│▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲│　　　│256
// │　　　│▲┌───┼────────┼───┐▲│　　　│
// │　　　│▲│■■■■■■■■■■■■■■■■│▲│　　　│
// │　　　│▲│■┌─┼────┳───┼─┐■│▲│　　　│
// │　　　│▲│■│　│　　　　Ａ　　　│　│■│▲│　　　│
// ├───┼▲┼■┼─Ｐ────┻───┼─┼■┼▲┼───┤┼
// │　　　│▲│■┣Ａ┫　　　　　　　　┣Ａ┫■│▲│　　　│
// │　　　│▲│■│　│　　　　　　　　│　│■│▲│　　　│
// │　　　│▲│■│　│　　　画面　　　│　│■│▲│　　　│
// │　　　│▲│■│　│　　　　　　　　│　│■│▲│　　　│画面サイズ(※1)
// │　　　│▲│■│　│　　　　　　　　│　│■│▲│　　　│
// │　　　│▲│■│　│　　　　　　　　│　│■│▲│　　　│
// │　　　│▲│■│　│　　　　　　　　│　│■│▲│　　　│
// ├───┼▲┼■┼─┼────┳───┼─┼■┼▲┼───┤┼
// │　　　│▲│■│　│　　　　Ａ　　　│　│■│▲│　　　│
// │　　　│▲│■└─┼────┻───┼─┘■│▲│　　　│
// │　　　│▲│■■■■■■■■■■■■■■■■│▲│　　　│
// │　　　│▲└───┼────────┼───┘▲│　　　│
// │　　　│▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲│　　　│256
// │　　　└─────┼────────┼─────┘　　　│
// │　　　　　　　　　│　　　　　　　　│　　　　　　　　　│
// │　　オブジェクト　│　　　　　　　　│　　　　　　　　　│
// │　　消去範囲　　　│　　　　　　　　│　　　　　　　　　│
// └─────────┴────────┴─────────┘┴

/// イベントサイズテーブル
u16 g_gm_event_size_tbl[GMD_EVENT_ID_MAX] = {
	// 敵
//  00
	128,		// E000 ハリセンボ
	255,		// E001 モトラ
	255,		// E002 スティンガー
	255,		// E003 ガブッチョ
	128,		// E004 メレオン
	255,		// E005 メレオン 落下タイプ
	255,		// E006 モグリン
	255,		// E007 ガードン
	128,		// E008 テルスター
	255,		// E009 カニパンチ

//  10
	255,		// E010 ハロゲン
	128,		// E011 ウニデス
	255,		// E012 ウニウニ
	255,		// E013 ぶくぶく
	255,		// E014 カマキラー
	128,		// E015 ハリセンボ 赤
	64,		// E016
	64,		// E017
	64,		// E018
	64,		// E019

//  20
	64,		// E020
	64,		// E021
	64,		// E022
	64,		// E023
	64,		// E024
	64,		// E025
	64,		// E026
	64,		// E027
	64,		// E028
	64,		// E029

//  30
	64,		// E030
	64,		// E031
	64,		// E032
	64,		// E033
	64,		// E034
	64,		// E035
	64,		// E036
	64,		// E037
	64,		// E038
	64,		// E039

//  40
	64,		// E040
	64,		// E041
	64,		// E042
	64,		// E043
	64,		// E044
	64,		// E045
	64,		// E046
	64,		// E047
	64,		// E048
	64,		// E049

//  50
	64,		// E050
	64,		// E051
	64,		// E052
	64,		// E053
	64,		// E054
	64,		// E055 ボスFinal
	64,		// E056 ボス4
	64,		// E057 ボス3
	64,		// E058 ボス2
	64,		// E059 ボス1

	// ギミック
//  60
	255,		// G000 接地
	255,		// G001 A面切り替え
	255,		// G002 B面切り替え
	64,		// G003 アイテム
	64,		// G004 アイテム
	64,		// G005 アイテム
	64,		// G006 アイテム
	64,		// G007 アイテム
	64,		// G008 アイテム
	64,		// G009 アイテム
//  70         
	32,		// G010 スプリング上
	32,		// G011 スプリング右上
	32,		// G012 スプリング右
	32,		// G013 スプリング右下
	32,		// G014 スプリング下
	32,		// G015 スプリング左下
	32,		// G016 スプリング左
	32,		// G017 スプリング左上
	32,		// G018 スプリング右上(地面埋まり)
	32,		// G019 スプリング左上(地面埋まり)
//  80         
	64,		// G020 スタート位置設定
	255,	// G021 浮島(通常)
	255,	// G022 浮島(大)
	255,	// G023 浮島(当たり)
	64,		// G024 大岩(追跡)
	128,	// G025 大岩(落下)
	64,		// G026 滑車本体
	64,		// G027 滑車ポール（左）
	64,		// G028 滑車ポール（右）
	64,		// G029 滑車ロープ（水平）
//  90         
	64,		// G030 滑車ロープ（斜め：右）
	64,		// G031 トゲ(上向き)
	64,		// G032 トゲ(左向き)
	64,		// G033 トゲ(下向き)
	64,		// G034 トゲ(右向き)
	64,		// G035 滑車ロープ（斜め：左）
	64,		// G036 大岩（傾斜）
	64,		// G037 出入りトゲ(上向き)
	64,		// G038 出入りトゲ(下向き)
	255,	// G039 落下ミス判定
// 100         
	32,		// G040 スプリング右上(地面埋まり) A面用
	32,		// G041 スプリング左上(地面埋まり) A面用
	64,		// G042 水位変更（左から）
	64,		// G043 水位変更（右から）
	64,		// G044 水位変更（上から）
	64,		// G045 水位変更（下から）
	64,		// G046 水位変更（開始時、再開時）
	64,		// G047ダッシュパネル右
	64,		// G048ダッシュパネル左
	64,		// G049ダッシュパネル縦右壁
// 110         
	64,		// G050ダッシュパネル縦左壁
	64,		// G051 息継ぎの泡
	64,		// G052 ターザンロープ
	64,		// G053 ターザンロープ左
	64,		// G054 ターザンロープ右
	64,		// G055 ウォータースライダー左（真下）
	64,		// G056 ウォータースライダー左（30度）
	64,		// G057 ウォータースライダー左（45度）
	64,		// G058 ウォータースライダー左（60度）
	64,		// G059 ウォータースライダー右（真下）
// 120         
	64,		// G060 ウォータースライダー右（30度）
	64,		// G061 ウォータースライダー右（45度）
	64,		// G062 ウォータースライダー右（60度）
#if _IPHONE
	512,		// G063 ゴールパネル(生成範囲を広げる)
#else
	128,		// G063 ゴールパネル
#endif // _IPHONE
	64,		// G064 崩落足場　右張り出し
	64,		// G065 崩落足場　左張り出し
	64,		// G066 上向ピストン
	64,		// G067 下向ピストン
	255,	// G068 浮島(回転移動)
	64,		// G069 ポイントマーカー
// 130         
	64,		// G070 破壊可能オブジェ
	64,		// G071 カメラセンター変更
	64,		// G072 スクロール制限
	255,		// G073 ベルトコンベヤー
	128,		// G074 破壊可能壁
	128,		// G075 破壊可能壁
	128,		// G076 破壊可能壁
	128,		// G077 破壊可能壁
	128,		// G078 破壊可能壁
	64,		// G079 Ｓ字パイプ
// 140         
	128,		// G080 破壊可能壁
	64,		// G081 ストッパー　普通
	64,		// G082 ストッパー　スロットスターター
	128,		// G083 破壊可能壁横型(床天井)
	64,		// G084 登るバンパー左くっつき
	64,		// G085 登るバンパー右くっつき
	64,		// G086	バンパー
	64,		// G087	バンパー
	64,		// G088	バンパー
	64,		// G089	バンパー
// 150
	64,		// G090	バンパー
	64,		// G091	バンパー
	64,		// G092	バンパー
	64,		// G093	バンパー
	64,		// G094	バンパー
	64,		// G095	バンパー
	64,		// G096 槍(上)
	64,		// G097 槍(下)
	64,		// G098 槍(左)
	64,		// G099 槍(右)
// 160         
	64,		// G100 コークスクリュー
	64,		// G101 大砲
	64,		// G102 ループカメラ
	64,		// G103 カプセル
	64,		// G104 ３耐バンパー（0度）
	64,		// G105	３耐バンパー（45度）
	64,		// G106	３耐バンパー（90度）
	64,		// G107	３耐バンパー（135度）
	64,		// G108	ボビン
	64,		// G109 フリッパー（上にはじく：左側）
// 170
	64,		// G110 フリッパー（上にはじく：右側）
	64,		// G111 フリッパー（左右にはじく）
	64,		// G112 スロット (G082と連動)
	64,		// G113 シーソー
	64,		// G114 シーソー
	64,		// G115 シーソー
	96,		// G116 丸太橋
	64,		// G117 スクロール制限解除
	64,		// G118 スプリングカタパルト０°
	64,		// G119 スプリングカタパルト４５°
// 180
	64,		// G120 スプリングカタパルト３１５°
	128,	// G121 歯車
	128,	// G122 移動歯車
	255,	// G123 移動歯車終点
	255,	// G124 歯車スイッチ
	64,		// G125 強制スピン
	64,		// G126 強制スピン
	64,		// G127 強制スピン
	64,		// G128 強制スピン
	255,	// G129 迫る壁
// 190
	255,	// G130 迫る壁止める
	255,	// G131 迫る壁速度変化
	64,		// G132 SpecialStage 四角柱
	64,		// G133 SpecialStage 丸柱
	64,		// G134 SpecialStage 丸柱（一方通行）
	64,		// G135 SpecialStage 丸柱のための矩形判定
	64,		// G136 SpecialStage 耐久柱
	64,		// G137 SpecialStage ゴール
	64,		// G138 SpecialStage カオスエメラルド
	64,		// G139 SpecialStage 時間パネル
// 200
	64,		// G140 SpecialStage リングゲート
	64,		// G141 スチームパイプ入り口→
	64,		// G142 スチームパイプ入り口←
	64,		// G143 スチームパイプ　パイプＡ↓
	64,		// G144 スチームパイプ　パイプＡ←
	64,		// G145 スチームパイプ　パイプＡ↑
	64,		// G146 スチームパイプ　パイプＡ→
	64,		// G147 スチームパイプ　パイプＢ↓
	64,		// G148 スチームパイプ　パイプＢ←
	64,		// G149 スチームパイプ　パイプＢ↑
// 210
	64,		// G150 スチームパイプ　パイプＢ→
	64,		// G151 スチームパイプ　ジョイント┌
	64,		// G152 スチームパイプ　ジョイント┐
	64,		// G153 スチームパイプ　ジョイント┘
	64,		// G154 スチームパイプ　ジョイント└
	64,		// G155 スチームパイプ　出口
	64,		// G156 排液装置入口
	110,	// G157 排液装置出口
	64,		// G158 ポップスチーム
	64,		// G159 ポップスチーム
// 220
	64,		// G160 ポップスチーム
	64,		// G161 ポップスチーム
	255,	// G162 トロッコ
	255,	// G163 トロッコ重力 平地下
	255,	// G164 トロッコ重力 平地左
	255,	// G165 トロッコ重力 平地上
	255,	// G166 トロッコ重力 平地右
	255,	// G167 トロッコ重力 30度左下
	255,	// G168 トロッコ重力 30度左上
	255,	// G169 トロッコ重力 30度右上
// 230
	255,	// G170 トロッコ重力 30度右下
	255,	// G171 トロッコ重力 45度左下
	255,	// G172 トロッコ重力 45度左上
	255,	// G173 トロッコ重力 45度右上
	255,	// G174 トロッコ重力 45度右下
	255,	// G175 トロッコ重力 60度左下
	255,	// G176 トロッコ重力 60度左上
	255,	// G177 トロッコ重力 60度右下
	255,	// G178 トロッコ重力 60度右下
	255,	// G179 トロッコ重力 R左下
// 240
	255,	// G180 トロッコ重力 R左上
	255,	// G181 トロッコ重力 R右上
	255,	// G182 トロッコ重力 R右下
	255,	// G183 トロッコ重力 逆R左下
	255,	// G184 トロッコ重力 逆R左上
	255,	// G185 トロッコ重力 逆R右上
	255,	// G186 トロッコ重力 逆R右下
	40,		// G187 スイッチ
	255,	// G188 スイッチ壁 ZONE3 横向き右出現
	255,	// G189 スイッチ壁 ZONE3 横向き左出現
// 250
	255,	// G190 スイッチ壁 ZONE3 縦向き下出現
	255,	// G191 スイッチ壁 ZONE3 縦向き上出現
	255,	// G192 スイッチ壁 ZONE3 ロング 横向き右出現
	255,	// G193 スイッチ壁 ZONE3 ロング 横向き左出現
	255,	// G194 スイッチ壁 ZONE3 ロング 縦向き下出現
	255,	// G195 スイッチ壁 ZONE3 ロング 縦向き上出現
	255,	// G196 スイッチ壁 ZONE4 横向き右出現
	255,	// G197 スイッチ壁 ZONE4 横向き左出現
	255,	// G198 スイッチ壁 ZONE4 縦向き下出現
	255,	// G199 スイッチ壁 ZONE4 縦向き上出現
// 260
	64,		// G200 ループ
	64,		// G201 シャッター入口
	64,		// G202 シャッター出口
	64,		// G203 ネオン針
	255,	// G204 トロッコ接地不可 上
	255,	// G205 トロッコ接地不可 左
	255,	// G206 トロッコ接地不可 上
	255,	// G207 トロッコ接地不可 右
	255,	// G208 重力強制変換 下
	255,	// G209 重力強制変換 左

// 270
	255,	// G210 重力強制変換 上
	255,	// G211 重力強制変換 右
	128,	// G212 破壊可能壁 水平タイプ
	64,		// G213 Zone3浮島付随滑車
	64,		// G214 Zone3浮島付随ロープ（縦）
	64,		// G215 Zone3浮島付随ロープ（横）
	64,		// G216 エンディングソニック操作無効
	64,		// G217 エンディングソニックブレーキ開始
	64,		// G218 ボス3経路
	64,		// G219 ボス3柱管理

// 280
	64,		// G220 エンディング用動物
	64,		// G221 ボスFINAL発動トリガ
	64,		// G222 ボスFINAL足場配置基準
	255,	// G223 データロードギミック
	64,		// G224 迫り出す柱（下から）
	64,		// G225 迫り出す柱（上から）
	64,		// G226 迫り出す柱（起動判定）
	128,	// G227 危険告知看板 下
	128,	// G228 危険告知看板 左
	128,	// G229 危険告知看板 上

// 290
	128,	// G230 危険告知看板 右
	64,		// G231 SpecialStage 矢印
	64,		// G232 SpecialStage RingGate終端柱
	64,		// G233 装飾フレーム管理（通路用）
	64,		// G234 装飾フレーム管理（ボス用）
	64,	
	64,	
	64,	
	64,	
	64,	

// 300
	// LocalEventBirth
	64,		// 大岩(落下)
	64,		// カプセル（本体）
	64,		// スクロール制限設置（プログラム呼び出し用）
	64,		// スクロール制限解除（プログラム呼び出し用）
	64,		// SPLリング
	64,		// 排液装置（噴出す水）
	256,	// 大岩を支える装置
	64,		// 大岩（追跡）
	256,	// テルスターニードル
	256,	// ウニデスニードル

// 300
	256,	// ウニウニニードル
	256,	// カマキラーの左手
	256,	// カマキラーの右手
	64,		// ステージ1ボス ボス1本体
	64,		// ステージ1ボス 鎖パーツ
	64,		// ステージ1ボス エッグマン
	64,		// ボス2本体
	64,		// ボス2エッグマン
	64,		// ボス2トゲボール
	64,		// ボス4本体
// 310
	64,		// ボス4エッグマン
	64,		// ボス4カプセル1段階目
	64,		// ボス4カプセル2段階目
	64,		// ちびエッグマン
	64,		// ちびエッグマン２段階目
	64,		// ちびエッグマン２段階目(SPEED)
	64,		// ちびエッグマン２段階目(BIG)
	64,		// ちびエッグマン２段階目(IRON)
	64,		// ボス5本体
	64,		// ボス5中心オブジェクト
// 320
	64,		// ボス5ロケット
	64,		// ボス5砲塔
	64,		// ボス5エッグマン

	64,		// 後ろ側トゲ
	64,		// トゲ台座
	64,		// ネオン針（針部）
	64,		// ネオン針（グレア部）
	64,		// ボス3柱
	64,		// ボス3柱
	64,		// ボス3柱
// 330
	64,		// ボス3柱
	64,		// ボス3壁
	64,		// ボス5足場
	64,		// ボス5カタパルト
};


/// 装飾物サイズテーブル
u16  g_gm_decorate_size_tbl[GMD_DECORATE_ID_MAX] = {
	64,		// zone3 壁
	64,		// zone3 壁
	64,		// zone3 壁
	64,		// zone3 壁
	128,	// zone3 壁
	128,	// zone3 壁
	
	64,		// zone3 ウォータースライダー左
	64,		// zone3 ウォータースライダー左
	64,		// zone3 ウォータースライダー左
	64,		// zone3 ウォータースライダー左
	64,		// zone3 ウォータースライダー右
	64,		// zone3 ウォータースライダー右
	64,		// zone3 ウォータースライダー右
	64,		// zone3 ウォータースライダー右

	64,		// グレア 
	64,		// グレア
	64,		// グレア
	
	64,		// zone1 ひまわり
	64,		// zone1 壁
	64,		// zone1 壁

	64,		// zone1 滝

	64,			//アシハナ
	64,			//アシハナ
	64,			//アシハナ
	64,			//花
	64,			//花
	64,			//木

	64,		// zone1 奥の滝

	64,			// zone1 壁
	64,			// zone1 壁
	64,			// zone1 壁
	64,			// zone1 壁
	64,			// zone1 壁
	
	64,			// zone1 壁（奥）
	64,			// zone1 壁（奥）
	64,			// zone1 壁（奥）
	64,			// zone1 壁（奥）
	64,			// zone1 壁（奥）
	64,			// zone1 壁（奥）
	64,			// zone1 壁（奥）
	
	64,				// zone1 滝
	64,				// zone1 滝
	64,				// zone1 滝
	64,				// zone1 滝
	64,				// zone1 滝
	64,				// zone1 滝
	64,				// zone1 滝
	
	64,				// zone1 滝（奥）
	64,				// zone1 滝（奥）
	64,				// zone1 滝（奥）
	64,				// zone1 滝（奥）
	64,				// zone1 滝（奥）
	64,				// zone1 滝（奥）
	64,				// zone1 滝（奥）

	64,		// グレア

	256,	//排液装置（左）
	128,	//排液装置出口（左）
	256,	//排液装置（右）
	128,	//排液装置出口（右）

	64,			//ZONE3岩
	64,			//ZONE3岩
	64,			//ZONE3岩
	64,			//ZONE3岩
	64,			//ZONE3岩
	64,			//ZONE3岩
	64,			//ZONE3岩

	64,			//ZONE3 ウォータースライダー用エフェクト水面下
	64,			//ZONE3 ウォータースライダー用エフェクト水飛沫
	64,			//ZONE3 蝋燭
	64,			//ZONE3 壁の顔

	64,			//ZONE3噴出す水
	64,			//ZONE3噴出す水
	64,			//ZONE3噴出す水
	64,			//ZONE3噴出す水

	64,				//ZONE3植物
	64,				//ZONE3植物
	64,				//ZONE3植物
	64,				//ZONE3植物
	64,				//ZONE3植物
	64,				//ZONE3植物

	64,	//ZONE3岩（前）
	64,	//ZONE3岩（前）
	64,	//ZONE3岩（前）
	64,	//ZONE3岩（前）
	64,	//ZONE3岩（前）
	64,		//ZONE3植物（前）
	64,		//ZONE3植物（前）
	64,		//壁（前）
	64,		//壁（前）

	64,		//ZONE3レール角
	64,		//ZONE3レール角
	64,		//ZONE3レール角（左右反転）
	64,		//ZONE3レール角（左右反転）

	64,			//ZONE4 歯車用
	64,			//ZONE4 歯車用
	64,			//ZONE4 歯車用
	64,			//ZONE4 歯車用
	64,			//ZONE4 歯車用
	64,			//ZONE4 歯車用

	64,			//ZONE1滝上部用エフェクト
	64,			//ZONE3 蝋燭
	
	64,			//ZONE3 ウォータースライダー排水溝
	64,			//ZONE3 ウォータースライダー排水溝（左右反転）
	64,			//ZONE3 大岩レール
	64,			//ZONE3 大岩レール
	64,			//ZONE3 大岩レール
	64,			//ZONE3 大岩レール（左右反転）
	64,			//ZONE3 大岩レール（左右反転）
	64,			//ZONE3 大岩レール（左右反転）
	64,			// グレア
	64,			// グレア
	64,			// zone1 ひまわり
	64,			//花
	64,			//花
	64,			//木
	
	64,			//ZONE4歯車用柱
	64,			//ZONE4歯車用柱
	64,			//ZONE4歯車用柱
	64,			//ZONE4歯車用柱（上下反転）
	64,			//ZONE4歯車用柱（上下反転）
	64,			//ZONE4歯車用柱（上下反転）
	64,			//ZONE4歯車用柱
	64,			//ZONE4歯車用柱
	64,			//ZONE4歯車用柱
	64,			//ZONE4歯車用柱（左右反転）
	64,			//ZONE4歯車用柱（左右反転）
	64,			//ZONE4歯車用柱（左右反転）

	64,			//ZONE4 歯車用（前）
	64,			//ZONE4 歯車用（前）
	64,			//ZONE4 歯車用（前）
	64,			//ZONE4 歯車用（前）
	64,			//ZONE4 歯車用（前）
	64,			//ZONE4 歯車用（前）
	
	64,				//ZONE4 柱
	128,			//ZONE4 柱
	128,			//ZONE4 柱
	64,				//ZONE4 柱
	128,			//ZONE4 柱
	64,				//ZONE4 柱
	128,			//ZONE4 柱
	
	64,			//ZONE4 柱

	64,				//ZONE4 パトランプ
	64,				//ZONE4 パトランプ

	64,					//ZONE4 柱
	64,					//ZONE4 柱
	64,					//ZONE4 柱
	64,					//ZONE4 柱
	64,					//ZONE4 柱
	64,					//ZONE4 柱
	64,					//ZONE4 柱	
	64,			//ZONE4 ポップスチーム角
	64,			//ZONE4 ポップスチーム角
	64,			//ZONE4 ポップスチーム角
	64,			//ZONE4 ポップスチーム角
	64,		//ZONE4 ポップスチーム角
	64,		//ZONE4 ポップスチーム角
	64,		//ZONE4 ポップスチーム角
	64,		//ZONE4 ポップスチーム角
	64,		//ZONE4 ポップスチーム角
	64,		//ZONE4 ポップスチーム角
	64,		//ZONE4 ポップスチーム角
	64,		//ZONE4 ポップスチーム角
	64,		//ZONE4 ポップスチームチューブ
	64,		//ZONE4 ポップスチームチューブ
	64,		//ZONE4 ポップスチームチューブ
	64,		//ZONE4 ポップスチームチューブ
	64,		//ZONE4 ポップスチームチューブ
	64,		//ZONE4 ポップスチームチューブ

	256,		//ZONE4 浮島用レール
	256,		//ZONE4 浮島用レール

	256,		//ZONEF シャッター
	256,		//ZONEF シャッター

	256,		//ZONEF シャッター
	256,		//ZONEF シャッター
	256,		//ZONEF シャッター
	256,		//ZONEF シャッター

	256,		//ZONEF シャッター
	256,		//ZONEF シャッター
	256,		//ZONEF シャッター
	256,		//ZONEF シャッター

	64,		//ZONE2 植物
	64,		//ZONE2 植物

	64,	//ZONEF ファイナルボス用ライト

	256,		//ZONEF シャッター
};

//----- Local Variables -----------------------------------------------------

//----- Global Functions ----------------------------------------------------
// ==========================================================================
// pt
/*!
 */
// ==========================================================================

//----- Local Functions -----------------------------------------------------



// ==========================================================================
// _pt
/*!
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
