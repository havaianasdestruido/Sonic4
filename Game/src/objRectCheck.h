// ================================================================
/*!
  @file objRectCheck.h
  @brief 喰らい判定

         毎フレームobjRectCheckAllGroupを呼ぶ事

  @author mana
                Copyright(c) 2004 Dimps

  $Id: objRectCheck.h 2 2011-04-11 05:21:26Z thamada $
  $Date:: 2011-04-11 14:21:26 +0900#$
 */
// ================================================================
/*
 * Memo
 *
 */
/*!
  @page  obj_rect obj 矩形当たり判定
 
  @section  obj_rect_use 使い方
    　 始めに ObjRectCheckInit() , ObjCollisionObjectClear() を2回を呼んで \n
       データを初期化する。（これは ObjInit()内で行われている）\n
    　 #_obj.flag に #OBD_OBJ_RECT を立てる。\n
    　 毎フレーム、ObjRectCheckAllGroup() を実行する。（これは objMain() で自動的に実行されます）\n
   
  @section  obj_rect_res 矩形登録
    　 #OBS_RECT_WORK を用意し、 ObjRectSet() で矩形サイズ、 ObjRectAtkSet() で 攻撃属性 ObjRectDefSet()で 防御属性、\n
    　 ObjRectGroupSet() で所属グループと攻撃対象グループ
    　 そのほか 位置、親オブジェクト、特殊ヒット設定、ヒット関数などを必要に応じて設定する。\n
    \n 
    　 #OBS_RECT_WORK.hit_flag と #OBS_RECT_WORK.def_flag\n
    　　矩形がヒットした時、攻撃側のhit_flagと防御側のdef_flagを1bitずつ比べた時、\n
    　　攻撃側が1で防御側が0であるbitがひとつもなければヒットした事にならない\n
    \n 
    　 #OBS_RECT_WORK.hit_power と #OBS_RECT_WORK.def_power\n
    　　矩形がヒットした時、攻撃側のhit_powerが防御側のdef_powerを上まっていないとヒットした事にならない\n
    
  @section  obj_rect_hit 矩形HIT後
    　 防御側のヒット関数が呼び出される（フラグの設定によっては攻撃側のヒット関数を呼ぶ事もできる）\n
    　 ヒット関数内の判定でヒットしなかった事にしたい場合は その中でObjRectFuncNoHit() を呼び出す。(呼び出さなくてもその後のヒットチェックが起きない程度で 大きなバグは出ない)\n
    \n
    　 #OBS_RECT_WORK::flag に #OBD_RECT_GROUPが立っていなければ、矩形リストから消える\n
    　 また、攻撃側に #OBD_RECT_HIT 、防御側に #OBD_RECT_DAMAGE が立てられ、連続して登録しても再ヒットしないようになっている\n
    　 これを戻すには ObjRectHitAgain() するか、フラグを寝かす\n
    　 これらは #OBS_RECT_WORK::flag の設定によって挙動が変わり、連続的にヒットさせ続けたり（ #OBD_RECT_NOHIT_UP #OBD_RECT_NODAMAGE ）、矩形が一度外れるまでヒットしなくしたり( #OBD_RECT_OUT )できる\n
    \n
    　 #OBS_RECT_WORK::ppCheck に自前の関数を登録する事で、矩形HIT後、さらに 判定を行う事ができる（球体、スポットなど）\n
    \n
    
  @section  obj_rect_sys 仕組み
    　 #_obj_user_resist_nx へ、矩形ポインタを登録し、\n
    　そのフレームの最後（ ObjRectCheckAllGroup() ）で #_obj_user_resist へ グループ順にソートしてコピーする。\n
    　次フレームの最後（ ObjRectCheckAllGroup() ）で #_obj_user_resist のリストを用いてグループごとに矩形判定を行い、ヒットしている場合、ヒット関数が呼び出される。\n
    　以上を繰り返す。\n
    \n
  @sa objRectCheck.c objRectCheck.h
 */

#ifndef _H_OBJRECTCHECK
#define _H_OBJRECTCHECK

