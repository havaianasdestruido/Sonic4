// ================================================================
/*!
    Copyright(c) 2009 Dimps CORP. All Rights Reserved. 

	@file       gmDecoGlare.cpp
	@brief      装飾（擬似グレア処理）
	@author	    Syuichi Gotou
	@note       ツールで配置した場所にテクスチャを貼ります
	@date	    Date: 2009/07/13
 */
// ================================================================

/*--- Include Files (Pre Definitions) ---------------------------------------*/
#include "pch.h"

#include "objObject.h"
#include "gmDecoGlare.h"
#include "gmEventTbl.h"
#include "gmDeco.h"

/*--- Definitions -----------------------------------------------------------*/

#define GMD_DECOGLARE_OFSEETID		(GMD_DECORATE_ID_GLARE_ORANGE)

//! グレアパラメータ
typedef struct _GMS_DECOGLARE_PARAM
{
	NNS_RGBA8888	color;	// 色
	float			size;	// 辺のサイズ（縦横比は1:1）
	float			sort_z;	// ソート用Ｚ値
	Sint32          ablend; // αブレンドタイプ
} GMS_DECOGLARE_PARAM;


//! グレアタイプ
// ここに追加すると同時に g_gm_decorate_tbl や tag_GME_DECORATE_ID
// gmDeco.cpp への各種追加を行うこと
typedef enum _GME_DECOGLARE_TYPE
{
	GME_DECOGLARE_ORANGE = 0, // 橙色
	GME_DECOGLARE_RED,        // 赤
	GME_DECOGLARE_GREEN,	  // 緑
	GME_DECOGLARE_SALMON,	  // サーモン
	GME_DECOGLARE_MAX,
} GME_DECOGLARE_TYPE;

//! グレアαブレンドタイプ
typedef enum _GME_DECOGLARE_ABLENDTYPE
{
	GME_DECOGLARE_ABLEND_NORMAL = 0, // 乗算
	GME_DECOGLARE_ABLEND_ADD,        // 加算
	GME_DECOGLARE_ABLEND_MAX,
} GME_DECOGLARE_ABLENDTYPE;


//! グレア種類別配列定義
GMS_DECOGLARE_PARAM _gm_decoGlare_param[] = {
//    color(RGBA)  size   z_sort 加算or乗算
	{ 0xFFF0B9FF, 40.0f,  1.0f,  GME_DECOGLARE_ABLEND_ADD },	// 橙色(255, 240, 185)
	{ 0xFF353FFF, 40.0f,  1.0f,  GME_DECOGLARE_ABLEND_ADD },	// 赤(255, 53, 63)
	{ 0xACEE54FF, 40.0f,  1.0f,  GME_DECOGLARE_ABLEND_ADD },	// 緑(172, 238, 84)
	{ 0xFF976CFF, 40.0f,  1.0f,  GME_DECOGLARE_ABLEND_ADD },	// ピンク(255, 151, 108)
};

/*--- Macros ----------------------------------------------------------------*/

/*--- Include Files (Post Definitions) --------------------------------------*/


/*--- Local Declarations ----------------------------------------------------*/


/*--- External Variables ----------------------------------------------------*/

/*--- Global Variables ------------------------------------------------------*/


/*--- Local Variables -------------------------------------------------------*/

static GMDECO_GLARE_INTERFACE  gmDeco_Glare_global;
GMDECO_GLARE_INTERFACE* pIF = &gmDeco_Glare_global;

/*--- Inline Functions ------------------------------------------------------*/
//## Inline Functions



/*--- Global Functions ------------------------------------------------------*/
//## Global Functions

// ==========================================================================
// GmDecoGlareSetData
/*!
 *	擬似グレア用データ設定
 *
 *	@param	amb_header	[in] グレア画像の入ったデータ（AMB）
 *  @note この時点で既にコンバートアドレスは行われている(GmDecoInitDataが呼ばれている)
 */
