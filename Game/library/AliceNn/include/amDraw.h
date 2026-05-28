/************************************************************************/
/*      amDraw.h                                                        */
/*            Copyright(c) 2000-2009 Dimps CORP. All Rights Reserved.   */
/*----------------------------------------------------------------------*/
/* 描画関連ライブラリヘッダ                                             */
/*----------------------------------------------------------------------*/
/************************************************************************/

#ifndef _AM_DRAW_H_
#define _AM_DRAW_H_

/*----- Update Logs ----------------------------------------------------*/

/*----- Include files --------------------------------------------------*/

typedef struct _AMS_DRAW_VIDEO_ AMS_DRAW_VIDEO;

#include "amMatrix.h"
#include "amRender.h"


/*----- Macros ---------------------------------------------------------*/

/*----- definitions ----------------------------------------------------*/

// ビデオ設定
struct _AMS_DRAW_VIDEO_ {
	float		draw_width;			// 描画解像度
	float		draw_height;
	float		disp_width;			// 表示解像度
	float		disp_height;
	BOOL		wide_screen;		// ワイドスクリーンフラグ
	BOOL		squeeze_screen;		// スクイーズフラグ
	float		refresh_rate;		// リフレッシュレート
	Sint32		video_standard;		// ビデオ標準
	float		width_2d;			// 2D解像度
	float		height_2d;
	float		draw_aspect;		// 描画イメージアスペクト比
	float		scale_x_2d;			// 仮想解像度スケール値
	float		scale_y_2d;
	float		base_x_2d;			// 仮想解像度ベース位置
	float		base_y_2d;
#if _PC | _XBOX
	HWND		hwnd;				// ウィンドウハンドル
	Sint32		style;
	D3DFORMAT	draw_format;		// 描画フォーマット
	D3DFORMAT	draw_format_z;
	D3DFORMAT	disp_format;		// 表示フォーマット
#if _PC
	Sint32		point_x0;			// ポインター有効座標
	Sint32		point_y0;
	Sint32		point_x1;
	Sint32		point_y1;
	BOOL		vsync_flag;			// モニタのVSyncに同期するか
#elif _XBOX
	AMS_XBOXDX_TILE_INFO	*tiling;	// タイリング
	BOOL		vsync_flag;			// モニタのVSyncに同期するか
#endif
#elif _PS3
	Sint32		style;
	Sint32		draw_format;		// 描画フォーマット
	Sint32		draw_format_z;
	Sint32		disp_format;		// 表示フォーマット
	float		disp_width_2d;		// 表示2D解像度
	float		resc_width;			// libresc解像度
	float		resc_height;
#elif _WII
	Sint32		draw_format;		// 描画フォーマット
	Sint32		disp_format;		// 表示フォーマット
	Uint32		depth_max;
	Sint32		point_x0;			// ポインター有効座標
	Sint32		point_y0;
	Sint32		point_x1;
	Sint32		point_y1;
	Uint16		copy_left;			// xfbコピーの左位置
	Uint16		copy_top;
	Uint16		copy_width;
	Uint16		copy_height;
	BOOL		vsync_flag;			// モニタのVSyncに同期するか
#endif
};

#define AMD_VMODE_STD_NTSC			(0)		// NTSC
#define AMD_VMODE_STD_PAL			(1)		// PAL
#define AMD_VMODE_STD_PAL60			(2)		// PAL60
#define AMD_VMODE_STD_PAL60P		(3)		// PAL60 Progressive
#define AMD_VMODE_STD_HIDEF			(4)		// HiDef

#define AMD_DISPLAYLIST_NUM			(4)		// ディスプレイリストの数

#if _IPHONE
#define AMD_REGISTLIST_NUM			(256)	// 登録リストの数
#else
#define AMD_REGISTLIST_NUM			(128)	// 登録リストの数
#endif
#define AMD_SORTLIST_NUM			(512)	// 半透明ソートリストの数

#define AMD_DISPLAY_WIDTH		_am_draw_video.disp_width
#define AMD_DISPLAY_HEIGHT		_am_draw_video.disp_height
#define AMD_SCREEN_WIDTH		_am_draw_video.draw_width
#define AMD_SCREEN_HEIGHT		_am_draw_video.draw_height
#define AMD_SCREEN_2D_WIDTH		_am_draw_video.width_2d
#define AMD_SCREEN_2D_HEIGHT	_am_draw_video.height_2d
#define AMD_2D_SCALE_X			_am_draw_video.scale_x_2d
#define AMD_2D_SCALE_Y			_am_draw_video.scale_y_2d
#define AMD_2D_BASE_X			_am_draw_video.base_x_2d
#define AMD_2D_BASE_Y			_am_draw_video.base_y_2d
#define AMD_SCREEN_ASPECT		_am_draw_video.draw_aspect
#define AMD_RENDER_WIDTH		_am_render_manager.target_now.width
#define AMD_RENDER_HEIGHT		_am_render_manager.target_now.height
#define AMD_RENDER_ASPECT		_am_render_manager.target_now.aspect

#define AMD_USE_VIRTUAL_RESOLUTION_2D		(1)


// 描画環境

// ディフューズカラー
typedef struct {
	NNE_MATCTRLMODE		mode;
	float		r, g, b;
} AMS_DRAWSTATE_DIFFUSE;

// アンビエント
typedef struct {
	NNE_MATCTRLMODE		mode;
	float		r, g, b;
} AMS_DRAWSTATE_AMBIENT;

// α
typedef struct {
	NNE_MATCTRLMODE		mode;
	float		alpha;
} AMS_DRAWSTATE_ALPHA;

// スペキュラー
typedef struct {
	NNE_MATCTRLMODE		mode;
	float		r, g, b;
} AMS_DRAWSTATE_SPECULAR;

// 環境マッピング
typedef struct {
	NNE_MATCTRL_TEXCOORDSRC	texsrc;
	NNS_MATRIX	texmtx;
} AMS_DRAWSTATE_ENVMAP;

// フレームバッファブレンド
typedef struct {
	NNE_MATCTRL_BLEND	mode;
} AMS_DRAWSTATE_BLEND;

// テクスチャオフセット
typedef struct {
	NNE_MATCTRLMODE		mode;
	float		u, v;
} AMS_DRAWSTATE_TEXOFFSET;

// フォグON/OFF
typedef struct {
	Sint32		flag;
} AMS_DRAWSTATE_FOG;

// フォグカラー
typedef struct {
	float		r, g, b;
} AMS_DRAWSTATE_FOG_COLOR;

// フォグレンジ
typedef struct {
	float		fnear, ffar;
} AMS_DRAWSTATE_FOG_RANGE;

// Z比較更新
typedef struct {
	Uint16		compare;
	Uint16		update;
	Sint32		func;
} AMS_DRAWSTATE_Z_MODE;

typedef struct {
	NNF_DRAWOBJ				drawflag;
	AMS_DRAWSTATE_DIFFUSE	diffuse;
	AMS_DRAWSTATE_AMBIENT	ambient;
	AMS_DRAWSTATE_SPECULAR	specular;
	AMS_DRAWSTATE_ENVMAP	envmap;
	AMS_DRAWSTATE_ALPHA		alpha;
	AMS_DRAWSTATE_BLEND		blend;
	AMS_DRAWSTATE_TEXOFFSET	texoffset[4];
	AMS_DRAWSTATE_FOG		fog;
	AMS_DRAWSTATE_FOG_COLOR	fog_color;
	AMS_DRAWSTATE_FOG_RANGE	fog_range;
	AMS_DRAWSTATE_Z_MODE	zmode;
} AMS_DRAWSTATE;

#define AMD_DRAWSTATE_STACK_NUM		(8)
#define NNE_MATCTRLMODE_OFF			((NNE_MATCTRLMODE)-1)
#define NNE_MATCTRL_BLEND_OFF		((NNE_MATCTRL_BLEND)-1)
#define NNE_MATCTRL_TEXCOORDSRC_OFF	((NNE_MATCTRL_TEXCOORDSRC)-1)

#if _PC | _XBOX
#define AMD_ZFUNC_NEVER				(D3DCMP_NEVER)
#define AMD_ZFUNC_LESS				(D3DCMP_LESS)
#define AMD_ZFUNC_EQUAL				(D3DCMP_EQUAL)
#define AMD_ZFUNC_LEQUAL			(D3DCMP_LESSEQUAL)
#define AMD_ZFUNC_GREATER			(D3DCMP_GREATER)
#define AMD_ZFUNC_GEQUAL			(D3DCMP_GREATEREQUAL)
#define AMD_ZFUNC_NOTEQUAL			(D3DCMP_NOTEQUAL)
#define AMD_ZFUNC_ALWAYS			(D3DCMP_ALWAYS)
#define AMD_FILTER_NONE				(D3DX_FILTER_NONE)
#define AMD_FILTER_LINEAR			(D3DX_FILTER_LINEAR)
#define AMD_FILTER_POINT			(D3DX_FILTER_NONE)
#define AMD_WRAP_CLAMP				(0)
#define AMD_WRAP_REPEAT				(1)
#elif _PS3
#define AMD_ZFUNC_NEVER				(NND_CMPFUNC_PS3_NEVER)
#define AMD_ZFUNC_LESS				(NND_CMPFUNC_PS3_LESS)
#define AMD_ZFUNC_EQUAL				(NND_CMPFUNC_PS3_EQUAL)
#define AMD_ZFUNC_LEQUAL			(NND_CMPFUNC_PS3_LEQUAL)
#define AMD_ZFUNC_GREATER			(NND_CMPFUNC_PS3_GREATER)
#define AMD_ZFUNC_GEQUAL			(NND_CMPFUNC_PS3_GEQUAL)
#define AMD_ZFUNC_NOTEQUAL			(NND_CMPFUNC_PS3_NOTEQUAL)
#define AMD_ZFUNC_ALWAYS			(NND_CMPFUNC_PS3_ALWAYS)
#define AMD_FILTER_NONE				(NNE_FILTER_NONE)
#define AMD_FILTER_LINEAR			(NNE_FILTER_LINEAR)
#define AMD_FILTER_POINT			(NNE_FILTER_NONE)
#define AMD_WRAP_CLAMP				(0)
#define AMD_WRAP_REPEAT				(1)
#elif _WII
#define AMD_ZFUNC_NEVER				(GX_NEVER)
#define AMD_ZFUNC_LESS				(GX_LESS)
#define AMD_ZFUNC_EQUAL				(GX_EQUAL)
#define AMD_ZFUNC_LEQUAL			(GX_LEQUAL)
#define AMD_ZFUNC_GREATER			(GX_GREATER)
#define AMD_ZFUNC_GEQUAL			(GX_GEQUAL)
#define AMD_ZFUNC_NOTEQUAL			(GX_NEQUAL)
#define AMD_ZFUNC_ALWAYS			(GX_ALWAYS)
#define AMD_FILTER_NONE				(GX_FALSE)
#define AMD_FILTER_LINEAR			(GX_LINEAR)
#define AMD_FILTER_POINT			(GX_NEAR)
#define AMD_WRAP_CLAMP				(GX_CLAMP)
#define AMD_WRAP_REPEAT				(GX_REPEAT)
#elif _IPHONE
#define AMD_ZFUNC_LEQUAL			(GL_LEQUAL)
#define AMD_WRAP_CLAMP				(GL_CLAMP_TO_EDGE)
#define AMD_WRAP_REPEAT				(GL_REPEAT)
#endif
#define AMD_ZFUNC_DEFAULT			AMD_ZFUNC_LEQUAL

