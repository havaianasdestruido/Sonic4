// ============================================================================
/*!
	@file	dmMovie.cpp
	@brief	ムービー

	@author	Kouji Hokazono <kouji_hokazono@dimps.co.jp>
		Copyright(c) 2010 Dimps
	$Id: dmMovie.cpp 2 2011-04-11 05:21:26Z thamada $
 */
// ============================================================================
/*
 * $Log$
 */

//------ Include ---------------------- インクルード ---------------------------******_IC*
#include "pch.h"

#if _IPHONE
extern BOOL _am_sample_is_ignore_audio_interruption; //オーディオ割り込み処理の無視
#include "izFade.h"
#include "gs.h"
#include "gsSound.h"
#include "gsMainSys.h"

#include "erMovie.hpp"
#include "dmMovie.hpp"
#include "erObject.hpp"
#include "erTask.hpp"
#include "accelBitset.hpp"



//------ Debug ------------------------ デバッグ -------------------------------******_DG*
#if defined(MTD_DEBUG)
#endif	//#if defined(MTD_DEBUG)
//------ Macro ------------------------ マクロ ---------------------------------******_MC*






















namespace dm {
namespace movie {


namespace {


//------------------------------------------------------------------------------**********
namespace setting {
	//ファイルパス
	namespace file {
		const char *c_movie	= GSS_BASE_PATH"MOVIE/TRIAL.MP4";	//ムービーファイル
	} //namespace file

	//メイン
	namespace main {
		namespace fadein {
			const IZE_FADE_SET_TYPE	c_type	= IZE_FADE_SET_TYPE_NORMAL;		//<フェード継続タイプ
			const IZE_FADE_TYPE		c_inout	= IZE_FADE_TYPE_BLACK_FADEIN;	//<フェード種類タイプ
			const float				c_frame	= 1.0f;							//<フェードフレーム数
		} //namespace fadein
		namespace fadeout {
			const IZE_FADE_SET_TYPE	c_type	= IZE_FADE_SET_TYPE_NORMAL;		//<フェード継続タイプ
			const IZE_FADE_TYPE		c_inout	= IZE_FADE_TYPE_BLACK_FADEOUT;	//<フェード種類タイプ
			const float				c_frame	= 1.0f;							//<フェードフレーム数
		} //namespace fadeout
	} //namespace main
} //namespace setting
//------------------------------------------------------------------------------**********













































// =============================================================================
// private_cast
/*!
	キャスト

	@note
		特殊化の定義は下の方に別定義
 */
// ==========================================================================
template <typename TTo, typename TFrom>
TTo private_cast(const TFrom &from);	//<定義しない事(定義が無い事に意義がある)
//------------------------------------------------------------------------------**********











} //namespace







































//------ Class ------------------------ クラス ---------------------------------******_CL*
/*!	@class
	@brief	ムービークラス
		ムービーを提供します。
 */
class CMain : public ::er::task::CTask<CMain> {
	//-- Enum Hack -------------------- 定数宣言 -------------------------------******_EH*
protected:
private:
	typedef ::er::task::CTask<CMain>	super_type;


	//-- Public Constant -------------- 公開定数 -------------------------------******PCS*
public:
	//-- Public Variable -------------- 公開変数 -------------------------------******PVA*
	//-- Public Function -------------- 公開関数 -------------------------------******PFC*
	// ============================================================================
	// CMain::CreateInstance
	/*!
		作成
	 */
	// ============================================================================
	static CMain &CreateInstance();

	// ============================================================================
	// CMain::operator()
	/*!
		フレーム毎に呼び出される関数
	 */
	// ============================================================================
	virtual void operator()() {
		preUpdate();
		super_type::operator()();
		if (m_flag[BFlag::Create]) {
			update();
			draw();
		} else {
			delete this;
		}
	}


	//-- Geter Function --------------- 取得関数 -------------------------------******PGF*
	//-- Seter Function --------------- 設定関数 -------------------------------******PSF*
	//-- Constructor And Destructor --- コンストラクタ・デストラクタ------------******_CD*
	// ============================================================================
	// CMain::CMain
	/*!
		デフォルトコンストラクタ
	 */
	// ============================================================================
public:
	CMain();

public:
	// ============================================================================
	// CMain::~CMain
	/*!
		デストラクタ
	 */
	// ============================================================================
	virtual ~CMain();


	//-- Local Constant --------------- ローカル定数 ---------------------------******LCS*
protected:
private:
	struct BFlag {
		enum {
			Create,

			Max,
			None,
		};
		typedef accel::CBitset<Max> Type;
	};


	//-- Local Variable --------------- ローカル変数 ---------------------------******LVA*
protected:
private:
	BFlag::Type			m_flag;		//<フラグ


	//-- Local Function --------------- ローカル関数 ---------------------------******LFC*
protected:
private:
	void preUpdate();
	void update();
	void draw();

