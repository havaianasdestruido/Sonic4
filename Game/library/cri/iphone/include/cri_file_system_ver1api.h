/****************************************************************************
 *
 * CRI Middleware SDK
 *
 * Copyright (c) 2006-2008 CRI Middleware Co.,Ltd.
 *
 * Library  : CRI File System
 * Module   : Library User's Header
 * File     : cri_file_system_ver1api.h
 * Date     : 2008-09-17
 *
 ****************************************************************************/
/*!
 *	\file		cri_file_system_ver1api.h
 */
 
#ifdef __cplusplus

/* 多重定義防止					*/
/* Prevention of redefinition	*/
#ifndef	CRI_FILE_SYSTEM_VER1PAI_H_INCLUDED
#define	CRI_FILE_SYSTEM_VER1PAI_H_INCLUDED

/***************************************************************************
 *      インクルードファイル
 *      Include files
 ***************************************************************************/
#include "cri_xpt.h"
#include "cri_allocator.h"
#include "cri_error.h"

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
namespace CriFs {
	/*JP
	 * \brief ファイルオープンモード
	 */
	/*EN
	 * \brief File Opening Mode
	 */
	enum FileMode {
		FILE_MODE_APPEND,			/*JP< 既存ファイルに追記								*/	/*EN< Appends to an existing file						*/
		FILE_MODE_CREATE,			/*JP< ファイルの新規作成（既存のファイルは上書き）		*/	/*EN< Creates a new file always							*/
		FILE_MODE_CREATE_NEW,		/*JP< ファイルの新規作成（上書き不可）					*/	/*EN< Creates a new file (Can not overwrite)			*/
		FILE_MODE_OPEN,				/*JP< 既存ファイルのオープン							*/	/*EN< Opens an existing file							*/
		FILE_MODE_OPEN_OR_CREATE,	/*JP< ファイルのオープン（存在しない場合は新規作成）	*/	/*EN< Opens a file if available (Or creates new file)	*/
		FILE_MODE_TRUNCATE,			/*JP< 既存ファイルを0Byteに切り詰めてオープン			*/	/*EN< Opens a file and truncates it						*/
		/* enum be 4bytes */
		FILE_MODE_ENUM_BE_SINT32 = 0x7FFFFFFF
	};
	
	/*JP
	 * \brief ファイルアクセス種別
	 */
	/*EN
	 * \brief Kind of File Access
	 */
	enum FileAccess {
		FILE_ACCESS_READ,			/*JP< 読み込みのみ		*/	/*EN< Read Only			*/
		FILE_ACCESS_WRITE,			/*JP< 書き込みのみ		*/	/*EN< Write Only		*/
		FILE_ACCESS_READ_WRITE,		/*JP< 読み書き			*/	/*EN< Read and Write	*/
		/* enum be 4bytes */
		FILE_ACCESS_ENUM_BE_SINT32 = 0x7FFFFFFF
	};
	
	/*JP
	 * \brief シーク開始位置
	 */
	/*EN
	 * \brief Seek Origin
	 */
	enum SeekOrigin {
		SEEK_FROM_BEGIN,		/*JP< 先頭位置から		*/	/*EN< from Top Position			*/
		SEEK_FROM_CURRENT,		/*JP< 現在位置から		*/	/*EN< from Current Position		*/
		SEEK_FROM_END,			/*JP< 終端位置から		*/	/*EN< from Terminal position	*/
		/* enum be 4bytes */
		SEEK_FROM_ENUM_BE_SINT32 = 0x7FFFFFFF
	};
	
	/*JP
	 * \brief 非同期処理ステータス
	 */
	/*EN
	 * \brief Async Status
	 */
	enum AsyncStatus {
		ASYNC_STATUS_STOP,		/*JP< 停止				*/	/*EN< Stop			*/
		ASYNC_STATUS_BUSY,		/*JP< 処理中			*/	/*EN< Busy			*/
		ASYNC_STATUS_COMPLETE,	/*JP< 完了				*/	/*EN< Complete		*/
		ASYNC_STATUS_CANCELED,	/*JP< キャンセルされた	*/	/*EN< Canceled		*/
		ASYNC_STATUS_ERROR,		/*JP< エラー			*/	/*EN< Error			*/
		/* enum be 4bytes */
		ASYNC_STATUS_ENUM_BE_SINT32 = 0x7FFFFFFF
	};
	
	/*JP
	 * \brief プライオリティ
	 */
	/*EN
	 * \brief Priority
	 */
	enum Priority {
		PRIORITY_HIGHEST 		= 10,	/*JP< 最高		*/	/*EN< Highest		*/
		PRIORITY_ABOVE_NORMAL 	= 9,	/*JP< 高		*/	/*EN< Above normal	*/
		PRIORITY_NORMAL 		= 8,	/*JP< 普通		*/	/*EN< Normal		*/
		PRIORITY_BELOW_NORMAL 	= 7,	/*JP< 低		*/	/*EN< Below normal	*/
		PRIORITY_LOWEST 		= 6,	/*JP< 最低		*/	/*EN< Lowest		*/
		/* enum be 4bytes */
		PRIORITY_ENUM_BE_SINT32 = 0x7FFFFFFF
	};
}

/***************************************************************************
 *      変数宣言
 *      Prototype Variables
 ***************************************************************************/
namespace CriFs {
	/* エラー省略用 */
	extern CriError ErrorContainer;
}

/***************************************************************************
 *      クラス宣言
 *      Prototype Classes
 ***************************************************************************/

/*-------------------------------------------------------------------------*
 *	CriFsManager
 *-------------------------------------------------------------------------*/
/*JP
 * \brief ファイルシステムマネージャ
 * \ingroup FSLIB_BASIC
 * \par 説明:
 * ファイルシステムマネージャとは、論理的なデバイスのようなものを表すモジュールです。<br>
 * 標準的なファイルを扱う標準ファイルシステムマネージャやパッキングされたファイルをアクセスするCPKファイルシステムマネージャなどがあります。
 */
/*EN
 * \brief File System Manager
 * \ingroup FSLIB_BASIC
 * \par
 * The File system manager is a module expressing a thing such as a logical device.<br>
 * It has Standard File Manager and CPK File Manager.
 */
class CriFsManager : public CriAllocator
{
public:
	/*JP
	 * \brief ファイルシステムマネージャオブジェクトの破棄
	 * \param err エラーコード<br>
	 * \par 説明:
	 * ファイルシステムマネージャオブジェクトを破棄します。
	 */
	/*EN
	 * \brief Destroy a File System Manager
	 * \param err A CriError information
	 * \par
	 * This function destroys the File System Manager object.
	 */
	virtual void Destroy(CriError &err=CriFs::ErrorContainer) = 0;

	/*JP
	 * \brief ファイルサイズの取得
	 * \param path ファイルパス
	 * \param err エラーコード<br>
	 * \return　ファイルサイズ
	 * \par 説明:
	 * 指定されたファイルのサイズを返します。
	 */
	/*EN
	 * \brief Get File size
	 * \param path filepath
	 * \param err A CriError information
	 * \return File size
	 * \par
	 * This function returns size of an appointed file.
	 */
	virtual CriSint64 GetFileSize(const CriChar8 *path, CriError &err=CriFs::ErrorContainer) = 0;

	
	virtual void* GetBinder(CriError &err=CriFs::ErrorContainer) = 0;

protected:
	CriFsManager() {};
	virtual ~CriFsManager() {};
};

/*-------------------------------------------------------------------------*
 *	CriFsManagerStandard
 *-------------------------------------------------------------------------*/
/*JP
 * \brief 標準ファイルシステムマネージャ
 * \ingroup FSLIB_BASIC
 * \par 説明:
 * 標準ファイルシステムマネージャとは、標準的なファイルを操作するためのオブジェクトです。
 */
/*EN
 * \brief Standard File System Manager Object
 * \ingroup FSLIB_BASIC
 * \par
 * Standard File System Manager is an object to access a standard file.
 */