typedef struct _AMS_COMMAND_HEADER_		AMS_COMMAND_HEADER;

// コマンドバッファヘッダ
typedef struct {
	Uint32		system_flag[4];		// システムフラグ
	Uint32		debug_flag[4];		// デバッグフラグ
	Sint32		user_header_size;	// ユーザーヘッダサイズ
	Uint16		display_flag;		// 表示フラグ
	Uint16		regist_flag;		// 登録フラグ
	float		icon_alpha;			// HOMEボタン禁止アイコンα値(Wii)
	Sint32		reserved[1];
} AMS_COMMAND_BUFFER_HEADER;

#define AMD_DSPFLG_SYSTEM_MENU			(0x0001)	// システムメニュー
#define AMD_DSPFLG_SYSTEM_PAUSE			(0x0002)	// システムポーズ

#define AMD_REGFLG_NO_SHADER_COMPILE	(0x0001)	// シェーダーをコンパイルしない


// ディスプレイリスト構造体
typedef struct {
	Sint32		counter;			// データの鮮度カウンタ
	char		*command_buf;		// コマンドバッファ
	char		*data_buf;			// データバッファ
	Sint32		command_buf_size;	// 使用済みコマンドバッファサイズ
	Sint32		data_buf_size;		// 使用済みデータバッファサイズ
} AMS_DISPLAYLIST;

// 登録リスト構造体
typedef struct {
	Sint32		command_id;			// コマンドID
	Sint32		param[11];			// パラメータ
} AMS_REGISTLIST;

#define AMD_REGIST_NONE						(0)
#define AMD_REGIST_LOAD_TEXTURE				(1)		// テクスチャの登録
#define AMD_REGIST_RELEASE_TEXTURE			(2)		// テクスチャの解放
#define AMD_REGIST_VERTEX_BUFFER_OBJECT		(3)		// バッファの取得
#define AMD_REGIST_DELETE_VERTEX_OBJECT		(4)		// バッファの解放
#define AMD_REGIST_LOAD_SHADER_OBJECT		(5)		// シェーダーのロード
#define AMD_REGIST_RELEASE_STD_SHADER		(6)		// シェーダーの解放
#define AMD_REGIST_LOAD_SHADER				(7)		// シェーダーのロード
#define AMD_REGIST_BUILD_SHADER				(8)		// シェーダーのビルド
#define AMD_REGIST_CREATE_SHADER			(9)		// シェーダーの作成
#define AMD_REGIST_RELEASE_SHADER			(10)	// シェーダーの解放
#define AMD_REGIST_LOAD_TEXTURE_IMAGE		(11)	// テクスチャの登録
#define AMD_REGIST_RELEASE_TEXTURE_IMAGE	(12)	// テクスチャの解放

// ソート構造体
typedef struct {
	Sint32		key;				// ソートキー
	AMS_COMMAND_HEADER	*command;	// コマンド
} AMS_DRAW_SORT;


// ディスプレイリスト管理構造体
typedef struct {
	// 管理情報
	AMS_MUTEX	mutex;				// ミューテックスオブジェクト
	Sint32		write_index;		// 書き込み中のディスプレイリストID
	Sint32		last_index;			// 最新のディスプレイリストID
	Sint32		read_index;			// 描画中のディスプレイリストID
	Sint32		user_header_size;	// ユーザーヘッダサイズ

	// 書き込み中の情報
	char		*command_buf_ptr;	// 書き込みコマンドバッファアドレス
	char		*data_buf_ptr;		// 書き込みデータバッファアドレス
	AMS_COMMAND_BUFFER_HEADER	*write_header;	// 書き込みコマンドバッファヘッダ
	char		*write_user_header;	// 書き込みユーザーヘッダ
	Sint32		reg_write_num;		// 書き込み登録データ数
#if AMD_TASK_THREAD_NUM > 1
	AMS_MUTEX	command_buf_mutex;	// 書き込みコマンドバッファミューテックス
	AMS_MUTEX	data_buf_mutex;		// 書き込みデータバッファミューテックス
#endif

	// 読み出し中の情報
	AMS_COMMAND_BUFFER_HEADER	*read_header;	// 読み出しコマンドバッファヘッダ
	char		*read_user_header;	// 読み出しユーザーヘッダ

	// 半透明ソート
	Sint32		sort_num;			// 有効データ数
	AMS_DRAW_SORT	sortlist[AMD_SORTLIST_NUM];

	// ディスプレイリスト
	AMS_DISPLAYLIST		displaylist[AMD_DISPLAYLIST_NUM];

	// 登録リスト
	Sint32		regist_num;			// 有効データ数
	Sint32		reg_write_index;	// 書き込みインデックス
	Sint32		reg_read_index;		// 読み出しインデックス
	Sint32		reg_end_index;		// 読み出し終端インデックス
#if AMD_TASK_THREAD_NUM > 1
	AMS_MUTEX	regist_buf_mutex;	// 登録バッファミューテックス
#endif

#if _PC | _XBOX | _PS3
	Uint16		reg_flag;			// 登録フラグ
#endif
	AMS_REGISTLIST	registlist[AMD_REGISTLIST_NUM];
} AMS_DISPLAYLIST_MANAGER;

#if _PC | _XBOX | _PS3
// 登録フラグ
#define AMD_REG_FLAG_BUILD_SHADER	(0x0001)	// シェーダービルド
#endif

// 描画コマンドヘッダ
struct _AMS_COMMAND_HEADER_ {
	Uint32		state;				// 描画ステート
	Sint32		command_id;			// コマンドID
	void		*param;				// パラメータ
	Sint32		reserved[1];
};

// システム予約ステート
#define AMD_COMMAND_STATE_MAKE_TASK		((Uint32)1 << 24)	// タスク生成
#define AMD_COMMAND_STATE_DEBUG_PRINT	((Uint32)2 << 24)	// デバッグ文字
#define AMD_COMMAND_STATE_DEBUG			((Uint32)3 << 24)	// デバッグ

// システム予約コマンドID
#define AMD_COMMAND_MAKE_TASK				(-1)	// タスク生成
#define AMD_COMMAND_DEBUG_PRINT				(-2)	// デバッグ文字表示
#define AMD_COMMAND_DEBUG_COLOR				(-3)	// デバッグ文字色
#define AMD_COMMAND_DEBUG_MEMORY			(-4)	// メモリメーター表示
#define AMD_COMMAND_DEBUG_THREAD			(-5)	// スレッドメーター表示
#define AMD_COMMAND_DRAW_OBJECT				(-6)	// オブジェクト描画
#define AMD_COMMAND_DRAW_OBJECT_MATMTN		(-7)	// オブジェクト描画
#define AMD_COMMAND_DRAW_OBJECT_MATERIAL	(-8)	// マテリアル設定＋オブジェクト描画
#define AMD_COMMAND_DRAW_MOTION				(-9)	// モーション描画
#define AMD_COMMAND_DRAW_MOTION_MATMTN		(-10)	// モーション描画
#define AMD_COMMAND_DRAW_MOTION_TRS			(-11)	// モーション描画(TRS付き)
#define AMD_COMMAND_DRAW_MOTION_TRS_MATMTN	(-12)	// モーション描画(TRS付き)
#define AMD_COMMAND_DRAW_PRIMITIVE_2D		(-13)	// プリミティブ描画(2D)
#define AMD_COMMAND_DRAW_PRIMITIVE_3D		(-14)	// プリミティブ描画(3D)
#define AMD_COMMAND_SET_DIFFUSE				(-15)	// ディフューズ設定
#define AMD_COMMAND_SET_AMBIENT				(-16)	// アンビエント設定
#define AMD_COMMAND_SET_ALPHA				(-17)	// α設定
#define AMD_COMMAND_SET_SPECULAR			(-18)	// スペキュラー設定
#define AMD_COMMAND_SET_ENVMAP				(-19)	// 環境マップ設定
#define AMD_COMMAND_SET_BLEND				(-20)	// ブレンドモード設定
#define AMD_COMMAND_SET_TEXOFFSET			(-21)	// テクスチャオフセット設定
#define AMD_COMMAND_SET_FOG					(-22)	// フォグ設定
#define AMD_COMMAND_SET_FOG_COLOR			(-23)	// フォグカラー設定
#define AMD_COMMAND_SET_FOG_RANGE			(-24)	// フォグレンジ設定
#define AMD_COMMAND_SET_Z_MODE				(-25)	// Z比較・更新設定

