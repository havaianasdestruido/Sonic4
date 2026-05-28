// ================================================================
/*!
  @file objDiffCollisionCheck.c
  @brief 地形判定 差分値テーブル地形用 判定ルーチン

  @author mana
                Copyright(c) 2004 Dimps

  $Id: objDiffCollisionCheck.cpp 2 2011-04-11 05:21:26Z thamada $
  $Date:: 2011-04-11 14:21:26 +0900#$
 */
// ================================================================
/*
 * Memo
 *
 *
 */


//----- Include Files --------------------------------------------------
#include "pch.h"
// #include "PCH.mch"
#include "objDiffCollisionCheck.h"
#include "objDiffCollisionField.h"
#include "objBlockCollisionField.h"
#include "objDiffCollisionObject.h"

#include "gmMain.h"		// HOGスペステ対応
#include "gmPlayer.h"	// HOGスペステ対応
//----- Macros ---------------------------------------------------------

//----- Macros Functions -----------------------------------------------

//----- Definitions ----------------------------------------------------
#define OBD_MOVE_SPDX_MAX (11) ///< 横移動速度最大dot値 坂道吸着チェック用
#define OBD_MOVE_SPDY_MAX (14) ///< 縦移動速度最大dot値 坂道吸着チェック用

#define GMD_COL_VIB_COLLECT_CHK		(0)	// オブジェクトの地面接地補正で振動してしまう症状対策のON/OFF(1でON)

// hit_vec
//  for width
#define OBD_HIT_VEC_FRONT	1		//!< 進行方向が壁にヒット
#define OBD_HIT_VEC_BACK	2		//!< 進行方向の逆側が壁にヒット

//----- External Declarations ------------------------------------------

//----- Static Declarations --------------------------------------------

//----- Global Variables -----------------------------------------------

//----- Local Variables ------------------------------------------------

//----- Global Functions -----------------------------------------------

//----- Local Functions ------------------------------------------------
static void objDiffCollisionDirCheck(OBS_OBJECT_WORK * pWork);
//static void objDiffCollisionSimpleCheck(OBS_OBJECT_WORK * pWork);

static void objDiffCollisionDirWidthCheck( OBS_OBJECT_WORK * pWork, u8 ucWall, s32 );
static void objDiffCollisionDirHeightCheck( OBS_OBJECT_WORK * pWork );
static s8 objDiffCollisionSimpleOverCheck( OBS_OBJECT_WORK * pWork );

#if (GMD_COL_VIB_COLLECT_CHK == 1)
inline s8 objDiffColVibCheck(s32, s32, s16, s16, s16, s16, u16, u16, s8, s8 );
#endif	//(GMD_COL_VIB_COLLECT_CHK == 1)
inline void objDiffColDirMove( s32* lPosX, s32* lPosY, s8 cDelta, u16 ucColFlag );

static void objDiffAttrSet(OBS_OBJECT_WORK * pWork, u32 ulAttr );

// ================================================================
// objDiffAttrSet
/*!
  指定した属性をオブジェクト地形属性フラグに設定する
 
  @param pWork    [in] オブジェクトポインタ
  @param ulAttr   [in] 地形属性データ
 
 @note
    フラグにズレが出た時のクッション役（ズレを起こすな）\n
    ■予定 この関数は関数ポインタにしてゲームごとの設定ができるようにする。
 */
// ================================================================
static void objDiffAttrSet(OBS_OBJECT_WORK * pWork, u32 ulAttr )
{
#if 0
    pWork->col_flag |= ulAttr;
#else
	// すり抜け足場属性は未使用
	if (ulAttr & OBD_COL_DATA_ATTR_CLIFF) {
		// 崖
		pWork->col_flag |= OBD_COLAT_CLIFF;
	}
	if (ulAttr & OBD_COL_DATA_ATTR_GRAIND) {
		// グラインド
		pWork->col_flag |= OBD_COLAT_GRAIND;
	}
#endif
/*
    u8 ucChipNo = (u8)(ulAttr >> NLD_MAPDAT_ATTR_BK_SHIFT);
    
    // 崖チェック
    if ( ucChipNo == NLD_MAPDAT_ATTR_BK_CLIFF ){
        pWork->usColFlag |= OBD_COLAT_CLIFF;
    }
    if ( ucChipNo == NLD_MAPDAT_ATTR_BK_GRIND_CLIFF )
        pWork->usColFlag |= OBD_COLAT_GRAIND_CLIFF;
    if ( ucChipNo == NLD_MAPDAT_ATTR_BK_GRIND ||
         ucChipNo == NLD_MAPDAT_ATTR_BK_GRIND_CLIFF ){
        pWork->usColFlag |= OBD_COLAT_GRAIND;
    }
 */
}

// ================================================================
// objCollision
/*!
  マップ当たりチェック 統合

  @param pData [out] OBS_COL_CHK_DATAポインタ
    
  @return   接地面までの距離(正: 浮いている  負: 埋まっている)
 
  @note
    オブジェクトフラグに合わせたチェック方法を行う
 */
// ================================================================
static s32 objCollision( OBS_COL_CHK_DATA* pData )
{
    if ( g_obj.flag & OBD_OBJ_COL_BLOCK )
        return ObjBlockCollision( pData );
    return ObjDiffCollision( pData );
    
}
// ================================================================
// objCollisionFast
/*!
  簡易 マップ当たりチェック

  @param pData [out] OBS_COL_CHK_DATAポインタ
  @return   接地面までの距離(正: 浮いている  負: 埋まっている)
 
  @note
    オブジェクトフラグに合わせたチェック方法を行う

 */
// ================================================================
static s32 objCollisionFast( OBS_COL_CHK_DATA* pData )
{
    if ( g_obj.flag & OBD_OBJ_COL_BLOCK )
        return ObjBlockCollision( pData );
    return ObjDiffCollisionFast( pData );
}
// ================================================================
// ObjCollisionUnion
/*!
  マップ当たりチェック 統合

  @param pWork [io] オブジェクトワークポインタ
  @param pData [in] 地形チェックワークポインタ
 
  @return 接地面までの距離(正: 浮いている  負: 埋まっている)
 
  @note
    座標から指定方向に 地形をチェックし、地形とオブジェクト地形の接地面までの距離を求める
 */
// ================================================================
s32 ObjCollisionUnion( OBS_OBJECT_WORK * pWork, OBS_COL_CHK_DATA* pData )
{
    s32 lDiff = 32,lDiffObj = 32; // 適当な浮いている値を設定
    
    if (!( pWork->move_flag & OBD_MOVE_NOCOLFIELD )){
        lDiff = objCollision( pData );
    }

    if (!( pWork->move_flag & OBD_MOVE_NOCOLOBJ )){
        // 地形オブジェクトのチェック
        lDiffObj = ObjCollisionObjectCheck(pWork, pData );
        
        // 埋まっている方を適用
        if ( lDiff > lDiffObj ){
            lDiff = lDiffObj;
        }
    }

    return lDiff;
}
// ================================================================
// ObjCollisionFastUnion
/*!
  簡易マップ当たりチェック 統合

  @param pData [in] 地形チェックワークポインタ
 
  @return   接地面までの距離(正: 浮いている  負: 埋まっている)
 
  @note
    座標から指定方向に 地形をチェックし、地形とオブジェクト地形の接地面までの距離を求める\n
    簡易チェックのため複雑な地形や速度で挙動がおかしくなる。
 */
// ================================================================
s32 ObjCollisionFastUnion( OBS_COL_CHK_DATA* pData )
{
    s32 lDiff = 32,lDiffObj = 32; // 適当な浮いている地を設定

    lDiff = objCollisionFast( pData );

    // 地形オブジェクトのチェック
    lDiffObj = ObjCollisionObjectFastCheck( pData );
        
    // 埋まっている方を適用
    if ( lDiff > lDiffObj ){
        lDiff = lDiffObj;
    }
    return lDiff;
}

// ================================================================
// ObjDiffCollisionEarthCheck
/*!
  オブジェクトの地面チェック 
 
  @param pWork [io] オブジェクトワークポインタ
 
  @note
    オブジェクトの地形矩形を元にオブジェクトを地面へ吸着します。\n
    オブジェクトの座標データを補正します。\n
    下り坂では速度に比例した差分値以上で地面から離れます。\n
    角度を用いるオブジェクトは角度情報も更新されます。
    内部で分岐する簡易チェックは未作成
 */
// ================================================================
void ObjDiffCollisionEarthCheck( OBS_OBJECT_WORK * pWork )
{
    //if ( pWork->move_flag & OBD_MOVE_COL_SIMPLE )
    // 単純チェック（あまり動かない敵やアイテムなどに使用）
    //     objDiffCollisionSimpleCheck(pWork);
    //else
    // 通常チェック
    objDiffCollisionDirCheck(pWork);
    
}
// ================================================================
// objDiffCollisionDirCheck
/*!
  オブジェクトの地面チェック 
 
  @param pWork [io] オブジェクトワークポインタ
 
  @return   返値説明
 
  @note
    オブジェクトの地形矩形を元に\n
    オブジェクトを地面へ吸着します。\n
    オブジェクトの座標データを補正します。\n
    下り坂では速度に比例した差分値以上で地面から離れます。\n
    ジャンプフラグを立てる事坂道吸着は回避されます。\n
    オブジェクトの角度によって地面方向を変化させてチェックします。
    
 */
