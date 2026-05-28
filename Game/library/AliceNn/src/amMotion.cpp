/*****************************************************************************/
/*      amMotion.cpp                Author : Takashi Nakano                  */
/*            Copyright(c) 2009 Dimps CORP. All Rights Reserved.             */
/*---------------------------------------------------------------------------*/
/* モーションライブラリプログラム                                            */
/*---------------------------------------------------------------------------*/
/* Date          Ver    Comment                                              */
/* 090409-       0.01   first version                                        */
/*****************************************************************************/

/*--- Include Files ---------------------------------------------------------*/

#include "alice.h"


/*--- Macros ----------------------------------------------------------------*/

/*--- Definitions -----------------------------------------------------------*/

/*--- External Valiables ----------------------------------------------------*/

extern void _amDrawMotionTRS(AMS_COMMAND_HEADER *command, NNF_DRAWOBJ drawflag);

void _amObjectResolvePointer(
		NNS_BINCNK_DATAHEADER *data, NNS_BINCNK_NOF0HEADER *nof0);


/*--- Global Variables ------------------------------------------------------*/

/*--- Local Variables -------------------------------------------------------*/

/*--- Global Functions ------------------------------------------------------*/

/*****************************************************************************/
/* AMS_MOTION *amMotionCreate(NNS_OBJECT *object, Sint32 flag)               */
/*---------------------------------------------------------------------------*/
/* [INPUT]   object : オブジェクト                                           */
/*           flag   : フラグ                                                 */
/* [RETURN]  モーション管理へのポインタ                                      */
/* [FUNCTION]  モーション構造体の生成                                        */
/*****************************************************************************/
AMS_MOTION *amMotionCreate(NNS_OBJECT *object, Sint32 flag)
{
	return	amMotionCreate(object,
			AMD_MOTION_DEFAULT_MAX, AMD_MOTION_MATERIAL_DEFAULT_MAX, flag);
}


/*****************************************************************************/
/* AMS_MOTION *amMotionCreate(NNS_OBJECT *object,                            */
/*                       Sint32 motion_num, Sint32 mmotion_num, Sint32 flag) */
/*---------------------------------------------------------------------------*/
/* [INPUT]   object      : オブジェクト                                      */
/*           motion_num  : モーション最大数                                  */
/*           mmotion_num : マテリアルモーション最大数                        */
/*           flag        : フラグ                                            */
/* [RETURN]  モーション管理へのポインタ                                      */
/* [FUNCTION]  モーション構造体の生成                                        */
/*****************************************************************************/
AMS_MOTION *amMotionCreate(NNS_OBJECT *object, Sint32 motion_num, Sint32 mmotion_num, Sint32 flag)
{
	amAssert(object);
	amAssert(motion_num);
	amAssert(mmotion_num);

	AMS_MOTION		*motion;
	AMS_MOTION_BUF	*mbuf;
	Sint32		node_num, i;

	motion_num	= (motion_num + 3) & ~3;
	mmotion_num	= (mmotion_num + 3) & ~3;

	node_num	= object->nNode;
	if (flag & AMD_MOTION_CREATE_FLAG_MARGE) {
		motion		= (AMS_MOTION *)amMemAlloc(sizeof(AMS_MOTION)
					+ sizeof(NNS_TRS) * 4 * node_num
					+ sizeof(NNS_MOTION *) * motion_num
					+ sizeof(NNS_MOTION *) * mmotion_num);
	} else {
		motion		= (AMS_MOTION *)amMemAlloc(sizeof(AMS_MOTION)
					+ sizeof(NNS_TRS) * 2 * node_num
					+ sizeof(NNS_MOTION *) * motion_num
					+ sizeof(NNS_MOTION *) * mmotion_num);
	}

	motion->mtnbuf	= (NNS_MOTION **)(motion + 1);
	motion->mmtn	= (NNS_MOTION **)(motion->mtnbuf + motion_num);
	motion->data	= (NNS_TRS *)(motion->mmtn + mmotion_num);

	motion->object	= object;
	motion->node_num	= node_num;
	for (i = 0; i < AMD_MOTION_FILE_MAX; i++) {
		motion->mtnfile[i].file		= NULL;
		motion->mtnfile[i].motion	= NULL;
		motion->mtnfile[i].motion_num	= 0;
	}
	motion->motion_num	= motion_num;
	for (i = 0; i < motion_num; i++)
		motion->mtnbuf[i]	= NULL;
	mbuf	= &motion->mbuf[0];
	for (i = 0; i < 2; i++, mbuf++) {
		mbuf->motion_id		= 0;
		mbuf->frame			= 0.0f;
		if (i == 0)
			mbuf->mbuf			= (NNS_TRS *)(motion->data + node_num);
		else if (flag & AMD_MOTION_CREATE_FLAG_MARGE) {
			mbuf->mbuf			= (NNS_TRS *)(motion->mbuf[0].mbuf + node_num);
			motion->mmbuf		= (NNS_TRS *)(motion->mbuf[1].mbuf + node_num);
			nnCalcTRSList(motion->mbuf[1].mbuf, object);
		} else {
			mbuf->mbuf			= NULL;
			motion->mmbuf		= NULL;
		}
	}
	nnCalcTRSList(motion->mbuf[0].mbuf, object);
	nnCalcTRSList(motion->data, object);

	motion->mmobject	= NULL;
	motion->mmobj_size	= 0;
	motion->mmotion_num	= mmotion_num;
	memset(motion->mmtn, 0, sizeof(NNS_MOTION *) * mmotion_num);

	return	motion;
}