// システム予約コマンドID(ソート)
#define AMD_COMMAND_SORT_DRAW_OBJECT		(-1)	// オブジェクト描画
#define AMD_COMMAND_SORT_DRAW_OBJECT_MATMTN	(-2)	// オブジェクト描画
#define AMD_COMMAND_SORT_DRAW_PRIMITIVE2D	(-3)	// プリミティブ描画（2D）
#define AMD_COMMAND_SORT_DRAW_PRIMITIVE3D	(-4)	// プリミティブ描画（3D）


// パラメータ：タスク生成
typedef struct {
	Sint32		prio;				// 優先順位
	void		(*proc)(AMS_TCB *);	// 処理関数
	Sint32		work_data[2];		// TCBワークに格納される値
} AMS_PARAM_MAKE_TASK;

// パラメータ：デバッグ文字表示
typedef struct {
	Sint16		pos_x;				// 表示位置
	Sint16		pos_y;
	char		text[1];			// 表示文字列
} AMS_PARAM_DEBUG_PRINT;

// パラメータ：デバッグ文字色設定
typedef struct {
	Uint32		color;				// 文字色
} AMS_PARAM_DEBUG_COLOR;

// パラメータ：オブジェクト描画
typedef struct {
	NNS_OBJECT		*object;		// オブジェクト
	NNS_MATRIX		*mtx;			// ベースマトリクス
	NNS_TEXLIST		*texlist;		// テクスチャリスト
	NNF_SUBOBJTYPE	sub_obj_type;
	NNF_DRAWOBJ		flag;
	NNS_MATERIALCALLBACK_FUNC	material_func;	// マテリアルコールバック
	float			scaleZ;			// 位置が一緒でスケールが違う場合を判定するのに使用
} AMS_PARAM_DRAW_OBJECT;

// パラメータ：マテリアル設定＋オブジェクト描画
typedef struct {
	NNS_OBJECT		*object;		// オブジェクト
	NNS_MATRIX		*mtx;			// ベースマトリクス
	NNS_TEXLIST		*texlist;		// テクスチャリスト
	NNF_SUBOBJTYPE	sub_obj_type;
	NNF_DRAWOBJ		flag;
	NNS_MATERIALCALLBACK_FUNC	material_func;	// マテリアルコールバック
	float			scaleZ;			// 位置が一緒でスケールが違う場合を判定するのに使用
	NNS_RGBA		color;			// マテリアルカラー
	NNS_VECTOR		scale;			// スケール
	float			scroll_u;		// UVスクロール用
	float			scroll_v;		// UVスクロール用
	Sint32			blend;			// マテリアルブレンド
	Sint32			reserved[2];
} AMS_PARAM_DRAW_OBJECT_MATERIAL;

// パラメータ：オブジェクト描画(ソート)
typedef struct {
	NNF_DRAWOBJ		drawflag;		// 描画フラグ
	AMS_PARAM_DRAW_OBJECT	*draw_object;	// オブジェクトパラメータ
	NNS_MATRIX		*mtx;			// マトリクスパレット
	NNF_NODESTATUS	*nstat_list;	// ノードステータスリスト
	AMS_DRAWSTATE	*draw_state;
#if _WII
	NNS_VTXLISTPTR	*vtx_list;		// エンベロープ頂点リスト
	Sint32			reserved[1];
#else
	Sint32			reserved[2];
#endif
} AMS_PARAM_SORT_DRAW_OBJECT;

// パラメータ：モーション描画
typedef struct {
	NNS_OBJECT		*object;		// オブジェクト
	NNS_MATRIX		*mtx;			// ベースマトリクス
	NNS_TEXLIST		*texlist;		// テクスチャリスト
	NNF_SUBOBJTYPE	sub_obj_type;
	NNF_DRAWOBJ		flag;
	NNS_MATERIALCALLBACK_FUNC	material_func;	// マテリアルコールバック
	NNS_MOTION		*motion;		// モーション
	float			frame;			// モーションフレーム
	Sint32			reserved[3];
} AMS_PARAM_DRAW_MOTION;

// パラメータ：モーション描画(TRS付き)
typedef struct {
	NNS_OBJECT		*object;		// オブジェクト
	NNS_MATRIX		*mtx;			// ベースマトリクス
	NNS_TEXLIST		*texlist;		// テクスチャリスト
	NNF_SUBOBJTYPE	sub_obj_type;
	NNF_DRAWOBJ		flag;
	NNS_MATERIALCALLBACK_FUNC	material_func;	// マテリアルコールバック
	NNS_MOTION		*motion;		// モーション
	float			frame;			// モーションフレーム
	NNS_TRS			*trslist;		// TRSリスト
	NNS_MOTION		*mmotion;		// マテリアルモーション
	float			mframe;			// マテリアルモーションフレーム
} AMS_PARAM_DRAW_MOTION_TRS;

// パラメータ：プリミティブ描画
// 16バイトアラインメントにしておかないと、amDrawPrimitive3D等でマトリクス取得したときに
// 正しくデータがコピーされないことがある
typedef struct {
	NNS_MATRIX		*mtx;			// ベースマトリクス
	union {
	    NNS_PRIM3D_PCT	*vtxPCT3D;	// ポリゴンデータ
		NNS_PRIM3D_PC	*vtxPC3D;
	    NNS_PRIM2D_PCT  *vtxPCT2D;
		NNS_PRIM2D_PC   *vtxPC2D;
	};

	union {
		NNE_PRIM3D_FMT format3D;	// 頂点フォーマット
		NNE_PRIM2D_FMT format2D;		
	};
	NNE_PRIM_TRIANGLE type;			// プリミティブタイプ
	Sint32			count;			// 頂点数
	NNS_TEXLIST		*texlist;		// テクスチャリスト
	Sint32			texId;			// テクスチャＩＤ
	NNE_PRIM_ALPHABLEND ablend;     // NNE_PRIM_ALPHABLEND_OFF or NNE_PRIM_ALPHABLEND_ON
	union {
		float		sortZ;			// 3Dソート用Z値
		float		zOffset;		// 2D用Z格納値
	};

	// ブレンド関数パラメータ
#if _PC | _XBOX
	NNE_BLENDMODE	bldSrc;
	NNE_BLENDMODE	bldDst;
	NNE_BLENDOP		bldMode;
#elif _PS3 | _IPHONE
	Sint32			bldSrc;
	Sint32			bldDst;
	Sint32			bldMode;
#elif _WII
	GXBlendFactor   bldSrc;
	GXBlendFactor   bldDst;
	GXBlendMode		bldMode;
#endif

	Sint16			aTest;			// 1ならばαテスト
	Sint16			zMask;			// 1ならばZバッファ更新しない
	Sint16			zTest;			// 1ならばZテストする
	Sint16			noSort;			// 1ならば半透明ソートしない
	NNE_PRIM_TEXWRAP uwrap;
	NNE_PRIM_TEXWRAP vwrap;
} AMS_PARAM_DRAW_PRIMITIVE; // 64byte

// パラメータ：テクスチャ座標オフセット設定
typedef struct {
	Sint32			slot;			// 対象スロット
	AMS_DRAWSTATE_TEXOFFSET	texoffset;
} AMS_PARAM_SET_TEXOFFSET;

// パラメータ：テクスチャの登録
typedef struct {
	NNS_TEXINFO	*pTexInfo;
	const void	*tex;
	NNF_TEXFILE_MINFILTER	minfilter;
	NNF_TEXFILE_MAGFILTER	magfilter;
	Uint32		globalIndex;
	Uint32		bank;
	NNF_TEXFLAG	flag;
	Uint32		size;
	void		*buf_delete;
} AMS_PARAM_LOAD_TEXTURE;

// パラメータ：テクスチャの解放
typedef struct {
	NNS_TEXLIST	*texlist;
} AMS_PARAM_RELEASE_TEXTURE;

// パラメータ：バッファの登録
typedef struct {
	NNS_OBJECT	*obj;
	NNS_OBJECT	*srcobj;
#if _PC | _XBOX
	NNF_VTXOBJ	vtxflag;
#elif _PS3 | _IPHONE
	NNF_BINDOBJ	bindflag;
#endif
	NNF_DRAWOBJ	drawflag;
} AMS_PARAM_VERTEX_BUFFER_OBJECT;

// パラメータ：バッファの解放
typedef struct {
	NNS_OBJECT	*obj;
} AMS_PARAM_DELETE_VERTEX_OBJECT;

// パラメータ：オブジェクトのシェーダーの登録
typedef struct {
	NNS_OBJECT	**obj;
	Sint32		flag_num;
	NNF_DRAWOBJ	*drawflag;
} AMS_PARAM_LOAD_SHADER_OBJECT;

// パラメータ：シェーダーのロード
typedef struct {
	void		*image;
} AMS_PARAM_LOAD_SHADER;

// パラメータ：シェーダーのビルド
typedef struct {
	const char	*vs_code;
	Sint32		vs_size;
	const char	*ps_code;
	Sint32		ps_size;
	void		**vs_shader;
	void		**ps_shader;
	void		*vs_param;
	void		*ps_param;
	Sint16		vs_constants;
	Sint16		ps_constants;
} AMS_PARAM_BUILD_SHADER;

// パラメータ：シェーダーの作成
typedef struct {
	void		*vs_image;
	void		*ps_image;
	void		**vs_shader;
	void		**ps_shader;
	void		*vs_param;
	void		*ps_param;
	Sint16		vs_constants;
	Sint16		ps_constants;
} AMS_PARAM_CREATE_SHADER;

// パラメータ：シェーダーの解放
typedef struct {
	void		*vs_shader;
	void		*ps_shader;
} AMS_PARAM_RELEASE_SHADER;

