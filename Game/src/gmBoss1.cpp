// =======================================================================
/*!
  @file	gmBoss1.cpp
  @brief ボス1

  @author Keisuke Tanaka
 				Copyright(c) 2009 Dimps
  $Id: gmBoss1.cpp 2 2011-04-11 05:21:26Z thamada $
 */
// =======================================================================
/*
 * $Log$
 */

/*------ Include Files -------------------------------------------------*/
#include "pch.h"
// TODO : 暫定対応 ↓↓↓↓↓↓↓↓
#if _WII
#pragma optimization_level 0
#endif //_WII
// TODO : 暫定対応 ↑↑↑↑↑↑↑↑
#include "akMath.h"
#include "gs.h"
#include "gmMain.h"
#include "gmGameDBuild.h"
#include "gmGamedat.h"
#include "gmEventTbl.h"
#include "gmCamera.h"
#include "gmSound.h"
#include "gmPadVib.h"
#include "gmEnemy.h"
#include "gmEffect.h"
#include "gmEffectCmn.h"
#include "gmEffectBoss.h"
#include "gmBossCommon.h"
#include "gmBoss1.h"

#include "gmPlySeq.h"
#include "gmPlyScoreDef.h"

#include "gmGmkCamScrLim.h"

#include "hgTrophy.h"

#if _IPHONE
#include "gmMap.h"
#endif // _IPHONE

// データヘッダ
#include "../file/common/arc/BOSS01.hmb"
#include "../file/common/model/BOSS01_MDL.hmb"
#include "../file/common/model/BOSS01_BODY_MTN.hmb"
#include "../file/common/model/BOSS01_CHAIN_MTN.hmb"
#include "../file/common/model/BOSS01_EGG_MTN.hmb"

/*------ Macros --------------------------------------------------------*/

/* ファイル関連 */
#define GMD_BOSS1_ARC	(g_gm_gamedat_enemy_arc)

/* 機能切り替えマクロ */
#define GMD_BOSS1_AKTNML_MOVE_USE_PARTIAL_CURVE		//!< 定義していると、通常攻撃左右移動の際に円の部分曲線で補間する

//############ ノード番号 #####################################################
// 本体
#define GMD_BOSS1_BODY_NODE_IDX_CHAIN_CONNECT	(13)	//!< 鎖接続ノード
#define GMD_BOSS1_BODY_NODE_IDX_EGG_CONNECT		(11)	//!< エッグマン接続ノード
#define GMD_BOSS1_BODY_NODE_IDX_BODY_POSTURE	(2)		//!< 本体姿勢
#define GMD_BOSS1_BODY_NODE_IDX_CHAIN_ROOT_PART	(9)		//!< 鎖の根元にある本体側のパーツ

#define GMD_BOSS1_BODY_NODE_SNM_NUM				(4)		//!< SNM登録ノード数
#define GMD_BOSS1_BODY_NODE_CNM_NUM				(1)		//!< CNM登録ノード数

// 鎖
#define GMD_BOSS1_CHAIN_NODE_IDX_BALL			(11)	//!< 鉄球部分
#define GMD_BOSS1_CHAIN_NODE_IDX_RING_START		(2)		//!< 最初のリングパーツ

#define GMD_BOSS1_CHAIN_NODE_SNM_NUM			(10)	//!< SNM登録ノード数
#define GMD_BOSS1_CHAIN_NODE_CNM_NUM			(9)		//!< CNM登録ノード数

//############ 共通 ###########################################################
/* 定義値 */
#define GMD_BOSS1_LIFE						(8)					//!< ライフ値
#define GMD_BOSS1_FINAL_LIFE				(4)					//!< FINALゾーン用初期ライフ値
#define GMD_BOSS1_EXTRA_ATK_THRESHOLD_LIFE	(3)					//!< このライフ値以下になったら追加攻撃開始
#define GMD_BOSS1_DEFAULT_POS_Z				(GMD_OBJ_DEFAULT_POS_Z_C)	//!< デフォルトZ位置

#define GMD_BOSS1_RIGHTWARD_ANGLE			(AKM_DEGtoA16(60.f))	//!< 右向き時の角度（正面方向基準）
#define GMD_BOSS1_LEFTWARD_ANGLE			(AKM_DEGtoA16(300.f))	//!< 左向き時の角度（正面方向基準）

#define GMM_BOSS1_STAGE_MAP_POS_OFST_Y()	((fx32)((GMM_MAIN_GET_ZONE_TYPE() == GSD_MAIN_ZONE_TYPE_FINAL) ? ((fx32)(FX32_ONE * 3456)): ((fx32)(0))))	//!< ステージ別Yオフセット

#define GMD_BOSS1_GROUND_POS_Y				((fx32)(FX32_ONE * 314) + GMM_BOSS1_STAGE_MAP_POS_OFST_Y())	//!< 地面の高さ

#define GMD_BOSS1_SCATTER_PARTS_NUM			(GMD_BOSS1_CHAIN_NODE_CNM_NUM)	//!< 爆発時の飛び散るパーツ数

#define GMD_BOSS1_DEFAULT_BLEND_SPD			((Float)0.125f)		//!< ボス１のデフォルトモーションブレンド速度

//############ 管理 ###########################################################
/* フラグ GMS_BOSS1_MGR_WORK::flag */
#define GMD_BOSS1_MGR_FLAG_SETUP_END			(1 << 0)	//!< 生成完了フラグ
#define GMD_BOSS1_MGR_FLAG_CLEAR_BOSS			(1 << 1)	//!< ボスを消去する

/* 定義値 */

//############ 本体 ###########################################################
/* フラグ GMS_BOSS1_BODY_WORK::flag */
#define GMD_BOSS1_BODY_FLAG_CHAIN_DEPEND		(1 << 0)	//!< 鎖独立動作フラグ
#define GMD_BOSS1_BODY_FLAG_INDP_ACT_FORBIDDEN	(1 << 1)	//!< エッグマン独立アクション禁止フラグ
#define GMD_BOSS1_BODY_FLAG_INVINCIBLE			(1 << 2)	//!< 無敵状態（当たりはあるが、ライフは減らない）
#define GMD_BOSS1_BODY_FLAG_CHAIN_DEAD			(1 << 3)	//!< 死亡フラグ（ライフが無い状態）
#define GMD_BOSS1_BODY_FLAG_ABURNER_ACTIVE		(1 << 4)	//!< アフターバーナー有効フラグ
#define GMD_BOSS1_BODY_FLAG_CHAIN_NODISP		(1 << 5)	//!< 鎖非表示
#define GMD_BOSS1_BODY_FLAG_CHAIN_NOHIT			(1 << 6)	//!< 鎖攻撃矩形オフ
#define GMD_BOSS1_BODY_FLAG_MANUAL_CHAIN_MOTION	(1 << 7)	//!< 手動鎖モーション（OBD_DISP_STOPのオフへの変更を抑制する）
#define GMD_BOSS1_BODY_FLAG_EFF_DEBRIS_CREATED	(1 << 8)	//!< 破片エフェクト生成済みフラグ（オンなら生成しない）

// シグナル系（例：B2C=bodyが立ててchainがチェックするフラグ。F=eFfect）
#define GMD_BOSS1_BODY_FLAG_SIGNAL_B2E_EGG_ESCAPE	(1 << 23)	//!< 逃亡通知
#define GMD_BOSS1_BODY_FLAG_SIGNAL_B2E_EGG_BURNT	(1 << 24)	//!< 黒こげテクスチャへの変更通知
#define GMD_BOSS1_BODY_FLAG_SIGNAL_B2F_ABURNER		(1 << 25)	//!< アフターバーナー生成通知フラグ
#define GMD_BOSS1_BODY_FLAG_SIGNAL_B2C_SCATTER		(1 << 26)	//!< パーツ飛散通知フラグ
#define GMD_BOSS1_BODY_FLAG_SIGNAL_B2C_EFF_SW		(1 << 27)	//!< エフェクト発生通知フラグ
#define GMD_BOSS1_BODY_FLAG_SIGNAL_C2E_ATK_HIT		(1 << 28)	//!< ヒット通知フラグ
#define GMD_BOSS1_BODY_FLAG_SIGNAL_B2E_EGG_DAMAGE	(1 << 29)	//!< body->eggダメージ通知フラグ
#define GMD_BOSS1_BODY_FLAG_SIGNAL_B2B_BODY_DAMAGE	(1 << 30)	//!< body->bodyダメージ通知フラグ
#define GMD_BOSS1_BODY_FLAG_SIGNAL_B2B_DEFEAT		(1 << 31)	//!< 死亡演出開始通知フラグ

/* 定義値 */
// 共通
#define GMD_BOSS1_BODY_HIDE_RADIUS_V		((fx32)(FX32_ONE * 0))	//!< この距離分、画面上端から離れたら完全に画面外に消えたと判定する
#define GMD_BOSS1_BODY_HIDE_RADIUS_H		((fx32)(FX32_ONE * 64))	//!< この距離分、画面右端から離れたら完全に画面外に消えたと判定する
#define GMD_BOSS1_BODY_DMG_FLICKER_RADIUS	((Float)32.f)			//!< ダメージ点滅処理用モデル半径値
#define GMD_BOSS1_BODY_DMG_NO_HIT_TIME		(10)					//!< ダメージ時ヒット無効時間
#define GMD_BOSS1_BODY_DEFAULT_ALTITUDE		((fx32)(FX32_ONE * 174) + GMM_BOSS1_STAGE_MAP_POS_OFST_Y())	//!< 基本高度
// 開始動作関連
#define GMD_BOSS1_BODY_START_POS_Y			((fx32)(FX32_ONE * -60) + GMM_BOSS1_STAGE_MAP_POS_OFST_Y())	//!< 出現初期位置
#define GMD_BOSS1_BODY_START_MOVE_DEST_AREA_X	((fx32)(FX32_ONE * 192))	//!< 開始演出横移動目標地点X（スクロールロック画面内での座標）
#define GMD_BOSS1_BODY_START_WAIT_END_TIME	(10)					//!< 目標位置に到達後、待機する時間
#define GMD_BOSS1_BODY_START_MOVE_FALL_SPD	((fx32)(FX32_ONE * 1))	//!< 登場時下方向移動速度
#define GMD_BOSS1_BODY_START_MOVE_SIDE_SPD	((fx32)(FX32_ONE * -1))	//!< 登場時左方向移動速度
// 鉄球準備関連
#define GMD_BOSS1_BODY_PREP_CHAIN_ACTIVE_TIMING_FRAME	(95)		//!< 鉄球準備動作に入ってからこのフレーム経過後に鉄球の当たりを有効にする
// 通常攻撃「開始」関連
#define GMD_BOSS1_BODY_PRE_ATKNML_SPD_ADD		((fx32)(FX32_ONE * 0.02f))
#define GMD_BOSS1_BODY_PRE_ATKNML_SPD_MAX_ABS	((fx32)(FX32_ONE * 1.8f))
#define GMD_BOSS1_BODY_PRE_ATKNML_CHAIN_INI_MTN_FRAME	(160.f)				//!< 通常攻撃開始時 鎖 初期モーションフレーム
// 通常攻撃関連
#define GMD_BOSS1_BODY_ATKNML_LEFT_LIMIT	((fx32)(FX32_ONE * 144))	//!< 通常攻撃移動範囲左端
#define GMD_BOSS1_BODY_ATKNML_RIGHT_LIMIT	((fx32)(FX32_ONE * 240))	//!< 通常攻撃移動範囲右端

#define GMD_BOSS1_BODY_ATKNML_CHAIN_INI_BLD_SPD	((Float)0.01f)			//!< 通常攻撃 鎖 初回ブレンド速度

#define GMD_BOSS1_BODY_ATKNML_CHAIN_MTN_SPD	((Float)1.25f)		//!< 以前は1.275。80を割ったときにキリがいいので1.25fに変更
#define GMD_BOSS1_BODY_ATKNML_MOVE_FRAME	((Sint32)(70 / GMD_BOSS1_BODY_ATKNML_CHAIN_MTN_SPD))	//!< 左右移動所要フレーム
#define GMD_BOSS1_BODY_ATKNML_DRIFT_FRAME	((Sint32)(90 / GMD_BOSS1_BODY_ATKNML_CHAIN_MTN_SPD))	//!< ドリフト移動所要フレーム
#define GMD_BOSS1_BODY_ATKNML_DRIFT_AMP		((fx32)(FX32_ONE * 32))	//!< ドリフト移動幅

#if defined(GMD_BOSS1_AKTNML_MOVE_USE_PARTIAL_CURVE)
	#define	GMD_BOSS1_BODY_ATKNML_MOVE_CURVE_ANGLE_WIDTH	(AKM_DEGtoA32(120.f))
	#define GMD_BOSS1_BODY_ATKNML_MOVE_CURVE_START_ANGLE	(AKM_DEGtoA32(30.f))
#endif /* defined(GMD_BOSS1_AKTNML_MOVE_USE_PARTIAL_CURVE) */

#define GMD_BOSS1_BODY_ATKNML_TURN_FRAME	(40)			//!< 通常攻撃時振り向き所要時間

// 叩きつけ攻撃関連
#define GMD_BOSS1_BODY_ATKBASH_HEIGHT		((fx32)(FX32_ONE * 88))	//!< 鉄球設置面から本体中心までの高さ
#define GMD_BOSS1_BODY_ATKBASH_TARG_X_LEFT	((fx32)(FX32_ONE * 176))	//!< 叩きつけ攻撃時の本体移動目標位置X（左向き）
#define GMD_BOSS1_BODY_ATKBASH_TARG_X_RIGHT	((fx32)(FX32_ONE * 208))	//!< 叩きつけ攻撃時の本体移動目標位置X（右向き）
#define GMD_BOSS1_BODY_ATKBASH_TARG_Y		((fx32)(GMD_BOSS1_GROUND_POS_Y - GMD_BOSS1_BODY_ATKBASH_HEIGHT))	//!< 叩きつけ攻撃時の本体移動目標位置Y
#define GMD_BOSS1_BODY_ATKBASH_MTN_FRAME	((Sint32)(39))		//!< 叩きつけ攻撃モーションフレーム数
#define GMD_BOSS1_BODY_ATKBASH_SPD_ADD_FACTOR	((Float)0.03125f)		//!< （距離／モーションフレーム数）に対してこの係数を掛けた結果を加速度にする

#define GMD_BOSS1_BODY_ATKBASH_HOMEPOS_X_LEFT	((fx32)(FX32_ONE * 128))	//!< 叩きつけの戻り位置（左側）
#define GMD_BOSS1_BODY_ATKBASH_HOMEPOS_X_RIGHT	((fx32)(FX32_ONE * 256))	//!< 叩きつけの戻り位置（右側）

#define GMD_BOSS1_BODY_ATKBASH_QUAKE_VAL_Y		((fx32)16 << FX32_SHIFT)	//!< 画面振動の大きさ

#define GMD_BOSS1_BODY_ATBASH_RETURN_TIME	(60)		// 叩きつけた後、元の高度に戻る移動処理に掛ける時間（フレーム）

// 撃破関係
#define GMD_BOSS1_BODY_DEFEAT_WAIT_START_TIME	(40)	// 撃破開始待機時間
#define GMD_BOSS1_BODY_DEFEAT_BOMB_CREATE_WIDTH		((fx32)(FX32_ONE * 80))
#define GMD_BOSS1_BODY_DEFEAT_BOMB_CREATE_HEIGHT	((fx32)(FX32_ONE * 80))
#define GMD_BOSS1_BODY_DEFEAT_BOMB_CREATE_INTERVAL_MIN	(10)	//!< 爆発エフェクト生成間隔最短時間
#define GMD_BOSS1_BODY_DEFEAT_BOMB_CREATE_INTERVAL_MAX	(10)	//!< 爆発エフェクト生成間隔最長時間
// ↑min=max=10は意図的に指定しています
#define GMD_BOSS1_BODY_DEFEAT_BOMB_CREATE_TIME			(120)	//!< 爆発生成時間
#define GMD_BOSS1_BODY_DEFEAT_BURNT_WAIT_TIME			(40)	//!< 黒こげ設定までの待ち時間

// 逃亡関連
#define GMD_BOSS1_BODY_ESCAPE_SCR_UNLOCK_X_FROM_RIGHT	((fx32)(FX32_ONE * -32.f))	//!< ボスがこのラインを通過したらスクロールロック解除（右端基準の座標）
#define GMD_BOSS1_BODY_ESCAPE_OUT_FINAL_ZONE_X_FROM_RIGHT	((fx32)(FX32_ONE * 96))	//!< ファイナルゾーンにおいてボスがこのラインを通過したら画面から退出したと判定（右端基準の座標）
#define GMD_BOSS1_BODY_ESCAPE_SPD_X_ADD		((fx32)(FX32_ONE * 0.1f))
#define GMD_BOSS1_BODY_ESCAPE_SPD_Y_ADD		((fx32)(FX32_ONE * 2.5f * -0.025f))
#define GMD_BOSS1_BODY_ESCAPE_SPD_X_MAX		((fx32)(FX32_ONE * 1.2f))
#define GMD_BOSS1_BODY_ESCAPE_SPD_Y_MAX		((fx32)(FX32_ONE * 2.5f * -0.3f))

// 跳ね返りパラメータ
#define GMD_BOSS1_BODY_PLY_NML_REBOUND_X					((fx32)(FX32_ONE * 4))
#define GMD_BOSS1_BODY_PLY_NML_REBOUND_Y					((fx32)(FX32_ONE * 3))
#define GMD_BOSS1_BODY_PLY_NML_REBOUND_NOJUMPMOVE_TIME		((fx32)(FX32_ONE * 25))
#define GMD_BOSS1_BODY_PLY_HOMING_REBOUND_X					((fx32)(FX32_ONE * 5))
#define GMD_BOSS1_BODY_PLY_HOMING_REBOUND_Y					((fx32)(FX32_ONE * 4))
#define GMD_BOSS1_BODY_PLY_HOMING_REBOUND_NOJUMPMOVE_TIME	((fx32)(FX32_ONE * 25))

//! 本体通常攻撃用食らい当たり矩形サイズ
#define GMD_BOSS1_BODY_ATKNML_DMG_RECT_SIZE_LEFT	(-24)
#define GMD_BOSS1_BODY_ATKNML_DMG_RECT_SIZE_TOP		(-24)
#define GMD_BOSS1_BODY_ATKNML_DMG_RECT_SIZE_RIGHT	(24)
#define GMD_BOSS1_BODY_ATKNML_DMG_RECT_SIZE_BOTTOM	(16)
//! 本体デフォルトくらい当たり矩形
#define GMD_BOSS1_BODY_DMG_RECT_SIZE_LEFT	(-24)
#define GMD_BOSS1_BODY_DMG_RECT_SIZE_TOP	(-24)
#define GMD_BOSS1_BODY_DMG_RECT_SIZE_RIGHT	(24)
#define GMD_BOSS1_BODY_DMG_RECT_SIZE_BOTTOM	(24)
//! 本体攻撃当たり矩形サイズ
#define GMD_BOSS1_BODY_ATK_RECT_SIZE_LEFT	(-16)
#define GMD_BOSS1_BODY_ATK_RECT_SIZE_TOP	(-16)
#define GMD_BOSS1_BODY_ATK_RECT_SIZE_RIGHT	(16)
#define GMD_BOSS1_BODY_ATK_RECT_SIZE_BOTTOM	(16)

//############ 鎖 #############################################################
/* フラグ */
//! 手動モーションマージ（遷移元モーションを再生しつつブレンドを行います。マージ終了後に自動でフラグオフになります）
#define GMD_BOSS1_CHAIN_FLAG_MANUAL_MOTION_MERGE	(1 << 0)
/* 定義値 */

// 鎖攻撃当たり矩形サイズ
#define GMD_BOSS1_CHAIN_ATK_RECT_SIZE_LEFT		(-16)
#define GMD_BOSS1_CHAIN_ATK_RECT_SIZE_TOP		(-16)
#define GMD_BOSS1_CHAIN_ATK_RECT_SIZE_RIGHT		(16)
#define GMD_BOSS1_CHAIN_ATK_RECT_SIZE_BOTTOM	(16)

//############ エッグマン #####################################################
/* フラグ */
#define GMD_BOSS1_EGG_FLAG_INDP_ACT_SET		(1 << 0)	//!< 独立アクション設定中フラグ
#define GMD_BOSS1_EGG_FLAG_SWEAT_ACTIVE		(1 << 1)	//!< 汗エフェクト有効中フラグ

/* 定義値 */

//############ 衝撃波エフェクト ###############################################
/* フラグ */
/* 定義値 */
#define GMD_BOSS1_EFF_SHOCKWAVE_SUB_NUM				(3)							//!< 衝撃波パーツの数
#define GMD_BOSS1_EFF_SHOCKWAVE_SUB_START_OFST_X	((fx32)(FX32_ONE * 40.f))	//!< エフェクト中心（親）からのオフセット
#define GMD_BOSS1_EFF_SHOCKWAVE_SUB_OFST_Y			((fx32)(FX32_ONE * -4.f))	//!< 親からのオフセットY
#define GMD_BOSS1_EFF_SHOCKWAVE_SUB_ROT_X			(AKM_DEGtoA32(-20.f))		//!< 衝撃波パーツの傾き
#define GMD_BOSS1_EFF_SHOCKWAVE_ATK_RECT_TIME		(10)						//!< 衝撃波攻撃当たり矩形有効時間
// 攻撃矩形サイズ
#define GMD_BOSS1_EFF_SHOCKWAVE_ATK_RECT_SIZE_LEFT		(-64)
#define GMD_BOSS1_EFF_SHOCKWAVE_ATK_RECT_SIZE_TOP		(-32)
#define GMD_BOSS1_EFF_SHOCKWAVE_ATK_RECT_SIZE_RIGHT		(64)
#define GMD_BOSS1_EFF_SHOCKWAVE_ATK_RECT_SIZE_BOTTOM	(32)

//############ パーツ飛散エフェクト ###########################################
/* フラグ */
/* 定義値 */
#define GMD_BOSS1_EFF_SCT_SPIN_AXIS_NUM		(2)		//!< 飛散パーツを回転させる軸の数

#define GMD_BOSS1_EFF_SCT_PART_RING_OFST_Y	((Float)8.f)	//!< リングパーツのNDCオフセットY
#define GMD_BOSS1_EFF_SCT_PART_IBALL_OFST_Y	((Float)32.f)	//!< 鉄球パーツのNDCオフセットY

#define GMD_BOSS1_EFF_SCT_PART_RING_SPIN_SPD_DEG	(AKM_DEGtoA32(20.f))	//!< リングパーツの回転角速度
#define GMD_BOSS1_EFF_SCT_PART_IBALL_SPIN_SPD_DEG	(AKM_DEGtoA32(5.f))	//!< 鉄球パーツの回転角速度

#define GMD_BOSS1_EFF_SCT_PART_RING_FLY_SPD			((Float)5.f)	//!< パーツ飛散速度
#define GMD_BOSS1_EFF_SCT_PART_IBALL_FLY_SPD		((Float)3.f)

#define GMD_BOSS1_EFF_SCT_PART_FLY_DELAY_MAX		(20)	//!< パーツ飛散遅延時間最大値（パーツが生成されてから飛散開始するまでの最大待機時間）
#define GMD_BOSS1_EFF_SCT_PART_FLY_DELETE_TIME		(180)	//!< 消去タイマ時間

// リングパーツ地形当たり矩形サイズ
#define GMD_BOSS1_EFF_SCT_PART_RING_FRECT_LEFT		(-8)
#define GMD_BOSS1_EFF_SCT_PART_RING_FRECT_TOP		(-8)
#define GMD_BOSS1_EFF_SCT_PART_RING_FRECT_RIGHT		(8)
#define GMD_BOSS1_EFF_SCT_PART_RING_FRECT_BOTTOM	(8)
// 鉄球パーツ地形当たり矩形サイズ
#define GMD_BOSS1_EFF_SCT_PART_IBALL_FRECT_LEFT		(-24)
#define GMD_BOSS1_EFF_SCT_PART_IBALL_FRECT_TOP		(-24)
#define GMD_BOSS1_EFF_SCT_PART_IBALL_FRECT_RIGHT	(24)
#define GMD_BOSS1_EFF_SCT_PART_IBALL_FRECT_BOTTOM	(24)

//############ 爆発エフェクト #################################################
/* フラグ */
/* 定義値 */
#define GMD_BOSS1_EFF_BOMB_OFST_Z					((fx32)(FX32_ONE * 32))	//!< 爆発エフェクト表示座標の親からのオフセット座標Z（ボスより手前に表示させるため一律にずらす）

//############ ダメージエフェクト #############################################
/* 定義値 */
#define GMD_BOSS1_EFF_DAMAGE_OFST_Z					((fx32)(FX32_ONE * 32))	//!< ダメージエフェクト表示座標の親からのオフセット（ボスより手前に表示させるため）

//############ アフターバーナーエフェクト #####################################
/* 定義値 */
#define GMD_BOSS1_EFF_ABURNER_DISP_OFST_Z			((Float)-30.f)			//!< 表示オフセットZ

//############ アフターバーナー煙エフェクト ###################################
/* 定義値 */
#define GMD_BOSS1_EFF_ABSMOKE_DISP_OFST_Z			((Float)-32.f)			//!< 表示オフセットZ

//############ 本体煙エフェクト ###############################################
/* 定義値 */
#define GMD_BOSS1_EFF_BODYSMOKE_DISP_OFST_Z			((Float)-32.f)			//!< 表示オフセットZ

//############ 本体小煙エフェクト #############################################
/* 定義値 */
#define GMD_BOSS1_EFF_SMALL_SMOKE_NUM				(3)						//!< 小煙発生数

//############ 破片エフェクト #################################################
/* 定義値 */
#define GMD_BOSS1_EFF_DEBRIS_PARENT_OFST_X			((fx32)(FX32_ONE * -16))	//!< 破片エフェクトを本体より後ろ側（マップ座標）に表示させるためのオフセットX

//############ 汗エフェクト ###################################################
/* 定義値 */
#define GMD_BOSS1_EFF_SWEAT_DISP_OFST_Y				((Float)32.f)			//!< 表示オフセットY

//############ 画面フラッシュ #################################################
/* 定義値 */
#define GMD_BOSS1_FLASH_SCREEN_FADEOUT_TIME			(4)		//!< 撃破のフラッシュ完全に白になるまでのフレーム
#define GMD_BOSS1_FLASH_SCREEN_DURATION_TIME		(5)		//!< 撃破のフラッシュ完全に白の間のフレーム
#define GMD_BOSS1_FLASH_SCREEN_FADEIN_TIME			(30)	//!< 撃破のフラッシュ完全に白から戻るまでのフレーム


//############ SE関連 #########################################################
/* 巻き取りSE */
// 共通
#define GMD_BOSS1_SE_REEL_INTERVAL_TIME	(15)
// 叩きつけ準備時
#define GMD_BOSS1_SE_ATK_BASH_PREP_REEL_START_WAIT_TIME	(110)
#define GMD_BOSS1_SE_ATK_BASH_PREP_REEL_PLAY_NUM		(3)
// 叩きつけ後
#define GMD_BOSS1_SE_ATK_BASH_FIN_REEL_START_WAIT_TIME	(70)	//!< 叩きつけ後 巻き取りSE再生開始待ち時間
#define GMD_BOSS1_SE_ATK_BASH_FIN_REEL_PLAY_NUM			(2)		//!< 叩きつけ後 巻き取りSE連続再生回数
#define GMD_BOSS1_SE_ATK_BASH_FIN_REEL_INTERVAL_TIME	(15)	//!< 叩きつけ後 巻き取りSE連続再生間隔

/*------ Macro Functions -----------------------------------------------*/
// =======================================================================
// GMM_BOSS1_MGR
/*!
  ボス1本体ワークから管理ワークを取り出す
  
  @param work	[in]	ボス１本体ワーク
  
  @return 管理ワーク(GMS_BOSS1_MGR_WORK)
 */
// =======================================================================
#define GMM_BOSS1_MGR(work)	((work)->mgr_work)

// =======================================================================
// GMM_BOSS1_AREA_***
/*!
  スクロール可能範囲矩形の座標を取得
  
  @return スクロール可能範囲矩形座標（マップ座標系, 固定小数）
  
  @note
  スクロールロックされた際の上下左右端の座標を得るのに使用します。
 */
// =======================================================================
#define GMM_BOSS1_AREA_LEFT()	((fx32)(g_gm_main_system.map_fcol.left << FX32_SHIFT))
#define GMM_BOSS1_AREA_TOP()	((fx32)(g_gm_main_system.map_fcol.top << FX32_SHIFT))
#define GMM_BOSS1_AREA_RIGHT()	((fx32)(g_gm_main_system.map_fcol.right << FX32_SHIFT))
#define GMM_BOSS1_AREA_BOTTOM()	((fx32)(g_gm_main_system.map_fcol.bottom << FX32_SHIFT))
// 画面中心
#define GMM_BOSS1_AREA_CENTER_X()	(GMM_BOSS1_AREA_LEFT() + ((GMM_BOSS1_AREA_RIGHT() - GMM_BOSS1_AREA_LEFT()) / 2))
#define GMM_BOSS1_AREA_CENTER_Y()	(GMM_BOSS1_AREA_TOP() + ((GMM_BOSS1_AREA_BOTTOM() - GMM_BOSS1_AREA_TOP()) / 2))

/*------ Definitions ---------------------------------------------------*/
//! ボス1 本体 ステート列挙型
typedef enum
{
	GME_BOSS1_BODY_STATE_NOP	= 0,	//!< 何もしない
	GME_BOSS1_BODY_STATE_START,			//!< 開始
	GME_BOSS1_BODY_STATE_PREP,			//!< 鉄球登場演出
	GME_BOSS1_BODY_STATE_PRE_ATK_NML,	//!< 通常攻撃開始処理
	GME_BOSS1_BODY_STATE_ATK_NML,		//!< 通常攻撃
	GME_BOSS1_BODY_STATE_ATK_BASH,		//!< 叩きつけ
	GME_BOSS1_BODY_STATE_DAMAGE_NML,	//!< 通常ダメージ
	GME_BOSS1_BODY_STATE_DEFEAT,		//!< 撃破
	GME_BOSS1_BODY_STATE_ESCAPE,		//!< 逃亡
	
	GME_BOSS1_BODY_STATE_MAX
} GME_BOSS1_BODY_STATE;


//! ボス1 パーツインデックス列挙型
typedef enum
{
	GME_BOSS1_PART_IDX_BODY	= 0,
	GME_BOSS1_PART_IDX_CHAIN,
	GME_BOSS1_PART_IDX_EGG,
	
	GME_BOSS1_PART_IDX_MAX
} GME_BOSS1_PART_IDX;

//! ボス1 アクションID列挙型
typedef enum
{
	GME_BOSS1_ACT_ID_APP_FALL	= 0,	//!< スタート時の降下
	GME_BOSS1_ACT_ID_APP_END,			//!< スタート時の降下後の余韻
	
	GME_BOSS1_ACT_ID_PREP_CHAIN,		//!< 鉄球登場演出
	
	GME_BOSS1_ACT_ID_PRE_ATK_NML_MOVE,	//!< 通常攻撃 開始移動
	GME_BOSS1_ACT_ID_ATK_NML_MOVE,		//!< 通常攻撃 移動中
	
	GME_BOSS1_ACT_ID_ATK_BASH_LOCK,		//!< 叩きつけ プレイヤーの方向を向く
	GME_BOSS1_ACT_ID_ATK_BASH_PREP,		//!< 叩きつけ 準備
	GME_BOSS1_ACT_ID_ATK_BASH_SWING,	//!< 叩きつけ 回転
	GME_BOSS1_ACT_ID_ATK_BASH_FINISH,	//!< 叩きつけ 余韻・鎖回収
	GME_BOSS1_ACT_ID_ATK_BASH_HOMING,	//!< 叩きつけ 定位置に戻る
	GME_BOSS1_ACT_ID_ATK_BASH_RESTORE,	//!< 叩きつけ 鎖を出す（通常攻撃につながる）
	
	GME_BOSS1_ACT_ID_DAMAGE_NML,		//!< 通常ダメージ
	
	GME_BOSS1_ACT_ID_ESCAPE,			//!< 逃亡
	
	GME_BOSS1_ACT_ID_MAX
} GME_BOSS1_ACT_ID;

//! ボス1 エッグマン独立アクションID列挙型
typedef enum
{
	GME_BOSS1_EGG_ACT_ID_LAUGH	= 0,
	GME_BOSS1_EGG_ACT_ID_DAMAGE,
	
	GME_BOSS1_EGG_ACT_ID_MAX
} GME_BOSS1_EGG_ACT_ID;


//! 爆発エフェクト タイプ列挙型
typedef enum
{
	GME_BOSS1_EFF_BOMB_TYPE_SMALL	= 0,	// 小
	
	GME_BOSS1_EFF_BOMB_TYPE_MAX
} GME_BOSS1_EFF_BOMB_TYPE;


typedef struct tag_GMS_BOSS1_MGR_WORK	GMS_BOSS1_MGR_WORK;
typedef struct tag_GMS_BOSS1_BODY_WORK	GMS_BOSS1_BODY_WORK;
typedef struct tag_GMS_BOSS1_CHAIN_WORK	GMS_BOSS1_CHAIN_WORK;
typedef struct tag_GMS_BOSS1_EGG_WORK	GMS_BOSS1_EGG_WORK;


//! 1ショットタイマワーク
typedef struct tag_GMS_BOSS1_1SHOT_TIMER
{
	Uint32	timer;
	BOOL	is_active;	//! 有効フラグ
} GMS_BOSS1_1SHOT_TIMER;

//! モーション再生停滞ワーク
typedef struct tag_GMS_BOSS1_MTN_SUSPEND_WORK
{
	BOOL	is_suspended;
	Uint32	suspend_timer;
} GMS_BOSS1_MTN_SUSPEND_WORK;

//! 爆発エフェクトワーク
typedef struct tag_GMS_BOSS1_EFF_BOMB_WORK
{
	OBS_OBJECT_WORK	*parent_obj;
	GME_BOSS1_EFF_BOMB_TYPE	bomb_type;
	Uint32	interval_timer;
	Uint32	interval_min;
	Uint32	interval_max;
	fx32	pos[MTD_XY];
	fx32	area[MTD_WH];
#if _IPHONE
	Sint32	interval_timer_sound;
#endif // _IPHONE
} GMS_BOSS1_EFF_BOMB_WORK;

//! ボス１管理ワーク
struct tag_GMS_BOSS1_MGR_WORK
{
	GMS_ENEMY_3D_WORK	ene_3d;
	
