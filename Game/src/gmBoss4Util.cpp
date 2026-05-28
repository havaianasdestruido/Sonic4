// =======================================================================
/*!
  @file	GmBoss4Util.cpp
  @brief ボス4 ユーティリティ関数

  @author K.Yurita(SafariGames)
 				Copyright(c) 2009 Dimps
  $Id: gmBoss4Util.cpp 2 2011-04-11 05:21:26Z thamada $
 */
// =======================================================================
/*
 * $Log$
 */

/*------ Include Files -------------------------------------------------*/
#include "pch.h"
#include "akMath.h"
#include "gs.h"
#include "gmMain.h"
#include "gmGameDBuild.h"
#include "gmGamedat.h"
#include "gmEventTbl.h"
#include "gmCamera.h"
#include "gmSound.h"
#include "gmEnemy.h"
#include "gmEffect.h"
#include "gmEffectCmn.h"
#include "gmEffectBoss.h"
#include "gmBossCommon.h"

#include "gmBoss4.h"
#include "gmBoss4Util.h"

#include "gmPlySeq.h"

#include "gmGmkCamScrLim.h"

#if	defined(_WII) && !defined(_PS3)
#pragma warning off (10178)			// 型宣言のプロトタイプのワーニングをOFFにする
									// ただしstatic宣言はつけれないため、問題がある場合は直接修正する
#pragma warn_impl_s2u_conv off		// signed unsignedの暗黙的な変換に対する警告ははずしておく(マクロ内に書き込んであるため)
#endif	//_WII

//----------------------------------------------------------------------
// データヘッダ
//	TODO : 最終的にBOSS4のデータに変更
//----------------------------------------------------------------------
#include "../file/common/arc/BOSS01.hmb"
#include "../file/common/model/BOSS01_MDL.hmb"
#include "../file/common/model/BOSS01_BODY_MTN.hmb"
#include "../file/common/model/BOSS01_CHAIN_MTN.hmb"
#include "../file/common/model/BOSS01_EGG_MTN.hmb"

/*------ Macros --------------------------------------------------------*/

/*------ Macro Functions -----------------------------------------------*/

/*------ Definitions ---------------------------------------------------*/

/*------ External Declarations -----------------------------------------*/

/*------ Static Declarations -------------------------------------------*/

/*------ Global Variables ----------------------------------------------*/

/*------ Static Variables ----------------------------------------------*/

/*------ Global Functions ----------------------------------------------*/
const	NNS_RGB		gm_boss4_color_white ={ 1.0, 1.0f, 1.0f };


// =======================================================================
// GmBoss4UtilInit1ShotTimer
/*!
  1ショットタイマ 初期化
  
  @param one_shot_timer	[io]	1ショットタイマワーク
 */
// =======================================================================
void GmBoss4UtilInit1ShotTimer( GMS_BOSS4_1SHOT_TIMER* one_shot_timer, Uint32 frame)
{
	MTM_ASSERT(one_shot_timer);
	
	one_shot_timer->timer	= frame;
	one_shot_timer->is_active	= TRUE;
}

// =======================================================================
// GmBoss4UtilUpdate1ShotTimer
/*!
  1ショットタイマ 更新
  
  @param one_shot_timer	[io]	1ショットタイマワーク
  
  @retval TRUE	指定フレーム到達
  @retval FALSE	指定フレームに満たないor超過
  
  @note
  既定フレーム経過したら一度だけTRUEを返すタイマの初期化
 */
// =======================================================================
BOOL GmBoss4UtilUpdate1ShotTimer(GMS_BOSS4_1SHOT_TIMER* one_shot_timer)
{
	MTM_ASSERT(one_shot_timer);
	
	if (one_shot_timer->is_active == FALSE) {
		return FALSE;
	}
	
	if (one_shot_timer->timer) {
		one_shot_timer->timer--;
	}
	else {
		one_shot_timer->is_active	= FALSE;
		return TRUE;
	}
	
	return FALSE;
}

// =======================================================================
//	GmBoss4UtilInitNodeMatrix()
/*!
	ノード参照システム簡易化
  
	@param	obj_work	[io]	オブジェクトワーク
	@param	node_work	[io]	ノードワーク
	@param	max_node	[in]	使用できるNodeMatrixの数

 */
// =======================================================================
void GmBoss4UtilInitNodeMatrix( GMS_BOSS4_NODE_MATRIX* node_work, OBS_OBJECT_WORK* obj_work, Sint32 max_node )
{
	// ノード閲覧システム導入
	node_work->initCount	= max_node;
	node_work->useCount		= 0;

	// BMCBシステム初期化
	GmBsCmnInitBossMotionCBSystem( obj_work, &node_work->mtn_mgr );
	
	// ノードマトリクス取得初期化
	GmBsCmnCreateSNMWork(	&node_work->snm_work,
							obj_work->obj_3d->object,
							(Uint16)max_node );

	// モーションコールバックを実行リストに追加
	GmBsCmnAppendBossMotionCallback( &node_work->mtn_mgr,
									 &node_work->snm_work.bmcb_link );


	node_work->obj_work = obj_work;

	for( int i=0;i< GMD_BOSS4_SNM_NO_MAX; i++){
		node_work->work[ i ] = -1;
	}

	// 初期化されたことを示す
	strcpy( node_work->_id, "SNM SYS" );
}

// =======================================================================
//	GmBoss4UtilExitNodeMatrix()
/*!
	ノード参照システム終了
  
	@param	obj_work	[io]	オブジェクトワーク
	@param	node_work	[io]	ノードワーク
	@param	max_node	[in]	使用できるNodeMatrixの数

 */
// =======================================================================
void GmBoss4UtilExitNodeMatrix( GMS_BOSS4_NODE_MATRIX* node_work )
{
	if (strcmp( node_work->_id, "SNM SYS" )!=0){
		// 初期化されていないか異常
#ifdef	_DEBUG
		amSystemLog( "初期化されていないノードシステムを開放しようとしています" );
#endif	//_DEBUG
		return;
	}

	// ボスモーションコールバックシステム解除・クリア
	GmBsCmnClearBossMotionCBSystem( node_work->obj_work );
	
	// ノードマトリクス取得関連解放
	GmBsCmnDeleteSNMWork( &node_work->snm_work );
	
	// 初期化してない状態に変更
	node_work->_id[0] = '\0';
}

