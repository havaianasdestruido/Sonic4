/****************************************************************************
 *
 * CRI Middleware SDK
 *
 * Copyright (c) 2008 CRI Middleware Co.,Ltd.
 *
 * Library  : CRI File System
 * Module   : Installer
 * File     : crifs_installer.h
 *
 ****************************************************************************/
/*JP
 * \addtogroup FSLIB_CRIFS_INSTALLER
 * HDDインストール機能を使用するためのAPIです。<br>
 * HDDインストール機能は、HDD等の書き込み可能なデバイスに対して、簡単にファイルのコピーを行うことができる機能です。<br>
 * 詳細については \ref crifs_installer を参照してください。
 * @{
 */
/*EN
 * \addtogroup FSLIB_CRIFS_INSTALLER
 * @{
 */

/* 多重定義防止 */
/* Prevention of redefinition */
#ifndef	CRIFS_INSTALLER_H_INCLUDED
#define	CRIFS_INSTALLER_H_INCLUDED

/***************************************************************************
 *      インクルードファイル
 *      Include files
 ***************************************************************************/
#include "cri_xpt.h"
#include "cri_error.h"
#include "cri_file_system.h"

/***************************************************************************
 *      定数マクロ
 *      Macro Constants
 ***************************************************************************/

/***************************************************************************
 *      処理マクロ
 *      Macro Functions
 ***************************************************************************/

/***************************************************************************
 *      データ型宣言
 *      Data Type Declarations
 ***************************************************************************/
/* インストーラオブジェクトの前方宣言 */
struct _CriFsInstallerObj;

/*JP
 * \brief CriFsInstallerハンドル
 * \par 説明:
 * インストーラにアクセスするためのハンドルです。<br>
 * ::criFsInstaller_Create関数により作成します。
 * \sa criFsInstaller_Create
 */
typedef struct _CriFsInstallerObj *CriFsInstallerHn;


/*JP
 * \brief インストーラステータス
 * \par 説明:
 * インストーラハンドルの状態を表します。<br>
 * ::criFsInstaller_GetStatus関数により取得できます。
 * \sa criFsInstaller_GetStatus
 */
/*EN
 * \brief Installer Status
 */
typedef enum {
	CRIFSINSTALLER_STATUS_STOP,		/*JP< 停止中		*/	/*EN< Stopping			*/
	CRIFSINSTALLER_STATUS_BUSY,		/*JP< 処理中		*/	/*EN< Busy				*/
	CRIFSINSTALLER_STATUS_COMPLETE,	/*JP< 完了			*/	/*EN< Complete			*/
	CRIFSINSTALLER_STATUS_ERROR		/*JP< エラー		*/	/*EN< Error				*/
} CriFsInstallerStatus;

/*JP
 * \brief インストーラプライオリティ
 * \par 説明:
 * 読み込みまたは書き込みの優先度です。<br>
 * \sa criFsInstaller_SetLoadPriority
 * \sa criFsInstaller_SetWritePriority
 */
/*EN
 * \brief Priority
 */
typedef enum {
	CRIFSINSTALLER_PRIORITY_HIGHEST 		= 2,	/*JP< 最高		*/	/*EN< Highest		*/
	CRIFSINSTALLER_PRIORITY_ABOVE_NORMAL 	= 1,	/*JP< 高		*/	/*EN< Above normal	*/
	CRIFSINSTALLER_PRIORITY_NORMAL 			= 0,	/*JP< 普通		*/	/*EN< Normal		*/
	CRIFSINSTALLER_PRIORITY_BELOW_NORMAL 	= -1,	/*JP< 低		*/	/*EN< Below normal	*/
	CRIFSINSTALLER_PRIORITY_LOWEST 			= -2	/*JP< 最低		*/	/*EN< Lowest		*/
} CriFsInstallerPriority;

/*JP
 * \brief コピーポリシー
 * \par 説明:
 * ファイルをコピーするか否かを判定する条件を指定します。
 * インストーラ作成時（::criFsInstaller_Create関数）に、::CriFsInstallerOption構造体にて指定します。
 * \sa CriFsInstallerOption
 * \sa criFsInstaller_Create
 */