	Sint32				life;	// ボスライフ
	
	Uint32				flag;
	
	Sint32				obj_create_cnt;	//!< オブジェクト生成カウント
	
	GMS_BOSS1_BODY_WORK	*body_work;
};


//! ボス１本体ワーク
struct tag_GMS_BOSS1_BODY_WORK
{
	GMS_ENEMY_3D_WORK	ene_3d;
	
	GME_BOSS1_BODY_STATE	state;		//!< ステート
	GME_BOSS1_BODY_STATE	prev_state;	//!< 前のステート
	
	GMS_BOSS1_MGR_WORK		*mgr_work;	//!< 管理ワーク
	
	void	(*proc_update)(GMS_BOSS1_BODY_WORK*);	//!< 更新処理関数
	
	Uint32	flag;
	
	GME_BOSS1_ACT_ID		whole_act_id;	//!< ボス１全体アクションID
	
	Uint16					egg_revert_mtn_id;	//!< エッグマン独立アクションからの戻り先モーション
	Uint16					reserved[1];
	
	Uint32					wait_timer;			//!< 汎用待機タイマ
	
	GMS_BS_CMN_BMCB_MGR		bmcb_mgr;		//!< ボスモーションCB管理ワーク
	GMS_BS_CMN_SNM_WORK		snm_work;		//!< SNMワーク
	Sint32					chain_snm_reg_id;
	Sint32					egg_snm_reg_id;
	Sint32					body_snm_reg_id;	//!< 本体（アフターバーナーや煙などをくっつけるため）
	Sint32					chaintop_snm_reg_id;	//!< 鎖の付け根座標取得
	
	GMS_BS_CMN_CNM_MGR_WORK	cnm_mgr_work;	//!< CNM管理ワーク
	Sint32					chaintop_cnm_reg_id;	//!< 鎖の付け根マトリクス操作
	
	GMS_BS_CMN_DMG_FLICKER_WORK	flk_work;	//!< ダメージ点滅ワーク
	
	GMS_BOSS1_1SHOT_TIMER	se_timer;		//!< SE用1ショットタイマ
	Uint32					se_cnt;			//!< SE再生カウント（何回も再生するときなどに利用）
	
	Uint32					no_hit_timer;	//!< 喰らい無効タイマ
	
	Sint32					move_time;		//!< 通常攻撃移動 所要時間
	Sint32					move_cnt;		//!< 通常攻撃移動 フレームカウント
	
	Angle16					cur_angle;		//!< 現在の向き
	Angle16					orig_angle;		//!< 振り向き開始時角度
	Angle32					turn_angle;		//!< 振り向き時オフセット角度
	Angle32					turn_amount;	//!< 現在の角度から最終的に何度回転させるか
	Angle32					turn_spd;		//!< 回転角速度
	Angle32					turn_gen_var;	//!< 緩やかターンの速度カーブ角度変数
	Angle32					turn_gen_factor;	//!< 緩やかターンの速度カーブ決定値（コサイン角度）
	
	fx32					drift_pivot_x;	//!< ドリフト移動 軸座標X
	Angle32					drift_angle;	//!< ドリフト移動 サイン波角度
	Angle32					drift_ang_spd;	//!< ドリフト移動 サイン波角速度
	Sint32					drift_timer;	//!< ドリフト移動 タイマ（所要時間厳守のため必要）
	
	fx32					atk_nml_alt;	//!< 通常攻撃の高度
	
	VecFx32					bash_targ_pos;	//!< 叩きつけ突進目標位置
	VecFx32					bash_ret_pos;	//!< 叩きつけ戻り目標位置
	VecFx32					bash_orig_pos;	//!< 叩きつけ各種移動開始元位置
	Angle32					bash_homing_deg;	//!< 叩きつけ戻り移動時速度カーブ角度
	
	
	GMS_BOSS1_EFF_BOMB_WORK	bomb_work;		//!< 爆発処理ワーク
	
	//! 構成パーツのオブジェクト（自分自身も含む）
	OBS_OBJECT_WORK	*parts_objs[GME_BOSS1_PART_IDX_MAX];
	
	//! 各構成パーツの再生停滞タイマ
	GMS_BOSS1_MTN_SUSPEND_WORK	mtn_suspend[GME_BOSS1_PART_IDX_MAX];
};

//! ボス１鎖ワーク
struct tag_GMS_BOSS1_CHAIN_WORK
{
	GMS_ENEMY_3D_WORK	ene_3d;
	
	GMS_BOSS1_MGR_WORK		*mgr_work;	//!< 管理ワーク
	
	Uint32				flag;
	
	void	(*proc_update)(GMS_BOSS1_CHAIN_WORK*);	//!< 更新処理関数
	
	GMS_BS_CMN_BMCB_MGR		bmcb_mgr;				//!< ボスモーションCB管理ワーク
	GMS_BS_CMN_SNM_WORK		snm_work;				//!< SNMワーク
	Sint32					ball_snm_reg_id;		//!< 鉄球SNM登録ID
	Sint32					sct_snm_reg_ids[GMD_BOSS1_SCATTER_PARTS_NUM];	//!< 爆発時の飛び散るパーツ数
	
	GMS_BS_CMN_CNM_MGR_WORK	cnm_mgr_work;	//!< CNM管理ワーク
	Sint32					sct_cnm_reg_ids[GMD_BOSS1_SCATTER_PARTS_NUM];	//!< 爆発時の飛び散るパーツ数
};

//! ボス１エッグマンワーク
struct tag_GMS_BOSS1_EGG_WORK
{
	GMS_ENEMY_3D_WORK	ene_3d;
	
	GMS_BOSS1_MGR_WORK		*mgr_work;	//!< 管理ワーク
	
	Uint32					flag;
	
	GME_BOSS1_EGG_ACT_ID	egg_act_id;
	
	void	(*proc_update)(GMS_BOSS1_EGG_WORK*);	//!< 更新処理関数
};

//! ボス１パーツアクション情報構造体
typedef struct tag_GMS_BOSS1_PART_ACT_INFO
{
	Uint16	act_id;	//!< モーション番号（AMBインデックス）
	Uint8	is_maintain;	//!< 前のアクション継続
	Uint8	is_repeat;		//!< リピート
	Float	mtn_spd;		//!< モーション再生速度
	BOOL	is_blend;		//!< ブレンド有無
	Float	blend_spd;		//!< ブレンド速度
	BOOL	is_merge_manual;	//!< マニュアルマージ有無
} GMS_BOSS1_PART_ACT_INFO;

//! 衝撃波エフェクトワーク
typedef struct tag_GMS_BOSS1_EFF_SHOCKWAVE_WORK
{
	GMS_EFFECT_3DES_WORK	eff_3des;
	
	GMS_BOSS1_MGR_WORK		*mgr_work;	//!< 管理ワーク
	
	Uint32					atk_rect_timer;	//!< 攻撃矩形有効時間タイマ
} GMS_BOSS1_EFF_SHOCKWAVE_WORK;

//! 衝撃波エフェクトサブパーツワーク
typedef struct tag_GMS_BOSS1_EFF_SHOCKWAVE_SUB_WORK
{
	GMS_EFFECT_3DES_WORK	eff_3des;
	
	GMS_BOSS1_MGR_WORK		*mgr_work;	//!< 管理ワーク
} GMS_BOSS1_EFF_SHOCKWAVE_SUB_WORK;

//! パーツ飛散エフェクト パーツ操作NDCワーク
typedef struct tag_GMS_BOSS1_EFF_SCT_PART_NDC_WORK
{
	GMS_BS_CMN_NODE_CTRL_OBJECT	ncd_obj;
	
	AMS_QUAT	spin_quat;	//!< 回転差分クォータニオン
	BOOL		is_ironball;	//!< 鉄球タイプフラグ（TRUEだと回転が遅く設定される）
} GMS_BOSS1_EFF_SCT_PART_NDC_WORK;

//! ボス１ 撃破時画面フラッシュワーク
typedef struct tag_GMS_BOSS1_FLASH_SCREEN_WORK
{
	GMS_EFFECT_COM_WORK	efct_com;
	GMS_CMN_FLASH_SCR_WORK	flash_work;	//!< 画面白フラッシュワーク
} GMS_BOSS1_FLASH_SCREEN_WORK;


//! 本体ステート開始関数
typedef void (*GMF_BOSS1_BODY_STATE_ENTER_FUNC)(GMS_BOSS1_BODY_WORK*);
//! 本体ステート終了関数
typedef void (*GMF_BOSS1_BODY_STATE_LEAVE_FUNC)(GMS_BOSS1_BODY_WORK*);


//! 本体ステート開始情報構造体
typedef struct tag_GMS_BOSS1_BODY_STATE_ENTER_INFO
{
	GMF_BOSS1_BODY_STATE_ENTER_FUNC	enter_func;	//!< 本体ステート開始関数
	BOOL	is_wrapped;		//!< 間接呼び出しフラグ
} GMS_BOSS1_BODY_STATE_ENTER_INFO;

/*------ External Declarations -----------------------------------------*/

/*------ Static Declarations -------------------------------------------*/
//############ 共通 ###########################################################
/* 補助関数 */
static void gmBoss1SetPartTextureBurnt(OBS_OBJECT_WORK *obj_work);
static BOOL gmBoss1IsScrollLockBusy(void);
static void gmBoss1Init1ShotTimer(GMS_BOSS1_1SHOT_TIMER *one_shot_timer, Uint32 frame);
static BOOL gmBoss1Update1ShotTimer(GMS_BOSS1_1SHOT_TIMER *one_shot_timer);
//############ ボス１管理 #####################################################
/* 補助関数 */
static void gmBoss1MgrIncObjCreateCount(GMS_BOSS1_MGR_WORK *mgr_work);
static void gmBoss1MgrDecObjCreateCount(GMS_BOSS1_MGR_WORK *mgr_work);
static BOOL gmBoss1MgrIsAllCreatedObjDeleted(GMS_BOSS1_MGR_WORK *mgr_work);
/* 制御処理 */
static void gmBoss1MgrWaitLoad(OBS_OBJECT_WORK *obj_work);
static void gmBoss1MgrWaitSetup(OBS_OBJECT_WORK *obj_work);
static void gmBoss1MgrMain(OBS_OBJECT_WORK *obj_work);
static void gmBoss1MgrWaitRelease(OBS_OBJECT_WORK *obj_work);

//############ ボス１本体 #####################################################
static void gmBoss1BodyExit(MTS_TASK_TCB *tcb);
/* 補助関数 */
static void gmBoss1BodySetActionWhole(GMS_BOSS1_BODY_WORK *body_work,
									  GME_BOSS1_ACT_ID act_id, BOOL force_change=FALSE);
static void gmBoss1BodySetSuspendAction(GMS_BOSS1_BODY_WORK *body_work,
										GME_BOSS1_PART_IDX part_idx,
										Uint32 suspend_time);
static void gmBoss1BodyUpdateSuspendAction(GMS_BOSS1_BODY_WORK *body_work);
static BOOL gmBoss1BodyCheckChainMotionMergeEnd(GMS_BOSS1_BODY_WORK *body_work);
static void gmBoss1BodySetNoHitTime(GMS_BOSS1_BODY_WORK *body_work);
static void gmBoss1BodyUpdateNoHitTime(GMS_BOSS1_BODY_WORK *body_work);
static void gmBoss1BodyExecDamageRoutine(GMS_BOSS1_BODY_WORK *body_work);
static void gmBoss1BodySetDmgRectSizeForAtkNml(GMS_BOSS1_BODY_WORK *body_work);
static void gmBoss1BodySetDmgRectSizeToDefault(GMS_BOSS1_BODY_WORK *body_work);
static void gmBoss1BodySetAtkRectToWeakAttacker(GMS_BOSS1_BODY_WORK *body_work);
static void gmBoss1BodySetAtkRectToNormal(GMS_BOSS1_BODY_WORK *body_work);
static BOOL gmBoss1BodyIsExtraAttack(GMS_BOSS1_BODY_WORK *body_work);
static BOOL gmBoss1BodyIsEscapeScrUnlock(GMS_BOSS1_BODY_WORK *body_work);
static BOOL gmBoss1BodyIsEscapeOutFinalZone(GMS_BOSS1_BODY_WORK *body_work);
static BOOL gmBoss1BodyIsDirectionPositiveFromCurrent(GMS_BOSS1_BODY_WORK *body_work, Angle16 target_angle);
static void gmBoss1BodyUpdateDirection(GMS_BOSS1_BODY_WORK *body);
static void gmBoss1BodySetDirectionNormal(GMS_BOSS1_BODY_WORK *body_work);
static void gmBoss1BodySetDirection(GMS_BOSS1_BODY_WORK *body_work, Angle16 deg);
#if (GMD_BOSS1_BOOL_USE_OBSOLETE_FUNCTION)
static void gmBoss1BodyInitTurn(GMS_BOSS1_BODY_WORK *body_work,
								Angle32 turn_amount, Angle32 turn_spd);
static void gmBoss1BodyInitTurn(GMS_BOSS1_BODY_WORK *body_work,
								Angle16 dest_angle, Sint32 frame, BOOL is_positive);
static BOOL gmBoss1BodyUpdateTurn(GMS_BOSS1_BODY_WORK *body_work, Float spd_rate=1.f);
#endif /* (GMD_BOSS1_BOOL_USE_OBSOLETE_FUNCTION) */
static void gmBoss1BodyInitTurnGently(GMS_BOSS1_BODY_WORK *body_work, Angle16 dest_angle,
									  Sint32 frame, BOOL is_positive);
static BOOL gmBoss1BodyUpdateTurnGently(GMS_BOSS1_BODY_WORK *body_work);
static void gmBoss1BodyInitPreANChainMotion(GMS_BOSS1_BODY_WORK *body_work);
static void gmBoss1BodyInitPreANMove(GMS_BOSS1_BODY_WORK *body_work);
static BOOL gmBoss1BodyUpdatePreANMove(GMS_BOSS1_BODY_WORK *body_work);
static void gmBoss1BodySetANChainInitialBlendSpd(GMS_BOSS1_BODY_WORK *body_work);
static void gmBoss1BodyInitAtkNmlMove(GMS_BOSS1_BODY_WORK *body_work, Sint32 frame);
static BOOL gmBoss1BodyUpdateAtkNmlMove(GMS_BOSS1_BODY_WORK *body_work);
static void gmBoss1BodySetFlipForAtkNmlMove(GMS_BOSS1_BODY_WORK *body_work);
static void gmBoss1BodyInitAtkNmlFlipAndTurn(GMS_BOSS1_BODY_WORK *body_work);
static BOOL gmBoss1BodyUpdateAtkNmlFlipAndTurn(GMS_BOSS1_BODY_WORK *body_work);
static void gmBoss1BodyInitAtkNmlDrift(GMS_BOSS1_BODY_WORK *body_work, Sint32 frame);
static BOOL gmBoss1BodyUpdateAtkNmlDrift(GMS_BOSS1_BODY_WORK *body_work);
static void gmBoss1BodyInitRush(GMS_BOSS1_BODY_WORK *body_work, BOOL is_left);
static BOOL gmBoss1BodyUpdateRush(GMS_BOSS1_BODY_WORK *body_work);
static void gmBoss1BodyInitBashReturn(GMS_BOSS1_BODY_WORK *body_work, BOOL is_left);
static BOOL gmBoss1BodyUpdateBashReturn(GMS_BOSS1_BODY_WORK *body_work);
static void gmBoss1BodyInitEscapeMove(GMS_BOSS1_BODY_WORK *body_work);
static BOOL gmBoss1BodyUpdateEscapeMove(GMS_BOSS1_BODY_WORK *body_work);
static void gmBoss1BodyInitDefeatState(GMS_BOSS1_BODY_WORK *body_work);
/* ノード操作関連 */
static void gmBoss1BodyUpdateChainTopDirection(GMS_BOSS1_BODY_WORK *body_work);
/* 処理関数 */
static void gmBoss1BodyDamageDefFunc(OBS_RECT_WORK *my_rect, OBS_RECT_WORK *your_rect);
static void gmBoss1BodyOutFunc(OBS_OBJECT_WORK *obj_work);
/* 制御処理 */
static void gmBoss1BodyChangeState(GMS_BOSS1_BODY_WORK *body_work,
								   GME_BOSS1_BODY_STATE state, BOOL is_wrapped=FALSE);
static void gmBoss1BodyWaitSetup(OBS_OBJECT_WORK *obj_work);
static void gmBoss1BodyMain(OBS_OBJECT_WORK *obj_work);
/* シーケンス */
// 開始ステート
static void gmBoss1BodyStateEnterStart(GMS_BOSS1_BODY_WORK *body_work);
static void gmBoss1BodyStateLeaveStart(GMS_BOSS1_BODY_WORK *body_work);
static void gmBoss1BodyStateUpdateStartWithWaitLockBegin(GMS_BOSS1_BODY_WORK *body_work);
static void gmBoss1BodyStateUpdateStartWithWaitLockComplete(GMS_BOSS1_BODY_WORK *body_work);
static void gmBoss1BodyStateUpdateStartWithFall(GMS_BOSS1_BODY_WORK *body_work);
static void gmBoss1BodyStateUpdateStartWithMove(GMS_BOSS1_BODY_WORK *body_work);
static void gmBoss1BodyStateUpdateStartWithWaitEnd(GMS_BOSS1_BODY_WORK *body_work);
// 鉄球準備ステート
static void gmBoss1BodyStateEnterPrep(GMS_BOSS1_BODY_WORK *body_work);
static void gmBoss1BodyStateLeavePrep(GMS_BOSS1_BODY_WORK *body_work);
static void gmBoss1BodyStateUpdatePrepWithWait(GMS_BOSS1_BODY_WORK *body_work);
// 通常攻撃開始ステート
static void gmBoss1BodyStateEnterPreAtkNml(GMS_BOSS1_BODY_WORK *body_work);
static void gmBoss1BodyStateLeavePreAtkNml(GMS_BOSS1_BODY_WORK *body_work);
static void gmBoss1BodyStateUpdatePreAtkNmlWithMove(GMS_BOSS1_BODY_WORK *body_work);
// 通常攻撃ステート
static void gmBoss1BodyStateEnterAtkNml(GMS_BOSS1_BODY_WORK *body_work);
static void gmBoss1BodyStateLeaveAtkNml(GMS_BOSS1_BODY_WORK *body_work);
static void gmBoss1BodyStateUpdateAtkNmlWithTurn(GMS_BOSS1_BODY_WORK *body_work);
static void gmBoss1BodyStateUpdateAtkNmlWithMove(GMS_BOSS1_BODY_WORK *body_work);
// 叩きつけステート
static void gmBoss1BodyStateEnterAtkBash(GMS_BOSS1_BODY_WORK *body_work);
static void gmBoss1BodyStateLeaveAtkBash(GMS_BOSS1_BODY_WORK *body_work);
static void gmBoss1BodyStateUpdateAtkBashWithLock(GMS_BOSS1_BODY_WORK *body_work);
static void gmBoss1BodyStateUpdateAtkBashWithPrep(GMS_BOSS1_BODY_WORK *body_work);
static void gmBoss1BodyStateUpdateAtkBashWithSwing(GMS_BOSS1_BODY_WORK *body_work);
static void gmBoss1BodyStateUpdateAtkBashWithFinish(GMS_BOSS1_BODY_WORK *body_work);
static void gmBoss1BodyStateUpdateAtkBashWithHoming(GMS_BOSS1_BODY_WORK *body_work);
// 通常ダメージステート
static void gmBoss1BodyStateEnterDmgNml(GMS_BOSS1_BODY_WORK *body_work);
static void gmBoss1BodyStateLeaveDmgNml(GMS_BOSS1_BODY_WORK *body_work);
// 撃破ステート
static void gmBoss1BodyStateEnterDefeat(GMS_BOSS1_BODY_WORK *body_work);
static void gmBoss1BodyStateLeaveDefeat(GMS_BOSS1_BODY_WORK *body_work);
static void gmBoss1BodyStateUpdateDefeatWithWaitStart(GMS_BOSS1_BODY_WORK *body_work);
static void gmBoss1BodyStateUpdateDefeatWithExplode(GMS_BOSS1_BODY_WORK *body_work);
static void gmBoss1BodyStateUpdateDefeatWithScatter(GMS_BOSS1_BODY_WORK *body_work);
static void gmBoss1BodyStateUpdateDefeatWithWaitEnd(GMS_BOSS1_BODY_WORK *body_work);
// 逃亡ステート
static void gmBoss1BodyStateEnterEscape(GMS_BOSS1_BODY_WORK *body_work);
static void gmBoss1BodyStateLeaveEscape(GMS_BOSS1_BODY_WORK *body_work);
static void gmBoss1BodyStateUpdateEscapeWithTurn(GMS_BOSS1_BODY_WORK *body_work);
static void gmBoss1BodyStateUpdateEscapeWithMoveLocked(GMS_BOSS1_BODY_WORK *body_work);
static void gmBoss1BodyStateUpdateEscapeWithMoveUnlocked(GMS_BOSS1_BODY_WORK *body_work);
static void gmBoss1BodyStateUpdateEscapeWithMoveMoveFinalZone(GMS_BOSS1_BODY_WORK *body_work);

//############ ボス１鎖 #######################################################
static void gmBoss1ChainExit(MTS_TASK_TCB *tcb);
/* 補助関数 */
static void gmBoss1ChainUpdateAtkRectPosition(GMS_BOSS1_CHAIN_WORK *chain_work);
/* 処理関数 */
static void gmBoss1ChainAtkHitFunc(OBS_RECT_WORK *my_rect, OBS_RECT_WORK *your_rect);
static void gmBoss1ChainOutFunc(OBS_OBJECT_WORK *obj_work);
/* 制御処理 */
static void gmBoss1ChainWaitSetup(OBS_OBJECT_WORK *obj_work);
static void gmBoss1ChainMain(OBS_OBJECT_WORK *obj_work);

//############ ボス１エッグマン ###############################################
static void gmBoss1EggExit(MTS_TASK_TCB *tcb);
/* 補助関数 */
static void gmBoss1EggSetActionIndependent(GMS_BOSS1_EGG_WORK *egg_work,
										   GME_BOSS1_EGG_ACT_ID act_id, BOOL force_change=FALSE);
static void gmBoss1EggRevertActionIndependent(GMS_BOSS1_EGG_WORK *egg_work);
/* 制御処理 */
static void gmBoss1EggWaitSetup(OBS_OBJECT_WORK *obj_work);
static void gmBoss1EggMain(OBS_OBJECT_WORK *obj_work);
/* シーケンス */
// 通常時停滞シーケンス
static void gmBoss1EggProcIdleInit(GMS_BOSS1_EGG_WORK *egg_work);
static void gmBoss1EggProcIdleUpdateLoop(GMS_BOSS1_EGG_WORK *egg_work);
// 笑いシーケンス
static void gmBoss1EggProcLaughInit(GMS_BOSS1_EGG_WORK *egg_work);
static void gmBoss1EggProcLaughUpdateLoop(GMS_BOSS1_EGG_WORK *egg_work);
// ダメージシーケンス
static void gmBoss1EggProcDamageInit(GMS_BOSS1_EGG_WORK *egg_work);
static void gmBoss1EggProcDamageUpdateLoop(GMS_BOSS1_EGG_WORK *egg_work);
// 逃亡シーケンス
static void gmBoss1EggProcEscapeInit(GMS_BOSS1_EGG_WORK* egg_work);
static void gmBoss1EggProcEscapeUpdateLoop(GMS_BOSS1_EGG_WORK *egg_work);

//############ ボス１衝撃波エフェクト #########################################
static GMS_EFFECT_3DES_WORK* gmBoss1EffShockwaveInit(GMS_BOSS1_CHAIN_WORK *chain_work);
static void gmBoss1EffShockwaveExit(MTS_TASK_TCB *tcb);
static void gmBoss1EffShockwaveProcMain(OBS_OBJECT_WORK *obj_work);
static GMS_EFFECT_3DES_WORK* gmBoss1EffShockwaveSubpartInit(GMS_BOSS1_EFF_SHOCKWAVE_WORK *sw_work,
															fx32 ofst_h, BOOL is_left);
static void gmBoss1EffShockwaveSubExit(MTS_TASK_TCB *tcb);
/* 補助関数 */
/* 処理関数 */
/* 制御処理 */

//############ ボス１パーツ飛散エフェクト #########################################
static void gmBoss1EffScatterInit(GMS_BOSS1_CHAIN_WORK *chain_work);
/* 補助関数 */
static void gmBoss1EffScatterSetPartParam(GMS_BOSS1_EFF_SCT_PART_NDC_WORK *sct_part_ndc,
										  BOOL is_ironball);
static void gmBoss1EffScatterSetFlyParam(GMS_BOSS1_EFF_SCT_PART_NDC_WORK *sct_part_ndc);
/* 処理関数 */
/* シーケンス */
static void gmBoss1EffScatterProcWait(OBS_OBJECT_WORK *obj_work);
static void gmBoss1EffScatterProcFly(OBS_OBJECT_WORK *obj_work);

//############ ボス１爆発エフェクト #########################################
static void gmBoss1EffBombInitCreate(GMS_BOSS1_EFF_BOMB_WORK *bomb_work,
									 GME_BOSS1_EFF_BOMB_TYPE bomb_type,
									 OBS_OBJECT_WORK *parent_obj,
									 fx32 pos_x, fx32 pos_y, fx32 width, fx32 height,
									 Uint32 interval_min, Uint32 interval_max);
static void gmBoss1EffBombUpdateCreate(GMS_BOSS1_EFF_BOMB_WORK *bomb_work);

//############ ダメージエフェクト #############################################
static void gmBoss1EffDamageInit(GMS_BOSS1_BODY_WORK *body_work);

//############ アフターバーナーエフェクト #####################################
static void gmBoss1EffAfterburnerSetEnable(GMS_BOSS1_BODY_WORK *body_work, BOOL is_enable);
static void gmBoss1EffAfterburnerUpdateCreate(GMS_BOSS1_BODY_WORK *body_work);
static void gmBoss1EffAfterburnerInit(GMS_BOSS1_BODY_WORK *body_work);
static void gmBoss1EffAfterburnerProcMain(OBS_OBJECT_WORK *obj_work);

//############ アフターバーナー煙エフェクト ###################################
static void gmBoss1EffABSmokeInit(GMS_BOSS1_BODY_WORK *body_work);
static void gmBoss1EffABSmokeProcMain(OBS_OBJECT_WORK *obj_work);

//############ 本体黒煙エフェクト #############################################
static void gmBoss1EffBodySmokeInit(GMS_BOSS1_BODY_WORK *body_work);
static void gmBoss1EffBodySmokeProcMain(OBS_OBJECT_WORK *obj_work);

//############ 本体小黒煙エフェクト ###########################################
static void gmBoss1EffBodySmallSmokeInit(GMS_BOSS1_BODY_WORK *body_work);
static void gmBoss1EffBodySmallSmokeProcMain(OBS_OBJECT_WORK *obj_work);

//############ 本体破片エフェクト #############################################
static void gmBoss1EffBodyDebrisInit(GMS_BOSS1_BODY_WORK *body_work);

//############ 汗エフェクト #############################################
static void gmBoss1EffSweatInit(GMS_BOSS1_EGG_WORK *egg_work);
static void gmBoss1EffSweatProcMain(OBS_OBJECT_WORK *obj_work);

//############ 画面フラッシュ #################################################
static void gmBoss1InitFlashScreen(void);
/* 補助関数 */
/* 処理関数*/
/* シーケンス */
static void gmBoss1FlashScreenMain(OBS_OBJECT_WORK *obj_work);

#if defined(MTD_DEBUG)
//############ デバッグ ###############################################
#endif /* defined(MTD_DEBUG) */

/*------ Global Variables ----------------------------------------------*/

/*------ Static Variables ----------------------------------------------*/
//! ボス全体アクションIDテーブル
const static GMS_BOSS1_PART_ACT_INFO gm_boss1_act_id_tbl[GME_BOSS1_ACT_ID_MAX][GME_BOSS1_PART_IDX_MAX]	= {
	// ACT_ID											IS_MAINTAIN		IS_REPEAT	MTN_SPD	IS_BLEND	BLEND_SPD
	// GME_BOSS1_ACT_ID_APP_FALL
	{
		{IDB_BOSS01_BODY_MTN_B01_PRO01_01B_ZNM,			FALSE,			TRUE,		1.f,	FALSE,		GMD_BOSS1_DEFAULT_BLEND_SPD,	FALSE},		// BODY
		{IDB_BOSS01_CHAIN_MTN_B01_PRO01_01C_ZNM,		FALSE,			TRUE,		1.f,	FALSE,		GMD_BOSS1_DEFAULT_BLEND_SPD,	FALSE},		// CHAIN
		{IDB_BOSS01_EGG_MTN_B01_PRO01_01E_ZNM,			FALSE,			TRUE,		1.f,	FALSE,		GMD_BOSS1_DEFAULT_BLEND_SPD,	FALSE},		// EGG
	},
	
	// GME_BOSS1_ACT_ID_APP_END	（※GENESIS版準拠の挙動になったので現在未使用）
	{
		{IDB_BOSS01_BODY_MTN_B01_PRO01_02B_ZNM,			FALSE,			FALSE,		1.f,	FALSE,		GMD_BOSS1_DEFAULT_BLEND_SPD,	FALSE},		// BODY
		{IDB_BOSS01_CHAIN_MTN_B01_PRO01_02C_ZNM,		FALSE,			FALSE,		1.f,	FALSE,		GMD_BOSS1_DEFAULT_BLEND_SPD,	FALSE},		// CHAIN
		{IDB_BOSS01_EGG_MTN_B01_PRO01_02E_ZNM,			FALSE,			FALSE,		1.f,	FALSE,		GMD_BOSS1_DEFAULT_BLEND_SPD,	FALSE},		// EGG
	},
	
	// GME_BOSS1_ACT_ID_PREP_CHAIN
	{
		{IDB_BOSS01_BODY_MTN_B01_STA01_01B_ZNM,			FALSE,			FALSE,		1.f,	TRUE,		GMD_BOSS1_DEFAULT_BLEND_SPD,	FALSE},		// BODY
		{IDB_BOSS01_CHAIN_MTN_B01_STA01_01C_ZNM,		FALSE,			FALSE,		1.f,	FALSE,		GMD_BOSS1_DEFAULT_BLEND_SPD,	FALSE},		// CHAIN
		{IDB_BOSS01_EGG_MTN_B01_STA01_01E_ZNM,			FALSE,			FALSE,		1.f,	TRUE,		GMD_BOSS1_DEFAULT_BLEND_SPD,	FALSE},		// EGG
	},
	
	//GME_BOSS1_ACT_ID_PRE_ATK_NML_MOVE
	{
		{IDB_BOSS01_BODY_MTN_B01_ATT01_01B_ZNM,			FALSE,			TRUE,		1.f,	TRUE,		GMD_BOSS1_DEFAULT_BLEND_SPD,	FALSE},		// BODY
		{IDB_BOSS01_CHAIN_MTN_B01_ATT01_01C_ZNM,		FALSE,			TRUE,		GMD_BOSS1_BODY_ATKNML_CHAIN_MTN_SPD,	TRUE,		GMD_BOSS1_DEFAULT_BLEND_SPD,	FALSE},		// CHAIN
		{IDB_BOSS01_EGG_MTN_B01_ATT01_01E_ZNM,			FALSE,			TRUE,		1.f,	TRUE,		GMD_BOSS1_DEFAULT_BLEND_SPD,	FALSE},		// EGG
	},
	
	// GME_BOSS1_ACT_ID_ATK_NML_MOVE
	{
		{0,												TRUE,			TRUE,		1.f,	FALSE,		GMD_BOSS1_DEFAULT_BLEND_SPD,	FALSE},		// BODY
		{IDB_BOSS01_CHAIN_MTN_B01_ATT01_01C_ZNM,		FALSE,			TRUE,		GMD_BOSS1_BODY_ATKNML_CHAIN_MTN_SPD,	TRUE,		0.05f,	TRUE},		// CHAIN
		{0,												TRUE,			TRUE,		1.f,	FALSE,		GMD_BOSS1_DEFAULT_BLEND_SPD,	FALSE},		// EGG
	},
	
	// GME_BOSS1_ACT_ID_ATK_BASH_LOCK
	{
		{0,												TRUE,			TRUE,		1.f,	FALSE,		GMD_BOSS1_DEFAULT_BLEND_SPD,	FALSE},		// BODY
		{0,												TRUE,			TRUE,		GMD_BOSS1_BODY_ATKNML_CHAIN_MTN_SPD,	FALSE,		GMD_BOSS1_DEFAULT_BLEND_SPD,	FALSE},		// CHAIN
		{0,												TRUE,			TRUE,		1.f,	FALSE,		GMD_BOSS1_DEFAULT_BLEND_SPD,	FALSE},		// EGG
	},
	
	// GME_BOSS1_ACT_ID_ATK_BASH_PREP
	{
		{IDB_BOSS01_BODY_MTN_B01_ATT02_01B_ZNM,			FALSE,			FALSE,		1.f,	TRUE,		GMD_BOSS1_DEFAULT_BLEND_SPD,	FALSE},		// BODY
		{IDB_BOSS01_CHAIN_MTN_B01_ATT02_01C_ZNM,		FALSE,			FALSE,		1.f,	TRUE,		0.007f/*0.01f*/,							TRUE},		// CHAIN
		{IDB_BOSS01_EGG_MTN_B01_ATT02_01E_ZNM,			FALSE,			FALSE,		1.f,	TRUE,		GMD_BOSS1_DEFAULT_BLEND_SPD,	FALSE},		// EGG
	},
	
	// GME_BOSS1_ACT_ID_ATK_BASH_SWING
	{
		{IDB_BOSS01_BODY_MTN_B01_ATT02_02B_ZNM,			FALSE,			FALSE,		1.f,	TRUE,		GMD_BOSS1_DEFAULT_BLEND_SPD,	FALSE},		// BODY
		{IDB_BOSS01_CHAIN_MTN_B01_ATT02_02C_ZNM,		FALSE,			FALSE,		1.f,	TRUE,		GMD_BOSS1_DEFAULT_BLEND_SPD,	FALSE},		// CHAIN
		{IDB_BOSS01_EGG_MTN_B01_ATT02_02E_ZNM,			FALSE,			FALSE,		1.f,	TRUE,		GMD_BOSS1_DEFAULT_BLEND_SPD,	FALSE},		// EGG
	},
	
	// GME_BOSS1_ACT_ID_ATK_BASH_FINISH
	{
		{IDB_BOSS01_BODY_MTN_B01_ATT02_03B_ZNM,			FALSE,			FALSE,		1.f,	FALSE,		GMD_BOSS1_DEFAULT_BLEND_SPD,	FALSE},		// BODY
		{IDB_BOSS01_CHAIN_MTN_B01_ATT02_03C_ZNM,		FALSE,			FALSE,		1.f,	FALSE,		GMD_BOSS1_DEFAULT_BLEND_SPD,	FALSE},		// CHAIN
		{IDB_BOSS01_EGG_MTN_B01_ATT02_03E_ZNM,			FALSE,			FALSE,		1.f,	FALSE,		GMD_BOSS1_DEFAULT_BLEND_SPD,	FALSE},		// EGG
	},
	
	// GME_BOSS1_ACT_ID_ATK_BASH_HOMING
	{
		{IDB_BOSS01_BODY_MTN_B01_ATT02_04B_ZNM,			FALSE,			TRUE,		1.f,	FALSE,		GMD_BOSS1_DEFAULT_BLEND_SPD,	FALSE},		// BODY
		{IDB_BOSS01_CHAIN_MTN_B01_ATT02_04C_ZNM,		FALSE,			TRUE,		1.f,	FALSE,		GMD_BOSS1_DEFAULT_BLEND_SPD,	FALSE},		// CHAIN
		{IDB_BOSS01_EGG_MTN_B01_ATT02_04E_ZNM,			FALSE,			TRUE,		1.f,	FALSE,		GMD_BOSS1_DEFAULT_BLEND_SPD,	FALSE},		// EGG
	},
	
	// GME_BOSS1_ACT_ID_ATK_BASH_RESTORE
	{
		{IDB_BOSS01_BODY_MTN_B01_ATT02_05B_ZNM,			FALSE,			FALSE,		1.f,	FALSE,		GMD_BOSS1_DEFAULT_BLEND_SPD,	FALSE},		// BODY
		{IDB_BOSS01_CHAIN_MTN_B01_ATT02_05C_ZNM,		FALSE,			FALSE,		1.f,	FALSE,		GMD_BOSS1_DEFAULT_BLEND_SPD,	FALSE},		// CHAIN
		{IDB_BOSS01_EGG_MTN_B01_ATT02_05E_ZNM,			FALSE,			FALSE,		1.f,	FALSE,		GMD_BOSS1_DEFAULT_BLEND_SPD,	FALSE},		// EGG
	},
	// GME_BOSS1_ACT_ID_DAMAGE_NML
	{
		{0,												TRUE,			FALSE,		1.f,	FALSE,		GMD_BOSS1_DEFAULT_BLEND_SPD,	FALSE},		// BODY
		{0,												TRUE,			FALSE,		1.f,	FALSE,		GMD_BOSS1_DEFAULT_BLEND_SPD,	FALSE},		// CHAIN
		{IDB_BOSS01_EGG_MTN_B01_DMG01_01E_ZNM,			FALSE,			FALSE,		1.f,	FALSE,		GMD_BOSS1_DEFAULT_BLEND_SPD,	FALSE},		// EGG
	},
	
	// GME_BOSS1_ACT_ID_ESCAPE
	{
		{IDB_BOSS01_BODY_MTN_B01_DMG02_01B_ZNM,			FALSE,			TRUE,		1.f,	TRUE,		GMD_BOSS1_DEFAULT_BLEND_SPD,	FALSE},		// BODY
		{0,												TRUE,			FALSE,		1.f,	FALSE,		GMD_BOSS1_DEFAULT_BLEND_SPD,	FALSE},		// CHAIN
		{IDB_BOSS01_EGG_MTN_B01_DMG02_01E_ZNM,			FALSE,			TRUE,		1.f,	TRUE,		GMD_BOSS1_DEFAULT_BLEND_SPD,	FALSE},		// EGG
	},
};

