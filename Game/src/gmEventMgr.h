// ================================================================
/*!
  @file gmEventMgr.h
  @brief 

  @author Ishizaki
				Copyright(c) 2008 Dimps

  $Id: gmEventMgr.h 2 2011-04-11 05:21:26Z thamada $
  $Date: 2009-02-23 12:04:31 +0900#$
 */
// ================================================================
/*
 * $Log: gmEventMgr.h,v $
 *
 */

#ifndef GM_EVENT_MGR_H_
#define GM_EVENT_MGR_H_


//----- Include Files -------------------------------------------------------


#if	defined(__cplusplus)
extern "C" {
#endif

//----- Definitions ---------------------------------------------------------

// イベント生成探索関数制御フラグ
// gm_evemgr_search_func_tbl の関数を制御
// GmEveMgrCreate*** 系のフラグが必要な場合に設定
#define GMD_EVE_SEARCH_PROC_FLAG_NONE					(0)			//!< 矩形チェックなし
#define GMD_EVE_SEARCH_PROC_FLAG_RECT_CREATE			(1 << 0)	//!< イベントサイズから生成矩形チェックを行う
#define GMD_EVE_SEARCH_PROC_FLAG_RECT_LCD_OFF			(1 << 1)	//!< LCD範囲内の生成禁止矩形チェックを行う
#define GMD_EVE_SEARCH_PROC_FLAG_ONLY_ENFORCE			(1 << 2)	//!< 強制生成イベントのみ対象
//#define GMD_EVE_SEARCH_PROC_FLAG_RECT_CREATE_NO_REV_L	(1 << 4)	//!< 生成矩形チェック時、補正を行わない 左
//#define GMD_EVE_SEARCH_PROC_FLAG_RECT_CREATE_NO_REV_T	(1 << 5)	//!< 生成矩形チェック時、補正を行わない 上
//#define GMD_EVE_SEARCH_PROC_FLAG_RECT_CREATE_NO_REV_R	(1 << 6)	//!< 生成矩形チェック時、補正を行わない 右
//#define GMD_EVE_SEARCH_PROC_FLAG_RECT_CREATE_NO_REV_B	(1 << 7)	//!< 生成矩形チェック時、補正を行わない 下
//#define GMD_EVE_SEARCH_PROC_FLAG_RECT_CREATE_NO_REV_ALL	(0xF0)		//!< 生成矩形チェック時、補正を行わない 全て



// GMS_EVE_RECORD_EVENT::pos_x
#define GMD_EVE_RECORD_CMD_SKIP			((u8)-1)	//!< イベント スキップコマンド 書き込むことで次のイベント生成チェックから生成されなくなる
													// 通常生成後、移行生成しない場合等に書き込む

// イベント生成範囲 デフォルト値
#define GMD_EVE_EVE_CREATE_SIZE			(256)		//!< 通常イベント
#define GMD_EVE_DEC_CREATE_SIZE			(256)		//!< 装飾物


#define GMD_EVE_BIRTH_WIDTH				(GMD_MAIN_SCR_SPD_MAX*2)	//!< イベント生成幅

// GMS_EVE_RECORD_EVENT:flag
// イベントフラグ
// 上位8ビットはシステム用
// 下位8ビットは各イベントごとの値
#define GMD_EVE_RECORD_EVENT_FLAG_USER_MASK				(0x00FF)	//!< ユーザー使用可能領域フラグマスク
// 以下システム
#define GMD_EVE_RECORD_EVENT_FLAG_SYSFLAG_MASK			(0xFF00)	//!< システム使用領域フラグマスク

//#define GMD_EVE_RECORD_EVENT_FLAG_NET_ACTION			(0x2000)	//!< 通信相手からのアクションあり（敵は死亡など）
//#define GMD_EVE_RECORD_EVENT_FLAG_NET_ACTION_SHIFT		(13)



#define GMD_EVE_RECORD_EVENT_FLAG_NO_CLIP					(0x0800)	//!< クリッピング無し
#define GMD_EVE_RECORD_EVENT_FLAG_NO_CLIP_EASE_SHIFT		(11)

#define GMD_EVE_RECORD_EVENT_FLAG_NO_CREATE_EASE			(0x1000)	//!< EASYモードの時生成しない
#define GMD_EVE_RECORD_EVENT_FLAG_NO_CREATE_EASE_SHIFT		(12)
#define GMD_EVE_RECORD_EVENT_FLAG_NO_CREATE_NORMAL			(0x2000)	//!< NORAMLモードの時生成しない
#define GMD_EVE_RECORD_EVENT_FLAG_NO_CREATE_NORMAL_SHIFT	(13)
#define GMD_EVE_RECORD_EVENT_FLAG_NO_CREATE_HARD			(0x4000)	//!< HARDモードの時生成しない
#define GMD_EVE_RECORD_EVENT_FLAG_NO_CREATE_HARD_SHIFT		(14)

#define GMD_EVE_RECORD_EVENT_FLAG_CREATE_ENFORCE			(0x8000)	//!< イベントを強制生成
#define GMD_EVE_RECORD_EVENT_FLAG_CREATE_ENFORCE_SHIFT		(15)

//#define GMD_EVE_RECORD_EVENT_FLAG_CREATE_A			(0x8000)    ///< イベントをＡ面に配置
//#define GMD_EVE_RECORD_EVENT_FLAG_CREATE_A_SHIFT		(15)


/// イベントレコード(*.ev)
typedef struct tag_GMS_EVE_RECORD_EVENT {
	u8		pos_x;		//!< X座標 or スキップコマンド
	u8		pos_y;		//!< Y座標
	u16		id;			//!< ID
	u16		flag;		//!< フラグ
	s8		left;		//!< 矩形 左
	s8		top;		//!< 矩形 上
	u8		width;		//!< 矩形 幅
	u8		height;		//!< 矩形 高さ
	union {				// ユーザ使用パラメータ(2バイト)自由に使用することができます
		struct {
			u8	byte_param[2];
        };
		u16		word_param;
	};
} GMS_EVE_RECORD_EVENT;
// byte_param[1] : ダメージ関連ステータス
// 敵 : ライフ値が設定されている場合のダメージ量記憶
// ギミック : 破壊状況等


/// 装飾物レコード(*.dc)
typedef struct tag_GMS_EVE_RECORD_DECORATE {
	u8		pos_x;		//!< X座標 or スキップコマンド
	u8		pos_y;		//!< Y座標
	u16		id;			//!< ID
} GMS_EVE_RECORD_DECORATE;

/// リングレコード(*.rg)
typedef struct tag_GMS_EVE_RECORD_RING {
	u8		pos_x;		// X座標 or スキップコマンド
	u8		pos_y;		// Y座標
} GMS_EVE_RECORD_RING;

/// イベントサーチ用ワーク
typedef struct tag_GMS_EVE_SEARCH_WORK {
	s32						block_no;
	GMS_EVE_RECORD_EVENT	*eve_rec_top;
	s32						eve_no;
} GMS_EVE_SEARCH_WORK;

//----- Macros --------------------------------------------------------------

//----- Macros Functions ----------------------------------------------------

//----- External Variables --------------------------------------------------

//----- External Declarations -----------------------------------------------
// ==========================================================================
// 初期化 終了処理
// ==========================================================================
// ==========================================================================
// GmEventMgrInit
/*!
 *	イベントマネージャ初期化
 *
 *	@note
 *		イベントデータ読み込み後に呼び出します
 */
// ==========================================================================
extern void GmEventMgrInit(void);

// ==========================================================================
// GmEventMgrStart
/*!
 *	イベントマネージャ 処理開始
 *
 *	@note
 *		GmEventMgrInit 実行後から呼び出せます。\n
 *		強制生成イベントと、毎フレーム実行用の関数設定などを行います。\n
 *		現在のカメラ位置のイベント生成は、カメラ設定後に行って下さい。
 */
// ==========================================================================
extern void GmEventMgrStart(void);

// ==========================================================================
// GmEventMgrExit
/*!
 *	イベントマネージャ終了処理
 */
// ==========================================================================
extern void GmEventMgrExit(void);

// ==========================================================================
// GmEventDataBuild
/*!
 *	イベントデータ 構築
 */
// ==========================================================================
extern void GmEventDataBuild(void);

// ==========================================================================
// GmEventDataFlush
/*!
 *	イベントデータ 開放
 */
// ==========================================================================
extern void GmEventDataFlush(void);

// ==========================================================================
// イベント生成
// ==========================================================================
// ==========================================================================
// gmEveMgrMain
/*!
 *	イベント生成 強制生成イベント生成
 *
 *	@note
 *		スタートギミック等も含む \n
 *		プレイヤーオブジェクト生成前に実行
 */
// ==========================================================================
extern void GmEventMgrCreateEventEnforce(void);

// ==========================================================================
// GmEventMgrCreateEventInRect
/*!
 *	イベント生成 指定矩形イベント生成
 *
 *	@param	left	[in]	生成矩形左
 *	@param	top		[in]	生成矩形上
 *	@param	right	[in]	生成矩形右
 *	@param	bottom	[in]	生成矩形下
 *
 *	@note
 *		指定矩形内の未生成イベントを生成
 */
// ==========================================================================
extern void GmEventMgrCreateEventInRect(u16 left, u16 top, u16 right, u16 bottom);

// ==========================================================================
// GmEveMgrCreateEventAll
/*!
 *	イベント生成 マップ全部
 */
// ==========================================================================
extern void GmEveMgrCreateEventAll(void);

// ==========================================================================
// GmEveMgrCreateStateEvent
/*!
 *	イベント管理 初期位置イベント生成
 *
 *	@note
 *		カメラのある位置のイベントについて画面内全生成を行います。
 *		カメラ位置が正しい位置に設定されている必要があります。
 */
// ==========================================================================
extern void GmEveMgrCreateStateEvent(void);

// ==========================================================================
// GmEveMgrCreateEventLcd
/*!
 *	イベント生成 1画面分
 *
 *	@param	flag	[in]	動作指定フラグ
 *	@param	ge		[in]	イベント生成を行う画面
 *
 *	@note
 *		2画面独立時専用で2画面間の隙間を考慮しません
 */
// ==========================================================================
#if _DS
extern void GmEveMgrCreateEventLcd(MTE_LCD_TYPE ge, u32 flag);
#else
extern void GmEveMgrCreateEventLcd(u32 flag);
#endif

#if _DS
// ==========================================================================
// GmEveMgrSearchEventLcdDS
/*!
 *	イベント生成 2画面分
 *
 *	@param	flag	[in]	動作指定フラグ
 *
 *	@note
 *		2画面連動時専用で2画面間の隙間も考慮します
 */
// ==========================================================================
extern void GmEveMgrSearchEventLcdDS(u32 flag);
#endif

// ==========================================================================
// ローカルイベントオブジェクト
// ==========================================================================
// ==========================================================================
// GmEventMgrLocalBirth
/*!
 *	ローカルイベントの生成
 *
 *	@param	id		[in]	生成するオブジェクトID (GMS_EVE_RECORD_EVENT:id)
 *	@param	pos_x	[in]	出現座標X
 *	@param	pos_y	[in]	出現座標Y
 *	@param	flag	[in]	GMS_EVE_RECORD_EVENT:flag に設定するフラグ
 *	@param	left	[in]	GMS_EVE_RECORD_EVENT:left に設定するフラグ
 *	@param	top		[in]	GMS_EVE_RECORD_EVENT:top に設定するフラグ
 *	@param	width	[in]	GMS_EVE_RECORD_EVENT:width に設定するフラグ
 *	@param	height	[in]	GMS_EVE_RECORD_EVENT:height に設定するフラグ
 *	@param	type	[in]	イベント生成関数呼び出し時タイプ 通常は0
 *
 *	@return	生成したオブジェクトのワークポインタ
 */
// ==========================================================================
extern OBS_OBJECT_WORK* GmEventMgrLocalEventBirth(u16 id, fx32 pos_x, fx32 pos_y, u16 flag, s8 left, s8 top, u8 width, u8 height, u8 type);

// ==========================================================================
// GmEventMgrLocalRingBirth
/*!
 *	ローカルリングの生成
 *
 *	@param	pos_x	[in]	出現座標X
 *	@param	pos_y	[in]	出現座標Y
 *	@param	type	[in]	イベント生成関数呼び出し時タイプ 通常は0
 *
 *	@return	生成したオブジェクトのワークポインタ
 */
// ==========================================================================
extern void GmEventMgrLocalRingBirth(fx32 pos_x, fx32 pos_y, u8 type);

// ==========================================================================
// GmEventMgrLocalDecoBirth
/*!
 *	ローカルイベント(装飾)の生成
 *
 *	@param	id		[in]	生成するオブジェクトID (GMS_EVE_RECORD_EVENT:id)
 *	@param	pos_x	[in]	出現座標X
 *	@param	pos_y	[in]	出現座標Y
 *	@param	type	[in]	イベント生成関数呼び出し時タイプ 通常は0
 *
 *	@return	生成したオブジェクトのワークポインタ
 */
// ==========================================================================
extern OBS_OBJECT_WORK* GmEventMgrLocalDecoBirth(u16 id, fx32 pos_x, fx32 pos_y, u8 type);

// ==========================================================================
// GmEventMgrLocalEventRelease
/*!
 *	ローカルイベントワーク使用フラグを寝かす　(未使用状態にする)
 *
 *	@param eve_rec	[in] 開放するローカルイベントワーク
 *
 */
// ==========================================================================
extern void GmEventMgrLocalEventRelease(GMS_EVE_RECORD_EVENT *eve_rec);

// ==========================================================================
// GmEventMgrLocalRingRelease
/*!
 *	ローカル装飾ワーク使用フラグを寝かす　(未使用状態にする)
 *
 *	@param eve_rec	[in] 開放するローカルイベントワーク
 *
 */
// ==========================================================================
extern void GmEventMgrLocalRingRelease(GMS_EVE_RECORD_RING *eve_rec);

// ==========================================================================
// GmEventMgrLocalDecoRelease
/*!
 *	ローカル装飾ワーク使用フラグを寝かす　(未使用状態にする)
 *
 *	@param eve_rec	[in] 開放するローカルイベントワーク
 *
 */
// ==========================================================================
extern void GmEventMgrLocalDecoRelease(GMS_EVE_RECORD_DECORATE *eve_rec);

// ==========================================================================
// GmEventMgrSearchEventWorkInit
/*!
 *	イベント検索用ワーク初期化
 *
 *	@param	eve_search_work [io]	GMS_EVE_SEARCH_WORK
 */
// ==========================================================================
extern void GmEventMgrSearchEventWorkInit(GMS_EVE_SEARCH_WORK *eve_search_work);

// ==========================================================================
// GmEventMgrSearchEvent
/*!
 *	イベントを順番に検索する
 *
 *	@return 取得したローカルイベント番号
 */
// ==========================================================================
extern GMS_EVE_RECORD_EVENT* GmEventMgrSearchEvent(GMS_EVE_SEARCH_WORK *eve_search_work);

// ==========================================================================
// Utility
// ==========================================================================
// ==========================================================================
// gmEveMgrMain
/*!
 *	イベントアドレスをオフセットアドレスに変換
 *
 *	@param	eve_addr	[in]	イベントアドレス(GMS_EVE_RECORD_EVENT)
 *
 *	@return	オフセットアドレス
 */
// ==========================================================================
extern u32 GmEventMgrEveAddr2OfstEveAddr(u32 eve_addr);

// ==========================================================================
// GmEventMgrGetRingNum
/*!
 *	ステージ内リング数を取得
 *
 *	@return	ステージ内リング数
 */
// ==========================================================================
extern u32 GmEventMgrGetRingNum(void);
	
/// -------------- mpp -----------------	


#if	defined(__cplusplus)
} /* extern "C" */
#endif

struct mppStorageWriter;
struct mppStorageReader;

void mppEM_StoreOriginMapDataToMemory();
void mppEM_ReserveMapData();
void mppEM_DeleteMapData(void);
void mppEM_PushMapData();
void mppEM_PopMapData(void);
void mppEM_SaveMapData(mppStorageWriter& sw);
bool mppEM_LoadMapData(mppStorageReader& sr);
void mppEM_dbgPrintEventMap(bool minimap);	
void mppEM_ReturnSomeEventsBackToMap();
bool mppEM_ForceRestoreEvent(int eventID);

#endif // GM_EVENT_MGR_H_

//----- Include Files -------------------------------------------------------
