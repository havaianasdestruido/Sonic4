// ================================================================
/*!
    Copyright(c) 2009 Dimps CORP. All Rights Reserved. 

	@file       amEffect.h
	@brief      エフェクト描画ライブラリ ヘッダ
	@author	    Syuichi Gotou
	@date	    Date: 2009/04/03 
 */
// ================================================================
#ifndef _AM_EFFECT_H
#define _AM_EFFECT_H

//----- Include files --------------------------------------------------
//----- Definitions ----------------------------------------------------
#define AMD_AME_MULTI_UPDATE		(0)				// 多重更新を許可する
#define AMD_AME_ROTATE_PRECALC		(1)				// 回転成分を前計算

#define AMD_AME_ECB_SIZE			(128)
#define AMD_AME_ENTRY_SIZE			(512)
#define AMD_AME_RUNTIME_SIZE		(512)
#define AMD_AME_WORK_SIZE			(2560)

#define AMD_AME_UNIT_FRAME			(1.0f)			// 単位フレーム
#define AMD_AME_UNIT_TIME			(1.0f/60.0f)	// 単位時間

// runtime state
#define AMD_AME_STATE_KILL			(0x8000)		// 削除
#define AMD_AME_STATE_ACCEPT_KILL	(0x4000)		// 削除許可
#define AMD_AME_STATE_ROOT			(0x2000)		// ルート(平行移動、回転を変更できる)

// ブレンドファンクション (EQU)
#define AMD_BFUNC_ADD					0			//!< ( Cs @ A ) + ( Cd @ B )
#define AMD_BFUNC_SUB					1			//!< ( Cs @ A ) - ( Cd @ B )
#define AMD_BFUNC_SUB_REV				2			//!< ( Cd @ B ) - ( Cs @ A )
#define AMD_BFUNC_MIN					3			//!< min( Cs, Cd )
#define AMD_BFUNC_MAX					4			//!< max( Cs, Cd )
#define AMD_BFUNC_ABS					5			//!< abs( Cs - Cd )

// ブレンドモード (A, B)
#define AMD_SRC_COLOR					0
#define AMD_ONE_MINUS_SRC_COLOR			1
#define AMD_DST_COLOR					0
#define AMD_ONE_MINUS_DST_COLOR			1
#define AMD_SRC_ALPHA					2			//!< NNE_BLENDMODE_SRCALPHA
#define AMD_ONE_MINUS_SRC_ALPHA			3			//!< NNE_BLENDMODE_INVSRCALPHA
#define AMD_DST_ALPHA					4
#define AMD_ONE_MINUS_DST_ALPHA			5
#define AMD_DOUBLE_SRC_ALPHA			6
#define AMD_ONE_MINUS_DOUBLE_SRC_ALPHA	7
#define AMD_DOUBLE_DST_ALPHA			8
#define AMD_ONE_MINUS_DOUBLE_DST_ALPHA	9
#define AMD_FIX_VALUE					10			//!< NNE_BLENDMODE_ONE

/*
	ブレンドモード パラメータ設定ファンクション

	・通常合成			EQU=AMD_BFUNC_ADD, A=AMD_SRC_ALPHA, B=AMD_ONE_MINUS_SRC_ALPHA
	・加算合成			EQU=AMD_BFUNC_ADD, A=AMD_SRC_ALPHA, B=AMD_FIX_VALUE
	・減算合成			EQU=AMD_BFUNC_SUB_REV, A=AMD_SRC_ALPHA, B=AMD_FIX_VALUE
	・光学フィルタ		EQU=AMD_BFUNC_ADD, A=AMD_DST_COLOR, B=AMD_SRC_COLOR

	※ FIXA, FIXB は1で固定
 */
#define AMM_BLEND(equ,a,b)		(((equ)<<8) | ((b)<<4) | (a))
#define AMM_BLEND_NORMAL		(AMM_BLEND(AMD_BFUNC_ADD, AMD_SRC_ALPHA, AMD_ONE_MINUS_SRC_ALPHA))
#define AMM_BLEND_ADD			(AMM_BLEND(AMD_BFUNC_ADD, AMD_SRC_ALPHA, AMD_FIX_VALUE))
#define AMM_BLEND_SUB			(AMM_BLEND(AMD_BFUNC_SUB_REV, AMD_SRC_ALPHA, AMD_FIX_VALUE))