// ================================================================
static void objDiffCollisionDirCheck(OBS_OBJECT_WORK * pWork)
{
	s32 sSpd; // 左右HITチェック保持用
#if 0	// 高速移動時分割チェックのためこの処理を１フレームに数回通過するため
		// OBD_MOVE_UNDERPREVのリセットが意図した通りに働いていない。
		// この処理をobjObject.cpp:ObjObjectCollision に移動して経過観察@kuramoto(09/09/08)
	// 地形フラグ寝かす
	pWork->move_flag &= ~OBD_MOVE_UNDERPREV;
	if ( pWork->move_flag & OBD_MOVE_UNDER )
		pWork->move_flag |= OBD_MOVE_UNDERPREV;
	pWork->move_flag &= ~OBD_MOVE_COL_MASK;
#endif
	// 属性フラグクリア
	pWork->col_flag = 0;

#if 0	
	sSpd = ( pWork->move.x );
#else
	if ((pWork->dir_fall + 0x2000) & 0x4000) {
		// 重力方向横の場合はYを横壁チェックの移動量とする
		sSpd = ( pWork->move.y );
	} else {
		// 重力方向縦の場合はXを横壁チェックの移動量とする
		sSpd = ( pWork->move.x );
	}
#endif
	// 左右のチェックを行う
	if ( !(pWork->move_flag & OBD_MOVE_NOCOL_W ) )
		objDiffCollisionDirWidthCheck( pWork, 0, sSpd );

	// 上下のチェックを行う
	if ( !(pWork->move_flag & OBD_MOVE_NOCOL_H ) )
		objDiffCollisionDirHeightCheck( pWork );

	if (  ( pWork->move_flag & OBD_MOVE_UNDER )
		&&(  (!pWork->dir.z)
		   ||(pWork->dir.z == 0x8000) ) ) {
		// 完全な平地に立っているときは垂直方向のＹ小数点以下を切り捨てる
		if ((pWork->dir_fall + 0x2000) & 0x4000) {
			// 横向き重力時はＸが地面方向
			pWork->pos.x &= 0xfffff000;
		} else {
			// 縦向き重力時はＹが地面方向
			pWork->pos.y &= 0xfffff000;
		}
	}
	// 左右の壁フラグのチェックを行う
	if ( !(pWork->move_flag & OBD_MOVE_NOCOL_W ) )
		objDiffCollisionDirWidthCheck( pWork, 1, sSpd );

}
// ================================================================
// objDiffSufSet
/*!
  オブジェクトからチェックフラグ設定
 
  @param pWork [in] オブジェクトワークポインタ
 
  @return   チェックフラグ
 
 */
// ================================================================
static u16 objDiffSufSet( OBS_OBJECT_WORK * pWork )
{
    u16 usSuf = 0;

    if ( pWork->move_flag & OBD_MOVE_THROUGH )
        usSuf |= OBD_COL_THROUGH;
        
    if (!( pWork->move_flag & OBD_MOVE_AIRFOOT ))
        usSuf |= OBD_COL_THROUGH;

    if ( pWork->flag & OBD_OBJECT_B )
        usSuf |= OBD_COL_B;
    
    if ( pWork->move_flag & OBD_MOVE_LIMIT_OUT )
        usSuf |= OBD_COL_LIMITWALL;
    
    return usSuf;
}
// ================================================================
// objDiffCollisionDirWidthCheck
/*!
  オブジェクトの壁チェック 
 
  @param pWork  [io] オブジェクトワークポインタ
  @param ucWall [in] FALSE 通常判定 TRUE 壁フラグ設定のみ行う
  @param sSpd   [in] 移動速度 1:19:12
 
  @note
    角度有り地形チェックの左右チェック
    進行方向から順にチェック、順によって設定値が変化する
 */
