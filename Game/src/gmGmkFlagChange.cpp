// ================================================================
/*!
  @file gmGmkFlagChange.c
  @brief フラグ切り替えギミック AB面や接地フラグなど

  @author mana
  @author modifier Ishizaki
                Copyright(c) 2009 Dimps
  $Id: gmGmkFlagChange.cpp 2 2011-04-11 05:21:26Z thamada $
 */
// ================================================================
/*
 * memo
 *
 *
 */

//----- Include Files --------------------------------------------------
#include "pch.h"
#include "gmGmkFlagChange.h"
#include "gmEnemy.h"
#include "gmMain.h"
#include "gmTask.h"
#include "gmPlySeq.h"
#include "gmCamera.h"
#include "gmGmkSsCircle.h"
#include "gmEnding.h"
#include "gmGameDat.h"
#include "gmGmkPressPillar.h"

//#include "gmPlayerGmk.h"



//----- Macros ---------------------------------------------------------

//----- Macros Functions -----------------------------------------------

//----- Definitions ----------------------------------------------------
// GMD_EVENT_ID_GMK_TOUCH_EARTH 接地
#define GMD_GMK_TOUCH_EARTH_LAND_FLIP_OFF	( 1 << 0 )	//!< HFLIP OFF
#define GMD_GMK_TOUCH_EARTH_LAND_FLIP_ON	( 1 << 1 )	//!< HFLIP ON
#define GMD_GMK_TOUCH_EARTH_LAND_FLIP_REV	( 1 << 4 )	//!< HFLIP 反転
#define GMD_GMK_TOUCH_EARTH_LAND_EXTRA		( 1 << 7 )	//!< 特殊条件フラグ(HOGだけ仕様)：バネジャンプとZONE2大砲/スプリングカタパルト射出の時だけ接地有効

// GMS_EVE_RECORD_EVENT : flag
// GMD_EVENT_ID_CHANGE_CAM_CENTER
#define GMD_GMK_CHANGE_CAM_CENTER_EVE_FLAG_OFST_X_MASK	(0x0007)	//!< Xオフセット量マスク
#define GMD_GMK_CHANGE_CAM_CENTER_EVE_FLAG_OFST_X_SIGN	(0x0008)	//!< X符号フラグ
#define GMD_GMK_CHANGE_CAM_CENTER_EVE_FLAG_OFST_Y_MASK	(0x0070)	//!< Yオフセット量マスク
#define GMD_GMK_CHANGE_CAM_CENTER_EVE_FLAG_OFST_Y_SIGN	(0x0080)	//!< Y符号フラグ
#define GMD_GMK_CHANGE_CAM_CENTER_EVE_FLAG_OFST_Y_SHIFT	(4)			//!< Yオフセットシフト量
#define GMD_GMK_CHANGE_CAM_CENTER_EVE_FLAG_MUL_SHIFT	(3)			//!< オフセット量乗算シフト量 *8

#if 0
// eve_rec->flag GMD_EVE_TOUCH_EARTH
//#define GMD_GMK_LAND_LEFT_OFF ( 1 << 0 )	// 現在未使用
//#define GMD_GMK_LAND_LEFT_ON  ( 1 << 1 )

#endif

//----- Global Variables -----------------------------------------------

//----- Local Variables ------------------------------------------------

//----- External Declarations ------------------------------------------

//----- Static Declarations --------------------------------------------
static void gmGmkFlagChangeDefFunc(OBS_RECT_WORK* mine_rect, OBS_RECT_WORK* match_rect);


//----- Global Functions -----------------------------------------------
// ================================================================
// GmGmkFlagChangeInit
/*!
  フラグ変更系ギミック初期化関数

  @param eve_rec   [io] レコードポインタ
  @param pos_x  [in] 出現座標
  @param pos_y  [in] 
  @param type [in] 処理内容タイプ 通常は0
 */