//! ボス１ エッグマン独立アクションIDテーブル
const static GMS_BOSS1_PART_ACT_INFO gm_boss1_egg_act_id_tbl[GME_BOSS1_EGG_ACT_ID_MAX]	= {
	// ACT_ID										IS_MAINTAIN		IS_REPEAT	MTN_SPD	IS_BLEND	BLEND_SPD
	// GME_BOSS1_EGG_ACT_ID_LAUGH
	{IDB_BOSS01_EGG_MTN_B01_PRO01_01E_ZNM,			FALSE,			FALSE,		1.f,	TRUE,		GMD_BOSS1_DEFAULT_BLEND_SPD,	FALSE},
	// GME_BOSS1_EGG_ACT_ID_DAMAGE
	{IDB_BOSS01_EGG_MTN_B01_DMG01_01E_ZNM,			FALSE,			FALSE,		1.f,	TRUE,		GMD_BOSS1_DEFAULT_BLEND_SPD,	FALSE},
};

//! 本体 ステート開始関数テーブル
const static GMS_BOSS1_BODY_STATE_ENTER_INFO gm_boss1_body_state_enter_info_tbl[GME_BOSS1_BODY_STATE_MAX]	= {
	{	NULL,							FALSE	},
	{	gmBoss1BodyStateEnterStart,		FALSE	},
	{	gmBoss1BodyStateEnterPrep,		FALSE	},
	{	gmBoss1BodyStateEnterPreAtkNml,	FALSE	},
	{	gmBoss1BodyStateEnterAtkNml,	FALSE	},
	{	gmBoss1BodyStateEnterAtkBash,	FALSE	},
	{	gmBoss1BodyStateEnterDmgNml,	FALSE	},
	{	gmBoss1BodyStateEnterDefeat,	TRUE	},
	{	gmBoss1BodyStateEnterEscape,	FALSE	},
};

//! 本体 ステート終了関数テーブル
const static GMF_BOSS1_BODY_STATE_LEAVE_FUNC gm_boss1_body_state_leave_func_tbl[GME_BOSS1_BODY_STATE_MAX]	= {
	NULL,
	gmBoss1BodyStateLeaveStart,
	gmBoss1BodyStateLeavePrep,
	gmBoss1BodyStateLeavePreAtkNml,
	gmBoss1BodyStateLeaveAtkNml,
	gmBoss1BodyStateLeaveAtkBash,
	gmBoss1BodyStateLeaveDmgNml,
	gmBoss1BodyStateLeaveDefeat,
	gmBoss1BodyStateLeaveEscape,
};

//! 衝撃波エフェクト 矩形 攻撃属性フラグテーブル
const static Uint16 gm_boss1_eff_sw_atk_flag_tbl[GME_EFFECT_RECT_NUM]	= {
	0,		// 食らい
	(GMD_OBJ_RECT_ATK_FLAG_NORMALATK | GMD_OBJ_RECT_ATK_FLAG_EFCTATK),	// 攻撃
};

//! 衝撃波エフェクト 矩形 防御属性フラグテーブル
const static Uint16 gm_boss1_eff_sw_def_flag_tbl[GME_EFFECT_RECT_NUM]	= {
	GMD_OBJ_RECT_DEF_FLAG_EFCTDEF,	// 食らい矩形
	GMD_OBJ_RECT_DEF_FLAG_EFCTATK,	// 攻撃矩形
};

//! 小黒煙エフェクト 表示オフセットテーブル
const static Float gm_boss1_eff_small_smoke_disp_ofst_tbl[GMD_BOSS1_EFF_SMALL_SMOKE_NUM][MTD_XYZ]	= {
	{	-8.f,	4.f,	28.f,	},
	{	16.f,	16.f,	0.f,	},
	{	-40.f,	0.f,	0.f,	},
};

// 構築済みモデル(obj_3d)格納リスト
static OBS_ACTION3D_NN_WORK *gm_boss1_obj_3d_list	= NULL;

/*------ Global Functions ----------------------------------------------*/

// =======================================================================
// GmBoss1Build
/*!
  ボス１ データ構築
 */
// =======================================================================
void GmBoss1Build(void)
{
	void	*mdl_amb;
	void	*tex_amb;
	mdl_amb	= ObjDataLoadAmbIndex(NULL, IDB_BOSS01_BOSS01_MDL_AMB, GMD_BOSS1_ARC);
	tex_amb	= ObjDataLoadAmbIndex(NULL, IDB_BOSS01_BOSS01_TEX_AMB, GMD_BOSS1_ARC);
	
	// モデル構築
	gm_boss1_obj_3d_list	=
		GmGameDBuildRegBuildModel((AMS_AMB_HEADER*)mdl_amb, (AMS_AMB_HEADER*)tex_amb,
								  NND_DRAWOBJ_SHADER_USER_PROFILE_TOON/*draw_flag*/);
	
	// モーション
	ObjDataLoadAmbIndex(ObjDataGet(GMD_DWORK_NO_BOSS_01_BODY_MTN),
						IDB_BOSS01_BOSS01_BODY_MTN_AMB, GMD_BOSS1_ARC);
	ObjDataLoadAmbIndex(ObjDataGet(GMD_DWORK_NO_BOSS_01_CHAIN_MTN),
						IDB_BOSS01_BOSS01_CHAIN_MTN_AMB, GMD_BOSS1_ARC);
	ObjDataLoadAmbIndex(ObjDataGet(GMD_DWORK_NO_BOSS_01_EGG_MTN),
						IDB_BOSS01_BOSS01_EGG_MTN_AMB, GMD_BOSS1_ARC);
	
	
	// 衝撃波00
	ObjDataLoadAmbIndex(ObjDataGet(GMD_DWORK_NO_BOSS_01_EF_SW00_ES),
						IDB_BOSS01_EFF_BOSSZ1_00_AME,
						GMD_BOSS1_ARC);
	
	
	// 衝撃波01
	ObjDataLoadAmbIndex(ObjDataGet(GMD_DWORK_NO_BOSS_01_EF_SW01_ES),
						IDB_BOSS01_EFF_BOSSZ1_01_AME,
						GMD_BOSS1_ARC);
	
	// 衝撃波02
	ObjDataLoadAmbIndex(ObjDataGet(GMD_DWORK_NO_BOSS_01_EF_SW02_ES),
						IDB_BOSS01_EFF_BOSSZ1_02_AME,
						GMD_BOSS1_ARC);
	
	// エフェクトをVRAMにロード
	// 衝撃波00
	GmEfctBossBuildSingleDataReg(IDB_BOSS01_EFF_BS1_TEX_AMB,
								 ObjDataGet(GMD_DWORK_NO_BOSS_01_EF_SW_AMBTEX),
								 ObjDataGet(GMD_DWORK_NO_BOSS_01_EF_SW_TEXLIST),
								 0, NULL, NULL,	//モデルなし
								 GMD_BOSS1_ARC);
	
	// 衝撃波01
	GmEfctBossBuildSingleDataReg(IDB_BOSS01_EFF_BS1_TEX_AMB,
								 ObjDataGet(GMD_DWORK_NO_BOSS_01_EF_SW_AMBTEX),
								 ObjDataGet(GMD_DWORK_NO_BOSS_01_EF_SW_TEXLIST),
								 0, NULL, NULL,	//モデルなし
								 GMD_BOSS1_ARC);
	
	// 衝撃波02
	GmEfctBossBuildSingleDataReg(IDB_BOSS01_EFF_BS1_TEX_AMB,
								 ObjDataGet(GMD_DWORK_NO_BOSS_01_EF_SW_AMBTEX),
								 ObjDataGet(GMD_DWORK_NO_BOSS_01_EF_SW_TEXLIST),
								 0, NULL, NULL,	//モデルなし
								 GMD_BOSS1_ARC);
}


// =======================================================================
// GmBoss1Flush
/*!
  ボス１ データ片付け
 */
// =======================================================================
void GmBoss1Flush(void)
{
	AMS_AMB_HEADER	*mdl_amb;
	
	// エフェクト解放
	GmEfctBossFlushSingleDataInit();	// ボス専用EFFフラッシュ開始
	
	// 衝撃波02
	ObjDataRelease(ObjDataGet(GMD_DWORK_NO_BOSS_01_EF_SW02_ES));
	// 衝撃波01
	ObjDataRelease(ObjDataGet(GMD_DWORK_NO_BOSS_01_EF_SW01_ES));
	// 衝撃波00
	ObjDataRelease(ObjDataGet(GMD_DWORK_NO_BOSS_01_EF_SW00_ES));
	
	// モーション
	ObjDataRelease(ObjDataGet(GMD_DWORK_NO_BOSS_01_EGG_MTN));
	ObjDataRelease(ObjDataGet(GMD_DWORK_NO_BOSS_01_CHAIN_MTN));
	ObjDataRelease(ObjDataGet(GMD_DWORK_NO_BOSS_01_BODY_MTN));
	
	// モデル解放
	mdl_amb	= (AMS_AMB_HEADER*)ObjDataLoadAmbIndex(NULL, IDB_BOSS01_BOSS01_MDL_AMB, GMD_BOSS1_ARC);
	
	GmGameDBuildRegFlushModel(gm_boss1_obj_3d_list, mdl_amb->file_num);
	
	gm_boss1_obj_3d_list	= NULL;
}



// =======================================================================
// GmBoss1Init
/*!
  ボス１（管理）初期化
  
  @param    eve_rec [io]    レコードポインタ
  @param    pos_x   [in]    出現Ｘ座標
  @param    pos_y   [in]    出現Ｙ座標
  @param    type    [in]    処理内容タイプ 通常は0
  
  @return	オブジェクトワーク
 */
// =======================================================================
OBS_OBJECT_WORK* GmBoss1Init(GMS_EVE_RECORD_EVENT *eve_rec,
							 fx32 pos_x, fx32 pos_y, u8 type)
{
	UNREFERENCED_PARAMETER(type);
	
	OBS_OBJECT_WORK	*obj_mgr;
	GMS_BOSS1_MGR_WORK	*mgr_work;
	
	// オブジェクト作成
	obj_mgr	= GMM_ENEMY_CREATE_WORK(eve_rec,
										pos_x, pos_y,
										sizeof(GMS_BOSS1_MGR_WORK),
										"BOSS1_MGR");
	
	mgr_work	= (GMS_BOSS1_MGR_WORK*)obj_mgr;
	
	// ワーク設定
	obj_mgr->flag	|= OBD_OBJECT_NOCLIP;
	obj_mgr->disp_flag	|= OBD_DISP_NODISP;
	obj_mgr->move_flag	|= (OBD_MOVE_NOMOVE | OBD_MOVE_NOCOL);
	
	// ライフ設定
	if (GmBsCmnIsFinalZoneType(obj_mgr)) {
		mgr_work->life	= GMD_BOSS1_FINAL_LIFE;
	}
	else {
		mgr_work->life	= GMD_BOSS1_LIFE;
	}
	
	// 処理関数設定
	obj_mgr->ppFunc	= gmBoss1MgrWaitLoad;
	
	return obj_mgr;
}

// =======================================================================
// GmBoss1BodyInit
/*!
  ボス１本体初期化
  
  @param    eve_rec [io]    レコードポインタ
  @param    pos_x   [in]    出現Ｘ座標
  @param    pos_y   [in]    出現Ｙ座標
  @param    type    [in]    処理内容タイプ 通常は0
  
  @return	オブジェクトワーク
 */
// =======================================================================
OBS_OBJECT_WORK* GmBoss1BodyInit(GMS_EVE_RECORD_EVENT *eve_rec,
								 fx32 pos_x, fx32 pos_y, u8 type)
{
	UNREFERENCED_PARAMETER(type);
	
	OBS_OBJECT_WORK	*obj_work;
	GMS_ENEMY_3D_WORK	*ene_3d;
	GMS_BOSS1_BODY_WORK	*body_work;
	
	// オブジェクト作成
	obj_work	= GMM_ENEMY_CREATE_WORK(eve_rec,
										pos_x, pos_y,
										sizeof(GMS_BOSS1_BODY_WORK),
										"BOSS1_BODY");
	
	ene_3d	= (GMS_ENEMY_3D_WORK*)obj_work;
	body_work	= (GMS_BOSS1_BODY_WORK*)obj_work;
	
	// 初期位置設定
	obj_work->pos.y	= GMD_BOSS1_BODY_START_POS_Y;
	
	// Z位置設定
	obj_work->pos.z	= GMD_BOSS1_DEFAULT_POS_Z;
	
	// 通常攻撃高度設定
	body_work->atk_nml_alt	= GMD_BOSS1_BODY_DEFAULT_ALTITUDE;	//!< 通常攻撃以外でも基本の高度として使用
	
	// ワーク設定
	obj_work->flag	|= OBD_OBJECT_NOCLIP;
	obj_work->disp_flag	|= OBD_DISP_HFLIP | OBD_DISP_REPEAT | OBD_DISP_NODIRFLIP;
	obj_work->move_flag	|= OBD_MOVE_NOCOLFIELD;
	obj_work->move_flag	&= ~OBD_MOVE_FALL;
	
	// 最初はホーミングの対象からはずす
	ene_3d->ene_com.enemy_flag	|= GMD_ENEMY_FLAG_NOHOMING;
	
	// ライフ設定
	ene_3d->ene_com.vit	= 1;	// 使用しないが念のため値を設定
	
	// 地形当たり不要
	
	// 食らい当たり設定
	gmBoss1BodySetDmgRectSizeToDefault(body_work);
	ene_3d->ene_com.rect_work[GMD_ENEMY_RECT_DEF].ppDef	= gmBoss1BodyDamageDefFunc;
	
	// 攻撃当たり設定
	// 矩形サイズは使用箇所にて設定
	ene_3d->ene_com.rect_work[GMD_ENEMY_RECT_ATK].flag	|= OBD_RECT_NOHIT;	// 最初は当たらない
	
	// 本体モデル初期化
	ObjObjectCopyAction3dNNModel(obj_work,
								 &gm_boss1_obj_3d_list[IDB_BOSS01_MDL_B01_BODY_ZNO],
								 &ene_3d->obj_3d);
	
	// 本体モーションロード
	ObjObjectAction3dNNMotionLoad(obj_work,
								  0,
								  TRUE,
								  ObjDataGet(GMD_DWORK_NO_BOSS_01_BODY_MTN),
								  NULL,
								  0,
								  NULL);
	
	// Wii向けトゥーン設定
	ObjDrawObjectSetToon(obj_work);
	
	// ブレンド速度設定
	obj_work->obj_3d->blend_spd	= 0.125f;
	
	// ユーザ描画ステートを使用
	obj_work->disp_flag	|= OBD_DISP_DRAWSTATE;
	
	// メイン処理設定
	obj_work->ppFunc	= gmBoss1BodyWaitSetup;
	
	// 初期アクション設定
	gmBoss1BodyChangeState(body_work, GME_BOSS1_BODY_STATE_NOP);
	
	// 専用描画処理設定
	obj_work->ppOut	= gmBoss1BodyOutFunc;
	
	// 終了処理差し替え
	mtTaskChangeTcbDestructor(obj_work->tcb, gmBoss1BodyExit);
	
#if _IPHONE
	// 専用ライト設定
	obj_work->obj_3d->use_light_flag &= ~OBD_LIGHT_USE_FLAG_0;
	obj_work->obj_3d->use_light_flag |= OBD_LIGHT_USE_FLAG_6;
#endif // _IPHONE

	return obj_work;
}


// =======================================================================
// GmBoss1ChainInit
/*!
  ボス１ 鎖 初期化
  
  @param    eve_rec [io]    レコードポインタ
  @param    pos_x   [in]    出現Ｘ座標
  @param    pos_y   [in]    出現Ｙ座標
  @param    type    [in]    処理内容タイプ 通常は0
  
  @return	オブジェクトワーク
 */
// =======================================================================
OBS_OBJECT_WORK* GmBoss1ChainInit(GMS_EVE_RECORD_EVENT *eve_rec,
								  fx32 pos_x, fx32 pos_y, u8 type)
{
	UNREFERENCED_PARAMETER(type);
	
	OBS_OBJECT_WORK	*obj_work;
	GMS_ENEMY_3D_WORK	*ene_3d;
	GMS_BOSS1_CHAIN_WORK	*chain_work;
	
	// オブジェクト作成
	obj_work	= GMM_ENEMY_CREATE_WORK(eve_rec,
										pos_x, pos_y,
										sizeof(GMS_BOSS1_CHAIN_WORK),
										"BOSS1_CHAIN");
	
	ene_3d	= (GMS_ENEMY_3D_WORK*)obj_work;
	chain_work	= (GMS_BOSS1_CHAIN_WORK*)obj_work;
	
	
	// ライフ無し
	
	// 地形当たり無し
	obj_work->move_flag	|= OBD_MOVE_NOCOL;
	
	// 鎖モデル初期化
	ObjObjectCopyAction3dNNModel(obj_work,
								 &gm_boss1_obj_3d_list[IDB_BOSS01_MDL_B01_CHAIN_ZNO],
								 &ene_3d->obj_3d);
	
	// 鎖モーションロード
	ObjObjectAction3dNNMotionLoad(obj_work,
								  0,//登録モーションID
								  TRUE,//merge
								  ObjDataGet(GMD_DWORK_NO_BOSS_01_CHAIN_MTN),
								  NULL,
								  0,
								  NULL);
	
	// Wii向けトゥーン設定
	ObjDrawObjectSetToon(obj_work);
	
	// ブレンド速度設定
	obj_work->obj_3d->blend_spd	= 0.125f;
	
	// メイン処理設定
	obj_work->ppFunc	= gmBoss1ChainWaitSetup;
	
	// ワーク設定
	obj_work->flag	|= OBD_OBJECT_NOCLIP;
	obj_work->disp_flag	|= OBD_DISP_REPEAT;
	
	// ホーミングの対象からはずす
	ene_3d->ene_com.enemy_flag	|= GMD_ENEMY_FLAG_NOHOMING;
	
	// 当たり矩形設定
	ObjRectWorkSet(&ene_3d->ene_com.rect_work[GMD_ENEMY_RECT_ATK],
				   GMD_BOSS1_CHAIN_ATK_RECT_SIZE_LEFT,
				   GMD_BOSS1_CHAIN_ATK_RECT_SIZE_TOP,
				   GMD_BOSS1_CHAIN_ATK_RECT_SIZE_RIGHT,
				   GMD_BOSS1_CHAIN_ATK_RECT_SIZE_BOTTOM);
	ene_3d->ene_com.rect_work[GMD_ENEMY_RECT_ATK].ppHit	= gmBoss1ChainAtkHitFunc;
	ene_3d->ene_com.rect_work[GMD_ENEMY_RECT_DEF].flag	|= OBD_RECT_NOHIT;	// 食らわない
	ene_3d->ene_com.rect_work[GMD_ENEMY_RECT_BODY].flag |= OBD_RECT_NOHIT;	// 体当たりは使わない
	
	// 専用描画処理設定
	obj_work->ppOut	= gmBoss1ChainOutFunc;
	
	// 終了処理差し替え
	mtTaskChangeTcbDestructor(obj_work->tcb, gmBoss1ChainExit);
		
#if _IPHONE
	// 専用ライト設定
	obj_work->obj_3d->use_light_flag &= ~OBD_LIGHT_USE_FLAG_0;
	obj_work->obj_3d->use_light_flag |= OBD_LIGHT_USE_FLAG_6;
#endif // _IPHONE

	return obj_work;
}


// =======================================================================
// GmBoss1EggInit
/*!
  ボス１ エッグマン 初期化
  
  @param    eve_rec [io]    レコードポインタ
  @param    pos_x   [in]    出現Ｘ座標
  @param    pos_y   [in]    出現Ｙ座標
  @param    type    [in]    処理内容タイプ 通常は0
  
  @return	オブジェクトワーク
 */
// =======================================================================


// =======================================================================

static int mppBoss1EffSweat_preloadTimer = 0;

void mppBoss1EffSweat_EndPreload(OBS_OBJECT_WORK *obj_work)
{
	GMS_BOSS1_EGG_WORK	*parent_egg	= (GMS_BOSS1_EGG_WORK*)obj_work->parent_obj;
	
	MTM_ASSERT(parent_egg);
	
	if (--mppBoss1EffSweat_preloadTimer<=0) {
		obj_work->flag	|= OBD_OBJECT_TASKCLEAR;
		ObjDrawKillAction3DES(obj_work);
	}
	
}

// =======================================================================
void mppBoss1EffSweat_BeginPreload(GMS_BOSS1_EGG_WORK *egg_work)
{
	GMS_EFFECT_3DES_WORK	*efct_work;
	efct_work	= GmEfctCmnEsCreate(GMM_BS_OBJ(egg_work), GME_EFCT_CMN_IDX_SWEAT);
	
	// 位置調整
	GmEffect3DESAddDispOffset(efct_work, 0, GMD_BOSS1_EFF_SWEAT_DISP_OFST_Y, 0);
	
	GMM_BS_OBJ(efct_work)->ppFunc	= mppBoss1EffSweat_EndPreload;
	
	mppBoss1EffSweat_preloadTimer = 15;//frames
}



OBS_OBJECT_WORK* GmBoss1EggInit(GMS_EVE_RECORD_EVENT *eve_rec,
								fx32 pos_x, fx32 pos_y, u8 type)
{
	UNREFERENCED_PARAMETER(type);
	
	OBS_OBJECT_WORK	*obj_work;
	GMS_ENEMY_3D_WORK	*ene_3d;
	GMS_BOSS1_EGG_WORK	*egg_work;
	
	// オブジェクト作成
	obj_work	= GMM_ENEMY_CREATE_WORK(eve_rec,
										pos_x, pos_y,
										sizeof(GMS_BOSS1_EGG_WORK),
										"BOSS1_EGG");
	
	ene_3d	= (GMS_ENEMY_3D_WORK*)obj_work;
	egg_work	= (GMS_BOSS1_EGG_WORK*)obj_work;
	
	// ライフ無し
	
	{{//qqq[11]//preload sweat effect
		mppBoss1EffSweat_BeginPreload(egg_work);
	}}
	
	// 地形当たり無し
	obj_work->move_flag	|= OBD_MOVE_NOCOL;
	
	ObjObjectCopyAction3dNNModel(obj_work,
								 &gm_boss1_obj_3d_list[IDB_BOSS01_MDL_EGGMAN_ZNO],
								 &ene_3d->obj_3d);
	
	// エッグマンモーションロード
	ObjObjectAction3dNNMotionLoad(obj_work,
								  0,
								  TRUE,
								  ObjDataGet(GMD_DWORK_NO_BOSS_01_EGG_MTN),
								  NULL,
								  0,
								  NULL);
	
	// Wii向けトゥーン設定
	ObjDrawObjectSetToon(obj_work);
	
	// ブレンド速度設定
	obj_work->obj_3d->blend_spd	= 0.125f;
	
	// ユーザ描画ステートを使用
	obj_work->disp_flag	|= OBD_DISP_DRAWSTATE;
	
	// メイン処理設定
	obj_work->ppFunc	= gmBoss1EggWaitSetup;
	
	// ワーク設定
	obj_work->flag	|= OBD_OBJECT_NOCLIP;
	obj_work->disp_flag	|= OBD_DISP_REPEAT;
	obj_work->disp_flag	|= OBD_DISP_NODIRFLIP;
	
	// ホーミングの対象からはずす
	ene_3d->ene_com.enemy_flag	|= GMD_ENEMY_FLAG_NOHOMING;
	
	// 終了処理差し替え
	mtTaskChangeTcbDestructor(obj_work->tcb, gmBoss1EggExit);
	
#if _IPHONE
	// 専用ライト設定
	obj_work->obj_3d->use_light_flag &= ~OBD_LIGHT_USE_FLAG_0;
	obj_work->obj_3d->use_light_flag |= OBD_LIGHT_USE_FLAG_6;
#endif // _IPHONE

	return obj_work;
}


/*------ Static Functions ----------------------------------------------*/

// ############################################################################
// 共通
// ############################################################################
// ============================================================================
// 補助関数
// ============================================================================
// =======================================================================
// gmBoss1SetPartTextureBurnt
/*!
  パーツのテクスチャを黒こげタイプにする
  
  @param obj_work	[io]	オブジェクトワーク
  
  @note
  スロット0のテクスチャオフセットをu+0.5しています。
 */
// =======================================================================
void gmBoss1SetPartTextureBurnt(OBS_OBJECT_WORK *obj_work)
{
	MTM_ASSERT(obj_work);
	MTM_ASSERT(obj_work->obj_3d);
	MTM_ASSERT(obj_work->disp_flag & OBD_DISP_DRAWSTATE);
	
	obj_work->obj_3d->drawflag |= NND_DRAWOBJ_MATCTRL_TEXOFFSET;
	obj_work->obj_3d->draw_state.texoffset[0].mode	= NNE_MATCTRLMODE_ADD;
	obj_work->obj_3d->draw_state.texoffset[0].u	= 0.5f;
}

// =======================================================================
// gmBoss1IsScrollLockBusy
/*!
  スクロールロック作動中判定
  
  @retval TRUE	スクロールロック作動中
  @retval FALSE	スクロールロック作動中でない（未作動or完了済み）
  
  @note
  スクロールロック発動からロック完了までの処理中か判定します。
 */
// =======================================================================
static BOOL gmBoss1IsScrollLockBusy(void)
{
	if (g_gm_main_system.game_flag & GMD_GAME_FLAG_SCR_LIMIT_BUSY) {
		return TRUE;
	}
	
	return FALSE;
}

// =======================================================================
// gmBoss1Init1ShotTimer
/*!
  1ショットタイマ 初期化
  
  @param one_shot_timer	[io]	1ショットタイマワーク
 */
// =======================================================================
void gmBoss1Init1ShotTimer(GMS_BOSS1_1SHOT_TIMER *one_shot_timer, Uint32 frame)
{
	MTM_ASSERT(one_shot_timer);
	
	one_shot_timer->timer	= frame;
	one_shot_timer->is_active	= TRUE;
}

// =======================================================================
// gmBoss1Update1ShotTimer
/*!
  1ショットタイマ 更新
  
  @param one_shot_timer	[io]	1ショットタイマワーク
  
  @retval TRUE	指定フレーム到達
  @retval FALSE	指定フレームに満たないor超過
  
  @note
  既定フレーム経過したら一度だけTRUEを返すタイマの初期化
 */
// =======================================================================
BOOL gmBoss1Update1ShotTimer(GMS_BOSS1_1SHOT_TIMER *one_shot_timer)
{
	MTM_ASSERT(one_shot_timer);
	
	if (one_shot_timer->is_active == FALSE) {
		return FALSE;
	}
	
	if (one_shot_timer->timer) {
		one_shot_timer->timer--;
	}
	else {
		one_shot_timer->is_active	= FALSE;
		return TRUE;
	}
	
	return FALSE;
}


// ############################################################################
// ボス1 管理
// ############################################################################

// ============================================================================
// 補助関数
// ============================================================================
// =======================================================================
// gmBoss1MgrIncObjCreateCount
/*!
  オブジェクト生成カウント 増加
  
  @param mgr_work	[io]	管理ワーク
  
  @note
  ボス専用データアーカイブを参照するオブジェクトを生成する際に呼び出してください。
 */
// =======================================================================
void gmBoss1MgrIncObjCreateCount(GMS_BOSS1_MGR_WORK *mgr_work)
{
	MTM_ASSERT(mgr_work->obj_create_cnt >= 0);
	
	mgr_work->obj_create_cnt	+= 1;
}

// =======================================================================
// gmBoss1MgrDecObjCreateCount
/*!
  オブジェクト生成カウント 減少
  
  @param mgr_work	[io]	管理ワーク
  
  @note
  ボス専用データアーカイブを参照するオブジェクトのデストラクト時に呼び出してください。
 */
// =======================================================================
void gmBoss1MgrDecObjCreateCount(GMS_BOSS1_MGR_WORK *mgr_work)
{
	MTM_ASSERT(mgr_work->obj_create_cnt > 0);
	
	mgr_work->obj_create_cnt	-= 1;
}

// =======================================================================
// gmBoss1MgrIsAllCreatedObjDeleted
/*!
  生成カウントされたオブジェクトが全て消去されたか判定
  
  @param mgr_work	[io]	管理ワーク
 */
// =======================================================================
BOOL gmBoss1MgrIsAllCreatedObjDeleted(GMS_BOSS1_MGR_WORK *mgr_work)
{
	MTM_ASSERT(mgr_work->obj_create_cnt >= 0);
	
	if (mgr_work->obj_create_cnt <= 0) {
		return TRUE;
	}
	else {
		return FALSE;
	}
}

// ============================================================================
// 制御処理
// ============================================================================
// =======================================================================
// gmBoss1MgrWaitLoad
/*!
  本体 ロード・ビルド完了待ち
  
  @param obj_work	[io]	オブジェクトワーク
 */
// =======================================================================
void gmBoss1MgrWaitLoad(OBS_OBJECT_WORK *obj_work)
{
	BOOL	is_load_end	= FALSE;
	
	if (GmBsCmnIsFinalZoneType(obj_work)) {
		// ボスラッシュ時データロード待ち
		if (GmMainDatLoadBossBattleLoadCheck(GMD_GAMEDAT_LOAD_BOSS_TYPE_1)) {
			is_load_end	= TRUE;
		}
	}
	else {
		// 通常ステージの場合はロード完了待ちを行わない
		is_load_end	= TRUE;
	}
	
	// データロード・ビルドが完了した時点で各パーツを生成
	if (is_load_end) {
		GMS_BOSS1_MGR_WORK	*mgr_work	= (GMS_BOSS1_MGR_WORK*)obj_work;
		OBS_OBJECT_WORK	*obj_body;
		OBS_OBJECT_WORK	*obj_chain;
		OBS_OBJECT_WORK	*obj_egg;
		GMS_BOSS1_BODY_WORK	*body_work;
		GMS_BOSS1_CHAIN_WORK	*chain_work;
		GMS_BOSS1_EGG_WORK	*egg_work;
		
		// 各パーツ生成
		
		obj_body	= GmEventMgrLocalEventBirth(GMD_EVENT_ID_BOSS1_BODY,
												obj_work->pos.x, obj_work->pos.y,
												0,//flag
												0,0,0,0,
												0);
		gmBoss1MgrIncObjCreateCount(mgr_work);	// オブジェクト生成数加算
		
		obj_chain	= GmEventMgrLocalEventBirth(GMD_EVENT_ID_BOSS1_CHAIN,
												obj_work->pos.x, obj_work->pos.y,
												0,//flag
												0,0,0,0,
												0);
		gmBoss1MgrIncObjCreateCount(mgr_work);	// オブジェクト生成数加算
		
		obj_egg		= GmEventMgrLocalEventBirth(GMD_EVENT_ID_BOSS1_EGG,
												obj_work->pos.x, obj_work->pos.y,
												0,//flag
												0,0,0,0,
												0);
		gmBoss1MgrIncObjCreateCount(mgr_work);	// オブジェクト生成数加算
		
		body_work	= (GMS_BOSS1_BODY_WORK*)obj_body;
		chain_work	= (GMS_BOSS1_CHAIN_WORK*)obj_chain;
		egg_work	= (GMS_BOSS1_EGG_WORK*)obj_egg;
		
		// 本体を管理の子に設定
		mgr_work->body_work	= body_work;
		
		// 管理ワークの参照設定
		body_work->mgr_work	= mgr_work;
		chain_work->mgr_work	= mgr_work;
		egg_work->mgr_work	= mgr_work;
		
		// 各パーツの親設定
		obj_body->parent_obj	= obj_work;
		obj_chain->parent_obj	= obj_body;
		obj_egg->parent_obj	= obj_body;
		
		
		// 各パーツへの参照を設定
		body_work->parts_objs[GME_BOSS1_PART_IDX_BODY]	= obj_body;
		body_work->parts_objs[GME_BOSS1_PART_IDX_CHAIN]	= obj_chain;
		body_work->parts_objs[GME_BOSS1_PART_IDX_EGG]	= obj_egg;
		
		// 処理関数設定
		obj_work->ppFunc	= gmBoss1MgrWaitSetup;
	}
}

