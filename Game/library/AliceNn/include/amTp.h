// ================================================================
/*!
 @file amTp.h
 @brief タッチパネル制御
 
 @author Satoshi Akitomi
 Copyright(c) 2009 Dimps
 $Id: amTp.h 2 2011-04-11 05:21:26Z thamada $
 */
// ================================================================
/*!
 @page amTp タッチパネル制御
 
 タッチパネルから入力されたデータを取得するところまでをサポートするライブラリです。＼n
 タッチパネルの入力をパッドと同様。OnやPushなどで取得することが出来ます。＼n
 入力は multi touchサポート状態で最大5点まで確保可能です。\n
 スワイプ、ピンチイン/アウトなどは取り合えず放置しています。\n
 図形認識などはアプリ側で実装する必要があります。
 
 @sa amTp.h
 */

#ifndef _AM_TP_H
#define _AM_TP_H

#if defined(__cplusplus)
extern "C" {
#endif
	
	//----- Macros ---------------------------------------------------------
	/*!
	 @defgroup AMD_XX
	 @brief 座標情報（仮）
	 */
#define AMD_XY  (2) //!< 
#define AMD_X   (0) //!<
#define AMD_Y   (1) //!< 
	
#define AMD_TP_TOUCH_POS_MAX            (5)  //!< 最大接触点数
	/*!
	 @defgroup AMD_TP_FLAG_XXX_SHIFT
	 @brief タッチパネル状態フラグのシフト値
	 */
	//@{
#define AMD_TP_FLAG_ON_SHIFT            (0)                             ///< 接触中
#define AMD_TP_FLAG_PREV_SHIFT          (1)                             ///< １フレーム前の接触状態
#define AMD_TP_FLAG_PUSH_SHIFT          (2)                             ///< 接触した瞬間
#define AMD_TP_FLAG_PULL_SHIFT          (3)                             ///< 離した瞬間
#define AMD_TP_FLAG_INVALID_SHIFT       (7)                             ///< 無効値
	//@}
	
	/*!
	 @defgroup AMD_TP_FLAG_XXX
	 @brief タッチパネル状態フラグ AMS_TP_TOUCH_CORE::flag
	 */
	//@{
#define AMD_TP_FLAG_ON                  (1 << AMD_TP_FLAG_ON_SHIFT)     ///< 接触中
#define AMD_TP_FLAG_PREV                (1 << AMD_TP_FLAG_PREV_SHIFT)   ///< １フレーム前の接触状態
#define AMD_TP_FLAG_PUSH                (1 << AMD_TP_FLAG_PUSH_SHIFT)   ///< 接触した瞬間
#define AMD_TP_FLAG_PULL                (1 << AMD_TP_FLAG_PULL_SHIFT)   ///< 離した瞬間
#define AMD_TP_FLAG_INVALID             (1 << AMD_TP_FLAG_INVALID_SHIFT) ///< 現フレームで取得した座標値が無効だった（この場合、直前の有効値が保存されます）
	//@}
	
	/*!
	 @defgroup AMD_TP_SAMPLING_FLAG_XXX
	 @brief サンプリング時付加情報 AMS_TP_TOUCH_CORE::sampling_flag
	 
	 この値が AMS_TP_TOUCH_CORE::flag に反映されます。
	 */
	//@{
#define AMD_TP_SAMPLING_FLAG_ON         (1 << 0)                        ///< 接触状態
#define AMD_TP_SAMPLING_FLAG_INVALID    (1 << 7)                        ///< 取得座標値が無効
	//@}
	
	//----- Macros Functions -----------------------------------------------
	
	//----- Definitions ----------------------------------------------------
	/*!
	 タッチパネル状態構造体
	 
	 通信でタッチパネルの入力をやり取りする場合、この情報を送受信することで、
	 タッチパネル状態を再現できます。＼n
	 */
	typedef struct _AMS_TP_TOUCH_CORE
		{
			Uint16                 sampling_buf[AMD_XY];    ///< １フレーム分のサンプリング座標値、古い値から順に格納されています。接触／非接触の瞬間でも必ずサンプリング数分格納されます
			Uint8                  sampling_num;            ///< １フレームあたりのサンプリング数、０なら停止しています
			Uint8                  sampling_flag;           ///< サンプリング時付加情報
			
		} AMS_TP_TOUCH_CORE;
	
	
	/// タッチパネル状態構造体
	typedef struct _AMS_TP_TOUCH_STATUS
		{
			AMS_TP_TOUCH_CORE   core;                                           ///< コア情報
			Uint16                 flag;                                           ///< フラグ
			Uint16                 on[AMD_XY];                                     ///< 現在位置、離れている場合は離れる直前の値を保持、複数回サンプリング時は最新の値が格納されます
			Uint16                 prev[AMD_XY];                                   ///< １フレーム前の位置、離れている場合は離れる直前の値を保持
			Uint16                 push[AMD_XY];                                   ///< 接触した瞬間の位置
			Uint16                 pull[AMD_XY];                                   ///< 離した瞬間の位置
		} AMS_TP_TOUCH_STATUS;
	
	//----- External Declarations ------------------------------------------
	/// @brief タッチパネル状態
	/// @note この値はシステムによって自動で更新されます。
	/*!
	 タッチパネル状態
	 
	 茉理ライブラリが自動で管理しているタッチパネル状態です。＼n
	 タッチパネルのサンプリングを開始しないと、
	 この変数には情報が格納されないので注意してください。＼n
	 通常はこの情報を参照してプログラムします。＼n
	 */
	extern AMS_TP_TOUCH_STATUS   _am_tp_touch[5];
	
	// ================================================================
	// amTpInit
	/*!
	 タッチパネルの初期化関数
	 
	 @note
	 タッチパネルを使用する前に一度だけ呼び出す必要があります。＼n
	 この関数はシステム側で呼ばれるためユーザー側では特に呼び出す必要はありません。
	 */
	// ================================================================
	extern void amTpInit(void);
	
	// ================================================================
	// amTpExecute
	/*!
	 タッチパネル処理
	 
	 @note
	 座標の更新などを行っています。
	 */
	// ================================================================
	extern void amTpExecute(void);
	
	// ================================================================
	// amTpUpdateStatus
	/*!
	 タッチパネル状態を更新する
	 
	 @param core   [in]  タッチパネルのコア情報（status->core と同じ値でも構いません）
	 @param status [out] タッチパネル情報
	 
	 @note
	 引数の AMS_TP_TOUCH_CORE 構造体の値をもとに、＼n
	 push・pull 時のフラグ・座標値が更新されます。＼n
	 リクエスト／オートサンプリングともに有効です。
	 */
	// ================================================================
	extern void amTpUpdateStatus(AMS_TP_TOUCH_STATUS *status, AMS_TP_TOUCH_CORE *core);
	
	// ================================================================
	// amTpIsTouchOn
	/*!
	 接触状態かどうか調べる
	 */
	// ================================================================
	extern inline BOOL amTpIsTouchOn(int index)
	{
		return (_am_tp_touch[index].flag & AMD_TP_FLAG_ON);
	}
	
	// ================================================================
	// amTpIsTouchPush
	/*!
	 接触した瞬間かどうか調べる
	 */
	// ================================================================
	extern inline BOOL amTpIsTouchPush(int index)
	{
		return (_am_tp_touch[index].flag & AMD_TP_FLAG_PUSH);
	}
	
	// ================================================================
	// amTpIsTouchPull
	/*!
	 非接触の瞬間かどうか調べる
 */
// ================================================================
extern inline BOOL amTpIsTouchPull(int index)
{
    return (_am_tp_touch[index].flag & AMD_TP_FLAG_PULL);
}


#if defined(__cplusplus)
} /* extern "C" */
#endif

#endif // _AM_TP_H
