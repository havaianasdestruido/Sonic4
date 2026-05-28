// ================================================================
/*!
  @file objRectCheck.c
  @brief 喰らい判定

  @author mana
                Copyright(c) 2004 Dimps

  $Id: objRectCheck.cpp 2 2011-04-11 05:21:26Z thamada $
  $Date:: 2011-04-11 14:21:26 +0900#$
 */
// ================================================================
/*
 * Memo
 *
 */
//----- Include Files --------------------------------------------------
#include "pch.h"
#include "objRectCheck.h"
#include "objObject.h"

//----- Macros ---------------------------------------------------------
// ================================================================
// OBM_SWAP
/*!
  値を入れ替える
  
  @param    a   [io] 値
  @param    b   [io] 値
 */
// ================================================================
//#define OBM_SWAP( a, b )   {(a)^=(b);(b)^=(a);(a)^=(b);}

//----- Macros Functions -----------------------------------------------

//----- Definitions ----------------------------------------------------
#define LEFT   (0) ///< 左
#define TOP    (1) ///< 上
#define RIGHT  (2) ///< 右
#define BOTTOM (3) ///< 下
#define BACK   (4) ///< 奥
#define FRONT  (5) ///< 手前

#define OBD_RECT_USER_RESIST	( 10 )											///< 1グループ当たり矩形登録平均数
#define OBD_RECT_NUM			( OBD_RECT_GROUP_NO_NUM * OBD_RECT_USER_RESIST )	///< 登録矩形最大数
#define OBD_RECT_DEFAULT_DEPTH	( 16 )											///< 標準Z値

#if defined (MTD_DEBUG)
/// デバック矩形描画用ワーク
typedef struct tag_OBS_DEBUG_RECT_DT_WORK {
	NNS_MATRIX		mtx;
	NNS_PRIM3D_P	vtx[5];
	NNS_RGBA		col;
} OBS_DEBUG_RECT_DT_WORK;
#endif

//----- External Declarations ------------------------------------------

//----- Static Declarations --------------------------------------------
static u16 objRectCheckFuncCall(OBS_RECT_WORK* pObjA, OBS_RECT_WORK*  pObjD);
static void objRectCheckGroup( OBS_RECT_WORK** GroupA, OBS_RECT_WORK** GroupD, u8 GroupNumA, u8 GroupNumD, u8 Index);

#if defined (MTD_DEBUG)
static void objDebugRectExDisp_DT(void *param);
#endif
//----- Global Variables -----------------------------------------------

//----- Local Variables ------------------------------------------------
static OBS_RECT_WORK* _obj_user_resist[OBD_RECT_NUM];            ///< 矩形ポインタ登録table
static u8             _obj_user_resist_num[OBD_RECT_GROUP_NO_NUM];  ///< 登録数
static u16            _obj_user_resist_all_num;                  ///< 全登録数
static u16            _obj_user_flag[OBD_RECT_GROUP_NO_NUM];        ///< 登録されたデータの統合フラグ
static OBS_RECT_WORK* _obj_user_resist_nx[OBD_RECT_NUM];           ///< 矩形ポインタ登録table
static u8             _obj_user_resist_num_nx[OBD_RECT_GROUP_NO_NUM]; ///< 登録数
static u16            _obj_user_flag_nx[OBD_RECT_GROUP_NO_NUM];       ///< 登録されたデータの統合フラグ
static u16            _obj_user_resist_all_num_nx;                ///< 全登録数

static u32 _obj_ulFlagBackA; ///< バックアップフラグ
static u32 _obj_ulFlagBackD; ///< バックアップフラグ
static u8  _obj_ucNoHit;     ///< チェック用フラグ

/* デバック用設定 */
#if defined (MTD_DEBUG)
/// ObjDebugRectDispAll で表示する矩形グループ設定
static u8	obj_debug_rect_disp_check_group = OBD_RECT_TARGET_G_FLAG_1 | OBD_RECT_TARGET_G_FLAG_2 | OBD_RECT_TARGET_G_FLAG_3 | OBD_RECT_TARGET_G_FLAG_4;
static NNS_RGBA obj_debug_rect_draw_col[] =
{
	{1.0f, 0.0f, 0.0f, 1.0f},		// 赤
	{0.0f, 1.0f, 0.0f, 1.0f},		// 緑
	{0.0f, 0.0f, 1.0f, 1.0f},		// 青
	{1.0f, 1.0f, 0.0f, 1.0f},		// 黄
                       
	{1.0f, 0.0f, 1.0f, 1.0f},		// ピンク
	{0.0f, 1.0f, 1.0f, 1.0f},		// 水色
	{0.0f, 0.0f, 0.0f, 1.0f},		// 黒
	{1.0f, 1.0f, 1.0f, 1.0f},		// 白
                       
	{1.0f, 0.25f, 0.25f, 1.0f},		// 薄赤
	{0.25f, 1.0f, 0.25f, 1.0f},		// 薄緑
	{0.25f, 0.25f, 1.0f, 1.0f},		// 薄青
	{1.0f, 1.0f, 0.25f, 1.0f},		// 薄黄
                       
	{1.0f, 0.25f, 1.0f, 1.0f},		// 薄ピンク
	{0.25f, 1.0f, 1.0f, 1.0f},		// 薄水色
	{0.5f, 0.5f, 0.5f, 1.0f},		// 灰
	{0.0f, 0.75f, 0.0f, 1.0f},		// 濃緑
};
#endif	// #if defined (MTD_DEBUG)
//----- Global Functions -----------------------------------------------


// ================================================================
// ObjRectSet
/*!
  当り設定

  @param pRec    [io] 矩形構造体
  @param cLeft   [in] 左端値
  @param cTop    [in] 上端値
  @param cRight  [in] 右端値
  @param cBottom [in] 下端値
    
 */
// ================================================================
OBS_RECT * ObjRectSet ( OBS_RECT * pRec, s16 cLeft, s16 cTop, s16 cRight, s16 cBottom)
{
    // pRec->flag |= OBD_RECT_ENABLE;

    pRec->left   = cLeft;
    pRec->top    = cTop;
    pRec->right  = cRight;
    pRec->bottom = cBottom;
    pRec->back   = -OBD_RECT_DEFAULT_DEPTH; // 標準奥行き設定
    pRec->front  = OBD_RECT_DEFAULT_DEPTH;
    
    // 入れ替えチェック
    if ( pRec->right < pRec->left )
        MTM_MATH_SWAP( pRec->left, pRec->right );
    if ( pRec->bottom < pRec->top )
        MTM_MATH_SWAP( pRec->top, pRec->bottom );

    VEC_Set( &pRec->pos, 0,0,0 );
    return pRec;
}
// ================================================================
// ObjRectZSet
/*!
  当り設定

  @param pRec    [io] 矩形構造体
  @param cLeft   [in] 左端値
  @param cTop    [in] 上端値
  @param cBack   [in] 奥端値
  @param cRight  [in] 右端値
  @param cBottom [in] 下端値
  @param cFront  [in] 前端値
    
 */
// ================================================================
OBS_RECT * ObjRectZSet ( OBS_RECT * pRec, s16 cLeft, s16 cTop, s16 cBack, s16 cRight, s16 cBottom, s16 cFront )
{
    // pRec->flag |= OBD_RECT_ENABLE;
    pRec->left   = cLeft;
    pRec->top    = cTop;
    pRec->right  = cRight;
    pRec->bottom = cBottom;
    pRec->back   = cBack; // 標準奥行き設定
    pRec->front  = cFront;

    // 入れ替えチェック
    if ( pRec->right < pRec->left )
        MTM_MATH_SWAP( pRec->left, pRec->right );
    if ( pRec->bottom < pRec->top )
        MTM_MATH_SWAP( pRec->top, pRec->bottom );
    if ( pRec->front < pRec->back )
        MTM_MATH_SWAP( pRec->back, pRec->front );

    VEC_Set( &pRec->pos, 0,0,0 );
    return pRec;
}

// ================================================================
// ObjRectAllSet
/*!
  当り設定

  @param pRec    [io] 矩形構造体
  @param pos     [in] 座標
  @param cLeft   [in] 左端値
  @param cTop    [in] 上端値
  @param cBack   [in] 奥端値
  @param cRight  [in] 右端値
  @param cBottom [in] 下端値
  @param cFront  [in] 前端値
    
 */
// ================================================================
OBS_RECT * ObjRectAllSet ( OBS_RECT * pRec, VecFx32 pos, s16 cLeft, s16 cTop, s16 cBack, s16 cRight, s16 cBottom, s16 cFront )
{
    // pRec->flag |= OBD_RECT_ENABLE;
    pRec->pos = pos;
    
    pRec->left   = cLeft;
    pRec->top    = cTop;
    pRec->right  = cRight;
    pRec->bottom = cBottom;
    pRec->back   = cBack; // 標準奥行き設定
    pRec->front  = cFront;

    // 入れ替えチェック
    if ( pRec->right < pRec->left )
        MTM_MATH_SWAP( pRec->left, pRec->right );
    if ( pRec->bottom < pRec->top )
        MTM_MATH_SWAP( pRec->top, pRec->bottom );
    if ( pRec->front < pRec->back )
        MTM_MATH_SWAP( pRec->back, pRec->front );

    return pRec;
}

// ================================================================
// ObjRectWorkSet
/*!
  当り設定

  @param pRec    [io] 矩形構造体
  @param cLeft   [in] 左端値
  @param cTop    [in] 上端値
  @param cRight  [in] 右端値
  @param cBottom [in] 下端値
    
 */
// ================================================================
OBS_RECT * ObjRectWorkSet ( OBS_RECT_WORK * pRec, s16 cLeft, s16 cTop, s16 cRight, s16 cBottom)
{
    pRec->flag |= OBD_RECT_ENABLE;

    return ObjRectSet ( &pRec->rect, cLeft, cTop, cRight, cBottom);

}
// ================================================================
// ObjRectWorkZSet
/*!
  当り設定

  @param pRec    [io] 矩形構造体
  @param cLeft   [in] 左端値
  @param cTop    [in] 上端値
  @param cBack   [in] 奥端値
  @param cRight  [in] 右端値
  @param cBottom [in] 下端値
  @param cFront  [in] 前端値
    
 */
// ================================================================
OBS_RECT * ObjRectWorkZSet ( OBS_RECT_WORK * pRec, s16 cLeft, s16 cTop, s16 cBack, s16 cRight, s16 cBottom, s16 cFront )
{
    pRec->flag |= OBD_RECT_ENABLE;

    return ObjRectZSet ( &pRec->rect, cLeft, cTop, cBack, cRight, cBottom, cFront );
}

// ================================================================
// ObjRectWorkAllSet
/*!
  当り設定

  @param pRec    [io] 矩形構造体
  @param pos     [in] 座標
  @param cLeft   [in] 左端値
  @param cTop    [in] 上端値
  @param cBack   [in] 奥端値
  @param cRight  [in] 右端値
  @param cBottom [in] 下端値
  @param cFront  [in] 前端値
    
 */
// ================================================================
OBS_RECT * ObjRectWorkAllSet ( OBS_RECT_WORK * pRec, VecFx32 pos, s16 cLeft, s16 cTop, s16 cBack, s16 cRight, s16 cBottom, s16 cFront )
{
    pRec->flag |= OBD_RECT_ENABLE;

    return ObjRectAllSet( &pRec->rect,  pos, cLeft, cTop, cBack, cRight, cBottom, cFront);
}


// ================================================================
// ObjRectGroupSet
/*!
  矩形グループ設定

  @param pRec          [io] 拡張矩形構造体
  @param group_no      [in] 矩形グループNO OBD_RECT_GROUP_NO_1 ～ OBD_RECT_GROUP_NO_8
  @param target_g_flag [in] 攻撃対象グループフラグ OBD_RECT_TARGET_G_FLAG_*

  @note
    target_g_flag は 攻撃対象分 or をかけて設定して下さい
 */
// ================================================================
void ObjRectGroupSet ( OBS_RECT_WORK * pRec, u8 group_no, u8 target_g_flag )
{
	MTM_ASSERT(group_no < OBD_RECT_GROUP_NO_NUM);
	MTM_ASSERT(target_g_flag);

	pRec->group_no		= group_no;
	pRec->target_g_flag	= target_g_flag;
}

// ================================================================
// ObjRectAtkSet
/*!
  当り属性設定

  @param pRec       [io] 拡張矩形構造体
  @param usHitFlag  [in] 攻撃フラグ
  @param usHitPower [in] 攻撃値
    
 */
// ================================================================
void ObjRectAtkSet ( OBS_RECT_WORK * pRec, u16 usHitFlag, s16 sHitPower )
{
    pRec->flag |= OBD_RECT_ENABLE;
    pRec->hit_flag  = usHitFlag;
    pRec->hit_power = sHitPower;
    pRec->flag &= ~OBD_RECT_HIT; 
    pRec->flag &= ~OBD_RECT_HIT_UP; 
}
// ================================================================
// ObjRectDefSet
/*!
  当り属性設定

  @param pRec       [io] 拡張矩形構造体
  @param usDefFlag  [in] 防御フラグ
  @param usDefPower [in] 防御値
    
 */
// ================================================================
void ObjRectDefSet ( OBS_RECT_WORK * pRec, u16 usDefFlag, s16 sDefPower)
{
    pRec->flag |= OBD_RECT_ENABLE;
    pRec->def_flag = usDefFlag;
    pRec->def_power = sDefPower;
    pRec->flag &= ~OBD_RECT_DAMAGE; 
}
// ================================================================
// ObjRectHitAgain
/*!
  当り属性設定

  @param pRec       [io] 拡張矩形構造体
    
 */