// ================================================================
static void objDiffCollisionDirWidthCheck( OBS_OBJECT_WORK * pWork, u8 ucWall, s32 sSpd )
{
    OBS_COL_CHK_DATA tData = {0};

    u16 usDir; // ワーク角度
    s8  cDif;    // 差分値
    s8  cDif2;   // 差分値
	s8	cDifF = 0;	// 前方差分値保持
    s16 sRecX1 = 0;  // 左右レクト
    s16 sRecY1 = 0;  // 左右レクト
    s16 sRecX2 = 0; // 左右レクト
    s16 sRecY2 = 0; // 左右レクト
    u8 ucFlip = 0;   // 上下フリップチェック用
    s8 cTopDown = 0; // 頭からのオフセット
    s32 lPosX; // ワーク座標
    s32 lPosY;
    s32 sSpdX;
	u8	hit_vec = 0;		// fx32小数点以下補正チェック用
	
    // チェックフラグを設定
    tData.flag = objDiffSufSet(pWork);
    tData.flag |= OBD_COL_THROUGH;

    lPosX = pWork->pos.x >> FX32_SHIFT;
    lPosY = pWork->pos.y >> FX32_SHIFT;

	// 前向き角度をセット
	usDir = OBD_OBJECT_STANDARD_DIR;								// 左向きが基準

	// 天井の時は反対
	if ( (((pWork->dir.z + 0x2000 ) & 0xc000) >> 14) == 2 ){
		usDir += 0x8000;
	}

	// 移動方向によってチェック順を変える
	// sSpdX = ( ((pWork->spd_m ) * mtMathCos( (u16)(pWork->dir.z ) )) >> 16 );
	sSpdX = sSpd;
#if 1
	// 向いている方向を進行方向とする
	if ( !(pWork->disp_flag & OBD_DISP_HFLIP) ){
		usDir = (u16)-usDir;
		ucFlip =1;
	}
#else
	if ( sSpdX > 0 ){
		// 右を向いている
		usDir = (u16)-usDir;
		ucFlip =1;
	}else{
		if ( !(pWork->disp_flag & OBD_DISP_HFLIP) ){
			usDir = (u16)-usDir;
			ucFlip =1;
		}
	}
#endif

#if 0	// 重力方向変化に伴うチェック方向対応（元の状態：上下のみの対応。90度/270度の変化には対応していない）
	if ( pWork->dir_fall ){
		usDir += pWork->dir_fall;
		if ( sSpdX > 0 ){
			usDir = (u16)-usDir;
			ucFlip = 0;
		}
	}
#else	// 重力方向変化に伴うチェック方向対応

	usDir += pWork->dir_fall;
	switch (((pWork->dir_fall + 0x2000) >> 14) & 3) {
		case 2:	// 重力方向上
			ucFlip ^= 1;
			usDir = (u16)(usDir - 0x8000);									// 反転
			break;
			
		default:
		case 0:	// 重力方向下
		case 1:	// 重力方向左
		case 3:	// 重力方向右
			break;
	}
#endif	// 重力方向変化に伴うチェック方向対応

	if ( (((pWork->dir.z + 0x2000 ) & 0xc000) >> 14) == 2 ){
		ucFlip ^= 1;
	}
	// プレイヤーの角度をプラスし、現在の前方向角度を計算する
	// ucDir += pWork->ucDir;
	if (!( pWork->move_flag & OBD_MOVE_JUMP ))
		usDir += pWork->dir.z;
    // 頭をぶつける場合
#if 1
	// 20100129 Ishizaki 横方向が重力方向の時に、横と頭上が同時に触れるとめり込む事があった対応
	cTopDown = objDiffCollisionSimpleOverCheck(pWork);
	if (cTopDown > -4) {
		// 規定値より小さい場合は補正
		cTopDown = 4;
	}
	else {
		cTopDown = (s8)(-cTopDown + 1);
	}
	// field_rectの高さより頭上めり込みが大きい場合は補正
	if (cTopDown >= (s8)(pWork->field_rect[OBD_BOTTOM] - pWork->field_rect[OBD_TOP])) {
		cTopDown = (s8)(pWork->field_rect[OBD_BOTTOM] - pWork->field_rect[OBD_TOP] - 1);
	}
#else
    if ( objDiffCollisionSimpleOverCheck(pWork) ){
        cTopDown = 6;
    }
#endif
    
    // ▼ TIPS!
    // バグの具合によっては角度の計算に補正を入れる
    // if ((s8)(ucDir + 0x20) > 0) {
    //     if ((s8)ucDir > 0) ucDir += 0x20-1;
    //     else               ucDir += 0x20;
    // } else {
    //     if ((s8)ucDir > 0) ucDir += 0x20;
    //     else               ucDir += 0x20-1;
    // }
    // こんな感じ (ずらした結果が左側で元のチェック方向が左側なら+0x1f右側なら+0x20 ～)
    // 多分この1度の差で垂直な壁を登る時にまっすぐにならないバグとかかと
    
    // 角度に合わせてチェック方向を設定する
    switch (((usDir + 0x2000) & 0xc000) >> 14) {
    case 0:
        // 下方向が進行方向
        sRecY1 = (s16)(pWork->field_rect[OBD_RIGHT] + pWork->field_ajst_w_db_f);
        sRecY2 = sRecY1;
        sRecX1 = (s16)(pWork->field_rect[OBD_BOTTOM] - pWork->field_ajst_w_db_b);
        //sRecX2 = (s16)(pWork->field_rect[OBD_TOP] + 4);
        sRecX2 = (s16)(pWork->field_rect[OBD_TOP] + cTopDown); // 20100129 Ishizaki 横方向が重力方向の時に、横と頭上が同時に触れるとめり込む事があった対応
        if ( ucFlip ){	// 20100201 Ishizaki コリジョン抜け対応
            sRecX1 = (s16)-sRecX1;
            sRecX2 = (s16)-sRecX2;
        }
        tData.vec = OBD_COL_DOWN;
        break;
    case 1:
        // 左方向が進行方向
        sRecX1 = (s16)(pWork->field_rect[OBD_LEFT] - pWork->field_ajst_w_dl_f);
        sRecX2 = sRecX1;
        sRecY1 = (s16)(pWork->field_rect[OBD_BOTTOM] - pWork->field_ajst_w_dl_b);
        sRecY2 = (s16)(pWork->field_rect[OBD_TOP] + cTopDown);
        if ( ucFlip ){
            sRecY1 = (s16)-sRecY1;
            sRecY2 = (s16)-sRecY2;
        }
        tData.vec = OBD_COL_LEFT;
        break;
    case 2:
        // 上方向が進行方向
        sRecY1 = (s16)(pWork->field_rect[OBD_LEFT] - pWork->field_ajst_w_dt_f);
        sRecY2 = sRecY1;
        sRecX1 = (s16)-(pWork->field_rect[OBD_BOTTOM] - pWork->field_ajst_w_dt_b);
        //sRecX2 = (s16)(pWork->field_rect[OBD_TOP] + 4);
        sRecX2 = (s16)-(pWork->field_rect[OBD_TOP] + cTopDown); // 20100129 Ishizaki 横方向が重力方向の時に、横と頭上が同時に触れるとめり込む事があった対応
        tData.vec = OBD_COL_UP;
		if ( ucFlip ){	// 20100201 Ishizaki コリジョン抜け対応
            sRecX1 = (s16)-sRecX1;
            sRecX2 = (s16)-sRecX2;
        }
        break;
    case 3:
        // 右方向が進行方向
        sRecX1 = (s16)(pWork->field_rect[OBD_RIGHT] + pWork->field_ajst_w_dr_f);
        sRecX2 = sRecX1;
        sRecY1 = (s16)-(pWork->field_rect[OBD_BOTTOM] - pWork->field_ajst_w_dr_b);
        sRecY2 = (s16)-(pWork->field_rect[OBD_TOP] + cTopDown);
        tData.vec = OBD_COL_RIGHT;
        if ( ucFlip ){
            sRecY1 = (s16)-sRecY1;
            sRecY2 = (s16)-sRecY2;
        }
        break;
    }
    
    // その方向の地形差分値を取得
    tData.pos_x = lPosX + sRecX1;
    tData.pos_y = lPosY + sRecY1;
    cDif  = (s8)ObjCollisionUnion( pWork, &tData );
    tData.pos_x = lPosX + sRecX2;
    tData.pos_y = lPosY + sRecY2;
    cDif2 = (s8)ObjCollisionUnion( pWork, &tData );

    // 小さい方の差分値を設定
    if ( cDif < cDif2) cDif = cDif;
    else               cDif = cDif2;

	cDifF = cDif;
    if ( cDif <= 0 ){
		hit_vec |= OBD_HIT_VEC_FRONT;
        if ( !ucWall ){
            // 方向に合わせて数値を設定
            objDiffColDirMove( &lPosX, &lPosY, cDif, tData.vec);
        }else{
            pWork->move_flag |= OBD_MOVE_FRONT;

#if 0	// 既存処理
            if (!( pWork->move_flag & OBD_MOVE_NOSPD )){
//				if ( tData.vec == OBD_COL_LEFT && pWork->move.x < 0 ){
				if ( tData.vec == OBD_COL_LEFT && sSpdX < 0 ){
                    pWork->spd_m = 0;
                    pWork->spd.x = 0;
                }
//				if ( tData.vec == OBD_COL_RIGHT && pWork->move.x > 0 ){
				if ( tData.vec == OBD_COL_RIGHT && sSpdX > 0 ){
                    pWork->spd_m = 0;
                    pWork->spd.x = 0;
                }
            }
            // 天井と床が壁として立ちふさがった時は角度をクリアする
            if ( tData.vec & OBD_COL_Y ){
                //pWork->ucDir = 0; // 接地時角度を0クリア
                if (!( pWork->move_flag & OBD_MOVE_NOSPD )){
                    //pWork->spd.y = 0;		// 20090817 重力有効時に落下速度がクリアされる為コメントアウト
                    pWork->spd_m = 0;
                }
            }
#else	// HOGスペステ対応
//			if (g_gs_main_sys_info.game_mode == GSD_GAME_MODE_SPECIAL_STAGE) {
//				// スペステ時。（dir_fallが 4000/c000 時に床が壁になった時に引っかかる対策症状対策だが上手く判定取れない。要修正090912
//				u16	chk_flag = 0;
//				if (pWork->move_flag & OBD_MOVE_UNDERPREV)	chk_flag = 1;
//				if (pWork->move_flag & OBD_MOVE_UNDER)		chk_flag ^= 1;
//				if (!chk_flag) {
//					if (!( pWork->move_flag & OBD_MOVE_NOSPD )){
//						if ( tData.vec == OBD_COL_LEFT && sSpdX < 0 ){
//							pWork->spd_m = 0;
//							pWork->spd.x = 0;
//						}
//						if ( tData.vec == OBD_COL_RIGHT && sSpdX > 0 ){
//							pWork->spd_m = 0;
//							pWork->spd.x = 0;
//						}
//					}
//				}
//			} else {
				// 通常時
				if (!( pWork->move_flag & OBD_MOVE_NOSPD )){
					if ((pWork->dir_fall + 0x2000) & 0x4000) {
						if ( tData.vec == OBD_COL_UP && sSpdX < 0 ){
							pWork->spd_m = 0;
							if (!(pWork->move_flag & OBD_MOVE_NOCLR_SPDXY)) {
								pWork->spd.x = 0;
							}
						}
						if ( tData.vec == OBD_COL_DOWN && sSpdX > 0 ){
							pWork->spd_m = 0;
							if (!(pWork->move_flag & OBD_MOVE_NOCLR_SPDXY)) {
								pWork->spd.x = 0;
							}
						}
					} else {
						if ( tData.vec == OBD_COL_LEFT && sSpdX < 0 ){
							pWork->spd_m = 0;
							if (!(pWork->move_flag & OBD_MOVE_NOCLR_SPDXY)) {
								pWork->spd.x = 0;
							}
						}
						if ( tData.vec == OBD_COL_RIGHT && sSpdX > 0 ){
							pWork->spd_m = 0;
							if (!(pWork->move_flag & OBD_MOVE_NOCLR_SPDXY)) {
								pWork->spd.x = 0;
							}
						}
					}
				}
//			}
			// 天井と床が壁として立ちふさがった時は速度をクリアする
			if ( tData.vec & OBD_COL_Y ){
				if (!( pWork->move_flag & OBD_MOVE_NOSPD )){
					if (GSM_MAIN_STAGE_IS_SPSTAGE_NOT_RETRY()) {
//スペステ壁跳ね帰り仕様変更によりここの処理が不要且つ不具合原因となったためコメントアウト
//  本来±90度画面回転した状態(側面が地面)の時、回転作用により接していた面が壁になった場合の対処だったが
//  側面が地面且つ壁に接している時、壁から離れようとした時に速度クリアが実行されるという副作用が生じていました。
//						// 画面回転により地面が壁に変化した場合
//						// GmPlySeqInitFallでspd_mからspd.*に速度計算させてからspd_mをクリア
//						// （１フレーム速度クリア判定にギャップを持たせる）
//						u16	chk_flag = 0;
//						if (pWork->move_flag & OBD_MOVE_UNDERPREV)	chk_flag = 1;
//						if (pWork->move_flag & OBD_MOVE_UNDER)		chk_flag ^= 1;
//						if (!chk_flag) {
//							pWork->spd_m = 0;
//						}
					} else {
						// 通常時
						pWork->spd_m = 0;
					}
				}
			}
#endif	// スペステ対応
        }
    }
    
    // 逆移動方向のチェック
    // 角度反転
    usDir += 0x8000;

    // ▼ TIPS!
    // バグの具合によっては角度の計算に補正を入れる
    
    // 角度に合わせてチェック方向を設定する
    switch (((usDir + 0x2000) & 0xc000) >> 14) {
    case 0:
        // 下方向が進行方向
        sRecY1 = (s16)(pWork->field_rect[OBD_RIGHT] + pWork->field_ajst_w_db_f);
        sRecY2 = (s16)sRecY1;
        sRecX1 = (s16)-(pWork->field_rect[OBD_BOTTOM] - pWork->field_ajst_w_db_b);
        //sRecX2 = (s16)(pWork->field_rect[OBD_TOP] + 4);
        sRecX2 = (s16)-(pWork->field_rect[OBD_TOP] + cTopDown); // 20100129 Ishizaki 横方向が重力方向の時に、横と頭上が同時に触れるとめり込む事があった対応
        tData.vec = OBD_COL_DOWN;
		if ( ucFlip ){	// 20100201 Ishizaki コリジョン抜け対応
            sRecX1 = (s16)-sRecX1;
            sRecX2 = (s16)-sRecX2;
        }
        break;
    case 1:
        // 左方向が進行方向
        sRecX1 = (s16)(pWork->field_rect[OBD_LEFT] - pWork->field_ajst_w_dl_f);
        sRecX2 = (s16)sRecX1;
        sRecY1 = (s16)-(pWork->field_rect[OBD_BOTTOM] - pWork->field_ajst_w_dl_b);
        sRecY2 = (s16)-(pWork->field_rect[OBD_TOP] + cTopDown);
        tData.vec = OBD_COL_LEFT;
        if ( ucFlip ){
            sRecY1 = (s16)-sRecY1;
            sRecY2 = (s16)-sRecY2;
        }
        break;
    case 2:
        // 上方向が進行方向
        sRecY1 = (s16)(pWork->field_rect[OBD_LEFT] - pWork->field_ajst_w_dt_f);
        sRecY2 = (s16)sRecY1;
        sRecX1 = (s16)(pWork->field_rect[OBD_BOTTOM] - pWork->field_ajst_w_dt_b);
        //sRecX2 = (s16)(pWork->field_rect[OBD_TOP] + 4);
        sRecX2 = (s16)(pWork->field_rect[OBD_TOP] + cTopDown); // 20100129 Ishizaki 横方向が重力方向の時に、横と頭上が同時に触れるとめり込む事があった対応
        tData.vec = OBD_COL_UP;
		if ( ucFlip ){	// 20100201 Ishizaki コリジョン抜け対応
            sRecX1 = (s16)-sRecX1;
            sRecX2 = (s16)-sRecX2;
        }
        break;
    case 3:
        // 右方向が進行方向
        sRecX1 = (s16)(pWork->field_rect[OBD_RIGHT] + pWork->field_ajst_w_dr_f);
        sRecX2 = (s16)sRecX1;
        sRecY1 = (s16)(pWork->field_rect[OBD_BOTTOM] - pWork->field_ajst_w_dr_b);
        sRecY2 = (s16)(pWork->field_rect[OBD_TOP] + cTopDown);
        tData.vec = OBD_COL_RIGHT;
        if ( ucFlip ){
            sRecY1 = (s16)-sRecY1;
            sRecY2 = (s16)-sRecY2;
        }
        break;
    }

    // その方向の地形差分値を取得
    tData.pos_x = lPosX + sRecX1;
    tData.pos_y = lPosY + sRecY1;
    cDif  = (s8)ObjCollisionUnion( pWork, &tData);
    tData.pos_x = lPosX + sRecX2;
    tData.pos_y = lPosY + sRecY2;
    cDif2 = (s8)ObjCollisionUnion( pWork, &tData );
    
    // 小さい方の差分値を設定
    if ( cDif < cDif2) cDif = cDif;
    else               cDif = cDif2;

    if ( cDif <= 0 ){
		hit_vec |= OBD_HIT_VEC_BACK;
        if ( !ucWall ){
            // 方向に合わせて数値を設定
            objDiffColDirMove( &lPosX, &lPosY, cDif, tData.vec);
        }else{
#if 0	// Zone4-3迫る壁に後ろから押された時、前方の壁と挟まれた時に圧死できない
            // FRONTが既に立っている場合、挟まり防止のため、密着ではBACKを立てない
            if ( !(pWork->move_flag & OBD_MOVE_FRONT) || cDif < 0 )
                pWork->move_flag |= OBD_MOVE_BACK;
#else	// Front側のめり込み量もチェックして圧死しやすく
            if ( !(pWork->move_flag & OBD_MOVE_FRONT)
            	|| (cDifF < 0)
            	|| (cDif < 0) )
				pWork->move_flag |= OBD_MOVE_BACK;
#endif	// Zone4-3迫る壁対策ここまで

            // 天井と床が壁として立ちふさがった時は角度をクリアする
            //if ( tData.vec == OBD_COL_DOWN || tData.vec == OBD_COL_UP )
            //    pWork->ucDir = 0; // 接地時角度を0クリア
            
            // 速度クリア
            if (!( pWork->move_flag & OBD_MOVE_NOSPD )){
#if 0
                if ( tData.vec == OBD_COL_LEFT && sSpd < 0 )
                    pWork->spd_m = 0;
                if ( tData.vec == OBD_COL_RIGHT && sSpd > 0 )
                    pWork->spd_m = 0;

//				if ( tData.vec == OBD_COL_LEFT && pWork->spd.x < 0 )
				if ( tData.vec == OBD_COL_LEFT && sSpd < 0 )
                    pWork->spd.x = 0;
//				if ( tData.vec == OBD_COL_RIGHT && pWork->spd.x > 0 )
				if ( tData.vec == OBD_COL_RIGHT && sSpd > 0 )
                    pWork->spd.x = 0;
#else
				if ((pWork->dir_fall + 0x2000) & 0x4000) {
					if ( tData.vec == OBD_COL_UP && sSpd < 0 ){
						pWork->spd_m = 0;
						if (!(pWork->move_flag & OBD_MOVE_NOCLR_SPDXY)) {
							pWork->spd.x = 0;
						}
					}
					if ( tData.vec == OBD_COL_DOWN && sSpd > 0 ){
						pWork->spd_m = 0;
						if (!(pWork->move_flag & OBD_MOVE_NOCLR_SPDXY)) {
							pWork->spd.x = 0;
						}
					}
				} else {
					if ( tData.vec == OBD_COL_LEFT && sSpd < 0 ){
						pWork->spd_m = 0;
						if (!(pWork->move_flag & OBD_MOVE_NOCLR_SPDXY)) {
							pWork->spd.x = 0;
						}
					}
					if ( tData.vec == OBD_COL_RIGHT && sSpd > 0 ){
						pWork->spd_m = 0;
						if (!(pWork->move_flag & OBD_MOVE_NOCLR_SPDXY)) {
							pWork->spd.x = 0;
						}
					}
				}
#endif
            }
        }
            
    }

    if ( !ucWall ){
        // 座標を反映する
        pWork->pos.x -= ((pWork->pos.x >> FX32_SHIFT) - lPosX) << FX32_SHIFT;
        pWork->pos.y -= ((pWork->pos.y >> FX32_SHIFT) - lPosY) << FX32_SHIFT;

		// DS仕様と違い、壁ヒット時に fx32の小数点以下単位で
		// 表示位置のがたつきが生じるため小数点以下の座標補正を行います。
		if (  (hit_vec & (OBD_HIT_VEC_FRONT | OBD_HIT_VEC_BACK))
			&&(sSpd) ) {

			BOOL bHit  = (BOOL)((hit_vec & OBD_HIT_VEC_BACK) == OBD_HIT_VEC_BACK);		// 接触面   back = TRUE
			BOOL bFlip = (BOOL)((pWork->disp_flag & OBD_DISP_HFLIP) == OBD_DISP_HFLIP);	// 左右反転 flip = TRUE
			BOOL bSpd  = (BOOL)(sSpd > 0);												// 移動速度 plus = TRUE
			if (pWork->dir_fall == 0xc000) {
				bSpd  = (BOOL)(sSpd < 0);												// 移動速度 plus = TRUE(特定重力の時は符号が逆)
			}
			
			if (bHit ^ bFlip ^ bSpd) {
				fx32 *pPos;
				if (tData.vec & OBD_COL_Y) {
					pPos = &pWork->pos.y;
				} else {
					pPos = &pWork->pos.x;
				}
				if (sSpd > 0) {
					if ((*pPos & FX32_DEC_MASK) > 0x00000800) {
						*pPos &= ~FX32_DEC_MASK;
						*pPos |= 0x00000800;
					}
				} else {
					if ((*pPos & FX32_DEC_MASK) < 0x00000800) {
						*pPos &= ~FX32_DEC_MASK;
						*pPos |= 0x00000800;
					}
				}
			}
		}
    }
}
// ================================================================
// objDiffCollisionSimpleOverCheck
/*!
  簡易頭チェック 
 
  @param pWork [in] オブジェクトワークポインタ
 
  @return   0 空、 非0 頭は壁の中
 
 */
