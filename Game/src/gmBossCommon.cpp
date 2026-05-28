// =======================================================================
/*!
  @file	gmBossCommon.cpp
  @brief ボス共通

  @author Keisuke Tanaka
 				Copyright(c) 2009 Dimps
  $Id: gmBossCommon.cpp 2 2011-04-11 05:21:26Z thamada $
 */
// =======================================================================
/*
 * $Log$
 */


/*------ Include Files -------------------------------------------------*/
#include "pch.h"
#include "akMath.h"
#include "gmMain.h"
#include "gmEnemy.h"
#include "gmEffect.h"
#include "izFade.h"
#include "gmFade.h"

#include "gmBossCommon.h"

/*------ Macros --------------------------------------------------------*/

/*------ Macro Functions -----------------------------------------------*/

/*------ Definitions ---------------------------------------------------*/
#define GMD_BS_CMN_DMG_FLICKER_ALPHA			(0 & _IPHONE)			//!< フリッカーをアルファで行う

//############ ダメージ点滅 ###################################################
/* 定義値 */
#define GMD_BS_CMN_DMG_FLICKER_DEFAULT_CYCLE	(5)						//!< ダメージ点滅周期数
#define GMD_BS_CMN_DMG_FLICKER_INTERVAL_TIME	(5)						//!< ダメージ点滅角度更新インターバル時間
#if GMD_BS_CMN_DMG_FLICKER_ALPHA
#define GMD_BS_CMN_DMG_FLICKER_ANGLE_SPD		(AKM_DEGtoA32(22.5f))	//!< ダメージ点滅サイン波角速度
#else
#define GMD_BS_CMN_DMG_FLICKER_ANGLE_SPD		(AKM_DEGtoA32(45.f))	//!< ダメージ点滅サイン波角速度
#endif // GMD_BS_CMN_DMG_FLICKER_ALPHA

/*------ External Declarations -----------------------------------------*/

/*------ Static Declarations -------------------------------------------*/
static BOOL gmBsCmnCheckActionFrameOverrunOnNextUpdate(const OBS_OBJECT_WORK *obj_work,
													   Float *overrun_frame);
static void gmBsCmnInitBossMotionCBLink(GMS_BS_CMN_BMCB_LINK *bmcb_link,
										GMF_BS_CMN_BMCB_FUNC bmcb_func,
										void *bmcb_param);
static void gmBsCmnClearBossMotionCBLink(GMS_BS_CMN_BMCB_LINK *bmcb_link);
static void gmBsCmnBossMotionCallbackFunc(const AMS_MOTION *motion,
										  const NNS_OBJECT *object,
										  void *mtn_cb_param);
static void gmBsCmnMotionCallbackStoreNodeMatrix(const AMS_MOTION *motion,
												 const NNS_OBJECT *object,
												 void *mtn_cb_param);
static void gmBsCmnMtxpltCallbackControlNodeMatrix(NNS_MATRIX *mtx_plt,
												   const NNS_OBJECT *object,
												   void *mplt_cb_param);
static void gmBsCmnNodeControlObjectMainFunc(OBS_OBJECT_WORK *obj_work);
static void gmBsCmnGetNodeWorldMtx(NNS_MATRIX *dest_mtx, const NNS_NODE *node,
								   const NNS_MATRIX *inv_view_mtx, const NNS_MATRIX *mtx_plt);
static void gmBsCmnGetNodeInvWorldMtx(NNS_MATRIX *dest_mtx, const NNS_NODE *node,
									  const NNS_MATRIX *inv_view_mtx, const NNS_MATRIX *mtx_plt);

/*------ Global Variables ----------------------------------------------*/

/*------ Static Variables ----------------------------------------------*/
//############ ユーティリティ #################################################
//! エフェクト 矩形 攻撃属性フラグテーブル
const static Uint16 gm_bs_cmn_efct_atk_flag_tbl[GME_EFFECT_RECT_NUM]	= {
	0,		// 食らい
	(GMD_OBJ_RECT_ATK_FLAG_NORMALATK | GMD_OBJ_RECT_ATK_FLAG_EFCTATK),	// 攻撃
};

//! エフェクト 矩形 防御属性フラグテーブル
const static Uint16 gm_bs_cmn_efct_def_flag_tbl[GME_EFFECT_RECT_NUM]	= {
	GMD_OBJ_RECT_DEF_FLAG_EFCTDEF,	// 食らい矩形
	GMD_OBJ_RECT_DEF_FLAG_EFCTATK,	// 攻撃矩形
};


//############ ダメージ点滅 ###################################################
//! ダメージ点滅デフォルトカラー
static const NNS_RGB gm_bs_cmn_dmg_flicker_default_color	= {1.f, 1.f, 1.f};


