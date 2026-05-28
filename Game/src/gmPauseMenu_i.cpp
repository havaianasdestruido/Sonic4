// ============================================================================
/*!
	@file	dmTitle_i.cpp
	@brief	タイトル(iPhone)

	@author	Kouji Hokazono <kouji_hokazono@dimps.co.jp>
		Copyright(c) 2009 Dimps
	$Id: gmPauseMenu_i.cpp 2 2011-04-11 05:21:26Z thamada $
 */
// ============================================================================
/*
 * $Log$
 */

//------ Include ---------------------- インクルード ---------------------------******_IC*
#include "pch.h"

#if _IPHONE
#include "aoTexture.h"
#include "aoAction.h"
#include "izFade.h"

#include "gmSound.h"
#include "gmPauseMenu.h"
#include "gmMain.h"

#include "erObject.hpp"
#include "erTask.hpp"
#include "erTrgAoAction.hpp"

#include "gs.h"
#include "gsEnvironment.h"
#include "gsMainSys.h"
extern BOOL _am_sample_draw_enable;

#include "accelArray.hpp"
#include "accelBitset.hpp"
#include "accelLengthof.hpp"
#include "accelInitializer.hpp"
#include "accelLerp.hpp"

//リソース系
#include "ace/G_PAUSE.HMA"
#include "ace/G_PAUSE_L.HMA"


//------ Debug ------------------------ デバッグ -------------------------------******_DG*
#if defined(MTD_DEBUG)
#endif	//#if defined(MTD_DEBUG)
//------ Macro ------------------------ マクロ ---------------------------------******_MC*






















namespace gs {
namespace pause_menu {


namespace {


//------------------------------------------------------------------------------**********
namespace setting {
	//ファイルパス
	namespace file {
		const char *c_global_ama	= GSS_BASE_PATH"G_COM/MENU/G_PAUSE.AMA";		//各国共通ファイル
		const char *c_global_amb	= GSS_BASE_PATH"G_COM/MENU/G_PAUSE.AMB";		//各国共通ファイル
		const char *c_lang_ama		= GSS_BASE_PATH"G_COM/MENU/G_PAUSE_L.AMA";		//国別ファイル
		const char *c_lang_amb[]	= {	GSS_BASE_PATH"G_COM/MENU/G_PAUSE_JP.AMB"	//国別ファイル
									,	GSS_BASE_PATH"G_COM/MENU/G_PAUSE_US.AMB"
									,	GSS_BASE_PATH"G_COM/MENU/G_PAUSE_FR.AMB"
									,	GSS_BASE_PATH"G_COM/MENU/G_PAUSE_IT.AMB"
									,	GSS_BASE_PATH"G_COM/MENU/G_PAUSE_GE.AMB"
									,	GSS_BASE_PATH"G_COM/MENU/G_PAUSE_SP.AMB"
									};
	} //namespace file
	//メイン
	namespace main {
		const Uint32 c_draw_state		= 4;		//<描画ステート
		const Uint32 c_task_priority	= 0x7100;	//<タスク優先度
		const Uint32 c_task_user		= 0;		//<タスク所有者
		const Uint32 c_task_attribute	= 0;		//<タスク属性

	} //namespace main
} //namespace setting
//------------------------------------------------------------------------------**********













































// ============================================================================
// private_cast
/*!
	キャスト

	@note
		特殊化の定義は下の方に別定義
 */
// ============================================================================
template <typename TTo, typename TFrom>
TTo private_cast(const TFrom &from);	//<定義しない事(定義が無い事に意義がある)
//------------------------------------------------------------------------------**********











} //namespace







































//------ Class ------------------------ クラス ---------------------------------******_CL*
/*!	@class
	@brief	タイトルクラス
		タイトルを提供します。
 */
class CMain : public ::er::task::CTask<CMain, ::er::task::CProcCount<CMain>, ::er::task::ITaskLink> {
	//-- Enum Hack -------------------- 定数宣言 -------------------------------******_EH*
protected:
private:
	typedef ::er::task::CTask<CMain, ::er::task::CProcCount<CMain>, ::er::task::ITaskLink>	super_type;
	typedef super_type::Task																TTask;

	//戻り値
	struct EReturn {
		enum Type {
			Retry,
			Option,
			Back,
			MainMenu,
			Cancel,

			Max,
			None
		};
	};


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
		if (m_flag.test(BFlag::Create)) {
			preUpdate();
		}
		super_type::operator()();
		if (m_flag.test(BFlag::Create)) {
			update();
			draw();
		}
	}

	// ============================================================================
	// CMain::Create
	/*!
		作成
	
		@retval	true	成功
		@retval	false	失敗
	 */
	// ============================================================================
	bool Create();

	// ============================================================================
	// CMain::Release
	/*!
		破棄
	 */
	// ============================================================================
	void Release();

	// ============================================================================
	// CMain::LoadFile
	/*!
		ファイル読み込み
	 */
	// ============================================================================
	void LoadFile() {
		fileLoadingStart();
	}

	// ============================================================================
	// CMain::CreateTexture
	/*!
		テクスチャ作成
	
		@retval	true	成功
		@retval	false	失敗
	 */
	// ============================================================================
	void CreateTexture() {
		creatingStart();
	}

	// ============================================================================
	// CMain::ReleaseTexture
	/*!
		テクスチャ破棄
	 */
	// ============================================================================
	void ReleaseTexture() {
		releasingStart();
	}

	// ============================================================================
	// CMain::Start
	/*!
		開始

		@param	prio	[in]	メインタスク優先度
	 */
	// ============================================================================
	void Start(u32 prio) {
		fadeInStart(prio);
	}

	// ============================================================================
	// CMain::Cancel
	/*!
		取消
	 */
	// ============================================================================
	void Cancel() {
		m_flag[BFlag::ReqCancel] = true;
	}


	//-- Geter Function --------------- 取得関数 -------------------------------******PGF*
	// =============================================================================
	// CFile::IsValid
	/*!
		有効確認
	
		@retval	true	有効
		@retval	false	無効
	 */
	// ==========================================================================
	bool IsValid() const {
		return m_flag[BFlag::CreatedTexture];
	}

	// =============================================================================
	// CFile::IsEmpty
	/*!
		空確認
	
		@retval	true	空
		@retval	false	存在
	 */
	// ==========================================================================
	bool IsEmpty() const {
		return !m_flag[BFlag::Create];
	}

	// =============================================================================
	// CFile::IsLoadFile
	/*!
		ファイル読み込み確認
	
		@retval	true	有効
		@retval	false	無効
	 */
	// ==========================================================================
	bool IsLoadFile() const {
		return m_flag[BFlag::LoadedFile];
	}

	// =============================================================================
	// CFile::IsCreatedTexture
	/*!
		テクスチャ作成確認
	
		@retval	true	有効
		@retval	false	無効
	 */
	// ==========================================================================
	bool IsCreatedTexture() const {
		return m_flag[BFlag::CreatedTexture];
	}

