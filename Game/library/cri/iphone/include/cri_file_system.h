/****************************************************************************
 *
 * CRI Middleware SDK
 *
 * Copyright (c) 2006-2009 CRI Middleware Co.,Ltd.
 *
 * Library  : CRI File System
 * Module   : Library User's Header
 * File     : cri_file_system.h
 *
 ****************************************************************************/
/*!
 *	\file		cri_file_system.h
 */

/* 多重定義防止					*/
/* Prevention of redefinition	*/
#ifndef	CRI_FILE_SYSTEM_H_INCLUDED
#define	CRI_FILE_SYSTEM_H_INCLUDED

/***************************************************************************
 *      インクルードファイル
 *      Include files
 ***************************************************************************/
#include "cri_xpt.h"
#include "cri_error.h"

#ifdef __cplusplus
	#if !defined(XPT_TGT_PSP)
		/* 旧バージョンAPI */
		/* Old version API */
		#include "cri_file_system_ver1api.h"
	#endif
#endif

/***************************************************************************
 *      定数マクロ
 *      Macro Constants
 ***************************************************************************/
/* バージョン情報 */
/* Version Number */
#define CRI_FS_VERSION		(0x02170300)
#define CRI_FS_VER_NUM		"2.17.03"
#define CRI_FS_VER_NAME		"CRI File System"

/*JP
 * \brief コンフィギュレーションのデフォルト値
 */
#if defined(XPT_TGT_NITRO)
#define	CRIFS_CONFIG_DEFAULT_NUM_BINDERS		(8)
#define	CRIFS_CONFIG_DEFAULT_NUM_LOADERS		(32)
#define	CRIFS_CONFIG_DEFAULT_NUM_GROUP_LOADERS	(2)
#define	CRIFS_CONFIG_DEFAULT_NUM_STDIO_HANDLES	(4)
#define	CRIFS_CONFIG_DEFAULT_NUM_INSTALLERS		(0)
#define	CRIFS_CONFIG_DEFAULT_MAX_BINDS			(8)
#define	CRIFS_CONFIG_DEFAULT_MAX_FILES			(32)
#define	CRIFS_CONFIG_DEFAULT_MAX_PATH			(128)
#else
#define	CRIFS_CONFIG_DEFAULT_NUM_BINDERS		(16)
#define	CRIFS_CONFIG_DEFAULT_NUM_LOADERS		(16)
#define	CRIFS_CONFIG_DEFAULT_NUM_GROUP_LOADERS	(2)
#define	CRIFS_CONFIG_DEFAULT_NUM_STDIO_HANDLES	(16)
#define	CRIFS_CONFIG_DEFAULT_NUM_INSTALLERS		(0)
#define	CRIFS_CONFIG_DEFAULT_MAX_BINDS			(16)
#define	CRIFS_CONFIG_DEFAULT_MAX_FILES			(16)
#define	CRIFS_CONFIG_DEFAULT_MAX_PATH			(256)
#endif

/***************************************************************************
 *      処理マクロ
 *      Macro Functions
 ***************************************************************************/
/* 旧バージョンとの互換用 */
/* For compatibility with old version */
#define CriFsConfiguration				CriFsConfig
#define criFs_InitializeConfiguration(config)	criFs_SetDefaultConfig(&config)
#define criFs_CalculateWorkSize(config, nbyte)	criFs_CalculateWorkSizeForLibrary(&config, nbyte)
#define criFs_Initialize(config, buffer, size)	criFs_InitializeLibrary(&config, buffer, size)
#define criFs_Finalize()				criFs_FinalizeLibrary()

/***************************************************************************
 *      データ型宣言
 *      Data Type Declarations
 ***************************************************************************/
/*==========================================================================
 *      CRI File System API
 *=========================================================================*/
/*JP
 * \brief スレッドモデル
 * \par 説明:
 * CRI File Systemライブラリがどのようなスレッドモデルで動作するかを表します。<br>
 * ライブラリ初期化時（::criFs_InitializeLibrary関数）に、::CriFsConfig構造体にて指定します。
 * \sa CriFsConfig
 * \sa criFs_InitializeLibrary
 */
typedef enum {
	/*JP
	 * \brief マルチスレッド
	 * \par 説明:
	 * ライブラリは内部でスレッドを作成し、マルチスレッドにて動作します。<br>
	 * スレッドは::criFs_InitializeLibrary関数呼び出し時に作成されます。
	 */
	/*EN Multi thread				*/
	CRIFS_THREAD_MODEL_MULTI 		= 0,
	/*JP
	 * \brief ユーザマルチスレッド
	 * \par 説明:
	 * ライブラリ内部ではスレッドを作成しませんが、ユーザが独自に作成したスレッドからサーバ処理関数（::criFs_ExecuteFileAccess関数、::criFs_ExecuteDataDecompression関数）を呼び出せるよう、内部の排他制御は行います。
	 */
	/*EN User multi thread				*/
	CRIFS_THREAD_MODEL_USER_MULTI 	= 1,
	/*JP
	 * \brief シングルスレッド
	 * \par 説明:
	 * ライブラリ内部でスレッドを作成しません。また、内部の排他制御も行いません。<br>
	 * このモデルを選択した場合、各APIとサーバ処理関数（::criFs_ExecuteFileAccess関数、::criFs_ExecuteDataDecompression関数）とを同一スレッドから呼び出すようにしてください。
	 */
	/*EN Single thread				*/
	CRIFS_THREAD_MODEL_SINGLE 		= 2,

	/* enum be 4bytes */
	CRIFS_THREAD_MODEL_ENUM_BE_SINT32 = 0x7FFFFFFF
} CriFsThreadModel;

/*JP
 * \brief コンフィギュレーション
 * \par 説明:
 * CRI File Systemライブラリの動作仕様を指定するための構造体です。<br>
 * ライブラリ初期化時（::criFs_InitializeLibrary関数）に引数として本構造体を指定します。<br>
 * \par
 * CRI File Systemライブラリは、初期化時に指定されたコンフィギュレーションに応じて、内部リソースを必要な数分だけ確保します。<br>
 * そのため、コンフィギュレーションに指定する値を小さくすることで、ライブラリが必要とするメモリのサイズを小さく抑えることが可能です。<br>
 * ただし、コンフィギュレーションに指定した数以上のハンドルを確保することはできなくなるため、値を小さくしすぎると、ハンドルの確保に失敗する可能性があります。<br>
 * \par 備考:
 * デフォルト設定を使用する場合、 ::criFs_SetDefaultConfig 関数でデフォルトパラメータをセットし、 ::criFs_InitializeLibrary 関数に指定してください。<br>
 * \attention
 * 将来的にメンバが増える可能性に備え、設定前に0クリアしてから使用してください。<br>
 * \sa criFs_InitializeLibrary, criFs_SetDefaultConfig
 */
typedef struct {
	/*JP
		\brief スレッドモデル
		\par 説明:
		CRI File Systemのスレッドモデルを指定します。<br>
		\sa CriFsThreadModel
	*/
	CriFsThreadModel thread_model;
	/*JP
		\brief 使用するCriFsBinderの数
		\par 説明:
		アプリケーション中で使用するバインダ（CriFsBinder）の数を指定します。<br>
		アプリケーション中で ::criFsBinder_Create 関数を使用してバインダを作成する場合、
		本パラメータに使用するバインダの数を指定する必要があります。<br>
		<br>
		num_bindersには「同時に使用するバインダの最大数」を指定します。<br>
		例えば、 ::criFsBinder_Create 関数と ::criFsBinder_Destroy 関数を交互に続けて実行するケースにおいては、
		最大同時には1つのバインダしか使用しないため、関数の呼び出し回数に関係なくnum_bindersに1を指定することが可能です。<br>
		逆に、ある場面でバインダを10個使用する場合には、その他の場面でバインダを全く使用しない場合であっても、
		num_bindersに10を指定する必要があります。<br>
		\par 備考:
		CRI File Systemライブラリは、使用するバインダの数分だけのメモリを初期化時に要求します。<br>
		そのため、num_bindersに必要最小限の値をセットすることで、ライブラリが必要とするメモリのサイズを抑えることが可能です。<br>
		\sa criFsBinder_Create, criFsBinder_Destroy
	*/
	CriSint32 num_binders;
	/*JP
		\brief 使用するCriFsLoaderの数
		\par 説明:
		アプリケーション中で使用するローダ（CriFsLoader）の数を指定します。<br>
		アプリケーション中で ::CriFsLoader_Create 関数を使用してローダを作成する場合、
		本パラメータに使用するローダの数を指定する必要があります。<br>
		<br>
		num_loadersには「同時に使用するローダの最大数」を指定します。<br>
		例えば、 ::CriFsLoader_Create 関数と ::CriFsLoader_Destroy 関数を交互に続けて実行するケースにおいては、
		最大同時には1つのローダしか使用しないため、関数の呼び出し回数に関係なくnum_loadersに1を指定することが可能です。<br>
		逆に、ある場面でローダを10個使用する場合には、その他の場面でローダを全く使用しない場合であっても、
		num_loadersに10を指定する必要があります。<br>
		\par 備考:
		CRI File Systemライブラリは、使用するローダの数分だけのメモリを初期化時に要求します。<br>
		そのため、num_loadersに必要最小限の値をセットすることで、ライブラリが必要とするメモリのサイズを抑えることが可能です。<br>
		\sa CriFsLoader_Create, CriFsLoader_Destroy
	*/
	CriSint32 num_loaders;
	/*JP
		\brief 使用するCriFsGroupLoaderの数
		\par 説明:
		アプリケーション中で使用するグループローダ（CriFsGroupLoader）の数を指定します。<br>
		アプリケーション中で ::criFsGroupLoader_Create 関数を使用してグループローダを作成する場合、
		本パラメータに使用するグループローダの数を指定する必要があります。<br>
		<br>
		num_group_loadersには「同時に使用するグループローダの最大数」を指定します。<br>
		例えば、 ::criFsGoupLoader_Create 関数と ::criFsGroupLoader_Destroy 関数を交互に続けて実行するケースにおいては、
		最大同時には1つのグループローダしか使用しないため、関数の呼び出し回数に関係なくnum_group_loadersに1を指定することが可能です。<br>
		逆に、ある場面でグループローダを10個使用する場合には、その他の場面でグループローダを全く使用しない場合であっても、
		num_group_loadersに10を指定する必要があります。<br>
		\par 備考:
		CRI File Systemライブラリは、使用するグループローダの数分だけのメモリを初期化時に要求します。<br>
		そのため、num_group_loadersに必要最小限の値をセットすることで、ライブラリが必要とするメモリのサイズを抑えることが可能です。<br>
		\sa criFsGroupLoader_Create, criFsGroupLoader_Destroy
	*/
	CriSint32 num_group_loaders;
	/*JP
		\brief 使用するCriFsStdioの数
		\par 説明:
		アプリケーション中で使用するCriFsStdioハンドルの数を指定します。<br>
		アプリケーション中で ::criFsStdio_OpenFile 関数を使用してCriFsStdioハンドルを作成する場合、
		本パラメータに使用するCriFsStdioハンドルの数を指定する必要があります。<br>
		<br>
		num_stdio_handlesには「同時に使用するCriFsStdioハンドルの最大数」を指定します。<br>
		例えば、 ::criFsStdio_OpenFile 関数と ::criFsStdio_CloseFile 関数を交互に続けて実行するケースにおいては、
		最大同時には1つのCriFsStdioハンドルしか使用しないため、関数の呼び出し回数に関係なくnum_stdio_handlesに1を指定することが可能です。<br>
		逆に、ある場面でCriFsStdioハンドルを10個使用する場合には、その他の場面でCriFsStdioハンドルを全く使用しない場合であっても、
		num_stdio_handlesに10を指定する必要があります。<br>
		\par 備考:
		CRI File Systemライブラリは、使用するCriFsStdioハンドルの数分だけのメモリを初期化時に要求します。<br>
		そのため、num_stdio_handlesに必要最小限の値をセットすることで、ライブラリが必要とするメモリのサイズを抑えることが可能です。<br>
		\attention
		ブリッジライブラリを使用してADXライブラリや救声主ライブラリを併用する場合、
		ADXTハンドルやcriSsPlyハンドルは内部的にCriFsStdioハンドルを作成します。<br>
		そのため、ブリッジライブラリを使用する場合には、CRI File Systemライブラリ初期化時に
		num_stdio_handlesにADXTハンドルやcriSsPlyハンドルの数を加えた値を指定してください。<br>
		\sa criFsStdio_OpenFile, criFsStdio_CloseFile
	*/
	CriSint32 num_stdio_handles;
	/*JP
		\brief 使用するCriFsInstallerの数
		\par 説明:
		アプリケーション中で使用するインストーラ（CriFsInstaller）の数を指定します。<br>
		アプリケーション中で ::criFsInstaller_Create 関数を使用してインストーラを作成する場合、
		本パラメータに使用するインストーラの数を指定する必要があります。<br>
		<br>
		num_installersには「同時に使用するインストーラの最大数」を指定します。<br>
		例えば、 ::criFsInstaller_Create 関数と ::criFsInstaller_Destroy 関数を交互に続けて実行するケースにおいては、
		最大同時には1つのインストーラしか使用しないため、関数の呼び出し回数に関係なくnum_installersに1を指定することが可能です。<br>
		逆に、ある場面でインストーラを10個使用する場合には、その他の場面でインストーラを全く使用しない場合であっても、
		num_installersに10を指定する必要があります。<br>
		\par 備考:
		CRI File Systemライブラリは、使用するインストーラの数分だけのメモリを初期化時に要求します。<br>
		そのため、num_installersに必要最小限の値をセットすることで、ライブラリが必要とするメモリのサイズを抑えることが可能です。<br>
		\attention
		::criFs_SetDefaultConfig マクロを使用してコンフィギュレーションを初期化する場合、num_installersの数は0に設定されます。<br>
		そのため、インストーラを使用する場合には、アプリケーション中でnum_installersを明示的に指定する必要があります。<br>
		\sa criFsInstaller_Create, criFsInstaller_Destroy
	*/
	CriSint32 num_installers;
	/*JP
		\brief 最大同時バインド数
		\par 説明:
		アプリケーション中でバインド処理を行い、保持するバインダID（CriFsBinderId）の数を指定します。<br>
		アプリケーション中で ::criBinder_BindCpk 関数等を使用してバインド処理を行う場合、
		本パラメータに使用するバインダIDの数を指定する必要があります。<br>
		<br>
		max_bindsには「同時に使用するバインダIDの最大数」を指定します。<br>
		例えば、 ::criFsBinder_BindCpk 関数と ::criFsBinder_Unbind 関数を交互に続けて実行するケースにおいては、
		最大同時には1つのバインダIDしか使用しないため、関数の呼び出し回数に関係なくmax_bindsに1を指定することが可能です。<br>
		逆に、ある場面でバインダIDを10個使用する場合には、その他の場面でバインドを一切行わない場合であっても、
		max_bindsに10を指定する必要があります。<br>
		\par 備考:
		CRI File Systemライブラリは、使用するバインダIDの数分だけのメモリを初期化時に要求します。<br>
		そのため、max_bindsに必要最小限の値をセットすることで、ライブラリが必要とするメモリのサイズを抑えることが可能です。<br>
		\sa criFsBinder_BindCpk, criFsBinder_BindFile, criFsBinder_BindFiles, criFsBinder_BindDirectory, criFsBinder_Unbind
	*/
	CriSint32 max_binds;
	/*JP
		\brief 最大同時オープンファイル数
		\par 説明:
		アプリケーション中でオープンするファイルの数を指定します。<br>
		アプリケーション中で ::criFsStdio_OpenFile 関数等を使用してファイルをオープンする場合、
		本パラメータにオープンするファイルの数を指定する必要があります。<br>
		<br>
		max_filesには「同時にオープンするファイルの最大数」を指定します。<br>
		例えば、 ::criFsStdio_OpenFile 関数と ::criFsStdio_CloseFile 関数を交互に続けて実行するケースにおいては、
		最大同時には1つのファイルしかオープンしないため、関数の呼び出し回数に関係なくmax_filesに1を指定することが可能です。<br>
		逆に、ある場面でファイルを10個オープンする場合には、その他の場面でファイルを1つしかオープンしない場合であっても、
		max_filesに10を指定する必要があります。<br>
		\par 補足:
		CRI File Systemライブラリは、以下の関数を実行した場合にファイルをオープンします。<br>
		\table "ファイルがオープンされる場面" align=center border=1 cellspacing=0 cellpadding=4
		{関数					|備考	}
		[criFsBinder_BindCpk	|オープンされるファイルの数は1つ。<br> criFsBinder_Unbind 関数が実行されるまでの間ファイルはオープンされ続ける。	]
		[criFsBinder_BindFile	|オープンされるファイルの数は1つ。<br> criFsBinder_Unbind 関数が実行されるまでの間ファイルはオープンされ続ける。	]
		[criFsBinder_BindFiles	|リストに含まれる数分ファイルがオープンされる。<br> criFsBinder_Unbind 関数が実行されるまでファイルはオープンされ続ける。	]
		[criFsLoader_Load		|オープンされるファイルの数は1つ。<br> ロードが完了するまでの間ファイルはオープンされ続ける。<br> バインダを指定した場合、ファイルはオープンされない（バインダが既にオープン済みのため）。	]
		[criFsStdio_OpenFile	|オープンされるファイルの数は1つ。<br> criFsStdio_CloseFile 関数が実行されるまでの間ファイルはオープンされ続ける。<br> バインダを指定した場合、ファイルはオープンされない（バインダが既にオープン済みのため）。	]
		[criFsInstaller_Copy	|オープンされるファイルの数は2つ。<br> ファイルコピーが完了するまでの間ファイルはオープンされ続ける。<br> バインダを指定した場合、オープンされるファイルは1つになる（1つをバインダが既にオープン済みのため）。	]
		\endtable
		\attention
		ブリッジライブラリを使用してADXライブラリや救声主ライブラリを併用する場合、
		ADXTハンドルやcriSsPlyハンドルは内部的にCriFsStdioハンドルを作成します。<br>
		そのため、ブリッジライブラリを使用する場合には、CRI File Systemライブラリ初期化時に
		max_filesにADXTハンドルやcriSsPlyハンドルの数を加えた値を指定してください。<br>
	*/
	CriSint32 max_files;
	/*JP
		\brief パスの最大長（バイト単位）
		\par 説明:
		アプリケーション中で指定するファイルパスの最大長を指定します。<br>
		アプリケーション中で ::criFsLoader_Load 関数等を使用してファイルにアクセスする場合、
		本パラメータにアプリケーションで使用するパス文字列の最大長を指定する必要があります。<br>
		<br>
		max_pathには「使用するパス文字列の最大数」を指定します。<br>
		ある場面で256バイトのファイルパスを使用する場合、その他の場面で32バイトのファイルパスしか使わない場合でも、
		max_pathには256を指定する必要があります。<br>
		\par 備考:
		パスの最大長には、終端のNULL文字を含んだ数を指定する必要があります。<br>
		（「文字数＋１バイト」の値を指定する必要があります。）<br>
		\attention
		PC等、ユーザがアプリケーションを自由な場所にインストール可能な場合には、想定される最大サイズを max_path に指定する必要がありますので、ご注意ください。<br>
	*/
	CriSint32 max_path;
} CriFsConfig;

/*JP
 * \brief ファイルオープンエラー発生時のリトライ方法
 */
typedef enum {
	CRIFS_OPEN_RETRY_NONE = 0,			/*JP< リトライしない */
	CRIFS_OPEN_RETRY_INFINITE = -1,		/*JP< 無限にリトライする */
	/* enum be 4bytes */
	CRIFS_OPEN_RETRY_ENUM_BE_SINT32 = 0x7FFFFFFF
} CriFsOpenRetryMode;

/*JP
 * \brief ファイルリードエラー発生時のリトライ方法
 */
typedef enum {
	CRIFS_READ_RETRY_NONE = 0,			/*JP< リトライしない */
	CRIFS_READ_RETRY_INFINITE = -1,		/*JP< 無限にリトライする */
	/* enum be 4bytes */
	CRIFS_READ_RETRY_ENUM_BE_SINT32 = 0x7FFFFFFF
} CriFsReadRetryMode;

/*JP
 * \brief メモリ確保関数
 * \ingroup FSLIB_CRIFS
 * \param[in]	obj		ユーザ指定オブジェクト
 * \param[in]	size	要求メモリサイズ（バイト単位）
 * \return		void*	確保したメモリのアドレス（失敗時はNULL）
 * \par 説明:
 * メモリ確保関数登録用のインターフェースです。<br>
 * CRI File Systemライブラリがライブラリ内で行なうメモリ確保処理を、
 * ユーザ独自のメモリ確保処理に置き換えたい場合に使用します。<br>
 * \par 備考:
 * コールバック関数が実行される際には、sizeに必要とされるメモリのサイズがセット
 * されています。<br>
 * コールバック関数内でsize分のメモリを確保し、確保したメモリのアドレスを
 * 戻り値として返してください。<br>
 * 尚、引数の obj には、::criFs_SetUserMallocFunction 関数で登録したユーザ指定
 * オブジェクトが渡されます。<br>
 * メモリ確保時にメモリマネージャ等を参照する必要がある場合には、
 * 当該オブジェクトを ::criFs_SetUserMallocFunction 関数の引数にセットしておき、
 * 本コールバック関数の引数を経由して参照してください。<br>
 * \attention
 * メモリの確保に失敗した場合、エラーコールバックが返されたり、呼び出し元の関数が
 * 失敗する可能性がありますのでご注意ください。
 * \sa CriFsFreeFunc, criFs_SetUserMallocFunction
 */
typedef void *(*CriFsMallocFunc)(void *obj, CriUint32 size);

/*JP
 * \brief メモリ解放関数
 * \ingroup FSLIB_CRIFS
 * \param[in]	obj		ユーザ指定オブジェクト
 * \param[in]	mem		解放するメモリアドレス
 * \return				なし
 * \par 説明:
 * メモリ解放関数登録用のインターフェースです。<br>
 * CRI File Systemライブラリ内がライブラリ内で行なうメモリ解放処理を、
 * ユーザ独自のメモリ解放処理に置き換えたい場合に使用します。<br>
 * \par 備考:
 * コールバック関数が実行される際には、memに解放すべきメモリのアドレスがセット
 * されています。<br>
 * コールバック関数内でmemの領域のメモリを解放してください。
 * 尚、引数の obj には、::criFs_SetUserFreeFunction 関数で登録したユーザ指定
 * オブジェクトが渡されます。<br>
 * メモリ確保時にメモリマネージャ等を参照する必要がある場合には、
 * 当該オブジェクトを ::criFs_SetUserFreeFunction 関数の引数にセットしておき、
 * 本コールバック関数の引数を経由して参照してください。<br>
 * \sa criFsMallocFunc, criFs_SetUserFreeFunction
 */
typedef void (*CriFsFreeFunc)(void *obj, void *mem);

/*==========================================================================
 *      CriFsIo API
 *=========================================================================*/
/*JP
 * \brief デバイスID
 */
/*EN
 * \brief Device ID
 */
