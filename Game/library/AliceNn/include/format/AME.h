// ================================================================
/*!
    Copyright(c) 2009 Dimps CORP. All Rights Reserved. 

	@file       AME.h
	@brief      AME ファイルフォーマット ヘッダ
	@author	    Syuichi Gotou
	@date	    Date: 2009/04/01 
 */
// ================================================================
#ifndef _AME_H
#define _AME_H

//----- Include Files --------------------------------------------------

//----- Definitions ----------------------------------------------------
#define AMD_AME_VERSION								(0x00020000)	//!<
#define AMD_AME_ROTATE_QUAT							(1)				//!< クォータニオンを使用する
#define AMD_AME_NODE_NAME_LENGTH					(12)			//!< ノード名の長さ

// node flag
#define AMD_AME_FLAG_NODE_MASK						(0xff000000)	//!< 
#define AMD_AME_FLAG_NODE_ZTEST						(0x01000000)	//!< Zテスト
#define AMD_AME_FLAG_NODE_ZMASK						(0x02000000)	//!< Zマスク
#define AMD_AME_FLAG_NODE_IGNORE_TRANSLATE			(0x04000000)	//!< 移動を無視
#define AMD_AME_FLAG_NODE_IGNORE_ROTATE				(0x08000000)	//!< 回転を無視
#define AMD_AME_FLAG_NODE_ZSORT_OFF					(0x10000000)	//!< Zソートをしない
#define AMD_AME_FLAG_NODE_ZSORT_OFF_NEAR			(0x10000000)	//!< Zソートをしない(半透明の最後に描画します)
#define AMD_AME_FLAG_NODE_ZSORT_OFF_FAR				(0x20000000)	//!< Zソートをしない(半透明の最初に描画します)
#define AMD_AME_FLAG_NODE_SOFTPTR_OFF				(0x40000000)	//!< ソフトパーティクルしない
#define AMD_AME_FLAG_NODE_SHADOW_ON					(0x80000000)	//!< 影を落とす
#define AMD_AME_FLAG_NODE_ZTEST_BIT					(24)			//!<
#define AMD_AME_FLAG_NODE_ZMASK_BIT					(25)			//!<

#define AMD_AME_FLAG_TEXTURE						(0x00001000)	//!< テクスチャ
#define AMD_AME_FLAG_TEXTURE_CROPPING				(0x00002000)	//!< クロッピング
#define AMD_AME_FLAG_TEXTURE_UV_SCROLL				(0x00004000)	//!< UVスクロール
#define AMD_AME_FLAG_TEXTURE_ANIMATION				(0x00008000)	//!< テクスチャアニメーション
#define AMD_AME_FLAG_TEXTURE_LOOP					(0x00010000)	//!< テクスチャアニメーションループ
#define AMD_AME_FLAG_TEXTURE_FLIP_U_RANDOM			(0x00020000)	//!< U軸反転(ランダム)
#define AMD_AME_FLAG_TEXTURE_FLIP_V_RANDOM			(0x00040000)	//!< V軸反転(ランダム)
#define AMD_AME_FLAG_TEXTURE_RANDOM_KEY				(0x00080000)	//!< ランダムキー
#define AMD_AME_FLAG_TEXTURE_FLIP_U_FIXED			(0x00100000)	//!< U軸反転(固定)
#define AMD_AME_FLAG_TEXTURE_FLIP_V_FIXED			(0x00200000)	//!< V軸反転(固定)

#define AMD_AME_FLAG_SPRITE_ALPHA_BLEND				(0x00000001)	//!< αブレンド
#define AMD_AME_FLAG_SPRITE_ALPHA_TEST				(0x00000002)	//!< αテスト
#define AMD_AME_FLAG_SPRITE_RANDOM_TWIST_REVERSE	(0x00000004)	//!< ランダム回転軸反転

#define AMD_AME_FLAG_LINE_ALPHA_BLEND				(0x00000001)	//!< αブレンド
#define AMD_AME_FLAG_LINE_ALPHA_TEST				(0x00000002)	//!< αテスト

