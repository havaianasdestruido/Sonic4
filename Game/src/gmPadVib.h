// ==========================================================================
/*!
  @file gmPadVib.h
  @brief パッド振動

  @author Ishizaki
				Copyright(c) 2009 Dimps

  $Id: gmPadVib.h 2 2011-04-11 05:21:26Z thamada $
  $Date:: 2011-04-11 14:21:26 +0900#$
 */
// ==========================================================================
/*
 * Memo
 *
 *	振動プライオリティについて
 *		プライオリティを設定すると、設定値が大きい振動が優先されます。
 *		プライオリティが同じ場合は上書きされます。
 *		主に、大きい振動のプライオリティを大、小さな振動のプライオリティを小に設定します。
 *		停止タイプ(GME_PAD_VIB_TYPE_STOP) の発行時は優先にかかわらず停止します。
 *		
 */

#ifndef GM_PAD_VIB_H_
#define GM_PAD_VIB_H_


//----- Include Files -------------------------------------------------------

#if	defined(__cplusplus)
extern "C" {
#endif

//----- Definitions ---------------------------------------------------------
typedef enum tag_GME_PAD_VIB_TYPE {
	GME_PAD_VIB_TYPE_STOP	= 0,	//!< 停止
	GME_PAD_VIB_TYPE_NORMAL,		//!< 通常振動
	GME_PAD_VIB_TYPE_DEC,			//!< 減衰
	GME_PAD_VIB_TYPE_ACC,			//!< 増幅
	GME_PAD_VIB_TYPE_INT,			//!< 断続

	GME_PAD_VIB_TYPE_MAX

} GME_PAD_VIB_TYPE;

#define GMD_PAD_VIB_DEF_INT_VIB_TIME	(30*FX32_ONE)	//!< 断続振動 標準振動時間
#define GMD_PAD_VIB_DEF_INT_STOP_TIME	(30*FX32_ONE)	//!< 断続振動 標準停止時間


/* パッド振動設定 */
#if _PS3
// 大
#define GMD_PAD_VIB_LARGE_TYPE			(GME_PAD_VIB_TYPE_NORMAL)
#define GMD_PAD_VIB_LARGE_TIME			(60.f)
#define GMD_PAD_VIB_LARGE_LEFT_VIB		(0x8000)		// PS3 は振動強度/8 の値で設定
#define GMD_PAD_VIB_LARGE_RIGHT_VIB		(0x8000)		// PS3 は振動強度/8 の値で設定
#define GMD_PAD_VIB_LARGE_ADD_DEC_TIME	(0.f)
#define GMD_PAD_VIB_LARGE_INT_VIB_TIME	(0.f)
#define GMD_PAD_VIB_LARGE_INT_STOP_TIME	(0.f)
#define GMD_PAD_VIB_LARGE_PRIO			(0x8000)

// 中
#define GMD_PAD_VIB_MID_TYPE			(GME_PAD_VIB_TYPE_NORMAL)
#define GMD_PAD_VIB_MID_TIME			(30.f)
#define GMD_PAD_VIB_MID_LEFT_VIB		(0x4000)		// PS3 は振動強度/8 の値で設定
#define GMD_PAD_VIB_MID_RIGHT_VIB		(0x4000)		// PS3 は振動強度/8 の値で設定
#define GMD_PAD_VIB_MID_ADD_DEC_TIME	(0.f)
#define GMD_PAD_VIB_MID_INT_VIB_TIME	(0.f)
#define GMD_PAD_VIB_MID_INT_STOP_TIME	(0.f)
#define GMD_PAD_VIB_MID_PRIO			(0x4000)

// 小
#define GMD_PAD_VIB_SMALL_TYPE			(GME_PAD_VIB_TYPE_NORMAL)
#define GMD_PAD_VIB_SMALL_TIME			(20.f)
#define GMD_PAD_VIB_SMALL_LEFT_VIB		(0x2000)		// PS3 は振動強度/8 の値で設定
#define GMD_PAD_VIB_SMALL_RIGHT_VIB		(0x2000)		// PS3 は振動強度/8 の値で設定
#define GMD_PAD_VIB_SMALL_ADD_DEC_TIME	(0.f)
#define GMD_PAD_VIB_SMALL_INT_VIB_TIME	(0.f)
#define GMD_PAD_VIB_SMALL_INT_STOP_TIME	(0.f)
#define GMD_PAD_VIB_SMALL_PRIO			(0x2000)

#elif _WII
// 大
#define GMD_PAD_VIB_LARGE_TYPE			(GME_PAD_VIB_TYPE_NORMAL)
#define GMD_PAD_VIB_LARGE_TIME			(60.f)
#define GMD_PAD_VIB_LARGE_LEFT_VIB		(0x8000)		// WII は振動強度影響なし
#define GMD_PAD_VIB_LARGE_RIGHT_VIB		(0x8000)		// WII は振動強度影響なし
#define GMD_PAD_VIB_LARGE_ADD_DEC_TIME	(0.f)
#define GMD_PAD_VIB_LARGE_INT_VIB_TIME	(0.f)
#define GMD_PAD_VIB_LARGE_INT_STOP_TIME	(0.f)
#define GMD_PAD_VIB_LARGE_PRIO			(0x8000)

// 中
#define GMD_PAD_VIB_MID_TYPE			(GME_PAD_VIB_TYPE_NORMAL)
#define GMD_PAD_VIB_MID_TIME			(10.f)
#define GMD_PAD_VIB_MID_LEFT_VIB		(0x4000)		// WII は振動強度影響なし
#define GMD_PAD_VIB_MID_RIGHT_VIB		(0x4000)		// WII は振動強度影響なし
#define GMD_PAD_VIB_MID_ADD_DEC_TIME	(0.f)
#define GMD_PAD_VIB_MID_INT_VIB_TIME	(0.f)
#define GMD_PAD_VIB_MID_INT_STOP_TIME	(0.f)
#define GMD_PAD_VIB_MID_PRIO			(0x4000)

// 小
#define GMD_PAD_VIB_SMALL_TYPE			(GME_PAD_VIB_TYPE_NORMAL)
#define GMD_PAD_VIB_SMALL_TIME			(5.f)
#define GMD_PAD_VIB_SMALL_LEFT_VIB		(0x2000)		// WII は振動強度影響なし
#define GMD_PAD_VIB_SMALL_RIGHT_VIB		(0x2000)		// WII は振動強度影響なし
#define GMD_PAD_VIB_SMALL_ADD_DEC_TIME	(0.f)
#define GMD_PAD_VIB_SMALL_INT_VIB_TIME	(0.f)
#define GMD_PAD_VIB_SMALL_INT_STOP_TIME	(0.f)
#define GMD_PAD_VIB_SMALL_PRIO			(0x2000)

#else
//#elif _PC || _XBOX
// 大
#define GMD_PAD_VIB_LARGE_TYPE			(GME_PAD_VIB_TYPE_NORMAL)
#define GMD_PAD_VIB_LARGE_TIME			(60.f)
#define GMD_PAD_VIB_LARGE_LEFT_VIB		(0x8000)
#define GMD_PAD_VIB_LARGE_RIGHT_VIB		(0x8000)
#define GMD_PAD_VIB_LARGE_ADD_DEC_TIME	(0.f)
#define GMD_PAD_VIB_LARGE_INT_VIB_TIME	(0.f)
#define GMD_PAD_VIB_LARGE_INT_STOP_TIME	(0.f)
#define GMD_PAD_VIB_LARGE_PRIO			(0x8000)

// 中
#define GMD_PAD_VIB_MID_TYPE			(GME_PAD_VIB_TYPE_NORMAL)
#define GMD_PAD_VIB_MID_TIME			(30.f)
#define GMD_PAD_VIB_MID_LEFT_VIB		(0x4000)
#define GMD_PAD_VIB_MID_RIGHT_VIB		(0x4000)
#define GMD_PAD_VIB_MID_ADD_DEC_TIME	(0.f)
#define GMD_PAD_VIB_MID_INT_VIB_TIME	(0.f)
#define GMD_PAD_VIB_MID_INT_STOP_TIME	(0.f)
#define GMD_PAD_VIB_MID_PRIO			(0x4000)

// 小
#define GMD_PAD_VIB_SMALL_TYPE			(GME_PAD_VIB_TYPE_NORMAL)
#define GMD_PAD_VIB_SMALL_TIME			(30.f)
#define GMD_PAD_VIB_SMALL_LEFT_VIB		(0x2000)
#define GMD_PAD_VIB_SMALL_RIGHT_VIB		(0x2000)
#define GMD_PAD_VIB_SMALL_ADD_DEC_TIME	(0.f)
#define GMD_PAD_VIB_SMALL_INT_VIB_TIME	(0.f)
#define GMD_PAD_VIB_SMALL_INT_STOP_TIME	(0.f)
#define GMD_PAD_VIB_SMALL_PRIO			(0x2000)

#endif

//----- Macros --------------------------------------------------------------

//----- Macros Functions ----------------------------------------------------
// ==========================================================================
// GMM_PAD_VIB_LARGE
/*!
 *	パッド振動 大
 */
// ==========================================================================
#define GMM_PAD_VIB_LARGE()	(GmPadVibSet(GMD_PAD_VIB_LARGE_TYPE, GMD_PAD_VIB_LARGE_TIME, \
								GMD_PAD_VIB_LARGE_LEFT_VIB, GMD_PAD_VIB_LARGE_RIGHT_VIB, \
								GMD_PAD_VIB_LARGE_ADD_DEC_TIME, \
								GMD_PAD_VIB_LARGE_INT_VIB_TIME, GMD_PAD_VIB_LARGE_INT_STOP_TIME, \
								GMD_PAD_VIB_LARGE_PRIO))