/*------ Global Functions ----------------------------------------------*/
// ############################################################################
// ユーティリティ
// ############################################################################
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
BOOL GmBsCmnIsActionEndPrecisely(const OBS_OBJECT_WORK *obj_work)
{
	/*
	  1.f == spd の場合はframe_max-1.fを超える前にENDフラグが立つので、
	  通常と同じ挙動になります。
	  1.f > spd, 1.f < spd の場合は次の更新でframe_max-1.fを超えてしまう見込みとなったら
	  TRUEを返すという挙動になります。
	  次のフレームでちょうどframe_max-1.fとなる見込みの場合はFALSEを返します
	  （実質再生速度が変わらないため）。
	 */
	
	if (obj_work->disp_flag & OBD_DISP_END) {
		return TRUE;
	}
	
	return gmBsCmnCheckActionFrameOverrunOnNextUpdate(obj_work, NULL);
}

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
BOOL GmBsCmnIsActionEndFlexibly(const OBS_OBJECT_WORK *obj_work, Float allow_ratio)
{
	OBS_ACTION3D_NN_WORK	*obj_3d	= obj_work->obj_3d;
	Float	spd = obj_3d->speed[0] * FX_FX32_TO_F32(g_obj.speed);
	Float	overrun	= 0.f;
	
	MTM_ASSERT(allow_ratio >= 0.f && allow_ratio <= 1.f);
	
	if (obj_work->disp_flag & OBD_DISP_END) {
		return TRUE;
	}
	
	// 超過フレームを取得
	gmBsCmnCheckActionFrameOverrunOnNextUpdate(obj_work, &overrun);
	
	// 超過フレーム数が再生速度の指定割合を超えていたら厳格チェック
	// （MEMO：すでにENDフラグが立っている場合はoverrunが速度と同値になるため
	//   下記条件文は信用できない）
	if (overrun > spd * allow_ratio) {
		return GmBsCmnIsActionEndPrecisely(obj_work);
	}
	else {
		return GmBsCmnIsActionEnd(obj_work);
	}
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
void GmBsCmnSetEfctAtkVsPly(GMS_EFFECT_COM_WORK *efct_com, Sint16 view_out_ofst)
{
	OBS_OBJECT_WORK	*obj_work	= (OBS_OBJECT_WORK*)efct_com;
	
	obj_work->flag	&= ~(OBD_OBJECT_NOHIT | OBD_OBJECT_NOCLIP);
	obj_work->move_flag	|= OBD_MOVE_NOCOL;
	
	obj_work->view_out_ofst	= view_out_ofst;
	
	GmEffectRectInit(efct_com,
					 gm_bs_cmn_efct_atk_flag_tbl,
					 gm_bs_cmn_efct_def_flag_tbl,
					 GMD_OBJ_RECT_GROUP_ENEMY,
					 GMD_OBJ_RECT_TARGET_GROUPFLAG_PLAYER);
}

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
BOOL GmBsCmnCheckRectMajorOverlapH(const OBS_RECT_WORK *my_rect, const OBS_RECT_WORK *your_rect,
								   fx32 *center_ofst_x)
{
	Sint32	my_left, my_right, my_center_x;
	Sint32	yr_left, yr_right, yr_center_x;
	
	Uint16 my_width;
	Uint16 yr_width;
	
	BOOL	result	= FALSE;
	
	MTM_ASSERT(my_rect);
	MTM_ASSERT(your_rect);
	
	// 自分の矩形のサイズ取得
	{
		ObjRectLTBSet(const_cast<OBS_RECT_WORK*>(my_rect), &my_left, NULL,  NULL);
		ObjRectWHDSet(const_cast<OBS_RECT_WORK*>(my_rect), &my_width, NULL, NULL);
		
		my_right	= my_left + my_width;
		
		my_center_x	= my_left + (my_width >> 1);
	}
	
	// 相手の矩形のサイズ取得
	{
		ObjRectLTBSet(const_cast<OBS_RECT_WORK*>(your_rect), &yr_left, NULL,  NULL);
		ObjRectWHDSet(const_cast<OBS_RECT_WORK*>(your_rect), &yr_width, NULL, NULL);
		
		yr_right	= yr_left + yr_width;
		
		yr_center_x	= yr_left + (yr_width >> 1);
	}
	
	
	// 一方の矩形中心が他方の矩形内にあるかチェック
	if (my_center_x < yr_center_x) {
		// 自分が相手より左
		if (my_right >= yr_center_x ||
			my_center_x >= yr_left) {
			result	= TRUE;
		}
	}
	else if (yr_center_x < my_center_x) {
		// 自分が相手より右
		if (yr_right >= my_center_x ||
			yr_center_x >= my_left) {
			result	= TRUE;
		}
	}
	else {
		// 矩形中心X座標が一致
		result	= TRUE;
	}
	
	// 相手の矩形中心の、自分矩形中心からのオフセットYを取得
	if (center_ofst_x) {
		*center_ofst_x	= (yr_center_x - my_center_x) << FX32_SHIFT;
	}
	
	return result;
}

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
BOOL GmBsCmnCheckRectMajorOverlapV(const OBS_RECT_WORK *my_rect, const OBS_RECT_WORK *your_rect,
								   fx32 *center_ofst_y)
{
	Sint32	my_top, my_bottom, my_center_y;
	Sint32	yr_top, yr_bottom, yr_center_y;
	
	Uint16	my_height;
	Uint16	yr_height;
	
	BOOL	result	= FALSE;
	
	MTM_ASSERT(my_rect);
	MTM_ASSERT(your_rect);
	
	// 自分の矩形のサイズ取得
	{
		ObjRectLTBSet(const_cast<OBS_RECT_WORK*>(my_rect), NULL, &my_top, NULL);
		ObjRectWHDSet(const_cast<OBS_RECT_WORK*>(my_rect), NULL, &my_height, NULL);
		
		my_bottom	= my_top + my_height;
		
		my_center_y	= my_top + (my_height >> 1);
	}
	
	// 相手の矩形のサイズ取得
	{
		ObjRectLTBSet(const_cast<OBS_RECT_WORK*>(your_rect), NULL, &yr_top, NULL);
		ObjRectWHDSet(const_cast<OBS_RECT_WORK*>(your_rect), NULL, &yr_height, NULL);
		
		yr_bottom	= yr_top + yr_height;
		
		yr_center_y	= yr_top + (yr_height >> 1);
	}
	
	
	// 一方の矩形中心が他方の矩形内にあるかチェック
	if (my_center_y < yr_center_y) {
		// 自分が相手より上
		if (my_bottom >= yr_center_y ||
			my_center_y >= yr_top) {
			result	= TRUE;
		}
	}
	else if (yr_center_y < my_center_y) {
		// 自分が相手より下
		if (yr_bottom >= my_center_y ||
			yr_center_y >= my_top) {
			result	= TRUE;
		}
	}
	else {
		// 矩形中心Y座標が一致
		result	= TRUE;
	}
	
	// 相手の矩形中心の、自分矩形中心からのオフセットYを取得
	if (center_ofst_y) {
		*center_ofst_y	= (yr_center_y - my_center_y) << FX32_SHIFT;
	}
	
	return result;
}

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
Uint32 GmBsCmnCheckRectHitSideHFirst(const OBS_RECT_WORK *my_rect, const OBS_RECT_WORK *your_rect)
{
	fx32	center_ofst_x;
	fx32	center_ofst_y;
	BOOL	is_major_ovl;
	
	// 水平方向の広範囲重なりをチェック＆中心オフセットを取得
	is_major_ovl	= GmBsCmnCheckRectMajorOverlapH(my_rect, your_rect, &center_ofst_x);
	
	// 垂直方向の中心オフセットを取得
	GmBsCmnCheckRectMajorOverlapV(my_rect, your_rect, &center_ofst_y);
	
	if (is_major_ovl) {
		// 水平方向に深い当たりの場合
		if (center_ofst_y < 0) {
			// 相手が上なら上側面
			return GMD_BS_CMN_RECT_HIT_SIDE_LEFT;
		}
		else {
			// 相手が下なら下側面
			return GMD_BS_CMN_RECT_HIT_SIDE_RIGHT;
		}
	}
	else {
		// 浅い当たりの場合
		if (center_ofst_x < 0) {
			// 相手が左なら左側面
			return GMD_BS_CMN_RECT_HIT_SIDE_TOP;
		}
		else {
			// 相手が右なら右側面
			return GMD_BS_CMN_RECT_HIT_SIDE_BOTTOM;
		}
	}
}

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
Uint32 GmBsCmnCheckRectHitSideVFirst(const OBS_RECT_WORK *my_rect, const OBS_RECT_WORK *your_rect)
{
	fx32	center_ofst_x;
	fx32	center_ofst_y;
	BOOL	is_major_ovl;
	
	// 垂直方向の広範囲重なりをチェック＆中心オフセットを取得
	is_major_ovl	= GmBsCmnCheckRectMajorOverlapV(my_rect, your_rect, &center_ofst_y);
	
	// 水平方向の中心オフセットを取得
	GmBsCmnCheckRectMajorOverlapH(my_rect, your_rect, &center_ofst_x);
	
	if (is_major_ovl) {
		// 垂直方向に深い当たりの場合
		if (center_ofst_x < 0) {
			// 相手が左なら左側面
			return GMD_BS_CMN_RECT_HIT_SIDE_LEFT;
		}
		else {
			// 相手が右なら右側面
			return GMD_BS_CMN_RECT_HIT_SIDE_RIGHT;
		}
	}
	else {
		// 浅い当たりの場合
		if (center_ofst_y < 0) {
			// 相手が上なら上側面
			return GMD_BS_CMN_RECT_HIT_SIDE_TOP;
		}
		else {
			// 相手が下なら下側面
			return GMD_BS_CMN_RECT_HIT_SIDE_BOTTOM;
		}
	}
}


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
void GmBsCmnInitBossMotionCBSystem(OBS_OBJECT_WORK *obj_work, GMS_BS_CMN_BMCB_MGR *bmcb_mgr)
{
	MTM_ASSERT(obj_work);
	MTM_ASSERT(obj_work->obj_3d);
	MTM_ASSERT(bmcb_mgr);
	
	// BMCB管理ワークを初期化
	amZeroMemory(bmcb_mgr, sizeof(GMS_BS_CMN_BMCB_MGR));
	bmcb_mgr->bmcb_head.next	= &bmcb_mgr->bmcb_tail;
	bmcb_mgr->bmcb_head.prev	= NULL;
	bmcb_mgr->bmcb_tail.next	= NULL;
	bmcb_mgr->bmcb_tail.prev	= &bmcb_mgr->bmcb_head;
	
	// モーションコールバック設定
	obj_work->obj_3d->mtn_cb_func	= gmBsCmnBossMotionCallbackFunc;
	obj_work->obj_3d->mtn_cb_param	= bmcb_mgr;
}


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
void GmBsCmnClearBossMotionCBSystem(OBS_OBJECT_WORK *obj_work)
{
	GMS_BS_CMN_BMCB_MGR	*bmcb_mgr;
	GMS_BS_CMN_BMCB_LINK	*cur_bmcb;
	
	MTM_ASSERT(obj_work);
	MTM_ASSERT(obj_work->obj_3d);
	MTM_ASSERT(obj_work->obj_3d->mtn_cb_param);
	
	bmcb_mgr	= (GMS_BS_CMN_BMCB_MGR*)obj_work->obj_3d->mtn_cb_param;
	
	// BMCBリンクを全て解除する（headとtailも連結解除）
	cur_bmcb	= bmcb_mgr->bmcb_head.next;
	while (cur_bmcb) {
		GMS_BS_CMN_BMCB_LINK	*next_link;
		next_link	= cur_bmcb->next;
		cur_bmcb->next	= NULL;
		cur_bmcb->prev	= NULL;
		
		if (NULL == cur_bmcb->bmcb_func) {
			// tail到達
			break;
		}
		else {
			cur_bmcb	= next_link;
		}
	}
	bmcb_mgr->bmcb_head.next	=
		bmcb_mgr->bmcb_head.prev	= NULL;
	bmcb_mgr->bmcb_tail.next	=
		bmcb_mgr->bmcb_tail.prev	= NULL;

	// BMCB管理ワークをクリア
	amZeroMemory(bmcb_mgr, sizeof(GMS_BS_CMN_BMCB_MGR));
	
	// モーションコールバック設定クリア
	obj_work->obj_3d->mtn_cb_func	= NULL;
	obj_work->obj_3d->mtn_cb_param	= NULL;
}


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
void GmBsCmnAppendBossMotionCallback(GMS_BS_CMN_BMCB_MGR *bmcb_mgr, GMS_BS_CMN_BMCB_LINK *bmcb_link)
{
	MTM_ASSERT(bmcb_mgr);
	MTM_ASSERT(bmcb_link);
	MTM_ASSERT(bmcb_link->bmcb_func);
	MTM_ASSERT(bmcb_link->bmcb_param);
	
	bmcb_link->prev	= bmcb_mgr->bmcb_tail.prev;
	bmcb_link->prev->next	= bmcb_link;
	bmcb_link->next	= &bmcb_mgr->bmcb_tail;
	bmcb_mgr->bmcb_tail.prev	= bmcb_link;
}


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
void GmBsCmnCreateSNMWork(GMS_BS_CMN_SNM_WORK *snm_work,
						   const NNS_OBJECT *object, Uint16 reg_max)
{
	UNREFERENCED_PARAMETER(object);
	MTM_ASSERT(snm_work);
	MTM_ASSERT(NULL == snm_work->node_info_list);
	
	// BMCBリンク初期化
	gmBsCmnInitBossMotionCBLink(&snm_work->bmcb_link,
								gmBsCmnMotionCallbackStoreNodeMatrix,
								snm_work);
	
	snm_work->reg_node_cnt	= 0;
	snm_work->reg_node_max	= reg_max;
	
	snm_work->node_info_list	= (GMS_BS_CMN_SNM_NODE_INFO*)amMemAlloc(reg_max * sizeof(GMS_BS_CMN_SNM_NODE_INFO));
	amZeroMemory(snm_work->node_info_list, reg_max * sizeof(GMS_BS_CMN_SNM_NODE_INFO));
}

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
void GmBsCmnDeleteSNMWork(GMS_BS_CMN_SNM_WORK *snm_work)
{
	MTM_ASSERT(snm_work);
	
	// BMCBリンククリア
	gmBsCmnClearBossMotionCBLink(&snm_work->bmcb_link);
	
	snm_work->reg_node_cnt	= 0;
	snm_work->reg_node_max	= 0;
	
	if (snm_work->node_info_list) {
		amMemFree(snm_work->node_info_list);
		snm_work->node_info_list	= NULL;
	}
}

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
Sint32 GmBsCmnRegisterSNMNode(GMS_BS_CMN_SNM_WORK *snm_work, Sint32 node_index)
{
	Sint32	snm_reg_id;
	MTM_ASSERT(snm_work->reg_node_cnt < snm_work->reg_node_max);
	
	snm_work->node_info_list[snm_work->reg_node_cnt].node_index	= node_index;
	snm_reg_id	= snm_work->reg_node_cnt;
	snm_work->reg_node_cnt++;
	
	return snm_reg_id;
}

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
NNS_MATRIX* GmBsCmnGetSNMMtx(const GMS_BS_CMN_SNM_WORK *snm_work, Sint32 snm_reg_id)
{
	MTM_ASSERT(snm_reg_id < snm_work->reg_node_max);
	
	return &snm_work->node_info_list[snm_reg_id].node_w_mtx;
}

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
void GmBsCmnUpdateObjectGeneralStuckWithNode(OBS_OBJECT_WORK *obj_work,
											 const GMS_BS_CMN_SNM_WORK *snm_work,
											 Sint32 snm_reg_id,
											 const NNS_MATRIX *ofst_mtx/*=NULL*/)
{
	NNS_MATRIX	*w_mtx;
	
	MTM_ASSERT(obj_work);
	
	// ノードのワールドマトリクス取得
	w_mtx	= GmBsCmnGetSNMMtx(snm_work, snm_reg_id);
	
	// ノードにくっつける
	obj_work->pos.x	= FX_F32_TO_FX32(NNM_MTX(*w_mtx, 0, 3));
	obj_work->pos.y	= -FX_F32_TO_FX32((NNM_MTX(*w_mtx, 1, 3)));
	obj_work->pos.z	= FX_F32_TO_FX32((NNM_MTX(*w_mtx, 2, 3)));
	
	// オフセット反映
	if (ofst_mtx) {
		
		// 回転成分は無視
		VEC_Set(&obj_work->pos,
				obj_work->pos.x + FX_F32_TO_FX32(NNM_MTX(*ofst_mtx, 0, 3)),
				obj_work->pos.y - FX_F32_TO_FX32(NNM_MTX(*ofst_mtx, 1, 3)),
				obj_work->pos.z + FX_F32_TO_FX32(NNM_MTX(*ofst_mtx, 2, 3)));
	}
}

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
void GmBsCmnUpdateObjectGeneralStuckWithNodeRelative(OBS_OBJECT_WORK *obj_work,
													 const GMS_BS_CMN_SNM_WORK *snm_work,
													 Sint32 snm_reg_id,
													 const VecFx32 *pivot_cur_pos,
													 const VecFx32 *pivot_prev_pos,
													 const NNS_MATRIX *ofst_mtx/*=NULL*/)
{
	MTM_ASSERT(pivot_cur_pos);
	MTM_ASSERT(pivot_prev_pos);
	
	GmBsCmnUpdateObjectGeneralStuckWithNode(obj_work, snm_work, snm_reg_id, ofst_mtx);
	
	// 前フレームでの相対座標を維持するように座標を修正
	VEC_Set(&obj_work->pos,
			(obj_work->pos.x - pivot_prev_pos->x) + pivot_cur_pos->x,
			(obj_work->pos.y - pivot_prev_pos->y) + pivot_cur_pos->y,
			(obj_work->pos.z - pivot_prev_pos->z) + pivot_cur_pos->z);
}

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
void GmBsCmnUpdateObject3DNNStuckWithNode(OBS_OBJECT_WORK *obj_work,
										  const GMS_BS_CMN_SNM_WORK *snm_work,
										  Sint32 snm_reg_id,
										  BOOL b_rotation,
										  const NNS_MATRIX *ofst_mtx/*=NULL*/)
{
	NNS_MATRIX	*w_mtx;
	NNS_MATRIX	*user_obj_mtx_r;
	
	MTM_ASSERT(obj_work);
	MTM_ASSERT(obj_work->obj_3d);
	
	// ユーザマトリクス領域取得
	user_obj_mtx_r	= &obj_work->obj_3d->user_obj_mtx_r;
	
	// ノードのワールドマトリクス取得
	w_mtx	= GmBsCmnGetSNMMtx(snm_work, snm_reg_id);
	
	// ノードにくっつける
	obj_work->pos.x	= FX_F32_TO_FX32(NNM_MTX(*w_mtx, 0, 3));
	obj_work->pos.y	= -FX_F32_TO_FX32((NNM_MTX(*w_mtx, 1, 3)));
	obj_work->pos.z	= FX_F32_TO_FX32((NNM_MTX(*w_mtx, 2, 3)));
	
	// 回転
	if (b_rotation) {
		// ノードの回転を反映
		obj_work->disp_flag	|= OBD_DISP_USERMTX_RIGHT;
		AkMathNormalizeMtx(user_obj_mtx_r, w_mtx);
	}
	else {
		// 単位行列をセット
		obj_work->disp_flag	&= ~OBD_DISP_USERMTX_RIGHT;
		nnMakeUnitMatrix(user_obj_mtx_r);
	}
	
	// オフセット反映
	if (ofst_mtx) {
		NNS_MATRIX	rot_mtx;
		NNS_VECTOR	rotated_vec;
		NNS_MATRIX	node_w_rot;
		
		/* オフセット平行移動成分をベクトル化 */
		
		// オフセット平行移動の回転計算に使用するので、ワールド平行移動座標は考慮しない
		nnCopyMatrix(&node_w_rot, w_mtx);
		NNM_MTX(node_w_rot, 0, 3)	=
			NNM_MTX(node_w_rot, 1, 3)	=
				NNM_MTX(node_w_rot, 2, 3)	= 0;
		// ノードローカルなオフセット座標にノードのワールド座標系での回転を反映させる
		nnMultiplyMatrix(&node_w_rot, &node_w_rot, ofst_mtx);
		nnCopyMatrixTranslationVector(&rotated_vec, &node_w_rot);
		
		/* 平行移動オフセットをオブジェクト座標に反映 */
		VEC_Set(&obj_work->pos,
				obj_work->pos.x + FX_F32_TO_FX32(rotated_vec.x),
				obj_work->pos.y - FX_F32_TO_FX32(rotated_vec.y),
				obj_work->pos.z + FX_F32_TO_FX32(rotated_vec.z));
		
		/* 回転マトリクスを抽出 */
		nnCopyMatrix(&rot_mtx, ofst_mtx);
		NNM_MTX(rot_mtx, 0, 3)	=
			NNM_MTX(rot_mtx, 1, 3)	=
				NNM_MTX(rot_mtx, 2, 3)	= 0;	// 平行移動成分をクリア
		
		/* オフセット回転をオブジェクト表示回転に反映*/
		
		// オフセット回転成分をユーザマトリクスに乗算
		obj_work->disp_flag	|= OBD_DISP_USERMTX_RIGHT;
		nnMultiplyMatrix(user_obj_mtx_r, user_obj_mtx_r, &rot_mtx);
	}
}

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
void GmBsCmnUpdateObject3DNNStuckWithNodeRelative(OBS_OBJECT_WORK *obj_work,
												  const GMS_BS_CMN_SNM_WORK *snm_work,
												  Sint32 snm_reg_id,
												  BOOL b_rotation,
												  const VecFx32 *pivot_cur_pos,
												  const VecFx32 *pivot_prev_pos,
												  const NNS_MATRIX *ofst_mtx/*=NULL*/)
{
	MTM_ASSERT(pivot_cur_pos);
	MTM_ASSERT(pivot_prev_pos);
	
	GmBsCmnUpdateObject3DNNStuckWithNode(obj_work, snm_work, snm_reg_id, b_rotation, ofst_mtx);
	
	// 前フレームでの相対座標を維持するように座標を修正
	VEC_Set(&obj_work->pos,
			(obj_work->pos.x - pivot_prev_pos->x) + pivot_cur_pos->x,
			(obj_work->pos.y - pivot_prev_pos->y) + pivot_cur_pos->y,
			(obj_work->pos.z - pivot_prev_pos->z) + pivot_cur_pos->z);
}

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
void GmBsCmnUpdateObject3DESStuckWithNode(OBS_OBJECT_WORK *obj_work,
										  const GMS_BS_CMN_SNM_WORK *snm_work,
										  Sint32 snm_reg_id,
										  BOOL b_rotation,
										  const NNS_MATRIX *ofst_mtx/*=NULL*/)
{
	NNS_MATRIX	*w_mtx;
	AMS_QUAT	*user_dir_quat;
	
	NNS_MATRIX	nml_w_mtx;
	
	MTM_ASSERT(obj_work);
	MTM_ASSERT(obj_work->obj_3des);
	
	// ユーザマトリクス領域取得
	user_dir_quat	= &obj_work->obj_3des->user_dir_quat;
	
	// ノードのワールドマトリクス取得
	w_mtx	= GmBsCmnGetSNMMtx(snm_work, snm_reg_id);
	
	// ノードにくっつける
	obj_work->pos.x	= FX_F32_TO_FX32(NNM_MTX(*w_mtx, 0, 3));
	obj_work->pos.y	= -FX_F32_TO_FX32((NNM_MTX(*w_mtx, 1, 3)));
	obj_work->pos.z	= FX_F32_TO_FX32((NNM_MTX(*w_mtx, 2, 3)));
	
	// 回転
	if (b_rotation) {
		// ノードの回転を反映
		obj_work->obj_3des->flag	|= OBD_ACTFLAG_3D_ES_USER_DIR_QUAT;
		
		// 正規化
		AkMathNormalizeMtx(&nml_w_mtx, w_mtx);
		
		// クォータニオンに変換＆設定
		nnMakeRotateMatrixQuaternion(user_dir_quat, &nml_w_mtx);
	}
	else {
		// 単位クォータニオンをセット
		obj_work->obj_3des->flag	&= ~OBD_ACTFLAG_3D_ES_USER_DIR_QUAT;
		nnMakeUnitQuaternion(user_dir_quat);
		
		nnMakeUnitMatrix(&nml_w_mtx);	// オフセット計算で使用
	}
	
	// オフセット反映
	if (ofst_mtx) {
		NNS_MATRIX	rot_mtx;
		NNS_QUATERNION	rot_quat;
		NNS_VECTOR	rotated_vec;
		NNS_MATRIX	node_w_rot;
		
		/* オフセット平行移動成分をベクトル化 */
		
		// オフセット平行移動の回転計算に使用するので、ワールド平行移動座標は考慮しない
		nnCopyMatrix(&node_w_rot, w_mtx);
		NNM_MTX(node_w_rot, 0, 3)	=
			NNM_MTX(node_w_rot, 1, 3)	=
				NNM_MTX(node_w_rot, 2, 3)	= 0;
		// ノードローカルなオフセット座標にノードのワールド座標系での回転を反映させる
		nnMultiplyMatrix(&node_w_rot, &node_w_rot, ofst_mtx);
		nnCopyMatrixTranslationVector(&rotated_vec, &node_w_rot);
		
		/* 平行移動オフセットをオブジェクト座標に反映 */
		VEC_Set(&obj_work->pos,
				obj_work->pos.x + FX_F32_TO_FX32(rotated_vec.x),
				obj_work->pos.y - FX_F32_TO_FX32(rotated_vec.y),
				obj_work->pos.z + FX_F32_TO_FX32(rotated_vec.z));
		
		/* 回転成分のクォータニオンを抽出 */
		AkMathNormalizeMtx(&rot_mtx, ofst_mtx);	// 平行移動成分はクリアされる
		nnMakeRotateMatrixQuaternion(&rot_quat, &rot_mtx);
		
		/* オフセット回転をオブジェクト表示回転に反映*/
		
		// オフセット回転成分をユーザクォータニオンに乗算
		obj_work->obj_3des->flag	|= OBD_ACTFLAG_3D_ES_USER_DIR_QUAT;
		nnMultiplyQuaternion(user_dir_quat, user_dir_quat, &rot_quat);
	}
}

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
void GmBsCmnUpdateObject3DESStuckWithNodeRelative(OBS_OBJECT_WORK *obj_work,
												  const GMS_BS_CMN_SNM_WORK *snm_work,
												  Sint32 snm_reg_id,
												  BOOL b_rotation,
												  const VecFx32 *pivot_cur_pos,
												  const VecFx32 *pivot_prev_pos,
												  const NNS_MATRIX *ofst_mtx/*=NULL*/)
{
	MTM_ASSERT(pivot_cur_pos);
	MTM_ASSERT(pivot_prev_pos);
	
	GmBsCmnUpdateObject3DESStuckWithNode(obj_work, snm_work, snm_reg_id, b_rotation, ofst_mtx);
	
	// 前フレームでの相対座標を維持するように座標を修正
	VEC_Set(&obj_work->pos,
			(obj_work->pos.x - pivot_prev_pos->x) + pivot_cur_pos->x,
			(obj_work->pos.y - pivot_prev_pos->y) + pivot_cur_pos->y,
			(obj_work->pos.z - pivot_prev_pos->z) + pivot_cur_pos->z);
}


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
void GmBsCmnInitCNMCb(OBS_OBJECT_WORK *obj_work, GMS_BS_CMN_CNM_MGR_WORK *cnm_mgr_work)
{
	UNREFERENCED_PARAMETER(cnm_mgr_work);
	
	MTM_ASSERT(obj_work);
	MTM_ASSERT(obj_work->obj_3d);
	MTM_ASSERT(NULL == obj_work->obj_3d->mplt_cb_func);
	MTM_ASSERT(NULL == obj_work->obj_3d->mplt_cb_param);
	MTM_ASSERT(cnm_mgr_work);
	MTM_ASSERT(cnm_mgr_work->reg_node_max);
	
	obj_work->obj_3d->mplt_cb_func	= gmBsCmnMtxpltCallbackControlNodeMatrix;
	obj_work->obj_3d->mplt_cb_param	= NULL;	// パラメータは毎フレーム設定する
}

// =======================================================================
// GmBsCmnClearCNMCb
/*!
  ノードマトリクス操作処理 解放
 
  @param obj_work	[io]	オブジェクトワーク
  
  @note
  obj_3dに設定されたコールバック関数とパラメータをクリアします。
 */
// =======================================================================
void GmBsCmnClearCNMCb(OBS_OBJECT_WORK *obj_work)
{
	MTM_ASSERT(obj_work);
	MTM_ASSERT(obj_work->obj_3d);
	
	obj_work->obj_3d->mplt_cb_func	= NULL;
	// 描画スレッド用データバッファなので解放不要
	obj_work->obj_3d->mplt_cb_param	= NULL;
}

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
void GmBsCmnCreateCNMMgrWork(GMS_BS_CMN_CNM_MGR_WORK *cnm_mgr_work,
							 const NNS_OBJECT *object, Uint16 reg_max)
{
	UNREFERENCED_PARAMETER(object);
	MTM_ASSERT(cnm_mgr_work);
	MTM_ASSERT(NULL == cnm_mgr_work->node_info_list);
	
	cnm_mgr_work->reg_node_cnt	= 0;
	cnm_mgr_work->reg_node_max	= reg_max;
	
	cnm_mgr_work->node_info_list	= (GMS_BS_CMN_CNM_NODE_INFO*)amMemAlloc(reg_max * sizeof(GMS_BS_CMN_CNM_NODE_INFO));
	amZeroMemory(cnm_mgr_work->node_info_list, reg_max * sizeof(GMS_BS_CMN_CNM_NODE_INFO));
}

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
void GmBsCmnDeleteCNMMgrWork(GMS_BS_CMN_CNM_MGR_WORK *cnm_mgr_work)
{
	cnm_mgr_work->reg_node_cnt	= 0;
	cnm_mgr_work->reg_node_max	= 0;
	
	if (cnm_mgr_work->node_info_list) {
		amMemFree(cnm_mgr_work->node_info_list);
		cnm_mgr_work->node_info_list	= NULL;
	}
}

// =======================================================================
// GmBsCmnUpdateCNMParam
/*!
  ノードマトリクス操作処理 マトリクスパレットCBパラメータ更新
  （メインスレッド側から呼び出し）
  
  @param obj_work		[io]	オブジェクトワーク
  @param cnm_mgr_work	[in]	CNM管理ワーク
  
  @note
  描画スレッド側に渡すパラメータを作成してobj_3dにセットします。
  マトリクスパレットコールバックが設定されていない場合は何もしません。
  !!!!!! ppOut()にて毎フレーム呼び出してください。!!!!!!
 */
// =======================================================================
void GmBsCmnUpdateCNMParam(OBS_OBJECT_WORK *obj_work, const GMS_BS_CMN_CNM_MGR_WORK *cnm_mgr_work)
{
	Uint32	header_part_size	= sizeof(GMS_BS_CMN_CNM_PARAM);
	Uint32	data_part_size		= cnm_mgr_work->reg_node_max * sizeof(GMS_BS_CMN_CNM_NODE_INFO);
	GMS_BS_CMN_CNM_PARAM	*param;
	void	*copy_dest;
	
	// コールバック初期化前に処理が行われないようにする
	if (obj_work->obj_3d->mplt_cb_func == NULL) {
		return;
	}
	
	param	= (GMS_BS_CMN_CNM_PARAM*)amDrawMallocDataBuffer((Sint32)(header_part_size + data_part_size));
	amZeroMemory(param, header_part_size + data_part_size);
	
	param->reg_node_cnt	= cnm_mgr_work->reg_node_cnt;
	
	// ノード情報を描画スレッドデータバッファにコピー
	copy_dest	= (void*)(param + 1);
	amCopyMemory(copy_dest, &cnm_mgr_work->node_info_list[0],
				 cnm_mgr_work->reg_node_max * sizeof(GMS_BS_CMN_CNM_NODE_INFO));
	
	// パラメータセット
	obj_work->obj_3d->mplt_cb_param	= param;
}


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
Sint32 GmBsCmnRegisterCNMNode(GMS_BS_CMN_CNM_MGR_WORK *cnm_mgr_work, Sint32 node_index)
{
	Sint32	cnm_reg_id;
	MTM_ASSERT(cnm_mgr_work->reg_node_cnt < cnm_mgr_work->reg_node_max);
	
	cnm_mgr_work->node_info_list[cnm_mgr_work->reg_node_cnt].node_index	= node_index;
	cnm_reg_id	= cnm_mgr_work->reg_node_cnt;
	cnm_mgr_work->reg_node_cnt++;
	
	return cnm_reg_id;
}

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
void GmBsCmnSetCNMMtx(GMS_BS_CMN_CNM_MGR_WORK *cnm_mgr_work, const NNS_MATRIX *w_mtx,
					  Sint32 cnm_reg_id, BOOL enables/*=FALSE*/)
{
	GMS_BS_CMN_CNM_NODE_INFO	*node_info;
	
	MTM_ASSERT(cnm_reg_id < cnm_mgr_work->reg_node_max);
	
	node_info	= &cnm_mgr_work->node_info_list[cnm_reg_id];
	
	nnCopyMatrix(&node_info->node_w_mtx, w_mtx);
	
	if (enables) {
		node_info->enable	= TRUE;
	}
}

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
void GmBsCmnChangeCNMModeNode(GMS_BS_CMN_CNM_MGR_WORK *cnm_mgr_work,
							  Sint32 cnm_reg_id, GME_BS_CMN_CNM_MODE mode)
{
	MTM_ASSERT(mode < GME_BS_CMN_CNM_MODE_MAX);
	cnm_mgr_work->node_info_list[cnm_reg_id].mode	= mode;
}

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
void GmBsCmnEnableCNMLocalCoordinate(GMS_BS_CMN_CNM_MGR_WORK *cnm_mgr_work,
									 Sint32 cnm_reg_id, BOOL enable)
{
	if (enable) {
		cnm_mgr_work->node_info_list[cnm_reg_id].flag	|= GMD_BS_CMN_CNM_FLAG_LOCAL_COORDINATE;
	}
	else {
		cnm_mgr_work->node_info_list[cnm_reg_id].flag	&= ~GMD_BS_CMN_CNM_FLAG_LOCAL_COORDINATE;
	}
}

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
void GmBsCmnEnableCNMInheritNodeScale(GMS_BS_CMN_CNM_MGR_WORK *cnm_mgr_work,
									  Sint32 cnm_reg_id, BOOL enable)
{
	if (enable) {
		cnm_mgr_work->node_info_list[cnm_reg_id].flag	|= GMD_BS_CMN_CNM_FLAG_INHERIT_SCALE;
	}
	else {
		cnm_mgr_work->node_info_list[cnm_reg_id].flag	&= ~GMD_BS_CMN_CNM_FLAG_INHERIT_SCALE;
	}
}

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
void GmBsCmnEnableCNMMtxNode(GMS_BS_CMN_CNM_MGR_WORK *cnm_mgr_work,
							 Sint32 cnm_reg_id, BOOL enable)
{
	if (enable) {
		cnm_mgr_work->node_info_list[cnm_reg_id].enable	= TRUE;
	}
	else {
		cnm_mgr_work->node_info_list[cnm_reg_id].enable	= FALSE;
	}
}


// =======================================================================
// GmBsCmnCreateNodeControlObjectBySize
/*!
  ノード操作オブジェクト生成（ワークサイズ指定）
  
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
GMS_BS_CMN_NODE_CTRL_OBJECT* GmBsCmnCreateNodeControlObjectBySize(OBS_OBJECT_WORK *parent_obj,
																  GMS_BS_CMN_CNM_MGR_WORK *cnm_mgr_work,
																  Sint32 cnm_reg_id,
																  GMS_BS_CMN_SNM_WORK *snm_work,
																  Sint32 snm_reg_id,
																  Uint32 work_size)
{
	OBS_OBJECT_WORK	*obj_work;
	GMS_BS_CMN_NODE_CTRL_OBJECT	*ndc_obj;
	
	MTM_ASSERT(cnm_mgr_work);
	MTM_ASSERT(work_size >= sizeof(GMS_BS_CMN_NODE_CTRL_OBJECT));
	
	obj_work	= GMM_EFFECT_CREATE_WORK(work_size,
										 parent_obj,
										 0,
										 "bs_cmn_node_ctl_obj");
	
	ndc_obj	= (GMS_BS_CMN_NODE_CTRL_OBJECT*)obj_work;
	
	// ワーク設定
	ndc_obj->cnm_mgr_work	= cnm_mgr_work;
	ndc_obj->cnm_reg_id	= cnm_reg_id;
	ndc_obj->snm_work		= snm_work;
	ndc_obj->snm_reg_id		= snm_reg_id;
	ndc_obj->is_enable	= FALSE;
	
	// 単位行列セット
	nnMakeUnitMatrix(&ndc_obj->w_mtx);
	
	// フラグ設定
	obj_work->disp_flag	|= OBD_DISP_NODISP;
	
	// 描画しない
	obj_work->ppOut	= NULL;
	
	// 処理関数設定
	obj_work->ppFunc	= gmBsCmnNodeControlObjectMainFunc;
	
	return ndc_obj;
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
  ユーザオフセットを設定しておくと、オフセット反映後の見た目に影響を与えないように
  OBS_OBJECT_WORK::pos を適切な値に設定します。
 */
// =======================================================================
void GmBsCmnAttachNCObjectToSNMNode(GMS_BS_CMN_NODE_CTRL_OBJECT *ndc_obj)
{
	OBS_OBJECT_WORK	*obj_work	= (OBS_OBJECT_WORK*)ndc_obj;
	AMS_VECTOR3	vec;
	NNS_MATRIX	w_mtx;
	NNS_MATRIX	nrm_rot;
	NNS_MATRIX	trans_mtx;
	
	// ノードマトリクス取得
	nnCopyMatrix(&w_mtx,
				 GmBsCmnGetSNMMtx(ndc_obj->snm_work,
								  ndc_obj->snm_reg_id));
	
	// 回転マトリクス取得
	AkMathNormalizeMtx(&nrm_rot, &w_mtx);	// scaleとtransを取り除く
	nnMakeRotateMatrixQuaternion(&ndc_obj->user_quat,
								 &nrm_rot);
	
	// オフセット後の画と合うように平行移動成分をずらす
	nnTransformVector(&vec, &nrm_rot, &ndc_obj->user_ofst);
	nnMakeTranslateMatrix(&trans_mtx, -vec.x, -vec.y, -vec.z);
	nnMultiplyMatrix(&w_mtx, &trans_mtx, &w_mtx);
	
	// ノードのスケールを使用
	GmBsCmnEnableCNMInheritNodeScale(ndc_obj->cnm_mgr_work,
									 ndc_obj->cnm_reg_id,
									 TRUE);
	
	// 座標設定
	obj_work->pos.x	= FX_F32_TO_FX32(NNM_MTX(w_mtx, 0, 3));
	obj_work->pos.y	= FX_F32_TO_FX32(-NNM_MTX(w_mtx, 1, 3));
	obj_work->pos.z	= FX_F32_TO_FX32(NNM_MTX(w_mtx, 2, 3));
}


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
void GmBsCmnSetWorldMtxFromNCObjectPosture(GMS_BS_CMN_NODE_CTRL_OBJECT *ndc_obj)
{
	OBS_OBJECT_WORK	*obj_work	= (OBS_OBJECT_WORK*)ndc_obj;
	
	// マトリクス乗算イメージ
	//   [平行移動][回転][オフセット]V
	
	// 平行移動
	nnMakeTranslateMatrix(&ndc_obj->w_mtx,
						  FX_FX32_TO_F32(obj_work->pos.x),
						  FX_FX32_TO_F32(-obj_work->pos.y),
						  FX_FX32_TO_F32(obj_work->pos.z));
	
	// 回転を乗算
	nnQuaternionMatrix(&ndc_obj->w_mtx, &ndc_obj->w_mtx, &ndc_obj->user_quat);
	
	// オフセット（回転前の平行移動 i.e. 基本姿勢時のノード座標基準）
	nnTranslateMatrix(&ndc_obj->w_mtx,
					  &ndc_obj->w_mtx,
					  ndc_obj->user_ofst.x,
					  ndc_obj->user_ofst.y,
					  ndc_obj->user_ofst.z);
	
	// スケールは反映しない（要望があれば対応）
}



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
void GmBsCmnSetObject3DNNFadedColor(OBS_OBJECT_WORK *obj_work, const NNS_RGB *color,
									Float intensity, Float radius/*=0.0f*/, Float length/*=10000.f*/)
{
	AMS_DRAWSTATE	*draw_state;
	NNS_VECTOR	cam_pos;
	Float			obj_pos_z;
	Float			distance;	// カメラから対象までの深度距離
	Float			fnear;
	Float			ffar;
	
	MTM_ASSERT(obj_work);
	MTM_ASSERT(obj_work->obj_3d);
	MTM_ASSERT(color);
	MTM_ASSERT(intensity >= 0.0f && intensity <= 1.0f);
	MTM_ASSERT(radius >= 0);
	
	// draw state 設定先取得
	draw_state	= &obj_work->obj_3d->draw_state;
	
	// フォグON
	draw_state->fog.flag	= NNE_ON;
	
	// フォグカラー設定
	draw_state->fog_color.r	= color->r;
	draw_state->fog_color.g	= color->g;
	draw_state->fog_color.b	= color->b;
	
	// カメラ座標取得
	ObjCameraDispPosGet(g_obj.glb_camera_id, &cam_pos);
	
	// 対象のZ座標取得
	obj_pos_z = FX_FX32_TO_F32(obj_work->pos.z);
	
	// カメラから対象までのZ距離を取得
	// （本来はビュー座標系の深度を使用しますが、今回はカメラの向きは変わらないので
	//   カメラが常に水平方向(-Z)を向いている前提で計算しています）
	distance	= nnAbs(cam_pos.z - obj_pos_z);
	
	// fnear が >0 になるように設定する
	// （カメラが近いとモデル全体に均一な色になりにくいので注意）
	if (length * intensity > distance) {
		fnear	= FLT_MIN;	// 誤差を覚悟でfnear==0にならないようにする
		ffar	= fnear + (distance / intensity);
	}
	else {
		fnear	= (distance - (length * intensity));
		// 誤差を覚悟でfnear==0にならないようにする
		if (fnear <= 0) {
			MTM_ASSERT(fnear == 0);
			fnear	= FLT_MIN;
		}
		ffar	= fnear + length;
	}
	
	// フォグレンジ設定（モデルの大きさを考慮）
	draw_state->fog_range.fnear	= fnear + radius;
	draw_state->fog_range.ffar	= ffar - radius;
}

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
void GmBsCmnClearObject3DNNFadedColor(OBS_OBJECT_WORK *obj_work)
{
	AMS_DRAWSTATE	init_drawstate;
	AMS_DRAWSTATE	*draw_state;
	
	MTM_ASSERT(obj_work);
	MTM_ASSERT(obj_work->obj_3d);
	
	draw_state	= &obj_work->obj_3d->draw_state;
	
	// 初期設定取得
	MI_CpuCopy8(&g_obj_draw_3dnn_draw_state, &init_drawstate, sizeof(AMS_DRAWSTATE));
	
	// フォグ関連だけコピー
	draw_state->fog	= init_drawstate.fog;
	draw_state->fog_color	= init_drawstate.fog_color;
	draw_state->fog_range	= init_drawstate.fog_range;
	
	MTM_ASSERT(draw_state->fog.flag == NNE_OFF);	// 念のためチェック
}

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
BOOL GmBsCmnIsSetSafeObject3DNNFadedColor(const OBS_OBJECT_WORK *obj_work)
{
	MTM_ASSERT(obj_work);
	MTM_ASSERT(obj_work->obj_3d);
	
	if (obj_work->obj_3d->draw_state.fog.flag == NNE_OFF) {
		return TRUE;
	}
	else {
		return FALSE;
	}
}


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
void GmBsCmnInitObject3DNNDamageFlicker(OBS_OBJECT_WORK *obj_work,
										GMS_BS_CMN_DMG_FLICKER_WORK *flk_work,
										Float radius)
{
	MTM_ASSERT(obj_work);
	MTM_ASSERT(obj_work->obj_3d);
	MTM_ASSERT(flk_work);
	
	flk_work->is_active			= TRUE;
	flk_work->cycles			= GMD_BS_CMN_DMG_FLICKER_DEFAULT_CYCLE;
	flk_work->interval_timer	= 0;
	flk_work->cur_angle			= 0;
	flk_work->radius			= radius;
	
	// 初期化
	GmBsCmnClearObject3DNNFadedColor(obj_work);
}

#if GMD_BS_CMN_DMG_FLICKER_ALPHA
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
BOOL GmBsCmnUpdateObject3DNNDamageFlicker(OBS_OBJECT_WORK *obj_work,
										  GMS_BS_CMN_DMG_FLICKER_WORK *flk_work)
{
	MTM_ASSERT(obj_work);
	MTM_ASSERT(obj_work->obj_3d);
	MTM_ASSERT(flk_work);
	
	if (flk_work->is_active == FALSE) {
		return TRUE;
	}
	
	if (flk_work->cycles) {
		
		// 既定時間毎に角度更新
		if (flk_work->interval_timer) {
			flk_work->interval_timer--;
		}
		else {
			
			// 角度更新
			flk_work->cur_angle	= flk_work->cur_angle + GMD_BS_CMN_DMG_FLICKER_ANGLE_SPD;
			
			// 0degを通り過ぎたら1cycleとし、次のサイクルは強制的に0degからスタート
			if (flk_work->cur_angle >= AKM_DEGtoA32(360.f)) {
				flk_work->cur_angle	= 0;
				flk_work->cycles--;
			}
		}
		
		// フェードカラー反映
		// 色変更
		obj_work->disp_flag |= OBD_DISP_DRAWSTATE;							// ユーザー描画ステート反映
		obj_work->obj_3d->drawflag |= NND_DRAWOBJ_MATCTRL_ALPHA;			// 制御あり
		//float color = 1.0f - nnCos(flk_work->cur_angle) / 2;
		//obj_work->obj_3d->draw_state.diffuse.mode = NNE_MATCTRLMODE_ADD;
		//obj_work->obj_3d->draw_state.diffuse.r = color;
		//obj_work->obj_3d->draw_state.diffuse.g = color;
		//obj_work->obj_3d->draw_state.diffuse.b= color;
		obj_work->obj_3d->draw_state.alpha.alpha = 1.0f - nnCos(flk_work->cur_angle);					// 半透明
		
		return FALSE;
	}
	else {
		if (flk_work->is_active) {
			// 終了時にクリアしておく
			GmBsCmnEndObject3DNNDamageFlicker(obj_work, flk_work);
			obj_work->disp_flag &= ~OBD_DISP_DRAWSTATE;							// ユーザー描画ステート反映
			obj_work->obj_3d->drawflag &= ~NND_DRAWOBJ_MATCTRL_ALPHA;			// アルファ制御あり
		}
		return TRUE;
	}
}
#else
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
BOOL GmBsCmnUpdateObject3DNNDamageFlicker(OBS_OBJECT_WORK *obj_work,
										  GMS_BS_CMN_DMG_FLICKER_WORK *flk_work)
{
	MTM_ASSERT(obj_work);
	MTM_ASSERT(obj_work->obj_3d);
	MTM_ASSERT(flk_work);
	
	if (flk_work->is_active == FALSE) {
		return TRUE;
	}
	
	if (flk_work->cycles) {
		
		// 既定時間毎に角度更新
		if (flk_work->interval_timer) {
			flk_work->interval_timer--;
		}
		else {
			
			// 角度更新
			flk_work->cur_angle	= flk_work->cur_angle + GMD_BS_CMN_DMG_FLICKER_ANGLE_SPD;
			
			// 0degを通り過ぎたら1cycleとし、次のサイクルは強制的に0degからスタート
			if (flk_work->cur_angle >= AKM_DEGtoA32(360.f)) {
				flk_work->cur_angle	= 0;
				flk_work->cycles--;
			}
		}
		
		// フェードカラー反映
		// （0.0f ～ 1.0f のマイナスコサイン波でintensityを決定）
		GmBsCmnSetObject3DNNFadedColor(obj_work,
									   &gm_bs_cmn_dmg_flicker_default_color,
									   (1.0f - nnCos(flk_work->cur_angle)) / 2);
		
		return FALSE;
	}
	else {
		if (flk_work->is_active) {
			// 終了時にクリアしておく
			GmBsCmnEndObject3DNNDamageFlicker(obj_work, flk_work);
		}
		return TRUE;
	}
}
#endif // GMD_BS_CMN_DMG_FLICKER_ALPHA

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
void GmBsCmnEndObject3DNNDamageFlicker(OBS_OBJECT_WORK *obj_work,
									   GMS_BS_CMN_DMG_FLICKER_WORK *flk_work)
{
	MTM_ASSERT(obj_work);
	MTM_ASSERT(obj_work->obj_3d);
	MTM_ASSERT(flk_work);
	
	// パラメータクリア
	amZeroMemory(flk_work, sizeof(GMS_BS_CMN_DMG_FLICKER_WORK));
	
	// フェードカラークリア
	GmBsCmnClearObject3DNNFadedColor(obj_work);
}

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
GMS_FADE_OBJ_WORK* GmBsCmnInitScreenFadingColor(const NNS_RGBA_U8 *start_color,
												const NNS_RGBA_U8 *end_color,
												Float frame)
{
	GMS_FADE_OBJ_WORK	*fade_obj_work;
	fade_obj_work	= GmFadeCreateFadeObj(GMD_TASK_PRIO_EFFECT,
										  GMD_TASK_GROUP_EFFECT,
										  GMD_TASK_PAUSELEVEL_DEF,
										  sizeof(GMS_FADE_OBJ_WORK),
										  IZD_FADE_DT_PRIO_DEF,
#if _IPHONE
										  OBD_DRAW_CMD_STATE_3DFIX); // 近景が消される対応
#else
										  OBD_DRAW_CMD_STATE_3DNN);
#endif // _IPHONE
	
	GmFadeSetFade(fade_obj_work,
				  IZE_FADE_SET_TYPE_NORMAL,
				  start_color->r, start_color->g, start_color->b, start_color->a,
				  end_color->r, end_color->g, end_color->b, end_color->a,
				  frame,
				  FALSE,
				  FALSE);
	
	return fade_obj_work;
}

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
BOOL GmBsCmnUpdateScreenFadingColor(GMS_FADE_OBJ_WORK *fade_obj_work)
{
	MTM_ASSERT(fade_obj_work);
	
	if (GmFadeIsEnd(fade_obj_work)) {
		return TRUE;
	}
	
	return FALSE;
}

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
void GmBsCmnClearScreenFadingColor(GMS_FADE_OBJ_WORK *fade_obj_work)
{
	fade_obj_work->obj_work.flag	|= OBD_OBJECT_TASKCLEAR_REQUEST;
}


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
void GmBsCmnInitFlashScreen(GMS_CMN_FLASH_SCR_WORK *flash_work,
							Float fo_frame, Float duration_frame, Float fi_frame)
{
	const static NNS_RGBA_U8 col_none	= {0, 0, 0, 0};
	const static NNS_RGBA_U8 col_white = {0xFF, 0xFF, 0xFF, 0xFF};
	
	MTM_ASSERT(flash_work);
	
	// ワーククリア
	amZeroMemory(flash_work, sizeof(GMS_CMN_FLASH_SCR_WORK));
	
	flash_work->active_flag	|= (1 << 0 | 1 << 1);
	flash_work->fi_frame	= fi_frame;
	flash_work->duration_timer	= duration_frame;
	
	// フェード開始
	flash_work->fade_obj_work	= GmBsCmnInitScreenFadingColor(&col_none, &col_white, fo_frame);
}