typedef enum {
	CRIFSINSTALLER_COPY_POLICY_ALWAYS 			= 0		/*JP< 常にコピーする					*/
							 							/*EN< Copy Always						*/
} CriFsInstallerCopyPolicy;

/*JP
 * \brief インストーラオプション
 * \par 説明:
 * インストーラの各種設定です。<br>
 * インストーラ作成時（::criFsInstaller_Create関数）に指定します。<br>
 * 将来的にメンバが増える可能性に備え、設定前に0クリアしてから使用してください。
 * \sa criFsInstaller_Create
 */
typedef struct {
	/*JP
	 * \brief コピーポリシー
	 * \par 説明:
	 * コピーポリシーを指定します。
	 * \sa CriFsInstallerCopyPolicy
	 */
	CriFsInstallerCopyPolicy copy_policy;
} CriFsInstallerOption;

/***************************************************************************
 *      変数宣言
 *      Prototype Variables
 ***************************************************************************/

/***************************************************************************
 *      関数宣言
 *      Prototype Functions
 ***************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif
/*JP
 * \brief サーバ処理の実行
 * \return	CriError	エラーコード
 * \par 説明:
 * サーバ処理を実行します。この関数は定期的（例：1Vに1回）に呼び出してください。<br>
 * マルチスレッドモデルの場合は、インストーラスレッドを明示的に実行状態にします。<br>
 * ユーザマルチスレッドモデル、シングルスレッドモデルの場合は、ロードリクエスト／ライトリクエスト等の処理をこの関数内で行います。
 * \par 例:
 * \code
 * for (;;) {
 *	CriFsInstallerStatus status;
 * 	criFsInstaller_GetStatus(installer, &status);
 * 	if (status == CRIFSINSTALLER_STATUS_COMPLETE) {
 * 		break;
 * 	}
 * 	if (status == CRIFSINSTALLER_STATUS_ERROR) {
 * 		return -1;
 * 	}
 *
 * 		:
 * 		:
 * 		:
 *
 * 	criFsInstaller_ExecuteMain();
 * 	criFs_ExecuteMain();
 *
 * 	WaitVsync();
 * }
 * \endcode
 * \sa CriFsThreadModel
 */
CriError CRIAPI criFsInstaller_ExecuteMain(void);


/*JP
 * \brief インストーラの作成
 * \param[out]	installer	インストーラハンドル
 * \param[in]	option	インストーラオプション
 * \return	CriError	エラーコード
 * \par 説明:
 * インストーラを作成し、インストーラハンドルを返します。<br>
 * \par 例:
 * \code
 * CriFsInstallerHn installer;
 * CriFsInstallerOption installer_option;
 * memset(&installer_option, 0, sizeof(installer_option));
 * installer_option.copy_policy = CRIFSINSTALLER_COPY_POLICY_ALWAYS;
 * criFsInstaller_Create(&installer, option);
 *
 * 		:
 * 		:
 * 		:
 *
 * criFsInstaller_Destroy(installer);
 * \endcode
 * \sa CriFsInstallerOption
 * \sa criFsInstaller_Destroy
 */
CriError CRIAPI criFsInstaller_Create(CriFsInstallerHn *installer, CriFsInstallerOption option);

/*JP
 * \brief インストーラの破棄
 * \param[in]	installer	インストーラハンドル
 * \return	CriError	エラーコード
 * \par 説明:
 * インストーラを破棄します。<br>
 * \par 例:
 * \code
 * CriFsInstallerHn installer;
 * CriFsInstallerOption installer_option;
 * memset(&installer_option, 0, sizeof(installer_option));
 * installer_option.copy_policy = CRIFSINSTALLER_COPY_POLICY_ALWAYS;
 * criFsInstaller_Create(&installer, option);
 *
 * 		:
 * 		:
 * 		:
 *
 * criFsInstaller_Destroy(installer);
 * \endcode
 * \sa criFsInstaller_Create
 */
CriError CRIAPI criFsInstaller_Destroy(CriFsInstallerHn installer);