/*****************************************************************************/
/* void amMotionDelete(AMS_MOTION *motion)                                   */
/*---------------------------------------------------------------------------*/
/* [INPUT]   motion : モーション管理                                         */
/* [FUNCTION]  モーション構造体の削除                                        */
/*****************************************************************************/
void amMotionDelete(AMS_MOTION *motion)
{
	if (motion->mmobject != NULL)
		amMemFree(motion->mmobject);

	amMemFree(motion);
}


/*****************************************************************************/
/* void amMotionRegistFile(AMS_MOTION *motion, Sint32 file_id,               */
/*                                                   AMS_AMB_HEADER *amb)    */
/*---------------------------------------------------------------------------*/
/* [INPUT]   motion  : モーション管理                                        */
/*           file_id : ファイルID                                            */
/*           amb     : AMBファイル                                           */
/* [FUNCTION]  モーションファイルの登録                                      */
/*****************************************************************************/
void amMotionRegistFile(AMS_MOTION *motion, Sint32 file_id, AMS_AMB_HEADER *amb)
{
	amAssert(motion);
	amAssert(amb);
	amAssert((file_id >= 0) && (file_id < AMD_MOTION_FILE_MAX));

	// いまのところ上書きは禁止
	amAssert(motion->mtnfile[file_id].file == NULL);

	AMS_MOTION_FILE	*mfile;
	NNS_MOTION	**mtnbuf;
	Sint32		i;

	mfile		= &motion->mtnfile[0];
	mtnbuf		= mfile->motion + mfile->motion_num;
	mfile++;
	for (i = 1; i < AMD_MOTION_FILE_MAX; i++, mfile++) {
		NNS_MOTION	**mtnbuf0;
		mtnbuf0		= mfile->motion + mfile->motion_num;
		if ((Sint32)mtnbuf < (Sint32)mtnbuf0)
			mtnbuf		= mtnbuf0;
	}
	if (mtnbuf == NULL)
		mtnbuf		= &motion->mtnbuf[0];

	mfile			= &motion->mtnfile[file_id];
	mfile->file		= amb;
	mfile->motion	= mtnbuf;
	mfile->motion_num	= amMotionSetup(mtnbuf, amb);
}


/*****************************************************************************/
/* void amMotionRegistFile(AMS_MOTION *motion, Sint32 file_id, void *buf)    */
/*---------------------------------------------------------------------------*/
/* [INPUT]   motion  : モーション管理                                        */
/*           file_id : ファイルID                                            */
/*           buf     : ファイル                                              */
/* [FUNCTION]  モーションファイルの登録                                      */
/*****************************************************************************/
void amMotionRegistFile(AMS_MOTION *motion, Sint32 file_id, void *buf)
{
	amAssert(motion);
	amAssert(buf);
	amAssert((file_id >= 0) && (file_id < AMD_MOTION_FILE_MAX));

	// いまのところ上書きは禁止
	amAssert(motion->mtnfile[file_id].file == NULL);

	AMS_MOTION_FILE	*mfile;
	NNS_MOTION	**mtnbuf;
	Sint32		i;

	mfile		= &motion->mtnfile[0];
	mtnbuf		= mfile->motion + mfile->motion_num;
	mfile++;
	for (i = 1; i < AMD_MOTION_FILE_MAX; i++, mfile++) {
		NNS_MOTION	**mtnbuf0;
		mtnbuf0		= mfile->motion + mfile->motion_num;
		if ((Sint32)mtnbuf < (Sint32)mtnbuf0)
			mtnbuf		= mtnbuf0;
	}
	if (mtnbuf == NULL)
		mtnbuf		= &motion->mtnbuf[0];

	mfile			= &motion->mtnfile[file_id];
	mfile->file		= buf;
	mfile->motion	= mtnbuf;
	mfile->motion_num	= amMotionSetup(mtnbuf, buf);
}


