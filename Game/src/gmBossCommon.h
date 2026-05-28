// =======================================================================
/*!
  @file	gmBossCommon.h
  @brief ボス共通

  @author Keisuke Tanaka
 				Copyright(c) 2009 Dimps
  $Id: gmBossCommon.h 2 2011-04-11 05:21:26Z thamada $
 */
// =======================================================================
/*
 * $Log$
 */

/*
  用語解説：
  	BMCB = Boss Motion CallBack （ボスモーションコールバック）
	BPLCB = Boss matrix Palette CallBack （ボスマトリクスパレットコールバック）
  	SNM = Store Node Matrix （ノードマトリクス格納）
	CNM	= Control Node Matrix	（ノードマトリクス操作）
 */


/* 重複インクルード回避手法 */

#ifndef GM_BOSS_COMMON_H_
#define GM_BOSS_COMMON_H_

#if	defined(__cplusplus)
extern "C" {
#endif

/*------ Include Files -------------------------------------------------*/
#include "gmFade.h"

/*------ Macros --------------------------------------------------------*/
#define GMD_BS_CMN_CNM_FLAG_INHERIT_SCALE		(1 << 0)	//!< スケール継承フラグ（TRUEの時、ノードの元々のスケール値を使用する。mode=REPLACEかつFLAG_LOCAL_COORDINATEがオフの時のみ有効）
#define GMD_BS_CMN_CNM_FLAG_LOCAL_COORDINATE	(1 << 1)	//!< 指定ノードのローカル座標として解釈

// ヒット側面フラグ
#define GMD_BS_CMN_RECT_HIT_SIDE_LEFT	(1 << 0)	//!< 左側面
#define GMD_BS_CMN_RECT_HIT_SIDE_TOP	(1 << 1)	//!< 上側面
#define GMD_BS_CMN_RECT_HIT_SIDE_RIGHT	(1 << 2)	//!< 右側面
#define GMD_BS_CMN_RECT_HIT_SIDE_BOTTOM	(1 << 3)	//!< 下側面

// ヒット側面フラグマスク
#define GMD_BS_CMN_RECT_HIT_SIDE_H_MASK	(GMD_BS_CMN_RECT_HIT_SIDE_LEFT | \
										 GMD_BS_CMN_RECT_HIT_SIDE_RIGHT)	// 水平方向当たり
#define GMD_BS_CMN_RECT_HIT_SIDE_V_MASK	(GMD_BS_CMN_RECT_HIT_SIDE_TOP | \
										 GMD_BS_CMN_RECT_HIT_SIDE_BOTTOM)	// 垂直方向当たり

/*------ Macro Functions -----------------------------------------------*/
//! オブジェクトワーク取得
#define GMM_BS_OBJ(work)	((OBS_OBJECT_WORK*)work)

/*------ Definitions ---------------------------------------------------*/
// ================================================================
// GMF_BS_CMN_BMCB_FUNC
/*!
  ボスモーションCB関数（メインスレッドでの呼び出し）

  @param	motion			[in]	モーションデータ（計算済み）
  @param	object			[in]	NNオブジェクト
  @param	bmcb_param		[io]	パラメータ
 */
// ================================================================
typedef void (*GMF_BS_CMN_BMCB_FUNC)(const AMS_MOTION *motion, const NNS_OBJECT *object, void *bmcb_param);

//! CNM 操作モード列挙型
typedef enum
{
	GME_BS_CMN_CNM_MODE_REPLACE	= 0,	//!< 書き換え
	GME_BS_CMN_CNM_MODE_MULT_LEFT,		//!< 左から乗算
	GME_BS_CMN_CNM_MODE_MULT_RIGHT,		//!< 右から乗算
	
	GME_BS_CMN_CNM_MODE_MAX
} GME_BS_CMN_CNM_MODE;

typedef struct tag_GMS_BS_CMN_BMCB_LINK	GMS_BS_CMN_BMCB_LINK;

//! ボスモーションコールバック リンクワーク
struct tag_GMS_BS_CMN_BMCB_LINK
{
	/*
	  bmcb_func	== NULL の場合は番兵ノード(i.e. head/tail)
	 */
	GMS_BS_CMN_BMCB_LINK	*next;			//!< 次のBMCBリンク
	GMS_BS_CMN_BMCB_LINK	*prev;			//!< 前のBMCBリンク
	GMF_BS_CMN_BMCB_FUNC	bmcb_func;		//!< ボスモーションコールバック関数
	void					*bmcb_param;	//!< ボスモーションコールバック引数
};

//! ボスモーションコールバック管理
typedef struct tag_GMS_BS_CMN_BMCB_MGR
{
	GMS_BS_CMN_BMCB_LINK	bmcb_head;
	GMS_BS_CMN_BMCB_LINK	bmcb_tail;
} GMS_BS_CMN_BMCB_MGR;


//! SNMノード情報構造体
typedef struct tag_GMS_BS_CMN_SNM_NODE_INFO
{
	Sint32	node_index;			//!< ノード番号（直接書き換え禁止）
	Uint32	reserved[3];
	NNS_MATRIX		node_w_mtx;	//!< ノードワールド変換マトリクス（ここに結果が格納されます）
} GMS_BS_CMN_SNM_NODE_INFO;

//! ノードマトリクス格納処理のパラメータ構造体
typedef struct tag_GMS_BS_CMN_SNM_WORK
{
	GMS_BS_CMN_BMCB_LINK	bmcb_link;	//!< ボスモーションCBリンク
	
	Uint16		reg_node_cnt;		//!< 登録ノード数カウント
	Uint16		reg_node_max;		//!< 最大ノード登録数
	Uint32		reserved[3];
	GMS_BS_CMN_SNM_NODE_INFO	*node_info_list;	//!< ノード情報構造体の配列へのポインタ
} GMS_BS_CMN_SNM_WORK;


//! CNMノード情報構造体
typedef struct tag_GMS_BS_CMN_CNM_NODE_INFO
{
	NNS_MATRIX		node_w_mtx;		//!< ノードに設定したいワールドマトリクス
	Sint32			node_index;		//!< ノード番号
	BOOL			enable;			//!< マトリクス書き込み有効フラグ（TRUEのときのみマトリクスパレットに反映される）
	GME_BS_CMN_CNM_MODE	mode;		//!< 操作モード（default=GME_BS_CMN_CNM_MODE_REPLACE）
	Uint32			flag;
} GMS_BS_CMN_CNM_NODE_INFO;

//! CNM管理ワーク構造体
typedef struct tag_GMS_BS_CMN_CNM_MGR_WORK
{
	Uint16	reg_node_cnt;	//!< 登録ノード数カウント
	Uint16	reg_node_max;	//!< 最大ノード登録数
	
	GMS_BS_CMN_CNM_NODE_INFO	*node_info_list;	//!< ノード情報構造体の配列へのポインタ
} GMS_BS_CMN_CNM_MGR_WORK;

//! ノードマトリクス操作処理のパラメータ構造体
typedef struct tag_GMS_BS_CMN_CNM_PARAM
{
	Uint16		reg_node_cnt;
	Uint16		reserved1[1];
	Uint32		reserved2[3];
	// これ以降 GMS_BS_CMN_CNM_NODE_INFO が登録数分つづく
} GMS_BS_CMN_CNM_PARAM;

