// ================================================================
/*!
  @file objDiffCollisionField.c
  @brief 地形取得 stafColからDS向けに引継ぎ\n
    _ODCS_FCOL_ADRS_TBLをポインタではなく内容をGlobalに保持することで\n
    部分挿げ替えができるようにしたほうがいいかなと

  @author mana
                Copyright(c) 2004 Dimps

  $Id: objDiffCollisionField.cpp 2 2011-04-11 05:21:26Z thamada $
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
#include "objDiffCollisionField.h"

//----- Macros ---------------------------------------------------------

//----- Macros Functions -----------------------------------------------



//----- Definitions ----------------------------------------------------
// Nitroヘッダに該当定義がみつからないので定義
#define BG_SC_CHAR_NO_MASK      0x03ff      ///< キャラクタＮｏ
#define BG_SC_H_FLIP            0x0400      ///< Ｈフリップ
#define BG_SC_V_FLIP            0x0800      ///< Ｖフリップ

#define OBD_MAP_CHAR_SIZE_X ( 8 ) ///< 1キャラの横ドット数
#define OBD_MAP_CHAR_SIZE_Y ( 8 ) ///< 1キャラの縦ドット数
#define OBD_MAP_BLOCK_CHAR_NUM_X ( 8 ) ///< １ブロックの横キャラ数
#define OBD_MAP_BLOCK_CHAR_NUM_Y ( 8 ) ///< １ブロックの縦キャラ数


// 各データサイズ
#define OBD_BLOCK_DATA_SIZE \
    (OBD_MAP_BLOCK_CHAR_NUM_X * OBD_MAP_BLOCK_CHAR_NUM_Y * 2)
#define OBD_MAP_CL_CHAR_DATA_SIZE \
    (OBD_MAP_CHAR_SIZE_X * OBD_MAP_CHAR_SIZE_Y / 8)
#define OBD_MAP_CL_DIFF_DATA_SIZE \
    (OBD_MAP_CHAR_SIZE_X * OBD_MAP_CHAR_SIZE_Y / 8)

//----- External Declarations ------------------------------------------

//----- Static Declarations --------------------------------------------

//----- Global Variables -----------------------------------------------

//----- Local Variables ------------------------------------------------

//----- Global Functions -----------------------------------------------
const OBS_DIFF_COLLISION* _obj_fcol = NULL;

//----- Local Functions ------------------------------------------------
#if !OBD_COL_NEW_DATA_TYPE
u32 objGetCharData( s32 lPosX, s32 lPosY, u8 ucSuf);
#endif

static s32 objGetColDataX( s32 lPosX, s32 lPosY, u16 ucSuf, u16* pDir, u32 *pAttr );
static s32 objGetColDataY( s32 lPosX, s32 lPosY, u16 ucSuf, u16* pDir, u32 *pAttr );
inline s32 objMapGetDiff( s32 lCol, s8 sPix, s8 sDelta );
inline s32 objMapGetForward(s8 sPix, s8 sDelta );
inline s32 objMapGetBack(s8 sPix, s8 sDelta );
static inline s32 objMapGetBackFront(s8 sPix, s8 sDelta );
inline s32 objMapGetForwardRev(s8 sPix, s8 sDelta );

#if OBD_COL_NEW_DATA_TYPE
static u8 objGetAttrData(s32 pos_x, s32 pos_y, u16 suf);
#else
inline u8 objGetAttrData( u16 usCharNo );
#endif

#if OBD_COL_NEW_DATA_TYPE
static MP_BLOCK* objGetMapBlockData(s32 pos_x, s32 pos_y, u16 suf);
static u8* objGetDiffCharData(s32 pos_x, s32 pos_y, u16 suf);
static s8 objGetXDiffData(s32 pos_x, s32 pos_y, u16 suf);
static s8 objGetYDiffData(s32 pos_x, s32 pos_y, u16 suf);
static void objGetConv88Pos(s32 pos_x, s32 pos_y, MP_BLOCK *mp_block, s32 *conv_pos_x, s32 *conv_pos_y);
static u16 objGetDirData(s32 pos_x, s32 pos_y, u16 suf);
static u8 objGetConvDiff(MP_BLOCK *mp_block, u8 diff);
#endif // #if OBD_COL_NEW_DATA_TYPE

// ================================================================
// ObjSetDiffCollision
/*!
  指定した地形情報アドレステーブルをチェックする地形に設定する
 
  @param pFat [in] ODCS_FCOL_ADRS_TBLポインタ
 
 */
// ================================================================
void ObjSetDiffCollision( const OBS_DIFF_COLLISION* pFat )
{
    _obj_fcol = pFat;
}
// ================================================================
// ObjGetDiffCollision
/*!
  設定されている地形データポインタを取得
 
  @return OBS_DIFF_COLLISIONポインタ
 
 */
// ================================================================
const OBS_DIFF_COLLISION* ObjGetDiffCollision()
{
    return _obj_fcol;
}

// ================================================================
// ObjDiffCollisionDetFast
/*!
  マップ当たりチェック 構造体を用いない

  @param lPosX  [in] X座標 1:31
  @param lPosY  [in] Y座標 1:31
  @param usFlag [in] チェック用フラグ #OBD_COL_B 、 #OBD_COL_THROUGH など
  @param usVec  [in] OBD_COL_DOWN 等、進行方向プラスorマイナス、XチェックorYチェックのフラグを設定 
  @param pDir   [out] 角度ポインタ NULLで設定を行わない 値は(0～2π を 0～256 に写像)
  @param pAttr  [out] 属性ポインタ NULLで設定を行わない
 
  @return   接地面までの距離(正: 浮いている  負: 埋まっている)
 
  @note
  座標から指定方向に 1 キャラを調べ、接地面までの距離を求める

 */
// ================================================================
s32 ObjDiffCollisionDetFast( s32 lPosX, s32 lPosY, u16 usFlag, u16 usVec, u16* pDir, u32* pAttr)
{
    OBS_COL_CHK_DATA tData;
    
    tData.pos_x = lPosX;
    tData.pos_y = lPosY;
    tData.dir = pDir;
    tData.attr = pAttr;
    tData.flag = usFlag;
    tData.vec = usVec;

    return ObjDiffCollisionFast( &tData);
}
// ================================================================
// ObjDiffCollisionFast
/*!
  マップ当たりチェック

  @param pData      [in] OBS_COL_CHK_DATAポインタ
 
  @return   接地面までの距離(正: 浮いている  負: 埋まっている)
 
  @note
  座標から指定方向に 1 キャラを調べ、接地面までの距離を求める

 */
