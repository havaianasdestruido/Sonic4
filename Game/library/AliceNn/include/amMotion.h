/*****************************************************************************/
/*      amMotion.h                  Author : Takashi Nakano                  */
/*            Copyright(c) 2009 Dimps CORP. All Rights Reserved.             */
/*---------------------------------------------------------------------------*/
/* モーションライブラリヘッダ                                                */
/*---------------------------------------------------------------------------*/
/* Date          Ver    Comment                                              */
/* 090409-       0.01   first version                                        */
/*****************************************************************************/

#ifndef _AM_MOTION_H
#define _AM_MOTION_H

/*--- Include Files (Pre Definitions) ---------------------------------------*/

/*--- Definitions -----------------------------------------------------------*/

#define AMD_MOTION_FILE_MAX			(4)		// 最大登録モーションファイル数
#define AMD_MOTION_DEFAULT_MAX			(64)	// 最大合計モーション数
#define AMD_MOTION_MATERIAL_DEFAULT_MAX	(16)	// 最大合計マテリアルモーション数

// モーションバッファ
typedef struct {
	Sint32		motion_id;			// モーションID
	float		frame;				// フレーム
	NNS_TRS		*mbuf;				// モーションバッファ
} AMS_MOTION_BUF;				// 12bytes

// モーションファイル
typedef struct {
	void		*file;				// モーションファイル
	Sint32		motion_num;			// モーション数
	NNS_MOTION	**motion;			// モーション先頭アドレス
} AMS_MOTION_FILE;				// 12bytes

// モーション管理
typedef struct {
	NNS_OBJECT	*object;			// オブジェクト
	Sint32		node_num;			// ノード数

	AMS_MOTION_FILE		mtnfile[AMD_MOTION_FILE_MAX];
	Sint32		motion_num;			// 最大モーション数
	NNS_MOTION	**mtnbuf;

	NNS_TRS		*data;				// 最終モーション

	NNS_TRS		*mmbuf;				// モーション中間バッファ
	AMS_MOTION_BUF	mbuf[2];		// モーションバッファ

	NNS_MATMOTOBJ	*mmobject;		// マテリアルモーションオブジェクト
	Uint32		mmobj_size;			// マテリアルモーションオブジェクトサイズ
	Sint32		mmotion_num;		// マテリアルモーション最大数
	NNS_MOTION	**mmtn;				// マテリアルモーション
	Sint32		mmotion_id;			// マテリアルモーションID
	float		mmotion_frame;		// マテリアルモーションフレーム

	Sint32		reserved[2];
} AMS_MOTION;


/*--- Macros ----------------------------------------------------------------*/

/*--- Include Files (Post Definitions) --------------------------------------*/

/*--- External Valiables ----------------------------------------------------*/

/*--- External Functions ----------------------------------------------------*/

/*****************************************************************************/
/* Sint32 amMotionId(Sint32 file_id, Sint32 motion_id)                       */
/*---------------------------------------------------------------------------*/
/* [INPUT]   file_id   : モーションファイルID                                */
/*           motion_id : モーションID                                        */
/* [FUNCTION]  モーションIDの取得                                            */
/*****************************************************************************/
inline Sint32 amMotionId(Sint32 file_id, Sint32 motion_id)
{
	return	((file_id << 16) | motion_id);
}

/*****************************************************************************/
/* AMS_MOTION *amMotionCreate(NNS_OBJECT *object, Sint32 flag)               */
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
AMS_MOTION *amMotionCreate(NNS_OBJECT *object, Sint32 flag = 0);
AMS_MOTION *amMotionCreate(NNS_OBJECT *object,
		Sint32 motion_num, Sint32 mmotion_num, Sint32 flag = 0);

#define AMD_MOTION_CREATE_FLAG_MARGE	(0x0001)		// 並列補間を使用する