/*JP
 * \brief ファイルのコピー
 * \param[in]	installer	インストーラハンドル
 * \param[in]	binder	CriFsBinderハンドル
 * \param[in]	src_path	コピー元ファイルパス名
 * \param[in]	dst_path	コピー先ファイルパス名
 * \param[in]	buffer	バッファへのポインタ
 * \param[in]	buffer_size	バッファのサイズ（2の倍数）
 * \return	CriError	エラーコード
 * \par 説明:
 * ファイルのコピーを開始します。<br>
 * buffer引数で指定するバッファは、読み込み／書き込みの中間バッファとして使用されます。
 * また、このバッファは内部的に二分割され、読み込み／書き込みを並列に行います。そのため、バッファサイズは2の倍数で指定する必要があります。<br>
 * <br>
 * 本関数は即時復帰関数です。<br>
 * コピーの完了状態を取得するには ::criFsInstaller_GetStatus関数を使用してください。
 * \par 例:
 * \code
 * criFsInstaller_Copy(
 * 	installer,
 * 	NULL,
 * 	src_file_path,
 * 	dst_file_path,
 * 	copy_buffer,
 * 	COPY_BUFFER_SIZE
 * );
 * for (;;) {
 *	CriFsInstallerStatus status;
 * 	criFsInstaller_GetStatus(installer, &status);
 * 	if (status == CRIFSINSTALLER_STATUS_COMPLETE) {
 * 		break;
 * 	}
 * 	if (status == CRIFSINSTALLER_STATUS_ERROR) {
 * 		return -1;
 * 	}
 *
 * 		:
 * 		:
 * 		:
 *
 * 	criFsInstaller_ExecuteMain();
 * 	criFs_ExecuteMain();
 *
 * 	WaitVsync();
 * }
 * \endcode
 * \sa criFsInstaller_GetStatus
 */
CriError CRIAPI criFsInstaller_Copy(CriFsInstallerHn installer,
	CriFsBinderHn binder, const CriChar8 *src_path, const CriChar8 *dst_path,
	void *buffer, CriSint64 buffer_size);

/*JP
 * \brief 処理の停止
 * \param[in]	installer	インストーラハンドル
 * \return	CriError	エラーコード
 * \par 説明:
 * 処理を停止します。<br>
 * <br>
 * 本関数は即時復帰関数です。<br>
 * 停止の完了状態を取得するには ::criFsInstaller_GetStatus関数を使用してください。
 * \par 例:
 * \code
 * criFsInstaller_Copy(
 * 	installer,
 * 	NULL,
 * 	src_file_path,
 * 	dst_file_path,
 * 	copy_buffer,
 * 	COPY_BUFFER_SIZE
 * );
 * for (;;) {
 *	CriFsInstallerStatus status;
 * 	criFsInstaller_GetStatus(installer, &status);
 * 	if (status == CRIFSINSTALLER_STATUS_COMPLETE) {
 * 		break;
 * 	}
 * 	if (status == CRIFSINSTALLER_STATUS_STOP) {
 * 		break;
 * 	}
 * 	if (status == CRIFSINSTALLER_STATUS_ERROR) {
 * 		return -1;
 * 	}
 *
 * 	if (user_stop_flag == CRI_TRUE) {
 * 		criFsInstaller_Stop(installer);
 * 	}
 *
 *
 * 		:
 * 		:
 * 		:
 *
 * 	criFsInstaller_ExecuteMain();
 * 	criFs_ExecuteMain();
 *
 * 	WaitVsync();
 * }
 * \endcode
 * \sa criFsInstaller_GetStatus
 */
CriError CRIAPI criFsInstaller_Stop(CriFsInstallerHn installer);