//! ノード操作オブジェクト
typedef struct tag_GMS_BS_CMN_NODE_CTRL_OBJECT
{
	GMS_EFFECT_COM_WORK	efct_com;
	GMS_BS_CMN_CNM_MGR_WORK	*cnm_mgr_work;	//!< 対象CNM管理ワーク
	Sint32				cnm_reg_id;			//!< CNM登録ID
	GMS_BS_CMN_SNM_WORK	*snm_work;			//!< 参照SNM（NULLなら不使用）
	Sint32				snm_reg_id;			//!< 参照SNM登録ID
	AMS_QUAT			user_quat;			//!< ユーザ使用クォータニオン（使わなくても良い）
	AMS_VECTOR3			user_ofst;			//!< ユーザ使用オフセット（使わなくても良い）
	Uint32				user_timer;			//!< 汎用タイマ
	NNS_MATRIX			w_mtx;				//!< 設定ワールドマトリクス
	BOOL				is_enable;			//!< 反映フラグ
	void (*proc_update)(OBS_OBJECT_WORK*);	//!< 更新関数
} GMS_BS_CMN_NODE_CTRL_OBJECT;

//! ダメージ点滅ワーク
typedef struct tag_GMS_BS_CMN_DMG_FLICKER_WORK
{
	BOOL	is_active;		//!< アクティブフラグ
	Uint32	cycles;			//!< 周期数
	Uint32	interval_timer;	//!< インターバルタイマ
	Angle32	cur_angle;		//!< 現在の角度
	Float	radius;			//!< モデル半径
	Uint32	reserved[3];
} GMS_BS_CMN_DMG_FLICKER_WORK;

//! 画面フラッシュワーク
typedef struct tag_GMS_CMN_FLASH_SCR_WORK
{
	GMS_FADE_OBJ_WORK	*fade_obj_work;	//!< フェードオブジェクトワーク
	Uint32	active_flag;	//!< アクティブフラグ（最下位bitより、duration→fiの有効フラグ）
	Float	duration_frame;	//!< 停滞フレーム(フラグ 1 << 0)
	Float	fi_frame;		//!< 立ち下りフェードアウトフレーム(フラグ 1 << 1)
	Float	duration_timer;	//!< 停滞タイマ
} GMS_CMN_FLASH_SCR_WORK;

//! 遅延サーチワーク
typedef struct tag_GMS_BS_CMN_DELAY_SEARCH_WORK
{
	VecFx32	*pos_hist_buf;		//!< 履歴記録バッファ
	Sint32	cur_point;			//!< 現在の履歴参照インデックス
	Sint32	hist_num;			//!< 履歴記録可能数
	const OBS_OBJECT_WORK	*targ_obj;	//!< 履歴監視対象オブジェクト
	Sint32	record_cnt;			//!< 記録カウンタ（指定遅延フレームの座標が既に記録されているかを判定するのに使用）
} GMS_BS_CMN_DELAY_SEARCH_WORK;

/*------ External Declarations -----------------------------------------*/

// ############################################################################
// ユーティリティ
// ############################################################################
// =======================================================================
// GmBsCmnGetPlayerObj
/*!
  プレイヤーのオブジェクトワーク取得
  
  @return プレイヤーのオブジェクトワーク(OBS_OBJECT_WORK)
 */
// =======================================================================
inline OBS_OBJECT_WORK* GmBsCmnGetPlayerObj(void)
{
	return (OBS_OBJECT_WORK*)g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P];
}

// =======================================================================
// GmBsCmnIsFinalZoneType
/*!
  ファイナルゾーンタイプチェック
  
  @param obj_work	[in]	ボスのオブジェクト
  
  @retval TRUE	ファイナルゾーンタイプ
  @retval FALSE	通常タイプ
  
  @note
  ボスをファイナルゾーン用として扱うかどうかを判定します。
  通常のボスとファイナルゾーン用とで処理を切り分ける際に使用してください。
  obj_workにはhyenaで配置する際のボスのイベントIDが設定されているオブジェクトを
  指定してください。
 */
// =======================================================================
inline BOOL GmBsCmnIsFinalZoneType(const OBS_OBJECT_WORK *obj_work)
{
	MTM_ASSERT(obj_work);
	UNREFERENCED_PARAMETER(obj_work);
	/*
	  現状ではステージ番号によって判別。
	  今後の仕様変更などによりこの方法が使えなくなった場合には
	  イベントレコードのフラグによる判別に変更する。
	 */
	
	if (GsGetMainSysInfo()->stage_id == GSD_MAIN_STAGE_ID_FINAL_1) {
		return TRUE;
	}
	
	return FALSE;
}

// =======================================================================
// GmBsCmnSetAction
/*!
  アクション設定
  
  @param obj_work	[io]	オブジェクトワーク
  @param act_id		[in]	アクションID
  @param is_repeat	[in]	リピートフラグ
 */
// =======================================================================
inline void GmBsCmnSetAction(OBS_OBJECT_WORK *obj_work, Sint32 act_id, BOOL is_repeat,
							 BOOL is_blend=FALSE)
{
	if (is_blend) {
		ObjDrawObjectActionSet3DNNBlend(obj_work, act_id);
	}
	else {
		ObjDrawObjectActionSet(obj_work, act_id);
	}
	
	if (is_repeat) {
		obj_work->disp_flag	|= OBD_DISP_REPEAT;
	}
	else {
		obj_work->disp_flag	&= ~OBD_DISP_REPEAT;
	}
}

// =======================================================================
// GmBsCmnIsActionEnd
/*!
  アクション再生終了チェック
 
  @param obj_work	[in]	オブジェクトワーク
  
  @retval	TRUE	アクション再生終了
  @retval	FALSE	アクション再生中
 */
// =======================================================================
inline BOOL GmBsCmnIsActionEnd(const OBS_OBJECT_WORK *obj_work)
{
	if (obj_work->disp_flag & OBD_DISP_END) {
		return TRUE;
	}
	else {
		return FALSE;
	}
}

// =======================================================================
// GmBsCmnIsActionEndPrecisely
/*!
  アクション再生終了チェック（厳格チェック）
  
  @param obj_work	[in]	オブジェクトワーク
  
  @retval	TRUE	アクション再生終了
  @retval	FALSE	アクション再生中
  
  @note
  次のフレームで「最終フレーム（=最大フレーム-1）」を超えるかかチェックします。
  objDrawでの通常のOBD_DISP_ENDを立てるかどうかの判定では最終フレーム
  に到達した時点でENDフラグが立つ（かつカレントフレームが最終フレームに設定される）が、
  再生速度が、1.fよりも大きく最大フレームを割り切れない値の場合、
  最終フレームに到達した時のフレームが再生速度に見合わない余分なフレームとなるため、
  最終フレームを超える直前のフレームを最終フレームとみなして判定を行います。
  （e.g. 最大フレームが12で再生速度が5の場合、0→5→10→11となり、11の時にENDフラグが立つため、
  ENDフラグが立つまで再生していると実質の再生速度が「1」の箇所が発生してしまう。
  本関数で判定した場合、10の時点でTRUEが返る。）
 */
// =======================================================================
extern BOOL GmBsCmnIsActionEndPrecisely(const OBS_OBJECT_WORK *obj_work);

