// ==========================================================================
/*!
  @file gmPlayer.h
  @brief 

  @author Ishizaki
				Copyright(c) 2009 Dimps

  $Id: gmPlayer.h 20 2011-04-22 12:46:46Z thamada $
  $Date:: 2011-04-22 21:46:46 +0900#$
 */
// ==========================================================================
/*
 * Memo
 *
 */

#ifndef GM_PLAYER_H_
#define GM_PLAYER_H_


//----- Include Files -------------------------------------------------------
#include "objObject.h"
#include "gmMain.h"

#include "gmPlayerDat.h"
#include "gmPlySeqDat.h"

#if	defined(__cplusplus)
extern "C" {
#endif

//----- Definitions ---------------------------------------------------------
#if _IPHONE
#define GMD_PLY_USE_CAM_ROT_TRUCK_JUMP	(1)		//!< ジャンプ中のカメラ回転を有効にする
#define GMD_PLY_SAFE_TOUCH_SPIN			(1 & _IPHONE)		//!< タッチ操作でのスピン暴発対応
#else
#define GMD_PLY_USE_CAM_ROT_TRUCK_JUMP	(1)		//!< ジャンプ中のカメラ回転を有効にする
#endif


/// プレイヤーデータ
enum {

	GMD_PLAYER_DATA_SON_MDL	= 0,
	GMD_PLAYER_DATA_SON_TEX,
	GMD_PLAYER_DATA_SSON_MDL,
	GMD_PLAYER_DATA_SSON_TEX,
	GMD_PLAYER_DATA_MOTION,
#if _WII
	GMD_PLAYER_DATA_LOD,
#endif
	//GMD_PLAYER_DATA_RECT,

	GMD_PLAYER_DATA_MAX,
};

// 操作タイプ切り替え◆
//#define GMD_PLAYER_GRAIND_MANIPULATE_NEW		(1)						//!< グラインド新操作有効
//#define GMD_PLAYER_GRAIND_MANIPULATE_TYPE_M		(1)						//!< グラインド新操作有効 タイプM

#define GMD_PLAYER_MTN_NUM	(136)	//!< プレイヤーモーション数

#define GMD_PLAYER_WATER_CHECK_OFST		(-10)	//!< 水中判定オフセット
#define GMD_PLAYER_WATER_FACE_UP_OFST	(4)		//!< 顔水中外判定オフセット

#define GMD_PLAYER_WATER_COUNT_TIME_OFST	(60*FX32_ONE)	//!< 息切れカウンタ 演出開始時間オフセット


// ==========================================================================
// スコア設定
// ==========================================================================
#if 0
// トリックスコア
#define GMD_PLAYER_SCORE_TRICK          ( 200  ) // トリック時の標準点数
#define GMD_PLAYER_SCORE_TRICK_SP       ( 400  ) // 派生トリック時の標準点数
#define GMD_PLAYER_SCORE_TRICK_MOVE     ( 200  ) // 移動トリック時の標準点数
#define GMD_PLAYER_SCORE_TRICK_GRAIND   ( 100  ) // グラインドトリック時の標準点数
#define GMD_PLAYER_SCORE_TRICK_JUST     ( 400  ) // ジャストトリック時の標準点数
#define GMD_PLAYER_SCORE_TRICK_COMBO2   ( 200  ) // トリックコンボ時の標準点数
#define GMD_PLAYER_SCORE_TRICK_COMBO3   ( 400  ) // トリックコンボ時の標準点数
#define GMD_PLAYER_SCORE_TRICK_COMBO4   ( 600  ) // トリックコンボ時の標準点数
#define GMD_PLAYER_SCORE_TRICK_GIMMICK  ( 200  ) // ギミック演出トリック時の標準点数

#define GMD_PLAYER_SCORE_TRICK_MAX		( 99999999 )	//!< トリックスコア最大値

// スピードスコア用
#define GMD_PLAYER_SCORE_WALK      (    0 ) // 
#define GMD_PLAYER_SCORE_DASH      ( 4000 ) // 
#define GMD_PLAYER_SCORE_BOOST     ( 8000 ) // 
#define GMD_PLAYER_SCORE_NITRO     (10000 ) // 

// リングスコア用
#define GMD_PLAYER_SCORE_RING      (   10 ) // リング一枚辺りの得点
#define GMD_PLAYER_SCORE_RING_BOSS ( 1000 ) // リング一枚辺りの得点 ボス戦
#endif

// ==========================================================================
// ライト設定
// ==========================================================================
#if 0
// 水中ライト
#define GMD_WATER_LIGHT0_R			( 31 - 10 )					//!< 0～31 左辺の値が標準のライト
#define GMD_WATER_LIGHT0_G			( 31 -  8 )
#define GMD_WATER_LIGHT0_B			( 31 -  0 )
#define GMD_WATER_LIGHT1_R			( 17 -  9 )					//!< 0～31
#define GMD_WATER_LIGHT1_G			( 17 -  7 )
#define GMD_WATER_LIGHT1_B			( 17 -  0 )
#define GMD_WATER_LIGHT2_R			(  8 -  8 )					//!< 0～31
#define GMD_WATER_LIGHT2_G			(  8 -  6 )
#define GMD_WATER_LIGHT2_B			(  8 -  0 )

// 残像ライト
#define GMD_BOOST_SONIC_LIGHT		( GX_RGB(  2, 12, 24) )		//!< 0～31
#define GMD_BOOST_BLAZE_LIGHT		( GX_RGB( 24, 20,  6) )		//!< 0～31
#endif

// ==========================================================================
// キー設定
// ==========================================================================
#if _WII
#define PAD_BUTTON_JUMP			( KEY_R_RIGHT )						//!< ジャンプボタン
#define PAD_BUTTON_TRANSFORM	( KEY_R_DOWN )						//!< 変身ボタン
#else
#define PAD_BUTTON_JUMP			( KEY_R_RIGHT | KEY_R_DOWN )		//!< ジャンプボタン
#define PAD_BUTTON_TRANSFORM	( KEY_R_LEFT | KEY_R_UP )			//!< 変身ボタン
#endif
//#define PAD_BUTTON_TRICK ( PAD_BUTTON_Y | PAD_BUTTON_X )
//#define PAD_BUTTON_NITRO ( PAD_BUTTON_Y | PAD_BUTTON_X )
//#define PAD_BUTTON_NITRO ( PAD_BUTTON_Y )

/// キーマッピング
typedef enum _GME_PLAYER_KEY_MAP {
	// クラシック
    GMD_PLAYER_KEY_MAP_UP		= 0,
    GMD_PLAYER_KEY_MAP_DOWN,
    GMD_PLAYER_KEY_MAP_LEFT,
    GMD_PLAYER_KEY_MAP_RIGHT,

	// 共通
    GMD_PLAYER_KEY_MAP_A,
    GMD_PLAYER_KEY_MAP_B,
    GMD_PLAYER_KEY_MAP_X,
    GMD_PLAYER_KEY_MAP_Y,
	
	// ノーマル
//	GMD_PLAYER_KEY_MAP_ROT_Z,		// wii : x 軸   ps3 : z 軸？

    GMD_PLAYER_KEY_MAP_MAX
} GME_PLAYER_KEY_MAP;

// ==========================================================================
// 矩形設定
// ==========================================================================
typedef enum _GME_PLAYER_RECT {
//	GMD_PLAYER_RECT_ID_BODY	= 0,		//!< 体あたり矩形
//	GMD_PLAYER_RECT_ID_ATK,				//!< 攻撃矩形
//	GMD_PLAYER_RECT_ID_NETVS_BODY,		//!< 対戦時相手側押し合い体矩形

	GMD_PLAYER_RECT_DEF	= 0,		//!< くらい矩形
	GMD_PLAYER_RECT_ATK,			//!< 攻撃矩形
	GMD_PLAYER_RECT_BODY,			//!< 体あたり(存在判定)

	GMD_PLAYER_RECT_NUM
} GME_PLAYER_RECT;


/// プレイヤーMAP端画面オフセット
#define GMD_PLAYER_MAP_END_OFST					( GMD_PL_REC_R + 8 )	//!< マップの端判定オフセット

// ==========================================================================
// 攻撃専用オブジェクト
// ◆後で別ソースに分離するかも
// ==========================================================================
typedef struct _GMS_OBJECT_WORK_ATK {
	OBS_OBJECT_WORK		obj;
	OBS_RECT_WORK		rect_work[2];

} GMS_OBJECT_WORK_ATK;


// ==========================================================================
// 通信設定
// ==========================================================================
typedef struct _GMS_PLAYER_PACKET
{
#if 1
	// 送信データ
	VecFx32	pos;			//!< 座標 1:19:12

	u16		disp_flag;		//!< 表示フラグ

	s16		anime_speed;	//!< アニメーション速度
	u8		act_state;		//!< アクション状態
	u8		dir_x;			//!< 角度(下位8bit切り捨て)
	u8		dir_y;
	u8		dir_z;
	u32		move_flag;

	u32		player_flag;	//!< NOKEYなど PACKET容量が足りなくなったら必要なフラグだけに絞る
	u32		gmk_flag;		//!< BELTなど  PACKET容量が足りなくなったら必要なフラグだけに絞る
	s16		move_x;			//!< 当たりチェック時に使用1:7:8
	s16		move_y;
	fx32	camera_pos_x;	//!< カメラ座標
	fx32	camera_pos_y;	//!< カメラ座標
	u32		time;			//!< グローバルタイマー
#else
    // 送信データ
	VecFx32 vPos;		///< 座標 1:19:12

    u32 ulDispFlag;		// 表示フラグ

    fx32 fAnimeSpeed;	// アニメーション速度
    u8 ucState;			// アクション状態
    u8 ucDirX;			// 角度(下位8bit切り捨て)
    u8 ucDirY;
    u8 ucDirZ;
//	VecU16  vDir;		///< オブジェクト角度 8:8 (注意、Fxには合わせていない)
    u32 ulMoveFlag;

    u32 ulPlayerFlag;	// NOKEYなど PACKET容量が足りなくなったら必要なフラグだけに絞る
    u32 ulGimmickFlag;	// BELTなど  PACKET容量が足りなくなったら必要なフラグだけに絞る
	s16 sMoveX;			// 当たりチェック時に使用1:7:8
    s16 sMoveY;
    fx32 fCameraPosX;	// カメラ座標
    fx32 fCameraPosY;	// カメラ座標
    u32 time;			// グローバルタイマー
#endif
} GMS_PLAYER_PACKET; 


// ==========================================================================
// プレイヤーオブジェクト
// ==========================================================================
/// プレイヤーモデルタイプ
enum {
	GMD_PLY_MODEL_TYPE_NORMAL	= 0,	//!< 通常
	GMD_PLY_MODEL_TYPE_SPIN,			//!< スピン
#if _IPHONE
	GMD_PLY_MODEL_TYPE_IPHONE,			//!< iPhone再生中
	GMD_PLY_MODEL_TYPE_SPINJUMP,		//!< スピンジャンプ系
#endif // _IPHONE

	GMD_PLY_MODEL_TYPE_MAX
};

/// プレイヤー管理モデルセット
typedef enum tag_GME_PLY_MODEL_SET {
	GMD_PLY_MODEL_SET_NORMAL	= 0,	//!< 通常(ソニック)モデル
	GMD_PLY_MODEL_SET_SUPER,			//!< スーパー(ソニック)モデル

	GMD_PLY_MODEL_SET_MAX
} GME_PLY_MODEL_SET;
	
#if _IPHONE
typedef enum tag_GME_PLY_SPIN_STATE {
	GMD_PLY_SPIN_STATE_NON = 0, //!< なし
	GMD_PLY_SPIN_STATE_NORMAL, //!< 通常スピン
	GMD_PLY_SPIN_STATE_TURN, //!< ターン待ち
	GMD_PLY_SPIN_STATE_WALK, //!< 歩きからスピン
		
	GMD_PLY_SPIN_STATE_MAX
} GME_PLY_SPIN_STATE;

/// 操作設定
typedef enum tag_GME_PLAYER_CONTROL_TYPE {
	GME_PLAYER_CONTROL_TYPE_A, //!< DPad Aタイプ
	GME_PLAYER_CONTROL_TYPE_B, //!< DPad Bタイプ
	GME_PLAYER_CONTROL_TYPE_TILT, //!< 傾斜タイプ
	
	GME_PLAYER_CONTROL_TYPE_NUM //!< 総数
} GME_PLAYER_CONTROL_TYPE;
#endif // _IPHONE

/// プレイヤーオブジェクト
typedef struct tag_GMS_PLAYER_WORK
{
	OBS_OBJECT_WORK			obj_work;
	OBS_ACTION3D_NN_WORK	*obj_3d[GMD_PLY_MODEL_TYPE_MAX];							//!< 描画モデルオブジェクト
	OBS_ACTION3D_NN_WORK	obj_3d_work[GMD_PLY_MODEL_TYPE_MAX*GMD_PLY_MODEL_SET_MAX];	//!< 管理描画モデルオブジェクトワーク
//	OBS_ACTION3D_NN_WORK	obj_3d_sp;		//!< 描画モデルオブジェクト スーパーソニック
//	OBS_ACTION3D_NNS_WORK	obj_3d_opt;		//!< 描画モデルオブジェクト プレイヤーオプション(尾等)
//	OBS_ACTION2D_WORK		obj_2d;			//!< 矩形取得用
	OBS_RECT_WORK			rect_work[GMD_PLAYER_RECT_NUM];	//!< 矩形データ

	u8		char_id;						//!< キャラクター番号 
	u8		player_id;						//!< プレイヤーID   _nl_playerの並び
	u8		ctrl_id;						//!< コントローラID 1p2p3p4pの並び
	u8		camera_no;						//!< カメラ番号 （多人数時使用）
	
	// シーケンス用データ
#if _IPHONE
	GME_PLY_SPIN_STATE      spin_state; //!< プレイヤースピン状態 iPhone専用
#endif // _IPHONE
	GME_PLY_ACT_STATE		act_state;		//!< プレイヤーアクション状態
	GME_PLY_ACT_STATE		prev_act_state;	//!< プレイヤーアクション状態 前のアクション
	GME_PLY_SEQ_STATE		seq_state;		//!< プレイヤーシーケンス状態
	GME_PLY_SEQ_STATE		prev_seq_state;	//!< プレイヤーシーケンス状態 前のシーケンス
	s32		timer;
	u32		player_flag;					//!< プレイヤーフラグ
	u32		gmk_flag;						//!< ギミックフラグ
	u32		gmk_flag2;						//!< ギミックフラグ2
	fx32	dash_power;						//!< ダッシュ溜め値

	fx32	prev_walk_roll_spd_max;			//!< 歩き時の傾斜対応最大速度保存

	//s32		jump_nofall_timer;				//!< ジャンプ中FALL無しタイマー

	// 動作関数群
	void	(*seq_func)(struct tag_GMS_PLAYER_WORK*);		//!< シーケンス
	// シーケンステーブル
	void	(*const*seq_init_tbl)(struct tag_GMS_PLAYER_WORK*);	//!< シーケンス初期化処理テーブル
	// シーケンスステートデータテーブル
	const GMS_PLY_SEQ_STATE_DATA	*seq_state_data_tbl;		//!< シーケンスステートデータテーブル


	NNS_MATRIX				ex_obj_mtx_r;		///< 演出用オブジェクトMATRIX 右掛

	// 必ず関数を設定しておく事
//	void	(*ppFw)(struct tag_GMS_PLAYER_WORK*);		//!< フットワーク
//	void	(*ppWalk)(struct tag_GMS_PLAYER_WORK*);		//!< 歩き
//	void	(*ppSquat)(struct tag_GMS_PLAYER_WORK*);	//!< しゃがみ
//	void	(*ppJump)(struct tag_GMS_PLAYER_WORK*);		//!< ジャンプ
//	void	(*ppJumpAtk)(struct tag_GMS_PLAYER_WORK*);	//!< ジャンプアクション（ホーミングなど）

#if _WII
	GMS_PLY_LOD_MTN_HEADER	*hand_lod_header;	//!< Wii用手ロッド切り替えデータヘッダ
	GMS_PLY_LOD_MTN	*hand_lod_mtn;				//!< Wii用手ロッド切り替えモーションデータ
	GMS_PLY_LOD_PAT	*hand_lod_pat;				//!< Wii用手ロッド切り替えパターンデータ
	u32				hand_lod_pat_no;			//!< Wii用手ロッド切り替えパターンNO
	u32		hand_mat_user_data;					//!< Wii用手ロッド切り替え設定

#endif
#if _IPHONE
	s16		spin_se_timer;						//!< スピンSE(高音)再生タイマー
	s16		spin_back_se_timer;					//!< スピンSE(低音)再生タイマー
#endif // _IPHONE

#if 0	// 使用されていない
	void	(*ppTrick)();					//!< トリックテンション
	void 	(*ppTrickAction)();				//!< トリックアクション
#endif

	// プレイヤー固有パラメタ
	s16		tension;						//!< テンション値
	s16		over_limit_spd;					//!< 坂道などで速度が限界を超えた時の速度保持ワーク
	fx32	no_spddown_timer;				//!< このタイマーが残っている間、自然減速度が減少する
	
	// プレイヤー固有動作値設定
	fx32	spd_add;						//!< 通常 加速値
	fx32	spd_max;						//!< 通常 最大速度値
	fx32	spd_dec;						//!< 通常放置 減速値	// ここから
	fx32	spd_spin;						//!< スピン 初速値
	fx32	spd_add_spin;					//!< スピン 加速値
	fx32	spd_max_spin;					//!< スピン 最大速度値
	fx32	spd_dec_spin;					//!< スピン放置 減速値
	fx32	spd_max_boost;					//!< ブースト 最大速度値
	fx32	spd_add_nitro;					//!< ニトロ 加速値
	fx32	spd_max_nitro;					//!< ニトロ 最大速度値
	fx32	spd_dec_nitro;					//!< ニトロ放置 減速値
	fx32	spd_chk_nitro;					//!< ニトロダウン（解除） 速度値
	fx32	spd_max_add_slope;				//!< 坂道時の最大速度アップ値
	fx32	spd_jump;						//!< ジャンプ 初速値
	fx32	spd_work_max;					//!< 坂道など一瞬臨界突破値(プレイヤー固有動作値外)
	fx32	spd_jump_add;					//!< ジャンプ横方向 加速値
	fx32	spd_jump_max;					//!< ジャンプ横方向 最大速度値
	fx32	spd_jump_dec;					//!< ジャンプ横方向放置 減速値	// ここから
	fx32	spd_add_spin_pinball;			//!< ピンボールスピンスピン 加速値
	fx32	spd_max_spin_pinball;			//!< ピンボールスピンスピン 最大速度値
	fx32	spd_dec_spin_pinball;			//!< ピンボールスピンスピン放置 減速値
	fx32	spd_max_add_slope_spin_pinball;	//!< ピンボールスピンスピン 坂道時の最大速度アップ値

	fx32	time_air;						//!< 空気の持ち時間
	fx32	time_damage;					//!< ダメージ後の無敵時間

	fx32	spd1;							//!< 速度１段階
	fx32	spd2;							//!< 速度２段階
	fx32	spd3;							//!< 速度３段階
	fx32	spd4;							//!< 速度４段階
	fx32	spd5;							//!< 速度５段階

	// SE用
//	NNSSndHandle	h_snd_se;				//!< 汎用SEハンドル
//	NNSSndHandle	h_snd_graind;			//!< グラインドSEハンドル
//	NNSSndHandle	h_snd_boost;			//!< ニトロブーストSEハンドル
//	NNSSndHandle	h_snd_gallery;			//!< 歓声SEハンドル

	// 速度関連
	VecFx32	boost_pos1;						//!< 前前フレーム座標
	VecFx32	boost_pos2;						//!< 前前前フレーム座標

	s16		spd_pool;						//!< 速度プール

	// リング
	s16		ring_num;						//!< 取得リング
	s16		ring_stage_num;					//!< ステージ累計取得リング

	// スコア
	u32		score;							//!< スコア

	// 各種タイマー
	fx32	invincible_timer;				//!< ダメージ後無敵タイマー
	fx32	genocide_timer;					//!< アイテム後無敵タイマー
//	s16		nitro_timer;					//!< ニトロ限界突破タイマー
//	s16		tension_combo_timer;			//!< 敵倒しテンションコンボタイマー
//	s16		tension_combo;					//!< 敵倒しテンションコンボ数
	fx32	pressure_timer;					//!< 押しつぶされ死亡タイマ
//	s16		multi_nohit_timer;				//!< 通信相手当たり無視タイマー
//	s16		confusion_timer;				//!< 混乱タイマー
//	s16		slow_timer;						//!< スロウタイマー
//	s16		invincible_item_timer;			//!< アイテムが取れないタイマー（連続引き寄せを起こさないためにに引き寄せられた時に使用）
	fx32	disapprove_item_catch_timer;	//!< アイテムが取れないタイマー(連続引き寄せを起こさないためにに引き寄せられた時に使用)
	fx32	water_timer;					//!< 水中タイマ
	fx32	no_key_timer;					//!< キー入力無効タイマー
//	s16		trick_act_timer;				//!< トリック制御用タイマー
//	s16		hs_trick_timer;					//!< 高速トリック用タイマー
//	s16		afterimage_timer;				//!< 残像表示タイマー
	fx32	homing_timer;					//!< ホーミング有効化待機タイマー
	fx32	hi_speed_timer;					//!< ハイスピード状態
	fx32	homing_boost_timer;				//!< ホーミング範囲ブーストタイマー
    
//	s16		blaze_timer;					//!< ブレイズ横トリックタイマー
//	fx32	blaze_prev_spd;					//!< ブレイズ横トリック前速度

	fx32	fall_timer;						//!< 落下タイマー	(プレイヤー固有動作値)
//	u8		boost_out_timer;				//!< ブースト終了タイマ
//	u8		nitro_ban_timer;				//!< ニトロ禁止タイマ
	fx32	no_jump_move_timer;				//!< ジャンプ移動不可解除タイマー
	fx32	maxdash_timer;					//!< 最大ダッシュ状態解除タイマー
	fx32	super_sonic_ring_timer;			//!< スーパーソニック用リング減算タイマー
	
	// カメラ関連
	fx32	camera_stop_timer;				//!< カメラ停止時間
	fx32	camera_ofst_x;					//!< カメラとプレイヤーとの距離
	fx32	camera_ofst_y;					//!< 
	fx32	camera_ofst_tag_x;				//!< カメラとプレイヤーとの距離目標値
	fx32	camera_ofst_tag_y;				//!< 
	fx32	camera_jump_pos_y;				//!< ジャンプ中に中心として扱うY座標

	// 描画設定
	//float	disp_ofst_y;					//!< 描画オフセット

	// トリック
//	void	*trick_work;					//!< トリック演出管理ワーク(トリック演出内で自動設定)
//	void	*trick_score_work;				//!< トリックスコア演出管理ワーク(トリックスコア演出内で自動設定)
//	u8		trick_combo;					//!< トリックコンボ
//	u8		trick_count;					//!< トリック減衰値(使用中ギミック毎のトリック使用済み回数)

	// ホーミング （ソニック専用かも
	OBS_OBJECT_WORK	*enemy_obj;
	OBS_OBJECT_WORK	*cursol_enemy_obj;		//!< カーソル生成対象エネミー

	// ホバー （ブレイズ専用かも
//	s16		hover_timer;
//	s16		trick_fire_timer;

	// プログラムターン
	u16		pgm_turn_dir;					//!< プログラム的ターン
	u16		pgm_turn_spd;					//!< ターン速度
	u16		*pgm_turn_dir_tbl;				//!< ターン用データテーブル
	s32		pgm_turn_tbl_cnt;				//!< ターン用データ再生用
	s32		pgm_turn_tbl_num;				//!< ターン用データテーブル数

	// 落下ターン
	GME_PLY_ACT_STATE		fall_act_state;		//!< 落下ターンする前の落下アクトステート

	// オートラン
	fx32	scroll_spd_x;					//!< オートラン時の画面スクロール速度 X
	//fx32	scroll_spd_y;					//!< オートラン時の画面スクロール速度 Y

	// 敵倒しスコアコンボ
	u32		score_combo_cnt;				//!< 敵倒し時スコアコンボカウンタ

	// ギミック用
//	GMS_EVE_RECORD_EVENT	*trick_gmk_eve_rec;	//!< トリック発生ギミックイベントレコード
	OBS_OBJECT_WORK			*gmk_obj;			//!< ギミックオブジェクト
	s16						gmk_camera_ofst_x;	//!< ギミック注視カメラオフセット
	s16						gmk_camera_ofst_y;
	s16						gmk_camera_center_ofst_x;		//!< 演出用カメラ中心点オフセットX (gmk_obj に依存せずに設定したい場合)
	s16						gmk_camera_center_ofst_y;		//!< 演出用カメラ中心点オフセットY
	s16						gmk_camera_gmk_center_ofst_x;	//!< ギミック依存演出用カメラ中心点オフセットX (gmk_obj==NULL 時に自動破棄したい場合)
	s16						gmk_camera_gmk_center_ofst_y;	//!< ギミック依存演出用カメラ中心点オフセットY
	s16						gmk_map_limit_left;		//!< マップ限界設定 GMD_PLGF_MAP_LIMIT_LCD_X(gmk_objに依存せず)
	s16						gmk_map_limit_right;	//!< マップ限界設定 GMD_PLGF_MAP_LIMIT_LCD_X(gmk_objに依存せず)
	s16						gmk_map_limit_top;		//!< マップ限界設定 GMD_PLGF_MAP_LIMIT_LCD_Y(gmk_objに依存せず)
	s16						gmk_map_limit_bottom;	//!< マップ限界設定 GMD_PLGF_MAP_LIMIT_LCD_Y(gmk_objに依存せず)
	s32						gmk_work0;			//!< Gimmick用ワーク
	s32						gmk_work1;
	s32						gmk_work2;
	s32						gmk_work3;
#if 0	// ◆使われていない
	s32 lFrontWallPosY; // 張り付き壁用 壁Y位置
	s32 lFrontWallPosZ; // 張り付き壁用 壁Z位置
#endif
	void					*opt_anime;			//!< 追加CAモーションデータ

	// ギミック固有
	u16						prev_dir_fall;		//!< 重力方向 前重力(4方向)
	u16						prev_dir_fall2;		//!< 重力方向 前重力(4方向)
	fx32					dir_fall_fix_timer;	//!< 重力方向 固定時間

	Angle32					ply_pseudofall_dir;				//!< ローカル擬似重力
	u16						jump_pseudofall_dir;			//!< ジャンプ中 擬似重力方向
	u16						jump_pseudofall_eve_id_set;		//!< ジャンプ中 擬似重力方向 今フレームセット(イベントID格納)
	u16						jump_pseudofall_eve_id_cur;		//!< ジャンプ中 擬似重力方向 現在有効 (イベントID格納)
	u16						jump_pseudofall_eve_id_wait;	//!< ジャンプ中 擬似重力方向 待機中(イベントID格納)

	fx32					truck_left_flip_timer;			//!< トロッコ左振り向きようタイマ

	OBS_OBJECT_WORK			*truck_obj;						//!< トロッコギミックオブジェクトワーク
	u16						truck_prev_dir;					//!< トロッコ時前フレーム角度取得
	u16						truck_prev_dir_fall;			//!< トロッコ時前フレーム重力角度
	NNS_MATRIX				truck_mtx_ply_mtn_pos;			//!< プレイヤーの存在位置取得 ぶら下がり死亡時など

#if GMD_PLY_USE_CAM_ROT_TRUCK_JUMP
	u16						truck_stick_prev_dir;			//!< 踏ん張り状態になる前の角度
#endif

	// エフェクト管理
	OBS_OBJECT_WORK			*efct_spin_jump_blur;		//!< スピンジャンプブラー
	OBS_OBJECT_WORK			*efct_spin_dash_blur;		//!< スピンダッシュブラー
	OBS_OBJECT_WORK			*efct_spin_dash_cir_blur;	//!< スピンダッシュサークルブラー
	OBS_OBJECT_WORK			*efct_spin_start_blur;		//!< スピン移行ブラー
	OBS_OBJECT_WORK			*efct_run_spray;			//!< 半水中走り水しぶき

	// ライト演出
	float					light_rate;				//!< スーパーソニックライト演出用 リムライト設定
	s32						light_anm_flag;			//!< スーパーソニックライト演出用 ライト方向

	// 演出対応
	// 呪われ演出
	s16						speed_curse;		//!< 現在呪われている数カウンタ(毎フレーム更新)
	s16						prev_speed_curse;	//!< 現在呪われている数カウンタ 前フレーム分

	fx32	warp_pos_x;						//!< ワープ先座標
	fx32	warp_pos_y;						//!< 

	// グラインド
	u8		graind_id;						//!< グラインドID
	u8		graind_prev_ride;				//!< グラインド矩形接触前の面

	// パレット保持用
//	OBS_PALETTE_TEX	tex_plt;

	// スペステ
	s16		nudge_di_timer;					//!< 揺らし禁止時間
	s16		nudge_timer;					//!< 揺らし時間
	fx32	nudge_ofst_x;					//!< 揺らしオフセット量
#if _PS3
	AOT_AXIS axis_y;							//!< 揺らし判定用axis値
#endif // _PS3
#if _IPHONE
	BOOL is_nudge;							//!< 揺らしがあったか否か
	NNS_VECTOR calc_accel;					//!< 揺らし判定計算用加速度パラメータ
#endif // _IPHONE

	// キー
	u16		key_on;
	u16		key_push;
	u16		key_repeat;
	u16		key_release;
	Angle32	key_rot_z;			//!< 傾き
	Angle32	key_walk_rot_z;		//!< 歩き用傾き
//	u32		*pActionIndex;						//!< アクションインデクステーブル◆現在使っていない
	u16		key_map[GMD_PLAYER_KEY_MAP_MAX];	//!< キーマップテーブル

	s32		key_repeat_timer[GMD_PLAYER_KEY_MAP_MAX];	// キーリピート用タイマー

#if _IPHONE
	Angle32	prev_key_rot_z;		//!< 前フレームの傾き
	s32 accel_counter;			//!< 加速度カウンタ
	s32 dir_vec_add;			//!< 角度追加設定
	GME_PLAYER_CONTROL_TYPE control_type; //!< 操作タイプ
	u16 *jump_rect; //!< ジャンプ入力判定用矩形
	u16 *ssonic_rect; //!< SSonic入力判定用矩形
#if GMD_PLY_SAFE_TOUCH_SPIN
	s32 safe_timer;	//!< タッチ時スピン発動待ちタイマー
	s32 safe_jump_timer; //!< タッチ時ジャンプ疑似入力タイマー
	s32 safe_spin_timer; //!< タッチ時スピン発動タイマー
#endif // GMD_PLY_SAFE_TOUCH_SPIN
#endif // _IPHONE

	// 通信用
//	MTS_PAD_KEY_STATUS	tPad;				//!< 通信時のKeyPushなど作成用	◆現在使っていない
	u32					net_ref_atk_time;		//!< はじき攻撃実行時間 最新時間保存
	GMS_PLAYER_PACKET	player_packet[4];		//!< プレイヤーパケット GMD_GAMEMAIN_CONTEST_WIFI_SEND_INT 数以上設定
	fx32				packet_camera_pos_x;	//!< パケットデータ内カメラ座標保存 X (上下非連動時のみ使用)
	fx32				packet_camera_pos_y;	//!< パケットデータ内カメラ座標保存 Y (上下非連動時のみ使用)
	s16					use_packet_buf_no;		//!< 次使用可能パケットバッファ
	
#if defined(MTD_DEBUG)  // デバッグ版

	u16		debug_key_on;					//!< デバッグキー値
	u16		debug_key_push;
	u16		debug_key_repeat;

	u8		debug_dclick_timer;				//!< Lボタンダブルクリックチェックタイマー
	u8		debug_flag;						//!< デバッグフラグ
	s16		debug_kind;						//!< デバッグ配置種類
	fx32	debug_pos_x;					//!< デバッグ配置座標
	fx32	debug_pos_y;					//!< 
    OBS_OBJECT_WORK* debug_disp_obj;		//!< デバッグ配置表示用
#endif

} GMS_PLAYER_WORK; 

// GMS_PLAYER_WORK::player_flag 
#define GMD_PLF_USER1				( 1 << 0 )	//!< 汎用使用フラグ1
#define GMD_PLF_USER2				( 1 << 1 )	//!< 汎用使用フラグ2
#define GMD_PLF_USER3				( 1 << 2 )	//!< 汎用使用フラグ3
#define GMD_PLF_USER4				( 1 << 3 )	//!< 汎用使用フラグ4
#define GMD_PLF_USER_MASK			(GMD_PLF_USER1|GMD_PLF_USER2|GMD_PLF_USER3|GMD_PLF_USER4)	//!< 汎用使用フラグマスク

#define GMD_PLF_PGM_TURN			( 1 << 4 )	//!< プログラムターン処理中
#define GMD_PLF_NOJUMPMOVE			( 1 << 5 )	//!< ジャンプタイプ移動を行わない
#define GMD_PLF_NOHOMING_SEARCH		( 1 << 6 )	//!< ホーミングサーチOFF
#define GMD_PLF_NOHOMING			( 1 << 7 )	//!< ホーミング実行不可に

#define GMD_PLF_PGM_TURN_RDM		( 1 << 8 )	//!< プログラムターン処理中 (後で反転タイプ)
//#define GMD_PLF_OPT_MDL_ENABLE		( 1 << 9 )	//!< オプションモデル有効
#define GMD_PLF_WALK_SMK_EFCT_OFF	( 1 << 9 )	//!< 歩き煙エフェクトOFF
#define GMD_PLF_DIE					( 1 <<10 )	//!< 死亡中
#define GMD_PLF_COLDIE				( 1 <<11 )	//!< 通常地形の挟まり死亡チェックを行う（普段は行わない）

#define GMD_PLF_NOCOLDIE			( 1 <<12 )	//!< オブジェクト地形の挟まり死亡のチェックをしない（普段は行う）
#define GMD_PLF_NOCAMERA_OFST		( 1 <<13 )	//!< 移動によるカメラ移動演出を行いません
#define GMD_PLF_SUPER_SONIC			( 1 <<14 )	//!< スーパーソニック発動中
#define GMD_PLF_AUTO_RUN			( 1 <<15 )	//!< オートラン状態

//#define GMD_PLF_N_PLY_DMG			( 1 <<16 )	//!< プレイヤーからのダメージくらい (通信パケット送信専用フラグ)
#define GMD_PLF_TATK_RETRY			( 1 <<16 )	//!< タイムアタックリトライ演出中
#define GMD_PLF_PINBALL_SONIC		( 1 <<17 )	//!< ピンボールソニック発動中
#define GMD_PLF_TRUCK_RIDE			( 1 <<18 )	//!< トロッコライド発動中
#define GMD_PLF_NO_ITEMSLOW			( 1 <<19 )	//!< アイテムスロウ無効中

#define GMD_PLF_ACT_GOAL			( 1 <<20 )	//!< 通常ACTゴール処理
#define GMD_PLF_BOSS_GOAL_PRE		( 1 <<21 )	//!< ボスゴール前処理
#define GMD_PLF_NOKEY				( 1 <<22 )	//!< キー取得を行いません（デモなどに使用）
#define GMD_PLF_GAMEOVER			( 1 <<23 )	//!< メインゲーム終了

#define GMD_PLF_GOAL				( 1 <<24 )	//!< ゴール中
#define GMD_PLF_RESET_FALL_PARAM	( 1 <<25 )	//!< ラストで落下パラメータを再設定する
#define GMD_PLF_WATER				( 1 <<26 )	//!< 水中
#define GMD_PLF_NOBRAKE				( 1 <<27 )	//!< このフレーム減速せず

#define GMD_PLF_BARRIER				( 1 <<28 )	//!< バリア
#define GMD_PLF_MAGNET				( 1 <<29 )	//!< 磁石
#define GMD_PLF_BOSS5_DEMO			( 1 <<30 )	//!< ボスFINAL演出処理
//#define GMD_PLF_AFTERIMAGE_UPDATE	( 1 <<31 )	//!< 残像のモーション更新を行います
#define GMD_PLF_PGM_FALL_TURN		( 1 <<31 )	//!< 落下ターン処理中

#define GMD_PLF_PGM_TURN_MASK		(GMD_PLF_PGM_TURN | GMD_PLF_PGM_TURN_RDM | GMD_PLF_PGM_FALL_TURN)

#define GMD_PLF_STATE_INIT_CLEAR_MASK	(GMD_PLF_USER_MASK | \
										GMD_PLF_NOHOMING | \
										GMD_PLF_NOCAMERA_OFST | \
										GMD_PLF_DIE | \
										GMD_PLF_NO_ITEMSLOW | \
										GMD_PLF_NOJUMPMOVE | \
										GMD_PLF_WALK_SMK_EFCT_OFF | \
										GMD_PLF_TATK_RETRY)
							/*GMD_PLF_NO_R_DASH | GMD_PLF_TRICK_SP | GMD_PLF_TRICKGMD_PLF_NITRO_NOSTART | GMD_PLF_NOTTENSION | */