#include "objObject.h"

#if	defined(__cplusplus)
extern "C" {
#endif

//----- Macros ---------------------------------------------------------
//================================================================
// OBM_LINE_AND_LINE
/**
 *	2つの範囲が重なっているか比較(当り用)
 *
 * @param x0 [in] 始点0
 * @param w0 [in] 範囲0
 * @param x1 [in] 始点1
 * @param w1 [in] 範囲1
 *
 *	@return	正否
 */
//================================================================
#define OBM_LINE_AND_LINE(x0, w0, x1, w1)                              \
((((s32)(x0) <= (s32)(x1)) && ((s32)(x0) + (s32)(w0) >= (s32)(x1))) || \
 (((s32)(x0) >= (s32)(x1)) && ((s32)(x1) + (s32)(w1) >= (s32)(x0))))

//================================================================
// OBM_POINT_IN_LINE
/**
 *	範囲内にが点があるか比較(当り用)
 *
 *	@param x0 [in] 始点0
 *	@param w0 [in] 範囲0
 *	@param x1 [in] 比較する点
 *
 *	@return	正否
 */
//================================================================
#define OBM_POINT_IN_LINE(x0, w0, x1)                              \
(((s32)(x0) <= (s32)(x1)) && ((s32)(x0) + (s32)(w0) >= (s32)(x1)))

//----- Macros Functions -----------------------------------------------

/*----- Definitions ----------------------------------------------------*/

/// 矩形オフセット 構造体
typedef struct _OBS_RECT
{
    s16 left;   ///< 右端
    s16 top;    ///< 上端
    s16 back;   ///< 奥

    union{
        s16 right; ///< 右端
        u16 width; ///< 幅
    };
    union{
        s16 bottom; ///< 下端
        u16 height; ///< 高さ
    };
    union{
        s16 front;  ///< 手前
        s16 depth;  ///< 奥行き
    };

    VecFx32  pos;    ///< 矩形座標、1:31 親の位置からのオフセット座標、親が居ない場合は直座標となる

} OBS_RECT;

/// 拡張矩形 構造体 登録システム等を用いる場合はこちらを使用する
typedef struct _OBS_RECT_WORK{
    OBS_RECT rect;		///< 矩形データ
    u32      flag;		///< チェック用フラグ

    struct _OBS_OBJECT_WORK* parent_obj;  ///< 親ポインタ    
    void  (*ppHit  )( struct _OBS_RECT_WORK*, struct _OBS_RECT_WORK* );  ///< HIT時呼び出し関数 NULL可		引数1 自分(攻撃側),  引数2 相手(防御側)
    void  (*ppDef  )( struct _OBS_RECT_WORK*, struct _OBS_RECT_WORK* );  ///< 喰らい時呼び出し関数  NULL可	引数1 自分(防御側),  引数2 相手(攻撃側)
    u32   (*ppCheck)( struct _OBS_RECT_WORK*, struct _OBS_RECT_WORK* );  ///< 専用当たりチェック関数、返り値が非0で当たった事になる NULL可
                                                                         ///< 矩形HIT後にチェックを行うのでこれを使う場合矩形を大きめに設定する事

    s16   hit_power;	///< 攻撃値、チェック対象の防御値を上まればヒットする
    s16   def_power;	///< 防御値、チェック対象の攻撃値を上まればヒットされない
    
    u16   hit_flag;		///< 攻撃属性フラグ
						///< チェック相手の対応する防御フラグがどれかひとつでも立っていなければヒットになり、パワーチェックを行う
    u16   def_flag;		///< 防御属性フラグ 29:3
						///< チェック相手の対応する攻撃フラグを無効化する
	u8	group_no;		///< 矩形所属グループNO  OBD_RECT_GROUP_*
	u8	target_g_flag;	///< チェック対象のグループフラグ(攻撃対象設定)

	/* 以下 ユーザー使用領域 */
    u32   attr_flag;	///< 属性フラグ ユーザー使用

    union {
		u32	 user_data;		///< ユーザーデータ
        void *pDataWork;	///< ポインタ
    };

    // struct OBS_RECT_WORK* pSource[4]; ///< ４つまでのダメージソースを記憶

} OBS_RECT_WORK;

/// OBS_RECT_WORK::group_no グループNO
enum {
	OBD_RECT_GROUP_NO_1	= 0,
	OBD_RECT_GROUP_NO_2,
	OBD_RECT_GROUP_NO_3,
	OBD_RECT_GROUP_NO_4,
	OBD_RECT_GROUP_NO_5,
	OBD_RECT_GROUP_NO_6,
	OBD_RECT_GROUP_NO_7,
	OBD_RECT_GROUP_NO_8,

	OBD_RECT_GROUP_NO_NUM			///< グループ最大数 管理グループ数が8を超える場合は target_g_flag のサイズを変更する事
};

// OBS_RECT_WORK::target_g_flag チェック対象グループフラグ
#define OBD_RECT_TARGET_G_FLAG_1	(1 << OBD_RECT_GROUP_NO_1)
#define OBD_RECT_TARGET_G_FLAG_2	(1 << OBD_RECT_GROUP_NO_2)
#define OBD_RECT_TARGET_G_FLAG_3	(1 << OBD_RECT_GROUP_NO_3)
#define OBD_RECT_TARGET_G_FLAG_4	(1 << OBD_RECT_GROUP_NO_4)
#define OBD_RECT_TARGET_G_FLAG_5	(1 << OBD_RECT_GROUP_NO_5)
#define OBD_RECT_TARGET_G_FLAG_6	(1 << OBD_RECT_GROUP_NO_6)
#define OBD_RECT_TARGET_G_FLAG_7	(1 << OBD_RECT_GROUP_NO_7)
#define OBD_RECT_TARGET_G_FLAG_8	(1 << OBD_RECT_GROUP_NO_8)


// OBS_RECT_WORK::flag
#define OBD_RECT_HFLIP       ( 1 << 0 ) ///< 左右反転してチェック
#define OBD_RECT_VFLIP       ( 1 << 1 ) ///< 上下反転してチェック
#define OBD_RECT_ENABLE      ( 1 << 2 ) ///< 有効な矩形である
//#define OBD_RECT_ATK_FUNC    ( 1 << 3 ) ///< 受け時にppHitを呼ばず、攻め時に呼ぶ	使わなくなった

//#define OBD_RECT_NO_POWCHECK ( 1 << 4 ) ///< 矩形同士の適用値判定を行わない(攻撃、防御属性や攻撃値を無視します) 使われていない
#define OBD_RECT_GROUP       ( 1 << 5 ) ///< 1フレーム内で複数HITする
#define OBD_RECT_NOHIT_UP    ( 1 << 6 ) ///< HITアップを相手に立たせません（常に当たり続けるようになります（OBD_RECT_OUTが立っている場合強制的に有効になる）
#define OBD_RECT_NODAMAGE    ( 1 << 7 ) ///< DAMAGEフラグを立てません（常に喰らい続けるようになります（OBD_RECT_OUTが立っている場合強制的に有効になる）

#define OBD_RECT_DAMAGE      ( 1 << 8 ) ///< 喰らい済みフラグ、立っている間、OBD_RECT_HITが立っているものとは判定しません（システムでフラグが立つ）
#define OBD_RECT_HIT         ( 1 << 9 ) ///< HIT済みフラグ、立っている間、OBD_RECT_DAMAGEが立っているものとは判定しません（システムでフラグが立つ）
#define OBD_RECT_OUT         ( 1 <<10 ) ///< HIT後、次フレーム以降もHITし続けた時、一旦HITしなかったフレームを挟まないとHIT関数を呼びません
#define OBD_RECT_NOHIT       ( 1 <<11 ) ///< ヒットチェックしません

#define OBD_RECT_NOPOS       ( 1 <<12 ) ///< 親の座標を無視する（親の反転フラグもみなくなる）

#define OBD_RECT_HIT_UP      ( 1 <<16 ) ///< HIT済みフラグ予定フラグ（システム使用）
// OBD_RECT_OUTを設定した場合だけ使用（システム使用）
#define OBD_RECT_FRAMEHIT    ( 1 <<17 ) ///< 今フレームでＨＩＴしたかどうかチェックするためのシステムフラグ（OBD_RECT_OUTが立っている時だけ有効）
#define OBD_RECT_FRAMEOUT    ( 1 <<18 ) ///< HITしてもHIT関数を呼ばない（OBD_RECT_OUTが立っている時だけ有効）
#define OBD_RECT_CHECK_FUNC  ( 1 <<19 ) ///< ppCheckに登録した関数だけでチェックする（通常の矩形チェックは絶対にHITした事になる）

#define OBD_RECT_NOAUTO_ENABLEOFF ( 1 <<20 ) ///< objectシステム関連処理で自動的に矩形をOFFにする処理を行わない


// OBS_RECT_WORK::hit_power, def_power
#define OBD_RECT_HIT_POWER_DEFAULT ( 64 ) ///< 標準攻撃値
#define OBD_RECT_DEF_POWER_DEFAULT ( 63 ) ///< 標準防御値


// OBS_RECT_WORK::hit_flag, def_flag 属性フラグ
#define OBD_HIT_BODY    ( 1 << 0 ) ///< 身体
#define OBD_HIT_NORMAL  ( 1 << 1 ) ///< 通常攻撃属性
#define OBD_HIT_USE01   ( 1 << 2 ) ///< ユーザー使用フラグ
#define OBD_HIT_USE02   ( 1 << 3 ) // 
#define OBD_HIT_USE03   ( 1 << 4 ) // 
#define OBD_HIT_USE04   ( 1 << 5 ) // 
#define OBD_HIT_USE05   ( 1 << 6 ) // 
#define OBD_HIT_USE06   ( 1 << 7 ) // 
#define OBD_HIT_USE07   ( 1 << 8 ) // 
#define OBD_HIT_USE08   ( 1 << 9 ) // 
#define OBD_HIT_USE09   ( 1 <<10 ) // 
#define OBD_HIT_USE10   ( 1 <<11 ) // 
#define OBD_HIT_USE11   ( 1 <<12 ) // 
#define OBD_HIT_USE12   ( 1 <<13 ) // 
#define OBD_HIT_USE13   ( 1 <<14 ) // 
#define OBD_HIT_USE14   ( 1 <<15 ) // 

#define OBD_HIT_NOATK ( 0x0000 ) ///< 攻撃属性に設定すれば ヒットする事はない
#define OBD_HIT_NOHIT ( 0xffff ) ///< 防御属性に設定すれば ヒットされる事はない

#define OBD_HIT_IGNORE ( 0 ) ///< ヒットなし

//----- External Variables --------------------------------------------------
// デバッグ用
#if defined (MTD_DEBUG)
#if defined _DS
extern MTS_ACTION_DS g_obj_debug_rect_act;
#endif
#endif // #if defined (MTD_DEBUG)

//----- External Declarations -----------------------------------------------
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
OBS_RECT * ObjRectSet ( OBS_RECT * pRec, s16 cLeft, s16 cTop, s16 cRight, s16 cBottom);

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
OBS_RECT * ObjRectZSet ( OBS_RECT * pRec, s16 cLeft, s16 cTop, s16 cBack, s16 cRight, s16 cBottom, s16 cFront );

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
OBS_RECT * ObjRectAllSet ( OBS_RECT * pRec, VecFx32 pos, s16 cLeft, s16 cTop, s16 cBack, s16 cRight, s16 cBottom, s16 cFront );

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
OBS_RECT * ObjRectWorkSet ( OBS_RECT_WORK * pRec, s16 cLeft, s16 cTop, s16 cRight, s16 cBottom);

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
OBS_RECT * ObjRectWorkZSet ( OBS_RECT_WORK * pRec, s16 cLeft, s16 cTop, s16 cBack, s16 cRight, s16 cBottom, s16 cFront );
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
OBS_RECT * ObjRectWorkAllSet ( OBS_RECT_WORK * pRec, VecFx32 pos, s16 cLeft, s16 cTop, s16 cBack, s16 cRight, s16 cBottom, s16 cFront );

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
void ObjRectGroupSet ( OBS_RECT_WORK * pRec, u8 group_no, u8 target_g_flag );

// ================================================================
// ObjRectAtkSet
/*!
  当り属性設定

  @param pRec       [io] 拡張矩形構造体
  @param usHitFlag  [in] 攻撃フラグ
  @param usHitPower [in] 攻撃値
    
 */
// ================================================================
void ObjRectAtkSet ( OBS_RECT_WORK * pRec, u16 usHitFlag, s16 sHitPower );
void ObjRectDefSet ( OBS_RECT_WORK * pRec, u16 usDefFlag, s16 sDefPower );

// ================================================================
// ObjRectHitAgain
/*!
  当り属性設定

  @param pRec       [io] 拡張矩形構造体
    
 */
// ================================================================
void ObjRectHitAgain ( OBS_RECT_WORK * pRec );

// ================================================================
// ObjRectCheckInit
/*!
  当り登録数 初期化
 
 */
// ================================================================
void ObjRectCheckInit();

// ================================================================
// ObjRectRegist
/*!
  矩形を当り登録する
 
  @param pObj1 [in] 対象矩形ポインタ
 
 */
// ================================================================
void ObjRectRegist(OBS_RECT_WORK * pObj );

// ================================================================
// objRectHitRegist
/*!
  総当り判定チェック
 */
// ================================================================
void ObjRectCheckAllGroup();

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
void ObjRectFuncNoHit(OBS_RECT_WORK* pObjA, OBS_RECT_WORK*  pObjD);


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
u16 ObjRectFlagCheck( u32 ulAtkFlag, u32 usDefFlag, s32 lAtkPower, s32 lDefPower);


// ================================================================
// ObjRectCheck
/*!
  矩形同士の当り判定を取る
 
  @param pObj1 [in] 対象矩形その１
  @param pObj2 [in] 対象矩形その２
 
  @return   0 NOHIT   1 HIT
 */
// ================================================================
u16 ObjRectCheck (OBS_RECT* pObj1, OBS_RECT* pObj2);

// ================================================================
// ObjRectWorkCheck
/*!
  矩形同士の当り判定を取る
 
  @param pObj1 [in] 対象矩形その１
  @param pObj2 [in] 対象矩形その２
 
  @return   0 NOHIT   1 HIT
 */
// ================================================================
u16 ObjRectWorkCheck (OBS_RECT_WORK* pObj1, OBS_RECT_WORK* pObj2);

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
u16 ObjRectWorkPointCheck(OBS_RECT_WORK* pObj, s32 lX, s32 lY, s32 lZ );

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
u16 ObjRectPointCheck(OBS_RECT* pObj, s32 lX, s32 lY, s32 lZ );

// ================================================================
// ObjRectRegistGet
/*!
  登録された矩形ポインタを取得する
 
  @param ucGroup [in] 登録グループフラグ
  @param usIndex [in] 番号

 @return 拡張矩形ポインタ
 */
// ================================================================
OBS_RECT_WORK* ObjRectRegistGet( u8 ucGroup, s16 sIndex );
// ================================================================
// ObjRectRegistNxGet
/*!
  このフレームに登録された矩形ポインタを取得する
 
  @param ucGroup [in] チェックする登録グループフラグ
  @param usIndex [in] 番号

 @return 拡張矩形ポインタ
 */
// ================================================================
OBS_RECT_WORK* ObjRectRegistNxGet( u8 ucGroup, s16 sIndex );

// ================================================================
// ObjRectPosGet
/*!
  端位置を設定
 
  @param vPos  [out] 位置
  @param pRec  [in]  矩形ワーク
 
 */
// ================================================================
void ObjRectPosGet( VecFx32 * vPos, OBS_RECT_WORK* pRec );

// ================================================================
// ObjRectCenterX
/*!
  矩形の中心を返す
 
  @param pWork     [in] 対象矩形その１
 
  @return   中心X座標
 */
// ================================================================
fx32 ObjRectCenterX( OBS_RECT_WORK* pWork );
fx32 ObjRectCenterY( OBS_RECT_WORK* pWork );
fx32 ObjRectCenterZ( OBS_RECT_WORK* pWork );

// ================================================================
// ObjRectHitCenterX
/*!
  ヒットしあった矩形の中心を返す（ヒットしあってないものの場合は真中が帰ってくる）
 
  @param pWork     [in] 対象矩形その１
  @param pAttacker [in] 対象矩形その２
 
  @return   中心X座標 1:23:8
 */
// ================================================================
s32 ObjRectHitCenterX( OBS_RECT_WORK* pWork, OBS_RECT_WORK* pAttacker );
s32 ObjRectHitCenterY( OBS_RECT_WORK* pWork, OBS_RECT_WORK* pAttacker );

// ================================================================
// ObjRectFuncblank
/*!
  当り、関数呼び出し空処理
 
  @param pObjA [in] 対象矩形 攻撃側
  @param pObjD [in] 対象矩形 防御側
 
 */
// ================================================================
void ObjRectFuncblank( OBS_RECT_WORK* pObjA, OBS_RECT_WORK*  pObjD );

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
void ObjRectLTBSet( OBS_RECT_WORK* pRec, s32 *lLeft, s32*lTop, s32 *lBack );
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
void ObjRectWHDSet( OBS_RECT_WORK* pRec, u16 *usWidth, u16* usHeight, u16 *usDepth );


#if defined(MTD_DEBUG)  // デバッグ版
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
void ObjDebugSetRectDispGroup(u8 group);

// ================================================================
// ObjDebugRectActionInit
/*!
  デバッグ用レクト表示アクション初期化 （リリース時、空マクロになります
 */
// ================================================================
void ObjDebugRectActionInit();

// ================================================================
// ObjDebugRectActionExit
/*!
  デバッグ用レクト表示アクション解放 （リリース時、空マクロになります
 */
// ================================================================
void ObjDebugRectActionExit();

// ================================================================
// ObjDebugRectDispAll
/*!
  登録した矩形を全てデバッグ表示する （リリース時、空マクロになります
 */
// ================================================================
void ObjDebugRectDispAll();

// ================================================================
// ObjDebugRectPointDisp
/*!
  指定位置にデバッグ矩形を表示する （リリース時、空マクロになります

  @param lX [in] X座標
  @param lY [in] Y座標
  @param sLeft    [in] 左オフセット
  @param sTop     [in] 上オフセット
  @param sRight   [in] 右オフセット
  @param sBottom  [in] 下オフセット
 */
// ================================================================
void ObjDebugRectPointDisp( fx32 lX, fx32 lY, s16 sLeft, s16 sTop, s16 sRight, s16 sBottom);

// ================================================================
// ObjDebugRectPointDisp3D
/*!
  指定位置に3Dでデバッグ矩形を表示する （リリース時、空マクロになります

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
void ObjDebugRectPointDisp3D( fx32 lX, fx32 lY, fx32 lZ, s16 sLeft, s16 sTop, s16 sRight, s16 sBottom, s16 sFront, s16 sBack);

// ================================================================
// objDebugRectDispWH
/*!
  指定位置にデバッグ矩形を表示する （リリース時、空マクロになります

  @param lX [in] X座標
  @param lY [in] Y座標
  @param sLeft    [in] 左オフセット
  @param sTop     [in] 上オフセット
  @param usWidth  [in] 幅
  @param usHeight [in] 高さ
 */
// ================================================================
void ObjDebugRectPointDispHW( s32 lX, s32 lY, s16 sLeft, s16 sTop, u16 usWidth, u16 usHeight);

// ================================================================
// ObjDebugRectDispObject
/*!
  指定位置にデバッグ地形判定矩形を表示する （リリース時、空マクロになります

  @param pObj [in] オブジェクトポインタ
 */
// ================================================================
void ObjDebugRectDispObject( OBS_OBJECT_WORK *pObj, float a, float r, float g, float b );

// ================================================================
// ObjDebugRectDisp
/*!
  指定矩形を表示する （リリース時、空マクロになります

  @param pRec [in] 矩形ポインタ
 */
// ================================================================
void ObjDebugRectDisp( OBS_RECT * pRec );

// ================================================================
// ObjDebugRectExDisp
/*!
  指定矩形を表示する （リリース時、空マクロになります

  @param pRec [in] 拡張矩形ポインタ
 */
// ================================================================
#if defined _DS
void ObjDebugRectExDisp( OBS_RECT_WORK * pRec );
#else
void ObjDebugRectExDisp(OBS_RECT_WORK * pRec, float a, float r, float g, float b);
#endif

// ================================================================
// ObjDebugPointDisp
/*!
  指定位置にデバッグ座標を表示する

  @param lX [in] X座標
  @param lY [in] Y座標
 */
// ================================================================
void ObjDebugPointDisp( s32 lX, s32 lY );

#else

#define ObjDebugSetRectDispGroup(group)
#define ObjDebugRectActionInit()
#define ObjDebugRectActionExit()
#define ObjDebugRectDispAll()
#define ObjDebugRectPointDisp( lX,  lY,  sLeft,  sTop,  usWidth,  usHeight)
#define ObjDebugRectPointDisp3D( lX, lY, lZ, sLeft, sTop, sRight, sBottom, sFront, sBack)
#define ObjDebugRectPointDispHW( lX,  lY,  sLeft,  sTop,  usWidth,  usHeight)
#if defined _DS
#define ObjDebugRectDispObject( pObj )
#else
#define ObjDebugRectDispObject( pObj, a, r, g, b )
#endif
#define ObjDebugRectDisp(pRec)
#if defined _DS
#define ObjDebugRectExDisp( pRec );
#else
#define ObjDebugRectExDisp( pRec, a, r, g, b );
#endif
#define ObjDebugPointDisp( lX, lY );

#endif  // #endif of #if defined(MTD_DEBUG)  // デバッグ版

#if	defined(__cplusplus)
} /* extern "C" */
#endif

