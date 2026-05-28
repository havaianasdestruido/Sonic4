// ============================================================================
/*!
	@file	erMovie.mm
	@brief	ムービークラス

	@author	Kouji Hokazono <kouji_hokazono@dimps.co.jp>
		Copyright(c) 2010 Dimps
	$Id$
 */
// ============================================================================
/*
 * $Log$
 */

//------ Include ---------------------- インクルード ---------------------------******_IC*
#include "erMovie.hpp"
#import <MediaPlayer/MediaPlayer.h>


//------ Debug ------------------------ デバッグ -------------------------------******_DG*
#if defined(MTD_DEBUG)
#endif	//#if defined(MTD_DEBUG)
//------ Macro ------------------------ マクロ ---------------------------------******_MC*
//------ C Global Functions ----------- C グローバル関数の定義 -------------------******CGF*
//------ C Global Functions ----------- C グローバル関数の定義 -------------------******CGF*






//------ Class ------------------------ クラス ---------------------------------******_CL*
@interface OcPlayer: NSObject
{
	MPMoviePlayerController	*m_controller;
	bool					m_is_play;
}
-(id)init:(const char *)url;
-(void)play;
-(void)stop;
-(bool)isPlay;
-(void)cbMovieDidFinished:(NSNotification*)notification;

@end
@implementation OcPlayer
//■//-- Local Constant --------------- ローカル定数 ---------------------------******LCS*
//■//-- Public Constant -------------- 公開定数 -------------------------------******PCS*
//■//-- Public Function -------------- 公開関数 -------------------------------******PFC*
// ============================================================================
// OcPlayer::init
/*!
	初期化
	
	@param	url	[in]	URL
 */
// ============================================================================
-(id)init:(const char *)url
{
	self = [super init];

	if(nil != self){
		m_is_play = false;
		NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
		NSString* nss_url = [NSString stringWithCString:url encoding:NSShiftJISStringEncoding];
		NSURL *nsu_url = [NSURL fileURLWithPath:nss_url];
		m_controller  = [[MPMoviePlayerController alloc] initWithContentURL:nsu_url];
	    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(cbMovieDidFinished:) name:MPMoviePlayerPlaybackDidFinishNotification object:m_controller];
		[pool release];
	}

	return self;
}

// ============================================================================
// OcPlayer::dealloc
/*!
	開放
 */
// ============================================================================
-(void)dealloc
{
	[[NSNotificationCenter defaultCenter] removeObserver:self name:MPMoviePlayerPlaybackDidFinishNotification object:m_controller];
	[m_controller release];

	[super dealloc];
}

// ============================================================================
// OcPlayer::play
/*!
	再生
 */
// ============================================================================
-(void)play
{
	m_is_play = true;
	[m_controller play];
}

// ============================================================================
// OcPlayer::stop
/*!
	停止
 */
// ============================================================================
-(void)stop
{
	[m_controller stop];
}


//■//-- Get Function ----------------- 取得関数 -------------------------------******PGF*
// ============================================================================
// OcPlayer::isPlay
/*!
	再生確認

	@retval	true	再生中
	@retval	false	再生中では無い
 */
// ============================================================================
-(bool)isPlay
{
	return m_is_play;
}


//■//-- Set Function ----------------- 設定関数 -------------------------------******PSF*
//■//-- Constructor And Destructor --- コンストラクタ・デストラクタ------------******_CD*
//■//-- Local Function --------------- ローカル関数 ---------------------------******LFC*
// ============================================================================
// OcPlayer::cbMovieDidFinished
/*!
	再生終了コールバック
 */
// ============================================================================
-(void)cbMovieDidFinished:(NSNotification*)notification
{
	m_is_play = false;
}


//------------------------------------------------------------------------------**********
@end









