#define GMD_PLF_STATE_GIMMICK_INIT_CLEAR_MASK	(GMD_PLF_USER_MASK | \
										GMD_PLF_NOHOMING | \
										GMD_PLF_NOCAMERA_OFST | \
										GMD_PLF_NO_ITEMSLOW | \
										GMD_PLF_NOJUMPMOVE | \
										GMD_PLF_WALK_SMK_EFCT_OFF)
							/*GMD_PLF_NO_R_DASHGMD_PLF_TRICK_SP | GMD_PLF_TRICK | GMD_PLF_NITRO_NOSTART | */


//#define GMD_PLF_N_ATK				( 1 << 4 )	//!< 現在攻撃中 (通信パケット送信専用フラグ)
//#define GMD_PLF_N_DMG				( 1 << 5 )	//!< ダメージくらい (通信パケット送信専用フラグ)
//#define GMD_PLF_NITRO				( 1 << 7 )	//!< ニトロ使用中
//#define GMD_PLF_BOOST				( 1 << 8 )	//!< ブースト中
//#define GMD_PLF_TRICK_SP			( 1 <<14 )	//!< 派生トリック可能フラグ
//#define GMD_PLF_TRICK				( 1 <<15 )	//!< トリック中
//#define GMD_PLF_R_DASH				( 1 <<16 )	//!< Rダッシュ可能状態
//#define GMD_PLF_R_DASH_HIT			( 1 <<17 )	//!< Rダッシュ使用
//#define GMD_PLF_NITRO_START			( 1 <<17 )	//!< ニトロ開始フラグ
//#define GMD_PLF_MAX_TENSION_USE		( 1 <<18 )	//!< テンションフィーバー時にニトロしましたフラグ
//#define GMD_PLF_NO_R_DASH			( 1 <<19 )	//!< Rダッシュ禁止状態
//#define GMD_PLF_NITRO_NOSTART		( 1 <<20 )	//!< ニトロ使用不可状態
//#define GMD_PLF_NOTTENSION 			( 1 <<22 )	//!< テンションを増減させません
//#define GMD_PLF_SPECIAL				( 1 <<30 ) // スペステ行き
//#define GMD_PLF_N_ATK_REF			( 1 <<30 )	//!< 対プレイヤー攻撃時はじき中



