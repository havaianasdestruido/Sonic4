// ============================================================================
/*!
	@file	dmTitle_i.cpp
	@brief	タイトル(iPhone)

	@author	Kouji Hokazono <kouji_hokazono@dimps.co.jp>
		Copyright(c) 2009 Dimps
	$Id: dmBuyScreen_i.cpp 2 2011-04-11 05:21:26Z thamada $
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

#include "dmBuyScreen.h"
#include "dmSound.h"
#include "gs.h"
#include "gsEnvironment.h"
extern BOOL _am_sample_draw_enable;

#include "erWeb.hpp"
#include "erTask.hpp"
#include "erTrgAoAction.hpp"

#include "accelArray.hpp"
#include "accelBitset.hpp"

//リソース系
#include "arc/D_BUY_SCREEN.HMB"
	#include "ace/D_BUY_SCREEN.HMA"
#include "arc/D_BUY_SCREEN_JP.HMB"
	#include "ace/D_BUY_SCREEN_JP.HMA"


//------ Debug ------------------------ デバッグ -------------------------------******_DG*
#if defined(MTD_DEBUG)
#endif	//#if defined(MTD_DEBUG)
//------ Macro ------------------------ マクロ ---------------------------------******_MC*






















namespace dm {
namespace buyscreen {


namespace {


//------------------------------------------------------------------------------**********
namespace setting {
	//ファイルパス
	namespace file {
		const char *c_global	= GSS_BASE_PATH"DEMO/BUY_SCREEN/D_BUY_SCREEN.AMB";		//各国共通ファイル
		const char *c_lang[]	= {	GSS_BASE_PATH"DEMO/BUY_SCREEN/D_BUY_SCREEN_JP.AMB"	//国別ファイル
								,	GSS_BASE_PATH"DEMO/BUY_SCREEN/D_BUY_SCREEN_US.AMB"
								,	GSS_BASE_PATH"DEMO/BUY_SCREEN/D_BUY_SCREEN_FR.AMB"
								,	GSS_BASE_PATH"DEMO/BUY_SCREEN/D_BUY_SCREEN_IT.AMB"
								,	GSS_BASE_PATH"DEMO/BUY_SCREEN/D_BUY_SCREEN_GE.AMB"
								,	GSS_BASE_PATH"DEMO/BUY_SCREEN/D_BUY_SCREEN_SP.AMB"
								};
	} //namespace file
	//メイン
	namespace main {
		namespace task {
			const u32	c_priority	= 0x2000;	//<タスク優先度
			const u32	c_user		= 0;		//<タスク所有者
			const u32	c_attribute	= 0;		//<タスク属性
		} //namespace task
		namespace fadein {
			const IZE_FADE_SET_TYPE	c_type	= IZE_FADE_SET_TYPE_NORMAL;		//<フェード継続タイプ
			const IZE_FADE_TYPE		c_inout	= IZE_FADE_TYPE_BLACK_FADEIN;	//<フェード種類タイプ
			const float				c_frame	= 16.0f;						//<フェードフレーム数
		} //namespace fadein
		namespace fadeout {
			const IZE_FADE_SET_TYPE	c_type	= IZE_FADE_SET_TYPE_NORMAL;		//<フェード継続タイプ
			const IZE_FADE_TYPE		c_inout	= IZE_FADE_TYPE_BLACK_FADEOUT;	//<フェード種類タイプ
			const float				c_frame	= 16.0f;						//<フェードフレーム数
		} //namespace fadeout
		namespace enter_effect {
			const u32	c_frame	= 30;							//<決定演出フレーム数
		} //namespace enter_effect
		namespace web {
			const char	*c_url	= "http://sega.com/apps";		//<購入サイトURL
		} //namespace web
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


	//-- Public Constant -------------- 公開定数 -------------------------------******PCS*
public:
	//-- Public Variable -------------- 公開変数 -------------------------------******PVA*
	//-- Public Function -------------- 公開関数 -------------------------------******PFC*
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
	void Start(DME_BUY_SCR_RESULT *result) {
		amAssert(result);
		m_result = result;
		fadeInStart();
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

			Max,
			None,
		};
		typedef accel::CBitset<Max> Type;
	};

	//メモリ開放が必要なファイル
	struct EMemFile {
		enum Type {
			Global,
			Lang,

			Max,
			None
		};
	};

	//ファイル
	struct EFile : public EMemFile {
		typedef int Type;
		enum {
			GlobalAma = EMemFile::Max,
			GlobalAmb,
			LangAma,
			LangAmb,

			Max,
			None
		};
	};

	//テクスチャ
	struct ETex {
		enum Type {
			Global,
			Lang,

			Max,
			None
		};
	};

	//アクション
	struct EAct {
		enum Type {
			Bgi,
			BuyLeft,
			BuyCenter,
			BuyRight,
			CancelLeft,
			CancelCenter,
			CancelRight,
			Buy,
			Cancel,

			Max,
			None,
		};
	};

	//トリガ
	struct ETrg {
		enum Type {
			Buy,
			Cancel,

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
				SortDraw,
				 
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
			if (flag[SAction::BFlag::NoDraw]) {
				//何もしない
			} else if (flag[SAction::BFlag::SortDraw]) {
				AoActSortRegAction(act);
			} else {
				AoActDraw(act);
			}
		}
	};

	//ボタン→結果変換テーブル
	static const DME_BUY_SCR_RESULT c_return_table[ETrg::Max];



	//-- Local Variable --------------- ローカル変数 ---------------------------******LVA*
protected:
private:
	BFlag::Type				m_flag;					//<フラグ
	DME_BUY_SCR_RESULT		*m_result;				//<リザルト

	AMS_FS					*m_fs[EMemFile::Max];	//<ファイル読み込みリクエスト
	void					*m_file[EFile::Max];	//<ファイル
	AOS_TEXTURE				m_tex[ETex::Max];		//<テクスチャ
	SAction					m_act[EAct::Max];		//<アクション
	er::CTrgAoAction		m_trg[ETrg::Max];		//<トリガ


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
	void fadeInStart();
	void fadeIn();
	void waitStart();
	void wait();
	void selectStart();
	void select();
	void enterEfctStart();
	void enterEfct();
	void fadeOutStart();
	void fadeOut();
	void releasingStart();
	void releasing();
	
	static DME_BUY_SCR_RESULT TrgIdxToReturnIdx(int trg_idx);



//------------------------------------------------------------------------------**********
}; //class CMain




//------ Class ------------------------ クラス ---------------------------------******_CL*
//■//-- Local Constant --------------- ローカル定数 ---------------------------******LCS*
//ボタン→戻り値変換テーブル
const DME_BUY_SCR_RESULT CMain::c_return_table[ETrg::Max] = {
								DMD_BUY_SCR_RESULT_BUY
							,	DMD_BUY_SCR_RESULT_CANCEL
							};


//■//-- Public Constant -------------- 公開定数 -------------------------------******PCS*
//■//-- Public Function -------------- 公開関数 -------------------------------******PFC*
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
	m_flag[BFlag::Create] = true;
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
		for (er::CTrgAoAction *trg = m_trg, *trg_end = m_trg + ETrg::Max; trg != trg_end; ++trg) {
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
		for (SAction *act = m_act, *act_end = m_act + EAct::Max; act != act_end; ++act) {
			act->Update();
		}
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
			for (const SAction *act = m_act, *act_end = m_act + EAct::Max; act != act_end; ++act) {
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
	m_fs[EMemFile::Global] = amFsReadBackground(const_cast<char *>(c_global));
	//国別
	GSE_LANGUAGE lang = GsEnvGetLanguage();
	m_fs[EMemFile::Lang] = amFsReadBackground(const_cast<char *>(c_lang[lang]));

	m_flag[BFlag::LoadFile] = true;

	//タスク設定
	using namespace setting::main::task;
	AttachTask("dmBuyScreen::Load", c_priority, c_user, c_attribute);
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
		//ファイル展開
		struct SLocalUnfoldTable {
			EMemFile::Type	file;
			u32				index;
		};
		const SLocalUnfoldTable c_local_unfold_table[EFile::Max] = {
			{EMemFile::None,	0}									//EMemFile::Global
		,	{EMemFile::None,	0}									//EMemFile::Lang
		,	{EMemFile::Global,	IDB_D_BUY_SCREEN_D_COMP_AMA}		//EFile::GlobalAma
		,	{EMemFile::Global,	IDB_D_BUY_SCREEN_D_COMP_AMB}		//EFile::GlobalAmb
		,	{EMemFile::Lang,	IDB_D_BUY_SCREEN_JP_D_COMP_JP_AMA}	//EFile::LangAma
		,	{EMemFile::Lang,	IDB_D_BUY_SCREEN_JP_D_COMP_JP_AMB}	//EFile::LangAmb
		};
		for (unsigned int i = 0; i < arrayof(c_local_unfold_table); ++i) {
			const SLocalUnfoldTable &unfold = c_local_unfold_table[i];
			if (unfold.file < EMemFile::Max) {
				m_file[i] = amBindGet(reinterpret_cast<AMS_AMB_HEADER *>(m_file[unfold.file]), unfold.index);
			}
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
	const EFile::Type c_local_create_table[ETex::Max] = {
		EFile::GlobalAmb,	//ETex::Global
		EFile::LangAmb,		//ETex::Lang
	};
	for (unsigned int i = 0; i < arrayof(c_local_create_table); ++i) {
		const EFile::Type &create = c_local_create_table[i];
		AoTexBuild(&m_tex[i], m_file[create]);
		AoTexLoad(&m_tex[i]);
	}

	m_flag[BFlag::CreateTexture] = true;

	//タスク設定
	using namespace setting::main::task;
	AttachTask("dmBuyScreen::Build", c_priority, c_user, c_attribute);
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
	for (AOS_TEXTURE *tex = m_tex, *tex_end = m_tex + ETex::Max; tex != tex_end; ++tex) {
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
void CMain::fadeInStart()
{
	using namespace setting::main;

	//アクション
	struct SLocalCreateActionTable {
		EFile::Type	file;
		ETex::Type	tex;
		Sint32		idx;
	};
	const SLocalCreateActionTable c_local_create_action_table[EAct::Max] = {
		{EFile::LangAma		, ETex::Lang	, IDA_D_COMP_JP_ACT_BG			},	//Bgi
		{EFile::GlobalAma	, ETex::Global	, IDA_D_COMP_ACT_BTN_BUY_L		},	//BuyLeft
		{EFile::GlobalAma	, ETex::Global	, IDA_D_COMP_ACT_BTN_BUY_C		},	//BuyCenter
		{EFile::GlobalAma	, ETex::Global	, IDA_D_COMP_ACT_BTN_BUY_R		},	//BuyRight
		{EFile::GlobalAma	, ETex::Global	, IDA_D_COMP_ACT_BTN_RETURN_L	},	//CancelLeft
		{EFile::GlobalAma	, ETex::Global	, IDA_D_COMP_ACT_BTN_RETURN_C	},	//CancelCenter
		{EFile::GlobalAma	, ETex::Global	, IDA_D_COMP_ACT_BTN_RETURN_R	},	//CancelRight
		{EFile::LangAma		, ETex::Lang	, IDA_D_COMP_JP_ACT_TEX_BUY		},	//Buy
		{EFile::LangAma		, ETex::Lang	, IDA_D_COMP_JP_ACT_TEX_RETURN	},	//Cancel
	};
	for (unsigned int i = 0; i < arrayof(c_local_create_action_table); ++i) {
		const SLocalCreateActionTable &create = c_local_create_action_table[i];
		const void *ama = m_file[create.file];
		SAction &act = m_act[i];
		act.act = AoActCreate(ama, create.idx);
		act.tex = &m_tex[create.tex];
		act.flag[SAction::BFlag::NoUpdate] = true;
		act.AcmInit();
	}

	//トリガ
	const EAct::Type c_local_create_trg_table[ETrg::Max] = {
		EAct::BuyCenter,	//ETrg::Buy
		EAct::CancelCenter,	//ETrg::Cancel
	};
	for (unsigned int i = 0; i < arrayof(c_local_create_trg_table); ++i) {
		const SAction &act  = m_act[c_local_create_trg_table[i]];
		er::CTrgAoAction &trg = m_trg[i];
		trg.Create(act.act);
	}
	
	//フェード開始
	using namespace setting::main::fadein;
	IzFadeInitEasy(c_type, c_inout, c_frame);

	m_flag[BFlag::Start] = true;

	//タスク設定
	using namespace setting::main::task;
	AttachTask("dmBuyScreen::Execute", c_priority, c_user, c_attribute);
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
	if (IzFadeIsEnd()) {
		IzFadeExit();
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
	const EAct::Type c_btn_action_table[ETrg::Max][3] = {
		{EAct::BuyLeft,		EAct::BuyCenter,	EAct::BuyRight},	//<ETrg::Buy
		{EAct::CancelLeft,	EAct::CancelCenter,	EAct::CancelRight},	//<ETrg::Cancel
	};
	typedef er::CTrgState::EState EState;

	int crnt_trg = -1;
	for (int i = 0; i < ETrg::Max; ++i) {
		er::CTrgAoAction &trg = m_trg[i];
		f32 frame;
		if (trg.GetState(0)[EState::Up] && trg.GetState(0)[EState::Prev]) {
			//決定
			frame = 1.0f;
			crnt_trg = i;
		} else if (trg.GetState(0)[EState::On]) {
			//ON
			frame = 2.0f;
		} else {
			//OFF
			frame = 0.0f;
		}

		for (unsigned int k = 0; k < arrayof(c_btn_action_table[i]); ++k) {
			const EAct::Type *btn_action = c_btn_action_table[i];
			AoActSetFrame(m_act[btn_action[k]].act, frame);
		}
	}

	//決定・キャンセル確認
	if (-1 != crnt_trg) {
		//決定なら
		//決定ボタンのみ決定アニメーション
		const EAct::Type *btn_action = c_btn_action_table[crnt_trg];
		for (SAction *act = m_act + btn_action[0], *act_end = m_act + (btn_action[2]+1); act != act_end; ++act) {
			act->flag[SAction::BFlag::NoUpdate] = false;
		}
		//SE再生
		DmSoundPlaySE("Ok");
		//戻り値算出
		*m_result = TrgIdxToReturnIdx(crnt_trg);
		
		//遷移
		enterEfctStart();
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
	using namespace setting::main::enter_effect;
	if (c_frame < GetCount()) {
		if (DMD_BUY_SCR_RESULT_BUY == *m_result) {
			//購入なら
			using namespace setting::main::web;
			er::web::StartWeb(c_url);
		} else {
			//購入しないなら
			fadeOutStart();
		}
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
	using namespace setting::main::fadeout;
	IzFadeInitEasy(c_type, c_inout, c_frame);

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
	if (IzFadeIsEnd()) {
		//破棄
		for (SAction *act = m_act, *act_end = m_act + EAct::Max; act != act_end; ++act) {
			AoActDelete(act->act);
		}

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
	for (AOS_TEXTURE *tex = m_tex, *tex_end = m_tex + ETex::Max; tex != tex_end; ++tex) {
		AoTexRelease(&*tex);
	}

	//タスク設定
	using namespace setting::main::task;
	AttachTask("dmBuyScreen::Flush", c_priority, c_user, c_attribute);
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
	for (AOS_TEXTURE *tex = m_tex, *tex_end = m_tex + ETex::Max; tex != tex_end; ++tex) {
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
// CMain::TrgIdxToReturnIdx
/*!
	トリガインデックス→戻るインデックス変換
 */