// =======================================================================
// gmBoss1MgrWaitSetup
/*!
  本体 パーツ生成完了待ち
  
  @param obj_work	[io]	オブジェクトワーク
 */
// =======================================================================
void gmBoss1MgrWaitSetup(OBS_OBJECT_WORK *obj_work)
{
	GMS_BOSS1_MGR_WORK	*mgr_work	= (GMS_BOSS1_MGR_WORK*)obj_work;
	GMS_BOSS1_BODY_WORK	*body_work	= mgr_work->body_work;
	BOOL	result	= TRUE;
	
	// 本体パーツの処理で、全てのパーツのアクションをまとめて設定しないといけないので
	// パーツが一通りそろっているかチェック
	for (Sint32 i = 0; i < GME_BOSS1_PART_IDX_MAX; ++i) {
		if (body_work->parts_objs[i] == NULL) {
			result	= FALSE;
		}
	}
	
	if (result) {
		mgr_work->flag	|= GMD_BOSS1_MGR_FLAG_SETUP_END;
		obj_work->ppFunc	= gmBoss1MgrMain;
	}
}

// =======================================================================
// gmBoss1MgrMain
/*!
  管理 メイン処理関数
  
  @param obj_work	[io]	オブジェクトワーク
 */
// =======================================================================
void gmBoss1MgrMain(OBS_OBJECT_WORK *obj_work)
{
	GMS_BOSS1_MGR_WORK	*mgr_work	= (GMS_BOSS1_MGR_WORK*)obj_work;
	
	if (mgr_work->flag & GMD_BOSS1_MGR_FLAG_CLEAR_BOSS) {
		if (mgr_work->body_work) {
			GMM_BS_OBJ(mgr_work->body_work)->flag	|= OBD_OBJECT_TASKCLEAR_REQUEST;
			mgr_work->body_work	= NULL;
		}
		
		if (GmBsCmnIsFinalZoneType(obj_work)) {
			// ファイナルゾーンではデータ解放を行う
			obj_work->ppFunc	= gmBoss1MgrWaitRelease;
		}
	}
}

// =======================================================================
// gmBoss1MgrWaitRelease
/*!
  管理 解放待ち処理
 */
// =======================================================================
void gmBoss1MgrWaitRelease(OBS_OBJECT_WORK *obj_work)
{
	GMS_BOSS1_MGR_WORK	*mgr_work	= (GMS_BOSS1_MGR_WORK*)obj_work;
	
	// REMINDER :
	// 各パーツやエフェクトのオブジェクトの生成時に生成数をカウント ＆
	// それらのデストラクタに生成数デクリメント処理を仕込んでおく
	// この関数内で生成数が0になるのを待って、0になったら全て消去されたと判定する
	
	if (gmBoss1MgrIsAllCreatedObjDeleted(mgr_work)) {
		GMS_ENEMY_COM_WORK	*ene_com	= (GMS_ENEMY_COM_WORK*)obj_work;
		
		// ファイナルゾーンでは自分も消去
		ene_com->enemy_flag	|= GMD_ENEMY_FLAG_DIE;	// 復活しないようにする
		obj_work->flag	|= OBD_OBJECT_TASKCLEAR;
		
		// データ開放処理開始
		GmGameDatReleaseBossBattleStart(GMD_GAMEDAT_LOAD_BOSS_TYPE_1);
		
		// スクロールロック解除（ファイナルゾーンでは左以外全部解除）
		GmGmkCamScrLimitRelease(GMD_GMK_SCR_LMT_RELEASE_TOP |
								GMD_GMK_SCR_LMT_RELEASE_RIGHT |
								GMD_GMK_SCR_LMT_RELEASE_BOTTOM);
	}
}

// ############################################################################
// ボス1 本体
// ############################################################################

// =======================================================================
// gmBoss1BodyExit
/*!
  本体終了処理
  
  @note
  ノードマトリクス取得関連バッファを解放しています。
 */
// =======================================================================
void gmBoss1BodyExit(MTS_TASK_TCB *tcb)
{
	OBS_OBJECT_WORK	*obj_work	= (OBS_OBJECT_WORK*)mtTaskGetTcbWork(tcb);
	GMS_BOSS1_BODY_WORK	*body_work	= (GMS_BOSS1_BODY_WORK*)obj_work;
	
	// オブジェクト生成数デクリメント
	gmBoss1MgrDecObjCreateCount(body_work->mgr_work);
	
	// ボスモーションコールバックシステム解除・クリア
	GmBsCmnClearBossMotionCBSystem(obj_work);
	
	// ノードマトリクス取得関連解放
	GmBsCmnDeleteSNMWork(&body_work->snm_work);
	
	// ノードマトリクス操作解除・クリア
	GmBsCmnClearCNMCb(obj_work);
	
	// ノードマトリクス操作処理管理ワーク削除
	GmBsCmnDeleteCNMMgrWork(&body_work->cnm_mgr_work);
	
	// エネミー標準終了処理
	GmEnemyDefaultExit(tcb);
}


// ============================================================================
// 補助関数
// ============================================================================
// =======================================================================
// gmBoss1BodySetActionWhole
/*!
  ボス１全体アクション設定
  
  @param body_work	[io]	本体ワーク
  @param act_id		[in]	全体アクションインデックス(GME_BOSS1_ACT_ID_XXX)
  
  @note
  本体と、本体を構成している各パーツのアクションをまとめて設定します。
 */
// =======================================================================
void gmBoss1BodySetActionWhole(GMS_BOSS1_BODY_WORK *body_work,
							   GME_BOSS1_ACT_ID act_id, BOOL force_change/*=FALSE*/)
{
	const GMS_BOSS1_PART_ACT_INFO	*pt_act_info	= gm_boss1_act_id_tbl[act_id];
	
	// 設定済みなら何もしない
	if (!force_change &&
		body_work->whole_act_id == act_id) {
		return;
	}
	
	// アクションID設定
	body_work->whole_act_id	= act_id;
	
	// 構成パーツのアクション設定を反映
	for (Sint32 i = 0; i < GME_BOSS1_PART_IDX_MAX; ++i) {
		
		if (body_work->parts_objs[i] == NULL) {
			continue;
		}
		
		// エッグマンについては、独立アクション中ならばアクション変更しない
		if (i == GME_BOSS1_PART_IDX_EGG) {
			GMS_BOSS1_EGG_WORK	*egg_work	= (GMS_BOSS1_EGG_WORK*)body_work->parts_objs[i];
			
			// 戻るべきモーション番号を記録しておく
			body_work->egg_revert_mtn_id	= pt_act_info[i].act_id;
			
			if (egg_work->flag & GMD_BOSS1_EGG_FLAG_INDP_ACT_SET) {
				continue;
			}
		}
		
		// 継続フラグがオフの時のみ、新たなアクションを設定
		if (FALSE == pt_act_info[i].is_maintain) {
			GmBsCmnSetAction(body_work->parts_objs[i],
							 pt_act_info[i].act_id,
							 pt_act_info[i].is_repeat,
							 pt_act_info[i].is_blend);
		}
		else if (pt_act_info[i].is_repeat) {
			// リピートフラグは継続フラグの有無に関わらず反映
			GMM_BS_OBJ(body_work)->disp_flag	|= OBD_DISP_REPEAT;
		}
		
		// 手動マージチェック
		if (pt_act_info[i].is_blend) {
			if (pt_act_info[i].is_merge_manual) {
				// 鎖のみサポート
				if (i == GME_BOSS1_PART_IDX_CHAIN) {
					GMS_BOSS1_CHAIN_WORK	*chain_work	= (GMS_BOSS1_CHAIN_WORK*)body_work->parts_objs[GME_BOSS1_PART_IDX_CHAIN];
					chain_work->flag |= GMD_BOSS1_CHAIN_FLAG_MANUAL_MOTION_MERGE;
					GMM_BS_OBJ(chain_work)->disp_flag	|= OBD_DISP_REPEAT;	// リピートを強制オン
				}
				else {
					MTM_ASSERT(!"gmBoss1.cpp::gmBoss1BodySetActionWhole() manual merge not supported\n");
				}
			}
		}
		
		// モーション速度設定
		body_work->parts_objs[i]->obj_3d->speed[0]	= pt_act_info[i].mtn_spd;
		
		// ブレンド速度設定
		body_work->parts_objs[i]->obj_3d->blend_spd	= pt_act_info[i].blend_spd;
	}
}

// =======================================================================
// gmBoss1BodySetSuspendAction
/*!
  再生停滞設定
  
  @param body_work		[io]	本体ワーク
  @param part_idx		[in]	パーツ番号
  @param suspend_time	[in]	停滞時間
  
  @note
  既定時間再生が停止されます。
  ブレンドは行われます（i.e. 遷移元モーションの再生は継続する）。
 */
// =======================================================================
void gmBoss1BodySetSuspendAction(GMS_BOSS1_BODY_WORK *body_work,
								 GME_BOSS1_PART_IDX part_idx,
								 Uint32 suspend_time)
{
	OBS_ACTION3D_NN_WORK	*obj_3d	= body_work->parts_objs[part_idx]->obj_3d;
	
	// 現状、鎖のみ対応
	// （エッグマンは独立アクションがあり、復帰が煩雑なので
	//   必要な場合は対応方法について要検討）
	MTM_ASSERT(part_idx == GME_BOSS1_PART_IDX_CHAIN);
	
	// 再生停止
	obj_3d->speed[0]	= 0;	// 復帰後の再生速度はアクションIDテーブルを参照して設定されます
	
	// 停滞有効
	body_work->mtn_suspend[part_idx].is_suspended	= TRUE;
	
	// 停滞時間設定
	body_work->mtn_suspend[part_idx].suspend_timer	= suspend_time;
}

// =======================================================================
// gmBoss1BodyUpdateSuspendAction
/*!
  再生停滞更新
  
  @param body_work	[io]	本体ワーク
  
  @note
  停滞タイマを更新し、タイマが満了したパーツについては停滞から復帰させます。
 */
// =======================================================================
void gmBoss1BodyUpdateSuspendAction(GMS_BOSS1_BODY_WORK *body_work)
{
	for (Sint32 i = 0; i < GME_BOSS1_PART_IDX_MAX; ++i) {
		GMS_BOSS1_MTN_SUSPEND_WORK	*suspend_work	= &body_work->mtn_suspend[i];
		
		if (suspend_work->is_suspended) {
			
			// 現状、鎖のみ対応
			// （エッグマンは独立アクションがあり、復帰が煩雑なので
			//   必要な場合は対応方法について要検討）
			MTM_ASSERT(i == GME_BOSS1_PART_IDX_CHAIN);
			
			// 既定時間停滞
			if (suspend_work->suspend_timer) {
				suspend_work->suspend_timer--;
			}
			else {
				
				// 再生速度復帰
				body_work->parts_objs[i]->obj_3d->speed[0]	=
					gm_boss1_act_id_tbl[body_work->whole_act_id][i].mtn_spd;
				
				// 停滞無効
				suspend_work->is_suspended	= FALSE;
			}
		}
	}
}


// =======================================================================
// gmBoss1BodyCheckChainMotionMergeEnd
/*!
  鎖パーツの手動モーションマージ終了チェック
  
  @param body_work	[in]	本体ワーク
  
  @retval TRUE	手動モーションマージ終了
  @retval FALSE	手動モーションマージ中
  
  @note
  鎖パーツが手動モーションマージ中がそうでないかを判定しています。
 */
// =======================================================================
BOOL gmBoss1BodyCheckChainMotionMergeEnd(GMS_BOSS1_BODY_WORK *body_work)
{
	GMS_BOSS1_CHAIN_WORK	*chain_work	= (GMS_BOSS1_CHAIN_WORK*)body_work->parts_objs[GME_BOSS1_PART_IDX_CHAIN];
	
	if (chain_work->flag & GMD_BOSS1_CHAIN_FLAG_MANUAL_MOTION_MERGE) {
		return FALSE;
	}
	else {
		return TRUE;
	}
}

// =======================================================================
// gmBoss1BodySetNoHitTime
/*!
  ヒット無効時間設定
  
  @param body_work	[io]	本体ワーク
  
  @note
  連続ヒットを防ぐためのヒット無効時間を設定します。
  無効時間経過後にヒット有効化されるようにするには、
  gmBoss1BodyUpdateNoHitTime()を毎フレーム呼んでください。
 */
// =======================================================================
void gmBoss1BodySetNoHitTime(GMS_BOSS1_BODY_WORK *body_work)
{
	GMS_ENEMY_COM_WORK	*ene_com	= (GMS_ENEMY_COM_WORK*)body_work;
	
	body_work->no_hit_timer	= GMD_BOSS1_BODY_DMG_NO_HIT_TIME;
	ene_com->rect_work[GMD_ENEMY_RECT_DEF].flag	|= OBD_RECT_NOHIT;
}

// =======================================================================
// gmBoss1BodyUpdateNoHitTime
/*!
  ヒット無効時間更新
  
  @param body_work	[io]	本体ワーク
  
  @note
  ヒット無効時間を更新して、タイマカウント完了時に喰らい矩形を復活させます。
  復活させたくない場合はこの関数を呼ばないでください。
 */
// =======================================================================
void gmBoss1BodyUpdateNoHitTime(GMS_BOSS1_BODY_WORK *body_work)
{
	if (body_work->no_hit_timer) {
		body_work->no_hit_timer--;
	}
	else {
		GMS_ENEMY_COM_WORK	*ene_com	= (GMS_ENEMY_COM_WORK*)body_work;
		
		ene_com->rect_work[GMD_ENEMY_RECT_DEF].flag	&= ~OBD_RECT_NOHIT;
	}
}

// =======================================================================
// gmBoss1BodyExecDamageRoutine
/*!
  ダメージ時処理
  
  @param body_work	[io]	本体ワーク
  
  @note
  ダメージ時の諸々の共通処理を行います。（ライフ減少、ゲージ更新、SE再生など）
 */
// =======================================================================
void gmBoss1BodyExecDamageRoutine(GMS_BOSS1_BODY_WORK *body_work)
{
	GMS_BOSS1_MGR_WORK	*mgr_work	= (GMS_BOSS1_MGR_WORK*)body_work->mgr_work;
	
	MTM_ASSERT(mgr_work);
	
	// ライフ減少
	if (mgr_work->life) {
		mgr_work->life	-= 1;
	}
	
	if (0 < mgr_work->life) {
		// ライフがある場合
		
		// ダメージ演出予約
		body_work->flag	|= GMD_BOSS1_BODY_FLAG_SIGNAL_B2B_BODY_DAMAGE;
	}
	else {
		// ライフが無い場合
		
		// ボス撃破タイミングのトロフィー獲得チェック
		HgTrophyTryAcquisition(HGE_TROPHY_CHECK_TIMING_DEFEAT_BOSS);
		
		// 死亡演出予約
		body_work->flag	|= GMD_BOSS1_BODY_FLAG_SIGNAL_B2B_DEFEAT;
		// これ以降ヒット処理しない
		GMM_BS_OBJ(body_work)->flag	|= OBD_OBJECT_NOHIT;
	}
}

// =======================================================================
// gmBoss1BodySetDmgRectSizeForAtkNml
/*!
  本体の食らい矩形サイズを通常攻撃用に設定
  
  @param body_work	[io]	本体ワーク
 */
// =======================================================================
void gmBoss1BodySetDmgRectSizeForAtkNml(GMS_BOSS1_BODY_WORK *body_work)
{
	GMS_ENEMY_COM_WORK	*ene_com	= (GMS_ENEMY_COM_WORK*)body_work;
	
	ObjRectWorkSet(&ene_com->rect_work[GMD_ENEMY_RECT_DEF],
				   GMD_BOSS1_BODY_ATKNML_DMG_RECT_SIZE_LEFT,
				   GMD_BOSS1_BODY_ATKNML_DMG_RECT_SIZE_TOP,
				   GMD_BOSS1_BODY_ATKNML_DMG_RECT_SIZE_RIGHT,
				   GMD_BOSS1_BODY_ATKNML_DMG_RECT_SIZE_BOTTOM);
}

// =======================================================================
// gmBoss1BodySetDmgRectSizeToDefault
/*!
  本体の食らい矩形サイズをデフォルトに設定
  
  @param body_work	[io]	本体ワーク
 */
// =======================================================================
void gmBoss1BodySetDmgRectSizeToDefault(GMS_BOSS1_BODY_WORK *body_work)
{
	GMS_ENEMY_COM_WORK	*ene_com	= (GMS_ENEMY_COM_WORK*)body_work;
	
	ObjRectWorkSet(&ene_com->rect_work[GMD_ENEMY_RECT_DEF],
				   GMD_BOSS1_BODY_DMG_RECT_SIZE_LEFT,
				   GMD_BOSS1_BODY_DMG_RECT_SIZE_TOP,
				   GMD_BOSS1_BODY_DMG_RECT_SIZE_RIGHT,
				   GMD_BOSS1_BODY_DMG_RECT_SIZE_BOTTOM);
}

// =======================================================================
// gmBoss1BodySetAtkRectToWeakAttacker
/*!
  本体の攻撃矩形を喰らい有りの攻撃用に設定
  
  @param body_work	[io]	本体ワーク
  
  @note
  プレイヤーの攻撃矩形が有効でない状態で当たるとプレイヤーに攻撃がヒットします。
  プレイヤーの攻撃矩形が有効の場合はボスに攻撃がヒットします。
 */
// =======================================================================
void gmBoss1BodySetAtkRectToWeakAttacker(GMS_BOSS1_BODY_WORK *body_work)
{
	GMS_ENEMY_COM_WORK	*ene_com	= (GMS_ENEMY_COM_WORK*)body_work;
	
	// 喰らい矩形よりも小さいサイズに設定する
	// （プレイヤーがスピン状態なのにボスの攻撃を食らう場合はさらに小さくする必要有り）
	ObjRectWorkSet(&ene_com->rect_work[GMD_ENEMY_RECT_ATK],
				   GMD_BOSS1_BODY_ATK_RECT_SIZE_LEFT,
				   GMD_BOSS1_BODY_ATK_RECT_SIZE_TOP,
				   GMD_BOSS1_BODY_ATK_RECT_SIZE_RIGHT,
				   GMD_BOSS1_BODY_ATK_RECT_SIZE_BOTTOM);
	
	ene_com->rect_work[GMD_ENEMY_RECT_ATK].flag	&= ~OBD_RECT_NOHIT;
}

// =======================================================================
// gmBoss1BodySetAtkRectToNormal
/*!
  本体の攻撃矩形設定を通常に設定
  
  @param body_work	[io]	本体ワーク
  
  @note
  通常の矩形設定に戻します。
 */
// =======================================================================
void gmBoss1BodySetAtkRectToNormal(GMS_BOSS1_BODY_WORK *body_work)
{
	GMS_ENEMY_COM_WORK	*ene_com	= (GMS_ENEMY_COM_WORK*)body_work;
	
	// 攻撃矩形を無効にする
	ObjRectWorkSet(&ene_com->rect_work[GMD_ENEMY_RECT_ATK], 0, 0, 0, 0);
	ene_com->rect_work[GMD_ENEMY_RECT_ATK].flag	|= OBD_RECT_NOHIT;
}

// =======================================================================
// gmBoss1BodyIsExtraAttack
/*!
  追加攻撃の条件がそろったかチェック
 
  @param body_work	[io]	本体ワーク
  
  @retval	TRUE	条件満たした
  @retval	FALSE	条件満たしていない
 */
// =======================================================================
BOOL gmBoss1BodyIsExtraAttack(GMS_BOSS1_BODY_WORK *body_work)
{
	if (GMM_BOSS1_MGR(body_work)->life <= GMD_BOSS1_EXTRA_ATK_THRESHOLD_LIFE) {
		return TRUE;
	}
	else {
		return FALSE;
	}
}

// =======================================================================
// gmBoss1BodyIsEscapeScrUnlock
/*!
  逃亡時のスクロール解除判定
  
  @retval TRUE	スクロール解除条件満たした
  @retval FALSE	スクロール解除条件満たしてない
 */
// =======================================================================
BOOL gmBoss1BodyIsEscapeScrUnlock(GMS_BOSS1_BODY_WORK *body_work)
{
	if (GMM_BS_OBJ(body_work)->pos.x >= GMM_BOSS1_AREA_RIGHT() + GMD_BOSS1_BODY_ESCAPE_SCR_UNLOCK_X_FROM_RIGHT) {
		return TRUE;
	}
	
	return FALSE;
}

// =======================================================================
// gmBoss1BodyIsEscapeOutFinalZone
/*!
  ファイナルゾーンで逃亡時のエリア（画面）退出判定
  
  @param body_work	[io]	本体ワーク
  
  @retval TRUE	エリア退出済み
  @retval FALSE	エリア内
 */
// =======================================================================
BOOL gmBoss1BodyIsEscapeOutFinalZone(GMS_BOSS1_BODY_WORK *body_work)
{
	if (GMM_BS_OBJ(body_work)->pos.x >= GMM_BOSS1_AREA_RIGHT() + GMD_BOSS1_BODY_ESCAPE_OUT_FINAL_ZONE_X_FROM_RIGHT) {
		return TRUE;
	}
	
	return FALSE;
}

// =======================================================================
// gmBoss1BodyIsDirectionPositiveFromCurrent
/*!
  最短回転が正回転方向か判定
  
  @param body_work		[io]	本体ワーク
  @param target_angle	[in]	目標方向
  
  @retval TRUE	現在の角度→指定角度が正回転方向
  @retval FALSE 現在の角度→指定角度が負回転方向
  
  @note
  現在の角度から指定角度への最短回転が正回転方向か判定します。
  (e.g. 現在の角度が130degで指定角が90degの場合は最短回転は負方向。
        現在の角度が270degだった場合は最短回転は正方向。)
 */
// =======================================================================
BOOL gmBoss1BodyIsDirectionPositiveFromCurrent(GMS_BOSS1_BODY_WORK *body_work, Angle16 target_angle)
{
	Angle32	diff_angle;
	
	// ANGLE_MASKすることでAngle16の範囲に収まる。
	// また、32bit型なので必ず正数になる
	diff_angle	= (Angle32)(MTD_MATH_ANGLE_MASK & ((Angle32)body_work->cur_angle - (Angle32)target_angle));
	
	if (diff_angle >= AKM_DEGtoA32(180)) {
		return TRUE;
	}
	else {
		return FALSE;
	}
}

// =======================================================================
// gmBoss1BodyUpdateDirection
/*!
  ボス１ 向き更新
  
  @param body_work	[io]	本体ワーク
  
  @note
  ボスの向き情報をオブジェクトの角度に反映します。
  毎フレーム呼んでください。
 */
// =======================================================================
void gmBoss1BodyUpdateDirection(GMS_BOSS1_BODY_WORK *body_work)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(body_work);
	
	obj_work->dir.y	= (Uint16)body_work->cur_angle;
}


// =======================================================================
// gmBoss1BodySetDirectionNormal
/*!
  標準の角度（真横より正面寄り）に設定
  
  @param body_work	[io]	本体ワーク
  
  @note
  真横よりも少し正面寄りに向くように角度設定を行います。
  OBD_DISP_HFLIPを参照して対応する角度に設定しているため、
  適切にOBD_DISP_HFLIPを設定しておいてください。
 */
// =======================================================================
void gmBoss1BodySetDirectionNormal(GMS_BOSS1_BODY_WORK *body_work)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(body_work);
	
	if (obj_work->disp_flag	& OBD_DISP_HFLIP) {
		gmBoss1BodySetDirection(body_work, GMD_BOSS1_LEFTWARD_ANGLE);
	}
	else {
		gmBoss1BodySetDirection(body_work, GMD_BOSS1_RIGHTWARD_ANGLE);
	}
	
	// パラメータクリア
	body_work->orig_angle	= 0;
	body_work->turn_angle	= 0;
}

// =======================================================================
// gmBoss1BodySetDirection
/*!
  任意角度設定
  
  @param body_work	[io]	本体ワーク
  @param deg		[in]	角度
  
  @note
  指定の値に角度を設定します。オブジェクトへの反映は行われません。
 */
// =======================================================================
void gmBoss1BodySetDirection(GMS_BOSS1_BODY_WORK *body_work, Angle16 deg)
{
	body_work->cur_angle	= deg;
}

#if (GMD_BOSS1_BOOL_USE_OBSOLETE_FUNCTION)
// =======================================================================
// gmBoss1BodyInitTurn
/*!
  振り向き回転処理を初期化
 
  @param body_work		[io]	本体ワーク
  @param turn_amount	[in]	振り向き回転量
  @param turn_spd		[in]	回転角速度
  
  @note
  turn_amountには現在の角度から差分でどれだけ回転させるか指定します。
  時計回りはマイナス値、反時計回りはプラス値を指定します。
  Angle32で扱える角度の範囲に注意してください。
 */
// =======================================================================
void gmBoss1BodyInitTurn(GMS_BOSS1_BODY_WORK *body_work,
						 Angle32 turn_amount, Angle32 turn_spd)
{
	MTM_ASSERT(0 == ((1 << 31) & (turn_amount ^ turn_spd))); // 符号比較
	
	body_work->orig_angle	= body_work->cur_angle;
	body_work->turn_angle	= 0;
	body_work->turn_amount	= turn_amount;
	body_work->turn_spd		= turn_spd;
	
	gmBoss1BodySetDirection(body_work,
							(Angle16)(body_work->orig_angle + body_work->turn_angle));
}

// =======================================================================
// gmBoss1BodyInitTurn
/*!
  振り向き回転処理を初期化（目標向き指定）
 
  @param body_work		[io]	本体ワーク
  @param dest_angle		[in]	目標となる角度（オフセットではなく、絶対的な角度）
  @param frame			[in]	回転にかけるフレーム数
  @param is_positive	[in]	正方向回転フラグ(TRUE : 正回転, FALSE : 負回転)
  
  @note
  目標の角度と所要フレーム数から、回転量と回転速度を算出して振り向き回転を初期化します。
  gmBoss1BodyUpdateTurn()時にspd_rateを指定するとフレーム数どおりに回転が完了しなくなります。
 */
// =======================================================================
void gmBoss1BodyInitTurn(GMS_BOSS1_BODY_WORK *body_work,
						 Angle16 dest_angle, Sint32 frame, BOOL is_positive)
{
	Uint16	turn_amount_u16;
	Angle32	turn_amount;
	Angle32	turn_spd;
	
	MTM_ASSERT(frame > 0);
	
	if (is_positive) {
		// 正方向（絶対値360度以内かつ適切な符号になるようにする）
		turn_amount_u16	= (Uint16)((Angle32)dest_angle - (Angle32)body_work->cur_angle);
		turn_amount	= (Angle32)turn_amount_u16;
	}
	else {
		// 負方向（絶対値360度以内かつ適切な符号になるようにする）
		turn_amount_u16	= (Uint16)(((Angle32)dest_angle - AKM_DEGtoA32(360)) -
								   ((Angle32)body_work->cur_angle - AKM_DEGtoA32(360)));
		turn_amount	= ((Angle32)((Uint32)turn_amount_u16) - AKM_DEGtoA32(360));	// キャストで符号情報が消えてるので戻す
	}
	
	// 回転速度取得
	turn_spd	= turn_amount / frame;
	
	// 回転初期化
	gmBoss1BodyInitTurn(body_work, turn_amount, turn_spd);
}


// =======================================================================
// gmBoss1BodyUpdateTurn
/*!
  振り向き回転処理更新
  
  @param body_work	[io]	本体ワーク
  @param spd_rate	[in]	速度係数（デフォルト1.f）
  
  @retval	TRUE	振り向き回転完了
  @retval	FALSE	振り向き回転中
  
  @note
  実際に回転を実施します。deg_addはgmBoss1BodyInitTurn()で指定した回転角と
  同じ符号になるようにしてください。
 */
// =======================================================================
BOOL gmBoss1BodyUpdateTurn(GMS_BOSS1_BODY_WORK *body_work, Float spd_rate/*=1.f*/)
{
	BOOL	result	= FALSE;
	Float	deg_spd;
	
	MTM_ASSERT(spd_rate >= 0.f);
	
	// 回転角度更新
	deg_spd	= spd_rate * body_work->turn_spd;
	MTM_ASSERT(MTM_MATH_ABS(deg_spd) <= (Sint32)0x7fffffff);
	body_work->turn_angle	+= (Angle32)deg_spd;
	
	// 目標到達判定
	if (body_work->turn_spd > 0) {
		if (body_work->turn_angle >= body_work->turn_amount) {
			result	= TRUE;
		}
	}
	else if (body_work->turn_spd < 0) {
		
		if (body_work->turn_angle <= body_work->turn_amount) {
			result	= TRUE;
		}
	}
	
	if (result) {
		// 目標角度にきっちりそろえる
		body_work->turn_angle	= body_work->turn_amount;
	}
	
	// 現在の向き設定
	gmBoss1BodySetDirection(body_work,
							(Angle16)((Angle32)body_work->orig_angle + body_work->turn_angle));
	
	return result;
}
#endif /* (GMD_BOSS1_BOOL_USE_OBSOLETE_FUNCTION) */

// =======================================================================
// gmBoss1BodyInitTurnGently
/*!
  緩やか振り向き回転 初期化
 
  @param body_work		[io]	本体ワーク
  @param dest_angle		[in]	目標角度
  @param frame			[in]	回転にかけるフレーム数
  @param is_positive	[in]	正方向回転フラグ(TRUE : 正回転, FALSE : 負回転)
 */
// =======================================================================
void gmBoss1BodyInitTurnGently(GMS_BOSS1_BODY_WORK *body_work, Angle16 dest_angle,
							   Sint32 frame, BOOL is_positive)
{
	Uint16	turn_amount_u16;
	Float	frame_deg;
	MTM_ASSERT(frame > 0);
	
	body_work->orig_angle	= body_work->cur_angle;
	body_work->turn_angle	= 0;
	body_work->turn_spd		= 0;
	
	if (is_positive) {
		// 正方向（絶対値360度以内かつ適切な符号になるようにする）
		turn_amount_u16	= (Uint16)((Angle32)dest_angle - (Angle32)body_work->cur_angle);
		body_work->turn_amount	= (Angle32)turn_amount_u16;
	}
	else {
		// 負方向（絶対値360度以内かつ適切な符号になるようにする）
		turn_amount_u16	= (Uint16)(((Angle32)dest_angle - AKM_DEGtoA32(360)) -
								   ((Angle32)body_work->cur_angle - AKM_DEGtoA32(360)));
		body_work->turn_amount	= ((Angle32)((Uint32)turn_amount_u16) - AKM_DEGtoA32(360));	// キャストで符号情報が消えてるので戻す
	}
	
	// 速度カーブ角度初期化
	body_work->turn_gen_var		= 0;
	// 速度カーブ決定値初期化（コサイン半回転を0～1.0に対応させるので180degをフレーム数で割る）
	frame_deg	= 180.f / frame;
	MTM_ASSERT(MTM_MATH_ABS(frame_deg) <= (Sint32)0x7fffffff);
	body_work->turn_gen_factor	= AKM_DEGtoA32(frame_deg);
	
	gmBoss1BodySetDirection(body_work,
							(Angle16)((Angle32)body_work->orig_angle + body_work->turn_angle));
}

// =======================================================================
// gmBoss1BodyUpdateTurnGently
/*!
  緩やか振り向き回転 更新
 
  @param body_work	[io]	本体ワーク
 */
// =======================================================================
BOOL gmBoss1BodyUpdateTurnGently(GMS_BOSS1_BODY_WORK *body_work)
{
	BOOL	result	= FALSE;
	Float	turn_angle_f;
	
	MTM_ASSERT(body_work->turn_gen_factor > 0);
	
	body_work->turn_gen_var	+= body_work->turn_gen_factor;
	if (body_work->turn_gen_var >= AKM_DEGtoA32(180)) {
		body_work->turn_gen_var	= AKM_DEGtoA32(180);
		result	= TRUE;
	}
	
	// コサインカーブで向きを決定
	turn_angle_f	= (body_work->turn_amount) * 0.5f * (1.f - nnCos(body_work->turn_gen_var));
	MTM_ASSERT(MTM_MATH_ABS(turn_angle_f) <= (Sint32)0x7fffffff);
	body_work->turn_angle	= (Angle32)(turn_angle_f);
	
	if (result) {
		// 目標角度にきっちりそろえる
		body_work->turn_angle	= body_work->turn_amount;
	}
	
	// 現在の向き設定
	gmBoss1BodySetDirection(body_work,
							(Angle16)((Angle32)body_work->orig_angle + body_work->turn_angle));
	
	return result;
}

// =======================================================================
// gmBoss1BodyInitPreANChainMotion
/*!
  通常攻撃「開始」 鎖モーション制御 初期化
  
  @param body_work	[io]	本体ワーク
  
  @note
  通常攻撃開始時の鎖の動きを手動で制御します。
 */
// =======================================================================
void gmBoss1BodyInitPreANChainMotion(GMS_BOSS1_BODY_WORK *body_work)
{
	OBS_OBJECT_WORK	*obj_chain;
	
	MTM_ASSERT(body_work->parts_objs[GME_BOSS1_PART_IDX_CHAIN]);
	
	obj_chain	= body_work->parts_objs[GME_BOSS1_PART_IDX_CHAIN];
	
	// 初期フレーム設定
	obj_chain->obj_3d->frame[0]	= GMD_BOSS1_BODY_PRE_ATKNML_CHAIN_INI_MTN_FRAME;
}


