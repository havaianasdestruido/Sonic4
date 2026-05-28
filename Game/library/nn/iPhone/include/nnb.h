/*---------------------------------------------------------------------------

    NN binary data library for sample

    Copyright (C) 2002 - 2009 SEGA Corporation CS R&D Support dept.
    All Rights Reserved.

    Module  : NN binary data library for sample
    File    : nnb.h
    Create  : 2002/06/04
    Modify  : 2003/03/28
    Modify  : 2003/12/02 Xbox用のチャンク名を追加(NXSF, NXSI)
    Modify  : 2004/02/26 ノードツリーオブジェクト、メッシュオブジェクトチャンク名を追加
    Modify  : 2004/08/06 Lindberg用のチャンク名を追加
    Modify  : 2004/08/06 NND_CHUNK_EFFECT_LIST_XBの追加
    Modify  : 2004/01/26 NNS_BINCNK_DATAHEADERのメンバPadをVersionに変更
    Modify  : 2005/03/10 DT06用のチャンク名を追加
    Modify  : 2005/03/15 DT06用のNOF0チャンク名を修正
    Modify  : 2005/03/30 ファイル名を格納するNFN0チャンクを追加(全機種共通)
    Modify  : 2005/04/01 PSP用のチャンク名を追加
    Modify  : 2006/03/15 PLAYSTATION3用のチャンク名を追加
    Modify  : 2008/05/21 DXG20用のチャンク名を追加
    Modify  : 2009/05/13 DT06削除
    Modify  : 2009/05/13 NND_PLATFORM_XENON削除
    Version : 1.18.34
    Note    :

---------------------------------------------------------------------------*/

#ifndef __NNB_H__
#define __NNB_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef struct {
	Uint32	Id;				// バイナリデータ識別文字
	Sint32	OfsNextId;		// 次の識別文字までのオフセット（バイト数）
	Sint32	nChunk;			// バイナリデータ数
	Sint32	OfsData;		// バイナリデータへのオフセット（バイト数）
	Sint32	SizeData;		// バイナリデータのサイズ（バイト数）
	Sint32	OfsNOF0;		// アドレス解決識別文字までのオフセット（バイト数）
	Sint32	SizeNOF0;		// アドレス解決識別文字のサイズ（バイト数）
	Sint32	Version;		// データバージョン
} NNS_BINCNK_FILEHEADER;

typedef struct {
	Uint32	Id;				// バイナリデータ識別文字
	Sint32	OfsNextId;		// 次の識別文字までのオフセット（バイト数）
	Sint32	OfsMainData;	// メイン構造体へのオフセット（バイト数）
	Sint32	Version;		// データバージョン
} NNS_BINCNK_DATAHEADER;

typedef struct {
	Uint32	Id;				// バイナリデータ識別文字
	Sint32	OfsNextId;		// 次の識別文字までのオフセット（バイト数）
	Sint32	nData;			// 解決するアドレスデータ数
	Sint32	Pad;			// パディング
} NNS_BINCNK_NOF0HEADER;

#if ( NND_PLATFORM == NND_PLATFORM_GC || NND_PLATFORM == NND_PLATFORM_PS3 || (NND_PLATFORM == NND_PLATFORM_DXG20 && defined(_XBOX)))
#define	NNM_CHUNK_ID( a, b, c, d )	((Uint32)(a) << 24 |	\
									 (Uint32)(b) << 16 |	\
									 (Uint32)(c) <<  8 |	\
									 (Uint32)(d))
#else
#define	NNM_CHUNK_ID( a, b, c, d )	((Uint32)(d) << 24 |	\
									 (Uint32)(c) << 16 |	\
									 (Uint32)(b) <<  8 |	\
									 (Uint32)(a))
#endif