	// =============================================================================
	// CFile::IsReleasedTexture
	/*!
		テクスチャ破棄確認
	
		@retval	true	有効
		@retval	false	無効
	 */
	// ==========================================================================
	bool IsReleasedTexture() const {
		return !m_flag[BFlag::CreateTexture];
	}

	// =============================================================================
	// CFile::IsPlay
	/*!
		実行確認
	
		@retval	true	空
		@retval	false	存在
	 */
	// ==========================================================================
	bool IsPlay() const {
		return !m_flag[BFlag::Start];
	}

	// =============================================================================
	// CFile::GetResult
	/*!
		結果取得
	
		@return	結果
	 */
	// ==========================================================================
	EReturn::Type GetResult() const {
		return m_return;
	}



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
	//フラグ
	struct BFlag {
		enum {
			Create,
			NoUpdate,
			NoDraw,
			LoadFile,
			LoadedFile,
			CreateTexture,
			CreatedTexture,
			Start,
			ReqCancel,

			Max,
			None,
		};
		typedef accel::CBitset<Max> Type;
	};

	//メモリ開放が必要なファイル
	struct EMemFile {
		enum Type {
			Ama,
			AmaLang,
			Amb,
			AmbLang,

			Max,
			None
		};
	};

	//ファイル
	struct EFile : public EMemFile {
		typedef int Type;
		enum {
			Max = EMemFile::Max,
			None
		};
	};

	//テクスチャ
	struct ETex {
		enum Type {
			Amb,
			AmbLang,

			Max,
			None
		};
	};

	//アクション
	struct EAct {
		enum Type {
			Bgi,
			Btn1Left,
			Btn1Center,
			Btn1Right,
			Btn3Left,
			Btn3Center,
			Btn3Right,
			Btn4Left,
			Btn4Center,
			Btn4Right,
			Retry,
			Back,
			Cancel,
			MsgRetry,
			MsgReturn,
			No,
			Yes,

			Max,
			None,
		};
	};

	//トリガ
	struct ETrg {
		enum Type {
			Btn1,
			Btn3,
			Btn4,

			Max,
			None,
		};
	};

	 //拡張アクション構造体
	 struct SAction {
		struct BFlag {
			enum {
				NoUpdate,
				NoDraw,
				 
				Max,
				None,
			};
			typedef accel::CBitset<Max> Type;
		};
		 
		AOS_ACTION				*act;
		const AOS_TEXTURE		*tex;
		BFlag::Type				flag;
		accel::CArray<float, 2>	scale;
		accel::CArray<float, 3>	pos;
		AOS_ACT_COL				color;
		
		void AcmInit() {
			pos = accel::CArray<float, 3>::initializer(0.0f, 0.0f, 0.0f);
			scale = accel::CArray<float, 2>::initializer(1.0f, 1.0f);
			color.c = 0xFFFFFFFF;
		}
		void Update() {
			AoActAcmPush();
			f32 frame = ((flag[SAction::BFlag::NoUpdate])? 0.0f: 1.0f);
			AoActSetTexture(AoTexGetTexList(const_cast<AOS_TEXTURE *>(tex)));
			if (accel::CArray<float, 2>::initializer(1.0f, 1.0f) != scale) {
				AoActAcmApplyScale(scale.x(), scale.y());
			}
			if (accel::CArray<float, 3>::initializer(0.0f, 0.0f, 0.0f) != pos) {
				AoActAcmApplyTrans(pos.x(), pos.y(), pos.z());
			}
			if (0xFFFFFFFF != color.c) {
				AoActAcmApplyColor(color);
			}
			AoActUpdate(act, frame);
			AoActAcmPop();
		}
		void Draw() const {
			if (!flag[SAction::BFlag::NoDraw]) {
				AoActSortRegAction(act);
			}
		}
	};
	
	//SE
	struct ESe {
		enum Type {
			Enter,		//決定
			Cancel,		//キャンセル
			Window,		//ウインドウ
			Pause,		//ポーズメニュー起動時

			Max,
			None,
		};
	};
	

	//ボタン→戻り値変換テーブル
	static EReturn::Type c_return_table[ETrg::Max];
	
	static const u32 c_pause_btn_se_frame = 15;		//<PAUSEボタンSEフレーム数
	static const u32 c_fade_in_frame = 8;			//<拡縮インフレーム数
	static const u32 c_fade_out_frame = 8;			//<拡縮アウトフレーム数
	static const u32 c_fade_enter_efct_frame = 10;	//<決定演出フレーム数



	//-- Local Variable --------------- ローカル変数 ---------------------------******LVA*
protected:
private:
	BFlag::Type				m_flag;					//<フラグ
	EReturn::Type			m_return;				//<リザルト
	EReturn::Type			m_really;				//<確認
	AMS_FS					*m_fs[EMemFile::Max];	//<ファイル読み込みリクエスト
	void					*m_file[EFile::Max];	//<ファイル
	AOS_TEXTURE				m_tex[ETex::Max];		//<テクスチャ
	SAction					m_act[EAct::Max];		//<アクション
	er::CTrgAoAction		m_trg[ETrg::Max];		//<トリガ
	GSS_SND_SE_HANDLE		*m_se_handle;			//<SE再生用ハンドル


	//-- Local Function --------------- ローカル関数 ---------------------------******LFC*
protected:
private:
	void preUpdate();
	void update();
	void draw();

	void fileLoadingStart();
	void fileLoading();
	void creatingStart();
	void creating();
	void fadeInStart(Sint32 prio = setting::main::c_task_priority);
	void fadeIn();
	void fadeIn2();
	void waitStart();
	void wait();
	void selectStart();
	void select();
	void reallyStart();
	void really();
	void enterEfctStart();
	void enterEfct();
	void pauseBtnCancelStart();
	void pauseBtnCancel();
	void fadeOutStart();
	void fadeOut();
	void releasingStart();
	void releasing();
	void playSe(ESe::Type se);
	
	static bool canGoStageSelect();
	static bool isSpecialStage();
	static EReturn::Type TrgIdxToReturnIdx(int trg_idx);



//------------------------------------------------------------------------------**********
}; //class CMain




//------ Class ------------------------ クラス ---------------------------------******_CL*
//■//-- Local Constant --------------- ローカル定数 ---------------------------******LCS*
//ボタン→戻り値変換テーブル
CMain::EReturn::Type CMain::c_return_table[ETrg::Max] = {
								EReturn::Retry
							,	EReturn::Back
							,	EReturn::Cancel
							};


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
	static CMain g_instance;
	return g_instance;
}

// ============================================================================
// CMain::Create
/*!
	作成

	@retval	true	成功
	@retval	false	失敗
 */
// ============================================================================
bool CMain::Create()
{
	return true;
}

// ============================================================================
// CMain::Release
/*!
	破棄
 */
// ============================================================================
void CMain::Release()
{
	if (m_flag[BFlag::Create]) {
		if (m_flag[BFlag::LoadFile]) {
			amAssert(m_flag[BFlag::LoadedFile]);

			for (void **file = &m_file[0], **file_end = &m_file[EMemFile::Max]; file != file_end; ++file) {
				amMemFree(*file);
			}
			m_flag[BFlag::LoadFile] = false;
			m_flag[BFlag::LoadedFile] = false;
		}

		DetachTask();
		m_flag.reset();
	}
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
}