/*****************************************************************************/
/* Sint32 amMotionSetup(NNS_MOTION **motion, AMS_AMB_HEADER *amb)            */
/*---------------------------------------------------------------------------*/
/* [INPUT]   amb    : AMBファイル                                            */
/* [OUTPUT]  motion : モーションへのポインタ                                 */
/* [RETURN]  モーション数                                                    */
/* [FUNCTION]  モーションの取得                                              */
/*****************************************************************************/
Sint32 amMotionSetup(NNS_MOTION **motion, AMS_AMB_HEADER *amb)
{
	amAssert(motion);
	amAssert(amb);

	AMS_AMB_FILE	*file;
	NNS_MOTION	**mbuf;
	Sint32		i, num, n;

	mbuf	= motion;
	num		= 0;
	*mbuf	= NULL;

	file	= amb->file;
	for (i = amb->file_num; i > 0; i--, file++) {
		n		= amMotionSetup(mbuf, file->data);
		num		+= n;
		mbuf	+= n;
	}

	return	num;
}


/*****************************************************************************/
/* Sint32 amMotionSetup(NNS_MOTION **motion, void *buf)                      */
/*---------------------------------------------------------------------------*/
/* [INPUT]   buf    : ファイル                                               */
/* [OUTPUT]  motion : モーションへのポインタ                                 */
/* [RETURN]  モーション数                                                    */
/* [FUNCTION]  モーションの取得                                              */
/*****************************************************************************/
Sint32 amMotionSetup(NNS_MOTION **motion, void *buf)
{
	amAssert(motion);
	amAssert(buf);

	NNS_BINCNK_FILEHEADER	*header;
	NNS_BINCNK_DATAHEADER	*data;
	NNS_BINCNK_NOF0HEADER	*nof0;
	NNS_MOTION	**mbuf;
	Uint8		*data0;
	Sint32		i, num;

	mbuf		= motion;
	num			= 0;
	*mbuf		= NULL;

	header		= (NNS_BINCNK_FILEHEADER *)buf;
#if _PC | _XBOX
	amAssert(header->Id == NND_CHUNK_HEADER_DXG20);
#endif

	// オフセット変換
	data	= (NNS_BINCNK_DATAHEADER *)((Uint8 *)buf + header->OfsData);
	data0	= (Uint8 *)data;
	nof0	= (NNS_BINCNK_NOF0HEADER *)((Uint8 *)buf + header->OfsNOF0);
	_amObjectResolvePointer(data, nof0);

	i		= header->nChunk;
	for (; i > 0; i++, data = (NNS_BINCNK_DATAHEADER *)
			((Uint8 *)&data->OfsMainData + amConvertLittleEndian(data->OfsNextId))) {
		switch (data->Id) {
#if _PC | _XBOX
			case	NND_CHUNK_MOTION_DXG20:
			case	NND_CHUNK_CAMERA_MOTION_DXG20:
			case	NND_CHUNK_MATERIAL_MOTION_DXG20:
				*(mbuf++)		= (NNS_MOTION *)(data0 + data->OfsMainData);
				num++;
				continue;
			case	NND_CHUNK_NEND_DXG20:
				break;
#elif _PS3
			case	NND_CHUNK_MOTION_PS3:
			case	NND_CHUNK_CAMERA_MOTION_PS3:
			case	NND_CHUNK_MATERIAL_MOTION_PS3:
				*(mbuf++)		= (NNS_MOTION *)(data0 + data->OfsMainData);
				num++;
				continue;
			case	NND_CHUNK_NEND_PS3:
				break;
#elif _WII
			case	NND_CHUNK_MOTION_GC:
			case	NND_CHUNK_CAMERA_MOTION_GC:
			case	NND_CHUNK_MATERIAL_MOTION_GC:
				*(mbuf++)		= (NNS_MOTION *)(data0 + data->OfsMainData);
				num++;
				continue;
			case	NND_CHUNK_NEND_GC:
				break;
#elif _IPHONE
			case	NND_CHUNK_MOTION_GLES11:
			case	NND_CHUNK_CAMERA_MOTION_GLES11:
			case	NND_CHUNK_MATERIAL_MOTION_GLES11:
				*(mbuf++)		= (NNS_MOTION *)(data0 + data->OfsMainData);
				num++;
				continue;
			case	NND_CHUNK_NEND_GLES11:
				break;
#endif
			default:
				continue;
		}
		break;
	}

	return	num;
}