// ==========================================================================
DME_BUY_SCR_RESULT CMain::TrgIdxToReturnIdx(int trg_idx)
{
	DME_BUY_SCR_RESULT result = c_return_table[trg_idx];
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












} //namespace buyscreen
} //namespace dm


//------ C Global Functions ----------- C グローバル関数の定義 -------------------******CGF*

// ===========================================================================
//! 製品版（完全版）購入画面 ワーク初期化
// ===========================================================================
void DmBuyScreenInit(DMS_BUY_SCR_WORK *work)
{
	amAssert(work);
	amZeroMemory(work, sizeof(DMS_BUY_SCR_WORK));
}

// ===========================================================================
//! 製品版（完全版）購入画面 ファイル読み込み開始
// ===========================================================================
void DmBuyScreenLoadStart(DMS_BUY_SCR_WORK *work)
{
	amAssert(work);
	using namespace dm::buyscreen;
	CMain *bs = reinterpret_cast<CMain *>(work->instance);
	delete bs;
	bs = new CMain();
	bs->Create();
	bs->LoadFile();
	work->instance = reinterpret_cast<void *>(bs);
}

// ===========================================================================
//! 製品版（完全版）購入画面 ファイル読み込み完了判定
// ===========================================================================
BOOL DmBuyScreenLoadIsFinished(const DMS_BUY_SCR_WORK *work)
{
	amAssert(work);
	amAssert(work->instance);
	using namespace dm::buyscreen;
	CMain &bs = *reinterpret_cast<CMain *>(work->instance);
	return ((bs.IsLoadFile())? TRUE: FALSE);
}