// ==========================================================================
// GMM_PAD_VIB_MID
/*!
 *	パッド振動 中
 */
// ==========================================================================
#define GMM_PAD_VIB_MID()	(GmPadVibSet(GMD_PAD_VIB_MID_TYPE, GMD_PAD_VIB_MID_TIME, \
								GMD_PAD_VIB_MID_LEFT_VIB, GMD_PAD_VIB_MID_RIGHT_VIB, \
								GMD_PAD_VIB_MID_ADD_DEC_TIME, \
								GMD_PAD_VIB_MID_INT_VIB_TIME, GMD_PAD_VIB_MID_INT_STOP_TIME, \
								GMD_PAD_VIB_MID_PRIO))

// ==========================================================================
// GMM_PAD_VIB_SMALL
/*!
 *	パッド振動 弱
 */
// ==========================================================================
#define GMM_PAD_VIB_SMALL()	(GmPadVibSet(GMD_PAD_VIB_SMALL_TYPE, GMD_PAD_VIB_SMALL_TIME, \
								GMD_PAD_VIB_SMALL_LEFT_VIB, GMD_PAD_VIB_SMALL_RIGHT_VIB, \
								GMD_PAD_VIB_SMALL_ADD_DEC_TIME, \
								GMD_PAD_VIB_SMALL_INT_VIB_TIME, GMD_PAD_VIB_SMALL_INT_STOP_TIME, \
								GMD_PAD_VIB_SMALL_PRIO))