class CriFsManagerStandard : public CriFsManager
{
public:
	/*JP
	 * \brief 標準ファイルシステムマネージャオブジェクトの作成
	 * \param heap CRI Heap オブジェクト
	 * \param err エラーコード<br>
	 * \return	標準ファイルシステムマネージャオブジェクト
	 * \par 説明:
	 * 標準ファイルシステムマネージャオブジェクトを作成します。<br>作成したオブジェクトを破棄するには、 CriFsManagerStandard::Destroy関数を使用します。
	 * \sa CriFsManagerStandard::Destroy
	 */
	/*EN
	 * \brief Create a Standard File System Manager
	 * \param heap A CRI Heap object
	 * \param err A CriError information
	 * \return	A Standard File System Manager object
	 * \par
	 * This function creates a Standard File System Manager object.<br>When you destroy this object, you must call CriFsManagerStandard::Destroy function. 
	 * \sa CriFsManagerStandard::Destroy
	 */
	static CriFsManagerStandard* CRIAPI Create(CriHeap heap, CriError &err = CriFs::ErrorContainer);

	/*JP
	 * \brief 標準ファイルシステムマネージャオブジェクトの作成
	 * \param heap CRI Heap オブジェクト
	 * \param path ディレクトリパス
	 * \param err エラーコード<br>
	 * \return	標準ファイルシステムマネージャオブジェクト
	 * \par 説明:
	 * 標準ファイルシステムマネージャオブジェクトを作成します。<br>作成したオブジェクトを破棄するには、 CriFsManagerStandard::Destroy関数を使用します。<br>
	 * 指定されたディレクトリをカレントディレクトリに設定します。
	 * \sa CriFsManagerStandard::Destroy
	 */
	/*EN
	 * \brief Create a Standard File System Manager
	 * \param heap A CRI Heap object
	 * \param path A Directory path
	 * \param err A CriError information
	 * \return	A Standard File System Manager object
	 * \par
	 * This function creates a Standard File System Manager object.<br>When you destroy this object, you must call CriFsManagerStandard::Destroy function. <br>
	 * This function sets the current directory.
	 * \sa CriFsManagerStandard::Destroy
	 */
	static CriFsManagerStandard* CRIAPI Create(CriHeap heap, const CriChar8 *path, CriError &err = CriFs::ErrorContainer);
	
	/*JP
	 * \brief 標準ファイルシステムマネージャオブジェクトの破棄
	 * \param	err エラーコード
	 * \par 説明:
	 * 標準ファイルシステムマネージャオブジェクトを破棄します。
	 */
	/*EN
	 * \brief Destroy a Standard File System Manager object
	 * \param err A CriError information
	 * \par
	 * This function deletes a Standard File System Manager object.
	 */
	virtual void Destroy(CriError &err = CriFs::ErrorContainer) = 0;
	
	/*JP
	 * \brief カレントディレクトリパスの設定
	 * \param path ディレクトリパス
	 * \param err エラーコード<br>
	 * \par 説明:
	 * 指定されたディレクトリをカレントディレクトリに設定します。<br>
	 * \sa CriFsManagerStandard::GetDirectory
	 */
	/*EN
	 * \brief Set Current Directory
	 * \param path A Directory path
	 * \param err A CriError information
	 * \par
	 * This function sets the current directory.<br>
	 * \sa CriFsManagerStandard::GetDirectory
	 */
	virtual void SetDirectory(const CriChar8 *path, CriError &err = CriFs::ErrorContainer) = 0;
	
	/*JP
	 * \brief カレントディレクトリの取得
	 * \param err エラーコード<br>
	 * \return	カレントディレクトリのパス
	 * \par 説明:
	 * カレントディレクトリのパスを返します。<br>
	 * \sa CriFsManagerStandard::SetDirectory
	 */
	/*EN
	 * \brief Get Current Directory
	 * \param err A CriError information
	 * \return	Path of the current directory
	 * \par
	 * This function returns path of the current directory.<br>
	 * \sa CriFsManagerStandard::SetDirectory
	 */
	virtual const CriChar8 *GetDirectory(CriError &err = CriFs::ErrorContainer) = 0;
	
	/*JP
	 * \brief ファイルサイズの取得
	 * \param path ファイルパス
	 * \param err エラーコード<br>
	 * \return　ファイルサイズ
	 * \par 説明:
	 * 指定されたファイルのサイズを返します。<br>
	 * 本関数は完了復帰関数です。復帰までに100ミリ秒程度の時間が掛かる場合があります。
	 */
	/*EN
	 * \brief Get File size
	 * \param path A File path
	 * \param err A CriError information
	 * \return File size
	 * \par
	 * This function returns size of an appointed file.<br>
	 * This function is a completion return function. It may have waiting time of around 100 miliseconds.
	 */
	virtual CriSint64 GetFileSize(const CriChar8 *path, CriError &err=CriFs::ErrorContainer) = 0;

protected:
	CriFsManagerStandard() {};
	virtual ~CriFsManagerStandard() {};
};


/*-------------------------------------------------------------------------*
 *	CriFsManagerCpk
 *-------------------------------------------------------------------------*/
class CriFsGroupLoader;
/*JP
 * \brief CPKファイルシステムマネージャ
 * \ingroup FSLIB_BASIC
 * \par 説明:
 * CPKファイルシステムマネージャ、パッキングされたCPKファイルを扱うためのオブジェクトです。
 */
/*EN
 * \brief CPK File System Manager
 * \ingroup FSLIB_BASIC
 * \par
 * CPK File System Manager is an object to access a CPK file.
 */
class CriFsManagerCpk : public CriFsManager
{
public:
	/*JP
	 * \brief マウントステータス
	 * \par 説明:
	 * CPKファイルをマウントする際に、 CriFsManagerCpk::GetMountStatus関数から返るステータスです。
	 */
	/*EN
	 * \brief Mount Status
	 * \par
	 * This is status returning from CriFsManagerCpk::GetMountStatus function.
	 */
	enum MountStatus {
		MOUNT_STATUS_STOP	= (0),	/*JP< 停止		*/	/*EN< Stop			*/
		MOUNT_STATUS_BUSY,			/*JP< 処理中	*/	/*EN< Busy			*/
		MOUNT_STATUS_COMPLETE,		/*JP< 完了		*/	/*EN< Complete		*/
		MOUNT_STATUS_ERROR,			/*JP< エラー	*/	/*EN< Error			*/
		/* enum be 4bytes */
		MOUNT_STATUS_ENUM_BE_SINT32 = 0x7FFFFFFF
	};

	/*JP
	 * \brief コンテンツファイル情報構造体
	 */
	/*EN
	 * \brief Structure of the content file information
	 */
	struct ContentsFileInfo {
		CriUint32		filesize;		/*JP< コンテンツファイルのファイルサイズ	*/	/*EN< File size	of content file			*/
		CriUint64		offset;			/*JP< コンテンツファイルのオフセット位置	*/	/*EN< Offset position of content file	*/
	};

	/*JP
	 * \brief コンテンツファイル詳細情報構造体
	 */
	/*EN
	 * \brief Structure of the content detail file information
	 */
	struct ContentsFileInfoDetail {
		CriChar8		*directory;     /*JP< コンテンツファイルのディレクトリ名    */	/*EN< Directory name of content file	*/
		CriChar8		*filename;		/*JP< コンテンツファイルのファイル名前      */	/*EN< Filename of content file			*/
		CriUint32		filesize;		/*JP< コンテンツファイルのファイルサイズ	*/	/*EN< File size	of content file			*/
		CriUint64		offset;			/*JP< コンテンツファイルのオフセット位置	*/	/*EN< Offset position of content file	*/
	};
		

	/*JP
	 * \if fsgroup
	 * \brief グループファイル情報構造体
	 * \endif
	 */
	/*EN
	 * \if fsgroup
	 * \brief Structure of the file information for group
	 * \endif
	 */
	struct GroupFileInfo {
		CriChar8		*directory;     /*JP< コンテンツファイルのディレクトリ名    */	/*EN< Directory name of content file	*/
		CriChar8		*filename;		/*JP< コンテンツファイルのファイル名前      */	/*EN< Filename of content file			*/
		CriUint32		filesize;		/*JP< コンテンツファイルのファイルサイズ	*/	/*EN< File size	of content file			*/
		void		*datapointer;	/*JP< コンテンツファイルのデータポインタ	*/	/*EN< Data pointer of content file		*/
		CriUint32		gfinfotag;		/*JP< コンテンツファイルの汎用タグ          */  /*EN< Tag for content file              */
	};
		