// =======================================================================
//	GmBoss4UtilGetNodeMatrix()
/*!
	ノード参照システム簡易化
  
	@param	node_work	[io]	ノードワーク
	@param	node_id		[in]	ノード番号

 */
// =======================================================================
const NNS_MATRIX* GmBoss4UtilGetNodeMatrix( GMS_BOSS4_NODE_MATRIX* node_work, Sint32 node_id )
{
#ifdef	_DEBUG
	if (node_id < 0){
		amSystemLog( "参照しようとしているノード番号が異常です:%d", node_id );
		return NULL;
	}
	if (node_id >= GMD_BOSS4_SNM_NO_MAX){
		amSystemLog( "参照しようとしているノード番号がGMD_BOSS4_SNM_NO_MAXを越えています : %d >= %d", node_id, GMD_BOSS4_SNM_NO_MAX );
		return NULL;
	}
#endif

	if (node_work->work[ node_id ]<0){

#ifdef	_DEBUG
		// ノード登録が可能?
		if ( node_work->initCount <= node_work->useCount ){
			amSystemLog( "ノード登録ができません。GmBoss4UtilInitNodeMatrix()の際にワークを増やしてください(現在=%d)", node_work->initCount );
			return NULL;		
		}
		node_work->useCount++;
#endif	//_DEBUG

		// ノードマトリクス取得ノード追加
		node_work->work[ node_id ] =
			GmBsCmnRegisterSNMNode( &node_work->snm_work, node_id );

		// 注意: 初期1フレームはきちんとした値が入らない。
		// obj_3dでobj_mtxを確保することでこの呼び出し方ができるのだが・・・。
		/*
		// 初期は即コールバックを呼ぶ
		amMatrixPush(&obj_mtx);
		node_work->obj_work->obj_3d->mtn_cb_func(	node_work->obj_work->obj_3d->motion, 
													node_work->obj_work->obj_3d->object,
													node_work->obj_work->obj_3d->mtn_cb_param);

		amMatrixPop();
		*/
	}
	
	NNS_MATRIX	*w_mtx;

	// ノードのワールドマトリクス取得
	w_mtx	= GmBsCmnGetSNMMtx( &node_work->snm_work,
								   node_work->work[ node_id ] );

	return w_mtx;
}


// =======================================================================
//	GmBoss4UtilSetNodeMatrixNN()
/*!
	ノードにくっつける
  
	@param	node_work	[io]	ノードワーク
	@param	node_id		[in]	ノード番号

 */
// =======================================================================
void	GmBoss4UtilSetNodeMatrixNN( OBS_OBJECT_WORK* obj_work,
								   GMS_BOSS4_NODE_MATRIX* node_work, Sint32 node_id ){

	GmBsCmnUpdateObject3DNNStuckWithNode(obj_work,
										 &node_work->snm_work,
										 node_work->work[ node_id ],
										 TRUE);
}


// =======================================================================
//	GmBoss4UtilSetMatrixNN()
/*!
	マトリクスにくっつける
  
	@param	node_work	[io]	ノードワーク
	@param	node_id		[in]	ノード番号

 */
// =======================================================================
void	GmBoss4UtilSetMatrixNN( OBS_OBJECT_WORK* obj_work, NNS_MATRIX*	w_mtx )
{
	NNS_MATRIX	*user_obj_mtx_r;
	
	MTM_ASSERT(obj_work);
	MTM_ASSERT(obj_work->obj_3d);
	
	// ユーザマトリクス領域取得
	user_obj_mtx_r	= &obj_work->obj_3d->user_obj_mtx_r;
	
	// ノードにくっつける
	obj_work->pos.x	= FX_F32_TO_FX32(NNM_MTX(*w_mtx, 0, 3));
	obj_work->pos.y	= -FX_F32_TO_FX32((NNM_MTX(*w_mtx, 1, 3)));
	obj_work->pos.z	= FX_F32_TO_FX32((NNM_MTX(*w_mtx, 2, 3)));
	
	// 回転
	if (1){	//b_rotation) {
		// ノードの回転を反映
		obj_work->disp_flag	|= OBD_DISP_USERMTX_RIGHT;
		AkMathNormalizeMtx(user_obj_mtx_r, w_mtx);
	}
	else {
		// 単位行列をセット
		obj_work->disp_flag	&= ~OBD_DISP_USERMTX_RIGHT;
		nnMakeUnitMatrix(user_obj_mtx_r);
	}
}

// =======================================================================
//	GmBoss4UtilSetNodeMatrixES()
/*!
	ノードにくっつける
  
	@param	node_work	[io]	ノードワーク
	@param	node_id		[in]	ノード番号

 */
// =======================================================================
void	GmBoss4UtilSetNodeMatrixES( OBS_OBJECT_WORK* obj_work,
								   GMS_BOSS4_NODE_MATRIX* node_work, Sint32 node_id ){

	GmBsCmnUpdateObject3DESStuckWithNode(obj_work,
										 &node_work->snm_work,
										 node_work->work[ node_id ],
										 TRUE);
}

// =======================================================================
//	GmBoss4UtilSetMatrixNN()
/*!
	マトリクスにくっつける
  
	@param	node_work	[io]	ノードワーク
	@param	node_id		[in]	ノード番号

 */