// ===========================================================================
//! 製品版（完全版）購入画面 テクスチャ構築開始
// ===========================================================================
void DmBuyScreenBuildStart(DMS_BUY_SCR_WORK *work)
{
	amAssert(work);
	amAssert(work->instance);
	using namespace dm::buyscreen;
	CMain &bs = *reinterpret_cast<CMain *>(work->instance);
	bs.CreateTexture();
}

// ===========================================================================
//! 製品版（完全版）購入画面 テクスチャ構築完了判定
// ===========================================================================
BOOL DmBuyScreenBuildIsFinished(const DMS_BUY_SCR_WORK *work)
{
	amAssert(work);
	amAssert(work->instance);
	using namespace dm::buyscreen;
	CMain &bs = *reinterpret_cast<CMain *>(work->instance);
	return ((bs.IsCreatedTexture())? TRUE: FALSE);
}

// ===========================================================================
//! 製品版（完全版）購入画面 開始
// ===========================================================================
void DmBuyScreenStart(DMS_BUY_SCR_WORK *work, BOOL is_ui_show, BOOL is_save)
{
	amAssert(work);
	amAssert(work->instance);
	using namespace dm::buyscreen;
	CMain &bs = *reinterpret_cast<CMain *>(work->instance);
	bs.Start(&work->result);
}