typedef enum {
	CRIFS_DEFAULT_DEVICE = 0,       /*JP< デフォルトデバイス */
#if defined(XPT_TGT_XBOX360)
	CRIFS_DEVICE_XBOX360_DVD = 0,   /* Xbox360 DVD */
	CRIFS_DEVICE_XBOX360_HDD = 1,   /* Xbox360 HDD */
	CRIFS_DEVICE_XBOX360_MU0 = 2,   /* Xbox360 MU0 */
	CRIFS_DEVICE_XBOX360_MU1 = 3,   /* Xbox360 MU1 */
#elif defined(XPT_TGT_PS3PPU)
	CRIFS_DEVICE_PS3_HOSTFS = 0,
	CRIFS_DEVICE_PS3_CFS = 1,
	CRIFS_DEVICE_PS3_DISCFS = 2,
	CRIFS_DEVICE_PS3_FAT = 3,
#elif defined(XPT_TGT_WII)
	CRIFS_DEVICE_WII_DVD = 0,       /* ディスクアプリ */
	CRIFS_DEVICE_WII_CNT = 0,       /* NANDアプリ     */
	CRIFS_DEVICE_WII_NAND = 1,
#elif defined(XPT_TGT_NITRO)
	CRIFS_DEVICE_NITRO_ROM = 0,
	CRIFS_DEVICE_NITRO_MCS = 1,
#elif defined(XPT_TGT_PSP)
	//CRIFS_DEVICE_PSP_UMD_MS = 0,
	//CRIFS_DEVICE_PSP_HOST = 1,
	CRIFS_DEVICE_PSP_UMD_MS_HOST = 0,  /* UMD, MemoryStick, Host */
#elif defined(XPT_TGT_EE)
	CRIFS_DEVICE_PS2_CDVD = 0,
	CRIFS_DEVICE_PS2_HOST = 1,
#endif
	CRIFS_DEVICE_MEMORY,            /*JP< メモリ */
	CRIFS_DEVICE_INVALID = -1,      /*JP< 無効 */

	/* enum be 4bytes */
	CRIFS_DEVICE_ENUM_BE_SINT32 = 0x7FFFFFFF
} CriFsDeviceId;

/*JP
 * \brief デバイス情報
 */
/*EN
 * \brief Device Information
 */
typedef struct {
	CriBool can_read;					/*JP< 読み込み可能なデバイスかどうか				*/
	CriBool can_write;					/*JP< 書き込み可能なデバイスかどうか				*/
	CriBool can_seek;					/*JP< シーク可能なデバイスかどうか					*/
	CriSint32 minimum_read_size;		/*JP< 読み込み可能な最小単位のサイズ				*/
	CriSint32 minimum_write_size;		/*JP< 書き込み可能な最小単位のサイズ				*/
	CriSint32 minimum_seek_size;		/*JP< シーク可能な最小単位のサイズ					*/
	CriSint32 read_buffer_alignment;	/*JP< 読み込み先バッファに要求されるアライメント	*/
	CriSint32 write_buffer_alignment;	/*JP< 書き込み先バッファに要求されるアライメント	*/
} CriFsDeviceInfo;

/*JP
 * \brief ファイルオープンモード
 */
/*EN
 * \brief File Opening Mode
 */
typedef enum {
	CRIFS_FILE_MODE_APPEND			= 0,	/*JP< 既存ファイルに追記								*/	/*EN< Appends to an existing file						*/
	CRIFS_FILE_MODE_CREATE			= 1,	/*JP< ファイルの新規作成（既存のファイルは上書き）		*/	/*EN< Creates a new file always							*/
	CRIFS_FILE_MODE_CREATE_NEW		= 2,	/*JP< ファイルの新規作成（上書き不可）					*/	/*EN< Creates a new file (Can not overwrite)			*/
	CRIFS_FILE_MODE_OPEN			= 3,	/*JP< 既存ファイルのオープン							*/	/*EN< Opens an existing file							*/
	CRIFS_FILE_MODE_OPEN_OR_CREATE	= 4,	/*JP< ファイルのオープン（存在しない場合は新規作成）	*/	/*EN< Opens a file if available (Or creates new file)	*/
	CRIFS_FILE_MODE_TRUNCATE		= 5,	/*JP< 既存ファイルを0Byteに切り詰めてオープン			*/	/*EN< Opens a file and truncates it						*/

													/* 特殊 */
	CRIFS_FILE_MODE_OPEN_WITHOUT_DECRYPTING	= 10,	/*JP< ファイルのオープン（暗号化解除しない）	*/	/*EN< Opens a file without decrypting	*/

	/* enum be 4bytes */
	CRIFS_FILE_MODE_ENUM_BE_SINT32 = 0x7FFFFFFF
} CriFsFileMode;

/*JP
 * \brief ファイルアクセス種別
 */
/*EN
 * \brief Kind of File Access
 */
typedef enum {
	CRIFS_FILE_ACCESS_READ			= 0,	/*JP< 読み込みのみ		*/	/*EN< Read Only			*/
	CRIFS_FILE_ACCESS_WRITE			= 1,	/*JP< 書き込みのみ		*/	/*EN< Write Only		*/
	CRIFS_FILE_ACCESS_READ_WRITE	= 2,	/*JP< 読み書き			*/	/*EN< Read and Write	*/

	/* enum be 4bytes */
	CRIFS_FILE_ACCESS_ENUM_BE_SINT32 = 0x7FFFFFFF
} CriFsFileAccess;

/*JP
 * \brief I/Oインターフェースのエラーコード
 */
/*JP
 * \brief Error of I/O Interface
 */
typedef enum {
	CRIFS_IO_ERROR_OK			=   0,	/*JP< エラーなし */
	CRIFS_IO_ERROR_NG			=  -1,	/*JP< 一般エラー */
	CRIFS_IO_ERROR_TRY_AGAIN	=  -2,	/*JP< リトライすべき */

										/* 特殊 */
	CRIFS_IO_ERROR_NG_NO_ENTRY	= -11,	/*JP< 個別エラー（ファイル無し） */

	/* enum be 4bytes */
	CRIFS_IO_ERROR_ENUM_BE_SINT32 = 0x7FFFFFFF
} CriFsIoError;

/*JP
 * \brief ファイルハンドル
 */
/*EN
 * \brief File Handle
 */
typedef void *CriFsFileHn;

/*JP
 * \brief I/Oインターフェース
 */
/*EN
 * \brief I/O Interface
 */
typedef struct {
	/*JP
	 * \brief ファイルの有無の確認
	 * \param[in]	path	ファイルのパス
	 * \param[out]	result	ファイルが存在するかどうか
	 * \return	CriFsIoError	エラーコード
	 * \par 説明:
	 * 指定されたファイルの有無を確認する関数です。<br>
	 * ファイルが存在する場合は CRI_TRUE を、
	 * 存在しない場合は CRI_FALSE を result にセットする必要があります。<br>
	 */
	CriFsIoError (*Exists)(const CriChar8 *path, CriBool *result);
	
	/*JP
	 * \brief ファイルの削除
	 * \param[in]	path	ファイルのパス
	 * \return	CriFsIoError	エラーコード
	 * \par 説明:
	 * 指定されたファイルを削除する関数です。<br>
	 * \par 備考:
	 * デバイスで書き込みを行なわない場合には、この関数を実装せず、
	 * 構造体のメンバに CRI_NULL を指定することも可能です。<br>
	 */
	CriFsIoError (*Remove)(const CriChar8 *path);
	
	/*JP
	 * \brief ファイル名の変更
	 * \param[in]	path	リネーム前のファイルのパス
	 * \param[in]	path	リネーム後のファイルのパス
	 * \return	CriFsIoError	エラーコード
	 * \par 説明:
	 * ファイル名の変更を行なう関数です。<br>
	 * old_path で指定されたファイルを、 new_path にリネームします。<br>
	 * \par 備考:
	 * デバイスで書き込みを行なわない場合には、この関数を実装せず、
	 * 構造体のメンバに CRI_NULL を指定することも可能です。<br>
	 */
	CriFsIoError (*Rename)(const CriChar8 *old_path, const CriChar8 *new_path);
	
	/*JP
	 * \brief ファイルのオープン
	 * \param[in]	path	ファイルのパス
	 * \param[in]	mode	ファイルオープンモード
	 * \param[in]	access	ファイルアクセス種別
	 * \param[out]	filehn	ファイルハンドル
	 * \return	CriFsIoError	エラーコード
	 * \par 説明:
	 * 指定されたファイルをオープンする関数です。<br>
	 * オープンに成功した場合、CriFsFileHn 型のファイルハンドルを返す必要があります。<br>
	 * \par 補足:
	 * CriFsFileHn は void ポインタとして定義されています。<br>
	 * 独自のファイル情報構造体を定義し、そのアドレスを CriFsFileHn 型にキャストして返してください。<br>
	 * 尚、ファイルオープン時にメモリの確保が必要な場合には、本関数内で動的にメモリの確保を行なってください。<br>
	 */
	CriFsIoError (*Open)(
		const CriChar8 *path, CriFsFileMode mode, CriFsFileAccess access, CriFsFileHn *filehn);
	
	/*JP
	 * \brief ファイルのクローズ
	 * \param[in]	filehn	ファイルハンドル
	 * \return	CriFsIoError	エラーコード
	 * \par 説明:
	 * 指定されたファイルハンドルをクローズする関数です。<br>
	 * ファイルオープン時に動的にメモリの確保を行なった場合は、クローズ時にメモリを解放してください。<br>
	 */
	CriFsIoError (*Close)(CriFsFileHn filehn);
	
	/*JP
	 * \brief ファイルサイズの取得
	 * \param[in]	filehn	ファイルハンドル
	 * \param[out]	file_size	ファイルサイズ
	 * \return	CriFsIoError	エラーコード
	 * \par 説明:
	 * 指定されたファイルハンドルから、当該ファイルのサイズを取得する関数です。<br>
	 * \attention
	 * この関数はメインスレッド上から直接実行される可能性があります。<br>
	 * そのため、この関数の中で長時間処理をブロックすることは避ける必要があります。<br>
	 * ファイルハンドルからファイルサイズを取得するのに時間がかかる場合には、
	 * ファイルオープン時にあらかじめファイルサイズを取得（ファイルハンドル内に保持）
	 * しておき、本関数実行時にその値を返すよう関数を実装してください。<br>
	 */
	CriFsIoError (*GetFileSize)(CriFsFileHn filehn, CriSint64 *file_size);
	
	/*JP
	 * \brief 読み込みの開始
	 * \param[in]	filehn	ファイルハンドル
	 * \param[in]	offset	読み込み開始位置
	 * \param[in]	read_size	読み込みサイズ
	 * \param[in]	buffer	読み込み先バッファ
	 * \param[in]	buffer	バッファサイズ
	 * \return	CriFsIoError	エラーコード
	 * \par 説明:
	 * データの読み込みを開始する関数です。<br>
	 * offset で指定された位置から、 read_size で指定されたサイズ分だけデータを
	 * buffer に読み込みます。<br>
	 * 関数のインターフェースとしては非同期I/O処理による実装を想定していますが、
	 * スレッドを使用する場合（スレッドモデルに CRIFS_THREAD_MODEL_MULTI を指定する場合）
	 * には、この関数を同期I/O処理を使って実装しても問題ありません。<br>
	 * （関数内でファイルの読み込みを完了するまで待っても問題ありません。）<br>
	 * \attention
	 * 実際に読み込めたサイズは、 GetReadSize 関数で返す必要があります。<br>
	 * 同期I/O処理により本関数を実装する場合でも、読み込めたサイズは GetReadSize 関数
	 * が実行されるまで、ファイルハンドル内に保持する必要があります。<br>
	 */
	CriFsIoError (*Read)(CriFsFileHn filehn, CriSint64 offset, CriSint64 read_size, void *buffer, CriSint64 buffer_size);
	
	/*JP
	 * \brief 読み込み完了チェック
	 * \param[in]	filehn	ファイルハンドル
	 * \param[out]	result	ファイルの読み込みが完了したかどうか
	 * \return	CriFsIoError	エラーコード
	 * \par 説明:
	 * ファイルの読み込みが完了したかどうかを確認する関数です。<br>
	 * ファイルの読み込みが完了した場合は CRI_TRUE を、
	 * 読み込み途中の場合は CRI_FALSE を result にセットする必要があります。<br>
	 * \attention
	 * リードエラーが発生した場合、 result に CRI_TRUE をセットし、
	 * 関数の戻り値は CRIFS_IO_ERROR_OK を返してください。
	 */
	CriFsIoError (*IsReadComplete)(CriFsFileHn filehn, CriBool *result);
	
	/*JP
	 * \brief ファイル読み込みのキャンセル発行
	 * \param[in]	filehn	ファイルハンドル
	 * \return	CriFsIoError	エラーコード
	 * \par 説明:
	 * デバイス側のファイル読み込みに対してキャンセルを発行し、即時に復帰する関数です。
	 * 戻り値は CRIFS_IO_ERROR_OK を返してください。<br>
	 * CRIFS_IO_ERROR_OK以外の値を返しても、
	 * CRI File Systemの動作はCRIFS_IO_ERROR_OKを返した場合と同じです。<br>
	 */
	CriFsIoError (*CancelRead)(CriFsFileHn filehn);
	
	/*JP
	 * \brief 読み込みサイズの取得
	 * \param[in]	filehn	ファイルハンドル
	 * \param[out]	read_size	読み込めたサイズ
	 * \return	CriFsIoError	エラーコード
	 * \par 説明:
	 * リード処理を行なった結果、実際にバッファに読み込めたデータのサイズを返す関数です。<br>
	 * ファイルの終端等では、 Read 関数で指定したサイズ分のデータが必ずしも読み込めるとは限りません。<br>
	 * \attention
	 * リードエラーが発生した場合、 read_size に -1 をセットし、
	 * 関数の戻り値は CRIFS_IO_ERROR_OK を返してください。
	 */
	CriFsIoError (*GetReadSize)(CriFsFileHn filehn, CriSint64 *read_size);
	
	/*JP
	 * \brief 書き込みの開始
	 * \param[in]	filehn	ファイルハンドル
	 * \param[in]	offset	書き込み開始位置
	 * \param[in]	write_size	書き込みサイズ
	 * \param[in]	buffer	書き込み先バッファ
	 * \param[in]	buffer	バッファサイズ
	 * \return	CriFsIoError	エラーコード
	 * \par 説明:
	 * データの書き込みを開始する関数です。<br>
	 * offset で指定された位置から、 write_size で指定されたサイズ分だけデータを
	 * buffer から書き込みます。<br>
	 * 関数のインターフェースとしては非同期I/O処理による実装を想定していますが、
	 * スレッドを使用する場合（スレッドモデルに CRIFS_THREAD_MODEL_MULTI を指定する場合）
	 * には、この関数を同期I/O処理を使って実装しても問題ありません。<br>
	 * （関数内でファイルの書き込みを完了するまで待っても問題ありません。）<br>
	 * \attention
	 * 実際に書き込めたサイズは、 GetWriteSize 関数で返す必要があります。<br>
	 * 同期I/O処理により本関数を実装する場合でも、書き込めたサイズは GetWriteSize 関数
	 * が実行されるまで、ファイルハンドル内に保持する必要があります。<br>
	 * \par 備考:
	 * デバイスで書き込みを行なわない場合には、この関数を実装せず、
	 * 構造体のメンバに CRI_NULL を指定することも可能です。<br>
	 */
	CriFsIoError (*Write)(CriFsFileHn filehn, CriSint64 offset, CriSint64 write_size, void *buffer, CriSint64 buffer_size);
	
	/*JP
	 * \brief 書き込み完了チェック
	 * \brief 書き込み完了チェック
	 * \param[in]	filehn	ファイルハンドル
	 * \param[out]	result	ファイルの書き込みが完了したかどうか
	 * \return	CriFsIoError	エラーコード
	 * \par 説明:
	 * ファイルの書き込みが完了したかどうかを確認する関数です。<br>
	 * ファイルの書き込みが完了した場合は CRI_TRUE を、
	 * 書き込み途中の場合は CRI_FALSE を result にセットする必要があります。<br>
	 * \attention
	 * ライトエラーが発生した場合、 result に CRI_TRUE をセットし、
	 * 関数の戻り値は CRIFS_IO_ERROR_OK を返してください。
	 * \par 備考:
	 * デバイスで書き込みを行なわない場合には、この関数を実装せず、
	 * 構造体のメンバに CRI_NULL を指定することも可能です。<br>
	 */
	CriFsIoError (*IsWriteComplete)(CriFsFileHn filehn, CriBool *result);
	
	/*JP
	 * \brief ファイル書き込みのキャンセル発行
	 * \param[in]	filehn	ファイルハンドル
	 * \param[out]	result	キャンセルが発行できたか
	 * \return	CriFsIoError	エラーコード
	 * \par 説明:
	 * デバイス側のファイル書き込みに対してキャンセルを発行し、即時に復帰する関数です。
	 * 戻り値は CRIFS_IO_ERROR_OK を返してください。<br>
	 * CRIFS_IO_ERROR_OK以外の値を返しても、
	 * CRI File Systemの動作はCRIFS_IO_ERROR_OKを返した場合と同じです。<br>
	 */
	CriFsIoError (*CancelWrite)(CriFsFileHn filehn);
	
	/*JP
	 * \brief 書き込みサイズの取得
	 * \param[in]	filehn	ファイルハンドル
	 * \param[out]	write_size	書き込めたサイズ
	 * \return	CriFsIoError	エラーコード
	 * \par 説明:
	 * ライト処理を行なった結果、実際にバッファに読み込めたデータのサイズを返す関数です。<br>
	 * \attention
	 * ライトエラーが発生した場合、 write_size に -1 をセットし、
	 * 関数の戻り値は CRIFS_IO_ERROR_OK を返してください。
	 * \par 備考:
	 * デバイスで書き込みを行なわない場合には、この関数を実装せず、
	 * 構造体のメンバに CRI_NULL を指定することも可能です。<br>
	 */
	CriFsIoError (*GetWriteSize)(CriFsFileHn filehn, CriSint64 *write_size);
	
	/*JP
	 * \brief フラッシュの実行
	 * \param[in]	filehn	ファイルハンドル
	 * \return	CriFsIoError	エラーコード
	 * \par 説明:
	 * 書き込み用にバッファリングされているデータを、
	 * 強制的にデバイスに書き出す処理を行う関数です。<br>
	 * （ ANSI C 標準の API では fflush 関数に相当する処理です。）<br>
	 * \par 備考:
	 * デバイスで書き込みを行なわない場合には、この関数を実装せず、
	 * 構造体のメンバに CRI_NULL を指定することも可能です。<br>
	 */
	CriFsIoError (*Flush)(CriFsFileHn filehn);
	
	/*JP
	 * \brief ファイルサイズの変更
	 * \param[in]	filehn	ファイルハンドル
	 * \param[out]	size	ファイルサイズ
	 * \return	CriFsIoError	エラーコード
	 * \par 説明:
	 * ファイルのサイズを指定したサイズに変更する関数です。<br>
	 * \par 補足:
	 * 本関数は、DMA転送サイズの制限等によりデバイスへの書き込みがバイト単位で
	 * 行なえない場合に、ファイルサイズを補正するために使用します。<br>
	 * そのため、書き込みがバイト単位で可能なデバイスについては、この関数を実装せず、
	 * 構造体のメンバに CRI_NULL を指定することも可能です。<br>
	 * \par 備考:
	 * デバイスで書き込みを行なわない場合には、この関数を実装せず、
	 * 構造体のメンバに CRI_NULL を指定することも可能です。<br>
	 */
	CriFsIoError (*Resize)(CriFsFileHn filehn, CriSint64 size);

	/*JP
	 * \brief ネイティブファイルハンドルの取得
	 * \param[in]	filehn	ファイルハンドル
	 * \param[out]	native_filehn	ネイティブのファイルハンドル
	 * \return	CriFsIoError	エラーコード
	 * \par 説明:
	 * プラットフォームSDKで利用されるファイルのハンドルを取得する関数です。<br>
	 * 例えば、 ANSI C 標準の fopen 関数を使用してファイルをオープンした場合、
	 * native_filehn としてファイルポインタ（ FILE * ）を返す必要があります。<br>
	 * \par 備考:
	 * 現状、PLAYSTATION3以外の機種ではこの関数を実装する必要はありません。<br>
	 */
	CriFsIoError (*GetNativeFileHandle)(CriFsFileHn filehn, void **native_filehn);
} CriFsIoInterface, *CriFsIoInterfacePtr;

/*JP
 * \brief I/O選択コールバック関数
 * \param[in]	path	ファイルのパス
 * \param[out]	device_id	デバイスID
 * \param[out]	ioif	I/Oインターフェース
 * \par 説明:
 * I/O選択コールバック関数は、CRI File SystemライブラリのI/O処理を、
 * ユーザの独自I/Oイーターフェースで置き換える際に使用します。<br>
 * 具体的には、ユーザは ::CriFsSelectIoCbFunc 型の関数を実装し、
 * その関数を ::criFsSetSelectIoCallback 関数にセットする必要があります。<br>
 * ::CriFsSelectIoCbFunc 関数は、入力されたファイルのパス（引数のpath）を解析し、
 * そのファイルが存在するデバイスのID（引数のdevice_id）と、
 * デバイスにアクセスするためのI/Oインターフェース（引数のioif）を返す必要があります。<br>
 * \par 補足:
 * ライブラリがデフォルト状態で利用するI/Oインターフェースは、 ::criFs_GetDefaultIoInterface 関数で取得可能です。<br>
 * 特定のファイルのみを独自のI/Oインターフェースを処理したい場合には、
 * 他のファイルを全て ::criFs_GetDefaultIoInterface 関数で取得したI/Oインターフェースで処理してください。<br>
 * \code
 * CriError 
 * \endcode
 * \sa criFs_SetSelectIoCallback, criFs_GetDefaultIoInterface
 */
typedef CriError (*CriFsSelectIoCbFunc)(
	const CriChar8 *path, CriFsDeviceId *device_id, CriFsIoInterfacePtr *ioif);

/*==========================================================================
 *      CriFsBinder API
 *=========================================================================*/
/*JP
 * \brief CriFsBinderハンドル
 * \struct CriFsBinderHn
 * \ingroup FSLIB_BINDER
 * \par 説明：
 * バインダとは、ファイルを効率良く扱うためのデータベースです。<br>
 * - CriFsBinderHn (バインダハンドル)とバインド<br>
 * バインダを利用するには、バインダハンドル( CriFsBinderHn )を作成し、
 * CPKファイル／ファイル／ディレクトリをバインダに結びつけます。
 * このバインダへの結び付けをバインドと呼びます。<br>
 * バインダを作成すると、バインダハンドル( CriFsBinderHn )が取得されます。<br>
 * - CriFsBinderId （バインダID）<br>
 * バインダにバインドを行うと、バインダIDが作成されます。個々のバインドを識別するために使用します。<br>
 * - ファイルのバインドとアンバインド<br>
 * バインダには、CPKファイルやファイル、ディレクトリをどのような組み合わせででもバインドできます。<br>
 * バインドした項目のバインド状態を解除することをアンバインドと呼びます。<br>
 * - 利用できるバインド数<br>
 * 作成できるバインダ数や同時にバインドできる最大数は、 CriFsConfig の
 * num_binders (バインダ数)や max_binds (同時バインド可能な最大数)で指定します。<br>
 * - CPKファイルのバインド<br>
 * CPKファイルに収納されている個々のファイル（コンテンツファイル）にアクセスするには、
 * CPKファイルをバインドする必要があります。<br>
 * CPKファイルのコンテンツファイルもバインドできます。元のCPKファイルをアンバインドした場合、
 * バインドされているコンテンツファイルもアンバインドされます（暗黙的アンバインド）。<br>
 * - バインダのプライオリティ<br>
 * バインダは、目的のファイルがどのバインダIDにあるのかを検索します。<br>
 * このバインダIDの検索順は、基本的にはバインドされた順番になりますが、バインダIDのプライオリティを
 * 操作することで、検索順を変更することができます。<br>
 * - バインダとCriFsのAPI<br>
 * CriFsLoader, CriFsGroupLoader, CriFsBinderには、バインダを引数に持つAPIがあります。
 * その際には、 CriFsBinderHn と CriFsBinderId 、どちらを指定するのかに注意してください。
 */