#endif // _H_OBJRECTCHECK


/*
 * Revision 1.16  2005/09/23 11:13:34  use1146
 * つづり修正
 *
 * Revision 1.15  2005/09/19 09:03:46  use1146
 * 保持座標を32bit化
 *
 * Revision 1.14  2005/09/06 14:00:49  use1173
 * プリコンパイルヘッダ対応
 *
 * Revision 1.13  2005/07/05 05:02:16  use1146
 * フラグ追加
 *
 * Revision 1.12  2005/06/16 05:07:12  use1146
 * Z軸対応
 *
 * Revision 1.11  2005/05/31 08:52:11  use1146
 * BELT対応
 *
 * Revision 1.10  2005/04/08 12:47:16  use1146
 * 空HIT関数追加
 *
 * Revision 1.9  2005/03/30 07:19:32  use1146
 * 矩形をs8からs16に変更
 *
 * Revision 1.8  2005/03/29 05:28:24  use1146
 * 属性にユーザー使用フラグ追加
 *
 * Revision 1.7  2005/03/22 06:32:05  use1146
 * 矩形中心取得追加
 *
 * Revision 1.6  2005/03/15 08:47:47  use1146
 * OUTフラグ追加
 *
 * Revision 1.5  2005/03/08 11:12:14  use1146
 * NOHIT対応
 *
 * Revision 1.4  2005/02/21 09:16:47  use1146
 * バッファ対応
 *
 * Revision 1.3  2005/02/03 05:54:54  use1146
 * フラグ修正
 *
 * Revision 1.2  2005/01/27 05:47:20  use1146
 * データ細分化
 *
 * Revision 1.1  2005/01/20 03:09:26  use1146
 * 登録
 *
 */