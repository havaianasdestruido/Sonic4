// ================================================================
/*!
    Copyright(c) 2009 Dimps CORP. All Rights Reserved. 

	@file       gmDecoGlare.h
	@brief      装飾（グレア）　ヘッダ
	@author	    Syuichi Gotou
	@date	    Date: 2009/07/13 
 */
// ================================================================

#ifndef _GM_DECO_GLARE_H
#define _GM_DECO_GLARE_H

//----- Include Files (Pre Definitions) --------------------------------


//----- Definitions ----------------------------------------------------

//! ビューア管理構造体
typedef struct tagGMDECO_GLARE_INTERFACE {
	AMS_AMB_HEADER*		amb_header; //!< AMBヘッダ
	NNS_TEXFILELIST*	tex_buf;    //!< テクスチャファイル先頭
	void*				texlistbuf; //!< テクスチャリストバッファ
	NNS_TEXLIST*		texlist;    //!< テクスチャリスト
	Sint32				texId;      //!< テクスチャＩＤ
	Sint32				regId;      //!< 登録ＩＤ（開放完了チェックに必要）
	Sint32              drawFlag;   //!< 擬似グレア描画していいかどうか？
} GMDECO_GLARE_INTERFACE;

//----- Macros ---------------------------------------------------------


//----- Enum Definitions -----------------------------------------------



//----- Type Definitions -----------------------------------------------



//----- External Definitions -------------------------------------------

//----- External Functions ---------------------------------------------

// ==========================================================================
// GmDecoGlareSetData
/*!
 *	擬似グレア用データ設定
 *
 *	@param	amb_header	[in] グレア画像の入ったデータ（AMB）
 *  @note この時点で既にコンバートアドレスは行われている(GmDecoInitDataが呼ばれている)
 */
// ==========================================================================
void GmDecoGlareSetData( AMS_AMB_HEADER* amb_header );

// ==========================================================================
// gmDecoGlareDraw
/*!
 *	擬似グレア描画
 *
 *	@param	obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
void GmDecoGlareDraw( OBS_OBJECT_WORK* obj_work );

// ==========================================================================
// GmDecoGlareDataRelease
/*!
 *	擬似グレア用データ開放
 *
 *  @note ambはgmDecoDataFlushで開放される
 */
// ==========================================================================
void GmDecoGlareDataRelease(void);

// ==========================================================================
// GmDecoGlareGetGlobal
/*!
 *	擬似グレア管理グローバル参照
 *
 *  @note 軌跡テクスチャなどに流用
 */
// ==========================================================================
GMDECO_GLARE_INTERFACE* GmDecoGlareGetGlobal(void);


#endif	// _GM_DECO_GLARE_H