struct _CriFsBinderHnObj;
typedef struct _CriFsBinderHnObj *CriFsBinderHn;

/*JP
 * \brief CriFsBinder ID
 * \struct CriFsBinderId
 * \ingroup FSLIB_BINDER
 * \par 説明：
 * バインダに対してバインドを行うと、 CriFsBinderId (バインダID)が作成されます。<br>
 * バインダIDは、個々のバインドを識別するためのもので、値は符号なし32ビット値の範囲を
 * とります。
 */
typedef CriUint32 CriFsBinderId;

/*JP
 * \brief CriFsBinder ID
 * \ingroup FSLIB_BINDER
 * \par 説明：
 * バインダIDは、個々のバインドを識別するためのもので、各バインダIDにユニークに割り振られます。
 * 値は符号なし32ビット値の範囲となります。
 * CRIFSBINDER_BID_NULL は未使用バインダに与えられるIDです。<br>
 */
enum {
	CRIFSBINDER_BID_NULL = 0,	/*JP< id 0番は NULL ID とする */
	CRIFSBINDER_BID_START,		/*JP< 実際の番号割り振りは１から始まる */

	/* enum be 4bytes */
	CRIFSBINDER_BID_ENUM_BE_SINT32 = 0x7FFFFFFF
};


/*JP
 * \brief メモリ確保コールバック関数
 * \ingroup FSLIB_BINDER
 * \param[in]	obj		メモリ管理オブジェクト
 * \param[in]	size	要求メモリサイズ(byte)
 * \return				メモリ確保成功：確保したメモリの先頭アドレス、メモリ確保失敗：NULL
 * \par 説明
 * 各種バインド関数に渡すワークアドレスをNULLとした場合や、CPK解析エンジンがメモリを確保する際に呼ばれます。<br>
 * \par 例：
 * \code
 * void *u_alloc(void *obj, CriSint32 size)
 * {
 *  CriUint8 *ptr;
 * 	ptr = user_alloc(obj, size);  // Users memory allocate function
 *	return ptr;
 * }
 * \endcode
 * \sa criFsBinder_SetUserHeapFunc()
 */
typedef void *(*CriFsBinderUserHeapAllocateCbFunc)(void *obj, CriSint32 size);

/*JP
 * \brief メモリ解放コールバック関数
 * \ingroup FSLIB_BINDER
 * \par メモリ解放関数
 * \param[in]	obj		メモリ管理オブジェクト
 * \param[in]	ptr		解放するメモリへのポインタ
 * \par 説明
 * バインド時にCriFsBinderUserHeapAllocateCbFuncで確保したメモリを解放する際に呼ばれます。
 * \par 例：
 * \code
 * void u_free(void *obj, void *ptr)
 * {
 * 	user_free_mem(obj, ptr);  // Users memory free function
 * }
 * \endcode
 * \sa criFsBinder_SetUserHeapFunc()
 */
typedef void (*CriFsBinderUserHeapFreeCbFunc)(void *obj, void *ptr);

/*JP
 * \brief ファイル情報構造体
 * \struct CriFsBinderFileInfo
 * \ingroup FSLIB_BINDER
 * \par 説明：
 * criFsBinder_Find(ById) 関数の出力情報です。
 * 検索したファイルにアクセスするための情報を格納します。<br>
 * CPKファイルのコンテンツファイルである場合、パス名にはCPKファイル名が格納されます。
 * \sa criFsBinder_Find(), criFsBinder_FindById()
 */
typedef struct {
	CriFsFileHn filehn;		/*JP< ファイルハンドラ */
	CriSint64 offset;		/*JP< オフセット */
	CriUint32 read_size;   	/*JP< 読込サイズ */
	CriUint32 extract_size;	/*JP< 展開サイズ：非圧縮の場合は読込サイズと同値になる */
	CriChar8 *path;			/*JP< パス名 */
	CriFsBinderId binderid; /*JP< 依存バインダ */
} CriFsBinderFileInfo;

/*JP
 * \brief コンテンツファイル情報構造体
 * \struct CriFsBinderContentsFileInfo
 * \ingroup FSLIB_BINDER
 * \par 説明：
 * criFsBinder_GetContensFileInfoById 関数の出力情報です。<br>
 * 検索したCPKファイルのコンテンツファイルにアクセスするための情報を格納します。<br>
 * \sa criFsBinder_GetContentsFileInfoById()
 */
typedef struct {
	CriChar8 *directory;	/*JP< ディレクトリ名 */
	CriChar8 *filename;		/*JP< ファイル名 */
	CriUint32 read_size; 	/*JP< 読込サイズ */
	CriUint32 extract_size;	/*JP< 展開サイズ：非圧縮の場合は読込サイズと同値になる */
	CriUint64 offset;		/*JP< オフセット */
	CriUint32 id;			/*JP< ファイルID */
	CriChar8 *ustr;
} CriFsBinderContentsFileInfo;

/*JP
 * \brief バインダステータス
 * \ingroup FSLIB_BINDER
 * \par 説明：
 * criFsBinder_GetStatus 関数で取得される、バインダIDの状態です。<br>
 * バインドが完了するまで、バインドした項目にアクセスすることはできません。<br>
 * バインド対象が存在しなかったり、バインドに必要なリソースが不足する場合は、
 * バインド失敗となります。<br>
 * バインド失敗時の詳しい情報はエラーコールバック関数で取得してください。
 * \sa criFsBinder_GetStatus()
 */
typedef enum {
	CRIFSBINDER_STATUS_NONE = 0,
	CRIFSBINDER_STATUS_ANALYZE,		/*JP< バインド処理中 */
	CRIFSBINDER_STATUS_COMPLETE,	/*JP< バインド完了 */
	CRIFSBINDER_STATUS_INVALID,		/*JP< バインド無効 */
	CRIFSBINDER_STATUS_ERROR,		/*JP< バインド失敗 */

	/* enum be 4bytes */
	CRIFSBINDER_STATUS_ENUM_BE_SINT32 = 0x7FFFFFFF
} CriFsBinderStatus;

/*JP
 * \brief バインド種別
 * \ingroup FSLIB_BINDER
 * \par 説明：
 * 何がバインドされているのかを示します。<br>
*/
typedef enum {
	CRIFSBINDER_KIND_NONE = 0, /*JP< なし */
	CRIFSBINDER_KIND_DIRECTORY, /*JP< ディレクトリバインド */
	CRIFSBINDER_KIND_CPK, /*JP< CPKバインド */
	CRIFSBINDER_KIND_CPKDOUBLE, /*JP< CPKバインド */
	CRIFSBINDER_KIND_FILE, /*JP< ファイルバインド */
	CRIFSBINDER_KIND_FILES, /*JP< 複数ファイルバインド */
	CRIFSBINDER_KIND_SYSTEM, /*JP< バインダーシステム関連 */

	/* enum be 4bytes */
	CRIFSBINDER_KIND_ENUM_BE_SINT32 = 0x7FFFFFFF
} CriFsBinderKind;

/*JP
 * \brief PrimaryCpk無効時のエラー種別
 * \ingroup FSLIB_BINDER
 * \par 説明：
 * PrimaryCpkが無効になった際の原因を示します。<br>
*/
typedef enum {
	CRIFS_PRIMARYCPK_ERROR_NONE = 0,		/*JP< エラー無し */
	CRIFS_PRIMARYCPK_ERROR_CRC,				/*JP< CRC不整合 */
	CRIFS_PRIMARYCPK_ERROR_CANNOT_READ,		/*JP< 読めない（メディアが無いなど） */
	CRIFS_PRIMARYCPK_ERROR_NONEXISTENT,		/*JP< (CPK)ファイルが無い（メディアは存在する）*/
	/* enum be 4bytes */
	CRIFS_PRIMARYCPK_ENUM_BE_SINT32 = 0x7FFFFFFF
} CriFsPrimaryCpkError;


/*JP
 * \brief バインダ情報
 * \struct CriFsBinderInfo
 * \ingroup FSLIB_BINDER
 * \par 説明：
 * バインダ情報取得APIの出力情報です。<br>
 * \sa criFsBinder_GetBinderIdInfo()
*/
typedef struct {
	CriFsBinderKind kind;				/*JP< バインダ種別 */
	CriFsBinderStatus status;			/*JP< バインダステータス */
	CriSint32 priority;					/*JP< プライオリティ設定 */
	CriSint32 nfiles;					/*JP< ファイル数<br>
										  バインダ種別<br>
										  CIFSBINDER_KIND_FILES:バインドファイル数<br>
										  CRIFSBINDER_KIND_CPK:コンテンツファイル数<br>
										  他：０
										*/
	const CriChar8 *path;				/*JP< Bind関数呼び出し時に渡されたパス名 */
	const CriChar8 *real_path;			/*JP< 実際にバインドする対象となったパス名 */
	const CriChar8 *current_directory;	/*JP< カレントディレクトリ設定 */
	CriFsBinderId bndrid;				/*JP< 他バインダを参照している場合の参照先バインダID<br>
											CPKのコンテンツファイルをファイルバインドした場合、
											CPKのバインダIDが入ります。 */
} CriFsBinderInfo;


/*==========================================================================
 *      CriFsLoader API
 *=========================================================================*/
/*JP
 * \brief CriFsLoaderハンドル
 */
struct _CriFsLoaderObj;
typedef struct _CriFsLoaderObj *CriFsLoaderHn;

/*JP
 * \brief ロード終了コールバック関数
 */
typedef void (*CriFsLoaderLoadEndCbFunc)(void *obj, CriFsLoaderHn loader);

/*JP
 * \brief ロードステータス
 */
/*EN
 * \brief Loading Status
 */
typedef enum {
	CRIFSLOADER_STATUS_STOP,		/*JP< 停止中		*/	/*EN< Stopping			*/
	CRIFSLOADER_STATUS_LOADING,		/*JP< ロード中		*/	/*EN< Loading			*/
	CRIFSLOADER_STATUS_COMPLETE,	/*JP< 完了			*/	/*EN< Complete			*/
	CRIFSLOADER_STATUS_ERROR,		/*JP< エラー		*/	/*EN< Error				*/
	/* enum be 4bytes */
	CRIFSLOADER_STATUS_ENUM_BE_SINT32 = 0x7FFFFFFF
} CriFsLoaderStatus;

/*JP
 * \brief ローダプライオリティ
 */
/*EN
 * \brief Priority
 */
typedef enum {
	CRIFSLOADER_PRIORITY_HIGHEST 		= 2,	/*JP< 最高		*/	/*EN< Highest		*/
	CRIFSLOADER_PRIORITY_ABOVE_NORMAL 	= 1,	/*JP< 高		*/	/*EN< Above normal	*/
	CRIFSLOADER_PRIORITY_NORMAL 		= 0,	/*JP< 普通		*/	/*EN< Normal		*/
	CRIFSLOADER_PRIORITY_BELOW_NORMAL 	= -1,	/*JP< 低		*/	/*EN< Below normal	*/
	CRIFSLOADER_PRIORITY_LOWEST 		= -2,	/*JP< 最低		*/	/*EN< Lowest		*/
	/* enum be 4bytes */
	CRIFSLOADER_PRIORITY_ENUM_BE_SINT32 = 0x7FFFFFFF
} CriFsLoaderPriority;

/*==========================================================================
 *      Group Loader API
 *=========================================================================*/
/*JP
 * \brief CriFsGroupLoader ハンドル
 * \par 説明：
 * CPKファイルのグループ情報を利用するには、グループローダを作成する必要があります。<br>
 * グループローダを作成すると、グループローダハンドルが返されます。<br>
 * グループローダへのアクセスは、グループローダハンドルを使用して行います。
 */
struct _CriFsGroupLoaderHnObj;
typedef struct _CriFsGroupLoaderHnObj *CriFsGroupLoaderHn;

/*JP
 * \brief グループファイル情報構造体
 * \ingroup FSLIB_GROUPLOADER
 * \par 説明：
 * グループローダで扱われる個々のファイルに関する情報です。
*/
typedef	struct {
	CriChar8	*directory;     /*JP< コンテンツファイルのディレクトリ名    */
	CriChar8	*filename;		/*JP< コンテンツファイルのファイル名        */
	CriUint32	filesize;		/*JP< コンテンツファイルのファイルサイズ	*/
	void		*datapointer;	/*JP< コンテンツファイルのデータポインタ	*/
	CriUint32	gfinfotag;		/*JP< コンテンツファイルの汎用タグ          */
	CriUint16	id;				/*JP< コンテンツファイルのID                */
} CriFsGroupFileInfo;

/*JP
 * \brief グループロードコールバック関数
 * \ingroup FSLIB_GROUPLOADER
 * \param[in]	obj		ユーザー登録オブジェクト
 * \param[in]	gfinfo	読み込むファイルの情報
 * \return		gfinfoで示されるファイルを読み込むバッファ領域へのポインタ（NULL:ロードのスキップ）
 * \par 説明：
 * グループロードコールバック関数は、グループローダ毎に設定します。<br>
 * グループロード時、ロードするファイル毎にロードの直前に呼出されます。<br>
 * 読み込むファイルのファイル名やサイズなどのファイル情報が gfinfoで渡されます。<br>
 * コールバック関数の返値はgfinfoで示されたファイルを読み込むバッファへのポインタとなります。
 * この返値がNULLの場合、そのファイルは読み込まれません。<br>
 * グループロードコールバック関数が設定されている場合、CPKファイルに設定されているグループ情報の
 * 読込アドレスは使用されません。
 * \sa criFsGroupLoader_SetLoadStartCallback()
 */
typedef void *(*CriFsGroupLoaderLoadStartCbFunc)(void *obj, const CriFsGroupFileInfo *gfinfo);

/*==========================================================================
 *      Log Output API
 *=========================================================================*/
/*JP
 * \brief アクセスログ出力モード
 */
typedef enum {
	CRIFS_LOGOUTPUT_MODE_DEFAULT,
	/* enum be 4bytes */
	CRIFS_LOGOUTPUT_MODE_ENUM_BE_SINT32 = 0x7FFFFFFF
} CriFsLogOutputMode;

/*JP
 * \brief ログ出力関数
 */
typedef void (*CriFsLogOutputFunc)(void *obj, const char* format, ...);

/*==========================================================================
 *      CriFsStdio API
 *=========================================================================*/
/*JP
 * \brief CriFsStdioハンドル
 */
struct _CriFsStdioObj;
typedef struct _CriFsStdioObj *CriFsStdioHn;

/* JP
 * \bref ファイル上のシーク開始位置
 */
typedef enum {
	CRIFSSTDIO_SEEK_SET = 0, /*JP< ファイルの先頭 */
	CRIFSSTDIO_SEEK_CUR = 1, /*JP< 現在の読込位置 */
	CRIFSSTDIO_SEEK_END = 2, /*JP< ファイルの終端 */
	/* enum be 4bytes */
	CRIFSSTDIO_SEEK_ENUM_BE_SINT32 = 0x7FFFFFFF
} CRIFSSTDIO_SEEK_TYPE;

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

/*==========================================================================
 *      CRI File System API
 *=========================================================================*/
/*JP
 * \brief デフォルトコンフィギュレーションのセット
 * \ingroup FSLIB_CRIFS
 * \param[in]	config	コンフィギュレーション
 * \par 説明:
 * ::criFs_InitializeLibrary 関数に設定するコンフィギュレーション（ ::CriFsConfig ）に、デフォルトの値をセットします。<br>
 * \par 補足:
 * コンフィギュレーションに設定する各パラメータを、アプリケーションで使用するハンドルの数に応じて調節することで、
 * ライブラリが必要とするメモリサイズを小さく抑えることが可能です。<br>
 * しかし、アプリケーション中で使用するハンドルの数が明確でない開発初期段階や、メモリサイズがタイトではないケースでは、
 * 本マクロを使用することによりで、初期化処理を簡略化することが可能です。<br>
 * \attention:
 * 本マクロでは、ほとんどのケースで必要充分な数のハンドルが確保できるよう、コンフィギュレーションの各パラメータに大きめの値をセットします。<br>
 * そのため、本マクロを使用した場合、ライブラリが必要とするワーク領域のサイズは大きくなりますので、ご注意ください。<br>
 * （メモリサイズがタイトなケースでは、本マクロを使用せず、コンフィギュレーションの各パラメータを個別に調節することをオススメいたします。）<br>
 * \sa
 * CriFsConfig
*/
#define criFs_SetDefaultConfig(p_config)	\
{\
	(p_config)->thread_model = CRIFS_THREAD_MODEL_MULTI;\
	(p_config)->num_binders = CRIFS_CONFIG_DEFAULT_NUM_BINDERS;\
	(p_config)->num_loaders = CRIFS_CONFIG_DEFAULT_NUM_LOADERS;\
	(p_config)->num_group_loaders = CRIFS_CONFIG_DEFAULT_NUM_GROUP_LOADERS;\
	(p_config)->num_stdio_handles = CRIFS_CONFIG_DEFAULT_NUM_STDIO_HANDLES;\
	(p_config)->num_installers = CRIFS_CONFIG_DEFAULT_NUM_INSTALLERS;\
	(p_config)->max_binds = CRIFS_CONFIG_DEFAULT_MAX_BINDS;\
	(p_config)->max_files = CRIFS_CONFIG_DEFAULT_MAX_FILES;\
	(p_config)->max_path = CRIFS_CONFIG_DEFAULT_MAX_PATH;\
}

/*JP
 * \brief ワーク領域サイズの計算
 * \ingroup FSLIB_CRIFS
 * \param[in]	config	コンフィギュレーション
 * \param[out]	nbyte	ワーク領域サイズ
 * \return	CriError	エラーコード
 * \par 説明:
 * CRI File Systemライブラリを使用するために必要な、ワーク領域のサイズを取得します。<br>
 * \par 備考:
 * ワーク領域のサイズはコンフィギュレーション（ ::CriFsConfig ）の内容によって変化します。<br>
 * ライブラリに割り当てるメモリサイズを削減したい場合には、コンフィギュレーションのパラメータを適宜調節してください。<br>
 * config に NULL を指定した場合、デフォルトのコンフィギュレーションが適用されます。
 * \sa CriFsConfig
 */
CriError CRIAPI criFs_CalculateWorkSizeForLibrary(const CriFsConfig *config, CriSint32 *nbyte);

/*JP
 * \brief CRI File Systemの初期化
 * \ingroup FSLIB_CRIFS
 * \param[in]	config	コンフィギュレーション
 * \param[in]	buffer	ワーク領域
 * \param[in]	size	ワーク領域サイズ
 * \return	CriError	エラーコード
 * \par 説明:
 * CRI File Systemライブラリを初期化します。<br>
 * ライブラリの機能を利用するには、必ずこの関数を実行する必要があります。<br>
 * （ライブラリの機能は、本関数を実行後、 ::criFs_FinalizeLibrary 関数を実行するまでの間、利用可能です。）<br>
 * ライブラリを初期化する際には、ライブラリが内部で利用するためのメモリ領域（ワーク領域）
 * を確保する必要があります。<br>
 * ワーク領域を確保する方法には、以下の2通りの方法があります。<br>
 * <b>(a) User Allocator方式</b>：メモリの確保／解放に、ユーザが用意した関数を使用する方法。<br>
 * <b>(b) Fixed Memory方式</b>：必要なメモリ領域を直接ライブラリに渡す方法。<br>
 * <br>
 * User Allocator方式を用いる場合、ユーザはCRI File Systemライブラリにメモリ確保関数を登録しておきます。<br>
 * workにNULL、sizeに0を指定して本関数を呼び出すことで、
 * ライブラリは登録済みのメモリ確保関数を使用して必要なメモリを自動的に確保します。<br>
 * ユーザがワーク領域を用意する必要はありません。<br>
 * 初期化時に確保されたメモリは、終了処理時（ ::criFs_FinalizeLibrary 関数実行時）に解放されます。<br>
 * <br>
 * Fixed Memory方式を用いる場合、ワーク領域として別途確保済みのメモリ領域を本関数に
 * 設定する必要があります。<br>
 * ワーク領域のサイズは ::criFs_CalculateWorkSizeForLibrary 関数で取得可能です。<br>
 * 初期化処理の前に ::criFs_CalculateWorkSizeForLibrary 関数で取得したサイズ分のメモリを予め
 * 確保しておき、本関数に設定してください。<br>
 * 尚、Fixed Memory方式を用いた場合、ワーク領域はライブラリの終了処理（ ::criFs_FinalizeLibrary 関数）
 * を行なうまでの間、ライブラリ内で利用され続けます。<br>
 * ライブラリの終了処理を行なう前に、ワーク領域のメモリを解放しないでください。<br>
 * \par 例:
 * 【User Allocator方式によるライブラリの初期化】<br>
 * User Allocator方式を用いる場合、ライブラリの初期化／終了の手順は以下の通りです。<br>
 * 	-# 初期化処理実行前に、 ::criFs_SetUserMallocFunction 関数と
 * ::criFs_SetUserFreeFunction 関数を用いてメモリ確保／解放関数を登録する。<br>
 * 	-# 初期化用コンフィグ構造体にパラメータをセットする。<br>
 * 	-# ::criFs_InitializeLibrary 関数で初期化処理を行う。<br>
 * （workにはNULL、sizeには0を指定する。）<br>
 * 	-# アプリケーション終了時に ::criFs_FinalizeLibrary 関数で終了処理を行なう。<br>
 * 	.
 * <br>具体的なコードは以下のとおりです。<br>
 * \code
 * // 独自のメモリ確保関数
 * void *user_malloc(void *obj, CriUint32 size)
 * {
 * 	void *mem;
 * 	
 * 	// メモリの確保
 * 	mem = malloc(size);
 * 	
 * 	return (mem);
 * }
 * 
 * // 独自のメモリ解放関数を用意
 * void user_free(void *obj, void *mem)
 * {
 * 	// メモリの解放
 * 	free(mem);
 * 	
 * 	return;
 * }
 * 
 * main()
 * {
 * 	CriFsConfig config;	// ライブラリ初期化用コンフィグ構造体
 * 		:
 * 	// 独自のメモリ確保関数を登録
 * 	criFs_SetUserMallocFunction(user_malloc, NULL);
 * 	
 * 	// 独自のメモリ解放関数を登録
 * 	criFs_SetUserFreeFunction(user_free, NULL);
 * 	
 * 	// ライブラリ初期化用コンフィグ構造体にデフォルト値をセット
 * 	criFs_SetDefaultConfig(&config);
 * 	
 * 	// ライブラリの初期化
 * 	// ワーク領域にはNULLと0を指定する。
 * 	// →必要なメモリは、登録したメモリ確保関数を使って確保される。
 * 	criFs_InitializeLibrary(&config, NULL, 0);
 * 		:
 * 	// アプリケーションのメイン処理
 * 		:
 * 	// アプリケーションを終了する際に終了処理を行う
 * 	// →初期化時に確保されたメモリは、登録したメモリ解放関数を使って解放される。
 * 	criFs_FinalizeLibrary();
 * 		:
 * }
 * \endcode
 * <br>
 * 【Fixed Memory方式によるライブラリの初期化】<br>
 * Fixed Memory方式を用いる場合、ライブラリの初期化／終了の手順は以下の以下の通りです。<br>
 * 	-# 初期化用コンフィグ構造体にパラメータをセットする。<br>
 * 	-# ライブラリの初期化に必要なワーク領域のサイズを、 ::criFs_CalculateWorkSizeForLibrary 
 * 関数を使って計算する。<br>
 * 	-# ワーク領域サイズ分のメモリを確保する。<br>
 * 	-# ::criFs_InitializeLibrary 関数で初期化処理を行う。<br>
 * （workには確保したメモリのアドレスを、sizeにはワーク領域のサイズを指定する。）<br>
 * 	-# アプリケーション終了時に ::criFs_FinalizeLibrary 関数で終了処理を行なう。<br>
 * 	-# ワーク領域のメモリを解放する。<br>
 * 	.
 * <br>具体的なコードは以下のとおりです。<br>
 * \code
 * main()
 * {
 * 	CriFsConfig config;	// ライブラリ初期化用コンフィグ構造体
 * 	void *work;				// ワーク領域アドレス
 * 	CriSint32 nbyte;		// ワーク領域サイズ
 * 		:
 * 	// ライブラリ初期化用コンフィグ構造体にデフォルト値をセット
 * 	criFs_SetDefaultConfig(&config);
 * 	
 * 	// ライブラリの初期化に必要なワーク領域のサイズを計算
 * 	size = criFs_CalculateWorkSizeForLibrary(&config);
 * 	
 * 	// ワーク領域用にメモリを確保
 * 	work = malloc((size_t)size);
 * 	
 * 	// ライブラリの初期化
 * 	// →確保済みのワーク領域を指定する。
 * 	criFs_InitializeLibrary(&config, NULL, 0);
 * 		:
 * 	// アプリケーションのメイン処理
 * 	// →この間、確保したメモリは保持し続ける。
 * 		:
 * 	// アプリケーションを終了する際に終了処理を行う
 * 	criFs_FinalizeLibrary();
 * 	
 * 	// 必要なくなったワーク領域を解放する
 * 	free(work);
 * 		:
 * }
 * \endcode

 * \par 備考:
 * ライブラリ使用中に確保できるハンドルの数（CriFsBinderやCriFsLoaderの数）は、
 * 初期化設定コンフィギュレーション（引数のconfig）で指定します。<br>
 * ライブラリが必要とするワーク領域のサイズも、コンフィギュレーション内容に応じて変化します。<br>
 * （ハンドル数を増やせば、必要なメモリのサイズも大きくなります。）<br>
 * config に NULL を指定した場合、デフォルトのコンフィギュレーションが適用されます。
 * \attention
 * 本関数を実行後、必ず対になる ::criFs_FinalizeLibrary 関数を実行してください。<br>
 * また、 ::criFs_FinalizeLibrary 関数を実行するまでは、本関数を再度実行することはできません。<br>
 * \sa CriFsConfig, criFs_CalculateWorkSizeForLibrary, criFs_FinalizeLibrary
 * \sa criFs_SetUserMallocFunction, criFs_SetUserFreeFunction
 */