	/*JP
	 * \brief CPKファイルシステムマネージャオブジェクトの作成
	 * \param heap CRI Heap オブジェクト
	 * \param err エラーコード<br>
	 * \return	CPKファイルシステムマネージャオブジェクト
	 * \par 説明:
	 * CPKファイルシステムマネージャオブジェクトを作成します。<br>作成したオブジェクトを破棄するには、 CriFsManagerCpk::Destroy関数を使用します。
	 * \sa CriFsManagerCpk::Destroy
	 */
	/*EN
	 * \brief Create a CPK File System Manager object
	 * \param heap A CRI Heap object
	 * \param err A CriError information
	 * \return	A CPK File System Manager object
	 * \par
	 * This function creates a CPK File System Manager object.<br>When you destroy this object, you must call CriFsManagerCpk::Destroy function. 
	 * \sa CriFsManagerCpk::Destroy
	 */
	static CriFsManagerCpk* CRIAPI Create(CriHeap heap, CriError &err = CriFs::ErrorContainer);

	/*JP
	 * \brief CPKファイルシステムマネージャオブジェクトの作成
	 * \param heap CRI Heap オブジェクト
	 * \param path ディレクトリパス
	 * \param err エラーコード<br>
	 * \return	CPKファイルシステムマネージャオブジェクト
	 * \par 説明:
	 * CPKファイルシステムマネージャオブジェクトを作成します。<br>作成したオブジェクトを破棄するには、 CriFsManagerCpk::Destroy関数を使用します。<br>
	 * 指定されたディレクトリをカレントディレクトリに設定します。<br>
	 * \sa CriFsManagerCpk::Destroy
	 */
	/*EN
	 * \brief Create a CPK File System Manager object
	 * \param heap A CRI Heap object
	 * \param path A Directory path
	 * \param err A CriError information
	 * \return	A CPK File System Manager object
	 * \par
	 * This function creates a CPK File System Manager object.<br>When you destroy this object, you must call CriFsManagerCpk::Destroy function. <br>
	 * This function sets the current directory.
	 * \sa CriFsManagerCpk::Destroy
	 */
	static CriFsManagerCpk* CRIAPI Create(CriHeap heap, const CriChar8 *path, CriError &err = CriFs::ErrorContainer);

	/*JP
	 * \brief CPKファイルシステムマネージャオブジェクトの破棄
	 * \param err エラーコード<br>
	 * \par 説明:
	 * CPKファイルシステムマネージャオブジェクトを破棄します。
	 * \sa CriFsManagerCpk::Create
	 */
	/*EN
	 * \brief Destroy a CPK File System Manager
	 * \param err A CriError information
	 * \par
	 * This function destroys a CPK File System Manager object.
	 * \sa CriFsManagerCpk::Create
	 */
	virtual void Destroy(CriError &err=CriFs::ErrorContainer) = 0;
	
	/*JP
	 * \brief カレントディレクトリパスの設定
	 * \param path ディレクトリパス
	 * \param err エラーコード<br>
	 * \par 説明:
	 * 指定されたディレクトリをカレントディレクトリに設定します。<br>
	 * \sa CriFsManagerCpk::GetDirectory
	 */
	/*EN
	 * \brief Set Current Directory
	 * \param path A Directory path
	 * \param err A CriError information
	 * \par
	 * This function sets the current directory.<br>
	 * \sa CriFsManagerCpk::GetDirectory
	 */
	virtual void SetDirectory(const CriChar8 *path, CriError &err = CriFs::ErrorContainer) = 0;
	
	/*JP
	 * \brief カレントディレクトリの取得
	 * \param err エラーコード<br>
	 * \return	カレントディレクトリのパス
	 * \par 説明:
	 * カレントディレクトリのパスを返します。<br>
	 * \sa CriFsManagerCpk::SetDirectory
	 */
	/*EN
	 * \brief Get Current Directory
	 * \param err A CriError information
	 * \return	Path of the current directory
	 * \par
	 * This function returns path of the current directory.<br>
	 * \sa CriFsManagerCpk::SetDirectory
	 */
	virtual const CriChar8 *GetDirectory(CriError &err = CriFs::ErrorContainer) = 0;
	
	/*JP
	 * \brief ファイルサイズの取得
	 * \param path ファイルパス
	 * \param err エラーコード<br>
	 * \return　ファイルサイズ
	 * \par 説明:
	 * 指定されたファイルのサイズを返します。<br>
	 * 圧縮されている場合は、展開後のサイズを返します。<br>
	 * 本関数は完了復帰関数です。しかし、待ち時間はほとんどありません。
	 */
	/*EN
	 * \brief Get File size
	 * \param path filepath
	 * \param err A CriError information
	 * \return File size
	 * \par
	 * This function returns size of an appointed file.<br>
	 * When file is compressed, this function returns decompressed file size.
	 * This function is completion return. There is not waiting time.
	 */
	virtual CriSint64 GetFileSize(const CriChar8 *path, CriError &err=CriFs::ErrorContainer) = 0;

	/*JP
	 * \brief ファイルサイズの取得
	 * \param id ファイルID
	 * \param err エラーコード<br>
	 * \return　ファイルサイズ
	 * \par 説明:
	 * 指定されたファイルIDを元にサイズを返します。<br>
	 * 圧縮されている場合は、展開後のサイズを返します。<br>
	 * 本関数は完了復帰関数です。しかし、待ち時間はほとんどありません。
	 */
	/*EN
	 * \brief Get File size
	 * \param id file ID
	 * \param err A CriError information
	 * \return File size
	 * \par
	 * This function returns size of an appointed file ID.<br>
	 * When file is compressed, this function returns decompressed file size.
	 * This function is completion return. There is not waiting time.
	 */
	virtual CriSint64 GetFileSize(CriUint16 id, CriError &err=CriFs::ErrorContainer) = 0;

	/*JP
	 * \brief パックファイルサイズの取得
	 * \param path ファイルパス
	 * \param err エラーコード<br>
	 * \return　ファイルサイズ
	 * \par 説明:
	 * 指定されたファイルのパックされたファイルサイズを返します。<br>
	 * ファイルが圧縮されていれば、圧縮されたファイルを返します。
	 * 本関数は完了復帰関数です。しかし、待ち時間はほとんどありません。
	 */
	/*EN
	 * \brief Get Packed File size
	 * \param path filepath
	 * \param err A CriError information
	 * \return Packed file size
	 * \par
	 * This function returns size of an appointed file.<br>
	 * When file is compressed, this function returns compressed file size.
	 * This function is completion return. There is not waiting time.
	 */
	virtual CriSint64 GetPackedFileSize(const CriChar8 *path, CriError &err=CriFs::ErrorContainer) = 0;

	/*JP
	 * \brief パックファイルサイズの取得
	 * \param id ファイルID
	 * \param err エラーコード<br>
	 * \return　ファイルサイズ
	 * \par 説明:
	 * 指定されたファイルIDを元にパックされたファイルサイズを返します。<br>
	 * ファイルが圧縮されていれば、圧縮されたファイルを返します。
	 * 本関数は完了復帰関数です。しかし、待ち時間はほとんどありません。
	 */
	/*EN
	 * \brief Get Packed File size
	 * \param id file ID
	 * \param err A CriError information
	 * \return Packed file size
	 * \par
	 * This function returns size of an appointed file ID.<br>
	 * When file is compressed, this function returns compressed file size.
	 * This function is completion return. There is not waiting time.
	 */
	virtual CriSint64 GetPackedFileSize(CriUint16 id, CriError &err=CriFs::ErrorContainer) = 0;