#define AMD_AME_FLAG_PLANE_ALPHA_BLEND				(0x00000001)	//!< αブレンド
#define AMD_AME_FLAG_PLANE_ALPHA_TEST				(0x00000002)	//!< αテスト
#define AMD_AME_FLAG_PLANE_RANDOM_ROTATE			(0x00000004)	//!< ランダム回転
#define AMD_AME_FLAG_PLANE_RANDOM_AXIS				(0x00000008)	//!< ランダム回転軸

#define AMD_AME_FLAG_MODEL_ALPHA_BLEND				(0x00000001)	//!< αブレンド
#define AMD_AME_FLAG_MODEL_ALPHA_TEST				(0x00000002)	//!< αテスト
#define AMD_AME_FLAG_MODEL_RANDOM_ROTATE			(0x00000004)	//!< ランダム回転
#define AMD_AME_FLAG_MODEL_RANDOM_AXIS				(0x00000008)	//!< ランダム回転軸

#define AMD_AME_FLAG_SURFACE_EDGE_ONLY				(0x00000001)	//!< 外周のみに生成

#define AMD_AME_FLAG_CIRLCE_EDGE_ONLY				(0x00000001)	//!< 外周のみに生成
#define AMD_AME_FLAG_CIRLCE_EQUALLY					(0x00000002)	//!< 均等に生成

#define AMD_AME_FLAG_FIELD_ROOT_TRANSLATE			(0x00000001)	//!< ルートの平行移動成分を反映する
#define AMD_AME_FLAG_FIELD_ROOT_ROTATE				(0x00000002)	//!< ルートの回転成分を反映する


//----- Type Definitions -----------------------------------------------
//! 
typedef AMS_RGBA8888		AMS_AME_COLOR;

// node type
typedef enum _AME_AME_NODE_TYPE {
	AME_AME_SUPER_CLASS_ID_MASK		= 0xff00,
	AME_AME_CLASS_ID_MASK			= 0x00ff,

	AME_AME_NODE_TYPE_EMITTER		= 0x0100,			// エミッタ
	AME_AME_NODE_TYPE_PARTICLE		= 0x0200,			// パーティクル
	AME_AME_NODE_TYPE_FIELD			= 0x0300,			// フィールド

	// emitter
	AME_AME_NODE_TYPE_OMNI			= 0x0100,			// 全方向
	AME_AME_NODE_TYPE_DIRECTIONAL	= 0x0101,			// 単方向
	AME_AME_NODE_TYPE_SURFACE		= 0x0102,			// 面
	AME_AME_NODE_TYPE_CIRCLE		= 0x0103,			// 円

	// particle
	AME_AME_NODE_TYPE_SIMPLE_SPRITE	= 0x0200,			// シンプルスプライト
	AME_AME_NODE_TYPE_SPRITE		= 0x0201,			// スプライト
	AME_AME_NODE_TYPE_LINE			= 0x0202,			// ライン
	AME_AME_NODE_TYPE_PLANE			= 0x0203,			// プレーン
	AME_AME_NODE_TYPE_MODEL			= 0x0204,			// モデル

	// field
	AME_AME_NODE_TYPE_GRAVITY		= 0x0300,			// 重力
	AME_AME_NODE_TYPE_UNIFORM		= 0x0301,			// 均一
	AME_AME_NODE_TYPE_RADIAL		= 0x0302,			// 放射状
	AME_AME_NODE_TYPE_VORTEX		= 0x0303,			// 渦
	AME_AME_NODE_TYPE_DRAG			= 0x0304,			// ドラッグ
	AME_AME_NODE_TYPE_NOISE			= 0x0305,			// ノイズ

	AME_AME_NODE_TYPE_USER_EMITTER	= 0x0108,			// 
	AME_AME_NODE_TYPE_USER_PARTICLE	= 0x0208,			// 
	AME_AME_NODE_TYPE_USER_FIELD	= 0x0308,			// 
} AME_AME_NODE_TYPE;

// blend mode
typedef enum _AME_AME_BLEND_MODE {
	AME_AME_ALPHA_NORMAL = 0,							// 半透明合成
	AME_AME_ALPHA_ADD,									// 加算合成
	AME_AME_ALPHA_SUB,									// 減算
	AME_AME_ALPHA_SUB1,									// 減算
} AME_AME_BLEND_MODE;