// ================================================================
s32 ObjDiffCollisionFast( OBS_COL_CHK_DATA * pData )
{
    s32 lCol = 0;
    s8  sPix;
    u16  usDir = 0; // 前角度保持
    u32  ulAttr = 0; // 前角度保持
    s8  cDelta = 8; // 進行方向値

    // 単純チェック
    if ( !_obj_fcol->cl_diff_datap ){
        switch ( pData->vec ){
        case OBD_COL_DOWN:
            lCol = _obj_fcol->bottom - pData->pos_y;
            break;
        case OBD_COL_UP:
            lCol = pData->pos_y - _obj_fcol->top;
            break;
        case OBD_COL_LEFT:
            lCol = pData->pos_x - _obj_fcol->left;
            break;
        case OBD_COL_RIGHT:
            lCol = _obj_fcol->right - pData->pos_x;
            break;
        }
        return (s32)MTM_MATH_CLIP(lCol, -31, 31);
    }

    // 前角度保持
    if ( pData->dir )
        usDir = *pData->dir;
    if ( pData->attr )
        ulAttr = *pData->attr;
    
    // フラグから進行方向の設定
    if ( pData->vec & OBD_COL_MINUS )
        cDelta = -8;
    
    // 地形差分値、角度を取得
    if ( pData->vec & OBD_COL_Y ){
        // X,Yを逆転してチェックする
        lCol = objGetColDataY( pData->pos_x, pData->pos_y, pData->flag, pData->dir, pData->attr ); 
        sPix = (s8)(pData->pos_y & 0x00000007);
    }else{
        lCol = objGetColDataX( pData->pos_x, pData->pos_y, pData->flag, pData->dir,pData->attr );
        sPix = (s8)(pData->pos_x & 0x00000007);
    }
    
    if ( lCol == 0) {
        // 地形にHITしなかったので角度を前の状態に戻す
        if ( pData->dir )
            *pData->dir = usDir;
        if ( pData->attr )
            *pData->attr = ulAttr;
        return objMapGetForward(sPix, cDelta);
    }
    else if ( lCol == 8) {
        // 隙間無しブロックにHIT
        return objMapGetBack(sPix, cDelta);
    }
    else {
        // 隙間有りブロックにHIT
        return objMapGetDiff( lCol, sPix, cDelta);
    }
}
// ================================================================
// ObjDiffCollisionDet
/*!
  マップ当たりチェック 構造体用いず

  @param lPosX  [in] X座標 1:31
  @param lPosY  [in] Y座標 1:31
  @param usFlag [in] チェック用フラグ #OBD_COL_B 、 #OBD_COL_THROUGH など
  @param usVec  [in] OBD_COL_DOWN 等、進行方向プラスorマイナス、XチェックorYチェックのフラグを設定 
  @param pDir   [out] 角度ポインタ NULLで設定を行わない 値は(0～2π を 0～256 に写像)
  @param pAttr  [out] 属性ポインタ NULLで設定を行わない
 
  @return   接地面までの距離(正: 浮いている  負: 埋まっている)
 
  @note
  座標から指定方向に ３キャラを調べ、接地面までの距離を求める

 */
// ================================================================
s32 ObjDiffCollisionDet(s32 lPosX, s32 lPosY, u16 usFlag, u16 usVec, u16* pDir, u32* pAttr)
{
    OBS_COL_CHK_DATA tData;
    
    tData.pos_x = lPosX;
    tData.pos_y = lPosY;
    tData.dir = pDir;
    tData.attr = pAttr;
    tData.flag = usFlag;
    tData.vec = usVec;

    return ObjDiffCollision( &tData);
}
// ================================================================
// objCollision
/*!
  マップ当たりチェック

  @param pObj       [in] チェックするオブジェクトポインタ
  @param pData      [in] OBS_COL_CHK_DATAポインタ
 
  @return   接地面までの距離(正: 浮いている  負: 埋まっている)
 
  @note
  座標から指定方向に ３キャラを調べ、接地面までの距離を求める

 */
// ================================================================
s32 ObjDiffCollision(OBS_COL_CHK_DATA * pData )
{
    s32 lCol = 0;
    s32 lMoveX = 0;
    s32 lMoveY = 0;
    s8  sPix;
    u16  usDir = 0; 
    u32  ulAttr = 0; 
    s8  cDeltaX = 0; // 進行方向値
    s8  cDeltaY = 0; // 進行方向値
    s32 (*pFunc)(s32, s32, u16, u16*, u32*  );

    // 単純チェック
    if ( !_obj_fcol->cl_diff_datap ){
        switch ( pData->vec ){
        case OBD_COL_DOWN:
            lCol = _obj_fcol->bottom - pData->pos_y;
            break;
        case OBD_COL_UP:
            lCol = pData->pos_y - _obj_fcol->top;
            break;
        case OBD_COL_LEFT:
            lCol = pData->pos_x - _obj_fcol->left;
            break;
        case OBD_COL_RIGHT:
            lCol = _obj_fcol->right - pData->pos_x;
            break;
        }
        return (s32)MTM_MATH_CLIP(lCol, -31, 31);
    }
    
    // 前角度保持
    if ( pData->dir )
        usDir = *pData->dir;
    // 前属性保持
    if ( pData->attr )
        ulAttr = *pData->attr;
    
    // フラグから進行方向の設定
    if ( pData->vec & OBD_COL_Y ){
        cDeltaY = 8;
        if ( pData->vec & OBD_COL_MINUS )
            cDeltaY = -8;
    }else{
        cDeltaX = 8;
        if ( pData->vec & OBD_COL_MINUS )
            cDeltaX = -8;
    }
    
    // 地形差分値、角度を取得
    if ( pData->vec & OBD_COL_Y ){
        sPix = (s8)(pData->pos_y & 0x00000007);
        pFunc = objGetColDataY;
    }else{
        sPix = (s8)(pData->pos_x & 0x00000007);
        pFunc = objGetColDataX;
    }
    
    // 地形判定
    lCol = pFunc( pData->pos_x, pData->pos_y, pData->flag, pData->dir, pData->attr );
    
    if ( lCol == 0) {
        // 地形にHITしなかったので次をチェック
        lMoveX += cDeltaX; // 移動分を追加
        lMoveY += cDeltaY;

        // 地形取得
        lCol = pFunc( pData->pos_x + lMoveX, pData->pos_y + lMoveY, pData->flag, pData->dir, pData->attr );

        if ( lCol == 0) {
            lMoveX += cDeltaX; // 移動分を追加
            lMoveY += cDeltaY;

            // 地形取得
            lCol = pFunc( pData->pos_x + lMoveX, pData->pos_y + lMoveY, pData->flag, pData->dir, pData->attr );
            
            // それぞれ２キャラ分プラスして値を返す
            if ( lCol == 0) {
                if ( pData->dir)
                    *pData->dir = usDir;
                if ( pData->attr)
                    *pData->attr = ulAttr;
                return objMapGetForward(sPix, (s8)(cDeltaX + cDeltaY) ) + 16;
            }else if ( lCol == 8 ){
#if 1
                return objMapGetBack(sPix, (s8)(cDeltaX + cDeltaY)) + 16;
#else
				// 20090810 暫定対応 地形判定時に1ドット浮く事がある対応
				if (pData->vec == OBD_COL_DOWN || pData->vec == OBD_COL_UP) {
					return objMapGetBackFront(sPix, (s8)(cDeltaX + cDeltaY)) + 16;
				}
				else {
					return objMapGetBack(sPix, (s8)(cDeltaX + cDeltaY)) + 16;
				}
#endif
            }else{
                return objMapGetDiff(lCol, sPix, (s8)(cDeltaX + cDeltaY)) + 16;
            }
        }
        // それぞれ１キャラ分プラスして値を返す
        else if ( lCol == 8 ){
#if 1
            return objMapGetBack(sPix, (s8)(cDeltaX + cDeltaY)) + 8;
#else
			// 20090810 暫定対応 地形判定時に1ドット浮く事がある対応
			if (pData->vec == OBD_COL_DOWN || pData->vec == OBD_COL_UP) {
				return objMapGetBackFront(sPix, (s8)(cDeltaX + cDeltaY)) + 8;
			}
			else {
				return objMapGetBack(sPix, (s8)(cDeltaX + cDeltaY)) + 8;
			}
#endif
        }else{
            return objMapGetDiff(lCol, sPix, (s8)(cDeltaX + cDeltaY)) + 8;
        }

        
    }
    else if ( lCol == 8) {
        if ( pData->dir )
            usDir = *pData->dir;
        if ( pData->attr )
            ulAttr = *pData->attr;

        lMoveX -= cDeltaX; // 移動分を追加
        lMoveY -= cDeltaY;

        // 地形取得
        lCol = pFunc( pData->pos_x + lMoveX, pData->pos_y + lMoveY, pData->flag, pData->dir, pData->attr );

        if ( lCol == 8) {
            if ( pData->dir )
                usDir = *pData->dir;
            if ( pData->attr )
                ulAttr = *pData->attr;
            lMoveX -= cDeltaX; // 移動分を追加
            lMoveY -= cDeltaY;

            // 地形取得
            lCol = pFunc( pData->pos_x + lMoveX, pData->pos_y + lMoveY, pData->flag, pData->dir, pData->attr );

            // それぞれ２キャラ分マイナスして値を返す
            if ( lCol == 0) {
                if ( pData->dir)
                    *pData->dir = usDir;
                if ( pData->attr)
                    *pData->attr = ulAttr;
                return objMapGetForwardRev(sPix, (s8)(cDeltaX + cDeltaY)) - 16;
            }else if ( lCol == 8 ){
                return objMapGetBack(sPix, (s8)(cDeltaX + cDeltaY)) - 16;
            }else{
                return objMapGetDiff(lCol, sPix, (s8)(cDeltaX + cDeltaY)) - 16;
            }
        }
        // それぞれ１キャラ分マイナスして値を返す
        else if ( lCol == 0 ){
            // 追加
            if ( pData->dir)
                *pData->dir = usDir;
            if ( pData->attr)
                *pData->attr = ulAttr;
            return objMapGetForwardRev(sPix, (s8)(cDeltaX + cDeltaY)) - 8;
        }else{
            return objMapGetDiff(lCol, sPix, (s8)(cDeltaX + cDeltaY)) - 8;
        }
    }
    else {
        // 隙間有りブロックにHIT
        return objMapGetDiff( lCol, sPix, (s8)(cDeltaX + cDeltaY));
    }
    
}