// =======================================================================
// gmBoss1BodyInitPreANMove
/*!
  通常攻撃「開始」移動処理 初期化
  
  @note
  左方向の移動のみ。
 */
// =======================================================================
void gmBoss1BodyInitPreANMove(GMS_BOSS1_BODY_WORK *body_work)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(body_work);
	
	obj_work->spd.x	= 0;
	
	obj_work->spd_add.x	= -GMD_BOSS1_BODY_PRE_ATKNML_SPD_ADD;
}

// =======================================================================
// gmBoss1BodyUpdatePreANMove
/*!
  通常攻撃「開始」移動処理 更新
  
  @retval TRUE	通常攻撃開始移動到達
  @retval FALSE	通常攻撃開始移動中
  
  @note
  左方向の移動のみ。
 */
// =======================================================================
BOOL gmBoss1BodyUpdatePreANMove(GMS_BOSS1_BODY_WORK *body_work)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(body_work);
	BOOL	result	= FALSE;
	
	// 速度制限
	if (MTM_MATH_ABS(obj_work->spd.x) >= GMD_BOSS1_BODY_PRE_ATKNML_SPD_MAX_ABS) {
		obj_work->spd.x	= -GMD_BOSS1_BODY_PRE_ATKNML_SPD_MAX_ABS;
		obj_work->spd_add.x	= 0;
	}
	
	// 到達チェック
	if (obj_work->pos.x <= GMM_BOSS1_AREA_LEFT() + GMD_BOSS1_BODY_ATKNML_LEFT_LIMIT) {
		obj_work->pos.x	= GMM_BOSS1_AREA_LEFT() + GMD_BOSS1_BODY_ATKNML_LEFT_LIMIT;
		result	= TRUE;
	}
	
	// 停止
	if (result) {
		GmBsCmnSetObjSpdZero(obj_work);
	}
	
	return result;
}

// =======================================================================
// gmBoss1BodySetANChainInitialBlendSpd
/*!
  通常攻撃 鎖 初回ブレンド速度設定
  
  @param body_work	[io]	本体ワーク
  
  @note
  通常攻撃初回時の鎖のモーションブレンド速度を設定します。
  アクション設定後に呼び出してください。
 */
// =======================================================================
void gmBoss1BodySetANChainInitialBlendSpd(GMS_BOSS1_BODY_WORK *body_work)
{
	OBS_OBJECT_WORK	*obj_chain;
	
	MTM_ASSERT(body_work->parts_objs[GME_BOSS1_PART_IDX_CHAIN]);
	
	obj_chain	= body_work->parts_objs[GME_BOSS1_PART_IDX_CHAIN];
	obj_chain->obj_3d->blend_spd	= GMD_BOSS1_BODY_ATKNML_CHAIN_INI_BLD_SPD;
}

// =======================================================================
// gmBoss1BodyInitAtkNmlMove
/*!
  通常攻撃移動処理 初期化
  
  @param body_work	[io]	本体ワーク
  @param frame		[in]	所要フレーム数
  
  @note
  OBD_DISP_HFLIP 設定に応じた(加)速度設定を行います。
 */
// =======================================================================
void gmBoss1BodyInitAtkNmlMove(GMS_BOSS1_BODY_WORK *body_work, Sint32 frame)
{
	// 所要フレーム設定
	body_work->move_time	= frame;
	
	// フレームカウント初期化
	body_work->move_cnt		= 0;
	
	// OBS_OBJECT_WORK::spd を使わないと、このフレームは速度反映されないので、
	// 代わりに今フレーム分の座標更新を一回手動実行しておく。
	gmBoss1BodyUpdateAtkNmlMove(body_work);
}

// =======================================================================
// gmBoss1BodyUpdateAtkNmlMove
/*!
  通常攻撃移動処理 更新
  
  @param body_work	[io]	本体ワーク
  
  @retval	TRUE	通常攻撃移動到達
  @retval	FALSE	通常攻撃移動中
 */
// =======================================================================
BOOL gmBoss1BodyUpdateAtkNmlMove(GMS_BOSS1_BODY_WORK *body_work)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(body_work);
	fx32	start_x;
	fx32	end_x;
	BOOL	result;
	
	start_x	= GMM_BOSS1_AREA_LEFT() + GMD_BOSS1_BODY_ATKNML_LEFT_LIMIT;
	end_x	= GMM_BOSS1_AREA_LEFT() + GMD_BOSS1_BODY_ATKNML_RIGHT_LIMIT;
	
	// 左右反映
	if (obj_work->disp_flag & OBD_DISP_HFLIP) {
		fx32	temp;
		temp	= start_x;
		start_x	= end_x;
		end_x	= temp;
	}
	
	// 所要フレーム数厳守で終了判定を行う
	if (body_work->move_cnt < body_work->move_time) {
		
#if defined(GMD_BOSS1_AKTNML_MOVE_USE_PARTIAL_CURVE)
		/* 円の部分曲線バージョン*/
		Angle32	diff_angle;
		Float	factor;
		Float	amp;
		
		Angle32	angle_width	= GMD_BOSS1_BODY_ATKNML_MOVE_CURVE_ANGLE_WIDTH;
		Angle32	start_angle	= GMD_BOSS1_BODY_ATKNML_MOVE_CURVE_START_ANGLE;
		
		body_work->move_cnt++;
		
		// 1フレーム分の角度
		diff_angle	= (Angle32)(angle_width / (Float)body_work->move_time);
		
		// コサイン値の振り幅
		amp	= nnCos(start_angle) - nnCos(start_angle + angle_width);
		
		// 既定範囲の角度のコサイン値を0.0f ～ 1.0f にマップする
		factor	= ((nnCos(start_angle) - nnCos(start_angle + diff_angle * body_work->move_cnt)) / amp);
		
		// 座標設定
		obj_work->pos.x	= start_x + (fx32)((end_x - start_x) * factor);
#else
		/* 線形補間バージョン */
		fx32	diff_x;
		
		body_work->move_cnt++;
		
		// 1フレーム分の移動量
		diff_x	= (fx32)((end_x - start_x) / (Float)body_work->move_time);
		
		// 経過フレーム数に対応した座標を反映
		obj_work->pos.x	= start_x + (fx32)((Float)body_work->move_cnt * diff_x);
#endif	/* defined(GMD_BOSS1_AKTNML_MOVE_USE_PARTIAL_CURVE) */
		
		result	= FALSE;
	}
	else {
		// 所要フレーム経過後はきっちりそろえる
		obj_work->pos.x	= end_x;
		
		result	= TRUE;
	}
	
	return result;
}

// =======================================================================
// gmBoss1BodySetFlipForAtkNmlMove
/*!
  通常攻撃移動 フリップ設定
  
  @param body_work	[io]	本体ワーク
  
  @note
  現在地に応じたフリップ設定を行います。
  片道の移動が終わったタイミングで呼び出してください。
 */
// =======================================================================
void gmBoss1BodySetFlipForAtkNmlMove(GMS_BOSS1_BODY_WORK *body_work)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(body_work);
	
	fx32	center	= GMM_BOSS1_AREA_CENTER_X();
	if (obj_work->pos.x < center) {
		obj_work->disp_flag	&= ~OBD_DISP_HFLIP;
	}
	else {
		obj_work->disp_flag	|= OBD_DISP_HFLIP;
	}
}

// =======================================================================
// gmBoss1BodyInitAtkNmlFlipAndTurn
/*!
  通常攻撃 フリップ+振り向き処理初期化
  
  @param body_work	[io]	本体ワーク
  
  @note
  フリップ設定を行った後に振り向き処理を初期化します。
  gmBoss1BodyUpdateAtkNmlFlipAndTurn()で更新してください。
  通常攻撃の移動が目標地点に到達したタイミングで呼びます。
 */
// =======================================================================
void gmBoss1BodyInitAtkNmlFlipAndTurn(GMS_BOSS1_BODY_WORK *body_work)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(body_work);
	Sint32	frame	= GMD_BOSS1_BODY_ATKNML_TURN_FRAME;
	
	// ドリフトと同時にターンするときは、ドリフトより長くならないようにする
	// （通常攻撃の折り返しではドリフトの所要フレーム数を厳守する必要があるため）
	MTM_ASSERT(frame < GMD_BOSS1_BODY_ATKNML_DRIFT_FRAME);
	
	gmBoss1BodySetFlipForAtkNmlMove(body_work);
	
	if (obj_work->disp_flag & OBD_DISP_HFLIP) {
		// これから左に向く
		gmBoss1BodyInitTurnGently(body_work, GMD_BOSS1_LEFTWARD_ANGLE,
								  frame, TRUE);
	}
	else {
		// これから右に向く
		gmBoss1BodyInitTurnGently(body_work, GMD_BOSS1_RIGHTWARD_ANGLE,
								  frame, FALSE);
	}
}

// =======================================================================
// gmBoss1BodyUpdateAtkNmlFlipAndTurn
/*!
  通常攻撃 フリップ+振り向き処理更新
  
  @param body_work	[io]	本体ワーク
  
  @retval	TRUE	振り向き完了
  @retval	FALSE	振り向き中
 */
// =======================================================================
BOOL gmBoss1BodyUpdateAtkNmlFlipAndTurn(GMS_BOSS1_BODY_WORK *body_work)
{
	return gmBoss1BodyUpdateTurnGently(body_work);
}

// =======================================================================
// gmBoss1BodyInitAtkNmlDrift
/*!
  通常攻撃 ドリフト移動処理 初期化
  
  @param body_work	[io]	本体ワーク
  @param frame		[in]	所要フレーム数
  
  @note
  OBD_DISP_HFLIP 設定に応じたドリフト移動を行います。
  ドリフト後の向きを設定した後に呼び出してください。
 */
// =======================================================================
void gmBoss1BodyInitAtkNmlDrift(GMS_BOSS1_BODY_WORK *body_work, Sint32 frame)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(body_work);
	
	MTM_ASSERT(frame > 0);
	
	// 速度クリア
	GmBsCmnSetObjSpdZero(obj_work);
	
	// 角度初期化
	body_work->drift_angle	= 0;
	
	// 角速度初期化
	body_work->drift_ang_spd	= (Angle32)nnRoundOff((AKM_DEGtoA32(180.f) / (Float)frame) + 0.5f);
	
	// タイマ初期化
	body_work->drift_timer	= frame;
	
	// 軸座標設定
	if (obj_work->disp_flag & OBD_DISP_HFLIP) {
		// これから左に向く（右にドリフト）
		body_work->drift_pivot_x	= GMM_BOSS1_AREA_LEFT() + GMD_BOSS1_BODY_ATKNML_RIGHT_LIMIT;
	}
	else {
		// これから右に向く（左にドリフト）
		body_work->drift_pivot_x	= GMM_BOSS1_AREA_LEFT() + GMD_BOSS1_BODY_ATKNML_LEFT_LIMIT;
	}
	
	// OBS_OBJECT_WORK::spd を使わないと、このフレームは速度反映されないので、
	// 代わりに今フレーム分の座標更新を一回手動実行しておく。
	gmBoss1BodyUpdateAtkNmlDrift(body_work);
}

// =======================================================================
// gmBoss1BodyUpdateAtkNmlDrift
/*!
  通常攻撃 ドリフト移動処理 更新
  
  @param param0 [in] 入力引数0説明
  
  @retval TRUE	ドリフト移動中
  @retval FALSE	ドリフト移動終了
 */
// =======================================================================
BOOL gmBoss1BodyUpdateAtkNmlDrift(GMS_BOSS1_BODY_WORK *body_work)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(body_work);
	fx32	offset_x;
	BOOL	result;
	
	if (body_work->drift_timer) {
		body_work->drift_timer--;
		
		// 角度更新
		body_work->drift_angle	= (Angle32)(MTD_MATH_ANGLE_MASK & (body_work->drift_angle +
																   body_work->drift_ang_spd));
		
		
		offset_x	= FX_Mul(FX_Sin(body_work->drift_angle), GMD_BOSS1_BODY_ATKNML_DRIFT_AMP);
		
		result	= FALSE;
	}
	else {
		offset_x	= 0;
		
		result	= TRUE;
	}
	
	// 左右ドリフト方向反映
	if (!(obj_work->disp_flag & OBD_DISP_HFLIP)) {
		offset_x	= -offset_x;
	}
	
	// 座標反映
	obj_work->pos.x	= body_work->drift_pivot_x + offset_x;
	
	return result;
}

// =======================================================================
// gmBoss1BodyInitRush
/*!
  叩きつけ突進処理 初期化
  
  @param body_work	[io]	本体ワーク
  @param is_left	[in]	左向きフラグ
 */
// =======================================================================
void gmBoss1BodyInitRush(GMS_BOSS1_BODY_WORK *body_work, BOOL is_left)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(body_work);
	fx32	spd_x;
	fx32	spd_y;
	
	if (is_left) {
		body_work->bash_targ_pos.x	= GMM_BOSS1_AREA_LEFT() + GMD_BOSS1_BODY_ATKBASH_TARG_X_LEFT;
		body_work->bash_targ_pos.y	= GMD_BOSS1_BODY_ATKBASH_TARG_Y;
		body_work->bash_targ_pos.z	= GMD_BOSS1_DEFAULT_POS_Z;
	}
	else {
		body_work->bash_targ_pos.x	= GMM_BOSS1_AREA_LEFT() + GMD_BOSS1_BODY_ATKBASH_TARG_X_RIGHT;
		body_work->bash_targ_pos.y	= GMD_BOSS1_BODY_ATKBASH_TARG_Y;
		body_work->bash_targ_pos.z	= GMD_BOSS1_DEFAULT_POS_Z;
	}
	
	spd_x	= (body_work->bash_targ_pos.x - obj_work->pos.x) / GMD_BOSS1_BODY_ATKBASH_MTN_FRAME;
	spd_y	= (body_work->bash_targ_pos.y - obj_work->pos.y) / GMD_BOSS1_BODY_ATKBASH_MTN_FRAME;
	
	obj_work->spd_add.x	= (fx32)(spd_x * GMD_BOSS1_BODY_ATKBASH_SPD_ADD_FACTOR);
	obj_work->spd_add.y	= (fx32)(spd_y * GMD_BOSS1_BODY_ATKBASH_SPD_ADD_FACTOR);
	obj_work->spd.x	= 0;
	obj_work->spd.y	= 0;
}

// =======================================================================
// gmBoss1BodyUpdateRush
/*!
  叩きつけ突進処理 更新
  
  @param body_work	[io]	本体ワーク
  
  @retval	TRUE	叩きつけ突進完了
  @retval	FALSE	叩きつけ突進中
 */
// =======================================================================
BOOL gmBoss1BodyUpdateRush(GMS_BOSS1_BODY_WORK *body_work)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(body_work);
	NNS_VECTOR	vec_diff;
	NNS_VECTOR	vec_spd;
	
	amVectorSet(&vec_diff,
				FX_FX32_TO_F32(body_work->bash_targ_pos.x) - FX_FX32_TO_F32(obj_work->pos.x),
				FX_FX32_TO_F32(body_work->bash_targ_pos.y) - FX_FX32_TO_F32(obj_work->pos.y),
				0);
	
	amVectorSet(&vec_spd,
				FX_FX32_TO_F32(obj_work->spd.x),
				FX_FX32_TO_F32(obj_work->spd.y),
				0);
	
	// 通り過ぎたかチェック
	if (0 >= nnDotProductVector(&vec_spd, &vec_diff)) {
		// 目標位置に固定する
		GmBsCmnSetObjSpdZero(obj_work);
		VEC_Set(&obj_work->pos,
				body_work->bash_targ_pos.x,
				body_work->bash_targ_pos.y,
				body_work->bash_targ_pos.z);
		return TRUE;
	}
	else {
		return FALSE;
	}
}

// =======================================================================
// gmBoss1BodyInitBashReturn
/*!
  叩きつけ 元の高度に戻る 初期化
  
  @param body_work	[io]	本体ワーク
  @param is_left	[in]	左側処理フラグ
  							(TRUE:左側の既定位置に戻る
 	 						 FALSE:右側の既定位置に戻る)
  
  @note
  現在の向きに応じて適切な位置に戻る処理の初期化を行います。
 */
// =======================================================================
void gmBoss1BodyInitBashReturn(GMS_BOSS1_BODY_WORK *body_work, BOOL is_left)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(body_work);
	
	if (is_left) {
		body_work->bash_ret_pos.x	= GMM_BOSS1_AREA_LEFT() + GMD_BOSS1_BODY_ATKBASH_HOMEPOS_X_LEFT;
		body_work->bash_ret_pos.y	= body_work->atk_nml_alt;
		body_work->bash_ret_pos.z	= GMD_BOSS1_DEFAULT_POS_Z;
	}
	else {
		body_work->bash_ret_pos.x	= GMM_BOSS1_AREA_LEFT() + GMD_BOSS1_BODY_ATKBASH_HOMEPOS_X_RIGHT;
		body_work->bash_ret_pos.y	= body_work->atk_nml_alt;
		body_work->bash_ret_pos.z	= GMD_BOSS1_DEFAULT_POS_Z;
	}
	
	VEC_Set(&body_work->bash_orig_pos,
			obj_work->pos.x,
			obj_work->pos.y,
			obj_work->pos.z);
	
	// 角度初期化
	body_work->bash_homing_deg	= 0;
}

// =======================================================================
// gmBoss1BodyUpdateBashReturn
/*!
  叩きつけ 元の高度に戻る 更新
  
  @param body_work	[io]	本体ワーク
 */
// =======================================================================
BOOL gmBoss1BodyUpdateBashReturn(GMS_BOSS1_BODY_WORK *body_work)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(body_work);
	
	body_work->bash_homing_deg	+= AKM_DEGtoA32(180 / GMD_BOSS1_BODY_ATBASH_RETURN_TIME);
	if (body_work->bash_homing_deg >= AKM_DEGtoA32(180)) {
		body_work->bash_homing_deg	= AKM_DEGtoA32(180);
		
		// 目標位置に固定
		obj_work->pos.x	= body_work->bash_ret_pos.x;
		obj_work->pos.y	= body_work->bash_ret_pos.y;
		
		return TRUE;
	}
	else {
		obj_work->pos.x	=
			body_work->bash_orig_pos.x +
				FX_Mul(body_work->bash_ret_pos.x - body_work->bash_orig_pos.x,
					   (FX32_ONE - mtMathCos(body_work->bash_homing_deg)) >> 1);
		obj_work->pos.y	=
			body_work->bash_orig_pos.y +
				FX_Mul(body_work->bash_ret_pos.y - body_work->bash_orig_pos.y,
					   (FX32_ONE - mtMathCos(body_work->bash_homing_deg)) >> 1);
		return FALSE;
	}
}

// =======================================================================
// gmBoss1BodyInitEscapeMove
/*!
  逃亡移動処理 初期化
  
  @param body_work	[io]	本体ワーク
 */
// =======================================================================
void gmBoss1BodyInitEscapeMove(GMS_BOSS1_BODY_WORK *body_work)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(body_work);
	
	GmBsCmnSetObjSpdZero(obj_work);
	
	if (obj_work->disp_flag & OBD_DISP_HFLIP) {
		
		obj_work->spd_add.x	= -GMD_BOSS1_BODY_ESCAPE_SPD_X_ADD;
	}
	else {
		obj_work->spd_add.x	= GMD_BOSS1_BODY_ESCAPE_SPD_X_ADD;
	}
	
	obj_work->spd_add.y	= GMD_BOSS1_BODY_ESCAPE_SPD_Y_ADD;
}

// =======================================================================
// gmBoss1BodyUpdateEscapeMove
/*!
  逃亡移動処理 更新
  
  @param body_work	[io]	本体ワーク
  
  @retval TRUE	スクロールロック位置
  @retval FALSE	移動中
 */
// =======================================================================
BOOL gmBoss1BodyUpdateEscapeMove(GMS_BOSS1_BODY_WORK *body_work)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(body_work);
	BOOL	result	= FALSE;
	
	// 速度制限
	if (MTM_MATH_ABS(obj_work->spd.x) >= GMD_BOSS1_BODY_ESCAPE_SPD_X_MAX) {
		obj_work->spd.x	= GMD_BOSS1_BODY_ESCAPE_SPD_X_MAX;
		obj_work->spd.y	= GMD_BOSS1_BODY_ESCAPE_SPD_Y_MAX;
		obj_work->spd_add.x	= 0;
		obj_work->spd_add.y	= 0;
	}
	
	// マップ上端・右端から外側に完全に消えたかチェック
	if (obj_work->pos.y < 0 - GMD_BOSS1_BODY_HIDE_RADIUS_V) {
		result	= TRUE;
	}
	else if (obj_work->pos.x >= (g_gm_main_system.map_size[MTD_X] << FX32_SHIFT) + GMD_BOSS1_BODY_HIDE_RADIUS_H) {
		result	= TRUE;
	}
	
	// 停止
	if (result) {
		GmBsCmnSetObjSpdZero(obj_work);
	}
	
	return result;
}

// =======================================================================
// gmBoss1BodyInitDefeatState
/*!
  撃破ステート初期化
  
  @param body_work	[io]	本体ワーク
  
  @note
  GMD_BOSS1_BODY_FLAG_CHAIN_DEPENDフラグ設定を持ち越しつつ
  DEFEATステートへの遷移を行います。
  DEFEATステートに遷移する場合は、
  gmBoss1BodyChangeState()を直接呼ばずにこの関数でステート遷移してください。
 */
// =======================================================================
void gmBoss1BodyInitDefeatState(GMS_BOSS1_BODY_WORK *body_work)
{
	BOOL	is_depend	= FALSE;
	
	if (body_work->flag & GMD_BOSS1_BODY_FLAG_CHAIN_DEPEND) {
		is_depend	= TRUE;
	}
	
	// ステート遷移
	gmBoss1BodyChangeState(body_work, GME_BOSS1_BODY_STATE_DEFEAT, TRUE);
	
	if (is_depend) {
		body_work->flag	|= GMD_BOSS1_BODY_FLAG_CHAIN_DEPEND;
	}
	else {
		body_work->flag	&= ~GMD_BOSS1_BODY_FLAG_CHAIN_DEPEND;
	}
}


// ============================================================================
// ノード操作関連
// ============================================================================
// =======================================================================
// gmBoss1BodyUpdateChainTopDirection
/*!
  鎖の付け根パーツを正面に向ける
  
  @param body_work	[io]	本体ワーク
  
  @note
  鎖の付け根パーツ（本体側のパーツ）を正面に向くようにCNMの設定を行います。
 */
// =======================================================================
void gmBoss1BodyUpdateChainTopDirection(GMS_BOSS1_BODY_WORK *body_work)
{
	NNS_MATRIX	*w_mtx;
	NNS_MATRIX	rotated_mtx;
	
	if (!(body_work->flag & GMD_BOSS1_BODY_FLAG_CHAIN_DEPEND)) {
		// ノードのワールドマトリクス取得
		w_mtx	= GmBsCmnGetSNMMtx(&body_work->snm_work,
								   body_work->chaintop_snm_reg_id);
		
		nnRotateYMatrix(&rotated_mtx, w_mtx,
						-GMM_BS_OBJ(body_work)->dir.y + AKM_DEGtoA16(90));
		
		GmBsCmnSetCNMMtx(&body_work->cnm_mgr_work,
						 &rotated_mtx,
						 body_work->chaintop_cnm_reg_id);
		GmBsCmnEnableCNMMtxNode(&body_work->cnm_mgr_work,
								body_work->chaintop_cnm_reg_id,
								TRUE);
	}
	else {
		GmBsCmnEnableCNMMtxNode(&body_work->cnm_mgr_work,
								body_work->chaintop_cnm_reg_id,
								FALSE);
	}
}


// ============================================================================
// 処理関数
// ============================================================================

// =======================================================================
// gmBoss1BodyDamageDefFunc
/*!
  本体 プレイヤー攻撃ヒット時くらい処理関数
  
  @param my_rect	[io]	自分の矩形
  @parma your_rect	[io]	相手の矩形
 */
// =======================================================================
void gmBoss1BodyDamageDefFunc(OBS_RECT_WORK *my_rect, OBS_RECT_WORK *your_rect)
{
	OBS_OBJECT_WORK	*my_obj		= my_rect->parent_obj;
	OBS_OBJECT_WORK	*your_obj	= your_rect->parent_obj;
	GMS_BOSS1_BODY_WORK	*body_work	= (GMS_BOSS1_BODY_WORK*)my_obj;
	//GMS_ENEMY_COM_WORK	*ene_com	= (GMS_ENEMY_COM_WORK*)my_obj;
	
	if (your_obj && GMD_OBJTYPE_PLAYER == your_obj->obj_type) {
		GMS_PLAYER_WORK	*ply_work	= (GMS_PLAYER_WORK*)your_obj;
		
		// プレイヤー跳ね返り
		GmPlySeqAtkReactionInit(ply_work);
		
		// ジャンプステート設定
		GmPlySeqSetJumpState(ply_work, 0,
							 (GMD_PLY_SEQ_SETJUMPSTATE_IGNORE_JUMPBTN |
							  GMD_PLY_SEQ_SETJUMPSTATE_NOHOMING));
		
		// ホーミング跳ね返り・通常跳ね返りにより吹っ飛びのパラメータを変える
		if (ply_work->seq_state == GME_PLY_SEQ_STATE_HOMING_REF) {
			// 水平方向速度設定
			ply_work->obj_work.spd_m = 0;
			if ( ply_work->obj_work.move.x >= 0 ){
				ply_work->obj_work.spd.x = -GMD_BOSS1_BODY_PLY_HOMING_REBOUND_X;
				
			}
			else {
				ply_work->obj_work.spd.x = GMD_BOSS1_BODY_PLY_HOMING_REBOUND_X;
			}
			
			
			// 垂直方向速度設定
			if (your_obj->pos.y <= my_obj->pos.y) {
				ply_work->obj_work.spd.y	= -GMD_BOSS1_BODY_PLY_HOMING_REBOUND_Y;
			}
			else {
				ply_work->obj_work.spd.y	= GMD_BOSS1_BODY_PLY_HOMING_REBOUND_Y;
			}
			
			// ジャンプ中移動禁止時間設定
			GmPlySeqSetNoJumpMoveTime(ply_work, GMD_BOSS1_BODY_PLY_HOMING_REBOUND_NOJUMPMOVE_TIME);
		}
		else {
			// 水平方向速度設定
			ply_work->obj_work.spd_m = 0;
			if ( ply_work->obj_work.move.x >= 0 ){
				ply_work->obj_work.spd.x = -GMD_BOSS1_BODY_PLY_NML_REBOUND_X;
				
			}
			else {
				ply_work->obj_work.spd.x = GMD_BOSS1_BODY_PLY_NML_REBOUND_X;
			}
			
			
			// 垂直方向速度設定
			if (your_obj->pos.y <= my_obj->pos.y) {
				ply_work->obj_work.spd.y	= -GMD_BOSS1_BODY_PLY_NML_REBOUND_Y;
			}
			else {
				ply_work->obj_work.spd.y	= GMD_BOSS1_BODY_PLY_NML_REBOUND_Y;
			}
			
			// ジャンプ中移動禁止時間設定
			GmPlySeqSetNoJumpMoveTime(ply_work, GMD_BOSS1_BODY_PLY_NML_REBOUND_NOJUMPMOVE_TIME);
		}
		
		// ヒット無効時間設定
		gmBoss1BodySetNoHitTime(body_work);
		
		// ダメージSE再生
		GmSoundPlaySE("Boss0_01");
		
		// 振動小
		GMM_PAD_VIB_SMALL_TIME(30);
		
		// ダメージエフェクト生成
		gmBoss1EffDamageInit(body_work);
		
		if (!(body_work->flag & GMD_BOSS1_BODY_FLAG_INVINCIBLE)) {
			// 無敵ではないときだけダメージ処理
			
			// ダメージ処理
			gmBoss1BodyExecDamageRoutine(body_work);
		}
	}
}

// =======================================================================
// gmBoss1BodyOutFunc
/*!
  本体 専用描画関数
  
  @param obj_work	[io]	オブジェクトワーク
  
  @note
  通常の描画に加えてノード操作処理も行っています。
 */
// =======================================================================
void gmBoss1BodyOutFunc(OBS_OBJECT_WORK *obj_work)
{
	GMS_BOSS1_BODY_WORK	*body_work	= (GMS_BOSS1_BODY_WORK*)obj_work;
	
	// ノード操作更新
	GmBsCmnUpdateCNMParam(obj_work, &body_work->cnm_mgr_work);
	
	// 標準描画関数
	ObjDrawActionSummary(obj_work);
}

// ============================================================================
// 制御処理
// ============================================================================

// =======================================================================
// gmBoss1BodyChangeState
/*!
  本体ステート変更
 
  @param body_work	[io]	本体ワーク
  @param state		[in]	遷移先ステート
  
  @note
  前ステートの終了関数呼び出しと、遷移先ステートの開始関数呼び出しを行います。
 */
// =======================================================================
void gmBoss1BodyChangeState(GMS_BOSS1_BODY_WORK *body_work,
							GME_BOSS1_BODY_STATE state, BOOL is_wrapped/*=FALSE*/)
{
	UNREFERENCED_PARAMETER(is_wrapped);
	const GMS_BOSS1_BODY_STATE_ENTER_INFO	*enter_info;
	GMF_BOSS1_BODY_STATE_LEAVE_FUNC	leave_func;
	
	// 前ステート終了処理
	leave_func	= gm_boss1_body_state_leave_func_tbl[body_work->state];
	if (leave_func) {
		leave_func(body_work);
	}
	
	// ステート設定
	body_work->prev_state	= body_work->state;
	body_work->state		= state;
	
	// 次ステート開始処理
	enter_info	= &gm_boss1_body_state_enter_info_tbl[body_work->state];
#if defined(MTD_DEBUG)	// 間接呼び出しチェック
	if (enter_info->is_wrapped) {
		MTM_ASSERT(is_wrapped != FALSE);
	}
	else {
		MTM_ASSERT(is_wrapped == FALSE);
	}
#endif /* defined(MTD_DEBUG) */
	if (enter_info->enter_func) {
		enter_info->enter_func(body_work);
	}
}

// =======================================================================
// gmBoss1BodyWaitSetup
/*!
  本体 生成完了待ち
  
  @param obj_work	[io]	オブジェクトワーク
 */
// =======================================================================
void gmBoss1BodyWaitSetup(OBS_OBJECT_WORK *obj_work)
{
	GMS_BOSS1_BODY_WORK	*body_work	= (GMS_BOSS1_BODY_WORK*)obj_work;
	
	
	// 全ての構成パーツの生成完了を待つ
	if (body_work->mgr_work->flag & GMD_BOSS1_MGR_FLAG_SETUP_END) {
		
		// BMCBシステム初期化
		GmBsCmnInitBossMotionCBSystem(obj_work,
									  &body_work->bmcb_mgr);
		
		// ノードマトリクス取得初期化
		GmBsCmnCreateSNMWork(&body_work->snm_work,
							 obj_work->obj_3d->object,
							 GMD_BOSS1_BODY_NODE_SNM_NUM);
		// モーションコールバックを実行リストに追加
		GmBsCmnAppendBossMotionCallback(&body_work->bmcb_mgr,
										&body_work->snm_work.bmcb_link);
		
		
		// ノードマトリクス取得ノード追加
		body_work->chain_snm_reg_id	=
			GmBsCmnRegisterSNMNode(&body_work->snm_work, GMD_BOSS1_BODY_NODE_IDX_CHAIN_CONNECT);
		body_work->egg_snm_reg_id	=
			GmBsCmnRegisterSNMNode(&body_work->snm_work, GMD_BOSS1_BODY_NODE_IDX_EGG_CONNECT);
		body_work->body_snm_reg_id	=
			GmBsCmnRegisterSNMNode(&body_work->snm_work, GMD_BOSS1_BODY_NODE_IDX_BODY_POSTURE);
		body_work->chaintop_snm_reg_id	=
			GmBsCmnRegisterSNMNode(&body_work->snm_work, GMD_BOSS1_BODY_NODE_IDX_CHAIN_ROOT_PART);
		
		
		// ノードマトリクス操作処理管理ワーク初期化
		GmBsCmnCreateCNMMgrWork(&body_work->cnm_mgr_work,
								obj_work->obj_3d->object,
								GMD_BOSS1_BODY_NODE_CNM_NUM);
		
		// ノードマトリクス操作コールバック初期化
		GmBsCmnInitCNMCb(obj_work, &body_work->cnm_mgr_work);
		
		// 操作ノード追加
		body_work->chaintop_cnm_reg_id	=
			GmBsCmnRegisterCNMNode(&body_work->cnm_mgr_work,
								   GMD_BOSS1_BODY_NODE_IDX_CHAIN_ROOT_PART);
		
		
		obj_work->ppFunc	= gmBoss1BodyMain;
		
		// 開始ステート設定
		gmBoss1BodyChangeState(body_work, GME_BOSS1_BODY_STATE_START);
	}
}

// =======================================================================
// gmBoss1BodyMain
/*!
  本体 メイン処理関数
  
  @param obj_work	[io]	オブジェクトワーク
 */
// =======================================================================
void gmBoss1BodyMain(OBS_OBJECT_WORK *obj_work)
{
	GMS_BOSS1_BODY_WORK	*body_work	= (GMS_BOSS1_BODY_WORK*)obj_work;
	
	// ヒット無効時間更新
	gmBoss1BodyUpdateNoHitTime(body_work);
	
	// 死亡演出開始チェック
	if (body_work->flag & GMD_BOSS1_BODY_FLAG_SIGNAL_B2B_DEFEAT) {
		
		
		body_work->flag	&= ~(GMD_BOSS1_BODY_FLAG_SIGNAL_B2B_DEFEAT |
							 GMD_BOSS1_BODY_FLAG_SIGNAL_B2B_BODY_DAMAGE);
							 
		// ↑ダメージシグナルは無視させる
		
		// 撃破ステートへ
		gmBoss1BodyInitDefeatState(body_work);
	}
	else {
		// ステート遷移が発生していないときだけ更新処理を行う
		
		// 更新処理
		if (body_work->proc_update) {
			body_work->proc_update(body_work);
		}
	}
	
	// モーション再生停滞更新
	gmBoss1BodyUpdateSuspendAction(body_work);
	
	// アフターバーナー生成ループ更新
	gmBoss1EffAfterburnerUpdateCreate(body_work);
	
	
	// ダメージ演出開始チェック
	if (body_work->flag & GMD_BOSS1_BODY_FLAG_SIGNAL_B2B_BODY_DAMAGE) {
		body_work->flag	&= ~(GMD_BOSS1_BODY_FLAG_SIGNAL_B2B_BODY_DAMAGE);
		
		// エッグマンにダメージを通知
		body_work->flag	|= GMD_BOSS1_BODY_FLAG_SIGNAL_B2E_EGG_DAMAGE;
		
		// ダメージ点滅初期化
		GmBsCmnInitObject3DNNDamageFlicker(obj_work, &body_work->flk_work,
										   GMD_BOSS1_BODY_DMG_FLICKER_RADIUS);
	}
	
	// ダメージ点滅更新
	GmBsCmnUpdateObject3DNNDamageFlicker(obj_work, &body_work->flk_work);
	
	// 角度反映
	gmBoss1BodyUpdateDirection(body_work);
	
	// 鎖付け根パーツの向きを更新
	gmBoss1BodyUpdateChainTopDirection(body_work);
}