/*EN
 * \brief Initialize the CRI File System
 * \ingroup FSLIB_CRIFS
 * \param[in]	config	Configuration
 * \param[in]	buffer	Work area
 * \param[in]	size	Size of work area
 * \return	CriError	Error information
 * \par
 * Initialize the CRI File System<br>
 * \sa criFs_FinalizeLibrary
 */
CriError CRIAPI criFs_InitializeLibrary(const CriFsConfig *config, void *buffer, CriSint32 size);

/*JP
 * \brief CRI File Systemの終了
 * \ingroup FSLIB_CRIFS
 * \return	CriError	エラーコード
 * \par 説明:
 * CRI File Systemライブラリを終了します。<br>
 * \attention
 * ::criFs_InitializeLibrary 関数実行前に本関数を実行することはできません。<br>
 * \sa criFs_InitializeLibrary
 */
/*EN
 * \brief Finalize the CRI File System
 * \ingroup FSLIB_CRIFS
 * \return	CriError	Error information
 * \par
 * Finalize the CRI File System<br>
 * \sa criFs_InitializeLibrary
 */
CriError CRIAPI criFs_FinalizeLibrary(void);

/*JP
 * \brief メモリ確保関数の登録
 * \ingroup FSLIB_CRIFS
 * \param[in]	func		メモリ確保関数
 * \param[in]	obj			ユーザ指定オブジェクト
 * \par 説明:
 * CRI File Systemライブラリにメモリ確保関数を登録します。<br>
 * CRI File Systemライブラリ内がライブラリ内で行なうメモリ確保処理を、
 * ユーザ独自のメモリ確保処理に置き換えたい場合に使用します。<br>
 * <br>
 * 本関数の使用手順は以下のとおりです。<br>
 * (1) ::CriFsMallocFunc インターフェースに副ったメモリ確保関数を用意する。<br>
 * (2) ::criFs_SetUserMallocFunction 関数を使用し、CRI File Systemライブラリに対して
 * メモリ確保関数を登録する。<br>
 * <br>
 * 具体的なコードの例は以下のとおりです。
 * \par 例:
 * \code
 * // 独自のメモリ確保関数を用意
 * void *user_malloc(void *obj, CriUint32 size)
 * {
 * 	void *mem;
 * 	
 * 	// メモリの確保
 * 	mem = malloc(size);
 * 	
 * 	return (mem);
 * }
 * 
 * main()
 * {
 * 		:
 * 	// メモリ確保関数の登録
 * 	criFs_SetUserMallocFunction(user_malloc, NULL);
 * 		:
 * }
 * \endcode
 * \par 備考:
 * 引数の obj に指定した値は、 ::CriFsMallocFunc に引数として渡されます。<br>
 * メモリ確保時にメモリマネージャ等を参照する必要がある場合には、
 * 当該オブジェクトを本関数の引数にセットしておき、コールバック関数で引数を経由
 * して参照してください。<br>
 * \attention
 * メモリ確保関数を登録する際には、合わせてメモリ解放関数（ ::CriFsFreeFunc ）を
 * 登録する必要があります。
 * \sa CriFsMallocFunc, criFs_SetUserFreeFunction
 */
void CRIAPI criFs_SetUserMallocFunction(CriFsMallocFunc func, void *obj);

/*JP
 * \brief メモリ解放関数の登録
 * \ingroup FSLIB_CRIFS
 * \param[in]	func		メモリ解放関数
 * \param[in]	obj			ユーザ指定オブジェクト
 * \par 説明:
 * CRI File Systemライブラリにメモリ解放関数を登録します。<br>
 * CRI File Systemライブラリ内がライブラリ内で行なうメモリ解放処理を、
 * ユーザ独自のメモリ解放処理に置き換えたい場合に使用します。<br>
 * <br>
 * 本関数の使用手順は以下のとおりです。<br>
 * (1) ::CriFsFreeFunc インターフェースに副ったメモリ解放関数を用意する。<br>
 * (2) ::criFs_SetUserFreeFunction 関数を使用し、CRI File Systemライブラリに対して
 * メモリ解放関数を登録する。<br>
 * <br>
 * 具体的なコードの例は以下のとおりです。
 * \par 例:
 * \code
 * // 独自のメモリ解放関数を用意
 * void user_free(void *obj, void *mem)
 * {
 * 	// メモリの解放
 * 	free(mem);
 * 	
 * 	return;
 * }
 * 
 * main()
 * {
 * 		:
 * 	// メモリ解放関数の登録
 * 	criFs_SetUserFreeFunction(user_free, NULL);
 * 		:
 * }
 * \endcode
 * \par 備考:
 * 引数の obj に指定した値は、 ::CriFsFreeFunc に引数として渡されます。<br>
 * メモリ確保時にメモリマネージャ等を参照する必要がある場合には、
 * 当該オブジェクトを本関数の引数にセットしておき、コールバック関数で引数を経由
 * して参照してください。<br>
 * \attention
 * メモリ解放関数を登録する際には、合わせてメモリ確保関数（ ::CriFsMallocFunc ）を
 * 登録する必要があります。
 * \sa CriFsFreeFunc, criFs_SetUserMallocFunction
 */
void CRIAPI criFs_SetUserFreeFunction(CriFsFreeFunc func, void *obj);


/*JP
 * \brief サーバ処理の実行
 * \ingroup FSLIB_CRIFS
 * \return	CriError	エラーコード
 * \par 説明:
 * CRI File Systemライブラリの内部状態を更新します。<br>
 * アプリケーションは、この関数を定期的（毎フレーム1回程度）に実行する必要があります。<br>
 * \attention
 * criFs_ExecuteMain を実行しない場合、ファイルのロードが進まない等の問題が発生する可能性があります。<br>
 */
CriError CRIAPI criFs_ExecuteMain(void);

/*JP
 * \brief ファイルアクセス処理の実行（非スレッド時環境向け）
 * \ingroup FSLIB_CRIFS
 * \return	CriError	エラーコード
 * \par 説明:
 * CRI File Systemライブラリのファイルアクセス処理を実行します。<br>
 * \attention
 * この関数は、スレッドを使用しない環境でCRI File Systemライブラリを使用する場合に呼び出す必要があります。<br>
 * スレッドを使用する環境では、この関数の代わりに、 criFs_ExecuteMain 関数を実行してください。<br>
 * \sa
 * criFs_ExecuteMain()
 */
CriError CRIAPI criFs_ExecuteFileAccess(void);

/*JP
 * \brief データ展開処理の実行（非スレッド時環境向け）
 * \ingroup FSLIB_CRIFS
 * \return	CriError	エラーコード
 * \par 説明:
 * CRI File Systemライブラリのデータ展開処理を実行します。<br>
 * \attention
 * この関数は、スレッドを使用しない環境でCRI File Systemライブラリを使用する場合に呼び出す必要があります。<br>
 * スレッドを使用する環境では、この関数の代わりに、 criFs_ExecuteMain 関数を実行してください。<br>
 * \sa
 * criFs_ExecuteMain()
 */
CriError CRIAPI criFs_ExecuteDataDecompression(void);

/*JP
 * \brief ファイルオープンエラー発生時のリトライ方法の設定
 * \ingroup FSLIB_CRIFS
 * \param[in]	mode	リトライ方法
 * \return	CriError	エラーコード
 * \par 説明:
 * ファイルのオープンに失敗した場合に、CRI File Systemライブラリ内でオープンのリトライを行なうかどうかを指定します。<br>
 * リトライ方法に CRIFS_OPEN_RETRY_INFINITE を指定した場合、ファイルがオープンできるまでCRI File Systemライブラリはオープン処理をリトライし続けます。<br>
 * CRIFS_OPEN_RETRY_NONE を指定した場合、CRI File Systemライブラリはリトライ処理を行なわず、ハンドルのステータスをエラー状態に遷移します。
 */
CriError CRIAPI criFs_SetOpenRetryMode(CriFsOpenRetryMode mode);

/*JP
 * \brief ファイルリードエラー発生時のリトライ方法の設定
 * \ingroup FSLIB_CRIFS
 * \param[in]	mode	リトライ方法
 * \return	CriError	エラーコード
 * ファイルのリードに失敗した場合に、CRI File Systemライブラリ内でリードのリトライを行なうかどうかを指定します。<br>
 * リトライ方法に CRIFS_READ_RETRY_INFINITE を指定した場合、ファイルがリードできるまでCRI File Systemライブラリはリード処理をリトライし続けます。<br>
 * CRIFS_READ_RETRY_NONE を指定した場合、CRI File Systemライブラリはリトライ処理を行なわず、ハンドルのステータスをエラー状態に遷移します。
 */
CriError CRIAPI criFs_SetReadRetryMode(CriFsReadRetryMode mode);

/*JP
 * \brief グループ優先区間の開始
 * \ingroup FSLIB_CRIFS
 * \return	CriError	エラーコード
 * \param[in]	groupname	グループ名
 * \param[in]	attrname	アトリビュート名
 * \par 説明:
 * グループ優先区間を開始します。<br>
 * 本関数実行後、 ::criFs_EndGroup 関数を実行するまでの間、指定したグループ内のファイルが優先的にロードされるようになります。<br>
 * （バインダ内に同じ名前のファイルが複数存在する場合でも、指定したグループ内のファイルが優先して選択されます。）<br>
 * 本関数を使用することにより、通常のローダを使用する場合でも、グループ化の恩恵を受けることが可能になります。<br>
 * \attention
 * 複数のグループ優先区間を重複させることはできません。<br>
 * 本関数を実行後、必ず対になる ::criFs_EndGroup 関数を実行してください。
 * 本関数と ::criFs_BeginLoadRegion 関数を併用することはできません。<br>
 * （CRI File System Ver.2.02.00より、 ::criFs_BeginLoadRegion 関数は本関数を呼び出すマクロに変更されました。）<br>
 * \sa criFs_EndGroup, criFs_BeginLoadRegion
 */
CriError CRIAPI criFs_BeginGroup(const CriChar8 *groupname, const CriChar8 *attrname);

/*JP
 * \brief グループ優先区間の終了
 * \ingroup FSLIB_CRIFS
 * \return	CriError	エラーコード
 * \par 説明:
 * グループ優先区間を終了します。<br>
 * \attention
 * 本関数と ::criFs_EndLoadRegion 関数を併用することはできません。<br>
 * （CRI File System Ver.2.02.00より、 ::criFs_EndLoadRegion 関数は本関数を呼び出すマクロに変更されました。）<br>
 * \sa criFs_BeginGroup
 */
CriError CRIAPI criFs_EndGroup(void);

/*JP
 * \brief バインダ使用数の取得
 * \ingroup FSLIB_CRIFS
 * \param[out]	cur_num		現在使用中のバインダの数
 * \param[out]	max_num		過去に最大同時に利用したバインダの数
 * \param[out]	limit		利用可能なバインダの上限数
 * \return	CriError	エラーコード
 * \par 説明:
 * バインダの使用数に関する情報を取得します。<br>
 */
CriError criFs_GetNumUsedBinders(CriSint32 *cur_num, CriSint32 *max_num, CriSint32 *limit);

/*JP
 * \brief ローダ使用数の取得
 * \ingroup FSLIB_CRIFS
 * \param[out]	cur_num		現在使用中のローダの数
 * \param[out]	max_num		過去に最大同時に利用したローダの数
 * \param[out]	limit		利用可能なローダの上限数
 * \return	CriError	エラーコード
 * \par 説明:
 * ローダの使用数に関する情報を取得します。<br>
 * \par 備考
 * バインダやインストーラ等は内部的にローダを使用します。<br>
 * そのため、CRI File Systemライブラリは、初期化時にコンフィギュレーション（ ::CriFsConfig ）
 * で指定された数以上のローダを作成する場合があります。<br>
 * そのため、本関数で取得するローダの上限数（limit）は、初期化時に指定した値と異なる場合があります。<br>
 */
CriError criFs_GetNumUsedLoaders(CriSint32 *cur_num, CriSint32 *max_num, CriSint32 *limit);

/*JP
 * \brief グループローダ使用数の取得
 * \ingroup FSLIB_CRIFS
 * \param[out]	cur_num		現在使用中のグループローダの数
 * \param[out]	max_num		過去に最大同時に利用したグループローダの数
 * \param[out]	limit		利用可能なグループローダの上限数
 * \return	CriError	エラーコード
 * \par 説明:
 * グループローダの使用数に関する情報を取得します。<br>
 */
CriError criFs_GetNumUsedGroupLoaders(CriSint32 *cur_num, CriSint32 *max_num, CriSint32 *limit);

/*JP
 * \brief CriFsStdioハンドル使用数の取得
 * \ingroup FSLIB_CRIFS
 * \param[out]	cur_num		現在使用中のCriFsStdioハンドルの数
 * \param[out]	max_num		過去に最大同時に利用したCriFsStdioハンドルの数
 * \param[out]	limit		利用可能なCriFsStdioハンドルの上限数
 * \return	CriError	エラーコード
 * \par 説明:
 * CriFsStdioハンドルの使用数に関する情報を取得します。<br>
 */
CriError criFs_GetNumUsedStdioHandles(CriSint32 *cur_num, CriSint32 *max_num, CriSint32 *limit);

/*JP
 * \brief インストーラ使用数の取得
 * \ingroup FSLIB_CRIFS
 * \param[out]	cur_num		現在使用中のインストーラの数
 * \param[out]	max_num		過去に最大同時に利用したインストーラの数
 * \param[out]	limit		利用可能なインストーラの上限数
 * \return	CriError	エラーコード
 * \par 説明:
 * インストーラの使用数に関する情報を取得します。<br>
 */
CriError criFs_GetNumUsedInstallers(CriSint32 *cur_num, CriSint32 *max_num, CriSint32 *limit);

/*JP
 * \brief バインド数の取得
 * \ingroup FSLIB_CRIFS
 * \param[out]	cur_num		現在バインド中の数
 * \param[out]	max_num		過去に最大同時にバインドした数
 * \param[out]	limit		バインド可能回数の上限値
 * \return	CriError	エラーコード
 * \par 説明:
 * バインド数に関する情報を取得します。<br>
 */
CriError criFs_GetNumBinds(CriSint32 *cur_num, CriSint32 *max_num, CriSint32 *limit);

/*JP
 * \brief オープンされたファイル数の取得
 * \ingroup FSLIB_CRIFS
 * \param[out]	cur_num		現在オープン中のファイルの数
 * \param[out]	max_num		過去に最大同時にオープンしたファイルの数
 * \param[out]	limit		オープン可能なファイルの上限値
 * \return	CriError	エラーコード
 * \par 説明:
 * ファイルのオープン数に関する情報を取得します。<br>
 */
CriError criFs_GetNumOpenedFiles(CriSint32 *cur_num, CriSint32 *max_num, CriSint32 *limit);

/*JP
 * \brief I/O選択コールバックの登録
 * \ingroup FSLIB_CRIFS
 * \param[in]	func	I/O選択コールバック
 * \return	CriError	エラーコード
 * \par 説明:
 * I/O選択コールバック関数（ ::CriFsSelectIoCbFunc ）を登録します。<br>
 * CRI File Systemライブラリはファイルにアクセスする際、まず初めに、そのファイルが存在するデバイスのID（ ::CriFsDeviceId ）と、
 * デバイスにアクセスするためのI/Oインターフェース（ ::CriFsIoInterface ）を選択します。<br>
 * デフォルト状態では、デバイスIDとI/Oインターフェースの選択はライブラリ内で暗黙的に行なわれますが、
 * 本関数を使用することで、デバイスIDとI/Oインターフェースをユーザが自由に指定することが可能になります。<br>
 * これにより、ユーザが独自に作成したI/Oインターフェースを使用してファイルにアクセスすることが可能になります。<br>
 * \code
 * // 独自のI/Oインターフェースを定義
 * // 備考）構造体のメンバ関数はユーザが独自に実装。
 * static CriFsIoInterface g_userIoInterface = {
 * 	userExists,
 * 	userRemove,
 * 	userRename,
 * 	userOpen,
 * 	userClose,
 * 	userGetFileSize,
 * 	userRead,
 * 	userIsReadComplete,
 * 	userGetReadSize,
 * 	userWrite,
 * 	userIsWriteComplete,
 * 	userGetWriteSize,
 * 	userFlush,
 * 	userResize,
 * 	userGetNativeFileHandle
 * };
 * 
 * // I/O選択コールバック関数
 * CriError user_select_io_callback(
 * 	const CriChar8 *path, CriFsDeviceId *device_id, CriFsIoInterfacePtr *ioif)
 * {
 * 	// パスを解析し、デバイスのIDを特定する
 * 	if (strncmp(path, …) == 0) {
 * 		(*device_id) = CRIFS_DEVICE_～;
 * 	} else {
 * 		(*device_id) = CRIFS_DEFAULT_DEVICE;
 * 	}
 * 	
 * 	// ファイルアクセスに使用するI/Oインターフェースを指定する
 * 	(*ioif) = g_userIoInterface;
 * 	
 * 	return (CRIERR_OK);
 * }
 * 
 * int main(…)
 * {
 * 		：
 * 	// I/O選択コールバックを登録
 * 	criFs_SetSelectIoCallback(user_select_io_callback);
 * 		：
 * }
 * \endcode
 * \sa CriFsSelectIoCbFunc, criFs_GetDefaultIoInterface
 */
CriError CRIAPI criFs_SetSelectIoCallback(CriFsSelectIoCbFunc func);

/*JP
 * \brief デフォルトI/Oインターフェースの取得
 * \ingroup FSLIB_CRIFS
 * \param[out]	ioif	I/Oインターフェース
 * \return	CriError	エラーコード
 * \par 説明:
 * CRI File Systemライブラリがデフォルトで利用するI/Oインターフェースを取得します。<br>
 * I/O選択コールバック（::CriFsSelectIoCbFunc）内でデフォルトの処理をさせたい場合には、
 * 本関数で取得したI/Oインターフェースを、出力値として返してください。<br>
 * \code
 * // 独自のI/Oインターフェースを定義
 * // 備考）構造体のメンバ関数はユーザが独自に実装。
 * static CriFsIoInterface g_userIoInterface = {
 * 	userExists,
 * 	userRemove,
 * 	userRename,
 * 	userOpen,
 * 	userClose,
 * 	userGetFileSize,
 * 	userRead,
 * 	userIsReadComplete,
 * 	userGetReadSize,
 * 	userWrite,
 * 	userIsWriteComplete,
 * 	userGetWriteSize,
 * 	userFlush,
 * 	userResize,
 * 	userGetNativeFileHandle
 * };
 * 
 * // I/O選択コールバック関数
 * CriError user_select_io_callback(
 * 	const CriChar8 *path, CriFsDeviceId *device_id, CriFsIoInterfacePtr *ioif)
 * {
 * 	// パスを解析し、デバイスのIDを特定する
 * 	if (strncmp(path, …) == 0) {
 * 		(*device_id) = CRIFS_DEVICE_～;
 * 	} else {
 * 		(*device_id) = CRIFS_DEFAULT_DEVICE;
 * 	}
 * 	
 * 	// sample.binのみを独自インターフェースで処理する
 * 	if (strcmp(path, "sample.bin") == 0) {
 * 		(*ioif) = g_userIoInterface;
 * 	} else {
 * 		// 他のファイルはデフォルトI/Oインターフェースで処理
 * 		criFs_GetDefaultIoInterface(ioif);
 * 	}
 * 	
 * 	return (CRIERR_OK);
 * }
 * 
 * int main(…)
 * {
 * 		：
 * 	// I/O選択コールバックを登録
 * 	criFs_SetSelectIoCallback(user_select_io_callback);
 * 		：
 * }
 * \endcode
 * \sa CriFsSelectIoCbFunc, criFs_SetSelectIoCallback
 */
CriError CRIAPI criFs_GetDefaultIoInterface(CriFsIoInterfacePtr *ioif);

/*JP
 * \brief デバイス情報の取得
 * \ingroup FSLIB_CRIFS
 * \param[in]	id		デバイスID
 * \param[out]	info	デバイス情報
 * \return	CriError	エラーコード
 * \par 説明:
 * 指定したデバイスの情報を取得します。<br>
 * 指定したデバイスがファイルの書き込みに対応しているかどうかや、
 * 読み書きに使用するバッファのアライメント調整が必要かどうかを確認することが可能です。<br>
 * \sa CriFsDeviceId, CriFsDeviceInfo, criFs_SetDeviceInfo
 */
CriError CRIAPI criFs_GetDeviceInfo(CriFsDeviceId id, CriFsDeviceInfo *info);

/*JP
 * \brief デバイス情報の設定
 * \ingroup FSLIB_CRIFS
 * \param[in]	id		デバイスID
 * \param[in]	info	デバイス情報
 * \return	CriError	エラーコード
 * 指定したデバイスの情報を変更します。<br>
 * I/Oレイヤ差し替え時、I/Oレイヤ側でデバイスの制限を緩和できる場合等に使用します。<br>
 * \sa CriFsDeviceId, CriFsDeviceInfo, criFs_GetDeviceInfo
 */
CriError CRIAPI criFs_SetDeviceInfo(CriFsDeviceId id, CriFsDeviceInfo info);

/*==========================================================================
 *      CriFsIo API
 *=========================================================================*/

