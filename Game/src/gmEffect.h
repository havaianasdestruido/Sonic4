// =======================================================================
/*!
  @file	gmEffect.h
  @brief エフェクト

  @author Keisuke Tanaka
 				Copyright(c) 2009 Dimps
  $Id: gmEffect.h 2 2011-04-11 05:21:26Z thamada $
 */
// =======================================================================
/*
 * $Log$
 */

/* 重複インクルード回避手法 */

#ifndef GM_EFFECT_H_
#define GM_EFFECT_H_

#if	defined(__cplusplus)
extern "C" {
#endif

/*
  >> REMINDER <<
  
  EffectaStudioエフェクトの描画を行う場合は、
  下記の関数呼び出しを行ってエフェクトシステムを初期化してください。
  
  ObjDrawESEffectSystemInit(GMD_TASK_PAUSELEVEL_DEF,
							  GMD_TASK_PRIO_EFFECT_SERVER,
							  GMD_TASK_GROUP_EFFECT_SERVER);
*/


/*------ Include Files -------------------------------------------------*/

/*------ Macros --------------------------------------------------------*/
// 3DES初期化設定フラグ
#define GMD_EFFECT_3DES_FLAG_NOFLIP				(1 << 0)	//!< フリップ反映しない
#define GMD_EFFECT_3DES_FLAG_STICKPARENT		(1 << 1)	//!< 親に付随
#define GMD_EFFECT_3DES_FLAG_SCALE_BY_MTX		(1 << 2)	//!< 行列でスケーリング（モデル使用エフェクトなどで使用。TYPE_EMTでは正常に反映されません）
#define GMD_EFFECT_3DES_FLAG_ENABLE_DIR			(1 << 4)	//!< オブジェクトのdirで回転する
#define GMD_EFFECT_3DES_FLAG_COPY_NODISP		(1 << 5)	//!< 親のNODISPをコピーする
/*! TYPE_EMT系の場合にデータのRotationを適用する（フラグオフの場合はデータ側の回転は無視されます。
  フラグオンでもプログラム側で設定したクォータニオンの左側にデータ側の回転が乗算されてしまう為、
  ES上での姿勢そのままで表示する場合など、プログラム側でエミッターの回転を制御しない場合での使用を推奨します。）
 */
#define GMD_EFFECT_3DES_FLAG_EMT_USE_DATA_ROT	(1 << 6)

/* 定義値 */
#define GMD_EFFECT_CR_PARAM_MODEL_IDX_NONE		(-1)		//!< 生成パラメータモデルインデックス 不使用指定マーク

/*------ Macro Functions -----------------------------------------------*/

#if defined(MTD_DEBUG)
#define GMM_EFFECT_CREATE_WORK(work_size, parent_obj, sort_prio, name) (GmEffectCreateWork(work_size, parent_obj, sort_prio, name))
#else
#define GMM_EFFECT_CREATE_WORK(work_size, parent_obj, sort_prio, name) (GmEffectCreateWork(work_size, parent_obj, sort_prio))
#endif /* defined(MTD_DEBUG) */

/*------ Definitions ---------------------------------------------------*/
//! 矩形設定
typedef enum
{
	GME_EFFECT_RECT_DEF	= 0,	//!< くらい矩形
	GME_EFFECT_RECT_ATK,		//!< 攻撃矩形
	
	// 以降必要に応じて追加
	GME_EFFECT_RECT_NUM			//!< 保持矩形数
} GME_EFFECT_RECT;

//! 3DES 配置タイプ
typedef enum
{
	GME_EFFECT_3DES_POS_TYPE_MTX	= 0,	//!< 行列による回転・平行移動
	GME_EFFECT_3DES_POS_TYPE_EMT,			//!< エミッターの設定による回転・平行移動
	GME_EFFECT_3DES_POS_TYPE_EMT_DEPEND,	//!< エミッターの設定＋obj_work.posのみ行列で設定（パーティクル位置がエミッタの位置に常に依存）
	
	GME_EFFECT_3DES_POS_TYPE_MAX
} GME_EFFECT_3DES_POS_TYPE;

//! エフェクトワーク共通部
typedef struct tag_GMS_EFFECT_COM_WORK
{
	OBS_OBJECT_WORK			obj_work;	//!< オブジェクトワーク
	
	/* 矩形情報 */
	OBS_RECT_WORK			rect_work[GME_EFFECT_RECT_NUM];
} GMS_EFFECT_COM_WORK;

//! 3DNN エフェクトワーク
typedef struct tag_GMS_EFFECT_3DNN_WORK
{
	GMS_EFFECT_COM_WORK		efct_com;
	
	OBS_ACTION3D_NN_WORK	obj_3d;		//!< 3DNNオブジェクト
} GMS_EFFECT_3DNN_WORK;

//! 3DES エフェクトワーク
typedef struct tag_GMS_EFFECT_3DES_WORK
{
	GMS_EFFECT_COM_WORK		efct_com;
	
	OBS_ACTION3D_ES_WORK	obj_3des;	//!< 3DESオブジェクト
	
	GME_EFFECT_3DES_POS_TYPE	saved_pos_type;	//!< 配置タイプ保存（値を変更しても配置タイプは変更されません）
	Uint32						saved_init_flag;	//!< 初期化設定フラグ保存（値を変更しても設定は変更されません）
} GMS_EFFECT_3DES_WORK;


//! エフェクト 生成パラメータ構造体
typedef struct tag_GMS_EFFECT_CREATE_PARAM
{
	Sint32				ame_idx;			//!< AMEデータのAMBインデックス
	GME_EFFECT_3DES_POS_TYPE	pos_type;
	Uint32				init_flag;
	AMS_VECTOR3			disp_ofst;
	NNS_ROTATE_A16		disp_rot;
	Float				scale;
	void	(*main_func)(OBS_OBJECT_WORK*);
	Sint32				model_idx;			//!< モデルデータのAMBインデックス（-1なら使用しない）
} GMS_EFFECT_CREATE_PARAM;


/*------ External Declarations -----------------------------------------*/
// ==========================================================================
// 初期化 終了処理
// ==========================================================================
// ==========================================================================
// GmEffectInit
/*!
 *	エフェクト関連 初期化
 *
 *	@note
 *		エフェクト関連の初期化を一括して行います
 */
// =========================================================================
extern void GmEffectInit(void);

// ==========================================================================
// GmEffectExit
/*!
 *	エフェクト関連 終了処理
 *
 *	@note
 *		エフェクト関連の終了処理を一括して行います
 */
// ==========================================================================
extern void GmEffectExit(void);

// ==========================================================================
// エフェクトワーク
// ==========================================================================
// ==========================================================================
// GmEffectCreateWork
/*!
 *	エフェクトワークの作成・初期化
 *
 *	@param	work_size	[in]	取得するTCBワークサイズ
 *	@param	parent_obj	[in]	親オブジェクト(NULL可)
 *	@param	sort_prio	[in]	エフェクトソート用のプライオリティ
 *	@param	name		[in]	デバッグ用TCB名(NULL可)
 *
 *	@return	取得したOBS_OBJECT_WORKワークアドレス
 *
 *	@note
 *		指定サイズのワークを持つオブジェクト管理TCBを作成し\n
 *		OBS_OBJECT_WORKの基本設定を行います。\n
 *		work_sizeは sizeof(OBS_OBJECT_WORK) 以上の値を設定して下さい。\n
 *		親オブジェクトが存在する場合は、初期座標を親オブジェクトの座標に設定します。\n
 *		矩形登録無し クリップ無し 当たり無し で設定します
 */
// ==========================================================================
#if defined(MTD_DEBUG)
extern OBS_OBJECT_WORK* GmEffectCreateWork(Uint32 work_size, OBS_OBJECT_WORK *parent_obj, u16 sort_prio, const char *name);
#else
extern OBS_OBJECT_WORK* GmEffectCreateWork(Uint32 work_size, OBS_OBJECT_WORK *parent_obj, u16 sort_prio);
#endif /* defined(MTD_DEBUG) */

// =======================================================================
// GmEffectDefaultExit
/*!
  エフェクト解放処理
  
  @param tcb	[in]	TCB
 */
// =======================================================================
extern void GmEffectDefaultExit(MTS_TASK_TCB *tcb);

// =======================================================================
// GmEffect3dESCreateByParam
/*!
  エフェクト3DES パラメータ指定生成
  
  @param create_param	[in]	生成パラメータ構造体
  @param parent_obj		[io]	親オブジェクト
  @param arc			[in]	エフェクトアーカイブ（NULL不可）
  @param ame_dwork		[in]	AMEデータDW（読み込み先）
  @param ambtex_dwork	[in]	テクスチャAMBデータDW（読み込み先）
  @param texlist_dwork	[in]	共有テクスチャリストDW（読み込み済み）
  @param model_dwork	[in]	モデルデータDW（使用しない場合はNULL指定）（読み込み先）
  @param object_dwork	[in]	共有オブジェクト（使用しない場合はNULL指定）（読み込み済み）
  
  @return エフェクト3DESワーク
 
  @note
  パラメータ情報構造体のデータに基づいてエフェクト生成を行います。
  使用するテクスチャリスト、オブジェクトが転送済みであることが前提です。
 */
// =======================================================================
extern GMS_EFFECT_3DES_WORK* GmEffect3dESCreateByParam(const GMS_EFFECT_CREATE_PARAM *create_param,
													   OBS_OBJECT_WORK *parent_obj,
													   void *arc,
													   OBS_DATA_WORK *ame_dwork,
													   OBS_DATA_WORK *ambtex_dwork,
													   OBS_DATA_WORK *texlist_dwork,
													   OBS_DATA_WORK *model_dwork,
													   OBS_DATA_WORK *object_dwork,
													   Uint32 work_size=sizeof(GMS_EFFECT_3DES_WORK));

#if defined(GMD_DEBUG_NO_CREATE_EFFECT)
// =======================================================================
// GmEffect3dESCreateDummy
/*!
  ダミーエフェクト生成
  
  @param parent_obj	[io]	親オブジェクト
  
  @return エフェクト3DESワーク
  
  @note
  最初からOBD_DISP_ENDフラグが立っている空オブジェクトを生成します。
 */
// =======================================================================
extern GMS_EFFECT_3DES_WORK* GmEffect3dESCreateDummy(OBS_OBJECT_WORK *parent_obj);
#endif /* defined(GMD_DEBUG_NO_CREATE_EFFECT) */

// =======================================================================
// GmEffectRectInit
/*!
  エフェクト矩形初期化
  
  @param	efct_com			[in]	エフェクトワーク
  @param	atk_flag_tbl		[in]	攻撃設定フラグテーブル
  @param	def_flag_tbl		[in]	防御設定フラグテーブル
  @@param	my_group			[in]	矩形グループ設定
  @param	target_group_flag	[in]	対象グループフラグ設定
  
  @note
  	atk_flag_tbl と def_flag_tbl は GMD_EFFECT_RECT_NUM分繋がる配列として渡してください
 */
// =======================================================================
extern void GmEffectRectInit(GMS_EFFECT_COM_WORK *efct_com,
							 const Uint16 *atk_flag_tbl, const Uint16 *def_flag_tbl,
							 Uint8 my_group, Uint8 target_group_flag);

// =======================================================================
// GmEnemyDefaultDefFunc
/*!
  エフェクトダメージ食らい処理
  
  @param my_rect	[io]	自分矩形ワークポインタ
  @param your_rect	[io]	相手矩形ワークポインタ
  
  @note
   ppDefへ登録
 */
// =======================================================================
extern void GmEffectDefaultDefFunc(OBS_RECT_WORK *my_rect, OBS_RECT_WORK *your_rect);

// =======================================================================
// GmEnemyDefaultAtkFunc
/*!
  エフェクト攻撃HIT処理
 
  @param my_rect	[io]	自分矩形ワークポインタ
  @param your_rect	[io]	相手矩形ワークポインタ
  
  @note
   ppHitへ登録
 */
// =======================================================================
extern void GmEffectDefaultAtkFunc(OBS_RECT_WORK *my_rect, OBS_RECT_WORK *your_rect);


// ==========================================================================
// エフェクト設定
// ==========================================================================
// =======================================================================
// GmEffect3DESSetupBase
/*!
  エフェクト 3D ES 基本設定
  
  @param efct_3des	[io]	3DESエフェクトワーク
  @param pos_type	[in]	配置タイプ(GME_EFFECT_3DES_POS_TYPE_XXX)
  @param init_flag	[in]	初期化設定フラグ(GMD_EFFECT_3DES_FLAG_XXX)
  
  @note
  ESの回転・平行移動の正常な反映は、フラグ設定に強く依存しているため、
  この関数によって初期設定を行うことを推奨します。
  OBS_OBJECT_WORK::obj_3des が設定されている時のみ有効です。
  設定されていない(=NULL)場合はアサートします。
  OBS_OBJECT_WORK や OBS_ACTION3D_ES_WORK の設定が上書きされますので、
  さらにフラグ設定などを行う場合は、この関数呼び出しの後に行ってください。
  デフォルトで、OBD_DISP_ENDフラグが立ったときに自分を消去する処理関数が設定されます。
  （必要であれば自前の処理関数に上書きしても問題ありません。）
 */
// =======================================================================
extern void GmEffect3DESSetupBase(GMS_EFFECT_3DES_WORK *efct_3des,
								  GME_EFFECT_3DES_POS_TYPE pos_type, Uint32 init_flag);


// =======================================================================
// GmEffect3DESChangeBase
/*!
  エフェクト 3D ES 基本設定 変更
  
  @param efct_3des	[io]	3DESエフェクトワーク
  @param pos_type	[in]	配置タイプ(GME_EFFECT_3DES_POS_TYPE_XXX)
  @param init_flag	[in]	初期化設定フラグ(GMD_EFFECT_3DES_FLAG_XXX)
  
  @note
  処理関数を変更せずに基本設定を上書き変更します。
 */
// =======================================================================
extern void GmEffect3DESChangeBase(GMS_EFFECT_3DES_WORK *efct_3des,
								   GME_EFFECT_3DES_POS_TYPE pos_type, Uint32 init_flag);


// =======================================================================
// GmEffect3DESSetDispOffset
/*!
  エフェクト 3DES 表示オフセット設定
  
  @param efct_3des	[io]	エフェクト 3DES ワーク
  @param ofst_x		[in]	表示オフセットX
  @param ofst_y		[in]	表示オフセットY
  @param ofst_z		[in]	表示オフセットZ
  
  @note
  表示座標オフセットを設定します。
  この関数で設定した値はオブジェクト自体の座標には影響しません。
  右手系（Y上）の座標系で、表示回転オフセットのみが適用された状態からのオフセットとなります。
 */
// =======================================================================
extern void GmEffect3DESSetDispOffset(GMS_EFFECT_3DES_WORK *efct_3des,
									  Float ofst_x, Float ofst_y, Float ofst_z);

// =======================================================================
// GmEffect3DESAddDispOffset
/*!
  エフェクト 3DES 表示オフセット加算設定
  
  @param efct_3des	[io]	エフェクト 3DES ワーク
  @param ofst_add_x	[in]	表示オフセット加算値X
  @param ofst_add_y	[in]	表示オフセット加算値Y
  @param ofst_add_z	[in]	表示オフセット加算値Z
  
  @note
  既に設定されている表示座標オフセットに対して加算した値を設定します。
  この関数で設定した値はオブジェクト自体の座標には影響しません。
  右手系（Y上）の座標系で、表示回転オフセットのみが適用された状態からのオフセットとなります。
 */
// =======================================================================
extern void GmEffect3DESAddDispOffset(GMS_EFFECT_3DES_WORK *efct_3des,
									  Float ofst_add_x, Float ofst_add_y, Float ofst_add_z);

// =======================================================================
// GmEffect3DESSetDispOffsetCircleX
/*!
  X軸周りの円上に表示オフセットを設定
 
  @param efct_3des	[io]	エフェクト 3DES ワーク
  @param radius		[in]	半径
  @param angle		[in]	角度
  
  @note
  X軸周りの半径"radius"の円上（YZ平面上）に表示オフセットを設定します。
 */
// =======================================================================
extern void GmEffect3DESSetDispOffsetCircleX(GMS_EFFECT_3DES_WORK *efct_3des,
											 Float radius, Angle16 angle);

// =======================================================================
// GmEffect3DESSetDispRotation
/*!
  エフェクト 3DES 表示回転設定
  
  @param efct_3des	[io]	エフェクト 3DES ワーク
  @param rot_x		[in]	表示回転X
  @param rot_y		[in]	表示回転Y
  @param rot_z		[in]	表示回転Z
  
  @note
  表示回転オフセットを設定します。
  この関数で設定した値はオブジェクト自体の座標や角度に影響しません。
  右手系（Y上）の座標系で、各種座標変換が適用されていない状態からの回転オフセットとなります。
 */
// =======================================================================
extern void GmEffect3DESSetDispRotation(GMS_EFFECT_3DES_WORK *efct_3des,
										Angle16 rot_x, Angle16 rot_y, Angle16 rot_z);

// =======================================================================
// GmEffect3DESAddDispRotation
/*!
  エフェクト 3DES 表示回転加算設定
  
  @param efct_3des	[io]	エフェクト 3DES ワーク
  @param rot_add_x	[in]	表示回転加算値X
  @param rot_add_y	[in]	表示回転加算値Y
  @param rot_add_z	[in]	表示回転加算値Z
  
  @note
  既に設定されている表示回転オフセットに対して加算した値を設定します。
  この関数で設定した値はオブジェクト自体の座標や角度に影響しません。
  右手系（Y上）の座標系で、各種座標変換が適用されていない状態からの回転オフセットとなります。
 */
// =======================================================================
extern void GmEffect3DESAddDispRotation(GMS_EFFECT_3DES_WORK *efct_3des,
										Angle16 rot_add_x, Angle16 rot_add_y, Angle16 rot_add_z);

// =======================================================================
// GmEffect3DESSetScale
/*!
  エフェクト 3DES スケール設定
  
  @param efct_3des	[io]	エフェクト 3DES ワーク
  @param scale_rate	[in]	スケール値
  
  @note
  オブジェクト自体のスケール値に影響します。
  ESエフェクトは表示スケール設定のために OBS_OBJECT_WORK::scale.x しか参照しませんが、
  見た目とオブジェクトシステムとの整合性（矩形サイズなど）を保つために、
  この関数を使用してスケール設定を行うことを推奨します。
 */
// =======================================================================
inline void GmEffect3DESSetScale(GMS_EFFECT_3DES_WORK *efct_3des, Float scale_rate)
{
	OBS_OBJECT_WORK	*obj_work	= (OBS_OBJECT_WORK*)efct_3des;
	
	obj_work->scale.x	=
		obj_work->scale.y	=
			obj_work->scale.z	= FX_F32_TO_FX32(scale_rate);
	
}

// =======================================================================
// GmEffect3DESSetDuplicateDraw
/*!
  複製描画設定
  
  @param efct_3des	[io]	エフェクト 3DES ワーク
  @param ofst_x		[in]	通常描画位置からのオフセットX（描画座標系）
  @param ofst_y		[in]	通常描画位置からのオフセットY（描画座標系）
  @param ofst_z		[in]	通常描画位置からのオフセットZ（描画座標系）
  
  @note
  通常の描画に加えて、指定したオフセット位置にもう一度描画を行うように設定します。
  設定をクリア・解除するにはGmEffect3DESClearDuplicateDraw()を呼び出してください。
 */
// =======================================================================
extern void GmEffect3DESSetDuplicateDraw(GMS_EFFECT_3DES_WORK *efct_3des,
										 Float ofst_x, Float ofst_y, Float ofst_z);

// =======================================================================
// GmEffect3DESClearDuplicateDraw
/*!
  複製描画解除
  
  @param efct_3des	[io]	エフェクト 3DES ワーク
  
  @note
  複製描画を解除し、設定値をクリアします。
 */
// =======================================================================
extern void GmEffect3DESClearDuplicateDraw(GMS_EFFECT_3DES_WORK *efct_3des);

// =======================================================================
// GmEffectDefaultMainFuncDeleteAtEnd
/*!
  エフェクト メイン処理関数 アニメーション終了時削除
  
  @param obj_work	[io]	オブジェクトワーク
  
  @note
  OBD_DISP_ENDフラグが立った時に自分をクリアします。
 */
// =======================================================================
extern void GmEffectDefaultMainFuncDeleteAtEnd(OBS_OBJECT_WORK *obj_work);

// =======================================================================
// GmEffectDefaultMainFuncDeleteAtEndCopyDirZ
/*!
  エフェクト メイン処理関数 アニメーション終了時削除（親Z角度コピー）
  
  @param obj_work	[io]	オブジェクトワーク
  
  @note
  親のdir.zをコピーします。
  OBD_DISP_ENDフラグが立った時に自分をクリアします。
 */
// =======================================================================
extern void GmEffectDefaultMainFuncDeleteAtEndCopyDirZ(OBS_OBJECT_WORK *obj_work);

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

#endif /* GM_EFFECT_H_ */