// =======================================================================
// GmBsCmnIsActionEndFlexibly
/*!
  アクション再生終了柔軟チェック
  
  @param obj_work		[in]	オブジェクトワーク
  @param allow_ratio	[in]	フレーム超過許容率(0.f～1.f)
  
  @retval	TRUE	アクション再生終了
  @retval	FALSE	アクション再生中
  
  @note
  次の更新でモーションフレームが最終フレームを超える場合、
  超過分のフレーム数が「許容フレーム数（=再生速度*allow_ratio）」を超えているか否かに応じて
  手法を切り替えて終了チェックを行います。
  許容フレーム数以内の場合は通常の判定（GmBsCmnIsActionEnd）を使用し、
  許容フレーム数を超える場合は厳格判定（GmBsCmnIsActionEndPrecisely）を使用します。
  （e.g. 再生速度8.fの時、 実質再生速度が2.f以下になるのを避けたい場合
  →allow_ratio=0.75fで本関数を使用して判定すればよい。
  最終フレームで実質再生速度が2フレーム以下（超過フレーム6超過）になってしまう場合は
  ENDフラグが立つ直前のフレームでTRUE。
  最終フレームで実質再生速度が2超過（超過フレーム6以下）となる場合はENDフラグが立った時点でTRUE。）
 */
// =======================================================================
extern BOOL GmBsCmnIsActionEndFlexibly(const OBS_OBJECT_WORK *obj_work, Float allow_ratio);

// =======================================================================
// GmBsCmnSetObjSpd
/*!
  オブジェクト速度設定
 
  @param obj_work	[io]	オブジェクトワーク
  @param spd_x		[in]	速度X
  @param spd_y		[in]	速度Y
  @param spd_z		[in]	速度Z
 */
// =======================================================================
inline void GmBsCmnSetObjSpd(OBS_OBJECT_WORK *obj_work, fx32 spd_x, fx32 spd_y, fx32 spd_z=0)
{
	obj_work->spd.x	= spd_x;
	obj_work->spd.y	= spd_y;
	obj_work->spd.z	= spd_z;
}

// =======================================================================
// GmBsCmnSetObjSpdZero
/*!
  オブジェクト速度・加速度を０に設定
 
  @param obj_work	[io]	オブジェクトワーク
 */
// =======================================================================
inline void GmBsCmnSetObjSpdZero(OBS_OBJECT_WORK *obj_work)
{
	obj_work->spd.x	=
		obj_work->spd.y	=
			obj_work->spd.z	=	0;
	obj_work->spd_add.x	=
		obj_work->spd_add.y	=
			obj_work->spd_add.z	=	0;
}

// =======================================================================
// GmBsCmnSetEfctAtkVsPly
/*!
  対プレイヤー攻撃用のオブジェクトとしてエフェクトを設定
  
  @param efct_com		[io]	エフェクト共通ワーク
  @param view_out_ofst	[in]	クリッピングオフセット
  
  @note
  対プレイヤーにヒットするように矩形設定を行っています。
  画面外でクリッピングされるように設定が行われます。
  クリッピング不要の場合は改めてOBD_OBJECT_NOCLIPを設定してください。
  地形当たりはオフに設定されます。
 */
// =======================================================================
extern void GmBsCmnSetEfctAtkVsPly(GMS_EFFECT_COM_WORK *efct_com, Sint16 view_out_ofst);

// =======================================================================
// GmBsCmnCheckRectMajorOverlapH
/*!
  矩形水平方向広範囲重なりチェック
  
  @param my_rect		[in]	自分の矩形
  @param your_rect		[in]	相手矩形
  @param center_ofst_x	[out]	相手の矩形中心の、自分矩形中心からのオフセットX(NULL可)
  
  @retval TRUE	広範囲に重なっている
  @retval FALSE	広範囲には重なっていないor全く重なっていない
  
  @note
  片方の矩形の半分以上が他方の矩形に重なっているか（水平方向のみ）チェックします。
  対象の2つの矩形をmy_rect,your_rectのどちらに指定するかにより、
  center_ofst_xに格納される値の符号に影響します。
  （この場合の「矩形中心」は、矩形の幾何学的な中心であり、
  親の座標とOBS_RECT::posから求められる矩形座標ではありません。）
 */
// =======================================================================
extern BOOL GmBsCmnCheckRectMajorOverlapH(const OBS_RECT_WORK *my_rect, const OBS_RECT_WORK *your_rect,
										  fx32 *center_ofst_x);

// =======================================================================
// GmBsCmnCheckRectMajorOverlapV
/*!
  矩形垂直方向広範囲重なりチェック
  
  @param my_rect		[in]	自分の矩形
  @param your_rect		[in]	相手矩形
  @param center_ofst_y	[out]	相手の矩形中心の、自分矩形中心からのオフセットY(NULL可)
  
  @retval TRUE	広範囲に重なっている
  @retval FALSE	広範囲には重なっていないor全く重なっていない
  
  @note
  片方の矩形の半分以上が他方の矩形に重なっているか（垂直方向のみ）チェックします。
  対象の2つの矩形をmy_rect,your_rectのどちらに指定するかにより、
  center_ofst_yに格納される値の符号に影響します。
  （この場合の「矩形中心」は、矩形の幾何学的な中心であり、
  親の座標とOBS_RECT::posから求められる矩形座標ではありません。）
 */
// =======================================================================
extern BOOL GmBsCmnCheckRectMajorOverlapV(const OBS_RECT_WORK *my_rect, const OBS_RECT_WORK *your_rect,
										  fx32 *center_ofst_y);

// =======================================================================
// GmBsCmnCheckRectHitSideHFirst
/*!
  矩形ヒット側面チェック
  
  @param my_rect	[in]	自分の矩形（この矩形の側面が判定対象となる）
  @param your_rect	[in]	相手の矩形
  
  @return ヒット側面フラグ(GMD_BS_CMN_RECT_HIT_SIDE_XXX)
  
  @note
  your_rectがmy_rectにヒットした際に、ヒットしたのがmy_rectのどの側面かを判定します。
  既に互いにヒットしている矩形である前提で判定を行うため、実際に重なっているかはチェックしません。
  実際に重なり合っていない矩形同士の場合、XY成分のうち、重なりのない方向については、
  その方向成分における最近傍の側面同士が接している（＝最小幅で重なっている）のと同等の扱いとなります。
  判定結果は上・下・左・右のいずれか1つです。
  本関数は矩形の角の判定において左・右側面を優先します。
 */
// =======================================================================
extern Uint32 GmBsCmnCheckRectHitSideHFirst(const OBS_RECT_WORK *my_rect, const OBS_RECT_WORK *your_rect);

// =======================================================================
// GmBsCmnCheckRectHitSideVFirst
/*!
  矩形ヒット側面チェック
  
  @param my_rect	[in]	自分の矩形（この矩形の側面が判定対象となる）
  @param your_rect	[in]	相手の矩形
  
  @return ヒット側面フラグ(GMD_BS_CMN_RECT_HIT_SIDE_XXX)
  
  @note
  your_rectがmy_rectにヒットした際に、ヒットしたのがmy_rectのどの側面かを判定します。
  既に互いにヒットしている矩形である前提で判定を行うため、実際に重なっているかはチェックしません。
  実際に重なり合っていない矩形同士の場合、XY成分のうち、重なりのない方向については、
  その方向成分における最近傍の側面同士が接している（＝最小幅で重なっている）のと同等の扱いとなります。
  判定結果は上・下・左・右のいずれか1つです。
  本関数は矩形の角の判定において上・下側面を優先します。
 */
// =======================================================================
extern Uint32 GmBsCmnCheckRectHitSideVFirst(const OBS_RECT_WORK *my_rect, const OBS_RECT_WORK *your_rect);