//----- Macro functions ------------------------------------------------

#define AMD_AME_NODE_TYPE(o)		((o)->type)
#define AMD_AME_SUPER_CLASS_ID(o)	((o)->type & AME_AME_SUPER_CLASS_ID_MASK)
#define AMD_AME_CLASS_ID(o)			((o)->type & AME_AME_CLASS_ID_MASK)
#define AMD_AME_IS_EMITTER(o)		(AMD_AME_SUPER_CLASS_ID(o) == AME_AME_NODE_TYPE_EMITTER)
#define AMD_AME_IS_PARTICLE(o)		(AMD_AME_SUPER_CLASS_ID(o) == AME_AME_NODE_TYPE_PARTICLE)
#define AMD_AME_IS_FIELD(o)			(AMD_AME_SUPER_CLASS_ID(o) == AME_AME_NODE_TYPE_FIELD)

// TODO: もっと高速な方法があるはず
#define amFloatToInt(a, b)			(*(a) = (int)(b))

//----- Macros ---------------------------------------------------------
//----- Enum Definitions -----------------------------------------------
// user attribute
typedef enum _AME_AME_USER_ATTRIBUTE {
	// 評価フラグ
	AME_AME_ATTR_INCLUSIVE	= 0,			// 一部条件
	AME_AME_ATTR_EXCLUSIVE,					// 全条件

	// マスク
	AME_AME_ATTR_GROUP_MASK	= 0xffff0000,	// グループマスク
	AME_AME_ATTR_USER_MASK	= 0x0000ffff,	// ユーザーマスク

	// グループID
	AME_AME_ATTR_GROUP_ALL	= AME_AME_ATTR_GROUP_MASK,
	AME_AME_ATTR_GROUP_00	= (1 << 16),
	AME_AME_ATTR_GROUP_01	= (1 << 17),
	AME_AME_ATTR_GROUP_02	= (1 << 18),
	AME_AME_ATTR_GROUP_03	= (1 << 19),
	AME_AME_ATTR_GROUP_04	= (1 << 20),
	AME_AME_ATTR_GROUP_05	= (1 << 21),
	AME_AME_ATTR_GROUP_06	= (1 << 22),
	AME_AME_ATTR_GROUP_07	= (1 << 23),
	AME_AME_ATTR_GROUP_08	= (1 << 24),
	AME_AME_ATTR_GROUP_09	= (1 << 25),
	AME_AME_ATTR_GROUP_0a	= (1 << 26),
	AME_AME_ATTR_GROUP_0b	= (1 << 27),
	AME_AME_ATTR_GROUP_0c	= (1 << 28),
	AME_AME_ATTR_GROUP_0d	= (1 << 29),
	AME_AME_ATTR_GROUP_0e	= (1 << 30),
	AME_AME_ATTR_GROUP_0f	= (1 << 31),
} AME_AME_USER_ATTRIBUTE;

// user group
#define AMD_AME_GROUP_AKIRA		AME_AME_ATTR_GROUP_00
#define AMD_AME_GROUP_JUDEA		AME_AME_ATTR_GROUP_01
#define AMD_AME_GROUP_SAKURA	AME_AME_ATTR_GROUP_02
#define AMD_AME_GROUP_GOTOU		AME_AME_ATTR_GROUP_03
#define AMD_AME_GROUP_TAKINAMI	AME_AME_ATTR_GROUP_04
#define AMD_AME_GROUP_KAMUI		AME_AME_ATTR_GROUP_05
#define AMD_AME_GROUP_ROOKIE00	AME_AME_ATTR_GROUP_06
#define AMD_AME_GROUP_ROOKIE01	AME_AME_ATTR_GROUP_07


//----- Type Definitions -----------------------------------------------

typedef struct _AMS_AME_LIST			AMS_AME_LIST;
typedef struct _AMS_AME_ECB				AMS_AME_ECB;
typedef struct _AMS_AME_ENTRY			AMS_AME_ENTRY;
typedef struct _AMS_AME_RUNTIME			AMS_AME_RUNTIME;
typedef struct _AMS_AME_RUNTIME_WORK	AMS_AME_RUNTIME_WORK;