// パラメータ：テクスチャの登録
typedef struct {
	void		**texture;
	void		*image;
	Sint32		size;
	Sint16		minfilter;
	Sint16		magfilter;
	Sint16		u_wrap;
	Sint16		v_wrap;
#if _WII
	NVS_GVROBJ	*gvrobj;
#endif
} AMS_PARAM_LOAD_TEXTURE_IMAGE;

// パラメータ：テクスチャの解放
typedef struct {
	void		*texture;
} AMS_PARAM_RELEASE_TEXTURE_IMAGE;

// αブレンドタイプ
typedef enum _am_draw_alphablend_type
{
	AMDRAWE_BLENDTYPE_NORMAL,   		// 乗算
	AMDRAWE_BLENDTYPE_ADD,   			// 加算　ADD (Src + Dest)
	AMDRAWE_BLENDTYPE_SUB,   			// 減算　SUB (Src - Dest)
	AMDRAWE_BLENDTYPE_NUM,
} AMDRAWE_BLENDTYPE;


/*----- External variables ---------------------------------------------*/

extern AMS_DRAW_VIDEO		_am_draw_video;
extern AMS_RENDER_TARGET	_am_draw_target;


// 描画コマンド処理関数
extern void (*_am_draw_command_func)(AMS_COMMAND_HEADER *, NNF_DRAWOBJ);
extern void (*_am_draw_command_sort)(AMS_COMMAND_HEADER *, NNF_DRAWOBJ);

extern AMS_DISPLAYLIST_MANAGER		_am_displaylist_manager;

extern NNS_MATRIX	_am_draw_world_view_matrix;

#if _PC | _XBOX | _PS3
extern Uint32	_am_draw_vs_code_size;
extern Uint32	_am_draw_ps_code_size;
extern void		*_am_draw_vs_code_buf;
extern void		*_am_draw_ps_code_buf;
#elif _WII
extern NNS_VECTOR		_am_draw_toonDir;
extern NNS_MATRIX44		_am_draw_fall_projmtx; // 滝
extern AMS_RENDER_TARGET	_am_draw_render_work;
#endif
extern NNS_RGBA_U8 _am_draw_bg_color;
extern Sint32	_am_draw_in_scene;

extern Sint32	_am_draw_offset_x;


/*----- Inline functions -----------------------------------------------*/

inline void amDrawConv2D(NNS_VECTOR2D *vd, NNS_VECTOR2D *vs)
{
#if !AMD_USE_VIRTUAL_RESOLUTION_2D
	*vd			= *vs;
#else
	vd->x		= vs->x * AMD_2D_SCALE_X + AMD_2D_BASE_X;
	vd->y		= vs->y * AMD_2D_SCALE_Y + AMD_2D_BASE_Y;
#endif
}


/*----- External functions ---------------------------------------------*/

/************************************************************************/
/* void amDrawSetShaderCompile(Sint32 flag)                             */
/*----------------------------------------------------------------------*/
/* [INPUT]  flag : シェーダーをランタイムでコンパイルするか             */
/* [FUNCTION] シェーダーコンパイルの設定                                */
/************************************************************************/
void amDrawSetShaderCompile(Sint32 flag);

/************************************************************************/
/* void amDrawSetVSyncAlarm(AMS_ALARM *alarm)                           */
/*----------------------------------------------------------------------*/
/* [INPUT]  alarm : V-Syncを通知するアラーム                            */
/* [FUNCTION] V-Sync待ち                                                */
/************************************************************************/
void amDrawSetVSyncAlarm(AMS_ALARM *alarm);

/************************************************************************/
/* void amDrawWaitVSync(void)                                           */
/*----------------------------------------------------------------------*/
/* [FUNCTION] V-Sync待ち                                                */
/************************************************************************/
void amDrawWaitVSync(void);

/************************************************************************/
/* Sint32 amDrawBegin(AMS_RENDER_TERGET *target, NNS_RGBA_U8 *color,    */
/*                            float depth, Sint32 stencil, Uint32 flag) */
/*----------------------------------------------------------------------*/
/* [INPUT]  target  : 描画対象レンダーターゲット                        */
/*          flag    : クリアフラグ(AMD_RENDER_CLEAR_～)                 */
/*          color   : 背景色                                            */
/*          depth   : デプス値                                          */
/*          stencil : ステンシル値                                      */
/* [FUNCTION] 描画開始                                                  */
/************************************************************************/
Sint32 amDrawBegin(AMS_RENDER_TARGET *target = NULL,
		Uint32 flag = AMD_RENDER_CLEAR_COLOR | AMD_RENDER_CLEAR_DEPTH,
		NNS_RGBA_U8 *color = NULL, float depth = 1.0f, Sint32 stencil = 0);

/************************************************************************/
/* void amDrawEnd(void)                                                 */
/*----------------------------------------------------------------------*/
/* [FUNCTION] 描画終了                                                  */
/************************************************************************/
void amDrawEnd(NNS_RGBA_U8 *color = NULL, float z = 1.0f, Sint32 stencil = 0);

/************************************************************************/
/* void amDrawBeginScene(void)                                          */
/*----------------------------------------------------------------------*/
/* [FUNCTION] シーン描画開始                                            */
/************************************************************************/
void amDrawBeginScene(void);

/************************************************************************/
/* void amDrawEndScene(void)                                            */
/*----------------------------------------------------------------------*/
/* [FUNCTION] シーン描画終了(ソート描画の開始)                          */
/************************************************************************/
void amDrawEndScene(void);

/************************************************************************/
/* void amDrawCreateBuffer(Sint32 command_size, Sint32 data_size,       */
/*                                                    Sint32 work_size) */
/*----------------------------------------------------------------------*/
/* [INPUT] command_size : コマンドバッファのサイズ                      */
/*         data_size    : データバッファのサイズ                        */
/*         work_size    : ワークバッファのサイズ                        */
/* [FUNCTION] 描画用バッファの確保                                      */
/************************************************************************/
void amDrawCreateBuffer(Sint32 command_size = 128 * 1024,
		Sint32 data_size = 1024 * 1024, Sint32 work_size = 1024 * 1024);

/************************************************************************/
/* void amDrawDeleteBuffer(void)                                        */
/*----------------------------------------------------------------------*/
/* [FUNCTION] 描画用バッファの解放                                      */
/************************************************************************/
void amDrawDeleteBuffer(void);

#if _PC
/************************************************************************/
/* void amDrawSetStdShaderCodeVS(void *buf, Uint32 size)                */
/*----------------------------------------------------------------------*/
/* [INPUT] buf  : 標準バーテックスシェーダーのコード                    */
/*         size : 標準バーテックスシェーダーのコードサイズ              */
/* [FUNCTION] 標準バーテックスシェーダーの設定                          */
/************************************************************************/
inline void amDrawSetStdShaderCodeVS(void *buf, Uint32 size)
{
	_am_draw_vs_code_buf	= buf;
	_am_draw_vs_code_size	= size;
}

/************************************************************************/
/* void amDrawSetStdShaderCodePS(void *buf, Uint32 size)                */
/*----------------------------------------------------------------------*/
/* [INPUT] buf  : 標準ピクセルシェーダーのコード                        */
/*         size : 標準ピクセルシェーダーのコードサイズ                  */
/* [FUNCTION] 標準バーテックスシェーダーの設定                          */
/************************************************************************/
inline void amDrawSetStdShaderCodePS(void *buf, Uint32 size)
{
	_am_draw_ps_code_buf	= buf;
	_am_draw_ps_code_size	= size;
}
#endif

/************************************************************************/
/* void amDrawDisplay(AMS_RENDER_TARGET *target, Sint32 index)          */
/*----------------------------------------------------------------------*/
/* [INPUT]  target : 表示するレンダーターゲット(NULLならばカレント)     */
/*          index  : target内のテクスチャインデックス                   */
/* [FUNCTION] ディスプレイ描画                                          */
/************************************************************************/
void amDrawDisplay(AMS_RENDER_TARGET *target = NULL, Sint32 index = 0);

/************************************************************************/
/* void amDrawInitDisplayList(Sint32 user_header_size)                  */
/*----------------------------------------------------------------------*/
/* [INPUT] user_header_size : ユーザーヘッダのサイズ                    */
/* [FUNCTION] ディスプレイリストの初期化                                */
/************************************************************************/
void amDrawInitDisplayList(Sint32 user_header_size = 0);

/************************************************************************/
/* void amDrawExitDisplayList(void)                                     */
/*----------------------------------------------------------------------*/
/* [FUNCTION] ディスプレイリストの終了                                  */
/************************************************************************/
void amDrawExitDisplayList(void);

/************************************************************************/
/* void amDrawSetDrawCommandFunc(                                       */
/*          void (*func)(AMS_COMMAND_HEADER *, NNF_DRAWOBJ drawflag),   */
/*          void (*sort)(AMS_COMMAND_HEADER *, NNF_DRAWOBJ drawflag))   */
/*----------------------------------------------------------------------*/
/* [INPUT] func : 描画コマンド処理関数                                  */
/*         sort : 描画コマンド処理関数(ソート描画時)                    */
/* [FUNCTION] 描画コマンド処理関数の設定                                */
/************************************************************************/
inline void amDrawSetDrawCommandFunc(
		void (*func)(AMS_COMMAND_HEADER *, NNF_DRAWOBJ drawflag),
		void (*sort)(AMS_COMMAND_HEADER *, NNF_DRAWOBJ drawflag))
{
	_am_draw_command_func	= func;
	_am_draw_command_sort	= sort;
}

/************************************************************************/
/* void amDrawOpenDisplayList(void)                                     */
/*----------------------------------------------------------------------*/
/* [FUNCTION] ディスプレイリストのオープン(Lock状態で呼ぶこと)          */
/************************************************************************/
void amDrawOpenDisplayList(void);

/************************************************************************/
/* void amDrawCloseDisplayList(void)                                    */
/*----------------------------------------------------------------------*/
/* [FUNCTION] ディスプレイリストのクローズ                              */
/*            次のディスプレイリストのオープンも一緒にする              */
/************************************************************************/
void amDrawCloseDisplayList(void);