/*****************************************************************************/
/* void amMotionSet(AMS_MOTION *motion, Sint32 mbuf_id, Sint32 motion_id)    */
/*---------------------------------------------------------------------------*/
/* [INPUT]   motion    : モーション管理                                      */
/*           mbuf_id   : モーションバッファID                                */
/*           motion_id : モーションID                                        */
/* [FUNCTION]  モーションの設定                                              */
/*****************************************************************************/
void amMotionSet(AMS_MOTION *motion, Sint32 mbuf_id, Sint32 motion_id)
{
	amAssert(motion);
	amAssert((Uint32)mbuf_id <= 1);
	amAssert(motion->mbuf[mbuf_id].mbuf != NULL);
	amAssert((motion_id & 0xffff) < motion->mtnfile[motion_id >> 16].motion_num);

	AMS_MOTION_BUF		*mbuf;

	mbuf	= &motion->mbuf[mbuf_id];
	mbuf->motion_id		= motion_id;
	mbuf->frame			=
			motion->mtnfile[motion_id >> 16].motion[motion_id & 0xffff]->StartFrame;
}


/*****************************************************************************/
/* void amMotionSetFrame(AMS_MOTION *motion, Sint32 mbuf_id, float frame)    */
/*---------------------------------------------------------------------------*/
/* [INPUT]   motion    : モーション管理                                      */
/*           mbuf_id   : モーションバッファID                                */
/*           frame     : モーションフレーム                                  */
/* [FUNCTION]  モーションフレームの設定                                      */
/*****************************************************************************/
void amMotionSetFrame(AMS_MOTION *motion, Sint32 mbuf_id, float frame)
{
	amAssert(motion);
	amAssert((Uint32)mbuf_id <= 1);
	amAssert(motion->mbuf[mbuf_id].mbuf != NULL);

	AMS_MOTION_BUF		*mbuf;

	mbuf	= &motion->mbuf[mbuf_id];
	mbuf->frame			= frame;
}


/*****************************************************************************/
/* void amMotionCalc(AMS_MOTION *motion, Sint32 mbuf_id)                     */
/*---------------------------------------------------------------------------*/
/* [INPUT]   motion    : モーション管理                                      */
/*           mbuf_id   : モーションバッファID                                */
/* [FUNCTION]  モーションの計算                                              */
/*****************************************************************************/
void amMotionCalc(AMS_MOTION *motion, Sint32 mbuf_id)
{
	amAssert(motion);

	AMS_MOTION_BUF		*mbuf;
	Sint32		i;
	Sint32		mfile_id;
	Sint32		motion_id;

	mbuf	= &motion->mbuf[0];
	for (i = 0; i < 2; i++, mbuf++, mbuf_id >>= 1) {
		if (!(mbuf_id & 1))
			continue;
		if (mbuf->mbuf == NULL)
			continue;

		motion_id	= mbuf->motion_id;
		mfile_id	= motion_id >> 16;
		motion_id	&= 0xffff;
		nnCalcTRSListMotion(mbuf->mbuf, motion->object,
				motion->mtnfile[mfile_id].motion[motion_id], mbuf->frame);
	}
}


/*****************************************************************************/
/* void amMotionApply(AMS_MOTION *motion, float marge, float per)            */
/*---------------------------------------------------------------------------*/
/* [INPUT]   motion    : モーション管理                                      */
/*           marge     : 並列補間率                                          */
/*           per       : 直列補間率                                          */
/* [FUNCTION]  最終モーションの計算                                          */
/*****************************************************************************/
void amMotionApply(AMS_MOTION *motion, float marge, float per)
{
	NNS_TRS		*mmbuf = motion->mbuf[0].mbuf;

	if (per <= 0.0f)
		return;			// 変化なし

	// 並列補間
	if (motion->mbuf[1].mbuf != NULL) {
		if (marge >= 1.0f)
			mmbuf		= motion->mbuf[1].mbuf;
		else if (marge > 0.0f) {
			if (per < 1.0f) {
				mmbuf		= motion->mmbuf;
				nnLinkMotion(mmbuf, motion->mbuf[0].mbuf, motion->mbuf[1].mbuf,
						motion->node_num, marge);
			} else {
				nnLinkMotion(motion->data, motion->mbuf[0].mbuf, motion->mbuf[1].mbuf,
						motion->node_num, marge);
				return;
			}
		}
	}

	// 直列補間
	if (per >= 1.0f)
		memcpy(motion->data, mmbuf, sizeof(NNS_TRS) * motion->node_num);
	else
		nnLinkMotion(motion->data, motion->data, mmbuf, motion->node_num, per);
}


/*****************************************************************************/
/* void amMotionGet(AMS_MOTION *motion, float marge, float per)              */
/*---------------------------------------------------------------------------*/
/* [INPUT]   motion    : モーション管理                                      */
/*           marge     : 並列補間率                                          */
/*           per       : 直列補間率                                          */
/* [FUNCTION]  最終モーションの計算(Calc + Apply)                            */
/*****************************************************************************/
void amMotionGet(AMS_MOTION *motion, float marge, float per)
{
	amMotionCalc(motion);
	amMotionApply(motion, marge, per);
}