// =============================================================================
// CMain::~CMain
/*!
	デストラクタ
 */
// ==========================================================================
CMain::~CMain()
{
//	amAssert(!m_flag[BFlag::Create]);
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
	if (m_flag[BFlag::Start] && !m_flag[BFlag::NoUpdate]) {
		for (er::CTrgAoAction *trg = &m_trg[0], *trg_end = &m_trg[ETrg::Max]; trg != trg_end; ++trg) {
			trg->Update();
		}
	}
}

// =============================================================================
// CMain::update
/*!
	更新
 */
// ==========================================================================
void CMain::update()
{
	if (m_flag[BFlag::Start]) {
		AoActAcmPush();
		AoActAcmApplyTrans(0.0f, 0.0f, -1000.0f);
		for (SAction *act = &m_act[0], *act_end = &m_act[EAct::Max]; act != act_end; ++act) {
			act->Update();
		}
		AoActAcmPop();
	}
}

// =============================================================================
// CMain::draw
/*!
	描画
 */
// ==========================================================================
void CMain::draw()
{
	if (_am_sample_draw_enable) {
		if (m_flag[BFlag::Start] && !m_flag[BFlag::NoUpdate]) {
			for (const SAction *act = &m_act[0], *act_end = &m_act[EAct::Max]; act != act_end; ++act) {
				act->Draw();
			}
		}
	}
}

// =============================================================================
// CMain::fileLoadingStart
/*!
	ファイルローディング開始
 */
// ==========================================================================
void CMain::fileLoadingStart()
{
	//読み込みリクエストの発行
	//各国共通
	using namespace setting::file;
	m_fs[EMemFile::Ama] = amFsReadBackground(const_cast<char *>(c_global_ama));
	m_fs[EMemFile::Amb] = amFsReadBackground(const_cast<char *>(c_global_amb));
	//国別
	GSE_LANGUAGE lang = GsEnvGetLanguage();
	m_fs[EMemFile::AmaLang] = amFsReadBackground(const_cast<char *>(c_lang_ama));
	m_fs[EMemFile::AmbLang] = amFsReadBackground(const_cast<char *>(c_lang_amb[lang]));

	m_flag[BFlag::Create] = true;
	m_flag[BFlag::LoadFile] = true;

	//タスク設定
	using namespace setting::main;
	AttachTask("gmPauseMenu::Load", c_task_priority, c_task_user, c_task_attribute);
	SetProc(&CMain::fileLoading);
}

// =============================================================================
// CMain::fileLoad
/*!
	ファイルローディング
 */
// ==========================================================================
void CMain::fileLoading()
{
	bool loaded = true;
	for (AMS_FS **fs = &m_fs[0], **fs_end = &m_fs[EMemFile::Max]; fs != fs_end; ++fs) {
		if (!amFsIsComplete(*fs)) {
			loaded = false;
			break;
		}
	}

	if (loaded) {
		//ロード完了
		//読み込み後処理
		for (int i = 0; i < EMemFile::Max; ++i) {
			//読み込みリクエスト開放
			m_file[i] = m_fs[i]->buf;
			m_fs[i]->buf = NULL;
			amFsClearRequest(m_fs[i]);
			m_fs[i] = NULL;

			//アドレス変換
			amConvertAddress(reinterpret_cast<Uint8 *>(m_file[i]));
		}

		m_flag[BFlag::LoadedFile] = true;

		//タスク破棄
		DetachTask();
	}
}

// =============================================================================
// CMain::creatingStart
/*!
	作成開始
 */
// ==========================================================================
void CMain::creatingStart()
{
	const EFile::Type c_local_create_table[] = {
		EFile::Amb,
		EFile::AmbLang,
	};
	for (unsigned int i = 0; i < accel::lengthof(c_local_create_table); ++i) {
		const EFile::Type &create = c_local_create_table[i];
		AoTexBuild(&m_tex[i], m_file[create]);
		AoTexLoad(&m_tex[i]);
	}

	m_flag[BFlag::CreateTexture] = true;

	//タスク設定
	using namespace setting::main;
	AttachTask("gmPauseMenu::Build", c_task_priority, c_task_user, c_task_attribute);
	SetProc(&CMain::creating);
}

// =============================================================================
// CMain::creating
/*!
	作成
 */
// ==========================================================================
void CMain::creating()
{
	//作成終了確認
	bool created = true;
	for (AOS_TEXTURE *tex = &m_tex[0], *tex_end = &m_tex[ETex::Max]; tex != tex_end; ++tex) {
		if (!AoTexIsLoaded(&*tex)) {
			created = false;
			break;
		}
	}

	if (created) {
		//構築完了
		m_flag[BFlag::CreatedTexture] = true;

		//タスク破棄
		DetachTask();
	}
}

// =============================================================================
// CMain::fadeInStart
/*!
	フェードイン開始
 */
