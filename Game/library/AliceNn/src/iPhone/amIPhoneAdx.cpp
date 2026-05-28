/*****************************************************************************/
/*      amIPhoneAdx.cpp                Author : Syuichi Gotou                */
/*            Copyright(c) 2009 Dimps CORP. All Rights Reserved.             */
/*---------------------------------------------------------------------------*/
/* iPhone用ADXライブラリ                                                     */
/*---------------------------------------------------------------------------*/
/* Date          Ver    Comment                                              */
/* 091216-       0.01   first version                                        */
/*****************************************************************************/


//----- Include Files --------------------------------------------------
#include "../alice.h"

#if AMD_USE_CRIADX

//----- Macros ---------------------------------------------------------

//----- Definitions ----------------------------------------------------

//! ファイルキャッシュ使用(Cri File Systemでは使用できない)
#define AMD_USE_ADX_FCACHE		(0)

//! ファイルキャッシュファイル名 (デフォルト)
#define AMD_FCACHE_FNAME		("fcache.lst")

/*
	キャッシング可能なファイル数の制限は以下のとおりです。
	ホスト ：27ファイル
	UMD/DVD： 8ファイル
 */
#define AMD_ADX_DEF_MAXFLEN		(ADXPSP_DEF_MAXFLEN_HOST)
#define AMD_ADX_DEF_FCACHE_SIZE	(ADXPSP_DEF_FCSIZE_HOST)

//----- Local Declarations ---------------------------------------------

static AMS_ADXT*	_amAdxGetHandle(Sint32 handle, Sint32 flag);
static Sint32		_amAdxSearchHandle(void);
static void			_amAdxUseHandle(Sint32 handle);

void 	_amAdx_err_func(void *obj, Char8 *msg);
void 	_amAdx_usr_func(void *obj);

//----- Global Variables -----------------------------------------------

Sint32				_am_adx_handle_max;
Sint32				_am_adx_stream_max;

//----- Local Variables ------------------------------------------------

#if AMD_USE_ADX_FCACHE
static Sint8		_am_adx_fcache_info[AMD_ADX_DEF_FCACHE_SIZE];
#endif

static AMS_ADXT*	_am_adx_buf;
static Sint8*		_am_adx_que;
static Sint8*		_am_adx_ring;
static Sint32		_am_adx_rings;

static float		_am_adx_mvol;		//!< マスターボリューム

static char			_am_adx_root_dir[32];		    //!< ルートディレクトリ

//----- Inline Functions -----------------------------------------------

//! 倍率→デシベル(x10)変換
inline Sint32 _amRate2dB(float rate)
{
	if ( rate <= 0.0001f )
		return -999;
	return (Sint32)(10.0f * 20.0f * log10f( rate ));
}

//! デシベル(x10)→倍率変換
inline float amdB2Rate(Sint32 db)
{
	if ( db <= -999 )
		return	0.0f;
	return (float)powf( 10.0f, ((float)db * 0.1f) * 0.05f );
}

//----- Global Functions -----------------------------------------------

//! amAdxInitSystem
/*!
	ADXシステムの初期化

	@param	pParam	[in]	初期化パラメータへのポインタ

	@note
	起動時に一度だけ呼び出して下さい。
 */