//! フラスタム情報
typedef struct _AMS_FRUSTUM {
	AMS_MATRIX		viewMatrix;		//!< View Matrix
	float			width;			//!< Width of Screen
	float			height;			//!< Height of Screen
	float			nearClip;		//!< Distance to Screen Plane
	float			farClip;		//!< Distance to Far Clipping Plane
	float			lrNormalX;
	float			lrNormalZ;
	float			tbNormalY;
	float			tbNormalZ;
} AMS_FRUSTUM;

// リスト
typedef struct _AMS_AME_LIST {
	AMS_AME_LIST*		next;					//!< 
	AMS_AME_LIST*		prev;					//!< 
} AMS_AME_LIST;

// ランタイム
typedef struct _AMS_AME_RUNTIME {
	AMS_AME_RUNTIME*		next;				//!< 
	AMS_AME_RUNTIME*		prev;				//!< 
	Sint32					state;				//!< 状態フラグ

	float					amount;				//!< 生成量
	Uint32					count;				//!< 生成した数

	AMS_AME_ECB*			ecb;				//!< エントリーコントロールブロックへのポインタ
	AMS_AME_NODE*			node;				//!< ノードへのポインタ
	AMS_AME_RUNTIME*		parent_runtime;		//!< 親ランタイムへのポインタ
	AMS_AME_RUNTIME*		spawn_runtime;		//!< スポーンランタイムへのポインタ
	AMS_AME_RUNTIME_WORK*	work;				//!< ランタイムワークへのポインタ
	AMS_AME_LIST			child_head;			//!< 子の先頭
    // 48byte

	AMS_AME_LIST			child_tail;			//!< 子の終端
	Sint32					child_num;			//!< 子の数

	AMS_AME_LIST			work_head;			//!< ワークの先頭
	AMS_AME_LIST			work_tail;			//!< ワークの終端
	AMS_AME_LIST			active_head;		//!< アクティブワークの先頭
	AMS_AME_LIST			active_tail;		//!< アクティブワークの終端
	Sint16					work_num;			//!< ワークの数
	Sint16					active_num;			//!< アクティブワークの数
    // 96byte

	NNS_TEXLIST*            texlist;            //!< テクスチャリスト
	Uint32					dummy[3];
	// 16byte
} AMS_AME_RUNTIME; // 112 byte

// ランタイムワーク
typedef struct _AMS_AME_RUNTIME_WORK {
	AMS_AME_RUNTIME_WORK*	next;				//!< 
	AMS_AME_RUNTIME_WORK*	prev;				//!<  
	float					time;				//!< 時間
	Uint32					flag;				//!< 制御フラグ
	AMS_VECTOR				position;			//!< 位置
	AMS_VECTOR				velocity;			//!< 移動量
#if AMD_AME_ROTATE_QUAT
	AMS_QUAT				rotate;				//!< 回転(Quat)
#else
	AMS_VECTOR				rotate;				//!< 回転(Eular XYZ)
#endif
	Uint32					dummy[16];
} AMS_AME_RUNTIME_WORK; // 128 byte

// エントリー
typedef struct _AMS_AME_ENTRY {
	AMS_AME_ENTRY*		next;					//!< 次のエントリー
	AMS_AME_ENTRY*		prev;					//!< 前のエントリー
	AMS_AME_RUNTIME*	runtime;				//!< ランタイムへのポインタ
	Uint32				reserved;				//!< 予約領域
} AMS_AME_ENTRY; // 16 byte

// エントリーコントロールブロック
typedef struct _AMS_AME_ECB {
	AMS_AME_ECB*		next;					//!< 次のエントリーコントロールブロック
	AMS_AME_ECB*		prev;					//!< 前のエントリーコントロールブロック
	Sint32				attribute;				//!< ユーザー属性
	Sint32				priority;				//!< 優先順位
	
	AMS_VECTOR			translate;				//!< 移動
#if AMD_AME_ROTATE_QUAT
	AMS_QUAT			rotate;					//!< 回転(Quat)
#else
	AMS_VECTOR			rotate;					//!< 回転(Eular XYZ)
#endif

	AMS_AME_BOUNDING	bounding;				//!< バンディング情報
	// 80byte

	Sint32				transparency;			//!< 透明度 (0^256)
	float				size_rate;				//!< サイズ拡縮率
	NNS_OBJECT*         pObj;					//!< オブジェクト管理用
	AMS_AME_ENTRY*		entry_head;				//!< エントリーの先頭
	AMS_AME_ENTRY*		entry_tail;				//!< エントリーの最後
	Sint32				entry_num;				//!< エントリー数
    Uint32              drawState;				//!< ディスプレイリスト用コマンドステート
	Uint32              drawObjState;			//!< ディスプレイリスト用コマンドステート
	
	//CreateしてからDrawをせずにUpdateすると１コマ飛ぶのでそれを防止用のフラグ。
	Sint32				skip_update;			//!< Createに立てられ、Draw or Update　時に寝かされます。
	Uint32				reserved[3];			//!< 予約領域
	// 48byte

} AMS_AME_ECB; // 128 byte

