// ==========================================================================
/*!
  @file gmPlyLod.cpp
  @brief 

  @author Ishizaki
				Copyright(c) 2009 Dimps

  $Id: gmPlyLod.cpp 2 2011-04-11 05:21:26Z thamada $
  $Date:: 2011-04-11 14:21:26 +0900#$
 */
// ==========================================================================
/*
 * Memo
 *
 */

//----- Include Files -------------------------------------------------------
#include "pch.h"

#include "gmPlyLod.h"

//----- Definitions ---------------------------------------------------------

//----- Macros --------------------------------------------------------------

//----- Macros Functions ----------------------------------------------------

//----- External Declarations -----------------------------------------------

//----- Static Declarations -------------------------------------------------

//----- Global Variables ----------------------------------------------------

//----- Local Variables -----------------------------------------------------
//static GMS_PLY_LOD_MTN_HEADER gm_ply_lod_mtn_header;

//----- Global Functions ----------------------------------------------------
// ==========================================================================
// GmPlyLodCnvAddr
/*!
 *	ロッドアクション アドレスコンバート
 *
 *	@param	data		[in]	ロッドモーションデータ
 */
// ==========================================================================
void GmPlyLodCnvAddr(void *data)
{
	s32		mtn_cnt;

	GMS_PLY_LOD_MTN_HEADER	*lod_header;
	GMS_PLY_LOD_MTN			*lod_mtn;

	MTM_ASSERT(data);

	lod_header = (GMS_PLY_LOD_MTN_HEADER*)data;
	lod_header->mtn_data = (GMS_PLY_LOD_MTN*)((u32)lod_header->mtn_data + (u32)data);

	lod_mtn = lod_header->mtn_data;
	for (mtn_cnt = 0; mtn_cnt < lod_header->mtn_num; mtn_cnt++, lod_mtn++) {
		lod_mtn->pat_data = (GMS_PLY_LOD_PAT*)((u32)lod_mtn->pat_data + (u32)data);
	}
}

#if 0
// ==========================================================================
// GmPlyLodSetMtnHeader
/*!
 *	ロッドアクション データ設定
 *
 *	@param	data		[in]	ロッドモーションデータ
 */
// ==========================================================================
void GmPlyLodSetMtnHeader(void *data)
{
	gm_ply_lod_mtn_header = data;
}

// ==========================================================================
// GmPlyLodGetMtnHeader
/*!
 *	ロッドアクション データ取得
 *
 *	@return	ロッドモーションデータ
 */
// ==========================================================================
GMS_PLY_LOD_MTN_HEADER* GmPlyLodGetMtnHeader(void)
{
	return (gm_ply_lod_mtn_header);
}
#endif

// ==========================================================================
// GmPlyLodGetLodActionPatData
/*!
 *	ロッドアクション プレイヤーハンド パターンデータ取得
 *
 *	@param	act_id		[in]	アクションID
 *	@param	frame		[in]	設定フレーム
 *
 *	@note
 *		データ参照のみの為 ゲーム外からの参照可能
 */
// ==========================================================================
GMS_PLY_LOD_PAT* GmPlyLodGetLodActionPatData(GMS_PLY_LOD_MTN_HEADER *header, s32 act_id, fx32 frame)
{
	GMS_PLY_LOD_MTN	*lod_mtn;
	GMS_PLY_LOD_PAT	*lod_pat;
	s32				i;

	MTM_ASSERT(header);
	MTM_ASSERT(act_id < header->mtn_num);

	lod_mtn = header->mtn_data + act_id;
	lod_pat = lod_mtn->pat_data;


	for (i = 0; i < lod_mtn->pat_num; i++, lod_pat++) {
		if (lod_pat->start_frame > frame) {
			break;
		}
	}

	MTM_ASSERT(i != 0);

	if (i != 0) {
	//	i--;
		lod_pat--;
	}

	return (lod_pat);
}


//----- Local Functions -----------------------------------------------------



// ==========================================================================
// _pt
/*!
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