/*****************************************************************************/
/* void amMotionDelete(AMS_MOTION *motion)                                   */
/*---------------------------------------------------------------------------*/
/* [INPUT]   motion : モーション管理                                         */
/* [FUNCTION]  モーション構造体の削除                                        */
/*****************************************************************************/
void amMotionDelete(AMS_MOTION *motion);

/*****************************************************************************/
/* void amMotionRegistFile(AMS_MOTION *motion, Sint32 file_id,               */
/*                                                   AMS_AMB_HEADER *amb)    */
/* void amMotionRegistFile(AMS_MOTION *motion, Sint32 file_id, void *buf)    */
/*---------------------------------------------------------------------------*/
/* [INPUT]   motion  : モーション管理                                        */
/*           file_id : ファイルID                                            */
/*           amb     : AMBファイル                                           */
/*           buf     : ファイル                                              */
/* [FUNCTION]  モーションファイルの登録                                      */
/*****************************************************************************/
void amMotionRegistFile(AMS_MOTION *motion, Sint32 file_id, AMS_AMB_HEADER *amb);
void amMotionRegistFile(AMS_MOTION *motion, Sint32 file_id, void *buf);

/*****************************************************************************/
/* Sint32 amMotionSetup(NNS_MOTION **motion, AMS_AMB_HEADER *amb)            */
/* Sint32 amMotionSetup(NNS_MOTION **motion, void *buf)                      */
/*---------------------------------------------------------------------------*/
/* [INPUT]   amb    : AMBファイル                                            */
/*           buf    : ファイル                                               */
/* [OUTPUT]  motion : モーションへのポインタ                                 */
/* [RETURN]  モーション数                                                    */
/* [FUNCTION]  モーションの取得                                              */
/*****************************************************************************/
Sint32 amMotionSetup(NNS_MOTION **motion, AMS_AMB_HEADER *amb);
Sint32 amMotionSetup(NNS_MOTION **motion, void *buf);

/*****************************************************************************/
/* void amMotionSet(AMS_MOTION *motion, Sint32 mbuf_id, Sint32 motion_id)    */
/*---------------------------------------------------------------------------*/
/* [INPUT]   motion    : モーション管理                                      */
/*           mbuf_id   : モーションバッファID                                */
/*           motion_id : モーションID                                        */
/* [FUNCTION]  モーションの設定                                              */
/*****************************************************************************/
void amMotionSet(AMS_MOTION *motion, Sint32 mbuf_id, Sint32 motion_id);

/*****************************************************************************/
/* void amMotionSetFrame(AMS_MOTION *motion, Sint32 mbuf_id, float frame)    */
/*---------------------------------------------------------------------------*/
/* [INPUT]   motion    : モーション管理                                      */
/*           mbuf_id   : モーションバッファID                                */
/*           frame     : モーションフレーム                                  */
/* [FUNCTION]  モーションフレームの設定                                      */
/*****************************************************************************/
void amMotionSetFrame(AMS_MOTION *motion, Sint32 mbuf_id, float frame);

/*****************************************************************************/
/* void amMotionCalc(AMS_MOTION *motion, Sint32 mbuf_id)                     */
/*---------------------------------------------------------------------------*/
/* [INPUT]   motion    : モーション管理                                      */
/*           mbuf_id   : モーションバッファID                                */
/* [FUNCTION]  モーションの計算                                              */
/*****************************************************************************/
void amMotionCalc(AMS_MOTION *motion, Sint32 mbuf_id = -1);

/*****************************************************************************/
/* void amMotionApply(AMS_MOTION *motion, float marge, float per)            */
/*---------------------------------------------------------------------------*/
/* [INPUT]   motion    : モーション管理                                      */
/*           marge     : 並列補間率                                          */
/*           per       : 直列補間率                                          */
/* [FUNCTION]  最終モーションの計算                                          */
/*****************************************************************************/
void amMotionApply(AMS_MOTION *motion, float marge = 0.0f, float per = 1.0f);

