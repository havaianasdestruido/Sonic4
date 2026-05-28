// ==========================================================================
/*!
  @file gmRing.h
  @brief リング関連

  @author Ishizaki
				Copyright(c) 2009 Dimps

  $Id: gmRing.h 2 2011-04-11 05:21:26Z thamada $
  $Date:: 2011-04-11 14:21:26 +0900#$
 */
// ==========================================================================
/*
 * memo
 *
 *
 */

#ifndef GM_RING_H_
#define GM_RING_H_

//----- Include Files -------------------------------------------------------
#include "objObject.h"
#include "gsMainSys.h"
#include "gmEventMgr.h"
#include "gmPlayer.h"
#include "gmSound.h"

#if	defined(__cplusplus)
extern "C" {
#endif


//----- Macros --------------------------------------------------------------

//----- Macros Functions ----------------------------------------------------

//----- Definitions ---------------------------------------------------------
//#define GMD_RING_DAMAGE_NUM				(64)							//!< ダメージリング最大作成数
#define GMD_RING_DAMAGE_NUM				(32)							//!< ダメージリング最大作成数
#define GMD_RING_MAX_NUM				(GMD_RING_DAMAGE_NUM + 64)		//!< 最大リング管理数

#if _IPHONE
#define GMD_RING_SE_PLAY_MAX_PER_FRAME	(1)					//!< フレームあたりのリング取得SE再生最大回数
#define GMD_RING_SE_PLAY_WAIT_FRAME		(3)					//!< 一度サウンド再生を行った際に次の再生まで待機するフレーム数
#else
#define GMD_RING_SE_PLAY_MAX_PER_FRAME	(2)					//!< フレームあたりのリング取得SE再生最大回数
#endif // _IPHONE

// リングシステムフラグ GMS_RING_SYS_WORK::flag
#define GMD_RING_SYS_FLAG_NODISP		(1<< 0)				//!< リングを非表示にする(処理も行わない)
//#define GMD_RING_SYS_FLAG_DAMAGE_CHK	(1<< 1)				//!< ダメージ復帰チェック中(このフラグが立っている間にダメージリングを取得するとdamage_numがクリアされない)
//#define GMD_RING_SYS_FLAG_MAGNET		(1<< 2)				//!< このフーレム内でマグネットチェックが呼ばれたフラグ
#define GMD_RING_SYS_FLAG_GET_SE_FLIP	(1<< 3)				//!< リング取得SE左右フリップ

#define GMD_RING_SYS_FLAG_DAMAGE_CHK_P1	(0x01000000)		//!< プレイヤー1 ダメージ復帰チェック中(このフラグが立っている間にダメージリングを取得するとdamage_numがクリアされない)
#define GMD_RING_SYS_FLAG_DAMAGE_CHK_P2	(0x02000000)		//!< プレイヤー2 ダメージ復帰チェック中(このフラグが立っている間にダメージリングを取得するとdamage_numがクリアされない)
							// 最大プレイヤー数分(GSD_PLAYER_MAX)確保する事


// リングワークフラグ GMS_RING_WORK::flag
#define GMD_RING_MAGNET_PLAYER			(0x0001)	//!< 磁力で引きよせを行ったプレイヤー判別フラグ
#define GMD_RING_MAGNET_PLAYER_MASK		(0x0001)	//!< 0 : プレイヤー1  1 : プレイヤー2
#define GMD_RING_FLAG_B					(0x0002)	//!< B面
#define GMD_RING_REVERSE				(0x0004)	//!< 上下反転
#define GMD_RING_DAMAGE					(0x0008)	//!< ダメージリング
#define GMD_RING_DAMAGE_PLAYER			(0x0010)	//!< 0 : プレイヤー1  1 : プレイヤー2
#define GMD_RING_DAMAGE_PLAYER_SHIFT	(4)


// 矩形設定
//#define GMD_RING_HIT_OFFSET_Y (-0x0a00 ) // リング中心オフセット
#define GMD_RING_HIT_LEFT				(-9/*-6*/)			//!< リング矩形左
#define GMD_RING_HIT_TOP				(-9/*-16*/)			//!< リング矩形上
#define GMD_RING_HIT_RIGHT				(9)					//!< リング矩形右
#define GMD_RING_HIT_BOTTOM				(9)					//!< リング矩形下
#define GMD_RING_HIT_BACK				(-8)				//!< リング矩形前
#define GMD_RING_HIT_FRONT				(8)					//!< リング矩形後ろ

#define GMD_RING_HIT_HEIGHT				(18)				//!< リング矩形高さ
#define GMD_RING_HIT_WIDTH				(18)				//!< リング矩形幅

#define GMD_RING_SIZE					(g_gm_ring_size)	//!< リングサイズ


#if 0
#define GMD_RING_NOTIMER ( 1 << 5 ) // タイムアウトしない
#define GMD_RING_NOCLIP  ( 1 << 6 ) // クリップアウトしない (多分使わないフラグ)
#define GMD_RING_CLEAR   ( 1 << 7 ) // 消去中
#define GMD_RING_NOHIT   ( 1 << 8 ) // 当たらない
#define GMD_RING_NOFALL  ( 1 << 9 ) // 落下しない
#define GMD_RING_NOMOVE  ( 1 <<10 ) // 移動しない
#define GMD_RING_NOCOL   ( 1 <<11 ) // 地形チェックなし
#define GMD_RING_DAMAGE  ( 1 <<12 ) // ダメージで飛んだリング
#define GMD_RING_MAGNET  ( 1 <<13 ) // 磁力引き寄せられ中
#define GMD_RING_DUCT    ( 1 <<14 ) // ダクト吸い込まれ中
#endif


/// リングオブジェクトワーク
typedef struct tag_GMS_RING_WORK {
	VecFx32	pos;				//!< リング座標
	VecFx32	scale;				//!< リング拡大率
	fx32	spd_x;				//!< リング移動速度 X
	fx32	spd_y;				//!< リング移動速度 Y

	s16		timer;				//!< リング生存タイマー
	u16		flag;				//!< リング状態フラグ

	GMS_EVE_RECORD_RING	*eve_rec;	//!< レコードポインタ

	struct tag_GMS_RING_WORK	*pre_ring;		//!< 前のリングワーク
	struct tag_GMS_RING_WORK	*post_ring;		//!< 後ろのリングワーク

	// リング関連ギミック設定
	OBS_OBJECT_WORK				*duct_obj;			//!< 吸い込んでるダクトオブジェクトワーク

} GMS_RING_WORK;

// リングシステム管理ワーク
typedef struct _GMS_RING_SYS_WORK
{
	u32						flag;										//!< リング管理フラグ

	void(*ring_draw_func)(GMS_RING_WORK*);									//!< リング描画関数
	//void(*twinkle_draw_func)(GMS_RING_WORK*);							//!< 取得時エフェクト描画関数
#if 1
	u16(*rec_func)(OBS_RECT*, OBS_RECT*);								//!< リング当たり判定関数
#else
	u16(*rec_func)(OBS_RECT_WORK*, s32, s32, s16, s16, u16, u16);		//!< リング当たり判定関数
#endif
	void(*col_func)(GMS_RING_WORK*);									//!< リング地形判定チェック関数

	// 表示アクション
	//OBS_ACTION3D_NN_WORK	ring_obj_3d;								//!< リング描画用
#if 1
//	MTS_ACTION_DS			ring_act;									//!< リング描画用
//	MTS_ACTION_DS			twinkle_act;								//!< リング消滅描画用

//	MTS_ACTION3D_SPRITE		ring_act3d;									//!< 3Dリング描画用
//	MTS_ACTION3D_SPRITE		twinkle_act3d;								//!< 3Dリング消滅描画用
#else
	MTS_ACTION				mAct[1];    // アニメーションを順再生用にしたいのでテーブルにしておく OPEで
	MTS_ACTION				mActB[1];   // B面用
	MTS_ACTION3D_SPRITE		mA3dSpr[1]; // 3D時用のアクション

    u32   pClear;  // クリアアニメA VRAMアドレス
    u32   pClearB; // クリアアニメB VRAMアドレス
#endif

	u16						dir;										//!< リング表示回転量

//	OBS_OBJECT_WORK			draw_obj;									//!< 描画用オブジェクト
    
//	u16						ucRingNum;									//!< リング枚数
	u8						damage_num[GSD_MAIN_PLAYER_MAX];			//!< ダメージ散らばり回数
    u8						player_num;									//!< プレイヤー総数
	fx16					ref_spd_base;								//!< リング地面跳ね返り時の速度ベース 0 の時は反映なし

	// リング管理リスト
	GMS_RING_WORK			*ring_list_start;							//!< 通常固定リング リスト先頭
	GMS_RING_WORK			*ring_list_end;								//!< 通常固定リング リスト最後尾
	GMS_RING_WORK			*twinkle_list_start;						//!< 取得エフェクト リスト先頭
	GMS_RING_WORK			*twinkle_list_end;							//!< 取得エフェクト リスト最後尾
	GMS_RING_WORK			*damage_ring_list_start;					//!< ダメージ飛び散りリング リスト先頭
	GMS_RING_WORK			*damage_ring_list_end;						//!< ダメージ飛び散りリング リスト最後尾
	GMS_RING_WORK			*slot_ring_list_start;						//!< スロットギフトリング リスト先頭
	GMS_RING_WORK			*slot_ring_list_end;						//!< スロットギフトリング リスト最後尾
//	GMS_RING_WORK			*magnet_ring_list_start;					//!< 磁力吸い寄せリング リスト先頭
//	GMS_RING_WORK			*magnet_ring_list_end;						//!< 磁力吸い寄せリング リスト最後尾

	s32						ring_list_cnt;								//!< 使用バッファカウンタ
	GMS_RING_WORK			*ring_list[GMD_RING_MAX_NUM];				//!< リングリスト
	GMS_RING_WORK			ring_list_buf[GMD_RING_MAX_NUM];			//!< リングリストバッファ


	// スロットギフトリング設定
	s32						wait_slot_ring_num;		//!< 生成待ちスロットリング数
	u16						slot_ring_create_dir;	//!< スロットリング生成角度
	OBS_OBJECT_WORK			*slot_target_obj;		//!< スロットリングターゲットオブジェクト
	s32						slot_ring_timer;		//!< 生成間隔タイマー

	// 手動描画用ワーク
	u16						draw_ring_count;		//!< リング総数
	u16						draw_ring_uv_frame;		//!< リングUV座標管理フレーム
	VecFx32					draw_ring_pos[GMD_RING_MAX_NUM];	//!< リング座標情報

    // 移動するリング用
//GMS_RING_WORK 			tRing[ GMD_RING_MOVE_NUM ];

	// SE
	GSS_SND_SE_HANDLE		*h_snd_ring[2];

	s32						ring_se_cnt;		//!< リング取得SE再生数カウンタ(フレームあたり)
												// カウンタのクリアはMain処理の最後に行う
												// (リング処理より早い処理がSEコールを行う事がある為)
#if _IPHONE
	u32						color;				//!< 色設定
	s32						se_wait;			//!< SE待機フレーム数
#endif // _IPHONE
} GMS_RING_SYS_WORK;

#if 0
// リング消去アクションワーク
typedef struct _GMS_RING_CLEAR_WORK
{
    s32 lPosX; // 1:23:8 リング座標
    s32 lPosY;
    
    // 表示アクション
	MTS_ACTION      mAct;  // アニメーションを順再生してみる用にテーブル
	MTS_ACTION      mActB; // B面用
    
} GMS_RING_CLEAR_WORK;

// 3Dリング消去アクションワーク
typedef struct _GMS_RING_CLEAR_3D_WORK
{
    // 表示アクション
    MTS_ACTION3D_SPRITE     mA3dSpr;
} GMS_RING_CLEAR_3D_WORK;
#endif

//----- External Variables --------------------------------------------------
extern s16 g_gm_ring_size;	//!< リングサイズ

//----- External Declarations -----------------------------------------------
// ==========================================================================
// データ構築
// ==========================================================================
// ==========================================================================
// GmRingBuild
/*!
 *	リングデータ構築
 */
// ==========================================================================
extern void GmRingBuild(void);

// ==========================================================================
// GmRingBuildCheck
/*!
 *	リングデータ構築 終了チェック
 *
 *	@reutrn	TRUE : 終了
 */
// ==========================================================================
extern BOOL GmRingBuildCheck(void);

// ==========================================================================
// GmRingFlush
/*!
 *	リングデータフラッシュ
 *
 *	@reutrn	リングシステムワークアドレス
 */
// ==========================================================================
extern void GmRingFlush(void);

// ==========================================================================
// GmRingFlushCheck
/*!
 *	リングデータフラッシュ終了チェック
 *
 *	@reutrn	TRUE : 終了
 */
// ==========================================================================
extern BOOL GmRingFlushCheck(void);

// ==========================================================================
// 初期化
// ==========================================================================
// ==========================================================================
// GmRingInit
/*!
 *	リング管理初期化関数
 */
// ==========================================================================
extern void GmRingInit(void);

// ==========================================================================
// 終了処理
// ==========================================================================
// ==========================================================================
// GmRingExit
/*!
 *	リング終了処理
 */
// ==========================================================================
extern void GmRingExit(void);

// ==========================================================================
// リング作成
// ==========================================================================
// ==========================================================================
// GmRingCreateDamageRing
/*!
 *	ダメージリング作成
 *
 *	@param	pos_x	[in] 発生座標
 *	@param	pos_y	[in]
 *	@param	pos_z	[in]
 *	@param	spd_x	[in] 初速
 *	@param	spd_y	[in]
 *	@param	flag	[in] フラグ
 *
 *	@return	GMS_RING_WORK アドレス
 */
// ==========================================================================
extern GMS_RING_WORK* GmRingCreateDamageRing(fx32 pos_x, fx32 pos_y, fx32 pos_z, fx32 spd_x, fx32 spd_y, u16 flag);

// ==========================================================================
// GmRingCreate
/*!
 *	通常リング作成
 *
 *	@param	eve_rec	[in]	レコードポインタ
 *	@param	pos_x	[in]	発生座標
 *	@param	pos_y	[in]
 *	@param	pos_z	[in]
 *
 *	@return	GMS_RING_WORK アドレス
 */
// ==========================================================================
extern GMS_RING_WORK* GmRingCreate(GMS_EVE_RECORD_RING* eve_rec, fx32 pos_x, fx32 pos_y, fx32 pos_z);

// ==========================================================================
// GmRingCreateSlotRing
/*!
 *	スロットリング作成
 *
 *	@param	target_obj	[in] ターゲットオブジェクト
 *	@param	dist		[in] 距離
 *	@param	dir			[in] 角度
 *
 *	@return	GMS_RING_WORK アドレス
 */
// ==========================================================================
extern GMS_RING_WORK* GmRingCreateSlotRing(OBS_OBJECT_WORK *target_obj, fx32 dist, u16 dir);

// ==========================================================================
// GmRingCheckRestSlotRing
/*!
 *	スロットリングが残っているかチェック
 *
 *	@return	TRUE : 未生成スロットリング, もしくは未取得スロットリングあり
 */
// ==========================================================================
extern BOOL GmRingCheckRestSlotRing(void);

// ==========================================================================
// GmEveRingInit
/*!
 *	リングイベント生成初期化関数
 *
 *	@param	eve_rec	[inout]	レコードポインタ
 *	@param	pos_x	[in]	出現座標
 *	@param	pos_y	[in]
 *	@param	type	[in]	処理内容タイプ 通常は0
 *
 *	@note
 *		デバック配置時のコール用
 */
// ==========================================================================
extern OBS_OBJECT_WORK* GmEveRingInit(GMS_EVE_RECORD_EVENT *eve_rec , fx32 pos_x,  fx32 pos_y, u8 type);

// ==========================================================================
// GmRingDamageSetNum
/*!
 *	ダメージ時の飛び散りリング作成 数指定有り
 *
 *	@param	ply_work	[in]	プレイヤーオブジェクト
 *	@param	ring_num	[in]	飛び散らせるリング数
 *
 *	@note
 *		所持リング数以上の値を設定された場合は所持リングすべてを放出
 */
// ==========================================================================
extern void GmRingDamageSetNum(GMS_PLAYER_WORK *ply_work, s16 ring_num);

// ==========================================================================
// GmRingDamageSet
/*!
 *	ダメージ時の飛び散りリング作成 全部放出
 *
 *	@param	ply_obj	[in]	プレイヤーオブジェクト
 */
// ==========================================================================
inline void GmRingDamageSet(GMS_PLAYER_WORK *ply_obj)
{
	GmRingDamageSetNum(ply_obj, ply_obj->ring_num);
}

// ==========================================================================
// GmRingSlotSetNum
/*!
 *	スロット用リング作成 数指定有り
 *
 *	@param	ply_work	[in]	プレイヤーオブジェクト
 *	@param	ring_num	[in]	取得するリング数
 *
 *	@note
 *		所持リング数以上の値を設定された場合は所持リングすべてを放出
 */
// ==========================================================================
extern void GmRingSlotSetNum(GMS_PLAYER_WORK *ply_work, s32 ring_num);

// ==========================================================================
// SE
// ==========================================================================
// ==========================================================================
// GmRingGetSE
/*!
 *	リング取得SE再生
 */
// ==========================================================================
extern void GmRingGetSE(void);

// ==========================================================================
// リングシステム関連
// ==========================================================================
// ==========================================================================
// GmRingGetWork
/*!
 *	リングシステムのワークを取得する
 *
 *	@return   ワーク
 */
// ==========================================================================
extern GMS_RING_SYS_WORK* GmRingGetWork( void );

// ==========================================================================
// リングシステムフラグ関連
// ==========================================================================
// ==========================================================================
// GmRingGetFlag
/*!
 *	リングシステムのフラグを取得する
 *
 *	@return   フラグ
 */
// ==========================================================================
extern u32 GmRingGetFlag( void );

// ==========================================================================
// GmRingSetFlag
/*!
 *	リングシステムのフラグを設定する
 *
 *	@param	flag	[in]	フラグ
 */
// ==========================================================================
extern void GmRingSetFlag(u32 flag);


// ==========================================================================
// リングシステム設定関連
// ==========================================================================
// ==========================================================================
// GmRingSetScale
/*!
 *	リングの基本スケールを設定する
 *
 *	@param	scale	[in]	スケール
 *
 *	@note
 *		通常スプライト時は FX32_ONE*2 まで
 */
// ==========================================================================
extern void GmRingSetScale(fx32 scale);

// ==========================================================================
// GmRingGetScale
/*!
 *	リングの基本スケールを取得する
 *
 *	@return	スケール
 */
// ==========================================================================
extern fx32 GmRingGetScale(void);

// ==========================================================================
// 特殊処理チェック
// ==========================================================================
// ==========================================================================
// GmRingMagnetCheck
/*!
 *	リング磁力判定、移動値設定処理
 *
 *	@param	ply_obj	[in]	プレイヤーワークポインタ
 */
// ==========================================================================
extern void GmRingMagnetCheck(GMS_PLAYER_WORK *ply_obj);

// ==========================================================================
// GmRingDuctCheck
/*!
 *	リングダクト判定、移動値設定処理
 *
 *	@param	obj_work	[in]	オブジェクトワークポインタ
 *
 *	@return	0 : 吸い込み無し	1 : 吸い込み有り
 */
// ==========================================================================
extern u16 GmRingDuctCheck(OBS_OBJECT_WORK* obj_work);


#if	defined(__cplusplus)
} /* extern "C" */
#endif

#endif // GM_RING_