void amAdxInitSystem(AMS_ADX_PARAM* pParam)
{   
	amAssert( pParam );

	amSystemLog( "AkiraMe - ADX System : Initialize ...\n" );

	_am_adx_handle_max	= pParam->handleNum;
	_am_adx_stream_max	= pParam->streamNum;
	_am_adx_mvol		= 1.0f;
	
	// AppDelegate.m で呼ぶので不要
#if (0)
	AdxiPhoneSprmFs sprm;

	memset(&sprm, 0, sizeof(sprm));
	sprm.rtdir=NULL;
	ADXIPHONE_SetupFileSystem(&sprm);
	
	ADXM_SetupFramework(ADXM_FRAMEWORK_DEFAULT, NULL);
	ADXM_SetCbErr(_amAdx_err_func, NULL);
	ADXIPHONE_SetUsrMainFunc(_amAdx_usr_func, NULL);
	
	ADXT_Init();
#endif
	
#if (0)
	// ADXスレッド初期化
	{
		int prio_vsync = 22;
		int prio_fs = 24;
		int prio_mwidle = 50;
		if (pParam->vsync_prio >= 0) {
			prio_vsync = pParam->vsync_prio;
		}
		if (pParam->fs_prio >= 0) {
			prio_fs = pParam->fs_prio;
		}
		if (pParam->idle_prio >= 0) {
			prio_mwidle = pParam->idle_prio;
		}
		ADXM_TPRM tprm;
		amZeroMemory(&tprm, sizeof(ADXM_TPRM));
		tprm.prio_vsync		= prio_vsync;			// Vsync Thread priority
		tprm.prio_fs		= prio_fs;				// Filesystem Thread priority
		tprm.prio_main		= AMD_THREAD_PRIO_MAIN;	// Main Thread Normary priority
		tprm.prio_mwidle	= prio_mwidle;			// Middleware Idle Thread priority
		ADXM_SetupThrd( &tprm );
	}
#endif

	// ADXTハンドルの生成
	if ( AMD_ADX_HANDLE_MAX ) {
		Sint32 size, ch, freq;

		ch		= pParam->channelMax;
		freq	= pParam->frequencyMax;
		size	= ADXT_CALC_WORK( ch, ADXT_PLY_STM, AMD_ADX_STREAM_MAX, freq );

		Sint8* buf = (Sint8*)amMemAllocSystem(
            (sizeof(AMS_ADXT) + sizeof(Sint8*) +
			sizeof(Sint8) + sizeof(Sint8)) * AMD_ADX_HANDLE_MAX );

		_am_adx_buf		= (AMS_ADXT*)buf;	buf += sizeof(AMS_ADXT) * AMD_ADX_HANDLE_MAX;
		_am_adx_que		= (Sint8*)buf;		buf += sizeof(Sint8) * AMD_ADX_HANDLE_MAX;
		_am_adx_ring	= (Sint8*)buf;

		_am_adx_rings	= AMD_ADX_HANDLE_MAX;

		for ( int i = 0; i < AMD_ADX_HANDLE_MAX; i++ ) {
			_am_adx_buf[i].pWork	= (Sint8*)amMemAllocSystem( size );
			_am_adx_buf[i].adxt		= ADXT_Create( ch, _am_adx_buf[i].pWork, size );
			amAssert( _am_adx_buf[i].adxt );
			_am_adx_buf[i].status	= AMD_ADX_STAT_STOP;
			_am_adx_buf[i].vol		= 1.0f;
			_am_adx_buf[i].volDb	= 0;

			_am_adx_que[i]	= i;
			_am_adx_ring[i]	= 1;
		}
	}

	amSystemLog( "Done.\n" );
}

//! amAdxExitSystem
/*!
	ADXシステムの終了

	@note
	終了時に一度だけ呼び出して下さい。
 */
void amAdxExitSystem(void)
{
	amSystemLog( "AkiraMe - ADX System : Exit ... " );

	// ADXTハンドルの開放
	if ( AMD_ADX_HANDLE_MAX ) {
		if ( _am_adx_buf ) {
			for ( int i = 0; i < AMD_ADX_HANDLE_MAX; i++ ) {
				if ( _am_adx_buf[i].adxt > 0 )
					ADXT_Destroy( _am_adx_buf[i].adxt );
				if ( _am_adx_buf[i].pWork )
					amMemFreeSystem( _am_adx_buf[i].pWork );
			}
			amMemFreeSystem( _am_adx_buf );
			_am_adx_buf		= NULL;
			_am_adx_que		= NULL;
			_am_adx_ring	= NULL;
		}
	}

	ADXT_Finish();
	ADXM_ShutdownFramework();
	ADXIPHONE_ShutdownFileSystem();
	amSystemLog( "Done.\n" );
}

//! amAdxExcuteMain
//  外部から呼ぶために用意（ただしメインスレッドから呼ぶこと）
//  通常はAppDelegate.m で呼ぶため不要
void amAdxExcuteMain(void)
{
	ADXM_ExecMain();
}

//! ADX再生 (ストリーム) ファイルパス指定
/*!
	@param	handle		[in]	ADXハンドル (0～)
	@param	fileName	[in]	ADXファイル名
	@param	flag		[in]	制御フラグ (省略可)

	@retval	AMD_ADX_FLAG_NOPLAY	再生しなかった
	@retval	0～					再生するADXハンドル
 */
Sint32 amAdxPlay(Sint32 handle, const char* fileName, Sint32 flag)
{
	char		fname[256];
	AMS_ADXT* pAdxt = _amAdxGetHandle( handle, flag );
	if ( pAdxt == NULL )
		return AMD_ADX_FLAG_NOPLAY;
		
#if AMD_FS_DEVICE_NAME
	AMD_STRCPY_S(fname, 256, _am_fs_device_name);
	AMD_STRCAT_S(fname, 256, fileName);
#endif

	ADXT_StartFname(pAdxt->adxt, (const Char8*)fname );

	_amAdxUseHandle( handle );

	return handle;
}