// =======================================================================
// GmBsCmnUpdateFlashScreen
/*!
  画面全体白フラッシュ 更新
  
  @param flash_work		[in]	画面フラッシュワーク
  
  @retval TRUE	白フラッシュ完了
  @retval FALSE 白フラッシュ更新中
 */
// =======================================================================
BOOL GmBsCmnUpdateFlashScreen(GMS_CMN_FLASH_SCR_WORK *flash_work)
{
	const static NNS_RGBA_U8 col_none	= {0, 0, 0, 0};
	const static NNS_RGBA_U8 col_white = {0xFF, 0xFF, 0xFF, 0xFF};
	
	MTM_ASSERT(flash_work);
	
	if (flash_work->active_flag == 0) {
		return TRUE;
	}
	
	if (GmBsCmnUpdateScreenFadingColor(flash_work->fade_obj_work)) {
		if (flash_work->active_flag & (1 << 0)) {
			
			if (flash_work->duration_timer > 0.0f) {
				flash_work->duration_timer	-= 1.f;
			}
			else {
				flash_work->active_flag	&= ~(1 << 0);
				GmBsCmnClearScreenFadingColor(flash_work->fade_obj_work);
				flash_work->fade_obj_work	= GmBsCmnInitScreenFadingColor(&col_white, &col_none,
																		   flash_work->fi_frame);
			}
		}
		else if (flash_work->active_flag & (1 << 1)) {
			GmBsCmnClearScreenFadingColor(flash_work->fade_obj_work);
			flash_work->fade_obj_work	= NULL;
			flash_work->active_flag	&= ~(1 << 1);
		}
	}
	
	return FALSE;
}