// GMS_PLAYER_WORK::gmk_flag
#define GMD_PLGF_TOUCH				( 1 << 0 )	//!< 自由接地フラグ
#define GMD_PLGF_TOUCH_FLIP			( 1 << 1 )	//!< 自由接地時 移動方向反転
#define GMD_PLGF_GRAIND_HITCHECK	( 1 << 2 )	//!< 今FRAMEのグラインド矩形HITをチェックするためのフラグ
#define GMD_PLGF_BOOST_CHK_ON_GMK	( 1 << 3 )	//!< ギミック中でもブースト終了チェックを行う

#define GMD_PLGF_CAMERA_GMK_X		( 1 << 4 )	//!< ギミックを注視する
#define GMD_PLGF_CAMERA_GMK_Y		( 1 << 5 )	//!< ギミックを注視する
#define GMD_PLGF_CAMERA_GMK_X_FIX	( 1 << 6 )	//!< ギミックを即注視する
#define GMD_PLGF_CAMERA_GMK_Y_FIX	( 1 << 7 )	//!< ギミックを即注視する

#define GMD_PLGF_BELT				( 1 << 8 )	//!< ベルトアクション時フラグ
#define GMD_PLGF_GMK_MAP_LIMIT_LCD_X ( 1 << 9 )	//!< 表示画面が壁端設定
#define GMD_PLGF_GMK_MAP_LIMIT_LCD_Y ( 1 <<10 )	//!< 表示画面が壁端設定（下画面も壁）
//#define GMD_PLGF_DH_BOARD			( 1 <<11 )	//!< 滑空ボード使用中フラグ
#define GMD_PLGF_TOUCH_FORCE_DIR	( 1 <<11 )	//!< 接地方向強制（大砲/スプリングカタパルト用）

