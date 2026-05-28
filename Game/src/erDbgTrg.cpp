// ============================================================================
/*!
	@file	erDbgTrg.cpp
	@brief	テストイベント・トリガ

	@author	Kouji Hokazono <kouji_hokazono@dimps.co.jp>
		Copyright(c) 2009 Dimps
	$Id: erDbgTrg.cpp 193 2011-05-27 07:34:33Z thamada $
 */
// ============================================================================
/*
 * $Log$
 */

//------ Include ---------------------- インクルード ---------------------------******_IC*
#include "pch.h"
#include "gmPadVirtualPad.hpp"
#include "gmPadPolarHandle.hpp"
#include "erDbgTrg.hpp"
#include "gsEnvironment.h"

#if _DEBUG

#if _WII
#pragma warn_notinlined off	//インライン展開出来無い関数に対する警告メッセージの無効化
#endif //_WII


//------ Debug ------------------------ デバッグ -------------------------------******_DG*
#if defined(MTD_DEBUG)
#endif	//#if defined(MTD_DEBUG)
//------ Macro ------------------------ マクロ ---------------------------------******_MC*
//------ C Global Functions ----------- C グローバル関数の定義 -------------------******CGF*
//------ C Global Functions ----------- C グローバル関数の定義 -------------------******CGF*