// =======================================================================
// GmBsCmnClearFlashScreen
/*!
  画面全体白フラッシュ クリア
  
  @param flash_work		[in]	画面フラッシュワーク
 */
// =======================================================================
void GmBsCmnClearFlashScreen(GMS_CMN_FLASH_SCR_WORK *flash_work)
{
	MTM_ASSERT(flash_work);
	
	// フェードをクリア
	if (flash_work->fade_obj_work) {
		GmBsCmnClearScreenFadingColor(flash_work->fade_obj_work);
		flash_work->fade_obj_work	= NULL;
	}
	
	// ワーククリア
	amZeroMemory(flash_work, sizeof(GMS_CMN_FLASH_SCR_WORK));
}


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
void GmBsCmnInitDelaySearch(GMS_BS_CMN_DELAY_SEARCH_WORK *dsearch_work,
							const OBS_OBJECT_WORK *targ_obj,
							VecFx32 *pos_hist_buf, Sint32 hist_num)
{
	MTM_ASSERT(dsearch_work);
	MTM_ASSERT(targ_obj);
	MTM_ASSERT(pos_hist_buf);
	
	// パラメータ初期化
	dsearch_work->pos_hist_buf	= pos_hist_buf;
	dsearch_work->cur_point	= -1;	// インデックス0から記録させるため
	dsearch_work->hist_num	= hist_num;
	dsearch_work->targ_obj	= targ_obj;
	dsearch_work->record_cnt	= 0;
	
	// 初回更新
	GmBsCmnUpdateDelaySearch(dsearch_work);
	
	MTM_ASSERT(dsearch_work->cur_point == 0);
	MTM_ASSERT(dsearch_work->record_cnt == 1);
}