typedef struct _AMS_AME_BOUNDING {
	AMS_VECTOR			center;							//!< 
	float				radius;							//!< 
	float				radius2;						//!< 
	Uint32				reserved[2];					//!< 
} AMS_AME_BOUNDING; // 32 byte

// ノード
typedef struct _AMS_AME_NODE AMS_AME_NODE;
struct _AMS_AME_NODE {
	Sint16				id;								//!< ノードID
	Sint16				type;							//!< ノードタイプ
	Uint32				flag;							//!< 制御フラグ
	Sint8				name[AMD_AME_NODE_NAME_LENGTH];	//!< ノード名
	AMS_AME_NODE*		child;							//!< 子ノードへのポインタ
	AMS_AME_NODE*		sibling;						//!< 兄弟ノードへのポインタ
	AMS_AME_NODE*		parent;							//!< 親ノードへのポインタ
}; // 32 byte

// テクスチャアニメーションキー
typedef struct _AMS_AME_TEX_ANIM_KEY {
	float				time;							//!< 有効時間
	float				l;								//!< テクスチャ座標左
	float				t;								//!< テクスチャ座標上
	float				r;								//!< テクスチャ座標右
	float				b;								//!< テクスチャ座標下
} AMS_AME_TEX_ANIM_KEY; // 20 byte

#if !_PS3
	#pragma warning(disable: 4200)
#endif
// テクスチャアニメーション
typedef struct _AMS_AME_TEX_ANIM {
	float					time;						//!< 1ループの合計時間
	Sint32					key_num;					//!< キーの数
	AMS_AME_TEX_ANIM_KEY	key_buf[0];					//!< キーバッファ
} AMS_AME_TEX_ANIM; // 8 + (20*key_num) byte
#if !_PS3
	#pragma warning(default: 4200)
#endif

// エミッタ：全方向
typedef struct _AMS_AME_NODE_OMNI {
	Sint16				id;								//!< 
	Sint16				type;							//!< 
	Uint32				flag;							//!< 
	Sint8				name[AMD_AME_NODE_NAME_LENGTH];	//!< 
	AMS_AME_NODE*		child;							//!< 
	AMS_AME_NODE*		sibling;						//!< 
	AMS_AME_NODE*		parent;							//!< 
	// 32 byte

	// object offset
	AMS_VECTOR			translate;						//!< 移動
#if AMD_AME_ROTATE_QUAT
	AMS_QUAT			rotate;							//!< 回転(Quat)
#else
	AMS_VECTOR			rotate;							//!< 回転(Eular XYZ)
#endif
	float				inheritance_rate;				//!< 親の継承率
	// object timing
	float				life;							//!< 寿命
	float				start_time;						//!< 開始時間
	// particle offset
	float				offset;							//!< 開始オフセット
	float				offset_chaos;					//!< 開始オフセットカオス
	// particle motion
	float				speed;							//!< 放射速度
	float				speed_chaos;					//!< 放射速度カオス
	// paritcle limit
	float				max_count;						//!< 最大発生数
	float				frequency;						//!< 発生頻度
	// 68 byte
} AMS_AME_NODE_OMNI;

// エミッタ：単方向
typedef struct _AMS_AME_NODE_DIRECTIONAL {
	Sint16				id;								//!< 
	Sint16				type;							//!< 
	Uint32				flag;							//!< 
	Sint8				name[AMD_AME_NODE_NAME_LENGTH];	//!< 
	AMS_AME_NODE*		child;							//!< 
	AMS_AME_NODE*		sibling;						//!< 
	AMS_AME_NODE*		parent;							//!< 
	// 32 byte

	// object offset
	AMS_VECTOR			translate;						//!< 移動
#if AMD_AME_ROTATE_QUAT
	AMS_QUAT			rotate;							//!< 回転(Quat)
#else
	AMS_VECTOR			rotate;							//!< 回転(Eular XYZ)
#endif
	float				inheritance_rate;				//!< 親の継承率
	// object timing
	float				life;							//!< 寿命
	float				start_time;						//!< 開始時間
	// particle offset
	float				offset;							//!< 開始オフセット
	float				offset_chaos;					//!< 開始オフセットカオス
	// particle motion
	float				speed;							//!< 放射速度
	float				speed_chaos;					//!< 放射速度カオス
	// paritcle limit
	float				max_count;						//!< 最大発生数
	float				frequency;						//!< 発生頻度
	float				spread;							//!< 拡散角度
	float				spread_variation;				//!< 拡散角度変化量
	// 76 byte
} AMS_AME_NODE_DIRECTIONAL;