	/*JP
	 * \brief CPKファイルのマウント
	 * \param	manager ファイルシステムマネージャ
	 * \param	path ファイルパス
	 * \param	offset ファイルオフセット
	 * \param	size ファイルサイズ
	 * \param	err エラーコード
	 * \par 説明:
	 * CPKファイルのマウントを行います。<br>本関数は完了復帰です。マウント完了の状態を調べるためには、 CriFsManagerCpk::MountStatus関数を使用してください。
	 * \sa CriFsManagerCpk::GetMountStatus
	 */
	/*EN
	 * \brief Mount the CPK File
	 * \param	manager A File System Manager
	 * \param	path A CPK File path
	 * \param	offset A File offset
	 * \param	size A Size of file
	 * \param 	err A CriError information
	 * \par
	 * This function mounts the CPK File.<br>
	 * This function is return immediately. You use CriFsManagerCpk::GetMountStatus function to examine mounting completion.<br>
	 * \sa CriFsManagerCpk::GetMountStatus
	 */
	virtual void MountCpkFile(CriFsManager *manager, const CriChar8 *path, CriSint64 offset, CriSint64 size, CriError &err = CriFs::ErrorContainer) = 0;

	/*JP
	 * \brief CPKファイルのマウント(ファイルオフセット、サイズの省略)
	 * \param	manager ファイルシステムマネージャ
	 * \param	path ファイルパス
	 * \param	err エラーコード
	 * \par 説明:
	 * CPKファイルのマウントを行います。<br>本関数は完了復帰です。マウント完了の状態を調べるためには、 CriFsManagerCpk::MountStatus関数を使用してください。
	 * \sa CriFsManagerCpk::GetMountStatus
	 */
	/*EN
	 * \brief Mount the CPK File (abbreviation File offset and size)
	 * \param	manager A File System Manager
	 * \param	path A CPK File path
	 * \param 	err A CriError information
	 * \par
	 * This function mounts the CPK File.<br>
	 * This function is return immediately. You use CriFsManagerCpk::GetMountStatus function to examine mounting completion.<br>
	 * \sa CriFsManagerCpk::GetMountStatus
	 */
	virtual void MountCpkFile(CriFsManager *manager, const CriChar8 *path, CriError &err = CriFs::ErrorContainer) = 0;

	/*JP
	 * \brief CPKファイルのマウント(ファイルシステムマネージャの省略)
	 * \param	path ファイルパス
	 * \param	offset ファイルオフセット
	 * \param	size ファイルサイズ
	 * \param	err エラーコード
	 * \par 説明:
	 * CPKファイルのマウントを行います。<br>本関数は完了復帰です。マウント完了の状態を調べるためには、 CriFsManagerCpk::MountStatus関数を使用してください。
	 * \sa CriFsManagerCpk::GetMountStatus
	 */
	/*EN
	 * \brief Mount the CPK File (abbreviation of File System Manager)
	 * \param	path A CPK File path
	 * \param	offset A File offset
	 * \param	size A Size of file
	 * \param 	err A CriError information
	 * \par
	 * This function mounts the CPK File.<br>
	 * This function is return immediately. You use CriFsManagerCpk::GetMountStatus function to examine mounting completion.<br>
	 * \sa CriFsManagerCpk::GetMountStatus
	 */
	virtual void MountCpkFile(const CriChar8 *path, CriSint64 offset, CriSint64 size, CriError &err = CriFs::ErrorContainer) = 0;

	/*JP
	 * \brief CPKファイルのマウント(ファイルオフセット、サイズ、ファイルシステムマネージャの省略)
	 * \param	path ファイルパス
	 * \param	err エラーコード
	 * \par 説明:
	 * CPKファイルのマウントを行います。<br>本関数は完了復帰です。マウント完了の状態を調べるためには、 CriFsManagerCpk::MountStatus関数を使用してください。
	 * \sa CriFsManagerCpk::GetMountStatus
	 */
	/*EN
	 * \brief Mount the CPK File (abbreviation of File System Manager and File offset, File size)
	 * \param	path A CPK File path
	 * \param 	err A CriError information
	 * \par
	 * This function mounts the CPK File.<br>
	 * This function is return immediately. You use CriFsManagerCpk::GetMountStatus function to examine mounting completion.<br>
	 * \sa CriFsManagerCpk::GetMountStatus
	 */
	virtual void MountCpkFile(const CriChar8 *path, CriError &err = CriFs::ErrorContainer) = 0;

	/*JP
	 * \brief マウントステータスの取得
	 * \param err エラーコード<br>
	 * \return	MountStatus マウントステータス
	 * \par 説明:
	 * CPKファイルのマウントステータスを取得します。
	 * \sa CriFsManagerCpk::MountCpkFile
	 */
	/*EN
	 * \brief Get Mount status
	 * \param err A CriError information
	 * \return	MountStatus Mounting status
	 * \par
	 * This function returns Mounting status.
	 * \sa CriFsManagerCpk::MountCpkFile
	 */
	virtual MountStatus GetMountStatus(CriError &err = CriFs::ErrorContainer) = 0;

	/* コンテンツファイル情報の取得 */	
	/*JP
	 * \brief コンテンツファイル情報の取得
	 * \param cfinf ContentsFileInfo* コンテンツファイル情報構造体
	 * \param path コンテンツファイル名
	 * \param err エラーコード
	 * \return	CriBool 取得に成功したか否か
	 * \par 説明:
	 * コンテンツファイル名を指定して情報を取得します。
	 */
	/*EN
	 * \brief Get Information of a contents file
	 * \param cfinf ContentsFileInfo* Structure of content file information
	 * \param path A Content file name
	 * \param err A CriError information
	 * \return	CriBool Result.
	 * \par
	 * This function gets a content file information.
	 */
	virtual CriBool GetContentsFileInfo(ContentsFileInfo *cfinf, const CriChar8 *path, CriError &err = CriFs::ErrorContainer) = 0;

	/*JP
	 * \brief コンテンツファイル情報の取得
	 * \param cfinf ContentsFileInfo* コンテンツファイル情報構造体
	 * \param id ID
	 * \param err エラーコード
	 * \return	CriBool 取得に成功したか否か
	 * \par 説明:
	 * コンテンツファイル名を指定して情報を取得します。
	 */
	/*EN
	 * \brief Get the Information of a contents file
	 * \param cfinf ContentsFileInfo* Structure of content file information
	 * \param id ID
	 * \param err A CriError information
	 * \return	CriBool Result.
	 * \par
	 * This function gets a content file information.
	 */
	virtual CriBool GetContentsFileInfo(ContentsFileInfo *cfinf, CriUint16 id, CriError &err = CriFs::ErrorContainer) = 0;

	/*JP
	 * \brief コンテンツファイル詳細情報の取得
	 * \param cfdinf ContentsFileInfoDetail* コンテンツファイル情報構造体
	 * \param index インデックス番号
	 * \param err エラーコード
	 * \return	CriBool 取得に成功したか否か
	 * \par 説明:
	 * インデックス番号を指定してコンテンツ情報を取得します。
	 */
	/*EN
	 * \brief Get the Detail information of a contents file
	 * \param cfdinf ContentsFileInfoDetail* Structure of content file information
	 * \param index A number of index
	 * \param err A CriError information
	 * \return	CriBool Result.
	 * \par
	 * This function gets a content file information by number of index.
	 */
	virtual CriBool GetContentsFileInfoDetails(ContentsFileInfoDetail *cfdinf, CriSint32 index, CriError &err = CriFs::ErrorContainer) = 0;
	
	/*JP
	 * \brief CPKファイルパス名の取得
	 * \param err エラーコード
	 * \return	CriChar8 * CPKファイルパス名
	 * \par 説明:
	 * CPKファイルパス名を取得します
	 */
	/*EN
	 * \brief Get the CPK file path
	 * \param err A CriError information
	 * \return	CriChar8 * CPK filename path
	 * \par
	 * This function gets a CPK file path string.
	 */
	virtual	const CriChar8 *GetCpkFilePath(CriError &err = CriFs::ErrorContainer) = 0;

	/*JP
	 * \brief コンテンツファイル数の取得
	 * \param err エラーコード
	 * \return	コンテンツファイル数
	 * \par 説明:
	 * コンテンツファイルの数を取得します。
	 */
	/*EN
	 * \brief Get the number of content
	 * \param err A CriError information
	 * \return	The number of content.
	 * \par
	 * This function gets number of content.
	 */
	virtual CriSint32 GetNumberOfContents(CriError &err = CriFs::ErrorContainer) = 0;
	