/*****************************************************************************/
/* void amMotionGet(AMS_MOTION *motion, float marge, float per)              */
/*---------------------------------------------------------------------------*/
/* [INPUT]   motion    : モーション管理                                      */
/*           marge     : 並列補間率                                          */
/*           per       : 直列補間率                                          */
/* [FUNCTION]  最終モーションの計算(Calc + Apply)                            */
/*****************************************************************************/
void amMotionGet(AMS_MOTION *motion, float marge = 0.0f, float per = 1.0f);

/*****************************************************************************/
/* void amMotionMaterialRegistFile(AMS_MOTION *motion,                       */
/*                                      Sint32 file_id, AMS_AMB_HEADER *amb) */
/* void amMotionMaterialRegistFile(AMS_MOTION *motion,                       */
/*                                               Sint32 file_id, void *file) */
/*---------------------------------------------------------------------------*/
/* [INPUT]   motion  : モーション管理                                        */
/*           file_id : ファイルID(0のみ)                                     */
/*           amb     : マテリアルモーションファイル                          */
/*           file    : マテリアルモーションファイル                          */
/* [FUNCTION]  マテリアルモーションファイルの登録                            */
/*****************************************************************************/
void amMotionMaterialRegistFile(AMS_MOTION *motion, Sint32 file_id, AMS_AMB_HEADER *amb);
void amMotionMaterialRegistFile(AMS_MOTION *motion, Sint32 file_id, void *file);

/*****************************************************************************/
/* void amMotionMaterialSet(AMS_MOTION *motion, Sint32 motion_id)            */
/*---------------------------------------------------------------------------*/
/* [INPUT]   motion    : モーション管理                                      */
/*           motion_id : モーションID                                        */
/* [FUNCTION]  マテリアルモーションの設定                                    */
/*****************************************************************************/
void amMotionMaterialSet(AMS_MOTION *motion, Sint32 motion_id);

/*****************************************************************************/
/* void amMotionMaterialSetFrame(AMS_MOTION *motion, float frame)            */
/*---------------------------------------------------------------------------*/
/* [INPUT]   motion    : モーション管理                                      */
/*           frame     : モーションフレーム                                  */
/* [FUNCTION]  マテリアルモーションフレームの設定                            */
/*****************************************************************************/
void amMotionMaterialSetFrame(AMS_MOTION *motion, float frame);

/*****************************************************************************/
/* void amMotionMaterialCalc(AMS_MOTION *motion)                             */
/*---------------------------------------------------------------------------*/
/* [INPUT]   motion    : モーション管理                                      */
/* [FUNCTION]  マテリアルモーションの計算                                    */
/*****************************************************************************/
void amMotionMaterialCalc(AMS_MOTION *motion);

/*****************************************************************************/
/* void amMotionDraw(Uint32 state, AMS_MOTION *motion,                       */
/*                               NNS_TEXLIST *texlist, NNF_DRAWOBJ drawflag, */
/*                                           NNS_MATERIALCALLBACK_FUNC func) */
/* void amMotionDraw(AMS_MOTION *motion,                                     */
/*                               NNS_TEXLIST *texlist, NNF_DRAWOBJ drawflag, */
/*                                           NNS_MATERIALCALLBACK_FUNC func) */
/*---------------------------------------------------------------------------*/
/* [INPUT] state    : 描画ステート(省略すると描画スレッド用になる)           */
/*         motion   : モーション管理                                         */
/*         texlist  : テクスチャリスト                                       */
/*         drawflag : 描画フラグ                                             */
/*         func     : マテリアルコールバック関数                             */
/* [FUNCTION]  モーションオブジェクトの描画                                  */
/*****************************************************************************/
void amMotionDraw(Uint32 state, AMS_MOTION *motion,
		NNS_TEXLIST *texlist, NNF_DRAWOBJ drawflag = 0,
		NNS_MATERIALCALLBACK_FUNC func = NULL);
void amMotionDraw(AMS_MOTION *motion,
		NNS_TEXLIST *texlist, NNF_DRAWOBJ drawflag = 0,
		NNS_MATERIALCALLBACK_FUNC func = NULL);

