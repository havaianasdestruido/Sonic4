/*****************************************************************************/
/*      amObject.cpp                Author : Takashi Nakano                  */
/*            Copyright(c) 2009 Dimps CORP. All Rights Reserved.             */
/*---------------------------------------------------------------------------*/
/* オブジェクトライブラリプログラム                                          */
/*---------------------------------------------------------------------------*/
/* Date          Ver    Comment                                              */
/* 090401-       0.01   first version                                        */
/*****************************************************************************/

/*--- Include Files ---------------------------------------------------------*/

#include "alice.h"


/*--- Macros ----------------------------------------------------------------*/

/*--- Definitions -----------------------------------------------------------*/

void _amObjectResolvePointer(
		NNS_BINCNK_DATAHEADER *data, NNS_BINCNK_NOF0HEADER *nof0);


/*--- External Valiables ----------------------------------------------------*/

/*--- Global Variables ------------------------------------------------------*/

/*--- Local Variables -------------------------------------------------------*/

/*--- Global Functions ------------------------------------------------------*/

/*****************************************************************************/
/* void amObjectSetup(NNS_OBJECT **object, NNS_TEXFILELIST **texfilelist,    */
/*                                                                void *buf) */
/*---------------------------------------------------------------------------*/
/* [INPUT]   buf         : ファイル                                          */
/* [OUTPUT]  object      : オブジェクトへのポインタ                          */
/*           texfilelist : テクスチャファイルリストへのポインタ              */
/* [FUNCTION]  オブジェクトとテクスチャファイルリストの取得                  */
/*****************************************************************************/
void amObjectSetup(NNS_OBJECT **object, NNS_TEXFILELIST **texfilelist, void *buf)
{
	amAssert(object);
	amAssert(texfilelist);
	amAssert(buf);

	NNS_BINCNK_FILEHEADER	*header;
	NNS_BINCNK_DATAHEADER	*data;
	NNS_BINCNK_NOF0HEADER	*nof0;
	Uint8		*data0;
	Sint32		i;

	*object			= NULL;
	*texfilelist	= NULL;

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
	for (; i > 0; i--, data = (NNS_BINCNK_DATAHEADER *)
			((Uint8 *)&data->OfsMainData + amConvertLittleEndian(data->OfsNextId))) {
		switch (data->Id) {
#if _PC | _XBOX
			case	NND_CHUNK_TEXTURE_DXG20:
				*texfilelist	= (NNS_TEXFILELIST *)
						(data0 + data->OfsMainData);
				continue;
			case	NND_CHUNK_OBJECT_DXG20:
				*object			= (NNS_OBJECT *)
						(data0 + data->OfsMainData);
				continue;
			case	NND_CHUNK_NEND_DXG20:
				break;
#elif _PS3
			case	NND_CHUNK_TEXTURE_PS3:
				*texfilelist	= (NNS_TEXFILELIST *)
						(data0 + data->OfsMainData);
				continue;
			case	NND_CHUNK_OBJECT_PS3:
				*object			= (NNS_OBJECT *)
						(data0 + data->OfsMainData);
				continue;
			case	NND_CHUNK_NEND_PS3:
				break;
#elif _WII
			case	NND_CHUNK_TEXTURE_GC:
				*texfilelist	= (NNS_TEXFILELIST *)
						(data0 + data->OfsMainData);
				continue;
			case	NND_CHUNK_OBJECT_GC:
				*object			= (NNS_OBJECT *)
						(data0 + data->OfsMainData);
				continue;
			case	NND_CHUNK_NEND_GC:
				break;
#elif _IPHONE
			case	NND_CHUNK_TEXTURE_GLES11:
				*texfilelist	= (NNS_TEXFILELIST *)
				(data0 + data->OfsMainData);
				continue;
			case	NND_CHUNK_OBJECT_GLES11:
				*object			= (NNS_OBJECT *)
				(data0 + data->OfsMainData);
				continue;
			case	NND_CHUNK_NEND_GLES11:
				break;
#endif
			default:
				continue;
		}
		break;
	}

	if (*texfilelist != NULL) {
		// テクスチャファイル名大文字化
		Sint32		i, j;
		char		*cp, ch;
		NNS_TEXFILE	*texfile;
		texfile		= (*texfilelist)->pTexFileList;
		for (i = (*texfilelist)->nTex; i > 0; i--, texfile++) {
			cp			= texfile->Filename;
			for (j = strlen(cp); j > 0; j--, cp++) {
				ch			= *cp;
				if ((ch >= 'a') && (ch <= 'z'))
					*cp			= ch & 0xdf;
			}
		}
	}
}


