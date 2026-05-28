// ================================================================
/*!
  @file objCollision.h
  @brief 地形判定まとめヘッダ

  @author mana
                Copyright(c) 2006 Dimps

  $Id: objCollision.h 2 2011-04-11 05:21:26Z thamada $
  $Date:: 2011-04-11 14:21:26 +0900#$
 */
// ================================================================
/*
 * Memo
 *
 */
/*!
  @page  obj_collision obj 地形判定
 
  @section  obj_collision_check 地形判定
    　 #g_obj.flag に #OBD_OBJ_COL_BLOCK を立てて、objSetBlockCollision() で地形データをセットすれば\n
    　 objDiffCollisionEarthCheck() や objCollisionUnion() などでブロック地形チェックを行う事ができる\n
    　 #g_obj.flag に #OBD_OBJ_COL_DIFF を立てて、objSetDiffCollision() で地形データをセットすれば\n
    　 同じ関数で差分地形チェックを行う事ができる。\n
    　 objSetBlockCollision()、 objSetDiffCollision() に渡す、データの中身は自分で設定する。\n
    \n
    　 チェック時、#OBS_COL_CHK_DATA を用意する。\n
    　 この中の pDir 、 pAttr はゲームで使用しないのであれば、NULLにしておく。\n
    　 使用する場合はそこにセットしたアドレスへ角度や地形属性の結果が返る。\n
    　 usFlag にはチェックするオブジェクトの状態や特殊なチェック方法などを設定する。\n
  @sa objDiffCollisionCheck.c objDiffCollisionCheck.h
  
  @section  obj_collision_diff 地形データ 差分地形
    　 地形データを8ドットx8ドットで区切りそれぞれに縦、横それぞれから見た時の差分値を設定したデータ\n
    　 またそのデータを64個の塊にして再配置されたもの\n
    　 超広大で複雑なマップを少なめのデータで表現できる。\n
    　 ただし、調整しづらく、マップがパターン化しやすい。\n
    　 広大なマップを使用するアクションゲームに向いている。
  @sa objDiffCollisionField.c objDiffCollisionField.h
    
  @section  obj_collision_block 地形データ ブロック地形
    　 地形データを16ドットｘ16ドット（ #OBD_MAP_BLOCK_SIZE ）で区切り、\n
    　 それぞれに土地の形、属性を指定する数値を割り当てる。（ #OBE_BLOCK_COL_ID ）\n
    　 データとしては単純な配列となる。
    　 現在作成中、地形種類を追加する時、 #_obj_block_collision_func に判定関数を追加、#OBE_BLOCK_COL_ID に地形の種類を追加する。\n
    　 地形データこのIDに合わせる。\n
    　 多様なマップを手軽に作成できる。ただし、広さに比例してデータが増える。また土地の形を増やすたびに判定関数が必要になる\n
    　 
  @sa objBlockCollisionField.c objBlockCollisionField.h

  @section  obj_collision_obj 地形データ オブジェクト
    　 オブジェクトを使った地形。プログラムのみでも地形を作成できる。\n
    　 これのみで地形を表現する事はあまり無いが、できなくもない。\n
    　 移動する足場などに向いている、欠点としては、数が増えると処理がかかってしまう\n
    　 地形データは現在、差分値のみ対応、ブロック地形用の判定は未作成\n

  @sa objDiffCollisionObject.c objDiffCollisionObject.h
 */

#ifndef _H_OBJCOLLSION
#define _H_OBJCOLLSION

//----- Include Files -------------------------------------------------------
#include "typedef.h"
#include "map_file_format.h"