/* PlayStation2 chunk name */
#define	NND_CHUNK_HEADER_PS2			NNM_CHUNK_ID( 'N','S','I','F' )	// バイナリファイル情報ヘッダ		(NSIF)
#define	NND_CHUNK_TEXTURE_PS2			NNM_CHUNK_ID( 'N','S','T','L' )	// テクスチャリスト情報ヘッダ		(NSTL)
#define	NND_CHUNK_OBJECT_PS2			NNM_CHUNK_ID( 'N','S','O','B' )	// オブジェクト情報ヘッダ			(NSOB)
#define	NND_CHUNK_MOTION_PS2			NNM_CHUNK_ID( 'N','S','M','O' )	// ノードモーション情報ヘッダ		(NSMO)
#define	NND_CHUNK_MORPH_MOTION_PS2		NNM_CHUNK_ID( 'N','S','M','M' )	// モーフモーション情報ヘッダ		(NSMM)
#define	NND_CHUNK_CAMERA_MOTION_PS2		NNM_CHUNK_ID( 'N','S','M','C' )	// カメラモーション情報ヘッダ		(NSMC)
#define	NND_CHUNK_LIGHT_MOTION_PS2		NNM_CHUNK_ID( 'N','S','M','L' )	// ライトモーション情報ヘッダ		(NSML)
#define	NND_CHUNK_MATERIAL_MOTION_PS2	NNM_CHUNK_ID( 'N','S','M','A' )	// マテリアルモーション情報ヘッダ	(NSMA)
#define	NND_CHUNK_LIGHT_PS2				NNM_CHUNK_ID( 'N','S','L','I' )	// ライト情報ヘッダ					(NSLI)
#define	NND_CHUNK_CAMERA_PS2			NNM_CHUNK_ID( 'N','S','C','A' )	// カメラ情報ヘッダ					(NSCA)
#define	NND_CHUNK_MORPH_PS2				NNM_CHUNK_ID( 'N','S','M','T' )	// モーフターゲット情報ヘッダ		(NSMT)
#define	NND_CHUNK_NODE_NAME_PS2			NNM_CHUNK_ID( 'N','S','N','N' )	// ノードネームリスト情報ヘッダ		(NSNN)
#define	NND_CHUNK_NODE_TREE_OBJECT_PS2	NNM_CHUNK_ID( 'N','S','N','T' )	// ノードツリーオブジェクト情報ヘッダ(NSNN)
#define	NND_CHUNK_MESH_OBJECT_PS2		NNM_CHUNK_ID( 'N','S','M','E' )	// メッシュオブジェクト情報ヘッダ	(NSME)
#define	NND_CHUNK_NOF0_PS2				NNM_CHUNK_ID( 'N','O','F','0' )	// アドレス解決情報ヘッダ			(NOF0)
#define	NND_CHUNK_NEND_PS2				NNM_CHUNK_ID( 'N','E','N','D' )	// データ終了ヘッダ					(NEND)
#define	NND_CHUNK_NFN0_PS2				NNM_CHUNK_ID( 'N','F','N','0' )	// ファイル名情報ヘッダ				(NFN0)

/* GAMECUBE chunk name */

#define	NND_CHUNK_HEADER_GC				NNM_CHUNK_ID( 'N','G','I','F' )	// バイナリファイル情報ヘッダ		(NGIF)
#define	NND_CHUNK_TEXTURE_GC			NNM_CHUNK_ID( 'N','G','T','L' )	// テクスチャリスト情報ヘッダ		(NGTL)
#define	NND_CHUNK_OBJECT_GC				NNM_CHUNK_ID( 'N','G','O','B' )	// オブジェクト情報ヘッダ			(NGOB)
#define	NND_CHUNK_MOTION_GC				NNM_CHUNK_ID( 'N','G','M','O' )	// ノードモーション情報ヘッダ		(NGMO)
#define	NND_CHUNK_MORPH_MOTION_GC		NNM_CHUNK_ID( 'N','G','M','M' )	// モーフモーション情報ヘッダ		(NGMM)
#define	NND_CHUNK_CAMERA_MOTION_GC		NNM_CHUNK_ID( 'N','G','M','C' )	// カメラモーション情報ヘッダ		(NGMC)
#define	NND_CHUNK_LIGHT_MOTION_GC		NNM_CHUNK_ID( 'N','G','M','L' )	// ライトモーション情報ヘッダ		(NGML)
#define	NND_CHUNK_MATERIAL_MOTION_GC	NNM_CHUNK_ID( 'N','G','M','A' )	// マテリアルモーション情報ヘッダ	(NGMA)
#define	NND_CHUNK_LIGHT_GC				NNM_CHUNK_ID( 'N','G','L','I' )	// ライト情報ヘッダ					(NGLI)
#define	NND_CHUNK_CAMERA_GC				NNM_CHUNK_ID( 'N','G','C','A' )	// カメラ情報ヘッダ					(NGCA)
#define	NND_CHUNK_MORPH_GC				NNM_CHUNK_ID( 'N','G','M','T' )	// モーフターゲット情報ヘッダ		(NGMT)
#define	NND_CHUNK_NODE_NAME_GC			NNM_CHUNK_ID( 'N','G','N','N' )	// ノードネームリスト情報ヘッダ		(NGNN)
#define	NND_CHUNK_NODE_TREE_OBJECT_GC	NNM_CHUNK_ID( 'N','G','N','T' )	// ノードツリーオブジェクト情報ヘッダ(NGNT)
#define	NND_CHUNK_MESH_OBJECT_GC		NNM_CHUNK_ID( 'N','G','M','E' )	// メッシュオブジェクト情報ヘッダ	(NGME)
#define	NND_CHUNK_NOF0_GC				NNM_CHUNK_ID( 'N','O','F','0' )	// アドレス解決情報ヘッダ			(NOF0)
#define	NND_CHUNK_NEND_GC				NNM_CHUNK_ID( 'N','E','N','D' )	// データ終了ヘッダ					(NEND)
#define	NND_CHUNK_NFN0_GC				NNM_CHUNK_ID( 'N','F','N','0' )	// ファイル名情報ヘッダ				(NFN0)