// ############################################################################
// ボスモーションコールバックシステム
// ############################################################################
// =======================================================================
// GmBsCmnInitBossMotionCBSystem
/*!
  ボスモーションコールバックシステム初期化
  
  @param obj_work	[io]	オブジェクトワーク
  @param bmcb_mgr	[io]	BMCB管理ワーク
  
  @note
  ボスモーションコールバックシステムを初期化します。
  bmcb_mgrは使用者側で確保・管理してください。
 */
// =======================================================================
extern void GmBsCmnInitBossMotionCBSystem(OBS_OBJECT_WORK *obj_work, GMS_BS_CMN_BMCB_MGR *bmcb_mgr);

// =======================================================================
// GmBsCmnClearBossMotionCBSystem
/*!
  ボスモーションコールバックシステム解除
 
  @param obj_work	[io]	オブジェクトワーク
  
  @note
  ボスモーションコールバックシステムを解除・設定クリアします。
  バッファの解放などは発生しません。
 */
// =======================================================================
extern void GmBsCmnClearBossMotionCBSystem(OBS_OBJECT_WORK *obj_work);

// =======================================================================
// GmBsCmnAppendBossMotionCallback
/*!
  ボスモーションコールバックを実行リストに追加
  
  @param bmcb_mgr	[io]	BMCB管理ワーク
  @param bmcb_link	[io]	BMCBリンク
  
  @note
  ボスモーションCBの実行リストにBMCBリンクを追加します。
  bmcb_linkはCB関数・パラメータが設定済みである必要があります。
 */
// =======================================================================
extern void GmBsCmnAppendBossMotionCallback(GMS_BS_CMN_BMCB_MGR *bmcb_mgr, GMS_BS_CMN_BMCB_LINK *bmcb_link);


// ############################################################################
// ノードマトリクス格納処理
// ############################################################################

// =======================================================================
// GmBsCmnCreateSNMWork
/*!
  ノードマトリクス格納処理ワークを作成・初期化
 
  @param snm_work	[io]	ノードマトリクス格納処理のワーク構造体へのポインタ
  @param object		[in]	NNオブジェクト
  @param reg_max	[in]	最大登録数
 
  @note
  内部でバッファを確保します。
  不要になったら必ずGmBsCmnDeleteSNMWork()で解放を行ってください。
 */
// =======================================================================
extern void GmBsCmnCreateSNMWork(GMS_BS_CMN_SNM_WORK *snm_work,
								 const NNS_OBJECT *object, Uint16 reg_max);

// =======================================================================
// GmBsCmnDeleteSNMWork
/*!
  ノードマトリクス格納処理ワーク削除
 
  @param snm_work	[io]	ノードマトリクス格納処理のワーク構造体へのポインタ
 
  @note
  登録されたバッファを解放します。
  解放済みの場合は何もしません。
 */
// =======================================================================
extern void GmBsCmnDeleteSNMWork(GMS_BS_CMN_SNM_WORK *snm_work);

// =======================================================================
// GmBsCmnRegisterSNMNode
/*!
  ノードマトリクス格納処理 対象ノード登録
 
  @param snm_work	[io]	ノードマトリクス格納処理のワーク構造体へのポインタ
  @param node_index	[in]	対象ノードインデックス
  
  @return SNM登録ID（格納されたマトリクスを取得する際に必要になります）
  
  @note
  マトリクス格納対象となるノードを登録します。
  登録最大数を超える場合はアサートします。
  重複チェックは行いません。
  登録解除はサポートしていません。
 */
// =======================================================================
extern Sint32 GmBsCmnRegisterSNMNode(GMS_BS_CMN_SNM_WORK *snm_work, Sint32 node_index);

// =======================================================================
// GmBsCmnGetSNMMtx
/*!
  ノードマトリクス格納処理 格納済みマトリクスへのポインタ取得
 
  @param snm_work	[in]	ノードマトリクス格納処理のワーク構造体へのポインタ
  @param snm_reg_id	[in]	対象ノードの登録ID
 
  @note
  格納済みのマトリクスのうち、指定の登録IDのマトリクスへのポインタを取得します。
 */
// =======================================================================
extern NNS_MATRIX* GmBsCmnGetSNMMtx(const GMS_BS_CMN_SNM_WORK *snm_work, Sint32 snm_reg_id);

// =======================================================================
// GmBsCmnUpdateObjectGeneralStuckWithNode
/*!
  オブジェクトのノード追随（オブジェクト汎用）
  
  @param obj_work	[io]	オブジェクトワーク
  @param snm_work	[in]	SNMワーク
  @param snm_reg_id	[in]	SNM登録ID
  @param ofst_mtx	[in]	オフセット変換マトリクス（ワールド座標系スケール, OBS_OBJECT_WORK::scaleは無視）
  
  @note
  ノード座標（平行移動）のみ反映されます。
  描画オブジェクト(obj_3d, obj_3des等)を参照せず、
  OBS_OBJECT_WORKのメンバのみ操作するため、どの描画タイプのオブジェクトでも使用可能です。
  ノードのマトリクスは前フレームに計算されたものであるため、
  設定される座標が追随先のノードよりも1フレーム遅れることに留意してください。
 */
// =======================================================================
extern void GmBsCmnUpdateObjectGeneralStuckWithNode(OBS_OBJECT_WORK *obj_work,
													const GMS_BS_CMN_SNM_WORK *snm_work,
													Sint32 snm_reg_id,
													const NNS_MATRIX *ofst_mtx=NULL);

// =======================================================================
// GmBsCmnUpdateObjectGeneralStuckWithNodeRelative
/*!
  オブジェクトのノード追随 相対座標配置（オブジェクト汎用）
  
  @param obj_work		[io]	オブジェクトワーク
  @param snm_work		[in]	SNMワーク
  @param snm_reg_id		[in]	SNM登録ID
  @param pivot_cur_pos	[in]	現在の基準座標
  @param pivot_prev_pos	[in]	前フレームでの基準座標
  @param ofst_mtx		[in]	オフセット変換マトリクス（ワールド座標系スケール, OBS_OBJECT_WORK::scaleは無視）
  
  @note
  GmBsCmnUpdateObjectGeneralStuckWithNode() の相対座標維持版です。
  ノードのワールド座標（1フレーム前）をそのまま採用せず、
  指定基準座標の前フレームからの変化分を加算することで
  前フレームでの「基準座標とノード座標の相対位置」が今フレームでも維持されるように調整します。
  これにより、例えば指定基準座標(pivot_**_pos)に追随対象のオブジェクトの座標を指定することで、
  オブジェクトの移動によって生じる1フレームのノード座標ズレを解消することができます。
  （モーション再生によって生じる1フレームのズレは依然残ります。）
 */
// =======================================================================
extern void GmBsCmnUpdateObjectGeneralStuckWithNodeRelative(OBS_OBJECT_WORK *obj_work,
															const GMS_BS_CMN_SNM_WORK *snm_work,
															Sint32 snm_reg_id,
															const VecFx32 *pivot_cur_pos,
															const VecFx32 *pivot_prev_pos,
															const NNS_MATRIX *ofst_mtx=NULL);