// ================================================================
void ObjRectHitAgain ( OBS_RECT_WORK * pRec )
{
    MTM_ASSERT( pRec );
    // 矩形外れるまで再HITを行わないフラグが立っていなければ
    if ( !(pRec->flag & OBD_RECT_OUT )){
        // 再HITのためにフラグを寝かす
        pRec->flag &= ~OBD_RECT_DAMAGE; 
        pRec->flag &= ~OBD_RECT_HIT; 
    }
}


// ================================================================
// ObjRectCheckInit
/*!
  当り登録数 初期化
 
 */
// ================================================================
void ObjRectCheckInit()
{
#if 1
    MI_CpuClear8( _obj_user_resist, sizeof(_obj_user_resist) );
    MI_CpuClear8( _obj_user_resist_nx, sizeof(_obj_user_resist_nx) );
    MI_CpuClear8( _obj_user_resist_num, sizeof(_obj_user_resist_num) );
    MI_CpuClear8( _obj_user_resist_num_nx, sizeof(_obj_user_resist_num_nx) );
    MI_CpuClear8( _obj_user_flag, sizeof(_obj_user_flag) );
    MI_CpuClear8( _obj_user_flag_nx, sizeof(_obj_user_flag_nx) );
    
#else
    u8 i,j;
    
    for ( i = 0; i < OBD_RECT_NUM; ++i ){
        _obj_user_resist[i] =  0;
        _obj_user_resist_nx[i] = 0;
    }
    // 登録数をクリアする
    for ( i = 0; i < OBD_RECT_GROUP_NO_NUM; ++i ){
        _obj_user_resist_num[i] = 0;
        _obj_user_resist_num_nx[i] = 0;
        _obj_user_flag[i] = 0;
        _obj_user_flag_nx[i] = 0;
    }
#endif

    _obj_user_resist_all_num = 0;
    _obj_user_resist_all_num_nx = 0;

    _obj_ulFlagBackA = 0;
    _obj_ulFlagBackD = 0;
    _obj_ucNoHit = 0;
}
//----- Local Functions ------------------------------------------------
// ================================================================
// objRectCheckOut
/*!
  当り登録引継ぎ
 
 */
// ================================================================
static void objRectCheckOut()
{
#if 1
    u16 i,j = 0;
    OBS_RECT_WORK* temp;
    
    // 登録ポインタを移動
    MI_CpuCopy8( _obj_user_resist_nx, _obj_user_resist, sizeof(_obj_user_resist) );
    MI_CpuClear8( _obj_user_resist_nx, sizeof(_obj_user_resist_nx) );

    // 登録数を移動
    MI_CpuCopy8( _obj_user_resist_num_nx, _obj_user_resist_num, sizeof(_obj_user_resist_num) );
    MI_CpuClear8( _obj_user_resist_num_nx, sizeof(_obj_user_resist_num_nx) );

    // 登録チェックフラグを移動
    MI_CpuCopy8( _obj_user_flag_nx, _obj_user_flag, sizeof(_obj_user_flag) );
    MI_CpuClear8( _obj_user_flag_nx, sizeof(_obj_user_flag_nx) );

    // 全体数を移動
    _obj_user_resist_all_num = _obj_user_resist_all_num_nx;
    _obj_user_resist_all_num_nx = 0;
    
    // 登録ポインタをソート(最大数が少ないのでバブルソート)
    for ( i = 0; i < _obj_user_resist_all_num-1 ; ++i ){
        // 後ろからチェック
        for ( j = (u16)(_obj_user_resist_all_num-1); j > i; --j ){
            //if ( (_obj_user_resist[j]->user_flag & OBD_RECT_USE_MASK) < (_obj_user_resist[j-1]->user_flag & OBD_RECT_USE_MASK) ){
			if (_obj_user_resist[j]->group_no < _obj_user_resist[j-1]->group_no) {
                // 入れ替え
                temp = _obj_user_resist[j-1];
                _obj_user_resist[j-1] = _obj_user_resist[j];
                _obj_user_resist[j] = temp;
            }
        }
    }

#else
    u8 i,j;

    // 登録ポインタを移動する
    for ( i = 0; i < OBD_RECT_NUM; ++i ){
        _obj_user_resist[i] = _obj_user_resist_nx[i];
        _obj_user_resist_nx[i] = 0;
    }
    
    // 登録数を移動する
    for ( i = 0; i < OBD_RECT_GROUP_NO_NUM; ++i ){
        _obj_user_resist_num[i] = _obj_user_resist_num_nx[i];
        _obj_user_resist_num_nx[i] = 0;
        _obj_user_flag[i] = _obj_user_flag_nx[i];
        _obj_user_flag_nx[i] = 0;
    }
#endif
}
// ================================================================
// ObjRectRegist
/*!
  矩形を当り登録する
 
  @param pObj1 [in] 対象矩形ポインタ
 
 */
// ================================================================
void ObjRectRegist(OBS_RECT_WORK * pObj )
{
#if 1
    // 有効チェック
    if ( pObj->flag & OBD_RECT_ENABLE ){
		// 矩形所属グループNOチェック
		if (pObj->group_no < OBD_RECT_GROUP_NO_NUM) {
			// 登録数オーバーチェック
			if (_obj_user_resist_all_num_nx < OBD_RECT_NUM) {
				// リストへ登録
				_obj_user_resist_nx[ _obj_user_resist_all_num_nx ] = pObj;
				// チェック対象グループフラグをセット
				_obj_user_flag_nx[pObj->group_no] |= pObj->target_g_flag;
				// グループごとの登録数をプラス
				++_obj_user_resist_num_nx[pObj->group_no];
				// 登録数をプラス
				++_obj_user_resist_all_num_nx;
			}
			return;
		}
    }
#else
    u16 i;
    // 有効チェック
    if ( pObj->flag & OBD_RECT_ENABLE ){
        // グループ数分ループ
        for(i = 0; i < OBD_RECT_GROUP_NO_NUM; ++i ){
            
            // 各グループ登録チェック
            if ( pObj->user_flag & (OBD_RECT_USE1 << i) ){
                
                // 登録数オーバーチェック
                if ( _obj_user_resist_all_num_nx < OBD_RECT_NUM ){

                    // リストへ登録
                    _obj_user_resist_nx[ _obj_user_resist_all_num_nx ] = pObj;
                    // フラグをセット
                    _obj_user_flag_nx[ i ] |= pObj->user_flag;
                    // グループごとの登録数をプラス
                    ++_obj_user_resist_num_nx[i];
                    // 登録数をプラス
                    ++_obj_user_resist_all_num_nx;
                }
                return;
            }
        }
    }
#endif
}
// ================================================================
// ObjRectCheckAllGroup
/*!
  総当り判定チェック
 */
// ================================================================
void ObjRectCheckAllGroup()
{
    u16 i, usAtk,usDef;
    u8  j;

    if ( g_obj.flag & OBD_OBJ_RECT_JUSTFRAME )
        // 登録データ移動と初期化
        objRectCheckOut();

    // デバッグ矩形表示
    ObjDebugRectDispAll();
    
    // ワーク初期化
    _obj_ulFlagBackA = 0;
    _obj_ulFlagBackD = 0;
    _obj_ucNoHit = 0;
    
    // 矩形表示登録＆OBD_RECT_FRAMEHIT寝かせ処理
    // ループ
    for(i = 0; i < _obj_user_resist_all_num; ++i ){
        if ( _obj_user_resist[i] ){
            if (_obj_user_resist[i]->flag & OBD_RECT_OUT )
                _obj_user_resist[i]->flag &= ~OBD_RECT_FRAMEHIT;
        }
    }

    // 位置初期化
    usAtk = 0;
    // 攻撃者ループ
    for(i = 0; i < OBD_RECT_GROUP_NO_NUM; ++i ){
        usDef = 0;
        // 防御者ループ
        for(j = 0; j < OBD_RECT_GROUP_NO_NUM; ++j ){
            
            // 防御グループの登録数チェック、攻撃グループの攻撃グループフラグチェック
            if ( _obj_user_resist_num[j] && (_obj_user_flag[i] & (OBD_RECT_TARGET_G_FLAG_1 << j)) ) {
                objRectCheckGroup( &_obj_user_resist[usAtk], &_obj_user_resist[usDef],
                                   _obj_user_resist_num[i], _obj_user_resist_num[j], j );
			}
            usDef += _obj_user_resist_num[j];
        }
        usAtk += _obj_user_resist_num[i];
    }

    // OBD_RECT_FRAMEHITチェック、HITUPチェック
    for(i = 0; i < _obj_user_resist_all_num; ++i ){
        if ( _obj_user_resist[i] ){
            // HITUPチェック
            if ( _obj_user_resist[i]->flag & OBD_RECT_HIT_UP ){
                // 一度でもHITした
                _obj_user_resist[i]->flag |= OBD_RECT_HIT;
                _obj_user_resist[i]->flag &= ~OBD_RECT_HIT_UP;
            }
            // OBD_RECT_FRAMEHITチェック
            if ( _obj_user_resist[i]->flag & OBD_RECT_OUT ){
                if (!( _obj_user_resist[i]->flag & OBD_RECT_FRAMEHIT ))
                    // 今フレームあたらなかった
                    _obj_user_resist[i]->flag &= ~OBD_RECT_FRAMEOUT;
            }
        }
    }

    if (!( g_obj.flag & OBD_OBJ_RECT_JUSTFRAME))
        // 登録データ移動と初期化
        objRectCheckOut();
        
}
// ================================================================
// ObjRectRegistGet
/*!
  登録された矩形ポインタを取得する
 
  @param ucGroup [in] チェックする登録グループフラグ
  @param usIndex [in] 番号

 @return 拡張矩形ポインタ
 */
// ================================================================
OBS_RECT_WORK* ObjRectRegistGet( u8 ucGroup, s16 sIndex )
{
    s16 i = 0;
    u16 num = 0;
    // グループループ
    for ( i= 0; i < OBD_RECT_GROUP_NO_NUM; ++i ){
        
        // チェックするグループかチェック
        if ( ucGroup & (1 << i) ){
            
            // インデクスがグループ内で収まっているかチェック
            if ( sIndex < _obj_user_resist_num[i] ){
                return _obj_user_resist[ num + sIndex ];
            }else{
                // オーバー分を差し引く
                sIndex -= _obj_user_resist_num[i];
                num += _obj_user_resist_num[i];
                if ( sIndex <= 0 )
                    continue;
            }
        }else{
            num += _obj_user_resist_num[i];
        }
    }
    return NULL;
}
// ================================================================
// ObjRectRegistNxGet
/*!
  このフレームに登録された矩形ポインタを取得する
 
  @param ucGroup [in] チェックする登録グループフラグ
  @param usIndex [in] 番号

 @return 拡張矩形ポインタ
 */
// ================================================================
OBS_RECT_WORK* ObjRectRegistNxGet( u8 ucGroup, s16 sIndex )
{
    s16 i = 0;
    u16 num = 0;
    // グループループ
    for ( i= 0; i < OBD_RECT_GROUP_NO_NUM; ++i ){
        
        // チェックするグループかチェック
        if ( ucGroup & (1 << i) ){
            
            // インデクスがグループ内で収まっているかチェック
            if ( sIndex < _obj_user_resist_num_nx[i] ){
                return _obj_user_resist_nx[ num + sIndex ];
            }else{
                // オーバー分を差し引く
                sIndex -= _obj_user_resist_num_nx[i];
                num += _obj_user_resist_num_nx[i];
                if ( sIndex <= 0 )
                    continue;
            }
        }else{
            num += _obj_user_resist_num_nx[i];
        }
    }
    return NULL;
}


// ================================================================
// objRectCheckGroup
/*!
  Groupごとの当り判定
 
  @param GroupA    [in] 攻撃対象矩形ポインタ
  @param GroupD    [in] 防御対象矩形ポインタ
  @param GroupNumA [in] 攻撃対象矩形ポインタ数
  @param GroupNumD [in] 防御対象矩形ポインタ数
  @param Index     [in] 防御側GROUP番号
 
 */
