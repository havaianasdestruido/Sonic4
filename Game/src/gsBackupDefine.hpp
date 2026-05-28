// ============================================================================
/*!
	@file	gsBackupDefine.hpp
	@brief	バックアップ・定義

	@author	Kouji Hokazono <kouji_hokazono@dimps.co.jp>
		Copyright(c) 2009 Dimps
	$Id: gsBackupDefine.hpp 2 2011-04-11 05:21:26Z thamada $
 */
// ============================================================================
/*
 * $Log$
 */

/*!
	@page gsBackupDefineMain バックアップ・定義

	@section gsBackupDefineSummary 概要
		バックアップ・定義を提供します。
 */

#pragma once
#if	defined(__cplusplus)
extern "C" {
#endif

//------ C Include Files -------------- インクルード ---------------------------******CIF*
//------ C Macro ---------------------- マクロ ---------------------------------******CMC*
//------ C External Definitions ------- グローバル変数及び関数の宣言 -----------******CED*


#if	defined(__cplusplus)
} // extern "C"
#endif

#if	defined(__cplusplus)
//------ Include ---------------------- インクルード ---------------------------******_IC*
//------ Debug ------------------------ デバッグ -------------------------------******_DG*
#if defined(MTD_DEBUG)
#endif	//#if defined(MTD_DEBUG)
//------ Macro ------------------------ マクロ ---------------------------------******_MC*
namespace gs {
namespace backup {









//------ Class ------------------------ クラス ---------------------------------******_CL*
/*!	@class
	@brief	バックアップ・ステージ列挙定義
		バックアップのステージ列挙を提供します。
 */
struct EStage {
	enum Type {
		Zone1Act1,	//<ゾーン1アクト1
		Zone1Act2,	//<ゾーン1アクト2
		Zone1Act3,	//<ゾーン1アクト3
		Zone1Boss,	//<ゾーン1ボス
		Zone2Act1,	//<ゾーン2アクト1
		Zone2Act2,	//<ゾーン2アクト2
		Zone2Act3,	//<ゾーン2アクト3
		Zone2Boss,	//<ゾーン2ボス
		Zone3Act1,	//<ゾーン3アクト1
		Zone3Act2,	//<ゾーン3アクト2
		Zone3Act3,	//<ゾーン3アクト3
		Zone3Boss,	//<ゾーン3ボス
		Zone4Act1,	//<ゾーン4アクト1
		Zone4Act2,	//<ゾーン4アクト2
		Zone4Act3,	//<ゾーン4アクト3
		Zone4Boss,	//<ゾーン4ボス
		Final,		//<ファイナル
		
		Max,
		None,
	};
};
//------------------------------------------------------------------------------**********























//------ Class ------------------------ クラス ---------------------------------******_CL*
/*!	@class
	@brief	バックアップ・スペステ列挙定義
		バックアップのスペステ列挙を提供します。
 */
struct ESpecialStage {
	enum Type {
		Stage1,	//<スペシャルステージ1
		Stage2,	//<スペシャルステージ2
		Stage3,	//<スペシャルステージ3
		Stage4,	//<スペシャルステージ4
		Stage5,	//<スペシャルステージ5
		Stage6,	//<スペシャルステージ6
		Stage7,	//<スペシャルステージ7
		
		Max,
		None,
	};
};
//------------------------------------------------------------------------------**********




















} //namespace backup
} //namespace gs
#endif //#if	defined(__cplusplus)

	// ============================================================================
	// gs::backup::SBackup
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
	// ============================================================================