namespace er {
namespace movie {









//------ Class ------------------------ クラス ---------------------------------******_CL*
//■//-- Local Constant --------------- ローカル定数 ---------------------------******LCS*
CPlayer *CPlayer::p_instance = NULL;		//<共通インスタンス


//■//-- Public Constant -------------- 公開定数 -------------------------------******PCS*
//■//-- Public Function -------------- 公開関数 -------------------------------******PFC*
// ============================================================================
// CPlayer::CreateInstance
/*!
	作成
 */
// ============================================================================
CPlayer &CPlayer::CreateInstance()
{
	if (!p_instance) {
		static CPlayer instance;
		p_instance = &instance;
	}
	return *p_instance;
}

// =============================================================================
// CPlayer::Create
/*!
	構築

	@param	path		[in]	ファイルパス
	@param	base_dir	[in]	ベースディレクトリ(NULL可)

	@retval	true	成功
	@retval	false	失敗

	@note
		失敗しません。
 */
// =============================================================================
bool CPlayer::Create(const char *path, const char *base_dir)
{
	Release();
	
	char file[256] = {};
	if (base_dir) {
		strcpy(file, base_dir);
		strcat(file, path);
		path = file;
	}

	OcPlayer *player = [[OcPlayer alloc] init:path];
	if (nil != player) {
		m_delegate = reinterpret_cast<void *>(player);
		m_flag[BFlag::Setup] = true;
	}

	return true;
}

// =============================================================================
// CPlayer::Release
/*!
	破棄
 */
// =============================================================================
void CPlayer::Release()
{
	if (m_flag[BFlag::Setup]) {
		OcPlayer *player = reinterpret_cast<OcPlayer *>(m_delegate);
		[player release];
		m_delegate = NULL;

		m_flag[BFlag::Setup] = false;
	}
}

// =============================================================================
// CPlayer::Play
/*!
	再生
 */
// =============================================================================
void CPlayer::Play()
{
	if (m_flag[BFlag::Setup]) {
		OcPlayer *player = reinterpret_cast<OcPlayer *>(m_delegate);
		[player play];
	}
}

// =============================================================================
// CPlayer::Stop
/*!
	停止
 */
// =============================================================================
void CPlayer::Stop()
{
 	if (m_flag[BFlag::Setup]) {
		OcPlayer *player = reinterpret_cast<OcPlayer *>(m_delegate);
		[player stop];
	}
}


//■//-- Get Function ----------------- 取得関数 -------------------------------******PGF*
// =============================================================================
// CPlayer::IsValid
/*!
	有効確認

	@retval	true	有効
	@retval	false	無効
 */
// =============================================================================
bool CPlayer::IsValid() const
{
	return m_flag[BFlag::Setup];
}

// =============================================================================
// CPlayer::IsPlay
/*!
	再生確認

	@retval	true	再生中
	@retval	false	再生中では無い
 */
// =============================================================================
bool CPlayer::IsPlay() const
{
	bool result = false;
 	if (m_flag[BFlag::Setup]) {
		OcPlayer *player = reinterpret_cast<OcPlayer *>(m_delegate);
		result = [player isPlay];
	}
	return result;
}


//■//-- Set Function ----------------- 設定関数 -------------------------------******PSF*
//■//-- Constructor And Destructor --- コンストラクタ・デストラクタ------------******_CD*
// ============================================================================
// CPlayer::CPlayer
/*!
	デフォルトコンストラクタ
 */
// ============================================================================
CPlayer::CPlayer()
{
	m_delegate = NULL;
}

// ============================================================================
// CPlayer::~CPlayer
/*!
	デストラクタ
 */
// ============================================================================
CPlayer::~CPlayer()
{
	Release();
	assert(!m_delegate);
}


//■//-- Local Function --------------- ローカル関数 ---------------------------******LFC*
//------------------------------------------------------------------------------**********



















































} //namespace movie
} //namespace er

// =============================================================================
// Function
/*!
	関数の説明

	@param	org1	[io]	引数１の説明
	@param	org2	[in]	引数２の説明
	@param	org3	[out]	引数３の説明

	@return	戻り値の説明
		or
	@retval	0	正常
	@retval	!0	異常

	@exception 例外
 
	@note
		補足説明
 */
// ==========================================================================