// =======================================================================
// GmBsCmnUpdateDelaySearch
/*!
  遅延サーチ処理 更新
  
  @param dsearch_work	[io]	遅延サーチワーク
 */
// =======================================================================
void GmBsCmnUpdateDelaySearch(GMS_BS_CMN_DELAY_SEARCH_WORK *dsearch_work)
{
	MTM_ASSERT(dsearch_work);
	MTM_ASSERT(dsearch_work->targ_obj);
	
	// 現在の参照インデックス更新
	dsearch_work->cur_point++;
	if (dsearch_work->cur_point >= dsearch_work->hist_num) {
		dsearch_work->cur_point	= 0;
	}
	
	// 記録数更新
	dsearch_work->record_cnt++;
	
	MTM_ASSERT(dsearch_work->record_cnt >= 0);
	
	// 対象オブジェクトの座標を記録
	dsearch_work->pos_hist_buf[dsearch_work->cur_point]	= dsearch_work->targ_obj->pos;
}

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
void GmBsCmnGetDelaySearchPos(const GMS_BS_CMN_DELAY_SEARCH_WORK *dsearch_work,
							  Sint32 delay_time, VecFx32 *pos)
{
	Sint32	ref_point;
	
	MTM_ASSERT(dsearch_work);
	MTM_ASSERT(pos);
	MTM_ASSERT(delay_time >= 0);
	MTM_ASSERT(delay_time < dsearch_work->hist_num);
	
	// 参照ポイント取得
	if (delay_time < dsearch_work->record_cnt) {
		// 履歴が十分に溜まっている場合は指定遅延フレームの記録を取得
		ref_point	= dsearch_work->cur_point - delay_time;
		if (ref_point < 0) {
			ref_point	= dsearch_work->hist_num + ref_point;
			MTM_ASSERT(ref_point >= 0);
		}
	}
	else {
		// 履歴が十分に溜まってない場合は初期記録座標を返す
		ref_point	= 0;
	}
	
	// 指定フレーム遅延した時点の座標取得
	*pos	= dsearch_work->pos_hist_buf[ref_point];
}