// ================================================================
static s8 objDiffCollisionSimpleOverCheck( OBS_OBJECT_WORK * pWork )
{
    OBS_COL_CHK_DATA tData = {0};

    u16 usDir; // ワーク角度
    s8 cDif1,cDif2; // 各点の地形までの距離
    s8 cDelta;      // 適応差分値
    s16 sRecX1 = 0, sRecX2 = 0;   // 左右レクト
    s16 sRecY1 = 0, sRecY2 = 0;   // 左右レクト

    // チェックフラグを設定
    tData.flag = objDiffSufSet(pWork);
    
    if ( pWork->move_flag & OBD_MOVE_JUMP )
        usDir = 0;
    else
        usDir = (u16)( ((pWork->dir.z + 0x2000) & 0xc000) >> 14 );

    // 重力方向チェック
    if ( pWork->dir_fall ){
        usDir += (u16)( ((pWork->dir_fall + 0x2000) & 0xc000) >> 14 );
        usDir &= 0x3;
    }

        // 角度に合わせてチェック方向を設定する
#if 1
	// 20100129 Ishizaki 横方向が重力方向の時に、横と頭上が同時に触れるとめり込む事があった対応
    switch ( usDir ) {
    default:
    case 0:
        // 下方向が床の時
        sRecX1 = (s16)(pWork->field_rect[OBD_RIGHT] - 2);
        sRecX2 = (s16)(pWork->field_rect[OBD_LEFT] + 2);
        sRecY1 = (s16)(pWork->field_rect[OBD_TOP] );
        sRecY2 = (s16)(pWork->field_rect[OBD_TOP] );
        tData.vec = OBD_COL_UP;
        break;
    case 1:
        // 左方向が床の時
        sRecY1 = (s16)(pWork->field_rect[OBD_LEFT] + 2);
        sRecY2 = (s16)(pWork->field_rect[OBD_RIGHT] - 2);
        sRecX1 = (s16)(-pWork->field_rect[OBD_TOP] );
        sRecX2 = (s16)(-pWork->field_rect[OBD_TOP] );
        tData.vec = OBD_COL_RIGHT;
        break;
    case 2:
        // 上方向が床の時
        sRecX1 = (s16)(pWork->field_rect[OBD_RIGHT] - 2);
        sRecX2 = (s16)(pWork->field_rect[OBD_LEFT] + 2);
        sRecY1 = (s16)(-pWork->field_rect[OBD_TOP] );
        sRecY2 = (s16)(-pWork->field_rect[OBD_TOP] );
        tData.vec = OBD_COL_DOWN;
        break;
    case 3:
        // 右方向が床の時
        sRecY1 = (s16)(pWork->field_rect[OBD_LEFT] + 2);
        sRecY2 = (s16)(pWork->field_rect[OBD_RIGHT] - 2);
        sRecX1 = (s16)(pWork->field_rect[OBD_TOP] );
        sRecX2 = (s16)(pWork->field_rect[OBD_TOP] );
        tData.vec = OBD_COL_LEFT;
        break;
    }

#else
    switch ( usDir ) {
    default:
    case 0:
        // 下方向が床の時
        sRecX1 = (s16)(pWork->field_rect[OBD_RIGHT] - 2);
        sRecX2 = (s16)(pWork->field_rect[OBD_LEFT] + 2);
        sRecY1 = (s16)(pWork->field_rect[OBD_TOP] );
        sRecY2 = (s16)(pWork->field_rect[OBD_TOP] );
        tData.vec = OBD_COL_UP;
        break;
    case 1:
        // 左方向が床の時
        sRecY1 = (s16)(pWork->field_rect[OBD_LEFT] + 2);
        sRecY2 = (s16)(pWork->field_rect[OBD_RIGHT] - 2);
        sRecX1 = (s16)(pWork->field_rect[OBD_RIGHT] );
        sRecX2 = (s16)(pWork->field_rect[OBD_RIGHT] );
        tData.vec = OBD_COL_RIGHT;
        break;
    case 2:
        // 上方向が床の時
        sRecX1 = (s16)(pWork->field_rect[OBD_RIGHT] - 2);
        sRecX2 = (s16)(pWork->field_rect[OBD_LEFT] + 2);
        sRecY1 = (s16)(pWork->field_rect[OBD_BOTTOM] );
        sRecY2 = (s16)(pWork->field_rect[OBD_BOTTOM] );
        tData.vec = OBD_COL_DOWN;
        break;
    case 3:
        // 右方向が床の時
        sRecY1 = (s16)(pWork->field_rect[OBD_LEFT] + 2);
        sRecY2 = (s16)(pWork->field_rect[OBD_RIGHT] - 2);
        sRecX1 = (s16)(pWork->field_rect[OBD_LEFT] );
        sRecX2 = (s16)(pWork->field_rect[OBD_LEFT] );
        tData.vec = OBD_COL_LEFT;
        break;
    }
#endif
    
    // 上方向の2点の小さい方の差分値を取得
    tData.pos_x = (pWork->pos.x >> FX32_SHIFT) + sRecX1;
    tData.pos_y = (pWork->pos.y >> FX32_SHIFT) + sRecY1;
    cDif1 = (s8)ObjCollisionUnion( pWork, &tData );
    tData.pos_x = (pWork->pos.x >> FX32_SHIFT) + sRecX2;
    tData.pos_y = (pWork->pos.y >> FX32_SHIFT) + sRecY2;
    cDif2 = (s8)ObjCollisionUnion( pWork, &tData );

    // 小さい方の差分値を設定
    if ( cDif1 < cDif2) cDelta = cDif1;
    else                cDelta = cDif2;
    
    // 埋まっている時
    if (cDelta <= 0) {
        return cDelta;
    }
    return 0;

}
// ================================================================
// objDiffCollisionDirHeightCheck
/*!
  オブジェクトの壁チェック 
 
  @param pWork [io] オブジェクトワークポインタ
 
  @note
    角度有り地形チェックの上下チェック
 */