	// 互換性維持のため
	CriSint32 GetNumContents(CriError &err = CriFs::ErrorContainer) { return GetNumberOfContents(err); }

	/*JP
	 * \if fsgroup
	 * \brief CriFsGroupLoaderオブジェクトの作成
	 * \param	groupname グループ名
	 * \param	attrname 属性名
	 * \param	err エラーコード
	 * \return	グループローダオブジェクト
	 * \par 説明:
	 * CriFsGroupLoaderオブジェクトを作成します。
	 * \endif
	 */
	/*EN
	 * \if fsgroup
	 * \brief Create a CriFsGroupLoader object
	 * \param	groupname A Name of group
	 * \param	attrname A Name of attribute
	 * \param err A CriError information
	 * \return	A CriFsGroupLoader object
	 * \par
	 * This function creates a CriFsGroupLoader object.
	 * \endif
	 */
	 virtual CriFsGroupLoader *CreateGroupLoader(const CriChar8 *groupname, const CriChar8 *attrname, CriError &err = CriFs::ErrorContainer) = 0;

	/*JP
	 * \if fsgroup
	 * \brief CriFsGroupLoaderオブジェクトの破棄
	 * \param	err エラーコード
	 * \par 説明:
	 * CriFsLoaderオブジェクトを破棄します。
	 * \sa CriFsManagerCpk::CreateGroupLoader
	 * \endif
	 */
	/*EN
	 * \if fsgroup
	 * \brief Destroy CriFsGroupLoader object
	 * \param err A CriError information
	 * \par
	 * This function deletes the CriFsGroupLoader object.
	 * \sa CriFsManagerCpk::CreateGroupLoader
	 * \endif
	 */
	 virtual void DestroyGroupLoader(CriFsGroupLoader *grouploader, CriError &err = CriFs::ErrorContainer) = 0;

	/*JP
	 * \if fsgroup
	 * \brief グループファイル数の取得
	 * \param	groupname グループ名
	 * \param	attrname 属性名
	 * \param	err エラーコード
	 * \par 説明:
	 * グループに属するファイル数を取得します。
	 * \sa CriFsManagerCpk::GetTotalGroupDataSize
	 * \endif
	 */
	/*EN
	 * \if fsgroup
	 * \brief Get the number of files in group
	 * \param	groupname A Name of group
	 * \param	attrname A Name of attribute
	 * \param err A CriError information
	 * \par
	 * This function gets number of files in group.
	 * \sa CriFsManagerCpk::GetTotalGroupDataSize
	 * \endif
	 */
	 virtual CriSint32 GetNumberOfGroupFiles(const CriChar8 *groupname, const CriChar8 *attrname, CriError &err = CriFs::ErrorContainer) = 0;

	/*JP
	 * \if fsgroup
	 * \brief グループデータサイズの取得
	 * \param	groupname グループ名
	 * \param	attrname 属性名
	 * \param	err エラーコード
	 * \par 説明:
	 * グループロードに必要なデータサイズを取得します。<br>
	 * アライメントなども加味されたデータサイズとなります。
	 * \sa CriFsManagerCpk::GetNumberOfGroupFiles
	 * \endif
	 */
	/*EN
	 * \if fsgroup
	 * \brief Get the size of data in group
	 * \param	groupname A Name of group
	 * \param	attrname A Name of attribute
	 * \param err A CriError information
	 * \par
	 * This function gets size of data in group. Alignment is included in data size.
	 * \sa CriFsManagerCpk::GetNumberOfGroupFiles
	 * \endif
	 */
	 virtual CriSint64 GetTotalGroupDataSize(const CriChar8 *groupname, const CriChar8 *attrname, CriError &err = CriFs::ErrorContainer) = 0;

protected:
	CriFsManagerCpk() {};
	virtual ~CriFsManagerCpk() {};
};

/*-------------------------------------------------------------------------*
 *	CriFsGroupLoader
 *-------------------------------------------------------------------------*/
/*JP
 * \if fsgroup
 * \brief グループローダ
 * \ingroup FSLIB_BASIC
 * \par 説明:
 * CriFsGroupLoaderはCPKファイルのグループロードを行うためのオブジェクトです。
 * \endif
 */
/*EN
 * \if fsgroup
 * \brief Group Loader
 * \ingroup FSLIB_BASIC
 * \par
 * CriFsGroupLoader is an object for performing the group load of a CPK file.
 * \endif
 */
class CriFsGroupLoader
{
public:
	enum Status {
		LOAD_STATUS_STOP=(0),	/*JP< 停止中		*/	/*EN< Stopping			*/
		LOAD_STATUS_LOADING,	/*JP< ロード中		*/	/*EN< Loading			*/
		LOAD_STATUS_COMPLETE,	/*JP< 完了			*/	/*EN< Complete			*/
		LOAD_STATUS_ERROR,		/*JP< エラー		*/	/*EN< Error				*/
		/* enum be 4bytes */
		LOAD_STATUS_ENUM_BE_SINT32 = 0x7FFFFFFF
	};

	/*JP
	 * \if fsgroup
	 * \brief データのロード
	 * \param	buffer バッファへのポインタ
	 * \param	buffer_size バッファのサイズ
	 * \param	gfinf[] CriFsManagerCpk::GroupFileInfo構造体の配列
	 * \param	numgfinf 配列の数
	 * \param	err エラーコード
	 * \par 説明:
	 * グループ化された複数ファイルデータの読み込みを開始します。<br>
	 * ファイル数分のCriFsManagerCpk::GroupFileInfo構造体の配列を指定する必要があります。<br>
	 * 本関数は即時復帰関数です。<br>ロードの完了状態を取得するには CriFsGroupLoader::GetStatus関数を使用してください。<br>
	 * \sa CriFsGroupLoader::GetStatus
	 * \endif
	 */
	/*EN
	 * \if fsgroup
	 * \brief Loading File Data
	 * \param	buffer		A Reading buffer
	 * \param	buffer_size A Size of Reading buffer
	 * \param	gfinf[]		A CriFsManagerCpk::GroupFileInfo array of structures
	 * \param	numgfinf	A Number of structures
	 * \param 	err 		A CriError information
	 * \par
	 * This function starts loading of the group of files.<br>
	 * This function is return immediately.
	 * You use an CriFsGroupLoader::GetStatus function to examine reading completion.<br>
	 * \sa CriFsGroupLoader::GetStatus
	 * \endif
	 */
	virtual CriSint32 LoadBulk(void *buffer, CriSint64 buffer_size, CriFsManagerCpk::GroupFileInfo gfinf[], CriSint32 numgfinf, CriError &err=CriFs::ErrorContainer) = 0;

	 /*JP
	 * \if fsgroup
	 * \brief ロードステータスの取得
	 * \param err エラーコード<br>
	 * \return	CriFsGroupLoader::Status ロードステータス
	 * \par 説明:
	 * ロードステータスを取得します。
	 * \image html fs_state_transition.png グループローダの状態遷移図
	 */
	/*EN
	 * \brief Get Loading status
	 * \param err A CriError information
	 * \return	CriFsGroupLoader::Status Loading Status
	 * \par
	 * This function returns Loading status.
	 * \image html fs_state_transition.png Transition of Group Loader Status
	 * \endif
	 */
	virtual CriFsGroupLoader::Status GetStatus(CriError &err=CriFs::ErrorContainer) = 0;

	 /*JP
	 * \if fsgroup
	 * \brief ロードされたファイル数の取得
	 * \param err エラーコード<br>
	 * \return	ロードされたファイル数
	 * \par 説明:
	 * CriFsGroupLoader::LoadBulk 関数で既に読み込まれたファイル数を返します
	 */
	/*EN
	 * \brief Get Loaded files
	 * \param err A CriError information
	 * \return	The loaded files
	 * \par
	 * This function returns Loaded number of files.
	 * \endif
	 */
	virtual CriSint32 GetLoadedFiles(CriError &err=CriFs::ErrorContainer) = 0;