// =======================================================================
// GmBsCmnUpdateObject3DNNStuckWithNode
/*!
  オブジェクトのノード追随（3DNN専用）
  
  @param obj_work	[io]	オブジェクトワーク（obj_3dが設定されている必要有り）
  @param snm_work	[in]	SNMワーク
  @param snm_reg_id	[in]	SNM登録ID
  @param b_rotation	[in]	回転反映フラグ（TRUE:回転を反映, FALSE:回転反映しない）
  @param ofst_mtx	[in]	オフセット変換マトリクス（ワールド座標系スケール, OBS_OBJECT_WORK::scaleは無視）
  
  @note
  関数内ではOBD_DISP_USERMTX_RIGHTフラグのオン・オフ設定や
  OBS_OBJECT_WORK::obj_3d->user_obj_mtx_r の書き換えが
  強制的に行なわれますのでご注意ください。
  b_rotationを有効にするとノードの回転成分が反映されます。
  回転を反映する場合はOBS_OBJECT_WORK::dir の設定や OBD_DISP_NODIRFLIP
  の有無に気を付けてください。
  ノードのマトリクスは前フレームに計算されたものであるため、
  姿勢・座標が、追随先のノードよりも1フレーム遅れることに留意してください。
 */
// =======================================================================
extern void GmBsCmnUpdateObject3DNNStuckWithNode(OBS_OBJECT_WORK *obj_work,
												 const GMS_BS_CMN_SNM_WORK *snm_work,
												 Sint32 snm_reg_id,
												 BOOL b_rotation,
												 const NNS_MATRIX *ofst_mtx=NULL);

// =======================================================================
// GmBsCmnUpdateObject3DNNStuckWithNodeRelative
/*!
  オブジェクトのノード追随 相対座標配置（3DNN専用）
  
  @param obj_work		[io]	オブジェクトワーク（obj_3dが設定されている必要有り）
  @param snm_work		[in]	SNMワーク
  @param snm_reg_id		[in]	SNM登録ID
  @param b_rotation		[in]	回転反映フラグ（TRUE:回転を反映, FALSE:回転反映しない）
  @param pivot_cur_pos	[in]	現在の基準座標
  @param pivot_prev_pos	[in]	前フレームでの基準座標
  @param ofst_mtx		[in]	オフセット変換マトリクス（ワールド座標系スケール, OBS_OBJECT_WORK::scaleは無視）
  
  @note
  GmBsCmnUpdateObject3DNNStuckWithNode() の相対座標維持版です。
  ノードのワールド座標（1フレーム前）をそのまま採用せず、
  指定基準座標の前フレームからの変化分を加算することで
  前フレームでの「基準座標とノード座標の相対位置」が今フレームでも維持されるように調整します。
  これにより、例えば指定基準座標(pivot_**_pos)に追随対象のオブジェクトの座標を指定することで、
  オブジェクトの移動によって生じる1フレームのノード座標ズレを解消することができます。
  （モーション再生によって生じる1フレームのズレは依然残ります。）
 */
// =======================================================================
extern void GmBsCmnUpdateObject3DNNStuckWithNodeRelative(OBS_OBJECT_WORK *obj_work,
														 const GMS_BS_CMN_SNM_WORK *snm_work,
														 Sint32 snm_reg_id,
														 BOOL b_rotation,
														 const VecFx32 *pivot_cur_pos,
														 const VecFx32 *pivot_prev_pos,
														 const NNS_MATRIX *ofst_mtx=NULL);

// =======================================================================
// GmBsCmnUpdateObject3DESStuckWithNode
/*!
  オブジェクトのノード追随（3DES専用）
  
  @param obj_work	[io]	オブジェクトワーク（obj_3desが設定されている必要有り）
  @param snm_work	[in]	SNMワーク
  @param snm_reg_id	[in]	SNM登録ID
  @param b_rotation	[in]	回転反映フラグ（TRUE:回転を反映, FALSE:回転反映しない）
  @param ofst_mtx	[in]	オフセット変換マトリクス（ワールド座標系スケール, OBS_OBJECT_WORK::scaleは無視）
  
  @note
  関数内ではOBD_ACTFLAG_3D_ES_USER_DIR_QUATフラグのオン・オフ設定や
  OBS_OBJECT_WORK::obj_3des->user_dir_quat の書き換えが
  強制的に行なわれますのでご注意ください。
  b_rotationを有効にするとノードの回転成分が反映されます。
  回転を反映する場合はOBS_OBJECT_WORK::dir の設定や OBD_DISP_NODIRFLIP
  の有無に気を付けてください。
  ノードのマトリクスは前フレームに計算されたものであるため、
  姿勢・座標が、追随先のノードよりも1フレーム遅れることに留意してください。
 */
// =======================================================================
extern void GmBsCmnUpdateObject3DESStuckWithNode(OBS_OBJECT_WORK *obj_work,
												 const GMS_BS_CMN_SNM_WORK *snm_work,
												 Sint32 snm_reg_id,
												 BOOL b_rotation,
												 const NNS_MATRIX *ofst_mtx=NULL);

// =======================================================================
// GmBsCmnUpdateObject3DESStuckWithNodeRelative
/*!
  オブジェクトのノード追随 相対座標配置（3DES専用）
  
  @param obj_work		[io]	オブジェクトワーク（obj_3desが設定されている必要有り）
  @param snm_work		[in]	SNMワーク
  @param snm_reg_id		[in]	SNM登録ID
  @param b_rotation		[in]	回転反映フラグ（TRUE:回転を反映, FALSE:回転反映しない）
  @param pivot_cur_pos	[in]	現在の基準座標
  @param pivot_prev_pos	[in]	前フレームでの基準座標
  @param ofst_mtx		[in]	オフセット変換マトリクス（ワールド座標系スケール, OBS_OBJECT_WORK::scaleは無視）
  
  @note
  GmBsCmnUpdateObject3DESStuckWithNode() の相対座標維持版です。
  ノードのワールド座標（1フレーム前）をそのまま採用せず、
  指定基準座標の前フレームからの変化分を加算することで
  前フレームでの「基準座標とノード座標の相対位置」が今フレームでも維持されるように調整します。
  これにより、例えば指定基準座標(pivot_**_pos)に追随対象のオブジェクトの座標を指定することで、
  オブジェクトの移動によって生じる1フレームのノード座標ズレを解消することができます。
  （モーション再生によって生じる1フレームのズレは依然残ります。）
 */
// =======================================================================
extern void GmBsCmnUpdateObject3DESStuckWithNodeRelative(OBS_OBJECT_WORK *obj_work,
														 const GMS_BS_CMN_SNM_WORK *snm_work,
														 Sint32 snm_reg_id,
														 BOOL b_rotation,
														 const VecFx32 *pivot_cur_pos,
														 const VecFx32 *pivot_prev_pos,
														 const NNS_MATRIX *ofst_mtx=NULL);



// ############################################################################
// ノードマトリクス操作処理
// ############################################################################

// =======================================================================
// GmBsCmnInitCNMCb
/*!
  ノードマトリクス操作処理 初期化
  
  @param obj_work		[io]	オブジェクトワーク
  @param cnm_mgr_work	[io]	ノードマトリクス操作処理の管理ワーク構造体へのポインタ
	  							（GmBsCmnCreateCNMMgrWork()で初期化しておくこと）
  
  @note
  obj_3dにコールバック関数とパラメータを設定します。
  cnm_paramは事前に初期化しておいてください。
 */
// =======================================================================
extern void GmBsCmnInitCNMCb(OBS_OBJECT_WORK *obj_work, GMS_BS_CMN_CNM_MGR_WORK *cnm_mgr_work);

// =======================================================================
// GmBsCmnClearCNMCb
/*!
  ノードマトリクス操作処理 解放
 
  @param obj_work	[io]	オブジェクトワーク
  
  @note
  obj_3dに設定されたコールバック関数とパラメータをクリアします。
 */