// ==========================================================================
// GMM_PAD_VIB_LARGE_TIME
/*!
 *	パッド振動 大 時間設定あり
 *
 *	@param	time	[in]	振動時間
 */
// ==========================================================================
#define GMM_PAD_VIB_LARGE_TIME(time)	(GmPadVibSet(GMD_PAD_VIB_LARGE_TYPE, (time), \
								GMD_PAD_VIB_LARGE_LEFT_VIB, GMD_PAD_VIB_LARGE_RIGHT_VIB, \
								GMD_PAD_VIB_LARGE_ADD_DEC_TIME, \
								GMD_PAD_VIB_LARGE_INT_VIB_TIME, GMD_PAD_VIB_LARGE_INT_STOP_TIME, \
								GMD_PAD_VIB_LARGE_PRIO))

// ==========================================================================
// GMM_PAD_VIB_MID_TIME
/*!
 *	パッド振動 中 時間設定あり
 *
 *	@param	time	[in]	振動時間
 */
// ==========================================================================
#define GMM_PAD_VIB_MID_TIME(time)	(GmPadVibSet(GMD_PAD_VIB_MID_TYPE, (time), \
								GMD_PAD_VIB_MID_LEFT_VIB, GMD_PAD_VIB_MID_RIGHT_VIB, \
								GMD_PAD_VIB_MID_ADD_DEC_TIME, \
								GMD_PAD_VIB_MID_INT_VIB_TIME, GMD_PAD_VIB_MID_INT_STOP_TIME, \
								GMD_PAD_VIB_MID_PRIO))

// ==========================================================================
// GMM_PAD_VIB_SMALL_TIME
/*!
 *	パッド振動 弱 時間設定あり
 *
 *	@param	time	[in]	振動時間
 */
// ==========================================================================
#define GMM_PAD_VIB_SMALL_TIME(time)	(GmPadVibSet(GMD_PAD_VIB_SMALL_TYPE, (time), \
								GMD_PAD_VIB_SMALL_LEFT_VIB, GMD_PAD_VIB_SMALL_RIGHT_VIB, \
								GMD_PAD_VIB_SMALL_ADD_DEC_TIME, \
								GMD_PAD_VIB_SMALL_INT_VIB_TIME, GMD_PAD_VIB_SMALL_INT_STOP_TIME, \
								GMD_PAD_VIB_SMALL_PRIO))