/*****************************************************************************/
/* void amMotionMaterialDraw(Uint32 state, AMS_MOTION *motion,               */
/*                               NNS_TEXLIST *texlist, NNF_DRAWOBJ drawflag, */
/*                                           NNS_MATERIALCALLBACK_FUNC func) */
/* void amMotionMaterialDraw(AMS_MOTION *motion,                             */
/*                               NNS_TEXLIST *texlist, NNF_DRAWOBJ drawflag, */
/*                                           NNS_MATERIALCALLBACK_FUNC func) */
/*---------------------------------------------------------------------------*/
/* [INPUT] state    : 描画ステート(省略すると描画スレッド用になる)           */
/*         motion   : モーション管理                                         */
/*         texlist  : テクスチャリスト                                       */
/*         drawflag : 描画フラグ                                             */
/*         func     : マテリアルコールバック関数                             */
/* [FUNCTION]  マテリアルモーションオブジェクトの描画                        */
/*****************************************************************************/
void amMotionMaterialDraw(Uint32 state, AMS_MOTION *motion,
		NNS_TEXLIST *texlist, NNF_DRAWOBJ drawflag = 0,
		NNS_MATERIALCALLBACK_FUNC func = NULL);
void amMotionMaterialDraw(AMS_MOTION *motion,
        NNS_TEXLIST *texlist, NNF_DRAWOBJ drawflag = 0,
		NNS_MATERIALCALLBACK_FUNC func = NULL);

/*****************************************************************************/
/* float amMotionGetStartFrame(AMS_MOTION *motion, Sint32 motion_id)         */
/*---------------------------------------------------------------------------*/
/* [INPUT]   motion    : モーション管理                                      */
/*           motion_id : モーションID                                        */
/* [RETURN]  モーション開始フレーム                                          */
/* [FUNCTION]  モーション開始フレームの取得                                  */
/*****************************************************************************/
float amMotionGetStartFrame(AMS_MOTION *motion, Sint32 motion_id);

/*****************************************************************************/
/* float amMotionGetEndFrame(AMS_MOTION *motion, Sint32 motion_id)           */
/*---------------------------------------------------------------------------*/
/* [INPUT]   motion    : モーション管理                                      */
/*           motion_id : モーションID                                        */
/* [RETURN]  モーション終了フレーム                                          */
/* [FUNCTION]  モーション終了フレームの取得                                  */
/*****************************************************************************/
float amMotionGetEndFrame(AMS_MOTION *motion, Sint32 motion_id);

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
void amMotionGetFrames(AMS_MOTION *motion,
		Sint32 motion_id, float *start, float *end);

/*****************************************************************************/
/* float amMotionMaterialGetStartFrame(AMS_MOTION *motion, Sint32 motion_id) */
/*---------------------------------------------------------------------------*/
/* [INPUT]   motion    : モーション管理                                      */
/*           motion_id : モーションID(0のみ)                                 */
/* [RETURN]  モーション開始フレーム                                          */
/* [FUNCTION]  マテリアルモーション開始フレームの取得                        */
/*****************************************************************************/
float amMotionMaterialGetStartFrame(AMS_MOTION *motion, Sint32 motion_id);

/*****************************************************************************/
/* float amMotionMaterialGetEndFrame(AMS_MOTION *motion, Sint32 motion_id)   */
/*---------------------------------------------------------------------------*/
/* [INPUT]   motion    : モーション管理                                      */
/*           motion_id : モーションID(0のみ)                                 */
/* [RETURN]  モーション終了フレーム                                          */
/* [FUNCTION]  マテリアルモーション終了フレームの取得                        */
/*****************************************************************************/
float amMotionMaterialGetEndFrame(AMS_MOTION *motion, Sint32 motion_id);

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
void amMotionMaterialGetFrames(AMS_MOTION *motion,
		Sint32 motion_id, float *start, float *end);

#endif