// =======================================================================
extern void GmBsCmnClearCNMCb(OBS_OBJECT_WORK *obj_work);

// =======================================================================
// GmBsCmnCreateCNMMgrWork
/*!
  ノードマトリクス操作処理管理ワークを作成・初期化
  
  @param cnm_mgr_work	[io]	ノードマトリクス操作処理の管理ワーク構造体へのポインタ
  @param object			[in]	NNオブジェクト
  @param reg_max		[in]	最大登録数
  
  @note
  内部でバッファを確保します。
  不要になったら必ずGmBsCmnDeleteCNMMgrWork()で解放を行ってください。
 */
// =======================================================================
extern void GmBsCmnCreateCNMMgrWork(GMS_BS_CMN_CNM_MGR_WORK *cnm_mgr_work,
									const NNS_OBJECT *object, Uint16 reg_max);

// =======================================================================
// GmBsCmnDeleteCNMMgrWork
/*!
  ノードマトリクス操作処理管理ワーク削除
  
  @param cnm_mgr_work	[io]	CNM管理ワーク
  
  @note
  ワークに登録されたバッファを開放します。
  cnm_mgr_work自体は解放しません。
 */
// =======================================================================
extern void GmBsCmnDeleteCNMMgrWork(GMS_BS_CMN_CNM_MGR_WORK *cnm_mgr_work);

// =======================================================================
// GmBsCmnUpdateCNMParam
/*!
  ノードマトリクス操作処理 マトリクスパレットCBパラメータ更新
  （メインスレッド側から呼び出し）
  
  @param obj_work		[io]	オブジェクトワーク
  @param cnm_mgr_work	[in]	CNM管理ワーク
  
  @note
  描画スレッド側に渡すパラメータを作成してobj_3dにセットします。
  毎フレーム呼び出してください。
 */
// =======================================================================
extern void GmBsCmnUpdateCNMParam(OBS_OBJECT_WORK *obj_work, const GMS_BS_CMN_CNM_MGR_WORK *cnm_mgr_work);

// =======================================================================
// GmBsCmnRegisterCNMNode
/*!
  ノードマトリクス操作処理 対象ノード登録
 
  @param cnm_mgr_work	[io]	ノードマトリクス操作処理のワーク構造体へのポインタ
  @param node_index		[in]	対象ノードインデックス
  
  @return CNM登録ID（書き込みたいマトリクスの設定場所を取得する際に必要になります。）
  
  @note
  マトリクス操作対象となるノードを登録します。
  登録最大数を超える場合はアサートします。
  重複チェックは行いません。
  登録解除はサポートしていません。
 */
// =======================================================================
extern Sint32 GmBsCmnRegisterCNMNode(GMS_BS_CMN_CNM_MGR_WORK *cnm_mgr_work, Sint32 node_index);

// =======================================================================
// GmBsCmnSetCNMMtx
/*!
  ノードマトリクス操作処理 書き込み予定マトリクス設定
  
  @param cnm_mgr_work	[io]	CNM管理ワーク
  @param w_mtx			[in]	反映させたいワールドマトリクス
  @param cnm_reg_id		[in]	対象ノードインデックス
  @param enables		[in]	有効化フラグ（デフォルト=FALSE）
  
  @note
  ノードに反映させたいワールドマトリクスを設定します。
  enablesをTRUE指定するとマトリクスパレットへの反映の有効化も行われます。
  FALSEに指定しても無効化はされません。
  無効化する場合は別途 GmBsCmnEnableCNMMtxNode(..., FALSE) を呼び出してください。
 */
// =======================================================================
extern void GmBsCmnSetCNMMtx(GMS_BS_CMN_CNM_MGR_WORK *cnm_mgr_work, const NNS_MATRIX *w_mtx,
							 Sint32 cnm_reg_id, BOOL enables=FALSE);

// =======================================================================
// GmBsCmnChangeCNMModeNode
/*!
  ノードマトリクス操作処理 ノードの操作モード変更
  
  @param cnm_mgr_work	[io]	CNM管理ワーク
  @param cnm_reg_id		[in]	対象ノードの登録ID
  @param mode			[in]	操作モード
  
  @note
  操作モードを変更します。デフォルトではGME_BS_CMN_CNM_MODE_REPLACEに設定されていますので、
  変更が必要な場合以外は呼び出す必要はありません。
 */
// =======================================================================
extern void GmBsCmnChangeCNMModeNode(GMS_BS_CMN_CNM_MGR_WORK *cnm_mgr_work,
									 Sint32 cnm_reg_id, GME_BS_CMN_CNM_MODE mode);

// =======================================================================
// GmBsCmnEnableCNMLocalCoordinate
/*!
  ノードマトリクス操作処理 ローカル座標系解釈設定
  
  @param cnm_mgr_work	[io]	CNM管理ワーク
  @param cnm_reg_id		[in]	対象ノードの登録ID
  @param enbale			[in]	有効（無効）フラグ（TRUE:有効, FALSE:無効）
  
  @note
  有効に設定すると、ノードに設定予定のマトリクスを
  対象ノードのローカル座標系での座標変換マトリクスとして解釈し、
  対象ノードのワールドマトリクスに変換した上で、それを各モードに応じた処理で書き込みます。
  （すなわち、モーション適用後の対象ノードを原点と解釈して座標変換を行います。）
  使用例：特定のノードへの操作を、その子ノードにも継承させたい場合などに使用できます。
          （対象の親・子ノードについて、全てローカル座標系解釈設定をオンにします）
 */
// =======================================================================
extern void GmBsCmnEnableCNMLocalCoordinate(GMS_BS_CMN_CNM_MGR_WORK *cnm_mgr_work,
											Sint32 cnm_reg_id, BOOL enable);

// =======================================================================
// GmBsCmnEnableCNMInheritNodeScale
/*!
  ノードマトリクス操作処理 ノードスケール継承設定
  
  @param cnm_mgr_work	[io]	CNM管理ワーク
  @param cnm_reg_id		[in]	対象ノードの登録ID
  @param enable			[in]	有効（無効）フラグ（TRUE:有効, FALSE:無効）
  
  @note
  enableをTRUE指定するとノードの元のスケールを引き継いだ上でw_mtxを乗算します。
  姿勢を変えたいが大きさは維持したい場合等に活用してください。
  （デフォルトではオフになっています。）
 */
// =======================================================================
extern void GmBsCmnEnableCNMInheritNodeScale(GMS_BS_CMN_CNM_MGR_WORK *cnm_mgr_work,
											 Sint32 cnm_reg_id, BOOL enable);

// =======================================================================
// GmBsCmnEnableCNMMtxNode
/*!
  ノードマトリクス操作処理 マトリクス反映有効設定
  
  @param cnm_mgr_work	[io]	CNM管理ワーク
  @param cnm_reg_id		[in]	対象ノードの登録ID
  @param enable			[in]	有効（無効）フラグ（TRUE:有効, FALSE:無効）
  
  @note
  指定登録IDのマトリクスのマトリクスパレットへの反映の有効・無効を設定します。
 */
// =======================================================================
extern void GmBsCmnEnableCNMMtxNode(GMS_BS_CMN_CNM_MGR_WORK *cnm_mgr_work,
									Sint32 cnm_reg_id, BOOL enable);