// ============================================================================
// 各状態のシーケンス処理
// ============================================================================
// =======================================================================
// gmBoss1BodyState{Enter|Leave|Update}StartXXX
/*!
  本体 開始ステート遷移時処理関数
 */
// =======================================================================
// 開始ステート開始関数
void gmBoss1BodyStateEnterStart(GMS_BOSS1_BODY_WORK *body_work)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(body_work);
	GMS_ENEMY_3D_WORK	*ene_3d	= (GMS_ENEMY_3D_WORK*)obj_work;
	
	// 矩形当たりオフ
	obj_work->flag	|= OBD_OBJECT_NOHIT;
	
	// 鎖当たりオフ
	body_work->flag	|= GMD_BOSS1_BODY_FLAG_CHAIN_NOHIT;
	
	// アクション設定（強制設定）
	gmBoss1BodySetActionWhole(body_work, GME_BOSS1_ACT_ID_APP_FALL, TRUE);
	
	// 鎖非表示
	body_work->flag	|= GMD_BOSS1_BODY_FLAG_CHAIN_NODISP;
	
	// ホーミングの対象からはずす
	ene_3d->ene_com.enemy_flag	|= GMD_ENEMY_FLAG_NOHOMING;
	
	// 速度設定
	GmBsCmnSetObjSpdZero(obj_work);
	
	// 通常角度に設定
	gmBoss1BodySetDirectionNormal(body_work);
	
	// 処理関数設定
	body_work->proc_update	= gmBoss1BodyStateUpdateStartWithWaitLockBegin;
}

// 開始ステート終了関数
void gmBoss1BodyStateLeaveStart(GMS_BOSS1_BODY_WORK *body_work)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(body_work);
	GMS_ENEMY_3D_WORK	*ene_3d	= (GMS_ENEMY_3D_WORK*)obj_work;
	
	// アフターバーナー無効化
	gmBoss1EffAfterburnerSetEnable(body_work, FALSE);
	
	// フラグクリア
	ene_3d->ene_com.enemy_flag	&= ~GMD_ENEMY_FLAG_NOHOMING;
	
	// 鎖当たりオン
	body_work->flag	&= ~GMD_BOSS1_BODY_FLAG_CHAIN_NOHIT;
	
	// 矩形当たりオン
	obj_work->flag	&= ~OBD_OBJECT_NOHIT;
	
	// フラグ元にもどす
	body_work->flag	&= ~GMD_BOSS1_BODY_FLAG_CHAIN_NODISP;
}

// 開始ステート更新 スクロールロック開始待ち
void gmBoss1BodyStateUpdateStartWithWaitLockBegin(GMS_BOSS1_BODY_WORK *body_work)
{
	if (gmBoss1IsScrollLockBusy()) {
		body_work->proc_update	= gmBoss1BodyStateUpdateStartWithWaitLockComplete;
	}
}

// 開始ステート更新 スクロールロック完了待ち
void gmBoss1BodyStateUpdateStartWithWaitLockComplete(GMS_BOSS1_BODY_WORK *body_work)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(body_work);
	
	if (!gmBoss1IsScrollLockBusy()) {
#if _IPHONE
		GmMapSetMapDrawSize(GME_MAP_DRAW_SIZE_ZONE1_BOSS);
#endif // _IPHONE
		
		// 速度設定
		GmBsCmnSetObjSpd(obj_work,
						 0,
						 GMD_BOSS1_BODY_START_MOVE_FALL_SPD,
						 0);
		
		// 処理関数設定
		body_work->proc_update	= gmBoss1BodyStateUpdateStartWithFall;
	}
}

// 開始ステート更新 降下処理
void gmBoss1BodyStateUpdateStartWithFall(GMS_BOSS1_BODY_WORK *body_work)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(body_work);
	
	if (obj_work->pos.y >= body_work->atk_nml_alt) {
		// 停止
		GmBsCmnSetObjSpdZero(obj_work);
		
		// 目標位置に固定する
		obj_work->pos.y	= body_work->atk_nml_alt;
		
		// 速度設定
		GmBsCmnSetObjSpd(obj_work,
						 GMD_BOSS1_BODY_START_MOVE_SIDE_SPD,
						 0,
						 0);
		
		// アフターバーナー有効化
		gmBoss1EffAfterburnerSetEnable(body_work, TRUE);
		
		// 処理関数設定
		body_work->proc_update	= gmBoss1BodyStateUpdateStartWithMove;
	}
}

// 開始ステート更新 水平移動処理
void gmBoss1BodyStateUpdateStartWithMove(GMS_BOSS1_BODY_WORK *body_work)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(body_work);
	
	fx32	dest_x	= GMM_BOSS1_AREA_LEFT() + GMD_BOSS1_BODY_START_MOVE_DEST_AREA_X;
	if (obj_work->pos.x <= dest_x) {
		// 停止
		GmBsCmnSetObjSpdZero(obj_work);
		
		// 目標位置に固定する
		obj_work->pos.x	= dest_x;
		
		// 待機時間設定
		body_work->wait_timer	= GMD_BOSS1_BODY_START_WAIT_END_TIME;
		
		// アフターバーナー無効化
		gmBoss1EffAfterburnerSetEnable(body_work, FALSE);
		
		// 処理関数設定
		body_work->proc_update	= gmBoss1BodyStateUpdateStartWithWaitEnd;
	}
}

// 開始ステート更新 終了待機処理
void gmBoss1BodyStateUpdateStartWithWaitEnd(GMS_BOSS1_BODY_WORK *body_work)
{
	if (body_work->wait_timer) {
		body_work->wait_timer--;
	}
	else {
		// 鉄球準備
		gmBoss1BodyChangeState(body_work, GME_BOSS1_BODY_STATE_PREP);
	}
}

// =======================================================================
// gmBoss1BodyState{Enter|Leave|Update}PrepXXX
/*!
  本体 鉄球準備ステート遷移時処理関数
 */
// =======================================================================
// 鉄球準備ステート開始関数
void gmBoss1BodyStateEnterPrep(GMS_BOSS1_BODY_WORK *body_work)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(body_work);
	GMS_ENEMY_3D_WORK	*ene_3d	= (GMS_ENEMY_3D_WORK*)obj_work;
	
	// 矩形当たりオフ
	obj_work->flag	|= OBD_OBJECT_NOHIT;
	
	// 鎖当たりオフ
	body_work->flag	|= GMD_BOSS1_BODY_FLAG_CHAIN_NOHIT;
	
	// アクション設定
	gmBoss1BodySetActionWhole(body_work, GME_BOSS1_ACT_ID_PREP_CHAIN);
	
	// 鎖表示させる
	body_work->flag	&= ~GMD_BOSS1_BODY_FLAG_CHAIN_NODISP;
	
	// ホーミングの対象からはずす
	ene_3d->ene_com.enemy_flag	|= GMD_ENEMY_FLAG_NOHOMING;
	
	// 停止
	GmBsCmnSetObjSpdZero(obj_work);
	
	// 鎖当たり有効開始タイミング設定
	body_work->wait_timer	= GMD_BOSS1_BODY_PREP_CHAIN_ACTIVE_TIMING_FRAME;
	
	// 鉄球登場SE再生
	GmSoundPlaySE("Boss1_01");
	
	// 処理関数設定
	body_work->proc_update	= gmBoss1BodyStateUpdatePrepWithWait;
}

// 鉄球準備ステート終了関数
void gmBoss1BodyStateLeavePrep(GMS_BOSS1_BODY_WORK *body_work)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(body_work);
	GMS_ENEMY_3D_WORK	*ene_3d	= (GMS_ENEMY_3D_WORK*)obj_work;
	
	// フラグクリア
	ene_3d->ene_com.enemy_flag	&= ~GMD_ENEMY_FLAG_NOHOMING;
	
	// 鎖当たりオン
	body_work->flag	&= ~GMD_BOSS1_BODY_FLAG_CHAIN_NOHIT;
	
	// 矩形当たりオン
	obj_work->flag	&= ~OBD_OBJECT_NOHIT;
}

// 鉄球準備ステート更新 終了待ち
void gmBoss1BodyStateUpdatePrepWithWait(GMS_BOSS1_BODY_WORK *body_work)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(body_work);
	
	// 既定時間後に鎖当たりオン
	if (body_work->wait_timer) {
		body_work->wait_timer--;
	}
	else {
		body_work->flag	&= ~GMD_BOSS1_BODY_FLAG_CHAIN_NOHIT;
	}
	
	if (GmBsCmnIsActionEnd(obj_work)) {
		gmBoss1BodyChangeState(body_work, GME_BOSS1_BODY_STATE_PRE_ATK_NML);
	}
}


// =======================================================================
// gmBoss1BodyState{Enter|Leave|Update}PreAtkNmlXXX
/*!
  本体 通常攻撃開始ステート遷移時処理関数
 */
// =======================================================================
// 通常攻撃開始ステート開始関数
void gmBoss1BodyStateEnterPreAtkNml(GMS_BOSS1_BODY_WORK *body_work)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(body_work);
	
	// 通常攻撃用に食らい矩形サイズを設定
	gmBoss1BodySetDmgRectSizeForAtkNml(body_work);
	
	// 矩形当たりオン
	obj_work->flag	&= ~OBD_OBJECT_NOHIT;
	
	// アクション設定
	gmBoss1BodySetActionWhole(body_work, GME_BOSS1_ACT_ID_PRE_ATK_NML_MOVE);
	
	// 通常攻撃開始時 鎖モーション設定初期化
	gmBoss1BodyInitPreANChainMotion(body_work);
	
	// フリップ設定
	gmBoss1BodySetFlipForAtkNmlMove(body_work);
	
	// 移動開始
	gmBoss1BodyInitPreANMove(body_work);
	
	// アフターバーナー有効化
	gmBoss1EffAfterburnerSetEnable(body_work, TRUE);
	
	// 処理関数設定
	body_work->proc_update	= gmBoss1BodyStateUpdatePreAtkNmlWithMove;
}

// 通常攻撃開始ステート終了関数
void gmBoss1BodyStateLeavePreAtkNml(GMS_BOSS1_BODY_WORK *body_work)
{
	// アフターバーナー無効化
	gmBoss1EffAfterburnerSetEnable(body_work, FALSE);
	
	// デフォルトの食らい矩形サイズに設定
	gmBoss1BodySetDmgRectSizeToDefault(body_work);
}

// 通常攻撃開始ステート更新 移動処理
void gmBoss1BodyStateUpdatePreAtkNmlWithMove(GMS_BOSS1_BODY_WORK *body_work)
{
	// 標準の角度に設定
	gmBoss1BodySetDirectionNormal(body_work);
	
	if (gmBoss1BodyUpdatePreANMove(body_work)) {
		
		// 通常攻撃へ
		gmBoss1BodyChangeState(body_work, GME_BOSS1_BODY_STATE_ATK_NML);
		
		// 通常攻撃ステート 初回のブレンド設定
		gmBoss1BodySetANChainInitialBlendSpd(body_work);
	}
}

// =======================================================================
// gmBoss1BodyState{Enter|Leave|Update}AtkNmlXXX
/*!
  本体 通常攻撃ステート遷移時処理関数
 */
// =======================================================================
// 通常攻撃ステート開始関数
void gmBoss1BodyStateEnterAtkNml(GMS_BOSS1_BODY_WORK *body_work)
{
	/* 振り向きから開始する */
	
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(body_work);
	BOOL	b_force_change	= FALSE;
	
	// 通常攻撃用に食らい矩形サイズを設定
	gmBoss1BodySetDmgRectSizeForAtkNml(body_work);
	
	// 矩形当たりオン
	obj_work->flag	&= ~OBD_OBJECT_NOHIT;
	
	// アクション設定（左方向時のみ強制設定）
	if (obj_work->disp_flag & OBD_DISP_HFLIP) {
		b_force_change	= TRUE;
	}
	gmBoss1BodySetActionWhole(body_work,
							  GME_BOSS1_ACT_ID_ATK_NML_MOVE,
							  b_force_change);
	
	// 通常攻撃振り向き初期化
	gmBoss1BodyInitAtkNmlFlipAndTurn(body_work);
	
	// 通常攻撃ドリフト移動初期化
	gmBoss1BodyInitAtkNmlDrift(body_work, GMD_BOSS1_BODY_ATKNML_DRIFT_FRAME);
	
	// アフターバーナー無効化
	gmBoss1EffAfterburnerSetEnable(body_work, FALSE);
	
	// 処理関数設定
	body_work->proc_update	= gmBoss1BodyStateUpdateAtkNmlWithTurn;
}

// 通常攻撃ステート終了関数
void gmBoss1BodyStateLeaveAtkNml(GMS_BOSS1_BODY_WORK *body_work)
{
	// アフターバーナー無効化
	gmBoss1EffAfterburnerSetEnable(body_work, FALSE);
	
	// デフォルトの食らい矩形サイズに設定
	gmBoss1BodySetDmgRectSizeToDefault(body_work);
}

// 通常攻撃ステート更新 振り向き処理
void gmBoss1BodyStateUpdateAtkNmlWithTurn(GMS_BOSS1_BODY_WORK *body_work)
{
	BOOL	drift_result;
	drift_result	= gmBoss1BodyUpdateAtkNmlDrift(body_work);
	
	if (gmBoss1BodyUpdateAtkNmlFlipAndTurn(body_work)) {
		if (drift_result) {
			// フリップ設定
			gmBoss1BodySetFlipForAtkNmlMove(body_work);
			
			// 移動開始
			gmBoss1BodyInitAtkNmlMove(body_work, GMD_BOSS1_BODY_ATKNML_MOVE_FRAME);
			
			// アフターバーナー有効化
			gmBoss1EffAfterburnerSetEnable(body_work, TRUE);
			
			// 鉄球振り子SE再生
			GmSoundPlaySE("Boss1_02");
			
			// 処理関数設定
			body_work->proc_update	= gmBoss1BodyStateUpdateAtkNmlWithMove;
		}
	}
}

// 通常攻撃ステート更新 移動処理
void gmBoss1BodyStateUpdateAtkNmlWithMove(GMS_BOSS1_BODY_WORK *body_work)
{
	// 標準の角度に設定
	gmBoss1BodySetDirectionNormal(body_work);
	
	// 追加攻撃開始チェック
	if (gmBoss1BodyIsExtraAttack(body_work)) {
		
		// ボス怒り状態BGM開始
		if (!GmBsCmnIsFinalZoneType(GMM_BS_OBJ(body_work))) {
			GmSoundChangeAngryBossBGM();
		}
		
		gmBoss1BodyChangeState(body_work, GME_BOSS1_BODY_STATE_ATK_BASH);
		return;
	}
	
	if (gmBoss1BodyUpdateAtkNmlMove(body_work)) {
		// 通常攻撃へ
		gmBoss1BodyChangeState(body_work, GME_BOSS1_BODY_STATE_ATK_NML);
	}
}


// =======================================================================
// gmBoss1BodyState{Enter|Leave|Update}AtkBashXXX
/*!
  本体 叩きつけステート遷移時処理関数
 */
// =======================================================================
// 叩きつけステート開始関数
void gmBoss1BodyStateEnterAtkBash(GMS_BOSS1_BODY_WORK *body_work)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(body_work);
	
	// アクション設定
	gmBoss1BodySetActionWhole(body_work, GME_BOSS1_ACT_ID_ATK_BASH_LOCK);
	
	// 速度停止
	GmBsCmnSetObjSpdZero(obj_work);
	
	// 通常角度に設定
	gmBoss1BodySetDirectionNormal(body_work);
	
	// 通常攻撃用に食らい矩形サイズを設定（通常攻撃からつながっているので）
	gmBoss1BodySetDmgRectSizeForAtkNml(body_work);
	
	if (GmBsCmnGetPlayerObj()->pos.x < obj_work->pos.x) {
		// プレイヤーが左なら左方向に向く
		obj_work->disp_flag	|= OBD_DISP_HFLIP;
		gmBoss1BodyInitTurnGently(body_work, AKM_DEGtoA16(270.f), 30, FALSE);
	}
	else {
		// プレイヤーが右なら右方向に向く
		obj_work->disp_flag	&= ~OBD_DISP_HFLIP;
		gmBoss1BodyInitTurnGently(body_work, AKM_DEGtoA16(90.f), 30, TRUE);
	}
	
	// 処理関数設定
	body_work->proc_update	= gmBoss1BodyStateUpdateAtkBashWithLock;
}

// 叩きつけステート終了関数
void gmBoss1BodyStateLeaveAtkBash(GMS_BOSS1_BODY_WORK *body_work)
{
	// 鎖追随解除
	body_work->flag	&= ~GMD_BOSS1_BODY_FLAG_CHAIN_DEPEND;
	
	// 無敵状態解除
	body_work->flag	&= ~GMD_BOSS1_BODY_FLAG_INVINCIBLE;
	
	// 本体矩形を通常に戻す
	gmBoss1BodySetAtkRectToNormal(body_work);
	
	// デフォルトの食らい矩形サイズに設定
	gmBoss1BodySetDmgRectSizeToDefault(body_work);
}

// 叩きつけステート更新 プレイヤーの方向を向く処理
void gmBoss1BodyStateUpdateAtkBashWithLock(GMS_BOSS1_BODY_WORK *body_work)
{
	if (gmBoss1BodyUpdateTurnGently(body_work)) {
		// アクション設定
		gmBoss1BodySetActionWhole(body_work, GME_BOSS1_ACT_ID_ATK_BASH_PREP);
		
		// 鉄球巻取りSEタイマ設定
		gmBoss1Init1ShotTimer(&body_work->se_timer, GMD_BOSS1_SE_ATK_BASH_PREP_REEL_START_WAIT_TIME);
		// 再生回数設定
		body_work->se_cnt	= GMD_BOSS1_SE_ATK_BASH_PREP_REEL_PLAY_NUM;
		
		body_work->proc_update	= gmBoss1BodyStateUpdateAtkBashWithPrep;
	}
}

// 叩きつけステート更新 準備処理
void gmBoss1BodyStateUpdateAtkBashWithPrep(GMS_BOSS1_BODY_WORK *body_work)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(body_work);
	
	// 鉄球巻取りSE再生
	if (gmBoss1Update1ShotTimer(&body_work->se_timer)) {
		if (body_work->se_cnt) {
			body_work->se_cnt--;
			GmSoundPlaySE("Boss1_03");
			gmBoss1Init1ShotTimer(&body_work->se_timer, GMD_BOSS1_SE_REEL_INTERVAL_TIME);
		}
	}
	
	// 本体の向きに関わらずDEPEND状態に自然に切り替えるために、
	// 手動マージの終了したタイミングでDEPENDフラグを立てる。
	// このときの手動マージのブレンドスピードが遅すぎると、
	// モーション終盤の、鉄球が傾いた状態の時にDEPENDフラグが立ってしまい、
	// 切り替わりが不自然になってしまうので、鎖がまだまっすぐな状態のときに
	// マージが終了するようにブレンド速度を設定しておくこと。
	if (gmBoss1BodyCheckChainMotionMergeEnd(body_work)) {
		// 鎖追随
		body_work->flag	|= GMD_BOSS1_BODY_FLAG_CHAIN_DEPEND;
	}
	
	if (GmBsCmnIsActionEnd(obj_work)) {
		
		// デフォルトの食らい矩形サイズに設定（これ以降はずっとデフォルトサイズ）
		gmBoss1BodySetDmgRectSizeToDefault(body_work);
		
		// 本体攻撃矩形を攻撃用に設定
		gmBoss1BodySetAtkRectToWeakAttacker(body_work);
		
		// 無敵状態
		body_work->flag	|= GMD_BOSS1_BODY_FLAG_INVINCIBLE;
		
		// 鎖追随
		body_work->flag	|= GMD_BOSS1_BODY_FLAG_CHAIN_DEPEND;
		
		// アクション設定
		gmBoss1BodySetActionWhole(body_work, GME_BOSS1_ACT_ID_ATK_BASH_SWING);
		
		// 突進初期化
		if (obj_work->disp_flag & OBD_DISP_HFLIP) {
			gmBoss1BodyInitRush(body_work, TRUE);
		}
		else {
			gmBoss1BodyInitRush(body_work, FALSE);
		}
		
		// 回転SE
		GmSoundPlaySE("Boss1_04");
		
		// 処理関数設定
		body_work->proc_update	= gmBoss1BodyStateUpdateAtkBashWithSwing;
	}
}

// 叩きつけステート更新 回転処理
void gmBoss1BodyStateUpdateAtkBashWithSwing(GMS_BOSS1_BODY_WORK *body_work)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(body_work);
	
	if (gmBoss1BodyUpdateRush(body_work)) {
		if (GmBsCmnIsActionEnd(obj_work)) {
			
			// デフォルトの食らい矩形サイズに設定（念のため）
			gmBoss1BodySetDmgRectSizeToDefault(body_work);
			
			// 本体矩形を喰らい有りの攻撃用に設定
			gmBoss1BodySetAtkRectToWeakAttacker(body_work);
			
			// 無敵状態解除
			body_work->flag	&= ~GMD_BOSS1_BODY_FLAG_INVINCIBLE;
			
			// 衝撃波生成通知
			body_work->flag	|=GMD_BOSS1_BODY_FLAG_SIGNAL_B2C_EFF_SW;
			
			// 画面振動
			GmCameraVibrationSet(0, GMD_BOSS1_BODY_ATKBASH_QUAKE_VAL_Y, 0);
			
			// 地面ヒットSE
			GmSoundPlaySE("Boss1_05");
			
			// 振動中
			GMM_PAD_VIB_MID_TIME(30);
			
			// アクション設定
			gmBoss1BodySetActionWhole(body_work, GME_BOSS1_ACT_ID_ATK_BASH_FINISH);
			
			// 鎖接続ノードの動きと鎖モーションを同期させるため、
			// 鎖の再生開始を1フレーム遅らせる
			gmBoss1BodySetSuspendAction(body_work, GME_BOSS1_PART_IDX_CHAIN, 1);
			
			// 鉄球巻取りSEタイマ設定
			gmBoss1Init1ShotTimer(&body_work->se_timer, GMD_BOSS1_SE_ATK_BASH_FIN_REEL_START_WAIT_TIME);
			// 再生回数設定
			body_work->se_cnt	= GMD_BOSS1_SE_ATK_BASH_FIN_REEL_PLAY_NUM;
			
			// 処理関数設定
			body_work->proc_update	= gmBoss1BodyStateUpdateAtkBashWithFinish;
		}
	}
}

// 叩きつけステート更新 余韻・鎖回収処理
void gmBoss1BodyStateUpdateAtkBashWithFinish(GMS_BOSS1_BODY_WORK *body_work)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(body_work);
	
	// 鉄球巻取りSE再生
	if (gmBoss1Update1ShotTimer(&body_work->se_timer)) {
		if (body_work->se_cnt) {
			body_work->se_cnt--;
			GmSoundPlaySE("Boss1_03");
			gmBoss1Init1ShotTimer(&body_work->se_timer, GMD_BOSS1_SE_REEL_INTERVAL_TIME);
		}
	}
	
	if (GmBsCmnIsActionEnd(obj_work)) {
		
		// 本体矩形を通常に設定
		gmBoss1BodySetAtkRectToNormal(body_work);
		
		// アクション設定
		gmBoss1BodySetActionWhole(body_work, GME_BOSS1_ACT_ID_ATK_BASH_HOMING);
		
		// 元の高度に戻る＆振り向き回転処理初期化
		if (obj_work->disp_flag & OBD_DISP_HFLIP) {
			gmBoss1BodyInitBashReturn(body_work, TRUE);
			
			obj_work->disp_flag	&= ~OBD_DISP_HFLIP;
			gmBoss1BodyInitTurnGently(body_work, AKM_DEGtoA16(90), 90, FALSE);
		}
		else {
			gmBoss1BodyInitBashReturn(body_work, FALSE);
			
			obj_work->disp_flag	|= OBD_DISP_HFLIP;
			gmBoss1BodyInitTurnGently(body_work, AKM_DEGtoA16(270), 90, TRUE);
		}
		
		// 処理関数設定
		body_work->proc_update	= gmBoss1BodyStateUpdateAtkBashWithHoming;
	}
}

// 叩きつけステート更新 定位置に戻る処理
void gmBoss1BodyStateUpdateAtkBashWithHoming(GMS_BOSS1_BODY_WORK *body_work)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(body_work);
	BOOL	result	= TRUE;
	
	if (FALSE == gmBoss1BodyUpdateTurnGently(body_work)) {
		result	= FALSE;
	}
	
	if (FALSE == gmBoss1BodyUpdateBashReturn(body_work)) {
		result	= FALSE;
	}
	
	if (result) {
		
		// デフォルトの食らい矩形サイズに設定（念のため）
		gmBoss1BodySetDmgRectSizeToDefault(body_work);
		
		// 本体矩形を攻撃用に設定
		gmBoss1BodySetAtkRectToWeakAttacker(body_work);
		
		// 無敵状態
		body_work->flag	|= GMD_BOSS1_BODY_FLAG_INVINCIBLE;
		
		// 鎖追随
		body_work->flag	|= GMD_BOSS1_BODY_FLAG_CHAIN_DEPEND;
		
		// アクション設定
		gmBoss1BodySetActionWhole(body_work, GME_BOSS1_ACT_ID_ATK_BASH_SWING);
		
		// 突進初期化
		if (obj_work->disp_flag & OBD_DISP_HFLIP) {
			gmBoss1BodyInitRush(body_work, TRUE);
		}
		else {
			gmBoss1BodyInitRush(body_work, FALSE);
		}
		
		// 回転SE
		GmSoundPlaySE("Boss1_04");
		
		body_work->proc_update	= gmBoss1BodyStateUpdateAtkBashWithSwing;
	}
}


// =======================================================================
// gmBoss1BodyState{Enter|Leave|Update}DmgNmlXXX
/*!
  本体 通常ダメージステート遷移時処理関数
 */
// =======================================================================
// 通常ダメージステート開始関数
void gmBoss1BodyStateEnterDmgNml(GMS_BOSS1_BODY_WORK *body_work)
{
	UNREFERENCED_PARAMETER(body_work);
}

// 通常ダメージステート終了関数
void gmBoss1BodyStateLeaveDmgNml(GMS_BOSS1_BODY_WORK *body_work)
{
	UNREFERENCED_PARAMETER(body_work);
}

// =======================================================================
// gmBoss1BodyStateEnterDefeat
/*!
  本体 撃破ステート遷移時処理関数
 */
// =======================================================================
// 撃破ステート開始関数
void gmBoss1BodyStateEnterDefeat(GMS_BOSS1_BODY_WORK *body_work)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(body_work);
	
	// 矩形当たりオフ
	obj_work->flag	|= OBD_OBJECT_NOHIT;
	
	// アニメーションストップ
	obj_work->disp_flag	|= OBD_DISP_STOP;
	
	// 鎖を死亡状態にする
	body_work->flag	|= GMD_BOSS1_BODY_FLAG_CHAIN_DEAD;
	
	// ホーミングアタックの対象からはずす
	body_work->ene_3d.ene_com.enemy_flag	|= GMD_ENEMY_FLAG_NOHOMING;
	
	// 速度設定
	GmBsCmnSetObjSpdZero(obj_work);
	
	// 待機時間設定
	body_work->wait_timer	= GMD_BOSS1_BODY_DEFEAT_WAIT_START_TIME;
	
	// ボス戦勝利BGM切り替え
	GmSoundChangeWinBossBGM();
	
	// 処理関数設定
	body_work->proc_update	= gmBoss1BodyStateUpdateDefeatWithWaitStart;
}

// 撃破ステート終了関数
void gmBoss1BodyStateLeaveDefeat(GMS_BOSS1_BODY_WORK *body_work)
{
	UNREFERENCED_PARAMETER(body_work);
}

// 撃破ステート更新 開始待機処理
void gmBoss1BodyStateUpdateDefeatWithWaitStart(GMS_BOSS1_BODY_WORK *body_work)
{
	// 既定時間待機
	if (body_work->wait_timer > 0) {
		body_work->wait_timer--;
	}
	else {
		
		// 小爆発初期化
		gmBoss1EffBombInitCreate(&body_work->bomb_work,
								 GME_BOSS1_EFF_BOMB_TYPE_SMALL,
								 GMM_BS_OBJ(body_work),
								 GMM_BS_OBJ(body_work)->pos.x,
								 GMM_BS_OBJ(body_work)->pos.y,
								 GMD_BOSS1_BODY_DEFEAT_BOMB_CREATE_WIDTH,
								 GMD_BOSS1_BODY_DEFEAT_BOMB_CREATE_HEIGHT,
								 GMD_BOSS1_BODY_DEFEAT_BOMB_CREATE_INTERVAL_MIN,
								 GMD_BOSS1_BODY_DEFEAT_BOMB_CREATE_INTERVAL_MAX);
		
		// 爆発時間
		body_work->wait_timer	= GMD_BOSS1_BODY_DEFEAT_BOMB_CREATE_TIME;
		
		// 処理関数設定
		body_work->proc_update	= gmBoss1BodyStateUpdateDefeatWithExplode;
	}
}

// 撃破ステート更新 爆発処理
void gmBoss1BodyStateUpdateDefeatWithExplode(GMS_BOSS1_BODY_WORK *body_work)
{
	if (body_work->wait_timer > 0) {
		body_work->wait_timer--;
		
		// 小爆発更新
		gmBoss1EffBombUpdateCreate(&body_work->bomb_work);
		
	}
	else {
		// パーツバラバラを鎖パーツに通知
		body_work->flag	|= GMD_BOSS1_BODY_FLAG_SIGNAL_B2C_SCATTER;
		
		// 大爆発SE再生
		GmSoundPlaySE("Boss0_03");
		
		// 画面フラッシュ
		gmBoss1InitFlashScreen();
		
		// 振動中
		GMM_PAD_VIB_MID_TIME(120);
		
		// 大爆発生成
		{
			OBS_OBJECT_WORK	*bomb_obj;
			bomb_obj	= (OBS_OBJECT_WORK*)GmEfctCmnEsCreate(GMM_BS_OBJ(body_work), GME_EFCT_CMN_IDX_BOMB_BIG);
			bomb_obj->pos.z	= bomb_obj->parent_obj->pos.z + GMD_BOSS1_EFF_BOMB_OFST_Z;
		}
		
		// 待機時間設定
		body_work->wait_timer	= GMD_BOSS1_BODY_DEFEAT_BURNT_WAIT_TIME;
		
		// スコア加算
		GmPlayerAddScoreNoDisp((GMS_PLAYER_WORK*)GmBsCmnGetPlayerObj(), GMD_PLY_SCORE_BOSS);
		
		// 処理関数設定
		body_work->proc_update	= gmBoss1BodyStateUpdateDefeatWithScatter;
	}
}

// 撃破ステート更新 パーツ飛散処理
void gmBoss1BodyStateUpdateDefeatWithScatter(GMS_BOSS1_BODY_WORK *body_work)
{
	if (body_work->wait_timer) {
		body_work->wait_timer--;
	}
	else {
		
		// 黒こげテクスチャに変更
		gmBoss1SetPartTextureBurnt(GMM_BS_OBJ(body_work));
		// エッグマンに黒こげ通知
		body_work->flag	|= GMD_BOSS1_BODY_FLAG_SIGNAL_B2E_EGG_BURNT;
		
		// アフターバーナー煙生成
		gmBoss1EffABSmokeInit(body_work);
		// 本体煙生成
		gmBoss1EffBodySmokeInit(body_work);
		// 本体小煙生成
		gmBoss1EffBodySmallSmokeInit(body_work);
		
		body_work->proc_update	= gmBoss1BodyStateUpdateDefeatWithWaitEnd;
	}
}

// 撃破ステート更新 爆発後待機処理
void gmBoss1BodyStateUpdateDefeatWithWaitEnd(GMS_BOSS1_BODY_WORK *body_work)
{
	// 既定時間待機
	if (body_work->wait_timer > 0) {
		body_work->wait_timer--;
	}
	else {
		gmBoss1BodyChangeState(body_work, GME_BOSS1_BODY_STATE_ESCAPE);
	}
}


// =======================================================================
// gmBoss1BodyState{Enter|Leave|Update}EscapeXXX
/*!
  本体 逃亡ステート遷移時処理関数
 */
// =======================================================================
// 逃亡ステート開始関数
void gmBoss1BodyStateEnterEscape(GMS_BOSS1_BODY_WORK *body_work)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(body_work);
	
	// 矩形当たりオフ
	obj_work->flag	|= OBD_OBJECT_NOHIT;
	
	// アニメーション再開
	obj_work->disp_flag	&= ~OBD_DISP_STOP;
	
	// アクション設定
	gmBoss1BodySetActionWhole(body_work, GME_BOSS1_ACT_ID_ESCAPE);
	
	// エッグマンに逃亡を通知
	body_work->flag	|= GMD_BOSS1_BODY_FLAG_SIGNAL_B2E_EGG_ESCAPE;
	
	// 振り返り初期化
	{
		BOOL	is_positive;
		if (gmBoss1BodyIsDirectionPositiveFromCurrent(body_work, GMD_BOSS1_RIGHTWARD_ANGLE)) {
			is_positive	= TRUE;
		}
		else {
			is_positive	= FALSE;
		}
		
		gmBoss1BodyInitTurnGently(body_work, GMD_BOSS1_RIGHTWARD_ANGLE,
								  90,
								  is_positive);
	}

	
	// 処理関数設定
	body_work->proc_update	= gmBoss1BodyStateUpdateEscapeWithTurn;
}

// 逃亡ステート終了関数
void gmBoss1BodyStateLeaveEscape(GMS_BOSS1_BODY_WORK *body_work)
{
	UNREFERENCED_PARAMETER(body_work);
}