/************************************************************************/
/* Sint32 amDrawGetDisplayList(void)                                    */
/*----------------------------------------------------------------------*/
/* [RETURN] 取得したディスプレイリストインデックス                      */
/* [FUNCTION] 読み出し用ディスプレイリストの取得                        */
/************************************************************************/
Sint32 amDrawGetDisplayList(void);

/************************************************************************/
/* void amDrawSetDisplayFlag(Sint32 flag, float alpha)                  */
/*----------------------------------------------------------------------*/
/* [INPUT]  flag  : 表示フラグ                                          */
/*          alpha : HOMEボタン禁止アイコンα値(Wiiのみ有効)             */
/* [FUNCTION] 表示フラグとHOMEボタン禁止アイコンのα値を設定する        */
/************************************************************************/
void amDrawSetDisplayFlag(Sint32 flag, float alpha);

/************************************************************************/
/* AMS_COMMAND_BUFFER_HEADER *amDrawGetWriteHeader(void)                */
/*----------------------------------------------------------------------*/
/* [FUNCTION] 書き込み用コマンドバッファヘッダの取得                    */
/************************************************************************/
inline AMS_COMMAND_BUFFER_HEADER *amDrawGetWriteHeader(void)
{
	return	_am_displaylist_manager.write_header;
}

/************************************************************************/
/* AMS_COMMAND_BUFFER_HEADER *amDrawGetReadHeader(void)                 */
/*----------------------------------------------------------------------*/
/* [FUNCTION] 読み込み用コマンドバッファヘッダの取得                    */
/************************************************************************/
inline AMS_COMMAND_BUFFER_HEADER *amDrawGetReadHeader(void)
{
	return	_am_displaylist_manager.read_header;
}

/************************************************************************/
/* char *amDrawGetWriteUserHeader(void)                                 */
/*----------------------------------------------------------------------*/
/* [FUNCTION] 書き込み用ディスプレイリストヘッダのユーザーヘッダの取得  */
/************************************************************************/
inline char *amDrawGetWriteUserHeader(void)
{
	return	_am_displaylist_manager.write_user_header;
}

/************************************************************************/
/* char *amDrawGetReadUserHeader(void)                                  */
/*----------------------------------------------------------------------*/
/* [FUNCTION] 読み出し用ディスプレイリストヘッダのユーザーヘッダの取得  */
/************************************************************************/
inline char *amDrawGetReadUserHeader(void)
{
	return	_am_displaylist_manager.read_user_header;
}

/************************************************************************/
/* void amDrawSetSystemFlagInt(Sint32 id, Sint32 val)                   */
/*----------------------------------------------------------------------*/
/* [INPUT]  id  : フラグID(0-3)                                         */
/*          val : 設定する値(32bit)                                     */
/* [FUNCTION] システムフラグに32bit整数値を設定する                     */
/************************************************************************/
inline void amDrawSetSystemFlagInt(Sint32 id, Sint32 val)
{
	_am_displaylist_manager.write_header->system_flag[id]	= (Uint32)val;
}

/************************************************************************/
/* void amDrawSetSystemFlagBool(Sint32 id, BOOL val)                    */
/*----------------------------------------------------------------------*/
/* [INPUT]  id  : フラグID(0-127)                                       */
/*          val : 設定する値(1bit)                                      */
/* [FUNCTION] システムフラグにBOOL値を設定する                          */
/************************************************************************/
void amDrawSetSystemFlagBool(Sint32 id, BOOL val);

/************************************************************************/
/* Sint32 amDrawGetSystemFlagInt(Sint32 id)                             */
/*----------------------------------------------------------------------*/
/* [INPUT]  id  : フラグID(0-3)                                         */
/* [FUNCTION] システムフラグから32bit整数値を取得する                   */
/************************************************************************/
inline Sint32 amDrawGetSystemFlagInt(Sint32 id)
{
	return	(Sint32)_am_displaylist_manager.read_header->system_flag[id];
}

/************************************************************************/
/* BOOL amDrawGetSystemFlagBool(Sint32 id)                              */
/*----------------------------------------------------------------------*/
/* [INPUT]  id  : フラグID(0-127)                                       */
/* [FUNCTION] システムフラグからBOOL値を取得する                        */
/************************************************************************/
BOOL amDrawGetSystemFlagBool(Sint32 id);

/************************************************************************/
/* void amDrawRegistCommand(Uint32 state, Sint32 command_id, void *param)*/
/*----------------------------------------------------------------------*/
/* [INPUT] state      : 描画ステート                                    */
/*         command_id : 描画コマンドID                                  */
/*         param      : パラメータへのポインタ                          */
/* [FUNCTION] ディスプレイリストにコマンドを登録する                    */
/************************************************************************/
void amDrawRegistCommand(Uint32 state, Sint32 command_id, void *param = NULL);

/************************************************************************/
/* Sint32 amDrawRegistCommand(Sint32 command_id, void *param)           */
/*----------------------------------------------------------------------*/
/* [INPUT] command_id : 登録コマンドID                                  */
/*         param      : パラメータへのポインタ                          */
/* [RETURN]  登録インデックス                                           */
/* [FUNCTION] 登録リストにコマンドを登録する                            */
/************************************************************************/
Sint32 amDrawRegistCommand(Sint32 command_id, void *param = NULL);

/************************************************************************/
/* Sint32 amDrawIsRegistComplete(Sint32 index)                          */
/*----------------------------------------------------------------------*/
/* [INPUT] index : 登録インデックス                                     */
/* [RETURN]  完了していたら1 していなかったら0                          */
/* [FUNCTION]  登録したコマンドの実行完了チェック                       */
/************************************************************************/
Sint32 amDrawIsRegistComplete(Sint32 index);

/************************************************************************/
/* char *amDrawMallocDataBuffer(Sint32 size)                            */
/*----------------------------------------------------------------------*/
/* [INPUT] size : 確保するデータ領域のサイズ                            */
/* [RETURN]  確保したデータバッファ                                     */
/* [FUNCTION] ディスプレイリストのデータバッファを確保する              */
/************************************************************************/
char *amDrawMallocDataBuffer(Sint32 size);

/************************************************************************/
/* void amDrawIncDataBuffer(Sint32 size)                                */
/*----------------------------------------------------------------------*/
/* [INPUT] size : 確保するデータ領域のサイズ                            */
/* [FUNCTION] ディスプレイリストのデータバッファのポインタを進める      */
/************************************************************************/
void amDrawIncDataBuffer(Sint32 size);

/************************************************************************/
/* char *amDrawGetDataBuffer(void)                                      */
/*----------------------------------------------------------------------*/
/* [RETURN]  データバッファ                                             */
/* [FUNCTION] ディスプレイリストのデータバッファを取得する              */
/*            データが可変長な場合などに使用する                        */
/************************************************************************/
char *amDrawGetDataBuffer(void);

/************************************************************************/
/* void amDrawSetDataBuffer(char *ptr)                                  */
/*----------------------------------------------------------------------*/
/* [INPUT] ptr : 設定するのデータバッファ                               */
/* [FUNCTION] ディスプレイリストのデータバッファを設定する              */
/*            データが可変長な場合などに使用する                        */
/************************************************************************/
void amDrawSetDataBuffer(char *ptr);

/************************************************************************/
/* char *amDrawMallocWorkBuffer(Sint32 size)                            */
/*----------------------------------------------------------------------*/
/* [INPUT] size : 確保するワーク領域のサイズ                            */
/* [RETURN]  確保したワーク領域                                         */
/* [FUNCTION] 描画用ワーク領域を確保する                                */
/************************************************************************/
char *amDrawMallocWorkBuffer(Sint32 size);

/************************************************************************/
/* void amDrawIncWorkBuffer(Sint32 size)                                */
/*----------------------------------------------------------------------*/
/* [INPUT] size : 確保するワーク領域のサイズ                            */
/* [FUNCTION] 描画用ワーク領域のポインタを進める                        */
/************************************************************************/
void amDrawIncWorkBuffer(Sint32 size);

/************************************************************************/
/* char *amDrawGetWorkBuffer(void)                                      */
/*----------------------------------------------------------------------*/
/* [RETURN]  ワークバッファ                                             */
/* [FUNCTION] ワークバッファを取得する                                  */
/*            ワークが可変長な場合などに使用する                        */
/************************************************************************/
char *amDrawGetWorkBuffer(void);

/************************************************************************/
/* void amDrawSetWorkBuffer(char *ptr)                                  */
/*----------------------------------------------------------------------*/
/* [INPUT] ptr : 設定するのワークバッファ                               */
/* [FUNCTION] ワークバッファを設定する                                  */
/*            ワークが可変長な場合などに使用する                        */
/************************************************************************/
void amDrawSetWorkBuffer(char *ptr);

/************************************************************************/
/* void amDrawInitState(void)                                           */
/*----------------------------------------------------------------------*/
/* [FUNCTION] 描画環境の初期化                                          */
/************************************************************************/
void amDrawInitState(void);

/************************************************************************/
/* void amDrawPushState(void)                                           */
/*----------------------------------------------------------------------*/
/* [FUNCTION] 描画環境のプッシュ                                        */
/************************************************************************/
void amDrawPushState(void);

/************************************************************************/
/* void amDrawPopState(void)                                            */
/*----------------------------------------------------------------------*/
/* [FUNCTION] 描画環境のポップ                                          */
/************************************************************************/
void amDrawPopState(void);

/************************************************************************/
/* void amDrawSetState(AMS_DRAWSTATE *state)                            */
/*----------------------------------------------------------------------*/
/* [INPUT]  state : 描画環境                                            */
/* [FUNCTION] 描画環境の設定                                            */
/************************************************************************/
void amDrawSetState(AMS_DRAWSTATE *state);

