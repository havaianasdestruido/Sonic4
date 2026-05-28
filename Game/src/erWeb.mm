// ============================================================================
/*!
	@file	erWeb.mm
	@brief	WEBクラス

	@author	Kouji Hokazono <kouji_hokazono@dimps.co.jp>
		Copyright(c) 2010 Dimps
	$Id$
 */
// ============================================================================
/*
 * $Log$
 */

//------ Include ---------------------- インクルード ---------------------------******_IC*
//------ Debug ------------------------ デバッグ -------------------------------******_DG*
#if defined(MTD_DEBUG)
#endif	//#if defined(MTD_DEBUG)
//------ Macro ------------------------ マクロ ---------------------------------******_MC*
//------ C Global Functions ----------- C グローバル関数の定義 -------------------******CGF*
//------ C Global Functions ----------- C グローバル関数の定義 -------------------******CGF*
namespace er {
namespace web {



//------ Class ------------------------ クラス ---------------------------------******_CL*
#include "erWeb.hpp"


//■//-- Local Constant --------------- ローカル定数 ---------------------------******LCS*
//■//-- Public Constant -------------- 公開定数 -------------------------------******PCS*
//■//-- Public Function -------------- 公開関数 -------------------------------******PFC*
// =============================================================================
// StartWeb
/*!
	WEBの開始
	
	@param	url	[in]	URL
 */
// ==========================================================================
void StartWeb(const char *url)
{
	NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];

	NSString* ns_url = [NSString stringWithCString:url encoding:NSShiftJISStringEncoding];
	[[UIApplication sharedApplication] openURL:[NSURL URLWithString:ns_url]];

	[pool release];
}



//■//-- Get Function ----------------- 取得関数 -------------------------------******PGF*
//■//-- Set Function ----------------- 設定関数 -------------------------------******PSF*
//■//-- Constructor And Destructor --- コンストラクタ・デストラクタ------------******_CD*
//■//-- Local Function --------------- ローカル関数 ---------------------------******LFC*
//------------------------------------------------------------------------------**********






























} //namespace web
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