namespace er {
namespace dbg {



//------ Class ------------------------ クラス ---------------------------------******_CL*
//■//-- Local Constant --------------- ローカル定数 ---------------------------******LCS*
//■//-- Public Constant -------------- 公開定数 -------------------------------******PCS*
//■//-- Public Function -------------- 公開関数 -------------------------------******PFC*
// ============================================================================
// CTrg::Enter
/*!
	決定された時に実行される関数
 */
// ============================================================================
void CTrg::Enter()
{
}

// ============================================================================
// CTrg::Focus
/*!
	選択されている時に実行される関数
 */
// ============================================================================
void CTrg::Focus()
{
	proc_type::operator()();

	//インフォメーション
	int x, y;
	x = 3;
	y = 12;
	CEvtBase::EColor::Type clr = CEvtBase::EColor::White;
	if (m_is_lock) {
#if defined(MTD_DEBUG)
		Printc(x, y++, clr, "%c         : Return", GsEnvDebugGetCancelKeyChar());
#endif //defined(MTD_DEBUG)
		if (GetPadStand(0) & GSD_KEY_CANCEL) {
			m_is_lock = false;
		}
	} else {
#if defined(MTD_DEBUG)
		Printc(x, y++, clr, "%c         : Enter", GsEnvDebugGetDecideKeyChar());
#endif //defined(MTD_DEBUG)
		if (GetPadStand(0) & GSD_KEY_DECIDE) {
			m_is_lock = true;
		}
	}
}

// ============================================================================
// CTrg::MoveEvent
/*!
	イベント間の移動を判定する関数

	@retval	0		移動しない
	@retval	1～		次のイベントへ
	@retval	～-1	前のイベントへ

	@note
		デフォルトでは ←・→ が移動操作になります
 */
// ============================================================================
CTrg::TMoveDirect CTrg::MoveEvent()
{
	TMoveDirect direct = TMoveDirect(0);
#if defined(MTD_DEBUG) 
	if (!m_is_lock) {
		direct = super_type::MoveEvent();
	}
#endif
	return direct;
}

//■//-- Get Function ----------------- 取得関数 -------------------------------******PGF*
//■//-- Set Function ----------------- 設定関数 -------------------------------******PSF*
//■//-- Constructor And Destructor --- コンストラクタ・デストラクタ------------******_CD*
//■//-- Local Function --------------- ローカル関数 ---------------------------******LFC*
// ============================================================================
// CTrg::clear
/*!
	クリア
 */
// ============================================================================
void CTrg::clear()
{
}

// ============================================================================
// CTrg::init
/*!
	初期化
 */
// ============================================================================
void CTrg::init()
{
	 SetProc(&CTrg::start);

	 m_rect.Create(0, 0, static_cast<Sint32>(AMD_SCREEN_2D_WIDTH), static_cast<Sint32>(AMD_SCREEN_2D_HEIGHT));
}

// ============================================================================
// CTrg::start
/*!
	プロシージャ
 */
// ============================================================================
void CTrg::start()
{
	amPrint(6, 14, "start()");

	int x, y;
	x = 6;
	y = 16;

#ifdef _IPHONE
	// iPhone入力状況を表示
#if defined(MTD_DEBUG) 	
	amPrintf(x, y++, "PAD      On(%08X)/ Rpt (%08X)/ Push(%08X)/ Pull(%08X)", GetPadDirect(0), GetPadRepeat(0), GetPadStand(0), GetPadRelease(0));
#endif
	for (int i = 0; i < 5; i++) {
		if (amTpIsTouchOn(i)) {
			amPrintf(x, y++, "Touch[%d] On(%3d,%3d) / Prev(%3d,%3d) / Push(%3d,%3d) / Pull(   ,   )", i,
					_am_tp_touch[i].on[AMD_X], _am_tp_touch[i].on[AMD_Y], _am_tp_touch[i].prev[AMD_X], _am_tp_touch[i].prev[AMD_Y],
					_am_tp_touch[i].push[AMD_X], _am_tp_touch[i].push[AMD_Y]
					);
		} else {
			amPrintf(x, y++, "Touch[%d] On(   ,   ) / Prev(   ,   ) / Push(   ,   ) / Pull(%3d,%3d)", i,
					 _am_tp_touch[i].pull[AMD_X], _am_tp_touch[i].pull[AMD_Y]
					 );
		}
	}
	amPrintf(x, y++, "accel core   %+1.4f %+1.4f %+1.4f", _am_iphone_accel_data.core.x,   _am_iphone_accel_data.core.y,   _am_iphone_accel_data.core.z);
	amPrintf(x, y++, "accel sensor %+1.4f %+1.4f %+1.4f", _am_iphone_accel_data.sensor.x, _am_iphone_accel_data.sensor.y, _am_iphone_accel_data.sensor.z);
	amPrintf(x, y++, "accel rot    %+5d %+5d %+5d", _am_iphone_accel_data.rot_x,    _am_iphone_accel_data.rot_y,    _am_iphone_accel_data.rot_z);
	y++;
#endif
	
	//バーチャルパッド
	{
		gm::CPadVirtualPad &virtual_pad = gm::CPadVirtualPad::CreateInstance();
		if (m_is_lock) {
			//ロック中

			if (!virtual_pad.IsValid()) {
				//未初期化
				float area[4] = {AMD_SCREEN_2D_WIDTH * 0.5f, 0.0f, AMD_SCREEN_2D_WIDTH, AMD_SCREEN_2D_HEIGHT};
				virtual_pad.Create(area);
			} else {
				//初期化済み
				virtual_pad.Update();
				int x2 = x + 5;
				for (int i = 0, max = 5; i < max; ++i) {
					Print(x2++, y, "%c", ((virtual_pad.IsFocus(i))? '0' + i: '_'));
				}
				x2++;
				u16 pad = virtual_pad.GetValue();
				Print(x2, y, "%04x", pad);
				x2 += 5;
				if (pad & KEY_L_UP) {
					Print(x2, y, "UP");
					x2 += 3;
				}
				if (pad & KEY_L_DOWN) {
					Print(x2, y, "DOWN");
					x2 += 5;
				}
				if (pad & KEY_L_LEFT) {
					Print(x2, y, "LEFT");
					x2 += 5;
				}
				if (pad & KEY_L_RIGHT) {
					Print(x2, y, "RIGHT");
					x2 += 6;
				}
				Print(x, y++, "Pad");
			}
		} else {
			//非ロック
			virtual_pad.Release();
		}
	}

	//極座標ハンドル
	{
		gm::CPadPolarHandle	&polar = gm::CPadPolarHandle::CreateInstance();
		if (m_is_lock) {
			//ロック中

			if (!polar.IsValid()) {
				//未初期化
				float area[4] = {0.0f, 0.0f, AMD_SCREEN_2D_WIDTH * 0.5f, AMD_SCREEN_2D_HEIGHT};
				polar.Create(area);
			} else {
				//初期化済み
				polar.Update();
				Angle32 angle = polar.GetValue<Angle32>();
				float rad = polar.GetValue<float>();
				s32 focus = polar.GetFocusTpIndex();
				Print(x, y++, "Polar  0x%08X,%6.2f,%-1d", angle, rad, focus);
				
				{//軸描画
					int center_x = 65;
					int center_y = 33;
					float sin, cos;
					amSinCos(angle, &sin, &cos);
					for (int i = 0; i < 7; ++i) {
						Print(center_x + i * sin, center_y - i * cos, "*");
					}
				}
				
				//差分モード
				if (amTpIsTouchOn(2)) {
					polar.SetValue(0.0f);
				}
			}
		} else {
			//非ロック
			polar.Release();
		}
	}
}


//------------------------------------------------------------------------------**********

































} //namespace dbg
} //namespace er

#endif // #if _DEBUG

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