#define GMD_PLGF_GMK_WALL			( 1 <<12 )	//!< ギミック奥、手前床張り付き
#define GMD_PLGF_GMK_NEON_A			( 1 <<13 )	//!< ネオン足場用、手前移動フラグ
#define GMD_PLGF_GMK_AIR_JUMP		( 1 <<14 )	//!< 空中状態でも通常シーケンスジャンプを可能にする
#define GMD_PLGF_GMK_EXMTX_R		( 1 <<15 )	//!< 演出用 ex_obj_mtx_r 使用

#define GMD_PLGF_GMK_S_PIPE			( 1 <<16 )	//!< Ｓ字パイプの影響を受けている
#define GMD_PLGF_GMK_NO_MAXDASH		( 1 <<17 )	//!< 強制的に最高速ダッシュ状態をOFFにする(1段階下のアクションになる)
#define GMD_PLGF_GMK_TRUCK_STICK	( 1 <<18 )	//!< トロッコ落下前踏ん張り
#define GMD_PLGF_WATER_ALERT		( 1 <<19 )	//!< 水中警告音呼び出しフラグ

#define GMD_PLGF_GMK_TRUCK_L		( 1 <<20 )	//!< トロッコ左向き状態中
#define GMD_PLGF_GMK_CREATE			( 1 <<21 )	//!< ギミック作成を行う
#define GMD_PLGF_GMK_WARP			( 1 <<22 )	//!< ワープ死亡
#define GMD_PLGF_GMK_B				( 1 <<23 )	//!< 2p用AB面設定

#define GMD_PLGF_GMK_JUMP_PSEUDOFALL_DIR_FIX	( 1 << 24 )	//!< 擬似ジャンプ重力固定中

#define GMD_PLGF_TOUCH_DISP_FLIP	( 1 <<25 )	//!< 自由接地時 描画方向反転
#define GMD_PLGF_CAMERA_CENTER_OFST	( 1 <<26 )	//!< カメラセンターオフセットを設定する(保持ギミックに依存しない)
#define GMD_PLGF_MAP_LIMIT_LCD_X	( 1 <<27 )	//!< 表示画面が壁端設定(保持ギミックに依存しない)

#define GMD_PLGF_MAP_LIMIT_LCD_Y	( 1 <<28 )	//!< 表示画面が壁端設定（下画面も壁）(保持ギミックに依存しない)
#define GMD_PLGF_GMK_HOLD_POS_Z		( 1 <<29 )	//!< Z座標を自動復帰させない
#define GMD_PLGF_GMK_TRUCK_DANGER	( 1 <<30 )	//!< トロッコ危険ぶら下がり中
#define GMD_PLGF_GMK_TRUCK_DANGER_RET	( 1 <<31 )	//!< トロッコ危険回避

#define GMD_PLGF_STATE_GIMMICK_INIT_CLEAR_MASK	(GMD_PLGF_CAMERA_GMK_X | \
										GMD_PLGF_CAMERA_GMK_Y | \
										GMD_PLGF_CAMERA_GMK_X_FIX | \
										GMD_PLGF_CAMERA_GMK_Y_FIX | \
										GMD_PLGF_CAMERA_CENTER_OFST | \
										GMD_PLGF_GMK_AIR_JUMP | \
										GMD_PLGF_GMK_EXMTX_R | \
										GMD_PLGF_GMK_S_PIPE | \
										GMD_PLGF_GMK_NO_MAXDASH)		//!< ギミックステート初期化時クリアフラグ
									//GMD_PLGF_GMK_TABLE_COL | GMD_PLGF_GMK_NONITROEFFECT*/)