// エミッタ：面
typedef struct _AMS_AME_NODE_SURFACE {
	Sint16				id;								//!< 
	Sint16				type;							//!< 
	Uint32				flag;							//!< 
	Sint8				name[AMD_AME_NODE_NAME_LENGTH];	//!< 
	AMS_AME_NODE*		child;							//!< 
	AMS_AME_NODE*		sibling;						//!< 
	AMS_AME_NODE*		parent;							//!< 
	// 32 byte

	// object offset
	AMS_VECTOR			translate;						//!< 移動
#if AMD_AME_ROTATE_QUAT
	AMS_QUAT			rotate;							//!< 回転(Quat)
#else
	AMS_VECTOR			rotate;							//!< 回転(Eular XYZ)
#endif
	float				inheritance_rate;				//!< 親の継承率
	// object timing
	float				life;							//!< 寿命
	float				start_time;						//!< 開始時間
	// particle offset
	float				offset;							//!< 開始オフセット
	float				offset_chaos;					//!< 開始オフセットカオス
	// particle motion
	float				speed;							//!< 放射速度
	float				speed_chaos;					//!< 放射速度カオス
	// paritcle limit
	float				max_count;						//!< 最大発生数
	float				frequency;						//!< 発生頻度
	float				width;							//!< 横幅
	float				width_variation;				//!< 横幅変化量
	float				height;							//!< 縦幅
	float				height_variation;				//!< 縦幅変化量
	// 84 byte
} AMS_AME_NODE_SURFACE;

// エミッタ：円
typedef struct _AMS_AME_NODE_CIRCLE {
	Sint16				id;								//!< 
	Sint16				type;							//!< 
	Uint32				flag;							//!< 
	Sint8				name[AMD_AME_NODE_NAME_LENGTH];	//!< 
	AMS_AME_NODE*		child;							//!< 
	AMS_AME_NODE*		sibling;						//!< 
	AMS_AME_NODE*		parent;							//!< 
	// 32 byte

	// object offset
	AMS_VECTOR			translate;						//!< 移動
#if AMD_AME_ROTATE_QUAT
	AMS_QUAT			rotate;							//!< 回転(Quat)
#else
	AMS_VECTOR			rotate;							//!< 回転(Eular XYZ)
#endif
	float				inheritance_rate;				//!< 親の継承率
	// object timing
	float				life;							//!< 寿命
	float				start_time;						//!< 開始時間
	// particle offset
	float				offset;							//!< 開始オフセット
	float				offset_chaos;					//!< 開始オフセットカオス
	// particle motion
	float				speed;							//!< 放射速度
	float				speed_chaos;					//!< 放射速度カオス
	// paritcle limit
	float				max_count;						//!< 最大発生数
	float				frequency;						//!< 発生頻度
	float				spread;							//!< 拡散角度
	float				spread_variation;				//!< 拡散角度変化量
	float				radius;							//!< 半径
	float				radius_variation;				//!< 半径変化量
	// 84 byte
} AMS_AME_NODE_CIRCLE;

