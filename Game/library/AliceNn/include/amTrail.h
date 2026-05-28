// ================================================================
/*!
    Copyright(c) 2009 Dimps CORP. All Rights Reserved. 

	@file       amTrail.h
	@brief      軌跡ライブラリ ヘッダ
	@author	    Syuichi Gotou
	@date	    Date: 2009/09/24 
 */
// ================================================================
#ifndef _AM_TRAIL_H
#define _AM_TRAIL_H

//----- Definitions ----------------------------------------------------

#define AMD_TRAIL_PARTSMAX		(64)		// 軌跡の構成要素数
#define AMD_TRAIL_MAX			(8)			// 軌跡最大数
#define AMD_TRAILEF_WORKSIZE	(160 - 32)	// エフェクトデータのワークサイズ


//----- Macros ---------------------------------------------------------



//----- Enum Definitions -----------------------------------------------

typedef	enum	_AME_TRAIL_DEF
{
	AMTRE_STATE_DELETE		= 0x8000,
	AMTRE_STATE_EFFECT		= 0x4000,
	AMTRE_STATE_SSP_SET		= 0x0200,		// スプライン補間パラメータ設定済
	AMTRE_STATE_SSP_CALC	= 0x0100,		// スプライン補間パラメータ計算済

	AMTRE_STATE_TOP			= 0x0001,
	AMTRE_STATE_END			= 0x0002,
	AMTRE_STATE_TE_MASK		= 0x0003,

	AMTRE_FLAG_FXPOS		= 0x0001,		// 位置データが固定小数

	AMTRE_MFLAG_PRAIMAL		= 0x00000001,	// 実点（保管された点ではない）
	AMTRE_MFLAG_CHECK1		= 0x00000002,	// 重複チェックされた点

	AMTRE_HANDLE_ACCELL		= 0x0001		// 加速時の軌跡


}	AME_TRAIL_DEF;

//----- Type Definitions -----------------------------------------------
typedef void (*AMTREffectProc)(void* pEffect);

// 軌跡エフェクトデータ構造体
typedef struct _AMS_TRAIL_EFFECT
{
	// システム領域
	_AMS_TRAIL_EFFECT*	pNext;			// 次のデータ
	_AMS_TRAIL_EFFECT*	pPrev;			// 前のデータ
	AMTREffectProc		Procedure;		// プロシージャ
	AMTREffectProc		Destractor;		// デストラクタ
	float				fFrame;			// 現在フレーム
	float				fEndFrame;		// 終了フレーム
	Uint32				drawState;		// 描画ステート
	Uint16				handleId;		// ハンドルID
	Sint16				flag;			// 
	// 32byte

	// ユーザー領域
	Uint8				Work[AMD_TRAILEF_WORKSIZE]; // 128byte
} AMS_TRAIL_EFFECT; // 160 bytes

// 軌跡パラメータ
typedef struct _AMS_TRAIL_PARAM
{
	// ユーザー設定領域
	NNS_RGBA		startColor;		// 始端カラー
	NNS_RGBA		endColor;		// 終端カラー
	NNS_RGBA		ptclColor;		// パーティクルカラー
	float			startSize;		// 軌跡最初の大きさ
	float			endSize;		// 軌跡最後の大きさ

	union {
		NNS_VECTOR*		trail_pos0;		// 軌跡位置へのポインタ
		AMS_VECTOR3I*	trail_pos;		// 軌跡位置へのポインタ（固定小数）
	};

	NNS_TEXLIST*	texlist;		// テクスチャリスト
	Sint32			texId;			// テクスチャID
	float			life;			// 軌跡の寿命
	float			vanish_time;	// 軌跡消去時間
	float			zBias;			// 軌跡のZ値に関係(AMTRE_FLAG_FXPOSのときはZ値で使用)
	// 80byte
	
	float			ptclSize;		// 根元パーティクルの大きさ
	Sint16			partsNum;		// 軌跡の構成要素数（長さに関係）
	Sint16			ptclFlag;       // 根元にテクスチャ描くかどうか？
	Sint16			ptclTexId;		// 根元テクスチャＩＤ
	Sint16			blendType;		// 加算とか乗算とか
	Sint16          zTest;			// 1ならばZテストする
	Sint16			zMask;          // 1ならばZバッファ更新しない
	// 96byte

	// システム使用領域
	float			time;			// 現在寿命
	float			vanish_rate;	// 消去の割合
	Sint16			trailId;        // 軌跡バッファ使用領域判定用
	Sint16			trailPartsId;	// 現在の軌跡構成要素ID
	Sint16			trailPartsNum;	// 登録軌跡構成要素数	
	Sint16			state;			// 状態
	// 112byte

	Sint16			list_no;		// 残像描画番号
	Sint16			dummy[7];
	// 128byte

} AMS_TRAIL_PARAM; // 128byte