/*****************************************************************************/
/* void amMotionMaterialRegistFile(AMS_MOTION *motion,                       */
/*                                      Sint32 file_id, AMS_AMB_HEADER *amb) */
/*---------------------------------------------------------------------------*/
/* [INPUT]   motion  : モーション管理                                        */
/*           file_id : ファイルID(0のみ)                                     */
/*           amb     : マテリアルモーションファイル                          */
/* [FUNCTION]  マテリアルモーションファイルの登録                            */
/*****************************************************************************/
void amMotionMaterialRegistFile(AMS_MOTION *motion, Sint32 file_id, AMS_AMB_HEADER *amb)
{
	amAssert(motion);
	amAssert(amb);
	amAssert(file_id == 0);

	Sint32		num, i;
	Uint32		max_size = 0, size;

	num		= amb->file_num;
	for (i = 0; i < num; i++) {
		amMotionSetup(&motion->mmtn[file_id + i], amb->file[i].data);
		size	= nnCalcMaterialMotionObjectBufferSize(
				motion->object, motion->mmtn[file_id + i]);
		if (size > max_size)
			max_size	= size;
	}

	if (motion->mmobject != NULL)
		amMemFree(motion->mmobject);

	motion->mmotion_id		= file_id;
	motion->mmotion_frame	= 0.0f;

	motion->mmobj_size	= max_size;
	motion->mmobject	= (NNS_MATMOTOBJ *)amMemAlloc(motion->mmobj_size);
	nnInitMaterialMotionObject(motion->mmobject,
			motion->object, motion->mmtn[motion->mmotion_id]);
}


/*****************************************************************************/
/* void amMotionMaterialRegistFile(AMS_MOTION *motion,                       */
/*                                               Sint32 file_id, void *file) */
/*---------------------------------------------------------------------------*/
/* [INPUT]   motion  : モーション管理                                        */
/*           file_id : ファイルID(0のみ)                                     */
/*           file    : マテリアルモーションファイル                          */
/* [FUNCTION]  マテリアルモーションファイルの登録                            */
/*****************************************************************************/
void amMotionMaterialRegistFile(AMS_MOTION *motion, Sint32 file_id, void *file)
{
	amAssert(motion);
	amAssert(file);
	amAssert(file_id == 0);

	amMotionSetup(&motion->mmtn[file_id], file);

	if (motion->mmobject != NULL)
		amMemFree(motion->mmobject);

	motion->mmotion_id		= file_id;
	motion->mmotion_frame	= 0.0f;

	motion->mmobj_size	= nnCalcMaterialMotionObjectBufferSize(
			motion->object, motion->mmtn[motion->mmotion_id]);
	motion->mmobject	= (NNS_MATMOTOBJ *)amMemAlloc(motion->mmobj_size);
	nnInitMaterialMotionObject(motion->mmobject,
			motion->object, motion->mmtn[motion->mmotion_id]);
}


/*****************************************************************************/
/* void amMotionMaterialSet(AMS_MOTION *motion, Sint32 motion_id)            */
/*---------------------------------------------------------------------------*/
/* [INPUT]   motion    : モーション管理                                      */
/*           motion_id : モーションID                                        */
/* [FUNCTION]  マテリアルモーションの設定                                    */
/*****************************************************************************/
void amMotionMaterialSet(AMS_MOTION *motion, Sint32 motion_id)
{
	amAssert(motion);

	motion->mmotion_id	= motion_id;
	motion->mmotion_frame	= 0.0f;

	nnInitMaterialMotionObject(motion->mmobject,
			motion->object, motion->mmtn[motion->mmotion_id]);
}


/*****************************************************************************/
/* void amMotionMaterialSetFrame(AMS_MOTION *motion, float frame)            */
/*---------------------------------------------------------------------------*/
/* [INPUT]   motion    : モーション管理                                      */
/*           frame     : モーションフレーム                                  */
/* [FUNCTION]  マテリアルモーションフレームの設定                            */
/*****************************************************************************/
void amMotionMaterialSetFrame(AMS_MOTION *motion, float frame)
{
	amAssert(motion);

	motion->mmotion_frame	= frame;
}


/*****************************************************************************/
/* void amMotionMaterialCalc(AMS_MOTION *motion)                             */
/*---------------------------------------------------------------------------*/
/* [INPUT]   motion    : モーション管理                                      */
/* [FUNCTION]  マテリアルモーションの計算                                    */
/*****************************************************************************/
void amMotionMaterialCalc(AMS_MOTION *motion)
{
	amAssert(motion);

	if (!amThreadCheckDraw())
		return;

	nnCalcMaterialMotion(motion->mmobject,
			motion->object, motion->mmtn[motion->mmotion_id], motion->mmotion_frame);
}