/* Xbox chunk name */
#define	NND_CHUNK_HEADER_XB				NNM_CHUNK_ID( 'N','X','I','F' )	// バイナリファイル情報ヘッダ		(NXIF)
#define	NND_CHUNK_TEXTURE_XB			NNM_CHUNK_ID( 'N','X','T','L' )	// テクスチャリスト情報ヘッダ		(NXTL)
#define	NND_CHUNK_OBJECT_XB				NNM_CHUNK_ID( 'N','X','O','B' )	// オブジェクト情報ヘッダ			(NXOB)
#define	NND_CHUNK_MOTION_XB				NNM_CHUNK_ID( 'N','X','M','O' )	// ノードモーション情報ヘッダ		(NXMO)
#define	NND_CHUNK_MORPH_MOTION_XB		NNM_CHUNK_ID( 'N','X','M','M' )	// モーフモーション情報ヘッダ		(NXMM)
#define	NND_CHUNK_CAMERA_MOTION_XB		NNM_CHUNK_ID( 'N','X','M','C' )	// カメラモーション情報ヘッダ		(NXMC)
#define	NND_CHUNK_LIGHT_MOTION_XB		NNM_CHUNK_ID( 'N','X','M','L' )	// ライトモーション情報ヘッダ		(NXML)
#define	NND_CHUNK_MATERIAL_MOTION_XB	NNM_CHUNK_ID( 'N','X','M','A' )	// マテリアルモーション情報ヘッダ	(NXMA)
#define	NND_CHUNK_LIGHT_XB				NNM_CHUNK_ID( 'N','X','L','I' )	// ライト情報ヘッダ					(NXLI)
#define	NND_CHUNK_CAMERA_XB				NNM_CHUNK_ID( 'N','X','C','A' )	// カメラ情報ヘッダ					(NXCA)
#define	NND_CHUNK_MORPH_XB				NNM_CHUNK_ID( 'N','X','M','T' )	// モーフターゲット情報ヘッダ		(NXMT)
#define	NND_CHUNK_NODE_NAME_XB			NNM_CHUNK_ID( 'N','X','N','N' )	// ノードネームリスト情報ヘッダ		(NXNN)
#define	NND_CHUNK_NODE_TREE_OBJECT_XB	NNM_CHUNK_ID( 'N','X','N','T' )	// ノードツリーオブジェクト情報ヘッダ(NXNT)
#define	NND_CHUNK_MESH_OBJECT_XB		NNM_CHUNK_ID( 'N','X','M','E' )	// メッシュオブジェクト情報ヘッダ	(NXME)
#define	NND_CHUNK_SHADER_FILE_XB		NNM_CHUNK_ID( 'N','X','S','F' )	// カスタムシェーダーファイル情報ヘッダ(NXSF)
#define	NND_CHUNK_SHADER_INDEX_XB		NNM_CHUNK_ID( 'N','X','S','I' )	// シェーダーインデックス情報ヘッダ	(NXSI)
#define	NND_CHUNK_EFFECT_LIST_XB		NNM_CHUNK_ID( 'N','X','E','F' )	// エフェクトリストヘッダ			(NXEF)
#define	NND_CHUNK_NOF0_XB				NNM_CHUNK_ID( 'N','O','F','0' )	// アドレス解決情報ヘッダ			(NOF0)
#define	NND_CHUNK_NEND_XB				NNM_CHUNK_ID( 'N','E','N','D' )	// データ終了ヘッダ					(NEND)
#define	NND_CHUNK_NFN0_XB				NNM_CHUNK_ID( 'N','F','N','0' )	// ファイル名情報ヘッダ				(NFN0)