//! 
typedef struct _AMS_AME_CREATE_PARAM {
	AMS_AME_ECB*			ecb;				// 
	AMS_AME_RUNTIME*		runtime;			// 
	AMS_AME_NODE*			node;				// 
	AMS_AME_RUNTIME_WORK*	work;				// 
	AMS_VECTOR*				position;			// 位置
	AMS_VECTOR*				velocity;			// 移動量
	AMS_VECTOR*				parent_position;	// 親の位置
	AMS_VECTOR*				parent_velocity;	// 親の移動量
} AMS_AME_CREATE_PARAM; // 32byte

typedef void	(*AmeInitFunc)(AMS_AME_CREATE_PARAM*);
typedef Sint32	(*AmeUpdateFunc)(AMS_AME_RUNTIME*);
typedef void	(*AmeDrawFunc)(AMS_AME_RUNTIME*);
typedef void	(*AmeFieldFunc)(AMS_AME_ECB*, AMS_AME_NODE*, AMS_AME_RUNTIME_WORK*);

typedef struct _AMS_AME_CUSTOM_PARAM {
	union {
		AmeInitFunc			pInitFunc;		// 初期化関数
		AmeFieldFunc		pFiledFunc;		// 
	};
	AmeUpdateFunc			pUpdateFunc;	// 更新関数
	AmeDrawFunc				pDrawFunc;		// 描画関数
} AMS_AME_CUSTOM_PARAM;


//----- Runtime Work -------------------------------------------------------
#define AMD_AME_RWFLAG_SKIP_ANIMATION		(0x00000002)
#define AMD_AME_RWFLAG_TWIST_REVERSE		(0x00000004)
#define AMD_AME_RWFLAG_TEXTURE_FLIP_U		(0x00000008)
#define AMD_AME_RWFLAG_TEXTURE_FLIP_V		(0x00000010)
#define AMD_AME_RWFLAG_SOFTPTR_OFF			(0x00000020)
#define AMD_AME_RWFLAG_SHADOW_ON			(0x00000040)

// エミッタ：全方向
typedef struct _AMS_AME_RUNTIME_WORK_OMNI {
	AMS_AME_RUNTIME_WORK*	next;				// 
	AMS_AME_RUNTIME_WORK*	prev;				// 
	float					time;				// 時間
	Uint32					flag;				// 制御フラグ
	// 16 byte

	AMS_VECTOR				position;			// 位置
	AMS_VECTOR				velocity;			// 移動量
#if AMD_AME_ROTATE_QUAT
	AMS_QUAT				rotate;				// 回転(Quat)
#else
	AMS_VECTOR				rotate;				// 回転(Eular XYZ)
#endif
	float					offset;				// 
	float					offset_chaos;		// 
	// 56 byte
} AMS_AME_RUNTIME_WORK_OMNI; // 72 byte

// エミッタ：単方向
typedef struct _AMS_AME_RUNTIME_WORK_DIRECTIONAL {
	AMS_AME_RUNTIME_WORK*	next;				// 
	AMS_AME_RUNTIME_WORK*	prev;				// 
	float					time;				// 時間
	Uint32					flag;				// 制御フラグ
	// 16 byte

	AMS_VECTOR				position;			// 位置
	AMS_VECTOR				velocity;			// 移動量
#if AMD_AME_ROTATE_QUAT
	AMS_QUAT				rotate;				// 回転(Quat)
#else
	AMS_VECTOR				rotate;				// 回転(Eular XYZ)
#endif
	float					spread;				// 拡散角度
	// 52 byte
} AMS_AME_RUNTIME_WORK_DIRECTIONAL; // 68 byte