//! ADX再生 (ストリーム) パーティション&パス指定
/*!
	@param	handle		[in]	ADXハンドル (0～)
	@param	partId		[in]	パーティションID(CPKファイルID)
	@param	fileName	[in]	ADXファイル名(CPKコンテンツファイル名)
	@param	flag		[in]	制御フラグ (省略可)

	@retval	AMD_ADX_FLAG_NOPLAY	再生しなかった
	@retval	0～					再生するADXハンドル
 */
Sint32 amAdxPlayCpk(
	Sint32 handle, Sint32 partId, const char* fileName, Sint32 flag)
{
	AMS_ADXT* pAdxt = _amAdxGetHandle( handle, flag );
	if ( pAdxt == NULL )
		return AMD_ADX_FLAG_NOPLAY;

	ADXT_StartFname(pAdxt->adxt, (const Char8*)fileName );

	_amAdxUseHandle( handle );

	return handle;
}

//! ADX再生 (オンメモリ)
/*!
	@param	handle		[in]	ADXハンドル (0～)
	@param	pBuf		[in]	ADXデータへのポインタ
	@param	flag		[in]	制御フラグ (省略可)

	@retval	AMD_ADX_FLAG_NOPLAY	再生しなかった
	@retval	0～					再生するADXハンドル
 */
Sint32 amAdxPlayMem(Sint32 handle, void* pBuf, Sint32 flag)
{
	amAssert( pBuf );

	AMS_ADXT* pAdxt = _amAdxGetHandle( handle, flag );
	if ( pAdxt == NULL )
		return AMD_ADX_FLAG_NOPLAY;

	ADXT_StartMem(pAdxt->adxt, pBuf );

	_amAdxUseHandle( handle );

	return handle;
}

//! ACX再生 (オンメモリ)
/*!
	@param	handle		[in]	ACXハンドル (0～)
	@param	pBuf		[in]	ACXデータへのポインタ
	@param	id			[in]	再生ID（0～）
	@param	flag		[in]	制御フラグ (省略可)

	@retval	AMD_ADX_FLAG_NOPLAY	再生しなかった
	@retval	0～					再生するACXハンドル
 */
Sint32 amAdxPlayMemIdx(Sint32 handle, void* pBuf, CriSint32 Id, Sint32 flag)
{
	amAssert( pBuf );

	AMS_ADXT* pAdxt = _amAdxGetHandle( handle, flag );
	if ( pAdxt == NULL )
		return AMD_ADX_FLAG_NOPLAY;

	ADXT_StartMemIdx(pAdxt->adxt, pBuf, Id );

	_amAdxUseHandle( handle );

	return handle;
}


//! ADXの停止
/*!
	指定ハンドルのADXを停止します。

	@param	handle	[in]	ADXハンドル (0～)
 */
void amAdxStop(Sint32 handle)
{
	amAssert( handle >= 0 );
	AMS_ADXT* pAdxt = &_am_adx_buf[ handle ];

	ADXT_Stop( pAdxt->adxt );
}

//! ADXの一時停止
/*!
	指定ハンドルのADXを一時停止/再開します。

	@param	handle	[in]	ADXハンドル (0～)
	@param	flag	[in]	制御フラグ (1:一時停止 0:再開)
 */
void amAdxPause(Sint32 handle, Sint32 flag)
{
	amAssert( handle >= 0 );
	AMS_ADXT* pAdxt = &_am_adx_buf[ handle ];

	ADXT_Pause( pAdxt->adxt, flag );
}

//! ADXの停止判定
/*!
	指定ハンドルのADXが停止中か判定します。

	@param	handle	[in]	ADXハンドル (0～)

	@retval	1	停止中
	@retval	0	停止中ではない
 */
Sint32 amAdxIsStop(Sint32 handle)
{
	amAssert( handle >= 0 );
	AMS_ADXT* pAdxt = &_am_adx_buf[ handle ];

	Sint32 stat = ADXT_GetStat( pAdxt->adxt );
	pAdxt->status = stat;
	if ( (stat != ADXT_STAT_STOP) && (stat != ADXT_STAT_PLAYEND) )
		return 0;
	return 1;
}

//! ADXの全停止判定
/*!
	全ハンドルのADXが停止中か判定します。

	@retval	1	停止中
	@retval	0	停止中ではない
 */
Sint32 amAdxIsStopAll(void)
{
	for ( int i = 0; i < AMD_ADX_HANDLE_MAX; i++ )
		if ( !amAdxIsStop( i ) ) return 0;
	return 1;
}

//! ADXのマスターボリューム設定
/*!
	ADXのマスターボリュームを取得します。

	@return マスターボリューム
 */
float amAdxGetMasterVol(void)
{
	return _am_adx_mvol;
}