	/*JP
	 * \if fsgroup
	 * \brief グループファイル数の取得
	 * \param	err エラーコード
	 * \par 説明:
	 * グループに属するファイル数を取得します。
	 * \sa CriFsGroupLoader::GetTotalGroupDataSize
	 * \endif
	 */
	/*EN
	 * \if fsgroup
	 * \brief Get the number of files in group
	 * \param err A CriError information
	 * \par
	 * This function gets number of files in group.
	 * \sa CriFsGroupLoader::GetTotalGroupDataSize
	 * \endif
	 */
	 virtual CriSint32 GetNumberOfGroupFiles(CriError &err = CriFs::ErrorContainer) = 0;

	/*JP
	 * \if fsgroup
	 * \brief グループデータサイズの取得
	 * \param	err エラーコード
	 * \par 説明:
	 * グループロードに必要なデータサイズを取得します。<br>
	 * アライメントなども加味されたデータサイズとなります。
	 * \sa CriFsGroupLoader::GetNumberOfGroupFiles
	 * \endif
	 */
	/*EN
	 * \if fsgroup
	 * \brief Get the size of data in group
	 * \param err A CriError information
	 * \par
	 * This function gets size of data in group. Alignment is included in data size.
	 * \sa CriFsGroupLoader::GetNumberOfGroupFiles
	 * \endif
	 */
	 virtual CriSint64 GetTotalGroupDataSize(CriError &err = CriFs::ErrorContainer) = 0;

	/*JP
	 * \if fsgroup
	 * \brief CriFsManagerCpk::GroupFileInfo構造体の配列インデクスの取得
	 * \param	gfinf[] CriFsManagerCpk::GroupFileInfo構造体の配列
	 * \param	numgfinf 配列の数
	 * \param	fpath ファイル名フルパス
	 * \param	err エラーコード
	 * \par 説明:
	 * 指定されたファイルのCriFsManagerCpk::GroupFileInfo構造体の配列インデクスを取得します。<br>
	 * ファイル名はフルパス名で指定します。<br>
	 * 指定されたファイルが見つからない場合は、返値が-1となります。
	 * \sa CriFsGroupLoader::GetNumberOfGroupFiles
	 * \endif
	 */
	/*EN
	 * \if fsgroup
	 * \brief Get the index of file in CriFsManagerCpk::GroupFileInfo array of structures.
	 * \param err A CriError information
	 * \param	gfinf[]		A CriFsManagerCpk::GroupFileInfo array of structures
	 * \param	numgfinf	A Number of structures
	 * \param	fpath    	A Filename path
	 * \param 	err 		A CriError information
	 * \par
	 * This function gets index of file in CriFsManagerCpk::GroupFileInfo array of structure.
	 * \sa CriFsManagerCpk::GroupFileInfo
	 * \endif
	 */
	 virtual CriSint32 GetGroupFileInfoIndex(const CriFsManagerCpk::GroupFileInfo gfinf[], CriSint32 numgfinf, const CriChar8 *fpath, CriError &err=CriFs::ErrorContainer) = 0;

	/*JP
	 * \if fsgroup
	 * \brief グループ名の取得
	 * \param	err エラーコード
	 * \par 説明:
	 * ローダーで扱うグループのグループ名を取得します。
	 * \endif
	 */
	/*EN
	 * \if fsgroup
	 * \brief Get a name of group.
	 * \param err A CriError information.
	 * \par
	 * This function gets a name of group.
	 * \endif
	 */
 	 virtual CriChar8 *GetGroupName(CriError &err = CriFs::ErrorContainer) = 0;

	/*JP
	 * \if fsgroup
	 * \brief グループ属性の取得
	 * \param	err エラーコード
	 * \par 説明:
	 * ローダーで扱うグループのグループ属性を取得します。
	 * \endif
	 */
	/*EN
	 * \if fsgroup
	 * \brief Get a attribute of group.
	 * \param err A CriError information.
	 * \par
	 * This function gets a attribute of groupp.
	 * \endif
	 */
 	 virtual CriChar8 *GetAttributeName(CriError &err = CriFs::ErrorContainer) = 0;

protected:
	CriFsGroupLoader() {};
	virtual ~CriFsGroupLoader() {};
};

/*-------------------------------------------------------------------------*
 *	CriFsLoader
 *-------------------------------------------------------------------------*/
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
 * CriFsLoader is reading module.It is very simple and easy.
 */
class CriFsLoader : public CriAllocator
{
public:
	/*JP
	 * \brief ロードステータス
	 */
	/*EN
	 * \brief Loading Status
	 */
	 enum Status {
		LOAD_STATUS_STOP=(0),	/*JP< 停止中		*/	/*EN< Stopping			*/
		LOAD_STATUS_LOADING,	/*JP< ロード中		*/	/*EN< Loading			*/
		LOAD_STATUS_COMPLETE,	/*JP< 完了			*/	/*EN< Complete			*/
		LOAD_STATUS_ERROR,		/*JP< エラー		*/	/*EN< Error				*/
		/* enum be 4bytes */
		LOAD_STATUS_ENUM_BE_SINT32 = 0x7FFFFFFF
	};

	/*JP
	 * \brief エラーコード
	 */
	/*EN
	 * \brief Error Code
	 */
	enum ErrorCode {
		ERROR_CODE_OK=(0),		/*JP< 正常							*/	/*EN< No Error					*/
		ERROR_CODE_CANT_OPEN,	/*JP< ファイルオープンできない		*/	/*EN< Can't Open a file			*/
		ERROR_CODE_READ_ERROR,	/*JP< 読み込みエラー				*/	/*EN< Reading error				*/
		/* enum be 4bytes */
		ERROR_CODE_ENUM_BE_SINT32 = 0x7FFFFFFF
	};

	/*JP
	 * \brief CriFsLoaderオブジェクトの作成
	 * \param heap CRI Heap オブジェクト
	 * \param err エラーコード
	 * \return	CriFsLoader オブジェクト
	 * \par 説明:
	 * CriFsLoaderオブジェクトを作成します。
	 */
	/*EN
	 * \brief Create a CriFsLoader
	 * \param heap A CRI Heap object
	 * \param err A CriError information
	 * \return	A CriFsLoader object
	 * \par
	 * This function creates a CriFsLoader object.
	 */
	static CriFsLoader *Create(CriHeap heap, CriError &err=CriFs::ErrorContainer);
	
	/*JP
	 * \brief CriFsLoaderオブジェクトの破棄
	 * \param	err エラーコード
	 * \par 説明:
	 * CriFsLoaderオブジェクトを破棄します。
	 * \sa CriFsLoader::Create
	 */
	/*EN
	 * \brief Destroy CriFsLoader object
	 * \param err A CriError information
	 * \par
	 * This function deletes the CriFsLoader object.
	 * \sa CriFsLoader::Create
	 */
	virtual void Destroy(CriError &err=CriFs::ErrorContainer) = 0;

	/*JP
	 * \brief プライオリティの取得
	 * \param	err エラーコード
	 * \return	CriFs::Priority 読み込みプライオリティ
	 * \par 説明:
	 * データロードのプライオリティを取得します。
	 * \sa CriFsLoader::SetPriority
	 */
	/*EN
	 * \brief Get a Priority
	 * \param err A CriError information
	 * \return CriFs::Priority Reading Priority
	 * \par
	 * This function gets Loading Priority.
	 * \sa CriFsLoader::SetPriority
	 */
	virtual CriFs::Priority GetPriority(CriError &err = CriFs::ErrorContainer) = 0;
	
	/*JP
	 * \brief プライオリティの設定
	 * \param	priority 読み込みプライオリティ
	 * \param	err エラーコード
	 * \par 説明:
	 * データロードのプライオリティを設定します。
	 * \sa CriFsLoader::GetPriority
	 */
	/*EN
	 * \brief Set a Priority
	 * \param priority Priority of the loader
	 * \param err A CriError information
	 * \par
	 * This function sets Loading Priority.
	 * \sa CriFsLoader::GetPriority
	 */
	virtual void SetPriority(CriFs::Priority priority, CriError &err = CriFs::ErrorContainer) = 0;
	