// =======================================================================
void	GmBoss4UtilSetMatrixES( OBS_OBJECT_WORK* obj_work, NNS_MATRIX*	w_mtx )
{
	AMS_QUAT	*user_dir_quat;
	
	NNS_MATRIX	nml_w_mtx;
	
	MTM_ASSERT(obj_work);
	MTM_ASSERT(obj_work->obj_3des);

	// ユーザマトリクス領域取得
	user_dir_quat	= &obj_work->obj_3des->user_dir_quat;
	
	// ノードにくっつける
	obj_work->pos.x	= FX_F32_TO_FX32(NNM_MTX(*w_mtx, 0, 3));
	obj_work->pos.y	= -FX_F32_TO_FX32((NNM_MTX(*w_mtx, 1, 3)));
	obj_work->pos.z	= FX_F32_TO_FX32((NNM_MTX(*w_mtx, 2, 3)));
	
	// 回転
	if (1) {
		// ノードの回転を反映
		obj_work->obj_3des->flag	|= OBD_ACTFLAG_3D_ES_USER_DIR_QUAT;
		
		// 正規化
		AkMathNormalizeMtx(&nml_w_mtx, w_mtx);
		
		// クォータニオンに変換＆設定
		//nnMakeRotateMatrixQuaternion(user_dir_quat, &nml_w_mtx);
	}
	else {
		// 単位クォータニオンをセット
		obj_work->obj_3des->flag	&= ~OBD_ACTFLAG_3D_ES_USER_DIR_QUAT;
		nnMakeUnitQuaternion(user_dir_quat);
		
		nnMakeUnitMatrix(&nml_w_mtx);	// オフセット計算で使用
	}

}

// =======================================================================
// GmBoss4UtilPlayerStop()
/*!
  ソニックをストップさせる
  
  @param b			[in]	ストップ

 */
// =======================================================================
void GmBoss4UtilPlayerStop(BOOL b)
{
	if (b){
		// TODO ソニックを操作不可にし、無敵にする
		GMS_PLAYER_WORK* ply = (GMS_PLAYER_WORK*)GmBsCmnGetPlayerObj();
		ply->no_key_timer = (60*3) * FX32_ONE;
	}else{
		GMS_PLAYER_WORK* ply = (GMS_PLAYER_WORK*)GmBsCmnGetPlayerObj();
		ply->no_key_timer = 0;
	}
}

// =======================================================================
// GmBoss4UtilTimerStop()
/*!
  ゲームタイマーをストップさせる
  
  @param b			[in]	ストップ

 */
// =======================================================================
void GmBoss4UtilTimerStop( BOOL b )
{
	if (b){
		g_gm_main_system.game_flag &= ~GMD_GAME_FLAG_COUNT_GAME_TIME;	// ゲームタイマ停止 
	}else{
		g_gm_main_system.game_flag |= GMD_GAME_FLAG_COUNT_GAME_TIME;	// ゲームタイマ作動
	}
}

// =======================================================================
// 簡易移動関数
// =======================================================================
// GmBoss4UtilInitMove()
/*!
  移動設定
  
  @param _work			[io]	MOVEワーク
  @param start			[in]	スタート位置
  @param end			[in]	エンド位置
  @param count			[in]	フレーム数
  @param type			[in]	0=等間隔/ 1=サインカーブ間隔

  @note
	スタート位置からエンド位置まで進むようにワークを定義する
 */
// =======================================================================
void GmBoss4UtilInitMove(GMS_BOSS4_MOVE* _work, VecFx32* start, VecFx32* end, Sint32 count, Sint32 type )
{
	_work->start.x	= start->x;
	_work->start.y	= start->y;
	_work->start.z	= start->z;

	_work->end.x	= end->x;
	_work->end.y	= end->y;
	_work->end.z	= end->z;

	_work->max_count= count;
	_work->type		= type;

	_work->now_count= 0;
}

// =======================================================================
// GmBoss4UtilUpdateMove()
/*!
  移動
  
  @param _work		[io]	MOVEワーク
  @praam pos		[out]	ポジション(NULLだとかきこまない)

  @note
	スタート位置からエンド位置まで進む
	移動完了時にTRUEを返す
	posには自分でワークを用意する必要がある
 */
// =======================================================================
BOOL GmBoss4UtilUpdateMove(GMS_BOSS4_MOVE* _work, VecFx32* pos )
{
	VecFx32	len;
	len.x = _work->end.x - _work->start.x;
	len.y = _work->end.y - _work->start.y;
	len.z = _work->end.z - _work->start.z;

	if (_work->now_count < _work->max_count){
		_work->now_count++;
	}

	// 到着
	if (_work->now_count >= _work->max_count){
		// max_countが0でも問題ないように
		_work->now_count = _work->max_count;

		_work->pos.x = _work->end.x;
		_work->pos.y = _work->end.y;
		_work->pos.z = _work->end.z;

		if (pos){
			pos->x = _work->end.x;
			pos->y = _work->end.y;
			pos->z = _work->end.z;
		}
		return true;
	}

	// 到着してないとき
	if (_work->type==0){
		// 等間隔に移動

		_work->pos.x = (fx32)(_work->start.x + len.x * ((float)_work->now_count / _work->max_count ));
		_work->pos.y = (fx32)(_work->start.y + len.y * ((float)_work->now_count / _work->max_count ));
		_work->pos.z = (fx32)(_work->start.z + len.z * ((float)_work->now_count / _work->max_count ));
	
	}else{

		if ( ((float)_work->now_count / _work->max_count ) <= 0.5 ){
			fx32 sin = FX_Cos( AKM_DEGtoA32( 180* ((float)_work->now_count / _work->max_count ) ) );

			float f = 0.5f - (sin * (1.0f/FX32_ONE) *0.5f);

			_work->pos.x = _work->start.x + (fx32)( len.x * f );
			_work->pos.y = _work->start.y + (fx32)( len.y * f );
			_work->pos.z = _work->start.z + (fx32)( len.z * f );
		}else{
			fx32 sin = FX_Cos( AKM_DEGtoA32( 180* ((float)_work->now_count / _work->max_count ) ) );
			float f = sin * (1.0f/FX32_ONE) *0.5f;

			_work->pos.x = _work->start.x + (fx32)( len.x*( 0.5f - f) );
			_work->pos.y = _work->start.y + (fx32)( len.y*( 0.5f - f) );
			_work->pos.z = _work->start.z + (fx32)( len.z*( 0.5f - f) );

		}

	}
	if (pos){
		pos->x = _work->pos.x;
		pos->y = _work->pos.y;
		pos->z = _work->pos.z;
	}
	return false;
}


// =======================================================================
// GmBoss4UtilUpdateMovePositon()
/*!
  移動要素をオブジェクトに反映させる
  
  @param move_work		[io]	MOVEワーク
  @param obj_work		[io]	OBJワーク

  @note
	毎フレーム呼ぶ必要がある
 */