//! ADXのマスターボリューム設定
/*!
	ADXのマスターボリュームを設定します。

	@param	vol		[in]	ボリューム倍率 (0.0f～)

	@note
	初期状態では1.0fに設定されています。
 */
void amAdxSetMasterVol(float vol)
{
	if ( _am_adx_mvol == vol ) return;
	_am_adx_mvol = vol;

	AMS_ADXT* pAdxt = _am_adx_buf;
	for ( int i = 0; i < AMD_ADX_HANDLE_MAX; i++, pAdxt++ )
		amAdxSetOutVol( i, pAdxt->vol );
}

//! ADXのボリューム取得
/*!
	指定ハンドルのADXボリュームを取得します。

	@param	handle	[in]	ADXハンドル (0～)

	@return	ボリューム倍率
 */
float amAdxGetOutVol(Sint32 handle)
{
	return _am_adx_buf[ handle ].vol;
}

//! ADXのボリューム設定
/*!
	指定ハンドルのADXボリュームを設定します。

	@param	handle	[in]	ADXハンドル (0～)
	@param	vol		[in]	ボリューム倍率 (0.0f～)
 */
void amAdxSetOutVol(Sint32 handle, float vol)
{
	Sint32		db;
	float		rate;
	AMS_ADXT*	pAdxt;

	pAdxt	= &_am_adx_buf[ handle ];
	rate	= _am_adx_mvol * vol;
	db		= _amRate2dB( rate );

	if ( db != pAdxt->volDb ) {
		ADXT_SetOutVol( pAdxt->adxt, db );
		pAdxt->volDb = db;
	}
	pAdxt->vol = vol;
}

Sint32 amAdxGetStatus(Sint32 handle)
{
	AMS_ADXT* pAdxt = &_am_adx_buf[ handle ];
	return ADXT_GetStat( pAdxt->adxt );
}

//----- Local Functions ------------------------------------------------

//! ハンドルの取得
AMS_ADXT* _amAdxGetHandle(Sint32 handle, Sint32 flag)
{
	AMS_ADXT* pAdxt;

	// 空きハンドルの検索
	if ( handle == AMD_ADX_HANDLE_BLANK ) {
		handle = _amAdxSearchHandle();
		if ( handle == -1 ) {
			switch ( flag ) {
			case AMD_ADX_FLAG_NOPLAY:
				return NULL;
			case AMD_ADX_FLAG_FIRST:
				if ( _am_adx_rings == 0 )
					return NULL;
				handle = _am_adx_que[0];
				break;
			default:
				handle = flag;
			}
		}
		pAdxt = &_am_adx_buf[ handle ];
	}
	// ハンドルの状態検査
	else {
		pAdxt = &_am_adx_buf[ handle ];

		Sint32 stat = ADXT_GetStat( pAdxt->adxt );
		pAdxt->status = stat;
		if ( (stat != ADXT_STAT_STOP) && (stat != ADXT_STAT_PLAYEND) ) {
			if ( flag == AMD_ADX_FLAG_NOPLAY )
				return NULL;
		}
	}

	return pAdxt;
}

//! 空きハンドルの検索
Sint32 _amAdxSearchHandle(void)
{
	AMS_ADXT* pAdxt = _am_adx_buf;
	for ( int i = 0; i < AMD_ADX_HANDLE_MAX; i++, pAdxt++ ) {
		if ( !_am_adx_ring[i] )
			continue;

		Sint32 stat = ADXT_GetStat( pAdxt->adxt );
		pAdxt->status = stat;
		if ( (stat == ADXT_STAT_STOP) || (stat == ADXT_STAT_PLAYEND) )
			return i;
	}
	return -1;
}

//! 新規使用ハンドルの登録
void _amAdxUseHandle(Sint32 handle)
{
	if ( _am_adx_ring[ handle ] == 0 )
		return;
	if ( _am_adx_que[ _am_adx_rings-1 ] == handle )
		return;

	Sint32 i = 0;
	while ( _am_adx_que[ i ] != handle )
		i++;

	for ( ; i < _am_adx_rings-1; i++ )
		_am_adx_que[i] = _am_adx_que[ i+1 ];

	_am_adx_que[ _am_adx_rings-1 ] = handle;
}

/* Callback function when an error in ADXT */
// 通常はAppDelegate.m で呼ぶため不要
void _amAdx_err_func(void *obj, Char8 *msg)
{
	amSystemLog( msg );
	amSystemLog( "\n" );
}

/* Callback from ADX UserMain thread */
// 通常はAppDelegate.m で呼ぶため不要
void _amAdx_usr_func(void *obj)
{
	ADXM_ExecMain();
}


#endif // AMD_USE_CRIADX