// GMS_PLAYER_WORK::gmk_flag2
#define GMD_PLGF2_TRUCK_SLOPEFLY_DEC	( 1 << 0 )	//!< トロッコ坂道飛び出し時減速
#define GMD_PLGF2_BARRIER_DISP_OFF		( 1 << 1 )	//!< バリアエフェクト表示OFF
#define GMD_PLGF2_INVINCIBLE_DISP_OFF	( 1 << 2 )	//!< 無敵エフェクト表示OFF
#define GMD_PLGF2_TRUCK_R_AREA			( 1 << 3 )	//!< R地形エリアにいる

#define GMD_PLGF2_TRUCK_R_AREA_PREV		( 1 << 4 )	//!< 前フレームでR地形エリアにいた
#define GMD_PLGF2_TRUCK_CAM_ROT_SLOW	( 1 << 5 )	//!< カメラ回転速度をゆっくりに
#define GMD_PLGF2_TRUCK_FALL_DEATH		( 1 << 6 )	//!< トロッコ落下死亡状態
#define GMD_PLGF2_SUPEREFCT_DISP_OFF	( 1 << 7 )	//!< スーパーソニックエフェクト表示OFF

#if GMD_PLY_USE_CAM_ROT_TRUCK_JUMP
#define GMD_PLGF2_TRUCK_JUMP_NO_CAM_ROT	( 1 << 8 )	//!< ジャンプ中にカメラ回転しない(接地で解除)
#endif
#if _IPHONE
#define GMD_PLGF2_TRUCK_JUMP_MOVE_ROT	( 1 << 9)	//!< ジャンプ中に回転方向へ超加速(接地で解除)
#endif // _IPHONE


#define GMD_PLGF2_STATE_GIMMICK_INIT_CLEAR_MASK		(GMD_PLGF2_BARRIER_DISP_OFF | \
											GMD_PLGF2_INVINCIBLE_DISP_OFF | \
											GMD_PLGF2_SUPEREFCT_DISP_OFF | \
											GMD_PLGF2_TRUCK_FALL_DEATH)		//!< ギミックステート初期化時クリアフラグ

// GMS_PLAYER_WORK::ucGraindPrevRide
#define GMD_PLG_GRAIND_OUTCHECK		( 1 << 1 )	//!< グラインド矩形から出たチェックするためのフラグ
// GMS_PLAYER_WORK::ucGraindID
#define GMD_PLG_GRAIND_ID_MASK		( 0x003F )	//!< ID部マスク
#define GMD_PLG_GRAIND_RIDE			( 1 << 7 )	//!< グラインドに乗りましたフラグ

// GMS_PLAYER_WORK::ucDebugFlag 
#define GMD_PLFD_SETMODE			( 1 << 0 )	//!< 配置モードフラグ
#define GMD_PLFD_MOVEMODE			( 1 << 1 )	//!< 移動モードフラグ

#define GMD_PLFD_SET_TYPE_SHIFT		( 2 )		//!< 配置モードシフト量
#define GMD_PLFD_SET_TYPE_MASK		( 0x03<<2 )	//!< 配置モードマスク
#define GMD_PLFD_ENESET				( 0 << 2 )	//!< 配置モード時、敵セット中
#define GMD_PLFD_DECOSET			( 1 << 2 )	//!< 配置モード時、装飾セット中
#define GMD_PLFD_GMKSET				( 2 << 2 )	//!< 配置モード時、ギミックセット中
#define GMD_PLFD_ITEMSET			( 3 << 2 )	//!< 配置モード時、アイテムセット中



/// アクションリセット用ワーク
typedef struct tag_GMS_PLAYER_RESET_ACT_WORK {
	float	frame[2];
	float	blend_spd;
	float	marge;
	u32		obj_3d_flag;
} GMS_PLAYER_RESET_ACT_WORK;


#if 0
// gmPlySpec.h へ移動
// ==========================================================================
// 各種プレイヤースペック
// ==========================================================================
// スピンダッシュ
#define GMD_PL_SPIN_SPDDA_SHIFT		( 5 )		//!< Spin加速、減速シフト値
#define GMD_PL_SPINDASH_SPD			(0x08000)	//!< Spin加速ダッシュ速度値
#define GMD_PL_SPINDASH_MUL			(0x00800)	//!< Spin加速ダッシュ掛け値
#define GMD_PL_SPINDASH_NOSPD_TIME	(72)		//!< Spinダッシュ時の減速無し時間
#define GMD_PL_SPINDASH_JUMP_NOSPD_TIME	(20)	//!< Spinダッシュジャンプ時の減速無し時間

#define GMD_PL_STOP_SPD				(0x00800)	//!< Spin時などの停止判定を行う値

// プレイヤー速度⇔アクション 対応速度定義
#define GMD_PL_1ST_SPD				(0x1400)	//!< Normal Slow (walk)  0x001 ～ 0x140
#define GMD_PL_2ND_SPD				(0x2800)	//!< Normal Low  (run)   0x141 ～ 0x280
#define GMD_PL_3RD_SPD				(0x4000)	//!< Normal Mid  (dash1) 0x281 ～ 0x400
#define GMD_PL_4TH_SPD				(0x7000)	//!< Normal Max  (dash2) 0x401 ～ 0x700
#define GMD_PL_5TH_SPD				(0x9000)	//!< Boost1              0x701 ～ 0x900
#define GMD_PL_MAX_SPD				(0xa000)	//!< Boost2              0x901 ～ 0xa00

// ホバー速度
#define GMD_PL_HOVER_SPDAD			(0x00300)
#define GMD_PL_HOVER_SPDDO			(0x00600)
#define GMD_PL_HOVER_N_SPDAD		(0x00380)
#define GMD_PL_HOVER_N_SPDDO		(0x00600)
#define GMD_PL_FALL_H_SPDAD			(0x00080)	//!< ホバー落下
#define GMD_PL_FALL_H_SPDMA			(0x02200) 


// 落ちるかどうかの分岐点
#define GMD_PL_FALL_SPD				(0x1e00)

// 傾斜判定角度
#define GMD_PL_KEI_DIR				(0x2000)//(0x20 - 4) // 坂道判定角度 45°
#define GMD_PL_KEI_DIR_SPIN			(0x1000)//(0x10 - 2) // 坂道判定角度 22.5°

// ダメージ受けたときの移動量(水中 X,Y ともに 1/2)
#define GMD_PL_DAMAGE_JUMP_X		((0x02000*3/4))
#define GMD_PL_DAMAGE_JUMP_Y		(-(0x04000*3/4))
#define GMD_PL_DAMAGE_CHECK_SPD		(0x01800) // この値以下の速度の場合、ダメージによる移動方向は向き依存

//これ以上の速度から逆レバー入力でブレーキアクション
//#define GMD_PL_BRAKE_PERMIT_SPD		(0x02000)
#define GMD_PL_BRAKE_PERMIT_SPD		(0x04000)

// グラインド
#define GMD_PL_GRAIND_SPDMI			(0x02000)		// 最低速度
#define GMD_PL_GRAIND_KEI_SPD		(0x00280)
#define GMD_PL_GRAIND_KEY_SPD		(0x00100)

// カメラずらし
#define GMD_PL_CAMERA_OFST_MAX		(0x58000)		// 最大ずらし値
#define GMD_PL_CAMERA_OFST_MINI_X	(16 << (8 + 4)/*FX32_SHIFT?*/)		// この値以下のずらしを無視する
#define GMD_PL_CAMERA_OFST_MINI_Y	(40 << (8 + 4))
#define GMD_PL_CAMERA_OFST_Y1_MAX	(0x16000)		// 最大ずらし値
#define GMD_PL_CAMERA_OFST_Y2_MAX	(-0x48000)		// 最大ずらし値

// ブレイズホバー時間
#define GMD_PL_BLAZE_HOVER_TIME (120 )

// ブーストに達するための時間(超過速度蓄積量
#define GMD_BOOST_POOL_TIME ( 96 )

// 放置時の加減速
#define GMD_PL_ADD_SPD				(0x00080)	// ◆未使用
#define GMD_PL_RET_SPD				(0x00600)	// ◆未使用
#define GMD_PL_STOP_SPD				(0x00800) // Spin時などの停止判定を行う値


// 自力で得れる最高速度
#define GMD_PL_USUAL_TOP_SPD		(0x07800) // 通常時	// ◆未使用
#define GMD_PL_BOOST_TOP_SPD		(0x0a000) // ブースト時	// ◆未使用

// 坂を利用した最高速度
#define GMD_PL_USUAL_MAX_SPD		(0x09800) // 通常時	// ◆未使用
#define GMD_PL_BOOST_MAX_SPD		(0x0c000) // ブースト時	// ◆未使用

// 水しぶき
#define GMD_PL_SPRASH_TIMER (10)

// Super Sonic
#define GMD_PL_JUMP_SPSONIC			(0x08000*3/4)	// ◆未使用

#endif
// ==========================================================================
// ゴースト
// ==========================================================================
// ゴースト
typedef struct _GMS_GHOST_DATA
{
    s8 sX;      // 移動差分値
    s8 sY;      // 
    s8 sZ:4;    //
    u8 sDir:4;  // 角度保持
    u8 ucState; // 表示状態

} GMS_GHOST_DATA;
// ゴースト記録量
#define GMD_GHOST_TIME ( 210 * 15 ) // 記録可能時間 3分半


//----- Macros --------------------------------------------------------------

//----- Macros Functions ----------------------------------------------------
// ================================================================
// GMD_PLAYER_WATER_SET
/*!
  水中速度計算マクロ

  @param pWork [in] プレイヤーワークポインタ
    
 */
// ================================================================
#if 1
#define GMD_PLAYER_WATER_SET( fSpd ) \
{\
	{\
		fSpd = (fx32)(((fSpd) >> 1) );\
	}\
}
#define GMD_PLAYER_WATERJUMP_SET( fSpd ) \
{\
	{\
		fSpd = (fx32)(((fSpd) >> 1) + ((fSpd) >> 2));\
	}\
}

#define GMD_PLAYER_WATER_GET(fSpd)		((fx32)((fSpd) >> 1))
#define GMD_PLAYER_WATERJUMP_GET(fSpd)	((fx32)(((fSpd) >> 1) + ((fSpd) >> 2)))

#else
#define GMD_PLAYER_WATER_SET( fSpd ) \
{\
	if (!GmMainIsBossStage()) {\
		fSpd = (fx32)((fSpd >> 1) );\
	}\
}
#define GMD_PLAYER_WATERJUMP_SET( fSpd ) \
{\
	if (!GmMainIsBossStage()) {\
		fSpd = (fx32)((fSpd >> 1) + (fSpd >> 2));\
	}\
}
#endif


//----- External Variables --------------------------------------------------
//extern void* g_gm_player_archive;
//extern OBS_DATA_WORK g_gm_player_mod[2];
//extern OBS_DATA_WORK g_gm_player_ica; // 各キャラクター共用
//extern OBS_DATA_WORK g_gm_player_iva; // 各キャラクター共用
//extern OBS_DATA_WORK g_gm_player_bac; // 各キャラクター共用
//extern OBS_DATA_WORK g_gm_player_opt_ica; // 各キャラクター共用 追加モーションデータ