/*****************************************************************************/
/* Sint32 amObjectLoad(NNS_OBJECT **object, NNS_OBJECT *obj_file,            */
/*                                                     NNF_DRAWOBJ drawflag) */
/*---------------------------------------------------------------------------*/
/* [INPUT]   obj_file    : オブジェクトへのポインタ(amObjectSetupで取得)     */
/*           drawflag    : 描画フラグ                                        */
/* [OUTPUT]  object      : オブジェクトへのポインタ(Wii以外は要free)         */
/* [RETURN]  登録完了をチェックするための登録ID                              */
/* [FUNCTION]  オブジェクト・シェーダーの登録                                */
/*****************************************************************************/
Sint32 amObjectLoad(NNS_OBJECT **object, NNS_OBJECT *obj_file, NNF_DRAWOBJ drawflag)
{
	AMS_PARAM_VERTEX_BUFFER_OBJECT	param;

#if _PC | _XBOX
	*object			= (NNS_OBJECT *)amMemAlloc(
			nnCalcVertexBufferObjectSizeDXG20(obj_file, 0));
#elif _PS3
	*object			= (NNS_OBJECT *)amMemAlloc(
			nnCalcBindBufferObjectSize(obj_file,
				0));
#elif _WII
	*object			= obj_file;
#elif _IPHONE
	*object			= (NNS_OBJECT *)amMemAlloc(
			nnCalcBindBufferObjectSizeGL(obj_file,
				0));
#endif
	param.obj		= *object;
	param.srcobj	= obj_file;
#if _PC | _XBOX
	param.vtxflag	= 0;
#elif _PS3 | _IPHONE
	param.bindflag	= 0;
#endif
	param.drawflag	= drawflag;

	return	amDrawRegistCommand(AMD_REGIST_VERTEX_BUFFER_OBJECT, &param);
}


/*****************************************************************************/
/* Sint32 amObjectLoad(NNS_OBJECT **object,                                  */
/*                      NNS_TEXLIST **texlist, void **texlistbuf, void *buf, */
/*                NNF_DRAWOBJ drawflag, char *filepath, AMS_AMB_HEADER *amb) */
/*---------------------------------------------------------------------------*/
/* [INPUT]   buf         : ファイル                                          */
/*           drawflag    : 描画フラグ                                        */
/*           filepath    : テクスチャファイルパス                            */
/*           amb         : テクスチャファイルが入ったAMBファイルへのポインタ */
/* [OUTPUT]  object      : オブジェクトへのポインタ(要free)                  */
/*           texlist     : テクスチャリストへのポインタ                      */
/*           texlistbuf  : テクスチャリストバッファ(要free)                  */
/* [RETURN]  登録完了をチェックするための登録ID                              */
/* [FUNCTION]  オブジェクト・シェーダーとテクスチャの登録                    */
/*****************************************************************************/
Sint32 amObjectLoad(NNS_OBJECT **object, NNS_TEXLIST **texlist, void **texlistbuf, void *buf, NNF_DRAWOBJ drawflag, char *filepath, AMS_AMB_HEADER *amb)
{
	NNS_OBJECT		*obj_file;
	NNS_TEXFILELIST	*texfilelist;
	Sint32			num, reg_index;

	amObjectSetup(&obj_file, &texfilelist, buf);

	num		= texfilelist->nTex;
	*texlistbuf		= amMemAlloc((Uint32)nnEstimateTexlistSize(num));
	nnSetUpTexlist(texlist, num, *texlistbuf);

	reg_index	= amObjectLoad(object, obj_file, drawflag);
	if ((filepath != NULL) || (amb != NULL))
		reg_index	= amTextureLoad(*texlist, texfilelist, filepath, amb);

	return	reg_index;
}

/*****************************************************************************/
/* Sint32 amObjectLoad(NNS_OBJECT **object, NNS_TEXFILELIST* txbfilelist,    */
/*                      NNS_TEXLIST **texlist, void **texlistbuf, void *buf, */
/*                NNF_DRAWOBJ drawflag, char *filepath, AMS_AMB_HEADER *amb) */
/*---------------------------------------------------------------------------*/
/* [INPUT]   txbfilelist : amTxbから取得したtexfilelist                      */
/*           buf         : オブジェクトファイル                              */
/*           drawflag    : 描画フラグ                                        */
/*           filepath    : テクスチャファイルパス                            */
/*           amb         : テクスチャファイルが入ったAMBファイルへのポインタ */
/* [OUTPUT]  object      : オブジェクトへのポインタ(要free)                  */
/*           texlist     : テクスチャリストへのポインタ                      */
/*           texlistbuf  : テクスチャリストバッファ(要free)                  */
/* [RETURN]  登録完了をチェックするための登録ID                              */
/* [FUNCTION]  オブジェクト・シェーダーとテクスチャの登録                    */
/*             テクスチャは txbfilelist で指定したものを使用する             */
/*             オブジェクトが本来指定するテクスチャファイルとは              */
/*　           違うテクスチャを設定することになるので要注意                  */
/*****************************************************************************/
Sint32 amObjectLoad(NNS_OBJECT **object, NNS_TEXFILELIST* txbfilelist, NNS_TEXLIST **texlist,
					void **texlistbuf, void *buf, NNF_DRAWOBJ drawflag,
					char *filepath, AMS_AMB_HEADER *amb)
{
	NNS_OBJECT		*obj_file;
	NNS_TEXFILELIST	*texfilelist;
	Sint32			num, reg_index;

	amObjectSetup(&obj_file, &texfilelist, buf);

	num		= txbfilelist->nTex;
	*texlistbuf		= amMemAlloc((Uint32)nnEstimateTexlistSize(num));
	nnSetUpTexlist(texlist, num, *texlistbuf);

	reg_index	= amObjectLoad(object, obj_file, drawflag);
	if ((filepath != NULL) || (amb != NULL))
		reg_index	= amTextureLoad(*texlist, txbfilelist, filepath, amb);

	return	reg_index;
}