/*==========================================================================
 *      CriFsBinder API
 *=========================================================================*/
/*JP
 * \brief CRI File System - Binder オブジェクト
 * \ingroup FSLIB_BINDER
 * \par 説明:
 * CriFsBinderとはファイルデータをデータベース化するためのモジュールです。
 */

/*JP
 * \brief バインダモジュール関数で使用されるメモリ管理関数の登録／削除
 * \ingroup FSLIB_BINDER
 * \param[in]	allocfunc	メモリ確保関数
 * \param[in]	freefunc	メモリ解放関数
 * \param[in]	obj			メモリ管理オブジェクト
 * \return		CriError	エラーコード
 * \par 説明：
 * CriFsBinder関数の引数で指定するワーク領域に NULL を指定した場合、内部から呼ばれるメモリ管理関数を登録します。<br>
 * CriFsBinder関数に NULL 以外を指定した場合は、渡された領域を利用します。<br>
 * <br>
 * 本関数により、CriFsBinder関数に必要なワーク領域の管理をユーザー独自のメモリ管理関数で動的に行うことが可能になります。<br>
 * 本関数によりメモリ管理関数を設定する場合は、criFs_InitializeLibrary関数の直後に本関数を呼び出してください。<br>
 * 本関数の引数allocfunc, freefuncがNULLの場合、登録されたメモリ管理関数を削除します。<br>
 * CriFsBinder関数は、現在登録されているメモリ管理関数を使用しますので、一旦登録されたメモリ管理関数を削除／変更する場合は、
 * 登録されたメモリ管理関数を利用して確保されたメモリ領域が無いことを確認するようにしてください。
 * \par CPKバインドについて：
 * CPKバインド時には、criFsBinder_GetWorkSizeForBindCpk関数の項にも記されてる通り、CPK解析に必要なワーク領域のサイズが
 * CPKの構成に依ってしまうので、事前に必要なワークサイズを見積ることが困難です。<br>
 * 本関数により、CPK解析エンジンが要求するメモリの管理をユーザー独自のメモリ管理関数で行うことが可能になります。<br>
 * \par 例：
 * \code
 * void *u_alloc(void *obj, CriSint32 size)
 * {
 *   :
 * }
 * void u_free(void *obj, void *ptr)
 * {
 *   :
 * }
 * void bind_cpk(void)
 * {
 *  CriFsBinderId id;
 *  CriSint32 wksize;
 *  void *work;
 *  // メモリ管理関数の登録
 *  criFsBinder_SetUserHeapFunc(u_alloc, u_free, u_mem_obj);
 *
 *  // CPK バインド
 *  criFsBinder_BindCpk(NULL, NULL, "sample.cpk", NULL, 0, &id);
 *    :
 *
 *  // ワークを指定した場合、指定されたワークを使用します。
 *  criFsBinder_GetWorkSizeForBindFile(NULL, "sample.cpk", &wksize);
 *  work = malloc(wksize);
 *  criFsBinder_BindFile(NULL, NULL, "sample.cpk", work, wksize, &id);
 *    :
 *
 *  // CPKバインド時、criFsBinder_GetWorkSizeForBindCpk は CPK解析以外の
 *  // バインドに必要な最低限必要なワークのサイズを返しますので、
 *  // 未確定な領域だけを動的に確保するような使いかたも可能です。
 *  criFsBinder_GetWorkSizeForBindCpk(NULL, "sample.cpk", &wksize);
 *  work = malloc(wksize);
 *  // CPK バインド
 *  criFsBinder_BindCpk(NULL, NULL, "sample.cpk", work, wksize, &id);
 *    :
 * }
 * \endcode
 */
CriError CRIAPI criFsBinder_SetUserHeapFunc(CriFsBinderUserHeapAllocateCbFunc allocfunc, CriFsBinderUserHeapFreeCbFunc freefunc, void *obj);

/*JP
 * \brief バインダの生成
 * \ingroup FSLIB_BINDER
 * \param[out]	bndrhn		バインダハンドル
 * \return		CriError	エラーコード
 * \par 説明：
 * バインダを生成し、バインダハンドルを返します。<br>
 * \par 例：
 * \code
 * criFsBinderHn bndrhn;
 * criFsBinder_Create(&bndrhn);
 * 		:
 * criFsBinder_Destroy(bndrhn);
 * \endcode
 * \sa criFsBinder_Destroy()
 */
CriError CRIAPI criFsBinder_Create(CriFsBinderHn *bndrhn);

/*JP
 * \brief バインダの破棄
 * \ingroup FSLIB_BINDER
 * \param[in]	bndrhn		バインダハンドル
 * \return		CriError	エラーコード
 * \par 説明：
 * バインダを破棄します。<br>
 * \par 注意：
 * 破棄するバインダにバインドされているバインダIDも同時に破棄されます。<br>
 * 本関数で破棄できるのは、::criFsBinder_Create 関数により生成されたバインダハンドルのみです。<br>
 * ::criFsBinder_GetHandle 関数により CriFsBinderId から取得されたバインダハンドルは破棄できません。<br>
 * CriFsBinderId については ::criFsBinder_Unbind 関数をご使用ください。
 * 
 * \sa criFsBinder_Create() criFsBinder_Unbind()
 */
CriError CRIAPI criFsBinder_Destroy(CriFsBinderHn bndrhn);

/*JP
 * \brief CPKファイルバインドのワークサイズ取得
 * \ingroup FSLIB_BINDER
 * \param[in]	srcbndrhn	バインドするCPKファイルにアクセスするためのバインダハンドル
 * \param[in]	path		バインドするCPKファイルのパス名
 * \param[out]	worksize	必要ワークサイズ（バイト）
 * \return		CriError	エラーコード
 * \par 説明：
 * criFsBinder_BindCpk関数に指定するワークサイズを取得します。<br>
 * 本関数では、CPKの情報を解析するために最低限必要とされるワークサイズを取得できます。<br>
 * コンテンツファイル数の少いCPKファイルであれば、本関数で得られる数値のままでもCPKをバインドできる可能性があります。<br>
 * CPKバインド中にワークメモリが不足した場合、エラーコールバック関数で不足分の容量が示されます。<br>
 * <br>
 * *** CPKバインドに必要なワークサイズの取得するには、以下の方法があります。 ***<br><br>
 * １. エラーコールバックを利用する。<br>
 * CPKバインド中にワークメモリが不足した場合、エラーコールバック関数により不足分のサイズが示されます。<br>
 * 現在確保しているワーク領域のサイズに、エラーコールバックで得られるサイズを加算してワーク領域を再確保し、CPKバインドをやりなおします。<br>
 * CPKバインドをやりなおす場合、メモリ不足エラーコールバックを起こしたバインダーIDをアンバインドする必要があります。<br>
 * <br>
 * ２. メモリ確保／解放コールバック関数( ::criFsBinder_SetUserHeapFunc )を利用する。<br>
 * CPKバインド時にワークメモリの確保／解放が必要な際に呼出されるコールバック関数を登録します。<br>
 * バインド時に適時メモリ確保コールバック関数が呼出された場合、要求されたメモリを確保してエラーコールバック関数の返値として返します。<br>
 * <br>
 * ３. 必要ワークサイズ取得API（ ::criFsBinder_AnalyzeWorkSizeForBindCpk ）を利用する。<br>
 * バインドするCPKから情報を取得し、必要となるワークサイズを算出する関数を利用します。<br>
 * この関数は完了復帰関数で、関数内部でCPKファイルから情報を読み出しています。そのため関数終了までに時間がかかります。<br>
 * <br>
 * ４. CPKパッキングツールを利用する。<br>
 * パッキングツールにCPKファイルをドラッグ＆ドロップし、CPKファイルビューア［詳細］テキストボックスの
 * 「Enable Filename info.」「Enable ID info.」[Enable Group info.]項目に表示されるバイト数を、
 * 本関数で取得される必要ワークサイズに加算します。<br>
 * ただし、この方法で得られるワークサイズは目安ですので、メモリ不足エラーコールバックが起きる可能性があります。<br>
 * \sa criFsBinder_BindCpk() criFsBinder_SetUserHeapFunc() criFsBinder_AnalyzeWorkSizeForBindCpk()
 */
CriError CRIAPI criFsBinder_GetWorkSizeForBindCpk(CriFsBinderHn srcbndrhn, const CriChar8 *path, CriSint32 *worksize);

/*JP
 * \brief CPKバインドに必要なワーク領域サイズの取得
 * \ingroup FSLIB_BINDER
 * \param[in]	srcbndrhn	バインダーハンドル
 * \param[in]	path		CPKファイルのパス
 * \param[in]	work		CPKヘッダ解析用のワーク領域
 * \param[in]	wksize	 	CPKヘッダ解析用のワーク領域のサイズ
 * \param[out]	rqsize		CPKバインドに必要なワーク領域のサイズ
 * \return		CriError	エラーコード
 * \par 説明：
 * srcbndrhn と path で指定されたCPKファイルを解析し、CPKバインドに必要なワーク領域の
 * サイズを取得します。<br>
 * 本関数は完了復帰関数です。<br>
 * 本関数は、指定されたCPKファイルのヘッダ情報を読み込んで解析しています。そのため、
 * 内部で読込待ちを行っています。
 * 本関数に渡すワーク領域は、criFsBinder_GetWorkSizeForBindCpk関数で得られたサイズの
 * ワーク領域を確保して渡してください。
 * \par 例：
 * \code
 * // ---- CPK解析に最低限必要なメモリの確保
 * criFsBinder_GetWorkSizeForBindCpk(bndrhn, path, &wksz)
 * work = malloc(wksz); 
 * // CPKバインドに必要なメモリ容量を解析
 * criFsBinder_AnalyzeWorkSizeForBindCpk(bndrhn, path, work, wksz, &nbyte);
 * free(work);
 * // ----
 * // CPKバインド用メモリの確保
 * bindwork = malloc(nbyte);
 * // CPKバインド
 * criFsBinder_BindCpk(srcbndr, path, bindwork, nbyte, &bndrid);
 * \endcode
 */
CriError CRIAPI criFsBinder_AnalyzeWorkSizeForBindCpk(
	CriFsBinderHn srcbndrhn, const CriChar8 *path, void *work, CriSint32 wksize, CriSint32 *rqsize);

/*JP
 * \brief ファイルバインドのワークサイズの取得
 * \ingroup FSLIB_BINDER
 * \param[in]	srcbndrhn	バインドするファイルにアクセスするためのバインダハンドル
 * \param[in]	path		バインドするファイルのパス名
 * \param[out]	worksize	必要ワークサイズ（バイト）
 * \return		CriError	エラーコード
 * \par 説明：
 * criFsBinder_BindFile関数に指定するワークサイズを取得します。
 * \sa criFsBinder_BindFile()
 */
CriError CRIAPI criFsBinder_GetWorkSizeForBindFile(CriFsBinderHn srcbndrhn, const CriChar8 *path, CriSint32 *worksize);

/*JP
 * \brief 複数ファイルバインドのワークサイズの取得
 * \ingroup FSLIB_BINDER
 * \param[in]	srcbndrhn	バインドするファイルにアクセスするためのバインダハンドル
 * \param[in]	filelist	バインドするファイル名のリスト（セパレータ：',''\\t''\\n'  ターミネイタ:'\\0'）
 * \param[out]	worksize	必要ワークサイズ（バイト）
 * \return		CriError	エラーコード
 * \par 説明：
 * criFsBinder_BindFiles関数に指定するワークサイズを取得します。
 * \sa criFsBinder_BindFiles()
 */
CriError CRIAPI criFsBinder_GetWorkSizeForBindFiles(CriFsBinderHn srcbndrhn, const CriChar8 *filelist, CriSint32 *worksize);

/*JP
 * \brief ディレクトリバインドのワークサイズの取得
 * \ingroup FSLIB_BINDER
 * \param[in]	srcbndrhn	バインドするディレクトリにアクセスするためのバインダハンドル
 * \param[in]	path		バインドするディレクトリのパス名
 * \param[out]	worksize	必要ワークサイズ（バイト）
 * \return		CriError	エラーコード
 * \par 説明：
 * criFsBinder_BindDirectory関数に指定するワークサイズを取得します。
 * \sa criFsBinder_BindDirectory()
 */
CriError CRIAPI criFsBinder_GetWorkSizeForBindDirectory(CriFsBinderHn srcbndrhn, const CriChar8 *path, CriSint32 *worksize);

/*JP
 * \brief Cpkファイルのバインド
 * \ingroup FSLIB_BINDER
 * \param[in]	bndrhn		バインド先のバインダハンドル
 * \param[in]	srcbndrhn	バインドするCPKファイルにアクセスするためのバインダハンドル
 * \param[in]	path		バインドする CPKファイルのパス名
 * \param[in]	work		バインド用（主にCPK解析）ワーク領域
 * \param[in]	worksize	ワーク領域のサイズ（バイト）
 * \param[out]	bndrid		バインダID
 * \return		CriError	エラーコード
 * \par 説明：
 * CPKファイルを利用するには、CPKファイルをバインドする必要があります。<br>
 * 本関数は、バインダ(bndrhn)にCPKファイル(path)をバインドし、バインダID(bndrid)を返します。<br>
 * srcbndrhnには、CPKファイルを検索するためのバインダを指定します。
 * これがNULLの場合、デフォルトデバイスを使用します。<br>
 * ワーク領域(work)のサイズは、criFsBinder_GetWorkSizeForBindCpkで取得できます。
 * ワーク領域は、バインダIDが破棄されるまで保持してください。<br>
 * メモリ確保／解放コールバック関数が登録されている場合、ワーク領域にNULL(ワークサイズは０）を設定すると、
 * 必要なワーク領域をメモリ確保／解放コールバック関数を使用して動的に確保します。<br>
 * バインドを開始できない場合、バインダIDはNULLが返されます。
 * バインダIDにNULL以外が返された場合は内部リソースを確保していますので、
 * バインドの成功／失敗に関らず、不要になったバインダIDはアンバインドしてください。<br><br>
 * バインドしたCPKファイルはオープン状態で保持されます。
 * そのため、バインダ内部でCriFsLoaderを作成しています。<br><br>
 * 本関数は即時復帰関数です。本関数から復帰した直後は、CPKのバインドはまだ完了しておらず、
 * CPKのコンテンツファイルへのアクセスは行えません。<br>
 * バインド状態が完了（ CRIFSBINDER_STATUS_COMPLETE ）となった後に、CPKは利用可能となります。<br>
 * バインド状態は criFsBinder_GetStatus 関数で取得します。<br>
 * \par 例：
 * \code
 * void *work;
 * CriSint32 wksz;
 * CriFsBinderId bndrid;
 * criFsBinder_GetWorkSizeForBindCpk(NULL, "smp.cpk",  &wksz);
 * work = malloc(wksz);
 * criFsBinder_BindCpk(bndrhn, NULL, "smp.cpk", work, wksz, &bndrid);
 * for (;;) {
 * 	CriFsBinderStatus status;
 * 	criFsBinder_GetStatus(bndrid, &status);
 * 	if (status == CRIFSBINDER_STATUS_COMPLETE) break;
 * }
 * \endcode
 * \sa criFsBinder_GetWorkSizeForBindCpk(), criFsBinder_SetUserHeapFunc(), criFsBinder_GetStatus(), criFsBinder_Unbind() 
*/
CriError CRIAPI criFsBinder_BindCpk(CriFsBinderHn bndrhn, CriFsBinderHn srcbndrhn, const CriChar8 *path, void *work, CriSint32 worksize, CriFsBinderId *bndrid);

/*JP
 * \brief ファイルのバインド
 * \ingroup FSLIB_BINDER
 * \param[in]	bndrhn		バインド先のバインダハンドル
 * \param[in]	srcbndrhn	バインドするファイルにアクセスするためのバインダハンドル
 * \param[in]	path		バインドするファイルのパス名
 * \param[in]	work		バインド用ワーク領域
 * \param[in]	worksize	ワーク領域のサイズ（バイト）
 * \param[out]	bndrid		バインダID
 * \return		CriError	エラーコード
 * \par 説明：
 * ファイルをバインドし、バインダIDを返します。<br>
 * srcbndrhnのバインダからpathで指定されたファイルを検索してバインドします。
 * srcbndrhnがNULLの場合、デフォルトデバイスを使用します。<br>
 * ワーク領域(work)のサイズは、criFsBinder_GetWorkSizeForBindFileで取得できます。
 * ワーク領域は、バインダIDが破棄されるまで保持して下さい。<br>
 * メモリ確保／解放コールバック関数が登録されている場合、ワーク領域にNULL(ワークサイズは０）を設定すると、
 * 必要なワーク領域をメモリ確保／解放コールバック関数を使用して動的に確保します。<br>
 * バインドを開始できない場合、バインダIDはNULLが返されます。
 * バインダIDにNULL以外が返された場合は内部リソースを確保していますので、
 * バインドの成功／失敗に関らず、不要になったバインダIDはアンバインドしてください。<br><br>
 * バインドされたファイルはファイルオープン状態で保持します。
 * このため、内部的にCriFsLoaderを作成しています。<br><br>
 * 本関数は即時復帰関数です。本関数から復帰した直後は、ファイルのバインドはまだ完了しておらず、
 * バインダIDを利用したファイルへのアクセスは行えません。<br>
 * バインダIDのバインド状態が完了（ CRIFSBINDER_STATUS_COMPLETE ）となった後に、
 * ファイルは利用可能となります。<br>
 * バインド状態は criFsBinder_GetStatus 関数で取得します。<br>
 * \par 例：
 * \code
 * void *work;
 * CriSint32 wksz;
 * CriFsBinderId bndrid;
 * criFsBinder_GetWorkSizeForBindFile(NULL, "sample.txt", &wksz);
 * work = malloc(wksz);
 * criFsBinder_BindFile(bndrhn, NULL, "sample.txt", work, wksz, &bndrid);
 * for (;;) {
 * 	CriFsBinderStatus status;
 * 	criFsBinder_GetStatus(bndrid, &status);
 * 	if (status == CRIFSBINDER_STATUS_COMPLETE) break;
 * }
 * \endcode
 * \sa criFsBinder_GetWorkSizeForBindFile(), criFsBinder_SetUserHeapFunc(), criFsBinder_GetStatus(), criFsBinder_Unbind()
 */
CriError CRIAPI criFsBinder_BindFile(CriFsBinderHn bndrhn, CriFsBinderHn srcbndrhn, const CriChar8 *path, void *work, CriSint32 worksize, CriFsBinderId *bndrid);

/*JP
 * \brief 複数ファイルのバインド
 * \ingroup FSLIB_BINDER
 * \param[in]	bndrhn		バインド先のバインダハンドル
 * \param[in]	srcbndrhn	バインドするファイルにアクセスするためのバインダハンドル
 * \param[in]	filelist	バインドするファイル名のリスト（セパレータ：',''\\t''\\n'  ターミネイタ:'\\0'）
 * \param[in]	work		バインド用ワーク領域
 * \param[in]	worksize	ワーク領域のサイズ
 * \param[out]	bndrid		バインダID
 * \return		CriError	エラーコード
 * \par 説明：
 * ファイルリスト(filelist)に列記されたファイルをバインドします。<br>
 * ファイルはsrcbndrhnから検索されますが、srcbndrhnがNULLの場合、デフォルトデバイスが使用されます。<br>
 * ワーク領域のサイズは、criFsBinder_GetWorkSizeForBindFiles で取得できます。
 * ワーク領域は、バインダIDが破棄されるまで保持して下さい。<br><br>
 * メモリ確保／解放コールバック関数が登録されている場合、ワーク領域にNULL(ワークサイズは０）を設定すると、
 * 必要なワーク領域をメモリ確保／解放コールバック関数を使用して動的に確保します。<br>
 * バインドを開始できない場合、バインダIDはNULLが返されます。
 * バインダIDにNULL以外が返された場合は内部リソースを確保していますので、
 * バインドの成功／失敗に関らず、不要になったバインダIDはアンバインドしてください。<br><br>
 * バインドしたファイルはファイルオープン状態で保持します。
 * 内部的には CriFsLoader がバインダIDに１つ作成され、ファイルハンドルがバインドするファイル数分使用されます。<br><br>
 * 本関数は即時復帰関数です。本関数から復帰した直後は、ファイルのバインドはまだ完了しておらず、
 * バインダを利用したファイルへのアクセスは行えません。<br>
 * バインド状態が完了（ CRIFSBINDER_STATUS_COMPLETE ）となった後に、ファイルは利用可能となります。<br>
 * バインド状態は criFsBinder_GetStatus 関数で取得します。<br>
 * \par 例：
 * \code
 * void *work;
 * CriSint32 wksz;
 * CriFsBinderId bndrid;
 * CriChar8 *flist = "a.txt,b.txt,c.txt";
 * criFsBinder_GetWorkSizeForBindFiles(NULL, flist, &wksz);
 * work = malloc(wksz);
 * criFsBinder_BindFiles(bndrhn, NULL, flist, work, wksz, &bndrid);
 * for (;;) {
 * 	CriFsBinderStatus status;
 * 	criFsBinder_GetStatus(bndrid, &status);
 * 	if (status == CRIFSBINDER_STATUS_COMPLETE) break;
 * }
 * \endcode
 * \sa criFsBinder_GetWorkSizeForBindFiles(), criFsBinder_SetUserHeapFunc(), criFsBinder_GetStatus(), criFsBinder_Unbind()
 */
CriError CRIAPI criFsBinder_BindFiles(CriFsBinderHn bndrhn, CriFsBinderHn srcbndrhn, const CriChar8 *filelist, void *work, CriSint32 worksize, CriFsBinderId *bndrid);

/*JP
 * \brief ディレクトリパスのバインド
 * \ingroup FSLIB_BINDER
 * \param[in]	bndrhn		バインダハンドル
 * \param[in]	srcbndrhn	バインドしたディレクトリ名でファイルにアクセスする際のバインダ
 * \param[in]	path		バインドするディレクトリパス名
 * \param[in]	work		バインド用ワーク領域
 * \param[in]	worksize	ワーク領域のサイズ（バイト）
 * \param[out]	bndrid		バインダID
 * \return		CriError	エラーコード
 * \par 説明：
 * ディレクトリパス名をバインドします。<br>
 * バインドするディレクトリ名は絶対パスで指定してください。
 * バインド時に指定されたディレクトリが存在するか否かはチェックしていません。<br>
 * バインドされるのはディレクトリパスだけで、指定されたディレクトリ内のファイルを
 * オープン状態にするものではありません。よってバインドに失敗しない限り、本関数から復帰時には
 * バインダIDのバインド状態は完了（ CRIFSBINDER_STATUS_COMPLETE ）となります。<br>
 * srcbndrhnには、本関数でバインドしたディレクトリを用いてファイルを検索する際に、
 * 検索対象となるバインダを指定します。<br>
 * ワーク領域(work)のサイズは、criFsBinder_GetWorkSizeForBindDirectoryで取得できます。
 * ワーク領域は、バインダIDが破棄されるまで保持して下さい。<br>
 * メモリ確保／解放コールバック関数が登録されている場合、ワーク領域にNULL(ワークサイズは０）を設定すると、
 * 必要なワーク領域をメモリ確保／解放コールバック関数を使用して動的に確保します。<br>
 * バインドを開始できない場合、バインダIDはNULLが返されます。
 * バインダIDにNULL以外が返された場合は内部リソースを確保していますので、
 * バインドの成功／失敗に関らず、不要になったバインダIDはアンバインドしてください。<br>
 * \attention
 * 本関数は開発支援用のデバッグ関数です。<br>
 * 本関数を使用した場合、以下の問題が発生する可能性があります。<br>
 * - criFsLoader_Load関数やcriFsBinder_GetFileSize関数内で処理が長時間ブロックされる。<br>
 * - バインドしたディレクトリ内のファイルにアクセスする際、音声やムービーのストリーム再生が途切れる。<br>
 * \par
 * マスターアップ時には本関数をアプリケーション中で使用しないようご注意ください。<br>
 * （ディレクトリ内のデータをCPKファイル化してcriFsBinder_BindCpk関数でバインドするか、またはディレクトリ内のファイルを全てcriFsBinder_BindFiles関数でバインドしてください。）<br>
 * \par 例：
 * \code
 * void *work;
 * CriSint32 wksz;
 * CriFsBinderId bndrid;
 * criFsBinder_GetWorkSizeForBindDirectory(bndrhn, "/cri/samples/", &wksz);
 * work = malloc(wksz);
 * criFsBinder_BindDirectory(bndrhn, bndrhn, "/cri/samples/", work, wksz, &bndrid);
 * \endcode
 * \sa criFsBinder_GetWorkSizeForBindDirectory(), criFsBinder_SetUserHeapFunc(), criFsBinder_GetStatus(), criFsBinder_Unbind(), criFsBinder_BindCpk(), criFsBinder_BindFiles()
 */
