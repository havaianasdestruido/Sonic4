// ==========================================================================
/*!
  @file gmObjDef.h
  @brief ゲーム中 オブジェクト設定

  @author Ishizaki
				Copyright(c) 2009 Dimps

  $Id: gmObjDef.h 2 2011-04-11 05:21:26Z thamada $
  $Date:: 2011-04-11 14:21:26 +0900#$
 */
// ==========================================================================
/*
 * Memo
 *
 */

#ifndef GM_OBJ_DEF_H_
#define GM_OBJ_DEF_H_


//----- Include Files -------------------------------------------------------

#if	defined(__cplusplus)
extern "C" {
#endif

//----- Definitions ---------------------------------------------------------
// ==========================================================================
// オブジェクト設定
// ==========================================================================
/// オブジェクトIDリスト
typedef enum _gme_objtype {
	GMD_OBJTYPE_PLAYER		= 1,
	GMD_OBJTYPE_ENEMY,
	GMD_OBJTYPE_GIMMICK,
	GMD_OBJTYPE_DECORATION,
	GMD_OBJTYPE_EFFECT,
	GMD_OBJTYPE_COCKPIT,
	GMD_OBJTYPE_FADE,
	GMD_OBJTYPE_MAPFAR,
//	GMD_OBJTYPE_DISP,
//	GMD_OBJTYPE_UNKNOWN,
    
	GMD_OBJTYPE_MAX
} GME_OBJTYPE;

// 標準設定
#define GMD_OBJ_DEF_FALL_SPD			(0x002a0)							//!< オブジェクト標準落下加速度
#define GMD_OBJ_DEF_FALL_SPDMA			(0x0f000)							//!< オブジェクト標準落下最大速度

// 矩形設定
/* 矩形設定 */
// 攻撃フラグ
#define GMD_OBJ_RECT_ATK_FLAG_NORMALATK			(OBD_HIT_NORMAL)	//!< 通常攻撃
#define GMD_OBJ_RECT_ATK_FLAG_BODYATK			(OBD_HIT_BODY)		//!< 体あたり
#define GMD_OBJ_RECT_ATK_FLAG_EFCTATK			(OBD_HIT_USE01)		//!< エフェクト攻撃
#define GMD_OBJ_RECT_ATK_FLAG_EXATK				(OBD_HIT_NORMAL)	//!< 特殊攻撃
#define GMD_OBJ_RECT_ATK_FLAG_EX_SETTING		(OBD_HIT_USE02)		//!< 特殊設定

// 防御フラグ
#define GMD_OBJ_RECT_DEF_FLAG_NORMALATK			(OBD_HIT_NOHIT)						//!< 通常攻撃 防御
#define GMD_OBJ_RECT_DEF_FLAG_WEAK_NORMALATK	(OBD_HIT_NOHIT ^ OBD_HIT_NORMAL)	//!< 通常攻撃のみあたる
#define GMD_OBJ_RECT_DEF_FLAG_WEAK_BODYATK		(OBD_HIT_NOHIT ^ OBD_HIT_BODY)		//!< 体あたりのみあたる
#define GMD_OBJ_RECT_DEF_FLAG_WEAK_EFCTATK		(OBD_HIT_NOHIT ^ OBD_HIT_USE01)		//!< エフェクトのみあたる
//#define GMD_OBJ_RECT_DEF_FLAG_EXATK				(OBD_HIT_NOHIT)						//!< 特殊攻撃 防御
#define GMD_OBJ_RECT_DEF_FLAG_WEAK_EX_SETTING	(OBD_HIT_NOHIT ^ OBD_HIT_USE02)		//!< 特殊設定のみあたる
#define GMD_OBJ_RECT_DEF_FLAG_NOHIT				(OBD_HIT_NOHIT)						//!< すべてあたらない

#define GMD_OBJ_RECT_DEF_FLAG_EFCTATK			(OBD_HIT_NOHIT ^ (OBD_HIT_USE01))	//!< エフェクト攻撃用防御フラグ
#define GMD_OBJ_RECT_DEF_FLAG_EFCTDEF			(OBD_HIT_NOHIT ^ (OBD_HIT_USE01))	//!< エフェクトくらい用防御フラグ

// 攻撃強度(攻撃強度が相手の防御強度以上だとあたる)
// 標準設定にしておくと、防御フラグがなければあたる
#define GMD_OBJ_RECT_ATK_POWER_DEFAULT			(1)		//!< 攻撃パワー 通常設定
#define GMD_OBJ_RECT_ATK_POWER_INVINCIBLE		(3)		//!< 攻撃パワー 通常無敵
//#define GMD_OBJ_RECT_ATK_POWER_FORCE_DEATH		(3)
#define GMD_OBJ_RECT_DEF_POWER_DEFAULT			(0)		//1< 防御パワー 通常設定
#define GMD_OBJ_RECT_DEF_POWER_GUARD			(2)		//1< 防御パワー 通常攻撃を食らわない
#define GMD_OBJ_RECT_DEF_POWER_INVINCIBLE		(3)		//!< 防御パワー 通常無敵

// 矩形グループ
#define GMD_OBJ_RECT_GROUP_PLAYER				(OBD_RECT_GROUP_NO_1)		//!< 矩形グループ プレイヤー
#define GMD_OBJ_RECT_GROUP_ENEMY				(OBD_RECT_GROUP_NO_2)		//!< 矩形グループ エネミー

// ターゲット矩形グループフラグ
#define GMD_OBJ_RECT_TARGET_GROUPFLAG_PLAYER	(OBD_RECT_TARGET_G_FLAG_1)	//!< ターゲット プレイヤー
#define GMD_OBJ_RECT_TARGET_GROUPFLAG_ENEMY		(OBD_RECT_TARGET_G_FLAG_2)	//!< ターゲット エネミー

// 属性フラグ OBS_RECT_WORK:attr_flag

#if 0
// OBS_RECT_WORK:usUserFlag 攻撃対象グループ 矩形所属グループ
#define GMD_OBJ_RECT_GROUP_PLAYER		(OBD_RECT_USE1)						//!< プレイヤー所属矩形グループ
#define GMD_OBJ_RECT_GROUP_A_PLAYER		(OBD_RECT_USE2_A)					//!< プレイヤー攻撃対象矩形グループ
#define GMD_OBJ_RECT_GROUP_ENEMY		(OBD_RECT_USE2)						//!< 敵所属矩形グループ
#define GMD_OBJ_RECT_GROUP_A_ENEMY		(OBD_RECT_USE1_A)					//!< 敵攻撃対象矩形グループ
#define GMD_OBJ_RECT_GROUP_PLAYER_SET	(GMD_OBJ_RECT_GROUP_PLAYER | GMD_OBJ_RECT_GROUP_A_PLAYER)	//!< プレイヤー矩形所属
#define GMD_OBJ_RECT_GROUP_ENEMY_SET	(GMD_OBJ_RECT_GROUP_ENEMY | GMD_OBJ_RECT_GROUP_A_ENEMY)		//!< 敵矩形所属
// OBS_RECT_WORK:usHitFlag 攻撃タイプ
#define GMD_OBJ_RECT_HIT_FLAG_NORMAL		(OBD_HIT_NORMAL)		//!< 攻撃 通常攻撃
#define GMD_OBJ_RECT_HIT_FLAG_DEATH			(OBD_HIT_USE01)			//!< 攻撃 即死攻撃
#define GMD_OBJ_RECT_HIT_FLAG_NEEDLE		(OBD_HIT_USE02)			//!< 攻撃 針攻撃
#define GMD_OBJ_RECT_HIT_FLAG_BODY			(OBD_HIT_BODY)			//!< 攻撃 体あたり
#define GMD_OBJ_RECT_HIT_FLAG_GMK			(OBD_HIT_USE03)			//!< 攻撃 ギミック演出用
// OBS_RECT_WORK:usDefFlag 防御タイプ
#define GMD_OBJ_RECT_DEF_FLAG_NORMAL		(OBD_HIT_NORMAL)		//!< 防御 通常攻撃
#define GMD_OBJ_RECT_DEF_FLAG_BODY			(OBD_HIT_BODY)			//!< 防御 体あたり
#define GMD_OBJ_RECT_DEF_FLAG_GMK			(OBD_HIT_USE03)			//!< 防御 ギミック演出用
//#define GMD_OBJ_RECT_DEF_FLAG_DEFENSLESS	(0)						//!< 防御 防御能力なし
#define GMD_OBJ_RECT_DEF_FLAG_INVINCIBLE	(0xFFFF)				//!< 防御 完全防御
// OBS_RECT_WORK:sHitPower 攻撃値
#define GMD_OBJ_RECT_HIT_POWER_DEFAULT		(OBD_HIT_HIT_DEFAULT)	//!< 攻撃 通常攻撃値
#define GMD_OBJ_RECT_HIT_POWER_POW1			(OBD_HIT_HIT_DEFAULT+2)	//!< 攻撃 攻撃値強化1
#define GMD_OBJ_RECT_HIT_POWER_NITRO		(GMD_OBJ_RECT_HIT_POWER_POW1)//!< 攻撃 ニトロ攻撃値
#define GMD_OBJ_RECT_HIT_POWER_ULTIMATE		(254)					//!< 攻撃 絶対攻撃値(無敵の相手を除く)
// OBS_RECT_WORK:sDefPower 防御値
#define GMD_OBJ_RECT_DEF_POWER_DEFAULT		(OBD_HIT_DEF_DEFAULT)	//!< 防御 通常防御値
#define GMD_OBJ_RECT_DEF_POWER_POW1			(OBD_HIT_DEF_DEFAULT+2)	//!< 防御 防御値強化1
#define GMD_OBJ_RECT_DEF_POWER_INVINCIBLE	(255)					//!< 防御 無敵設定値
#endif

// =====================================================================
// Z位置指定
// =====================================================================
#define GMD_OBJ_DEFAULT_POS_Z_C				(0)											//!< 中央
#define GMD_OBJ_DEFAULT_POS_Z_C_FRONT		(GMD_OBJ_DEFAULT_POS_Z_C + 32*FX32_ONE)		//!< 中央 前
#define GMD_OBJ_DEFAULT_POS_Z_C_BACK		(GMD_OBJ_DEFAULT_POS_Z_C - 32*FX32_ONE)		//!< 中央 後ろ

#define GMD_OBJ_DEFAULT_POS_Z_A				(128*FX32_ONE)								//!< A面
#define GMD_OBJ_DEFAULT_POS_Z_A_FRONT		(GMD_OBJ_DEFAULT_POS_Z_A + 32*FX32_ONE)		//!< A面
#define GMD_OBJ_DEFAULT_POS_Z_A_BACK		(GMD_OBJ_DEFAULT_POS_Z_A - 32*FX32_ONE)		//!< A面

#define GMD_OBJ_DEFAULT_POS_Z_B				(-128*FX32_ONE)								//!< B面
#define GMD_OBJ_DEFAULT_POS_Z_B_FRONT		(GMD_OBJ_DEFAULT_POS_Z_B + 32*FX32_ONE)		//!< B面
#define GMD_OBJ_DEFAULT_POS_Z_B_BACK		(GMD_OBJ_DEFAULT_POS_Z_B - 32*FX32_ONE)		//!< B面

#define GMD_OBJ_DEFAULT_POS_Z_N				(256*FX32_ONE)					//!< 超近景
#define GMD_OBJ_DEFAULT_POS_Z_N_FRONT		(GMD_OBJ_DEFAULT_POS_Z_N + 32*FX32_ONE)		//!< 超近景
#define GMD_OBJ_DEFAULT_POS_Z_N_BACK		(GMD_OBJ_DEFAULT_POS_Z_N - 32*FX32_ONE)		//!< 超近景

#define GMD_OBJ_DEFAULT_POS_Z_M				(-256*FX32_ONE)					//!< 中景
#define GMD_OBJ_DEFAULT_POS_Z_M_FRONT		(GMD_OBJ_DEFAULT_POS_Z_M + 32*FX32_ONE)		//!< 中景
#define GMD_OBJ_DEFAULT_POS_Z_M_BACK		(GMD_OBJ_DEFAULT_POS_Z_M - 32*FX32_ONE)		//!< 中景

#define GMD_OBJ_DEFAULT_POS_Z_M1			(-384*FX32_ONE)					//!< 中景 スクロールタイプ1
#define GMD_OBJ_DEFAULT_POS_Z_M1_FRONT		(GMD_OBJ_DEFAULT_POS_Z_M1 + 32*FX32_ONE)		//!< 中景
#define GMD_OBJ_DEFAULT_POS_Z_M1_BACK		(GMD_OBJ_DEFAULT_POS_Z_M1 - 32*FX32_ONE)		//!< 中景

#define GMD_OBJ_DEFAULT_POS_Z_M2			(-512*FX32_ONE)					//!< 中景 スクロールタイプ2
#define GMD_OBJ_DEFAULT_POS_Z_M2_FRONT		(GMD_OBJ_DEFAULT_POS_Z_M2 + 32*FX32_ONE)		//!< 中景
#define GMD_OBJ_DEFAULT_POS_Z_M2_BACK		(GMD_OBJ_DEFAULT_POS_Z_M2 - 32*FX32_ONE)		//!< 中景

#define GMD_OBJ_DEFAULT_POS_Z_M3			(-640*FX32_ONE)					//!< 中景 スクロールタイプ3
#define GMD_OBJ_DEFAULT_POS_Z_M3_FRONT		(GMD_OBJ_DEFAULT_POS_Z_M3 + 32*FX32_ONE)		//!< 中景
#define GMD_OBJ_DEFAULT_POS_Z_M3_BACK		(GMD_OBJ_DEFAULT_POS_Z_M3 - 32*FX32_ONE)		//!< 中景

#define GMD_OBJ_GIMMICK_POS_Z				(GMD_OBJ_DEFAULT_POS_Z_C)		//!< 通常のギミック位置
#define GMD_OBJ_GIMMICK_POS_Z_FRONT			(GMD_OBJ_DEFAULT_POS_Z_C_FRONT)	//!< プレイヤーより前のギミック
#define GMD_OBJ_GIMMICK_POS_Z_BACK			(GMD_OBJ_DEFAULT_POS_Z_C_BACK)	//!< プレイヤーより後ろのギミック

#define GMD_OBJ_ENEMY_POS_Z					(GMD_OBJ_DEFAULT_POS_Z_C)		//!< 通常のエネミー位置
#define GMD_OBJ_ENEMY_POS_Z_FRONT			(GMD_OBJ_DEFAULT_POS_Z_C_FRONT)	//!< プレイヤーより前のエネミー
#define GMD_OBJ_ENEMY_POS_Z_BACK			(GMD_OBJ_DEFAULT_POS_Z_C_BACK)	//!< プレイヤーより後ろのエネミー


// =====================================================================
// オブジェクトポーズレベル
// =====================================================================
#define GMD_OBJ_OBJPAUSELEVEL_DEF		(0)			//!< オブジェクトポーズレベルデフォルト


//----- Macros --------------------------------------------------------------

//----- Macros Functions ----------------------------------------------------

//----- External Variables --------------------------------------------------

//----- External Declarations -----------------------------------------------

#if	defined(__cplusplus)
} /* extern "C" */
#endif

#endif // GM_OBJ_DEF_H_

//----- Include Files -------------------------------------------------------