#if !OBD_COL_NEW_DATA_TYPE
// ================================================================
// objGetCharData
/*!
  フラグ付き当たりキャラクタ番号（AGBスクリーンデータ）を返す
 
  @param lPosX [in] 座標
  @param lPosY [in] 座標
  @param ucSuf [in] 面の表裏(0: A面  1: B面) | 属性（すり抜け等）
 
  @return   フラグ付き当たりキャラクタ番号（AGBスクリーンデータ）
 
 */
// ================================================================
u32 objGetCharData( s32 lPosX, s32 lPosY, u8 ucSuf) 
{
    s32 lTempX  = lPosX >> 3;
    s32 lTempY  = lPosY >> 3;
    
    s32 lCharX,lCharY;
    s32 lBlockX,lBlockY;

    u32	ulBlockNo;
    u32	ulCharNo;
    u32 ulCharData;

    
    // ブロック座標を計算
    lBlockX = lTempX / OBD_MAP_BLOCK_CHAR_NUM_X;
    lBlockY = lTempY / OBD_MAP_BLOCK_CHAR_NUM_Y;

    // キャラ座標を計算
    lCharX = lTempX - (OBD_MAP_BLOCK_CHAR_NUM_X * lBlockX);
    lCharY = lTempY - (OBD_MAP_BLOCK_CHAR_NUM_Y * lBlockY);
    
    // ブロック座標からブロック番号を計算
    ulBlockNo = *(_obj_fcol->block_map_datap[ucSuf] + (_obj_fcol->map_block_num_x * lBlockY + lBlockX) );
    
    // キャラ座標からキャラ番号を計算  (Y*ブロックサイズ + X)
    ulCharNo  = (u32)((lCharY * OBD_MAP_BLOCK_CHAR_NUM_Y) + lCharX);
    
    // キャラデータを取得 ブロック先頭アドレス＋ブロック数（1ブロックサイズ ）＋キャラ数( 1キャラ 0x02byte)
    ulCharData = *(u16*)( (u32)_obj_fcol->block_datap
                          + ((ulBlockNo * OBD_MAP_CHAR_SIZE_X* OBD_MAP_CHAR_SIZE_Y * sizeof(u16)) + (ulCharNo << 1)) );
    
    return ulCharData;
}
#endif // #if !OBD_COL_NEW_DATA_TYPE

#if OBD_COL_NEW_DATA_TYPE
// ================================================================
// objGetMapBlockData
/*!
  マップブロック情報取得
 
  @param pos_y	[in] 座標
  @param pos_y	[in] 座標
  @param suf	[in] 面の表裏(0: A面  1: B面)
 
  @return   マップブロック情報取得
 
 */
// ================================================================
MP_BLOCK* objGetMapBlockData(s32 pos_x, s32 pos_y, u16 suf)
{
	s32			temp_x, temp_y;
	s32			block_x, block_y;
	s32			char_x, char_y;
	MP_BLOCK	*map_block;

	temp_x = pos_x >> 3;	// 1キャラ8ドット
	temp_y = pos_y >> 3;

	// ブロック座標を計算
	block_x = temp_x >> 3;	//  8キャラ1ブロック
	block_y = temp_y >> 3;

	// キャラ座標を計算
	char_x = temp_x - block_x * OBD_MAP_BLOCK_CHAR_NUM_X;
	char_y = temp_y - block_y * OBD_MAP_BLOCK_CHAR_NUM_Y;

	// ブロック座標からブロックデータ取得
	map_block = (_obj_fcol->block_map_datap[suf] + _obj_fcol->map_block_num_x * block_y + block_x);

	return (map_block);
}

// ================================================================
// objGetDiffCharData
/*!
  差分情報取得
 
  @param pos_y	[in] 座標
  @param pos_y	[in] 座標
  @param suf	[in] 面の表裏(0: A面  1: B面)
 
  @return   キャラ単位差分情報取得
 
 */
// ================================================================
u8* objGetDiffCharData(s32 pos_x, s32 pos_y, u16 suf)
{
	s32			temp_x, temp_y;
	s32			block_x, block_y;
	s32			char_x, char_y;
	MP_BLOCK	*map_block;
	DF_BLOCK	*diff_block;

	s32			b_char_x, b_char_y;
	s32			block_no, rot;
	u8			*diff_char;

	temp_x = pos_x >> 3;	// 1キャラ8ドット
	temp_y = pos_y >> 3;

	// ブロック座標を計算
	block_x = temp_x >> 3;	//  8キャラ1ブロック
	block_y = temp_y >> 3;

	// キャラ座標を計算
	char_x = temp_x - block_x * OBD_MAP_BLOCK_CHAR_NUM_X;
	char_y = temp_y - block_y * OBD_MAP_BLOCK_CHAR_NUM_Y;

	// ブロック座標からブロックデータ取得
	map_block = (_obj_fcol->block_map_datap[suf] + _obj_fcol->map_block_num_x * block_y + block_x);
	block_no	= map_block->id;
	rot			= map_block->rot;

	// ブロック内キャラ位置を取得
	objGetConv88Pos(char_x, char_y, map_block, &b_char_x, &b_char_y);

	// 差分情報ブロック取得
	diff_block = _obj_fcol->cl_diff_datap + block_no;

	// 差分情報(キャラ)取得
	diff_char = &(diff_block->df[b_char_y][b_char_x][0]);

	return (diff_char);
}

// ================================================================
// objGetXDiffData
/*!
  X差分情報取得
 
  @param pos_y	[in] 座標
  @param pos_y	[in] 座標
  @param suf	[in] 面の表裏(0: A面  1: B面)
 
  @return   X差分情報
 */