// ==========================================================================
void CMain::fadeInStart(Sint32 prio)
{
	using namespace setting::main;

	//アクション
	struct SLocalCreateActionTable {
		EFile::Type	file;
		ETex::Type	tex;
		Sint32		idx;
	};
	SLocalCreateActionTable local_create_action_table[EAct::Max] = {
		{EFile::Ama		, ETex::Amb		, IDA_G_PAUSE_ACT_BACK			},	//Bgi
		{EFile::Ama		, ETex::Amb		, IDA_G_PAUSE_ACT_BTN01_L		},	//Btn1Left
		{EFile::Ama		, ETex::Amb		, IDA_G_PAUSE_ACT_BTN01_C		},	//Btn1Center
		{EFile::Ama		, ETex::Amb		, IDA_G_PAUSE_ACT_BTN01_R		},	//Btn1Right
		{EFile::Ama		, ETex::Amb		, IDA_G_PAUSE_ACT_BTN03_L		},	//Btn3Left
		{EFile::Ama		, ETex::Amb		, IDA_G_PAUSE_ACT_BTN03_C		},	//Btn3Center
		{EFile::Ama		, ETex::Amb		, IDA_G_PAUSE_ACT_BTN03_R		},	//Btn3Right
		{EFile::Ama		, ETex::Amb		, IDA_G_PAUSE_ACT_BTN04_L		},	//Btn4Left
		{EFile::Ama		, ETex::Amb		, IDA_G_PAUSE_ACT_BTN04_C		},	//Btn4Center
		{EFile::Ama		, ETex::Amb		, IDA_G_PAUSE_ACT_BTN04_R		},	//Btn4Right
		{EFile::AmaLang	, ETex::AmbLang	, IDA_G_PAUSE_JP_ACT_TEX_RETRY	},	//Retry
		{EFile::AmaLang	, ETex::AmbLang	, IDA_G_PAUSE_JP_ACT_TEX_WORLD	},	//Back
		{EFile::AmaLang	, ETex::AmbLang	, IDA_G_PAUSE_JP_ACT_TEX_MODORU	},	//Cancel
		{EFile::AmaLang	, ETex::AmbLang	, IDA_G_PAUSE_JP_ACT_MSG_RETRY	},	//MsgRetry
		{EFile::AmaLang	, ETex::AmbLang	, IDA_G_PAUSE_JP_ACT_MSG_WORLD	},	//MsgReturn
		{EFile::AmaLang	, ETex::AmbLang	, IDA_G_PAUSE_JP_ACT_TEX_IIE	},	//No
		{EFile::AmaLang	, ETex::AmbLang	, IDA_G_PAUSE_JP_ACT_TEX_HAI	},	//Yes
	};
	if (!canGoStageSelect()) {
		//ステージセレクトにいけないなら“ステセレに戻る”を“メインメニューに戻る”に変更
		local_create_action_table[EAct::Back].idx = IDA_G_PAUSE_JP_ACT_TEX_MAIN;
		local_create_action_table[EAct::MsgReturn].idx = IDA_G_PAUSE_JP_ACT_MSG_MAIN;
	}
	if (isSpecialStage()) {
		//スペシャルステージなら“アクトをやり直す”を“ステージをやり直す”に変更
		local_create_action_table[EAct::MsgRetry].idx = IDA_G_PAUSE_JP_ACT_MSG_RETRY2;
	}
	for (unsigned int i = 0; i < EAct::Max; ++i) {
		const SLocalCreateActionTable &create = local_create_action_table[i];
		const void *ama = m_file[create.file];
		SAction &act = m_act[i];
		act.act = AoActCreate(ama, create.idx);
		act.tex = &m_tex[create.tex];
		act.flag[SAction::BFlag::NoUpdate] = true;
		act.flag[SAction::BFlag::NoDraw] = true;
		act.AcmInit();
	}

	//トリガ
	const EAct::Type c_local_create_trg_table[ETrg::Max] = {
		EAct::Btn1Center,
		EAct::Btn3Center,
		EAct::Btn4Center,
	};
	for (unsigned int i = 0; i < ETrg::Max; ++i) {
		const SAction &act  = m_act[c_local_create_trg_table[i]];
		er::CTrgAoAction &trg = m_trg[i];
		trg.Create(act.act);
	}

	m_flag[BFlag::Start] = true;

	//背景表示
	m_act[EAct::Bgi].flag[SAction::BFlag::NoDraw] = false;
	m_act[EAct::Bgi].scale = accel::CArray<float, 2>::initializer(0.0f, 0.0f);
	
	//SEハンドル確保
	m_se_handle = GsSoundAllocSeHandle();
	
	//タスク設定
	using namespace setting::main;
	AttachTask("gmPauseMenu::Execute", prio, c_task_user, c_task_attribute);

	playSe(ESe::Enter);
	SetProc(&CMain::fadeIn);
}

// =============================================================================
// CMain::fadeIn
/*!
	フェードイン
 */
// ==========================================================================
void CMain::fadeIn()
{
	if (c_pause_btn_se_frame < GetCount()) {
		playSe(ESe::Pause);
		SetProc(&CMain::fadeIn2);
	}
}

// =============================================================================
// CMain::fadeIn2
/*!
	フェードイン
 */
// ==========================================================================
void CMain::fadeIn2()
{
	float prgrs = static_cast<float>(GetCount()) / c_fade_in_frame;
	m_act[EAct::Bgi].scale = accel::CArray<float, 2>::initializer(prgrs, prgrs);

	if (c_fade_in_frame < GetCount()) {
		waitStart();
	}
}

// =============================================================================
// CMain::waitStart
/*!
	全てのタップが離れるまで待つ の開始
 */
// ==========================================================================
void CMain::waitStart()
{
	//ボタン表示
	for (SAction *act = &m_act[EAct::Btn1Left], *act_end = &m_act[EAct::Cancel+1]; act != act_end; ++act) {
		act->flag[SAction::BFlag::NoDraw] = false;
	}

	SetProc(&CMain::wait);
}

// =============================================================================
// CMain::wait
/*!
	全てのタップが離れるまで待つ
 */
// ==========================================================================
void CMain::wait()
{
	bool is_on = false;
	for (int i = 0, max = arrayof(_am_tp_touch); i < max; ++i) {
		if (amTpIsTouchOn(i)) {
			is_on = true;
			break;
		}
		
	}
	if (!is_on || (60 < GetCount())) {
		//全てのタップが離れているか、60f経過すると次へ
		selectStart();
	}
}

// =============================================================================
// CMain::selectStart
/*!
	選択開始
 */
// ==========================================================================
void CMain::selectStart()
{
	//全非更新へ
	for (SAction *act = m_act, *act_end = m_act + EAct::Max; act != act_end; ++act) {
		act->flag[SAction::BFlag::NoUpdate] = true;
		AoActSetFrame(act->act, 0.0f);
		act->pos = accel::CArray<float, 3>::initializer(0.0f, 0.0f, 0.0f);
	}
	//ボタン表示
	for (SAction *act = &m_act[EAct::Btn1Left], *act_end = &m_act[EAct::Cancel+1]; act != act_end; ++act) {
		act->flag[SAction::BFlag::NoDraw] = false;
	}
	//再確認非表示
	for (SAction *act = &m_act[EAct::MsgRetry], *act_end = &m_act[EAct::Yes+1]; act != act_end; ++act) {
		act->flag[SAction::BFlag::NoDraw] = true;
	}

	m_return = EReturn::None;
	SetProc(&CMain::select);
}

// =============================================================================
// CMain::select
/*!
	選択
 */
// ==========================================================================
void CMain::select()
{
	//トリガ
	const ETrg::Type c_trg_table[] = {
		ETrg::Btn1,
		ETrg::Btn3,
		ETrg::Btn4,
	};
	const EAct::Type c_btn_action_table[arrayof(c_trg_table)][3] = {
		{EAct::Btn1Left, EAct::Btn1Center, EAct::Btn1Right},
		{EAct::Btn3Left, EAct::Btn3Center, EAct::Btn3Right},
		{EAct::Btn4Left, EAct::Btn4Center, EAct::Btn4Right},
	};
	typedef er::CTrgState::EState EState;

	int crnt_trg = -1;
	for (int i = 0; i < arrayof(c_trg_table); ++i) {
		er::CTrgAoAction &trg = m_trg[c_trg_table[i]];
		f32 frame;
		if (trg.GetState(0)[EState::Up] && trg.GetState(0)[EState::Prev]) {
			//決定
			frame = 2.0f;
			crnt_trg = i;
		} else if (trg.GetState(0)[EState::On]) {
			//ON
			frame = 3.0f;
		} else {
			//OFF
			frame = 0.0f;
		}

		for (unsigned int k = 0; k < accel::lengthof(c_btn_action_table[i]); ++k) {
			const EAct::Type *btn_action = c_btn_action_table[i];
			AoActSetFrame(m_act[btn_action[k]].act, frame);
		}
	}

	//決定・キャンセル確認
	if (-1 != crnt_trg) {
		//決定なら
		//決定ボタンのみ決定アニメーション
		const EAct::Type *btn_action = c_btn_action_table[crnt_trg];
		for (SAction *act = &m_act[btn_action[0]], *act_end = &m_act[btn_action[2]+1]; act != act_end; ++act) {
			act->flag[SAction::BFlag::NoUpdate] = false;
		}
		//戻り値算出
		m_return = TrgIdxToReturnIdx(crnt_trg);
	} else if (m_flag[BFlag::ReqCancel]) {
		//キャンセルなら
		m_return = EReturn::Cancel;
	}

	//遷移確認
	if (0 <= GmMainKeyCheckPauseKeyPush()) {
		pauseBtnCancelStart();
	} else switch (m_return) {
	case EReturn::None: //操作無し
		break;
	case EReturn::Cancel: //ゲームに戻る
		playSe(ESe::Cancel);
		enterEfctStart();
		break;
	default: //リトライ・メインメニュー	
		playSe(ESe::Enter);
		reallyStart();
		break;
	}
}