// パーティクル：スプライト
typedef struct _AME_NODE_SPRITE {
	Sint16				id;								//!< 
	Sint16				type;							//!< 
	Uint32				flag;							//!< 
	Sint8				name[AMD_AME_NODE_NAME_LENGTH];	//!< 
	AMS_AME_NODE*		child;							//!< 
	AMS_AME_NODE*		sibling;						//!< 
	AMS_AME_NODE*		parent;							//!< 
	// 32 byte

	// object offset
	AMS_VECTOR			translate;						//!< 移動
#if AMD_AME_ROTATE_QUAT
	AMS_QUAT			rotate;							//!< 回転(Quat)
#else
	AMS_VECTOR			rotate;							//!< 回転(Eular XYZ)
#endif
	float				z_bias;							//!< Zバイアス
	float				inheritance_rate;				//!< 親の継承率
	// object timing
	float				life;							//!< 寿命
	float				start_time;						//!< 開始時間
	// sprite size
	float				size;							//!< サイズ
	float				size_chaos;						//!< サイズカオス
	// sprite scale
	float				scale_x_start;					//!< 開始スケールX
	float				scale_x_end;					//!< 終了スケールX
	float				scale_y_start;					//!< 開始スケールY
	float				scale_y_end;					//!< 終了スケールY
	// sprite twist
	float				twist_angle;					//!< 回転
	float				twist_angle_chaos;				//!< 回転カオス
	float				twist_angle_speed;				//!< 回転速度
	// sprite color
	AMS_AME_COLOR		color_start;					//!< 開始カラー
	AMS_AME_COLOR		color_end;						//!< 終了カラー
	Sint32				blend;							//!< ブレンドモード
	// texture
	Sint16				texture_slot;					//!< テクスチャスロットID
	Sint16				texture_id;						//!< テクスチャID
	// texture cropping
	float				cropping_l;						//!< クロッピング左
	float				cropping_t;						//!< クロッピング上
	float				cropping_r;						//!< クロッピング右
	float				cropping_b;						//!< クロッピング下
	// texture uv scroll
	float				scroll_u;						//!< Uスクロール値
	float				scroll_v;						//!< Vスクロール値
	// texture animation
	AMS_AME_TEX_ANIM	tex_anim;						//!< 
} AMS_AME_NODE_SPRITE;

// パーティクル：ライン
typedef struct _AMS_AME_NODE_LINE {
	Sint16				id;								//!< 
	Sint16				type;							//!< 
	Uint32				flag;							//!< 
	Sint8				name[AMD_AME_NODE_NAME_LENGTH];	//!< 
	AMS_AME_NODE*		child;							//!< 
	AMS_AME_NODE*		sibling;						//!< 
	AMS_AME_NODE*		parent;							//!< 
	// 32 byte

	// object offset
	AMS_VECTOR			translate;						//!< 移動
#if AMD_AME_ROTATE_QUAT
	AMS_QUAT			rotate;							//!< 回転(Quat)
#else
	AMS_VECTOR			rotate;							//!< 回転(Eular XYZ)
#endif
	float				z_bias;							//!< Zバイアス
	float				inheritance_rate;				//!< 親の継承率
	// object timing
	float				life;							//!< 寿命
	float				start_time;						//!< 開始時間
	// line length
	float				length_start;					//!< 開始長さ
	float				length_end;						//!< 終了長さ
	// line width
	float				inside_width_start;				//!< 内側の開始幅
	float				inside_width_end;				//!< 内側の終了幅
	float				outside_width_start;			//!< 外側の開始幅
	float				outside_width_end;				//!< 外側の終了幅
	// line color
	AMS_AME_COLOR		inside_color_start;				//!< 内側の開始カラー
	AMS_AME_COLOR		inside_color_end;				//!< 内側の終了カラー
	AMS_AME_COLOR		outside_color_start;			//!< 外側の開始カラー
	AMS_AME_COLOR		outside_color_end;				//!< 外側の終了カラー
	Sint32				blend;							//!< ブレンドモード
	// texture
	Sint16				texture_slot;					//!< テクスチャスロットID
	Sint16				texture_id;						//!< テクスチャID
	// texture cropping
	float				cropping_l;						//!< クロッピング左
	float				cropping_t;						//!< クロッピング上
	float				cropping_r;						//!< クロッピング右
	float				cropping_b;						//!< クロッピング下
	// texture uv scroll
	float				scroll_u;						//!< Uスクロール値
	float				scroll_v;						//!< Vスクロール値
	// texture animation
	AMS_AME_TEX_ANIM	tex_anim;						//!< 
} AMS_AME_NODE_LINE;

