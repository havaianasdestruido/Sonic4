// ==========================================================================
/*!
  @file gmEneSting.h
  @brief エネミー スティンガー

  @author Ishizaki
				Copyright(c) 2009 Dimps

  $Id: gmEneSting.h 2 2011-04-11 05:21:26Z thamada $
  $Date:: 2011-04-11 14:21:26 +0900#$
 */
// ==========================================================================
/*
 * Memo
 *
 */

#ifndef GM_ENE_STING_H_
#define GM_ENE_STING_H_


//----- Include Files -------------------------------------------------------

#if	defined(__cplusplus)
extern "C" {
#endif

//----- Definitions ---------------------------------------------------------

//----- Macros --------------------------------------------------------------

//----- Macros Functions ----------------------------------------------------

//----- External Variables --------------------------------------------------

//----- External Declarations -----------------------------------------------
// ==========================================================================
// GmEneStingBuild
/*!
 *	エネミー スティンガー データ構築
 */
// ==========================================================================
extern void GmEneStingBuild(void);

// ==========================================================================
// GmEneStingFlush
/*!
 *	エネミー スティンガー データ片付け
 */
// ==========================================================================
extern void GmEneStingFlush(void);

// ==========================================================================
// GmEneStingInit
/*!
 *	エネミー スティンガー 初期化関数
 *
 *	@param eve_rec	[io] レコードポインタ
 *	@param pos_x	[in] 出現座標
 *	@param pos_y	[in] 
 *	@param type		[in] 処理内容タイプ 通常は0
 *
 *	@note
 *		user_work	: 左移動限界\n
 *		usre_flag	: 右移動限界
 */
// ==========================================================================
extern OBS_OBJECT_WORK* GmEneStingInit(GMS_EVE_RECORD_EVENT *eve_rec ,fx32 pos_x, fx32 pos_y, u8 type);

// ==========================================================================
// 弾
// ==========================================================================
// ==========================================================================
// GmEneStingCreateBullet
/*!
 *	エネミー スティンガー 弾 メレオン共用
 *
 *	@param	parent_obj		[in]	親オブジェクト
 *	@parma	ofst_flash_x	[in]	フラッシュエフェクトオフセットX
 *	@parma	ofst_flash_y	[in]	フラッシュエフェクトオフセットY
 *	@parma	ofst_flash_z	[in]	フラッシュエフェクトオフセットZ
 *	@parma	ofst_bul_x		[in]	弾エフェクトオフセットX
 *	@parma	ofst_bul_y		[in]	弾エフェクトオフセットY
 *	@parma	ofst_bul_z		[in]	弾エフェクトオフセットZ
 *	@parma	spd_x			[in]	弾速度X
 *	@parma	spd_y			[in]	弾速度Y
 *	@parma	dir				[in]	弾角度
 */
// ==========================================================================
extern void GmEneStingCreateBullet(OBS_OBJECT_WORK *parent_obj, fx32 ofst_flash_x, fx32 ofst_flash_y, fx32 ofst_flash_z,
							fx32 ofst_bul_x, fx32 ofst_bul_y, fx32 ofst_bul_z,
							fx32 spd_x, fx32 spd_y, Angle16 dir);

#if	defined(__cplusplus)
} /* extern "C" */
#endif

#endif // GM_ENE_STING_H_

//----- Include Files -------------------------------------------------------