/// プレイヤーデータ格納用データワーク
extern OBS_DATA_WORK	g_gm_player_data_work[GSD_MAIN_PLAYER_MAX][GMD_PLAYER_DATA_MAX];

//extern GMS_PLAYER_WORK* _nl_player[8];
// >> g_gm_main_ply_obj_list[GSD_PLAYER_MAX] in gmMain
//extern u8 _nl_player_num;

/* デバッグ */
#if defined (MTD_DEBUG)
/// シーケンス名
extern const char *g_gm_player_seq_name_tbl[];
#endif

//----- External Declarations -----------------------------------------------
// ==========================================================================
// データロード
// ==========================================================================
// ==========================================================================
// GmPlayerBuild
/*!
 *	プレイヤーデータ構築
 */
// ==========================================================================
extern void GmPlayerBuild(void);

// ==========================================================================
// GmPlayerFlush
/*!
 *	プレイヤーデータ開放
 */
// ==========================================================================
extern void GmPlayerFlush(void);

// ==========================================================================
// GmPlayerBuildCheck
/*!
 *	プレイヤーデータ構築 終了チェック
 */
// ==========================================================================
extern BOOL GmPlayerBuildCheck(void);

// ==========================================================================
// GmPlayerFlushCheck
/*!
 *	プレイヤーデータ 片付け 終了チェック
 */
// ==========================================================================
extern BOOL GmPlayerFlushCheck(void);

// ==========================================================================
// GmPlayerLoad
/*!
 *	プレイヤーデータ読み込み
 *
 *	@param	char_id	[in]	キャラクターID GSE_CHAR_ID
 */
// ==========================================================================
extern void GmPlayerLoad(GSE_CHAR_ID char_id);

// ==========================================================================
// GmPlayerRelease
/*!
 *	プレイヤー解放
 */
// ==========================================================================
extern void GmPlayerRelease(void);

#if 0
// ================================================================
// GmPlayerTextureLoad
/*!
  プレイヤーテクスチャー読み込み
 */
// ================================================================
extern void GmPlayerTextureLoad();

// ================================================================
// GmPlayerTextureRelease
/*!
  プレイヤーテクスチャー解放
 */
// ================================================================
extern void GmPlayerTextureRelease();
#endif

// =====================================================================
// データ初期化
// =====================================================================
// ================================================================
// GmPlayerInit
/*!
 *	プレイヤーオブジェクト初期化関数
 *
 *	@param	char_id		[in]	プレイヤーキャラクタータイプ GSE_CHAR_ID_****
 *	@param	ctrl_id		[in]	コントローラーID
 *	@param	player_id	[in]	プレイヤーID
 *	@param	camera_id	[in]	カメラID
 *
 *	@return	プレイヤーオブジェクトワーク
 */
// ================================================================
extern GMS_PLAYER_WORK* GmPlayerInit(GSE_CHAR_ID char_id, u16 ctrl_id, u16 player_id, u16 camera_id);

// ================================================================
// GmPlayerResetInit
/*!
  プレイヤー状態初期化（死亡時など

  @param pWork   [in] 対象プレイヤーワークポインタ

 */
// ================================================================
extern void GmPlayerResetInit(GMS_PLAYER_WORK  * pWork);

// ================================================================
// GmPlayerSetModel
/*!
 *	プレイヤーオブジェクト モデルセット
 *
 *	@param	ply_work		[in]	プレイヤーワーク
 *
 *	@note
 *		char_idに従い、モデルデータとモーションデータを設定します。
 */
// ================================================================
extern void GmPlayerInitModel(GMS_PLAYER_WORK *ply_work);

// ================================================================
// GmPlayerSetModel
/*!
 *	プレイヤーオブジェクト モデルセット
 *
 *	@param	ply_work		[in]	プレイヤーワーク
 *	@param	model_set		[in]	設定するモデルセット
 */
// ================================================================
extern void GmPlayerSetModel(GMS_PLAYER_WORK *ply_work, GME_PLY_MODEL_SET model_set);

// =====================================================================
// プレイヤーステータス パラメータ設定
// =====================================================================
// ================================================================
// GmPlayerStateInit
/*!
  プレイヤー状態初期化

  @param pWork   [in] 対象プレイヤーワークポインタ

 */
// ================================================================
extern void GmPlayerStateInit(GMS_PLAYER_WORK  * pWork);

// ================================================================
// GmPlayerSetCardinalPoints
/*!
  プレイヤー基点座標設定

  @param pos_x		[in] 基点座標X(fx32)
  @param pos_y		[in] 基点座標Y(fx32)
  @param pos_z		[in] 基点座標Z(fx32)

  @note
    自プレイヤーの基点座標を設定します \n
    ゴースト表示時はゴースト側のプレイヤーオブジェクトの座標も設定します。
 */
// ================================================================
//extern void GmPlayerSetCardinalPoints(fx32 pos_x, fx32 pos_y, fx32 pos_z);

// ================================================================
// GmPlayerStateGimmickInit
/*!
  プレイヤー状態ギミック初期化（ギミックHIT時

  @param ply_work		[in] 対象プレイヤーワークポインタ

  @note
    StateInitの内部でも呼ばれます
 */
// ================================================================
extern void GmPlayerStateGimmickInit(GMS_PLAYER_WORK  *ply_work);

// ================================================================
// GmPlayerSpdParameterSet
/*!
  プレイヤーの速度などの基本パラメーターをセットする

  @param pWork   [in] プレイヤーワークポインタ

  @note
    StateInitの内部でも呼ばれます
 */
// ================================================================
extern void GmPlayerSpdParameterSet(GMS_PLAYER_WORK *pWork);

// ================================================================
// GmPlayerSpdParameterSetWater
/*!
  パラメータ設定 水中関連設定のみ

  @param	ply_work	[in] プレイヤーワークポインタ
  @param	water		[in] TRUE : 水中設定  FALSE : 陸上設定
 */
// ================================================================
extern void GmPlayerSpdParameterSetWater(GMS_PLAYER_WORK *ply_work, BOOL water);

// ================================================================
// GmPlayerSetAtk
/*!
  攻撃設定

  @param ply_work   [in] プレイヤーワークポインタ

 */
// ================================================================
extern void GmPlayerSetAtk(GMS_PLAYER_WORK *ply_work);

// ================================================================
// GmPlayerSetDefInvincible
/*!
  くらい無敵設定

  @param ply_work   [in] プレイヤーワークポインタ

 */
// ================================================================
extern void GmPlayerSetDefInvincible(GMS_PLAYER_WORK *ply_work);

// ================================================================
// GmPlayerSetDefNormal
/*!
  くらい通常設定

  @param ply_work   [in] プレイヤーワークポインタ

 */
// ================================================================
extern void GmPlayerSetDefNormal(GMS_PLAYER_WORK *ply_work);

// ================================================================
// GmPlayerBreathingSet
/*!
  息継ぎステータス設定

  @param ply_work   [in] プレイヤーワークポインタ

  @note
	プレイヤーが息継ぎをした時に窒息状態を元に戻します

 */
// ================================================================
extern void GmPlayerBreathingSet(GMS_PLAYER_WORK *ply_work);

// ================================================================
// GmPlayerSetMarkerPoint
/*!
  中間ポイント設定

  @param ply_work   [in] プレイヤーワークポインタ
  @param pos_x		[in] 再開位置 X
  @param pos_y		[in] 再開位置 Y

 */
// ================================================================
extern void GmPlayerSetMarkerPoint(GMS_PLAYER_WORK *ply_work, fx32 pos_x, fx32 pos_y);

// ================================================================
// GmPlayerSetSuperSonic
/*!
  スーパーソニック設定

  @param ply_work   [in] プレイヤーワークポインタ

 */
// ================================================================
extern void GmPlayerSetSuperSonic(GMS_PLAYER_WORK *ply_work);

// ================================================================
// GmPlayerSetEndSuperSonic
/*!
	スーパーソニック終了設定

	@param ply_work   [in] プレイヤーワークポインタ
 */
// ================================================================
extern void GmPlayerSetEndSuperSonic(GMS_PLAYER_WORK *ply_work);

// ================================================================
// GmPlayerSetSplStgSonic
/*!
	スペステソニック設定

	@param ply_work   [in] プレイヤーワークポインタ
 */
// ================================================================
extern void GmPlayerSetSplStgSonic(GMS_PLAYER_WORK *ply_work);

// ================================================================
// GmPlayerSetPinballSonic
/*!
	ピンボールソニック設定

	@param ply_work   [in] プレイヤーワークポインタ
 */
// ================================================================
extern void GmPlayerSetPinballSonic(GMS_PLAYER_WORK *ply_work);

// ================================================================
// GmPlayerSetEndPinballSonic
/*!
	ピンボールソニック終了設定

	@param ply_work   [in] プレイヤーワークポインタ
 */
// ================================================================
extern void GmPlayerSetEndPinballSonic(GMS_PLAYER_WORK *ply_work);

// ================================================================
// GmPlayerSetTruckRide
/*!
	トロッコライド設定

	@param	ply_work		[in]	プレイヤーワークポインタ
	@param	truck_obj		[in]	トロッコオブジェクト
	@param	field_left		[in]	地面あたり設定左
	@param	field_top		[in]	地面あたり設定上
	@param	field_right		[in]	地面あたり設定右
	@param	field_bottom	[in]	地面あたり設定下
 */
// ================================================================
extern void GmPlayerSetTruckRide(GMS_PLAYER_WORK *ply_work, OBS_OBJECT_WORK *truck_obj, s16 field_left, s16 field_top, s16 field_right, s16 field_bottom);

// ================================================================
// GmPlayerSetEndTruckRide
/*!
	トロッコライド終了設定

	@param ply_work   [in] プレイヤーワークポインタ
 */
// ================================================================
extern void GmPlayerSetEndTruckRide(GMS_PLAYER_WORK *ply_work);

// ================================================================
// GmPlayerSetGoalState
/*!
	ゴール時プレイヤー設定

	@param ply_work   [in] プレイヤーワークポインタ
 */
// ================================================================
extern void GmPlayerSetGoalState(GMS_PLAYER_WORK *ply_work);

// ================================================================
// GmPlayerSetAutoRun
/*!
	ソニック オートラン

	@param	ply_work		[in]	プレイヤーワークポインタ
	@param	scroll_spd_x	[in]	画面スクロール速度
	@param	enable			[in]	TRUE : 有効化  FALSE : 無効化
 */
// ================================================================
extern void GmPlayerSetAutoRun(GMS_PLAYER_WORK *ply_work, fx32 scroll_spd_x, BOOL enable);

// ================================================================
// GmPlayerStateClearTrickCombo
/*!
  プレイヤートリックコンボクリア

  @param ply_work		[in] 対象プレイヤーワークポインタ
 */
// ================================================================
#if 0
#if 0
extern void GmPlayerStateClearTrickCombo(GMS_PLAYER_WORK *ply_work);
#else
static inline void GmPlayerStateClearTrickCombo(GMS_PLAYER_WORK *ply_work)
{
    ply_work->trick_combo = 0;
    ply_work->gmk_flag &= ~GMD_PLGF_GMK_TRICK_COMBO;
}
#endif
#endif

