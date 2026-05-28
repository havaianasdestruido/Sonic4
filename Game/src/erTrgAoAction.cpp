// ============================================================================
/*!
	@file	erTrgAoAction.cpp
	@brief	タッチパネルアクション(トリガ・AoAction拡張)

	@author	Kouji Hokazono <kouji_hokazono@dimps.co.jp>
		Copyright(c) 2009-2010 Dimps
	$Id: erTrgAoAction.cpp 2 2011-04-11 05:21:26Z thamada $
 */
// ============================================================================
/*
 * $Log$
 */

//------ Include ---------------------- インクルード ---------------------------******_IC*
#include "pch.h"
#include "erTrgAoAction.hpp"



//------ Debug ------------------------ デバッグ -------------------------------******_DG*
#if defined(MTD_DEBUG)
#endif	//#if defined(MTD_DEBUG)
//------ Macro ------------------------ マクロ ---------------------------------******_MC*
//------ C Global Functions ----------- C グローバル関数の定義 -------------------******CGF*
//------ C Global Functions ----------- C グローバル関数の定義 -------------------******CGF*


namespace er {
//------ Class ------------------------ クラス ---------------------------------******_CL*
//■//-- Local Constant --------------- ローカル定数 ---------------------------******LCS*
//■//-- Public Constant -------------- 公開定数 -------------------------------******PCS*
//■//-- Public Function -------------- 公開関数 -------------------------------******PFC*
// =============================================================================
// CTrgAoAction::Create
/*!
	作成

	@param	act	[in]	使用するアクション

	@retval	true	成功
	@retval	false	失敗

	@note
		NULLが入力されると失敗します。
 */
// ==========================================================================
bool CTrgAoAction::Create(const AOS_ACTION *act)
{
	Release();

	m_act = act;

	return create();
}

// =============================================================================
// CTrgAoAction::Release
/*!
	破棄
 */
// ==========================================================================
void CTrgAoAction::Release()
{
	if (m_flag.test(BFlag::Setup)) {
		m_flag.set(BFlag::Setup, false);
	}
}

// =============================================================================
// CTrgAoAction::IsValid
/*!
	有効確認

	@retval	true	有効
	@retval	false	無効
 */
// ==========================================================================
bool CTrgAoAction::IsValid() const
{
	return m_flag.test(BFlag::Setup);
}


//■//-- Get Function ----------------- 取得関数 -------------------------------******PGF*
//■//-- Set Function ----------------- 設定関数 -------------------------------******PSF*
//■//-- Local Function --------------- ローカル関数 ---------------------------******LFC*
// =============================================================================
// CTrgAoAction::create
/*!
	生成
 */
// ==========================================================================
bool CTrgAoAction::create()
{
	bool result;
	if (NULL == m_act) {
		//アクションが無効なら
		result = false;
	} else {
		//アクションが有効なら
		result = true;

		//初期化
		ResetState();
		SetRepeatInterval();
		SetDoubleClickTime();
		SetMoveThreshold();

		m_flag.set(BFlag::Setup, true);
	}

	return result;
}

// =============================================================================
// CTrgAoAction::hitTest
/*!
	ヒットテスト

	@param	pos		[in]	タッチ位置
	@param	index	[in]	タッチインデックス

	@retval	true	ヒット
	@retval	false	未ヒット
 */
// ==========================================================================
bool CTrgAoAction::hitTest(const TPos &pos, Uint32 index)
{
	bool result = false;
	if (m_flag.test(BFlag::Setup)) {
		amAssert(m_act);
		
		u32 hit_num = AoActGetHitNum(m_act);
		if (1 < hit_num) {
			//複数の当たり情報を持つ
			AOS_ACT_HIT *hit_tbl = reinterpret_cast<AOS_ACT_HIT *>(amMemAlloc(sizeof(AOS_ACT_HIT) * hit_num));
			AoActGetHitTbl(hit_tbl, hit_num, m_act);
			for (AOS_ACT_HIT *hit = &hit_tbl[0], *hit_end = &hit_tbl[hit_num]; hit != hit_end; ++hit) {
				if (AoActHitTestCorReverse(hit, pos.x(), pos.y())) {
					result = true;
					break;
				}
			}
			amMemFree(hit_tbl );
		} else if (0 < hit_num) {
			//単体の当たり情報しか持たない
			AOS_ACT_HIT hit_buf;
			AOS_ACT_HIT *hit = &hit_buf;
			AoActGetHitTbl(hit, 1, m_act);
			result = AoActHitTestCorReverse(hit, pos.x(), pos.y());
		}
	}
	return result;

	UNREFERENCED_PARAMETER(index);
}


//------------------------------------------------------------------------------**********


































































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