// エミッタ：面
typedef struct _AMS_AME_RUNTIME_WORK_SURFACE {
	AMS_AME_RUNTIME_WORK*	next;				// 
	AMS_AME_RUNTIME_WORK*	prev;				// 
	float					time;				// 時間
	Uint32					flag;			    // 制御フラグ
	// 16 byte

	AMS_VECTOR				position;			// 位置
	AMS_VECTOR				velocity;			// 移動量
#if AMD_AME_ROTATE_QUAT
	AMS_QUAT				rotate;				// 回転(Quat)
#else
	AMS_VECTOR				rotate;				// 回転(Eular XYZ)
#endif
	float					width;
	float					height;
	float					offset;
	float					offset_chaos;
	// 64 byte
} AMS_AME_RUNTIME_WORK_SURFACE; // 80 byte

// エミッタ：円
typedef struct _AMS_AME_RUNTIME_WORK_CIRCLE {
	AMS_AME_RUNTIME_WORK*	next;				// 
	AMS_AME_RUNTIME_WORK*	prev;				// 
	float					time;				// 時間
	Uint32					flag;				// 制御フラグ
	// 16 byte

	AMS_VECTOR				position;			// 位置
	AMS_VECTOR				velocity;			// 移動量
#if AMD_AME_ROTATE_QUAT
	AMS_QUAT				rotate;				// 回転(Quat)
#else
	AMS_VECTOR				rotate;				// 回転(Eular XYZ)
#endif
	float					spread;
	float					radius;
	float					offset;
	float					offset_chaos;
	// 64 byte
} AMS_AME_RUNTIME_WORK_CIRCLE; // 80 byte

// パーティクル：シンプルスプライト
typedef struct _AMS_AME_RUNTIME_WORK_SIMPLE_SPRITE {
	AMS_AME_RUNTIME_WORK*	next;				// 
	AMS_AME_RUNTIME_WORK*	prev;				// 
	float					time;				// 時間
	Uint32					flag;			    // 制御フラグ
	// 16 byte

	AMS_VECTOR				position;			// 位置
	AMS_VECTOR				velocity;			// 移動量
	AMS_VECTOR				st;					// テクスチャ座標
	AMS_VECTOR				size;				// サイズ
	AMS_AME_COLOR			color;				// 色
	float					tex_time;			// 
	Sint32					tex_no;				// 
	// 76 byte
} AMS_AME_RUNTIME_WORK_SIMPLE_SPRITE; // 92 byte

// パーティクル：スプライト
typedef struct _AMS_AME_RUNTIME_WORK_SPRITE {
	AMS_AME_RUNTIME_WORK*	next;				// 
	AMS_AME_RUNTIME_WORK*	prev;				// 
	float					time;				// 時間
	Uint32					flag;				// 制御フラグ
	// 16 byte

	AMS_VECTOR				position;			// 位置
	AMS_VECTOR				velocity;			// 移動量
	AMS_VECTOR				st;					// テクスチャ座標
	AMS_VECTOR				size;				// サイズ
	AMS_AME_COLOR			color;				// 色
	float					twist;				// 回転
	float					twist_speed;		// 回転速度
	float					tex_time;			// 
	Sint32					tex_no;				// 
	// 84 byte
} AMS_AME_RUNTIME_WORK_SPRITE; // 100 byte

// パーティクル：ライン
typedef struct _AMS_AME_RUNTIME_WORK_LINE {
	AMS_AME_RUNTIME_WORK*	next;				// 
	AMS_AME_RUNTIME_WORK*	prev;				// 
	float					time;				// 時間
	Uint32					flag;				// 制御フラグ
	// 16 byte

	AMS_VECTOR				position;			// 位置
	AMS_VECTOR				velocity;			// 移動量
	AMS_VECTOR				st;					// テクスチャ座標
	AMS_AME_COLOR			inside_color;		// 内側の色
	AMS_AME_COLOR			outside_color;		// 外側の色
	float					inside_width;		// 内側の幅
	float					outside_width;		// 外側の幅
	float					length;				// サイズ
	float					tex_time;			// 
	Sint32					tex_no;				// 
	// 76 byte
} AMS_AME_RUNTIME_WORK_LINE; // 92 byte