// =============================================================================
// CMain::reallyStart
/*!
	再確認開始
 */
// ==========================================================================
void CMain::reallyStart()
{
	//全非更新へ
	for (SAction *act = m_act, *act_end = m_act + EAct::Max; act != act_end; ++act) {
		act->flag[SAction::BFlag::NoUpdate] = true;
		AoActSetFrame(act->act, 0.0f);
		act->pos = accel::CArray<float, 3>::initializer(0.0f, 0.0f, 0.0f);
	}
	//戻る台紙非表示
	for (SAction *act = &m_act[EAct::Btn4Left], *act_end = &m_act[EAct::Btn4Right+1]; act != act_end; ++act) {
		act->flag[SAction::BFlag::NoDraw] = true;
	}
	//メッセージテキストの位置と拡大量調整
	for (SAction *act = &m_act[EAct::MsgRetry], *act_end = &m_act[EAct::MsgReturn+1]; act != act_end; ++act) {
		float c_text_scale = 1.5f * 1.125f;
		act->pos = accel::CArray<float, 3>::initializer(480.0f, 269.0f, 0.0f);
		act->scale = accel::CArray<float, 2>::initializer(c_text_scale, c_text_scale);
	}
	//再確認表示
	switch (m_return) {
	case EReturn::Retry: //リトライ
		m_act[EAct::MsgRetry].flag[SAction::BFlag::NoDraw] = false;
		m_act[EAct::MsgReturn].flag[SAction::BFlag::NoDraw] = true;
		m_act[EAct::Retry].flag[SAction::BFlag::NoDraw] = true;
		m_act[EAct::Back].flag[SAction::BFlag::NoDraw] = true;
		m_act[EAct::Cancel].flag[SAction::BFlag::NoDraw] = true;
		m_act[EAct::No].flag[SAction::BFlag::NoDraw] = false;
		m_act[EAct::Yes].flag[SAction::BFlag::NoDraw] = false;
		for (SAction *act = &m_act[EAct::Btn1Left], *act_end = &m_act[EAct::Btn1Right+1]; act != act_end; ++act) {
			act->pos = accel::CArray<float, 3>::initializer(0.0f, 194.0f, 0.0f);
		}
		for (SAction *act = &m_act[EAct::Btn3Left], *act_end = &m_act[EAct::Btn3Right+1]; act != act_end; ++act) {
			act->pos = accel::CArray<float, 3>::initializer(0.0f, 194.0f, 0.0f);
		}
		break;
	case EReturn::Back: //ステージセレクト
	case EReturn::MainMenu: //メインメニュー
		m_act[EAct::MsgRetry].flag[SAction::BFlag::NoDraw] = true;
		m_act[EAct::MsgReturn].flag[SAction::BFlag::NoDraw] = false;
		m_act[EAct::Retry].flag[SAction::BFlag::NoDraw] = true;
		m_act[EAct::Back].flag[SAction::BFlag::NoDraw] = true;
		m_act[EAct::Cancel].flag[SAction::BFlag::NoDraw] = true;
		m_act[EAct::No].flag[SAction::BFlag::NoDraw] = false;
		m_act[EAct::Yes].flag[SAction::BFlag::NoDraw] = false;
		for (SAction *act = &m_act[EAct::Btn1Left], *act_end = &m_act[EAct::Btn1Right+1]; act != act_end; ++act) {
			act->pos = accel::CArray<float, 3>::initializer(380.0f, 194.0f, 0.0f);
		}
		for (SAction *act = &m_act[EAct::Btn3Left], *act_end = &m_act[EAct::Btn3Right+1]; act != act_end; ++act) {
			act->pos = accel::CArray<float, 3>::initializer(-380.0f, 194.0f, 0.0f);
		}
		break;
	}

	m_really = EReturn::None;
	SetProc(&CMain::really);
}

// =============================================================================
// CMain::really
/*!
	再確認
 */
// ==========================================================================
void CMain::really()
{
	//トリガ
	const ETrg::Type c_trg_table[] = {
		ETrg::Btn1,
		ETrg::Btn3,
	};
	const EAct::Type c_btn_action_table[arrayof(c_trg_table)][3] = {
		{EAct::Btn1Left, EAct::Btn1Center, EAct::Btn1Right},
		{EAct::Btn3Left, EAct::Btn3Center, EAct::Btn3Right},
//		{EAct::Btn4Left, EAct::Btn4Center, EAct::Btn4Right},
	};
	typedef er::CTrgState::EState EState;

	int crnt_trg = -1;
	for (int i = 0; i < arrayof(c_trg_table); ++i) {
		er::CTrgAoAction &trg = m_trg[c_trg_table[i]];
		f32 frame;
		if (trg.GetState(0)[EState::Up] && trg.GetState(0)[EState::Prev]) {
			//決定
			frame = 2.0f;
			crnt_trg = i;
		} else if (trg.GetState(0)[EState::On]) {
			//ON
			frame = 3.0f;
		} else {
			//OFF
			frame = 0.0f;
		}

		for (unsigned int k = 0; k < accel::lengthof(c_btn_action_table[i]); ++k) {
			const EAct::Type *btn_action = c_btn_action_table[i];
			AoActSetFrame(m_act[btn_action[k]].act, frame);
		}
	}

	//決定・キャンセル確認
	if (-1 != crnt_trg) {
		//決定なら
		//決定ボタンのみ決定アニメーション
		const EAct::Type *btn_action = c_btn_action_table[crnt_trg];
		for (SAction *act = &m_act[btn_action[0]], *act_end = &m_act[btn_action[2]+1]; act != act_end; ++act) {
			act->flag[SAction::BFlag::NoUpdate] = false;
		}
		//戻り値算出
		m_really = TrgIdxToReturnIdx(crnt_trg);
	} else if (m_flag[BFlag::ReqCancel]) {
		//キャンセルなら
		m_really = EReturn::Cancel;
	}

	//遷移確認
	if (0 <= GmMainKeyCheckPauseKeyPush()) {
		//ゲームに戻る
		m_return = EReturn::Cancel;
		pauseBtnCancelStart();
	} else if (EReturn::None == m_really) {
		//操作無し
	} else if (m_return == m_really) {
		playSe(ESe::Enter);
		enterEfctStart();
	} else {
		playSe(ESe::Cancel);
		selectStart();
	}
}