// ================================================================
static void objDiffCollisionDirHeightCheck( OBS_OBJECT_WORK * pWork )
{
    OBS_COL_CHK_DATA tData = {0};
    OBS_OBJECT_WORK* pRide = pWork->ride_obj;

    s32 lPosX;		// ワーク座標X
    s32 lPosY;		// ワーク座標Y
    s32 sMoveX;		// 移動方向チェック用X
    s32 sMoveY;		// 移動方向チェック用Y
    
    u32 ulAttr = 0;

    u16  usDir;		// ワーク角度
    u16  usDir1,usDir2,usDir3; // 保持角度

    s8 cDelta;		// 適応差分値BottomChk
    s8 cDeltaTop;	// 適応差分値TopChk
    s8 cDif1,cDif2; // 各点の地形までの距離
    
    s8 cSpd;        // 坂道吸着チェック速度
    s16 sRecX1 = 0, sRecX2 = 0;   // 左右レクト
    s16 sRecY1 = 0, sRecY2 = 0;   // 左右レクト
    s16 sRecX3 = 0;       // すり抜けチェック左右レクト
    s16 sRecY3 = 0;       // すり抜けチェック左右レクト

	BOOL hit_once = FALSE;	// 上下同時ヒットチェック
    // チェックフラグセット
    tData.flag = objDiffSufSet(pWork);
    
    lPosX = pWork->pos.x >> FX32_SHIFT;
    lPosY = pWork->pos.y >> FX32_SHIFT;

    usDir1 = usDir2 = (u16)pWork->dir.z;

    // ▼ TIPS!
    // バグの具合によっては角度の計算に補正を入れる

    // チェック方向を設定

//チェック方向の設定＠スペステテスト
#if 1
// ●元の状態
	if ( pWork->move_flag & OBD_MOVE_JUMP )
		usDir = 0;
	else
		usDir = (u16)( ((pWork->dir.z + 0x2000) & 0xc000) >> 14 );

    // 重力方向チェック
    if ( pWork->dir_fall ){
        usDir += (u16)( ((pWork->dir_fall + 0x2000) & 0xc000) >> 14 );
        usDir &= 0x3;
    }
#elif 0
// ●ジャンプ中は本体角度見ない（元の状態互換）
	if ( pWork->move_flag & OBD_MOVE_JUMP )
        usDir = (u16)( ((pWork->dir_fall + 0x2000) & 0xc000) >> 14 );
	} else {
		usDir = (u16)( ((pWork->dir.z + 0x2000) & 0xc000) >> 14 );
	}
#else
// ●ジャンプ中も本体角度＋重力角度（本編ではバグが起こる状態）
		usDir = (u16)( ((pWork->dir.z + 0x2000) & 0xc000) >> 14 );
        usDir += (u16)( ((pWork->dir_fall + 0x2000) & 0xc000) >> 14 );
        usDir &= 0x3;