/*****************************************************************************/
/* Sint32 amObjectLoadShader(NNS_OBJECT **object, NNF_DRAWOBJ drawflag)      */
/*---------------------------------------------------------------------------*/
/* [INPUT]   object      : オブジェクトへのポインタ                          */
/*           drawflag    : 描画フラグ                                        */
/* [RETURN]  登録完了をチェックするための登録ID                              */
/* [FUNCTION]  オブジェクトのシェーダーの登録                                */
/*****************************************************************************/
Sint32 amObjectLoadShader(NNS_OBJECT **object, NNF_DRAWOBJ drawflag)
{
	AMS_PARAM_LOAD_SHADER_OBJECT	param;
	NNF_DRAWOBJ		*list;

	list	= (NNF_DRAWOBJ *)amDrawMallocDataBuffer(sizeof(NNF_DRAWOBJ));
	list[0]	= drawflag;

	param.obj		= object;
	param.flag_num	= 1;
	param.drawflag	= list;

	return	amDrawRegistCommand(AMD_REGIST_LOAD_SHADER_OBJECT, &param);
}


/*****************************************************************************/
/* Sint32 amObjectLoadShader(NNS_OBJECT **object,                            */
/*                                        Sint32 num, NNF_DRAWOBJ *drawflag) */
/*---------------------------------------------------------------------------*/
/* [INPUT]   object      : オブジェクトへのポインタ                          */
/*           num         : 描画フラグ数                                      */
/*           drawflag    : 描画フラグリストへのポインタ                      */
/* [RETURN]  登録完了をチェックするための登録ID                              */
/* [FUNCTION]  オブジェクトのシェーダーの登録                                */
/*****************************************************************************/
Sint32 amObjectLoadShader(NNS_OBJECT **object, Sint32 num, NNF_DRAWOBJ *drawflag)
{
	AMS_PARAM_LOAD_SHADER_OBJECT	param;
	NNF_DRAWOBJ		*list;

	list	= (NNF_DRAWOBJ *)amDrawMallocDataBuffer(sizeof(NNF_DRAWOBJ) * num);
	memcpy(list, drawflag, sizeof(NNF_DRAWOBJ) * num);

	param.obj		= object;
	param.flag_num	= num;
	param.drawflag	= list;

	return	amDrawRegistCommand(AMD_REGIST_LOAD_SHADER_OBJECT, &param);
}


/*****************************************************************************/
/* Sint32 amObjectRelease(NNS_OBJECT *object)                                */
/*---------------------------------------------------------------------------*/
/* [INPUT]   object : オブジェクトへのポインタ                               */
/* [RETURN]  解放完了をチェックするための登録ID                              */
/* [FUNCTION]  オブジェクトの解放                                            */
/*****************************************************************************/
Sint32 amObjectRelease(NNS_OBJECT *object)
{
	AMS_PARAM_DELETE_VERTEX_OBJECT	*param;
	param	= (AMS_PARAM_DELETE_VERTEX_OBJECT *)amDrawMallocDataBuffer(
			sizeof(AMS_PARAM_DELETE_VERTEX_OBJECT));
	param->obj		= object;
	return	amDrawRegistCommand(AMD_REGIST_DELETE_VERTEX_OBJECT, param);
}


/*****************************************************************************/
/* Sint32 amObjectRelease(NNS_OBJECT *object, NNS_TEXLIST *texlist)          */
/*---------------------------------------------------------------------------*/
/* [INPUT] object  : オブジェクトへのポインタ                                */
/*         texlist : テクスチャリストへのポインタ                            */
/* [RETURN]  解放完了をチェックするための登録ID                              */
/* [FUNCTION]  オブジェクトとテクスチャの解放                                */
/*****************************************************************************/
Sint32 amObjectRelease(NNS_OBJECT *object, NNS_TEXLIST *texlist)
{
	amObjectRelease(object);
	return	amTextureRelease(texlist);
}


/*--- Local Functions -------------------------------------------------------*/

void _amObjectResolvePointer(NNS_BINCNK_DATAHEADER *data, NNS_BINCNK_NOF0HEADER *nof0)
{
	Uint32		*src, *dst;
	Uint32		base, offset;
	Sint32		i;

	i		= nof0->nData;
	dst		= (Uint32 *)data;
	src		= (Uint32 *)(nof0 + 1);
	base	= (Uint32)data;

	for (; i > 0; i--, src++) {
		offset	= *src >> 2;
		dst[offset]		+= base;
	}

	// 二重解決の防止
	nof0->nData	= 0;
}