/* Lindberg chunk name */
#define	NND_CHUNK_HEADER_LB				NNM_CHUNK_ID( 'N','L','I','F' )	// バイナリファイル情報ヘッダ		(NLIF)
#define	NND_CHUNK_TEXTURE_LB			NNM_CHUNK_ID( 'N','L','T','L' )	// テクスチャリスト情報ヘッダ		(NLTL)
#define	NND_CHUNK_OBJECT_LB				NNM_CHUNK_ID( 'N','L','O','B' )	// オブジェクト情報ヘッダ			(NLOB)
#define	NND_CHUNK_MOTION_LB				NNM_CHUNK_ID( 'N','L','M','O' )	// ノードモーション情報ヘッダ		(NLMO)
#define	NND_CHUNK_MORPH_MOTION_LB		NNM_CHUNK_ID( 'N','L','M','M' )	// モーフモーション情報ヘッダ		(NLMM)
#define	NND_CHUNK_CAMERA_MOTION_LB		NNM_CHUNK_ID( 'N','L','M','C' )	// カメラモーション情報ヘッダ		(NLMC)
#define	NND_CHUNK_LIGHT_MOTION_LB		NNM_CHUNK_ID( 'N','L','M','L' )	// ライトモーション情報ヘッダ		(NLML)
#define	NND_CHUNK_MATERIAL_MOTION_LB	NNM_CHUNK_ID( 'N','L','M','A' )	// マテリアルモーション情報ヘッダ	(NLMA)
#define	NND_CHUNK_LIGHT_LB				NNM_CHUNK_ID( 'N','L','L','I' )	// ライト情報ヘッダ					(NLLI)
#define	NND_CHUNK_CAMERA_LB				NNM_CHUNK_ID( 'N','L','C','A' )	// カメラ情報ヘッダ					(NLCA)
#define	NND_CHUNK_MORPH_LB				NNM_CHUNK_ID( 'N','L','M','T' )	// モーフターゲット情報ヘッダ		(NLMT)
#define	NND_CHUNK_NODE_NAME_LB			NNM_CHUNK_ID( 'N','L','N','N' )	// ノードネームリスト情報ヘッダ		(NLNN)
#define	NND_CHUNK_NODE_TREE_OBJECT_LB	NNM_CHUNK_ID( 'N','L','N','T' )	// ノードツリーオブジェクト情報ヘッダ(NLNT)
#define	NND_CHUNK_MESH_OBJECT_LB		NNM_CHUNK_ID( 'N','L','M','E' )	// メッシュオブジェクト情報ヘッダ	(NLME)
#define	NND_CHUNK_SHADER_FILE_LB		NNM_CHUNK_ID( 'N','L','S','F' )	// カスタムシェーダーファイル情報ヘッダ(NLSF)
#define	NND_CHUNK_SHADER_INDEX_LB		NNM_CHUNK_ID( 'N','L','S','I' )	// シェーダーインデックス情報ヘッダ	(NLSI)
#define	NND_CHUNK_NOF0_LB				NNM_CHUNK_ID( 'N','O','F','0' )	// アドレス解決情報ヘッダ			(NOF0)
#define	NND_CHUNK_NEND_LB				NNM_CHUNK_ID( 'N','E','N','D' )	// データ終了ヘッダ					(NEND)
#define	NND_CHUNK_NFN0_LB				NNM_CHUNK_ID( 'N','F','N','0' )	// ファイル名情報ヘッダ				(NFN0)

/* OpenGL ES 1.1 chunk name */
#define	NND_CHUNK_HEADER_GLES11				NNM_CHUNK_ID( 'N','I','I','F' )	// バイナリファイル情報ヘッダ		(NLIF)
#define	NND_CHUNK_TEXTURE_GLES11			NNM_CHUNK_ID( 'N','I','T','L' )	// テクスチャリスト情報ヘッダ		(NLTL)
#define	NND_CHUNK_OBJECT_GLES11				NNM_CHUNK_ID( 'N','I','O','B' )	// オブジェクト情報ヘッダ			(NLOB)
#define	NND_CHUNK_MOTION_GLES11				NNM_CHUNK_ID( 'N','I','M','O' )	// ノードモーション情報ヘッダ		(NLMO)
#define	NND_CHUNK_MORPH_MOTION_GLES11		NNM_CHUNK_ID( 'N','I','M','M' )	// モーフモーション情報ヘッダ		(NLMM)
#define	NND_CHUNK_CAMERA_MOTION_GLES11		NNM_CHUNK_ID( 'N','I','M','C' )	// カメラモーション情報ヘッダ		(NLMC)
#define	NND_CHUNK_LIGHT_MOTION_GLES11		NNM_CHUNK_ID( 'N','I','M','L' )	// ライトモーション情報ヘッダ		(NLML)
#define	NND_CHUNK_MATERIAL_MOTION_GLES11	NNM_CHUNK_ID( 'N','I','M','A' )	// マテリアルモーション情報ヘッダ	(NLMA)
#define	NND_CHUNK_LIGHT_GLES11				NNM_CHUNK_ID( 'N','I','L','I' )	// ライト情報ヘッダ					(NLLI)
#define	NND_CHUNK_CAMERA_GLES11				NNM_CHUNK_ID( 'N','I','C','A' )	// カメラ情報ヘッダ					(NLCA)
#define	NND_CHUNK_MORPH_GLES11				NNM_CHUNK_ID( 'N','I','M','T' )	// モーフターゲット情報ヘッダ		(NLMT)
#define	NND_CHUNK_NODE_NAME_GLES11			NNM_CHUNK_ID( 'N','I','N','N' )	// ノードネームリスト情報ヘッダ		(NLNN)
#define	NND_CHUNK_NODE_TREE_OBJECT_GLES11	NNM_CHUNK_ID( 'N','I','N','T' )	// ノードツリーオブジェクト情報ヘッダ(NLNT)
#define	NND_CHUNK_MESH_OBJECT_GLES11		NNM_CHUNK_ID( 'N','I','M','E' )	// メッシュオブジェクト情報ヘッダ	(NLME)
#define	NND_CHUNK_SHADER_FILE_GLES11		NNM_CHUNK_ID( 'N','I','S','F' )	// カスタムシェーダーファイル情報ヘッダ(NLSF)
#define	NND_CHUNK_SHADER_INDEX_GLES11		NNM_CHUNK_ID( 'N','I','S','I' )	// シェーダーインデックス情報ヘッダ	(NLSI)
#define	NND_CHUNK_NOF0_GLES11				NNM_CHUNK_ID( 'N','O','F','0' )	// アドレス解決情報ヘッダ			(NOF0)
#define	NND_CHUNK_NEND_GLES11				NNM_CHUNK_ID( 'N','E','N','D' )	// データ終了ヘッダ					(NEND)
#define	NND_CHUNK_NFN0_GLES11				NNM_CHUNK_ID( 'N','F','N','0' )	// ファイル名情報ヘッダ				(NFN0)