#if	defined(__cplusplus)
extern "C" {
#endif

//----- Definitions ---------------------------------------------------------
#define OBD_COL_NEW_DATA_TYPE	(1)		///< 地形新データタイプ


// 地形チェック物体進行方向フラグ
#define OBD_COL_PLUS  (         0) ///< プラス方向 （値なし）
#define OBD_COL_MINUS ( 0x01 << 0) ///< マイナス方向
// 地形チェック方向フラグ
#define OBD_COL_X     (         0) ///< X軸方向（値なし）
#define OBD_COL_Y     ( 0x01 << 1) ///< Y軸方向
#define OBD_COL_3CHAR ( 0x01 << 2) ///< 3キャラ分、先チェックを行う 未使用

// 地形フラグセット
#define OBD_COL_DOWN  ( OBD_COL_Y | OBD_COL_PLUS  ) ///< 下向きへのチェック
#define OBD_COL_UP    ( OBD_COL_Y | OBD_COL_MINUS ) ///< 上向きへのチェック
#define OBD_COL_LEFT  ( OBD_COL_X | OBD_COL_MINUS ) ///< 左向きへのチェック
#define OBD_COL_RIGHT ( OBD_COL_X | OBD_COL_PLUS  ) ///< 右向きへのチェック

// ucSuf
#define OBD_COL_THROUGH       ( 0x80) ///< すり抜け時
#define OBD_COL_LIMITWALL     ( 0x40) ///< マップ外を壁としてチェックする
#define OBD_COL_SIMPLE        ( 0x20) ///< 単純チェック（処理が軽いが特定の状態で挙動がおかしくなる）
#define OBD_COL_B             ( 0x01) ///< B面時

// 地形属性フラグ attr （
// 今後、ゲームごとの変化に対応できるようにUSER_FLAGに対応する。
// グラインドの様な特定ゲーム専用フラグはUSER_FLAGを用いるようにする。
//#define OBD_COL_ATTR_THROUGH  ( 0x01 ) ///< すり抜け
//#define OBD_COL_ATTR_CLIFF    ( 0x02 ) ///< 崖
//#define OBD_COL_ATTR_GRAIND   ( 0x04 ) ///< グラインド
// データ側での属性フラグ配置
// ◆初期化時の関数登録毎にするかも
#define OBD_COL_DATA_ATTR_THROUGH	( 0x01 )	///< すり抜け
#define OBD_COL_DATA_ATTR_CLIFF		( 0x02 )	///< 崖
#define OBD_COL_DATA_ATTR_GRAIND	( 0x04 )	///< グラインド

// サーフェイス引き数に設定 ucSuf | OBD_COL_THROUGH_CHECK_D(pPlayer->sSpdY)
//#define OBD_COL_THROUGH_CHECK_D( sSpdY ) ( (( (sSpdY) <     0 ) ? (OBD_COL_THROUGH) : (0) ) ) 
//#define OBD_COL_THROUGH_CHECK( sSpdY )   ( (( (sSpdY) < 0x300 ) ? (OBD_COL_THROUGH) : (0) ) ) 


/// 差分地形当たりデータアドレステーブル
typedef struct tag_OBS_DIFF_COLLISION {
#if OBD_COL_NEW_DATA_TYPE
    DF_BLOCK*  cl_diff_datap;     ///< 4bytes: 当たり差分データ先頭アドレス
    DI_BLOCK*  direc_datap;       ///< 4bytes: 当たり角度データ先頭アドレス
    //u16* block_datap;       ///< 4bytes: ブロックデータ先頭アドレス 
    MP_BLOCK* block_map_datap[2]; ///< 4bytes: マップ先頭アドレス
    AT_BLOCK*  char_attr_datap;   ///< 4bytes: キャラクタ属性データ先頭アドレス 
    u16  map_block_num_x;   ///< 2bytes: マップ ブロック数X
    u16  map_block_num_y;   ///< 2bytes: マップ ブロック数Y
	u32  diff_block_num;	///< 4bytes: 差分データ情報数
	u32  dir_block_num;		///< 4bytes: 角度データ情報数
	u32  attr_block_num;	///< 4bytes: 属性データ情報数
    s32  left;              ///< 4bytes: マップ開始位置 1:31
    s32  top;               ///< 4bytes: マップ開始位置 1:31
    s32  right;             ///< 4bytes: マップ終了位置 1:31
    s32  bottom;            ///< 4bytes: マップ終了位置 1:31
#else
    s8*  cl_diff_datap;     ///< 4bytes: 当たり差分データ先頭アドレス
    u8*  direc_datap;       ///< 4bytes: 当たり角度データ先頭アドレス
    u16* block_datap;       ///< 4bytes: ブロックデータ先頭アドレス 
    u16* block_map_datap[2];///< 4bytes: マップ先頭アドレス
    u8*  char_attr_datap;   ///< 4bytes: キャラクタ属性データ先頭アドレス 
    u16  map_block_num_x;   ///< 2bytes: マップ ブロック数X
    u16  map_block_num_y;   ///< 2bytes: マップ ブロック数Y
    s32  left;              ///< 4bytes: マップ開始位置 1:31
    s32  top;               ///< 4bytes: マップ開始位置 1:31
    s32  right;             ///< 4bytes: マップ終了位置 1:31
    s32  bottom;            ///< 4bytes: マップ終了位置 1:31
#endif
} OBS_DIFF_COLLISION;

/// ブロック地形当たりデータアドレステーブル
typedef struct tag_OBS_BLOCK_COLLISION {
    u8* pData[2]; ///< 4bytes: 当たりデータ先頭アドレス
    u32 width;    ///< 4bytes: マップデータの幅 1:31
    u32 height;   ///< 4bytes: マップデータの高さ 1:31
    s32 left;     ///< 4bytes: マップ開始位置 1:31
    s32 top;      ///< 4bytes: マップ開始位置 1:31
    s32 right;    ///< 4bytes: マップ終了位置 1:31
    s32 bottom;   ///< 4bytes: マップ終了位置 1:31

} OBS_BLOCK_COLLISION;

#if 0
// objObject.hへ移動
/// オブジェクト地形チェック用構造体
typedef struct tag_OBS_COLLISION_OBJ {
    struct _OBS_OBJECT_WORK * obj;      ///< 登録したオブジェクトワークポインタ NULL可
    struct _OBS_OBJECT_WORK * rider_obj;    ///< 乗っているオブジェクト  NULL可
    struct _OBS_OBJECT_WORK * toucher_obj;  ///< 接触しているオブジェクト  NULL可

    VecFx32           pos;     ///< 1:19:12 親からのオフセット位置（親の設定が無ければ直座標）
    s16               ofst_x;   ///< オフセットX値 1:15
    s16               ofst_y;   ///< オフセットY値 1:15
    
    u32               flag;   ///< フラグ #OBD_COLOBJ_HFLIP など
    u16               dir;    ///< 角度
    u16               attr;   ///< 属性

    s8*               diff_data;    ///< 地形差分データ先頭アドレス
    u8*               dir_data;     ///< 地形角度データ先頭アドレス（キャラごとの角度）
    u8*               attr_data;    ///< 地形属性データ先頭アドレス（キャラごとの属性）
    u16               width;  ///< 地形幅   ドット単位
    u16               height; ///< 地形高さ ドット単位

    // 計算用データ
    VecFx32           check_pos;   ///< 計算用ワーク 1:19:12、objCollisionObjectRegistにvPosと親の座標から設定される、
    s16               check_ofst_x; ///< 計算用ワーク チェックオフセットX値 1:15
    s16               check_ofst_y; ///< 計算用ワーク チェックオフセットY値 1:15
    s32               left;        ///< 計算用ワーク
    s32               top;         ///< 計算用ワーク
    s32               right;       ///< 計算用ワーク
    s32               bottom;      ///< 計算用ワーク
    u16               check_dir;  ///< 計算用ワーク チェック角度
} OBS_COLLISION_OBJ;

/// OBS_COLLISION_OBJ::flag
#define OBD_COLOBJ_HFLIP		( 1 << 0 )	///< 左右反転してチェック
#define OBD_COLOBJ_VFLIP		( 1 << 1 )	///< 上下反転してチェック
#define OBD_COLOBJ_DIR			( 1 << 2 )	///< 角度に合わせて地形回転を行う
#define OBD_COLOBJ_DIR_FLIP		( 1 << 3 )	///< 角度をフリップ

#define OBD_COLOBJ_NOPOS_PARENT	( 1 << 4 )	///< 親の座標を無視する
#define OBD_COLOBJ_NODIR_PARENT	( 1 << 5 )	///< 親の角度を無視する
#define OBD_COLOBJ_NODIR		( 1 << 6 )	///< dirを無効にする
#define OBD_COLOBJ_NOATTR		( 1 << 7 )	///< attrを無効にする

#define OBD_COLOBJ_NOHIT		( 1 << 8 )	///< チェックを行わない

#define OBD_COLOBJ_SYS_HFLIP	( 1 << 30 )	///< システム用 左右反転チェックフラグ
#define OBD_COLOBJ_SYS_VFLIP	( 1 << 31 )	///< システム用 上下反転チェックフラグ 
#endif

/// 地形チェック判定データ
typedef struct tag_OBS_COL_CHK_DATA {
    s32 pos_x;   ///< 座標 X 1:31
    s32 pos_y;   ///< 座標 Y 1:31
    u16* dir;   ///< 角度データ設定先ポインタ NULL可
    u32* attr;  ///< 属性データ設定先ポインタ NULL可
    u16 flag;  ///< チェックフラグ #OBD_COL_DOWN など
    u16 vec;   ///< 方向フラグ（ 4ビットしか使わないが、アライン合わせで16bit）#OBD_COL_DOWN など
} OBS_COL_CHK_DATA;


//----- Macros --------------------------------------------------------------

//----- Macros Functions ----------------------------------------------------

//----- External Variables --------------------------------------------------

//----- External Declarations -----------------------------------------------

#if	defined(__cplusplus)
} /* extern "C" */
#endif

//----- Include Files -------------------------------------------------------
#include "objObject.h"


#endif // _H_OBJDIFFCOLLSIONFIELD

