/*****************************************************************************/
/*      amTexture.h                 Author : Takashi Nakano                  */
/*            Copyright(c) 2009 Dimps CORP. All Rights Reserved.             */
/*---------------------------------------------------------------------------*/
/* テクスチャライブラリヘッダ                                                */
/*---------------------------------------------------------------------------*/
/* Date          Ver    Comment                                              */
/* 090401-       0.01   first version                                        */
/*****************************************************************************/

#ifndef _AM_TEXTURE_H
#define _AM_TEXTURE_H

/*--- Include Files (Pre Definitions) ---------------------------------------*/

/*--- Definitions -----------------------------------------------------------*/

/*--- Macros ----------------------------------------------------------------*/

/*--- Include Files (Post Definitions) --------------------------------------*/

/*--- External Valiables ----------------------------------------------------*/

/*--- External Functions ----------------------------------------------------*/

/*****************************************************************************/
/* Sint32 amTextureLoad(NNS_TEXLIST *texlist, NNS_TEXFILELIST *texfilelist,  */
/*                                      char *filepath, AMS_AMB_HEADER *amb) */
/*---------------------------------------------------------------------------*/
/* [INPUT] texlist     : テクスチャリストへのポインタ                        */
/*         texfilelist : テクスチャファイル定義リストへのポインタ            */
/*         filepath    : テクスチャファイルパス                              */
/*         amb         : テクスチャファイルが入ったAMBファイルへのポインタ   */
/* [RETURN]  登録完了をチェックするための登録ID                              */
/* [FUNCTION]  テクスチャの登録                                              */
/*****************************************************************************/
Sint32 amTextureLoad(NNS_TEXLIST *texlist, NNS_TEXFILELIST *texfilelist,
		char *filepath, AMS_AMB_HEADER *amb = NULL);

/*****************************************************************************/
/* Sint32 amTextureLoad(NNS_TEXINFO *texinfo, NNS_TEXFILE *texfile,          */
/*                               char *filepath, void *texbuf, Sint32 size)  */
/*---------------------------------------------------------------------------*/
/* [INPUT] texinfo : テクスチャ情報へのポインタ                              */
/*         texfile : テクスチャファイル定義へのポインタ                      */
/*         filepath : テクスチャファイルパス                                 */
/*         texbuf  : テクスチャファイルへのポインタ                          */
/*         size    : ファイルサイズ                                          */
/* [RETURN]  登録完了をチェックするための登録ID                              */
/* [FUNCTION]  テクスチャの登録                                              */
/*****************************************************************************/
Sint32 amTextureLoad(NNS_TEXINFO *texinfo, NNS_TEXFILE *texfile,
		char *filepath, void *texbuf = NULL, Sint32 size = 0);

/*****************************************************************************/
/* Sint32 amTextureLoad(void **texture, void *image, Sint32 size,            */
/*                         Sint32 minfilter, Sint32 magfilter,               */
/*                               Sint32 u_wrap, Sint32 v_wrap, void *gvrobj) */
/*---------------------------------------------------------------------------*/
/* [INPUT] image       : テクスチャイメージ                                  */
/*         size        : テクスチャイメージデータサイズ                      */
/*         minfilter   : 縮小フィルタ                                        */
/*         magfilter   : 拡大フィルタ                                        */
/*         u_wrap      : U方向テクスチャラップモード                         */
/*         v_wrap      : V方向テクスチャラップモード                         */
/*         gvrobj      : GVRオブジェクト(Wiiのみ)                            */
/* [OUTPUT] texture    : テクスチャへのポインタ                              */
/* [RETURN]  登録完了をチェックするための登録ID                              */
/* [FUNCTION]  テクスチャの登録                                              */
/*****************************************************************************/
Sint32 amTextureLoad(void **texture, void *image, Sint32 size,
		Sint32 minfilter, Sint32 magfilter,
		Sint32 u_wrap = AMD_WRAP_CLAMP, Sint32 v_wrap = AMD_WRAP_CLAMP,
		void *gvrobj = NULL);

/*****************************************************************************/
/* Sint32 amTextureRelease(NNS_TEXLIST *texlist)                             */
/*---------------------------------------------------------------------------*/
/* [INPUT] texlist     : テクスチャリストへのポインタ                        */
/* [RETURN]  解放完了をチェックするための登録ID                              */
/* [FUNCTION]  テクスチャの解放                                              */
/*****************************************************************************/
Sint32 amTextureRelease(NNS_TEXLIST *texlist);

/*****************************************************************************/
/* Sint32 amTextureRelease(void *texture)                                    */
/*---------------------------------------------------------------------------*/
/* [INPUT] texture     : テクスチャへのポインタ                              */
/* [RETURN]  解放完了をチェックするための登録ID                              */
/* [FUNCTION]  テクスチャの解放                                              */
/*****************************************************************************/
Sint32 amTextureRelease(void *texture);

/*****************************************************************************/
/* Sint32 amTextureIsComplete(Sint32 index)                                  */
/*---------------------------------------------------------------------------*/
/* [INPUT] index : 登録インデックス                                          */
/* [RETURN]  完了していたら1 していなかったら0                               */
/* [FUNCTION]  登録したコマンドの実行完了チェック                            */
/*****************************************************************************/
Sint32 amTextureIsComplete(Sint32 index);


#endif
