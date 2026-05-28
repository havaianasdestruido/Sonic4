// ==========================================================================
/*!
  @file gmGmkBridge.cpp
  @brief ÉMÉ~ÉbÉNä€ëæã¥

  @author Kuramoto
				Copyright(c) 2009 Dimps
  $Id: gmGmkBridge.cpp 2 2011-04-11 05:21:26Z thamada $
  $Date: 2011-04-11 14:21:26 +0900 (Êúà, 11 4 2011) $
 */
// ==========================================================================
/*
 * $Log: gmGmkBridge.cpp,v $
 */

//----- Include Files -------------------------------------------------------
#include "pch.h"
#include "objObject.h"
#include "gmEnemy.h"
#include "gmEffect.h"
#include "gmMainDat.h"
#include "gmEventTbl.h"
#include "gmGameDBuild.h"
#include "gmGameDat.h"

#include "gmGmkBridge.h"

// ÉfÅ[É^ÉwÉbÉ_
#include "common/model/GMK_BRIDGE_MDL.HMB"


//----- Macros --------------------------------------------------------------
#define GMD_GMK_BRIDGE_PRIM (1)

//----- Macros Functions ----------------------------------------------------

//----- Definitions ---------------------------------------------------------
#define GMD_GMK_BRIDGE_OFFS	(-96 << FX32_SHIFT)		// ï`âÊà íuÉIÉtÉZÉbÉg
#define GMD_GMK_BRIDGE_GAP	(13 << FX32_SHIFT)		// ä€ëæÇPñ{ÇÃëæÇ≥
#define GMD_GMK_BRIDGE_NUM	(5)						// ÇPÉuÉçÉbÉNÇÃä€ëæñ{êî

#define GMD_GMK_BRIDGE_TIME	(16)					// íæÇ›çûÇﬁéûä‘
#define GMD_GMK_BRIDGE_FLEX	(8)					// ÇΩÇÌÇ›çûÇﬁó 
// eve_rec->flag
#define GMD_GMK_BRIDGE_EVE_FLAG_BLOCK3	(1 << 0)	// ÉuÉçÉbÉNêîÇRÇÃèÍçáON(OFFÇ»ÇÁÇQÉuÉçÉbÉN)
//----- External Declarations -----------------------------------------------

//----- Static Declarations -------------------------------------------------
static void gmGmkBridgeMain(OBS_OBJECT_WORK *obj_work);
static void gmGmkBridgeDrawFunc(OBS_OBJECT_WORK *obj_work);
static void gmGmkBridgeDecoDrawFunc(OBS_OBJECT_WORK *obj_work);

//----- Global Variables ----------------------------------------------------

//----- Local Variables -----------------------------------------------------
static OBS_ACTION3D_NN_WORK *gm_gmk_bridge_obj_3d_list = NULL;

//----- Global Functions ----------------------------------------------------
// ==========================================================================
// GmGmkBridgeBuild
/*!
 *	ÉMÉ~ÉbÉN ä€ëæã¥ ÉfÅ[É^ç\íz
 */
// ==========================================================================
void GmGmkBridgeBuild(void)
{
	gm_gmk_bridge_obj_3d_list =
			GmGameDBuildRegBuildModel((AMS_AMB_HEADER*)GmGameDatGetGimmickData(GMD_DWORK_NO_GMK_BRIDGE_MODEL),
								(AMS_AMB_HEADER*)GmGameDatGetGimmickData(GMD_DWORK_NO_GMK_BRIDGE_TEX),
								0/*draw_flag*/);
}

// ==========================================================================
// GmGmkBridgeFlush
/*!
 *	ÉMÉ~ÉbÉN ä€ëæã¥ ÉfÅ[É^ï–ïtÇØ
 */
// ==========================================================================
void GmGmkBridgeFlush(void)
{
	AMS_AMB_HEADER	*amb = (AMS_AMB_HEADER*)GmGameDatGetGimmickData(GMD_DWORK_NO_GMK_BRIDGE_MODEL);

	GmGameDBuildRegFlushModel(gm_gmk_bridge_obj_3d_list, amb->file_num);
}

// ==========================================================================
// GmGmkBridgeInit
/*!
 *	ÉMÉ~ÉbÉN ä€ëæã¥ èâä˙âªä÷êî
 *
 *	@param	eve_rec	[inout]	ÉåÉRÅ[ÉhÉ|ÉCÉìÉ^
 *	@param	pos_x	[in]	èoåªç¿ïW
 *	@param	pos_y	[in]
 *	@param	type	[in]	èàóùì‡óeÉ^ÉCÉv í èÌÇÕ0
 *
 *	@note
 */
