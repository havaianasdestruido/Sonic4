// ==========================================================================
/*!
  @file dmCmnBackup.cpp
  @brief デモ・共通バックアップモジュール

  @author Kazuki Yoshida
				Copyright(c) 2009 Dimps

  $Id: dmCmnBackup.cpp 2 2011-04-11 05:21:26Z thamada $
  $Date:: 2011-04-11 14:21:26 +0900#$
 */
// ==========================================================================
/*
 * Memo
 *
 */

//----- Include Files -------------------------------------------------------
#include "pch.h"
#include "gs.h"
#include "gsMainSys.h"
#include "aoStorage.h"

#include "gsBackup.hpp"

#include "dmCmnBackup.h"

//----- Definitions ---------------------------------------------------------

//----- Macros --------------------------------------------------------------

//----- Macros Functions ----------------------------------------------------

//----- External Declarations -----------------------------------------------

//----- Static Declarations -------------------------------------------------
static BOOL dmCmnBackupIsCmpSaveData(void);
static BOOL dmCmnBackupMathCompare(void);

//----- Global Variables ----------------------------------------------------

//----- Local Variables -----------------------------------------------------


//----- Global Functions ----------------------------------------------------
// ==========================================================================
// DmCmnBackupLoad
/*!
 *	データロード処理
 */
// ==========================================================================
void DmCmnBackupLoad(void)
{
	using namespace gs::backup;
	SBackup &backup = SBackup::CreateInstance();
	Uint32	size = sizeof(backup);
	
	AoStorageClearError();
	
	AoStorageLoadStart(&backup, size);
}



// ==========================================================================
// DmCmnBackupIsLoadFinished
/*!
 *	データロードチェック処理
 *
 *	@note	内部でセーブ直後のデータと比較し、変更がある場合のみ、
 *			セーブ処理を行う。
 *	@param	is_first	[in] 初回セーブかどうか
 *	@reutrn	TRUE : 終了
 *			FALSE: セーブ中
 */
// ==========================================================================
BOOL DmCmnBackupIsLoadFinished(void)
{
	GSS_MAIN_SYS_INFO *gs_main = GsGetMainSysInfo();
	
	if (AoStorageLoadIsFinished()) {
		// ロード直後のデータを比較用にコピーしておく
		amCopyMemory(&gs_main->cmp_backup
					 , &gs_main->backup
					 , sizeof(GSS_BACKUP));
		
		return TRUE;
	}
	
	return FALSE;
}



// ==========================================================================
// DmCmnBackupIsLoadSuccessed
/*!
 *	データロード成功チェック処理
 *
 *	@note	ロード成功、またはロード失敗でもセーブ不要エラーの場合は
 *			成功とみなし、それ以外のタイトルへ戻す必要のある場合のみ、
 *			失敗と返すセーブ成功チェック処理
 *	@reutrn	TRUE : 成功
 *			FALSE: ロード失敗(タイトルへ遷移させる場合のエラー)
 */
// ==========================================================================
BOOL DmCmnBackupIsLoadSuccessed(void)
{
	GSS_MAIN_SYS_INFO *gs_main = GsGetMainSysInfo();
	BOOL result = FALSE;
	
#if 0
	AOE_STORAGE_ERROR is_error = AOD_STORAGE_ERROR_NONE;
#endif
	
	if (AoStorageLoadIsSuccessed()) {
		gs_main->is_save_run = TRUE;
		//ok
		result = TRUE;
	}
	
	else {
#if 1
		result = FALSE;
#else	
		is_error = AoStorageGetError();
		
		if (is_error == AOD_STORAGE_ERROR_NO_SAVE) {
			gs_main->is_save_run = FALSE;
			//ok
			result = TRUE;
		}
		
		else {
			// セーブエラー(タイトルへ強制的に戻す)
			result = FALSE;
		}
#endif
	}
	
	return result;
}



// ==========================================================================
// DmCmnBackupSave
/*!
 *	データセーブ処理
 *
 *	@note	内部でセーブ直後のデータと比較し、変更がある場合のみ、
 *			セーブ処理を行う。
 *	@param	is_first	[in] 初回セーブかどうか
 *	@param	is_new		[in] ロードなしセーブかどうか(TRUEの場合、is_firstは必ずFALSE)
 *
 *	@reutrn	TRUE : 終了
 */
