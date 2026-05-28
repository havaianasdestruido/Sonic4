/*****************************************************************************/
/*      amObject.h                  Author : Takashi Nakano                  */
/*            Copyright(c) 2009 Dimps CORP. All Rights Reserved.             */
/*---------------------------------------------------------------------------*/
/* オブジェクトライブラリヘッダ                                              */
/*---------------------------------------------------------------------------*/
/* Date          Ver    Comment                                              */
/* 090401-       0.01   first version                                        */
/*****************************************************************************/

#ifndef _AM_OBJECT_H
#define _AM_OBJECT_H

/*--- Include Files (Pre Definitions) ---------------------------------------*/

/*--- Definitions -----------------------------------------------------------*/

/*--- Macros ----------------------------------------------------------------*/

/*--- Include Files (Post Definitions) --------------------------------------*/

/*--- External Valiables ----------------------------------------------------*/

/*--- External Functions ----------------------------------------------------*/

/*****************************************************************************/
/* void amObjectSetup(NNS_OBJECT **object, NNS_TEXFILELIST **texfilelist,    */
/*                                                                void *buf) */
/*---------------------------------------------------------------------------*/
/* [INPUT]   buf         : ファイル                                          */
/* [OUTPUT]  object      : オブジェクトへのポインタ                          */
/*           texfilelist : テクスチャファイルリストへのポインタ              */
/* [FUNCTION]  オブジェクトとテクスチャファイルリストの取得                  */
/*****************************************************************************/
void amObjectSetup(NNS_OBJECT **object, NNS_TEXFILELIST **texfilelist, void *buf);

/*****************************************************************************/
/* Sint32 amObjectLoad(NNS_OBJECT **object, NNS_OBJECT *obj_file,            */
/*                                                     NNF_DRAWOBJ drawflag) */
/*---------------------------------------------------------------------------*/
/* [INPUT]   obj_file    : オブジェクトへのポインタ(amObjectSetupで取得)     */
/*           drawflag    : 描画フラグ                                        */
/* [OUTPUT]  object      : オブジェクトへのポインタ(要free)                  */
/* [RETURN]  登録完了をチェックするための登録ID                              */
/* [FUNCTION]  オブジェクト・シェーダーの登録                                */
/*****************************************************************************/
Sint32 amObjectLoad(NNS_OBJECT **object, NNS_OBJECT *obj_file,
		NNF_DRAWOBJ drawflag = 0);

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
Sint32 amObjectLoad(NNS_OBJECT **object, NNS_TEXLIST **texlist,
		void **texlistbuf, void *buf, NNF_DRAWOBJ drawflag,
		char *filepath, AMS_AMB_HEADER *amb = NULL);

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
					char *filepath, AMS_AMB_HEADER *amb=NULL);

/*****************************************************************************/
/* Sint32 amObjectLoadShader(NNS_OBJECT **object, NNF_DRAWOBJ drawflag)      */
/*---------------------------------------------------------------------------*/
/* [INPUT]   object      : オブジェクトへのポインタ                          */
/*           drawflag    : 描画フラグ                                        */
/* [RETURN]  登録完了をチェックするための登録ID                              */
/* [FUNCTION]  オブジェクトのシェーダーの登録                                */
/*****************************************************************************/
Sint32 amObjectLoadShader(NNS_OBJECT **object, NNF_DRAWOBJ drawflag);

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
Sint32 amObjectLoadShader(NNS_OBJECT **object, Sint32 num, NNF_DRAWOBJ *drawflag);

/*****************************************************************************/
/* Sint32 amObjectRelease(NNS_OBJECT *object)                                */
/*---------------------------------------------------------------------------*/
/* [INPUT]   object : オブジェクトへのポインタ                               */
/* [RETURN]  解放完了をチェックするための登録ID                              */
/* [FUNCTION]  オブジェクトの解放                                            */
/*****************************************************************************/
Sint32 amObjectRelease(NNS_OBJECT *object);

/*****************************************************************************/
/* Sint32 amObjectRelease(NNS_OBJECT *object, NNS_TEXLIST *texlist)          */
/*---------------------------------------------------------------------------*/
/* [INPUT] object  : オブジェクトへのポインタ                                */
/*         texlist : テクスチャリストへのポインタ                            */
/* [RETURN]  解放完了をチェックするための登録ID                              */
/* [FUNCTION]  オブジェクトとテクスチャの解放                                */
/*****************************************************************************/
Sint32 amObjectRelease(NNS_OBJECT *object, NNS_TEXLIST *texlist);

#endif