// ==========================================================================
OBS_OBJECT_WORK* GmGmkBridgeInit(GMS_EVE_RECORD_EVENT *eve_rec, fx32 pos_x, fx32 pos_y, u8 type)
{
	GMS_ENEMY_3D_WORK	*gmk_work;
	OBS_OBJECT_WORK		*obj_work;

	UNREFERENCED_PARAMETER(type);

	obj_work = GMM_ENEMY_CREATE_WORK(eve_rec, pos_x, pos_y, sizeof(GMS_ENEMY_3D_WORK), "GMK_BRIDGE");
	gmk_work = (GMS_ENEMY_3D_WORK*)obj_work;

	// ÉIÉuÉWÉFÉNÉgì«Ç›çûÇ›
	ObjObjectCopyAction3dNNModel(obj_work,
					&gm_gmk_bridge_obj_3d_list[IDB_GMK_BRIDGE_MDL_GMK_BRIDGE_ZNO],
					&gmk_work->obj_3d);

	// ÉuÉçÉbÉNêîê›íË
	if (eve_rec->flag & GMD_GMK_BRIDGE_EVE_FLAG_BLOCK3) {
		obj_work->user_work = 3;							// ÇRÉuÉçÉbÉNí∑
	} else {
		obj_work->user_work = 2;							// ÇQÉuÉçÉbÉNí∑
	}
	obj_work->user_flag = 0;								// ÉvÉåÉCÉÑÅ[Ç™ç≈å„Ç…èÊÇ¡ÇΩà íu
	obj_work->user_timer = 0;								// èÊÇËç~ÇËÇääÇÁÇ©Ç…Ç∑ÇÈÇΩÇﬂÇÃÉ^ÉCÉ}Å[
	
	// óDêÊê›íË
	obj_work->pos.z = GMD_OBJ_GIMMICK_POS_Z_BACK;

	// å¬ï ê›íË
	obj_work->move_flag |= OBD_MOVE_NOMOVE | OBD_MOVE_NOCOL;	// à⁄ìÆñ≥Çµ ínå`Ç†ÇΩÇËñ≥Çµ
	obj_work->disp_flag |= OBD_DISP_NODIRFLIP;

	// ÉÅÉCÉìèàóù
	obj_work->ppFunc = gmGmkBridgeMain;
	// ï`âÊèàóùïœçX
	obj_work->ppOut = gmGmkBridgeDrawFunc;

	// ínå`ê›íË
	{
		OBS_COLLISION_WORK *col_work = &gmk_work->ene_com.col_work;
		
		col_work->obj_col.obj		= obj_work;

// diff_dataÇÉZÉbÉgÇ∑ÇÈÇ±Ç∆Ç≈ pixelíPà Ç≈ÇÃÉ`ÉFÉbÉNÇ∆Ç»ÇÈÅBÇ∑ÇËî≤ÇØÇ…Ç‡ëŒâûÅB
// g_gm_default_col ÇégópÇ∑ÇÈèÍçáÇÕãÈå`ÉTÉCÉYÇ…8dotíPà ÇÃêßå¿Ç™ïtÇ´Ç‹Ç∑ÅB
		col_work->obj_col.diff_data	= (s8*)g_gm_default_col;	// äÓñ{ínå`èÓïÒ
		col_work->obj_col.flag |= OBD_COLOBJ_NOFREE_DIFF_DATA;		// diff_dataÇäJï˙ÇµÇ»Ç¢

		col_work->obj_col.width		= (u16)(obj_work->user_work * 64);	// ínå`ÉTÉCÉYê›íË(ÉhÉbÉg)
		col_work->obj_col.height	= 16;
		col_work->obj_col.ofst_x	= (GMD_GMK_BRIDGE_OFFS >> FX32_SHIFT);
		col_work->obj_col.ofst_y	= 0;
		col_work->obj_col.attr		= OBD_COL_DATA_ATTR_THROUGH;		// Ç∑ÇËî≤ÇØOK

	}

	// ëïè¸éqÉ^ÉXÉNê∂ê¨
	{
		OBS_OBJECT_WORK			*sub_work;
		GMS_EFFECT_3DNN_WORK	*efct_work;

		sub_work = GMM_EFFECT_CREATE_WORK(sizeof(GMS_EFFECT_3DNN_WORK), obj_work/*parent_obj*/, 0/*sort_prio*/, "GMK_SPILE");
		efct_work = (GMS_EFFECT_3DNN_WORK*)sub_work;
		// ÉIÉuÉWÉFÉNÉgì«Ç›çûÇ›
		ObjObjectCopyAction3dNNModel(sub_work,
						&gm_gmk_bridge_obj_3d_list[IDB_GMK_BRIDGE_MDL_GMK_BRIDGE_DECO_ZNO],
						&efct_work->obj_3d);
		// ÉuÉçÉbÉNêîê›íË
		sub_work->user_work = obj_work->user_work;
		// óDêÊê›íË
		sub_work->pos.z = GMD_OBJ_GIMMICK_POS_Z_BACK;

		// å¬ï ê›íË
		sub_work->move_flag |= OBD_MOVE_NOMOVE | OBD_MOVE_NOCOL;	// à⁄ìÆñ≥Çµ ìñÇΩÇËñ≥Çµ
		sub_work->disp_flag |= OBD_DISP_NODIRFLIP;
		sub_work->ppOut = gmGmkBridgeDecoDrawFunc;
		sub_work->obj_3d->drawflag |= NND_DRAWOBJ_DOUBLESIDE;
	}
	return (obj_work);
}