// =======================================================================
void GmBoss4UtilUpdateMovePosition(GMS_BOSS4_MOVE* _work, OBS_OBJECT_WORK* obj_work )
{
	obj_work->pos.x = _work->pos.x;
	obj_work->pos.y = _work->pos.y;
	obj_work->pos.z = _work->pos.z;
}



// =======================================================================
// gmBoss4BodyIsDirectionPositiveFromCurrent
/*!
  最短回転が正回転方向か判定
  
  @param body_work		[io]	本体ワーク
  @param target_angle	[in]	目標方向
  
  @retval TRUE	現在の角度→指定角度が正回転方向
  @retval FALSE 現在の角度→指定角度が負回転方向
  
  @note
  現在の角度から指定角度への最短回転が正回転方向か判定します。
  (e.g. 現在の角度が130degで指定角が90degの場合は最短回転は負方向。
        現在の角度が270degだった場合は最短回転は正方向。)
 */
// =======================================================================
BOOL GmBoss4UtilIsDirectionPositiveFromCurrent(GMS_BOSS4_DIRECTION*	_work, Angle16 target_angle)
{
	Angle32	diff_angle;
	
	// ANGLE_MASKすることでAngle16の範囲に収まる。
	// また、32bit型なので必ず正数になる
	diff_angle	= MTD_MATH_ANGLE_MASK & ((Angle32)_work->cur_angle - (Angle32)target_angle);
	
	if (diff_angle >= AKM_DEGtoA32(180)) {
		return TRUE;
	}
	else {
		return FALSE;
	}
}

// =======================================================================
// GmBoss4UtilUpdateDirection
/*!
  ボス１ 向き更新
  
  @param body_work	[io]	本体ワーク
  
  @note
  ボスの向き情報をオブジェクトの角度に反映します。
  毎フレーム呼んでください。
 */
// =======================================================================
#if _IPHONE
void GmBoss4UtilUpdateDirection(GMS_BOSS4_DIRECTION* _work, OBS_OBJECT_WORK* obj_work, BOOL flag)
#else
void GmBoss4UtilUpdateDirection(GMS_BOSS4_DIRECTION* _work, OBS_OBJECT_WORK* obj_work )
#endif // _IPHONE
{
	// HFLIPも設定します
	if (_work->direction == GME_BOSS4_DIR_LEFT){
		obj_work->disp_flag	|= OBD_DISP_HFLIP;
	}else{
		obj_work->disp_flag	&= ~OBD_DISP_HFLIP;
	}

#if _IPHONE
	// フラグが成立していれば回転を与えないで終了
	if (flag) {
		return;
	}
#endif // _IPHONE
	obj_work->dir.y	= (Uint16)_work->cur_angle;
}

// =======================================================================
// GmBoss4UtilSetDirectionNormal
/*!
  標準の角度（真横より正面寄り）に設定
  
  @param body_work	[io]	本体ワーク
  
  @note
  真横よりも少し正面寄りに向くように角度設定を行います。
  GME_BOSS4_DIRで向きは設定されます
 */
// =======================================================================
void GmBoss4UtilSetDirectionNormal( GMS_BOSS4_DIRECTION* _work )
{
/*
	// Directionにより、方向を決定します。
	// HFLIPも設定します
	if (_work->direction == GME_BOSS4_DIR_LEFT){
		obj_work->disp_flag	|= OBD_DISP_HFLIP;
	}else{
		obj_work->disp_flag	&= ~OBD_DISP_HFLIP;
	}
*/
//	if (obj_work->disp_flag	& OBD_DISP_HFLIP) {
	if (_work->direction == GME_BOSS4_DIR_LEFT){
		GmBoss4UtilSetDirection( _work, GMD_BOSS4_LEFTWARD_ANGLE);
	}
	else {
		GmBoss4UtilSetDirection( _work, GMD_BOSS4_RIGHTWARD_ANGLE);
	}
	
	// パラメータクリア
	_work->orig_angle	= 0;
	_work->turn_angle	= 0;
}

// =======================================================================
// GmBoss4UtilSetDirection
/*!
  任意角度設定
  
  @param body_work	[io]	本体ワーク
  @param deg		[in]	角度
  
  @note
  指定の値に角度を設定します。オブジェクトへの反映は行われません。
 */
// =======================================================================
void GmBoss4UtilSetDirection(GMS_BOSS4_DIRECTION* _work, Angle16 deg)
{
	_work->cur_angle	= deg;
}

// =======================================================================
// gmBoss4BodyInitTurn
/*!
  振り向き回転処理を初期化
 
  @param body_work		[io]	本体ワーク
  @param turn_amount	[in]	振り向き回転量
  @param turn_spd		[in]	回転角速度
  
  @note
  turn_amountには現在の角度から差分でどれだけ回転させるか指定します。
  時計回りはマイナス値、反時計回りはプラス値を指定します。
  Angle32で扱える角度の範囲に注意してください。
 */
// =======================================================================
void GmBoss4UtilInitTurn(GMS_BOSS4_DIRECTION* _work,
						 Angle32 turn_amount, Angle32 turn_spd)
{
	MTM_ASSERT(0 == ((1 << 31) & (turn_amount ^ turn_spd))); // 符号比較
	
	_work->orig_angle	= _work->cur_angle;
	_work->turn_angle	= 0;
	_work->turn_amount	= turn_amount;
	_work->turn_spd		= turn_spd;
	
	GmBoss4UtilSetDirection(_work,
							(Angle16)(_work->orig_angle + _work->turn_angle));
}

// =======================================================================
// GmBoss4UtilInitTurn
/*!
  振り向き回転処理を初期化（目標向き指定）
 
  @param body_work		[io]	本体ワーク
  @param dest_angle		[in]	目標となる角度（オフセットではなく、絶対的な角度）
  @param frame			[in]	回転にかけるフレーム数
  @param is_positive	[in]	正方向回転フラグ(TRUE : 正回転, FALSE : 負回転)
  
  @note
  目標の角度と所要フレーム数から、回転量と回転速度を算出して振り向き回転を初期化します。
  GmBoss4UtilUpdateTurn()時にspd_rateを指定するとフレーム数どおりに回転が完了しなくなります。
 */