// ================================================================
static void objRectCheckGroup( OBS_RECT_WORK** GroupA, OBS_RECT_WORK** GroupD, u8 GroupNumA, u8 GroupNumD, u8 Index)
{
    u16 i,j;
    OBS_RECT_WORK * pObjP;
    OBS_RECT_WORK * pObjE;

    s32 lLeftest1, lTopest1;
    s32 lLeftest2, lTopest2;
    s32 lBack1,lBack2;
    u16 usWidth1, usHeight1;
    u16 usWidth2, usHeight2;
    u16 usDepth1, usDepth2;

    // 攻撃グループ
    for (i= 0; i < GroupNumA; ++i ){
        pObjP = GroupA[i];

        // もう消えてる
        if ( pObjP == NULL )
            continue;
        
        // もう判定しなくもいい状態
        if ( pObjP->flag & ( OBD_RECT_NOHIT ) ||
             !(pObjP->flag & OBD_RECT_ENABLE) )
            continue;
        
        // 今からチェックするGROUPが攻撃対象かチェック
#if 1
		if (!(pObjP->target_g_flag & (1 << Index))) {
			continue;
		}
#else
        if (! ( (pObjP->user_flag >> OBD_RECT_GROUP_NO_NUM) & ( 1 << Index ) ))
            continue;
#endif
        
        // 端位置設定
        if ( pObjP->parent_obj )
            // 親のフラグも判定
            if ( pObjP->parent_obj->flag & ( OBD_OBJECT_NOHIT | OBD_OBJECT_TASKCLEAR ) )
                continue;
        
        // 端位置を設定
        ObjRectLTBSet( pObjP, &lLeftest1, &lTopest1, &lBack1 );
        
        // 幅設定
        ObjRectWHDSet( pObjP, &usWidth1, &usHeight1, &usDepth1 );

        // 防御側ループ
        for (j= 0; j < GroupNumD; ++j ){
            pObjE = GroupD[j];

            // もう消えてる
            if ( GroupA[i] == NULL )
                break;
            // もう消えてる
            if ( pObjE == NULL )
                continue;
            // 自分とチェック
            if ( pObjE == pObjP )
                continue;
            // 親オブジェクトが同じ
            if ( pObjE->parent_obj == pObjP->parent_obj && pObjE->parent_obj)
                continue;

            // もう判定しなくもいい状態
            if ( (pObjE->flag | pObjP->flag) & ( OBD_RECT_NOHIT ) ||
                 !(pObjE->flag & OBD_RECT_ENABLE) )
                continue;

            // 親判定
            //if ( pObjE == pObjP->parent_obj )
            //    continue;

            // 端位置設定
            if ( pObjE->parent_obj )
                // 親のフラグも判定
                if ( pObjE->parent_obj->flag & ( OBD_OBJECT_NOHIT | OBD_OBJECT_TASKCLEAR ) )
                    continue;

            // 端位置を設定
            ObjRectLTBSet( pObjE, &lLeftest2, &lTopest2, &lBack2 );

            // 幅設定
            ObjRectWHDSet( pObjE, &usWidth2, &usHeight2, &usDepth2 );
            
            // 判定
#if OBD_USE_RECT_CHECK_DIPTH
            if( ( pObjE->flag | pObjP->flag) & OBD_RECT_CHECK_FUNC || 
                (( OBM_LINE_AND_LINE(lLeftest1, usWidth1, lLeftest2,  usWidth2)) &&
                 ( OBM_LINE_AND_LINE(lTopest1, usHeight1,  lTopest2, usHeight2)) &&
                 ( OBM_LINE_AND_LINE(lBack1,    usDepth1,    lBack2,  usDepth2))   ) ){
#else
            if( ( pObjE->flag | pObjP->flag) & OBD_RECT_CHECK_FUNC || 
                (( OBM_LINE_AND_LINE(lLeftest1, usWidth1, lLeftest2,  usWidth2)) &&
                 ( OBM_LINE_AND_LINE(lTopest1, usHeight1,  lTopest2, usHeight2)) ) ){
#endif

                u16 usRet;
                // ダメージ判定、処理
                usRet = objRectCheckFuncCall( pObjP, pObjE );

                // 攻撃側がグループ攻撃でなかったのでリストから消える
                if ( usRet & 0x01 ){
                    if ( pObjP->flag & OBD_RECT_HIT_UP ){
                        pObjP->flag |= OBD_RECT_HIT;
                        pObjP->flag &= ~OBD_RECT_HIT_UP;
                    }
                    GroupA[i] = NULL;
                }

                // 防御側がグループ攻撃でなかったのでリストから消える
                if ( usRet & 0x02 ){
                    if ( pObjE->flag & OBD_RECT_HIT_UP ){
                        pObjE->flag |= OBD_RECT_HIT;
                        pObjE->flag &= ~OBD_RECT_HIT_UP;
                    }
                    GroupD[j] = NULL;
                }
                
                /* 総チェックするので逆パターンはチェックしない
                    
                // 逆チェック
                usRet = objRectCheckFuncCall( pObjE, pObjP );

                // 攻撃側がグループ攻撃でなかったのでリストから消える
                if ( usRet & 0x01 )
                    GroupD[j] = NULL;

                // 防御側がグループ攻撃でなかったのでリストから消える
                if ( usRet & 0x02 )
                    GroupA[i] = NULL;
                //}
                    
                 */
            }

        }
    }
}
// ================================================================
// ObjRectPosGet
/*!
  端位置を設定
 
  @param vPos  [out] 位置
  @param pRec  [in]  矩形ワーク
 
 */
// ================================================================
void ObjRectPosGet( VecFx32 * vPos, OBS_RECT_WORK* pRec )
{
    if ( pRec->parent_obj && !(pRec->flag & OBD_RECT_NOPOS) ){
        // 親座標から座標を設定
        vPos->x = pRec->parent_obj->pos.x + pRec->rect.pos.x;
        vPos->y = pRec->parent_obj->pos.y + pRec->rect.pos.y;
        vPos->z = pRec->parent_obj->pos.z + pRec->rect.pos.z;
    }else{
        // ワークに設定された座標を用いる
        vPos->x = pRec->rect.pos.x;
        vPos->y = pRec->rect.pos.y;
        vPos->z = pRec->rect.pos.z;
    }
}
// ================================================================
// ObjRectLTBSet
/*!
  端位置を設定
 
  @param pRec  [in] 矩形ワーク
  @param lLeft [out] 左端位置
  @param lTop  [out] 上端位置
  @param lBack [out] 奥端位置
 
 */
// ================================================================
void ObjRectLTBSet( OBS_RECT_WORK* pRec, s32 *lLeft, s32*lTop, s32 *lBack )
{
    s32 temp = 0;
    if ( pRec->parent_obj && !(pRec->flag & OBD_RECT_NOPOS) ){

        // 親の位置から設定する
        if ( lLeft ){
            if ( (pRec->parent_obj->disp_flag & OBD_DISP_HFLIP) ^ (pRec->flag & OBD_RECT_HFLIP) )
                temp = - (pRec)->rect.right;
            else
                temp = (pRec)->rect.left;

            // 拡大率チェック
            if ( pRec->parent_obj->scale.x != FX32_ONE )
                temp = FX_Mul( temp, pRec->parent_obj->scale.x);
			if ( g_obj.draw_scale.x != FX32_ONE && !(pRec->parent_obj->disp_flag & OBD_DISP_NODRAWSCALE) &&
					!(g_obj.flag & OBD_OBJ_RECT_NOUSE_DRAWSCALE)) {
                temp = FX_Mul( temp, g_obj.draw_scale.x);
			}
            
            *lLeft = ( (pRec->parent_obj->pos.x + pRec->rect.pos.x ) >> FX32_SHIFT) + temp;
        }
        if ( lTop ){
            if ( (pRec->parent_obj->disp_flag & OBD_DISP_VFLIP) ^ (pRec->flag & OBD_RECT_VFLIP) )
                temp = - (pRec)->rect.bottom;
            else
                temp = (pRec)->rect.top;

            // 拡大率チェック
            if ( pRec->parent_obj->scale.y != FX32_ONE )
                temp = FX_Mul( temp, pRec->parent_obj->scale.y);
			if ( g_obj.draw_scale.y != FX32_ONE && !(pRec->parent_obj->disp_flag & OBD_DISP_NODRAWSCALE) &&
					!(g_obj.flag & OBD_OBJ_RECT_NOUSE_DRAWSCALE)) {
                temp = FX_Mul( temp, g_obj.draw_scale.y);
			}
            
            *lTop = ( (pRec->parent_obj->pos.y + pRec->rect.pos.y ) >> FX32_SHIFT) + temp;
        }

        if ( lBack ){
            temp = (pRec)->rect.back;
            // 拡大率チェック
            if ( pRec->parent_obj->scale.z != FX32_ONE )
                temp = FX_Mul( temp, pRec->parent_obj->scale.z);
			if ( g_obj.draw_scale.z != FX32_ONE && !(pRec->parent_obj->disp_flag & OBD_DISP_NODRAWSCALE) &&
					!(g_obj.flag & OBD_OBJ_RECT_NOUSE_DRAWSCALE)) {
                temp = FX_Mul( temp, g_obj.draw_scale.z);
			}
            
            *lBack  = ( (pRec->parent_obj->pos.z + pRec->rect.pos.z ) >> FX32_SHIFT) + temp;
        }

    }else{
        // ワークに設定された座標を用いる
        if ( lLeft ){
            if ( pRec->flag & OBD_RECT_HFLIP )
                temp = -(pRec)->rect.right;
            else
                temp = (pRec)->rect.left;
            *lLeft = ( pRec->rect.pos.x >> FX32_SHIFT) + temp;
        }
        if ( lTop ){
            if ( pRec->flag & OBD_RECT_VFLIP )
                temp = -(pRec)->rect.bottom;
            else
                temp = (pRec)->rect.top;
            *lTop  = ( pRec->rect.pos.y >> FX32_SHIFT) + temp;
        }
        if ( lBack )
            *lBack  = ( pRec->rect.pos.z >> FX32_SHIFT) + (pRec)->rect.back;
    }
}
// ================================================================
// ObjRectWHDSet
/*!
  幅、高さ、深さ を設定
 
  @param pRec  [in] 矩形ワーク
  @param lLeft [out] 左端位置
  @param lTop  [out] 上端位置
  @param lBack [out] 奥端位置
 
 */
// ================================================================
void ObjRectWHDSet( OBS_RECT_WORK* pRec, u16 *usWidth, u16* usHeight, u16 *usDepth )
{
	fx32	temp_scale;

    if ( pRec->parent_obj && !(pRec->flag & OBD_RECT_NOPOS) ){
        // 幅設定
        if ( usWidth ){
            *usWidth  = (u16)(((pRec)->rect.right - (pRec)->rect.left ) );
            // 拡大率チェック
#if 1
			temp_scale = FX32_ONE;
			if (pRec->parent_obj->scale.x != FX32_ONE) {
				temp_scale = pRec->parent_obj->scale.x;
			}
			if ( g_obj.draw_scale.x != FX32_ONE && !(pRec->parent_obj->disp_flag & OBD_DISP_NODRAWSCALE) &&
					!(g_obj.flag & OBD_OBJ_RECT_NOUSE_DRAWSCALE)) {
				temp_scale = FX_Mul(temp_scale, g_obj.draw_scale.x);
			}
			if (temp_scale != FX32_ONE) {
                *usWidth = (u16)FX_Mul( *usWidth, temp_scale);
			}
#else
            if ( pRec->parent_obj->scale.x != FX32_ONE )
                *usWidth = (u16)FX_Mul( *usWidth, pRec->parent_obj->scale.x);
			if ( g_obj.draw_scale.x != FX32_ONE && !(pRec->parent_obj->disp_flag & OBD_DISP_NODRAWSCALE) &&
					!(g_obj.flag & OBD_OBJ_RECT_NOUSE_DRAWSCALE)) {
                *usWidth = (u16)FX_Mul( *usWidth, g_obj.draw_scale.x);
			}
#endif
        }
        // 高さ
        if ( usHeight ){
            *usHeight  = (u16)(((pRec)->rect.bottom - (pRec)->rect.top ) );
            // 拡大率チェック
#if 1
			temp_scale = FX32_ONE;
			if (pRec->parent_obj->scale.y != FX32_ONE) {
				temp_scale = pRec->parent_obj->scale.y;
			}
			if ( g_obj.draw_scale.y != FX32_ONE && !(pRec->parent_obj->disp_flag & OBD_DISP_NODRAWSCALE) &&
					!(g_obj.flag & OBD_OBJ_RECT_NOUSE_DRAWSCALE)) {
				temp_scale = FX_Mul(temp_scale, g_obj.draw_scale.y);
			}
			if (temp_scale != FX32_ONE) {
                *usHeight = (u16)FX_Mul( *usHeight, temp_scale);
			}
#else
            if ( pRec->parent_obj->scale.y != FX32_ONE )
                *usHeight = (u16)FX_Mul( *usHeight, pRec->parent_obj->scale.y);
			if ( g_obj.draw_scale.y != FX32_ONE && !(pRec->parent_obj->disp_flag & OBD_DISP_NODRAWSCALE) &&
					!(g_obj.flag & OBD_OBJ_RECT_NOUSE_DRAWSCALE)) {
                *usHeight = (u16)FX_Mul( *usHeight, g_obj.draw_scale.y);
			}
#endif
        }
        // 奥行き
        if ( usDepth ){
            *usDepth  = (u16)(((pRec)->rect.front - (pRec)->rect.back ) );
            // 拡大率チェック
#if 1
			temp_scale = FX32_ONE;
			if (pRec->parent_obj->scale.z != FX32_ONE) {
				temp_scale = pRec->parent_obj->scale.z;
			}
			if ( g_obj.draw_scale.z != FX32_ONE && !(pRec->parent_obj->disp_flag & OBD_DISP_NODRAWSCALE) &&
					!(g_obj.flag & OBD_OBJ_RECT_NOUSE_DRAWSCALE)) {
				temp_scale = FX_Mul(temp_scale, g_obj.draw_scale.z);
			}
			if (temp_scale != FX32_ONE) {
                *usDepth = (u16)FX_Mul( *usDepth, temp_scale);
			}

#else
            if ( pRec->parent_obj->scale.z != FX32_ONE )
                *usDepth = (u16)FX_Mul( *usDepth, pRec->parent_obj->scale.z);
			if ( g_obj.draw_scale.z != FX32_ONE && !(pRec->parent_obj->disp_flag & OBD_DISP_NODRAWSCALE) &&
					!(g_obj.flag & OBD_OBJ_RECT_NOUSE_DRAWSCALE)) {
                *usDepth = (u16)FX_Mul( *usDepth, g_obj.draw_scale.z);
			}
#endif
        }
    }else{
        // 幅設定
        if ( usWidth ){
            *usWidth  = (u16)(((pRec)->rect.right - (pRec)->rect.left ) );
        }
        // 高さ
        if ( usHeight ){
            *usHeight  = (u16)(((pRec)->rect.bottom - (pRec)->rect.top ) );
        }
        // 奥行き
        if ( usDepth ){
            *usDepth  = (u16)(((pRec)->rect.front - (pRec)->rect.back ) );
        }
    }
}
// ================================================================
// ObjRectFlagCheck
/*!
  攻撃無効化チェック
 
  @param ulAtkFlag [in] 攻撃側フラグ
  @param usDefFlag [in] 防御側フラグ
  @param ulAtkFlag [in] 攻撃値
  @param usDefFlag [in] 防御値
 
  @return   0 無効   1 HIT
 */
// ================================================================
u16 ObjRectFlagCheck( u32 ulAtkFlag, u32 usDefFlag, s32 lAtkPower, s32 lDefPower)
{
	if ((ulAtkFlag & (~usDefFlag)) && (lAtkPower >= lDefPower)) {
		// 防御フラグが無いものがあり 防御値が攻撃値以下
		return (1);
	}
	return (0);
}
/*
//#define DBM_ATTACK_CANCEL_CHECK( ulAtkFlag, usDefFlag )            \
//    ( ( ( ulAtkFlag & ~(OBD_HIT_POWER_MASK | OBD_HIT_IGNORE) ) & ~( usDefFlag & ~OBD_HIT_POWER_MASK ) ) && \
//      ( ( ( ulAtkFlag &  OBD_HIT_POWER_MASK ) >= ( usDefFlag & OBD_HIT_POWER_MASK ) ) ) )
*/
// ================================================================
// ObjRectFuncblank
/*!
  当り、関数呼び出し空処理
 
  @param pObjA [in] 対象矩形 攻撃側
  @param pObjD [in] 対象矩形 防御側
 
 */
// ================================================================
void ObjRectFuncblank( OBS_RECT_WORK* pObjA, OBS_RECT_WORK*  pObjD )
{
#if defined _DS
#else
	UNREFERENCED_PARAMETER(pObjA);
	UNREFERENCED_PARAMETER(pObjD);
#endif
}
// ================================================================
// objRectCheckFuncCall
/*!
  当り、関数呼び出しチェック
 
  @param pObjA [in] 対象矩形 攻撃側
  @param pObjD [in] 対象矩形 防御側
 
  @return   0 複数HIT   1 単発HIT
 */
// ================================================================
static u16 objRectCheckFuncCall(OBS_RECT_WORK* pObjA, OBS_RECT_WORK*  pObjD)
{
    u16 ret = 0;
    // バックアップ
    _obj_ulFlagBackA = pObjA->flag;
    _obj_ulFlagBackD = pObjD->flag;
    
    // 自分には当らない
    // if ( pObjA == pObjD )
    //     return ret;

    
    // Aの攻撃がHIT済みでDがダメージ中
    //（ ダメージソースが他の攻撃で、1の攻撃が他の敵に当ったものでも多段HIT回避となる）
    if ( pObjA->flag & OBD_RECT_HIT && pObjD->flag & OBD_RECT_DAMAGE )
        return ret;

    // 属性チェック
    if ( ObjRectFlagCheck( pObjA->hit_flag,  pObjD->def_flag, pObjA->hit_power,  pObjD->def_power) ){

        if ( pObjA->ppCheck )
            if ( !pObjA->ppCheck( pObjA, pObjD ) )
                return ret;
        
        if ( pObjD->ppCheck )
            if ( !pObjD->ppCheck( pObjD, pObjA ) )
                return ret;
        

        
        // 連続HIT回避用フラグをそれぞれ立てる
        if (!( pObjD->flag & (OBD_RECT_NOHIT_UP | OBD_RECT_OUT) || pObjA->flag & (OBD_RECT_NOHIT_UP | OBD_RECT_OUT)))
            pObjA->flag |= OBD_RECT_HIT_UP;
        if (!( pObjD->flag & (OBD_RECT_NODAMAGE | OBD_RECT_OUT) || pObjA->flag & (OBD_RECT_NODAMAGE | OBD_RECT_OUT)))
            pObjD->flag |= OBD_RECT_DAMAGE;

        // フレームアウト済みチェック
        if ( !(pObjA->flag & OBD_RECT_OUT) || !(pObjA->flag & OBD_RECT_FRAMEOUT)){
            // ヒット処理呼び出し
                if ( pObjA->flag & OBD_RECT_OUT )
                    pObjA->flag |= OBD_RECT_FRAMEOUT;
                // 攻め時に呼び出し
                if ( pObjA->ppHit )
                    pObjA->ppHit( pObjA, pObjD );
        }

        // NOHITチェック
        if ( _obj_ucNoHit ){
            _obj_ucNoHit = 0;
            return ret;
        }
        // 入れ替える 攻撃関数と防御関数を分けたので入れ替えない
        // MTM_MATH_SWAP(ulFlagBackA, _obj_ulFlagBackD);

        // 今フレームも矩形HITを確認
        if ( pObjA->flag & OBD_RECT_OUT)
            pObjA->flag |= OBD_RECT_FRAMEHIT;
        
        // フレームアウト済みチェック
        if ( !(pObjD->flag & OBD_RECT_OUT) || !(pObjD->flag & OBD_RECT_FRAMEOUT)){
                if ( pObjD->flag & OBD_RECT_OUT )
                    pObjD->flag |= OBD_RECT_FRAMEOUT;
                // 通常呼び出し（受け時）
                if ( pObjD->ppDef )
                    pObjD->ppDef( pObjD, pObjA );	// 20080514 引数:自分のワークが先になるように変更
                    //pObjD->ppDef( pObjA, pObjD );
        }
        // NOHITチェック
        if ( _obj_ucNoHit ){
            _obj_ucNoHit = 0;
            return ret;
        }

        // 今フレームも矩形HITを確認
        if ( pObjD->flag & OBD_RECT_OUT)
            pObjD->flag |= OBD_RECT_FRAMEHIT;
        
        // 複数HIT矩形でなければリストから消える
        if (!(pObjA->flag & OBD_RECT_GROUP))
            ret |= 0x01;
        // 複数HIT矩形でなければリストから消える
        if (!(pObjD->flag & OBD_RECT_GROUP))
            ret |= 0x02;
    }
    return ret;
}
// ================================================================
// ObjRectFuncNoHit
/*!
  HIT関数内で当たらなかった（処理が行われなかった）場合に呼ぶ関数
 
  @param pObjA [in] 対象矩形 攻撃側
  @param pObjD [in] 対象矩形 防御側
 
  @note
    使わなくて主な動作に支障はありません\n
    ただし状態によって以後HITするタイミングがずれる事あります\n
    登録したppHit内でのみ使用可\n
 */
// ================================================================
void ObjRectFuncNoHit(OBS_RECT_WORK* pMine, OBS_RECT_WORK*  pDamage)
{
    // フラグを書き戻す
    pMine->flag   = _obj_ulFlagBackA;
    pDamage->flag = _obj_ulFlagBackD;

    _obj_ucNoHit = 1;
}
// ================================================================
// ObjRectWorkCheck
/*!
  矩形同士の当り判定を取る
 
  @param pObj1 [in] 対象矩形その１
  @param pObj2 [in] 対象矩形その２
 
  @return   0 NOHIT   1 HIT
 */
// ================================================================
u16 ObjRectWorkCheck (OBS_RECT_WORK* pObj1, OBS_RECT_WORK* pObj2)
{
    if ( pObj1->flag & OBD_RECT_ENABLE && pObj2->flag & OBD_RECT_ENABLE  &&
         !(pObj1->flag & OBD_RECT_NOHIT) && !(pObj2->flag & OBD_RECT_NOHIT)){
        
        s32 lLeftest1, lTopest1;
        s32 lLeftest2, lTopest2;
        u16 usWidth1,usHeight1;
        u16 usWidth2,usHeight2;
        s32 lBack1,lBack2;
        u16 usDepth1, usDepth2;

        // 親のNOHITフラグチェック（不要かも）
        if ( pObj1->parent_obj && pObj1->parent_obj->flag & (OBD_OBJECT_NOHIT | OBD_OBJECT_TASKCLEAR) )
            return 0;
        if ( pObj2->parent_obj && pObj2->parent_obj->flag & (OBD_OBJECT_NOHIT | OBD_OBJECT_TASKCLEAR) )
            return 0;
        
        // 端位置を設定
        ObjRectLTBSet( pObj1, &lLeftest1, &lTopest1, &lBack1 );
        ObjRectLTBSet( pObj2, &lLeftest2, &lTopest2, &lBack2 );

        // 幅設定
        ObjRectWHDSet( pObj1, &usWidth1, &usHeight1, &usDepth1 );
        ObjRectWHDSet( pObj2, &usWidth2, &usHeight2, &usDepth2 );


        // 判定
#if OBD_USE_RECT_CHECK_DIPTH
        if( ( OBM_LINE_AND_LINE(lLeftest1, usWidth1, lLeftest2,  usWidth2)) &&
            ( OBM_LINE_AND_LINE(lTopest1, usHeight1,  lTopest2, usHeight2)) &&
            ( OBM_LINE_AND_LINE(lBack1,    usDepth1,    lBack2,  usDepth2)) ){
#else
        if( ( OBM_LINE_AND_LINE(lLeftest1, usWidth1, lLeftest2,  usWidth2)) &&
            ( OBM_LINE_AND_LINE(lTopest1, usHeight1,  lTopest2, usHeight2)) ){
#endif
            return 1;
        }
    }
    return 0;
        
}

// ================================================================
// ObjRectCheck
/*!
  矩形同士の当り判定を取る
 
  @param pObj1 [in] 対象矩形その１
  @param pObj2 [in] 対象矩形その２
 
  @return   0 NOHIT   1 HIT
 */
// ================================================================
u16 ObjRectCheck (OBS_RECT* pObj1, OBS_RECT* pObj2)
{
    s32 lLeftest1, lTopest1;
    s32 lLeftest2, lTopest2;
    s32 lBack1,lBack2;
    u16 usWidth1,usHeight1;
    u16 usWidth2,usHeight2;
    u16 usDepth1, usDepth2;

    // 端位置を設定
    lLeftest1 = (pObj1->pos.x >> FX32_SHIFT) + pObj1->left;
    lTopest1  = (pObj1->pos.y >> FX32_SHIFT) + pObj1->top;
    lBack1    = (pObj1->pos.z >> FX32_SHIFT) + pObj1->back;
    lLeftest2 = (pObj2->pos.x >> FX32_SHIFT) + pObj2->left;
    lTopest2  = (pObj2->pos.y >> FX32_SHIFT) + pObj2->top;
    lBack2    = (pObj2->pos.z >> FX32_SHIFT) + pObj2->back;

    // 幅設定
    usWidth1  = (u16)(pObj1->right  - pObj1->left );
    usHeight1 = (u16)(pObj1->bottom - pObj1->top  );
    usWidth2  = (u16)(pObj2->right  - pObj2->left );
    usHeight2 = (u16)(pObj2->bottom - pObj2->top  );
    usDepth1  = (u16)(pObj1->front  - pObj1->back );
    usDepth2  = (u16)(pObj1->front  - pObj2->back );

    // 判定
#if OBD_USE_RECT_CHECK_DIPTH
    if( ( OBM_LINE_AND_LINE(lLeftest1, usWidth1, lLeftest2,  usWidth2)) &&
        ( OBM_LINE_AND_LINE(lTopest1, usHeight1,  lTopest2, usHeight2)) &&
        ( OBM_LINE_AND_LINE(lBack1,    usDepth1,    lBack2,  usDepth2)) ){
#else
    if( ( OBM_LINE_AND_LINE(lLeftest1, usWidth1, lLeftest2,  usWidth2)) &&
        ( OBM_LINE_AND_LINE(lTopest1, usHeight1,  lTopest2, usHeight2)) ){
#endif
        return 1;
    }
    return 0;
}
// ================================================================
// ObjRectWorkPointCheck
/*!
  矩形当りと指定範囲の判定を行う
 
  @param pObj1 [in] 対象矩形その１
  @param sX       [in] 対象２の座標値 1:31
  @param sY       [in] 対象２の座標値 
  @param sZ       [in] 対象２の座標値 
 
  @return   0 NOHIT   1 HIT
 */
// ================================================================
u16 ObjRectWorkPointCheck(OBS_RECT_WORK* pObj, s32 lX, s32 lY, s32 lZ )
{
    if ( pObj->flag & OBD_RECT_ENABLE &&
         !(pObj->flag & OBD_RECT_NOHIT) ){
        s32 lLeftest1, lTopest1;
        s32 lLeftest2, lTopest2;
        s32 lBack1,lBack2;
        u16 usWidth1,usHeight1;
        u16 usDepth1;

        // 端位置を設定
        ObjRectLTBSet( pObj, &lLeftest1, &lTopest1, &lBack1 );
        
        lLeftest2 = ( lX );
        lTopest2  = ( lY );
        lBack2    = ( lZ );

        // 幅設定
        ObjRectWHDSet( pObj, &usWidth1, &usHeight1, &usDepth1 );

        // 判定
#if OBD_USE_RECT_CHECK_DIPTH
        if( (OBM_POINT_IN_LINE(lLeftest1, usWidth1, lLeftest2)) &&
            (OBM_POINT_IN_LINE(lTopest1, usHeight1,  lTopest2)) &&
            (OBM_POINT_IN_LINE(lBack1,   usDepth1,     lBack2)) ){
#else
        if( (OBM_POINT_IN_LINE(lLeftest1, usWidth1, lLeftest2)) &&
            (OBM_POINT_IN_LINE(lTopest1, usHeight1,  lTopest2)) ){
#endif
            return 1;
        }
    }
    return 0;
}
// ================================================================
// ObjRectPointCheck
/*!
  矩形当りと指定範囲の判定を行う
 
  @param pObj     [in] 対象矩形その１
  @param sX       [in] 対象２の座標値 1:31
  @param sY       [in] 対象２の座標値 
  @param sZ       [in] 対象２の座標値 
 
  @return   0 NOHIT   1 HIT
 */
// ================================================================
u16 ObjRectPointCheck(OBS_RECT* pObj, s32 lX, s32 lY, s32 lZ )
{
    s32 lLeftest1, lTopest1;
    s32 lLeftest2, lTopest2;
    s32 lBack1,lBack2;
    u16 usWidth1,usHeight1;
    u16 usDepth1;

    // 端位置を設定
    lLeftest1 = pObj->left + (pObj->pos.x >> FX32_SHIFT);
    lTopest1  = pObj->top  + (pObj->pos.y >> FX32_SHIFT);
    lBack1    = pObj->back + (pObj->pos.z >> FX32_SHIFT);

    lLeftest2 = ( lX );
    lTopest2  = ( lY );
    lBack2    = ( lZ );

    // 幅設定
    usWidth1  = (u16)(pObj->right  - pObj->left);
    usHeight1 = (u16)(pObj->bottom - pObj->top);
    usDepth1  = (u16)(pObj->front  - pObj->back);

    // 判定
#if OBD_USE_RECT_CHECK_DIPTH
    if( (OBM_POINT_IN_LINE(lLeftest1, usWidth1, lLeftest2)) &&
        (OBM_POINT_IN_LINE(lTopest1, usHeight1,  lTopest2)) &&
        (OBM_POINT_IN_LINE(lBack1,   usDepth1,     lBack2)) ){
#else
    if( (OBM_POINT_IN_LINE(lLeftest1, usWidth1, lLeftest2)) &&
        (OBM_POINT_IN_LINE(lTopest1, usHeight1,  lTopest2)) ){
#endif
        return 1;
    }
    return 0;
}
// ================================================================
// ObjRectCenterX
/*!
  矩形の中心を返す
 
  @param pWork     [in] 対象矩形
 
  @return   中心X座標
 */
// ================================================================
fx32 ObjRectCenterX( OBS_RECT_WORK* pWork )
{
    s32 pos;
    
    //pos = ((pWork->rect.pos.x >> FX32_SHIFT) + ((pWork->rect.left + pWork->rect.right) >> 1));
    //pos <<= FX32_SHIFT;
	pos = pWork->rect.pos.x + (((pWork->rect.left + pWork->rect.right) >> 1) << FX32_SHIFT);
    if ( pWork->parent_obj ){
        pos += pWork->parent_obj->pos.x;
    }
    
    return pos;

}
// ================================================================
// ObjRectCenterY
/*!
  矩形の中心を返す
 
  @param pWork     [in] 対象矩形
 
  @return   中心Y座標
 */
// ================================================================
fx32 ObjRectCenterY( OBS_RECT_WORK* pWork )
{
    s32 pos;
    
    //pos = ((pWork->rect.pos.y >> FX32_SHIFT) + ((pWork->rect.top + pWork->rect.bottom) >> 1));
    //pos <<= FX32_SHIFT;
	pos = pWork->rect.pos.y + (((pWork->rect.top + pWork->rect.bottom) >> 1) << FX32_SHIFT);
    if ( pWork->parent_obj ){
        pos += pWork->parent_obj->pos.y;
    }
    
    return pos;
}
// ================================================================
// ObjRectCenterZ
/*!
  矩形の中心を返す
 
  @param pWork     [in] 対象矩形
 
  @return   中心Z座標
 */
// ================================================================
fx32 ObjRectCenterZ( OBS_RECT_WORK* pWork )
{
    s32 pos;
    
    //pos = ((pWork->rect.pos.z >> FX32_SHIFT) + ((pWork->rect.back + pWork->rect.front) >> 1));
    //pos <<= FX32_SHIFT;
    pos = pWork->rect.pos.z + (((pWork->rect.back + pWork->rect.front) >> 1) << FX32_SHIFT);
    if ( pWork->parent_obj ){
        pos += pWork->parent_obj->pos.z;
    }
    
    return pos;
}

// ================================================================
// ObjRectHitCenterX
/*!
  ヒットしあった矩形の中心を返す（ヒットしあってないものの場合は真中が帰ってくる）
 
  @param pWork     [in] 対象矩形その１
  @param pAttacker [in] 対象矩形その２
 
  @return   中心X座標 1:19:12
 */
// ================================================================
s32 ObjRectHitCenterX( OBS_RECT_WORK* pWork, OBS_RECT_WORK* pAttacker )
{
    s32 lPosMax = 0;
    s32 lPosMin = 0;
    s32 lPosAnswer = 0;
    s32 lPos[4];
    u16 usWidth;
    u8  i = 0;
    u8  j = 0;
    u8  k,l;

    // 位置を設定
    ObjRectLTBSet(pWork, &lPos[0], NULL, NULL );
    ObjRectWHDSet(pWork, &usWidth, NULL, NULL );
    lPos[1] = lPos[0] + usWidth;
    
    ObjRectLTBSet(pAttacker, &lPos[2], NULL, NULL );
    ObjRectWHDSet(pAttacker, &usWidth, NULL, NULL );
    lPos[3] = lPos[2] + usWidth;

    // 一番大きいものと小さいものを選択して削除
    lPosMax = lPos[i]; k = i; ++i;
    if ( lPos[i] > lPosMax ){ lPosMax = lPos[i]; k= i;} ++i;
    if ( lPos[i] > lPosMax ){ lPosMax = lPos[i]; k= i;} ++i;
    if ( lPos[i] > lPosMax ){ lPosMax = lPos[i]; k= i;} ++i;
                            
    i = 0;                  
    lPosMin = lPos[i]; l = i; ++i;
    if ( lPos[i] < lPosMin ){ lPosMin = lPos[i]; l = i;} ++i;
    if ( lPos[i] < lPosMin ){ lPosMin = lPos[i]; l = i;} ++i;
    if ( lPos[i] < lPosMin ){ lPosMin = lPos[i]; l = i;} ++i;

    for ( i = 0; ;++i){
        if ( i != k && i != l ){
            lPos[j] = lPos[i];
            if ( j )
                break;
            ++j;
        }
    }

    // 残った二つを引いて、更に半分にする
    lPosAnswer = MTM_MATH_ABS(((lPos[0] - lPos[1]) >> 1));
    if ( lPos[0] > lPos[1]){
        lPosAnswer += lPos[1];
    }else{
        lPosAnswer += lPos[0];
    }

    return (lPosAnswer << FX32_SHIFT);
}
// ================================================================
// ObjRectHitCenterY
/*!
  ヒットしあった矩形の中心を返す（ヒットしあってないものの場合は真中が帰ってくる）
 
  @param pWork     [in] 対象矩形その１
  @param pAttacker [in] 対象矩形その２
 
  @return   中心Y座標 1:19:12
 */
// ================================================================
s32 ObjRectHitCenterY( OBS_RECT_WORK* pWork, OBS_RECT_WORK* pAttacker )
{
    s32 lPosMax = 0;
    s32 lPosMin = 0;
    s32 lPosAnswer = 0;
    s32 lPos[4];
    u16 usHeight;
    u8  i = 0;
    u8  j = 0;
    u8  k,l;

    // 位置を設定
    ObjRectLTBSet(pWork, NULL, &lPos[0], NULL );
    ObjRectWHDSet(pWork, NULL, &usHeight, NULL );
    lPos[1] = lPos[0] + usHeight;
    
    ObjRectLTBSet(pAttacker, NULL, &lPos[2], NULL );
    ObjRectWHDSet(pAttacker, NULL, &usHeight, NULL );
    lPos[3] = lPos[2] + usHeight;

    // 一番大きいものと小さいものを選択して削除
    lPosMax = lPos[i]; k= i; ++i;
    if ( lPos[i] > lPosMax ){ lPosMax = lPos[i]; k= i;} ++i;
    if ( lPos[i] > lPosMax ){ lPosMax = lPos[i]; k= i;} ++i;
    if ( lPos[i] > lPosMax ){ lPosMax = lPos[i]; k= i;} ++i;

    i = 0;
    lPosMin = lPos[i]; l = i; ++i;
    if ( lPos[i] < lPosMin ){ lPosMin = lPos[i]; l = i;} ++i;
    if ( lPos[i] < lPosMin ){ lPosMin = lPos[i]; l = i;} ++i;
    if ( lPos[i] < lPosMin ){ lPosMin = lPos[i]; l = i;} ++i;

    for ( i = 0; ;++i){
        if ( i != k && i != l ){
            lPos[j] = lPos[i];
            if ( j )
                break;
            ++j;
        }
    }

    // 残った二つを引いて、更に半分にする

    lPosAnswer = MTM_MATH_ABS(((lPos[0] - lPos[1]) >> 1));
    if ( lPos[0] > lPos[1]){
        lPosAnswer += lPos[1];
    }else{
        lPosAnswer += lPos[0];
    }

    return (lPosAnswer << FX32_SHIFT);
}




#if defined(MTD_DEBUG)  // デバッグ版

#define LEFT   (0)
#define TOP    (1)
#define RIGHT  (2)
#define BOTTOM (3)
#define WIDTH  (2)
#define HEIGHT (3)

#if defined _DS
MTS_ACTION_DS g_obj_debug_rect_act;
#include "/debug/dbg_rect.h"
#endif



// ================================================================
// ObjDebugSetRectDispGroup
/*!
 *	矩形デバック表示で表示するグループの設定
 *
 *	@param group		[in] 表示するグループフラグ OBD_RECT_TARGET_G_FLAG_*
 *
 *	@note
 *		表示させたい矩形グループのフラグを設定します。
 *		この関数はマスター版ではOFFになります。
 */
// ================================================================
void ObjDebugSetRectDispGroup(u8 group)
{
	obj_debug_rect_disp_check_group = group;
}

// ================================================================
// ObjDebugRectActionInit
/*!
  デバッグ用レクト表示アクション初期化
 */
// ================================================================
void ObjDebugRectActionInit()
{
#if defined _DS
    void * pBac = NULL;
    u32 addr_A = 0, addr_B = 0;
    u32 size;
    u32 flag_ds = 0;
    // ファイル読み込み
    pBac = mtFsLoadFile( "/debug/dbg_rect.bac", MTD_FS_DEST_AUTO_ALLOC_HEAD );

    switch( g_obj.vram_map_mode ){
    case OBD_OBJ_VRAM_MMODE_32:
        size = mtActGetChaName32MaxFromBac( pBac );
        break;
    default:
    case OBD_OBJ_VRAM_MMODE_64:
        size = mtActGetChaName64MaxFromBac( pBac );
        break;
    case OBD_OBJ_VRAM_MMODE_128:
        size = mtActGetChaName128MaxFromBac( pBac );
        break;
    case OBD_OBJ_VRAM_MMODE_256:
        size = mtActGetChaName256MaxFromBac( pBac );
        break;
    }
    
    if ( !(g_obj.flag & OBD_OBJ_RECT_D_NOVRAM_A) ){
         addr_A = (u32)mtVramAllocObj(MTE_GE2_A, size);
    }else{
        flag_ds |=MTD_ACT_DS_FLAG_DISABLE_GE_A ;
    }
    if ( !(g_obj.flag & OBD_OBJ_RECT_D_NOVRAM_B) ){
         addr_B = (u32)mtVramAllocObj(MTE_GE2_B, size);
    }else{
        flag_ds |= MTD_ACT_DS_FLAG_DISABLE_GE_B;
    }
         
    // アクション初期化
    mtActInitStructDS(
        &g_obj_debug_rect_act, pBac, 0, flag_ds, MTD_ACT_FLAG_CLIP | MTD_ACT_FLAG_NO_PLT,
        MTE_CHA_VRAM_ADDRESS, addr_A,
        MTE_PLT_VRAM_ADDRESS, HW_OBJ_PLTT,
        MTE_CHA_VRAM_ADDRESS, addr_B,
        MTE_PLT_VRAM_ADDRESS, HW_DB_OBJ_PLTT, 0, 0 );
	g_obj_debug_rect_act.plt_ofst_no[MTE_GE2_A] = 15;

    mtActUpdateDS( &g_obj_debug_rect_act, NULL, NULL );
#endif	// #if defined _DS
}
// ================================================================
// ObjDebugRectActionExit
/*!
  デバッグ用レクト表示アクション解放
 */
// ================================================================
void ObjDebugRectActionExit()
{
#if defined _DS
    if ( NULL != g_obj_debug_rect_act.act.bac_addr )
    {
        mtMemFreeMain((void*)g_obj_debug_rect_act.act.bac_addr);
        g_obj_debug_rect_act.act.bac_addr    = NULL;
        if ( g_obj_debug_rect_act.cha_addr[0] )
            mtVramFreeObj( MTE_GE2_A, g_obj_debug_rect_act.cha_addr[0]  );
        if ( g_obj_debug_rect_act.cha_addr[1] )
            mtVramFreeObj( MTE_GE2_B, g_obj_debug_rect_act.cha_addr[1]  );
    }
    g_obj_debug_rect_act.flag = 0;
#endif	// #if defined _DS
}
// ================================================================
// ObjDebugRectDispAll
/*!
  登録した矩形を全てデバッグ表示する
 */
// ================================================================
void ObjDebugRectDispAll()
{
#if defined _DS
    s16 i;
    OBS_RECT_WORK* pRec = NULL;
    MTS_ACTION_DS* pAct = &g_obj_debug_rect_act;

    // 未初期化チェック
    if ( NULL == g_obj_debug_rect_act.act.bac_addr )
        return;
    if (!( g_obj.flag & (OBD_OBJ_RECTF_D | OBD_OBJ_RECT_D)))
        return;

    // 処理無視チェック
    pAct->flag &= ~(MTD_ACT_DS_FLAG_DISABLE_GE_A | MTD_ACT_DS_FLAG_DISABLE_GE_B);
    if ( !(g_obj.flag & OBD_OBJ_VRAM_AB) ){
        if ( g_obj.flag & OBD_OBJ_VRAM_B ){
            pAct->flag |= MTD_ACT_DS_FLAG_DISABLE_GE_A;
        }else{
            pAct->flag |= MTD_ACT_DS_FLAG_DISABLE_GE_B;
        }
    }
    
    // 3D用矩形を表示するためにカメラを設定する
    if ( g_obj.flag & OBD_OBJ_FULL3D ){

        MtxFx43     vt;
        MtxFx44     pt;

        MTX_LookAt(
            &g_obj.camera3d->pos,
            &g_obj.camera3d->up,
            &g_obj.camera3d->at,
            &vt );

        MTX_PerspectiveW(
            mtMathSin( g_obj.camera3d->persp.fov ),
            mtMathCos( g_obj.camera3d->persp.fov ),
            g_obj.camera3d->persp.aspect,
            g_obj.camera3d->persp.near,
            g_obj.camera3d->persp.far,
            g_obj.camera3d->persp.scale_w,
            &pt );

        NNS_G3dGeMtxMode( GX_MTXMODE_PROJECTION );
        NNS_G3dGeLoadMtx44( &pt );
        NNS_G3dGeMultMtx43( &vt );
    }

    // 矩形
    if ( g_obj.flag & OBD_OBJ_RECT_D ) {
	    for ( i = 0; ; ++i ){

	        // 拡張矩形を順に取得
	        pRec = ObjRectRegistGet(obj_debug_rect_disp_check_group, i);

	        // 終了
	        if ( pRec == NULL )
	            break;

	        // デバッグ表示
	        ObjDebugRectExDisp(pRec);

	    }
    }

    // 地形判定矩形
    if ( g_obj.flag & OBD_OBJ_RECTF_D &&
                g_obj.flag & OBD_OBJ_COLMAP) {
        OBS_OBJECT_WORK	* search_obj_work;

        search_obj_work = ObjObjectSearchRegistObject(NULL, 0xFFFF);
        while (search_obj_work) {
            ObjDebugRectDispObject(search_obj_work);

            search_obj_work = ObjObjectSearchRegistObject(search_obj_work, 0xFFFF);
        }
    }
#else

	s16				i, col_no;
	OBS_RECT_WORK	*pRec = NULL;

    if (!( g_obj.flag & (OBD_OBJ_RECTF_D | OBD_OBJ_RECT_D)))
        return;

    // 矩形
	//nnBeginDrawPrimitiveLine2D(&col, NNE_PRIM_ALPHABLEND_OFF);
	if (g_obj.flag & OBD_OBJ_RECT_D) {
		for (i = 0; ;i++) {
			// 拡張矩形を順に取得
			pRec = ObjRectRegistGet(obj_debug_rect_disp_check_group, i);

			// 終了
			if (pRec == NULL) {
	            break;
			}

			if (pRec->hit_flag & OBD_HIT_BODY ||
					!(pRec->def_flag & OBD_HIT_BODY)) {
				// 体あたり
				col_no = 15;
			}
			else if (pRec->hit_flag & OBD_HIT_NORMAL) {
				// 攻撃
				col_no = 8;
			}
			else if (!(pRec->def_flag & OBD_HIT_NORMAL)) {
				// くらい
				col_no = 10;
			}
			else {
				// その他
				col_no = 3;
			}

			// デバッグ表示
			ObjDebugRectExDisp(pRec,
					obj_debug_rect_draw_col[col_no].a,
					obj_debug_rect_draw_col[col_no].r,
					obj_debug_rect_draw_col[col_no].g,
					obj_debug_rect_draw_col[col_no].b);
		}
	}
	//nnEndDrawPrimitiveLine2D();

	// 地形判定矩形
	//nnBeginDrawPrimitiveLine2D(&col, NNE_PRIM_ALPHABLEND_OFF);
	if (g_obj.flag & OBD_OBJ_RECTF_D &&
				g_obj.flag & OBD_OBJ_COLMAP) {
		OBS_OBJECT_WORK	* search_obj_work;

		search_obj_work = ObjObjectSearchRegistObject(NULL, 0xFFFF);
		while (search_obj_work) {
			ObjDebugRectDispObject(search_obj_work, 1.f, 1.f, 1.f, 1.f);

			search_obj_work = ObjObjectSearchRegistObject(search_obj_work, 0xFFFF);
		}
	}
	//nnEndDrawPrimitiveLine2D();


#endif	// #if defined _DS
}

// ================================================================
// ObjDebugRectDispObject
/*!
  指定位置にデバッグ地形判定矩形を表示する （リリース時、空マクロになります

  @param pObj [in] オブジェクトポインタ
 */
// ================================================================
#if defined _DS
void ObjDebugRectDispObject( OBS_OBJECT_WORK *pObj )
{
    MTS_ACTION_DS* pAct = &g_obj_debug_rect_act;
    u32 ulObjFlagBackUp = g_obj.flag;
    
    if (!( g_obj.flag & OBD_OBJ_RECTF_D ) ||
    		((pObj)->move_flag & OBD_MOVE_NOCOL)) {
        return;
	}

    pAct->act.flag |= MTD_ACT_FLAG_FLIP_V;
    pAct->act.flag |= MTD_ACT_FLAG_FLIP_H;
    
    g_obj.flag |= OBD_OBJ_RECT_D;
    
    ObjDebugRectPointDisp( (pObj)->pos.x, (pObj)->pos.y, (pObj)->field_rect[0], (pObj)->field_rect[1], (pObj)->field_rect[2], (pObj)->field_rect[3]);

    pAct->act.flag &= ~MTD_ACT_FLAG_FLIP_V;
    pAct->act.flag &= ~MTD_ACT_FLAG_FLIP_H;

    g_obj.flag = ulObjFlagBackUp;
}
#else
void ObjDebugRectDispObject( OBS_OBJECT_WORK *pObj, float a, float r, float g, float b )
{
	OBS_RECT_WORK		rect;

    if (!( g_obj.flag & OBD_OBJ_RECTF_D ) ||
    	(	((pObj)->move_flag & OBD_MOVE_NOCOL)
    	 &&(!((pObj)->col_work && (pObj)->col_work->obj_col.obj)) ) ) {
        return;
	}

	if (pObj->field_rect[MTD_LEFT]
		| pObj->field_rect[MTD_TOP]
		| pObj->field_rect[MTD_RIGHT]
		| pObj->field_rect[MTD_BOTTOM]) {

		rect.rect.left		= pObj->field_rect[MTD_LEFT];
		rect.rect.top		= pObj->field_rect[MTD_TOP];
		rect.rect.right		= pObj->field_rect[MTD_RIGHT];
		rect.rect.bottom	= pObj->field_rect[MTD_BOTTOM];

	}
	else if ((pObj)->col_work == NULL) {
		// 描画しない
		return;
	}
	else {

		rect.rect.left		= (pObj)->col_work->obj_col.ofst_x;
		rect.rect.top		= (pObj)->col_work->obj_col.ofst_y;
		rect.rect.right		= (s16)((pObj)->col_work->obj_col.ofst_x + (pObj)->col_work->obj_col.width);
		rect.rect.bottom	= (s16)((pObj)->col_work->obj_col.ofst_y + (pObj)->col_work->obj_col.height);
		r = 0.f;
		g = 1.f;
		b = 1.f;
	}
	rect.rect.pos.x = rect.rect.pos.y = rect.rect.pos.z = 0;

	rect.parent_obj = pObj;
	rect.flag = OBD_RECT_ENABLE;

	ObjDebugRectExDisp(&rect, a, r, g, b);
}
#endif	// #if defined _DS

// ================================================================
// ObjDebugRectDisp
/*!
  指定矩形を表示する

  @param pRec [in] 矩形ポインタ
 */
// ================================================================
void ObjDebugRectDisp( OBS_RECT * pRec )
{
#if defined _DS
    s16 sL;
    s16 sT;
    s16 sR;
    s16 sB;
    s16 sFr;
    s16 sBk;

    sL = pRec->left;
    sR = pRec->right;
    sT = pRec->top;
    sB = pRec->bottom;
    sFr = pRec->front;
    sBk = pRec->back;

    if ( g_obj.flag & OBD_OBJ_FULL3D ){
        ObjDebugRectPointDisp3D( pRec->pos.x,
                                 pRec->pos.y,
                                 pRec->pos.z,
                                 sL, sT, sR, sB, sFr, sBk );
    }else{
        ObjDebugRectPointDisp( pRec->pos.x,
                               pRec->pos.y,
                                   sL, sT, sR, sB);
        
    }
#else
	UNREFERENCED_PARAMETER(pRec);
#endif	// #if defined _DS
}
// ================================================================
// ObjDebugRectExDisp
/*!
  指定矩形を表示する
    
  @param pRec [in] 拡張矩形ポインタ
 */
// ================================================================
#if defined _DS
void ObjDebugRectExDisp(OBS_RECT_WORK * pRec )
{
    s16 sL;
    s16 sT;
    s16 sR;
    s16 sB;
    s16 sFr;
    s16 sBk;
    VecFx32 vPos;
    u16 usFlag = 0;
    if ( !(pRec->flag & OBD_RECT_ENABLE) )
        return;
    if ( (pRec->flag & OBD_RECT_NOHIT) )
        return;

    // 反転チェック
    if ( pRec->parent_obj && !(pRec->flag & OBD_RECT_NOPOS) ){
        // 親の位置から設定する
        if ( pRec->parent_obj->disp_flag & OBD_DISP_HFLIP )
            usFlag |= OBD_RECT_HFLIP;
        if ( pRec->parent_obj->disp_flag & OBD_DISP_VFLIP )
            usFlag |= OBD_RECT_VFLIP;
    }else{
        if ( pRec->flag & OBD_RECT_HFLIP )
            usFlag |= OBD_RECT_HFLIP;
        if ( pRec->flag & OBD_RECT_VFLIP )
            usFlag |= OBD_RECT_VFLIP;
    }

    // 上下左右位置を取得
    if ( usFlag & OBD_RECT_HFLIP ){
        sL = (s16)-pRec->rect.right;
        sR = (s16)-pRec->rect.left;
    }else{
        sL = pRec->rect.left;
        sR = pRec->rect.right;
    }
    if ( usFlag & OBD_RECT_VFLIP ){
        sT = (s16)-pRec->rect.bottom;
        sB = (s16)-pRec->rect.top;
    }else{
        sT = pRec->rect.top;
        sB = pRec->rect.bottom;
    }
    sFr = pRec->rect.front;
    sBk = pRec->rect.back;

    // 座標を取得
    ObjRectPosGet( &vPos, pRec );

    // スケーリング
    if ( pRec->parent_obj && !(pRec->flag & OBD_RECT_NOPOS) ){
        // 拡大率チェック

#if 1
		fx32	temp_scale;

		temp_scale = FX32_ONE;
        if ( pRec->parent_obj->scale.x != FX32_ONE ) {
        	temp_scale = pRec->parent_obj->scale.x;
        }
		if ( g_obj.draw_scale.x != FX32_ONE && !(pRec->parent_obj->disp_flag & OBD_DISP_NODRAWSCALE) &&
				!(g_obj.flag & OBD_OBJ_RECT_NOUSE_DRAWSCALE)) {
			temp_scale = FX_Mul(temp_scale, g_obj.draw_scale.x);
		}
		if (temp_scale != FX32_ONE) {
            sL = (s16)(FX_Mul( sL << FX32_SHIFT, temp_scale) >> FX32_SHIFT);
            sR = (s16)(FX_Mul( sR << FX32_SHIFT, temp_scale) >> FX32_SHIFT);
		}

		temp_scale = FX32_ONE;
        if ( pRec->parent_obj->scale.y != FX32_ONE ) {
        	temp_scale = pRec->parent_obj->scale.y;
        }
		if ( g_obj.draw_scale.y != FX32_ONE && !(pRec->parent_obj->disp_flag & OBD_DISP_NODRAWSCALE) &&
				!(g_obj.flag & OBD_OBJ_RECT_NOUSE_DRAWSCALE)) {
			temp_scale = FX_Mul(temp_scale, g_obj.draw_scale.y);
		}
		if (temp_scale != FX32_ONE) {
            sT = (s16)(FX_Mul( sT << FX32_SHIFT, temp_scale) >> FX32_SHIFT);
            sB = (s16)(FX_Mul( sB << FX32_SHIFT, temp_scale) >> FX32_SHIFT);
		}

		temp_scale = FX32_ONE;
        if ( pRec->parent_obj->scale.z != FX32_ONE ) {
        	temp_scale = pRec->parent_obj->scale.z;
        }
		if ( g_obj.draw_scale.z != FX32_ONE && !(pRec->parent_obj->disp_flag & OBD_DISP_NODRAWSCALE) &&
				!(g_obj.flag & OBD_OBJ_RECT_NOUSE_DRAWSCALE)) {
			temp_scale = FX_Mul(temp_scale, g_obj.draw_scale.z);
		}
		if (temp_scale != FX32_ONE) {
            sFr = (s16)(FX_Mul( sFr << FX32_SHIFT, temp_scale) >> FX32_SHIFT);
            sBk = (s16)(FX_Mul( sBk << FX32_SHIFT, temp_scale) >> FX32_SHIFT);
		}
#else
        if ( pRec->parent_obj->scale.x != FX32_ONE ){
            sL = (s16)(FX_Mul( sL << FX32_SHIFT, pRec->parent_obj->scale.x) >> FX32_SHIFT);
            sR = (s16)(FX_Mul( sR << FX32_SHIFT, pRec->parent_obj->scale.x) >> FX32_SHIFT);
        }
        if ( pRec->parent_obj->scale.y != FX32_ONE ){
            sT = (s16)(FX_Mul( sT << FX32_SHIFT, pRec->parent_obj->scale.y) >> FX32_SHIFT);
            sB = (s16)(FX_Mul( sB << FX32_SHIFT, pRec->parent_obj->scale.y) >> FX32_SHIFT);
        }
        if ( pRec->parent_obj->scale.z != FX32_ONE ){
            sFr = (s16)(FX_Mul( sFr << FX32_SHIFT, pRec->parent_obj->scale.z) >> FX32_SHIFT);
            sBk = (s16)(FX_Mul( sBk << FX32_SHIFT, pRec->parent_obj->scale.z) >> FX32_SHIFT);
        }
#endif
    }
    
    // 表示
    if ( g_obj.flag & OBD_OBJ_FULL3D ){
        ObjDebugRectPointDisp3D( vPos.x, vPos.y, vPos.z, sL, sT, sR, sB, sFr, sBk);
    }else{
        ObjDebugRectPointDisp( vPos.x, vPos.y, sL, sT, sR, sB);
    }

}
#else
void ObjDebugRectExDisp(OBS_RECT_WORK * pRec, float a, float r, float g, float b)
{
	OBS_DEBUG_RECT_DT_WORK	dt_work;
	u16				flag = 0;
	VecFx32			obj_pos;
	NNS_VECTOR		pos;

    s16 sL, sT, sR, sB, sBk, sFr;

    if ( !(pRec->flag & OBD_RECT_ENABLE) )
        return;
    if ( (pRec->flag & OBD_RECT_NOHIT) )
        return;

    // 反転チェック
    if ( pRec->parent_obj && !(pRec->flag & OBD_RECT_NOPOS) ){
        // 親の位置から設定する
        if ( pRec->parent_obj->disp_flag & OBD_DISP_HFLIP )
            flag |= OBD_RECT_HFLIP;
        if ( pRec->parent_obj->disp_flag & OBD_DISP_VFLIP )
            flag |= OBD_RECT_VFLIP;
    }
	else {
        if ( pRec->flag & OBD_RECT_HFLIP )
            flag |= OBD_RECT_HFLIP;
        if ( pRec->flag & OBD_RECT_VFLIP )
            flag |= OBD_RECT_VFLIP;
    }

    // 座標を取得
	ObjRectPosGet(&obj_pos, pRec);

    // 上下左右位置を取得
    if ( flag & OBD_RECT_HFLIP ){
        sL = (s16)-pRec->rect.right;
        sR = (s16)-pRec->rect.left;
    }
	else {
        sL = pRec->rect.left;
        sR = pRec->rect.right;
    }
    if ( flag & OBD_RECT_VFLIP ){
        sT = (s16)-pRec->rect.bottom;
        sB = (s16)-pRec->rect.top;
    }
	else {
        sT = pRec->rect.top;
        sB = pRec->rect.bottom;
    }
    sFr = pRec->rect.front;
    sBk = pRec->rect.back;

    // スケーリング
    if ( pRec->parent_obj && !(pRec->flag & OBD_RECT_NOPOS) ){
        // 拡大率チェック
		fx32	temp_scale;

		temp_scale = FX32_ONE;
        if ( pRec->parent_obj->scale.x != FX32_ONE ) {
        	temp_scale = pRec->parent_obj->scale.x;
        }
		if ( g_obj.draw_scale.x != FX32_ONE && !(pRec->parent_obj->disp_flag & OBD_DISP_NODRAWSCALE) &&
				!(g_obj.flag & OBD_OBJ_RECT_NOUSE_DRAWSCALE)) {
			temp_scale = FX_Mul(temp_scale, g_obj.draw_scale.x);
		}
		if (temp_scale != FX32_ONE) {
            sL = (s16)(FX_Mul( sL << FX32_SHIFT, temp_scale) >> FX32_SHIFT);
            sR = (s16)(FX_Mul( sR << FX32_SHIFT, temp_scale) >> FX32_SHIFT);
		}

		temp_scale = FX32_ONE;
        if ( pRec->parent_obj->scale.y != FX32_ONE ) {
        	temp_scale = pRec->parent_obj->scale.y;
        }
		if ( g_obj.draw_scale.y != FX32_ONE && !(pRec->parent_obj->disp_flag & OBD_DISP_NODRAWSCALE) &&
				!(g_obj.flag & OBD_OBJ_RECT_NOUSE_DRAWSCALE)) {
			temp_scale = FX_Mul(temp_scale, g_obj.draw_scale.y);
		}
		if (temp_scale != FX32_ONE) {
            sT = (s16)(FX_Mul( sT << FX32_SHIFT, temp_scale) >> FX32_SHIFT);
            sB = (s16)(FX_Mul( sB << FX32_SHIFT, temp_scale) >> FX32_SHIFT);
		}

		temp_scale = FX32_ONE;
        if ( pRec->parent_obj->scale.z != FX32_ONE ) {
        	temp_scale = pRec->parent_obj->scale.z;
        }
		if ( g_obj.draw_scale.z != FX32_ONE && !(pRec->parent_obj->disp_flag & OBD_DISP_NODRAWSCALE) &&
				!(g_obj.flag & OBD_OBJ_RECT_NOUSE_DRAWSCALE)) {
			temp_scale = FX_Mul(temp_scale, g_obj.draw_scale.z);
		}
		if (temp_scale != FX32_ONE) {
            sFr = (s16)(FX_Mul( sFr << FX32_SHIFT, temp_scale) >> FX32_SHIFT);
            sBk = (s16)(FX_Mul( sBk << FX32_SHIFT, temp_scale) >> FX32_SHIFT);
		}
    }

	// カメラ位置から座標調整
	pos.x = FXM_FX32_TO_FLOAT(obj_pos.x);
	pos.y = FXM_FX32_TO_FLOAT(-obj_pos.y);
#if !_PS3
	pos.z = FXM_FX32_TO_FLOAT(obj_pos.z) + 30.f;
#else
	pos.z = FXM_FX32_TO_FLOAT(obj_pos.z) - 30.f;
#endif

	dt_work.vtx[0].Pos.x = pos.x + sL;
	dt_work.vtx[0].Pos.y = pos.y - sT;
	dt_work.vtx[0].Pos.z = pos.z;

	dt_work.vtx[1].Pos.x = pos.x + sR;
	dt_work.vtx[1].Pos.y = pos.y - sT;
	dt_work.vtx[1].Pos.z = pos.z;

	dt_work.vtx[2].Pos.x = pos.x + sR;
	dt_work.vtx[2].Pos.y = pos.y - sB;
	dt_work.vtx[2].Pos.z = pos.z;

	dt_work.vtx[3].Pos.x = pos.x + sL;
	dt_work.vtx[3].Pos.y = pos.y - sB;
	dt_work.vtx[3].Pos.z = pos.z;

	dt_work.vtx[4].Pos.x = pos.x + sL;
	dt_work.vtx[4].Pos.y = pos.y - sT;
	dt_work.vtx[4].Pos.z = pos.z;

	dt_work.col.a = a;
	dt_work.col.r = r;
	dt_work.col.g = g;
	dt_work.col.b = b;
    

	nnCopyMatrix(&dt_work.mtx, amMatrixGetCurrent());		// ◆とりあえずカレントマトリクス

    // 表示
	ObjDraw3DNNUserFunc(objDebugRectExDisp_DT, &dt_work, sizeof(OBS_DEBUG_RECT_DT_WORK), OBD_DRAW_CMD_STATE_3DNN);

}
#endif	// #if defined _DS

// ================================================================
// objDebugRectExDisp_DT
/*!
  描画スレッド内 矩形描画

  @param lX [in] X座標
  @param lY [in] Y座標
  @param sLeft    [in] 左オフセット
  @param sTop     [in] 上オフセット
  @param sRight   [in] 右オフセット
  @param sBottom  [in] 下オフセット
 */
// ================================================================
void objDebugRectExDisp_DT(void *param)
{
	OBS_DEBUG_RECT_DT_WORK	*dt_work = (OBS_DEBUG_RECT_DT_WORK*)param;

    // 表示
	NNS_MATRIX						base_mtx;

	amMatrixPush();

	nnMultiplyMatrix(&base_mtx, amMatrixGetCurrent(), &dt_work->mtx);
	nnMultiplyMatrix(&base_mtx, &_am_draw_world_view_matrix, &base_mtx);
	nnSetPrimitive3DMatrix(&base_mtx);

#if _PC | _XBOX
	nnSetPrimitive3DAlphaTestDXG20(NNE_FALSE);
	nnSetPrimitive3DDepthMaskDXG20(NNE_TRUE);
	nnSetPrimitive3DDepthTestDXG20(NNE_TRUE);
	nnSetPrimitive3DDepthFuncDXG20(NNE_CMPFUNC_LESS);
#elif _PS3
    nnSetPrimitive3DAlphaFuncPS3(NND_CMPFUNC_PS3_ALWAYS, 1.f);
	nnSetPrimitive3DDepthMaskPS3(NNE_TRUE);
	nnSetPrimitive3DDepthFuncPS3(NND_CMPFUNC_PS3_ALWAYS);//NND_CMPFUNC_PS3_LESS);
#endif

	nnBeginDrawPrimitiveLine3D(&dt_work->col, NNE_PRIM_ALPHABLEND_OFF);
	nnDrawPrimitiveLine3D(NNE_PRIM_LINE_STRIP, dt_work->vtx, 5);
	nnEndDrawPrimitiveLine3D();
	amMatrixPop();
}

// ================================================================
// ObjDebugRectPointDisp
/*!
  指定位置にデバッグ矩形を表示する

  @param lX [in] X座標
  @param lY [in] Y座標
  @param sLeft    [in] 左オフセット
  @param sTop     [in] 上オフセット
  @param sRight   [in] 右オフセット
  @param sBottom  [in] 下オフセット
 */
// ================================================================
void ObjDebugRectPointDisp( fx32 lX, fx32 lY, s16 sLeft, s16 sTop, s16 sRight, s16 sBottom)
{
#if defined _DS
    MTS_ACTION_DS* pAct = &g_obj_debug_rect_act;

    s16 xposi[2],yposi[2];
    s16 rect[4];
    u16 i;
    // 未初期化チェック
    if ( NULL == g_obj_debug_rect_act.act.bac_addr )
        return;

    if (!( g_obj.flag & OBD_OBJ_RECT_D ))
        return;
    
    rect[0] = sLeft;
    rect[1] = sTop;
    rect[2] = sRight;
    rect[3] = sBottom;
    
    for ( i = 0; i < 2; ++i ){
        // 表示位置計算
        if ( g_obj.flag & OBD_OBJ_CAMERA ){
            xposi[i] = (s16)( (lX >> FX32_SHIFT) - (g_obj.camera[i][0] >> FX32_SHIFT));
            yposi[i] = (s16)( (lY >> FX32_SHIFT) - (g_obj.camera[i][1] >> FX32_SHIFT));
        }else{
            xposi[i] = (s16)(lX >> FX32_SHIFT);
            yposi[i] = (s16)(lY >> FX32_SHIFT);
        }
    //    pAct->plt_ofst_no[i] = 0;
    }
    // 左上
    for ( i = 0; i < 2; ++i ){
        pAct->pos[i][0] = (s16)(xposi[i] + rect[LEFT]);
        pAct->pos[i][1] = (s16)(yposi[i] + rect[TOP]);
    }
    mtActDrawDS (pAct);

    // 右上
    for ( i = 0; i < 2; ++i ){
        pAct->pos[i][0] = (s16)(xposi[i] + rect[RIGHT]);
        pAct->pos[i][1] = (s16)(yposi[i] + rect[TOP]);
    }
    pAct->act.flag ^= MTD_ACT_FLAG_FLIP_H;
    mtActDrawDS (pAct);

    // 右下
    for ( i = 0; i < 2; ++i ){
        pAct->pos[i][0] = (s16)(xposi[i] + rect[RIGHT]);
        pAct->pos[i][1] = (s16)(yposi[i] + rect[BOTTOM]);
    }
    pAct->act.flag ^= MTD_ACT_FLAG_FLIP_V;
    mtActDrawDS (pAct);

    // 左下
    for ( i = 0; i < 2; ++i ){
        pAct->pos[i][0] = (s16)(xposi[i] + rect[LEFT]);
        pAct->pos[i][1] = (s16)(yposi[i] + rect[BOTTOM]);
    }
    pAct->act.flag ^= MTD_ACT_FLAG_FLIP_H;
    mtActDrawDS (pAct);

    // フラグ戻す
    pAct->act.flag ^= MTD_ACT_FLAG_FLIP_V;
#else
	UNREFERENCED_PARAMETER(lX);
	UNREFERENCED_PARAMETER(lY);
	UNREFERENCED_PARAMETER(sLeft);
	UNREFERENCED_PARAMETER(sTop);
	UNREFERENCED_PARAMETER(sRight);
	UNREFERENCED_PARAMETER(sBottom);
#endif	// #if defined _DS 
}
// ================================================================
// ObjDebugPointDisp
/*!
  指定位置にデバッグ座標を表示する

  @param lX [in] X座標
  @param lY [in] Y座標
 */
// ================================================================
void ObjDebugPointDisp( s32 lX, s32 lY )
{
#if defined _DS
    MTS_ACTION_DS* pAct = &g_obj_debug_rect_act;

    s16 xposi[2],yposi[2];
    u16 i;

    // 未初期化チェック
    if ( NULL == g_obj_debug_rect_act.act.bac_addr )
        return;

    if (!( g_obj.flag & OBD_OBJ_RECT_D ))
        return;
    
    for ( i = 0; i < 2; ++i ){
        // 表示位置計算
        if ( g_obj.flag & OBD_OBJ_CAMERA ){
            xposi[i] = (s16)( (lX >> FX32_SHIFT) - (g_obj.camera[i][0] >> FX32_SHIFT));
            yposi[i] = (s16)( (lY >> FX32_SHIFT) - (g_obj.camera[i][1] >> FX32_SHIFT));
        }else{
            xposi[i] = (s16)(lX >> FX32_SHIFT);
            yposi[i] = (s16)(lY >> FX32_SHIFT);
        }
    //    pAct->plt_ofst_no[i] = 0;
    }
    // 左上
    for ( i = 0; i < 2; ++i ){
        pAct->pos[i][0] = (s16)(xposi[i]);
        pAct->pos[i][1] = (s16)(yposi[i]);
    }
    mtActDrawDS (pAct);

    // 右下
    for ( i = 0; i < 2; ++i ){
        pAct->pos[i][0] = (s16)(xposi[i] +1);
        pAct->pos[i][1] = (s16)(yposi[i] +1);
    }
    pAct->act.flag ^= MTD_ACT_FLAG_FLIP_H;
    pAct->act.flag ^= MTD_ACT_FLAG_FLIP_V;
    mtActDrawDS (pAct);


    // フラグ戻す
    pAct->act.flag ^= MTD_ACT_FLAG_FLIP_H;
    pAct->act.flag ^= MTD_ACT_FLAG_FLIP_V;
#else
	UNREFERENCED_PARAMETER(lX);
	UNREFERENCED_PARAMETER(lY);
#endif	// #if defined _DS
}
// ================================================================
// ObjDebugRectPointDisp3D
/*!
  指定位置に3Dでデバッグ矩形を表示する

  @param lX [in] X座標
  @param lY [in] Y座標
  @param lZ [in] Y座標
  @param sLeft    [in] 左オフセット
  @param sTop     [in] 上オフセット
  @param sRight   [in] 右オフセット
  @param sBottom  [in] 下オフセット
  @param sFront   [in] 前オフセット
  @param sBack    [in] 奥オフセット
 */
// ================================================================
void ObjDebugRectPointDisp3D( fx32 lX, fx32 lY, fx32 lZ, s16 sLeft, s16 sTop, s16 sRight, s16 sBottom, s16 sFront, s16 sBack)
{
#if defined _DS
    if (!( g_obj.flag & OBD_OBJ_RECT_D ))
        return;

    {
        fx32    x   =  ( ( lX ) + ( sLeft << FX32_SHIFT ) );
        fx32    y   = -( ( lY ) + ( sTop  << FX32_SHIFT ) );
        fx32    z   =  ( ( lZ ) + ( sBack << FX32_SHIFT ) );

        mtUtilFlushOnlyVPG3dGlb();
        NNS_G3dGeMtxMode( GX_MTXMODE_POSITION );
        NNS_G3dGeIdentity();
        NNS_G3dGeTranslate( x, y, z );
        NNS_G3dGeScale(
            FX32_ONE * ( sRight - sLeft ),
            FX32_ONE * ( sBottom - sTop ),
            FX32_ONE * ( sFront - sBack ) );

        NNS_G3dGeTexImageParam(
            GX_TEXFMT_NONE,
            GX_TEXGEN_NONE,
            GX_TEXSIZE_S8, GX_TEXSIZE_T8,
            GX_TEXREPEAT_NONE,
            GX_TEXFLIP_NONE,
            GX_TEXPLTTCOLOR0_TRNS,
            0 );
        NNS_G3dGePolygonAttr( 0, GX_POLYGONMODE_MODULATE, GX_CULL_NONE, 0, 0, GX_POLYGON_ATTR_MISC_NONE );
        NNS_G3dGeColor( GX_RGB( 0, 31, 0 ) );

        // 側面
        NNS_G3dGeBegin( GX_BEGIN_QUAD_STRIP );
            NNS_G3dGeVtx10(  0 * FX16_ONE,  0 * FX16_ONE, +1 * FX16_ONE );
            NNS_G3dGeVtx10(  0 * FX16_ONE, -1 * FX16_ONE, +1 * FX16_ONE );

            NNS_G3dGeVtx10( +1 * FX16_ONE,  0 * FX16_ONE, +1 * FX16_ONE );
            NNS_G3dGeVtx10( +1 * FX16_ONE, -1 * FX16_ONE, +1 * FX16_ONE );

            NNS_G3dGeVtx10( +1 * FX16_ONE,  0 * FX16_ONE,  0 * FX16_ONE );
            NNS_G3dGeVtx10( +1 * FX16_ONE, -1 * FX16_ONE,  0 * FX16_ONE );

            NNS_G3dGeVtx10(  0 * FX16_ONE,  0 * FX16_ONE,  0 * FX16_ONE );
            NNS_G3dGeVtx10(  0 * FX16_ONE, -1 * FX16_ONE,  0 * FX16_ONE );

            NNS_G3dGeVtx10(  0 * FX16_ONE,  0 * FX16_ONE, +1 * FX16_ONE );
            NNS_G3dGeVtx10(  0 * FX16_ONE, -1 * FX16_ONE, +1 * FX16_ONE );
        NNS_G3dGeEnd();

        // 上下面
        NNS_G3dGeBegin( GX_BEGIN_QUADS );
            // 上
            NNS_G3dGeVtx10(  0 * FX16_ONE,  0 * FX16_ONE, +1 * FX16_ONE );
            NNS_G3dGeVtx10( +1 * FX16_ONE,  0 * FX16_ONE, +1 * FX16_ONE );
            NNS_G3dGeVtx10( +1 * FX16_ONE,  0 * FX16_ONE,  0 * FX16_ONE );
            NNS_G3dGeVtx10(  0 * FX16_ONE,  0 * FX16_ONE,  0 * FX16_ONE );
            // 下
            NNS_G3dGeVtx10(  0 * FX16_ONE,  0 * FX16_ONE, +1 * FX16_ONE );
            NNS_G3dGeVtx10(  0 * FX16_ONE,  0 * FX16_ONE,  0 * FX16_ONE );
            NNS_G3dGeVtx10( +1 * FX16_ONE,  0 * FX16_ONE,  0 * FX16_ONE );
            NNS_G3dGeVtx10( +1 * FX16_ONE,  0 * FX16_ONE, +1 * FX16_ONE );
        NNS_G3dGeEnd();
/*
        NNS_G3dGeBegin( GX_BEGIN_QUADS );
            NNS_G3dGeVtx10(  0 * FX16_ONE,  0 * FX16_ONE, 0 * FX16_ONE );
            NNS_G3dGeVtx10(  0 * FX16_ONE, -1 * FX16_ONE, 0 * FX16_ONE );
            NNS_G3dGeVtx10( +1 * FX16_ONE, -1 * FX16_ONE, 0 * FX16_ONE );
            NNS_G3dGeVtx10( +1 * FX16_ONE,  0 * FX16_ONE, 0 * FX16_ONE );
        NNS_G3dGeEnd();
*/
    }
#else
	UNREFERENCED_PARAMETER(lX);
	UNREFERENCED_PARAMETER(lY);
	UNREFERENCED_PARAMETER(lZ);
	UNREFERENCED_PARAMETER(sLeft);
	UNREFERENCED_PARAMETER(sTop);
	UNREFERENCED_PARAMETER(sRight);
	UNREFERENCED_PARAMETER(sBottom);
	UNREFERENCED_PARAMETER(sFront);
	UNREFERENCED_PARAMETER(sBack);
#endif	// #if defined _DS
}

// ================================================================
// ObjDebugRectPointDispHW
/*!
  指定位置にデバッグ矩形を表示する

  @param lX [in] X座標
  @param lY [in] Y座標
  @param sLeft    [in] 左オフセット
  @param sTop     [in] 上オフセット
  @param usWidth  [in] 幅
  @param usHeight [in] 高さ
 */
// ================================================================
void ObjDebugRectPointDispHW( s32 lX, s32 lY, s16 sLeft, s16 sTop, u16 usWidth, u16 usHeight)
{
#if defined _DS
    MTS_ACTION_DS* pAct = &g_obj_debug_rect_act;

    s16 xposi[2],yposi[2];
    s16 rect[4];
    u16 i;
    // 未初期化チェック
    if ( NULL == g_obj_debug_rect_act.act.bac_addr )
        return;
    if (!( g_obj.flag & OBD_OBJ_RECT_D ))
        return;
    
    rect[0] = sLeft;
    rect[1] = sTop;
    rect[2] = (s16)usWidth;
    rect[3] = (s16)usHeight;
    
    for ( i = 0; i < 1; ++i ){

        // 表示位置計算
        if ( g_obj.flag & OBD_OBJ_CAMERA ){
            xposi[i] = (s16)( (lX >> FX32_SHIFT) - (g_obj.camera[i][0] >> FX32_SHIFT) );
            yposi[i] = (s16)( (lY >> FX32_SHIFT) - (g_obj.camera[i][1] >> FX32_SHIFT) );
        }else{
            xposi[i] = (s16)(lX >> FX32_SHIFT);
            yposi[i] = (s16)(lY >> FX32_SHIFT);
        }

    //    pAct->plt_ofst_no[i] = 0;

        // 左上
        pAct->pos[i][0] = (s16)(xposi[i] + rect[LEFT]);
        pAct->pos[i][1] = (s16)(yposi[i] + rect[TOP]);
        mtActDrawDS (pAct);

        // 右上
        pAct->pos[i][0] = (s16)(xposi[i] + rect[LEFT] + rect[WIDTH]);
        pAct->pos[i][1] = (s16)(yposi[i] + rect[TOP]);
        pAct->act.flag ^= MTD_ACT_FLAG_FLIP_H;
        mtActDrawDS (pAct);

        // 右下
        pAct->pos[i][0] = (s16)(xposi[i] + rect[LEFT] + rect[WIDTH]);
        pAct->pos[i][1] = (s16)(yposi[i] + rect[TOP] + rect[HEIGHT]);
        pAct->act.flag ^= MTD_ACT_FLAG_FLIP_V;
        mtActDrawDS (pAct);

        // 左下
        pAct->pos[i][0] = (s16)(xposi[i] + rect[LEFT]);
        pAct->pos[i][1] = (s16)(yposi[i] + rect[TOP] + rect[HEIGHT]);
        pAct->act.flag ^= MTD_ACT_FLAG_FLIP_H;
        mtActDrawDS (pAct);

        // フラグ戻す
        pAct->act.flag ^= ~MTD_ACT_FLAG_FLIP_V;
    }
#else
	UNREFERENCED_PARAMETER(lX);
	UNREFERENCED_PARAMETER(lY);
	UNREFERENCED_PARAMETER(sLeft);
	UNREFERENCED_PARAMETER(sTop);
	UNREFERENCED_PARAMETER(usWidth);
	UNREFERENCED_PARAMETER(usHeight);
#endif	// #if defined _DS
}
#endif  // #endif of #if defined(MTD_DEBUG)  // デバッグ版


// ================================================================
// test_func
/*!
  テスト関数
 
  @param param0 [in] 入力引数0説明
  @param param1 [out] 出力ポインタ引数1説明
  @param param2 [io] 入出力ポインタ引数2説明
 
  @return   返値説明
 
  @note
  補足説明
 */
// ================================================================

/*
 * Revision 1.21  2005/09/27 09:32:58  use1146
 * 当たり登録数追加
 *
 * Revision 1.20  2005/09/23 11:13:34  use1146
 * つづり修正
 *
 * Revision 1.19  2005/09/19 09:03:46  use1146
 * 保持座標を32bit化
 *
 * Revision 1.18  2005/09/06 14:00:49  use1173
 * プリコンパイルヘッダ対応
 *
 * Revision 1.17  2005/06/16 05:07:12  use1146
 * Z軸対応
 *
 * Revision 1.16  2005/06/13 07:46:42  use1146
 * 常にZ軸チェック
 *
 * Revision 1.15  2005/05/31 08:52:11  use1146
 * BELT対応
 *
 * Revision 1.14  2005/04/25 06:20:34  use1146
 * 単発チェック時のNOHITチェック追加
 *
 * Revision 1.13  2005/04/15 02:15:02  use1146
 * 喰らった側の矩形がリストから消えるように修正
 *
 * Revision 1.12  2005/04/08 12:47:17  use1146
 * 空HIT関数追加
 *
 * Revision 1.11  2005/03/30 07:19:32  use1146
 * 矩形をs8からs16に変更
 *
 * Revision 1.10  2005/03/22 06:32:05  use1146
 * 矩形中心取得追加
 *
 * Revision 1.9  2005/03/15 08:47:47  use1146
 * OUTフラグ追加
 *
 * Revision 1.8  2005/03/08 11:12:15  use1146
 * NOHIT対応
 *
 * Revision 1.7  2005/02/25 08:25:36  use1146
 * リスト削除修正
 *
 * Revision 1.6  2005/02/21 09:16:47  use1146
 * バッファ対応
 *
 * Revision 1.5  2005/02/14 09:08:39  use1146
 * HIT関数の引数修正
 *
 * Revision 1.4  2005/02/09 11:32:20  use1146
 * VFLIP対応
 *
 * Revision 1.3  2005/02/03 05:54:29  use1146
 * チェックループの判定修正
 *
 * Revision 1.2  2005/01/27 05:47:20  use1146
 * データ細分化
 *
 * Revision 1.1  2005/01/20 03:09:26  use1146
 * 登録
 *
 */