/*JP
 * \brief ステータスの取得
 * \param[in]	installer	インストーラハンドル
 * \param[out]	status	ステータス
 * \return	CriError	エラーコード
 * \par 説明:
 * ステータスを取得します。<br>
 * \par 例:
 * \code
 * CriFsInstallerStatus status;
 * criFsInstaller_GetStatus(installer, &status);
 * switch(status) {
 * case CRIFSINSTALLER_STATUS_STOP:
 * 	  :
 * 	break;
 * case CRIFSINSTALLER_STATUS_BUSY:
 * 	  :
 * 	break;
 * case CRIFSINSTALLER_STATUS_COMPLETE:
 * 	  :
 * 	break;
 * case CRIFSINSTALLER_STATUS_ERROR:
 * 	  :
 * 	break;
 * default:
 * 	  :
 * 	break;
 * }
 * \endcode
 */
CriError CRIAPI criFsInstaller_GetStatus(CriFsInstallerHn installer, CriFsInstallerStatus *status);

/*JP
 * \brief 進捗状況の取得
 * \param[in]	installer	インストーラハンドル
 * \param[out]	progress	進捗状況
 * \return	CriError	エラーコード
 * \par 説明:
 * 処理の進捗状況を取得します。進捗状況は0.0～1.0の32ビット浮動小数点数です。<br>
 * ファイルコピー処理の場合、値の精度はコピーバッファのサイズに依存します。
 * \par 例:
 * \code
 * CriFloat32 progress;
 * CriFloat32 progress_old = 0.0f;
 *
 * criFsInstaller_Copy(
 * 	installer,
 * 	NULL,
 * 	src_file_path,
 * 	dst_file_path,
 * 	copy_buffer,
 * 	COPY_BUFFER_SIZE
 * );
 * for (;;) {
 *	CriFsInstallerStatus status;
 * 	criFsInstaller_GetProgress(installer, &progress);
 * 	if (progress != progress_old) {
 * 		printf("progress:%5.2f\n", progress*100.0f);
 * 		progress_old = progress;
 * 	}
 * 	criFsInstaller_GetStatus(installer, &status);
 * 	if (status == CRIFSINSTALLER_STATUS_COMPLETE) {
 * 		break;
 * 	}
 * 	if (status == CRIFSINSTALLER_STATUS_ERROR) {
 * 		return -1;
 * 	}
 *
 * 		:
 * 		:
 * 		:
 *
 * 	criFsInstaller_ExecuteMain();
 * 	criFs_ExecuteMain();
 *
 * 	WaitVsync();
 * }
 * \endcode
 */
CriError CRIAPI criFsInstaller_GetProgress(CriFsInstallerHn installer, CriFloat32 *progress);


/*JP
 * \brief ロードプライオリティの設定
 * \param[in]	installer	インストーラハンドル
 * \param[out]	prio	ロードプライオリティ
 * \return	CriError	エラーコード
 * \par 説明:
 * ロードプライオリティを設定します。<br>
 * ここで設定したプライオリティは、他の::CriFsInstallerHn、及び::CriFsLoaderHnでのロード処理の順序付けに使用されます。
 * \par 例:
 * \code
 * criFsInstaller_SetLoadPriority(installer, CRIFSINSTALLER_PRIORITY_LOWEST);
 * \endcode
 * \sa CriFsInstallerPriority
 */
CriError CRIAPI criFsInstaller_SetLoadPriority(CriFsInstallerHn installer, CriFsInstallerPriority prio);
/*JP
 * \brief ライトプライオリティの設定
 * \param[in]	installer	インストーラハンドル
 * \param[out]	prio	ライトプライオリティ
 * \return	CriError	エラーコード
 * \par 説明:
 * ライトプライオリティを設定します。<br>
 * ここで設定したプライオリティは、他の::CriFsInstallerHnでのライト処理の順序付けに使用されます。
 * \par 例:
 * \code
 * criFsInstaller_SetWritePriority(installer, CRIFSINSTALLER_PRIORITY_HIGHEST);
 * \endcode
 * \sa CriFsInstallerPriority
 */
CriError CRIAPI criFsInstaller_SetWritePriority(CriFsInstallerHn installer, CriFsInstallerPriority prio);

#ifdef __cplusplus
}
#endif

#endif	/* CRIFS_INSTALLER_H_INCLUDED */

/*JP
 * @}
 */
/*EN
 * @}
 */


/* --- end of file --- */