/*****************************************************************************/
/* void amMotionDraw(Uint32 state, AMS_MOTION *motion, NNS_TEXLIST *texlist, */
/*                     NNF_DRAWOBJ drawflag, NNS_MATERIALCALLBACK_FUNC func) */
/*---------------------------------------------------------------------------*/
/* [INPUT] state    : 描画ステート                                           */
/*         motion   : モーション管理                                         */
/*         texlist  : テクスチャリスト                                       */
/*         drawflag : 描画フラグ                                             */
/*         func     : マテリアルコールバック関数                             */
/* [FUNCTION]  モーションオブジェクトの描画                                  */
/*****************************************************************************/
void amMotionDraw(Uint32 state, AMS_MOTION *motion, NNS_TEXLIST *texlist, NNF_DRAWOBJ drawflag, NNS_MATERIALCALLBACK_FUNC func)
{
	AMS_PARAM_DRAW_MOTION_TRS	*param;
	NNS_MATRIX		*mtx;
	Sint32			num;

	amAssert(motion);

	amThreadCheckSafe(0, "amMotionDraw");

	num		= motion->node_num;

	param	= (AMS_PARAM_DRAW_MOTION_TRS *)amDrawMallocDataBuffer(
					sizeof(AMS_PARAM_DRAW_MOTION_TRS) + sizeof(NNS_MATRIX) +
					sizeof(NNS_TRS) * num);

	mtx		= (NNS_MATRIX *)(param + 1);
	nnCopyMatrix(mtx, amMatrixGetCurrent());

	param->object	= motion->object;
	param->mtx		= mtx;
	param->sub_obj_type	= 0;
	param->flag		= drawflag;
	param->texlist	= texlist;
	param->trslist	= (NNS_TRS *)(mtx + 1);
	param->material_func	= func;
	memcpy(param->trslist, motion->data, sizeof(NNS_TRS) * num);

	Sint32		motion_id;
	motion_id			= motion->mbuf[0].motion_id;
	param->motion		= motion->mtnfile[motion_id >> 16].motion[motion_id & 0xffff];
	param->frame		= motion->mbuf[0].frame;

	amDrawRegistCommand(state, AMD_COMMAND_DRAW_MOTION_TRS, param);
}


/*****************************************************************************/
/* void amMotionMaterialDraw(Uint32 state, AMS_MOTION *motion,               */
/*                               NNS_TEXLIST *texlist, NNF_DRAWOBJ drawflag, */
/*                                           NNS_MATERIALCALLBACK_FUNC func) */
/*---------------------------------------------------------------------------*/
/* [INPUT] state    : 描画ステート                                           */
/*         motion   : モーション管理                                         */
/*         texlist  : テクスチャリスト                                       */
/*         drawflag : 描画フラグ                                             */
/*         func     : マテリアルコールバック関数                             */
/* [FUNCTION]  モーションオブジェクトの描画                                  */
/*****************************************************************************/
void amMotionMaterialDraw(Uint32 state, AMS_MOTION *motion, NNS_TEXLIST *texlist, NNF_DRAWOBJ drawflag, NNS_MATERIALCALLBACK_FUNC func)
{
	AMS_PARAM_DRAW_MOTION_TRS	*param;
	NNS_MATRIX		*mtx;
	Sint32			num;

	amAssert(motion);

	if (motion->mmobject == NULL) {
		amMotionDraw(state, motion, texlist, drawflag);
		return;
	}

	amThreadCheckSafe(0, "amMotionMaterialDraw");

	num		= motion->node_num;

	param	= (AMS_PARAM_DRAW_MOTION_TRS *)amDrawMallocDataBuffer(
					sizeof(AMS_PARAM_DRAW_MOTION_TRS) + sizeof(NNS_MATRIX) +
					sizeof(NNS_TRS) * num + motion->mmobj_size);

	mtx		= (NNS_MATRIX *)(param + 1);
	nnCopyMatrix(mtx, amMatrixGetCurrent());

	param->mtx		= mtx;
	param->sub_obj_type	= 0;
	param->flag		= drawflag;
	param->texlist	= texlist;
	param->trslist	= (NNS_TRS *)(mtx + 1);
	param->material_func	= func;
	memcpy(param->trslist, motion->data, sizeof(NNS_TRS) * num);

//	param->object	= (NNS_OBJECT *)(param->trslist + num);
//	memcpy(param->object, motion->mmobject, motion->mmobj_size);
	param->object	= motion->object;
	param->mmotion	= motion->mmtn[motion->mmotion_id];
	param->mframe	= motion->mmotion_frame;

	Sint32		motion_id;
	motion_id			= motion->mbuf[0].motion_id;
	if (motion->mtnfile[motion_id >> 16].file != NULL) {
		param->motion		= motion->mtnfile[motion_id >> 16].motion[motion_id & 0xffff];
		param->frame		= motion->mbuf[0].frame;
	} else {
		param->motion		= NULL;
		param->frame		= 0.0f;
	}

	amDrawRegistCommand(state, AMD_COMMAND_DRAW_MOTION_TRS_MATMTN, param);
}