CriError CRIAPI criFsBinder_BindDirectory(CriFsBinderHn bndrhn, CriFsBinderHn srcbndrhn, const CriChar8 *path, void *work, CriSint32 worksize, CriFsBinderId *bndrid);

/*JP
 * \brief バインダIDの削除（アンバインド）
 * \ingroup FSLIB_BINDER
 * \param[in]	bndrid		バインダID
 * \return		CriError	エラーコード
 * \par 説明：
 * バインダIDをバインダから削除します。<br>
 * 指定されたバインダIDを削除できない場合、CRIERR_NGを返します。<br>
 * 本関数は完了復帰関数です。本関数の内部処理では、必要に応じてファイルのクローズ処理を行います。<br>
 * アンバインドするバインダIDに依存している他のバインダIDも同時にアンバインドされます（暗黙的アンバインド）。<br>
 * 例えば、CPKバインダIDのコンテンツファイルをFILEバインドしているバインダIDは、
 * 参照元のCPKバインダIDがアンバインドされると、暗黙的アンバンドされます。
 * 暗黙的にアンバインドされた項目は、暗黙的アンバインドリストに追加されます。<br>
 * 暗黙的アンバインドされたバインダIDは、通常どおりにcriFsBinder_Unbind関数でアンバインドするか、
 * criFsBinder_CleanImplicitUnbindList関数で暗黙的アンバインドリストをクリアする必要があります。
 * \par 例：
 * \code
 * // CPKファイルのバインド
 * criFsBinder_BindCpk(bndrhn, NULL, cpkpath, cpkwk, cpkwksz, &cpkid);
 * 	:
 * // fileidは、cpkidのコンテンツファイルをバインド
 * criFsBinder_BindFile(bndrhn, bndrhn, cntspath, filewk, filewksz, &fileid);
 * 	:
 * // CPKバインダIDのアンバインド
 * criFsBinder_Unbind(cpkid);	// ここでfileidは暗黙的アンバインドされる。
 * // FileバインダIDのアンバインド
 * criFsBinder_Unbind(fileid);
 * \endcode
 * \sa criFsBinder_BindCpk(), criFsBinder_BindFile(), criFsBinder_SetUserHeapFunc(), criFsBinder_CleanImplicitUnbindList()
 */
CriError CRIAPI criFsBinder_Unbind(CriFsBinderId bndrid);

/*JP
 * \brief 暗黙的アンバインドリストのクリア
 * \ingroup FSLIB_BINDER
 * \return	CriError	エラーコード
 * \par 説明：
 * 暗黙的アンバインドリストに登録されている全てのバインダIDを未使用リストへ戻します。<br>
 * 暗黙的アンバインドされるのバインダIDは、次の様な場合です。<br>
 *  ・他のバインダIDに依存したファイルをバインドしている場合（CPKのコンテンツファイルなど）。<br>
 *  ・親バインダIDがアンバインドされた場合。
 * \sa criFsBinder_Unbind()
 */
CriError CRIAPI criFsBinder_CleanImplicitUnbindList(void);

/*JP
 * \brief バインド状態の取得
 * \ingroup FSLIB_BINDER
 * \param[in]	bndrid	バインダID
 * \param[out]	status	CriFsBinderStatusバインダステータス
 * \return	CriError	エラーコード
 * \par 説明：
 * 指定されたバインダIDのバインド状態を取得します。 <br>
 * バインド状態が CRIFSBINDER_STATUS_COMPLETE になるまでは、
 * そのバインダIDによるファイルアクセスを行えません。<br>
 * \par 例：
 * \code
 * CriFsBinderStatus status;
 * criFsBinder_GetStatus(bndrid, &status);
 * \endcode
 * \sa criFsBinder_BindCpk(), criFsBinder_BindFile(), criFsBinder_BindFiles()
 */
CriError CRIAPI criFsBinder_GetStatus(CriFsBinderId bndrid, CriFsBinderStatus *status);

/*JP
 * \brief ファイル情報の取得（ファイル名指定）
 * \ingroup FSLIB_BINDER
 * \param[in]	bndrhn		バインダハンドル
 * \param[in]	filepath	ファイルのフルパス
 * \param[out]	finfo		ファイル情報構造体
 * \param[out]	exist		ファイル検索結果（TRUE:取得成功　FALSE:取得失敗）
 * \return		CriError	エラーコード
 * \par 説明：
 * 指定されたファイルをバインダから検索し、その情報を返します。<br>
 * バインド状態が CRIFSBINDER_STATUS_COMPLETE であるバインダIDのみを検索対象とします。<br>
 * ファイルが見付かった場合は existにTRUEを、ファイル情報構造体(finfo)にファイル情報をセットします。<br>
 * ファイルが見付からない場合、existにFALSEをセットします。<br>
 * finfoがNULLの場合、existにファイルの検索結果のみをセットします。
 * \par 例：
 * \code
 * CriFsBinderFileInfo finfo;
 * CriBool exist;
 * criFsBinder_Find(bndrhn, "a.txt", &finfo, &exist);
 * if (exist == TRUE) { // File is found. 
 * }
 * else {// File cannot found. 
 * }
 * \endcode
 * * \sa criFsBinder_GetStatus()
*/
CriError CRIAPI criFsBinder_Find(CriFsBinderHn bndrhn, const CriChar8 *filepath, CriFsBinderFileInfo *finfo, CriBool *exist);

/*JP
 * \brief ファイル情報の取得（ID指定）
 * \ingroup FSLIB_BINDER
 * \param[in]	bndrhn		バインダハンドル
 * \param[in]	id			CPKコンテンツファイルID
 * \param[out]	finfo		ファイル情報構造体
 * \param[out]	exist		ファイル検索結果（TRUE:取得成功　FALSE:取得失敗）
 * \return		CriError 	エラーコード
 * \par 説明：
 * バインダハンドルから、指定されたIDのファイルを検索し、その情報を返します。<br>
 * ID情報つきCPKファイルをバインドしている必要があります。<br>
 * バインド状態が CRIFSBINDER_STATUS_COMPLETE であるバインダIDのみを検索対象とします。<br>
 * ファイルが見付かった場合は existにTRUEをセットし、ファイル情報構造体(finfo)にファイル情報をセットします。<br>
 * ファイルが見付からない場合、existにFALSEをセットします。<br>
 * finfoがNULLの場合、existにファイルの検索結果(TRUE/FALSE)のみをセットします。
 * \par 例：
 * \code
 * CriFsBinderFileInfo finfo;
 * CriBool exist;
 * criFsBinder_FindById(bndrhn, 10, &finfo, &exist);
 * if (exist == TRUE) { // File is found.
 * }
 * else { // File cannot found.
 * }
 * \endcode
 * \sa criFsBinder_GetStatus() criFsBinder_BindCpk()
*/
CriError CRIAPI criFsBinder_FindById(CriFsBinderHn bndrhn, CriUint16 id, CriFsBinderFileInfo *finfo, CriBool *exist);

/*JP
 * \brief CriFsBinderHnの取得
 * \ingroup FSLIB_BINDER
 * \param[in]	bndrid		バインダID
 * \param[out]	bndrhn		バインダハンドル
 * \return		CriError	エラーコード
 * \par 説明：
 * CriFsBinderIdをCriFsBinderHnに変換します。<br>
 * 新たにCriFsBinderHnを生成するものではなく、型変換を行うものと考えてください。 <br>
 * よって、実体としては同じものを指しており、本関数でCriFsBinderHnのリソースを消費することはありません。<br>
 * 元のCriFsBinderIdもそのまま CriFsBinderId として使用できます。<br>
 * バインドされたファイルの情報を得る場合、バインドされたバインダIDが順に検索されます。<br>
 * このため、特定のバインダIDにあるファイルにアクセスしたい場合、目的のバインダIDから
 * バインダハンドルを取得して使用することで、効率的な検索を行うことが可能になります。<br>
 * \par 注意：
 * 本関数により取得された CriFsBinderHn は ::criFsBinder_Destroy 関数では破棄できません。
 * 元となる CriFsBinderId を ::criFsBinder_Unbind 関数でアンバインドしてください。<br>
 * \par 例：
 * \code
 * // CPKをバインドする
 * criFsBinder_BindCpk(parent_bndrhn, NULL, cpkpath, work, worksize, &cpk_bndrid);
 * // バインダID からバインダハンドルを得る
 * criFsBinder_GetHandle(cpk_bndrid, &cpk_bndrhn);
 * // このハンドルを用いてファイル情報を取得する
 * criFsBinder_Find(cpk_bndrhn, filepath, &finfo, &exist);
 *  :
 * // バインダIDをアンバインドする。取得されたバインダハンドルも使用できなくなる
 * criFsBinder_Unbind(cpk_bndrid);
 * \endcode
 * \sa criFsBinder_Unbind
 */
CriError CRIAPI criFsBinder_GetHandle(CriFsBinderId bndrid, CriFsBinderHn *bndrhn);

/*JP
 * \brief ファイルサイズの取得（ファイル名指定）
 * \ingroup FSLIB_BINDER
 * \param[in]	bndrhn		バインダハンドル
 * \param[in]	filepath	ファイルのフルパス
 * \param[out]	size		ファイルのサイズ
 * \return		CriError	エラーコード
 * \par 説明：
 * 指定されたファイルのファイルサイズを取得します。<br>
 * まず、bndrhn のバインダから目的のファイルを検索します。 <br>
 * bndrhn に目的のファイルが存在しない場合、デフォルトデバイスのfilepathのファイルを探します。
 * このとき、ファイルをオープン待ちが入る場合があります。<br>
 * バインド状態が CRIFSBINDER_STATUS_COMPLETE であるバインダIDが検索対象とします。<br>
 * 指定されたファイルが存在しない場合、sizeには負値が設定されます。
 * \attention
 * バインダハンドルにNULLを指定した場合や、criFsBinder_BindDirectory関数でディレクトリをバインドしたハンドルを指定した場合、
 * 本関数内で長時間処理がブロックされる場合があります。
 * \sa criFsBinder_GetFileSizeById()
 */
CriError CRIAPI criFsBinder_GetFileSize(CriFsBinderHn bndrhn, const CriChar8 *filepath, CriSint64 *size);

/*JP
 * \brief ファイルサイズの取得（ID指定）
 * \ingroup FSLIB_BINDER
 * \param[in]	bndrhn		バインダハンドル
 * \param[in]	id			ファイルのID
 * \param[out]	size		ファイルのサイズ
 * \return		CriError	エラーコード
 * \par 説明：
 * ファイルサイズを取得します。<br>
 * ID情報つきCPKファイルがバインドされている必要があります。<br>
 * bndrhn のバインダから目的のファイルを検索します。 
 * バインド状態が CRIFSBINDER_STATUS_COMPLETE であるCPKバインダIDのみを検索対象とします。<br>
 * \sa criFsBinder_GetFileSize()
 */
CriError CRIAPI criFsBinder_GetFileSizeById(CriFsBinderHn bndrhn, CriUint16 id, CriSint64 *size);

/*JP
 * \brief プライオリティ値の取得
 * \ingroup FSLIB_BINDER
 * \param[in]	bndrid		バインダID
 * \param[out]	priority	プライオリティ値
 * \return		CriError	エラーコード
 * \par 説明：
 * バインダIDのプライオリティ値を取得します。<br>
 * プライオリティにより、バインダハンドル内における、バインダIDの検索順を制御できます。<br>
 * バインド時のプライオリティ値は０で、検索順は同プライオリティ内のバインド順になります。<br>
 * プライオリティ値の大きい方が高プライオリティとなり、検索順が高くなります。
 * \sa criFsBinder_SetPriority()
 */
CriError CRIAPI criFsBinder_GetPriority(CriFsBinderId bndrid, CriSint32 *priority);

/*JP
 * \brief プライオリティ値の設定
 * \ingroup FSLIB_BINDER
 * \param[in]	bndrid		バインダID
 * \param[in]	priority	プライオリティ値
 * \return		CriError	エラーコード
 * \par 説明：
 * バインダIDにプライオリティを設定します。 <br>
 * プライオリティにより、バインダハンドル内における、バインダIDの検索順を制御できます。<br>
 * バインド時のプライオリティ値は０で、検索順は同プライオリティ内のバインド順になります。<br>
 * プライオリティ値の大きい方が高プライオリティとなり、検索順が高くなります。
 * \par 例：
 * \code
 * // a.cpk(a_id), b.cpk(b_id) の順にバインド
 * criFsBinder_BindCpk(bndrhn, NULL, "a.cpk", a_wk, a_wksz, a_id);
 * criFsBinder_BindCpk(bndrhn, NULL, "b.cpk", a_wk, a_wksz, b_id);
 * // この時点では a_id, b_idの順に検索される。
 * 	:
 * criFsBinder_SetPriority(b_id, 1);
 * // b_id のプライオリティをデフォルト値よりも上げたので、
 * // 検索順は b_id, a_id となる。
 * \endcode
 * \sa criFsBinder_GetPriority()
*/
CriError CRIAPI criFsBinder_SetPriority(CriFsBinderId bndrid, CriSint32 priority);

/*JP
 * \brief カレントディレクトリの設定
 * \ingroup FSLIB_BINDER
 * \param[in]	bndrId		バインダID
 * \param[in]	path		カレントディレクトリ
 * \param[in]	work		カレントディレクトリ名保存用ワーク領域
 * \param[in]	worksize	カレントディレクトリ名保存用ワーク領域サイズ
 * \return		CriError	エラーコード
 * \par 説明：
 * バインダIDにカレントディレクトリを設定します。<br>
 * 必要なワーク領域が確保できない場合、カレントディレクトリの設定に失敗します。<br>
 * この場合、既に設定されているカレントディレクトリ設定は破棄されます。<br>
 * バインダIDを利用してファイルを参照する際に、カレントディレクトリがパス名の前に付加されます。<br>
 * 指定するワーク領域のサイズは、設定するカレントディレクトリ名を格納するために使用されます。<br>
 * 最低でも、strlen(path)+1 バイトの領域を確保して渡してください。
 * メモリ確保／解放コールバック関数が登録されている場合、ワーク領域にNULL(ワークサイズは０）を設定すると、
 * 必要なワーク領域をメモリ確保／解放コールバック関数を使用して動的に確保します。<br>
 * \par 例：
 * \code
 * // バインド直後は、カレントディレクトリは設定されていない。
 * criFsBinder_BindCpk(bndrhn, NULL, "cpk.cpk", wk, wksz, bndrid);
 * 	:
 * criFsBinder_Find(bndrhn, "a.txt", NULL, &exist);　// "a.txt" で検索される
 * 	:
 * // カレントディレクトリ"/folder/"を設定
 * worksz = strlen("/folder/")+1;
 * work = malloc(worksz);
 * criFsBinder_SetCurrentDirectory(bndrid, "/folder/", work, worksz);
 * criFsBinder_Find(bndrhn, "a.txt", NULL, &exist);　// "/folder/a.txt" で検索される
 * \endcode
 * \sa criFsBinder_SetUserHeapFunc()
 */
CriError CRIAPI criFsBinder_SetCurrentDirectory(CriFsBinderId bndrId, const CriChar8 *path, void *work, CriSint32 worksize);

/*JP
 * \brief CPKコンテンツファイル情報の取得
 * \ingroup FSLIB_BINDER
 * \param[in]	bndrhn		バインダハンドル
 * \param[in]	id			ファイルID
 * \param[out]	cfinf		CriFsBinderContentsFileInfo構造体へのポインタ
 * \return		CriError	エラーコード
 * \par 説明：
 * ID+ファイル名情報付きCPKファイルから、指定されたファイルIDのファイル情報を取得します。<br>
 * 指定されたファイルを格納しているCPKが、ID+ファイル名情報付CPKである必要があります。<br>
 * 指定したバインダハンドルに、同じIDのファイルが複数存在する場合、最初に見付けたファイルを
 * 格納しているCPKが選択されます。<br>
 * 特定のCPKファイルを直接指定したい場合、criiFsBinder_GetHandle関数により、そのCPKのバインダIDから
 * バインダハンドルを取得し、本関数の引数としてください。<br>
 * \par 注意：
 * 本機能を使用するには、『CPK File Builder Ver.1.03以降』を使用して作成した、ID+ファイル名情報付CPKを
 * 用いる必要があります。
 * \par 例：
 * \code
 * CriFsBinderContentsFileInfo cfinf;
 * CriUint16 id = 0x0010;
 * // CPKをバインド
 * criFsBinder_BindCpk(parent_bndrhn, NULL, cpkpath, work, worksize, &cpk_bndrid);
 * // ID 0x0010 のファイル情報を取得
 * criFsBinder_GetContentsFileInfoById(parent_bndrhn, 0x0010, &cfinf);
 * \endcode
 */
CriError CRIAPI criFsBinder_GetContentsFileInfoById(CriFsBinderHn bndrhn, CriUint16 id, CriFsBinderContentsFileInfo *cfinf);

/*JP
 * \brief バインダID情報の取得
 * \ingroup FSLIB_BINDER
 * \param[in]	bndrid		バインダID
 * \param[out]	binf		取得情報
 * \return		CriError	エラーコード
 * \par 説明：
 * 指定されたバインダIDのバインダ種別（CPK/ファイル/ディレクトリ等）やバインドしたファイル名、プライオリティ設定などの
 * 情報を取得します。<br>
 */
CriError CRIAPI criFsBinder_GetBinderIdInfo(CriFsBinderId bndrid, CriFsBinderInfo *binf);

/*JP
 * \brief グループファイル数の取得
 * \ingroup FSLIB_BINDER
 * \param[in]	bndrid		バインダID
 * \param[in]	groupname	グループ名
 * \param[in]	attrname	アトリビュート名
 * \param[out]	groupfiles	グループファイル数
 * \return		CriError	エラーコード
 * \par 説明：
 * 指定したバインダID、グループ名、アトリビュート名に適合するファイルの数を取得します。<br>
 * 適合するグループファイルが存在しない場合、ファイル数は０になります。<br>
 * 無効なバインダIDを指定した場合、エラーコールバックが起きます。<br>
 */
CriError CRIAPI criFsBinder_GetNumberOfGroupFiles(CriFsBinderId bndrid, const CriChar8 *groupname, const CriChar8 *attrname, CriSint32 *groupfiles);


/*==========================================================================
 *      CriFsLoader API
 *=========================================================================*/
/*JP
 * \brief CRI File System - Loader オブジェクト
 * \ingroup FSLIB_BASIC
 * \par 説明:
 * CriFsLoaderとはファイルデータを簡単に読み出すためのモジュールです。
 */
/*EN
 * \brief CRI File System - Loader Object
 * \ingroup FSLIB_BASIC
 * \par
 * CriFsLoader is loading module. It is very simple and easy.
 */

/*JP
 * \brief CriFsLoaderの作成
 * \ingroup FSLIB_BASIC
 * \param[out]	loader	CriFsLoaderハンドル
 * \return	CriError	エラーコード
 * \par 説明:
 * CriFsLoaderを作成します。
 */
/*EN
 * \brief Create a CriFsLoader
 * \ingroup FSLIB_BASIC
 * \param[out]	loader	CriFsLoader handle
 * \return	CriError	Error information
 * \par
 * This function creates a CriFsLoader.
 */
CriError CRIAPI criFsLoader_Create(CriFsLoaderHn *loader);

/*JP
 * \brief CriFsLoaderの破棄
 * \ingroup FSLIB_BASIC
 * \param[in]	loader	CriFsLoaderハンドル
 * \return	CriError	エラーコード
 * \par 説明:
 * CriFsLoaderを破棄します。
 * \sa criFsLoader_Create
 */
/*EN
 * \brief Destroy CriFsLoader
 * \ingroup FSLIB_BASIC
 * \param[in]	loader	CriFsLoader handle
 * \return	CriError	Error information
 * \par
 * This function deletes the CriFsLoader.
 * \sa criFsLoader_Create
 */
CriError CRIAPI criFsLoader_Destroy(CriFsLoaderHn loader);

/*JP
 * \brief ロード完了コールバックの登録
 * \ingroup FSLIB_BASIC
 * \param[in]	loader	CriFsLoaderハンドル
 * \param[in]	func	コールバック関数
 * \param[in]	obj		コールバック関数へ渡す引数
 * \return	CriError	エラーコード
 * \par 説明:
 * ロード完了時に実行されるコールバック関数を登録します。<br>
 * ロード完了コールバックは、ローダのステータスが CRIFSLOADER_STATUS_LOADING から
 * 他のステータスに遷移した直後に呼び出されます。<br>
 * （ CRIFSLOADER_STATUS_COMPLETE 以外にも、 CRIFSLOADER_STATUS_STOP や 
 * CRIFSLOADER_STATUS_ERROR に遷移する際にもコールバックは実行されます。）<br>
 * \par 備考:
 * 厳密には、ステータス遷移～コールバック実行までの間に他の処理が割り込みで動作する
 * 余地があるため、ステータス遷移とコールバック実行のタイミングがズレる可能性があります。<br>
 * \attention
 * ロード完了コールバックを実行している間、他のファイルのロードがブロックされます。<br>
 * そのため、ロード完了コールバック内で負荷の高い処理を行なわないよう、ご注意ください。<br>
 */
CriError CRIAPI criFsLoader_SetLoadEndCallback(
	CriFsLoaderHn loader, CriFsLoaderLoadEndCbFunc func, void *obj);

/*JP
 * \brief データのロード
 * \ingroup FSLIB_BASIC
 * \param[in]	loader	CriFsLoaderハンドル
 * \param[in]	binder	CriFsBinderハンドル
 * \param[in]	path	ファイルパス名
 * \param[in]	offset	ファイルの先頭からのオフセット位置
 * \param[in]	load_size	ロードサイズ
 * \param[in]	buffer	バッファへのポインタ
 * \param[in]	buffer_size	バッファのサイズ
 * \return	CriError	エラーコード
 * \par 説明:
 * 指定されたバインダとファイル名で、データの読み込みを開始します。<br>
 * ファイル内の offset バイト目から、load_size バイト分読み込みます。<br>
 * 本関数は即時復帰関数です。<br>ロードの完了状態を取得するには criFsLoader_GetStatus関数を使用してください。<br>
 * \sa criFsLoader_GetStatus
 */