//----- Local Functions -----------------------------------------------------
// ==========================================================================
// gmGmkBridgeMain
/*!
 *	ÉMÉ~ÉbÉN ä€ëæã¥ ï`âÊê›íËèàóù
 *
 *	@param obj_work	[in] ÉIÉuÉWÉFÉNÉgÉèÅ[ÉN
 */
// ==========================================================================
void gmGmkBridgeMain(OBS_OBJECT_WORK *obj_work)
{
	GMS_PLAYER_WORK		*ply_work = g_gm_main_system.ply_work[GSD_MAIN_PLAYER_1P];
	GMS_ENEMY_3D_WORK	*gmk_work = (GMS_ENEMY_3D_WORK*)obj_work;
	fx32	pos_x;
	fx32	len_x;
	fx32	rate;
	fx32	ofs_y;

	if (gmk_work->ene_com.col_work.obj_col.rider_obj == &ply_work->obj_work) {
		// ÉvÉåÉCÉÑÅ[Ç™èÊÇ¡ÇƒÇ¢ÇÈ
		if (obj_work->user_timer < GMD_GMK_BRIDGE_TIME) {
			obj_work->user_timer++;
		}
		obj_work->user_flag = (u32)(ply_work->obj_work.pos.x);					// ÉvÉåÉCÉÑÅ[ÇÃèÊÇ¡ÇΩà íuÇuser_flag Ç…ï€éù
	} else {
		if (obj_work->user_timer) {
			obj_work->user_timer--;
		}
	}
	if (obj_work->user_timer) {
		len_x = (fx32)(obj_work->user_work * GMD_GMK_BRIDGE_GAP * GMD_GMK_BRIDGE_NUM);	// ã¥ëSí∑
		len_x = len_x >> 1;														// ã¥îºï™
		pos_x = (fx32)(obj_work->user_flag) - (obj_work->pos.x + GMD_GMK_BRIDGE_OFFS + len_x);	// ã¥íÜêSÇ©ÇÁÇÃÉvÉåÉCÉÑÅ[à íu
		rate = FX_Div(pos_x , len_x);
		rate = MTM_MATH_CLIP(rate, -FX32_ONE, FX32_ONE);
		rate = rate << 2;
		ofs_y = mtMathCos(rate) * GMD_GMK_BRIDGE_FLEX
				* obj_work->user_timer / GMD_GMK_BRIDGE_TIME;
		gmk_work->ene_com.col_work.obj_col.ofst_y = (s16)(MTM_MATH_ABS(ofs_y) >> FX32_SHIFT);

	} else {
		gmk_work->ene_com.col_work.obj_col.ofst_y = 0;
	}
}
// ==========================================================================
// gmGmkBridgeDrawFunc
/*!
 *	ÉMÉ~ÉbÉN ä€ëæã¥ ï`âÊê›íËèàóù
 *
 *	@param obj_work	[in] ÉIÉuÉWÉFÉNÉgÉèÅ[ÉN
 */