/************************************************************************/
/* AMS_DRAWSTATE *amDrawGetState(AMS_DRAWSTATE *state)                  */
/*----------------------------------------------------------------------*/
/* [OUTPUT]  state : 描画環境                                           */
/* [RETURN]  描画環境（参照のみならこちらで可）                         */
/* [FUNCTION] 描画環境の取得                                            */
/************************************************************************/
AMS_DRAWSTATE *amDrawGetState(AMS_DRAWSTATE *state = NULL);

/************************************************************************/
/* void amDrawExecute(void)                                             */
/*----------------------------------------------------------------------*/
/* [FUNCTION] ディスプレイリストタスクの実行                            */
/************************************************************************/
void amDrawExecute(void);

/************************************************************************/
/* void amDrawExecCommand(Uint32 state, NNF_DRAWOBJ drawflag)           */
/*----------------------------------------------------------------------*/
/* [INPUT] state    : 実行するステート                                  */
/*         drawflag : 描画フラグ                                        */
/* [FUNCTION] ディスプレイリストの実行                                  */
/************************************************************************/
void amDrawExecCommand(Uint32 state, NNF_DRAWOBJ drawflag = 0);

/************************************************************************/
/* void amDrawExecRegist(void)                                          */
/*----------------------------------------------------------------------*/
/* [FUNCTION] 登録リストの実行                                          */
/************************************************************************/
void amDrawExecRegist(void);

/************************************************************************/
/* void amDrawAddSort(AMS_COMMAND_HEADER *command, Sint32 key)          */
/*----------------------------------------------------------------------*/
/* [INPUT] command : 描画コマンドヘッダ                                 */
/*         key     : ソートキー                                         */
/* [FUNCTION] ソートリストへの描画コマンド登録                          */
/************************************************************************/
void amDrawAddSort(AMS_COMMAND_HEADER *command, Sint32 key);

/************************************************************************/
/* void amDrawSetProjection(NNS_MATRIX44 *proj_mtx,                     */
/*                                       NNE_PROJECTION_TYPE proj_type) */
/*----------------------------------------------------------------------*/
/* [INPUT] proj_mtx  : プロジェクションマトリクス                       */
/*         proj_type : プロジェクションタイプ                           */
/* [FUNCTION] プロジェクションマトリクスの設定                          */
/************************************************************************/
void amDrawSetProjection(NNS_MATRIX44 *proj_mtx, NNE_PROJECTION_TYPE proj_type);

/************************************************************************/
/* NNS_MATRIX44 *amDrawGetProjectionMatrix(void)                        */
/*----------------------------------------------------------------------*/
/* [RETURN] プロジェクションマトリクス(read only)                       */
/* [FUNCTION] プロジェクションマトリクスの取得                          */
/************************************************************************/
NNS_MATRIX44 *amDrawGetProjectionMatrix(void);

/************************************************************************/
/* NNE_PROJECTION_TYPE amDrawGetProjectionType(void)                    */
/*----------------------------------------------------------------------*/
/* [RETURN] プロジェクションタイプ(read only)                           */
/* [FUNCTION] プロジェクションタイプの取得                              */
/************************************************************************/
NNE_PROJECTION_TYPE amDrawGetProjectionType(void);

/************************************************************************/
/* void amDrawSetWorldViewMatrix(NNS_MATRIX *mtx)                       */
/*----------------------------------------------------------------------*/
/* [INPUT] mtx : ワールド→ビュー変換マトリクス                         */
/* [FUNCTION] ワールドビュー変換マトリクスの設定                        */
/************************************************************************/
inline void amDrawSetWorldViewMatrix(NNS_MATRIX *mtx)
{
	amThreadCheckSafe(1, "amDrawSetWorldViewMatrix");

	if (mtx == NULL)
		mtx		= amMatrixGetCurrent();

	nnCopyMatrix(&_am_draw_world_view_matrix, mtx);
}

/************************************************************************/
/* NNS_MATRIX *amDrawGetWorldViewMatrix(void)                           */
/*----------------------------------------------------------------------*/
/* [RETURN] ワールド→ビュー変換マトリクスへのポインタ                  */
/* [FUNCTION] ワールドビュー変換マトリクスの取得                        */
/************************************************************************/
inline NNS_MATRIX *amDrawGetWorldViewMatrix(void)
{
	amThreadCheckSafe(1, "amDrawGetWorldViewMatrix");

	return	&_am_draw_world_view_matrix;
}

/************************************************************************/
/* void amDrawSetBGColor(NNS_RGBA_U8 *bgColor)                          */
/*----------------------------------------------------------------------*/
/* [INPUT] bgColor : 背景色                                             */
/* [FUNCTION] 背景色の設定                                              */
/************************************************************************/
inline void amDrawSetBGColor(NNS_RGBA_U8 *bgColor)
{
	amThreadCheckSafe(1, "amDrawSetBGColor");

	_am_draw_bg_color = *bgColor;
}

/************************************************************************/
/* NNS_RGBA_U8* amDrawGetBGColor(void)                                  */
/*----------------------------------------------------------------------*/
/* [RETURN] 背景色へのポインタ                                          */
/* [FUNCTION] 背景色の取得                                              */
/************************************************************************/
inline NNS_RGBA_U8* amDrawGetBGColor(void)
{
	amThreadCheckSafe(1, "amDrawGetBGColor");

	return	&_am_draw_bg_color;
}

/************************************************************************/
/* void amDrawMakeTask(TaskProc proc, Uint16 prio, void *data)          */
/* void amDrawMakeTask(TaskProc proc, Uint16 prio, Uint32 data)         */
/*----------------------------------------------------------------------*/
/* [INPUT] proc : 処理関数                                              */
/*         prio : 優先順位                                              */
/*         data : TCBワークの先頭に格納されるデータへのポインタ(8byte)  */
/*         data : TCBワークの先頭に格納されるデータ(4byte)              */
/* [FUNCTION] 描画タスク生成                                            */
/************************************************************************/
void amDrawMakeTask(TaskProc proc, Uint16 prio, void *data = NULL);
void amDrawMakeTask(TaskProc proc, Uint16 prio, Uint32 data);

/************************************************************************/
/* void amDrawPrintf(Sint32 pos_x, Sint32 pos_y, char *format, ...)     */
/*----------------------------------------------------------------------*/
/* [INPUT] pos_x, pos_y : 表示位置                                      */
/*         format : printf互換の書式                                    */
/* [FUNCTION] デバッグ文字の表示                                        */
/************************************************************************/
void amDrawPrintf(Sint32 pos_x, Sint32 pos_y, char *format, ...);

/************************************************************************/
/* void amDrawPrint(Sint32 pos_x, Sint32 pos_y, char *text)             */
/*----------------------------------------------------------------------*/
/* [INPUT] pos_x, pos_y : 表示位置                                      */
/*         text : 表示文字列                                            */
/* [FUNCTION] デバッグ文字の表示                                        */
/************************************************************************/
void amDrawPrint(Sint32 pos_x, Sint32 pos_y, char *text);

/************************************************************************/
/* void amDrawPrintColor(Uint32 rgba)                                   */
/*----------------------------------------------------------------------*/
/* [INPUT] rgba : 表示文字色                                            */
/* [FUNCTION] デバッグ文字色の設定                                      */
/************************************************************************/
void amDrawPrintColor(Uint32 rgba);

/************************************************************************/
/* void amDrawObject(Uint32 state, NNS_OBJECT *object,                  */
/*                         NNS_TEXLIST *texlist, NNF_DRAWOBJ drawflag,  */
/*                                     NNS_MATERIALCALLBACK_FUNC func)  */
/* void amDrawObject(NNS_OBJECT *object,                                */
/*                         NNS_TEXLIST *texlist, NNF_DRAWOBJ drawflag,  */
/*                                     NNS_MATERIALCALLBACK_FUNC func)  */
/*----------------------------------------------------------------------*/
/* [INPUT] state    : 描画ステート(省略すると描画スレッド用になる)      */
/*         object   : オブジェクト                                      */
/*         texlist  : テクスチャリスト                                  */
/*         drawflag : 描画フラグ                                        */
/*         func     : マテリアルコールバック関数                        */
/* [FUNCTION] オブジェクト描画                                          */
/************************************************************************/
void amDrawObject(Uint32 state, NNS_OBJECT *object, NNS_TEXLIST *texlist,
		NNF_DRAWOBJ drawflag = 0, NNS_MATERIALCALLBACK_FUNC func = NULL);
void amDrawObject(NNS_OBJECT *object, NNS_TEXLIST *texlist,
		NNF_DRAWOBJ drawflag = 0, NNS_MATERIALCALLBACK_FUNC func = NULL);

/************************************************************************/
/* void amDrawObjectMaterialMotion(Uint32 state, NNS_MATMOTOBJ *object, */
/*                         NNS_TEXLIST *texlist, NNF_DRAWOBJ drawflag,  */
/*                                     NNS_MATERIALCALLBACK_FUNC func)  */
/* void amDrawObjectMaterialMotion(NNS_MATMOTOBJ *object,               */
/*                         NNS_TEXLIST *texlist, NNF_DRAWOBJ drawflag,  */
/*                                     NNS_MATERIALCALLBACK_FUNC func)  */
/*----------------------------------------------------------------------*/
/* [INPUT] state    : 描画ステート(省略すると描画スレッド用になる)      */
/*         object   : マテリアルモーションオブジェクト                  */
/*         texlist  : テクスチャリスト                                  */
/*         drawflag : 描画フラグ                                        */
/*         func     : マテリアルコールバック関数                        */
/* [FUNCTION] オブジェクト描画                                          */
/************************************************************************/
void amDrawObjectMaterialMotion(Uint32 state, NNS_MATMOTOBJ *object,
		NNS_TEXLIST *texlist, NNF_DRAWOBJ drawflag = 0,
		NNS_MATERIALCALLBACK_FUNC func = NULL);
void amDrawObjectMaterialMotion(NNS_MATMOTOBJ *object,
		NNS_TEXLIST *texlist, NNF_DRAWOBJ drawflag = 0,
		NNS_MATERIALCALLBACK_FUNC func = NULL);