/*EN
 * \brief Loading File Data
 * \ingroup FSLIB_BASIC
 * \param[in]	loader	CriFsLoader handle
 * \param[in]	binder	CriFsBinder handle
 * \param[in]	path 	File path
 * \param[in]	offset	Seek position from top of file
 * \param[in]	load_size	Loading size
 * \param[in]	buffer	buffer
 * \param[in]	buffer_size Size of buffer
 * \return	CriError	Error information
 * \par
 * This function starts loading of data by appointed File path and the binder.<br>
 * This function is return immediately.
 * You use an criFsLoader_GetStatus function to examine loading completion.<br>
 * \sa criFsLoader_GetStatus
 */
CriError CRIAPI criFsLoader_Load(CriFsLoaderHn loader,
	CriFsBinderHn binder, const CriChar8 *path, CriSint64 offset,
	CriSint64 load_size, void *buffer, CriSint64 buffer_size);


/*JP
 * \brief データのロード (CPKファイル内のファイルIDを指定)
 * \ingroup FSLIB_BASIC
 * \param[in]	loader	CriFsLoaderハンドル
 * \param[in]	binder	CriFsBinderハンドル
 * \param[in]	id ファイルID
 * \param[in]	offset	ファイルの先頭からのオフセット位置
 * \param[in]	load_size	ロードサイズ
 * \param[in]	buffer	バッファへのポインタ
 * \param[in]	buffer_size	バッファのサイズ
 * \return	CriError	エラーコード
 * \par 説明:
 * 指定されたバインダとファイルIDで、データの読み込みを開始します。<br>
 * ファイル内の offset バイト目から、load_size バイト分読み込みます。<br>
 * 本関数は即時復帰関数です。<br>ロードの完了状態を取得するには criFsLoader_GetStatus関数を使用してください。<br>
 * \sa criFsLoader_GetStatus
 */
/*EN
 * \brief Loading File Data (Using File ID in CPK File)
 * \ingroup FSLIB_BASIC
 * \param[in]	loader	CriFsLoader handle
 * \param[in]	binder	CriFsBinder handle
 * \param[in]	id	File ID
 * \param[in]	offset	Seek position from top of file
 * \param[in]	load_size	Loading size
 * \param[in]	buffer	buffer
 * \param[in]	buffer_size Size of buffer
 * \return	CriError	Error information
 * \par
 * This function starts loading of data by appointed File ID and the binder.<br>
 * This function is return immediately.
 * You use an criFsLoader_GetStatus function to examine loading completion.<br>
 * \sa criFsLoader_GetStatus
 */
CriError CRIAPI criFsLoader_LoadById(CriFsLoaderHn loader,
	CriFsBinderHn binder, CriUint16 id, CriSint64 offset,
	CriSint64 load_size, void *buffer, CriSint64 buffer_size);

/*JP
 * \brief ロードの停止
 * \ingroup FSLIB_BASIC
 * \param[in]	loader	CriFsLoaderハンドル
 * \return	CriError	エラーコード
 * \par 説明:
 * ロードを停止します。<br>
 * 本関数は即時復帰関数です。停止状態を取得するには ::criFsLoader_GetStatus 関数を使用してください。<br>
 * \attention
 * 本関数を実行しても、ローダのステータスが CRIFSLOADER_STATUS_STOP に変わるまでは、バッファへのデータ転送が続いている可能性があります。<br>
 * ステータスが更新されるまでは、データロード先のバッファを解放しないでください。<br>
 * \sa criFsLoader_GetStatus
 */
/*EN
 * \brief Stop Loading
 * \ingroup FSLIB_BASIC
 * \param[in]	loader	CriFsLoader handle
 * \return	CriError	Error information
 * \par
 * This function stops the loading.
 * \sa criFsLoader_GetStatus
 */
CriError CRIAPI criFsLoader_Stop(CriFsLoaderHn loader);

 /*JP
 * \brief ロードステータスの取得
 * \ingroup FSLIB_BASIC
 * \param[in]	loader	CriFsLoaderハンドル
 * \param[out]	status	ロードステータス
 * \return	CriError	エラーコード
 * \par 説明:
 * ロードステータスを取得します。
 * \image html fs_state_transition.png ファイルシステムローダの状態遷移図
 */
/*EN
 * \brief Get Loading status
 * \ingroup FSLIB_BASIC
 * \param[in]	loader	CriFsLoader handle
 * \param[out]	status	Loading status
 * \return	CriError	Error information
 * \par
 * This function returns Loading status.
 * \image html fs_state_transition.png Transition of File System Loader Status
 */
CriError CRIAPI criFsLoader_GetStatus(CriFsLoaderHn loader, CriFsLoaderStatus *status);

/*JP
 * \brief ロードサイズの取得
 * \ingroup FSLIB_BASIC
 * \param[in]	loader	CriFsLoaderハンドル
 * \param[out]	size	ロードサイズ
 * \return	CriError	エラーコード
 * \par 説明:
 * ロードされたサイズを取得します。<br>
 */
/*EN
 * \brief Get Loading size
 * \ingroup FSLIB_BASIC
 * \param[in]	loader	CriFsLoader handle
 * \param[out]	size	Loading size
 * \return	CriError	Error information
 * \par
 * This function returns Loading size.
 */
CriError CRIAPI criFsLoader_GetLoadSize(CriFsLoaderHn loader, CriSint64 *size);

/*JP
 * \brief プライオリティの取得
 * \ingroup FSLIB_BASIC
 * \param[in]	loader	CriFsLoaderハンドル
 * \param[out]	priority	読み込みプライオリティ
 * \return	CriError	エラーコード
 * \par 説明:
 * データロードのプライオリティを取得します。
 * \sa criFsLoader_SetPriority
 */
/*EN
 * \brief Get a Priority
 * \ingroup FSLIB_BASIC
 * \param[in]	loader	CriFsLoader handle
 * \param[out]	priority	Priority of loader
 * \return	CriError	Error information
 * \par
 * This function gets priority of the loader.
 */
CriError CRIAPI criFsLoader_GetPriority(CriFsLoaderHn loader, CriFsLoaderPriority *prio);

/*JP
 * \brief プライオリティの設定
 * \ingroup FSLIB_BASIC
 * \param[in]	loader	CriFsLoaderハンドル
 * \param[in]	priority	読み込みプライオリティ
 * \return	CriError	エラーコード
 * \par 説明:
 * データロードのプライオリティを設定します。<br>
 * 複数のローダに対し、同時に ::criFsLoader_Load 関数でロードを実行した場合、
 * プライオリティの高いローダが先に読み込みを行います。<br>
 * また、既に低プライオリティのローダが巨大なデータを読み込んでいる最中でも、
 * 後から高プライオリティのローダの読み込みを開始すれば、低プライオリティのローダの処理に割り込んで、
 * 高プライオリティのローダの読み込みが先に実行されます。<br>
 * \per 備考:
 * 複数のローダが全て同一プライオリティであった場合、
 * データの読み込みは ::criFsLoader_Load 関数を実行した順に処理されます。<br>
 * \attention
 * ファイルの読み込みが行なわれていない状態でロードを開始した場合、
 * プライオリティに関係なく、そのロード処理が即座に開始されます。<br>
 * そのため、ファイルの読み込みが行なわれていない状態で低プライオリティローダの読み込みを行なった場合、
 * 直後に高プライオリティのローダで読み込みを開始したとしても、
 * 低プライオリティローダの読み込みがある程度行なわれることになります。<br>
 * （単位読み込みサイズ分のデータを処理するまでは、他のローダに処理がスイッチすることはありません。）<br>
 * \sa criFsLoader_GetPriority, criFsLoader_Load, criFsLoader_SetReadUnitSize
 */
/*EN
 * \brief Set a Priority
 * \ingroup FSLIB_BASIC
 * \param[in]	loader	CriFsLoader handle
 * \param[in]	priority	Priority of loader
 * \return	CriError	Error information
 * \par
 * This function sets priority of the loader.
 */
CriError CRIAPI criFsLoader_SetPriority(CriFsLoaderHn loader, CriFsLoaderPriority prio);

/*JP
 * \brief 単位読み込みサイズの設定
 * \ingroup FSLIB_BASIC
 * \param[in]	loader	CriFsLoaderハンドル
 * \param[in]	unit_size	単位読み込みサイズ
 * \return	CriError	エラーコード
 * \param err エラーコード
 * \par 説明:
 * 単位読み込みサイズを設定します。
 * CriFsLoaderは、大きなサイズのリード要求を処理する際、それを複数の小さな単位のリード処理に分割して連続実行します。<br>
 * この関数を使用することで単位リード処理サイズを変更することが可能です。<br>
 * リード要求のキャンセルや、高プライオリティのリード処理の割り込み等は、単位リードサイズ境界でのみ処理されます。<br>
 * そのため、ユニットサイズを小さく設定すると、I/O処理のレスポンスが向上します。逆に、ユニットサイズを大きく設定すると、ファイル単位の読み込み速度が向上します。
 */
/*EN
 * \brief Get Read Unit Size
 * \ingroup FSLIB_BASIC
 * \param[in]	loader	CriFsLoader handle
 * \param[in]	unit_size	The size of read unit
 * \return	CriError	Error information
 * \par
 * When loading large data, CriFsLoader divides it into multiple small unit, and reads them.<br>
 * This function sets the size of read units.<br>
 * Interrupt request, such as cancel of current load process, can be applied only at unit boundary.<br>
 * Therefore, if the unit size is smaller, the response of I/O process becomes better. If the unit size is larger, the speed of single reading becomes better.
 */
CriError CRIAPI criFsLoader_SetReadUnitSize(CriFsLoaderHn loader, CriSint64 unit_size);

/*==========================================================================
 *      CriFsGroupLoader API
 *=========================================================================*/
/*JP
 * \brief CRI File System - Group Loader オブジェクト
 * \ingroup FSLIB_GROUPLOADER
 * \par 説明:
 * CriFsGroupLoaderとは、CPKファイル内でグループとして関連付けられた一連のファイルを、
 * 一括して読み出すためのモジュールです。
 */

/*JP
 * \brief グループローダの作成
 * \ingroup FSLIB_GROUPLOADER
 * \param[in]	binder_id		バインダID
 * \param[in]	groupname		グループ名
 * \param[in]	attrname		アトリビュート名
 * \param[out]	grouploaderhn	グループローダハンドル
 * \return		CriError		エラーコード
 * \par 説明：
 * グループローダを作成し、グループローダハンドルを返します。<br>
 * 本関数は完了復帰関数です。<br><br>
 * グループ情報付きのCPKファイルをバインドしたバインダIDが必要です。<br>
 * 指定されたグループ名やアトリビュート名が存在しない場合、グループローダを作成しません。<br>
 * グループローダが扱うグループ名とアトリビュート名は、グループローダ作成後に変更することは
 * できません。<br>
 * 他のグループ＋アトリビュートを扱う場合、別のグループローダを作成します。<br><br>
 * アトリビュート名にNULLを指定した場合、指定グループに属する全てのファイルがグループロードの対象
 * となります。<br>
 * また、パッキングツールのアトリビュート指定を「none」とした場合も、アトリビュート名にNULLを指定
 * します。
 * \par 例：
 * \code
 * CriFsBinderId bndrid;
 * CriFsGroupLoaderHn gldrhn;
 * // グループ情報付きCPKファイル"group.cpk"のバインド
 * criFsBinder_BindCpk(bndrhn, NULL, "group.cpk", wk, wksz, bndrid);
 * 	:
 * // グループ"GROUP1", アトリビュート"IMG"を扱うグループローダを作成。
 * criFsGroupLoader_Create(bndrid, "GROUP1", "IMG", &gldrhn);
 * \endcode
 * \code
 * // グループ"GROUP"の全てのファイルを扱うグループローダを作成。
 * criFsGroupLoader_Create(bndrid, "GROUP", NULL, &gldrhn);
 * \endcode
 * \sa criFsGroupLoader_Destroy()
 */
CriError CRIAPI criFsGroupLoader_Create(CriFsBinderId binder_id, const CriChar8 *groupname, const CriChar8 *attrname, CriFsGroupLoaderHn *grouploaderhn);

/*JP
 * \brief グループローダの破棄
 * \ingroup FSLIB_GROUPLOADER
 * \param[in]	grouploaderhn	グループローダハンドル
 * \return		CriError		エラーコード
 * \par 説明：
 * グループローダを破棄します。<br>
 * 本関数は完了復帰関数です。<br>
 * グループロード中に本関数を呼び出した場合はロードを中断し、ロードのためにグループローダ内部で確保していた
 *  ::CriFsLoaderHn を解放します。<br>
 * グループロード中のグループローダを破棄する場合、内部の ::CriFsLoaderHn が停止するのを待ちますので、
 * 本関数から復帰するまでに時間がかかる場合があります。<br>
 * これを回避するには、グループローダのステータスが ::CRIFSLOADER_STATUS_LOADING でないことを確認してから、
 * 本関数を呼出します。
 * \sa criFsGroupLoader_Create() criFsGroupLoader_GetStatus()
 */
CriError CRIAPI criFsGroupLoader_Destroy(CriFsGroupLoaderHn grouploaderhn);

/*JP
 * \brief ロード開始コールバック関数の設定
 * \ingroup FSLIB_GROUPLOADER
 * \param[in]	grouploaderhn	グループローダハンドル
 * \param[in]	func			グループローダコールバック関数
 * \param[in]	obj				グループローダコールバック関数引数
 * \return	CriError			エラーコード
 * \par 説明：
 * グループロード実施時にファイル毎に呼び出されるコールバック関数を設定します。<br>
 * 本関数で設定したコールバック関数は、ロードリクエストを行う前にファイル毎に呼出されます
 * （つまり、ファイル数と同じ回数コールバック関数が呼出されます）。<br>
 * グループロードコールバック関数を設定した場合、ファイル毎にコールバック関数を呼出すため、
 * 複数ファイルの一括ロードは行えません。<br>
 * グループロードコールバック関数にNULLを設定した場合、コールバック関数の設定は解除されます。<br>
 * <br>
 * ●コールバック関数について<br>
 * コールバック関数は引数として、obj:ユーザーが指定したオブジェクトと、gfinfo:ロードするファイルの
 * 情報構造体が渡されます。<br>
 * コールバック関数の返値は、そのファイルを読み込むバッファへのポインタとなります。<br>
 * ファイルの読込を行いたくない場合は、 NULL を返値としてください。<br>
 * \sa criFsGroupLoader_LoadBulk()
 */
CriError CRIAPI criFsGroupLoader_SetLoadStartCallback(CriFsGroupLoaderHn grouploaderhn, CriFsGroupLoaderLoadStartCbFunc func, void *obj);

/*JP
 * \brief グループファイル数の取得
 * \ingroup FSLIB_GROUPLOADER
 * \param[in]	grouploaderhn	グループローダハンドル
 * \param[out]	nfiles			グループファイル数
 * \return		CriError		エラーコード
 * \par 説明：
 * 指定のグループに属するファイル数を取得します。<br>
 * ::criFsGroupLoader_LoadBulk 関数の引数 gfinf の配列の要素数は、本関数で取得される
 * グループファイル数となります。
 * \sa criFsGroupLoader_GetTotalGroupDataSize(), criFsGroupLoader_LoadBulk()
 */
CriError CRIAPI criFsGroupLoader_GetNumberOfGroupFiles(CriFsGroupLoaderHn grouploaderhn, CriSint32 *nfiles);

/*JP
 * \brief グループデータサイズの取得
 * \ingroup FSLIB_GROUPLOADER
 * \param[in]	grouploaderhn	グループローダハンドル
 * \param[out]	datasize 		データサイズ
 * \return		CriError		エラーコード
 * \par 説明：
 * グループロードに必要な読込領域のサイズを取得します。<br>
 * アライメントなども加味されたデータサイズとなります。
 * \sa criFsGroupLoader_GetNumberOfGroupFiles(), criFsGroupLoader_LoadBulk()
 */
CriError CRIAPI criFsGroupLoader_GetTotalGroupDataSize(CriFsGroupLoaderHn grouploaderhn, CriSint64 *datasize);

/*JP
 * \brief グループファイル情報の取得
 * \ingroup FSLIB_GROUPLOADER
 * \param[in]	grouploaderhn	グループローダハンドル
 * \param[out]	gfinf[]			CriFsGroupFileInfo構造体の配列
 * \param[in]	numginf			配列(gfinf[])の要素数
 * \return		CriError		エラーコード
 * \par 説明：
 * グループ化された複数ファイルデータ情報を取得します。<br>
 * ファイル数分の ::CriFsGroupFileInfo 構造体の配列を指定する必要があります。<br>
 * パッキングツールのアトリビュート指定を「none」としたグループに対しては、本関数に指定するグループ
 * ローダ作成時のアトリビュート名指定をNULLとして下さい。<br>
 * 取得されるグループファイル情報( ::CriFsGroupFileInfo )の内、datapointerで指される読込先はNULLとなります。
 * \par 例：
 * \code
 * CriSint32 nfiles;
 * CriFsGroupFileInfo *gfinf;
 * // グループファイル情報構造体配列領域の確保
 * criFsGroupLoader_GetNumberOfGroupFiles(gldrhn, &nfiles);
 * gfinf = malloc( sizeof(CriFsGroupFileInfo) * nfiles );
 * // グループファイル読み込み領域の確保
 * criFsGroupLoader_GetTotalGroupDataSize(gldrhn, &datasize);
 * databuff = malloc(datasize);
 * // グループロード情報の取得
 * criFsGroupLoader_GetGroupFileInfos(gldrhn, gfinf, nfiles);
 * \endcode
*/
CriError CRIAPI criFsGroupLoader_GetGroupFileInfos(
	CriFsGroupLoaderHn grouploaderhn, CriFsGroupFileInfo gfinf[], CriSint32 numgfinf);

/*JP
 * \brief グループロードの開始
 * \ingroup FSLIB_GROUPLOADER
 * \param[in]	grouploaderhn	グループローダハンドル
 * \param[out]	buffer			ロード先バッファへのポインタ
 * \param[in]	buffer_size		ロード先バッファのサイズ
 * \param[out]	gfinf[]			CriFsGroupInfo構造体の配列
 * \param[in]	numginf			配列(gfinf[])の要素数
 * \return		CriError		エラーコード
 * \par 説明：
 * グループ化された複数ファイルデータの読み込みを開始します。<br>
 * ファイル数分の ::CriFsGroupFileInfo 構造体の配列を指定する必要があります。<br>
 * 指定のグループファイルを読み込めるサイズのロード領域が必要です。<br>
 * 本関数は即時復帰関数です。<br>ロードの完了状態を取得するには ::criFsGroupLoader_GetStatus 関数を
 * 使用してください。<br><br>
 * パッキングツールのアトリビュート指定を「none」としたグループを読み込む場合、本関数に指定するグループ
 * ローダは、グループローダ作成時にアトリビュート名指定をNULLとします。<br><br>
 * グループロードコールバック関数を設定した場合、コールバック関数の返値をロードアドレスとしますので、
 * 本関数の引数 buffer, buffer_size は参照されません。<br>
 * グループローダ内部で複数の ::CriFsLoaderHn を利用してロードを行います。 ::CriFsLoaderHn を１つも作成
 * できない場合、エラーコールバックが呼ばれます。グループローダで作成した ::CriFsLoaderHn は、
 * グループロード完了時に破棄されます。
 * \par 例：
 * \code
 * CriSint32 nfiles;
 * CriFsGroupFileInfo *gfinf;
 * CriSint64 datasize;
 * void *databuff;
 * // グループファイル情報構造体配列領域の確保
 * criFsGroupLoader_GetNumberOfGroupFiles(gldrhn, &nfiles);
 * gfinf = malloc( sizeof(CriFsGroupFileInfo) * nfiles );
 * // グループファイル読み込み領域の確保
 * criFsGroupLoader_GetTotalGroupDataSize(gldrhn, &datasize);
 * databuff = malloc(datasize);
 * // グループロード
 * criFsGroupLoader_LoadBulk(gldrhn, databuff, datasize, gfinf, nfiles);
 * // グループロード完了待ち
 * for (;;) {
 * 	CriFsLoaderStatus status;
 * 	criFsGroupLoader_GetStatus(gldrhn, &status);
 * 	if (status == CRIFSLOADER_STATUS_COMPLETE) break;
 * }
 * \endcode
 * \sa criFsGroupLoader_GetStatus() criFsGroupLoader_SetLoadStartCallback()  criFsGroupLoader_Stop() criFsGroupLoader_Create()
 */
CriError CRIAPI criFsGroupLoader_LoadBulk(CriFsGroupLoaderHn gourploaderhn, void *buffer, CriSint64 buffer_size, CriFsGroupFileInfo gfinf[], CriSint32 numgfinf);

/*JP
 * \brief グループロードの停止（中断）
 * \ingroup FSLIB_GROUPLOADER
 * \param[in]	grouploaderhn	グループローダハンドル
 * \return		CriError		エラーコード
 * \par 説明：
 * グループローダでのファイルロードを中断します。<br>
 * グループロードを中断するまでにロードされたファイル数やファイルの内容はそのまま保持されます。<br>
 * <br>
 * 本関数は即時復帰関数です。<br>
 * グループロードの際、グループローダ内部で ::CriFsLoaderHn を作成してファイルロードを行います。
 * 本関数を呼出した場合、グループロードに使用している ::CriFsLoaderHn に中断（Stop）の指示を出して
 * 復帰します。<br>
 * そのため、本関数復帰時点では、まだファイルロード中である可能性があります。<br>
 * グループローダは、内部で使用している ::CriFsLoaderHn のロード停止確認後に、その ::CriFsLoaderHn を
 * 解放し、グループローダのステータスを ::CRIFSLOADER_STATUS_STOP にします（既にエラーステ
 * ータスである場合は、::CRIFSLOADER_STATUS_ERROR のままとなります）。<br>
 * この一連の処理は ::criFsGroupLoader_GetStatus 関数呼び出し時に行われますので、本関数呼び出し後は、
 * グループローダのステータスが ::CRIFSLOADER_STATUS_LOADING でないことを確認してください。<br>
 * そうしない場合、グループローダ内部で使用している ::CriFsLoaderHn が解放されませんので、
 * 他のグループローダが ::CriFsLoaderHn を確保できなくなる可能性があります。
 * <br>
 * \sa criFsGroupLoader_GetStatus() criFsGroupLoader_LoadBulk()
 */
CriError CRIAPI criFsGroupLoader_Stop(CriFsGroupLoaderHn grouploaderhn);

/*JP
 * \brief ロードステータスの取得
 * \ingroup FSLIB_GROUPLOADER
 * \param[in]	grouploaderhn	グループローダハンドル
 * \param[out]	status			CriFsGroupLoaderStatusロードステータス
 * \return		CriError		エラーコード
 * \par 説明：
 * グループローダのロードステータスを返します。<br>
 * グループロード対象の全てのファイルのロードを完了した場合、
 * ::CRIFSLOADER_STATUS_COMPLETE を返します。
 * \sa criFsGroupLoader_LoadBulk()
 */
CriError CRIAPI criFsGroupLoader_GetStatus(CriFsGroupLoaderHn grouploaderhn, CriFsLoaderStatus *status);

/*JP
 * \brief ロードされたファイル数の取得
 * \ingroup FSLIB_GROUPLOADER
 * \param[in]	grouploaderhn	グループローダハンドル
 * \param[out]	nfiles			ロードされたファイル数
 * \return		CriError		エラーコード
 * \par 説明：
 * ::criFsGroupLoader_LoadBulk 関数で既に読み込まれたファイルの数を返します。
 * \sa criFsGroupLoader_LoadBulk()
 */
CriError CRIAPI criFsGroupLoader_GetLoadedFiles(CriFsGroupLoaderHn grouploaderhn, CriSint32 *nfiles);