	void movieStart();
	void movie();
	void movieEnd();


//------------------------------------------------------------------------------**********
}; //class CMain




//------ Class ------------------------ クラス ---------------------------------******_CL*
//■//-- Local Constant --------------- ローカル定数 ---------------------------******LCS*
//■//-- Public Constant -------------- 公開定数 -------------------------------******PCS*
//■//-- Public Function -------------- 公開関数 -------------------------------******PFC*
// ============================================================================
// CMain::Init
/*!
	作成
 */
// ============================================================================
CMain &CMain::CreateInstance()
{
	//通常
	CMain *main = new("Movie", 0x1000) CMain();
	return *main;
}


//■//-- Get Function ----------------- 取得関数 -------------------------------******PGF*
//■//-- Set Function ----------------- 設定関数 -------------------------------******PSF*
//■//-- Constructor And Destructor --- コンストラクタ・デストラクタ------------******_CD*
// =============================================================================
// CMain::CMain
/*!
	デフォルトコンストラクタ
 */
// ==========================================================================
CMain::CMain()
{
	m_flag[BFlag::Create] = true;
	movieStart();
}

// =============================================================================
// CMain::~CMain
/*!
	デストラクタ
 */
// ==========================================================================
CMain::~CMain()
{
	amAssert(!m_flag[BFlag::Create]);

	SyDecideEvtCase(0);
#if defined(AMD_DEBUG)
	if (GSD_EVT_ID_DEBUG_DEMO == SyGetEvtInfo()->old_evt_id) {
		//デバッグメニューから来たらデバッグメニューに戻る
		SyDecideEvt(GSD_EVT_ID_DEBUG_DEMO);
	}
#endif //defined(AMD_DEBUG)
	SyChangeNextEvt();
}


//■//-- Local Function --------------- ローカル関数 ---------------------------******LFC*
// =============================================================================
// CMain::preUpdate
/*!
	更新
 */
// ==========================================================================
void CMain::preUpdate()
{
}

// =============================================================================
// CMain::update
/*!
	更新
 */
// ==========================================================================
void CMain::update()
{
}

// =============================================================================
// CMain::draw
/*!
	描画
 */
// ==========================================================================
void CMain::draw()
{
}

// =============================================================================
// CMain::movieStart
/*!
	ムービー開始
 */
// ==========================================================================
void CMain::movieStart()
{
	SetProc(&CMain::movie);

	//サウンド終了
	GsSoundExit();
	CriSmpSoundOutput_StopSound();
	_am_sample_is_ignore_audio_interruption = TRUE; //オーディオ割り込みを無視する

	//メディアプレイヤー構築・再生
	er::movie::CPlayer &player = er::movie::CPlayer::CreateInstance();
	player.Create(setting::file::c_movie, _am_fs_device_name);
	player.Play();
}

// =============================================================================
// CMain::movie
/*!
	ムービー
 */
// ==========================================================================
void CMain::movie()
{
	er::movie::CPlayer &player = er::movie::CPlayer::CreateInstance();
	if (!player.IsPlay()) {
		movieEnd();
	}
}

// =============================================================================
// CMain::movieEnd
/*!
	ムービー終了
 */
// ==========================================================================
void CMain::movieEnd()
{
	//メディアプレイヤー破棄
	er::movie::CPlayer &player = er::movie::CPlayer::CreateInstance();
	player.Release();

	//ボリューム待避
	GSS_MAIN_SYS_INFO *gs_main = GsGetMainSysInfo();
	float volume_bgm = gs_main->bgm_volume;
	float volume_se = gs_main->se_volume;

	//サウンド構築
	_am_sample_is_ignore_audio_interruption = FALSE; //オーディオ割り込みを無視しない
	CriSmpSoundOutput_ReStartSound();
	GsSoundInit();
	
	//ボリューム復元
	gs_main->bgm_volume = volume_bgm;
	gs_main->se_volume = volume_se;
	GsSoundSetVolume(GSE_SND_TYPE_BGM, gs_main->bgm_volume);
	GsSoundSetVolume(GSE_SND_TYPE_SE, gs_main->se_volume);

	m_flag[BFlag::Create] = false;
}


//------------------------------------------------------------------------------**********











































namespace {
// =============================================================================
// private_cast
/*!
	キャスト
 */
// ==========================================================================
//------------------------------------------------------------------------------**********
} //namespace












} //namespace movie
} //namespace dm


//------ C Global Functions ----------- C グローバル関数の定義 -------------------******CGF*
// ============================================================================
// DmMovieInit
/*!
	ムービー・初期化

	@param	arg	[in]	引数
 */
// ============================================================================
void DmMovieInit(void *)
{
	if(true) {  //qqq - disable movie
		SyDecideEvtCase(0);
#if defined(AMD_DEBUG)
		if (GSD_EVT_ID_DEBUG_DEMO == SyGetEvtInfo()->old_evt_id) {
			//デバッグメニューから来たらデバッグメニューに戻る
			SyDecideEvt(GSD_EVT_ID_DEBUG_DEMO);
		}
#endif //defined(AMD_DEBUG)
		SyChangeNextEvt();
	}
	return;

#if defined(MTD_DEBUG)
	int tapsum = 0;
	for (int i = 0; i < 5; ++i) {
		if (amTpIsTouchOn(i)) {
			++tapsum;
		}
	}
	if (3 <= tapsum) {
		SyDecideEvtCase(4);
		SyChangeNextEvt();		
	} else //下に繋げる
#endif //defined(MTD_DEBUG)
	dm::movie::CMain::CreateInstance();
}




// =============================================================================
// pxTemplate::Function
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
#endif //_IPHONE