/*****************************************************************************/
/* void amMotionDraw(AMS_MOTION *motion, NNS_TEXLIST *texlist,               */
/*                     NNF_DRAWOBJ drawflag, NNS_MATERIALCALLBACK_FUNC func) */
/*---------------------------------------------------------------------------*/
/* [INPUT] motion   : モーション管理                                         */
/*         texlist  : テクスチャリスト                                       */
/*         drawflag : 描画フラグ                                             */
/*         func     : マテリアルコールバック関数                             */
/* [FUNCTION]  モーションオブジェクトの描画                                  */
/*****************************************************************************/
void amMotionDraw(AMS_MOTION *motion, NNS_TEXLIST *texlist, NNF_DRAWOBJ drawflag, NNS_MATERIALCALLBACK_FUNC func)
{
	amThreadCheckSafe(1, "amMotionDraw");

	AMS_COMMAND_HEADER			*command;
	AMS_PARAM_DRAW_MOTION_TRS	*param;

	command	= (AMS_COMMAND_HEADER *)amDrawMallocWorkBuffer(
			sizeof(AMS_COMMAND_HEADER) + sizeof(AMS_PARAM_DRAW_MOTION_TRS));
	param	= (AMS_PARAM_DRAW_MOTION_TRS *)(command + 1);

	command->command_id	= AMD_COMMAND_DRAW_MOTION_TRS;
	command->param		= param;

	param->object		= motion->object;
	param->mtx			= NULL;
	param->sub_obj_type	= 0;
	param->flag			= drawflag;
	param->texlist		= texlist;
	param->trslist		= motion->data;
	param->material_func	= func;

	Sint32		motion_id;
	motion_id			= motion->mbuf[0].motion_id;
	param->motion		= motion->mtnfile[motion_id >> 16].motion[motion_id & 0xffff];
	param->frame		= motion->mbuf[0].frame;

	_amDrawMotionTRS(command, drawflag);
}


/*****************************************************************************/
/* void amMotionMaterialDraw(AMS_MOTION *motion, NNS_TEXLIST *texlist,       */
/*                     NNF_DRAWOBJ drawflag, NNS_MATERIALCALLBACK_FUNC func) */
/*---------------------------------------------------------------------------*/
/* [INPUT] motion   : モーション管理                                         */
/*         texlist  : テクスチャリスト                                       */
/*         drawflag : 描画フラグ                                             */
/*         func     : マテリアルコールバック関数                             */
/* [FUNCTION]  モーションオブジェクトの描画                                  */
/*****************************************************************************/
void amMotionMaterialDraw(AMS_MOTION *motion, NNS_TEXLIST *texlist, NNF_DRAWOBJ drawflag, NNS_MATERIALCALLBACK_FUNC func)
{
	if (motion->mmobject == NULL) {
		amMotionDraw(motion, texlist, drawflag);
		return;
	}

	amThreadCheckSafe(1, "amMotionMaterialDraw");

	AMS_COMMAND_HEADER			*command;
	AMS_PARAM_DRAW_MOTION_TRS	*param;

	command	= (AMS_COMMAND_HEADER *)amDrawMallocWorkBuffer(
			sizeof(AMS_COMMAND_HEADER) + sizeof(AMS_PARAM_DRAW_MOTION_TRS));
	param	= (AMS_PARAM_DRAW_MOTION_TRS *)(command + 1);

	command->command_id	= AMD_COMMAND_DRAW_MOTION_TRS_MATMTN;
	command->param		= param;

	param->object		= (NNS_OBJECT *)motion->mmobject;
	param->mtx			= NULL;
	param->sub_obj_type	= 0;
	param->flag			= drawflag;
	param->texlist		= texlist;
	param->trslist		= motion->data;
	param->material_func	= func;
	param->mmotion		= NULL;
	param->mframe		= NULL;

	Sint32		motion_id;
	motion_id			= motion->mbuf[0].motion_id;
	if (motion->mtnfile[motion_id >> 16].file != NULL) {
		param->motion		= motion->mtnfile[motion_id >> 16].motion[motion_id & 0xffff];
		param->frame		= motion->mbuf[0].frame;
	} else {
		param->motion		= NULL;
		param->frame		= 0.0f;
	}

	_amDrawMotionTRS(command, drawflag);
}