// =======================================================================
// GmBsCmnCreateNodeControlObjectBySize
/*!
  ノード操作オブジェクト生成
  
  @param parent_obj		[io]	親オブジェクト
  @param cnm_mgr_work	[io]	CNM管理ワーク
  @param cnm_reg_id		[in]	CNM登録ID
  @param snm_work		[io]	SNMワーク（NULL可）
  @param snm_reg_id		[in]	SNM登録ID
  @param work_size		[in]	ワークサイズ
  
  @return ノード操作オブジェクトワーク(GMS_BS_CMN_NODE_CTRL_OBJECT)
  
  @note
  OBS_OBJECT_WORK::ppFuncは書き換えないでください。
  更新処理は GMS_BS_CMN_NODE_CTRL_OBJECT::proc_update に設定してください。
  GMS_BS_CMN_NODE_CTRL_OBJECT::w_mtx を設定するとCNM管理ワークに自動的に反映されます。
  初期状態ではマトリクスパレットへの反映がオフになっています。
  is_enable メンバにTRUEを設定することで反映がオンになります。
 */
// =======================================================================
extern GMS_BS_CMN_NODE_CTRL_OBJECT* GmBsCmnCreateNodeControlObjectBySize(OBS_OBJECT_WORK *parent_obj,
																		 GMS_BS_CMN_CNM_MGR_WORK *cnm_mgr_work,
																		 Sint32 cnm_reg_id,
																		 GMS_BS_CMN_SNM_WORK *snm_work,
																		 Sint32 snm_reg_id,
																		 Uint32 work_size);

// =======================================================================
// GmBsCmnCreateNodeControlObject
/*!
  ノード操作オブジェクト生成
  
  @param parent_obj		[io]	親オブジェクト
  @param cnm_mgr_work	[io]	CNM管理ワーク
  @param cnm_reg_id		[in]	CNM登録ID
  @param snm_work		[io]	SNMワーク（NULL可）
  @param snm_reg_id		[in]	SNM登録ID
  
  @return ノード操作オブジェクトワーク(GMS_BS_CMN_NODE_CTRL_OBJECT)
  
  @note
  OBS_OBJECT_WORK::ppFuncは書き換えないでください。
  更新処理は GMS_BS_CMN_NODE_CTRL_OBJECT::proc_update に設定してください。
  GMS_BS_CMN_NODE_CTRL_OBJECT::w_mtx を設定するとCNM管理ワークに自動的に反映されます。
  初期状態ではマトリクスパレットへの反映がオフになっています。
  is_enable メンバにTRUEを設定することで反映がオンになります。
 */
// =======================================================================
inline GMS_BS_CMN_NODE_CTRL_OBJECT* GmBsCmnCreateNodeControlObject(OBS_OBJECT_WORK *parent_obj,
																   GMS_BS_CMN_CNM_MGR_WORK *cnm_mgr_work,
																   Sint32 cnm_reg_id,
																   GMS_BS_CMN_SNM_WORK *snm_work,
																   Sint32 snm_reg_id)
{
	return GmBsCmnCreateNodeControlObjectBySize(parent_obj,
												cnm_mgr_work,
												cnm_reg_id,
												snm_work,
												snm_reg_id,
												sizeof(GMS_BS_CMN_NODE_CTRL_OBJECT));
}

// =======================================================================
// GmBsCmnAttachNCObjectToSNMNode
/*!
  ノード操作オブジェクトをSNMノードにくっつける
  
  @param ndc_obj	[io]	ノード操作オブジェクト
  
  @note
  生成時に指定したSNMノードの姿勢をノード操作オブジェクトに反映します。
  ノードのマトリクスをコピーするわけではなく、ノードマトリクスに相当する
  姿勢をノード操作オブジェクトの各パラメータに設定します。
  SNMを使用しているため、本来の姿勢からは1フレーム遅れた値が反映されます。
  本来のノード姿勢を反映させたい場合などに利用してください
  （パーツをばらす前の初期姿勢設定など）。
 */
// =======================================================================
extern void GmBsCmnAttachNCObjectToSNMNode(GMS_BS_CMN_NODE_CTRL_OBJECT *ndc_obj);

// =======================================================================
// GmBsCmnSetWorldMtxFromNCObjectPosture
/*!
  ノード操作オブジェクトの姿勢情報からワールドマトリクスを設定
  
  @param ndc_obj		[io]	ノード操作オブジェクト
  
  @note
  ノード操作オブジェクトの姿勢情報からワールドマトリクスを作成し、
  CNMマトリクス設定用のマトリクスメンバに格納します。
  回転情報はOBS_OBJECT_WORK::dir を使用せず、
  代わりに GMS_BS_CMN_NODE_CTRL_OBJECT::user_quat を使用します。
 */
// =======================================================================
extern void GmBsCmnSetWorldMtxFromNCObjectPosture(GMS_BS_CMN_NODE_CTRL_OBJECT *ndc_obj);


// ############################################################################
// フェードカラー
// ############################################################################
// =======================================================================
// GmBsCmnSetObject3DNNFadedColor
/*!
  フェードカラーで色づけ
  
  @param obj_work	[io]	オブジェクトワーク
  @param color		[io]	色（強度1.0f時）
  @param intensity	[in]	強度（[0.0f, 1.0f] の範囲）
  @param radius		[in]	対象モデル半径（intensity=0.0f,1.0fをちゃんと反映させるために必要。）
  							（デフォルト=0.0f）
  @param length		[in]	フォグの near distance から far distance 間での距離（最大値）
  							（デフォルト=10000.f）
  
  @note
  OBS_ACTION3D_NN_WORK::draw_stateのフォグ設定を上書くため注意してください。
  対象の座標が変わると色味も変わるため、毎フレーム呼び出してください。
  対象モデルとカメラの距離が近いと誤差が大きくなります。
  radiusはZ方向の厚みだけ考慮すれば問題ありません（e.g.カメラ向きの板ポリならば0.0fで良い）
 */
// =======================================================================
extern void GmBsCmnSetObject3DNNFadedColor(OBS_OBJECT_WORK *obj_work, const NNS_RGB *color,
										   Float intensity, Float radius=0.0f, Float length=10000.f);

// =======================================================================
// GmBsCmnClearObject3DNNFadedColor
/*!
  フェードカラークリア
  
  @param obj_work	[io]	オブジェクトワーク
  
  @note
  フェードカラー設定を初期設定に戻します。
  OBS_ACTION3D_NN_WORK::draw_stateのフォグ設定を上書くため注意してください。
 */
// =======================================================================
extern void GmBsCmnClearObject3DNNFadedColor(OBS_OBJECT_WORK *obj_work);

// =======================================================================
// GmBsCmnIsSetSafeObject3DNNFadedColor
/*!
  フェードカラー設定しても問題ないか判定
  
  @param obj_work	[in]	オブジェクトワーク
  
  @retval	TRUE	フォグ未設定
  @retval	FALSE	フォグ設定済み
 
  @note
  OBS_ACTION3D_NN_WORK::draw_stateのフォグのON/OFFフラグをチェックしています。
  その他のフォグ関連設定はチェックしていません。
 */
// =======================================================================
extern BOOL GmBsCmnIsSetSafeObject3DNNFadedColor(const OBS_OBJECT_WORK *obj_work);


// ############################################################################
// ダメージ点滅
// ############################################################################
// =======================================================================
// GmBsCmnInitObject3DNNDamageFlicker
/*!
  ダメージ点滅初期化
 
  @param obj_work	[io]	オブジェクトワーク（obj_3dが設定されている必要があります）
  @param flk_work	[io]	ダメージ点滅ワーク
  @param radius		[in]	モデル半径（ビュー座標系におけるZ方向の厚みだけ考慮すればよい）
  
  @note
  前の点滅状態を引き継がずに強制的に初期化する場合はclean_init=TRUEを設定してください。
 */