// =======================================================================
void GmBoss4UtilInitTurn(GMS_BOSS4_DIRECTION* _work,
						 Angle16 dest_angle, Sint32 frame, BOOL is_positive)
{
	Uint16	turn_amount_u16;
	Angle32	turn_amount;
	Angle32	turn_spd;
	
	MTM_ASSERT(frame > 0);
	
	if (is_positive) {
		// 正方向（絶対値360度以内かつ適切な符号になるようにする）
		turn_amount_u16	= (Uint16)((Angle32)dest_angle - (Angle32)_work->cur_angle);
		turn_amount	= (Angle32)turn_amount_u16;
	}
	else {
		// 負方向（絶対値360度以内かつ適切な符号になるようにする）
		turn_amount_u16	= (Uint16)(((Angle32)dest_angle - AKM_DEGtoA32(360)) -
								   ((Angle32)_work->cur_angle - AKM_DEGtoA32(360)));
		turn_amount	= ((Angle32)((Uint32)turn_amount_u16) - AKM_DEGtoA32(360));	// キャストで符号情報が消えてるので戻す
	}
	
	// 回転速度取得
	turn_spd	= turn_amount / frame;
	
	// 回転初期化
	GmBoss4UtilInitTurn(_work, turn_amount, turn_spd);
}


// =======================================================================
// gmBoss4BodyUpdateTurn
/*!
  振り向き回転処理更新
  
  @param body_work	[io]	本体ワーク
  @param spd_rate	[in]	速度係数（デフォルト1.f）
  
  @retval	TRUE	振り向き回転完了
  @retval	FALSE	振り向き回転中
  
  @note
  実際に回転を実施します。deg_addはgmBoss4BodyInitTurn()で指定した回転角と
  同じ符号になるようにしてください。
 */
// =======================================================================
BOOL GmBoss4UtilUpdateTurn(GMS_BOSS4_DIRECTION* _work, Float spd_rate/*=1.f*/)
{
	BOOL	result	= FALSE;
	Float	deg_spd;
	
	MTM_ASSERT(spd_rate >= 0.f);
	
	// 回転角度更新
	deg_spd	= spd_rate * _work->turn_spd;
	MTM_ASSERT(MTM_MATH_ABS(deg_spd) <= (Sint32)0x7fffffff);
	_work->turn_angle	+= (Angle32)deg_spd;
	
	// 目標到達判定
	if (_work->turn_spd > 0) {
		if (_work->turn_angle >= _work->turn_amount) {
			result	= TRUE;
		}
	}
	else if (_work->turn_spd < 0) {
		
		if (_work->turn_angle <= _work->turn_amount) {
			result	= TRUE;
		}
	}
	
	if (result) {
		// 目標角度にきっちりそろえる
		_work->turn_angle	= _work->turn_amount;
	}
	
	// 現在の向き設定
	GmBoss4UtilSetDirection(_work,
							(Angle16)((Angle32)_work->orig_angle + _work->turn_angle));
	
	return result;
}

// =======================================================================
// gmBoss4BodyInitTurnGently
/*!
  緩やか振り向き回転 初期化
 
  @param body_work		[io]	本体ワーク
  @param dest_angle		[in]	目標角度
  @param frame			[in]	回転にかけるフレーム数
  @param is_positive	[in]	正方向回転フラグ(TRUE : 正回転, FALSE : 負回転)
 */
// =======================================================================
void GmBoss4UtilInitTurnGently(GMS_BOSS4_DIRECTION* _work, Angle16 dest_angle,
							   Sint32 frame, BOOL is_positive)
{
	Uint16	turn_amount_u16;
	Float	frame_deg;
	MTM_ASSERT(frame > 0);
	
	_work->orig_angle	= _work->cur_angle;
	_work->turn_angle	= 0;
	_work->turn_spd		= 0;
	
	if (is_positive) {
		// 正方向（絶対値360度以内かつ適切な符号になるようにする）
		turn_amount_u16	= (Uint16)((Angle32)dest_angle - (Angle32)_work->cur_angle);
		_work->turn_amount	= (Angle32)turn_amount_u16;
	}
	else {
		// 負方向（絶対値360度以内かつ適切な符号になるようにする）
		turn_amount_u16	= (Uint16)(((Angle32)dest_angle - AKM_DEGtoA32(360)) -
								   ((Angle32)_work->cur_angle - AKM_DEGtoA32(360)));
		_work->turn_amount	= ((Angle32)((Uint32)turn_amount_u16) - AKM_DEGtoA32(360));	// キャストで符号情報が消えてるので戻す
	}
	
	// 速度カーブ角度初期化
	_work->turn_gen_var		= 0;
	// 速度カーブ決定値初期化（コサイン半回転を0～1.0に対応させるので180degをフレーム数で割る）
	frame_deg	= 180.f / frame;
	MTM_ASSERT(MTM_MATH_ABS(frame_deg) <= (Sint32)0x7fffffff);
	_work->turn_gen_factor	= AKM_DEGtoA32(frame_deg);
	
	GmBoss4UtilSetDirection(_work,
							(Angle16)((Angle32)_work->orig_angle + _work->turn_angle));
}

// =======================================================================
// gmBoss4BodyUpdateTurnGently
/*!
  緩やか振り向き回転 更新
 
  @param body_work	[io]	本体ワーク
 */
// =======================================================================
BOOL GmBoss4UtilUpdateTurnGently(GMS_BOSS4_DIRECTION* _work)
{
	BOOL	result	= FALSE;
	Float	turn_angle_f;
	
	MTM_ASSERT(_work->turn_gen_factor > 0);
	
	_work->turn_gen_var	+= _work->turn_gen_factor;
	if (_work->turn_gen_var >= AKM_DEGtoA32(180)) {
		_work->turn_gen_var	= AKM_DEGtoA32(180);
		result	= TRUE;
	}
	
	// コサインカーブで向きを決定
	turn_angle_f	= (_work->turn_amount) * 0.5f * (1.f - nnCos(_work->turn_gen_var));
	MTM_ASSERT(MTM_MATH_ABS(turn_angle_f) <= (Sint32)0x7fffffff);
	_work->turn_angle	= (Angle32)(turn_angle_f);
	
	if (result) {
		// 目標角度にきっちりそろえる
		_work->turn_angle	= _work->turn_amount;
	}
	
	// 現在の向き設定
	GmBoss4UtilSetDirection(_work,
							(Angle16)((Angle32)_work->orig_angle + _work->turn_angle));
	
	return result;
}