// ==========================================================================
// GMM_PAD_VIB_LARGE_NOEND
/*!
 *	パッド振動 大 永久振動
 *
 *	@note
 *		必ず停止してください。
 */
// ==========================================================================
#define GMM_PAD_VIB_LARGE_NOEND()	(GmPadVibSet(GMD_PAD_VIB_LARGE_TYPE, -1, \
								GMD_PAD_VIB_LARGE_LEFT_VIB, GMD_PAD_VIB_LARGE_RIGHT_VIB, \
								GMD_PAD_VIB_LARGE_ADD_DEC_TIME, \
								GMD_PAD_VIB_LARGE_INT_VIB_TIME, GMD_PAD_VIB_LARGE_INT_STOP_TIME, \
								GMD_PAD_VIB_LARGE_PRIO))

// ==========================================================================
// GMM_PAD_VIB_MID_NOEND
/*!
 *	パッド振動 中 永久振動
 *
 *	@note
 *		必ず停止してください。
 */
// ==========================================================================
#define GMM_PAD_VIB_MID_NOEND()	(GmPadVibSet(GMD_PAD_VIB_MID_TYPE, -1, \
								GMD_PAD_VIB_MID_LEFT_VIB, GMD_PAD_VIB_MID_RIGHT_VIB, \
								GMD_PAD_VIB_MID_ADD_DEC_TIME, \
								GMD_PAD_VIB_MID_INT_VIB_TIME, GMD_PAD_VIB_MID_INT_STOP_TIME, \
								GMD_PAD_VIB_MID_PRIO))

// ==========================================================================
// GMM_PAD_VIB_SMALL_NOEND
/*!
 *	パッド振動 弱 永久振動
 *
 *	@note
 *		必ず停止してください。
 */
// ==========================================================================
#define GMM_PAD_VIB_SMALL_NOEND()	(GmPadVibSet(GMD_PAD_VIB_SMALL_TYPE, -1, \
								GMD_PAD_VIB_SMALL_LEFT_VIB, GMD_PAD_VIB_SMALL_RIGHT_VIB, \
								GMD_PAD_VIB_SMALL_ADD_DEC_TIME, \
								GMD_PAD_VIB_SMALL_INT_VIB_TIME, GMD_PAD_VIB_SMALL_INT_STOP_TIME, \
								GMD_PAD_VIB_SMALL_PRIO))

// ==========================================================================
// GMM_PAD_VIB_STOP
/*!
 *	パッド振動 停止
 */
// ==========================================================================
#define GMM_PAD_VIB_STOP()	(GmPadVibSet(GME_PAD_VIB_TYPE_STOP, 0.f, 0, 0, 0.f, 0.f, 0.f))

//----- External Variables --------------------------------------------------

//----- External Declarations -----------------------------------------------
// ==========================================================================
// GmPadVibInit
/*!
 *	パッド振動初期化
 */
// ==========================================================================
extern void GmPadVibInit(void);

// ==========================================================================
// GmPadVibExit
/*!
 *	パッド振動終了処理
 */
// ==========================================================================
extern void GmPadVibExit(void);

// ==========================================================================
// GmPadVibInit
/*!
 *	パッド振動初期化
 *
 *	@param	vib_type		[in]	振動タイプ GME_PAD_VIB_TYPE
 *	@param	time			[in]	振動時間 (フレーム)
 *	@param	left_vib		[in]	左振動度合い
 *	@param	right_vib		[in]	右振動度合い
 *	@param	add_dec_time	[in]	減衰・増幅時間 (フレーム)
 *	@param	int_vib_time	[in]	断続振動 振動時間 (フレーム)
 *	@param	int_stop_time	[in]	断続振動 停止時間 (フレーム)
 *	@param	prio			[in]	振動プライオリティ
 *
 *	@note
 *		time <= -1.f で時間無制限になります。
 */
// ==========================================================================
extern void GmPadVibSet(GME_PAD_VIB_TYPE vib_type, float time, u16 left_vib, u16 right_vib, float add_dec_time, float int_vib_time, float int_stop_time, u32 prio=0);


#if	defined(__cplusplus)
} /* extern "C" */
#endif

#endif // GM_PAD_VIB_H_

//----- Include Files -------------------------------------------------------