// パーティクル：プレーン
typedef struct _AMS_AME_NODE_PLANE {
	Sint16				id;								//!< 
	Sint16				type;							//!< 
	Uint32				flag;							//!< 
	Sint8				name[AMD_AME_NODE_NAME_LENGTH];	//!< 
	AMS_AME_NODE*		child;							//!< 
	AMS_AME_NODE*		sibling;						//!< 
	AMS_AME_NODE*		parent;							//!< 
	// 32 byte

	// object offset
	AMS_VECTOR			translate;						//!< 移動
#if AMD_AME_ROTATE_QUAT
	AMS_QUAT			rotate;							//!< 回転(Quat)
#else
	AMS_VECTOR			rotate;							//!< 回転(Eular XYZ)
#endif
	// plane rotate
#if AMD_AME_ROTATE_QUAT
	AMS_VECTOR			rotate_axis;					//!< 軸回転(XYZ=axis W=angle)
#else
	AMS_VECTOR			rotate_velocity;				//!< 各軸の回転量
#endif
	float				z_bias;							//!< Zバイアス
	float				inheritance_rate;				//!< 親の継承率
	// object timing
	float				life;							//!< 寿命
	float				start_time;						//!< 開始時間
	// plane size
	float				size;							//!< サイズ
	float				size_chaos;						//!< サイズカオス
	// plane scale
	float				scale_x_start;					//!< 開始スケールX
	float				scale_x_end;					//!< 終了スケールX
	float				scale_y_start;					//!< 開始スケールY
	float				scale_y_end;					//!< 終了スケールY
	// plane color
	AMS_AME_COLOR		color_start;					//!< 開始カラー
	AMS_AME_COLOR		color_end;						//!< 終了カラー
	Sint32				blend;							//!< ブレンドモード
	// texture
	Sint16				texture_slot;					//!< テクスチャスロットID
	Sint16				texture_id;						//!< テクスチャID
	// texture cropping
	float				cropping_l;						//!< クロッピング左
	float				cropping_t;						//!< クロッピング上
	float				cropping_r;						//!< クロッピング右
	float				cropping_b;						//!< クロッピング下
	// texture uv scroll
	float				scroll_u;						//!< Uスクロール値
	float				scroll_v;						//!< Vスクロール値
	// texture animation
	AMS_AME_TEX_ANIM	tex_anim;						//!< 
} AMS_AME_NODE_PLANE;

// パーティクル：モデル
typedef struct _AMS_AME_NODE_MODEL {
	Sint16				id;								//!< 
	Sint16				type;							//!< 
	Uint32				flag;							//!< 
	Sint8				name[AMD_AME_NODE_NAME_LENGTH];	//!< 
	AMS_AME_NODE*		child;							//!< 
	AMS_AME_NODE*		sibling;						//!< 
	AMS_AME_NODE*		parent;							//!< 
	// 32 byte

	// object offset
	AMS_VECTOR			translate;						//!< 移動
#if AMD_AME_ROTATE_QUAT
	AMS_QUAT			rotate;							//!< 回転(Quat)
#else
	AMS_VECTOR			rotate;							//!< 回転(Eular XYZ)
#endif
	// model rotate
#if AMD_AME_ROTATE_QUAT
	AMS_VECTOR			rotate_axis;					//!< 軸回転(XYZ=axis W=angle)
#else
	AMS_VECTOR			rotate_velocity;				//!< 各軸の回転量
#endif
	// model scale
	AMS_VECTOR			scale_start;					//!< 開始スケール(XYZ)
	AMS_VECTOR			scale_end;						//!< 終了スケール(XYZ)
	float				z_bias;							//!< Zバイアス
	float				inheritance_rate;				//!< 親の継承率
	// object timing
	float				life;							//!< 寿命
	float				start_time;						//!< 開始時間
	Sint8				model_name[8];					//!< モデル名
	Sint32				lod;							//!< LOD
	// model color
	AMS_AME_COLOR		color_start;					//!< 開始カラー
	AMS_AME_COLOR		color_end;						//!< 終了カラー
	Sint32				blend;							//!< ブレンドモード
	// uv scroll
	float				scroll_u;						//!< Uスクロール値
	float				scroll_v;						//!< Vスクロール値
} AMS_AME_NODE_MODEL;

// フィールド：重力
typedef struct _AMS_AME_NODE_GRAVITY {
	Sint16				id;								//!< 
	Sint16				type;							//!< 
	Uint32				flag;							//!< 
	Sint8				name[AMD_AME_NODE_NAME_LENGTH];	//!< 
	AMS_AME_NODE*		child;							//!< 
	AMS_AME_NODE*		sibling;						//!< 
	AMS_AME_NODE*		parent;							//!< 
	// 32 byte

	AMS_VECTOR			direction;						//!< 方向
	float				magnitude;						//!< 強さ
	// 20 byte
} AMS_AME_NODE_GRAVITY;