/*------ Static Functions ----------------------------------------------*/
// =======================================================================
// gmBsCmnCheckActionFrameOverrunOnNextUpdate
/*!
  次のモーション更新での最終フレーム超過チェック
  
  @param obj_work		[in]	オブジェクトワーク
  @param overrun_frame	[out]	超過フレーム数格納先（NULL可）
  
  @retval	TRUE	次のフレームで最終フレームを超える
  @retval	FALSE	次のフレームで最終フレームにちょうど到達or満たない
  
  @note
  最終フレームを「超えた」場合にのみTRUEを返します。
  次のフレームでちょうど最終フレームとおなじフレームになる場合は
  FALSEを返すことに注意してください。
  また、既に最終フレームに到達している場合でもさらに更新される前提で判定が行われるため、
  TRUEを返し、超過フレームも設定されることに注意してください。
 */
// =======================================================================
BOOL gmBsCmnCheckActionFrameOverrunOnNextUpdate(const OBS_OBJECT_WORK *obj_work,
												Float *overrun_frame)
{
	OBS_ACTION3D_NN_WORK	*obj_3d	= obj_work->obj_3d;
	Float	spd = obj_3d->speed[0] * FX_FX32_TO_F32(g_obj.speed);
	Float	frame_max =
		amMotionGetEndFrame(obj_3d->motion, obj_3d->act_id[0]) -
			amMotionGetStartFrame(obj_3d->motion, obj_3d->act_id[0]);
	
	if (obj_3d->frame[0] + spd > frame_max - 1.f) {
		if (overrun_frame) {
			*overrun_frame	= obj_3d->frame[0] + spd - (frame_max - 1.f);
		}
		return TRUE;
	}
	
	if (overrun_frame) {
		*overrun_frame	= 0.f;
	}
	
	return FALSE;
}