/*****************************************************************************/
/* float amMotionGetStartFrame(AMS_MOTION *motion, Sint32 motion_id)         */
/*---------------------------------------------------------------------------*/
/* [INPUT]   motion    : モーション管理                                      */
/*           motion_id : モーションID                                        */
/* [RETURN]  モーション開始フレーム                                          */
/* [FUNCTION]  モーション開始フレームの取得                                  */
/*****************************************************************************/
float amMotionGetStartFrame(AMS_MOTION *motion, Sint32 motion_id)
{
	amAssert(motion);

	return	motion->mtnfile[motion_id >> 16].motion[motion_id & 0xffff]->StartFrame;
}


/*****************************************************************************/
/* float amMotionGetEndFrame(AMS_MOTION *motion, Sint32 motion_id)           */
/*---------------------------------------------------------------------------*/
/* [INPUT]   motion    : モーション管理                                      */
/*           motion_id : モーションID                                        */
/* [RETURN]  モーション終了フレーム                                          */
/* [FUNCTION]  モーション終了フレームの取得                                  */
/*****************************************************************************/
float amMotionGetEndFrame(AMS_MOTION *motion, Sint32 motion_id)
{
	amAssert(motion);

	return	motion->mtnfile[motion_id >> 16].motion[motion_id & 0xffff]->EndFrame;
}


/*****************************************************************************/
/* void amMotionGetFrames(AMS_MOTION *motion, Sint32 motion_id,              */
/*                                                 float *start, float *end) */
/*---------------------------------------------------------------------------*/
/* [INPUT]   motion    : モーション管理                                      */
/*           motion_id : モーションID                                        */
/* [OUTPUT]  start     : 開始フレーム                                        */
/*           end       : 終了フレーム                                        */
/* [FUNCTION]  モーション開始終了フレームの取得                              */
/*****************************************************************************/
void amMotionGetFrames(AMS_MOTION *motion, Sint32 motion_id, float *start, float *end)
{
	amAssert(motion);
	amAssert(start);
	amAssert(end);

	NNS_MOTION	*nnmtn;

	nnmtn		= motion->mtnfile[motion_id >> 16].motion[motion_id & 0xffff];

	*start		= nnmtn->StartFrame;
	*end		= nnmtn->EndFrame;
}


/*****************************************************************************/
/* float amMotionMaterialGetStartFrame(AMS_MOTION *motion, Sint32 motion_id) */
/*---------------------------------------------------------------------------*/
/* [INPUT]   motion    : モーション管理                                      */
/*           motion_id : モーションID(0のみ)                                 */
/* [RETURN]  モーション開始フレーム                                          */
/* [FUNCTION]  マテリアルモーション開始フレームの取得                        */
/*****************************************************************************/
float amMotionMaterialGetStartFrame(AMS_MOTION *motion, Sint32 motion_id)
{
	UNREFERENCED_PARAMETER(motion_id);

	amAssert(motion);

	return	motion->mmtn[motion->mmotion_id]->StartFrame;
}


/*****************************************************************************/
/* float amMotionMaterialGetEndFrame(AMS_MOTION *motion, Sint32 motion_id)   */
/*---------------------------------------------------------------------------*/
/* [INPUT]   motion    : モーション管理                                      */
/*           motion_id : モーションID(0のみ)                                 */
/* [RETURN]  モーション終了フレーム                                          */
/* [FUNCTION]  マテリアルモーション終了フレームの取得                        */
/*****************************************************************************/
float amMotionMaterialGetEndFrame(AMS_MOTION *motion, Sint32 motion_id)
{
	UNREFERENCED_PARAMETER(motion_id);

	amAssert(motion);

	return	motion->mmtn[motion->mmotion_id]->EndFrame;
}


/*****************************************************************************/
/* void amMotionMaterialGetFrames(AMS_MOTION *motion, Sint32 motion_id,      */
/*                                                 float *start, float *end) */
/*---------------------------------------------------------------------------*/
/* [INPUT]   motion    : モーション管理                                      */
/*           motion_id : モーションID(0のみ)                                 */
/* [OUTPUT]  start     : 開始フレーム                                        */
/*           end       : 終了フレーム                                        */
/* [FUNCTION]  モーション開始終了フレームの取得                              */
/*****************************************************************************/
void amMotionMaterialGetFrames(AMS_MOTION *motion, Sint32 motion_id, float *start, float *end)
{
	UNREFERENCED_PARAMETER(motion_id);

	amAssert(motion);
	amAssert(start);
	amAssert(end);

	NNS_MOTION	*nnmtn;

	nnmtn		= motion->mmtn[motion->mmotion_id];

	*start		= nnmtn->StartFrame;
	*end		= nnmtn->EndFrame;
}


/*--- Local Functions -------------------------------------------------------*/