/* PSP chunk name */
#define	NND_CHUNK_HEADER_PSP			NNM_CHUNK_ID( 'N','U','I','F' )	// バイナリファイル情報ヘッダ		(NUIF)
#define	NND_CHUNK_TEXTURE_PSP			NNM_CHUNK_ID( 'N','U','T','L' )	// テクスチャリスト情報ヘッダ		(NUTL)
#define	NND_CHUNK_OBJECT_PSP			NNM_CHUNK_ID( 'N','U','O','B' )	// オブジェクト情報ヘッダ			(NUOB)
#define	NND_CHUNK_MOTION_PSP			NNM_CHUNK_ID( 'N','U','M','O' )	// ノードモーション情報ヘッダ		(NUMO)
#define	NND_CHUNK_MORPH_MOTION_PSP		NNM_CHUNK_ID( 'N','U','M','M' )	// モーフモーション情報ヘッダ		(NUMM)
#define	NND_CHUNK_CAMERA_MOTION_PSP		NNM_CHUNK_ID( 'N','U','M','C' )	// カメラモーション情報ヘッダ		(NUMC)
#define	NND_CHUNK_LIGHT_MOTION_PSP		NNM_CHUNK_ID( 'N','U','M','L' )	// ライトモーション情報ヘッダ		(NUML)
#define	NND_CHUNK_MATERIAL_MOTION_PSP	NNM_CHUNK_ID( 'N','U','M','A' )	// マテリアルモーション情報ヘッダ	(NUMA)
#define	NND_CHUNK_LIGHT_PSP				NNM_CHUNK_ID( 'N','U','L','I' )	// ライト情報ヘッダ					(NULI)
#define	NND_CHUNK_CAMERA_PSP			NNM_CHUNK_ID( 'N','U','C','A' )	// カメラ情報ヘッダ					(NUCA)
#define	NND_CHUNK_MORPH_PSP				NNM_CHUNK_ID( 'N','U','M','T' )	// モーフターゲット情報ヘッダ		(NUMT)
#define	NND_CHUNK_NODE_NAME_PSP			NNM_CHUNK_ID( 'N','U','N','N' )	// ノードネームリスト情報ヘッダ		(NUNN)
#define	NND_CHUNK_NODE_TREE_OBJECT_PSP	NNM_CHUNK_ID( 'N','U','N','T' )	// ノードツリーオブジェクト情報ヘッダ(NUNT)
#define	NND_CHUNK_MESH_OBJECT_PSP		NNM_CHUNK_ID( 'N','U','M','E' )	// メッシュオブジェクト情報ヘッダ	(NUME)
#define	NND_CHUNK_NOF0_PSP				NNM_CHUNK_ID( 'N','O','F','0' )	// アドレス解決情報ヘッダ			(NOF0)
#define	NND_CHUNK_NEND_PSP				NNM_CHUNK_ID( 'N','E','N','D' )	// データ終了ヘッダ					(NEND)
#define	NND_CHUNK_NFN0_PSP				NNM_CHUNK_ID( 'N','F','N','0' )	// ファイル名情報ヘッダ				(NFN0)