// =======================================================================
// GmBoss4UtilLookAtPlayer
/*!
  緩やか振り向き回転で、勝手にプレイヤーの向きを向く。
 
  @param _work		[io]	方向ワーク
  @param obj_work	[io]	OBJワーク
  @param time		[in]	方向転換時間
 */
// =======================================================================
BOOL GmBoss4UtilLookAtPlayer(GMS_BOSS4_DIRECTION* _work, OBS_OBJECT_WORK* obj_work, Sint32 time)
{
	// プレイヤーの向きが反対になったら方向初期化
	if (GmBsCmnGetPlayerObj()->pos.x < obj_work->pos.x) {

		// プレイヤーが左なら左方向に向く
		_work->direction = GME_BOSS4_DIR_LEFT;
		GmBoss4UtilInitTurnGently( _work, GMD_BOSS4_LEFTWARD_ANGLE, time, FALSE);// TODO : 仮
	}
	else {
		// プレイヤーが右なら右方向に向く
		_work->direction = GME_BOSS4_DIR_RIGHT;
		GmBoss4UtilInitTurnGently( _work, GMD_BOSS4_RIGHTWARD_ANGLE, time, TRUE);// TODO : 仮
	}
	return GmBoss4UtilUpdateTurnGently(_work);
}

// =======================================================================
// GmBoss4UtilLookAtPlayerCheckDirection
/*!
  緩やか振り向き回転で、勝手にプレイヤーの向きを向く。
  (Updateで使用できるようにしたバージョン。ただ初期化が絶対に必要)
 
  @param _work		[io]	方向ワーク
  @param obj_work	[io]	OBJワーク
  @param time		[in]	方向転換時間
 */
// =======================================================================
BOOL GmBoss4UtilLookAtPlayerCheckDirection(GMS_BOSS4_DIRECTION* _work, OBS_OBJECT_WORK* obj_work, Sint32 time)
{
	// プレイヤーの向きが反対になったら方向初期化
	if (GmBsCmnGetPlayerObj()->pos.x < obj_work->pos.x) {

		if (_work->direction!=GME_BOSS4_DIR_LEFT){
			// プレイヤーが左なら左方向に向く
			_work->direction = GME_BOSS4_DIR_LEFT;
			GmBoss4UtilInitTurnGently( _work, GMD_BOSS4_LEFTWARD_ANGLE, time, FALSE);// TODO : 仮
		}
	}
	else {
		if (_work->direction!=GME_BOSS4_DIR_RIGHT){
			// プレイヤーが右なら右方向に向く
			_work->direction = GME_BOSS4_DIR_RIGHT;
			GmBoss4UtilInitTurnGently( _work, GMD_BOSS4_RIGHTWARD_ANGLE, time, TRUE);// TODO : 仮
		}
	}
	return GmBoss4UtilUpdateTurnGently(_work);
}

// =======================================================================
// GmBoss4UtilLookAtCenter
/*!
  緩やか振り向き回転で、勝手に中央を向く。
 
  @param _work		[io]	方向ワーク
  @param obj_work	[io]	OBJワーク
  @param time		[in]	方向転換時間
 */
// =======================================================================
BOOL GmBoss4UtilLookAtCenter(GMS_BOSS4_DIRECTION* _work, OBS_OBJECT_WORK* obj_work, Sint32 time)
{
	// プレイヤーの向きが反対になったら方向初期化
	if ( GMM_BOSS4_AREA_CENTER_X() < obj_work->pos.x) {
		// 自分が右なら左方向に向く
		//obj_work->disp_flag	|= OBD_DISP_HFLIP;
		_work->direction = GME_BOSS4_DIR_LEFT;
		GmBoss4UtilInitTurnGently( _work, GMD_BOSS4_LEFTWARD_ANGLE, time, FALSE);// TODO : 仮
	}
	else {
		// 自分が左なら右方向に向く
		//obj_work->disp_flag	&= ~OBD_DISP_HFLIP;
		_work->direction = GME_BOSS4_DIR_RIGHT;
		GmBoss4UtilInitTurnGently( _work, GMD_BOSS4_RIGHTWARD_ANGLE, time, TRUE);// TODO : 仮
	}
	return GmBoss4UtilUpdateTurnGently(_work);
}

// =======================================================================
// GmBoss4UtilLookAtPlayer
/*!
  緩やか振り向き回転で、勝手その方向を向く
  GmBoss4UtilLookAt***()を先に実行しておく必要がある
 
  @param _work		[io]	方向ワーク
  @param obj_work	[io]	OBJワーク
  @param time		[in]	方向転換時間
 */
// =======================================================================
BOOL GmBoss4UtilLookAt(GMS_BOSS4_DIRECTION* _work )
{
	return GmBoss4UtilUpdateTurnGently(_work);
}


// ############################################################################
// 点滅
// ############################################################################
// =======================================================================
// GmBoss4UtilInitFlicker
/*!
  点滅初期化
 
  @param obj_work	[io]	オブジェクトワーク（obj_3dが設定されている必要があります）
  @param flk_work	[io]	ダメージ点滅ワーク
  @param radius		[in]	モデル半径（ビュー座標系におけるZ方向の厚みだけ考慮すればよい）
  
  @note
  前の点滅状態を引き継がずに強制的に初期化する場合はclean_init=TRUEを設定してください。
 */