/************************************************************************/
/* void amDrawObjectSetMaterial()                                       */
/*----------------------------------------------------------------------*/
/* [INPUT] state    : 描画ステート                                      */
/*         object   : オブジェクト                                      */
/*         texlist  : テクスチャリスト                                  */
/*         scale    : スケール                                          */
/*         color    : マテリアルカラー                                  */
/*         u        : テクスチャＵ値                                    */
/*         v        : テクスチャＶ値                                    */
/*         blend    : αブレンド                                        */
/*         drawflag : 描画フラグ                                        */
/* [FUNCTION] マテリアル情報のセット＋オブジェクト描画                  */
/************************************************************************/
void amDrawObjectSetMaterial(Uint32 state, NNS_OBJECT *object, NNS_TEXLIST *texlist,
		NNS_VECTOR* scale, NNS_RGBA color, float u, float v, Sint32 blend, NNF_DRAWOBJ drawflag,
		NNS_MATERIALCALLBACK_FUNC func = NULL);

/************************************************************************/
/* void amDrawMotion(Uint32 state, NNS_MOTION *motion, float frame,     */
/*     NNS_OBJECT *object, NNS_TEXLIST *texlist, NNF_DRAWOBJ drawflag,  */
/*                                     NNS_MATERIALCALLBACK_FUNC func)  */
/* void amDrawMotion(NNS_MOTION *motion, float frame,                   */
/*     NNS_OBJECT *object, NNS_TEXLIST *texlist, NNF_DRAWOBJ drawflag,  */
/*                                     NNS_MATERIALCALLBACK_FUNC func)  */
/*----------------------------------------------------------------------*/
/* [INPUT] state    : 描画ステート(省略すると描画スレッド用になる)      */
/*         motion   : モーション                                        */
/*         frame    : モーションフレーム                                */
/*         object   : オブジェクト                                      */
/*         texlist  : テクスチャリスト                                  */
/*         drawflag : 描画フラグ                                        */
/*         func     : マテリアルコールバック関数                        */
/* [FUNCTION] モーション描画                                            */
/************************************************************************/
void amDrawMotion(Uint32 state, NNS_MOTION *motion, float frame,
		NNS_OBJECT *object, NNS_TEXLIST *texlist, NNF_DRAWOBJ drawflag = 0,
		NNS_MATERIALCALLBACK_FUNC func = NULL);
void amDrawMotion(NNS_MOTION *motion, float frame,
		NNS_OBJECT *object, NNS_TEXLIST *texlist, NNF_DRAWOBJ drawflag = 0,
		NNS_MATERIALCALLBACK_FUNC func = NULL);

/************************************************************************/
/* void amDrawMotionMaterialMotion(Uint32 state, NNS_MOTION *motion,    */
/*                         float frame, NNS_MATMOTOBJ *object,          */
/*                         NNS_TEXLIST *texlist, NNF_DRAWOBJ drawflag,  */
/*                                     NNS_MATERIALCALLBACK_FUNC func)  */
/* void amDrawMotionMaterialMotion(NNS_MOTION *motion,                  */
/*                         float frame, NNS_MATMOTOBJ *object,          */
/*                         NNS_TEXLIST *texlist, NNF_DRAWOBJ drawflag,  */
/*                                     NNS_MATERIALCALLBACK_FUNC func)  */
/*----------------------------------------------------------------------*/
/* [INPUT] state    : 描画ステート(省略すると描画スレッド用になる)      */
/*         motion   : モーション                                        */
/*         frame    : モーションフレーム                                */
/*         object   : マテリアルモーションオブジェクト                  */
/*         texlist  : テクスチャリスト                                  */
/*         drawflag : 描画フラグ                                        */
/*         func     : マテリアルコールバック関数                        */
/* [FUNCTION] モーション描画                                            */
/************************************************************************/
void amDrawMotionMaterialMotion(Uint32 state, NNS_MOTION *motion,
		float frame, NNS_MATMOTOBJ *object, NNS_TEXLIST *texlist,
		NNF_DRAWOBJ drawflag = 0, NNS_MATERIALCALLBACK_FUNC func = NULL);
void amDrawMotionMaterialMotion(NNS_MOTION *motion,
		float frame, NNS_MATMOTOBJ *object, NNS_TEXLIST *texlist,
		NNF_DRAWOBJ drawflag = 0, NNS_MATERIALCALLBACK_FUNC func = NULL);

/************************************************************************/
/* void amDrawPrimitive3D()                                             */
/*----------------------------------------------------------------------*/
/* [INPUT] state    : 描画ステート                                      */
/*         param    : プリミティブ描画設定                              */
/* [FUNCTION] プリミティブ描画（3D）                                    */
/************************************************************************/
void amDrawPrimitive3D(Uint32 state, AMS_PARAM_DRAW_PRIMITIVE* setParam);

/************************************************************************/
/* void amDrawPrim2D()                                                  */
/*----------------------------------------------------------------------*/
/* [INPUT] state    : 描画ステート                                      */
/*         param    : プリミティブ描画設定                              */
/* [FUNCTION] プリミティブ描画（2D）                                    */
/************************************************************************/
void amDrawPrim2D(Uint32 state, AMS_PARAM_DRAW_PRIMITIVE* setParam);

/************************************************************************/
/* void amDrawGetPrimBlendParam()                                       */
/*----------------------------------------------------------------------*/
/* [INPUT] type     : αブレンドタイプ                                  */
/*         param    : プリミティブ描画設定                              */
/* [FUNCTION] プリミティブ描画用αブレンド設定値を取得                  */
/************************************************************************/
void amDrawGetPrimBlendParam(AMDRAWE_BLENDTYPE type, AMS_PARAM_DRAW_PRIMITIVE* setParam);

/************************************************************************/
/* void amDrawSetMaterialDiffuse(Uint32 state,                          */
/*                     NNE_MATCTRLMODE mode, float r, float g, float b) */
/* void amDrawSetMaterialDiffuse(                                       */
/*                     NNE_MATCTRLMODE mode, float r, float g, float b) */
/*----------------------------------------------------------------------*/
/* [INPUT] state   : 描画ステート(省略すると描画スレッド用になる)       */
/*         mode    : マテリアル制御モード(NNE_MATCTRLMODE_OFFでOFF)     */
/*         r, g, b : ディフューズカラー                                 */
/* [FUNCTION] ディフューズカラーの設定                                  */
/************************************************************************/
void amDrawSetMaterialDiffuse(Uint32 state,
		NNE_MATCTRLMODE mode, float r = 1.0f, float g = 1.0f, float b = 1.0f);
void amDrawSetMaterialDiffuse(
		NNE_MATCTRLMODE mode, float r = 1.0f, float g = 1.0f, float b = 1.0f);

/************************************************************************/
/* void amDrawSetMaterialAmbient(Uint32 state,                          */
/*                     NNE_MATCTRLMODE mode, float r, float g, float b) */
/* void amDrawSetMaterialAmbient(                                       */
/*                     NNE_MATCTRLMODE mode, float r, float g, float b) */
/*----------------------------------------------------------------------*/
/* [INPUT] state   : 描画ステート(省略すると描画スレッド用になる)       */
/*         mode    : マテリアル制御モード(NNE_MATCTRLMODE_OFFでOFF)     */
/*         r, g, b : アンビエントカラー                                 */
/* [FUNCTION] アンビエントカラーの設定                                  */
/************************************************************************/
void amDrawSetMaterialAmbient(Uint32 state,
		NNE_MATCTRLMODE mode, float r = 1.0f, float g = 1.0f, float b = 1.0f);
void amDrawSetMaterialAmbient(
		NNE_MATCTRLMODE mode, float r = 1.0f, float g = 1.0f, float b = 1.0f);

/************************************************************************/
/* void amDrawSetMaterialAlpha(Uint32 state,                            */
/*                                   NNE_MATCTRLMODE mode, float alpha) */
/* void amDrawSetMaterialAlpha(NNE_MATCTRLMODE mode, float alpha)       */
/*----------------------------------------------------------------------*/
/* [INPUT] state   : 描画ステート(省略すると描画スレッド用になる)       */
/*         mode    : マテリアル制御モード(NNE_MATCTRLMODE_OFFでOFF)     */
/*         alpha   : α値                                               */
/* [FUNCTION] α値の設定                                                */
/************************************************************************/
void amDrawSetMaterialAlpha(Uint32 state,
		NNE_MATCTRLMODE mode, float alpha = 1.0f);
void amDrawSetMaterialAlpha(NNE_MATCTRLMODE mode, float alpha = 1.0f);

/************************************************************************/
/* void amDrawSetMaterialSpecular(Uint32 state,                         */
/*                     NNE_MATCTRLMODE mode, float r, float g, float b) */
/* void amDrawSetMaterialSpecular(                                      */
/*                     NNE_MATCTRLMODE mode, float r, float g, float b) */
/*----------------------------------------------------------------------*/
/* [INPUT] state   : 描画ステート(省略すると描画スレッド用になる)       */
/*         mode    : マテリアル制御モード(NNE_MATCTRLMODE_OFFでOFF)     */
/*         r, g, b : スペキュラーカラー                                 */
/* [FUNCTION] アンビエントカラーの設定                                  */
/************************************************************************/
void amDrawSetMaterialSpecular(Uint32 state,
		NNE_MATCTRLMODE mode, float r = 1.0f, float g = 1.0f, float b = 1.0f);
void amDrawSetMaterialSpecular(
		NNE_MATCTRLMODE mode, float r = 1.0f, float g = 1.0f, float b = 1.0f);

