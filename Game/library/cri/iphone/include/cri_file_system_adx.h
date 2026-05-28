/****************************************************************************
 *
 * CRI Middleware SDK
 *
 * Copyright (c) 2006-2009 CRI Middleware Co.,Ltd.
 *
 * Library  : CRI File System
 * Module  : ADX + CRI File System Interface / Header
 * File     : cri_file_system_adx.h
 *
 ****************************************************************************/
/*!
 *	\file	cri_file_system_adx.h
 */

/* 多重定義防止					*/
/* Prevention of redefinition	*/
#ifndef CRI_FILE_SYSTEM_ADX_H_INCLUDED
#define CRI_FILE_SYSTEM_ADX_H_INCLUDED

/***************************************************************************
 *      インクルードファイル
 *      Include files
 ***************************************************************************/
#include <cri_xpt.h>
#include <cri_file_system.h>
#include <cri_adxf.h>
#include <cri_adxt.h>
#include <cri_adxcs.h>
#include <cri_aixp.h>

#if defined(XPT_TGT_PSP)
	#include <cri_adxatr.h>
	#include <crimwsfd_psp.h>
#else
	#include <crimwsfd.h>
#endif

/***************************************************************************
 *      定数マクロ
 *      Macro Constants
 ***************************************************************************/
/* バージョン情報 */
/* Version Number */
#define CRI_FS_ADX_VERSION		(0x01190200)
#define CRI_FS_ADX_VER_NUM		"1.19.02"
#define CRI_FS_ADX_VER_NAME		"CRI File System ADX"

/***************************************************************************
 *      処理マクロ
 *      Macro Functions
 ***************************************************************************/

/***************************************************************************
 *      データ型宣言
 *      Data Type Declarations
 ***************************************************************************/
/*--------------------------------------------------------------------------
 * ファイルシステムのセットアップパラメータ構造体
 * Parameter structure for file system setup function
 *-------------------------------------------------------------------------*/
/*JP
 * \brief ファイルシステムのセットアップパラメータ構造体
 * \ingroup FSLIB_CRIFS_ADX
 * \par 用途：
 * ADXF_SetupCriFileSystem 関数の引数として与える構造体です。<br>
 * ルートディレクトリにはボリューム名を含めることはできません。<br>
 * \par 例：
 * \code
 * AdxcpkSprmFs prm;
 * prm.rtdir = "/sample/crifilesystem/"
 * ADXF_SetupCriFileSystem(&prm);
 * \endcode
 */
typedef struct {
	CriChar8	*rtdir;			/*JP< ルートディレクトリ */
} AdxcpkSprmFs;


#if defined(XPT_TGT_PSP) || defined(XPT_TGT_PC)
/*--------------------------------------------------------------------------
 * ベリファイチェック関数の返値
 * Return value for verify check function
 *-------------------------------------------------------------------------*/
typedef enum {
	ADX_CPKVERIFY_SUCCESS = 0,	/*JP< ベリファイチェック成功  */
	ADX_CPKVERIFY_FAILED,		/*JP< ベリファイチェック失敗 */
	ADX_CPKVERIFY_UNAUTHORIZED,	/*JP< ベリファイチェックできない */
	/* enum be 4bytes */
	ADX_CPKVERIFY_ENUM_BE_SINT32 = 0x7FFFFFFF
} AdxCpkVerifyError;
#endif

/***************************************************************************
 *      変数宣言
 *      Prototype Variables
 ***************************************************************************/

/***************************************************************************
 *      関数宣言
 *      Prototype Functions
 ***************************************************************************/