// 軌跡の構成要素
typedef struct _AMS_TRAIL_PARTS
{
	NNS_VECTOR			pos;
	NNS_VECTOR			sub_pos;		// 剣軌跡で使用
	NNS_VECTOR			dir;
	float				time;			// 寿命
	_AMS_TRAIL_PARTS*	pNext;
	_AMS_TRAIL_PARTS*	pPrev;
	// 48byte

	Uint32				m_Flag;			// 汎用フラグ
	Sint16				partsId;		// 配列における順番
	Uint16				Dummy[5];
} AMS_TRAIL_PARTS; // 64byte(16byte アライン)

// 軌跡構造体
typedef struct _AMS_TRAIL_PARTSDATA
{
	AMS_TRAIL_PARTS  parts[AMD_TRAIL_PARTSMAX];
	AMS_TRAIL_PARTS  trailHead;
	AMS_TRAIL_PARTS  trailTail;
} AMS_TRAIL_PARTSDATA;

// 軌跡システム管理用
typedef struct _AMS_TRAIL_INTERFACE
{
	AMS_TRAIL_PARTSDATA	trailData[AMD_TRAIL_MAX];
	AMS_TRAIL_EFFECT*	trailEffect[AMD_TRAIL_MAX]; // 軌跡エフェクト管理ポインタ
	Sint16			trailId;	// 現在処理中の軌跡
	Sint16          trailNum;	// 軌跡の数
	Sint16			trailState;	// 軌跡の状態
} AMS_TRAIL_INTERFACE;

//----- External Definitions -------------------------------------------

//----- External Functions ---------------------------------------------

//{### 軌跡エフェクトシステムインターフェース
// ================================================================
// amTrailEFInitialize
/*!
	軌跡エフェクトシステム起動
	NULL ← _amTrailEF_head ⇔ _amTrailEF_tail → NULL
*/
// ================================================================
void amTrailEFInitialize(void);

// ================================================================
// amTrailEFUpdate
/*!
	軌跡エフェクトシステムの更新（メインスレッドで呼ぶこと）
	
	@param handleId	ハンドルID
*/
// ================================================================
void amTrailEFUpdate(Uint16 handleId);

// ================================================================
// amTrailEFDraw
/*!
	軌跡エフェクト描画（メインスレッドで呼ぶこと）
	
	@param handleId	ハンドルID
	@param texlist  テクスチャリスト
	@param state    描画ステート
*/
// ================================================================
void amTrailEFDraw(Uint16 handleId, NNS_TEXLIST* texlist, Uint32 state=0);

// ================================================================
// amTrailEFDeleteGroup
/*!
	軌跡エフェクトのグループ削除
	
	@param handleId	ハンドルID
	@note  プロシージャの停止およびデストラクタを実行する  
*/
// ================================================================
void amTrailEFDeleteGroup(Uint16 handleId);

// ================================================================
// amTrailEFOffsetPos
/*!
	軌跡エフェクトの位置オフセット
	
	@param handleId	ハンドルID
	@param offset	オフセット
	@note  関数使用後、軌跡のベース位置にもオフセット値を加算しておくこと
*/
// ================================================================
void amTrailEFOffsetPos(Uint16 handleId, NNS_VECTOR* offset);



//{### 軌跡登録
// ================================================================
// amTrailMakeEffect
/*!
	軌跡作成  

	@param	param     : 軌跡パラメーター
	@param	handleId  : ハンドルＩＤ
	@param  flag      : 位置データに固定小数使うかなどのフラグ
	@note param で設定していないパラメータはちゃんと０クリアーしておくこと
*/
// ================================================================
void amTrailMakeEffect(AMS_TRAIL_PARAM* param, Uint16 handleId, Sint16 flag=0);


#endif	// _AM_TRAIL_H