// ==========================================================================
void DmCmnBackupSave(BOOL is_first, BOOL is_new, BOOL is_del)
{
	GSS_MAIN_SYS_INFO *gs_main = GsGetMainSysInfo();
	using namespace gs::backup;
	SBackup &backup = SBackup::CreateInstance();
	Uint32	size = sizeof(backup);
	BOOL cmp_data = FALSE;
	
#if !_WII
	UNREFERENCED_PARAMETER(is_del);
#endif
	
#if _WII
	gs::backup::SSystem &sys_data = gs::backup::SSystem::CreateInstance();
#endif // #if _WII
	
	// エラーチェッククリア
	AoStorageClearError();
	
	if (is_first) {
#if _WII
//		sys_data.SetLastSaveChrono(OSGetTime());
#endif // #if _WII
		
		// 新規データセーブ
		AoStorageSaveStart(&backup, size, TRUE, FALSE);
		
		// セーブ直後のデータを比較用にコピーしておく
		amCopyMemory(&gs_main->cmp_backup
					 , &gs_main->backup
					 , sizeof(GSS_BACKUP));
	}
	
	else {
		// 新規セーブの場合、有無を言わさずセーブする
		if (is_new) {
#if _WII
			// セーブした時間の設定
//			sys_data.SetLastSaveChrono(OSGetTime());
#endif // #if _WII
			
			// セーブ開始
			AoStorageSaveStart(&backup, size, FALSE, TRUE);
			
			// セーブ直後のデータを比較用にコピーしておく
			amCopyMemory(&gs_main->cmp_backup
						 , &gs_main->backup
						 , sizeof(GSS_BACKUP));
		}
		
		// セーブしていいかどうかをフラグで判定
		else if (gs_main->is_save_run) {
			// ここで既存セーブデータと違いを比較
			cmp_data = dmCmnBackupIsCmpSaveData();
			
			// 既存のデータと違いがある場合、データセーブ
			if (cmp_data) {
#if _WII
				if (is_del == FALSE) {
					sys_data.SetLastSaveChrono(OSGetTime());
				}
#endif // #if _WII
				
				// セーブ開始
				AoStorageSaveStart(&backup, size, FALSE, FALSE);
				
				// セーブ直後のデータを比較用にコピーしておく
				amCopyMemory(&gs_main->cmp_backup
							 , &gs_main->backup
							 , sizeof(GSS_BACKUP));
			}
		}
	}
	
}



// ==========================================================================
// DmCmnBackupIsSaveFinished
/*!
 *	データセーブ処理
 *
 *	@note	内部でセーブ直後のデータと比較し、変更がある場合のみ、
 *			セーブ処理を行う。
 *	@param	is_first	[in] 初回セーブかどうか
 *	@reutrn	TRUE : 終了
 *			FALSE: セーブ中
 */
// ==========================================================================
BOOL DmCmnBackupIsSaveFinished(void)
{
	if (AoStorageSaveIsFinished()) {
		return TRUE;
	}
	
	return FALSE;
}



// ==========================================================================
// DmCmnBackupIsSaveSuccessed
/*!
 *	データセーブ処理
 *
 *	@note	セーブ成功、またはセーブ失敗でもセーブ不要エラーの場合は
 *			成功とみなし、それ以外のタイトルへ戻す必要のある場合のみ、
 *			失敗と返すセーブ成功チェック処理
 *	@reutrn	TRUE : 成功
 *			FALSE: セーブ失敗(タイトルへ遷移させる場合のエラー)
 */
// ==========================================================================
BOOL DmCmnBackupIsSaveSuccessed(void)
{
	GSS_MAIN_SYS_INFO *gs_main = GsGetMainSysInfo();
	AOE_STORAGE_ERROR is_error = AOD_STORAGE_ERROR_NONE;
	BOOL result = FALSE;
	
	if (AoStorageSaveIsSuccessed()) {
		gs_main->is_save_run = TRUE;
		
		result = TRUE;
	}
	
	else {
		is_error = AoStorageGetError();
		
		if (is_error == AOD_STORAGE_ERROR_NO_SAVE) {
			gs_main->is_save_run = FALSE;
			
			result = TRUE;
		}
		
		else {
			// セーブエラー(タイトルへ強制的に戻す)
//			main_work->proc_update = dmTitleProcWaitInput;
//			main_work->proc_input = dmTitleInputProcTitle;
			
			result = FALSE;
		}
	}
	
	return result;
}



//----- Local Functions -----------------------------------------------------
// ==========================================================================
// dmCmnBackupIsCmpSaveData
/*!
 *	セーブデータ比較処理
 *
 *	@reutrn	TRUE : 違いあり
 *			FALSE: データ差異なし
 */
// ==========================================================================
BOOL dmCmnBackupIsCmpSaveData(void)
{
	BOOL result = FALSE;
	
	if (dmCmnBackupMathCompare()) {
		result = FALSE;
	}
	
	else {
		result = TRUE;
	}
	
#if 1//_WII
	result = TRUE;
#endif
	
	return result;
}