#ifdef  __cplusplus
extern "C" {
#endif

/*==========================================================================
 *      CRI File System ADX API
 *=========================================================================*/
/*JP
 * \brief	CRI File Systemのセットアップ
 * \ingroup FSLIB_CRIFS_ADX
 * \param[in] sprmd セットアップパラメータ構造体
 * \par 説明：
 * CRI File SystemをADXライブラリのファイルアクセスレイヤとしてセットアップし、
 * sprmd->rtdir をルートディレクトリに設定します。<br>
 * 引数がNULLの場合は、ルートディレクトリを設定しません。<br>
 * 設定されたルートディレクトリは、相対パスで指定されたファイル名でアクセスする場合に、
 * ファイル名に付加されます。<br>
 * 本関数を実行することで、ADXライブラリは内部的にCRI File Systemを経由して
 * ファイルにアクセスするよう、動作を変更します。<br>
 * その結果、ADXライブラリのAPIでCPKファイル内に配置されたデータの読み出しや、
 * ADXデータの再生が可能になります。<br>
 * （バインダの機能をADX APIで利用するには、別途 ::ADXF_AddBinderVol 関数
 * でボリューム名を登録する必要があります。）
 * \attention
 * 本関数を実行する前に、ADXライブラリのファイルシステムをセットアップしておく必要があります。
 * \par 例：
 * \code
 * // ADXライブラリのファイルシステムをセットアップ
 * // 注意）この関数はプラットフォームごとに異なります。
 * ADXPC_SetupFileSystem(…);
 * 
 * // ADXライブラリのファイルアクセスレイヤをCRI File Systemに変更
 * ADXF_SetupCriFileSystem(…);
 * \endcode
 * \sa ADXF_ShutdownCriFileSystem, ADXF_AddBinderVol
 */
void CRIAPI ADXF_SetupCriFileSystem(AdxcpkSprmFs *sprmd);

/*JP
 * \brief	CRI File Systemのシャットダウン
 * \ingroup FSLIB_CRIFS_ADX
 * \par 説明：
 * ADXライブラリによる、CRI File Systemの利用を停止します。<br>
 * 本関数を実行すると、ADXライブラリはADXライブラリ標準のファイルアクセスレイヤを
 * 使用してファイルにアクセスするよう、動作を戻します。<br>
 * （ ::ADXF_SetupCriFileSystem 関数の効果を打ち消します。）<br>
 * \sa ADXF_SetupCriFileSystem
 */
void CRIAPI ADXF_ShutdownCriFileSystem(void);

/*JP
 * \brief バインダのボリューム登録
 * \ingroup FSLIB_CRIFS_ADX
 * \param[in]	volname	ボリュームに付ける名前
 * \param[in]	binder	ボリュームとして登録するCriFsBinderHn
 * \par 説明：
 * バインダハンドルをライブラリに登録し、バインダの機能を利用できるようにします。<br>
 * バインダハンドルは、本関数で指定したボリューム名に関連付けられます。<br>
 * ADXライブラリのAPIに指定するファイルのパスに、本関数で登録したバインダのボリューム名
 * を付加することで、CPKファイル内のコンテンツへのアクセス可能となります。<br>
 * 異なるボリューム名で、複数のバインダハンドルを登録することも可能です。<br>
 * \attention
 * バインダハンドルを破棄する際には、必ず事前に ::ADXF_DelBinderVol 関数で
 * ボリューム登録を解除してください。
 * \par 例：
 * \code
 * ADXF_AddBinderVol("VOL1", binderhn);
 * ADXF_SetDefBinderVol("VOL1");
 * ADXT_StartFname(adxt, "a.adx");  // "VOL1:a.adx"を指定したのと同義
 * \endcode
 * \sa ADXF_DelBinderVol
 */
void CRIAPI ADXF_AddBinderVol(const CriChar8 *volname, CriFsBinderHn bndrhn);

/*JP
 * \brief	登録済のボリューム名の削除
 * \ingroup FSLIB_CRIFS_ADX
 * \param[in]	volname	ボリューム名
 * \par 説明：
 * 指定したボリューム名を削除します。<br>
 * \attention
 * ボリューム名とバインダの関連付けは削除されますが、バインダは破棄されません。<br>
 * 不要になったバインダは、別途 ::criFsBinder_Destroy 関数で破棄する必要があります。<br>
 * \sa ADXF_AddBinderVol
 */
void CRIAPI ADXF_DelBinderVol(const CriChar8 *volname);

/*JP
 * \brief フルパス名に含まれるボリューム名の既登録チェック
 * \ingroup FSLIB_CRIFS_ADX
 * \param[in]	volname	ボリューム名
 * \return	指定されたボリューム名が既に登録されているかをCriBoolで返します。<br>
 * CriTRUE	：指定のボリューム名は登録されている。<br>
 * CriFALSE	：指定のボリューム名は登録されていない。<br>
 * \par	説明：
 * 指定されたフルパスに含まれるボリューム名が、既に登録されているかどうか確認します。<br>
 */
CriBool CRIAPI ADXF_IsBinderVol(const CriChar8 *fullpath);

/*JP
 * \brief デフォルトボリュームの設定
 * \ingroup FSLIB_CRIFS_ADX
 * \param[in]	volname	ボリューム名
 * \par 説明：
 * デフォルトボリューム名を設定します。<br>
 * 複数のボリュームが存在する場合に、この関数によりデフォルトのボリュームを
 * 切り換えることが可能です。
 * \par 備考：
 * デフォルトに設定されたボリュームが削除されても、デフォルトボリュームは変りません。<br>
 * デフォルトボリュームとしたボリュームを削除した場合は、適切なボリュームを設定し直してください。
 */
void CRIAPI ADXF_SetDefBinderVol(const CriChar8 *volname);

#if defined(XPT_TGT_PSP) || defined(XPT_TGT_PC)
/*JP
 * \brief 読込データのベリファイチェック(ファイル名指定)
 * \ingroup FSLIB_CRIFS_ADX
 * \param[in]	fullpath 読み込んだデータのボリューム名付ファイル名
 * \param[in]	dataptr 読み込んだデータの先頭アドレス
 * \param[in]	datasize 読み込んだデータのサイズ（バイト）
 * \par 説明：
 * ADXブリッジライブラリ経由で読み込んだCPKコンテンツファイルのデータベリファイチェックを行い、
 * 結果を関数値として返します。<br>
 * ファイル名は、ボリューム名付のファイル名を指定します。ボリューム名が無い場合は、
 * デフォルトボリュームが選択されます。
 * 本関数を使用するには、CRC情報付きのCPKが必要です。<br>
 * 本関数は完了復帰関数です。<br>
 * <br>
 * 関数の返値は以下のようになります；
 *	ADX_CPKVERIFY_OK：ベリファイチェックを行い、成功しました。
 *	ADX_CPKVERIFY_FAILED：ベリファイチェックを行い、失敗しました。
 *	ADX_CPKVERIFY_UNAUTHORIZED：ベリファイチェックを行えませんでした。CRC情報のないCPKである可能性があります。
 */
AdxCpkVerifyError CRIAPI ADXF_VerifyData(const CriChar8 *fullpath, void *dataptr, CriSint32 datasize);

/*JP
 * \brief 読込データのベリファイチェック(ID指定)
 * \ingroup FSLIB_CRIFS_ADX
 * \param[in]	volume ボリューム名
 * \param[in]	id 読み込んだデータのID
 * \param[in]	dataptr 読み込んだデータの先頭アドレス
 * \param[in]	datasize 読み込んだデータのサイズ（バイト）
 * \par 説明：
 * ADXブリッジライブラリ経由で読み込んだCPKコンテンツファイルのデータベリファイチェックを行い、
 * 結果を関数値として返します。<br>
 * ボリューム名が無い場合はデフォルトボリュームが選択されます。
 * 本関数を使用するには、CRC情報付きのCPKが必要です。<br>
 * 本関数は完了復帰関数です。<br>
 * <br>
 * 関数の返値は以下のようになります；
 *	ADX_CPKVERIFY_OK：ベリファイを行い、チェックに成功しました。
 *	ADX_CPKVERIFY_FAILED：ベリファイを行い、チェックに失敗しました。
 *	ADX_CPKVERIFY_UNAUTHORIZED：ベリファイは行えませんでした。CRC情報のないCPKである可能性があります。
 */
AdxCpkVerifyError CRIAPI ADXF_VerifyDataById(const CriChar8 *volume, CriUint16 id, void *dataptr, CriSint32 datasize);
#endif


/*==========================================================================
 *      ADX/AHX
 *=========================================================================*/
/*JP
 * \brief ID名指定によるCPKコンテンツファイル(ADXファイル)再生の開始
 * \ingroup FSLIB_CRIFS_ADX
 * \param[in]	adxt	ADXTハンドル
 * \param[in]	volname	ボリューム名
 * \param[in]	id  ファイルID	
 * \par 説明：
 * id で指定されたCPKコンテンツファイル(ADXファイル)の再生を開始します。<br>
 * volname が NULL の場合、デフォルトボリューム指定となります。
 * ID情報付CPKファイルがバインドされている必要があります。<br>
 */
void CRIAPI ADXT_StartCpkId(ADXT adxt, const CriChar8 *volname, CriUint16 id);

/*JP
 * \brief ID名指定によるCPKコンテンツファイル(ADXファイル)ループ再生の開始
 * \ingroup FSLIB_CRIFS_ADX
 * \param[in]	adxt	ADXTハンドル
 * \param[in]	volname	ボリューム名
 * \param[in]	id  ファイルID	
 * \par 説明：
 * id で指定されたCPKコンテンツファイル(ADXファイル)のループ再生を開始します。<br>
 * volname が NULL の場合、デフォルトボリューム指定となります。
 * ID情報付CPKファイルがバインドされている必要があります。<br>
 */
void CRIAPI ADXT_StartCpkIdLp(ADXT adxt, const CriChar8 *volname, CriUint16 id);

/*==========================================================================
 *      ADXCS
 *=========================================================================*/
/*JP
 * \brief ID名指定によるCPKコンテンツファイル(ADXファイル)CS再生の開始
 * \ingroup FSLIB_CRIFS_ADX
 * \param[in]	adxcs	ADXTCSハンドル
 * \param[in]	volname	ボリューム名
 * \param[in]	id  ファイルID	
 * \par 説明：
 * id で指定されたCPKコンテンツファイルの再生を開始します。<br>
 * volname が NULL の場合、デフォルトボリューム指定となります。
 * ID情報付CPKファイルがバインドされている必要があります。<br>
 */
void CRIAPI ADXCS_StartCpkId(ADXCS adxcs, const CriChar8 *volname, CriUint16 id);

/*JP
 * \brief ID名指定によるCPKコンテンツファイル(ADXファイル)ループCS再生の開始
 * \ingroup FSLIB_CRIFS_ADX
 * \param[in]	adxcs	ADXTCSハンドル
 * \param[in]	volname	ボリューム名
 * \param[in]	id  ファイルID	
 * \par 説明：
 * id で指定されたCPKコンテンツファイルのループ再生を開始します。<br>
 * volname が NULL の場合、デフォルトボリューム指定となります。
 * ID情報付CPKファイルがバインドされている必要があります。<br>
 */
void CRIAPI ADXCS_StartCpkIdLp(ADXCS adxcs, const CriChar8 *volname, CriUint16 id);

/*==========================================================================
 *      AIX
 *=========================================================================*/
/*JP
 * \brief ID名指定によるCPKコンテンツファイル(AIXファイル)再生の開始
 * \ingroup FSLIB_CRIFS_ADX
 * \param[in]	aixp	AIXPハンドル
 * \param[in]	volname	ボリューム名
 * \param[in]	id  ファイルID	
 * \param[in]	atr  ディレクトリ情報	
 * \par 説明：
 * id で指定されたCPKコンテンツファイルの再生を開始します。<br>
 * volname が NULL の場合、デフォルトボリューム指定となります。
 * ID情報付CPKファイルがバインドされている必要があります。<br>
 */
void CRIAPI AIXP_StartCpkId(AIXP aixp, const CriChar8 *volname, CriUint16 id, void *atr);

/*==========================================================================
 *      Sofdec
 *=========================================================================*/
/*JP
 * \brief ID名指定によるCPKコンテンツファイル(Sofdecファイル)再生の開始
 * \ingroup FSLIB_CRIFS_ADX
 * \param[in]	ply		MWPLYハンドル
 * \param[in]	volname	ボリューム名
 * \param[in]	id		ファイルID	
 * \par 説明：
 * id で指定されたCPKコンテンツファイルの再生を開始します。<br>
 * volname が NULL の場合、デフォルトボリューム指定となります。
 * ID情報付CPKファイルがバインドされている必要があります。<br>
 */
void CRIAPI mwPlyStartCpkId(MWPLY ply, const CriChar8 *volname, CriUint16 id);

/*JP
 * \brief ID名指定によるCPKコンテンツファイル(Sofdecファイル)ループ再生の開始
 * \ingroup FSLIB_CRIFS_ADX
 * \param[in]	ply		MWPLYハンドル
 * \param[in]	volname	ボリューム名
 * \param[in]	id		ファイルID	
 * \par 説明：
 * id で指定されたCPKコンテンツファイルのループ再生を開始します。<br>
 * volname が NULL の場合、デフォルトボリューム指定となります。
 * ID情報付CPKファイルがバインドされている必要があります。<br>
 */
void CRIAPI mwPlyStartCpkIdLp(MWPLY ply, const CriChar8 *volname, CriUint16 id);

#if defined(XPT_TGT_PSP)
/*==========================================================================
 *      ADXATR
 *=========================================================================*/
/*JP
 * \brief ID名指定によるCPKコンテンツファイル(ATRAC3ファイル)再生の開始
 * \ingroup FSLIB_CRIFS_ADX
 * \param[in]	atr		ADXATRハンドル
 * \param[in]	volname	ボリューム名
 * \param[in]	id		ファイルID	
 * \par 説明：
 * id で指定されたCPKコンテンツファイルの再生を開始します。<br>
 * volname が NULL の場合、デフォルトボリューム指定となります。
 * ID情報付CPKファイルがバインドされている必要があります。<br>
 */
void CRIAPI ADXATR_StartCpkId(ADXATR atr, const CriChar8 *volname, CriUint16 id);
#endif

#if defined(XPT_TGT_PSP)
/*==========================================================================
 *      UMD
 *=========================================================================*/
/*JP
 * \brief UMDドライブのアボート
 * \ingroup FSLIB_CRIFS_ADX
 * \par 説明：
 * UMDドライブのアボート処理を行います。<BR>
 * ADXPSP_AbortUmdDrive() の代わりに呼び出してください。
 */
void CRIAPI criFs_AbortAdxUmdDrive_PSP(void);
#endif

#ifdef	__cplusplus
}
#endif

#endif



/* end of file */