// パーティクル：プレーン
typedef struct _AMS_AME_RUNTIME_WORK_PLANE {
	AMS_AME_RUNTIME_WORK*	next;				// 
	AMS_AME_RUNTIME_WORK*	prev;				// 
	float					time;				// 時間
	Uint32					flag;				// 制御フラグ
	// 16 byte

	AMS_VECTOR				position;			// 位置
	AMS_VECTOR				velocity;			// 移動量
#if AMD_AME_ROTATE_QUAT
	AMS_QUAT				rotate;				// 回転(Quat)
	AMS_VECTOR				rotate_axis;		// 軸回転(XYZ=axis W=angle)
#else
	AMS_VECTOR				rotate;				// 回転(Eular XYZ)
	AMS_VECTOR				rotate_velocity;	// 各軸の回転量
#endif
	AMS_VECTOR				st;					// テクスチャ座標
	AMS_VECTOR				size;				// サイズ
	AMS_AME_COLOR			color;				// 色
	float					tex_time;
	Sint32					tex_no;
	// 108 byte
} AMS_AME_RUNTIME_WORK_PLANE; // 124 byte

// パーティクル：モデル
typedef struct _AMS_AME_RUNTIME_WORK_MODEL {
	AMS_AME_RUNTIME_WORK*	next;				// 
	AMS_AME_RUNTIME_WORK*	prev;				// 
	float					time;				// 時間
	Uint32					flag;				// 制御フラグ
	// 16 bytes

	AMS_VECTOR				position;			// 位置
	AMS_VECTOR				velocity;			// 移動量
#if AMD_AME_ROTATE_QUAT
	AMS_QUAT				rotate;				// 回転(Quat)
	AMS_VECTOR				rotate_axis;		// 軸回転(XYZ=axis W=angle)
#else
	AMS_VECTOR				rotate;				// 回転(Eular XYZ)
	AMS_VECTOR				rotate_velocity;	// 各軸の回転量
#endif
	AMS_VECTOR				scale;				// スケール
	AMS_AME_COLOR			color;				// 色
	float					scroll_u;			// Uスクロール値
	float					scroll_v;			// Vスクロール値
	// 92 byte
} AMS_AME_RUNTIME_WORK_MODEL; // 108 byte


//----- External variables ---------------------------------------------
extern NNS_MATRIX			_am_ef_worldViewMtx;
extern NNS_VECTOR			_am_ef_camPos;


//----- External functions -------------------------------------------

// エフェクトシステムの初期化
void amEffectSystemInit();

// エフェクトシステムのリセット
void amEffectSystemReset();

// サーバー関数
void amEffectExecute();

// カスタムパーティクルの登録
void amEffectRegistCustomFunc(Sint32 classId, AMS_AME_CUSTOM_PARAM* pParam);

// カスタムパーティクルの登録解除
void amEffectUnregistCustomFunc(Sint32 classId);

// AMEファイルとNNS_OBJECTの関連付け
void amEffectSetObject(AMS_AME_ECB* ecb, NNS_OBJECT* amo, Sint32 state);

// ワールドビューマトリクスの設定
void amEffectSetWorldViewMatrix(NNS_MATRIX* mtx);

// ソート用カメラ位置の設定
void amEffectSetCameraPos(NNS_VECTOR* pos);

// エフェクト描画のON/OFF
void amEffectEnableDraw(Sint32 flag);

// 単位時間の設定
void amEffectSetUnitTime(float speed, Sint32 frame_rate);

// 単位時間の取得
float amEffectGetUnitFrame(void);

// ノードの検索
AMS_AME_NODE* amEffectSearchNode(AMS_AME_NODE* node, Sint32 id);
AMS_AME_NODE* amEffectSearchNode(AMS_AME_HEADER* header, Sint32 id);

// ================================================================
/*!
	エフェクトを削除できるか判定します

	@param1	[in]	エントリーコントロールブロックへのポインタ

	@retval			0 - 削除不可
	@retval			1 - 削除可能*

	@note			パーティクルが残っている場合は0を返します
*/
// ================================================================
inline Sint32 amEffectIsDelete(AMS_AME_ECB* ecb)
{
	amAssert( ecb );
	return ecb->entry_num < 0;
}

// エフェクトの作成
AMS_AME_ECB* amEffectCreate(AMS_AME_NODE* node, Sint32 attribute=0, Sint32 priority=0);
AMS_AME_ECB* amEffectCreate(AMS_AME_HEADER* header, Sint32 attribute=0, Sint32 priority=0);

// エフェクトを削除します
void amEffectDelete(AMS_AME_ECB* ecb);

// エフェクトのグループ削除
void amEffectDeleteGroup(Sint32 attr, Sint32 flag=AME_AME_ATTR_EXCLUSIVE);