/* Xenon chunk name */
#define	NND_CHUNK_HEADER_XE				NNM_CHUNK_ID( 'N','E','I','F' )	// バイナリファイル情報ヘッダ		(NXIF)
#define	NND_CHUNK_TEXTURE_XE			NNM_CHUNK_ID( 'N','E','T','L' )	// テクスチャリスト情報ヘッダ		(NXTL)
#define	NND_CHUNK_OBJECT_XE				NNM_CHUNK_ID( 'N','E','O','B' )	// オブジェクト情報ヘッダ			(NXOB)
#define	NND_CHUNK_MOTION_XE				NNM_CHUNK_ID( 'N','E','M','O' )	// ノードモーション情報ヘッダ		(NXMO)
#define	NND_CHUNK_MORPH_MOTION_XE		NNM_CHUNK_ID( 'N','E','M','M' )	// モーフモーション情報ヘッダ		(NXMM)
#define	NND_CHUNK_CAMERA_MOTION_XE		NNM_CHUNK_ID( 'N','E','M','C' )	// カメラモーション情報ヘッダ		(NXMC)
#define	NND_CHUNK_LIGHT_MOTION_XE		NNM_CHUNK_ID( 'N','E','M','L' )	// ライトモーション情報ヘッダ		(NXML)
#define	NND_CHUNK_MATERIAL_MOTION_XE	NNM_CHUNK_ID( 'N','E','M','A' )	// マテリアルモーション情報ヘッダ	(NXMA)
#define	NND_CHUNK_LIGHT_XE				NNM_CHUNK_ID( 'N','E','L','I' )	// ライト情報ヘッダ					(NXLI)
#define	NND_CHUNK_CAMERA_XE				NNM_CHUNK_ID( 'N','E','C','A' )	// カメラ情報ヘッダ					(NXCA)
#define	NND_CHUNK_MORPH_XE				NNM_CHUNK_ID( 'N','E','M','T' )	// モーフターゲット情報ヘッダ		(NXMT)
#define	NND_CHUNK_NODE_NAME_XE			NNM_CHUNK_ID( 'N','E','N','N' )	// ノードネームリスト情報ヘッダ		(NXNN)
#define	NND_CHUNK_NODE_TREE_OBJECT_XE	NNM_CHUNK_ID( 'N','E','N','T' )	// ノードツリーオブジェクト情報ヘッダ(NXNT)
#define	NND_CHUNK_MESH_OBJECT_XE		NNM_CHUNK_ID( 'N','E','M','E' )	// メッシュオブジェクト情報ヘッダ	(NXME)
#define	NND_CHUNK_SHADER_FILE_XE		NNM_CHUNK_ID( 'N','E','S','F' )	// カスタムシェーダーファイル情報ヘッダ(NXSF)
#define	NND_CHUNK_SHADER_INDEX_XE		NNM_CHUNK_ID( 'N','E','S','I' )	// シェーダーインデックス情報ヘッダ	(NXSI)
#define	NND_CHUNK_EFFECT_LIST_XE		NNM_CHUNK_ID( 'N','E','E','F' )	// エフェクトリストヘッダ			(NXEF)
#define	NND_CHUNK_NOF0_XE				NNM_CHUNK_ID( 'N','O','F','0' )	// アドレス解決情報ヘッダ			(NOF0)
#define	NND_CHUNK_NEND_XE				NNM_CHUNK_ID( 'N','E','N','D' )	// データ終了ヘッダ					(NEND)
#define	NND_CHUNK_NFN0_XE				NNM_CHUNK_ID( 'N','F','N','0' )	// ファイル名情報ヘッダ				(NFN0)