// =======================================================================
extern void GmBsCmnInitObject3DNNDamageFlicker(OBS_OBJECT_WORK *obj_work,
											   GMS_BS_CMN_DMG_FLICKER_WORK *flk_work,
											   Float radius);

// =======================================================================
// GmBsCmnUpdateObject3DNNDamageFlicker
/*!
  ダメージ点滅更新
  
  @param obj_work	[io]	オブジェクトワーク（obj_3dが設定されている必要があります）
  @param flk_work	[io]	ダメージ点滅ワーク
  
  @retval	TRUE	更新終了
  @retval	FALSE	更新中
  
  @note
  毎フレーム呼んでください。
  TRUEを待たずに終了する場合はGmBsCmnEndObject3DNNDamageFlicker()を呼んで
  終了処理を行ってください。
 */
// =======================================================================
extern BOOL GmBsCmnUpdateObject3DNNDamageFlicker(OBS_OBJECT_WORK *obj_work,
												 GMS_BS_CMN_DMG_FLICKER_WORK *flk_work);

// =======================================================================
// GmBsCmnEndObject3DNNDamageFlicker
/*!
  ダメージ点滅終了
  
  @param obj_work	[io]	オブジェクトワーク（obj_3dが設定されている必要があります）
  @param flk_work	[io]	ダメージ点滅ワーク
  
  @note
  ダメージ点滅を終了してパラメータをクリアします。
  更新が終了していない状態で呼び出すこともできますが、
  即時的に表示が切り替わることに留意してください。
 */
// =======================================================================
extern void GmBsCmnEndObject3DNNDamageFlicker(OBS_OBJECT_WORK *obj_work,
											  GMS_BS_CMN_DMG_FLICKER_WORK *flk_work);

// ############################################################################
// 画面フェード
// ############################################################################
// =======================================================================
// GmBsCmnInitScreenFadingColor
/*!
  画面全体フェードカラー 初期化
  
  @param start_color	[in]	開始カラー
  @param end_color		[in]	終了カラー
  @param frame			[in]	フェードにかけるフレーム数
  
  @return フェードオブジェクトワーク(GMS_FADE_OBJ_WORK)
  
  @note
  start_colorからend_colorへframeフレームを掛けてフェードします。
  不要になった時点でGmBsCmnClearScreenFadingColor()を必ず呼び出してください。
 */
// =======================================================================
extern GMS_FADE_OBJ_WORK* GmBsCmnInitScreenFadingColor(const NNS_RGBA_U8 *start_color,
													   const NNS_RGBA_U8 *end_color,
													   Float frame);

// =======================================================================
// GmBsCmnUpdateScreenFadingColor
/*!
  画面全体フェードカラー 更新
  
  @param fade_obj_work	[io]	フェードオブジェクトワーク
  
  @retval TRUE	終了（目標カラーに到達）
  @retval FALSE	フェード中
  
  @note
  毎フレーム呼び出してください。
 */
// =======================================================================
extern BOOL GmBsCmnUpdateScreenFadingColor(GMS_FADE_OBJ_WORK *fade_obj_work);

// =======================================================================
// GmBsCmnClearScreenFadingColor
/*!
  画面全体フェードカラー処理クリア
  
  @param fade_obj_work	[io]	フェードオブジェクトワーク
  
  @note
  フェードカラーをクリアします。
  画面フェードカラー処理が不要になった時点で必ず呼び出してください。
 */
// =======================================================================
extern void GmBsCmnClearScreenFadingColor(GMS_FADE_OBJ_WORK *fade_obj_work);


// ############################################################################
// 撃破時画面フラッシュ
// ############################################################################
// =======================================================================
// GmBsCmnInitFlashScreen
/*!
  画面全体白フラッシュ 初期化
  
  @param flash_work		[in]	画面フラッシュワーク
  @param fo_frame		[in]	フェードアウトにかけるフレーム数
  @param duration_frame	[in]	フェードアウト後の状態で停滞するフレーム数
  @param fi_frame		[in]	フェードインにかけるフレーム数
  
  @note
  ボス撃破時用に画面全体の白フラッシュを行います。
 */
// =======================================================================
extern void GmBsCmnInitFlashScreen(GMS_CMN_FLASH_SCR_WORK *flash_work,
								   Float fo_frame, Float duration_frame, Float fi_frame);

// =======================================================================
// GmBsCmnUpdateFlashScreen
/*!
  画面全体白フラッシュ 更新
  
  @param flash_work		[in]	画面フラッシュワーク
  
  @retval TRUE	白フラッシュ完了
  @retval FALSE 白フラッシュ更新中
 */
// =======================================================================
extern BOOL GmBsCmnUpdateFlashScreen(GMS_CMN_FLASH_SCR_WORK *flash_work);

// =======================================================================
// GmBsCmnClearFlashScreen
/*!
  画面全体白フラッシュ クリア
  
  @param flash_work		[in]	画面フラッシュワーク
 */
// =======================================================================
extern void GmBsCmnClearFlashScreen(GMS_CMN_FLASH_SCR_WORK *flash_work);


// ############################################################################
// 遅延サーチ
// ############################################################################
// =======================================================================
// GmBsCmnInitDelaySearch
/*!
  遅延サーチ処理 初期化
  
  @param dsearch_work	[io]	遅延サーチワーク
  @param targ_obj		[in]	対象オブジェクト
  @param pos_hist_buf	[io]	座標履歴記録バッファ
  @param hist_num		[in]	バッファサイズ（履歴記録可能数）
 */
// =======================================================================
extern void GmBsCmnInitDelaySearch(GMS_BS_CMN_DELAY_SEARCH_WORK *dsearch_work,
								   const OBS_OBJECT_WORK *targ_obj,
								   VecFx32 *pos_hist_buf, Sint32 hist_num);

// =======================================================================
// GmBsCmnUpdateDelaySearch
/*!
  遅延サーチ処理 更新
  
  @param dsearch_work	[io]	遅延サーチワーク
 */
// =======================================================================
extern void GmBsCmnUpdateDelaySearch(GMS_BS_CMN_DELAY_SEARCH_WORK *dsearch_work);

// =======================================================================
// GmBsCmnGetDelaySearchPos
/*!
  遅延サーチ処理 遅延座標取得
  
  @param dsearch_work	[in]	遅延サーチワーク
  @param delay_time		[in]	座標を取得したい遅延フレーム
  @param pos			[out]	格納先
  
  @note
  指定した遅延フレームの記録を取得するのに十分な履歴が溜まっていない場合、
  GmBsCmnInitDelaySearch()を呼んだ時点での座標を返します。
 */
// =======================================================================
extern void GmBsCmnGetDelaySearchPos(const GMS_BS_CMN_DELAY_SEARCH_WORK *dsearch_work,
									 Sint32 delay_time, VecFx32 *pos);

// =======================================================================
// test_func
/*!
  関数機能
 
  @param param0 [in] 入力引数0説明
  @param param1 [out] 出力ポインタ引数1説明
  @param param2 [io] 入出力ポインタ引数2説明
 
  @return 返値説明
 
  @note
  補足説明
 */
// =======================================================================

#if	defined(__cplusplus)
} /* extern "C" */
#endif

#endif /* GM_BOSS_COMMON_H_ */