// ================================================================
OBS_OBJECT_WORK* GmGmkFlagChangeInit(GMS_EVE_RECORD_EVENT *eve_rec, fx32 pos_x, fx32 pos_y, u8 type)
{
	GMS_ENEMY_COM_WORK	*gmk_work;
	OBS_OBJECT_WORK		*obj_work;
	OBS_RECT_WORK		*rect_work;

	UNREFERENCED_PARAMETER(type);

	obj_work = GMM_ENEMY_CREATE_WORK(eve_rec, pos_x, pos_y, sizeof(GMS_ENEMY_COM_WORK), "GMK_FLAG_CNG");
	gmk_work = (GMS_ENEMY_COM_WORK*)obj_work;

    // 矩形設定
	rect_work = &gmk_work->rect_work[GMD_ENEMY_RECT_BODY];
	ObjRectGroupSet(rect_work,
						GMD_OBJ_RECT_GROUP_ENEMY,
						GMD_OBJ_RECT_TARGET_GROUPFLAG_PLAYER);
	ObjRectAtkSet(rect_work, 0, GMD_OBJ_RECT_ATK_POWER_DEFAULT);
	ObjRectDefSet(rect_work, GMD_OBJ_RECT_DEF_FLAG_WEAK_BODYATK, GMD_OBJ_RECT_DEF_POWER_DEFAULT);	// 身体のみHIT可
	ObjRectSet(&rect_work->rect,
			eve_rec->left,  eve_rec->top,
			(s16)(eve_rec->width + eve_rec->left), (s16)(eve_rec->height + eve_rec->top));
	rect_work->ppDef = gmGmkFlagChangeDefFunc;
	rect_work->parent_obj = obj_work;
	rect_work->flag |= OBD_RECT_NODAMAGE | OBD_RECT_NOHIT_UP;


	// 個別設定
	obj_work->move_flag |= OBD_MOVE_NOMOVE | OBD_MOVE_NOCOL | OBD_DISP_NODISP;

	// 矩形固定設定
//	switch (gmk_work->eve_rec->id) {
//	case GMD_EVENT_ID_GMK_:
//		break;
//	default:
//	}

	return (obj_work);
}

//----- Local Functions ------------------------------------------------
// ================================================================
// gmGmkFlagChangeDefFunc
/*!
  ヒット時処理

	@param	mine_rect		[in]	自分矩形ワークポインタ
	@param	match_rect		[in]	相手矩形ワークポインタ

  @note
	ppDefへ登録
 */