// 逃亡ステート更新 振り返り
void gmBoss1BodyStateUpdateEscapeWithTurn(GMS_BOSS1_BODY_WORK *body_work)
{
	if (gmBoss1BodyUpdateTurnGently(body_work)) {
		// 加速移動初期化
		gmBoss1BodyInitEscapeMove(body_work);
		
		// 処理関数設定
		if (GmBsCmnIsFinalZoneType(GMM_BS_OBJ(GMM_BOSS1_MGR(body_work)))) {
			// ファイナルゾーンタイプ
			body_work->proc_update	= gmBoss1BodyStateUpdateEscapeWithMoveMoveFinalZone;
		}
		else {
			// 通常タイプ
			body_work->proc_update	= gmBoss1BodyStateUpdateEscapeWithMoveLocked;
		}
	}
}

// 逃亡ステート更新 移動処理（スクロールロック中）
void gmBoss1BodyStateUpdateEscapeWithMoveLocked(GMS_BOSS1_BODY_WORK *body_work)
{
	// 移動更新
	gmBoss1BodyUpdateEscapeMove(body_work);
		
	// スクロール解除判定
	if (gmBoss1BodyIsEscapeScrUnlock(body_work)) {
		
#if _IPHONE
		GmMapSetMapDrawSize(GME_MAP_DRAW_SIZE_HORI);
#endif // _IPHONE
		
		// 破片エフェクト生成
		gmBoss1EffBodyDebrisInit(body_work);
		body_work->flag	|= GMD_BOSS1_BODY_FLAG_EFF_DEBRIS_CREATED;	// 念のため
		
		// スクロールロック解除
		GmGmkCamScrLimitRelease(GMD_GMK_SCR_LMT_RELEASE_RIGHT);
		
		// 処理関数設定
		body_work->proc_update	= gmBoss1BodyStateUpdateEscapeWithMoveUnlocked;
	}
}

// 逃亡ステート更新 移動処理（スクロールロック解除後）
void gmBoss1BodyStateUpdateEscapeWithMoveUnlocked(GMS_BOSS1_BODY_WORK *body_work)
{
	// 画面外移動待ち
	if (gmBoss1BodyUpdateEscapeMove(body_work)) {
		// 消去
		GMM_BOSS1_MGR(body_work)->flag	|= GMD_BOSS1_MGR_FLAG_CLEAR_BOSS;
		
		body_work->proc_update	= NULL;
	}
}

// 逃亡ステート更新 ファイナルゾーン用移動処理
void gmBoss1BodyStateUpdateEscapeWithMoveMoveFinalZone(GMS_BOSS1_BODY_WORK *body_work)
{
	if (!(body_work->flag & GMD_BOSS1_BODY_FLAG_EFF_DEBRIS_CREATED)) {
		if (gmBoss1BodyIsEscapeScrUnlock(body_work)) {
			// 通常タイプでのスクロールロック解除タイミングで破片エフェクトを生成する
			gmBoss1EffBodyDebrisInit(body_work);
			body_work->flag	|= GMD_BOSS1_BODY_FLAG_EFF_DEBRIS_CREATED;
		}
	}
	
	if (gmBoss1BodyUpdateEscapeMove(body_work) ||
		gmBoss1BodyIsEscapeOutFinalZone(body_work)) {
		
		// スクロールロックの解除はボスオブジェクトを消去した後に行う
		
		// 消去
		GMM_BOSS1_MGR(body_work)->flag	|= GMD_BOSS1_MGR_FLAG_CLEAR_BOSS;
		
		body_work->proc_update	= NULL;
	}
}


// ############################################################################
// ボス1 鎖
// ############################################################################

// =======================================================================
// gmBoss1ChainExit
/*!
  鎖終了処理
  
  @note
  ノードマトリクス取得関連バッファを解放しています。
 */
// =======================================================================
void gmBoss1ChainExit(MTS_TASK_TCB *tcb)
{
	OBS_OBJECT_WORK	*obj_work	= (OBS_OBJECT_WORK*)mtTaskGetTcbWork(tcb);
	GMS_BOSS1_CHAIN_WORK	*chain_work	= (GMS_BOSS1_CHAIN_WORK*)obj_work;
	
	// オブジェクト生成数デクリメント
	gmBoss1MgrDecObjCreateCount(chain_work->mgr_work);
	
	// ボスモーションコールバックシステム解除・クリア
	GmBsCmnClearBossMotionCBSystem(obj_work);
	
	// ノードマトリクス取得関連解放
	GmBsCmnDeleteSNMWork(&chain_work->snm_work);
	
	// ノードマトリクス操作解除・クリア
	GmBsCmnClearCNMCb(obj_work);
	
	// ノードマトリクス操作処理管理ワーク削除
	GmBsCmnDeleteCNMMgrWork(&chain_work->cnm_mgr_work);
	
	// エネミー標準終了処理
	GmEnemyDefaultExit(tcb);
}

// ============================================================================
// 補助関数
// ============================================================================

// =======================================================================
// gmBoss1ChainUpdateAtkRectPosition
/*!
  攻撃矩形位置更新
  
  @param chain_work	[io]	鎖ワーク
  
  @note
  矩形の位置を鉄球の中心に合わせます。
 */
// =======================================================================
void gmBoss1ChainUpdateAtkRectPosition(GMS_BOSS1_CHAIN_WORK *chain_work)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(chain_work);
	const NNS_MATRIX	*w_mtx;
	
	// 鉄球中心ノードのワールドマトリクス取得
	w_mtx	= GmBsCmnGetSNMMtx(&chain_work->snm_work,
							   chain_work->ball_snm_reg_id);
	
	// 矩形の、オブジェクト中心からのオフセットを設定
	VEC_Set(&chain_work->ene_3d.ene_com.rect_work[GMD_ENEMY_RECT_ATK].rect.pos,
			FX_F32_TO_FX32(NNM_MTX(*w_mtx, 0, 3)) - obj_work->pos.x,
			FX_F32_TO_FX32(-NNM_MTX(*w_mtx, 1, 3)) - obj_work->pos.y,
			0);	// Zはオフセットしない
}

// ============================================================================
// 処理関数
// ============================================================================
// =======================================================================
// gmBoss1ChainAtkHitFunc
/*!
  鎖 攻撃ヒット時処理関数
  
  @param my_rect	[io]	自分の矩形
  @parma your_rect	[io]	相手の矩形
 */
// =======================================================================
void gmBoss1ChainAtkHitFunc(OBS_RECT_WORK *my_rect, OBS_RECT_WORK *your_rect)
{
	GMS_BOSS1_BODY_WORK	*body_work	= (GMS_BOSS1_BODY_WORK*)my_rect->parent_obj->parent_obj;
	
	// ヒットしたことを本体に知らせる
	body_work->flag	|= GMD_BOSS1_BODY_FLAG_SIGNAL_C2E_ATK_HIT;
	
	// エネミー標準攻撃ヒット処理
	GmEnemyDefaultAtkFunc(my_rect, your_rect);
}

// =======================================================================
// gmBoss1ChainOutFunc
/*!
  鎖 専用描画関数
  
  @param obj_work	[io]	オブジェクトワーク
  
  @note
  通常の描画に加えてノード操作処理も行っています。
 */
// =======================================================================
void gmBoss1ChainOutFunc(OBS_OBJECT_WORK *obj_work)
{
	GMS_BOSS1_CHAIN_WORK	*chain_work	= (GMS_BOSS1_CHAIN_WORK*)obj_work;
	
	// ノード操作更新
	GmBsCmnUpdateCNMParam(obj_work, &chain_work->cnm_mgr_work);
	
	// 標準描画関数
	ObjDrawActionSummary(obj_work);
}

// ============================================================================
// 制御処理
// ============================================================================
// =======================================================================
// gmBoss1ChainWaitSetup
/*!
  鎖 生成完了待ち
  
  @param obj_work	[io]	オブジェクトワーク
 */
// =======================================================================
void gmBoss1ChainWaitSetup(OBS_OBJECT_WORK *obj_work)
{
	GMS_BOSS1_CHAIN_WORK	*chain_work	= (GMS_BOSS1_CHAIN_WORK*)obj_work;
	GMS_BOSS1_BODY_WORK		*parent_body	= (GMS_BOSS1_BODY_WORK*)obj_work->parent_obj;
	
	// 全ての構成パーツのロードを待つ
	if (GMM_BOSS1_MGR(parent_body)->flag & GMD_BOSS1_MGR_FLAG_SETUP_END) {
		
		// BMCBシステム初期化
		GmBsCmnInitBossMotionCBSystem(obj_work,
									  &chain_work->bmcb_mgr);
		
		// ノードマトリクス取得初期化
		GmBsCmnCreateSNMWork(&chain_work->snm_work,
							 obj_work->obj_3d->object,
							 GMD_BOSS1_CHAIN_NODE_SNM_NUM);
		// SNMを行うモーションコールバックを実行リストに追加
		GmBsCmnAppendBossMotionCallback(&chain_work->bmcb_mgr,
										&chain_work->snm_work.bmcb_link);
		// ノードマトリクス取得ノード追加
		chain_work->ball_snm_reg_id	=
			GmBsCmnRegisterSNMNode(&chain_work->snm_work,
								   GMD_BOSS1_CHAIN_NODE_IDX_BALL);
		
		// 飛散パーツのノードマトリクス取得ノード追加
		for (Sint32 i = 0; i < GMD_BOSS1_SCATTER_PARTS_NUM; ++i) {
			chain_work->sct_snm_reg_ids[i]	=
				GmBsCmnRegisterSNMNode(&chain_work->snm_work,
									   GMD_BOSS1_CHAIN_NODE_IDX_RING_START + i);
		}
		
		// ノードマトリクス操作処理管理ワーク初期化
		GmBsCmnCreateCNMMgrWork(&chain_work->cnm_mgr_work,
								obj_work->obj_3d->object,
								GMD_BOSS1_CHAIN_NODE_CNM_NUM);
		
		// ノードマトリクス操作コールバック初期化
		GmBsCmnInitCNMCb(obj_work, &chain_work->cnm_mgr_work);
		
		// 操作ノード追加
		for (Sint32 i = 0; i < GMD_BOSS1_SCATTER_PARTS_NUM; ++i) {
			chain_work->sct_cnm_reg_ids[i]	=
				GmBsCmnRegisterCNMNode(&chain_work->cnm_mgr_work,
									   GMD_BOSS1_CHAIN_NODE_IDX_RING_START + i);
		}
		
		
		// メイン処理設定
		obj_work->ppFunc	= gmBoss1ChainMain;
	}
}

// =======================================================================
// gmBoss1ChainMain
/*!
  鎖 メイン処理関数
  
  @param obj_work	[io]	オブジェクトワーク
 */
// =======================================================================
void gmBoss1ChainMain(OBS_OBJECT_WORK *obj_work)
{
	GMS_BOSS1_CHAIN_WORK	*chain_work	= (GMS_BOSS1_CHAIN_WORK*)obj_work;
	GMS_BOSS1_BODY_WORK	*body_work	= (GMS_BOSS1_BODY_WORK*)obj_work->parent_obj;
	BOOL	b_apply_rotaion	= FALSE;
	
	if (body_work->flag & GMD_BOSS1_BODY_FLAG_CHAIN_DEPEND) {
		// 本体と一体になって動く
		obj_work->disp_flag	|= OBD_DISP_NODIRFLIP;
		b_apply_rotaion	= TRUE;
	}
	else {
		// 鎖が独立して動く
		obj_work->disp_flag	&= ~OBD_DISP_NODIRFLIP;
		b_apply_rotaion	= FALSE;
		
	}
	
	// 手動モーションマージ（元モーションをストップさせずにブレンド）
	if (chain_work->flag & GMD_BOSS1_CHAIN_FLAG_MANUAL_MOTION_MERGE) {
		obj_work->obj_3d->flag	&= ~OBD_ACTFLAG_3D_NN_BLEND;
		if (obj_work->obj_3d->marge > .0f) {
			obj_work->obj_3d->marge	-= obj_work->obj_3d->blend_spd;
		}
		else {
			chain_work->flag	&= ~GMD_BOSS1_CHAIN_FLAG_MANUAL_MOTION_MERGE;
			// 本来のリピート設定に戻す
			if (gm_boss1_act_id_tbl[body_work->whole_act_id][GME_BOSS1_PART_IDX_CHAIN].is_repeat) {
				obj_work->disp_flag	|= OBD_DISP_REPEAT;
			}
			else {
				obj_work->disp_flag	&= ~OBD_DISP_REPEAT;
			}
			obj_work->obj_3d->marge	= .0f;
		}
	}
	
	// 親アニメーションストップチェック
	if (GMM_BS_OBJ(body_work)->disp_flag & OBD_DISP_STOP) {
		obj_work->disp_flag	|= OBD_DISP_STOP;
	}
	else {
		if (!(body_work->flag & GMD_BOSS1_BODY_FLAG_MANUAL_CHAIN_MOTION)) {
			obj_work->disp_flag	&= ~OBD_DISP_STOP;
		}
	}
	
	// 非表示チェック
	if (body_work->flag & GMD_BOSS1_BODY_FLAG_CHAIN_NODISP) {
		obj_work->disp_flag	|= OBD_DISP_NODISP;
	}
	else {
		obj_work->disp_flag	&= ~OBD_DISP_NODISP;
	}
	
	// 矩形オフチェック
	if (body_work->flag & GMD_BOSS1_BODY_FLAG_CHAIN_NOHIT) {
		obj_work->flag	|= OBD_OBJECT_NOHIT;
	}
	else {
		obj_work->flag	&= ~OBD_OBJECT_NOHIT;
	}
	
	// 死亡チェック（親アニメーションストップにともなうDISP_STOPを上書く）
	if (body_work->flag & GMD_BOSS1_BODY_FLAG_CHAIN_DEAD) {
		obj_work->disp_flag	|= OBD_DISP_STOP;
		obj_work->flag	|= OBD_OBJECT_NOHIT;
	}
	
	// ノードにくっつける
	GmBsCmnUpdateObject3DNNStuckWithNode(obj_work, &body_work->snm_work,
									 body_work->chain_snm_reg_id,
									 b_apply_rotaion);
	
	// 衝撃波発生チェック
	if (body_work->flag & GMD_BOSS1_BODY_FLAG_SIGNAL_B2C_EFF_SW) {
		body_work->flag &= ~GMD_BOSS1_BODY_FLAG_SIGNAL_B2C_EFF_SW;
		
		gmBoss1EffShockwaveInit(chain_work);
	}
	
	// パーツ飛散発生チェック
	if (body_work->flag & GMD_BOSS1_BODY_FLAG_SIGNAL_B2C_SCATTER) {
		body_work->flag	&= ~GMD_BOSS1_BODY_FLAG_SIGNAL_B2C_SCATTER;
		
		gmBoss1EffScatterInit(chain_work);
	}
	
	// 当たり矩形位置調整
	gmBoss1ChainUpdateAtkRectPosition(chain_work);
}



// ############################################################################
// ボス1 エッグマン
// ############################################################################
// =======================================================================
// gmBoss1ChainExit
/*!
  鎖終了処理
  
  @note
  ノードマトリクス取得関連バッファを解放しています。
 */
// =======================================================================
void gmBoss1EggExit(MTS_TASK_TCB *tcb)
{
	OBS_OBJECT_WORK	*obj_work	= (OBS_OBJECT_WORK*)mtTaskGetTcbWork(tcb);
	GMS_BOSS1_EGG_WORK	*egg_work	= (GMS_BOSS1_EGG_WORK*)obj_work;
	
	// オブジェクト生成数デクリメント
	gmBoss1MgrDecObjCreateCount(egg_work->mgr_work);
	
	// エネミー標準終了処理
	GmEnemyDefaultExit(tcb);
}

// ============================================================================
// 補助関数
// ============================================================================
// =======================================================================
// gmBoss1EggSetActionIndependent
/*!
  エッグマン 独立アクション設定
  
  @param egg_work	[io]	本体ワーク
  @param act_id		[in]	全体アクションインデックス(GME_BOSS1_ACT_ID_XXX)
  
  @note
  エッグマンの独立アクションを設定します。
  gmBoss1EggRevertActionIndependent()を呼ぶと全体アクションの設定に戻ります。
 */
// =======================================================================
void gmBoss1EggSetActionIndependent(GMS_BOSS1_EGG_WORK *egg_work,
									GME_BOSS1_EGG_ACT_ID act_id, BOOL force_change/*=FALSE*/)
{
	const GMS_BOSS1_PART_ACT_INFO	*pt_act_info	= &gm_boss1_egg_act_id_tbl[act_id];
	OBS_OBJECT_WORK	*obj_egg	= GMM_BS_OBJ(egg_work);
	GMS_BOSS1_BODY_WORK	*parent_body	= (GMS_BOSS1_BODY_WORK*)obj_egg->parent_obj;
	
	// エッグマン独立アクション禁止なら何もしない
	if (parent_body->flag & GMD_BOSS1_BODY_FLAG_INDP_ACT_FORBIDDEN) {
		return;
	}
	
	// 独立アクション設定中に指定アクション設定済みなら何もしない
	if (!force_change &&
		((egg_work->flag & GMD_BOSS1_EGG_FLAG_INDP_ACT_SET)&&
		 (egg_work->egg_act_id == act_id))) {
		return;
	}
	
	// アクションID設定
	egg_work->egg_act_id	= act_id;
	
	// 独立アクション設定中
	egg_work->flag	|= GMD_BOSS1_EGG_FLAG_INDP_ACT_SET;
	
	// 構成パーツのアクション設定を反映
	
	
	// 継続フラグがオフの時のみ、新たなアクションを設定
	if (FALSE == pt_act_info->is_maintain) {
		GmBsCmnSetAction(obj_egg,
						 pt_act_info->act_id,
						 pt_act_info->is_repeat,
						 pt_act_info->is_blend);
	}
	else if (pt_act_info->is_repeat) {
		// リピートフラグは継続フラグの有無に関わらず反映
		GMM_BS_OBJ(egg_work)->disp_flag	|= OBD_DISP_REPEAT;
	}
	
#if defined(MTD_DEBUG)	// とりあえずエッグマンではサポートしない
	// 手動マージチェック
	if (pt_act_info->is_blend) {
		if (pt_act_info->is_merge_manual) {
			MTM_ASSERT(!"gmBoss1.cpp::gmBoss1EggSetActionIndependent() manual merge not supported\n");
		}
	}
#endif /* defined(MTD_DEBUG) */
	
	// モーション速度設定
	obj_egg->obj_3d->speed[0]	= pt_act_info->mtn_spd;
	
	// ブレンド速度設定
	obj_egg->obj_3d->blend_spd	= pt_act_info->blend_spd;
}



// =======================================================================
// gmBoss1EggRevertActionIndependent
/*!
  エッグマン 独立アクションから復帰
  
  @param egg_work	[io]	本体ワーク
  
  @note
  エッグマンの独立アクションから、本来のアクションに設定を戻します。
 */
// =======================================================================
void gmBoss1EggRevertActionIndependent(GMS_BOSS1_EGG_WORK *egg_work)
{
	OBS_OBJECT_WORK	*obj_egg	= GMM_BS_OBJ(egg_work);
	GMS_BOSS1_BODY_WORK	*body_work	= (GMS_BOSS1_BODY_WORK*)obj_egg->parent_obj;
	
	MTM_ASSERT(egg_work->flag & GMD_BOSS1_EGG_FLAG_INDP_ACT_SET);
	
	// 独立アクション解除
	egg_work->flag	&= ~GMD_BOSS1_EGG_FLAG_INDP_ACT_SET;
	
	// アクション設定
	GmBsCmnSetAction(obj_egg, body_work->egg_revert_mtn_id,
					 gm_boss1_act_id_tbl[body_work->whole_act_id][GME_BOSS1_PART_IDX_EGG].is_repeat,
					 TRUE);
	
	// 本体の経過フレームに合わせる
	obj_egg->obj_3d->frame[0]	= GMM_BS_OBJ(body_work)->obj_3d->frame[0];
}


// ============================================================================
// 制御処理
// ============================================================================

// =======================================================================
// gmBoss1EggWaitSetup
/*!
  エッグマン ロード完了待ち
  
  @param obj_work	[io]	オブジェクトワーク
 */
// =======================================================================
void gmBoss1EggWaitSetup(OBS_OBJECT_WORK *obj_work)
{
	GMS_BOSS1_BODY_WORK	*parent_body	= (GMS_BOSS1_BODY_WORK*)obj_work->parent_obj;
	GMS_BOSS1_EGG_WORK	*egg_work	= (GMS_BOSS1_EGG_WORK*)obj_work;
	
	if (GMM_BOSS1_MGR(parent_body)->flag & GMD_BOSS1_MGR_FLAG_SETUP_END) {
		// 更新関数設定
		obj_work->ppFunc	= gmBoss1EggMain;
		
		// 初期シーケンス設定
		gmBoss1EggProcIdleInit(egg_work);
	}
}

// =======================================================================
// gmBoss1EggMain
/*!
  エッグマン メイン処理関数
  
  @param obj_work	[io]	オブジェクトワーク
 */
// =======================================================================
void gmBoss1EggMain(OBS_OBJECT_WORK *obj_work)
{
	GMS_BOSS1_BODY_WORK	*body_work	= (GMS_BOSS1_BODY_WORK*)obj_work->parent_obj;
	GMS_BOSS1_EGG_WORK	*egg_work	= (GMS_BOSS1_EGG_WORK*)obj_work;
	
	// エッグマン設置ノードにくっつける
	GmBsCmnUpdateObject3DNNStuckWithNode(obj_work,
										 &body_work->snm_work,
										 body_work->egg_snm_reg_id,
										 TRUE);
	
	// 更新処理
	if (egg_work->proc_update) {
		egg_work->proc_update(egg_work);
	}
	
	// 逃亡演出開始チェック
	if (body_work->flag & GMD_BOSS1_BODY_FLAG_SIGNAL_B2E_EGG_ESCAPE) {
		body_work->flag	&= ~GMD_BOSS1_BODY_FLAG_SIGNAL_B2E_EGG_ESCAPE;
		gmBoss1EggProcEscapeInit(egg_work);
	}
	
	// ダメージ演出開始チェック
	if (body_work->flag & GMD_BOSS1_BODY_FLAG_SIGNAL_B2E_EGG_DAMAGE) {
		body_work->flag &= ~GMD_BOSS1_BODY_FLAG_SIGNAL_B2E_EGG_DAMAGE;
		gmBoss1EggProcDamageInit(egg_work);
	}
	
	// 黒こげチェック
	if (body_work->flag & GMD_BOSS1_BODY_FLAG_SIGNAL_B2E_EGG_BURNT) {
		body_work->flag &= ~GMD_BOSS1_BODY_FLAG_SIGNAL_B2E_EGG_BURNT;
		// エッグマンを黒こげテクスチャに設定
		gmBoss1SetPartTextureBurnt(obj_work);
	}
	
	// 親アニメーションストップチェック
	if (GMM_BS_OBJ(body_work)->disp_flag & OBD_DISP_STOP) {
		obj_work->disp_flag	|= OBD_DISP_STOP;
	}
	else {
		obj_work->disp_flag	&= ~OBD_DISP_STOP;
	}
}


// ============================================================================
// シーケンス処理
// ============================================================================

// =======================================================================
// gmBoss1EggProcIdle***
/*!
  通常時停滞シーケンス処理関数
 */
// =======================================================================
// 通常時停滞シーケンス 初期化
void gmBoss1EggProcIdleInit(GMS_BOSS1_EGG_WORK *egg_work)
{
	// 処理関数設定
	egg_work->proc_update	= gmBoss1EggProcIdleUpdateLoop;
}

// 通常時停滞シーケンス 更新
void gmBoss1EggProcIdleUpdateLoop(GMS_BOSS1_EGG_WORK *egg_work)
{
	OBS_OBJECT_WORK	*obj_egg	= GMM_BS_OBJ(egg_work);
	GMS_BOSS1_BODY_WORK	*body_work	= (GMS_BOSS1_BODY_WORK*)obj_egg->parent_obj;
		
	// ヒット通知確認
	if (body_work->flag & GMD_BOSS1_BODY_FLAG_SIGNAL_C2E_ATK_HIT) {
		body_work->flag &= ~GMD_BOSS1_BODY_FLAG_SIGNAL_C2E_ATK_HIT;
		
		// 笑いシーケンス初期化
		gmBoss1EggProcLaughInit(egg_work);
	}
}

// =======================================================================
// gmBoss1EggProcLaugh***
/*!
  笑いシーケンス処理関数
 */
// =======================================================================
// 笑いシーケンス初期化
void gmBoss1EggProcLaughInit(GMS_BOSS1_EGG_WORK *egg_work)
{
	// エッグマン独立アクション設定
	gmBoss1EggSetActionIndependent(egg_work, GME_BOSS1_EGG_ACT_ID_LAUGH);
	
	// 処理関数設定
	egg_work->proc_update	= gmBoss1EggProcLaughUpdateLoop;
}

// 笑いシーケンス更新
void gmBoss1EggProcLaughUpdateLoop(GMS_BOSS1_EGG_WORK *egg_work)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(egg_work);
	
	if (GmBsCmnIsActionEnd(obj_work)) {
		// アクションを元に戻す
		gmBoss1EggRevertActionIndependent(egg_work);
		
		// 通常停滞に戻る
		gmBoss1EggProcIdleInit(egg_work);
	}
}

// =======================================================================
// gmBoss1EggProcDamage***
/*!
  ダメージシーケンス処理関数
 */
// =======================================================================
// ダメージシーケンス初期化
void gmBoss1EggProcDamageInit(GMS_BOSS1_EGG_WORK *egg_work)
{
	// エッグマン独立アクション設定
	gmBoss1EggSetActionIndependent(egg_work, GME_BOSS1_EGG_ACT_ID_DAMAGE);
	
	// 汗エフェクト生成
	gmBoss1EffSweatInit(egg_work);
	
	// 処理関数設定
	egg_work->proc_update	= gmBoss1EggProcDamageUpdateLoop;
}

// ダメージシーケンス更新
void gmBoss1EggProcDamageUpdateLoop(GMS_BOSS1_EGG_WORK *egg_work)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(egg_work);
	
	if (GmBsCmnIsActionEnd(obj_work)) {
		
		// 汗終了
		egg_work->flag	&= ~GMD_BOSS1_EGG_FLAG_SWEAT_ACTIVE;
		
		// アクションを元に戻す
		gmBoss1EggRevertActionIndependent(egg_work);
		
		// 通常停滞に戻る
		gmBoss1EggProcIdleInit(egg_work);
	}
}

// =======================================================================
// gmBoss1EggProcDefeat***
/*!
  撃破シーケンス処理関数
 */
// =======================================================================
// 撃破シーケンス初期化
void gmBoss1EggProcEscapeInit(GMS_BOSS1_EGG_WORK* egg_work)
{
	if (!(egg_work->flag & GMD_BOSS1_EGG_FLAG_SWEAT_ACTIVE)) {
		// 汗エフェクト生成
		gmBoss1EffSweatInit(egg_work);
	}
	
	// 処理関数設定
	egg_work->proc_update	= gmBoss1EggProcEscapeUpdateLoop;
}

// 撃破シーケンス更新
void gmBoss1EggProcEscapeUpdateLoop(GMS_BOSS1_EGG_WORK *egg_work)
{
	UNREFERENCED_PARAMETER(egg_work);
}


// ############################################################################
// 衝撃波エフェクト
// ############################################################################

// =======================================================================
// gmBoss1EffShockwaveInit
/*!
  衝撃波 初期化
  
  @param body_work	[io]	本体ワーク
  
  @return 3desエフェクトワーク
  
  @note
  真ん中パーツと、左右それぞれの衝撃波パーツで構成されます。
 */
// =======================================================================
GMS_EFFECT_3DES_WORK* gmBoss1EffShockwaveInit(GMS_BOSS1_CHAIN_WORK *chain_work)
{
	GMS_EFFECT_3DES_WORK	*eff_3des;
	GMS_BOSS1_EFF_SHOCKWAVE_WORK	*sw_work;
	OBS_OBJECT_WORK	*obj_work;
	
	obj_work	= GMM_EFFECT_CREATE_WORK(sizeof(GMS_BOSS1_EFF_SHOCKWAVE_WORK),
										 GMM_BS_OBJ(chain_work),
										 0,
										 "B01_ShockWave");
	
	eff_3des	= (GMS_EFFECT_3DES_WORK*)obj_work;
	sw_work	= (GMS_BOSS1_EFF_SHOCKWAVE_WORK*)eff_3des;
	
	// 管理ワークへの参照設定
	sw_work->mgr_work	= chain_work->mgr_work;
	
	// オブジェクト生成数加算
	gmBoss1MgrIncObjCreateCount(sw_work->mgr_work);
	
	// ファイナルゾーンの時はエフェクトを差し替え
	Sint32	es_dwork_no;
	if (GmBsCmnIsFinalZoneType(GMM_BS_OBJ(chain_work->mgr_work))) {
		es_dwork_no	= GMD_DWORK_NO_BOSS_01_EF_SW02_ES;
	}
	else {
		es_dwork_no	= GMD_DWORK_NO_BOSS_01_EF_SW00_ES;
	}
	
	// ESエフェクトデータロード
	ObjObjectAction3dESEffectLoad(GMM_BS_OBJ(eff_3des),
								  &eff_3des->obj_3des,
								  ObjDataGet(es_dwork_no),
								  NULL,//filename
								  0,//amb_index
								  NULL);
	
	// ESテクスチャデータロード
	ObjObjectAction3dESTextureLoad(GMM_BS_OBJ(eff_3des),
								   &eff_3des->obj_3des,
								   ObjDataGet(GMD_DWORK_NO_BOSS_01_EF_SW_AMBTEX),
								   NULL,//filename
								   0,//amb_index
								   NULL,
								   FALSE);	// 転送しない
	// ロード済みESテクスチャセット
	ObjObjectAction3dESTextureSetByDwork(obj_work,
										 ObjDataGet(GMD_DWORK_NO_BOSS_01_EF_SW_TEXLIST));
	
	// 基本設定
	GmEffect3DESSetupBase(eff_3des, GME_EFFECT_3DES_POS_TYPE_EMT,
						  GMD_EFFECT_3DES_FLAG_NOFLIP);
	
	
	// 座標設定
	{
		const NNS_MATRIX	*w_mtx;
		
		// 鉄球中心ノードのワールドマトリクス取得
		w_mtx	= GmBsCmnGetSNMMtx(&chain_work->snm_work,
								   chain_work->ball_snm_reg_id);
		
		// 地面の高さに設置
		VEC_Set(&obj_work->pos,
				FX_F32_TO_FX32(NNM_MTX(*w_mtx, 0, 3)),
				GMD_BOSS1_GROUND_POS_Y,
				0);
	}
	
	// 当たり設定
	obj_work->flag	&= ~OBD_OBJECT_NOHIT;
	GmEffectRectInit(&eff_3des->efct_com,
					 gm_boss1_eff_sw_atk_flag_tbl,
					 gm_boss1_eff_sw_def_flag_tbl,
					 GMD_OBJ_RECT_GROUP_ENEMY,
					 GMD_OBJ_RECT_TARGET_GROUPFLAG_PLAYER);
	
	// 矩形サイズ設定
	ObjRectWorkSet(&eff_3des->efct_com.rect_work[GME_EFFECT_RECT_ATK],
				   GMD_BOSS1_EFF_SHOCKWAVE_ATK_RECT_SIZE_LEFT,
				   GMD_BOSS1_EFF_SHOCKWAVE_ATK_RECT_SIZE_TOP,
				   GMD_BOSS1_EFF_SHOCKWAVE_ATK_RECT_SIZE_RIGHT,
				   GMD_BOSS1_EFF_SHOCKWAVE_ATK_RECT_SIZE_BOTTOM);
	
	// 衝撃波攻撃矩形有効時間設定
	sw_work->atk_rect_timer	= GMD_BOSS1_EFF_SHOCKWAVE_ATK_RECT_TIME;
	
	// 衝撃波処理関数設定
	obj_work->ppFunc	= gmBoss1EffShockwaveProcMain;
	
	// 左右サブパーツ作成
	gmBoss1EffShockwaveSubpartInit(sw_work, GMD_BOSS1_EFF_SHOCKWAVE_SUB_START_OFST_X, TRUE);	// 左パーツ
	gmBoss1EffShockwaveSubpartInit(sw_work, GMD_BOSS1_EFF_SHOCKWAVE_SUB_START_OFST_X, FALSE);	// 右パーツ
	
	// 終了処理差し替え
	mtTaskChangeTcbDestructor(obj_work->tcb, gmBoss1EffShockwaveExit);
	
	return eff_3des;
}

// =======================================================================
// gmBoss1EffShockwaveExit
/*!
  衝撃波 終了処理
  
  @param tcb	[io]	TCB
 */
// =======================================================================
void gmBoss1EffShockwaveExit(MTS_TASK_TCB *tcb)
{
	GMS_BOSS1_EFF_SHOCKWAVE_WORK	*sw_work	= (GMS_BOSS1_EFF_SHOCKWAVE_WORK*)mtTaskGetTcbWork(tcb);
	
	// オブジェクト生成数デクリメント
	gmBoss1MgrDecObjCreateCount(sw_work->mgr_work);
	
	GmEffectDefaultExit(tcb);
}

// =======================================================================
// gmBoss1EffShockwaveProcMain
/*!
  衝撃波 処理関数
 */
// =======================================================================
void gmBoss1EffShockwaveProcMain(OBS_OBJECT_WORK *obj_work)
{
	GMS_BOSS1_EFF_SHOCKWAVE_WORK	*sw_work	= (GMS_BOSS1_EFF_SHOCKWAVE_WORK*)obj_work;
	
	// 既定時間攻撃矩形を有効にする
	if (sw_work->atk_rect_timer) {
		sw_work->atk_rect_timer--;
	}
	else {
		obj_work->flag	|= OBD_OBJECT_NOHIT;
	}
	
	if (obj_work->disp_flag & OBD_DISP_END) {
		obj_work->flag	|= OBD_OBJECT_TASKCLEAR;
	}
}

// =======================================================================
// gmBoss1EffShockwaveSubpartInit
/*!
  衝撃波 サブパーツ 初期化
  
  @param sw_work	[io]	親衝撃波ワーク
  @param ofst_h		[in]	親からの水平方向オフセット
  @param is_left	[in]	左側パーツフラグ
  
  @return 3DESエフェクトワーク
 */