// =======================================================================
void GmBoss4UtilInitFlicker( OBS_OBJECT_WORK *obj_work,
										GMS_BOSS4_FLICKER_WORK *flk_work,
										Sint32		times,		// 点滅回数
										Sint32		start,		// 始まるまでの時間
										Sint32		spd,		// 点滅スピード
										Sint32		interval,	// 点滅間隔
										const NNS_RGB*	rgb
										)
{
	Angle32 t = AKM_DEGtoA32( 360.0f / (spd+1) );

	MTM_ASSERT(obj_work);
	MTM_ASSERT(obj_work->obj_3d);
	MTM_ASSERT(flk_work);
	
	flk_work->is_active			= TRUE;
	flk_work->cycles			= times;
	flk_work->interval_timer	= start;
	flk_work->cur_angle			= 0;
	flk_work->add_timer			= t;
	flk_work->interval_flk		= interval;

	// カラーの設定
	flk_work->color.r			= rgb->r;
	flk_work->color.g			= rgb->g;
	flk_work->color.b			= rgb->b;

	// 初期化
	GmBsCmnClearObject3DNNFadedColor(obj_work);
}

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
BOOL GmBoss4UtilUpdateFlicker( OBS_OBJECT_WORK *obj_work,
										  GMS_BOSS4_FLICKER_WORK *flk_work )
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
			flk_work->cur_angle	= flk_work->cur_angle + flk_work->add_timer;
			
			// 0degを通り過ぎたら1cycleとし、次のサイクルは強制的に0degからスタート
			if (flk_work->cur_angle >= AKM_DEGtoA32(360.f)) {
				flk_work->cur_angle	= 0;
				flk_work->cycles--;
				flk_work->interval_timer = flk_work->interval_flk;
			}
		}
		
		// フェードカラー反映
		// （0.0f ～ 1.0f のマイナスコサイン波でintensityを決定）
		GmBsCmnSetObject3DNNFadedColor(obj_work,
									   &flk_work->color,
									   (1.0f - nnCos(flk_work->cur_angle)) / 2);
		
		return FALSE;
	}
	else {
		if (flk_work->is_active) {
			// 終了時にクリアしておく
			GmBoss4UtilEndFlicker(obj_work, flk_work);
		}
		return TRUE;
	}
}

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
void GmBoss4UtilEndFlicker( OBS_OBJECT_WORK *obj_work,
									   GMS_BOSS4_FLICKER_WORK *flk_work)
{
	MTM_ASSERT(obj_work);
	MTM_ASSERT(obj_work->obj_3d);
	MTM_ASSERT(flk_work);
	
	// パラメータクリア
	amZeroMemory(flk_work, sizeof(GMS_BOSS4_FLICKER_WORK));
	
	// フェードカラークリア
	GmBsCmnClearObject3DNNFadedColor(obj_work);
}

// =======================================================================
// GmBoss4UtilRotateVecFx32
/*!
  XY方向ベクトルをZ軸回転で角度分回した結果を返す
  
  @param f		[io]	方向ベクトル
  @param angle	[in]	角度
  
 */
// =======================================================================
void GmBoss4UtilRotateVecFx32( VecFx32* f, Angle32 angle )
{
	NNS_MATRIX		mtx;
//	NNS_VECTOR		pos0, pos1;
	NNM_MTX( mtx, 0, 0) = 1.0f;
	NNM_MTX( mtx, 1, 0) = 0.0f;
	NNM_MTX( mtx, 2, 0) = 0.0f;
	NNM_MTX( mtx, 3, 0) = 0.0f;

	NNM_MTX( mtx, 0, 1) = 0.0f;
	NNM_MTX( mtx, 1, 1) = 1.0f;
	NNM_MTX( mtx, 2, 1) = 0.0f;
	NNM_MTX( mtx, 3, 1) = 0.0f;

	NNM_MTX( mtx, 0, 2) = 0.0f;
	NNM_MTX( mtx, 1, 2) = 0.0f;
	NNM_MTX( mtx, 2, 2) = 1.0f;
	NNM_MTX( mtx, 3, 2) = 0.0f;

	NNM_MTX( mtx, 0, 3) = 0.0f;
	NNM_MTX( mtx, 1, 3) = 0.0f;
	NNM_MTX( mtx, 2, 3) = 0.0f;
	NNM_MTX( mtx, 3, 3) = 1.0f;

	nnMakeRotateZMatrix( &mtx, angle );
	nnTranslateMatrix( &mtx, &mtx, FX_FX32_TO_F32( f->x ), FX_FX32_TO_F32( f->y ), FX_FX32_TO_F32( f->z ) );

	f->x = FX_F32_TO_FX32( NNM_MTX( mtx, 0, 3) )/*mtx[3][0]*/;
	f->y = FX_F32_TO_FX32( NNM_MTX( mtx, 1, 3) )/*mtx[3][1]*/;
	f->z = FX_F32_TO_FX32( NNM_MTX( mtx, 2, 3) )/*mtx[3][2]*/;
}

static	GMS_RING_WORK*	gm_boss4_util_ring = NULL;
// =======================================================================
// GmBoss4UtilIterateDamageRingInit
/*!
  ダメージリングを列挙開始
  
 */
// =======================================================================
void GmBoss4UtilIterateDamageRingInit()
{
	gm_boss4_util_ring = GmRingGetWork()->damage_ring_list_start;
}


// =======================================================================
// GmBoss4UtilIterateDamageRingGet
/*!
  ダメージリングを取得

  @return	ダメージリング構造体(なければNULL)
 */
// =======================================================================
GMS_RING_WORK* GmBoss4UtilIterateDamageRingGet()
{
	GMS_RING_WORK*	ring_work = gm_boss4_util_ring;

	if (ring_work==NULL){
		return NULL;
	}
	gm_boss4_util_ring = ring_work->post_ring;
	return ring_work;
}

// =======================================================================
// GmBoss4UtilSetPlayerReaction
/*!
//	第一段階の
//	カプセルを攻撃した後のBOSS4共通リアクションの設定
  
  @param player		[io]	プレイヤーワーク
  @param enemy		[io]	カプセルワーク
 */