// ==========================================================================
void gmGmkBridgeDrawFunc(OBS_OBJECT_WORK *obj_work)
{
	GMS_ENEMY_3D_WORK	*gmk_work = (GMS_ENEMY_3D_WORK*)obj_work;
	fx32	pos_x, pos_y, log_x;
	fx32	len_x, len_xl, len_xr;
	fx32	rate;

	len_x = (fx32)(obj_work->user_work * GMD_GMK_BRIDGE_GAP * GMD_GMK_BRIDGE_NUM);	// ã¥ëSí∑
	pos_x = (fx32)(obj_work->user_flag) - (obj_work->pos.x + GMD_GMK_BRIDGE_OFFS);	// ã¥ç∂í[Ç©ÇÁÇÃÉvÉåÉCÉÑÅ[à íu
	len_xl = pos_x;
	len_xr = len_x - pos_x;
	pos_y = gmk_work->ene_com.col_work.obj_col.ofst_y << FX32_SHIFT;

	obj_work->ofst.x = GMD_GMK_BRIDGE_OFFS + (GMD_GMK_BRIDGE_GAP >> 1);
	obj_work->ofst.y = GMD_GMK_BRIDGE_GAP >> 1;
#if GMD_GMK_BRIDGE_PRIM
	// ï`âÊÇµÇ»Ç¢Ç»ÇÁèIóπ
	if (obj_work->disp_flag & OBD_DISP_NODISP) {
		return;
	}
	
	u32 color = GmMainGetLightColor();
	
	// ä€ëæÇïKóvêîï™ÇæÇØï`âÊ
	NNS_TEXLIST* texlist = obj_work->obj_3d->texlist;
	VecFx32  vec;
	vec.x = obj_work->pos.x + obj_work->ofst.x;
	vec.y = -(obj_work->pos.y + obj_work->ofst.y);
	vec.z = obj_work->pos.z + obj_work->ofst.z;
	NNS_MATRIX	obj_mtx;
	nnMakeUnitMatrix(&obj_mtx);
	nnTranslateMatrix(&obj_mtx, &obj_mtx,
					FX_FX32_TO_F32(vec.x), FX_FX32_TO_F32(vec.y), FX_FX32_TO_F32(vec.z));
//	nnScaleMatrix(&obj_mtx, &obj_mtx, 3.2f, 3.2f, 3.2f);

	//	ÉvÉäÉ~ÉeÉBÉuÇ∆ÇµÇƒìoò^
	AMS_PARAM_DRAW_PRIMITIVE dat;
	// ÉxÅ[ÉXÉ}ÉgÉäÉbÉNÉXÇ»Çµ
	
	// ÉvÉäÉ~ÉeÉBÉuê›íË
	dat.type = NNE_PRIM_TRIANGLE_LIST;
	dat.count = 6 * 5 * obj_work->user_work;
	dat.ablend = NNE_PRIM_ALPHABLEND_OFF;
	// ÉAÉãÉtÉ@ÉuÉåÉìÉhê›íË
#if defined(_PC) | defined(_XBOX)
	dat.bldSrc = NNE_BLENDMODE_SRCALPHA;
	dat.bldDst = NNE_BLENDMODE_INVSRCALPHA;
	dat.bldMode = NNE_BLENDOP_ADD;
#else
	dat.bldSrc = NND_BLENDFUNC_GL_SRC_ALPHA;
	dat.bldDst = NND_BLENDFUNC_GL_ONE_MINUS_SRC_ALPHA;
	dat.bldMode = NND_BLENDOP_GL_FUNC_ADD;
#endif
	// ÉeÉXÉgê›íË
	dat.aTest = 1;
	dat.zMask = 0;
	dat.zTest = 1;
	
	// É\Å[ÉgÇµÇ»Ç¢
	dat.noSort = 1;

	// ÉeÉNÉXÉ`ÉÉÉNÉâÉìÉvê›íË
	dat.uwrap = NNE_PRIM_TEXWRAP_CLAMP;
	dat.vwrap = NNE_PRIM_TEXWRAP_CLAMP;
	
	// ÉeÉNÉXÉ`ÉÉê›íË
	dat.texlist = texlist;
	dat.texId   = 0;
	
	// í∏ì_ÉfÅ[É^çÏê¨
	NNS_PRIM3D_PCT* v = (NNS_PRIM3D_PCT*)amDrawMallocDataBuffer((s32)(sizeof(NNS_PRIM3D_PCT) * dat.count));
	dat.vtxPCT3D = v;
	dat.format3D = NNE_PRIM3D_FMT_PCT;
	
#endif // GMD_GMK_BRIDGE_PRIM
	for (int j = 0; j < (s16)obj_work->user_work; j++) {
		for (int i = 0; i < 5; i++) {
			if (obj_work->user_timer) {
				// É\ÉjÉbÉNÇ™èÊÇ¡ÇƒÇ¢ÇÈéûÇÕYç¿ïWï‚ê≥
				log_x = j * GMD_GMK_BRIDGE_GAP * GMD_GMK_BRIDGE_NUM + i * GMD_GMK_BRIDGE_GAP; // ä€ëæà íu
				if (log_x < pos_x) {
					rate = FX_Div(log_x, len_xl);
				} else if (log_x > pos_x) {
					log_x = len_x - log_x;
					rate = FX_Div(log_x, len_xr);
				} else {
					rate = FX32_ONE;
				}
//				obj_work->ofst.y = FX_Mul(pos_y, rate) + (GMD_GMK_BRIDGE_GAP >> 1);	// à íuî‰ó¶ÇÊÇËYç¿ïWéZèo
				obj_work->ofst.y = (GMD_GMK_BRIDGE_GAP >> 1) + FX_Mul(pos_y, rate)
									* obj_work->user_timer / GMD_GMK_BRIDGE_TIME;	// à íuî‰ó¶ÇÊÇËYç¿ïWéZèo
			}
#if GMD_GMK_BRIDGE_PRIM
			int idx = i + 5 * j;
			float ofst_x = FX_FX32_TO_F32(GMD_GMK_BRIDGE_GAP * idx);
			float ofst_y = FX_FX32_TO_F32(obj_work->ofst.y);
			NNS_PRIM3D_PCT* v_tbl = v + idx * 6;
			// UVç¿ïWê›íË (ébíË)
			v_tbl[0].Tex.u = v_tbl[1].Tex.u = 0.1f;
			v_tbl[2].Tex.u = v_tbl[3].Tex.u = 0.9f;
			v_tbl[0].Tex.v = v_tbl[2].Tex.v = 0.1f;
			v_tbl[1].Tex.v = v_tbl[3].Tex.v = 0.9f;
			
			// ÉJÉâÅ[ê›íË
			v_tbl[0].Col = color;
			v_tbl[1].Col = v_tbl[2].Col = v_tbl[3].Col = v_tbl[0].Col;
					
			// í∏ì_ê›íË & à⁄ìÆ
			v_tbl[0].Pos.x = v_tbl[1].Pos.x = -(- 6.5f) + ofst_x;
			v_tbl[2].Pos.x = v_tbl[3].Pos.x =  (- 6.5f) + ofst_x;
			v_tbl[0].Pos.y = v_tbl[2].Pos.y = -(-13.0f) - ofst_y;
			v_tbl[1].Pos.y = v_tbl[3].Pos.y =  (- 0.0f) - ofst_y;
			v_tbl[0].Pos.z = v_tbl[1].Pos.z = v_tbl[2].Pos.z = v_tbl[3].Pos.z =  -1.0f;

			v_tbl[4] = v_tbl[2];
			v_tbl[5] = v_tbl[3];
			v_tbl[3] = v_tbl[1];
#else
			ObjDrawActionSummary(obj_work);
			obj_work->ofst.x += GMD_GMK_BRIDGE_GAP;
#endif // GMD_GMK_BRIDGE_PRIM
		}
	}
#if GMD_GMK_BRIDGE_PRIM
	amMatrixPush(&obj_mtx);
	ObjDraw3DNNDrawPrimitive(&dat);
	amMatrixPop();
#endif // GMD_GMK_BRIDGE_PRIM
}

// ==========================================================================
// gmGmkBridgeDecoDrawFunc
/*!
 *	ÉMÉ~ÉbÉN ä€ëæã¥ëïè¸ ï`âÊê›íËèàóù
 *
 *	@param obj_work	[in] ÉIÉuÉWÉFÉNÉgÉèÅ[ÉN
 */
// ==========================================================================
void gmGmkBridgeDecoDrawFunc(OBS_OBJECT_WORK *obj_work)
{
	// ç∂í[ëïè¸
	obj_work->scale.x = -MTM_MATH_ABS(obj_work->scale.x);
	obj_work->ofst.x = GMD_GMK_BRIDGE_OFFS;
	ObjDrawActionSummary(obj_work);

	// âEí[ëïè¸
	obj_work->scale.x = -obj_work->scale.x;
	obj_work->ofst.x = (fx32)(GMD_GMK_BRIDGE_OFFS + obj_work->user_work * GMD_GMK_BRIDGE_GAP * GMD_GMK_BRIDGE_NUM);
	ObjDrawActionSummary(obj_work);
}


//	// ==========================================================================
//	// _pt
//	/*!
//	 *	@param	tcb	[in]	TCB
//	 */
//	// ==========================================================================