// エフェクトの終了
void amEffectKill(AMS_AME_ECB* ecb);

// エフェクトのグループ終了
void amEffectKillGroup(Sint32 attr, Sint32 flag=AME_AME_ATTR_EXCLUSIVE);

// エフェクトの更新
void amEffectUpdate(AMS_AME_ECB* ecb);

// エフェクトのグループ更新
void amEffectUpdateGroup(Sint32 attr, Sint32 flag=AME_AME_ATTR_EXCLUSIVE);

// エフェクトの描画
void amEffectDraw(AMS_AME_ECB* ecb, NNS_TEXLIST* texlist, Uint32 state);

// エフェクトのグループ描画
void amEffectDrawGroup(NNS_TEXLIST* texlist, Sint32 attr, Uint32 state, Sint32 flag=AME_AME_ATTR_EXCLUSIVE);

// エフェクトの平行移動の設定
void amEffectSetTranslate(AMS_AME_ECB* ecb, AMS_VECTOR* translate);

// エフェクトの平行移動の設定
void amEffectTranslate(AMS_AME_ECB* ecb, AMS_VECTOR* translate);

// エフェクトの回転の設定
void amEffectSetRotate(AMS_AME_ECB* ecb, Angle32 x, Angle32 y, Angle32 z);
void amEffectSetRotate(AMS_AME_ECB* ecb, AMS_QUAT* q, Sint32 offset=0);

// エフェクトの回転の設定
void amEffectRotate(AMS_AME_ECB* ecb, Angle32 x, Angle32 y, Angle32 z);
void amEffectRotate(AMS_AME_ECB* ecb, AMS_QUAT* q);

// ================================================================
/*!
	エフェクトの透明度を設定します。

	@param	[in]		エントリーコントロールブロックへのポインタ
	@param	[in]		透明度(0.0～1.0)

	@note				TECH移植
*/
// ================================================================
inline void amEffectSetTransparency(AMS_AME_ECB* ecb, float t)
{
	amAssert( ecb );
	amAssert( 0.0f <= t && t <= 1.0f );
//	ecb->transparency = t;
	amFloatToInt( &(ecb->transparency ), t * 256.0f );
}

// ================================================================
/*!
	エフェクトの拡縮率を設定します。

	@param1	[in]		エントリーコントロールブロックへのポインタ
	@param2	[in]		拡縮率
*/
// ================================================================
inline void amEffectSetSizeRate(AMS_AME_ECB* ecb, float t)
{
	amAssert( ecb );
	amAssert( 0.0f <= t );
	ecb->size_rate = t;
}

// パーティクルの作成
void amEffectCreateParticle(AMS_AME_CREATE_PARAM* param);

// ================================================================
/*!
	カラーの補間

	@param1	[out]		出力先
	@param2	[in]		色1
	@param3	[in]		色2
	@param3	[in]		補完率

	@note				pC1 と pC2 のカラー値をrate の割合で補間します
*/
// ================================================================
inline void amEffectLerpColor(AMS_AME_COLOR *pCO, const AMS_AME_COLOR *pC1, const AMS_AME_COLOR *pC2, float rate)
{
	int uRate;
	amFloatToInt(&uRate, rate * 256.0f);

	pCO->r = Uint8( ((pC1->r << 8) + (pC2->r - pC1->r) * uRate) >> 8 );
	pCO->g = Uint8( ((pC1->g << 8) + (pC2->g - pC1->g) * uRate) >> 8 );
	pCO->b = Uint8( ((pC1->b << 8) + (pC2->b - pC1->b) * uRate) >> 8 );
	pCO->a = Uint8( ((pC1->a << 8) + (pC2->a - pC1->a) * uRate) >> 8 );
}

// ================================================================
/*!
	自身の前後のリンクを繋げる
	
	@param1	[out]		リスト
*/
// ================================================================
inline void amEffectDisconnectLink(AMS_AME_LIST* list)
{
	amAssert( list );
	list->prev->next = list->next;
	list->next->prev = list->prev;
}

// ランタイムワークの開放
void amEffectFreeRuntimeWork(AMS_AME_RUNTIME_WORK* work);


#if AMD_DEBUG
//! デバッグ情報表示
extern void amEffectDebugDisplayInfo(void);
#else
#define amEffectDebugDisplayInfo()	((void*)NULL)
#endif


#endif	// _AM_EFFECT_H