// =============================================================================
// CMain::enterEfctStart
/*!
	決定演出開始
 */
// ==========================================================================
void CMain::enterEfctStart()
{
	SetProc(&CMain::enterEfct);
}

// =============================================================================
// CMain::enterEfct
/*!
	決定演出
 */
// ==========================================================================
void CMain::enterEfct()
{
	if (c_fade_enter_efct_frame < GetCount()) {
		fadeOutStart();
	}
}

// =============================================================================
// CMain::pauseBtnCancelStart
/*!
	ポーズボタンキャンセル開始
 */
// ==========================================================================
void CMain::pauseBtnCancelStart()
{
	//SE再生
	playSe(ESe::Enter);

	SetProc(&CMain::pauseBtnCancel);
}

// =============================================================================
// CMain::pauseBtnCancel
/*!
	ポーズボタンキャンセル
 */
// ==========================================================================
void CMain::pauseBtnCancel()
{
	if (c_pause_btn_se_frame < GetCount()) {
		fadeOutStart();
	}
}

// =============================================================================
// CMain::fadeOutStart
/*!
	フェードアウト開始
 */
// ==========================================================================
void CMain::fadeOutStart()
{
	//全非更新へ
	for (SAction *act = m_act, *act_end = m_act + EAct::Max; act != act_end; ++act) {
		act->flag[SAction::BFlag::NoUpdate] = true;
		act->flag[SAction::BFlag::NoDraw] = true;
	}
	//背景のみ表示
	m_act[EAct::Bgi].flag[SAction::BFlag::NoUpdate] = false;
	m_act[EAct::Bgi].flag[SAction::BFlag::NoDraw] = false;
	
	//SE再生
	playSe(ESe::Pause);

	SetProc(&CMain::fadeOut);
}

// =============================================================================
// CMain::fadeOut
/*!
	フェードアウト
 */
// ==========================================================================
void CMain::fadeOut()
{
	float prgrs = 1.0f - (static_cast<float>(GetCount()) / c_fade_out_frame);
	m_act[EAct::Bgi].scale = accel::CArray<float, 2>::initializer(prgrs, prgrs);

	if (c_fade_out_frame < GetCount()) {
		//破棄
		for (SAction *act = &m_act[0], *act_end = &m_act[EAct::Max]; act != act_end; ++act) {
			AoActDelete(act->act);
		}

		//SEハンドル解放
		GsSoundFreeSeHandle(m_se_handle);

		m_flag[BFlag::Start] = false;

		//タスク破棄
		DetachTask();
	}
}

// =============================================================================
// CMain::releasingStart
/*!
	破棄開始
 */
// ==========================================================================
void CMain::releasingStart()
{
	//破棄
	for (AOS_TEXTURE *tex = &m_tex[0], *tex_end = &m_tex[ETex::Max]; tex != tex_end; ++tex) {
		AoTexRelease(&*tex);
	}

	//タスク設定
	using namespace setting::main;
	AttachTask("gmPauseMenu::Flush", c_task_priority, c_task_user, c_task_attribute);
	SetProc(&CMain::releasing);
}

// =============================================================================
// CMain::releasing
/*!
	破棄
 */
// ==========================================================================
void CMain::releasing()
{
	//破棄終了確認
	bool released = true;
	for (AOS_TEXTURE *tex = &m_tex[0], *tex_end = &m_tex[ETex::Max]; tex != tex_end; ++tex) {
		if (!AoTexIsReleased(&*tex)) {
			released = false;
			break;
		}
	}

	if (released) {
		//開放完了
		m_flag[BFlag::CreateTexture] = false;
		m_flag[BFlag::CreatedTexture] = false;

		//タスク破棄
		DetachTask();
	}
}

// =============================================================================
// CMain::playSe
/*!
	SE再生
 */
// ==========================================================================
void CMain::playSe(ESe::Type se)
{
	amAssert(se < ESe::Max);

	const char *c_se_name_tbl[ESe::Max] = {
		"Ok",			//決定
		"Cancel",		//キャンセル
		"Window",		//ウインドウ
		"Pause",		//ポーズメニュー起動時
	};

	// グローバルワーク取得
	GmSoundPlaySE(const_cast<char *>(c_se_name_tbl[se]), m_se_handle);
}

// =============================================================================
// CMain::canGoStageSelect
/*!
	ステージセレクトに移行可能か確認
 */
// ==========================================================================
bool CMain::canGoStageSelect()
{
	return ((GsMainSysIsStageClear(static_cast<s32>(GSD_MAIN_STAGE_ID_1_1)))? true: false);
}

// =============================================================================
// CMain::isSpecialStage
/*!
	ステージセレクト確認
 */
// ==========================================================================
bool CMain::isSpecialStage()
{
	return ((GSM_MAIN_STAGE_IS_SPSTAGE())? true: false);
}

// =============================================================================
// CMain::TrgIdxToReturnIdx
/*!
	トリガインデックス→戻るインデックス変換
 */
// ==========================================================================
CMain::EReturn::Type CMain::TrgIdxToReturnIdx(int trg_idx)
{
	EReturn::Type result = c_return_table[trg_idx];
	if (!canGoStageSelect() && (EReturn::Back == result)) {
		//ステージセレクトに戻れない時に、“ステセレに戻る”が選択されたなら
		result = EReturn::MainMenu;
	}
	return result;
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












} //namespace pause_menu
} //namespace gs


//------ C Global Functions ----------- C グローバル関数の定義 -------------------******CGF*

// ***************************************************************************
// リソース管理
// ***************************************************************************
// ===========================================================================
//	GmPauseMenuLoadStart
/*!
	リソースファイル読み込み開始

	@note
	ポーズメニューに必要なファイルの読み込みを開始します。\n
	この関数は即時復帰となります。\n
	完了判定はGmPauseMenuLoadIsFinished関数で行うようにして下さい。\n
	既に読み込みを開始している場合に呼び出すと、アサートし何も行いません。\n
	GmPauseMenuLoadIsFinished関数がTRUEを返す状態で呼び出すと、
	アサートし何も行いません。\n
*/
// ===========================================================================
void GmPauseMenuLoadStart(void)
{
	// ファイル読み込み開始
	using namespace gs::pause_menu;
	CMain &pm = CMain::CreateInstance();
	pm.Create();
	pm.LoadFile();
}