// ==========================================================================
void GmDecoGlareSetData( AMS_AMB_HEADER* amb_header )
{
	memset(pIF, 0, sizeof(GMDECO_GLARE_INTERFACE));
	pIF->amb_header = (AMS_AMB_HEADER*)amBindGet( amb_header, GMD_DECO_DATA_INDEX_AMB_TEX );

	// 先頭にtxbがあること前提
	char* buf = (char*)amBindGet(pIF->amb_header, 0);
	pIF->tex_buf = amTxbGetTexFileList(buf);

	pIF->texlistbuf = amMemAlloc((Uint32)nnEstimateTexlistSize(pIF->tex_buf->nTex));
	nnSetUpTexlist(&pIF->texlist, pIF->tex_buf->nTex, pIF->texlistbuf);
	pIF->regId = amTextureLoad(pIF->texlist, pIF->tex_buf, NULL, pIF->amb_header);
	pIF->drawFlag = AMD_ON;
	pIF->texId = 0;
}

// ==========================================================================
// GmDecoGlareDraw
/*!
 *	擬似グレア描画
 *
 *	@param	obj_work	[in] オブジェクトワーク
 */
// ==========================================================================
void GmDecoGlareDraw( OBS_OBJECT_WORK* obj_work )
{
	GME_DECORATE_ID deco_id = (GME_DECORATE_ID)obj_work->user_work;

	if ( deco_id < GMD_DECOGLARE_OFSEETID ) return;

	GMS_DECOGLARE_PARAM dpm;

	switch ( deco_id )
	{
	case GMD_DECORATE_ID_GLARE_ORANGE:
	case GMD_DECORATE_ID_GLARE_ORANGE_ENDING:
		dpm = _gm_decoGlare_param[GME_DECOGLARE_ORANGE];
		break;
	case GMD_DECORATE_ID_GLARE_RED:
		dpm = _gm_decoGlare_param[GME_DECOGLARE_RED];
		break;
	case GMD_DECORATE_ID_GLARE_GREEN:
	case GMD_DECORATE_ID_GLARE_GREEN_ENDING:
		dpm = _gm_decoGlare_param[GME_DECOGLARE_GREEN];
		break;
	case GMD_DECORATE_ID_GLARE_SALMON:
		dpm = _gm_decoGlare_param[GME_DECOGLARE_SALMON];
		break;
	default:
		return;
	}

	if ( !amDrawIsRegistComplete(pIF->regId) ) return;
	if ( pIF->drawFlag != AMD_ON ) return;
	if ( obj_work->disp_flag & OBD_DISP_NODISP ) return;	// 20091224 Ishizaki

	// 位置取得
	VecFx32	pos = obj_work->pos;
//	pos.x += obj_work->ofst.x;
//	pos.y += obj_work->ofst.y;
//	pos.z += obj_work->ofst.z;
	float x = (float)(pos.x >> FX32_SHIFT);
	float y = -(float)(pos.y >> FX32_SHIFT);
	float z = (float)(GMD_OBJ_DEFAULT_POS_Z_N >> FX32_SHIFT);

	AMS_PARAM_DRAW_PRIMITIVE param;
	memset(&param, 0, sizeof(AMS_PARAM_DRAW_PRIMITIVE));
	param.aTest = 0;
	param.zMask = 0;
	param.zTest = 1;
	param.ablend = NNE_PRIM_ALPHABLEND_ON;

	// αブレンド設定
	switch (dpm.ablend)
	{
	case GME_DECOGLARE_ABLEND_NORMAL:
		amDrawGetPrimBlendParam(AMDRAWE_BLENDTYPE_NORMAL, &param);
		break;

	case GME_DECOGLARE_ABLEND_ADD:
		amDrawGetPrimBlendParam(AMDRAWE_BLENDTYPE_ADD, &param);
		break;
	}

#if (1)
	// テクスチャあり
	NNS_PRIM3D_PCT *poliData = (NNS_PRIM3D_PCT *)amDrawMallocDataBuffer(sizeof(NNS_PRIM3D_PCT) * 6);
	float hw = dpm.size * 0.5f;
	float hh = dpm.size * 0.5f;

	x += 10;
	y -= 10;

    // 頂点
	amVectorSet((NNS_VECTOR*)&poliData[0], x - hw, y + hh, z);
	amVectorSet((NNS_VECTOR*)&poliData[1], x + hw, y + hh, z);
	amVectorSet((NNS_VECTOR*)&poliData[2], x - hw, y - hh, z);
	amVectorSet((NNS_VECTOR*)&poliData[5], x + hw, y - hh, z);

    // カラー
	poliData[0].Col = dpm.color;
	poliData[1].Col = dpm.color;
	poliData[2].Col = dpm.color;
	poliData[5].Col = dpm.color;

	// UV
	poliData[0].Tex.u = 0.0f; poliData[0].Tex.v = 0.0f;
	poliData[1].Tex.u = 1.0f; poliData[1].Tex.v = 0.0f;
	poliData[2].Tex.u = 0.0f; poliData[2].Tex.v = 1.0f;
	poliData[5].Tex.u = 1.0f; poliData[5].Tex.v = 1.0f;

	poliData[3] = poliData[1];
	poliData[4] = poliData[2];

	// プリミティブ描画設定
	param.format3D = NNE_PRIM3D_FMT_PCT;
	param.type = NNE_PRIM_TRIANGLE_LIST;
	param.vtxPCT3D = poliData;
	param.texlist = pIF->texlist;
	param.texId = pIF->texId;
	param.count = 6;
	param.sortZ = dpm.sort_z;
#else
	// テクスチャなし
	NNS_PRIM3D_PC *poliData = (NNS_PRIM3D_PC *)amDrawMallocDataBuffer(sizeof(NNS_PRIM3D_PC) * 6);
	float hw = dpm.size * 0.5f;
	float hh = dpm.size * 0.5f;

    // 頂点
	amVectorSet((NNS_VECTOR*)&poliData[0], x - hw, y + hh, z);
	amVectorSet((NNS_VECTOR*)&poliData[1], x + hw, y + hh, z);
	amVectorSet((NNS_VECTOR*)&poliData[2], x - hw, y - hh, z);
	amVectorSet((NNS_VECTOR*)&poliData[5], x + hw, y - hh, z);

    // カラー
	poliData[0].Col = dpm.color;
	poliData[1].Col = dpm.color;
	poliData[2].Col = dpm.color;
	poliData[5].Col = dpm.color;

	poliData[3] = poliData[1];
	poliData[4] = poliData[2];

	// プリミティブ描画設定
	param.format3D = NNE_PRIM3D_FMT_PC;
	param.type = NNE_PRIM_TRIANGLE_LIST;
	param.vtxPC3D = poliData;
	param.texlist = 0;
	param.texId = -1;
	param.count = 6;
	param.sortZ = dpm.sort_z;
#endif

	amDrawPrimitive3D(OBD_DRAW_CMD_STATE_WATER, &param);
}

// ==========================================================================
// GmDecoGlareDataRelease
/*!
 *	擬似グレア用データ開放
 *
 *  @note ambはgmDecoDataFlushで開放される
 */
// ==========================================================================
void GmDecoGlareDataRelease(void)
{
	pIF->drawFlag = AMD_OFF;

	if ( pIF->texlist )
	{
        pIF->regId = amTextureRelease(pIF->texlist);
		pIF->texlist = NULL;
	}

	if ( pIF->texlistbuf)
	{
        amMemFree(pIF->texlistbuf);
		pIF->texlistbuf = NULL;
	}
}

// ==========================================================================
// GmDecoGlareGetGlobal
/*!
 *	擬似グレア管理グローバル参照
 *
 *  @note 軌跡テクスチャなどに流用
 */
// ==========================================================================
GMDECO_GLARE_INTERFACE* GmDecoGlareGetGlobal(void)
{
	amAssert(pIF);
    return pIF;
}

/*--- Local Functions ------------------------------------------------------*/
//## Local Functions



/*--- TCB Functions ---------------------------------------------------------*/
//## TCB Functions