// =====================================================================
// デモ状態
// =====================================================================
// ================================================================
// GmPlayerDemoInit
/*!
  プレイヤーをデモ状態に設定する

  @param ply_work [in] プレイヤーワークポインタ
 */
// ================================================================
extern void GmPlayerDemoInit(GMS_PLAYER_WORK *pWork);

#if 0
// =====================================================================
// 無敵デモ
// =====================================================================
// ================================================================
// GmPlayerMutekiDemoInit
/*!
  プレイヤーを無敵デモ状態に設定する

  @param ply_work [in] プレイヤーワークポインタ
  
  @note
  別の状態に戻す場合は必ずGmPlayerMutekiDemoEnd関数を呼び出してから戻してください。
 */
// ================================================================
extern void GmPlayerMutekiDemoInit(GMS_PLAYER_WORK *ply_work);

// ================================================================
// GmPlayerMutekiDemoEnd
/*!
  無敵デモ状態終了関数

  @param ply_work [in] プレイヤーワークポインタ
 */
// ================================================================
extern void GmPlayerMutekiDemoEnd(GMS_PLAYER_WORK *ply_work);
#endif

// =====================================================================
// プレイヤー リング テンション ストック管理
// =====================================================================
// ================================================================
// GmPlayerRingGet
/*!
  リング増加関数
 
  @param pPlayer  [io] プレイヤーワークポインタ
  @param sRing    [in] 増加リング数

  @note
		ダメージリング取得時の累計リング減算は外で行う事
 */
// ================================================================
extern void GmPlayerRingGet( GMS_PLAYER_WORK *pPlayer, s16 sRing );

// ================================================================
// GmPlayerRingDec
/*!
  リング減少関数
 
  @param ply_work	 [io] プレイヤーワークポインタ
  @param dec_ring    [in] 減少リング数

  @note
		ダメージリングを振りまかずにリングを減らす場合に使用
 */
// ================================================================
extern void GmPlayerRingDec( GMS_PLAYER_WORK *ply_work, s16 dec_ring );

// ================================================================
// GmPlayerStockGet
/*!
  プレイヤー人数増加関数
 
  @param pPlayer  [io] プレイヤーワークポインタ
  @param sStock   [in] 増加人数

 */
// ================================================================
extern void GmPlayerStockGet( GMS_PLAYER_WORK *pPlayer, s16 sStock );

// ================================================================
// GmPlayerComboScore
/*!
 *	コンボスコア加算
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 *	@param	score		[in]	加算スコア
 *	@param	pos_x		[in]	表示位置X
 *	@param	pos_y		[in]	表示位置Y
 */
// ================================================================
extern void GmPlayerAddScore(GMS_PLAYER_WORK *ply_work, s32 score, fx32 pos_x, fx32 pos_y);

// ================================================================
// GmPlayerAddScoreNoDisp
/*!
 *	スコア加算 表示無し
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 *	@param	score		[in]	加算スコア
 */
// ================================================================
extern void GmPlayerAddScoreNoDisp(GMS_PLAYER_WORK *ply_work, s32 score);


// ================================================================
// GmPlayerComboScore
/*!
 *	コンボスコア加算
 *	@param	ply_work	[in]	プレイヤーワーク
 *	@param	pos_x		[in]	表示位置X
 *	@param	pos_y		[in]	表示位置Y
 */
// ================================================================
extern void GmPlayerComboScore(GMS_PLAYER_WORK *ply_work, fx32 pos_x, fx32 pos_y);

// =====================================================================
// アイテム取得
// =====================================================================
// =====================================================================
// GmPlayerItemHiSpeedSet
/*!
	アイテム ハイスピード取得
 
	@param ply_work  [io] プレイヤーワークポインタ

 */
// =====================================================================
extern void GmPlayerItemHiSpeedSet(GMS_PLAYER_WORK *ply_work);

// ================================================================
// GmPlayerItemInvincibleSet
/*!
	アイテム 無敵取得
 
	@param ply_work  [io] プレイヤーワークポインタ

 */
// ================================================================
extern void GmPlayerItemInvincibleSet(GMS_PLAYER_WORK *ply_work);

// ================================================================
// GmPlayerItemRing10Set
/*!
	アイテム リング10取得
 
	@param ply_work  [io] プレイヤーワークポインタ

 */
// ================================================================
extern void GmPlayerItemRing10Set(GMS_PLAYER_WORK *ply_work);

// ================================================================
// GmPlayerItemBarrierSet
/*!
	アイテム バリア取得
 
	@param ply_work  [io] プレイヤーワークポインタ

 */
// ================================================================
extern void GmPlayerItemBarrierSet(GMS_PLAYER_WORK *ply_work);

// ================================================================
// GmPlayerItem1UPSet
/*!
	アイテム 1UP取得
 
	@param ply_work  [io] プレイヤーワークポインタ

 */
// ================================================================
extern void GmPlayerItem1UPSet(GMS_PLAYER_WORK *ply_work);

#if 0
// ================================================================
// GmPlayerTensionGet
/*!
  テンション増加関数
 
  @param pPlayer  [io] プレイヤーワークポインタ
  @param sTension [in] 増加テンション値 1:11:4

 */
// ================================================================
extern void GmPlayerTensionGet( GMS_PLAYER_WORK *pPlayer, s16 sTension );

// ================================================================
// GmPlayerTensionEnemyGet
/*!
  敵を倒してテンション増加関数
 
  @param pPlayer  [io] プレイヤーワークポインタ
  @param sTension [in] 増加テンション値 1:11:4

 */
// ================================================================
extern void GmPlayerTensionEnemyGet( GMS_PLAYER_WORK *pPlayer, s16 sTension );
#endif

// =====================================================================
// アイテム取得
// =====================================================================
#if 0
// ================================================================
// GmPlayerNoEnemySet
/*!
  無敵取得
 
  @param pPlayer  [io] プレイヤーワークポインタ

 */
// ================================================================
extern void GmPlayerNoEnemySet( GMS_PLAYER_WORK *pPlayer );

// ================================================================
// GmPlayerBarrierSet
/*!
  バリア取得
 
  @param pPlayer  [io] プレイヤーワークポインタ

 */
// ================================================================
extern void GmPlayerBarrierSet( GMS_PLAYER_WORK *pPlayer );

// ================================================================
// GmPlayerBarrierMagnetSet
/*!
  磁力バリア取得
 
  @param pPlayer  [io] プレイヤーワークポインタ

 */
// ================================================================
extern void GmPlayerBarrierMagnetSet( GMS_PLAYER_WORK *pPlayer );

// ================================================================
// GmPlayerHyperSpeedTrickSet
/*!
  高速トリック取得
 
  @param pPlayer  [io] プレイヤーワークポインタ

 */
// ================================================================
extern void GmPlayerHyperSpeedTrickSet( GMS_PLAYER_WORK *pPlayer );

// ================================================================
// GmPlayerSlowSet
/*!
  スロウ取得
 
  @param pPlayer  [io] プレイヤーワークポインタ

 */
// ================================================================
extern void GmPlayerSlowSet( GMS_PLAYER_WORK *pPlayer );

// ================================================================
// GmPlayerConfusionSet
/*!
  混乱取得
 
  @param pPlayer  [io] プレイヤーワークポインタ

 */
// ================================================================
extern void GmPlayerConfusionSet( GMS_PLAYER_WORK *pPlayer );

// ================================================================
// GmPlayerTensionDownSet
/*!
  テンションダウン取得
 
  @param pPlayer  [io] プレイヤーワークポインタ

 */
// ================================================================
extern void GmPlayerTensionDownSet( GMS_PLAYER_WORK *pPlayer );

// ================================================================
// GmPlayerWarpSet
/*!
  ワープ取得
 
  @param pPlayer  [io] プレイヤーワークポインタ

 */
// ================================================================
extern void GmPlayerWarpSet( GMS_PLAYER_WORK *pPlayer );
#endif

// =====================================================================
// プレイヤーアクション設定
// =====================================================================
// ================================================================
// GmPlayerActionChange
/*!
  アクションチェンジ

  @param ply_work   [in] プレイヤーワークポインタ
  @param seq_type	[in] 設定する状態インデクス

 */
// ================================================================
extern void GmPlayerActionChange(GMS_PLAYER_WORK *ply_work, GME_PLY_ACT_STATE act_state);

// ================================================================
// GmPlayerSaveResetAction
/*!
  アクション再設定用情報保存

  @param ply_work		[in] プレイヤーワークポインタ
  @param act_reset_work	[in] アクションリセット用情報ワーク

  @note
	アクション再設定用情報を保存します。

 */
// ================================================================
extern void GmPlayerSaveResetAction(GMS_PLAYER_WORK *ply_work, GMS_PLAYER_RESET_ACT_WORK *act_reset_work);

// ================================================================
// GmPlayerResetAction
/*!
  アクション再設定

  @param ply_work   [in] プレイヤーワークポインタ
  @param act_reset_work	[in] アクションリセット用情報ワーク

  @note
	現在の設定でアクションを再設定します。

 */
// ================================================================
extern void GmPlayerResetAction(GMS_PLAYER_WORK *ply_work, GMS_PLAYER_RESET_ACT_WORK *act_reset_work);

// ================================================================
// GmPlayerSetActionFrame
/*!
  プレイヤーアクションフレーム設定

	@param	ply_work	[in]	プレイヤーワークポインタ
	@param	frame		[in]	設定フレーム

	@note
		frame < 0 で最終フレームに設定
 */
// ================================================================
// extern void GmPlayerSetActionFrame(GMS_PLAYER_WORK *ply_work, fx32 frame);

// Walk
// ================================================================
// GmPlayerWalkActionSet
/*!
  速度に合わせて歩きアクションを設定
 */
// ================================================================
extern void GmPlayerWalkActionSet( GMS_PLAYER_WORK *pWork );

// ================================================================
// GmPlayerWalkActionCheck
/*!
  速度に合わせて歩きアクションを変化させていく
 */
// ================================================================
extern void GmPlayerWalkActionCheck( GMS_PLAYER_WORK *pWork );

// =====================================================================
// プレイヤー状態 アクションステータス設定
// =====================================================================
// ================================================================
// GmPlayerAnimeSpeedSetWalk
/*!
  アニメーションスピード設定

  @param pWork   [io] プレイヤーポインタ
  @param sSpdSet [in] アニメ速度の基準となる数値
    
 */
// ================================================================
extern void GmPlayerAnimeSpeedSetWalk(GMS_PLAYER_WORK *pWork, fx32 fSpdSet );

// ================================================================
// GmPlayerSpdSet
/*!
  プレイヤーの速度を設定する

  @param pWork  [in] プレイヤーワークポインタ
  @param fSpdX [in] ジャンプ速度X
  @param fSpdY [in] 

  @note
 */
// ================================================================
extern void GmPlayerSpdSet(GMS_PLAYER_WORK *pWork, fx32 fSpdX, fx32 fSpdY);

// ==========================================================================
// GmPlayerSetReverse
/*!
 *	オブジェクト反転
 *
 *	@param	lact_work	[in]	リンクアクション管理ワーク
 *
 *	@note
 *		キーコマンドの再設定も行います
 */
// ==========================================================================
extern void GmPlayerSetReverse(GMS_PLAYER_WORK *ply_work);

