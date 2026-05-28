// ============================================================================
/*!
	@file	dbgPadEmu.cpp
	@brief	パッドエミュレータ

	@author	Kouji Hokazono <kouji_hokazono@dimps.co.jp>
		Copyright(c) 2009 Dimps
	$Id: dbgPadEmu.cpp 2 2011-04-11 05:21:26Z thamada $
 */
// ============================================================================
/*
 * $Log$
 */

//------ Include ---------------------- インクルード ---------------------------******_IC*
#include "pch.h"
#include "dbgPadEmu.hpp"


#if _WII
#pragma warn_notinlined off	//インライン展開出来無い関数に対する警告メッセージの無効化
#endif //_WII


//------ Debug ------------------------ デバッグ -------------------------------******_DG*
#if defined(MTD_DEBUG)
#endif	//#if defined(MTD_DEBUG)
//------ Macro ------------------------ マクロ ---------------------------------******_MC*
//------ C Global Functions ----------- C グローバル関数の定義 -------------------******CGF*
//------ C Global Functions ----------- C グローバル関数の定義 -------------------******CGF*


namespace dbg {
//------ Class ------------------------ クラス ---------------------------------******_CL*
//■//-- Local Constant --------------- ローカル定数 ---------------------------******LCS*
//パッドトリガエリアテーブル
const CPadEmu::TTrgArea CPadEmu::c_trg_area_table[EMode::Max][ETrgPad::Max] = {
				{//タップモード
					{{-.0f/8.0f, -.0f/8.0f, -.0f/8.0f, -.0f/8.0f}},	//<方向キー・ドラッグ
					{{1.0f/8.0f, 1.0f/8.0f, 3.0f/8.0f, 7.0f/8.0f}},	//<方向キー・左
					{{3.0f/8.0f, 1.0f/8.0f, 5.0f/8.0f, 4.0f/8.0f}},	//<方向キー・上
					{{5.0f/8.0f, 1.0f/8.0f, 7.0f/8.0f, 7.0f/8.0f}},	//<方向キー・右
					{{3.0f/8.0f, 4.0f/8.0f, 5.0f/8.0f, 7.0f/8.0f}},	//<方向キー・下
					{{0.0f/8.0f, 1.0f/8.0f, 1.0f/8.0f, 7.0f/8.0f}},	//<左ボタン
					{{2.0f/8.0f, 0.0f/8.0f, 6.0f/8.0f, 1.0f/8.0f}},	//<上ボタン
					{{7.0f/8.0f, 1.0f/8.0f, 8.0f/8.0f, 7.0f/8.0f}},	//<右ボタン
					{{2.0f/8.0f, 7.0f/8.0f, 6.0f/8.0f, 8.0f/8.0f}},	//<下ボタン
					{{0.0f/8.0f, 0.0f/8.0f, 1.0f/8.0f, 1.0f/8.0f}},	//<L1ボタン
					{{7.0f/8.0f, 0.0f/8.0f, 8.0f/8.0f, 1.0f/8.0f}},	//<R1ボタン
					{{1.0f/8.0f, 0.0f/8.0f, 2.0f/8.0f, 1.0f/8.0f}},	//<L2ボタン
					{{6.0f/8.0f, 0.0f/8.0f, 7.0f/8.0f, 1.0f/8.0f}},	//<R2ボタン
					{{1.0f/8.0f, 7.0f/8.0f, 2.0f/8.0f, 8.0f/8.0f}},	//<Selectボタン
					{{6.0f/8.0f, 7.0f/8.0f, 7.0f/8.0f, 8.0f/8.0f}},	//<Startボタン
					{{7.0f/8.0f, 7.0f/8.0f, 8.0f/8.0f, 8.0f/8.0f}},	//<フリー1
					{{0.0f/8.0f, 7.0f/8.0f, 1.0f/8.0f, 8.0f/8.0f}},	//<フリー2
					{{-.0f/8.0f, -.0f/8.0f, -.0f/8.0f, -.0f/8.0f}},	//<フリー3
					{{-.0f/8.0f, -.0f/8.0f, -.0f/8.0f, -.0f/8.0f}},	//<フリー4
				},
				{//ドラッグモード
					{{1.0f/8.0f, 1.0f/8.0f, 7.0f/8.0f, 7.0f/8.0f}},	//<方向キー・ドラッグ
					{{-.0f/8.0f, -.0f/8.0f, -.0f/8.0f, -.0f/8.0f}},	//<方向キー・左
					{{-.0f/8.0f, -.0f/8.0f, -.0f/8.0f, -.0f/8.0f}},	//<方向キー・上
					{{-.0f/8.0f, -.0f/8.0f, -.0f/8.0f, -.0f/8.0f}},	//<方向キー・右
					{{-.0f/8.0f, -.0f/8.0f, -.0f/8.0f, -.0f/8.0f}},	//<方向キー・下
					{{0.0f/8.0f, 1.0f/8.0f, 1.0f/8.0f, 7.0f/8.0f}},	//<左ボタン
					{{2.0f/8.0f, 0.0f/8.0f, 6.0f/8.0f, 1.0f/8.0f}},	//<上ボタン
					{{7.0f/8.0f, 1.0f/8.0f, 8.0f/8.0f, 7.0f/8.0f}},	//<右ボタン
					{{2.0f/8.0f, 7.0f/8.0f, 6.0f/8.0f, 8.0f/8.0f}},	//<下ボタン
					{{0.0f/8.0f, 0.0f/8.0f, 1.0f/8.0f, 1.0f/8.0f}},	//<L1ボタン
					{{7.0f/8.0f, 0.0f/8.0f, 8.0f/8.0f, 1.0f/8.0f}},	//<R1ボタン
					{{1.0f/8.0f, 0.0f/8.0f, 2.0f/8.0f, 1.0f/8.0f}},	//<L2ボタン
					{{6.0f/8.0f, 0.0f/8.0f, 7.0f/8.0f, 1.0f/8.0f}},	//<R2ボタン
					{{1.0f/8.0f, 7.0f/8.0f, 2.0f/8.0f, 8.0f/8.0f}},	//<Selectボタン
					{{6.0f/8.0f, 7.0f/8.0f, 7.0f/8.0f, 8.0f/8.0f}},	//<Startボタン
					{{7.0f/8.0f, 7.0f/8.0f, 8.0f/8.0f, 8.0f/8.0f}},	//<フリー1
					{{0.0f/8.0f, 7.0f/8.0f, 1.0f/8.0f, 8.0f/8.0f}},	//<フリー2
					{{-.0f/8.0f, -.0f/8.0f, -.0f/8.0f, -.0f/8.0f}},	//<フリー3
					{{-.0f/8.0f, -.0f/8.0f, -.0f/8.0f, -.0f/8.0f}},	//<フリー4
	},
	
				{//ゲームモード
					{{-.0f/8.0f, -.0f/8.0f, -.0f/8.0f, -.0f/8.0f}},	//<方向キー・ドラッグ
					{{0.0f/8.0f, 1.0f/8.0f, 2.0f/8.0f, 7.0f/8.0f}},	//<方向キー・左
					{{2.0f/8.0f, 0.0f/8.0f, 6.0f/8.0f, 1.0f/8.0f}},	//<方向キー・上
					{{6.0f/8.0f, 1.0f/8.0f, 8.0f/8.0f, 7.0f/8.0f}},	//<方向キー・右
					{{2.0f/8.0f, 7.0f/8.0f, 6.0f/8.0f, 8.0f/8.0f}},	//<方向キー・下
					{{-.0f/8.0f, -.0f/8.0f, -.0f/8.0f, -.0f/8.0f}},	//<左ボタン
					{{6.0f/8.0f, 0.0f/8.0f, 8.0f/8.0f, 1.0f/8.0f}},	//<上ボタン
					{{-.0f/8.0f, -.0f/8.0f, -.0f/8.0f, -.0f/8.0f}},	//<右ボタン
					{{-.0f/8.0f, -.0f/8.0f, -.0f/8.0f, -.0f/8.0f}},	//<下ボタン
					{{-.0f/8.0f, -.0f/8.0f, -.0f/8.0f, -.0f/8.0f}},	//<L1ボタン
					{{-.0f/8.0f, -.0f/8.0f, -.0f/8.0f, -.0f/8.0f}},	//<R1ボタン
					{{0.0f/8.0f, 7.0f/8.0f, 2.0f/8.0f, 8.0f/8.0f}},	//<L2ボタン
					{{6.0f/8.0f, 7.0f/8.0f, 8.0f/8.0f, 8.0f/8.0f}},	//<R2ボタン
					{{0.0f/8.0f, 0.0f/8.0f, 2.0f/8.0f, 1.0f/8.0f}},	//<Selectボタン
					{{-.0f/8.0f, -.0f/8.0f, -.0f/8.0f, -.0f/8.0f}},	//<Startボタン
					{{-.0f/8.0f, -.0f/8.0f, -.0f/8.0f, -.0f/8.0f}},	//<フリー1
					{{-.0f/8.0f, -.0f/8.0f, -.0f/8.0f, -.0f/8.0f}},	//<フリー2
					{{-.0f/8.0f, -.0f/8.0f, -.0f/8.0f, -.0f/8.0f}},	//<フリー3
					{{-.0f/8.0f, -.0f/8.0f, -.0f/8.0f, -.0f/8.0f}},	//<フリー4
				},
				{//ダミーモード
					{{-.0f/8.0f, -.0f/8.0f, -.0f/8.0f, -.0f/8.0f}},	//<方向キー・ドラッグ
					{{-.0f/8.0f, -.0f/8.0f, -.0f/8.0f, -.0f/8.0f}},	//<方向キー・左
					{{-.0f/8.0f, -.0f/8.0f, -.0f/8.0f, -.0f/8.0f}},	//<方向キー・上
					{{-.0f/8.0f, -.0f/8.0f, -.0f/8.0f, -.0f/8.0f}},	//<方向キー・右
					{{-.0f/8.0f, -.0f/8.0f, -.0f/8.0f, -.0f/8.0f}},	//<方向キー・下
					{{-.0f/8.0f, -.0f/8.0f, -.0f/8.0f, -.0f/8.0f}},	//<左ボタン
					{{-.0f/8.0f, -.0f/8.0f, -.0f/8.0f, -.0f/8.0f}},	//<上ボタン
					{{-.0f/8.0f, -.0f/8.0f, -.0f/8.0f, -.0f/8.0f}},	//<右ボタン
					{{-.0f/8.0f, -.0f/8.0f, -.0f/8.0f, -.0f/8.0f}},	//<下ボタン
					{{-.0f/8.0f, -.0f/8.0f, -.0f/8.0f, -.0f/8.0f}},	//<L1ボタン
					{{-.0f/8.0f, -.0f/8.0f, -.0f/8.0f, -.0f/8.0f}},	//<R1ボタン
					{{-.0f/8.0f, -.0f/8.0f, -.0f/8.0f, -.0f/8.0f}},	//<L2ボタン
					{{-.0f/8.0f, -.0f/8.0f, -.0f/8.0f, -.0f/8.0f}},	//<R2ボタン
					{{-.0f/8.0f, -.0f/8.0f, -.0f/8.0f, -.0f/8.0f}},	//<Selectボタン
					{{-.0f/8.0f, -.0f/8.0f, -.0f/8.0f, -.0f/8.0f}},	//<Startボタン
					{{7.0f/8.0f, 7.0f/8.0f, 8.0f/8.0f, 8.0f/8.0f}},	//<フリー1
					{{0.0f/8.0f, 7.0f/8.0f, 1.0f/8.0f, 8.0f/8.0f}},	//<フリー2
					{{7.0f/8.0f, 0.0f/8.0f, 8.0f/8.0f, 1.0f/8.0f}},	//<フリー3
					{{0.0f/8.0f, 0.0f/8.0f, 1.0f/8.0f, 1.0f/8.0f}},	//<フリー4
				},
			};

//パッドトリガ→トリガ変換テーブル
const CPadEmu::TPadInfo CPadEmu::c_trg2pad_table[ETrgPad::Max] = {
				0,				//<方向キー・ドラッグ
				KEY_L_LEFT,		//<方向キー・左
				KEY_L_UP,		//<方向キー・上
				KEY_L_RIGHT,	//<方向キー・右
				KEY_L_DOWN,		//<方向キー・下
				KEY_R_LEFT,		//<左ボタン
				KEY_R_UP,		//<上ボタン
				KEY_R_RIGHT,	//<右ボタン
				KEY_R_DOWN,		//<下ボタン
				KEY_L1,			//<L1ボタン
				KEY_R1,			//<R1ボタン
				KEY_L2,			//<L2ボタン
				KEY_R2,			//<R2ボタン
				KEY_SELECT,		//<Selectボタン
				KEY_START,		//<Startボタン
				0,				//<フリー1
				0,				//<フリー2
				0,				//<フリー3
				0,				//<フリー4
			};

CPadEmu *CPadEmu::p_instance = NULL;	//<共通インスタンス
CPadEmu CPadEmu::p_instance_data;		//<共通インスタンスデータ


//■//-- Public Constant -------------- 公開定数 -------------------------------******PCS*
//■//-- Public Function -------------- 公開関数 -------------------------------******PFC*
// =============================================================================
// CPadEmu::CreateInstance
/*!
	インスタンス構築

	@return インスタンス
 */
// ==========================================================================
CPadEmu &CPadEmu::CreateInstance()
{
	static CPadEmu pad_emu;
	return pad_emu;
}

// =============================================================================
// CPadEmu::Create
/*!
	構築

	@param	config	[in]	コンフィグデータ
	@param	size	[in]	コンフィグデータサイズ
	@param	mode	[in]	設定モード

	@retval	true	成功
	@retval	false	失敗

	@note
		失敗しません。
 */
// ==========================================================================
bool CPadEmu::Create(const accel::CArray<Float32, 4> *config, Uint32 size)
{
	Release();

	//トリガ構築
	size = (std::min<Uint32>)(size, TTrg::static_size);
	for (TTrg::size_type i = 0; i < TTrg::static_size; ++i) {
		const Float32 width = 480.0f;
		const Float32 height = 320.0f;
		const TTrgArea &tbl = config[i];
		accel::CArray<Sint32, 4> rect;
		rect.left()   = static_cast<Sint32>(tbl.left() * width);
		rect.top()    = static_cast<Sint32>(tbl.top() * height);
		rect.right()  = static_cast<Sint32>(tbl.right() * width);
		rect.bottom() = static_cast<Sint32>(tbl.bottom() * height);
		m_trg[i].Create(rect);
	}
	m_trg[ETrgPad::LeftDrag].SetMoveThreshold(30);

	//プロシージャ設定
	SetProc(&CPadEmu::Update);

	m_flag[BFlag::Setup] = true;
	return true;
}
bool CPadEmu::Create(EMode::Type mode)
{
	return Create(c_trg_area_table[mode]);
}

// =============================================================================
// CPadEmu::Release
/*!
	破棄
 */
// ==========================================================================
void CPadEmu::Release()
{
	if (m_flag[BFlag::Setup]) {
		//プロシージャ削除
		SetProc();

		//トリガ破棄
		for (TTrg::iterator trg = m_trg.begin(), trg_end = m_trg.end(); trg != trg_end; ++trg) {
			trg->Release();
		}

		m_flag[BFlag::Setup] = false;
	}
}

// =============================================================================
// IIsValid::IsValid
/*!
	有効確認

	@retval	true	有効
	@retval	false	無効
 */
// ==========================================================================
bool CPadEmu::IsValid() const
{
	return m_flag[BFlag::Setup];
}

// =============================================================================
// CPadEmu::Update
/*!
	描画
 */
// ==========================================================================
void CPadEmu::Update()
{
	if (m_flag.test(BFlag::Setup)) {
		for (TTrg::iterator trg = m_trg.begin(), trg_end = m_trg.end(); trg != trg_end; ++trg) {
			trg->Update();
		}
	}
}

// =============================================================================
// CPadEmu::operator[]
/*!
	インデクサ
 */
// ==========================================================================
const er::CTrgState &CPadEmu::operator[](ETrgPad::Type pad) const
{
	return m_trg[pad].GetState();
}


//■//-- Get Function ----------------- 取得関数 -------------------------------******PGF*
// ============================================================================
// CPadEmu::GetPadDirect
/*!
	パッドの直値の取得

	@return	パッド情報
 */
// ============================================================================
CPadEmu::TPadInfo CPadEmu::GetPadDirect() const
{
	TPadInfo result = TPadInfo(0);

	//ボタントリガ
	for (TTrg::size_type i = ETrgPad::Type(0); i < TTrg::static_size; ++i) {
		if (!(result & c_trg2pad_table[i])) {
			//パッドが有効で無いなら
			const TTrg::value_type &trg = m_trg[i];
			if (trg.GetState()[er::CTrgState::EState::On]) {
				result |= c_trg2pad_table[i];
			}
		}
	}

	//方向トリガ
	if (m_trg[ETrgPad::LeftDrag].GetState()[er::CTrgState::EState::Move]) {
		const er::CTrgState &state = m_trg[ETrgPad::LeftDrag].GetState();
		er::CTrgState::TMove move = state.GetLastMove();
		er::CTrgState::TMoveThreshold thrsld = state.GetMoveThreshold();

		if (move.x() < -thrsld) {
			result |= KEY_L_LEFT;
		} else if (thrsld < move.x()) {
			result |= KEY_L_RIGHT;
		}
		if (move.y() < -thrsld) {
			result |= KEY_L_UP;
		} else if (thrsld < move.y()) {
			result |= KEY_L_DOWN;
		}
	}

	return result;
}

// ============================================================================
// CPadEmu::GetPadStand
/*!
	パッドのONエッジの取得

	@return	パッド情報
 */
// ============================================================================
CPadEmu::TPadInfo CPadEmu::GetPadStand() const
{
	TPadInfo result = TPadInfo(0);

	//ボタントリガ
	for (TTrg::size_type i = ETrgPad::Type(0); i < TTrg::static_size; ++i) {
		if (!(result & c_trg2pad_table[i])) {
			//パッドが有効で無いなら
			const TTrg::value_type &trg = m_trg[i];
			if (trg.GetState()[er::CTrgState::EState::Down]) {
				result |= c_trg2pad_table[i];
			}
		}
	}

	//方向トリガ
	if (m_trg[ETrgPad::LeftDrag].GetState()[er::CTrgState::EState::Move]) {
		const er::CTrgState &state = m_trg[ETrgPad::LeftDrag].GetState();
		er::CTrgState::TMove move = state.GetLastMove();
		er::CTrgState::TMoveThreshold thrsld = state.GetMoveThreshold();

		if (move.x() < -thrsld) {
			result |= KEY_L_LEFT;
		} else if (thrsld < move.x()) {
			result |= KEY_L_RIGHT;
		}
		if (move.y() < -thrsld) {
			result |= KEY_L_UP;
		} else if (thrsld < move.y()) {
			result |= KEY_L_DOWN;
		}
	}
	
	return result;
}

// ============================================================================
// CPadEmu::GetPadRelease
/*!
	パッドのOFFエッジの取得

	@return	パッド情報
 */
// ============================================================================
CPadEmu::TPadInfo CPadEmu::GetPadRelease() const
{
	TPadInfo result = TPadInfo(0);

	//ボタントリガ
	for (TTrg::size_type i = ETrgPad::Type(0); i < TTrg::static_size; ++i) {
		if (!(result & c_trg2pad_table[i])) {
			//パッドが有効で無いなら
			const TTrg::value_type &trg = m_trg[i];
			if (trg.GetState()[er::CTrgState::EState::Up]) {
				result |= c_trg2pad_table[i];
			}
		}
	}

	//方向トリガ
	if (m_trg[ETrgPad::LeftDrag].GetState()[er::CTrgState::EState::Move]) {
		const er::CTrgState &state = m_trg[ETrgPad::LeftDrag].GetState();
		er::CTrgState::TMove move = state.GetLastMove();
		er::CTrgState::TMoveThreshold thrsld = state.GetMoveThreshold();

		if (move.x() < -thrsld) {
			result |= KEY_L_LEFT;
		} else if (thrsld < move.x()) {
			result |= KEY_L_RIGHT;
		}
		if (move.y() < -thrsld) {
			result |= KEY_L_UP;
		} else if (thrsld < move.y()) {
			result |= KEY_L_DOWN;
		}
	}
	
	return result;
}

// ============================================================================
// CPadEmu::GetPadRepeat
/*!
	パッドのリピートの取得

	@return	パッド情報
 */
// ============================================================================
CPadEmu::TPadInfo CPadEmu::GetPadRepeat() const
{
	TPadInfo result = TPadInfo(0);

	//ボタントリガ
	for (TTrg::size_type i = ETrgPad::Type(0); i < TTrg::static_size; ++i) {
		if (!(result & c_trg2pad_table[i])) {
			//パッドが有効で無いなら
			const TTrg::value_type &trg = m_trg[i];
			if (trg.GetState()[er::CTrgState::EState::Lock] && trg.GetState()[er::CTrgState::EState::Repeat]) {
				result |= c_trg2pad_table[i];
			}
		}
	}

	//方向トリガ
	if (m_trg[ETrgPad::LeftDrag].GetState()[er::CTrgState::EState::Move]) {
		const er::CTrgState &state = m_trg[ETrgPad::LeftDrag].GetState();
		er::CTrgState::TMove move = state.GetLastMove();
		er::CTrgState::TMoveThreshold thrsld = state.GetMoveThreshold();

		if (move.x() < -thrsld) {
			result |= KEY_L_LEFT;
		} else if (thrsld < move.x()) {
			result |= KEY_L_RIGHT;
		}
		if (move.y() <  -thrsld) {
			result |= KEY_L_UP;
		} else if (thrsld < move.y()) {
			result |= KEY_L_DOWN;
		}
	}
	
	return result;
}


//■//-- Set Function ----------------- 設定関数 -------------------------------******PSF*
//■//-- Constructor And Destructor --- コンストラクタ・デストラクタ------------******_CD*
//■//-- Local Function --------------- ローカル関数 ---------------------------******LFC*
//------------------------------------------------------------------------------**********



























































































} //namespace dbg

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