	/*JP
	 * \brief 単位読み込みサイズの設定
	 * \param size 単位読み込みサイズ
	 * \param err エラーコード
	 * \par 説明:
	 * 単位読み込みサイズを設定します。
	 * CriFsLoaderは、大きなサイズのリード要求を処理する際、それを複数の小さな単位のリード処理に分割して連続実行します。<br>
	 * この関数を使用することで単位リード処理サイズを変更することが可能です。<br>
	 * リード要求のキャンセルや、高プライオリティのリード処理の割り込み等は、単位リードサイズ境界でのみ処理されます。<br>
	 * そのため、ユニットサイズを小さく設定すると、I/O処理のレスポンスが向上します。逆に、ユニットサイズを大きく設定すると、ファイル単位の読み込み速度が向上します。
	 * \sa CriFsLoader::GetReadUnitSize
	 */
	/*EN
	 * \brief Get Read Unit Size
	 * \param err The size of read unit
	 * \param err A CriError information
	 * \par
	 * When reading large data, CriFsLoader divides it into multiple small unit, and reads them.<br>
	 * This function sets the size of read units.<br>
	 * Interrupt request, such as cancel of current read process, can be applied only at unit boundary.<br>
	 * Therefore, if the unit size is smaller, the response of I/O process becomes better. If the unit size is larger, the speed of single reading becomes better.
	 * \sa CriFsLoader::GetReadUnitSize
	 */
	virtual void SetReadUnitSize(CriSint64 size, CriError &err = CriFs::ErrorContainer) = 0;
	
	/*JP
	 * \brief 単位読み込みサイズの取得
	 * \param err エラーコード
	 * \return 単位読み込みサイズ
	 * \par 説明:
	 * 単位読み込みサイズを取得します。
	 * \sa CriFsReader::SetReadUnitSize
	 */
	/*EN
	 * \brief Get Read Unit Size
	 * \param err A CriError information
	 * \return read unit size
	 * \par
	 * This function gets the size of read units.
	 * \sa CriFsReader::SetReadUnitSize
	 */
	virtual CriSint64 GetReadUnitSize(CriError &err = CriFs::ErrorContainer) = 0;
	
	/*JP
	 * \brief データのロード
	 * \param	manager ファイルシステムマネージャ
	 * \param	path ファイルパス名
	 * \param	offset ファイルの先頭からのオフセット位置
	 * \param	read_size リードサイズ
	 * \param	buffer バッファへのポインタ
	 * \param	buffer_size バッファのサイズ
	 * \param	err エラーコード
	 * \par 説明:
	 * 指定されたファイルシステムマネージャとファイル名で、データの読み込みを開始します。<br>
	 * ファイル内の offset バイト目から、read_size バイト分読み込みます。<br>
	 * 本関数は即時復帰関数です。<br>ロードの完了状態を取得するには CriFsLoader::GetStatus関数を使用してください。<br>
	 * \sa CriFsLoader::GetStatus
	 */
	/*EN
	 * \brief Loading File Data
	 * \param	manager		A File System Manager
	 * \param	path 		A File path
	 * \param	offset		A Seek position from top of file
	 * \param	read_size	A Reading size
	 * \param	buffer		A Reading buffer
	 * \param	buffer_size A Size of Reading buffer
	 * \param 	err 		A CriError information
	 * \par
	 * This function starts loading of data by appointed File path and the File System Manager.<br>
	 * This function is return immediately.
	 * You use an CriFsLoader::GetStatus function to examine reading completion.<br>
	 * \sa CriFsLoader::GetStatus
	 */
	virtual void Load(CriFsManager *manager, const CriChar8 *path,
		CriSint64 offset, CriSint64 read_size, void *buffer, CriSint64 buffer_size,
		CriError &err=CriFs::ErrorContainer) = 0;

	/*JP
	 * \brief データのロード(シーク位置とサイズの省略)
	 * \param	manager ファイルシステムマネージャ
	 * \param	path ファイルパス名
	 * \param	buffer バッファへのポインタ
	 * \param	buffer_size バッファのサイズ
	 * \param	err エラーコード
	 * \par 説明:
	 * 指定されたファイルシステムマネージャとファイルで、データの読み込みを開始します。<br>
	 * 本関数は即時復帰関数です。<br>ロードの完了状態を取得するには CriFsLoader::GetStatus関数を使用してください。<br>
	 * - ファイルの先頭から読み込みを開始します。<br>
	 * \sa CriFsLoader::GetStatus
	 */
	/*EN
	 * \brief Loading File Data(Abbreviation of Seek Position)
	 * \param	manager		A File System Manager
	 * \param	path 		A File path
	 * \param	buffer		A Reading buffer
	 * \param	buffer_size A Size of Reading buffer
	 * \param 	err 		A CriError information
	 * \par
	 * This function starts loading of data by appointed File path and the File System Manager.<br>
	 * This function is return immediately. You use an CriFsLoader::GetStatus function to examine reading completion.<br>
	 * - This function starts reading from the top of file.<br>
	 * \sa CriFsLoader::GetStatus
	 */
	virtual void Load(CriFsManager *manager, const CriChar8 *path,
		void *buffer, CriSint64 buffer_size, CriError &err=CriFs::ErrorContainer) = 0;

	/*JP
	 * \brief データのロード (CPKファイル内のファイル識別子を指定)
	 * \param	manager CPKファイルシステムマネージャ
	 * \param	fid ファイル識別子
	 * \param	offset ファイルの先頭からのオフセット位置
	 * \param	read_size リードサイズ
	 * \param	buffer バッファへのポインタ
	 * \param	buffer_size バッファのサイズ
	 * \param	err エラーコード
	 * \par 説明:
	 * 指定されたCPKファイルシステムマネージャとファイル識別子で、データの読み込みを開始します。<br>
	 * ファイル内の offset バイト目から、read_size バイト分読み込みます。<br>
	 * 本関数は即時復帰関数です。<br>ロードの完了状態を取得するには CriFsLoader::GetStatus関数を使用してください。<br>
	 * \sa CriFsLoader::GetStatus
	 */
	/*EN
	 * \brief Loading File Data (Using File ID in CPK File)
	 * \param	manager		A File System Manager
	 * \param	file 		A File ID
	 * \param	offset		A Seek position from top of file
	 * \param	read_size	A Reading size
	 * \param	buffer		A Reading buffer
	 * \param	buffer_size A Size of Reading buffer
	 * \param 	err 		A CriError information
	 * \par
	 * This function starts loading of data by appointed File ID and the File System Manager of CPK.<br>
	 * This function is return immediately.
	 * You use an CriFsLoader::GetStatus function to examine reading completion.<br>
	 * \sa CriFsLoader::GetStatus
	 */
	virtual void Load(CriFsManagerCpk *manager, CriUint32 fid,
		CriSint64 offset, CriSint64 read_size, void *buffer, CriSint64 buffer_size,
		CriError &err=CriFs::ErrorContainer) = 0;

	/*JP
	 * \brief データのロード(シーク位置とサイズの省略、CPKファイル内のファイル識別子を指定)
	 * \param	manager CPKファイルシステムマネージャ
	 * \param	path ファイルパス名
	 * \param	buffer バッファへのポインタ
	 * \param	buffer_size バッファのサイズ
	 * \param	err エラーコード
	 * \par 説明:
	 * 指定されたCPKファイルシステムマネージャとファイル識別子で、データの読み込みを開始します。<br>
	 * 本関数は即時復帰関数です。<br>ロードの完了状態を取得するには CriFsLoader::GetStatus関数を使用してください。<br>
	 * - ファイルの先頭から読み込みを開始します。<br>
	 * \sa CriFsLoader::GetStatus
	 */
	/*EN
	 * \brief Loading File Data(Abbreviation of Seek Position, Using File ID in CPK File)
	 * \param	manager		A File System Manager
	 * \param	path 		A File path
	 * \param	buffer		A Reading buffer
	 * \param	buffer_size A Size of Reading buffer
	 * \param 	err 		A CriError information
	 * \par
	 * This function starts loading of data by appointed File ID in CPK File and the File System Manager CPK.<br>
	 * This function is return immediately. You use an CriFsLoader::GetStatus function to examine reading completion.<br>
	 * - This function starts reading from the top of file.<br>
	 * \sa CriFsLoader::GetStatus
	 */
	virtual void Load(CriFsManagerCpk *manager, CriUint32 fid,
		void *buffer, CriSint64 buffer_size, CriError &err=CriFs::ErrorContainer) = 0;