/* PS3 chunk name */
#define	NND_CHUNK_HEADER_PS3			NNM_CHUNK_ID( 'N','C','I','F' )	// バイナリファイル情報ヘッダ		(NUIF)
#define	NND_CHUNK_TEXTURE_PS3			NNM_CHUNK_ID( 'N','C','T','L' )	// テクスチャリスト情報ヘッダ		(NUTL)
#define	NND_CHUNK_OBJECT_PS3			NNM_CHUNK_ID( 'N','C','O','B' )	// オブジェクト情報ヘッダ			(NUOB)
#define	NND_CHUNK_MOTION_PS3			NNM_CHUNK_ID( 'N','C','M','O' )	// ノードモーション情報ヘッダ		(NUMO)
#define	NND_CHUNK_MORPH_MOTION_PS3		NNM_CHUNK_ID( 'N','C','M','M' )	// モーフモーション情報ヘッダ		(NUMM)
#define	NND_CHUNK_CAMERA_MOTION_PS3		NNM_CHUNK_ID( 'N','C','M','C' )	// カメラモーション情報ヘッダ		(NUMC)
#define	NND_CHUNK_LIGHT_MOTION_PS3		NNM_CHUNK_ID( 'N','C','M','L' )	// ライトモーション情報ヘッダ		(NUML)
#define	NND_CHUNK_MATERIAL_MOTION_PS3	NNM_CHUNK_ID( 'N','C','M','A' )	// マテリアルモーション情報ヘッダ	(NUMA)
#define	NND_CHUNK_LIGHT_PS3				NNM_CHUNK_ID( 'N','C','L','I' )	// ライト情報ヘッダ					(NULI)
#define	NND_CHUNK_CAMERA_PS3			NNM_CHUNK_ID( 'N','C','C','A' )	// カメラ情報ヘッダ					(NUCA)
#define	NND_CHUNK_MORPH_PS3				NNM_CHUNK_ID( 'N','C','M','T' )	// モーフターゲット情報ヘッダ		(NUMT)
#define	NND_CHUNK_NODE_NAME_PS3			NNM_CHUNK_ID( 'N','C','N','N' )	// ノードネームリスト情報ヘッダ		(NUNN)
#define	NND_CHUNK_NODE_TREE_OBJECT_PS3	NNM_CHUNK_ID( 'N','C','N','T' )	// ノードツリーオブジェクト情報ヘッダ(NUNT)
#define	NND_CHUNK_MESH_OBJECT_PS3		NNM_CHUNK_ID( 'N','C','M','E' )	// メッシュオブジェクト情報ヘッダ	(NUME)
#define	NND_CHUNK_NOF0_PS3				NNM_CHUNK_ID( 'N','O','F','0' )	// アドレス解決情報ヘッダ			(NOF0)
#define	NND_CHUNK_NEND_PS3				NNM_CHUNK_ID( 'N','E','N','D' )	// データ終了ヘッダ					(NEND)
#define	NND_CHUNK_NFN0_PS3				NNM_CHUNK_ID( 'N','F','N','0' )	// ファイル名情報ヘッダ				(NFN0)