// ===========================================================================
//	GmPauseMenuLoadIsFinished
/*!
	リソースファイル読み込み完了判定

	@return 真：完了済み　偽：それ以外
	@note
	GmPauseMenuLoadStart関数で開始したファイル読み込みが
	完了したかどうかを判定します。\n
	GmPauseMenuLoadStart関数を呼び出す前と、
	GmPauseMenuRelease関数を呼び出した後は、は常にFALSEを返します。\n
*/
// ===========================================================================
BOOL GmPauseMenuLoadIsFinished(void)
{
	using namespace gs::pause_menu;
	CMain &pm = CMain::CreateInstance();
	return pm.IsLoadFile();
}

// ===========================================================================
//	GmPauseMenuBuildStart
/*!
	リソース構築開始

	@note
	ポーズメニューに必要なリソースの構築を開始します。\n
	この関数は即時復帰となります。\n
	完了判定はGmPauseMenuBuildIsFinished関数で行うようにして下さい。\n
	GmPauseMenuLoadIsFinished関数がFALSEを返す状態では
	呼び出すことはできません。\n
	(呼び出した場合はアサートし何も行いません。)\n
	既に構築を開始している場合に呼び出すと、アサートし何も行いません。\n
	GmPauseMenuBuildIsFinished関数がTRUEを返す状態で呼び出すと、
	アサートし何も行いません。\n
*/
// ===========================================================================
void GmPauseMenuBuildStart(void)
{
	// 構築開始
	using namespace gs::pause_menu;
	CMain &pm = CMain::CreateInstance();
	pm.CreateTexture();
}

// ===========================================================================
//	GmPauseMenuBuildIsFinished
/*!
	リソース構築完了判定

	@return 真：完了済み　偽：それ以外
	@note
	GmPauseMenuBuildStart関数で開始した構築処理が
	完了したかどうかを判定します。\n
	GmPauseMenuBuildStart関数を呼び出す前と、
	GmPauseMenuFlushStart関数を呼び出した後は、は常にFALSEを返します。\n
*/
// ===========================================================================
BOOL GmPauseMenuBuildIsFinished(void)
{
	using namespace gs::pause_menu;
	CMain &pm = CMain::CreateInstance();
	return pm.IsCreatedTexture();
}

// ===========================================================================
//	GmPauseMenuFlushStart
/*!
	リソース解放開始

	@note
	ポーズメニューに必要なリソースの解放を開始します。\n
	この関数は即時復帰となります。\n
	完了判定はGmPauseMenuFlushIsFinished関数で行うようにして下さい。\n
	GmPauseMenuBuildIsFinished関数がFALSEを返す状態では
	呼び出すことはできません。\n
	(呼び出した場合はアサートし何も行いません。)\n
	既に解放を開始している場合に呼び出すと、アサートし何も行いません。\n
	GmPauseMenuFlushIsFinished関数がTRUEを返す状態で呼び出すと、
	アサートし何も行いません。\n
*/
// ===========================================================================
void GmPauseMenuFlushStart(void)
{
	// 解放開始
	using namespace gs::pause_menu;
	CMain &pm = CMain::CreateInstance();
	pm.ReleaseTexture();
}

// ===========================================================================
//	GmPauseMenuFlushIsFinished
/*!
	リソース解放完了判定

	@return 真：完了済み　偽：それ以外
	@note
	GmPauseMenuFlushStart関数で開始した解放処理が
	完了したかどうかを判定します。\n
	GmPauseMenuFlushStart関数を呼び出す前と、
	GmPauseMenuBuildStart関数を呼び出した後は、は常にFALSEを返します。\n
*/
// ===========================================================================
BOOL GmPauseMenuFlushIsFinished(void)
{
	using namespace gs::pause_menu;
	CMain &pm = CMain::CreateInstance();
	return pm.IsReleasedTexture();
}

// ===========================================================================
//	GmPauseMenuRelease
/*!
	リソースファイル解放

	@note
	ポーズメニューに必要なファイルの解放を行います。\n
	この関数は完了復帰となります。\n
	GmPauseMenuFlushIsFinished関数がFALSEを返す状態では
	呼び出すことはできません。\n
	(呼び出した場合はアサートし何も行いません。)\n
	既に解放済みの場合は何も行いません。\n
*/
// ===========================================================================
void GmPauseMenuRelease(void)
{
	using namespace gs::pause_menu;
	CMain &pm = CMain::CreateInstance();
	pm.Release();
}


// ***************************************************************************
// 実行
// ***************************************************************************
// ===========================================================================
//	GmPauseMenuStart
/*!
	ポーズメニュー開始

	@param prio		[in] メインタスク優先度
	@note
	ポーズメニューを開始します。\n
	以降、GmPauseMenuIsFinished関数がTRUEを返すまで、
	内部でポーズメニュー処理を進めます。\n
*/
// ===========================================================================
void GmPauseMenuStart(u32 prio)
{
	using namespace gs::pause_menu;
	CMain &pm = CMain::CreateInstance();
	pm.Start(prio);

	UNREFERENCED_PARAMETER(prio);
}

// ===========================================================================
//	GmPauseMenuCancel
/*!
	ポーズメニューキャンセル

	@note
	GmPauseMenuStart関数で開始したポーズメニュー処理を強制的に終了させます。\n
	メニューがどのような状態であっても、即時に終了するので、
	リセット時などを除き、通常は使用しないで下さい。\n
	この関数を呼び出した場合、GmPauseMenuGetResult関数で取得できる結果は、
	必ずGME_PMENU_RESULT_CANCELとなります。\n
	この関数を呼び出した直後から、
	GmPauseMenuIsFinished関数がTRUEを返すようになります。\n
*/
// ===========================================================================
void GmPauseMenuCancel(void)
{
	using namespace gs::pause_menu;
	CMain &pm = CMain::CreateInstance();
	pm.Cancel();
}

// ===========================================================================
//	GmPauseMenuIsFinished
/*!
	ポーズメニュー完了判定

	@note
	GmPauseMenuStart関数で開始したポーズメニュー処理が
	完了したかを判定します。\n
	GmPauseMenuStart関数呼び出し前はTRUEを返します。\n
*/
// ===========================================================================
BOOL GmPauseMenuIsFinished(void)
{
	using namespace gs::pause_menu;
	CMain &pm = CMain::CreateInstance();
	return pm.IsPlay();
}

// ===========================================================================
//	GmPauseMenuGetResult
/*!
	ポーズメニュー結果取得

	@note
	GmPauseMenuStart関数で開始したポーズメニューにおいて、
	ユーザが何を選択したかを返します。\n
	この関数はGmPauseMenuIsFinished関数がTRUEを返す状態で呼び出して下さい。\n
	それ以外の状態で呼び出すとアサートしGME_PMENU_RESULT_NONEを返します。\n
	GmPauseMenuStart関数を呼び出す前に呼び出した場合は
	GME_PMENU_RESULT_NONEを返します。\n
*/
// ===========================================================================
GME_PMENU_RESULT GmPauseMenuGetResult(void)
{
	using namespace gs::pause_menu;
	CMain &pm = CMain::CreateInstance();
	return GME_PMENU_RESULT(pm.GetResult());
}