// フィールド：均一
typedef struct _AMS_AME_NODE_UNIFORM {
	Sint16				id;								//!< 
	Sint16				type;							//!< 
	Uint32				flag;							//!< 
	Sint8				name[AMD_AME_NODE_NAME_LENGTH];	//!< 
	AMS_AME_NODE*		child;							//!< 
	AMS_AME_NODE*		sibling;						//!< 
	AMS_AME_NODE*		parent;							//!< 
	// 32 byte

	AMS_VECTOR			direction;						//!< 方向
	float				magnitude;						//!< 強さ
	// 20 byte
} AMS_AME_NODE_UNIFORM;

// フィールド：放射状
typedef struct _AMS_AME_NODE_RADIAL {
	Sint16				id;								//!< 
	Sint16				type;							//!< 
	Uint32				flag;							//!< 
	Sint8				name[AMD_AME_NODE_NAME_LENGTH];	//!< 
	AMS_AME_NODE*		child;							//!< 
	AMS_AME_NODE*		sibling;						//!< 
	AMS_AME_NODE*		parent;							//!< 
	// 32 byte

	AMS_VECTOR			position;						//!< 位置
	float				magnitude;						//!< 強さ
	float				attenuation;					//!< 減衰率
	// 24 byte
} AMS_AME_NODE_RADIAL;

// フィールド：渦
typedef struct _AMS_AME_NODE_VORTEX {
	Sint16				id;								//!< 
	Sint16				type;							//!< 
	Uint32				flag;							//!< 
	Sint8				name[AMD_AME_NODE_NAME_LENGTH];	//!< 
	AMS_AME_NODE*		child;							//!< 
	AMS_AME_NODE*		sibling;						//!< 
	AMS_AME_NODE*		parent;							//!< 
	// 32 byte

	AMS_VECTOR			position;						//!< 位置
	AMS_VECTOR			axis;							//!< 軸
	// 32 byte
} AMS_AME_NODE_VORTEX;

// フィールド：ドラッグ
typedef struct _AMS_AME_NODE_DRAG {
	Sint16				id;								//!< 
	Sint16				type;							//!< 
	Uint32				flag;							//!< 
	Sint8				name[AMD_AME_NODE_NAME_LENGTH];	//!< 
	AMS_AME_NODE*		child;							//!< 
	AMS_AME_NODE*		sibling;						//!< 
	AMS_AME_NODE*		parent;							//!< 
	// 32 byte

	AMS_VECTOR			position;						//!< 位置
	float				magnitude;						//!< 強さ
	// 20 byte
} AMS_AME_NODE_DRAG;

// フィールド：ノイズ
typedef struct _AMS_AME_NODE_NOISE {
	Sint16				id;								//!< 
	Sint16				type;							//!< 
	Uint32				flag;							//!< 
	Sint8				name[AMD_AME_NODE_NAME_LENGTH];	//!< 
	AMS_AME_NODE*		child;							//!< 
	AMS_AME_NODE*		sibling;						//!< 
	AMS_AME_NODE*		parent;							//!< 
	// 32 byte

	AMS_VECTOR			axis;							//!< 影響方向
	float				magnitude;						//!< 強さ
	// 20 byte
} AMS_AME_NODE_NOISE;

// ファイルヘッダ
typedef struct _AMS_AME_HEADER {
	Sint8				file_id[4];						//!< ファイル識別子 #AME
	Sint32				file_version;					//!< ファイルバージョン
	Sint32				node_num;						//!< ノード数
	
	union
	{
		Uint32			node_ofst;						//!< ノードオフセット
		AMS_AME_NODE*	node;							//!< ルートノードへのポインタ
	};
	
	AMS_AME_BOUNDING	bounding;						//!< バウンディング情報
	Uint32				reserved[4];					//!< 予約領域
} AMS_AME_HEADER, AME_HEADER; // 64 byte


//----- External Definitions -------------------------------------------

// AME ファイルアドレス変換
extern Sint32 amAMEConv(Uint8* pFile);


#endif	// _AME_H