/*JP
 * \brief ファイルの読込状態の取得
 * \ingroup FSLIB_GROUPLOADER
 * \param[in]	gfinfo		読込状態を取得するファイルの CriFsGroupFileInfo 構造体へのポインタ
 * \param[out]	result		ファイルの読込状態（TRUE：読込済 FALSE:未読込)
 * \return		CriError	エラーコード
 * \par 説明：
 * 指定されたファイルが読み込まれたかどうかを取得します。<br>
 * 読込状態を取得するファイルの ::CriFsGroupFileInfo 構造体へのポインタは 
 * ::criFsGroupLoader_GetGroupFileInfoIndex 関数や
 * ::criFsGroupLoader_GetGroupFileInfo 関数で取得します。
 * \sa criFsGroupLoader_GetGroupFileInfoIndex(), criFsGroupLoader_GetGroupFileInfo()
 */
CriError CRIAPI criFsGroupLoader_IsLoaded(const CriFsGroupFileInfo *gfinfo, CriBool *result);

/*JP
 * \brief CriFsGroupFileInfo構造体の配列インデクスの取得
 * \ingroup FSLIB_GROUPLOADER
 * \param[in]	grouploaderhn	グループローダハンドル
 * \param[in]	fpath			ファイル名フルパス
 * \param[out]	index 			配列インデクス
 * \return		CriError		エラーコード
 * \par 説明：
 * 指定されたファイルの ::CriFsGroupFileInfo 構造体の配列インデクスを取得します。<br>
 * ファイル名はフルパス名で指定します。指定されたファイルの検索にはバイナリサーチを使用します。<br>
 * 指定されたファイルが見つからない場合は、返値が-1となります。<br>
 * グループロードされた個々のファイルが実際にロードされたアドレスは、
 * ::CriFsGroupFileInfo 構造体に記述されます。<br>
 * ロードされたデータへアクセスするには、::CriFsGroupFileInfo 構造体を取得する必要があります。<br>
 * ::CriFsGroupFileInfo 構造体を取得する方法には、ロードしたファイルのファイル名もしくは
 * コンテンツファイルIDを指定し、該当する構造体要素を取得する方法と、本関数で取得したインデクスにより
 * 構造体配列を直接アクセスする方法の２通りがあります。
 * \sa criFsGroupLoader_GetNumberOfGroupFiles(), criFsGroupLoader_GetGroupFileInfo()
 */
CriError CRIAPI criFsGroupLoader_GetGroupFileInfoIndex(CriFsGroupLoaderHn grouploaderhn,  const CriChar8 *fpath, CriSint32 *index);

/*JP
 * \brief CriFsGroupFileInfo構造体の配列インデクスの取得(ID指定)
 * \ingroup FSLIB_GROUPLOADER
 * \param[in]	grouploaderhn	グループローダハンドル
 * \param[in]	id				コンテンツファイルID
 * \param[out]	index 			配列インデクス
 * \return		CriError		エラーコード
 * \par 説明：
 * 指定されたファイルの ::CriFsGroupFileInfo 構造体の配列インデクスを取得します。<br>
 * ファイルはコンテンツファイルIDで指定します。指定されたファイルの検索にはリニアサーチを使用しています。<br>
 * 指定されたファイルが見つからない場合は、返値が-1となります。<br>
 * グループロードされた個々のファイルが実際にロードされたアドレスは、
 * ::CriFsGroupFileInfo 構造体に記述されます。<br>
 * 検索方法の都合上、既にグループロード済で ::CriFsGroupFileInfo 情報を取得している場合は、
 *  ::CriFsGroupFileInfo 情報のIDを直接検索する方が、本関数でインデクスを取得するよりも検索効率が高いです。<br>
 * ロードされたデータへアクセスするには、::CriFsGroupFileInfo 構造体を取得する必要があります。<br>
 * ::CriFsGroupFileInfo 構造体を取得する方法には、ロードしたファイルのファイル名もしくは
 * コンテンツファイルIDを指定し、該当する構造体要素を取得する方法と、本関数で取得したインデクスにより
 * 構造体配列を直接アクセスする方法の２通りがあります。
 * \sa criFsGroupLoader_GetNumberOfGroupFiles(), criFsGroupLoader_GetGroupFileInfoById()
 */
CriError CRIAPI criFsGroupLoader_GetGroupFileInfoIndexById(CriFsGroupLoaderHn grouploaderhn, CriSint16 id, CriSint32 *index);

/*JP
 * \brief CriFsGroupFileInfo構造体の取得
 * \ingroup FSLIB_GROUPLOADER
 * \param[in]	grouploaderhn	グループローダハンドル
 * \param[in]	fpath 			ファイル名フルパス
 * \param[out]	gfinfo			CriFsGroupFileInfo構造体へのポインタへのポインタ
 * \return		CriError 		エラーコード
 * \par 説明：
 * 指定されたファイルの ::CriFsGroupFileInfo 構造体へのポインタを取得します。<br>
 * ファイル名はフルパス名で指定します。<br>
 * 指定されたファイルが見つからない場合は、返値が NULL となります。
 * \sa criFsGroupLoader_GetNumberOfGroupFiles(), criFsGroupLoader_GetGroupFileInfoIndex()
 */
CriError CRIAPI criFsGroupLoader_GetGroupFileInfo(CriFsGroupLoaderHn grouploaderhn, const CriChar8 *fpath, const CriFsGroupFileInfo **gfinfo);

/*JP
 * \brief CriFsGroupFileInfo構造体の取得(ID指定)
 * \ingroup FSLIB_GROUPLOADER
 * \param[in]	grouploaderhn	グループローダハンドル
 * \param[in]	id	 			コンテンツファイルID
 * \param[out]	gfinfo			CriFsGroupFileInfo構造体へのポインタへのポインタ
 * \return		CriError 		エラーコード
 * \par 説明：
 * 指定されたファイルの ::CriFsGroupFileInfo 構造体へのポインタを取得します。<br>
 * ファイルはコンテンツファイルIDで指定します。<br>
 * 指定されたファイルが見つからない場合は、返値が NULL となります。
 * \sa criFsGroupLoader_GetNumberOfGroupFiles(), criFsGroupLoader_GetGroupFileInfoIndexById()
 */
CriError CRIAPI criFsGroupLoader_GetGroupFileInfoById(CriFsGroupLoaderHn grouploaderhn, CriSint16 id, const CriFsGroupFileInfo **gfinfo);

/*JP
 * \brief グループ名の取得
 * \ingroup FSLIB_GROUPLOADER
 * \param[in]	grouploaderhn	グループローダハンドル
 * \param[out]	groupname 		グループ名
 * \return		CriError 		エラーコード
 * \par 説明：
 * グループローダで扱うグループのグループ名を取得します。
 */
CriError CRIAPI criFsGroupLoader_GetGroupName(CriFsGroupLoaderHn grouploaderhn, const CriChar8 **groupname);

/*JP
 * \brief グループ属性の取得
 * \ingroup FSLIB_GROUPLOADER
 * \param[in] 	grouploaderhn	グループローダハンドル
 * \param[out]	attrname 		グループ属性
 * \return		CriError 		エラーコード
 * \par 説明：
 * グループローダで扱うグループのグループ属性を取得します。
 */
CriError CRIAPI criFsGroupLoader_GetAttributeName(CriFsGroupLoaderHn grouploaderhn, const CriChar8 **attrname);

/*JP
 * \brief プライオリティの取得
 * \ingroup FSLIB_GROUPLOADER
 * \param[in]	grouploader	CriFsGroupLoaderハンドル
 * \param[out]	priority	読み込みプライオリティ
 * \return	CriError	エラーコード
 * \par 説明:
 * グループローダ内で使用するローダ( ::CriFsLoaderHn )のプライオリティを取得します（初期値は ::CRIFSLOADER_PRIORITY_NORMAL です）。
 * ローダのプライオリティに関しては、::criFsLoader_GetPriority や ::criFsLoader_SetPriority の説明も参照ください。
 * \sa criFsGroupLoader_GetLoaderPriority, criFsLoader_GetPriority, criFsLoader_SetPriority
 */
CriError CRIAPI criFsGroupLoader_GetLoaderPriority(CriFsGroupLoaderHn grouploaderhn, CriFsLoaderPriority *prio);

/*JP
 * \brief プライオリティの設定
 * \ingroup FSLIB_GROUPLOADER
 * \param[in]	grouploader	CriFsGroupLoaderハンドル
 * \param[in]	priority	読み込みプライオリティ
 * \return	CriError	エラーコード
 * \par 説明:
 * グループローダ内で使用するローダ( ::CriFsLoaderHn )のプライオリティを設定します（初期値は ::CRIFSLOADER_PRIORITY_NORMAL です）。
 * ローダのプライオリティに関しては、::criFsLoader_GetPriority や ::criFsLoader_SetPriority の説明も参照ください。
 * \sa criFsGroupLoader_SetLoaderPriority, criFsLoader_SetPriority
 */
CriError CRIAPI criFsGroupLoader_SetLoaderPriority(CriFsGroupLoaderHn grouploaderhn, CriFsLoaderPriority prio);



/*==========================================================================
 *      Log Output API
 *=========================================================================*/
/*JP
 * \brief ログ出力機能の追加
 * \ingroup FSLIB_CRIFS
 * \param[in] mode				ログ出力モード
 * \param[in] param				拡張パラメータ
 * \return	CriError 			エラーコード
 * \par 説明:
 * ログ出力機能を有効にし、ファイルアクセスログの出力を開始します。<br>
 * 本関数を実行すると、ファイルにアクセスするタイミングで、デバッガ等にファイルアクセスログが出力されるようになります。<br>
 * \attention
 * 本関数を実行後、必ず対になる ::criFs_DetachLogOutput 関数を実行してください。<br>
 * また、 ::criFs_DetachLogOutput 関数を実行するまでは、本関数を再度実行することはできません。<br>
 * \sa criFs_DetachLogOutput
 */
CriError CRIAPI criFs_AttachLogOutput(CriFsLogOutputMode mode, void *param);

/*JP
 * \brief ログ出力機能の削除
 * \ingroup FSLIB_CRIFS
 * \return	CriError 			エラーコード
 * \par 説明:
 * ログ出力機能を無効にし、ファイルアクセスログの出力を停止します。<br>
 * 本関数を実行することで、デバッガ等へのファイルアクセスログの出力を停止することが可能です。<br>
 * \attention
 * ::criFs_DetachLogOutput 関数実行前に本関数を実行することはできません。<br>
 * \sa criFs_AttachLogOutput
 */
CriError CRIAPI criFs_DetachLogOutput(void);

/*JP
 * \brief ユーザ定義ログ出力関数の登録
 * \ingroup FSLIB_CRIFS
 * \param[in]	func			ログ出力関数
 * \param[in]	obj				ログ出力関数に渡すオブジェクト
 * \return	CriError 			エラーコード
 * \par 説明:
 * ログの出力関数をユーザ指定の関数に置き換えます。<br>
 * 本関数を使用することで、ファイルアクセスログの出力方法をユーザが自由に
 * カスタマイズすることが可能です。
 * \par 備考:
 * 本関数を使用していない場合や、ログ出力関数（func）にNULLを指定した場合、
 * CRI File Systemライブラリのデフォルトログ出力関数が使用されます。
 */
CriError CRIAPI criFs_SetUserLogOutputFunction(CriFsLogOutputFunc func, void *obj);

/*JP
 * \brief ロード区間の開始
 * \ingroup FSLIB_CRIFS
 * \param[in] name				ロード区間名
 * \return	CriError 			エラーコード
 * \par 説明:
 * ロード区間の開始を宣言します。<br>
 * ::criFs_AttachLogOutput 関数でファイルアクセスログの出力を有効にしている場合、本関数の引数（name）で指定したロード区間名がログに出力されます。<br>
 * ロード区間は、ファイルを最適に配置するための目安として使用されます。<br>
 * CPK File Builderでファイルアクセスログからグループを作成する場合、本関数で定義したロード区間がグループに変換されます。<br>
 * （同一ロード区間内でロードするファイル同士は、最適配置時に近い場所に配置される可能性が高くなります。）<br>
 * \attention
 * 複数のロード区間を重複させることはできません。<br>
 * 本関数を実行後、必ず対になる ::criFs_EndLoadRegion 関数を実行してください。
 * 本関数と ::criFs_BeginGroup 関数を併用することはできません。<br>
 * （CRI File System Ver.2.02.00より、本関数の機能が ::criFs_BeginGroup 関数に統合され、関数自体も ::criFs_BeginGroup 関数を呼び出すマクロに変更されました。）<br>
 * \sa criFs_EndLoadRegion, criFs_BeginGroup
 */
#define criFs_BeginLoadRegion(name)			criFs_BeginGroup(name, NULL)

/*JP
 * \brief ロード区間の終了
 * \ingroup FSLIB_CRIFS
 * \return	CriError 			エラーコード
 * \par 説明:
 * ロード区間の終了を宣言します。
 * \attention
 * 本関数と ::criFs_EndGroup 関数を併用することはできません。<br>
 * （CRI File System Ver.2.02.00より、本関数の機能が ::criFs_EndGroup 関数に統合され、関数自体も ::criFs_EndGroup 関数を呼び出すマクロに変更されました。）<br>
 * \sa criFs_BeginLoadRegion, criFs_EndGroup
 */
#define criFs_EndLoadRegion()				criFs_EndGroup()

/*==========================================================================
 *      CriFsStdio API
 *=========================================================================*/
/*JP
 * \brief ANSI C に準じたファイルオープン
 * \ingroup FSLIB_CRIFSSTDIO
 * \param[in] bndr  オープンしたいファイルがバインドされているCriFsBinderのハンドル
 * \param[in] fname オープンしたいファイルパス
 * \param[in] mode  オープンモード ("r":読み込み専用モード,"w":書き込み専用モード)
 * \return	CriFsStdioHn 成功した場合、有効なCriFsStdioハンドルを返します。<br>
 *                       失敗した場合はNULLを返します。                        
 * \par 説明：
 * 指定されたファイルをオープンします。<br>
 * 第一引数には、オープンしたいファイルがバインドされているバインダを指定します。<br>
 * プラットフォーム標準のファイルパスからファイルをオープンしたい場合、第一引数にはNULLを指定します。<br>
 * 第二引数には、オープンしたいファイルパスを文字列で指定します。<br>
 * 第三引数は、オープンのモードです。"r"を指定すると読み込み専用モード、<br>
 * "w"を指定すると書き込み専用モードでファイルをオープンします。<br>
 * 書き込み専用モードは、ファイル書き込みをサポートしているプラットフォームでのみ正常に動作し、
 * 未サポートのプラットフォームではエラーコールバックが発生し、オープンは失敗します。<br>
 * 
 * \sa criFsStdio_CloseFile
 */
CriFsStdioHn CRIAPI criFsStdio_OpenFile(CriFsBinderHn bndr, const char *fname, const char *mode);

/*JP
 * \brief ANSI C に準じたファイルクローズ
 * \ingroup FSLIB_CRIFSSTDIO
 * \param[in] stdhn クローズするファイルのCriFsStdioハンドル
 * \return CriError エラーコード<br>
 * \par 説明：
 * 指定したファイルをクローズします。<br>
 * 第一引数には、クローズしたいファイルのCriFsStdioハンドルを指定します。<br>
 * \sa criFsStdio_OpenFile
 */
CriError CRIAPI criFsStdio_CloseFile(CriFsStdioHn stdhn);

/*JP
 * \brief ANSI C に準じたAPIに基づくファイルサイズ取得
 * \ingroup FSLIB_CRIFSSTDIO
 * \param[in] stdhn サイズを取得したいファイルのCriFsStdioハンドル
 * \return CriSint64 指定したハンドルが有効であれば、ファイルサイズを返します。
 * \par 説明：
 * 指定したファイルのサイズを取得します。<br>
 * 第一引数には、サイズを取得したいファイルのCriFsStdioハンドルを指定します。<br>
 */
CriSint64 CRIAPI criFsStdio_GetFileSize(CriFsStdioHn stdhn);

/*JP
 * \brief ANSI C に準じたファイルリードオフセットの取得
 * \ingroup FSLIB_CRIFSSTDIO
 * \param[in] stdhn リードオフセットを取得したいファイルのCriFsStdioハンドル
 * \return CriSint64 指定したハンドルが有効であれば、リードオフセット（byte）を返します。
 * \par 説明：
 * 指定したファイルの読込位置を取得します。<br>
 * 第一引数には、読込位置を取得したいファイルの、CriFsStdioハンドルを指定します。<br>
 * \sa criFsStdio_SeekFile
 */
CriSint64 CRIAPI criFsStdio_TellFileOffset(CriFsStdioHn stdhn);

/*JP
 * \brief ANSI C に準じた ファイルリードオフセットのシーク
 * \ingroup FSLIB_CRIFSSTDIO
 * \param[in] stdhn                リードオフセットをシークしたいファイルのCriFsStdioハンドル
 * \param[in] offset               シークのオフセット（byte）
 * \param[in] CRIFSSTDIO_SEEK_TYPE シーク開始位置の指定
 * \return CriSint64 成功 0<br>
 *                   失敗 -1<br>
 * \par 説明：
 * 指定したファイルのリードオフセットをシークします。<br>
 * 第一引数には、リードオフセットをシークしたいファイルの、CriFsStdioハンドルを指定します。<br>
 * 第二引数には、シークのオフセットを指定します。単位はbyteです。<br>
 * \attention
 * ファイル先頭より手前にシークすることは出来ません。ファイルリードオフセットがファイル先頭より
 * 手前になるようシークオフセットを指定した場合、シーク結果のファイルリードオフセットはファイル先頭になります。<br>
 * 一方、ファイル終端を超えたシークは可能です。<br>
 * また、指定のCriFsStdioハンドルが中間バッファを持つ場合、
 * 本関数で中間バッファの有効範囲外にシークすると、中間バッファの内容が破棄されます。<br>
 * \sa criFsStdio_TellFileOffset
 * \sa criFsStdio_SetInterstageBuffer
 */
CriSint64 CRIAPI criFsStdio_SeekFile(CriFsStdioHn rdr, CriSint64 offset, CRIFSSTDIO_SEEK_TYPE seek_type);

/*JP
 * \brief ANSI C に準じたAPIに基づくファイル読込用中間バッファの設定
 * \ingroup FSLIB_CRIFSSTDIO
 * \param[in] stdhn            中間バッファを設定したいCriFsStdioハンドル
 * \param[in] temp_buffer      中間バッファの先頭アドレス
 * \param[in] temp_buffer_size 中間バッファのサイズ（byte）
 * \return CriSint64 中間バッファ設定後のファイル読込位置
 * \par 説明：
 * 指定したファイルを読み込む際の中間バッファを設定します。<br>
 * 第一引数には、中間バッファを設定したいCriFsStdioハンドルを指定します。<br>
 * 第二引数には、中間バッファとして利用するメモリ領域の先頭アドレスを指定します。
 * NULLを指定した場合、中間バッファを使いません。<br>
 * 第三引数には、中間バッファのサイズを指定します。単位はbyteです。
 * 0を指定した場合、第二引数に有効なアドレスを指定していても、中間バッファを使いません。<br>
 * \attention
 * criFsStdio_OpenFile()で得られたファイルハンドルは、デフォルトでは中間バッファを持ちません。<br>
 * 中間バッファが必要な場合、本関数で設定する必要があります。<br>
 * 中間バッファを設定すると、最大で temp_buffer_size 分のデータをファイルから
 * 中間バッファへ読み込むようになります。<br>
 * 中間バッファ内にデータがある限り、
 * criFsStdio_ReadFile()によるファイル読込はメモリコピーになるため、
 * 連続して細かいファイル読込を行う場合
 * 物理的なファイルアクセスの発生頻度を抑えることができます。<br>
 * ただし、criFsStdio_SeekFile()で中間バッファの有効範囲外にシークすると、
 * 中間バッファの内容が破棄されます。
 * \sa criFsStdio_TellFileOffset
 * \sa criFsStdio_SeekFile
 * \sa criFsStdio_OpenFile
 * \sa criFsStdio_ReadFile
 */
CriSint64 CRIAPI criFsStdio_SetInterstageBuffer(CriFsStdioHn stdhn, CriUint8 *temp_buffer, CriUint32 temp_buffer_size);

/*JP
 * \brief ANSI C に準じたAPIに基づくファイルからのデータ読込
 * \ingroup FSLIB_CRIFSSTDIO
 * \param[in] stdhn 読込元のCriFsStdioハンドル
 * \param[in] rsize 読込要求サイズ（byte）
 * \param[in] buf   読込先バッファ
 * \param[in] bsize 読込先バッファのサイズ（byte）
 * \return CriSint64 読込成功 読み込めたサイズ（byte）<br>
 *                   読込失敗 -1
 * \par 説明：
 * ファイルからデータを指定サイズ（byte）分読み込みます。<br>
 * 第一引数には、データの読込元であるファイルのCriFsStdioハンドルを指定します。<br>
 * 第二引数には、読み込むサイズを指定します。<br>
 * 第三引数には、読み込んだデータの書き込み先バッファを指定します。<br>
 * 第四引数には、読み込んだデータの書き込み先バッファサイズを指定します。<br>
 * \attention
 * 戻り値は、常に<b>読込要求サイズ以下になる</b>ことに注意してください。<br>
 * 例えばファイル終端では、戻り値が読込要求サイズより小さくなることがありますが、
 * 読込に失敗しているわけではありません。読込に失敗した場合、-1を返します。
 */
CriSint64 CRIAPI criFsStdio_ReadFile(CriFsStdioHn stdhn, CriSint64 rsize, void *buf, CriSint64 bsize);


/*JP
 * \brief ANSI C に準じたAPIに基づくデータのファイル書込
 * \ingroup FSLIB_CRIFSSTDIO
 * \param[in] stdhn 書込先のCriFsStdioハンドル
 * \param[in] rsize 書込要求サイズ（byte）
 * \param[in] buf   書込元バッファ
 * \param[in] bsize 書込元バッファのサイズ（byte）
 * \return CriSint64 書込成功 書き込めたサイズ（byte）<br>
 *                   書込失敗 -1
 * \par 説明：
 * ファイルからデータを指定サイズ（byte）分読み込みます。<br>
 * 第一引数には、データの書込先であるファイルのCriFsStdioハンドルを指定します。<br>
 * 第二引数には、書き込むサイズを指定します。<br>
 * 第三引数には、書込元となるデータのバッファを指定します。<br>
 * 第四引数には、書込元となるデータのバッファサイズを指定します。<br>
 * \attention
 * 戻り値は、書込に成功したサイズ（byte）です。<br>
 * 書込に失敗した場合、-1を返します。
 * 本関数は、ファイル書き込みをサポートしているプラットフォームにおいてのみ使用することができます。<br>
 * ファイル書き込みをサポートしていないプラットフォームで本関数を呼び出すと、<br>
 * リンク時にシンボルが見つからず、ビルドエラーになります。
 */
CriSint64 CRIAPI criFsStdio_WriteFile(CriFsStdioHn stdhn, CriSint64 rsize, void *buf, CriSint64 bsize);

/*JP
 * \brief CriFsStdioハンドルのファイル読み込みプライオリティを変更
 * \ingroup FSLIB_CRIFSSTDIO
 * \param[in] stdhn プライオリティを変更したいCriFsStdioハンドル
 * \param[in] prio  変更したいプライオリティの値
 * \return CriError CRIERR_OK 成功<br>
 *                  その他   失敗
 * \par 説明：
 * criFsStdio_ReadFile()を使ったファイル読み込み優先度を、CriFsStdioハンドル毎に設定します。<br>
 */
CriError criFsStdio_SetReadPriority(CriFsStdioHn stdhn, CriFsLoaderPriority prio);


#ifdef __cplusplus
}
#endif

#endif	/* CRI_FILE_SYSTEM_H_INCLUDED */

/* --- end of file --- */