// =======================================================================
GMS_EFFECT_3DES_WORK* gmBoss1EffShockwaveSubpartInit(GMS_BOSS1_EFF_SHOCKWAVE_WORK *sw_work,
													 fx32 ofst_h, BOOL is_left)
{
	OBS_OBJECT_WORK	*obj_work;
	GMS_EFFECT_3DES_WORK	*eff_3des;
	GMS_BOSS1_EFF_SHOCKWAVE_SUB_WORK	*sw_sub_work;
	
	MTM_ASSERT(ofst_h >= 0);
	
	obj_work	= GMM_EFFECT_CREATE_WORK(sizeof(GMS_BOSS1_EFF_SHOCKWAVE_SUB_WORK),
										 GMM_BS_OBJ(sw_work)->parent_obj,
										 0,
										 "B01_SW_Subpart");
	// ↑真ん中パーツを親にすると、親再生終了時に強制停止してしまうので鎖を親にする
	
	eff_3des	= (GMS_EFFECT_3DES_WORK*)obj_work;
	sw_sub_work	= (GMS_BOSS1_EFF_SHOCKWAVE_SUB_WORK*)eff_3des;
	
	// 管理ワークへの参照設定
	sw_sub_work->mgr_work	= sw_work->mgr_work;
	
	// オブジェクト生成数加算
	gmBoss1MgrIncObjCreateCount(sw_sub_work->mgr_work);
	
	// ESエフェクトデータロード
	ObjObjectAction3dESEffectLoad(GMM_BS_OBJ(eff_3des),
								  &eff_3des->obj_3des,
								  ObjDataGet(GMD_DWORK_NO_BOSS_01_EF_SW01_ES),
								  NULL,//filename
								  0,//amb_index
								  NULL);
	
	// ESテクスチャデータロード
	ObjObjectAction3dESTextureLoad(GMM_BS_OBJ(eff_3des),
								   &eff_3des->obj_3des,
								   ObjDataGet(GMD_DWORK_NO_BOSS_01_EF_SW_AMBTEX),
								   NULL,//filename
								   0,//amb_index
								   NULL,
								   FALSE);	// 転送しない
	// ロード済みESテクスチャセット
	ObjObjectAction3dESTextureSetByDwork(obj_work,
										 ObjDataGet(GMD_DWORK_NO_BOSS_01_EF_SW_TEXLIST));
	
	// 基本設定
	GmEffect3DESSetupBase(eff_3des, GME_EFFECT_3DES_POS_TYPE_EMT,
						  0);	// フリップする
	
	// 表示角度設定
	GmEffect3DESSetDispRotation(eff_3des, GMD_BOSS1_EFF_SHOCKWAVE_SUB_ROT_X, 0, 0);
	
	// 表示オフセット設定
	GmEffect3DESSetDispOffset(eff_3des,
							  0,
							  -FX_FX32_TO_F32(GMD_BOSS1_EFF_SHOCKWAVE_SUB_OFST_Y),
							  FX_FX32_TO_F32(-ofst_h));
	
	// 座標設定（真ん中パーツ基準）
	obj_work->pos.x	= GMM_BS_OBJ(sw_work)->pos.x;
	obj_work->pos.y	= GMM_BS_OBJ(sw_work)->pos.y;
	obj_work->pos.z	= GMM_BS_OBJ(sw_work)->pos.z;
	
	// 左右設定を反映
	if (is_left) {
		obj_work->disp_flag	&= ~OBD_DISP_HFLIP;	// 左側パーツは右向き
	}
	else {
		obj_work->disp_flag	|= OBD_DISP_HFLIP;	// 右側パーツは左向き
	}
	
	// 終了処理差し替え
	mtTaskChangeTcbDestructor(obj_work->tcb, gmBoss1EffShockwaveSubExit);
	
	return eff_3des;
}

// =======================================================================
// gmBoss1EffShockwaveSubExit
/*!
  衝撃波サブパーツ 終了処理
  
  @param tcb	[io]	TCB
 */
// =======================================================================
void gmBoss1EffShockwaveSubExit(MTS_TASK_TCB *tcb)
{
	GMS_BOSS1_EFF_SHOCKWAVE_SUB_WORK	*sw_sub_work	= (GMS_BOSS1_EFF_SHOCKWAVE_SUB_WORK*)mtTaskGetTcbWork(tcb);
	
	// オブジェクト生成数デクリメント
	gmBoss1MgrDecObjCreateCount(sw_sub_work->mgr_work);
	
	GmEffectDefaultExit(tcb);
}

// ############################################################################
// パーツ飛散エフェクト
// ############################################################################

// =======================================================================
// gmBoss1EffScatterInit
/*!
  パーツ飛散エフェクト 初期化
  
  @param chain_work	[io]	鎖ワーク
  
  @note
  ノード操作オブジェクトを生成します。
 */
// =======================================================================
void gmBoss1EffScatterInit(GMS_BOSS1_CHAIN_WORK *chain_work)
{
	GMS_BS_CMN_NODE_CTRL_OBJECT	*ndc_obj;
	GMS_BOSS1_EFF_SCT_PART_NDC_WORK	*sct_part_ndc;
	
	for (Sint32 i = 0; i < GMD_BOSS1_SCATTER_PARTS_NUM; ++i) {
		ndc_obj	= GmBsCmnCreateNodeControlObjectBySize(GMM_BS_OBJ(chain_work),
													   &chain_work->cnm_mgr_work,
													   chain_work->sct_cnm_reg_ids[i],
													   &chain_work->snm_work,
													   chain_work->sct_snm_reg_ids[i],
													   sizeof(GMS_BOSS1_EFF_SCT_PART_NDC_WORK));
		
		sct_part_ndc	= (GMS_BOSS1_EFF_SCT_PART_NDC_WORK*)ndc_obj;
		
		// 遅延時間をランダムに設定
		ndc_obj->user_timer	= (Uint32)(mtMathRand() % GMD_BOSS1_EFF_SCT_PART_FLY_DELAY_MAX);
		
		// ワーク設定
		ndc_obj->is_enable	= FALSE;	// 最初は反映しない
		
		// パーツ毎の初期パラメータ設定
		gmBoss1EffScatterSetPartParam(sct_part_ndc,
									  ((i == (GMD_BOSS1_SCATTER_PARTS_NUM - 1)) ? TRUE : FALSE));	// 最後のパーツ==鉄球
		
		// 最初は地面に当たらない
		GMM_BS_OBJ(ndc_obj)->move_flag	|= OBD_MOVE_NOCOLOBJ | OBD_MOVE_NOCOLFIELD;
		
		// ユーザ使用クォータニオン初期化
		nnMakeUnitQuaternion(&ndc_obj->user_quat);
		
		// 処理関数設定
		ndc_obj->proc_update	= gmBoss1EffScatterProcWait;
	}
}


// ============================================================================
// 補助関数
// ============================================================================
// =======================================================================
// gmBoss1EffScatterSetPartParam
/*!
  パーツ飛散エフェクト パーツ毎パラメータ設定
 
  @param sct_part_ndc	[io]	パーツ操作NDCワーク
  @param is_ironball	[in]	鉄球タイプフラグ
  
  @note
  パーツ毎のパラメータ設定を行います。
 */
// =======================================================================
void gmBoss1EffScatterSetPartParam(GMS_BOSS1_EFF_SCT_PART_NDC_WORK *sct_part_ndc,
								   BOOL is_ironball)
{
	GMS_BS_CMN_NODE_CTRL_OBJECT	*ndc_obj	= (GMS_BS_CMN_NODE_CTRL_OBJECT*)sct_part_ndc;
	Angle32	spin_spd_angle;
	
	// パーツ種類別の設定
	if (is_ironball) {
		sct_part_ndc->is_ironball	= TRUE;
		
		// オフセット設定（回転中心をずらすため）
		ndc_obj->user_ofst.y	= GMD_BOSS1_EFF_SCT_PART_IBALL_OFST_Y;
		
		// 回転角速度
		spin_spd_angle	= GMD_BOSS1_EFF_SCT_PART_IBALL_SPIN_SPD_DEG;
	}
	else {
		sct_part_ndc->is_ironball	= FALSE;
		
		// オフセット設定（回転中心をずらすため）
		ndc_obj->user_ofst.y	= GMD_BOSS1_EFF_SCT_PART_RING_OFST_Y;
		
		// 回転角速度
		spin_spd_angle	= GMD_BOSS1_EFF_SCT_PART_RING_SPIN_SPD_DEG;
	}
	
	// 既定回数ひねりを加える差分回転クォータニオンを設定
	nnMakeUnitQuaternion(&sct_part_ndc->spin_quat);
	for (Sint32 i = 0; i < GMD_BOSS1_EFF_SCT_SPIN_AXIS_NUM; ++i) {
		Float	rand_z;
		Angle16	rand_angle;
		NNS_VECTOR	spin_axis;
		AMS_QUAT	diff_rot;
		
		// ランダムな回転軸を設定
		rand_z	= FX_FX32_TO_F32(AkMathRandFx()) * 2.f - 1.f;	// -1.f ～ 1.f
		rand_z	= MTM_MATH_CLIP(rand_z, -1.f, 1.f);
		rand_angle	= AKM_DEGtoA16(360.f * FX_FX32_TO_F32(AkMathRandFx()));	// 0～360degのランダム角度
		AkMathGetRandomUnitVector(&spin_axis, rand_z, rand_angle);
		
		nnMakeRotateAxisQuaternion(&diff_rot, spin_axis.x, spin_axis.y, spin_axis.z,
								   spin_spd_angle);
		nnMultiplyQuaternion(&sct_part_ndc->spin_quat, &diff_rot, &sct_part_ndc->spin_quat);
	}
	
	// 地形当たり設定
	if (is_ironball) {
		ObjObjectFieldRectSet(GMM_BS_OBJ(ndc_obj),
							  GMD_BOSS1_EFF_SCT_PART_IBALL_FRECT_LEFT,
							  GMD_BOSS1_EFF_SCT_PART_IBALL_FRECT_TOP,
							  GMD_BOSS1_EFF_SCT_PART_IBALL_FRECT_RIGHT,
							  GMD_BOSS1_EFF_SCT_PART_IBALL_FRECT_BOTTOM);
	}
	else {
		ObjObjectFieldRectSet(GMM_BS_OBJ(ndc_obj),
							  GMD_BOSS1_EFF_SCT_PART_RING_FRECT_LEFT,
							  GMD_BOSS1_EFF_SCT_PART_RING_FRECT_TOP,
							  GMD_BOSS1_EFF_SCT_PART_RING_FRECT_RIGHT,
							  GMD_BOSS1_EFF_SCT_PART_RING_FRECT_BOTTOM);
	}
}

// =======================================================================
// gmBoss1EffScatterSetFlyParam
/*!
  パーツ飛散エフェクト パーツ飛散パラメータ設定
  
  @param sct_part_ndc	[io]	パーツ操作NDCワーク
  
  @note
  主に移動関連のパラメータを設定します。
 */
// =======================================================================
void gmBoss1EffScatterSetFlyParam(GMS_BOSS1_EFF_SCT_PART_NDC_WORK *sct_part_ndc)
{
	OBS_OBJECT_WORK	*obj_work	= GMM_BS_OBJ(sct_part_ndc);
	
	// 左右90度の範囲でランダムに飛散
	/*
	  ＼     ／
	  ←＼ ／→
	   ← C →
      ←／ ＼→
	  ／     ＼
	*/
	Sint32	rand_deg	= ((Sint32)mtMathRand() % 180);
	Angle32	rand_angle	= AKM_DEGtoA32(rand_deg + (rand_deg > 90 ? 45 : -45));
	Float	spd;
	
	if (sct_part_ndc->is_ironball) {
		spd	= GMD_BOSS1_EFF_SCT_PART_IBALL_FLY_SPD;
	}
	else {
		spd	= GMD_BOSS1_EFF_SCT_PART_RING_FLY_SPD;
	}
	
	obj_work->spd.y	= (fx32)(FX32_ONE * spd * nnSin(rand_angle));
	obj_work->spd.x	= (fx32)(FX32_ONE * spd * nnCos(rand_angle));
	obj_work->move_flag	|= OBD_MOVE_FALL;
}

// ============================================================================
// 処理関数
// ============================================================================

// ============================================================================
// 制御処理
// ============================================================================


// ============================================================================
// シーケンス処理
// ============================================================================

// =======================================================================
// gmBoss1EffScatterProc***
/*!
  パーツ飛散エフェクト パーツ更新関数
 */
// =======================================================================
// パーツ更新 飛散前待機処理
void gmBoss1EffScatterProcWait(OBS_OBJECT_WORK *obj_work)
{
	GMS_BS_CMN_NODE_CTRL_OBJECT	*ndc_obj	= (GMS_BS_CMN_NODE_CTRL_OBJECT*)obj_work;
	GMS_BOSS1_EFF_SCT_PART_NDC_WORK	*sct_part_ndc	= (GMS_BOSS1_EFF_SCT_PART_NDC_WORK*)ndc_obj;
	
	
	if (ndc_obj->user_timer) {
		ndc_obj->user_timer--;
	}
	else {
		// 初期位置設定
		GmBsCmnAttachNCObjectToSNMNode(ndc_obj);
		
		// 飛散パラメータ設定
		gmBoss1EffScatterSetFlyParam(sct_part_ndc);
		
		// ノードへのマトリクス反映有効化
		ndc_obj->is_enable	= TRUE;
		
		// 消去タイマ設定
		ndc_obj->user_timer	= GMD_BOSS1_EFF_SCT_PART_FLY_DELETE_TIME;
		
		// 処理関数設定
		ndc_obj->proc_update	= gmBoss1EffScatterProcFly;
	}
}

// パーツ更新 飛散処理
void gmBoss1EffScatterProcFly(OBS_OBJECT_WORK *obj_work)
{
	GMS_BS_CMN_NODE_CTRL_OBJECT	*ndc_obj	= (GMS_BS_CMN_NODE_CTRL_OBJECT*)obj_work;
	GMS_BOSS1_EFF_SCT_PART_NDC_WORK	*sct_part_ndc	= (GMS_BOSS1_EFF_SCT_PART_NDC_WORK*)ndc_obj;
	
	// 回転運動させる
	nnMultiplyQuaternion(&ndc_obj->user_quat, &sct_part_ndc->spin_quat, &ndc_obj->user_quat);
	
	// 姿勢設定
	GmBsCmnSetWorldMtxFromNCObjectPosture(ndc_obj);
	
	if (ndc_obj->user_timer) {
		ndc_obj->user_timer--;
	}
	else {
		//ndc_obj->is_enable	= FALSE;	// ←最後の更新値を反映させておくのでFALSEにしない
		obj_work->flag	|= OBD_OBJECT_TASKCLEAR;
	}
}


// ############################################################################
// 爆発エフェクト
// ############################################################################
// =======================================================================
// gmBoss1EffBombInitCreate
/*!
  爆発エフェクト 生成処理初期化
 
  @param bomb_work		[io]	本体ワーク
  @param bomb_type		[in]	爆発タイプ
  @param pos_x			[in]	生成範囲中心座標X
  @param pos_y			[in]	生成範囲中心座標Y
  @param width			[in]	生成範囲幅
  @param height			[in]	生成範囲高さ
  @param interval_min	[in]	生成間隔最短時間
  @param interval_max	[in]	生成間隔最長時間
 */
// =======================================================================
void gmBoss1EffBombInitCreate(GMS_BOSS1_EFF_BOMB_WORK *bomb_work,
							  GME_BOSS1_EFF_BOMB_TYPE bomb_type,
							  OBS_OBJECT_WORK *parent_obj,
							  fx32 pos_x, fx32 pos_y, fx32 width, fx32 height,
							  Uint32 interval_min, Uint32 interval_max)
{
	MTM_ASSERT(bomb_work);
	MTM_ASSERT(parent_obj);
	
	bomb_work->parent_obj	= parent_obj;
	bomb_work->bomb_type	= bomb_type;
	bomb_work->interval_timer	= 0;
	bomb_work->interval_min	= interval_min;
	bomb_work->interval_max	= interval_max;
	bomb_work->pos[MTD_X]	= pos_x;
	bomb_work->pos[MTD_Y]	= pos_y;
	bomb_work->area[MTD_WIDTH]	= width;
	bomb_work->area[MTD_HEIGHT]	= height;
#if _IPHONE
	bomb_work->interval_timer_sound = 0;
#endif // _IPHONE
}

// =======================================================================
// gmBoss1EffBombUpdateCreate
/*!
  爆発エフェクト 生成処理更新
  
  @param bomb_work	[io]	爆発ワーク
  
  @note
  初期化時に指定されたパラメータで爆発エフェクトを生成します。
  生成期間中は毎フレーム呼び出してください。
 */
// =======================================================================
void gmBoss1EffBombUpdateCreate(GMS_BOSS1_EFF_BOMB_WORK *bomb_work)

{
	MTM_ASSERT(bomb_work->parent_obj);	// Z位置を決めるのに親を使用
	
	if (bomb_work->interval_timer) {
		bomb_work->interval_timer--;
	}
	else {
		GMS_EFFECT_3DES_WORK	*efct_work	= NULL;
		OBS_OBJECT_WORK	*obj_work;
		fx32	width	= bomb_work->area[MTD_WIDTH];
		fx32	height	= bomb_work->area[MTD_HEIGHT];
		fx32	rand_x;
		fx32	rand_y;
		
		// (pos_x, pos_y) が中心となるwidth x heightの長方形内のランダムな座標を取得
		rand_x	= FX_Mul(AkMathRandFx(), width);
		rand_y	= FX_Mul(AkMathRandFx(), height);
		
		// エフェクト生成
		switch (bomb_work->bomb_type) {
		case GME_BOSS1_EFF_BOMB_TYPE_SMALL:	// 小爆発
			efct_work	= GmEfctCmnEsCreate(NULL, GME_EFCT_CMN_IDX_BOMB);
			
#if _IPHONE
			if (--bomb_work->interval_timer_sound > 0) {
				break;
			}
			bomb_work->interval_timer_sound = 3;
#endif // _IPHONE
			// 小爆発SE再生
			GmSoundPlaySE("Boss0_02");
			break;
		default:
			MTM_ASSERT(!"gmBoss1::gmBoss1EffBombUpdateCreate() invalid bomb type\n");
			return;
		}
		
		obj_work	= GMM_BS_OBJ(efct_work);
		
		MTM_ASSERT(obj_work);
		
		// 座標設定
		obj_work->pos.x	= bomb_work->pos[MTD_X] - (width >> 1) + rand_x;
		obj_work->pos.y	= bomb_work->pos[MTD_Y] - (height >> 1) + rand_y;
		// Z方向に一律にずらす
		obj_work->pos.z	= GMM_BS_OBJ(bomb_work->parent_obj)->pos.z + GMD_BOSS1_EFF_BOMB_OFST_Z;
		
		// 次の生成までのインターバルを設定
		{
			Uint32	rand_ofst;
			rand_ofst	= (Uint32)((AkMathRandFx() * (bomb_work->interval_max - bomb_work->interval_min)) >> FX32_SHIFT);
			bomb_work->interval_timer = bomb_work->interval_min + rand_ofst;
		}
	}
}


// ############################################################################
// ダメージエフェクト
// ############################################################################
// =======================================================================
// gmBoss1EffDamageInit
/*!
  ダメージエフェクト初期化
  
  @param body_work	[io]	本体ワーク
  
  @note
  破片の出るエフェクトです。
 */
// =======================================================================
void gmBoss1EffDamageInit(GMS_BOSS1_BODY_WORK *body_work)
{
	OBS_OBJECT_WORK	*parent_obj	= GMM_BS_OBJ(body_work);
	GMS_EFFECT_3DES_WORK	*efct_work;
	efct_work	= GmEfctBossCmnEsCreate(parent_obj, GME_EFCT_BOSS_CMN_IDX_BOSS_DM);
	GMM_BS_OBJ(efct_work)->pos.z	+= GMD_BOSS1_EFF_DAMAGE_OFST_Z;
}

// ############################################################################
// アフターバーナーエフェクト
// ############################################################################
// =======================================================================
// gmBoss1EffAfterburnerSetEnable
/*!
  アフターバーナーエフェクト 初期化
  
  @param body_work	[io]	本体ワーク
  @param is_enable	[in]	有効フラグ（TRUE:有効化, FALSE:無効化）
  
  @note
  TRUEを指定するとアフターバーナーエフェクトの生成をトリガーします。
  FALSEを指定するとすでに生成されているアフターバーナーエフェクトが消去されるように
  フラグ設定が行われます。
 */
// =======================================================================
void gmBoss1EffAfterburnerSetEnable(GMS_BOSS1_BODY_WORK *body_work, BOOL is_enable)
{
	if (is_enable) {
		MTM_ASSERT(!(body_work->flag & GMD_BOSS1_BODY_FLAG_ABURNER_ACTIVE));
		MTM_ASSERT(!(body_work->flag & GMD_BOSS1_BODY_FLAG_SIGNAL_B2F_ABURNER));
		
		body_work->flag	|= GMD_BOSS1_BODY_FLAG_SIGNAL_B2F_ABURNER;
	}
	else {
		body_work->flag	&= ~GMD_BOSS1_BODY_FLAG_ABURNER_ACTIVE;
		body_work->flag	&= ~GMD_BOSS1_BODY_FLAG_SIGNAL_B2F_ABURNER;
	}
}

// =======================================================================
// gmBoss1EffAfterburnerUpdateCreate
/*!
  アフターバーナーエフェクト生成処理更新
  
  @param body_work	[io]	本体ワーク
  
  @note
  フラグ監視して、必要ならばエフェクトの生成を行います。
  毎フレーム呼び出してください。
 */
// =======================================================================
void gmBoss1EffAfterburnerUpdateCreate(GMS_BOSS1_BODY_WORK *body_work)
{
	if (body_work->flag & GMD_BOSS1_BODY_FLAG_SIGNAL_B2F_ABURNER) {
		body_work->flag	&= ~GMD_BOSS1_BODY_FLAG_SIGNAL_B2F_ABURNER;
		body_work->flag	|= GMD_BOSS1_BODY_FLAG_ABURNER_ACTIVE;
		gmBoss1EffAfterburnerInit(body_work);
	}
}

// =======================================================================
// gmBoss1EffAfterburnerInit
/*!
  アフターバーナーエフェクト初期化
  
  @param body_work	[io]	本体ワーク
  
  @note
  アフターバーナーエフェクトを生成します。
  直接呼ばないでください。
 */
// =======================================================================
void gmBoss1EffAfterburnerInit(GMS_BOSS1_BODY_WORK *body_work)
{
	OBS_OBJECT_WORK	*parent_obj	= GMM_BS_OBJ(body_work);
	GMS_EFFECT_3DES_WORK	*efct_work;
	efct_work	= GmEfctBossCmnEsCreate(parent_obj, GME_EFCT_BOSS_CMN_IDX_JET_B);
	
	// 表示オフセット調整
	GmEffect3DESAddDispOffset(efct_work, 0, 0, GMD_BOSS1_EFF_ABURNER_DISP_OFST_Z);
	
	GMM_BS_OBJ(efct_work)->ppFunc	= gmBoss1EffAfterburnerProcMain;
}

// =======================================================================
// gmBoss1EffAfterburnerProcMain
/*!
  アフターバーナーエフェクト メイン更新処理関数
  
  @param obj_work	[io]	オブジェクトワーク
  
  @note
  アフターバーナー有効フラグがオフになったら自身の消去処理を行います。
 */
// =======================================================================
void gmBoss1EffAfterburnerProcMain(OBS_OBJECT_WORK *obj_work)
{
	GMS_BOSS1_BODY_WORK	*parent_body	= (GMS_BOSS1_BODY_WORK*)obj_work->parent_obj;
	
	MTM_ASSERT(parent_body->snm_work.reg_node_max);
	
	if (obj_work->disp_flag & OBD_DISP_END) {
		obj_work->flag	|= OBD_OBJECT_TASKCLEAR;
	}
	
	// アフターバーナー有効フラグが消えたらkill
	if (!(parent_body->flag & GMD_BOSS1_BODY_FLAG_ABURNER_ACTIVE)) {
		ObjDrawKillAction3DES(obj_work);
	}
	
	// 本体SNMマトリクスでくっつける
	GmBsCmnUpdateObject3DESStuckWithNode(obj_work,
									 &parent_body->snm_work,
									 parent_body->body_snm_reg_id,
									 TRUE);
}

// =======================================================================
// gmBoss1EffABSmokeInit
/*!
  アフターバーナー煙エフェクト 初期化
  
  @param body_work	[io]	本体ワーク
 */
// =======================================================================
void gmBoss1EffABSmokeInit(GMS_BOSS1_BODY_WORK *body_work)
{
	OBS_OBJECT_WORK	*parent_obj	= GMM_BS_OBJ(body_work);
	GMS_EFFECT_3DES_WORK	*efct_work;
	efct_work	= GmEfctBossCmnEsCreate(parent_obj, GME_EFCT_BOSS_CMN_IDX_JET_B_SMORK);
	
	// 表示オフセット調整
	GmEffect3DESAddDispOffset(efct_work, 0, 0, GMD_BOSS1_EFF_ABSMOKE_DISP_OFST_Z);
	
	GMM_BS_OBJ(efct_work)->ppFunc	= gmBoss1EffABSmokeProcMain;
}

// =======================================================================
// gmBoss1EffABSmokeProcMain
/*!
  アフターバーナー煙エフェクト メイン更新処理関数
  
  @param obj_work	[io]	オブジェクトワーク
 */
// =======================================================================
void gmBoss1EffABSmokeProcMain(OBS_OBJECT_WORK *obj_work)
{
	GMS_BOSS1_BODY_WORK	*parent_body	= (GMS_BOSS1_BODY_WORK*)obj_work->parent_obj;
	
	MTM_ASSERT(parent_body->snm_work.reg_node_max);
	
	// SNMから取得したやつを設定（アフターバーナーと同じ座標）
	GmBsCmnUpdateObject3DESStuckWithNode(obj_work,
									 &parent_body->snm_work,
									 parent_body->body_snm_reg_id,
									 TRUE);
}

// =======================================================================
// gmBoss1EffBodySmokeInit
/*!
  本体煙エフェクト初期化
  
  @param body_work	[io]	本体ワーク
 */
// =======================================================================
void gmBoss1EffBodySmokeInit(GMS_BOSS1_BODY_WORK *body_work)
{
	OBS_OBJECT_WORK	*parent_obj	= GMM_BS_OBJ(body_work);
	GMS_EFFECT_3DES_WORK	*efct_work;
	efct_work	= GmEfctBossCmnEsCreate(parent_obj, GME_EFCT_BOSS_CMN_IDX_BOSS_SMORK);
	
	// 表示オフセット調整
	GmEffect3DESAddDispOffset(efct_work, 0, 0, GMD_BOSS1_EFF_BODYSMOKE_DISP_OFST_Z);
	
	GMM_BS_OBJ(efct_work)->ppFunc	= gmBoss1EffBodySmokeProcMain;
}

// =======================================================================
// gmBoss1EffBodySmokeProcMain
/*!
  本体煙エフェクト メイン更新処理関数
  
  @param obj_work	[io]	オブジェクトワーク
 */
// =======================================================================
void gmBoss1EffBodySmokeProcMain(OBS_OBJECT_WORK *obj_work)
{
	GMS_BOSS1_BODY_WORK	*parent_body	= (GMS_BOSS1_BODY_WORK*)obj_work->parent_obj;
	
	MTM_ASSERT(parent_body->snm_work.reg_node_max);
	
	// SNMから取得したやつを設定（アフターバーナーと同じ座標）
	GmBsCmnUpdateObject3DESStuckWithNode(obj_work,
									 &parent_body->snm_work,
									 parent_body->body_snm_reg_id,
									 TRUE);
}

// =======================================================================
// gmBoss1EffBodySmallSmokeInit
/*!
  本体小煙エフェクト初期化
  
  @param body_work	[io]	本体ワーク
 */
// =======================================================================
void gmBoss1EffBodySmallSmokeInit(GMS_BOSS1_BODY_WORK *body_work)
{
	OBS_OBJECT_WORK	*parent_obj	= GMM_BS_OBJ(body_work);
	
	for (Sint32 i = 0; i < GMD_BOSS1_EFF_SMALL_SMOKE_NUM; ++i) {
		GMS_EFFECT_3DES_WORK	*efct_work;
		efct_work	= GmEfctBossCmnEsCreate(parent_obj, GME_EFCT_BOSS_CMN_IDX_BOSS_SMOKE02);
		
		// 表示オフセット設定
		GmEffect3DESSetDispOffset(efct_work,
								  gm_boss1_eff_small_smoke_disp_ofst_tbl[i][MTD_X],
								  gm_boss1_eff_small_smoke_disp_ofst_tbl[i][MTD_Y],
								  gm_boss1_eff_small_smoke_disp_ofst_tbl[i][MTD_Z]);
		
		GMM_BS_OBJ(efct_work)->ppFunc	= gmBoss1EffBodySmallSmokeProcMain;
	}
}

// =======================================================================
// gmBoss1EffBodySmallSmokeProcMain
/*!
  本体小煙エフェクト メイン更新処理関数
  
  @param obj_work	[io]	オブジェクトワーク
 */
// =======================================================================
void gmBoss1EffBodySmallSmokeProcMain(OBS_OBJECT_WORK *obj_work)
{
	GMS_BOSS1_BODY_WORK	*parent_body	= (GMS_BOSS1_BODY_WORK*)obj_work->parent_obj;
	
	MTM_ASSERT(parent_body->snm_work.reg_node_max);
	obj_work->flag	&= ~OBD_OBJECT_PARENT_FIX;
	// SNMから取得したやつを設定
	GmBsCmnUpdateObject3DESStuckWithNode(obj_work,
										 &parent_body->snm_work,
										 parent_body->body_snm_reg_id,
										 TRUE);
}

// =======================================================================
// gmBoss1EffBodyDebrisInit
/*!
  本体破片エフェクト初期化
  
  @param body_work	[io]	本体ワーク
 */
// =======================================================================
void gmBoss1EffBodyDebrisInit(GMS_BOSS1_BODY_WORK *body_work)
{
	OBS_OBJECT_WORK	*obj_work;
	
	obj_work	= (OBS_OBJECT_WORK*)GmEfctBossCmnEsCreate(GMM_BS_OBJ(body_work),
														  GME_EFCT_BOSS_CMN_IDX_BOSS_PARTS);
	
	// マップ座標でボスの後方側にオフセットして表示
	// （厳密でなくてもよいのでノード追随は使用しない）
	obj_work->parent_ofst.x	= GMD_BOSS1_EFF_DEBRIS_PARENT_OFST_X;
}

// =======================================================================
// gmBoss1EffSweatInit
/*!
  汗エフェクト 初期化
  
  @param egg_work	[io]	エッグマンワーク
 */
// =======================================================================
void gmBoss1EffSweatInit(GMS_BOSS1_EGG_WORK *egg_work)
{
	GMS_EFFECT_3DES_WORK	*efct_work;
	efct_work	= GmEfctCmnEsCreate(GMM_BS_OBJ(egg_work), GME_EFCT_CMN_IDX_SWEAT);
	
	// 位置調整
	GmEffect3DESAddDispOffset(efct_work, 0, GMD_BOSS1_EFF_SWEAT_DISP_OFST_Y, 0);
	
	GMM_BS_OBJ(efct_work)->ppFunc	= gmBoss1EffSweatProcMain;

	egg_work->flag	|= GMD_BOSS1_EGG_FLAG_SWEAT_ACTIVE;
}

// =======================================================================
// gmBoss1EffSweatProcMain
/*!
  汗エフェクト メイン更新処理関数
 */
// =======================================================================
void gmBoss1EffSweatProcMain(OBS_OBJECT_WORK *obj_work)
{
	GMS_BOSS1_EGG_WORK	*parent_egg	= (GMS_BOSS1_EGG_WORK*)obj_work->parent_obj;
	
	MTM_ASSERT(parent_egg);
	
	if (!(parent_egg->flag & GMD_BOSS1_EGG_FLAG_SWEAT_ACTIVE)) {
		ObjDrawKillAction3DES(obj_work);
	}
	
	if (obj_work->disp_flag & OBD_DISP_END) {
		obj_work->flag	|= OBD_OBJECT_TASKCLEAR;
	}
}

// ############################################################################
// ボス１ 画面フラッシュ
// ############################################################################
// =======================================================================
// gmBoss1InitFlashScreen
/*!
  撃破時画面白フラッシュ処理
  
  @note
  非表示エフェクトオブジェクトを生成しています。
 */
// =======================================================================
void gmBoss1InitFlashScreen(void)
{
	GMS_BOSS1_FLASH_SCREEN_WORK	*flash_scr;
	OBS_OBJECT_WORK	*obj_work;
	
	obj_work	= GMM_EFFECT_CREATE_WORK(sizeof(GMS_BOSS1_FLASH_SCREEN_WORK),
										 NULL,
										 0,
										 "boss1_flash_scr");
	
	flash_scr	= (GMS_BOSS1_FLASH_SCREEN_WORK*)obj_work;
	
	// 表示はしない
	obj_work->disp_flag	|= (OBD_DISP_NODISP | OBD_DISP_NOUPDATE);
	
	// クリッピングしない
	obj_work->flag	|= OBD_OBJECT_NOCLIP;
	
	// 画面フラッシュ初期化
	GmBsCmnInitFlashScreen(&flash_scr->flash_work, 
						   GMD_BOSS1_FLASH_SCREEN_FADEOUT_TIME,
						   GMD_BOSS1_FLASH_SCREEN_DURATION_TIME,
						   GMD_BOSS1_FLASH_SCREEN_FADEIN_TIME);
	
	// メイン処理設定
	obj_work->ppFunc	= gmBoss1FlashScreenMain;
}

// ============================================================================
// 補助関数
// ============================================================================
// ============================================================================
// 処理関数
// ============================================================================
// ============================================================================
// 制御関数
// ============================================================================
// ============================================================================
// シーケンス
// ============================================================================
// =======================================================================
// gmBoss1FlashScreenMain
/*!
  撃破時画面白フラッシュ処理 処理関数
  
  @param obj_work	[io]	オブジェクトワーク
 */
// =======================================================================
void gmBoss1FlashScreenMain(OBS_OBJECT_WORK *obj_work)
{
	GMS_BOSS1_FLASH_SCREEN_WORK	*flash_scr	= (GMS_BOSS1_FLASH_SCREEN_WORK*)obj_work;
	
	if (GmBsCmnUpdateFlashScreen(&flash_scr->flash_work)) {
		GmBsCmnClearFlashScreen(&flash_scr->flash_work);
		
		obj_work->flag	|= OBD_OBJECT_TASKCLEAR;
	}
}



// =======================================================================
// test_func
/*!
  関数機能
 
  @param param0 [in] 入力引数0説明
  @param param1 [out] 出力ポインタ引数1説明
  @param param2 [io] 入出力ポインタ引数2説明
 
  @return 返値説明
 
  @note
  補足説明
 */
// =======================================================================