// ==========================================================================
// GmSoundFlush
/*!
 *	ゲームサウンド 片付け
 */
// ==========================================================================
/*
void GmSoundFlush(void)
{
	// BGMデータフラッシュ
	GsSoundFlushBgm();
	// SEデータフラッシュ
	GsSoundFlushSe(&gm_sound_data_work_list[GME_SOUND_DATA_IDX_SE]);
}
*/

// ============================================================================
// dmCmnBackupMathCompare
/*!
	比較

	@param	lhs		[io]	要素1
	@param	rhs		[in]	要素2
	@param	size	[in]	サイズ

	@retval	TRUE	同値
	@retval	FALSE	相違

	@note
		共に4バイトアラインされているデータが一番高速に比較されます。
		次が共に2バイトアラインされているデータになり、
		最悪ケースはどちらかがアラインされていないデータになります。

		サイズのアラインは関係有りません。
 */
// ============================================================================
BOOL dmCmnBackupMathCompare(void)
{
	GSS_MAIN_SYS_INFO *gs_main = GsGetMainSysInfo();
	
	// セーブ有効フラグがOFFの場合、FALSE
	if (!gs_main->is_save_run) {
		return TRUE;
	}
	
	// システムデータインスタンス作成
	gs::backup::SSystem &sys_data
		= gs::backup::SSystem::CreateInstance();
	
	
	// 通常ステージデータインスタンス作成
	gs::backup::SStage &stg_data
		= gs::backup::SStage::CreateInstance();
	
	
	// スペシャルステージデータインスタンス作成
	gs::backup::SSpecial &spe_data
		= gs::backup::SSpecial::CreateInstance();
	
	
	// オプションデータインスタンス取得
	gs::backup::SOption &opt_data
		= gs::backup::SOption::CreateInstance();
	
	
	// システムデータ差異チェック
	
	
	// 比較用システムデータインスタンス取得
	gs::backup::SSystem &cmp_sys_data
		= gs_main->cmp_backup.GetSystem();
	
	
	// 比較用通常ステージデータインスタンス取得
	gs::backup::SStage &cmp_stg_data
		= gs_main->cmp_backup.GetStage();
	
	
	// 比較用スペシャルステージデータインスタンス取得
	gs::backup::SSpecial &cmp_spe_data
		= gs_main->cmp_backup.GetSpecial();
	
	
	// 比較用オプションデータインスタンス取得
	gs::backup::SOption &cmp_opt_data
		= gs_main->cmp_backup.GetOption();
	
	
	// システムデータ比較
	// (システムデータは一部比較しないデータがあるため、一つ一つ比較)
	
	// 残機比較
	if (sys_data.GetPlayerStock() != cmp_sys_data.GetPlayerStock()) {
		return FALSE;
	}
	
	// 累計エネミー撃退数比較
	if (sys_data.GetKilled() != cmp_sys_data.GetKilled()) {
		return FALSE;
	}
	
	// クリア回数比較
	if (sys_data.GetClearCount() != cmp_sys_data.GetClearCount()) {
		return FALSE;
	}
	
	// アナウンス比較
	for (int i = 0; i < 7; ++i) {
		gs::backup::SSystem::EAnnounce::Type msg_type
			= gs::backup::SSystem::EAnnounce::OpenZoneSelect;
		
		msg_type = (gs::backup::SSystem::EAnnounce::Type)i;
		
		if (sys_data.IsAnnounce(msg_type) != cmp_sys_data.IsAnnounce(msg_type)) {
			return FALSE;
		}
	}
	
#if _WII
	// 最後にクリアしたACT比較
	if (sys_data.GetLastClearAct() != cmp_sys_data.GetLastClearAct()) {
		return FALSE;
	}
	
	// DWCユーザーデータ比較
//	if (sys_data.GetDwcUserData() != cmp_sys_data.GetDwcUserData()) {
//		return FALSE;
//	}
#endif
	
	
	// ステージデータ比較
	if (memcmp((const void *)&cmp_stg_data
			   , (const void *)&stg_data
			   , sizeof(gs::backup::SStage)) != 0) {
		return FALSE;
	}
	
	// スペステデータ比較
	if (memcmp((const void *)&cmp_spe_data
			   , (const void *)&spe_data
			   , sizeof(gs::backup::SSpecial)) != 0) {
		return FALSE;
	}
	
	// オプションデータ比較
	if (memcmp((const void *)&cmp_opt_data
			   , (const void *)&opt_data
			   , sizeof(gs::backup::SOption)) != 0) {
		return FALSE;
	}
	
	
	return TRUE;
}


// ==========================================================================
// _pt
/*!
 *	@param	tcb	[in]	TCB
 */
// ==========================================================================