#endif

    
    // 角度に合わせてチェック方向を設定する
    switch ( usDir ) {
    default:
    case 0:
        // 下方向が進行方向
        sRecX1 = (s16)(pWork->field_rect[OBD_RIGHT] + pWork->field_ajst_h_db_r);
        sRecY1 = (s16)(pWork->field_rect[OBD_BOTTOM]);
        sRecX2 = (s16)(pWork->field_rect[OBD_LEFT] - pWork->field_ajst_h_db_l);
        sRecY2 = (s16)(pWork->field_rect[OBD_BOTTOM]);
        sRecX3 = 0;
        sRecY3 = (s16)-g_obj.col_through_dot;
        tData.vec = OBD_COL_DOWN;
        sMoveX = pWork->move.x;
        sMoveY = pWork->move.y;
        break;
    case 1:
        // 左方向が進行方向
        sRecX1 = (s16)-(pWork->field_rect[OBD_BOTTOM]);
        sRecY1 = (s16)(pWork->field_rect[OBD_LEFT] - pWork->field_ajst_h_dl_l);
        sRecX2 = (s16)-(pWork->field_rect[OBD_BOTTOM]);
        sRecY2 = (s16)(pWork->field_rect[OBD_RIGHT] + pWork->field_ajst_h_dl_r);
        sRecX3 = (s16)(pWork->field_rect[OBD_BOTTOM] - MTM_MATH_ABS(pWork->field_rect[OBD_RIGHT]));
        sRecY3 = 0;
        tData.vec = OBD_COL_LEFT;
        sMoveY = (s32)-pWork->move.x;
        sMoveX = (s32)-pWork->move.y;
        break;
    case 2:
        // 上方向が進行方向
        sRecX1 = (s16)(pWork->field_rect[OBD_RIGHT] + pWork->field_ajst_h_dt_r);
        sRecY1 = (s16)-(pWork->field_rect[OBD_BOTTOM]);
        sRecX2 = (s16)(pWork->field_rect[OBD_LEFT] - pWork->field_ajst_h_dt_l);
        sRecY2 = (s16)-(pWork->field_rect[OBD_BOTTOM]);
        sRecX3 = 0;
        sRecY3 = g_obj.col_through_dot;
        tData.vec = OBD_COL_UP;
        sMoveX = (s32)-pWork->move.x;
        sMoveY = (s32)-pWork->move.y;
        break;
    case 3:
        // 右方向が進行方向
        sRecX1 = (s16)(pWork->field_rect[OBD_BOTTOM]);		//			左上が丸まっているアールで左下にゆっくり移動する際
        sRecY1 = (s16)(pWork->field_rect[OBD_LEFT] - pWork->field_ajst_h_dr_l);	// - 1);	押し戻される状況を改善するため補正値(±1)を(±2)に修正。
        sRecX2 = (s16)(pWork->field_rect[OBD_BOTTOM]);		//			要経過観察@kuramoto(09/08/27)
        sRecY2 = (s16)(pWork->field_rect[OBD_RIGHT] + pWork->field_ajst_h_dr_r);	// + 1);	
        sRecX3 = (s16)-(pWork->field_rect[OBD_BOTTOM] - MTM_MATH_ABS(pWork->field_rect[OBD_LEFT]));
        sRecY3 = 0;
        tData.vec = OBD_COL_RIGHT;
        sMoveY = pWork->move.x;
        sMoveX = pWork->move.y;
        break;
    }
    
    if ( !(pWork->move_flag & OBD_MOVE_THROUGH) ){
        // 前フレーム、下地形HITがない時（つまり空中）、すり抜けありなしチェックを行う
        if ( !(pWork->move_flag & OBD_MOVE_UNDERPREV) ){
            u16 usFlag = tData.flag;

            // AIRFOOT設定チェック
            tData.flag &= ~OBD_COL_THROUGH;
            tData.pos_x = lPosX + (sRecX1+sRecX3);
            tData.pos_y = lPosY + (sRecY1+sRecY3);
            cDif1 = (s8)ObjCollisionUnion( pWork, &tData );
            tData.pos_x = lPosX + (sRecX2+sRecX3);
            tData.pos_y = lPosY + (sRecY2+sRecY3);
            cDif2 = (s8)ObjCollisionUnion( pWork, &tData );
            tData.flag = usFlag;

            // AIRFOOTチェックでオブジェクト地形に乗ってしまった時のためのバックアップセット
            pWork->ride_obj = pRide;

            // 小さい方の差分値を設定
            if ( cDif1 < cDif2) cDelta = cDif1;
            else                cDelta = cDif2;

            if ( cDelta >= 0 )
                // すり抜けない
                pWork->move_flag |= OBD_MOVE_AIRFOOT;
            else
                // すり抜ける
                pWork->move_flag &= ~OBD_MOVE_AIRFOOT;

        }else{
            // 縦のチェックか、前フレーム地面に立っていれば
            if ( tData.vec & OBD_COL_Y || pWork->col_flag_prev & OBD_COLAT_GRAIND )
                // すり抜けない
                pWork->move_flag |= OBD_MOVE_AIRFOOT;
            else
                // すり抜ける
                pWork->move_flag &= ~OBD_MOVE_AIRFOOT;
        }
    }
    if ( !( pWork->move_flag & OBD_MOVE_AIRFOOT ) )
        // すり抜けフラグ設定
        tData.flag |= OBD_COL_THROUGH;

    // 矩形下方2点の下方向への地形まで差分値をチェック
    tData.attr = &ulAttr;
    tData.dir = &usDir1;
    tData.pos_x = lPosX + sRecX1;
    tData.pos_y = lPosY + sRecY1;
    cDif1 = (s8)ObjCollisionUnion( pWork, &tData );
    objDiffAttrSet(pWork, ulAttr);
    tData.dir = &usDir2;
    tData.pos_x = lPosX + sRecX2;
    tData.pos_y = lPosY + sRecY2;
    cDif2 = (s8)ObjCollisionUnion( pWork, &tData );
    objDiffAttrSet(pWork, ulAttr);
    

    // 足元を厳密にチェックする
    tData.dir = &usDir3;
    tData.attr = NULL;
    tData.pos_y = lPosY + sRecY1;
    if( pWork->move_flag & OBD_MOVE_UNDERALL && !(pWork->move_flag & OBD_MOVE_NOCOLOBJ) ){
        u8 i;
        s32 lPosTemp;
        s16 sRecMax;
        // チェック設定
        if ( sRecX1 > sRecX2){
            sRecMax = (s16)((sRecX1 - sRecX2) - 1);
            lPosTemp = lPosX + sRecX2;
        }else{
            sRecMax = (s16)((sRecX2 - sRecX1) - 1);
            lPosTemp = lPosX + sRecX1;
        }
        // チェック
        for ( i= 1; i < sRecMax; ++i ){
            tData.pos_x = lPosTemp + i;
            cDelta = (s8)ObjCollisionObjectCheck( pWork, &tData );
            if ( cDelta < cDif1){
                cDif1 = cDelta;
                usDir1 = usDir3;
            }
        }
    }
    
    // 真中の地形フラグを取得
    {
        tData.dir = NULL;
        tData.attr = &ulAttr;
#if 0	// 元の状態
        tData.pos_y = lPosY + sRecY2;
        
        if ( sRecX1 < sRecX2)
            tData.pos_x = (lPosX + sRecX1) + ((MTM_MATH_ABS(sRecX1)+MTM_MATH_ABS(sRecX2)) >> 1);
        else
            tData.pos_x = (lPosX + sRecX2) + ((MTM_MATH_ABS(sRecX1)+MTM_MATH_ABS(sRecX2)) >> 1);
#else	// 角度に合わせて中心Ｘ/Ｙ計算を変える
		if (usDir & 0x01) {
			// 横方向移動
	        tData.pos_x = lPosX + sRecX1;
	        
	        if ( sRecY1 < sRecY2)
	            tData.pos_y = (lPosY + sRecY1) + ((MTM_MATH_ABS(sRecY1)+MTM_MATH_ABS(sRecY2)) >> 1);
	        else
	            tData.pos_y = (lPosY + sRecY2) + ((MTM_MATH_ABS(sRecY1)+MTM_MATH_ABS(sRecY2)) >> 1);
		} else {
			// 縦方向移動
	        tData.pos_y = lPosY + sRecY2;
	        
	        if ( sRecX1 < sRecX2)
	            tData.pos_x = (lPosX + sRecX1) + ((MTM_MATH_ABS(sRecX1)+MTM_MATH_ABS(sRecX2)) >> 1);
	        else
	            tData.pos_x = (lPosX + sRecX2) + ((MTM_MATH_ABS(sRecX1)+MTM_MATH_ABS(sRecX2)) >> 1);
		}
#endif
        // 取得
        ObjCollisionUnion( pWork, &tData );
        objDiffAttrSet(pWork, ulAttr);
        
        // すり抜ける設定の時グラインドを無視
        //if ( !(pWork->move_flag & OBD_MOVE_AIRFOOT) ) {
        //    pWork->col_flag &= ~OBD_COLAT_GRAIND;
        //}
    }
    // すり抜けグラインドチェック
    if ( pWork->col_flag & OBD_COLAT_GRAIND ){
        // 移動状態チェック
        if ( pWork->move_flag & OBD_MOVE_JUMP && (sMoveY) > 0 ){
            // 崖による角度設定は無視する
            if (!(pWork->col_flag & OBD_COLAT_CLIFF )){
                // 角度チェック
                if ( (u16)(usDir1 + pWork->dir_fall) >= 0x4000 && (u16)(usDir1 + pWork->dir_fall) <= 0xc000 ||
                     (u16)(usDir2 + pWork->dir_fall) >= 0x4000 && (u16)(usDir2 + pWork->dir_fall) <= 0xc000 ){
                    cDif1 = 24;
                    cDif2 = 24;
                }
            }
        }
        
    }
    
    // 小さい方の差分値を設定
    if ( cDif1 < cDif2) cDelta = cDif1;
    else                cDelta = cDif2;
    
    // 差分値で補正を行う
    if ( cDelta ) {
        // 地面埋まり時
        if (cDelta < 0) {
            // 下は壁
#if OBD_USE_NOLANDING
			if (!( pWork->move_flag & OBD_MOVE_JUMP && (sMoveY) < 0x0000 ) &&
					!(pWork->sys_flag & ((OBD_SYSF_NOLANDING_UNDER | OBD_SYSF_NOLANDING_UNDERPREV) << usDir))) {
                pWork->move_flag |= OBD_MOVE_UNDER;
			}
#else
			if (!( pWork->move_flag & OBD_MOVE_JUMP && (sMoveY) < 0x0000 ) )
                pWork->move_flag |= OBD_MOVE_UNDER;
#endif
            // 速度クリア
            //if (!( pWork->move_flag & OBD_MOVE_NOSPD ))
            //    pWork->spd.y = 0;
            // 埋まり過ぎチェック
//■■■■Zone4ピストン押し出し不具合のため対処■■■■
#if 0
//			if (cDelta >= -OBD_MOVE_SPDY_MAX && pWork->move_flag & OBD_MOVE_UNDER){
			if (cDelta >= -OBD_MOVE_SPDY_MAX*2) {	// プレイヤー落下速度限界＋OBJコリジョンの上昇速度を加味しｘ２で判定
				if (cDelta < -0x10) {
					hit_once = TRUE;				// めり込みすぎて天井同時判定してしまうケースの対処用
				}
#else
			if (   (  (!(pWork->move_flag & OBD_MOVE_TOP_DIFF))
					&&(cDelta >= -OBD_MOVE_SPDY_MAX)
					&&(pWork->move_flag & OBD_MOVE_UNDER) )
				|| (  (pWork->move_flag & OBD_MOVE_TOP_DIFF)
					&&(cDelta >= -OBD_MOVE_SPDY_MAX*2)) ) {
				if (  (cDelta < -0x10)
					&&(pWork->move_flag & OBD_MOVE_TOP_DIFF) ) {
					hit_once = TRUE;				// めり込みすぎて天井同時判定してしまうケースの対処用
				}
#endif
//■■■■Zone4ピストン押し出し不具合のため対処■■■■
#if (GMD_COL_VIB_COLLECT_CHK == 1)
// ■■■■ アイテムBOXが地面に1dot浮いたり密着したりする要因なのでカットしてみる
// ■■■■ SonicAdvance2 の頃に追加した対処だが現在状況でも必要な対処か不明。影響ないようなら要経過観察後正式にカットの予定@kuramoto(09/08/26)
                // 前フレーム1dot浮いていて、今回1dot埋まっている場合は振動対策を行う
                if ( cDelta == -1 && pWork->move_flag & OBD_MOVE_COL_VIB ){
                    // 振動チェック

                    // 移動後の差分値をチェック
                    cDif1 = objDiffColVibCheck( lPosX, lPosY, sRecX1, sRecX2, sRecY1, sRecY2, tData.vec, tData.flag , cDif1, cDif2);

                    // 移動後が前と同じなら振動とみなし、移動を行わない
                    if ( cDif1 != 1){
                        // 座標更新
                        objDiffColDirMove( &lPosX, &lPosY, cDelta, tData.vec);
                        pWork->move_flag &= ~OBD_MOVE_COL_VIB;
                    }

                }else{
                    // 座標更新
                    objDiffColDirMove( &lPosX, &lPosY, cDelta, tData.vec);
                    pWork->move_flag &= ~OBD_MOVE_COL_VIB;
                }
#else	// (GMD_COL_VIB_COLLECT_CHK == 1)
				objDiffColDirMove( &lPosX, &lPosY, cDelta, tData.vec);
				pWork->move_flag &= ~OBD_MOVE_COL_VIB;
#endif	// (GMD_COL_VIB_COLLECT_CHK == 1)
            }
            
        } else {
            // 1dot浮きフラグを設定
            if ( cDelta == 1)
                pWork->move_flag |= OBD_MOVE_COL_VIB;

            // 浮いている時は接地チェックを行わない
            if (!( pWork->move_flag & OBD_MOVE_JUMP )){
                if ( usDir & 0x01 )
                    // 横方向が地面の時
                    // cSpd = (s8)( (MTM_MATH_ABS(pWork->sSpdY) >> FX32_SHIFT) + 3 );
                    // cSpd = (s8)( (MTM_MATH_ABS(pWork->sSpdM) >> FX32_SHIFT) + 3 );
                    cSpd = (s8)( (MTM_MATH_ABS(sMoveY) >> FX32_SHIFT) + 3 );
                    
                else
                    // 縦方向が地面の時
                    // cSpd = (s8)( (MTM_MATH_ABS(pWork->sSpdM) >> FX32_SHIFT) + 3 );
                    cSpd = (s8)( (MTM_MATH_ABS(sMoveX) >> FX32_SHIFT) + 3 );
                    
                if ( cSpd > OBD_MOVE_SPDX_MAX )
                    cSpd = OBD_MOVE_SPDX_MAX;
                // cSpd = MAM_MIN(speed, LN3D_PL_DOWN_MAX);

                // 坂道吸着
#if OBD_USE_NOLANDING
                if (cDelta <= cSpd &&
					!(pWork->sys_flag & ((OBD_SYSF_NOLANDING_UNDER | OBD_SYSF_NOLANDING_UNDERPREV) << usDir))) {
#else
                if (cDelta <= cSpd) {
#endif
                    // 下は壁
                    pWork->move_flag |= OBD_MOVE_UNDER;

                    // 座標更新
                    objDiffColDirMove( &lPosX, &lPosY, cDelta, tData.vec);
                    
                    // オブジェクト地形チェックを行う、角度なし移動オブジェクトは坂道吸着時に再度Rideオブジェクトなどをチェックする
                    if ( !( pWork->move_flag & OBD_MOVE_NOCOLOBJ ) && !(pWork->move_flag & OBD_MOVE_DIR) && !pWork->touch_obj ){
                        // Ride,Touch再チェック
                        tData.attr = NULL;
                        tData.dir = NULL;
                        tData.pos_x = lPosX + sRecX1;
                        tData.pos_y = lPosY + sRecY1;
                        ObjCollisionObjectCheck(pWork, &tData );
                        tData.pos_x = lPosX + sRecX2;
                        tData.pos_y = lPosY + sRecY2;
                        ObjCollisionObjectCheck(pWork, &tData );
                    }

                }else{
                    // 浮いた
                    pWork->move_flag &= ~OBD_MOVE_UNDER;
                }
            }
        }
    }else{
        // ぴったり
#if OBD_USE_NOLANDING
		if (!( pWork->move_flag & OBD_MOVE_JUMP && pWork->spd.y < 0 ) &&
				!(pWork->sys_flag & ((OBD_SYSF_NOLANDING_UNDER | OBD_SYSF_NOLANDING_UNDERPREV) << usDir))){
#else
        if (!( pWork->move_flag & OBD_MOVE_JUMP && pWork->spd.y < 0 ) ){
#endif
            pWork->move_flag |= OBD_MOVE_UNDER;
        }
    }
    // 地面にくっついてる時は角度を反映する
    if ( pWork->move_flag & OBD_MOVE_UNDER){
    
        // ▼ TIPS!
        // バグの具合によっては更新する角度が奇数の時角度を更新しない
        // ↑Luna3では奇数値は崖のフラグだった、Naluでは更新してもOK
        // if ( /*cDif1 <= 1 || cDif2 <= 1*/1){
        if ( !(pWork->move_flag  & OBD_MOVE_FLY) ){
            if ( !(pWork->col_flag & OBD_COLAT_CLIFF) && pWork->move_flag & OBD_MOVE_DIR ){
                if      ( cDif1 < cDif2 ){ usDir1 = usDir1;}
                else if ( cDif1 > cDif2 ){ usDir1 = usDir2;}
                else{
                    // 埋まり具合が同じ時は現在の角度に近い値のものを使用
                    if ( MTM_MATH_ABS((u16)(pWork->dir.z + pWork->dir_fall) - usDir1) > MTM_MATH_ABS((u16)(pWork->dir.z + pWork->dir_fall) - usDir2)){
                        usDir1 = usDir2;
                    }

                }
                // 重力方向チェック			20090817 重力方向が地形角度に加算されてしまうので下3行コメントアウト
                //if ( pWork->dir_fall ){
                //    usDir1 += pWork->dir_fall;
                //}
#if 1 // 重力方向変更を加味@kuramoto(09/08/28)
                if ( pWork->move_flag & OBD_MOVE_DIR_SLOW )
                    pWork->dir.z = ObjRoopMove16( pWork->dir.z, (u16)(usDir1 - pWork->dir_fall), 0x0100);
                else
                    pWork->dir.z = (u16)(usDir1 - pWork->dir_fall);
#else // 重力方向変更を無視(元の状態)
                if ( pWork->move_flag & OBD_MOVE_DIR_SLOW )
                    pWork->dir.z = ObjRoopMove16( pWork->dir.z, usDir1, 0x0100);
                else
                    pWork->dir.z = usDir1;
#endif// 重力方向変更対応
#if 0
				// ■■■■ 45度問題対策テスト ■■■■
				{
					u16 dir_z = pWork->dir.z;
					if (  (pWork->dirz_buff[0] != dir_z)
						&&(pWork->dirz_buff[1] == dir_z)
						&&(pWork->dirz_buff[0] == pWork->dirz_buff[2]) ) {
						pWork->dir.z = (pWork->dirz_buff[0] + dir_z) /2;
					}
					pWork->dirz_buff[2] = pWork->dirz_buff[1];
					pWork->dirz_buff[1] = pWork->dirz_buff[0];
					pWork->dirz_buff[0] = dir_z;
#elif 0
					u16 dir_z = pWork->dir.z & ~0xc000;
					if (  (dir_z >= 0x1c00)
						&&(dir_z <= 0x2400) ) {
						pWork->dir.z = (pWork->dir.z & 0xc000) + 0x2000;
					}
				}
				// ■■■■ 45度問題対策テスト ■■■■
#endif
            }
			// ↓↓↓↓HOGスペステ対応↓↓↓↓
			else if (  (GSM_MAIN_STAGE_IS_SPSTAGE_NOT_RETRY())
					 &&(!(pWork->move_flag & OBD_MOVE_UNDERPREV)) ) {
				// 空中から地面に接地した時の角度情報取得
//				pWork->dir.z = -usDir1;	// 地面角度取得するとObjCol3x3の影響でどこの角度を取るかまちまちで正常動作しない
				pWork->dir.z = (u16)(0-(((g_gm_main_system.pseudofall_dir + 0x2000) & 0x3fff) - 0x2000));	// 重力を0x2000～0xe000の間にして地面角度を取得
				
			}
			// ↑↑↑↑HOGスペステ対応↑↑↑↑
        }
        // 速度クリア
        if (  (!(pWork->move_flag & OBD_MOVE_NOSPD))
        	&&(!(pWork->move_flag & OBD_MOVE_NOCLR_SPDXY)) ) {

            // 重力変換のためDir設定しなおす
            if ( pWork->move_flag & OBD_MOVE_JUMP )
                usDir = 0;
            else
                usDir = (u16)( ((pWork->dir.z + 0x2000) & 0xc000) >> 14 );
            
            switch ( usDir ) {
            default:
            case 0:
                // 下方向が進行方向
#if 1
				// HOG速度分解対応(20091017_Ishizaki)
                if ( pWork->spd.y > 0){
                    // 坂道加減速対応
                    if ( pWork->move_flag & OBD_MOVE_SLOPE ){
                        //if ( pWork->ucDir >= 0)
                        // pWork->sSpdM -= pWork->sSpdY;
                        pWork->spd.x += ( (( pWork->spd.y ) *
							mtMathSin( (u16)(pWork->dir.z - g_gm_main_system.pseudofall_dir + pWork->dir_fall) )) >> FX32_SHIFT );
                        //else
                        //    pWork->sSpdM += pWork->sSpdY;
                    }
                    pWork->spd.y = 0;
                }
#else
                if ( pWork->spd.y > 0){
                    // 坂道加減速対応
                    if ( pWork->move_flag & OBD_MOVE_SLOPE ){
                        //if ( pWork->ucDir >= 0)
                        // pWork->sSpdM -= pWork->sSpdY;
                        pWork->spd.x += ( (( pWork->spd.y ) * mtMathSin( (u16)(pWork->dir.z ) )) >> FX32_SHIFT );
                        //else
                        //    pWork->sSpdM += pWork->sSpdY;
                    }
                    pWork->spd.y = 0;
                }
#endif
                break;
            case 1:
                // 左方向が進行方向
                if ( pWork->spd.x < 0)
                    pWork->spd.x = 0;
                break;
            case 2:
                // 上方向が進行方向
                if ( pWork->spd.y <0)
                    pWork->spd.y = 0;
                break;
            case 3:
                // 右方向が進行方向
                if ( pWork->spd.x > 0)
                    pWork->spd.x = 0;
                break;
            }
        }

    }
    else{
        // 着地していないので乗っていない
        pWork->ride_obj = pRide;
    }
    
    // 天井チェック
//■■■■Zone4ピストン押し出し不具合のため対処■■■■
#if 0
//	if ( (sMoveY) < 0x0100 ){				// ソニック下降中に下降速度以上に速いOBJコリジョンに上から押される場合に天井判定を取る必要あるためコメントアウト
	{
#else
	if (  (pWork->move_flag & OBD_MOVE_TOP_DIFF)	// ソニック(OBD_MOVE_TOP_DIFF = ON)は下降中でも天井判定
		||((sMoveY) < 0x0100 ) ) {
#endif
//■■■■Zone4ピストン押し出し不具合のため対処■■■■
        tData.flag |= OBD_COL_THROUGH;
        switch ( tData.vec ) {
        default:
        case OBD_COL_DOWN:
            // 下方向が床の時
            sRecY1 = (s16)(pWork->field_rect[OBD_TOP] + 2);
            sRecY2 = (s16)(pWork->field_rect[OBD_TOP] + 2);
            tData.vec = OBD_COL_UP;
            break;
        case OBD_COL_LEFT:
            // 左方向が床の時
            sRecX1 = (s16)(pWork->field_rect[OBD_RIGHT] - 2);
            sRecX2 = (s16)(pWork->field_rect[OBD_RIGHT] - 2);
            tData.vec = OBD_COL_RIGHT;
            break;
        case OBD_COL_UP:
            // 上方向が床の時
			sRecY1 = (s16)(pWork->field_rect[OBD_BOTTOM] - 2);
			sRecY2 = (s16)(pWork->field_rect[OBD_BOTTOM] - 2);
            tData.vec = OBD_COL_DOWN;
            break;
        case OBD_COL_RIGHT:
            // 右方向が床の時
            sRecX1 = (s16)(pWork->field_rect[OBD_LEFT] + 2);
            sRecX2 = (s16)(pWork->field_rect[OBD_LEFT] + 2);
            tData.vec = OBD_COL_LEFT;
            break;
        }

        // 上方向の2点の小さい方の差分値を取得
        tData.attr = NULL;
        tData.dir = NULL;
        tData.pos_x = lPosX + sRecX1;
        tData.pos_y = lPosY + sRecY1;
        cDif1 = (s8)ObjCollisionUnion( pWork, &tData );
        tData.pos_x = lPosX + sRecX2;
        tData.pos_y = lPosY + sRecY2;
        cDif2 = (s8)ObjCollisionUnion( pWork, &tData );

        // 小さい方の差分値を設定
        if ( cDif1 < cDif2) cDeltaTop = cDif1;
        else                cDeltaTop = cDif2;

        // 埋まっている時
        if (cDeltaTop <= 0) {
            // 上は壁
            pWork->move_flag |= OBD_MOVE_OVER;
            // 埋まり過ぎチェック
//■■■■Zone4ピストン押し出し不具合のため対処■■■■
#if 0
			if (cDeltaTop >= -OBD_MOVE_SPDY_MAX ) {
				objDiffColDirMove( &lPosX, &lPosY, cDeltaTop, tData.vec);
			}
#else
			if (  (   (!(pWork->move_flag & OBD_MOVE_TOP_DIFF))	// 既存どおりOBD_MOVE_SPDY_MAXで判定
					&&(cDeltaTop >= -OBD_MOVE_SPDY_MAX ))
				||(   (pWork->move_flag & OBD_MOVE_TOP_DIFF)	// ソニック(OBD_MOVE_TOP_DIFF = ON)はプレイヤー落下速度限界＋OBJコリジョンの下降速度を加味しｘ２で判定
					&&(cDeltaTop >= -OBD_MOVE_SPDY_MAX*2)) ) {
				if (hit_once && (pWork->move_flag & OBD_MOVE_UNDER)) {	// めり込みすぎて天井同時判定してしまうケースの対処用
					if (cDelta > cDeltaTop) {
						objDiffColDirMove( &lPosX, &lPosY, (s8)(cDeltaTop - cDelta), tData.vec); // 天井めり込み量の方が大きければ、床分を戻し天井めり込み量を補正
					}
				} else {
					// 座標更新
					objDiffColDirMove( &lPosX, &lPosY, cDeltaTop, tData.vec);
				}
#endif
//■■■■Zone4ピストン押し出し不具合のため対処■■■■
#if 0	// 既存処理
                // 速度クリア
                if ( (!( pWork->move_flag & OBD_MOVE_NOSPD )) && (sMoveY < 0)){
                    if ( tData.vec & OBD_COL_Y )
                        pWork->spd.y = 0;
                    else
                        pWork->spd.x = 0;
                }
#else	// HOGスペステ用対応（画面が回転していて天井接触時、Ｘ速度が高い場合には角度補正影響で天井に張り付き続けるため速度を０にする）
				if (!g_gm_main_system.pseudofall_dir) {
	                // 速度クリア
	                if (  (  (!( pWork->move_flag & OBD_MOVE_NOSPD ))
						   &&(!(pWork->move_flag & OBD_MOVE_NOCLR_SPDXY)) )
	                	&&(sMoveY < 0) ) {
	                    if ( tData.vec & OBD_COL_Y )
	                        pWork->spd.y = 0;
	                    else
	                        pWork->spd.x = 0;
	                }
				} else {
					if ( (!( pWork->move_flag & OBD_MOVE_NOSPD ))) {
						pWork->spd_m = 0;
						if (!(pWork->move_flag & OBD_MOVE_NOCLR_SPDXY)) {
							pWork->spd.x = 0;
							pWork->spd.y = 0;
						}
					}
				}
#endif
            }
        }
    }
    // 座標を反映する
    pWork->pos.x -= ((pWork->pos.x >> FX32_SHIFT) - lPosX) << FX32_SHIFT;
    pWork->pos.y -= ((pWork->pos.y >> FX32_SHIFT) - lPosY) << FX32_SHIFT;

	// DS仕様と違い、壁ヒット時に fx32の小数点以下単位で
	// 表示位置のがたつきが生じるため小数点以下の座標補正を行います。
	if (pWork->move_flag & OBD_MOVE_UNDER) {
		fx32 *pPos;
		if (tData.vec & OBD_COL_Y) {
			pPos = &pWork->pos.y;
		} else {
			pPos = &pWork->pos.x;
		}
		if (sMoveY > 0) {
			if ((*pPos & FX32_DEC_MASK) > 0x00000800) {
				*pPos &= ~FX32_DEC_MASK;
				*pPos |= 0x00000800;
			}
		} else {
			if ((*pPos & FX32_DEC_MASK) < 0x00000800) {
				*pPos &= ~FX32_DEC_MASK;
				*pPos |= 0x00000800;
			}
		}
	}
}
// ================================================================
// objDiffColDirMove
/*!
    方向に合わせて座標値を更新する
 
  @param lPosX     [io] X座標
  @param lPosY     [io] Y座標
  @param cDelta    [in] 移動値
  @param ucColFlag [in] OBD_COL_DOWN 等、進行方向プラスorマイナス、XチェックorYチェックのフラグを設定 
 
 */
// ================================================================
inline void objDiffColDirMove( s32* lPosX, s32* lPosY, s8 cDelta, u16 usColFlag )
{
    switch ( usColFlag ) {
    default:
    case OBD_COL_DOWN: // 下チェック時
        *lPosY += cDelta;
        break;
    case OBD_COL_LEFT: // 左チェック時
        *lPosX -= cDelta;
        break;
    case OBD_COL_UP: // 上チェック時
        *lPosY -= cDelta;
        break;
    case OBD_COL_RIGHT: // 右チェック時
        *lPosX += cDelta;
        break;
    }
}
#if (GMD_COL_VIB_COLLECT_CHK == 1)
// ================================================================
// objDiffColVibCheck
/*!
    連続振動回避チェックのために仮に移動した値で差分値を求める
 
  @param lPosX  [in] X座標
  @param lPosY  [in] Y座標
  @param sRecX1 [in] 矩形値
  @param sRecY1 [in] 矩形値
  @param sRecX2 [in] 矩形値 
  @param sRecY2 [in] 矩形値
  @param usVec  [in] OBD_COL_DOWN 等、進行方向プラスorマイナス、XチェックorYチェックのフラグを設定 
  @param usFlag [in] チェック用フラグ #OBD_COL_B 、 #OBD_COL_THROUGH など
  @param cDif1  [in] 差分値１ 片足
  @param cDif2  [in] 差分値２

 
  @return   予定差分値
 
 */
// ================================================================
inline s8 objDiffColVibCheck(s32 lPosX, s32 lPosY, s16 sRecX1,s16 sRecX2,s16 sRecY1,s16 sRecY2,
                             u16 usVec, u16 usFlag, s8 cDif1, s8 cDif2)
{
    OBS_COL_CHK_DATA tData = {0};
    s8 cDelta;

    tData.vec = usVec;
    tData.flag = usFlag;
    
    // 小さい方の差分値を設定
    if ( cDif1 < cDif2) cDelta = cDif1;
    else                cDelta = cDif2;
    
    switch ( tData.vec ) {
    case OBD_COL_DOWN: // 下チェック時
        sRecY1 += cDelta;
        sRecY2 += cDelta;
        break;
    case OBD_COL_LEFT: // 左チェック時
        sRecX1 -= cDelta;
        sRecX2 -= cDelta;
        break;
    case OBD_COL_UP: // 上チェック時
        sRecY1 -= cDelta;
        sRecY2 -= cDelta;
        break;
    case OBD_COL_RIGHT: // 右チェック時
        sRecX1 += cDelta;
        sRecX2 += cDelta;
        break;

    }
    // 使用する方の差分値で移動先のチェックを行う
    tData.pos_x = lPosX + sRecX1;
    tData.pos_y = lPosY + sRecY1;
    cDif1 = (s8)objCollisionFast( &tData );
    tData.pos_x = lPosX + sRecX2;
    tData.pos_y = lPosY + sRecY2;
    cDif2 = (s8)objCollisionFast( &tData );
    
    if ( cDif1 < cDif2) 
        return cDif1;
    return cDif2;
}
#endif	// (GMD_COL_VIB_COLLECT_CHK == 1)

#if 0    
// ================================================================
// objDiffFastCollisionComp
/*!
  指定した2点の小さい方の差分値を返す 
 
  @param pDir      [out] 角度ポインタ NULLで設定を行わない 値は(0～2π を 0～256 に写像)
  @param lPosX1    [in] X座標
  @param lPosY1    [in] Y座標
  @param lPosX2    [in] X座標
  @param lPosY2    [in] Y座標
  @param ucSuf     [in] チェック用フラグ #OBD_COL_B 、 #OBD_COL_THROUGH など
  @param ucFlag    [in] OBD_COL_DOWN 等、進行方向プラスorマイナス、XチェックorYチェックのフラグを設定 
 
  @return   小さい差分値
 
 */
// ================================================================
inline s8 objDiffFastCollisionComp( s32 lPosX1, s32 lPosY1, s32 lPosX2, s32 lPosY2, u8 ucSuf, u8 ucFlag, u8* pDir )
{
    s8 cDif1,cDif2; // 各点の地形までの距離
    s8 cDalta;      // 適応差分値

    cDif1 = (s8)ObjCollisionUnion( pWork, lPosX1, lPosY1, ucSuf, ucFlag, pDir, NULL );
    cDif2 = (s8)ObjCollisionUnion( pWork, lPosX2, lPosY2, ucSuf, ucFlag, pDir, NULL );

    // 小さい方の差分値を設定
    if ( cDif1 < cDif2) cDalta = cDif1;
    else                cDalta = cDif2;

    // 小さい方の差分値を返す
    return cDalta;
}
#endif

/*
 * Revision 1.45  2005/09/27 09:33:16  use1146
 * 速度方向チェックミス修正
 *
 * Revision 1.44  2005/09/26 09:49:10  use1146
 * 崖グラインドすり抜け対応
 *
 * Revision 1.43  2005/09/16 07:42:40  use1146
 * 地形オブジェクトチェック関数追加
 *
 * Revision 1.42  2005/09/15 12:25:53  use1146
 * ↓天井のバグ修正
 *
 * Revision 1.41  2005/09/15 11:36:44  use1146
 * 天井の左右チェック方向反転
 *
 * Revision 1.40  2005/09/13 05:28:07  use1146
 * 不自然な挟まり死回避のため、後ろチェックを1ドット修正
 *
 * Revision 1.39  2005/09/06 14:00:52  use1173
 * プリコンパイルヘッダ対応
 *
 * Revision 1.38  2005/09/06 12:51:37  use1146
 * すり抜けグラインドが天井として前後チェックでHITし、角度が0になってしまうのを修正
 *
 * Revision 1.37  2005/09/01 13:53:54  use1146
 * 逆さグラインド対応
 *
 * Revision 1.36  2005/08/19 07:07:47  use1146
 * 壁登り時のすり抜けチェック修正
 *
 * Revision 1.35  2005/08/19 05:20:21  use1146
 * 坂道吸着チェック時の移動量チェック修正
 *
 * Revision 1.34  2005/08/17 13:23:15  use1146
 * MOVE_DIRの立っていないオブジェクトは坂道吸着時にライドオブジェクトを再チェックするように修正
 *
 * Revision 1.33  2005/08/17 02:44:54  use1159
 * ride関係の不具合の修正
 *
 * Revision 1.32  2005/08/12 13:02:27  use1146
 * TpoDown値修正
 *
 * Revision 1.31  2005/08/11 13:33:42  use1146
 * 中心が矩形外のオブジェクトのREVERSE時の挙動修正
 *
 * Revision 1.30  2005/07/26 09:23:34  use1146
 * FRONTチェックバグ修正
 *
 * Revision 1.29  2005/07/25 12:34:31  use1146
 * 前当たり時のNOSPD対応
 *
 * Revision 1.28  2005/07/22 08:42:54  use1146
 * 左右チェック前に頭ぶつけチェックを追加
 *
 * Revision 1.27  2005/07/11 10:39:20  use1146
 * 厳密足元チェック修正、頭当たり時速度クリアチェック修正
 *
 * Revision 1.26  2005/06/28 12:27:29  use1146
 * 坂道処理修正
 *
 * Revision 1.25  2005/06/28 08:50:53  use1146
 * 逆重力時のオブジェクト坂道で角度が128になるバグ修正
 *
 * Revision 1.24  2005/06/24 09:16:47  use1146
 * ループグラインドの逆さ位置に乗れない処理を逆重力対応
 *
 * Revision 1.23  2005/06/16 05:08:20  use1146
 * 追加フラグ対応
 *
 * Revision 1.22  2005/06/10 11:10:29  use1146
 * ワーニングかいじゃ
 *
 * Revision 1.21  2005/06/03 02:45:38  use1146
 * 左右判定の中心が矩形の外に会った時対応
 *
 * Revision 1.20  2005/05/31 08:52:56  use1146
 * 0に近い方の角度チェックを現在の値に近い角度チェックに修正
 *
 * Revision 1.19  2005/05/26 07:47:31  use1146
 * 角度ジョジョに有効対応
 *
 * Revision 1.18  2005/05/19 08:38:05  use1146
 * 重力変換対応１
 *
 * Revision 1.17  2005/05/03 07:05:16  use1146
 * UNDERPREVフラグ対応
 *
 * Revision 1.16  2005/05/03 05:16:32  use1146
 * すり抜けチェックを修正
 *
 * Revision 1.15  2005/04/15 02:16:13  use1146
 * グラインド崖対応
 *
 * Revision 1.14  2005/04/08 12:49:19  use1146
 * 壁HIT時の速度クリア修正
 *
 * Revision 1.13  2005/04/06 10:00:51  use1146
 * グラインド崖対応
 *
 * Revision 1.12  2005/03/30 07:20:21  use1146
 * すり抜けチェックの各角度対応
 *
 * Revision 1.11  2005/03/29 12:02:23  use1146
 * 角度セットにフラグチェックを追加
 *
 * Revision 1.10  2005/03/29 05:33:06  use1146
 * 各不具合修正
 *
 * Revision 1.9  2005/03/10 11:27:26  use1146
 * 足元厳密チェック追加
 *
 * Revision 1.8  2005/03/08 11:11:53  use1146
 * RIDE作成
 *
 * Revision 1.7  2005/03/01 05:32:06  use1146
 * 坂チェックミス修正
 *
 * Revision 1.6  2005/02/25 08:32:17  use1146
 * スルー、グラインド対応
 *
 * Revision 1.5  2005/02/21 09:16:33  use1146
 * 属性取得対応
 *
 * Revision 1.4  2005/02/09 11:32:12  use1146
 * サーフェイス対応
 *
 * Revision 1.3  2005/02/03 05:53:12  use1146
 * 前方向の判定を2点に修正
 *
 * Revision 1.2  2005/01/27 05:48:07  use1146
 * 坂道対応
 *
 * Revision 1.1  2005/01/20 03:09:30  use1146
 * 登録
 *
 */