// ==========================================================================
// GmPlayerSetReverseOnlyState
/*!
 *	オブジェクト反転 ステータスのみ
 *
 *	@param	lact_work	[in]	リンクアクション管理ワーク
 *
 *	@note
 *		後でアクションが変更されるものとし、ステータスのみ変換します。\n
 *		キーコマンドの再設定も行います
 */
// ==========================================================================
extern void GmPlayerSetReverseOnlyState(GMS_PLAYER_WORK *ply_work);

// ================================================================
// キー入力関係
// ================================================================
// ================================================================
// gmPlayerKeyCheckWalkLeft
/*!
  プレイヤーキーチェック 歩き左
  
  @param    player  [in] 対象プレイヤー
  
  @return   TRUE : 左歩き入力あり
 */
// ================================================================
extern BOOL GmPlayerKeyCheckWalkLeft(GMS_PLAYER_WORK *player);

// ================================================================
// gmPlayerKeyCheckWalkRight
/*!
  プレイヤーキーチェック 歩き右
  
  @param    player  [in] 対象プレイヤー
  
  @return   TRUE : 左歩き入力あり
 */
// ================================================================
extern BOOL GmPlayerKeyCheckWalkRight(GMS_PLAYER_WORK *player);

// ================================================================
// GmPlayerKeyCheckJumpKeyOn
/*!
  プレイヤーキーチェック ジャンプ キーON
  
  @param    player  [in] 対象プレイヤー
  
  @return   TRUE : ジャンプ入力あり
 */
// ================================================================
extern BOOL GmPlayerKeyCheckJumpKeyOn(GMS_PLAYER_WORK *ply_work);

// ================================================================
// GmPlayerKeyCheckJumpKeyPush
/*!
  プレイヤーキーチェック ジャンプ キーPUSH
  
  @param    player  [in] 対象プレイヤー
  
  @return   TRUE : ジャンプ入力あり
 */
// ================================================================
extern BOOL GmPlayerKeyCheckJumpKeyPush(GMS_PLAYER_WORK *ply_work);

// ================================================================
// GmPlayerKeyGetGimmickRotZ
/*!
  プレイヤーキー取得 ギミック用 コントローラー回転量取得
  
  @param    player  [in] 対象プレイヤー
  
  @return   ROT Z
 */
// ================================================================
extern Angle32 GmPlayerKeyGetGimmickRotZ(GMS_PLAYER_WORK *ply_work);

// ================================================================
// GmPlayerKeyCheckTransformKeyPush
/*!
  プレイヤーキーチェック スーパーソニック変身 キーPUSH
  
  @param    player  [in] 対象プレイヤー
  
  @return   TRUE : 変身キー入力あり
 */
// ================================================================
extern BOOL GmPlayerKeyCheckTransformKeyPush(GMS_PLAYER_WORK *ply_work);

// =====================================================================
// 移動ユーティリティ
// =====================================================================
// ================================================================
// GmPlySeqMoveWalk
/*!
  プレイヤー移動設定関数

  @param pWork [in] プレイヤーワークポインタ
    
 */
// ================================================================
extern void GmPlySeqMoveWalk(GMS_PLAYER_WORK *pWork);

// ================================================================
// GmPlySeqMoveJump
/*!
  プレイヤー移動設定関数

  @param pWork [in] プレイヤーワークポインタ
    
 */
// ================================================================
extern void GmPlySeqMoveJump(GMS_PLAYER_WORK *pWork);

// ================================================================
// GmPlySeqMoveSpin
/*!
  プレイヤー移動設定関数

  @param pWork [in] プレイヤーワークポインタ
    
 */
// ================================================================
extern void GmPlySeqMoveSpin(GMS_PLAYER_WORK *pWork);

// ================================================================
// GmPlySeqMoveFly
/*!
  プレイヤー移動設定関数

  @param pWork [in] プレイヤーワークポインタ
    
 */
// ================================================================
extern void GmPlySeqMoveFly(GMS_PLAYER_WORK *pWork);

// ================================================================
// GmPlySeqMoveAuto
/*!
  プレイヤー移動設定関数

  @param pWork [in] プレイヤーワークポインタ
    
 */
// ================================================================
extern void GmPlySeqMoveAuto(GMS_PLAYER_WORK *pWork);

// =====================================================================
// プレイヤーステータス設定
// =====================================================================
#if 0
// ================================================================
// GmPlayerTrickScore
/*!
  トリックスコア取得

  @param ply_work   [in] プレイヤーワークポインタ
  @param ulScore [in] 取得スコア値
 */
// ================================================================
extern void GmPlayerTrickScore(GMS_PLAYER_WORK *ply_work, u32 ulScore);

// ================================================================
// GmPlayerAddSpeedCurse
/*!
  速度ダウン呪い追加

  @param ply_work   [in] プレイヤーワークポインタ
 */
// ================================================================
extern void GmPlayerAddSpeedCurse(GMS_PLAYER_WORK *ply_work);
#endif

// =====================================================================
// HIT処理
// =====================================================================
// ================================================================
// GmPlayerDamageReactionInit
/*!
  ダメージくらい時処理

  @param pDamage [in] 攻撃者矩形拡張ワークポインタ
  @param pMine   [in] 自分矩形拡張ワークポインタ

  @note
		ppDefへ登録
		通常ダメージ処理 \n
		処理が変わる場合は GmPlayerDamageInit も修正する事
 */
// ================================================================
extern void GmPlayerDamageReactionInit(OBS_RECT_WORK* pDamage, OBS_RECT_WORK *pMine);

// ================================================================
// GmPlayerDamageInit
/*!
  プレイヤーダメージくらい状態設定処理

  @param ply_work	[in] プレイヤーワーク

  @note
		直接プレイヤーをダメージ処理へ移行
 */
// ================================================================
extern void GmPlayerDamageInit(GMS_PLAYER_WORK *ply_work);

#if 0
// =====================================================================
// 攻撃オブジェクト作成
// =====================================================================
// ================================================================
// GmAttackObjInit
/*!
  攻撃オブジェクト生成 関数

  @param pPer  [in] 親ポインタ
  @param sL    [in] 矩形左端 1:15
  @param sT    [in]     上端 1:15
  @param sR    [in]     右端 1:15
  @param sB    [in]     下端 1:15
  @param sTime [in] 消去タイマー、0でタイマー消去無し、非0で親死亡消去無し

  @return ワークポインタ

  @note
    攻撃を行うだけの矩形オブジェクト作成します\n
 */
// ================================================================
extern GMS_OBJECT_WORK_ATK* GmAttackObjInit( OBS_OBJECT_WORK *pPer, s16 sL, s16 sT, s16 sR, s16 sB, s16 sTime );
#endif

// =====================================================================
// 重力対応
// =====================================================================
// ================================================================
// GmReverseCheck
/*!
  重力ステージは真中で重力切り替え

  @param fPosX  [in] 座標
  @param fPosY  [in] 座標

  @return 1 逆重力、0 通常重力
 */
// ================================================================
extern u32 GmReverseCheck( fx32 fPosX, fx32 fPosY );

#if 0
// =====================================================================
// ネットワーク対応
// =====================================================================
// ================================================================
// GmPlayerSetCameraByPacket
/*!
  プレイヤーパケットからカメラ座標を設定する

  @param usPlayerNo [in] プレイヤー番号
 */
// ================================================================
extern void GmPlayerSetCameraByPacket( u16 usPlayerNo );
#endif

// ==========================================================================
// ライト設定
// ==========================================================================
// ==========================================================================
// GmPlayerSetDefLight
/*!
 *	プレイヤー標準ライト設定
 *  
 *	@param	ply_work	[in]	プレイヤー
 *
 *	@note
 *		プレイヤーのライトを標準に戻します
 */
// ==========================================================================
inline void GmPlayerSetDefLight(void)
{
	ObjDrawSetParallelLight(NNE_LIGHT_6, &g_gm_main_system.ply_light_col, 1.f, &g_gm_main_system.ply_light_vec);
}

// ==========================================================================
// GmPlayerSetLight
/*!
 *	プレイヤーライト設定
 *  
 *	@param	light_vec	[in]	ライト方向ベクトル
 *	@param	light_col	[in]	ライトカラー
 */
// ==========================================================================
extern void GmPlayerSetLight(NNS_VECTOR *light_vec, NNS_RGBA *light_col);

// ==========================================================================
// GmPlayerSetDefRimParam
/*!
 *	プレイヤー標準リムライトパラメータ設定
 *  
 *	@param	ply_work		[in]	プレイヤーワーク
 */
// ==========================================================================
extern void GmPlayerSetDefRimParam(GMS_PLAYER_WORK *ply_work);

// ==========================================================================
// GmPlayerSetRimParam
/*!
 *	プレイヤーリムライトパラメータ設定
 *  
 *	@param	ply_work		[in]	プレイヤーワーク
 *	@param	toon_rim_param	[in]	リムライトパラメータ
 */
// ==========================================================================
extern void GmPlayerSetRimParam(GMS_PLAYER_WORK *ply_work, NNS_RGB *toon_rim_param);

// ==========================================================================
// Utility
// ==========================================================================
// ==========================================================================
// GmPlayerCheckGimmickEnable
/*!
 *	ギミックオブジェクトが有効かチェック
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 *
 *	@return	TRUE : 有効
 */
// ==========================================================================
extern BOOL GmPlayerCheckGimmickEnable(GMS_PLAYER_WORK *ply_work);

// ==========================================================================
// GmPlayerIsTransformSuperSonic
/*!
 *	スーパーソニックになれるかチェック
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 *
 *	@return	TRUE : 変身できる
 */
// ==========================================================================
extern BOOL GmPlayerIsTransformSuperSonic(GMS_PLAYER_WORK *ply_work);

// ==========================================================================
// GmPlayerCameraOffsetSet
/*!
 *	演出用カメラ中心点オフセットをセット
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 *	@param	ofs_x		[in]	オフセットＸ
 *	@param	ofs_y		[in]	オフセットＹ
 */
// ==========================================================================
extern void GmPlayerCameraOffsetSet(GMS_PLAYER_WORK *ply_work, s16 ofs_x, s16 ofs_y);

#if _IPHONE
// ==========================================================================
// GmPlayerIsStateWait
/*!
 *	現在待機中か否かのチェック
 *
 *	@param	ply_work	[in]	プレイヤーワーク
 *
 *	@return	TRUE : 待機中
 */
// ==========================================================================
extern BOOL GmPlayerIsStateWait(GMS_PLAYER_WORK *ply_work);
#endif // _IPHONE

#if 0
// ================================================================
// GmPlayerSetOfstSubPlayer
/*!
  サブキャラクター(対戦相手・ゴースト等)の表示オフセット設定

  @param ofst_x	[in]	表示オフセットX
  @param ofst_y	[in]	表示オフセットY
  @param ofst_z	[in]	表示オフセットZ
 */
// ================================================================
extern void GmPlayerSetOfstSubPlayer(fx32 ofst_x, fx32 ofst_y, fx32 ofst_z);

// ================================================================
// GmPlayerGetPlayerNum
/*!
  現在生成されているプレイヤー数の取得

 */
// ================================================================
extern u8 GmPlayerGetPlayerNum(void);
#endif

#if	defined(__cplusplus)
} /* extern "C" */
#endif

#endif // GM_PLAYER_H_

//----- Include Files -------------------------------------------------------
