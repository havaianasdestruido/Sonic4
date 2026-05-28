// ===========================================================================
/*!
	@file	aoStorageWin32.cpp
	@brief	AoLibrary セーブ＆ロード管理モジュール定義(Win32)

	@author	K.OKUGAWA Copyright (C) 2010 Dimps.
 */
// ===========================================================================

// ----- Include Files ---------------------------------------（インクルード）

#if _IPHONE
#include <algorithm>
#include <Foundation/Foundation.h>

// ----- Macros ------------------------------------------------（マクロ定義）
// ----- Macro Functions -----------------------------------（処理マクロ定義）
// ----- Definitions -------------------------------------------（定数の宣言）
// ----- Struct Definitions --------------------------------------（型の宣言）
// ----- Class Definitions -------------------------------------（クラス宣言）
// ----- External Declarations -----------------（グローバル変数及び関数宣言）
// ----- Static Declarations -----------------（スタティック変数及び関数宣言）
// ----- Global Variables ----------------------（グローバル変数の定義：外部）
// ----- Static Variables --------------------（スタティック変数の定義：局所）
// ----- Global Functions ----------------------（グローバル関数の定義：外部）

// ===========================================================================
//	AoStorageGetSaveDirectoryPath
/*!
	セーブデータのディレクトリパスを取得

	@param out			[in] 出力先(NULL可)
	@param out_size		[in] 出力先のサイズ
	@return 書き出せなかった残り文字数
	
	@note
	out が NULL の場合は out_size は無視されますが文字数は正常に返ります。\n
	out_size サイズには末端の'\0'を含めますが、
	戻り値には末端の'\0'を含めません。\n
*/
// ===========================================================================
unsigned int AoStorageGetSaveDirectoryPath(char *out, unsigned int out_size)
{
	NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];

	unsigned int result;
	
	//ディレクトリパスを取得
	NSArray* paths = NSSearchPathForDirectoriesInDomains(
							NSDocumentDirectory, NSUserDomainMask, YES);
	NSString *docpath = [paths objectAtIndex:0];
	
	//パスの文字数取得
	unsigned int length = [docpath lengthOfBytesUsingEncoding:NSShiftJISStringEncoding];
	if (length + 1 <= out_size) {
		//\0含めて全て書き出せる
		result = 0;
	} else if (2 <= out_size) {
		//全ては書き出せないが、一部書き出せる
		result = length - out_size + 1;
	} else {
		//全く書き出せない
		result = length;
	}
	
	//書き出し
	if (out && (0 < out_size)) {
		//書き出し先が有るなら
		const char* path = [docpath cStringUsingEncoding:NSShiftJISStringEncoding];
		unsigned int copy_size = std::min(out_size - 1, length);
		memcpy(out, path, sizeof(char) * copy_size);
		out[copy_size] = '\0';
	}

	[pool release];
	return result;
}

// ***************************************************************************
// セーブ
// ***************************************************************************
// ===========================================================================
//	AoStorageSaveMm
/*!
	Objective-C上のファイル書き出し

	@param path	[in] ファイルパス
	@param data	[in] 書き出すデータ
	@param size	[in] 書き出すサイズ
	@retval 0		成功
	@retval 0以外	失敗
*/
// ===========================================================================
unsigned int AoStorageSaveMm(const char *path, const void *data, unsigned int size)
{
	unsigned int result = 0;
	NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];

	NSString *ns_path = [[NSString alloc] initWithUTF8String:path];
	NSData *ns_data = [NSData dataWithBytes:data length:size];
	
	[ns_data writeToFile:ns_path atomically:YES];

	[ns_path release];

	[pool release];
	return result;
}


// ***************************************************************************
// ロード
// ***************************************************************************
// ===========================================================================
//	AoStorageLoadMm
/*!
	Objective-C上のファイル読み込み

	@param path	[in] ファイルパス
	@param data	[in] 読み込み先
	@param size	[in] 読み込みサイズ
	@retval 0		成功
	@retval 0以外	失敗
	
	@note
		ファイルサイズと読み込みサイズが違うと失敗します。\n
		
*/
// ===========================================================================
unsigned int AoStorageLoadMm(const char *path, void *data, unsigned int size)
{
	unsigned int result = 0;
	NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];

	NSString *ns_path = [[NSString alloc] initWithUTF8String:path];
	NSData *ns_data = [[NSData alloc] initWithContentsOfFile:ns_path];
	
	do {
		//ファイルサイズチェック
		unsigned int file_size = [ns_data length];
		if (size != file_size) {
			result = 1;
			break;
		}

		//ファイル読み込み
		[ns_data getBytes:data length:size];
	} while (false);

	[ns_data release];
	[ns_path release];

	[pool release];
	return result;
}


// ----- Static Functions --------------------（スタティック関数の定義：局所）

#endif // _IPHONE

// ===========================================================================
//	function
/*!
	説明

	@param param0	[in] 入力引数0説明
	@param param1	[out] 出力ポインタ引数1説明
	@param param2	[io] 入出力ポインタ引数2説明
	@return 返値説明
	@note 補足説明
*/
// ===========================================================================

// ===========================================================================
//	int variable
// ---------------------------------------------------------------------------
//!	変数説明
// ===========================================================================

	// =======================================================================
	//	function
	/*!
		説明

		@param param0	[in] 入力引数0説明
		@param param1	[out] 出力ポインタ引数1説明
		@param param2	[io] 入出力ポインタ引数2説明
		@return 返値説明
		@note 補足説明
	*/
	// =======================================================================

// ***************************************************************************
// ラベル
// ***************************************************************************