// =======================================================================
void	GmBoss4UtilSetPlayerAttackReaction( OBS_OBJECT_WORK* player, OBS_OBJECT_WORK* enemy )
{
	UNREFERENCED_PARAMETER(enemy);

	GMS_PLAYER_WORK* ply_work = (GMS_PLAYER_WORK*)player;
	OBS_OBJECT_WORK* body_work = GmBoss4GetBodyWork();

	if (ply_work->obj_work.move_flag & OBD_MOVE_JUMP) {
		// プレイヤージャンプ(ホーミング含む)跳ね返り
		GmPlySeqAtkReactionInit(ply_work);
//		if (ply_work->seq_state == GME_PLY_SEQ_STATE_HOMING_REF){
			// ジャンプステート設定
			GmPlySeqSetJumpState(ply_work, 0,
								 (GMD_PLY_SEQ_SETJUMPSTATE_IGNORE_JUMPBTN |
								  GMD_PLY_SEQ_SETJUMPSTATE_NOHOMING));
//		}
		// 水平方向速度設定
		ply_work->obj_work.spd_m = 0;


		// ボスから離れる方向へ
		if (body_work!=NULL){

			if ( ply_work->obj_work.pos.x < body_work->pos.x ){
				ply_work->obj_work.spd.x = -FX_F32_TO_FX32( GMD_BOSS4_PLYATK_HORM_AFTER_SPD_X );
				if (GmBoss4GetScrollOffset()!=0){
					ply_work->obj_work.spd.x = FX_F32_TO_FX32(3.0f);
				}
			}else{
				ply_work->obj_work.spd.x =  FX_F32_TO_FX32( GMD_BOSS4_PLYATK_HORM_AFTER_SPD_X );
			}

		}else{

			if ( ply_work->obj_work.move.x >= 0 ){
				ply_work->obj_work.spd.x = -FX_F32_TO_FX32( GMD_BOSS4_PLYATK_HORM_AFTER_SPD_X );
				if (GmBoss4GetScrollOffset()!=0){
					ply_work->obj_work.spd.x = FX_F32_TO_FX32(3.0f);
				}
			} else {
				ply_work->obj_work.spd.x =  FX_F32_TO_FX32( GMD_BOSS4_PLYATK_HORM_AFTER_SPD_X );
			}
			if (ply_work->seq_state != GME_PLY_SEQ_STATE_HOMING_REF){
	//			ply_work->obj_work.spd.x /=2;
			}
		}
			
		// 垂直方向速度設定
		ply_work->obj_work.spd.y	=  -FX_F32_TO_FX32( GMD_BOSS4_PLYATK_HORM_AFTER_SPD_Y );

		// ジャンプ中移動禁止時間設定
		GmPlySeqSetNoJumpMoveTime(ply_work, 25 * FX32_ONE);//仮 ← 25 frame は少なくとも1stでは最終決定値

	}else {
		// プレイヤー地上跳ね返り
		ply_work->obj_work.disp_flag ^= OBD_DISP_HFLIP;			// 反転させる
		GmPlySeqChangeSequence(ply_work, GME_PLY_SEQ_STATE_SPIN);
		if (ply_work->obj_work.spd_m) {
			ply_work->obj_work.spd_m = -ply_work->obj_work.spd_m;	// 反転させる
		} else {
			fx32 boss_x = 0;
			if (body_work != NULL) {						// 念のため保険でNULLチェック
				boss_x = body_work->pos.x;
			}
			// ソニック停止状態なら位置関係から速度を与える
			if (boss_x > ply_work->obj_work.pos.x) {
				ply_work->obj_work.spd_m = (fx32)(-FX32_ONE * 12);
				ply_work->obj_work.disp_flag |= OBD_DISP_HFLIP;
			} else {
				ply_work->obj_work.spd_m = (fx32)(FX32_ONE * 12);
				ply_work->obj_work.disp_flag &= ~OBD_DISP_HFLIP;
			}
		}
	}

//	ply_work->obj_work.pos.x += GmBoss4GetScrollOffset();
}

// =======================================================================
// GmBoss4UtilCheckPAL()
/*!
	PAL(50fps)かをチェックする
 */
// =======================================================================
BOOL	GmBoss4UtilCheckPAL(){
	return FALSE;
}

// =======================================================================
// GmBoss4UtilIsScrollLocking()
/*!
	画面がロックされているかチェック
 */
// =======================================================================
BOOL	GmBoss4UtilIsScrollLocking()
{

	if (g_gm_main_system.map_fcol.left == 0 && g_gm_main_system.map_fcol.right == g_gm_main_system.map_fcol.map_block_num_x*64){
		return FALSE;
	}
	return TRUE;
}



// =======================================================================
// GmBoss4UtilInitNoHitTimer()
/*!
	時間内、当たりをなくすよう設定を行います。
  
  @param timer_work	[io]	タイマーワーク(Sint32)
  @param ene_com	[io]	GMS_ENEMY_COM_WORK
  @param timer		[in]	カウンター値

 */
// =======================================================================
void	GmBoss4UtilInitNoHitTimer( GMS_BOSS4_NOHIT_TIMER* work, GMS_ENEMY_COM_WORK* ene_com, Sint32 time)
{
	work->ene_com	= ene_com;
	work->timer		= time+1;

	GmBoss4UtilUpdateNoHitTimer( work );
}


// =======================================================================
// GmBoss4UtilUpdateNoHitTimer()
/*!
	時間の更新を行います。
	時間内、当たりをなくします。
  
 */
// =======================================================================
BOOL	GmBoss4UtilUpdateNoHitTimer( GMS_BOSS4_NOHIT_TIMER* work )
{
	GMS_ENEMY_COM_WORK	*ene_com	= (GMS_ENEMY_COM_WORK*)work->ene_com;

	if (work->timer > 0){

		work->timer--;

//		ene_com->rect_work[GMD_ENEMY_RECT_DEF].flag	|= OBD_RECT_NOHIT;
		ene_com->rect_work[GMD_ENEMY_RECT_ATK].flag |= OBD_RECT_NOHIT;
		return FALSE;
	}
	else {
//		ene_com->rect_work[GMD_ENEMY_RECT_DEF].flag	&= ~OBD_RECT_NOHIT;
		ene_com->rect_work[GMD_ENEMY_RECT_ATK].flag &= ~OBD_RECT_NOHIT;
	}
	return TRUE;
}


/*------ Static Functions ----------------------------------------------*/

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