/* DXG20 chunk name */
#if !defined(_XBOX)
// DXG20 On Windows
#define	NND_CHUNK_HEADER_DXG20				NNM_CHUNK_ID( 'N','Z','I','F' )	// バイナリファイル情報ヘッダ		(NZIF)
#define	NND_CHUNK_TEXTURE_DXG20				NNM_CHUNK_ID( 'N','Z','T','L' )	// テクスチャリスト情報ヘッダ		(NZTL)
#define	NND_CHUNK_OBJECT_DXG20				NNM_CHUNK_ID( 'N','Z','O','B' )	// オブジェクト情報ヘッダ			(NZOB)
#define	NND_CHUNK_MOTION_DXG20				NNM_CHUNK_ID( 'N','Z','M','O' )	// ノードモーション情報ヘッダ		(NZMO)
#define	NND_CHUNK_MORPH_MOTION_DXG20		NNM_CHUNK_ID( 'N','Z','M','M' )	// モーフモーション情報ヘッダ		(NZMM)
#define	NND_CHUNK_CAMERA_MOTION_DXG20		NNM_CHUNK_ID( 'N','Z','M','C' )	// カメラモーション情報ヘッダ		(NZMC)
#define	NND_CHUNK_LIGHT_MOTION_DXG20		NNM_CHUNK_ID( 'N','Z','M','L' )	// ライトモーション情報ヘッダ		(NZML)
#define	NND_CHUNK_MATERIAL_MOTION_DXG20		NNM_CHUNK_ID( 'N','Z','M','A' )	// マテリアルモーション情報ヘッダ	(NZMA)
#define	NND_CHUNK_LIGHT_DXG20				NNM_CHUNK_ID( 'N','Z','L','I' )	// ライト情報ヘッダ					(NZLI)
#define	NND_CHUNK_CAMERA_DXG20				NNM_CHUNK_ID( 'N','Z','C','A' )	// カメラ情報ヘッダ					(NZCA)
#define	NND_CHUNK_MORPH_DXG20				NNM_CHUNK_ID( 'N','Z','M','T' )	// モーフターゲット情報ヘッダ		(NZMT)
#define	NND_CHUNK_NODE_NAME_DXG20			NNM_CHUNK_ID( 'N','Z','N','N' )	// ノードネームリスト情報ヘッダ		(NZNN)
#define	NND_CHUNK_NODE_TREE_OBJECT_DXG20	NNM_CHUNK_ID( 'N','Z','N','T' )	// ノードツリーオブジェクト情報ヘッダ(NZNT)
#define	NND_CHUNK_MESH_OBJECT_DXG20			NNM_CHUNK_ID( 'N','Z','M','E' )	// メッシュオブジェクト情報ヘッダ	(NZME)
#define	NND_CHUNK_SHADER_FILE_DXG20			NNM_CHUNK_ID( 'N','Z','S','F' )	// カスタムシェーダーファイル情報ヘッダ(NZSF)
#define	NND_CHUNK_SHADER_INDEX_DXG20		NNM_CHUNK_ID( 'N','Z','S','I' )	// シェーダーインデックス情報ヘッダ	(NZSI)
#define	NND_CHUNK_EFFECT_LIST_DXG20			NNM_CHUNK_ID( 'N','Z','E','F' )	// エフェクトリストヘッダ			(NZEF)
#define	NND_CHUNK_NOF0_DXG20				NNM_CHUNK_ID( 'N','O','F','0' )	// アドレス解決情報ヘッダ			(NOF0)
#define	NND_CHUNK_NEND_DXG20				NNM_CHUNK_ID( 'N','E','N','D' )	// データ終了ヘッダ					(NEND)
#define	NND_CHUNK_NFN0_DXG20				NNM_CHUNK_ID( 'N','F','N','0' )	// ファイル名情報ヘッダ				(NFN0)
#else
// DXG20 On xbox360
#define	NND_CHUNK_HEADER_DXG20				NNM_CHUNK_ID( 'N','E','I','F' )	// バイナリファイル情報ヘッダ		(NXIF)
#define	NND_CHUNK_TEXTURE_DXG20				NNM_CHUNK_ID( 'N','E','T','L' )	// テクスチャリスト情報ヘッダ		(NXTL)
#define	NND_CHUNK_OBJECT_DXG20				NNM_CHUNK_ID( 'N','E','O','B' )	// オブジェクト情報ヘッダ			(NXOB)
#define	NND_CHUNK_MOTION_DXG20				NNM_CHUNK_ID( 'N','E','M','O' )	// ノードモーション情報ヘッダ		(NXMO)
#define	NND_CHUNK_MORPH_MOTION_DXG20		NNM_CHUNK_ID( 'N','E','M','M' )	// モーフモーション情報ヘッダ		(NXMM)
#define	NND_CHUNK_CAMERA_MOTION_DXG20		NNM_CHUNK_ID( 'N','E','M','C' )	// カメラモーション情報ヘッダ		(NXMC)
#define	NND_CHUNK_LIGHT_MOTION_DXG20		NNM_CHUNK_ID( 'N','E','M','L' )	// ライトモーション情報ヘッダ		(NXML)
#define	NND_CHUNK_MATERIAL_MOTION_DXG20		NNM_CHUNK_ID( 'N','E','M','A' )	// マテリアルモーション情報ヘッダ	(NXMA)
#define	NND_CHUNK_LIGHT_DXG20				NNM_CHUNK_ID( 'N','E','L','I' )	// ライト情報ヘッダ					(NXLI)
#define	NND_CHUNK_CAMERA_DXG20				NNM_CHUNK_ID( 'N','E','C','A' )	// カメラ情報ヘッダ					(NXCA)
#define	NND_CHUNK_MORPH_DXG20				NNM_CHUNK_ID( 'N','E','M','T' )	// モーフターゲット情報ヘッダ		(NXMT)
#define	NND_CHUNK_NODE_NAME_DXG20			NNM_CHUNK_ID( 'N','E','N','N' )	// ノードネームリスト情報ヘッダ		(NXNN)
#define	NND_CHUNK_NODE_TREE_OBJECT_DXG20	NNM_CHUNK_ID( 'N','E','N','T' )	// ノードツリーオブジェクト情報ヘッダ(NXNT)
#define	NND_CHUNK_MESH_OBJECT_DXG20			NNM_CHUNK_ID( 'N','E','M','E' )	// メッシュオブジェクト情報ヘッダ	(NXME)
#define	NND_CHUNK_SHADER_FILE_DXG20			NNM_CHUNK_ID( 'N','E','S','F' )	// カスタムシェーダーファイル情報ヘッダ(NXSF)
#define	NND_CHUNK_SHADER_INDEX_DXG20		NNM_CHUNK_ID( 'N','E','S','I' )	// シェーダーインデックス情報ヘッダ	(NXSI)
#define	NND_CHUNK_EFFECT_LIST_DXG20			NNM_CHUNK_ID( 'N','E','E','F' )	// エフェクトリストヘッダ			(NXEF)
#define	NND_CHUNK_NOF0_DXG20				NNM_CHUNK_ID( 'N','O','F','0' )	// アドレス解決情報ヘッダ			(NOF0)
#define	NND_CHUNK_NEND_DXG20				NNM_CHUNK_ID( 'N','E','N','D' )	// データ終了ヘッダ					(NEND)
#define	NND_CHUNK_NFN0_DXG20				NNM_CHUNK_ID( 'N','F','N','0' )	// ファイル名情報ヘッダ				(NFN0)
#endif

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __NNB_H__ */

/* End of file */