// ================================================================
void gmGmkFlagChangeDefFunc(OBS_RECT_WORK* mine_rect, OBS_RECT_WORK* match_rect)
{
    GMS_ENEMY_COM_WORK	*gmk_work = (GMS_ENEMY_COM_WORK *)mine_rect->parent_obj;
    GMS_PLAYER_WORK		*ply_work = (GMS_PLAYER_WORK *)match_rect->parent_obj;
    OBS_OBJECT_WORK		*ply_obj = (OBS_OBJECT_WORK *)ply_work;
    
    if ( gmk_work == NULL ) return;
    if ( ply_work == NULL ) return;
	if (ply_work->obj_work.obj_type != GMD_OBJTYPE_PLAYER) {
		return;
	}

	switch ( gmk_work->eve_rec->id ) {
	default:
		MTM_ASSERT(!"gmGmkFlagChangeDefFunc:eve_id error\n");
		break;
		
	case GMD_EVENT_ID_GMK_TOUCH_EARTH:
		// 接地フラグ
		if (gmk_work->eve_rec->flag & GMD_GMK_TOUCH_EARTH_LAND_EXTRA) {
			// 特殊条件フラグチェック
			if (  (ply_work->seq_state != GME_PLY_SEQ_STATE_GMK_SPRINGJUMP)			// スプリングジャンプ
				&&(ply_work->seq_state != GME_PLY_SEQ_STATE_GMK_SPRINGCTPLT_UP)		// スプリングカタパルト射出
				&&(ply_work->seq_state != GME_PLY_SEQ_STATE_GMK_SPRINGCTPLT_LR)		// スプリングカタパルト射出
				&&(ply_work->seq_state != GME_PLY_SEQ_STATE_GMK_CANNON_SHOOT) ) {	// 大砲射出
				// 上記条件以外のときは接地を無視する
				break;
			} else {
				// 接地後の移動方向を強制する
				ply_work->gmk_flag |= GMD_PLGF_TOUCH_FORCE_DIR;
			}
		}
		if (ply_obj->obj_type == GMD_OBJTYPE_PLAYER && ply_obj->move_flag & OBD_MOVE_JUMP) {
			ply_work->gmk_flag |= GMD_PLGF_TOUCH;
			if ( gmk_work->eve_rec->flag & GMD_GMK_TOUCH_EARTH_LAND_FLIP_REV ) {
				ply_work->gmk_flag |= GMD_PLGF_TOUCH_FLIP;
			}
			else if (gmk_work->eve_rec->flag & GMD_GMK_TOUCH_EARTH_LAND_FLIP_ON) {
				if (!(ply_work->obj_work.disp_flag & OBD_DISP_HFLIP)) {
				ply_work->gmk_flag |= GMD_PLGF_TOUCH_DISP_FLIP;
				}
			}
			else if (gmk_work->eve_rec->flag & GMD_GMK_TOUCH_EARTH_LAND_FLIP_OFF) {
				if (ply_work->obj_work.disp_flag & OBD_DISP_HFLIP) {
				ply_work->gmk_flag |= GMD_PLGF_TOUCH_DISP_FLIP;
				}
			}
			// トリックコンボクリア
			//GmPlayerStateClearTrickCombo(ply_work);
        }
        break;
	case GMD_EVENT_ID_GMK_A:
		// A面へ移行
		ply_obj->flag &= ~OBD_OBJECT_B;
		if ( ply_obj->obj_type == GMD_OBJTYPE_PLAYER ) {
			ply_work->graind_prev_ride = 0;
		}
		break;
	case GMD_EVENT_ID_GMK_B:
		// B面へ移行
		ply_obj->flag |= OBD_OBJECT_B;
		if ( ply_obj->obj_type == GMD_OBJTYPE_PLAYER ) {
			ply_work->graind_prev_ride = 0;
		}
		break;
	case GMD_EVENT_ID_FALLDIE:
		// 落下死亡判定
		//if (ply_obj->obj_type == GMD_OBJTYPE_PLAYER && !(ply_obj->move_flag & OBD_MOVE_NOCOL)) {
		if (ply_obj->obj_type == GMD_OBJTYPE_PLAYER) {
			GmPlySeqChangeDeath(ply_work);
		}
		break;
	case GMD_EVENT_ID_CHANGE_CAM_CENTER:
		// カメラセンター変更
		if (ply_obj->obj_type == GMD_OBJTYPE_PLAYER) {
			u16	flag = gmk_work->eve_rec->flag;
			s16	camera_ofst;

			camera_ofst = (s16)(flag & GMD_GMK_CHANGE_CAM_CENTER_EVE_FLAG_OFST_X_MASK);
			if (flag & GMD_GMK_CHANGE_CAM_CENTER_EVE_FLAG_OFST_X_SIGN) {
				camera_ofst = (s16)-camera_ofst;
			}
			ply_work->gmk_camera_center_ofst_x = (s16)(camera_ofst << GMD_GMK_CHANGE_CAM_CENTER_EVE_FLAG_MUL_SHIFT);

			camera_ofst = (s16)((flag & GMD_GMK_CHANGE_CAM_CENTER_EVE_FLAG_OFST_Y_MASK) >> GMD_GMK_CHANGE_CAM_CENTER_EVE_FLAG_OFST_Y_SHIFT);
			if (flag & GMD_GMK_CHANGE_CAM_CENTER_EVE_FLAG_OFST_Y_SIGN) {
				camera_ofst = (s16)-camera_ofst;
			}
			ply_work->gmk_camera_center_ofst_y = (s16)(camera_ofst << GMD_GMK_CHANGE_CAM_CENTER_EVE_FLAG_MUL_SHIFT);
		}
		break;
	case GMD_EVENT_ID_LOOP_CAMERA:
		// ループカメラ
		if (ply_obj->obj_type == GMD_OBJTYPE_PLAYER) {
#if !_IPHONE
			if (g_gs_main_sys_info.game_flag & GSD_MAINSYS_GAME_FLAG_INPUT_CLASSIC) {
				// クラシック操作時はループ回転カメラなし
				break;
			}
#endif // _IPHONE
			OBS_CAMERA	*obj_camera = ObjCameraGet(GME_CAMERA_NO_MAIN);
			obj_camera->flag |= OBD_CAMERA_ROT_EX;
		}
		break;
		
	case GMD_EVENT_ID_SS_ONEWAY_RECT:
		// SpecialStage 一方通行
		if (ply_obj->obj_type == GMD_OBJTYPE_PLAYER) {
			GmGmkSsOnewayThrough(gmk_work->eve_rec->flag);
		}
		break;

	case GMD_EVENT_ID_END_SON_NOP:
		// Ending ソニック NOP をセット
		GmEndingPlyNopSet();
		break;

	case GMD_EVENT_ID_END_SON_BRAKE:
		// Ending ソニック Brake をセット
		GmEndingPlyBrakeSet();
		break;

	case GMD_EVENT_ID_GMK_DATA_LOAD:
		// ボスデータロード
		MTM_ASSERT(gmk_work->eve_rec->flag < GMD_GAMEDAT_LOAD_BOSS_TYPE_MAX);
		GmMainDatLoadBossBattleStart((GME_GAMEDAT_LOAD_BOSS_TYPE)gmk_work->eve_rec->flag);
		// イベント破棄
		gmk_work->enemy_flag |= GMD_ENEMY_FLAG_DIE;
		gmk_work->obj_work.flag |= OBD_OBJECT_TASKCLEAR_REQUEST | OBD_OBJECT_NOHIT;
		break;

	case GMD_EVENT_ID_GMK_P_PILLAR_SW:
		// Zone4 迫り出す柱起動判定
		GmGmkPressPillarStartup(gmk_work->eve_rec->flag);
		break;
	}

#if 0
	case GMD_EVE_GRAIND:
        // グラインドに乗るためにA面へ移行 IDの設定されていないものは無視
        if ( ply_obj->obj_type == GMD_OBJTYPE_PLAYER ) {
            // グラインドIDをチェック
            if ( ((ply_work->graind_id & ~GMD_PLG_GRAIND_RIDE) != (gmk_work->eve_rec->flag+1)) &&
					(!( ply_work->act_state >= GMD_PLY_STATE_GRAIND && ply_work->act_state <= GMD_PLY_STATE_GRAIND_END ))) {
                // 初HIT時のみ設定する
                if( !(ply_work->graind_prev_ride & GMD_PLG_GRAIND_OUTCHECK) ){
                    // HIT前の設定面を保持する
                    ply_work->graind_prev_ride = (u8)((ply_obj->flag & OBD_OBJECT_B) | GMD_PLG_GRAIND_OUTCHECK);

#if GMD_EVENT_DEBUG_GMK_ADJUST_OFF		// ◆ギミック整理に伴い一旦OFF
                    // ネオングラインドは常にB面から来たことにする
                    if ( gmk_work->eve_rec->id == GMD_EVE_NEON_GRAIND ) {
                        ply_work->graind_prev_ride |= OBD_OBJECT_B;
					}
#endif

                    // グラインドを設定する
                    ply_work->graind_id = (u8)((gmk_work->eve_rec->flag & GMD_PLG_GRAIND_ID_MASK)+1);
                    // A面へ
                    ply_obj->flag &= ~OBD_OBJECT_B;
                }
            }
            ply_work->gmk_flag |= GMD_PLGF_GRAIND_HITCHECK;
        }
        break;
    case GMD_EVE_G_A:
        // A面へ移行(グラインド用)
		if ( ply_obj->obj_type == GMD_OBJTYPE_PLAYER &&
				( ply_work->act_state >= GMD_PLY_STATE_GRAIND && ply_work->act_state <= GMD_PLY_STATE_GRAIND_END ) ) {
			ply_obj->flag &= ~OBD_OBJECT_B;
		}
        break;
    case GMD_EVE_G_B:
		// B面へ移行(グラインド用)
		if ( ply_obj->obj_type == GMD_OBJTYPE_PLAYER &&
				( ply_work->act_state >= GMD_PLY_STATE_GRAIND && ply_work->act_state <= GMD_PLY_STATE_GRAIND_END ) ) {
			ply_obj->flag |= OBD_OBJECT_B;
		}
        break;
#endif
#if 0
	case GMD_EVE_CHANGE_CAM_CENTER:
		// カメラセンター変更
		if (ply_obj->obj_type == GMD_OBJTYPE_PLAYER) {
			u16	flag = gmk_work->eve_rec->flag;
			s16	camera_ofst;

			camera_ofst = (s16)(flag & GMD_GMK_CHANGE_CAM_CENTER_EVE_FLAG_OFST_X_MASK);
			if (flag & GMD_GMK_CHANGE_CAM_CENTER_EVE_FLAG_OFST_X_SIGN) {
				camera_ofst = (s16)-camera_ofst;
			}
			ply_work->gmk_camera_center_ofst_x = (s16)(camera_ofst << GMD_GMK_CHANGE_CAM_CENTER_EVE_FLAG_MUL_SHIFT);

			camera_ofst = (s16)((flag & GMD_GMK_CHANGE_CAM_CENTER_EVE_FLAG_OFST_Y_MASK) >> GMD_GMK_CHANGE_CAM_CENTER_EVE_FLAG_OFST_Y_SHIFT);
			if (flag & GMD_GMK_CHANGE_CAM_CENTER_EVE_FLAG_OFST_Y_SIGN) {
				camera_ofst = (s16)-camera_ofst;
			}
			ply_work->gmk_camera_center_ofst_y = (s16)(camera_ofst << GMD_GMK_CHANGE_CAM_CENTER_EVE_FLAG_MUL_SHIFT);
		}
		break;
#endif
}

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