// ================================================================
s8 objGetXDiffData(s32 pos_x, s32 pos_y, u16 suf)
{
	s32			pix_x, pix_y;
	MP_BLOCK	*map_block;
	u8			*diff_char;
	s8			diff;

	// 差分情報(キャラ)取得
	diff_char = objGetDiffCharData(pos_x, pos_y, suf);

	// キャラ中のドット位置を取得
	pix_x = pos_x & 0x07;
	pix_y = pos_y & 0x07;

	// マップブロック取得
	map_block = objGetMapBlockData(pos_x, pos_y, suf);

	// 回転, 反転を反映して差分情報を取得
	objGetConv88Pos(pix_x, pix_y, map_block, &pix_x, &pix_y);

	// Y位置のX差分情報を取得
	if (map_block->rot & 0x01) {
		diff = (s8)*(diff_char + pix_x);	// 90, 270回転時はXYが逆になる 
	}
	else {
		diff = (s8)*(diff_char + pix_y);
	}

	// 地形データ回転・反転コンバート
	diff = (s8)(objGetConvDiff(map_block, (u8)(diff)));

	diff &= 0x0F;		// Y軸差分を削除

	// 4bitデータを8bitに復旧
	if (diff & 0x08) {
		diff |= ~0x0f;
	}
	// 指定差分値が地形のみの値（-8）なら絶対値を設定
	if (diff == -8) {
		diff = 8;
	}

	return (diff);
}

// ================================================================
// objGetYDiffData
/*!
  Y差分情報取得
 
  @param pos_y	[in] 座標
  @param pos_y	[in] 座標
  @param suf	[in] 面の表裏(0: A面  1: B面)
 
  @return   Y差分情報
 */
// ================================================================
s8 objGetYDiffData(s32 pos_x, s32 pos_y, u16 suf)
{
	s32			pix_x, pix_y;
	MP_BLOCK	*map_block;
	u8			*diff_char;
	s8			diff;

	// 差分情報(キャラ)取得
	diff_char = objGetDiffCharData(pos_x, pos_y, suf);

	// キャラ中のドット位置を取得
	pix_x = pos_x & 0x07;
	pix_y = pos_y & 0x07;

	// マップブロック取得
	map_block = objGetMapBlockData(pos_x, pos_y, suf);

	// 回転, 反転を反映して差分情報を取得
	objGetConv88Pos(pix_x, pix_y, map_block, &pix_x, &pix_y);

	// X位置のY差分情報取得
	if (map_block->rot & 0x01) {
		diff = (s8)*(diff_char + pix_y);	// 90, 270回転時はXYが逆になる
	}
	else {
		diff = (s8)*(diff_char + pix_x);
	}

	// 地形データ回転・反転コンバート
	diff = (s8)(objGetConvDiff(map_block, (u8)(diff)));

	diff = (s8)((diff >> 4) & 0x0F);		// X軸差分を削除

	// 4bitデータを8bitに復旧
	if (diff & 0x08) {
		diff |= ~0x0f;
	}
	// 指定差分値が地形のみの値（-8）なら絶対値を設定
	if (diff == -8) {
		diff = 8;
	}

	return (diff);
}

// ================================================================
// objGetDirData
/*!
  角度情報取得
 
  @param pos_y	[in] 座標
  @param pos_y	[in] 座標
  @param suf	[in] 面の表裏(0: A面  1: B面)
 
  @return   角度情報
 */