	 /*JP
	 * \brief ロードステータスの取得
	 * \param err エラーコード<br>
	 * \return	CriFsLoader::Status ロードステータス
	 * \par 説明:
	 * ロードステータスを取得します。
	 * \image html fs_state_transition.png ファイルシステムローダの状態遷移図
	 * \sa CriFsLoader::GetErrorCode
	 */
	/*EN
	 * \brief Get Loading status
	 * \param err A CriError information
	 * \return	CriFsLoader::Status Loading Status
	 * \par
	 * This function returns Loading status.
	 * \image html fs_state_transition.png Transition of File System Loader Status
	 * \sa CriFsLoader::GetErrorCode
	 */
	virtual CriFsLoader::Status GetStatus(CriError &err=CriFs::ErrorContainer) = 0;
	
	/*JP
	 * \brief ローダエラーコードの取得
	 * \param err エラーコード<br>
	 * \return	CriFsLoader::ErrorCode ローダエラーコード
	 * \par 説明:
	 * ローダエラーコードを取得します。<br>
	 * ローダステータスがエラーの際、エラーの詳細を調べる事が出来ます。
	 */
	/*EN
	 * \brief Get Error Code of Loading
	 * \param err A CriError information
	 * \return	CriFsLoader::ErrorCode Error Code of Loading
	 * \par
	 * This function returns Error Code of Loading.<br>
	 * You can examine the details of error.
	 */
	virtual CriFsLoader::ErrorCode GetErrorCode(CriError &err=CriFs::ErrorContainer) = 0;
	
	/*JP
	 * \brief ロードの停止
	 * \param err エラーコード<br>
	 * \par 説明:
	 * ロードを停止します。<br>本関数は即時復帰関数です。停止状態を取得するには CriFsLoader::GetStatus関数を使用してください。<br>
	 * \sa CriFsLoader::GetStatus
	 */
	/*EN
	 * \brief Stop Loading
	 * \param err A CriError information
	 * \par
	 * This function stops the loading.
	 * \sa CriFsLoader::GetStatus
	 */
	virtual void Stop(CriError &err=CriFs::ErrorContainer) = 0;
	
	/*JP
	 * \brief ロードサイズの取得
	 * \param err エラーコード<br>
	 * \return	ロードサイズ
	 * \par 説明:
	 * ロードされたサイズを取得します。<br>
	 */
	/*EN
	 * \brief Get Loading size
	 * \param err A CriError information
	 * \return	Loading size
	 * \par
	 * This function returns Loading size.
	 */
	virtual CriSint64 GetLoadSize(CriError &err=CriFs::ErrorContainer) = 0;

protected:
	/* コンストラクタ＆デストラクタ */
	CriFsLoader(void) {};
	virtual ~CriFsLoader(void) {};
};


/*-------------------------------------------------------------------------*
 *	CriFsIo
 *-------------------------------------------------------------------------*/
class CriFsIo
{
public:
	/* デバイス情報 */
	typedef struct {
		CriChar8 path[32];
		CriSint32 bus;
		CriBool can_read;
		CriBool can_write;
		CriBool can_seek;
		CriSint32 minimum_read_size;
		CriSint32 minimum_write_size;
		CriSint32 minimum_seek_size;
		CriSint32 read_buffer_alignment;
		CriSint32 write_buffer_alignment;
	} DeviceInfo;
	
	static const CriSint64 INVALID_SIZE = -1;
	
	/* デバイス情報の取得 */
	virtual CriBool GetDeviceInfo(const CriChar8 *path, DeviceInfo *devinfo) = 0;
	
	/* I/O処理 */
	virtual void *Open(CriHeap heap, const CriChar8 *path, CriFs::FileMode mode, CriFs::FileAccess access) = 0;
	virtual CriBool Close(CriHeap heap, void *fp) = 0;
	virtual CriSint64 GetFileSize(void *fp) = 0;
	virtual CriSint64 Read(void *fp, CriSint64 offset, void *buffer, CriSint64 size) = 0;
	virtual CriSint64 Write(void *fp, CriSint64 offset, void *buffer, CriSint64 size) = 0;
	virtual CriBool Flush(void *fp) = 0;
	virtual CriBool Resize(void *fp, CriSint64 size) = 0;
	
	/* コンストラクタ＆デストラクタ */	
	CriFsIo(void) {};
	virtual ~CriFsIo(void) {};
};



/***************************************************************************
 *      関数宣言
 *      Prototype Functions
 ***************************************************************************/
/*-------------------------------------------------------------------------*
 *	CriFs
 *-------------------------------------------------------------------------*/
/*JP
 * \brief CRI File System オブジェクト
 * \ingroup FSLIB_BASIC
 * \par 説明:
 * CRI File Systemの基本となるオブジェクトクラスです。
 */
/*EN
 * \brief CRI File System Object
 * \ingroup FSLIB_BASIC
 * \par
 * Basic object class for CRI File System.
 */
namespace CriFs 
{
	/*JP
	 * \brief CRI File Systemの初期化
	 * \param err エラーコード<br>
	 * \par 説明:
	 * CRI File Systemを初期化します。<br>
	 * CRI File Systemを使用するためには、この関数をコールして初期化を行ってください。<br>
	 * \sa CriFs::Finalize
	 */
	/*EN
	 * \brief Initialize the CRI File System
	 * \param err A CriError information
	 * \par
	 * Initialize the CRI File System<br>
	 * \sa CriFs::Finalize
	 */
	void CRIAPI Initialize(CriError &err = CriFs::ErrorContainer);
	
	/*JP
	 * \brief CRI File Systemの終了
	 * \param err エラーコード<br>
	 * \par 説明:
	 * CRI File Systemを終了します。<br>
	 * \sa CriFs::Initialize
	 */
	/*EN
	 * \brief Finalize the CRI File System
	 * \param err A CriError information
	 * \par
	 * Finalize the CRI File System<br>
	 * \sa CriFs::Initialize
	 */
	 void CRIAPI Finalize(CriError &err = CriFs::ErrorContainer);
	
	/*JP
	 * \brief 外部I/Oインタフェースの登録
	 * \param io CriFsIo I/Oインタフェース
	 * \param err エラーコード<br>
	 * \par 説明:
	 * 外部I/Oインタフェースを登録します。<br>
	 * 独自のI/Oモジュールを使用することが可能になります。
	 */
	/*EN
	 * \brief Set External I/O Interface
	 * \param io CriFsIo I/O Interface
	 * \param err A CriError information
	 * \par
	 * This function sets the I/O Interface<br>
	 */
	void CRIAPI SetIoInterface(CriFsIo *io, CriError &err = CriFs::ErrorContainer);
	
	/*JP
	 * \brief I/Oインタフェースの取得
	 * \param err エラーコード<br>
	 * \return CriFsIo I/Oインタフェース
	 * \par 説明:
	 * I/Oインタフェースを取得します。<br>
	 */
	/*EN
	 * \brief Set External I/O Interface
	 * \param err A CriError information
	 * \return CriFsIo I/O Interface
	 * \par
	 * This function gets the I/O Interface<br>
	 */
	CriFsIo * CRIAPI GetIoInterface(CriError &err = CriFs::ErrorContainer);
	
//	void CRIAPI AllocateStreamingBuffer(CriHeap heap, CriUint32 size, CriError &err = CriFs::ErrorContainer);
//	void CRIAPI ReleaseStreamingBuffer(CriError &err = CriFs::ErrorContainer);
}

#endif	/* CRI_FILE_SYSTEM_VER1PAI_H_INCLUDED */

#endif /* __cplusplus */

/* --- end of file --- */