// ############################################################################
// ボスモーションコールバック
// ############################################################################
// =======================================================================
// gmBsCmnBossMotionCallbackFunc
/*!
  ボスモーションコールバックシステム BMCBリンクワーク初期化
 
  @param bmcb_link	[io]	BMCBリンクワーク
  @param bmcb_func	[in]	BMCB関数
  @param bmcb_param	[io]	パラメータ
  
  @note
  ボスモーションCBリンクワークを初期化します。
 */
// =======================================================================
void gmBsCmnInitBossMotionCBLink(GMS_BS_CMN_BMCB_LINK *bmcb_link,
								 GMF_BS_CMN_BMCB_FUNC bmcb_func,
								 void *bmcb_param)
{
	MTM_ASSERT(bmcb_link);
	MTM_ASSERT(bmcb_func);
	
	amZeroMemory(bmcb_link, sizeof(GMS_BS_CMN_BMCB_LINK));
	bmcb_link->bmcb_func	= bmcb_func;
	bmcb_link->bmcb_param	= bmcb_param;
}

// =======================================================================
// gmBsCmnClearBossMotionCBLink
/*!
  ボスモーションコールバックシステム BMCBリンクワーククリア
 
  @param bmcb_link	[io]	BMCBリンクワーク
  
  @note
  リンクワークの設定を全てクリアします。
 */
// =======================================================================
void gmBsCmnClearBossMotionCBLink(GMS_BS_CMN_BMCB_LINK *bmcb_link)
{
	MTM_ASSERT(bmcb_link);
	MTM_ASSERT(bmcb_link->next == NULL && bmcb_link->prev == NULL);	// 既にリストから外れていることが前提
	
	amZeroMemory(bmcb_link, sizeof(GMS_BS_CMN_BMCB_LINK));
}

// =======================================================================
// gmBsCmnBossMotionCallbackFunc
/*!
  ボスモーションコールバックシステム メインコールバック関数
 
  @param motion			[in]	モーションデータ
  @param object			[in]	NNオブジェクト
  @param mtn_cb_param	[io]	パラメータ
  
  @note
  モーションコールバックシステムに登録されたコールバック関数の呼び出しを行います。
 */
// =======================================================================
void gmBsCmnBossMotionCallbackFunc(const AMS_MOTION *motion,
								   const NNS_OBJECT *object,
								   void *mtn_cb_param)
{
	GMS_BS_CMN_BMCB_MGR	*bmcb_mgr	= (GMS_BS_CMN_BMCB_MGR*)mtn_cb_param;
	GMS_BS_CMN_BMCB_LINK	*cur_bmcb;
	
	// 最初の有効ノードを取得
	cur_bmcb	= bmcb_mgr->bmcb_head.next;
	
	// リスト登録順に実行
	while (cur_bmcb) {
		if (cur_bmcb->bmcb_func) {
			cur_bmcb->bmcb_func(motion, object, cur_bmcb->bmcb_param);
		}
		else {
			// tail到達
			break;
		}
		
		cur_bmcb	= cur_bmcb->next;
		
		// NULL終端による番兵は想定していない
		MTM_ASSERT(cur_bmcb);
	}
}


// ############################################################################
// ノードマトリクス格納処理
// ############################################################################
// =======================================================================
// gmBsCmnMotionCallbackStoreNodeMatrix
/*!
  モーションコールバック ノードのワールド変換マトリクスを格納
 
  @param motion			[in]	モーションデータ
  @param object			[in]	NNオブジェクト
  @param mtn_cb_param	[io]	パラメータ
  
  @note
  指定されたノードのワールドマトリクスを、指定の領域に格納します。
 */
// =======================================================================
void gmBsCmnMotionCallbackStoreNodeMatrix(const AMS_MOTION *motion,
										  const NNS_OBJECT *object,
										  void *mtn_cb_param)
{
	GMS_BS_CMN_SNM_WORK	*snm_work	= (GMS_BS_CMN_SNM_WORK*)mtn_cb_param;
	NNS_MATRIX	base_mtx;
	
	MTM_ASSERT(mtn_cb_param);
	
	/*
	  階層マトリクスから、ノードのワールドマトリクスを取得します。
	  メモ：
	  マトリクスパレットは階層マトリクスに対して基本姿勢逆行列が乗算されたものなので
	  ノード自身のワールドマトリクスにはなりません。
	  従って、ここでは階層マトリクスリストの生成までで止めておくのが正解。
	  （nnCalcMatrixPaletteMatrixList()などを行う必要はない。）
	 */
	
	// ベースマトリクス取得
	nnMakeUnitMatrix(&base_mtx);
	nnMultiplyMatrix(&base_mtx, &base_mtx, amMatrixGetCurrent());
	
	// 指定ノードの階層マトリクスを取得
	for (Sint32 i = 0; i < snm_work->reg_node_cnt; ++i) {
		Sint32	node_index	= snm_work->node_info_list[i].node_index;
		NNS_MATRIX	node_mtx;
		
		// 指定ノードの階層マトリクスを求める
		nnCalcNodeMatrixTRSList(&node_mtx, object, node_index, motion->data, &base_mtx);
		// 指定の領域にマトリクスをコピー
		amCopyMemory(&snm_work->node_info_list[i].node_w_mtx,
					 &node_mtx,
					 sizeof(NNS_MATRIX));
	}
}


// ############################################################################
// ノードマトリクス操作処理
// ############################################################################
// =======================================================================
// gmBsCmnMotionCallbackStoreNodeMatrix
/*!
  マトリクスパレットコールバック ノードのマトリクスパレットを書き換え
  （描画スレッドで呼び出し）
  
  @param mtx_plt		[in]	モーションデータ
  @param object			[in]	NNオブジェクト
  @param mplt_cb_param	[io]	パラメータ
  
  @note
  パラメータで渡されたワールドマトリクスが指定ノードに反映されるように、
  マトリクスパレットを書き換えます。
 */