// ===========================================================================
//! 製品版（完全版）購入画面 完了判定
// ===========================================================================
BOOL DmBuyScreenIsFinished(const DMS_BUY_SCR_WORK *work)
{
	amAssert(work);
	amAssert(work->instance);
	using namespace dm::buyscreen;
	CMain &bs = *reinterpret_cast<CMain *>(work->instance);
	return ((bs.IsPlay())? TRUE: FALSE);
}

// ===========================================================================
//! 製品版（完全版）購入画面 結果取得
// ===========================================================================
DME_BUY_SCR_RESULT DmBuyScreenGetResult(const DMS_BUY_SCR_WORK *work)
{
	amAssert(work);
	return work->result;
}

// ===========================================================================
//! 製品版（完全版）購入画面 テクスチャ解放開始
// ===========================================================================
void DmBuyScreenFlushStart(DMS_BUY_SCR_WORK *work)
{
	amAssert(work);
	amAssert(work->instance);
	using namespace dm::buyscreen;
	CMain &bs = *reinterpret_cast<CMain *>(work->instance);
	bs.ReleaseTexture();
}

// ===========================================================================
//! 製品版（完全版）購入画面 テクスチャ解放完了判定
// ===========================================================================
BOOL DmBuyScreenFlushIsFinished(const DMS_BUY_SCR_WORK *work)
{
	amAssert(work);
	amAssert(work->instance);
	using namespace dm::buyscreen;
	CMain &bs = *reinterpret_cast<CMain *>(work->instance);
	return ((bs.IsReleasedTexture())? TRUE: FALSE);
}

// ===========================================================================
//! 製品版（完全版）購入画面 ファイル解放
// ===========================================================================
void DmBuyScreenRelease(DMS_BUY_SCR_WORK *work)
{
	amAssert(work);
	if (work->instance) {
		using namespace dm::buyscreen;
		CMain *bs = reinterpret_cast<CMain *>(work->instance);
		bs->Release();
		delete bs;
		work->instance = NULL;
	}
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