// ================================================================
u16 objGetDirData(s32 pos_x, s32 pos_y, u16 suf)
{
	s32			temp_x, temp_y;
	s32			block_x, block_y;
	s32			char_x, char_y;
	MP_BLOCK	*map_block;
	DI_BLOCK	*di_block;

	s32			b_char_x, b_char_y;
	s32			block_no, rot;
	u16			dir;

	temp_x = pos_x >> 3;	// 1キャラ8ドット
	temp_y = pos_y >> 3;

	// ブロック座標を計算
	block_x = temp_x >> 3;	//  8キャラ1ブロック
	block_y = temp_y >> 3;

	// キャラ座標を計算
	char_x = temp_x - block_x * OBD_MAP_BLOCK_CHAR_NUM_X;
	char_y = temp_y - block_y * OBD_MAP_BLOCK_CHAR_NUM_Y;

	// ブロック座標からブロックデータ取得
	map_block = (_obj_fcol->block_map_datap[suf] + _obj_fcol->map_block_num_x * block_y + block_x);
	block_no	= map_block->id;
	rot			= map_block->rot;

	// ブロック内キャラ位置を取得
	objGetConv88Pos(char_x, char_y, map_block, &b_char_x, &b_char_y);

	// 角度情報ブロック取得
	di_block = _obj_fcol->direc_datap + block_no;

	// 角度情報取得
	dir = (u16)(di_block->di[b_char_y][b_char_x] << 8);	// 8bit > 16bit
#if 1
	// フリップを反映
	if (map_block->flip_h) {
		dir = (u16)(-(s16)dir);
	}
	if (map_block->flip_v) {
		dir = (u16)(-((s16)dir + 0x4000) - 0x4000);
	}

	// 回転を反映
	dir = (u16)(dir + rot*0x4000);

	dir = (u16)(-(s16)dir);//(u16)(0x10000 - dir);

#if 0
	if (dir & 0x2000) {	// ◆暫定処理 0x2000 0x6000 0xA000 0xE000 (45度系)をずらす
		dir -= 0x100;
#else
	if (!((dir & ~0xC000) ^ 0x2000)) {		// ◆ 0x2000 0x6000 0xA000 0xE000 (45度系)なら横にずらす
		if (dir & 0x4000)	dir += 0x100;	// 0x6000 0xE000 なら MINUS
		else				dir -= 0x100;	// 0x2000 0xA000 なら PLUS
#endif
	}

#else
#if 0
	if (dir & 0x2000) {	// ◆暫定処理 0x2000 0x6000 0xA000 0xE000 (45度系)をずらす
		dir -= 0x100;
#else
	if (!((dir & ~0xC000) ^ 0x2000)) {		// ◆ 0x2000 0x6000 0xA000 0xE000 (45度系)なら横にずらす
		if (dir & 0x4000)	dir += 0x100;	// 0x6000 0xE000 なら MINUS
		else				dir -= 0x100;	// 0x2000 0xA000 なら PLUS
#endif
	}

	dir = (u16)(-(s16)dir);//(u16)(0x10000 - dir);

	// 回転を反映
	dir = (u16)(dir - rot*0x4000);

	// フリップを反映
	if (map_block->flip_h) {
		dir = (u16)(-(s16)dir);
	}
	if (map_block->flip_v) {
		dir = (u16)(-((s16)dir + 0x4000) - 0x4000);
	}
#endif

	return (dir);
}

// ================================================================
// objGetConv88Pos
/*!
  回転・反転を反映した 8*8範囲データ参照位置
 
  @param pos_x			[in]	変換前X位置座標(0～7)
  @param pos_y			[in]	変換前Y位置座標(0～7)
  @param mp_block		[in]	変換用マップデータ
  @param conv_pos_x	[out]	変換後X位置格納バッファ NULL不可
  @param conv_pos_y	[out]	変換後Y位置格納バッファ NULL不可

  @note
	マップデータに従い、参照キャラ位置を取得します。\n
	変換データ範囲は 8*8 (0～7) です。
 
 */
// ================================================================
void objGetConv88Pos(s32 pos_x, s32 pos_y, MP_BLOCK *mp_block, s32 *conv_pos_x, s32 *conv_pos_y)
{
	s32	temp_pos_x, temp_pos_y;

	// 回転
	switch (mp_block->rot) {
	default:
		MTM_ASSERT(0);
	case 0:
		temp_pos_x = pos_x;
		temp_pos_y = pos_y;
		break;

	case 1:
		temp_pos_x = 7 - pos_y;
		temp_pos_y = pos_x;
		break;

	case 2:
		temp_pos_x = 7 - pos_x;
		temp_pos_y = 7 - pos_y;
		break;

	case 3:
		temp_pos_x = pos_y;
		temp_pos_y = 7 - pos_x;
		break;
	}

	// 反転
	if (mp_block->flip_h) {
		temp_pos_x = 7 - temp_pos_x;
	}
	if (mp_block->flip_v) {
		temp_pos_y = 7 - temp_pos_y;
	}

	*conv_pos_x = temp_pos_x;
	*conv_pos_y = temp_pos_y;
}

// ================================================================
// objGetConvDiff
/*!
  回転・反転を反映した 差分データ作成
 
  @param mp_block	[in]	変換用マップデータ
  @param diff		[in]	変換する差分情報

  @return	変換後差分情報

  @note
	参照データ(diff)は、回転・反転後の位置である必要があります。
 
 */
// ================================================================
u8 objGetConvDiff(MP_BLOCK *mp_block, u8 diff)
{
	u8	temp_diff;

	u8	diff_x, diff_y;

	diff_x = (u8)(diff & 0x0F);
	diff_y = (u8)((diff >> 4) & 0x0F);

	// 反転
	if (mp_block->flip_h) {
		if ((diff_x & 0x07)) {
			diff_x = (u8)((diff_x + 8) & 0x0F);
		}
	}
	if (mp_block->flip_v) {
		if ((diff_y & 0x07)) {
			diff_y = (u8)((diff_y + 8) & 0x0F);
		}
	}

	// 回転
	switch (mp_block->rot) {
	default:
		MTM_ASSERT(0);
	case 0:
		break;

	case 1:
		// x > y(f), y > x
		temp_diff = diff_x;
		diff_x = diff_y;
		diff_y = temp_diff;
		if ((diff_y & 0x07)) {
			diff_y = (u8)((diff_y + 8) & 0x0F);
		}
		break;

	case 2:
		// x > x(f), y > y(f)
		if ((diff_x & 0x07)) {
			diff_x = (u8)((diff_x + 8) & 0x0F);
		}
		if ((diff_y & 0x07)) {
			diff_y = (u8)((diff_y + 8) & 0x0F);
		}
		break;

	case 3:
		// x > y, y > x(f)
		temp_diff = diff_x;
		diff_x = diff_y;
		diff_y = temp_diff;
		if ((diff_x & 0x07)) {
			diff_x = (u8)((diff_x + 8) & 0x0F);
		}
		break;
	}

	return (u8)(((diff_x & 0x0F) | (diff_y << 4)));
}

#endif //OBD_COL_NEW_DATA_TYPE

// ================================================================
// objMapGetDiff
/*!
  地表までの距離を求める
 
  @param lCol [in] 地形差分値
  @param sPix [in] キャラ辺りの座標位置
 
  @return   距離 ドット単位
 
  @note
  当たりあり
 */
// ================================================================
inline s32 objMapGetDiff( s32 lCol, s8 sPix, s8 sDelta )
{
    s32 lRet;
    
    /*if ( lCol > 0){
        // 浮いている
        lRet = lCol - ( sPix + 1 );
    }else{
        // 埋まっている
        lRet = lCol + sPix;
    }*/
    if ( lCol > 0){
        if ( sDelta > 0 ){
            // 浮いている
            lRet = lCol - ( sPix + 1 );
        }else{
            // 埋まっている
            lRet = (8 - sPix);
        }
    }else{
        if ( sDelta > 0 ){
            // 埋まっている
            lRet = -(sPix + 1);
        }else{
            // 浮いている
            lRet = lCol + sPix;
        }
        
    }
    
    return lRet;
}


// ================================================================
// objMapGetForward
/*!
  地表までの距離を求める
 
  @param sPix [in] キャラ辺りの座標位置
  @param sDelta [in] 進行方向
 
  @return   距離 ドット単位
 
  @note
  当たりなし、次のキャラへ移行する 
 */
// ================================================================
inline s32 objMapGetForward(s8 sPix, s8 sDelta )
{
    s32 lRet;

    
    if ( sDelta > 0){
        
        lRet = 8 - sPix;
    }else{
        lRet = 1 + sPix;
    }
    
    return lRet;
}

// ================================================================
// objMapGetBack
/*!
  地表までの距離を求める
 
  @param sPix [in] キャラ辺りの座標位置
  @param sDelta [in] 進行方向
 
  @return   距離 ドット単位
 
  @note
  当たり全埋まり、次のキャラへ移行する 
 */
// ================================================================
inline s32 objMapGetBack(s8 sPix, s8 sDelta )
{
    s32 lRet;

    if ( sDelta > 0){
        lRet = -(sPix + 1);
    }else{
        lRet = sPix - 8;
    }
    
    return lRet;
}

// ================================================================
// objMapGetBack
/*!
  地表までの距離を求める
 
  @param sPix [in] キャラ辺りの座標位置
  @param sDelta [in] 進行方向
 
  @return   距離 ドット単位
 
  @note
  当たり全埋まり、前のキャラへ移行する 
 */
// ================================================================
inline s32 objMapGetBackFront(s8 sPix, s8 sDelta )
{
    s32 lRet;

    if ( sDelta > 0){
        lRet = -(sPix);
    }else{
        lRet = sPix - 8;
    }
    
    return lRet;
}
// ================================================================
// objMapGetForwardRev
/*!
  地表までの距離を求める
 
  @param sPix [in] キャラ辺りの座標位置
  @param sDelta [in] 進行方向
 
  @return   距離 ドット単位
 
  @note
  当たりなしだが，次のキャラへは移行しない
 */
// ================================================================
inline s32 objMapGetForwardRev(s8 sPix, s8 sDelta )
{
    s32 lRet;

    if ( sDelta > 0){
        lRet = 8 - (sPix + 1);
    }else{
        lRet = sPix;
    }
    
    return lRet;
}
// ================================================================
// objGetColDataX
/*!
  指定した位置の地形データを取得する
 
  @param lPosX     [in] X座標
  @param lPosY     [in] Y座標
  @param ucSuf     [in] BG面 0x80のビットが立っている場合はすり抜け床をすり抜ける
  @param pDir      [out] 角度情報
 
  @return   X軸の地形差分値
 
 */
// ================================================================
static s32 objGetColDataX( s32 lPosX, s32 lPosY, u16 ucSuf, u16* pDir, u32 * pAttr )
{
#if OBD_COL_NEW_DATA_TYPE
#if 0	// (元の状態 10.01.05 kuramoto)
		// マップ外を壁とする場合に、
		// マップ外壁に当たりそうな時地形側壁判定を無視するケースが見られたので、
		// 地形側壁とマップ外壁を比較して優先される値を返すよう修正変更
    s8  cCol;		// 地形差分値
	u8	attr = 0;

    if ( ucSuf & OBD_COL_LIMITWALL ){
        // マップ外を壁扱いにする
        if (((s32)((lPosX & 0xfffffff8)) > (s32)(_obj_fcol->right  - 1)) ||
            ((s32)lPosX < (s32)_obj_fcol->left - 7) ){
            cCol = 8;
			return cCol;
        }
        if (((s32)((lPosX & 0xfffffff8)+0x8) > (s32)(_obj_fcol->right - 1)) ){
            cCol = (s8)((_obj_fcol->right-1) & 0x7);
            if ( !cCol )
                cCol = 8;
			return cCol;
        }
        if (((s32)(lPosX & 0xfffffff8) < (s32)_obj_fcol->left) ){
            if ( _obj_fcol->left & 0x7 )
                cCol = (s8)(0x8 + (0x8 - (_obj_fcol->left & 0x7)));
            else
                cCol = 8;
            cCol |= ~0x0f;
            if (cCol == -8)
                cCol = 8;
			return cCol;
        }
        if (((s32)lPosY > (s32)(_obj_fcol->bottom - 1)) ||
            ((s32)lPosY < (s32)_obj_fcol->top) ){
            cCol = 8;
			return cCol;
        }
    }else{
        // 座標クリッピング
        lPosX = (s32)MTM_MATH_CLIP(lPosX, _obj_fcol->left, _obj_fcol->right - 1);
        lPosY = (s32)MTM_MATH_CLIP(lPosY, _obj_fcol->top, _obj_fcol->bottom - 1);
    }

	// 差分データ取得
	cCol = objGetXDiffData(lPosX, lPosY, (u16)(ucSuf & 0x01));

    // すり抜け地形をすり抜け設定で、属性がすり抜け地形であれば地形は空にする
	attr = objGetAttrData(lPosX, lPosY, (u16)(ucSuf & 0x01));
    if ((ucSuf & OBD_COL_THROUGH) && (attr & OBD_COL_DATA_ATTR_THROUGH) ){
    // if ((ucSuf & OBD_COL_THROUGH) && (objGetAttrData(usCharNo) == 2 ) ){
        cCol = 0;
    }

	// 角度情報チェック
	if (pDir && cCol) {
		*pDir = objGetDirData(lPosX, lPosY, (u16)(ucSuf & 0x01));
	}

	// 属性情報チェック
	if (pAttr && cCol) {
		*pAttr = attr;
	}

    return cCol;
#else	// (修正後 10.01.05 kuramoto)
		// マップ外を壁とする場合に、
		// マップ外壁に当たりそうな時地形側壁判定を無視するケースが見られたので、
		// 地形側壁とマップ外壁を比較して優先される値を返すよう修正変更
    s8  cCol;		// 地形差分値
    s8  cColF = 0;	// 地形差分値(マップ外用)
	u8	attr = 0;

    if ( ucSuf & OBD_COL_LIMITWALL ){
        // マップ外を壁扱いにする
        if (((s32)((lPosX & 0xfffffff8)) > (s32)(_obj_fcol->right  - 1)) ||
            ((s32)lPosX < (s32)_obj_fcol->left - 7) ){
            cColF = 8;
        } else
        if (((s32)((lPosX & 0xfffffff8)+0x8) > (s32)(_obj_fcol->right - 1)) ){
            cColF = (s8)((_obj_fcol->right-1) & 0x7);
            if ( !cColF )
                cColF = 8;
        } else
        if (((s32)(lPosX & 0xfffffff8) < (s32)_obj_fcol->left) ){
            if ( _obj_fcol->left & 0x7 )
                cColF = (s8)(0x8 + (0x8 - (_obj_fcol->left & 0x7)));
            else
                cColF = 8;
            cColF |= ~0x0f;
            if (cColF == -8)
                cColF = 8;
#if 0	// 左画面壁判定修正(before)10/01/20@kuramoto
		// X判定関数の中でY成分見ているのがそもそも間違い？
		// 不具合要因となっていた為削除
        } else
        if (((s32)lPosY > (s32)(_obj_fcol->bottom - 1)) ||
            ((s32)lPosY < (s32)_obj_fcol->top) ){
            cColF = 8;
#endif
        }
    }else{
        // 座標クリッピング
        lPosX = (s32)MTM_MATH_CLIP(lPosX, _obj_fcol->left, _obj_fcol->right - 1);
        lPosY = (s32)MTM_MATH_CLIP(lPosY, _obj_fcol->top, _obj_fcol->bottom - 1);
    }

	// 差分データ取得
	cCol = objGetXDiffData(lPosX, lPosY, (u16)(ucSuf & 0x01));

    // すり抜け地形をすり抜け設定で、属性がすり抜け地形であれば地形は空にする
	attr = objGetAttrData(lPosX, lPosY, (u16)(ucSuf & 0x01));
    if ((ucSuf & OBD_COL_THROUGH) && (attr & OBD_COL_DATA_ATTR_THROUGH) ){
    // if ((ucSuf & OBD_COL_THROUGH) && (objGetAttrData(usCharNo) == 2 ) ){
        cCol = 0;
    }

	// 地形とマップ外を比較してマップ外の方が優先されるならマップ外を値として返す
	if (MTM_MATH_ABS(cCol) < MTM_MATH_ABS(cColF)) {
		return cColF;
	}
	// 角度情報チェック
	if (pDir && cCol) {
		*pDir = objGetDirData(lPosX, lPosY, (u16)(ucSuf & 0x01));
	}

	// 属性情報チェック
	if (pAttr && cCol) {
		*pAttr = attr;
	}

    return cCol;
#endif
#else

    s32 ulPix; // キャラ辺りのドット位置
    s8  cCol;   // 地形差分値
    u16 usCharNo; // キャラ番号
    u16 usCharData;

    if ( ucSuf & OBD_COL_LIMITWALL ){
        // マップ外を壁扱いにする
        if (((s32)((lPosX & 0xfffffff8)) > (s32)(_obj_fcol->right  - 1)) ||
            ((s32)lPosX < (s32)_obj_fcol->left - 7) ){
            cCol = 8;
            return cCol;
        }

        if (((s32)((lPosX & 0xfffffff8)+0x8) > (s32)(_obj_fcol->right - 1)) ){
            cCol = (s8)((_obj_fcol->right-1) & 0x7);
            if ( !cCol )
                cCol = 8;
            return cCol;
        }
        if (((s32)(lPosX & 0xfffffff8) < (s32)_obj_fcol->left) ){
            if ( _obj_fcol->left & 0x7 )
                cCol = (s8)(0x8 + (0x8 - (_obj_fcol->left & 0x7)));
            else
                cCol = 8;
            cCol |= ~0x0f;
            if (cCol == -8)
                cCol = 8;
            return cCol;
        }
        if (((s32)lPosY > (s32)(_obj_fcol->bottom - 1)) ||
            ((s32)lPosY < (s32)_obj_fcol->top) ){
            cCol = 8;
            return cCol;
        }
    }else{
        // 座標クリッピング
        lPosX = (s32)MTM_MATH_CLIP(lPosX, _obj_fcol->left, _obj_fcol->right - 1);
        lPosY = (s32)MTM_MATH_CLIP(lPosY, _obj_fcol->top, _obj_fcol->bottom - 1);
    }

    // キャラデータ取得
    usCharData = (u16)objGetCharData(lPosX, lPosY, (u8)(ucSuf & 0x01));
    // キャラ番号抽出
    usCharNo = (u16)( usCharData & BG_SC_CHAR_NO_MASK);

    // １キャラ辺りのドット位置を取得
    ulPix = lPosY & 0x07;

    if ( usCharData & BG_SC_V_FLIP ){
        ulPix = 7 - ulPix; // 座標もVフリップ
    }
  
    // 地形差分テーブルから指定座標の差分値を取得
    cCol =  _obj_fcol->cl_diff_datap[ (usCharNo << 3) + ulPix ];
    cCol &=  0x0f; // Y軸の差分値を削除
    
    // 8bitのマイナス値に設定（データは4bitなので自前でマイナスにする）
    if (cCol & 0x08)
        cCol |= ~0x0f;

    // 指定差分値が地形のみの値（-8）なら絶対値を設定
    if (cCol == -8)
        cCol = 8;
    
    // すり抜け地形をすり抜け設定で、属性がすり抜け地形であれば地形は空にする
    if ((ucSuf & OBD_COL_THROUGH) && (objGetAttrData(usCharNo) & OBD_COL_DATA_ATTR_THROUGH) ){
    // if ((ucSuf & OBD_COL_THROUGH) && (objGetAttrData(usCharNo) == 2 ) ){
        cCol = 0;
    }

    if ( (usCharData & BG_SC_H_FLIP) ) {
        // 地形差分値をHフリップ
        if (!( cCol == 8 || cCol == 0 )){
            if ( cCol > 0 ){
                cCol -= 8;
            }else{
                cCol += 8;
            }
        }
    }
    // 角度取得チェック
    if ( pDir && cCol ){
        u16 ucDir;
        // 角度データを取得
        ucDir = (u16)(*(_obj_fcol->direc_datap + usCharNo) << 8);	// 8bit >> 16bit
        if (usCharData & BG_SC_V_FLIP) {
            // 角度情報をVフリップ
            ucDir = (u16)(-((s16)ucDir + 0x4000) - 0x4000);
        }

        if ((usCharData & BG_SC_H_FLIP)) {
            // 角度情報をHフリップ
            if ( cCol )
                ucDir = (u16)(-(s16)ucDir);
        }
        // 角度情報を設定
        *pDir = ucDir;
    }
    if ( pAttr && cCol  )
        *pAttr = objGetAttrData(usCharNo);
    return cCol;
#endif
}
// ================================================================
// objGetColDataY
/*!
  指定した位置の地形データを取得する
 
  @param lPosX     [in] X座標
  @param lPosY     [in] Y座標
  @param ucSuf     [in] BG面 0x80のビットが立っている場合はすり抜け床をすり抜ける
  @param pDir      [out] 角度情報
  @param pDir      [out] 属性情報
 
  @return   Y軸の地形差分値
 
 */
// ================================================================
static s32  objGetColDataY( s32 lPosX, s32 lPosY,u16 ucSuf, u16* pDir, u32 * pAttr)
{
#if OBD_COL_NEW_DATA_TYPE
    s8 cCol;   // 地形差分値
	u8	attr = 0;

    if ( ucSuf & OBD_COL_LIMITWALL ){
        // マップ外を壁扱いにする
        if (((s32)((lPosY & 0xfffffff8)) > (s32)(_obj_fcol->bottom  - 1)) ||
            ((s32)lPosY < (s32)_obj_fcol->top - 7) ){
            cCol = 8;
            return cCol;
        }
        if (((s32)((lPosY & 0xfffffff8)+0x8) > (s32)(_obj_fcol->bottom - 1)) ){
            cCol = (s8)((_obj_fcol->bottom-1) & 0x7);
            if ( !cCol )
                cCol = 8;
            return cCol;
        }
        if (((s32)(lPosY & 0xfffffff8) < (s32)_obj_fcol->top) ){
            if ( _obj_fcol->top & 0x7 )
                cCol = (s8)(0x8 + (0x8 - (_obj_fcol->top & 0x7)));
            else
                cCol = 8;
            cCol |= ~0x0f;
            if (cCol == -8)
                cCol = 8;
            return cCol;
        }
#if 0	// 左画面壁判定修正(after)10/01/20@kuramoto
		// Y判定関数の中でX成分見ているのがそもそも間違い？
		// 不具合要因となっていた為削除
        if (((s32)lPosX > (s32)(_obj_fcol->right - 1)) ||
            ((s32)lPosX < (s32)_obj_fcol->left) ){
            cCol = 8;
            return cCol;
        }
#endif
    }else{
        // 座標クリッピング
        lPosX = (s32)MTM_MATH_CLIP(lPosX, _obj_fcol->left, _obj_fcol->right - 1);
        lPosY = (s32)MTM_MATH_CLIP(lPosY, _obj_fcol->top, _obj_fcol->bottom - 1);
    }
    
	// 差分データ取得
	cCol = objGetYDiffData(lPosX, lPosY, (u16)(ucSuf & 0x01));

    // すり抜け地形をすり抜け設定で、属性がすり抜け地形であれば地形は空にする
	attr = objGetAttrData(lPosX, lPosY, (u16)(ucSuf & 0x01));
    if ((ucSuf & OBD_COL_THROUGH) && (attr & OBD_COL_DATA_ATTR_THROUGH) ){
    // if ((ucSuf & OBD_COL_THROUGH) && (objGetAttrData(usCharNo) == 2 ) ){
        cCol = 0;
    }

    // 角度取得チェック
	if (pDir && cCol) {
		*pDir = objGetDirData(lPosX, lPosY, (u16)(ucSuf & 0x01));
	}

    if ( pAttr && cCol  )
        *pAttr = objGetAttrData(lPosX, lPosY, (u16)(ucSuf & 0x01));
    
    return cCol;
#else
    s8 cCol;   // 地形差分値
    s32 ulPix; // キャラ辺りのドット位置
    u16 usCharNo; // キャラ番号
    u16 usCharData;

    if ( ucSuf & OBD_COL_LIMITWALL ){
        // マップ外を壁扱いにする
        if (((s32)((lPosY & 0xfffffff8)) > (s32)(_obj_fcol->bottom  - 1)) ||
            ((s32)lPosY < (s32)_obj_fcol->top - 7) ){
            cCol = 8;
            return cCol;
        }
        if (((s32)((lPosY & 0xfffffff8)+0x8) > (s32)(_obj_fcol->bottom - 1)) ){
            cCol = (s8)((_obj_fcol->bottom-1) & 0x7);
            if ( !cCol )
                cCol = 8;
            return cCol;
        }
        if (((s32)(lPosY & 0xfffffff8) < (s32)_obj_fcol->top) ){
            if ( _obj_fcol->top & 0x7 )
                cCol = (s8)(0x8 + (0x8 - (_obj_fcol->top & 0x7)));
            else
                cCol = 8;
            cCol |= ~0x0f;
            if (cCol == -8)
                cCol = 8;
            return cCol;
        }
        if (((s32)lPosX > (s32)(_obj_fcol->right - 1)) ||
            ((s32)lPosX < (s32)_obj_fcol->left) ){
            cCol = 8;
            return cCol;
        }
    }else{
        // 座標クリッピング
        lPosX = (s32)MTM_MATH_CLIP(lPosX, _obj_fcol->left, _obj_fcol->right - 1);
        lPosY = (s32)MTM_MATH_CLIP(lPosY, _obj_fcol->top, _obj_fcol->bottom - 1);
    }
    // キャラデータ取得
    usCharData = (u16)objGetCharData(lPosX, lPosY, (u8)(ucSuf & 0x01));
    // キャラ番号取得
    usCharNo = (u16)( usCharData & BG_SC_CHAR_NO_MASK );

    // １キャラ辺りのドット位置を取得
    ulPix = lPosX & 0x07;

    if ( ( usCharData & BG_SC_H_FLIP ))
        ulPix = 7 - ulPix; // 座標もHフリップ
  
    // 地形差分テーブルから指定座標の差分値を取得
    cCol =  _obj_fcol->cl_diff_datap[ (usCharNo << 3) + ulPix ];
    cCol >>= 4; // X軸の差分値を削除
    
    // 8bitのマイナス値に設定（データは4bitなので自前でマイナスにする）
    if (cCol & 0x08)
        cCol |= ~0x0f;

    // 指定差分値が地形のみの値（-8）なら絶対値を設定
    if (cCol == -8)
        cCol = 8;
    // すり抜け地形をすり抜け設定で、属性がすり抜け地形であれば地形は空にする
    if ((ucSuf & OBD_COL_THROUGH) && (objGetAttrData(usCharNo) & OBD_COL_DATA_ATTR_THROUGH) ){
    // if ((ucSuf & OBD_COL_THROUGH) && (objGetAttrData(usCharNo) == 2 ) ){
        cCol = 0;
    }

    if ( usCharData & BG_SC_V_FLIP ) {
        // 地形差分値をVフリップ
        if (!( cCol == 8 || cCol == 0 )){
            if ( cCol > 0 ){
                cCol -= 8;
            }else{
                cCol += 8;
            }
        }
    }
    // 角度取得チェック
    if ( pDir && cCol ){
        u16 ucDir;
        // 角度データを取得
        ucDir = (u16)(*(_obj_fcol->direc_datap + usCharNo) << 8);		// 8bit >> 16bit
        
        if ((usCharData & BG_SC_H_FLIP)) {
            // 角度情報をHフリップ
            ucDir = (u16)(-(s16)ucDir);
        }
        if (usCharData & BG_SC_V_FLIP) {
            // 角度情報をVフリップ
            if (cCol != 0){
                ucDir = (u16)(-((s16)ucDir + 0x4000) - 0x4000);
            }
        }

        // 角度情報を設定
        *pDir = ucDir;
    }

    if ( pAttr && cCol  )
        *pAttr = objGetAttrData(usCharNo);
    
    return cCol;
#endif
}

#if OBD_COL_NEW_DATA_TYPE
// ================================================================
// objGetAttrData
/*!
  指定したキャラ番号の属性を取得する
 
  @param pos_x	[in] X座標
  @param pos_y	[in] Y座標
  @param suf	[in] BG面
 
  @return   属性値
 */
// ================================================================
u8 objGetAttrData(s32 pos_x, s32 pos_y, u16 suf)
{
	s32			temp_x, temp_y;
	s32			block_x, block_y;
	s32			char_x, char_y;
	MP_BLOCK	*map_block;
	AT_BLOCK	*attr_block;

	s32			b_char_x, b_char_y;
	s32			block_no;
	u8			attr;

	temp_x = pos_x >> 3;	// 1キャラ8ドット
	temp_y = pos_y >> 3;

	// ブロック座標を計算
	block_x = temp_x >> 3;	//  8キャラ1ブロック
	block_y = temp_y >> 3;

	// キャラ座標を計算
	char_x = temp_x - block_x * OBD_MAP_BLOCK_CHAR_NUM_X;
	char_y = temp_y - block_y * OBD_MAP_BLOCK_CHAR_NUM_Y;

	// ブロック座標からブロックデータ取得
	map_block = (_obj_fcol->block_map_datap[suf] + _obj_fcol->map_block_num_x * block_y + block_x);
	block_no	= map_block->id;

	// ブロック内キャラ位置を取得
	objGetConv88Pos(char_x, char_y, map_block, &b_char_x, &b_char_y);

	// 属性情報ブロック取得
	attr_block = _obj_fcol->char_attr_datap + block_no;

	// 差分情報(キャラ)取得
	attr = attr_block->at[b_char_y][b_char_x];

	return (attr);
}
#else
// ================================================================
// objGetAttrData
/*!
  指定したキャラ番号の属性を取得する
 
  @param usCharNo [in] キャラ番号
 
  @return   属性値 0or1
 
 */
// ================================================================
inline u8 objGetAttrData( u16 usCharNo )
{
    return (u8)( ( (_obj_fcol->char_attr_datap[ (usCharNo /*<< 3*/)])/* >> ( (usCharNo & 0x7) << 1)) & 0x3 */));
    
}
#endif

// ================================================================
// ObjGetColDataDir
/*!
  指定した位置の角度を取得する
 
  @param lPosX     [in] X座標
  @param lPosY     [in] Y座標
  @param ucSuf     [in] BG面
 
  @return   角度
 
 */
// ================================================================
u16 ObjGetColDataDir( s32 lPosY, s32 lPosX, u8 ucSuf )
{
#if OBD_COL_NEW_DATA_TYPE
    
	return (objGetDirData(lPosX, lPosY, ucSuf));

#else
    s8 cCol;   // 地形差分値
    u16 usCharNo; // キャラ番号
    u16 usCharData;
    u16 ucDir;

    if ( ucSuf & OBD_COL_LIMITWALL ){
        // マップ外を壁扱いにする
        if (((s32)((lPosY & 0xfffffff8)) > (s32)(_obj_fcol->bottom  - 1)) ||
            ((s32)lPosY < (s32)_obj_fcol->top - 7) ){
            cCol = 8;
            return cCol;
        }
        if (((s32)((lPosY & 0xfffffff8)+0x8) > (s32)(_obj_fcol->bottom - 1)) ){
            cCol = (s8)((_obj_fcol->bottom-1) & 0x7);
            if ( !cCol )
                cCol = 8;
            return cCol;
        }
        if (((s32)(lPosY & 0xfffffff8) < (s32)_obj_fcol->top) ){
            if ( _obj_fcol->top & 0x7 )
                cCol = (s8)(0x8 + (0x8 - (_obj_fcol->top & 0x7)));
            else
                cCol = 8;
            cCol |= ~0x0f;
            if (cCol == -8)
                cCol = 8;
            return cCol;
        }
        if (((s32)lPosX > (s32)(_obj_fcol->right - 1)) ||
            ((s32)lPosX < (s32)_obj_fcol->left) ){
            cCol = 8;
            return cCol;
        }
    }else{
        // 座標クリッピング
        lPosX = (s32)MTM_MATH_CLIP(lPosX, _obj_fcol->left, _obj_fcol->right - 1);
        lPosY = (s32)MTM_MATH_CLIP(lPosY, _obj_fcol->top, _obj_fcol->bottom - 1);
    }
    // キャラデータ取得
    usCharData = (u16)objGetCharData(lPosX, lPosY, (u8)(ucSuf & 0x01));

    // キャラ番号取得
    usCharNo = (u16)( usCharData & BG_SC_CHAR_NO_MASK );

    // 角度データを取得
    ucDir = (u16)(*(_obj_fcol->direc_datap + usCharNo) << 8);		// 8bit >> 16bit
        
    if ((usCharData & BG_SC_H_FLIP)) {
        // 角度情報をHフリップ
        ucDir = (u16)(-(s16)ucDir);
    }
    if (usCharData & BG_SC_V_FLIP) {
        // 角度情報をVフリップ
        ucDir = (u16)(-((s16)ucDir + 0x4000) - 0x4000);
    }
    
    // 角度情報を設定
    return ucDir;
#endif
}


/*
 * Revision 1.14  2005/09/26 09:41:20  use1146
 * 角度のみ取得関数追加
 *
 * Revision 1.13  2005/09/06 14:00:52  use1173
 * プリコンパイルヘッダ対応
 *
 * Revision 1.12  2005/08/11 13:33:09  use1146
 * 単純チェック準備
 *
 * Revision 1.11  2005/08/08 09:07:48  use1146
 * Y軸のLIMITチェックミス修正
 *
 * Revision 1.10  2005/06/16 05:08:34  use1146
 * マップ外壁修正
 *
 * Revision 1.9  2005/06/10 11:10:03  use1146
 * 画面端可変対応
 *
 * Revision 1.8  2005/03/22 06:32:40  use1146
 * 1ドット修正
 *
 * Revision 1.7  2005/03/15 08:47:58  use1146
 * 1キャラの地形に対応
 *
 * Revision 1.6  2005/03/10 11:27:42  use1146
 * フリップバグ修正
 *
 * Revision 1.5  2005/02/25 08:31:51  use1146
 * 地形チップ修正対応
 *
 * Revision 1.4  2005/02/25 08:25:55  use1146
 * マップ外壁対応
 *
 * Revision 1.3  2005/02/21 09:16:33  use1146
 * 属性取得対応
 *
 * Revision 1.2  2005/01/20 06:02:37  use1146
 * 角度設定修正
 *
 * Revision 1.1  2005/01/20 03:09:29  use1146
 * 登録
 *
 */