// =======================================================================
void gmBsCmnMtxpltCallbackControlNodeMatrix(NNS_MATRIX *mtx_plt,
											const NNS_OBJECT *object,
											void *mplt_cb_param)
{
	GMS_BS_CMN_CNM_PARAM	*cnm_param	= (GMS_BS_CMN_CNM_PARAM*)mplt_cb_param;
	GMS_BS_CMN_CNM_NODE_INFO	*node_info_list	= (GMS_BS_CMN_CNM_NODE_INFO*)(cnm_param + 1);
	
	if (mplt_cb_param == NULL) {
		return;
	}
	
	/*
	  マトリクスパレットはノードのワールドマトリクスではないため、
	  与えられたワールドマトリクスをセットした後、
	  最終的に基本姿勢逆行列を乗算してマトリクスパレット化しています。
	  （右乗算モードでは乗算前に基本姿勢逆行列の逆行列を乗算してワールドマトリクス化しています。
	  また、左乗算モードでは基本姿勢逆行列には干渉しないため、マトリクスパレット化・ワールドマトリクス化等はしていません。）
	 */
	
	
	// 変更前のマトリクスパレットを退避
	NNS_MATRIX	*orig_mtx_plt	= (NNS_MATRIX*)amDrawMallocWorkBuffer(object->nMtxPal * sizeof(NNS_MATRIX));
	amCopyMemory(orig_mtx_plt, mtx_plt, object->nMtxPal * sizeof(NNS_MATRIX));
	
	
	// 指定ノードのマトリクスパレットを書き換え
	for (Sint32 i = 0; i < cnm_param->reg_node_cnt; ++i) {
		GMS_BS_CMN_CNM_NODE_INFO	*cnm_node_info	= &node_info_list[i];
		
		if (cnm_node_info->enable) {
			Sint32 mtx_idx	= object->pNodeList[cnm_node_info->node_index].iMatrix;
			
			if (mtx_idx != NND_MTXIDX_NIL) {
				NNS_MATRIX	candidate_mtx;
				NNS_MATRIX	inv_view_mtx;
				NNS_MATRIX	node_w_mtx;
				
				// ビューマトリクスをはがす ＆ 指定ノードのマトリクスパレットを取得
				nnInvertMatrix(&inv_view_mtx, amDrawGetWorldViewMatrix());
				nnMultiplyMatrix(&candidate_mtx,
								 &inv_view_mtx,
								 &mtx_plt[mtx_idx]);
				
				
				/* ---------- node_w_mtxを取得 ---------- */
				if (cnm_node_info->flag & GMD_BS_CMN_CNM_FLAG_LOCAL_COORDINATE) {
					NNS_MATRIX	cur_mtx;
					NNS_MATRIX	inv_cur_mtx;
					
					NNS_MATRIX	init_mtx;
					
					// 親のノードインデックス
					Sint32	parent_idx	= object->pNodeList[cnm_node_info->node_index].iParent;
					
					// 基本姿勢行列（基本姿勢逆行列の逆行列）
					nnInvertMatrix(&init_mtx,
								   &object->pNodeList[cnm_node_info->node_index].InvInitMtx);
					
					if (cnm_node_info->mode == GME_BS_CMN_CNM_MODE_REPLACE) {
						// 書き換えモードの場合は基本姿勢に対して座標変換を行う
						
						NNS_MATRIX	parent_mtx;
						NNS_MATRIX	diff_mtx;
						NNS_MATRIX	parent_init_mtx;
						
						// ビューマトリクスをはがした親ノードのマトリクスを得る
						nnMultiplyMatrix(&parent_mtx,
										 &inv_view_mtx,
										 &mtx_plt[object->pNodeList[parent_idx].iMatrix]);//はがす
						
						// 親ノードのワールドマトリクスを取得
						nnInvertMatrix(&parent_init_mtx,
									   &object->pNodeList[parent_idx].InvInitMtx);
						nnMultiplyMatrix(&parent_mtx, &parent_mtx, &parent_init_mtx);//親のwmtx
						
						// 親ノードの基本姿勢から対象ノードの基本姿勢への変換マトリクスを得る
						nnMultiplyMatrix(&diff_mtx,
										 &object->pNodeList[parent_idx].InvInitMtx,
										 &init_mtx);
						
						// 当該ノードを基本姿勢にリセットする（先祖ノードのTRSの影響は受ける）
						nnMultiplyMatrix(&cur_mtx, &parent_mtx, &diff_mtx);
						
						// 基本姿勢逆行列を掛けてマトリクスパレット化
						nnMultiplyMatrix(&candidate_mtx, &cur_mtx,
										 &object->pNodeList[cnm_node_info->node_index].InvInitMtx);
						// ※ビューマトリクスを乗算したものをmtx_pltに格納していないことに注意
						//   当該ノードを基本姿勢にリセットした後のマトリクスを関数内の以降の処理で使用する場合は
						//   ここに格納処理を追加する。
						
						// 書き込みマトリクスを作る
						// (ローカル座標系での変換マトリクス) * 基本姿勢にリセット済みの対象ノードの変換マトリクス
						// = (cur_mtx * cnm_node_info->node_w_mtx * inv_cur_mtx) * cur_mtx
						// = cur_mtx * cnm_node_info->node_w_mtx
						nnMultiplyMatrix(&node_w_mtx, &cur_mtx, &cnm_node_info->node_w_mtx);
					}
					else {
						// 乗算モード
						
						NNS_MATRIX	parent_cur_mtx;
						NNS_MATRIX	inv_parent_orig_mtx;
						
						// 対象ノードのマトリクスパレットを得る
						nnCopyMatrix(&cur_mtx, &candidate_mtx);
						
						// 対象ノードのワールドマトリクスを得る
						nnMultiplyMatrix(&cur_mtx, &cur_mtx, &init_mtx);
						
						// 親ノードの変更前のワールドマトリクスの「逆行列」を取得
						gmBsCmnGetNodeInvWorldMtx(&inv_parent_orig_mtx,
												  &object->pNodeList[parent_idx],
												  &inv_view_mtx, orig_mtx_plt);
						
						// 親ノードの変更後のワールドマトリクスを取得
						gmBsCmnGetNodeWorldMtx(&parent_cur_mtx,
											   &object->pNodeList[parent_idx],
											   &inv_view_mtx, mtx_plt);
						
						
						/* 変換マトリクスを作る */
						// モーション適用後の対象ノードローカルで座標変換するマトリクス）
						nnInvertMatrix(&inv_cur_mtx, &cur_mtx);
						nnMultiplyMatrix(&node_w_mtx, &cnm_node_info->node_w_mtx, &inv_cur_mtx);
						nnMultiplyMatrix(&node_w_mtx, &cur_mtx, &node_w_mtx);
						// 親ノードのモーション適用後の座標変換を継承するように反映
						nnMultiplyMatrix(&node_w_mtx, &inv_parent_orig_mtx, &node_w_mtx);
						nnMultiplyMatrix(&node_w_mtx, &parent_cur_mtx, &node_w_mtx);
					}
				}
				else {
					// node_w_mtxをそのままワールド座標系で解釈
					nnCopyMatrix(&node_w_mtx, &cnm_node_info->node_w_mtx);
				}
				
				/* ---------- モード別処理 ---------- */
				if (cnm_node_info->mode == GME_BS_CMN_CNM_MODE_MULT_LEFT) {
					
					// 基本姿勢逆行列には干渉しないので、ワールドマトリクスに変換不要
					
					// 左から乗算
					nnMultiplyMatrix(&candidate_mtx,
									 &node_w_mtx,
									 &candidate_mtx);
				}
				else if (cnm_node_info->mode == GME_BS_CMN_CNM_MODE_MULT_RIGHT) {
					NNS_MATRIX init_mtx;
					// 基本姿勢行列（基本姿勢逆行列の逆行列）
					nnInvertMatrix(&init_mtx,
								   &object->pNodeList[cnm_node_info->node_index].InvInitMtx);
					// ノードのワールドマトリクス
					nnMultiplyMatrix(&candidate_mtx, &candidate_mtx, &init_mtx);
					
					// 右から乗算
					nnMultiplyMatrix(&candidate_mtx,
									 &candidate_mtx,
									 &node_w_mtx);
				}
				else {
					MTM_ASSERT(cnm_node_info->mode == GME_BS_CMN_CNM_MODE_REPLACE);
					
					if (cnm_node_info->flag & GMD_BS_CMN_CNM_FLAG_INHERIT_SCALE &&
						!(cnm_node_info->flag & GMD_BS_CMN_CNM_FLAG_LOCAL_COORDINATE)) {
						// ↑FLAG_LOCAL_COORDINATEがオンの時は親のノードマトリクスが引き継がれるので
						//   改めてスケールを乗算しない
						
						NNS_MATRIX	init_mtx;
						// 基本姿勢行列（基本姿勢逆行列の逆行列）
						nnInvertMatrix(&init_mtx,
									   &object->pNodeList[cnm_node_info->node_index].InvInitMtx);
						// ノードのワールドマトリクス
						nnMultiplyMatrix(&candidate_mtx, &candidate_mtx, &init_mtx);
						// スケール取り出し
						AkMathExtractScaleMtx(&candidate_mtx, &candidate_mtx);
						
						nnMultiplyMatrix(&candidate_mtx,
										 &node_w_mtx,
										 &candidate_mtx);
					}
					else {
						nnCopyMatrix(&candidate_mtx, &node_w_mtx);
					}
				}
				
				// 左から乗算するときはワールドマトリクスに変換していないので↓不要
				if (cnm_node_info->mode != GME_BS_CMN_CNM_MODE_MULT_LEFT) {
					// 基本姿勢逆行列を乗算（階層マトリクス相当→マトリクスパレット化）
					nnMultiplyMatrix(&candidate_mtx,
									 &candidate_mtx,
									 &object->pNodeList[cnm_node_info->node_index].InvInitMtx);
				}
				
				// ビューマトリクスを乗算 ＆ マトリクスパレット書き込み
				nnMultiplyMatrix(&mtx_plt[mtx_idx],
								 amDrawGetWorldViewMatrix(),
								 &candidate_mtx);
			}
		}
	}
}

// =======================================================================
// gmBsCmnNodeControlObjectMainFunc
/*!
  ノード操作オブジェクト メイン処理関数
  
  @param obj_work	[io]	オブジェクトワーク
 */
// =======================================================================
void gmBsCmnNodeControlObjectMainFunc(OBS_OBJECT_WORK *obj_work)
{
	GMS_BS_CMN_NODE_CTRL_OBJECT	*ndc_obj	= (GMS_BS_CMN_NODE_CTRL_OBJECT*)obj_work;
	
	
	if (ndc_obj->proc_update) {
		ndc_obj->proc_update(obj_work);
	}
	else {
		// 念のため単位行列をセット
		nnMakeUnitMatrix(&ndc_obj->w_mtx);
	}
	
	// CNMマトリクスセット
	GmBsCmnSetCNMMtx(ndc_obj->cnm_mgr_work,
					 &ndc_obj->w_mtx,
					 ndc_obj->cnm_reg_id);
	
	// 有効フラグ反映
	GmBsCmnEnableCNMMtxNode(ndc_obj->cnm_mgr_work,
							ndc_obj->cnm_reg_id,
							ndc_obj->is_enable);
}

// =======================================================================
// gmBsCmnGetNodeWorldMtx
/*!
  マトリクスパレットから指定ノードのワールドマトリクスを取得
  
  @param dest_mtx		[out]	結果格納先
  @param node			[in]	対象ノード
  @param inv_view_mtx	[in]	ビューマトリクスの逆行列
  @param mtx_plt		[in]	参照マトリクスパレット
 */
// =======================================================================
void gmBsCmnGetNodeWorldMtx(NNS_MATRIX *dest_mtx, const NNS_NODE *node,
							const NNS_MATRIX *inv_view_mtx, const NNS_MATRIX *mtx_plt)
{
	NNS_MATRIX	init_mtx;
						
	// ビューマトリクスをはがしたノードのマトリクスを得る
	nnMultiplyMatrix(dest_mtx, inv_view_mtx, &mtx_plt[node->iMatrix]);
	
	// 親ノードのワールドマトリクスを取得
	nnInvertMatrix(&init_mtx, &node->InvInitMtx);
	nnMultiplyMatrix(dest_mtx, dest_mtx, &init_mtx);
}

// =======================================================================
// gmBsCmnGetNodeInvWorldMtx
/*!
  マトリクスパレットから指定ノードの「逆」ワールドマトリクスを取得
  
  @param dest_mtx		[out]	結果格納先
  @param node			[in]	対象ノード
  @param inv_view_mtx	[in]	ビューマトリクスの逆行列
  @param mtx_plt		[in]	参照マトリクスパレット
  
  @note
  gmBsCmnGetNodeWorldMtx()の結果から逆行列を取得するよりも手数が少ない実装となっています。
 */
// =======================================================================
void gmBsCmnGetNodeInvWorldMtx(NNS_MATRIX *dest_mtx, const NNS_NODE *node,
							   const NNS_MATRIX *inv_view_mtx, const NNS_MATRIX *mtx_plt)
{
	// ビューマトリクスをはがしたノードのマトリクスを得る
	nnMultiplyMatrix(dest_mtx, inv_view_mtx, &mtx_plt[node->iMatrix]);
	
	// 逆行列を取得（この時点で基本姿勢行列はくっついたまま）
	nnInvertMatrix(dest_mtx, dest_mtx);
	
	// 既に逆行列にしてあるので、基本姿勢「逆」行列を「左から」掛けてワールドマトリクス（の逆行列）化
	nnMultiplyMatrix(dest_mtx, &node->InvInitMtx, dest_mtx);
}


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