#if defined(MTD_DEBUG)
// ***************************************************************************
// デバッグ
// ***************************************************************************
static void gmPmDgbEvTaskWaitStart(AMS_TCB* tcb);
static void gmPmDbgEvTaskWaitLoad(AMS_TCB* tcb);
static void gmPmDbgEvTaskWaitBuild(AMS_TCB* tcb);
static void gmPmDbgEvTaskWaitExecute(AMS_TCB* tcb);
static void gmPmDbgEvTaskExecute(AMS_TCB* tcb);
static void gmPmDbgEvTaskWaitFlush(AMS_TCB* tcb);
static void gmPmDbgEvTaskPre(AMS_TCB* tcb);
static void gmPmDbgEvTaskPost(AMS_TCB* tcb);
static void gmPmDbgEvTaskDraw(AMS_TCB* tcb);


// ===========================================================================
//! ポーズメニュー確認用イベント開始
// ===========================================================================
void GmPauseMenuDebugEventStart(void* arg)
{
	UNREFERENCED_PARAMETER(arg);
	using namespace gs::pause_menu::setting::main;

	// アクション設定
	AoActSysSetDrawStateEnable(TRUE);
	AoActSysSetDrawState(c_draw_state);

	// タスク作成
	AMS_TCB* tcb = amTaskMake(
		gmPmDgbEvTaskWaitStart, NULL, 0x1000, 0, 0, "gmPauseMenu::DebugEvent");

	// タスク開始
	amTaskStart(tcb);
}

// ===========================================================================
//! 開始待ち
// ===========================================================================
void gmPmDgbEvTaskWaitStart(AMS_TCB* tcb)
{
	amPrint(4, 4, "PLEASE PUSH KEY TO START.");
	amPrintf(4, 5, "%c:START", GsEnvDebugGetDecideKeyChar());
	amPrintf(4, 6, "%c:FINISH", GsEnvDebugGetCancelKeyChar());

	// 終了判定
	if (AoPadSomeoneStand(GSD_KEY_CANCEL) >= 0) {
		amTaskDelete(tcb);
		SyDecideEvt(GSD_EVT_ID_DEBUG_DEMO);
		SyChangeNextEvt();

		AoActSysSetDrawStateEnable();
		AoActSysSetDrawState();
		return;
	}

	// 開始判定
	else if (AoPadSomeoneStand(GSD_KEY_DECIDE) >= 0) {
		GmPauseMenuLoadStart();
		amTaskSetProcedure(tcb, gmPmDbgEvTaskWaitLoad);
	}
}

// ===========================================================================
//! ファイル読み込み待ち
// ===========================================================================
void gmPmDbgEvTaskWaitLoad(AMS_TCB* tcb)
{
	amPrint(4, 4, "NOW LOADING...");
	if (GmPauseMenuLoadIsFinished()) {
		GmPauseMenuBuildStart();
		amTaskSetProcedure(tcb, gmPmDbgEvTaskWaitBuild);
	}
}

// ===========================================================================
//! 構築待ち
// ===========================================================================
void gmPmDbgEvTaskWaitBuild(AMS_TCB* tcb)
{
	amPrint(4, 4, "NOW BUILDING...");
	if (GmPauseMenuBuildIsFinished()) {
		amTaskSetProcedure(tcb, gmPmDbgEvTaskWaitExecute);
	}
}

// ===========================================================================
//! 実行待ち
// ===========================================================================
void gmPmDbgEvTaskWaitExecute(AMS_TCB* tcb)
{
	amPrint(4, 4, "PLEASE PUSH KEY TO EXECUTE.");
	amPrintf(4, 5, "%c:EXECUTE", GsEnvDebugGetDecideKeyChar());
	amPrintf(4, 6, "%c:BACK", GsEnvDebugGetCancelKeyChar());
	amPrintf(4, 8, "PREV RESULT : %d", GmPauseMenuGetResult());

	// ワーク取得
	AMS_TCB** tcb_tbl = (AMS_TCB**)amTaskGetWork(tcb);

	// 解放判定
	if (AoPadSomeoneStand(GSD_KEY_CANCEL) >= 0) {
		GmPauseMenuFlushStart();
		amTaskSetProcedure(tcb, gmPmDbgEvTaskWaitFlush);
		return;
	}

	// 実行判定
	else if (AoPadSomeoneStand(GSD_KEY_DECIDE) >= 0) {
		tcb_tbl[0] = amTaskMake(gmPmDbgEvTaskPre, 0, 0x0000, 0, 0, "");
		tcb_tbl[1] = amTaskMake(gmPmDbgEvTaskPost, 0, 0xffff, 0, 0, "");
		GmPauseMenuStart(0x2000);
		amTaskSetProcedure(tcb, gmPmDbgEvTaskExecute);
	}
}

// ===========================================================================
//! 実行中
// ===========================================================================
void gmPmDbgEvTaskExecute(AMS_TCB* tcb)
{
	amPrint(4, 4, "EXECUTE.");

	// ワーク取得
	AMS_TCB** tcb_tbl = (AMS_TCB**)amTaskGetWork(tcb);

	// キャンセル判定
	if (AoPadSomeoneStand(KEY_R_UP) >= 0) {
		GmPauseMenuCancel();
	}

	// 終了判定
	if (GmPauseMenuIsFinished()) {
		amTaskDelete(tcb_tbl[0]);
		amTaskDelete(tcb_tbl[1]);
		amTaskSetProcedure(tcb, gmPmDbgEvTaskWaitExecute);
	}
}

// ===========================================================================
//! 解放待ち
// ===========================================================================
void gmPmDbgEvTaskWaitFlush(AMS_TCB* tcb)
{
	amPrint(4, 4, "NOW FLUSHING...");
	if (GmPauseMenuFlushIsFinished()) {
		GmPauseMenuRelease();
		amTaskSetProcedure(tcb, gmPmDgbEvTaskWaitStart);
	}
}

// ===========================================================================
//! 前処理
// ===========================================================================
void gmPmDbgEvTaskPre(AMS_TCB* tcb)
{
	UNREFERENCED_PARAMETER(tcb);

	// アクションソートバッファクリア
	AoActSortUnregAll();

	// アクションアキュムレートクリア
	AoActAcmInit();
}

// ===========================================================================
//! 後処理
// ===========================================================================
void gmPmDbgEvTaskPost(AMS_TCB* tcb)
{
	UNREFERENCED_PARAMETER(tcb);

	// アクションソート実行
	AoActSortExecute();

	// アクション描画
	AoActSortDraw();

	// アクションソートバッファクリア
	AoActSortUnregAll();

	// 描画タスク生成
	amDrawMakeTask(gmPmDbgEvTaskDraw, (u16)0x8000, (u32)0);
}

// ===========================================================================
//! 描画タスク
// ===========================================================================
void gmPmDbgEvTaskDraw(AMS_TCB* tcb)
{
	UNREFERENCED_PARAMETER(tcb);

	AoActDrawPre();
	amDrawExecCommand(gs::pause_menu::setting::main::c_draw_state);
	amDrawEndScene();
}


#endif // defined(MTD_DEBUG)



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