/************************************************************************/
/* void amDrawSetMaterialEnvMap(Uint32 state,                           */
/*                  NNE_MATCTRL_TEXCOORDSRC texsrc, NNS_MATRIX *texmtx) */
/* void amDrawSetMaterialEnvMap(                                        */
/*                  NNE_MATCTRL_TEXCOORDSRC texsrc, NNS_MATRIX *texmtx) */
/*----------------------------------------------------------------------*/
/* [INPUT] state   : 描画ステート(省略すると描画スレッド用になる)       */
/*         texsrc  : テクスチャ座標ソース(NNE_MATCTRLMODE_OFFでOFF)     */
/*         texmtx  : テクスチャ座標変換マトリクス                       */
/* [FUNCTION] テクスチャ座標変換マトリクスの設定                        */
/************************************************************************/
void amDrawSetMaterialEnvMap(Uint32 state,
		NNE_MATCTRL_TEXCOORDSRC texsrc, NNS_MATRIX *texmtx = NULL);
void amDrawSetMaterialEnvMap(
		NNE_MATCTRL_TEXCOORDSRC texsrc, NNS_MATRIX *texmtx = NULL);

/************************************************************************/
/* void amDrawSetMaterialBlendMode(Uint32 state, NNE_MATCTRL_BLEND mode)*/
/* void amDrawSetMaterialBlendMode(NNE_MATCTRL_BLEND mode)              */
/*----------------------------------------------------------------------*/
/* [INPUT] state   : 描画ステート(省略すると描画スレッド用になる)       */
/*         mode    : フレームバッファブレンドモード                     */
/*                     (NNE_MATCTRLMODE_OFFでOFF)                       */
/* [FUNCTION] フレームバッファブレンドモードの設定                      */
/************************************************************************/
void amDrawSetMaterialBlendMode(Uint32 state, NNE_MATCTRL_BLEND mode);
void amDrawSetMaterialBlendMode(NNE_MATCTRL_BLEND mode);

/************************************************************************/
/* void amDrawSetMaterialTexOffset(Uint32 state,                        */
/*            NNE_TEXSLOT slot, NNE_MATCTRLMODE mode, float u, float v) */
/* void amDrawSetMaterialTexOffset(                                     */
/*            NNE_TEXSLOT slot, NNE_MATCTRLMODE mode, float u, float v) */
/*----------------------------------------------------------------------*/
/* [INPUT] state   : 描画ステート(省略すると描画スレッド用になる)       */
/*         slot    : テクスチャスロット番号(-1ならばすべて)             */
/*         mode    : マテリアル制御モード(NNE_MATCTRLMODE_OFFでOFF)     */
/*         u, v    : テクスチャ座標オフセット                           */
/* [FUNCTION] テクスチャ座標オフセットの設定                            */
/************************************************************************/
void amDrawSetMaterialTexOffset(Uint32 state,
		NNE_TEXSLOT slot, NNE_MATCTRLMODE mode,
		float u = 0.0f, float v = 0.0f);
void amDrawSetMaterialTexOffset(NNE_TEXSLOT slot, NNE_MATCTRLMODE mode,
		float u = 0.0f, float v = 0.0f);

/************************************************************************/
/* void amDrawSetFog(Uint32 state, Sint32 flag)                         */
/* void amDrawSetFog(Sint32 flag)                                       */
/*----------------------------------------------------------------------*/
/* [INPUT] state   : 描画ステート(省略すると描画スレッド用になる)       */
/*         flag    : フォグのON/OFF                                     */
/* [FUNCTION] フォグの設定                                              */
/************************************************************************/
void amDrawSetFog(Uint32 state, Sint32 flag);
void amDrawSetFog(Sint32 flag);

/************************************************************************/
/* void amDrawSetFogColor(Uint32 state, float r, float g, float b)      */
/* void amDrawSetFogColor(float r, float g, float b)                    */
/*----------------------------------------------------------------------*/
/* [INPUT] state   : 描画ステート(省略すると描画スレッド用になる)       */
/*         r, g, b : フォグのカラー                                     */
/* [FUNCTION] フォグカラーの設定                                        */
/************************************************************************/
void amDrawSetFogColor(Uint32 state, float r, float g, float b);
void amDrawSetFogColor(float r, float g, float b);

/************************************************************************/
/* void amDrawSetFogRange(Uint32 state, float near, float far)          */
/* void amDrawSetFogRange(float near, float far)                        */
/*----------------------------------------------------------------------*/
/* [INPUT] state   : 描画ステート(省略すると描画スレッド用になる)       */
/*         near    : フォグの開始範囲                                   */
/*         far     : フォグの終了範囲                                   */
/* [FUNCTION] フォグレンジの設定                                        */
/************************************************************************/
void amDrawSetFogRange(Uint32 state, float near, float far);
void amDrawSetFogRange(float near, float far);

/************************************************************************/
/* void amDrawSetZMode(Uint32 state,                                    */
/*                      NNE_BOOL compare, Sint32 func, NNE_BOOL update) */
/* void amDrawSetZMode(NNE_BOOL compare, Sint32 func, NNE_BOOL update)  */
/*----------------------------------------------------------------------*/
/* [INPUT] state   : 描画ステート(省略すると描画スレッド用になる)       */
/*         compare : Z比較をするかどうか                                */
/*         func    : Z比較関数                                          */
/*         update  : Zバッファを更新するかどうか                        */
/* [FUNCTION] Zバッファ比較・更新モードの設定                           */
/************************************************************************/
void amDrawSetZMode(Uint32 state,
		NNE_BOOL compare, Sint32 func, NNE_BOOL update);
void amDrawSetZMode(NNE_BOOL compare, Sint32 func, NNE_BOOL update);

/************************************************************************/
/* void amDrawPrimitive2D(NNE_PRIM2D_FMT format,                        */
/*                            const void *vtx, Sint32 count, float pri) */
/*----------------------------------------------------------------------*/
/* [INPUT] format : 頂点フォーマット                                    */
/*         type   : トライアングルプリミティブタイプ                    */
/*         vtx    : プリミティブ頂点データへのポインタ                  */
/*         count  : 頂点数                                              */
/*         pri    : プライオリティ                                      */
/* [FUNCTION] 2Dプリミティブ描画                                        */
/************************************************************************/
#if AMD_USE_VIRTUAL_RESOLUTION_2D
void amDrawPrimitive2D(NNE_PRIM2D_FMT format,
		NNE_PRIM_TRIANGLE type,	const void *vtx, Sint32 count, float pri);
#else
#define amDrawPrimitive2D(_format, _type, _vtx, _count, _pri) \
	nnDrawPrimitive2D(_type, _vtx, _count, _pri)
#endif

/************************************************************************/
/* void amDrawPrimitiveLine2D(NNE_PRIM_LINE type,                       */
/*                            const void *vtx, Sint32 count, float pri) */
/*----------------------------------------------------------------------*/
/* [INPUT] type   : ラインプリミティブタイプ                            */
/*         vtx    : プリミティブ頂点データへのポインタ                  */
/*         count  : 頂点数                                              */
/*         pri    : プライオリティ                                      */
/* [FUNCTION] 2Dラインプリミティブ描画                                  */
/************************************************************************/
#if AMD_USE_VIRTUAL_RESOLUTION_2D
void amDrawPrimitiveLine2D(
		NNE_PRIM_LINE type, const void *vtx, Sint32 count, float pri);
#else
#define amDrawPrimitiveLine2D(_type, _vtx, _count, _pri) \
	nnDrawPrimitiveLine2D(_type, _vtx, _count, _pri)
#endif

/************************************************************************/
/* void amDrawPrimitivePoint2D(NNE_PRIM2D_POINT_FMT format,             */
/*                            const void *vtx, Sint32 count, float pri) */
/*----------------------------------------------------------------------*/
/* [INPUT] format : 頂点フォーマット                                    */
/*         vtx    : プリミティブ頂点データへのポインタ                  */
/*         count  : 頂点数                                              */
/*         pri    : プライオリティ                                      */
/* [FUNCTION] 2Dポイントプリミティブ描画                                */
/************************************************************************/
#if AMD_USE_VIRTUAL_RESOLUTION_2D
void amDrawPrimitivePoint2D(NNE_PRIM2D_POINT_FMT format,
		const void *vtx, Sint32 count, float pri);
#else
#define amDrawPrimitivePoint2D(_format, _vtx, _count, _pri) \
	nnDrawPrimitivePoint2D(_vtx, _count, _pri)
#endif

#if _WII
/************************************************************************/
/* void amDrawToonMaterial(NNS_DRAWCALLBACK_VAL *val)                   */
/*----------------------------------------------------------------------*/
/* [INPUT] val   : ドローコールバック変数へのポインタ                   */
/* [FUNCTION] Wiiトゥーン用マテリアルコールバック                       */
/*      メインスレッドの描画コール(amMotionDrawなど)に設定すると        */
/*      トゥーン描画になります                                          */
/*      _am_draw_toonDir にライト方向を設定しておいてください           */
/************************************************************************/
NNE_BOOL amDrawToonMaterial(NNS_DRAWCALLBACK_VAL *val);

/************************************************************************/
/* void amDrawWaterFallMaterial(NNS_DRAWCALLBACK_VAL *val)              */
/*----------------------------------------------------------------------*/
/* [INPUT] val   : ドローコールバック変数へのポインタ                   */
/* [FUNCTION] Wii滝用マテリアルコールバック                             */
/*      メインスレッドの描画コール(amMotionMaterialDraw)に設定すると    */
/*      滝描画になります                                                */
/************************************************************************/
NNE_BOOL amDrawWaterFallMaterial(NNS_DRAWCALLBACK_VAL *val);

/************************************************************************/
/* void amDrawSeaMaterial(NNS_DRAWCALLBACK_VAL *val)                    */
/*----------------------------------------------------------------------*/
/* [INPUT] val   : ドローコールバック変数へのポインタ                   */
/* [FUNCTION] Wii海用マテリアルコールバック                             */
/*      メインスレッドの描画コール(amMotionMaterialDraw)に設定すると    */
/*      海描画になります                                                */
/************************************************************************/
NNE_BOOL amDrawSeaMaterial(NNS_DRAWCALLBACK_VAL *val);
#endif

void amDrawBuildShader(void);


#endif // _AM_DRAW_H_
