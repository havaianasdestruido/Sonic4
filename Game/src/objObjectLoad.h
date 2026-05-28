// ================================================================
/*!
  @file objObjectLoad.h
  @brief オブジェクト読み込み、取得、解放

  @author mana
                Copyright(c) 2004 Dimps

  $Id: objObjectLoad.h 2 2011-04-11 05:21:26Z thamada $
  $Date:: 2011-04-11 14:21:26 +0900#$
 */
// ================================================================
/*
 * Memo
 *
 */
/*!
  @page  obj_load obj データロード
 
  @section  obj_load_file ファイル読み込み、開放
    　 ObjDataLoad() で取得、 ObjDataRelease() で開放を行う。\n
    　 この時、 #OBS_DATA_WORK を用意すれば、既に読み込んだデータであれば、\n
    　 読み直しが行われず、開放時も すべて開放され、どこからも参照されなくなった時のみ実際に開放され、メモリが共用される。\n
    　 （ただし #OBS_DATA_WORKは一番最初に初期化してある事）\n
    　 これを用いれば、処理速度の向上とメモリ消費量の削減が可能となる。\n
    　 また、ロードの第3引数のアーカイブポインタを設定すれば、1度目の読み込みでも処理が遅くならない上、アーカイブを展開していれば、ワンカートリッジでもそのままのソースで使用する事ができる\n
    \n
    　 ObjNarcGetFile() でアーカイブから指定したファイルのみを取り出す事もできる。\n
    　 #OBS_OBJECT_WORK を介して 扱う場合、これらはほぼ自動で処理される\n
    \n
    
  @section  obj_load_mod モデルデータ取得
    　 ObjModLoadTexSetUpAndRelease() でモデルデータのテクスチャを除いたデータを読み込む。\n
    　 テクスチャはこの関数の内部で一旦読み込むが、そこでVRAMに転送した後、不要になるのでシステムメモリから除き、メモリ節約している\n
    　 基本的に、objDataLoad() で取得済みのファイルに対して行う。\n
    \n
    
  @section  obj_load_vram VRAM取得、開放
    　 ObjVramAllocTex() でVRAMを取得し、 ObjVramRelease()でVRAMを開放する。\n
    　 #OBS_DATA_WORKを用いて、同じデータを共用する事ができる。\n
    　 アニメーションしないが、多量に表示されるスプライトなどに特に有効となる\n
    　 共用しない場合は、これら関数を通す必要は無い。\n
    \n
    　 ObjActVramAllocDS() 、 ObjActVramReleaseDS() を用いる事で、2画面両方のVRAMを取得、開放できる。
    　 ただし、ここで用いる #OBS_DATA_WORK は 二つ分連続した #OBS_DATA_WORK である事\n
    　 （ OBS_DATA_WORK _obj_sample_data[2]; // このような形 ）\n
    \n
    　 #OBS_OBJECT_WORK を介して 扱う場合、これらはほぼ自動で処理される\n
    \n
    
  @section  obj_load_tvram テクスチャ、パレットVRAM取得、開放
    　 画像データはobjVramAlloc() で取得し、 パレットデータは ObjVramAllocTexPlt()で取得を行う\n
    　 両方取得した場合、片方だけ取得した場合どちらでも ObjVramReleaseTex()を一度呼ぶ事で双方のVRAMを開放する。\n
    　 #OBS_DATA_WORKを用いて、同じデータを共用する事ができる。\n
    　 アニメーションしないが、多量に表示されるスプライトなどに特に有効となる\n
    　 共用しない場合は、これら関数を通す必要は無い。\n
    \n
    　 #OBS_OBJECT_WORK を介して 扱う場合、これらはほぼ自動で処理される\n
    \n
    
  @section  obj_load_act アクションデータ読み込み、開放
    　 ObjActLoad() と ObjActRelease() を使って、アクションデータとVRAMを統括して扱う事ができる\n
    　 内部で ファイル読み込み、VRAM取得、アクション初期化、 ファイル開放、VRAM開放を行う\n
    　 また、キャラサイズに #OBD_AUTO_CHARSIZEを用いれば、自動でそのアクションデータの最大キャラ数のVRAMを取得する。\n
    \n
    
  @section  obj_load_object OBS_OBJECT_WORKを用いた使用
    　 #OBS_OBJECT_WORKを用いれば 上記の項目のほとんどは意識せずに用い、データを共用する事ができる。\n
    \n
    　・ パレット
    　　　 ObjObjectPaletteLoad() を用いて、#OBS_OBJECT_WORKのパレットを取得すれば、オブジェクト死亡時、自動的に開放が行われる\n
    　　　 #OBS_OBJECT_WORK 内のアクション用のメモリも自動で取得、開放される。\n
    　　　 また、2画面用の管理や表示設定を簡単に操作できるようになる\n
    \n
    　・ アクション
    　　　 ObjObjectActionLoad() を用いて、#OBS_OBJECT_WORKのアクションデータとVRAMを取得すれば、オブジェクト死亡時、自動的に開放が行われる\n
    　　　 また、2画面用の管理も行える\n
    \n
    　・ 3Dスプライト
    　　　 ObjObjectAction3dSpriteLoad() を用いれば、オブジェクト死亡時、自動でアクションデータ、VRAM（パレット含む）が開放される\n
    　　　 #OBS_OBJECT_WORK 内の3Dスプライト用のメモリも自動で取得、開放される。\n
    　　　 テクスチャサイズに #OBD_AUTO_CHARSIZEを用いれば、自動でそのアクションデータの最大のテクスチャサイズのVRAMを取得する。\n
    　　　 パレット数に OBD_AUTO_PLTSIZEを用いれば、自動でそのアクションデータの最大のパレット数を取得する。\n
    \n
    　・ 3Dモデル
    　　　 ObjObjectAction3dModelLoad() を用いて、#OBS_OBJECT_WORKのモデルデータを取得すれば、オブジェクト死亡時、自動的に開放が行われる\n
    　　　 続けて、objObjectAction3dAnimeLoad() で各アニメーションデータを読み込めば、関連付けを気にせず、2Dのアクションに近い形で各アニメーションさせる事ができる\n
    　　　 #OBS_OBJECT_WORK 内の3D用のメモリも自動で取得、開放される。\n
    　　　 ただし、フル3Dなゲームでの使い勝手は未整備\n
    \n
    
    　・ 3Dモデル、1M1S （平たく言えばアニメーションなし）
    　　　 ObjObjectAction3dModelSimpleLoad() を用いて モデルデータを取得すれば、オブジェクト死亡時、自動的に開放が行われる\n
    　　　 また複数のシェイプで構成されたモデルでも ObjObjectAction3dModelSimpleLoad() をシェイプ数分呼び出せば、自動でリスト化される\n
    　　　 #OBS_OBJECT_WORK 内の3D用のメモリ、（追加分も含む）も自動で取得、開放される。\n
    　　　 また表示も自動でリストになっている分のシェイプが表示される。\n
    \n
    
    　・ 共用VRAM、TexVRAM
    　　　 ObjObjectVramAlloc() を用いて、共用VRAMを取得すれば、自動で誰も参照しなくなった時に開放される\n
    　　　 ObjObjectVramAllocTex() も同上。\n
    \n
    　・ オブジェクト地形データ
    　　　 ObjObjectCollisionDifSet() を用いて、オブジェクト差分地形データを読み込めば、\n
    　　　 自動でオブジェクト内の地形メモリ、を取得、開放を行う、また、地形登録も自動で行われる\n
    　　　 ObjObjectCollisionDirSet() ObjObjectCollisionAtrSet() も同上である。
    \n
    
  @sa objObjectLoad.c objObjectLoad.h objObject.c objObject.h 
    
*/
#ifndef _H_OBJOBJECTLOAD
#define _H_OBJOBJECTLOAD


#include "objObject.h"

#if	defined(__cplusplus)
extern "C" {
#endif


//----- Definitions ----------------------------------------------------

#define OBD_LOAD_INITIAL_DRAW	(1 & (_PC | _IPHONE))

/// データアドレス共有管理ワーク \n Globalに宣言し同じデータを扱うオブジェクトでデータを共有する
typedef struct _OBS_DATA_WORK
{
    void* pData;	///< 管理データアドレス
    u16   num;		///< ロードされて開放されていない数
} OBS_DATA_WORK;

// OBS_DATA_WORK::num
#define OBD_DATA_ARCHIVE_FLAG ( 1 << 15 )		///< アーカイブデータ

// objObjectActionLoadで使用
#define OBD_AUTO_CHARSIZE ( (u16)-1 ) ///< 自動でそのファイルの最大サイズを取得
#define OBD_AUTO_PLTSIZE  ( (u16)-1 ) ///< 自動でそのファイルの最大サイズを取得

//----- Macros ---------------------------------------------------------

//----- Macros Functions -----------------------------------------------

//----- External Declarations ------------------------------------------

#if OBD_LOAD_INITIAL_DRAW
// ==========================================================================
// ObjLoadInitDraw
/*!	カクつきの原因と思われるテクスチャ初回描画を先行して実行
 *
 *	@return	TRUE:描画完了 FALSE:描画中
 *
 *	@note	ObjLoad***を用いてロードされたオブジェクト/エフェクトをそのまま描画します。
 *			ObjLoadSetInitDrawFlagにて本関数での描画に登録するか否かを設定できます。
 *			登録した場合はObjLoadClearDrawにてクリアしない限り生き続けるます。
 *			オブジェクトを開放しても登録され続けるので使い方に注意してください。
 */
// ==========================================================================
extern BOOL ObjLoadInitDraw(void);

// ==========================================================================
// ObjLoadClearDraw
/*!	描画命令をクリア
 */
// ==========================================================================
extern void ObjLoadClearDraw(void);

// ==========================================================================
// ObjLoadSetInitDrawFlag
/*!
 *	InitialDrawへの登録を行うか否かのフラグ
 *
 *	@param flag [in] TRUE:登録する FALSE:登録しない
 *
 *	@note flagがTRUEの場合はデータロード時に登録を行います。
 *		ObjInitを読んだ際に flag = FALSEで呼び出されます。
 *		このフラグの初期値はFALSEです。
 *		
 *		この関数が呼び出された場合、強制的にObjLoadClearDrawを呼びます。
 */
// ==========================================================================
extern void ObjLoadSetInitDrawFlag(BOOL flag);
#endif // OBD_LOAD_INITIAL_DRAW

// =====================================================================
// バインドファイル読み込み
// =====================================================================
// ==========================================================================
// ObjDataLoadAmbIndex
/*!	バインドファイルからのデータ読み込み

	@param	data_work	[io]	データワーク
	@param	index		[in]	取得するデータインデックス
	@param	amb			[in]	AMBデータヘッダ

	@return	データアドレス
 */
// ==========================================================================
extern void* ObjDataLoadAmbIndex(OBS_DATA_WORK *data_work, s32 index, void *amb);

// ================================================================
// アーカイブ ファイル読み込み
// ================================================================
#if defined _DS
// ================================================================
// ObjDataNarcToFile
/*!
  ファイルパスを使ってアーカイブポインタからファイルポインタ取得
 
  @param pPath    [in] 読み込むファイルパス
  @param pArchive [in] アーカイブポインタ
 
 */
// ================================================================
void* ObjDataNarcToFile( const char* pPath, void* pArchive );
#endif // #if defined _DS

#if defined _DS
// ================================================================
// ObjNarcGetFile
/*!
  NARCファイルから指定したファイルを取り出します。

  @param pData    [io] 設定するデータアドレス
  @param pPath    [in] ファイルパス
  @param pArchive [in] アーカイブアドレス
    
 */
// ================================================================
void ObjNarcGetFile( OBS_DATA_WORK* pData, char * pPath,void * pArchive );
#endif // #if defined _DS

// ================================================================
// データワーク
// ================================================================
// ================================================================
// ObjDataSet
/*!
  使用データを指定管理ワークに使用設定
 
  @param pWork   [io] データ管理ワークポインタ（Globalなもの）
  @param pArchive [in] データポインタ
 
 */
// ================================================================
void* ObjDataSet( OBS_DATA_WORK* pWork, void * pData );

// ================================================================
// ObjDataGetInc
/*!
  データワークからカウント付きでデータを取得
 
  @param pWork   [io] データ管理ワークポインタ（Globalなもの）
 
 */
// ================================================================
void* ObjDataGetInc( OBS_DATA_WORK* pWork );

// ================================================================
// ObjDataLoad
/*!
  使用数チェックありデータ読み込み
 
  @param data_work	[io] データ管理ワークポインタ
  @param filename   [in] 読み込むファイルパス
  @param archive	[in] アーカイブポインタ
 
 */
// ================================================================
void* ObjDataLoad(OBS_DATA_WORK *data_work, const char*filename, void *archive);


#if defined _DS
// ================================================================
// ObjDataLoadBB
/*!
	使用数チェックありデータ読み込み BinBine
 
	@param data_work	[io] データ管理ワークポインタ（Globalなもの）
	@param path   [in] BBファイルパス
	@param index	 [in] インデックス

	@return	取得データバッファ
 */
// ================================================================
extern void* ObjDataLoadBB(OBS_DATA_WORK* data_work, const char *path, u16 index);
#endif	// #if defined _DS

// ================================================================
// ObjDataRelease
/*!
  使用数チェックありデータ解放
 
  @param pWork   [io] データ管理ワークポインタ
 */
// ================================================================
void ObjDataRelease( OBS_DATA_WORK* pWork );

#if defined _DS
// ================================================================
// モデルデータ テクスチャ分離
// ================================================================
// ================================================================
// ObjModLoadTexSetUpAndRelease
/*!
  モデルデータからテクスチャをセットアップし、メモリ上のテクスチャを解放します。(VRAM上のテクスチャは残る)

  @param pData    [io] データアドレス
  @param pPath    [in] ファイルパス
    
 */
// ================================================================
void ObjModLoadTexSetUpAndRelease( OBS_DATA_WORK* pData, char * pPath );
#endif	// #if defined _DS


// ================================================================
// VRAM
// ================================================================
#if defined _DS
// ================================================================
// ObjVramAlloc
/*!
  共用VRAM取得
 
  @param pWork   [io] データ管理ワークポインタ（Globalなもの）
  @param ge_type [in] 対象グラフィックスエンジンのタイプ MTE_GE2_TYPE列挙型
  @param num_cha [in] 確保するキャラクタ数

  @Return 取得したVRAMのアドレス

  @note
    この関数で取得したメモリは自分で解放する事\n
    objObjectVramAllocを通した場合はオブジェクトで管理され明示的に解放する必要はありません 05.03.22
 */
// ================================================================
u32 ObjVramAlloc( OBS_DATA_WORK* pWork, MTE_GE2_TYPE ge_type, u32 num_cha );

// ================================================================
// ObjVramRelease
/*!
  使用数チェックありVRAM解放
 
  @param pWork   [io] データ管理ワークポインタ
  @param ge_type [in] 対象グラフィックスエンジンのタイプ MTE_GE2_TYPE列挙型

 */
// ================================================================
void ObjVramRelease( OBS_DATA_WORK* pWork, MTE_GE2_TYPE ge_type );

// ================================================================
// ObjVramAllocTex
/*!
  共用テクスチャVRAM取得
 
  @param pWork   [io] データ管理ワークポインタ（Globalなもの）
  @param num_cha [in] 確保するキャラクタ数

  @Return 取得したVRAMのアドレス

  @note
    この関数で取得したメモリは自分で解放する事\n
    objObjectVramAllocTexを通した場合はオブジェクトで管理され明示的に解放する必要はありません 05.03.22
 */
// ================================================================
u32 ObjVramAllocTex( OBS_DATA_WORK* pWork, u32 num_cha );

// ================================================================
// ObjVramAllocTexPlt
/*!
  共用テクスチャパレット取得
 
  @param pWork   [io] データ管理ワークポインタ（Globalなもの）
  @param num_cha [in] 確保するパレット数

  @Return 取得したVRAMのアドレス

  @note
    この関数で取得したメモリは自分で解放する事\n
    objObjectVramAllocTexを通した場合はオブジェクトで管理され明示的に解放する必要はありません 05.03.22
 */
// ================================================================
u32 ObjVramAllocTexPlt( OBS_DATA_WORK* pWork, u16 num_plt );

// ================================================================
// ObjVramReleaseTex
/*!
  使用数チェックありテクスチャVRAM解放
 
  @param pWork   [io] データ管理ワークポインタ

 */
// ================================================================
extern void ObjVramReleaseTex(OBS_DATA_WORK* pWork);

// ================================================================
// ObjVramReleaseTexPlt
/*!
  使用数チェックありテクスチャパレット解放
 
  @param pWork   [io] データ管理ワークポインタ

 */
// ================================================================
extern void ObjVramReleaseTexPlt(OBS_DATA_WORK* pWork);

// ================================================================
// アクションVRAM
// ================================================================
// ================================================================
// ObjActVramAllocDS
/*!
  アクション用共用VRAMアドレス取得

  @param pAct       [in] アクションポインタ
  @param usCharSize [in] キャラサイズ
  @param pData      [in] アドレス管理ワークポインタ二つ分

  @note この関数で取得したメモリは自分でobjActVramReleaseを呼んで解放する事
 */
// ================================================================
void ObjActVramAllocDS( MTS_ACTION_DS *pAct, u16 usCharSize, OBS_DATA_WORK* pData );

// ================================================================
// ObjActVramReleaseDS
/*!
  アクション用共用VRAMアドレス取得

  @param pData [in] アドレス管理ワークポインタ二つ分

 */
// ================================================================
void ObjActVramReleaseDS( OBS_DATA_WORK* pData );

// ================================================================
// オブジェクトVRAM
// ================================================================
// ================================================================
// ObjObjectVramAlloc
/*!
  オブジェクト用共用VRAMアドレス取得

  @param pWork [in] オブジェワークポインタ
  @param usCharSize [in] キャラサイズ
  @param pData [in] VRAM 管理ワーク先頭ポインタ（二つ分の領域を使用します）

 */
// ================================================================
void ObjObjectVramAlloc( OBS_OBJECT_WORK *pWork, u16 usCharSize, OBS_DATA_WORK* pDataA);

// ================================================================
// ObjObjectVramAllocTex
/*!
  共用テクスチャVRAM取得

  @param pWork		[io] オブジェクトワーク
  @param num_cha	[in] 確保するキャラクタ数
  @param num_plt	[in] 確保するパレット数
  @param pData		[io] データ管理ワークポイン二つ分（Globalなもの）(二つは連続している事)

  @note
	OBS_ACTION3D_SPRITE_WORK, OBS_ACTION3D_SS_WORK, OBS_ACTION3D_SMA_WORK の優先で\n
	VRAMを取得します。\n
	優先の高いワークが存在する場合は、優先度が下位のワークへの\n
	VRAM取得は行いません。
 */
// ================================================s================
void ObjObjectVramAllocTex( OBS_OBJECT_WORK *pWork, u32 num_cha, u16 num_plt, OBS_DATA_WORK* pData );

// ================================================================
// ObjObjectVramAllocOnlyTex
/*!
  共用テクスチャVRAM取得
 
  @param pWork		[io] オブジェクトワーク
  @param num_cha	[in] 確保するキャラクタ数
  @param pData		[io] データ管理ワークポイント（Globalなもの）

  @note
	OBS_ACTION3D_SPRITE_WORK, OBS_ACTION3D_SS_WORK, OBS_ACTION3D_SMA_WORK の優先で\n
	VRAMを取得します。\n
	優先の高いワークが存在する場合は、優先度が下位のワークへの\n
	VRAM取得は行いません。
 */
// ================================================================
extern void ObjObjectVramAllocOnlyTex( OBS_OBJECT_WORK *pWork, u32 num_cha, OBS_DATA_WORK* pData );

// ================================================================
// ObjObjectVramAllocOnlyTexPlt
/*!
  共用テクスチャパレットVRAM取得
 
  @param pWork		[io] オブジェクトワーク
  @param num_plt	[in] 確保するパレット数
  @param pData		[io] データ管理ワークポイント（Globalなもの）

  @note
	OBS_ACTION3D_SPRITE_WORK, OBS_ACTION3D_SS_WORK, OBS_ACTION3D_SMA_WORK の優先で\n
	VRAMを取得します。\n
	優先の高いワークが存在する場合は、優先度が下位のワークへの\n
	VRAM取得は行いません。
 */
// ================================================================
extern void ObjObjectVramAllocOnlyTexPlt( OBS_OBJECT_WORK *pWork, u16 num_plt, OBS_DATA_WORK* pData );

// ================================================================
// ObjObjectTransTexPlt
/*!
  テクスチャパレット転送
 
  @param pWork		[io] オブジェクトワーク
  @param bac		[io] bac データ
  @param act_id		[io] アクションID

  @note
	OBS_OBJECT_WORKの保持するアクションのパレット領域に、指定のアクションのパレットを転送します。
	OBS_ACTION3D_SPRITE_WORK, OBS_ACTION3D_SS_WORK, OBS_ACTION3D_SMA_WORK(未対応) の優先で\n
	転送設定を行います。\n
	優先の高いワークが存在する場合は、優先度が下位のワークへの\n
	転送設定は行いません。\n
	パレット領域をあらかじめ取得しておく必要があります。
 */
// ================================================================
extern void ObjObjectTransTexPlt(OBS_OBJECT_WORK *pWork, void *bac, u16 act_id);

// ================================================================
// ObjObjectVramRelease
/*!
  オブジェクトVRAM解放

  @param pWork [in] オブジェクトワークポインタ
 */
// ================================================================
void ObjObjectVramRelease( OBS_OBJECT_WORK * pWork );
#endif // #if defined _DS

// ================================================================
// パレットデータ
// ================================================================
#if defined _DS
// ================================================================
// ObjObjectPaletteLoad
/*!
  アクションからパレットデータ読み込み（ファイル読み込み済みである事）

  @param pWork [in] オブジェワークポインタ
  @param sAnimeID [in] アクションID
  @param sPltID   [in] パレットID （objPalette.h参照

 */
// ================================================================
void ObjObjectPaletteLoad( OBS_OBJECT_WORK *pWork, u16 usActionID, s16 sPltID );

// ================================================================
// ObjObjectPaletteRelease
/*!
  パレット解放

  @param pWork [in] オブジェワークポインタ
 */
// ================================================================
void ObjObjectPaletteRelease( OBS_OBJECT_WORK *pWork );
#endif // #if defined _DS

#if defined _DS
// ================================================================
// アクションデータロード
// ================================================================
// ================================================================
// ObjActLoad
/*!
  アクションデータ読み込み

  @param pAct  [in] アクションポインタ MTD_ACT_DS_FLAG_DISABLE_GE_AorBによって取得しないVRAMを設定する
  @param pPath [in] BACファイルパス
  @param usCharSize [in] キャラサイズ
  @param pData [in] アクションデータ管理ワークポインタ（NULLの場合、同じオブジェクト作成ごとにリソースを消費します）
  @param pArchive [in] アーカイブポインタ (NULLの場合、ROMからロードを行います、従ってワンカートリッジ不可)

  @note
    この関数で取得したメモリは自分で解放する事 05.03.01
    pAct->flagはあらかじめ初期化しておく事
 */
// ================================================================
void ObjActLoad( MTS_ACTION_DS *pAct, const char* pPath, u16 usCharSize, OBS_DATA_WORK* pData, void * pArchive);

// ================================================================
// ObjActRelease
/*!
  アクション用共用VRAMアドレス取得

  @param pData [in] アドレス管理ワークポインタ二つ分
  @param pAct  [in] アクションポインタ

 */
// ================================================================
void ObjActRelease( OBS_DATA_WORK* pData, MTS_ACTION_DS *pAct );
#endif

// ================================================================
// オブジェクトアクションデータロード
// ================================================================
#if OBD_USE_ACTION3D_NN
// ================================================================
// ObjAction3dNNModelLoad
/*!
	3Dモデルデータ読み込み

	@param	obj_3d			[in]	3Dオブジェクトワークポインタ
	@param	data_work		[in]	3Dモデルデータワーク
	@param	filename		[in]	3Dモデルデータファイル名
	@param	index			[in]	3DモデルデータAMBインデックス
	@param	archive			[in]	モデルデータを含むアーカイブ
	@param	filename_tex	[in]	テクスチャファイル名
	@param	amb_tex			[in]	テクスチャAMB
	@param	drawflag		[in]	描画フラグ

	@note
		filename が有効な場合は filename、無効の場合はindexでオブジェクトファイルをambから取得します。\n
		amb_tex が無効の場合は、filename_texの読み込みが終了するまで処理をロックします。\n
		filename_tex, amb_tex のどちらかが必ず有効であるようにして下さい。
 */
// ================================================================
extern void ObjAction3dNNModelLoad(OBS_ACTION3D_NN_WORK *obj_3d,
									OBS_DATA_WORK *data_work, const char *filename, s32 index, void *archive,
									const char *filename_tex, void *amb_tex, NNF_DRAWOBJ drawflag);

// ================================================================
// ObjCopyAction3dNNModel
/*!
	3Dモデルデータコピー

	@param	src_obj_3d		[in]	コピー元3Dオブジェクトワークポインタ
	@param	dest_obj_3d		[in]	コピー先3Dオブジェクトワークポインタ (NULL可)

	@note
		ObjAction3dNNModelLoad等でロード済みの3Dモデルのコピーをobj_workにセットします。\n
		この関数でコピーした3Dオブジェクトは、オブジェクト破棄時に開放しないようにして下さい。
 */
// ================================================================
extern void ObjCopyAction3dNNModel(OBS_ACTION3D_NN_WORK *src_obj_3d, OBS_ACTION3D_NN_WORK *dest_obj_3d);

// ================================================================
// ObjObjectCopyAction3dNNModel
/*!
	3Dモデルデータコピー

	@param	obj_work		[in]	オブジェクトワークポインタ
	@param	src_obj_3d		[in]	コピー元3Dオブジェクトワークポインタ
	@param	dest_obj_3d		[in]	コピー先3Dオブジェクトワークポインタ (NULL可)

	@note
		ObjAction3dNNModelLoad等でロード済みの3Dモデルのコピーをobj_workにセットします。\n
		この関数でコピーした3Dオブジェクトは、オブジェクト破棄時に開放されません。
 */
// ================================================================
extern void ObjObjectCopyAction3dNNModel(OBS_OBJECT_WORK *obj_work, OBS_ACTION3D_NN_WORK *src_obj_3d, OBS_ACTION3D_NN_WORK *dest_obj_3d);

// ================================================================
// ObjObjectAction3dNNModelReleaseCopy
/*!
	3Dモデル データコピーしたモデルを解放

	@param	obj_work		[in]	オブジェクトワークポインタ
 */
// ================================================================
extern void ObjObjectAction3dNNModelReleaseCopy(OBS_OBJECT_WORK *obj_work);

// ================================================================
// ObjObjectAction3dNNModelLoad
/*!
	3Dモデルデータ読み込み

	@param	obj_work		[in]	オブジェクトワークポインタ
	@param	obj_3d			[in]	3Dオブジェクトワークポインタ
									NULLの場合、オブジェワークに既に登録されているか確認し、無ければ動的に取得します。
									(ObjectExitで解放されます)

	@param	data_work		[in]	3Dモデルデータワーク
	@param	filename		[in]	3Dモデルデータファイル名
	@param	index			[in]	3DモデルデータAMBインデックス
	@param	archive			[in]	モデルデータを含むアーカイブ
	@param	filename_tex	[in]	テクスチャファイル名
	@param	amb_tex			[in]	テクスチャAMB
	@param	drawflag		[in]	描画フラグ

	@note
		filename が有効な場合は filename、無効の場合はindexでオブジェクトファイルをambから取得します。\n
		amb_tex が無効の場合は、filename_texの読み込みが終了するまで処理をロックします。\n
		filename_tex, amb_tex のどちらかが必ず有効であるようにして下さい。
 */
// ================================================================
extern void ObjObjectAction3dNNModelLoad(OBS_OBJECT_WORK *obj_work, OBS_ACTION3D_NN_WORK *obj_3d,
									OBS_DATA_WORK *data_work, const char *filename, s32 index, void *archive,
									const char *filename_tex, void *amb_tex, NNF_DRAWOBJ drawflag);

// ================================================================
// ObjAction3dNNModelLoadTxb
/*!
	3Dモデルデータ読み込み TXB使用

	@param	obj_3d			[in]	3Dオブジェクトワークポインタ
	@param	data_work		[in]	3Dモデルデータワーク
	@param	filename		[in]	3Dモデルデータファイル名
	@param	index			[in]	3DモデルデータAMBインデックス
	@param	archive			[in]	モデルデータを含むアーカイブ
	@param	filename_tex	[in]	テクスチャファイル名
	@param	amb_tex			[in]	テクスチャAMB
	@param	drawflag		[in]	描画フラグ
	@param	txb				[in]	TXBデータ

	@note
		filename が有効な場合は filename、無効の場合はindexでオブジェクトファイルをambから取得します。\n
		amb_tex が無効の場合は、filename_texの読み込みが終了するまで処理をロックします。\n
		filename_tex, amb_tex のどちらかが必ず有効であるようにして下さい。
 */
// ================================================================
extern void ObjAction3dNNModelLoadTxb(OBS_ACTION3D_NN_WORK *obj_3d,
									OBS_DATA_WORK *data_work, const char *filename, s32 index, void *archive,
									const char *filename_tex, void *amb_tex, NNF_DRAWOBJ drawflag,
									void *txb);


// ================================================================
// ObjObjectAction3dNNModelLoadTxb
/*!
	3Dモデルデータ読み込み TXB使用

	@param	obj_work		[in]	オブジェクトワークポインタ
	@param	obj_3d			[in]	3Dオブジェクトワークポインタ
									NULLの場合、オブジェワークに既に登録されているか確認し、無ければ動的に取得します。
									(ObjectExitで解放されます)

	@param	data_work		[in]	3Dモデルデータワーク
	@param	filename		[in]	3Dモデルデータファイル名
	@param	index			[in]	3DモデルデータAMBインデックス
	@param	archive			[in]	モデルデータを含むアーカイブ
	@param	filename_tex	[in]	テクスチャファイル名
	@param	amb_tex			[in]	テクスチャAMB
	@param	drawflag		[in]	描画フラグ
	@param	txb				[in]	TXBファイル

	@note
		filename が有効な場合は filename、無効の場合はindexでオブジェクトファイルをambから取得します。\n
		amb_tex が無効の場合は、filename_texの読み込みが終了するまで処理をロックします。\n
		filename_tex, amb_tex のどちらかが必ず有効であるようにして下さい。
 */
// ================================================================
extern void ObjObjectAction3dNNModelLoadTxb(OBS_OBJECT_WORK *obj_work, OBS_ACTION3D_NN_WORK *obj_3d,
									OBS_DATA_WORK *data_work, const char *filename, s32 index, void *archive,
									const char *filename_tex, void *amb_tex, NNF_DRAWOBJ drawflag, void *txb);

// ================================================================
// ObjAction3dNNMotionLoad
/*!
	3Dモーションデータ読み込み

	@param	obj_3d			[in]	3Dオブジェクトワークポインタ
	@param	reg_file_id		[in]	登録モーションファイルID
	@param	marge			[in]	並列補間使用の有無 TRUE : 使用
	@param	data_work		[in]	3Dモーションデータワーク
	@param	filename		[in]	3Dモーションデータファイル名
	@param	index			[in]	3DモーションデータAMBインデックス
	@param	archive			[in]	モーションデータを含むアーカイブ
	@param	motion_num		[in]	使用モーション数 ディフォルト AMD_MOTION_DEFAULT_MAX
	@param	mmotion_num		[in]	使用マテリアルモーション数 ディフォルト AMD_MOTION_MATERIAL_DEFAULT_MAX

	@note
		filename が有効な場合は filename、無効の場合はindexでオブジェクトファイルをambから取得します。\n
		amb_tex が無効の場合は、filename_texの読み込みが終了するまで処理をロックします。\n
		filename_tex, amb_tex のどちらかが必ず有効であるようにして下さい。
 */
// ================================================================
extern void ObjAction3dNNMotionLoad(OBS_ACTION3D_NN_WORK *obj_3d, s32 reg_file_id, BOOL marge,
									OBS_DATA_WORK *data_work, const char *filename, s32 index, void *archive,
									s32 motion_num=AMD_MOTION_DEFAULT_MAX, s32 mmotion_num=AMD_MOTION_MATERIAL_DEFAULT_MAX);

// ================================================================
// ObjObjectAction3dNNMotionLoad
/*!
	3Dモーションデータ読み込み

	@param	obj_work		[in]	オブジェクトワークポインタ
	@param	reg_file_id		[in]	登録モーションファイルID
	@param	marge			[in]	並列補間使用の有無 TRUE : 使用
	@param	data_work		[in]	3Dモーションデータワーク
	@param	filename		[in]	3Dモーションデータファイル名
	@param	index			[in]	3DモーションデータAMBインデックス
	@param	amb				[in]	モーションデータを含むAMB
	@param	motion_num		[in]	使用モーション数 ディフォルト AMD_MOTION_DEFAULT_MAX
	@param	mmotion_num		[in]	使用マテリアルモーション数 ディフォルト AMD_MOTION_MATERIAL_DEFAULT_MAX

	@note
		filename が有効な場合は filename、無効の場合はindexでオブジェクトファイルをambから取得します。\n
		amb_tex が無効の場合は、filename_texの読み込みが終了するまで処理をロックします。\n
		filename_tex, amb_tex のどちらかが必ず有効であるようにして下さい。
 */
// ================================================================
extern void ObjObjectAction3dNNMotionLoad(OBS_OBJECT_WORK *obj_work, s32 reg_file_id, BOOL marge,
									OBS_DATA_WORK *data_work, const char *filename, s32 index, void *archive,
									s32 motion_num=AMD_MOTION_DEFAULT_MAX, s32 mmotion_num=AMD_MOTION_MATERIAL_DEFAULT_MAX);

// ================================================================
// ObjAction3dNNMaterialMotionLoad
/*!
	3Dマテリアルモーションデータ読み込み

	@param	obj_3d			[in]	3Dオブジェクトワークポインタ
	@param	reg_file_id		[in]	登録モーションファイルID
	@param	data_work		[in]	3Dモーションデータワーク
	@param	filename		[in]	3Dモーションデータファイル名
	@param	index			[in]	3DモーションデータAMBインデックス
	@param	archive			[in]	モーションデータを含むアーカイブ
	@param	motion_num		[in]	使用モーション数 ディフォルト AMD_MOTION_DEFAULT_MAX
	@param	mmotion_num		[in]	使用マテリアルモーション数 ディフォルト AMD_MOTION_MATERIAL_DEFAULT_MAX

	@note
		filename, archive, data_work の順でデータを取得します。
 */
// ================================================================
extern void ObjAction3dNNMaterialMotionLoad(OBS_ACTION3D_NN_WORK *obj_3d, s32 reg_file_id,
									OBS_DATA_WORK *data_work, const char *filename, s32 index, void *archive,
									s32 motion_num=AMD_MOTION_DEFAULT_MAX, s32 mmotion_num=AMD_MOTION_MATERIAL_DEFAULT_MAX);

// ================================================================
// ObjObjectAction3dNNMaterialMotionLoad
/*!
	3Dマテリアルモーションデータ読み込み

	@param	obj_work		[in]	オブジェクトワークポインタ
	@param	reg_file_id		[in]	登録モーションファイルID
	@param	data_work		[in]	3Dモーションデータワーク
	@param	filename		[in]	3Dモーションデータファイル名
	@param	index			[in]	3DモーションデータAMBインデックス
	@param	archive			[in]	モーションデータを含むアーカイブ
	@param	motion_num		[in]	使用モーション数 ディフォルト AMD_MOTION_DEFAULT_MAX
	@param	mmotion_num		[in]	使用マテリアルモーション数 ディフォルト AMD_MOTION_MATERIAL_DEFAULT_MAX

	@note
		filename, archive, data_work の順でデータを取得します。
 */
// ================================================================
extern void ObjObjectAction3dNNMaterialMotionLoad(OBS_OBJECT_WORK *obj_work, s32 reg_file_id,
									OBS_DATA_WORK *data_work, const char *filename, s32 index, void *archive,
									s32 motion_num=AMD_MOTION_DEFAULT_MAX, s32 mmotion_num=AMD_MOTION_MATERIAL_DEFAULT_MAX);

// ================================================================
// ObjAction3dNNModelLoadCheck
/*!
	3Dモデルデータ読み込み終了チェック

	@param	obj_3d			[in]	3Dオブジェクトワークポインタ

	@return	TRUE : 読み込み終了

	@note
		データ読み込みが終了している場合は、関連フラグの設定を行います。
 */
// ================================================================
extern BOOL ObjAction3dNNModelLoadCheck(OBS_ACTION3D_NN_WORK *obj_3d);

// ================================================================
// ObjAction3dNNModelRelease
/*!
	3Dモデルデータ開放

	@param	obj_3d			[in]	3Dオブジェクトワークポインタ

	@note
		モデルバッファとテクスチャの解放を行います\n
		ObjAction3dNNModelReleaseCheck で開放終了をチェックしてください。
 */
// ================================================================
extern void ObjAction3dNNModelRelease(OBS_ACTION3D_NN_WORK *obj_3d);

// ================================================================
// ObjAction3dNNModelReleaseCheck
/*!
	3Dモデルデータ開放終了チェック

	@param	obj_3d			[in]	3Dオブジェクトワークポインタ

	@return	TRUE : 開放終了

	@note
		オブジェクトの開放終了をチェックし、objectを保持している場合は\n
		メモリから開放します。
 */
// ================================================================
extern BOOL ObjAction3dNNModelReleaseCheck(OBS_ACTION3D_NN_WORK *obj_3d);

// ================================================================
// ObjAction3dNNMotionRelease
/*!
	3Dモーションデータ開放

	@param	obj_3d			[in]	3Dオブジェクトワークポインタ
 */
// ================================================================
extern void ObjAction3dNNMotionRelease(OBS_ACTION3D_NN_WORK *obj_3d);

#endif	// #if OBD_USE_ACTION3D_NN


#if OBD_USE_ACTION3D_ES
// ================================================================
// ObjAction3dESEffectLoad
/*!
	ESエフェクトデータ読み込み

	@param	obj_3des		[in]	ESオブジェクトワークポインタ
	@param	data_work		[in]	ESエフェクトデータワーク
	@param	filename		[in]	ESエフェクトデータファイル名
	@param	index			[in]	ESエフェクトデータAMBインデックス
	@param	archive			[in]	ESエフェクトデータを含むアーカイブ
	@param	user_attr		[in]	ユーザー属性(AME_AME_USER_ATTRIBUTE)
	@param	ecb_prio		[in]	ECBプライオリティ

	@note
		filename が有効な場合は filename、無効の場合はindexでAMEファイルをambから取得します。\n
 */
// ================================================================
extern void ObjAction3dESEffectLoad(OBS_ACTION3D_ES_WORK *obj_3des,
									OBS_DATA_WORK *data_work, const char *filename, s32 index, void *archive,
									Sint32 user_attr=0, Sint32 ecb_prio=0);

// ================================================================
// ObjObjectAction3dESEffectLoad
/*!
	ESエフェクトデータ読み込み

	@param	obj_work		[in]	オブジェクトワークポインタ
	@param	obj_3des		[in]	ESオブジェクトワークポインタ
									NULLの場合、オブジェワークに既に登録されているか確認し、無ければ動的に取得します。
									(ObjectExitで解放されます)

	@param	data_work		[in]	ESエフェクトデータワーク
	@param	filename		[in]	ESエフェクトデータファイル名
	@param	index			[in]	ESエフェクトデータAMBインデックス
	@param	archive			[in]	ESエフェクトデータを含むアーカイブ
	@param	user_attr		[in]	ユーザー属性（AME_AME_USER_ATTRIBUTE）
	@param	ecb_prio		[in]	ECBプライオリティ

	@note
		filename が有効な場合は filename、無効の場合はindexでオブジェクトファイルをambから取得します。\n
 */
// ================================================================
extern void ObjObjectAction3dESEffectLoad(OBS_OBJECT_WORK *obj_work, OBS_ACTION3D_ES_WORK *obj_3des,
										  OBS_DATA_WORK *data_work, const char *filename, s32 index, void *archive,
										  Sint32 user_attr=0, Sint32 ecb_prio=0);

// =======================================================================
// ObjAction3dESEffectRelease
/*!
  3D ES エフェクトデータ(ECB)解放
  
  @param obj_3des	[io]	3DESオブジェクトワークポインタ
  
  @note
  	エフェクトデータの解放を行います。
	即時解放なので解放待ちは発生しません。
 */
// =======================================================================
extern void ObjAction3dESEffectRelease(OBS_ACTION3D_ES_WORK *obj_3des);

// ================================================================
// ObjAction3dESTextureLoad
/*!
	ESテクスチャデータ読み込み

	@param	obj_3des		[in]	ESオブジェクトワークポインタ
	@param	data_work		[in]	ESテクスチャAMBデータワーク
	@param	filename		[in]	ESテクスチャAMBデータファイル名(*.amb)
	@param	index			[in]	ESテクスチャAMBデータAMBインデックス
	@param	archive			[in]	ESエフェクトAMBデータを含むアーカイブ
	@param	load_tex		[in]	テクスチャ転送フラグ
									（TRUE：テクスチャ転送行う, FALSE：テクスチャ転送行わない）

	@note
		filename が有効な場合は filename、無効の場合はindexでテクスチャAMBファイルをambから取得します。\n
		ESテクスチャデータとは、テクスチャリスト(TXB)とテクスチャをバインドしたもの(AMB)です。
		archiveは「テクスチャリストとテクスチャをバインドしたamb」そのものではなく、\n
		それをさらにバインドしたものです。\n
 */
// ================================================================
extern void ObjAction3dESTextureLoad(OBS_ACTION3D_ES_WORK *obj_3des,
									 OBS_DATA_WORK *data_work, const char *filename, s32 index, void *archive,
									 BOOL load_tex=FALSE);

// ================================================================
// ObjObjectAction3dESTextureLoad
/*!
	ESテクスチャデータ読み込み

	@param	obj_work		[in]	オブジェクトワークポインタ
	@param	obj_3des		[in]	ESオブジェクトワークポインタ（NULL不可）
	@param	data_work		[in]	ESテクスチャAMBデータワーク
	@param	filename		[in]	ESテクスチャAMBデータファイル名(*.amb)
	@param	index			[in]	ESテクスチャAMBデータAMBインデックス
	@param	archive			[in]	ESテクスチャAMBデータを含むアーカイブ
	@param	load_tex		[in]	テクスチャ転送フラグ
									（TRUE：テクスチャ転送行う, FALSE：テクスチャ転送行わない）
	@note
		filename が有効な場合は filename、無効の場合はindexでテクスチャAMBファイルをambから取得します。\n
		ESテクスチャデータとは、テクスチャリスト(TXB)とテクスチャをバインドしたもの(AMB)です。
		archiveは「テクスチャリストとテクスチャをバインドしたamb」そのものではなく、\n
		それをさらにバインドしたものです。\n
 */
// ================================================================
extern void ObjObjectAction3dESTextureLoad(OBS_OBJECT_WORK *obj_work, OBS_ACTION3D_ES_WORK *obj_3des,
										   OBS_DATA_WORK *data_work, const char *filename, s32 index, void *archive,
										   BOOL load_tex=FALSE);

// =======================================================================
// ObjAction3dESTextureRelease
/*!
  3D ES テクスチャデータ解放
  
  @param obj_3des	[io]	3DESオブジェクトワークポインタ
  
  @note
  	テクスチャの解放を行います。
  ObjAction3dESTextureReleaseCheck() で解放終了をチェックしてください。
 */
// =======================================================================
extern void ObjAction3dESTextureRelease(OBS_ACTION3D_ES_WORK *obj_3des);

// =======================================================================
// ObjAction3dESTextureReleaseCheck
/*!
  3D ES テクスチャデータ解放終了チェック
  
  @param obj_3des	[io]	3DESオブジェクトワークポインタ
 
  @retval TRUE	解放終了
  @retval FALSE	解放中
  
  @note
  	テクスチャの解放終了をチェックし、texlistbufをメモリから解放します。
  	（データワークでtexlistを共有している場合は参照カウントデクリメントを行い、
	必要であればバッファの解放を行います。）
 */
// =======================================================================
extern BOOL ObjAction3dESTextureReleaseCheck(OBS_ACTION3D_ES_WORK *obj_3des);

// =======================================================================
// ObjAction3dESTextureLoadToDwork
/*!
  ESテクスチャデータ読み込み→データワーク格納
  
  @param texlist_dwork	[io]	テクスチャリスト格納先データワーク
  @param amb_tex		[in]	テクスチャAMB（texlist_dworkロード済みならNULL可）
  @param texlist_buf	[out]	テクスチャリストバッファアドレス格納先
  
  @return テクスチャデータロード完了チェック用インデックス
          読み込み待ちが発生しない場合は「-1」を返します。
  
  @note
  テクスチャデータをロードして、テクスチャリストをデータワークにセットします。
  *texilst_buf は texlist_dwork->pData と実体は同じものになります。
  amDrawIsRegistComplete()で完了チェックを行ってください。
  テクスチャをVRAMに常駐させて共有したい場合などに利用してください。
 */
// =======================================================================
extern Sint32 ObjAction3dESTextureLoadToDwork(OBS_DATA_WORK *texlist_dwork, void *amb_tex, void **texlist_buf);

// =======================================================================
// ObjAction3dESTextureReleaseDwork
/*!
  ESテクスチャデータ解放（データワーク使用）
  
  @param texlist_dwork	[io]	テクスチャリストが格納してあるデータワーク
  
  @return テクスチャデータ解放完了チェック用インデックス。
          解放待ちが発生しない場合は「-1」を返します。
  
  @note
  -1以外が返された場合はObjAction3dESTextureReleaseDworkCheck()で
  完了チェックを行ってください。
 */
// =======================================================================
extern Sint32 ObjAction3dESTextureReleaseDwork(OBS_DATA_WORK *texlist_dwork);

// =======================================================================
// ObjAction3dESTextureReleaseDworkCheck
/*!
  ESテクスチャデータ解放チェック（データワーク使用）
 
  @param texlist_dwork	[io]	テクスチャリストが格納してあるデータワーク
  @param reg_index		[in]	テクスチャデータ解放完了チェック用インデックス
 
  @retval TRUE	解放完了
  @retval FALSE	解放中
  
  @note
  解放完了時にバッファ解放を行っているため、
  TRUEが返るまで毎フレーム呼び続けてください。
 */
// =======================================================================
extern BOOL ObjAction3dESTextureReleaseDworkCheck(OBS_DATA_WORK *texlist_dwork, Sint32 reg_index);

// =======================================================================
// ObjObjectAction3dESTextureSetByDwork
/*!
  登録済みESテクスチャセット
 
  @param obj_work		[io]	オブジェクトワーク
  @param texlist_dwork	[io]	テクスチャリストデータワーク
 
  @note
  データワークで管理されているテクスチャリストをオブジェクトに設定します。
  事前にObjObjectAction3dESTextureLoad(..., load_tex=FALSE)を呼び出しておいてください。
  オブジェクトが破棄されるときに自動的に解放処理が行われます。
 */
// =======================================================================
extern void ObjObjectAction3dESTextureSetByDwork(OBS_OBJECT_WORK *obj_work, OBS_DATA_WORK *texlist_dwork);

// ================================================================
// ObjAction3dESModelLoad
/*!
	ESモデルデータ読み込み

	@param	obj_3des		[in]	ESオブジェクトワークポインタ
	@param	data_work		[in]	ESモデルデータワーク
	@param	filename		[in]	ESモデルデータファイル名(*.?no)
	@param	index			[in]	ESモデルデータAMBインデックス
	@param	archive			[in]	ESモデルデータを含むアーカイブ
	@param	drawflag		[in]	描画フラグ
	@param	load_model		[in]	モデル転送フラグ
									（TRUE：モデル転送行う, FALSE：モデル転送行わない）

	@note
		filename が有効な場合は filename、無効の場合はindexでオブジェクトファイルをambから取得します。\n
 */
// ================================================================
extern void ObjAction3dESModelLoad(OBS_ACTION3D_ES_WORK *obj_3des,
								   OBS_DATA_WORK *data_work, const char *filename, s32 index, void *archive, NNF_DRAWOBJ drawflag,
								   BOOL load_model=FALSE);

// ================================================================
// ObjObjectAction3dESModelLoad
/*!
	ESモデルデータ読み込み

	@param	obj_work		[in]	オブジェクトワークポインタ
	@param	obj_3des		[in]	ESオブジェクトワークポインタ（NULL不可）
	@param	data_work		[in]	ESモデルデータワーク
	@param	filename		[in]	ESモデルデータファイル名(*.amb)
	@param	index			[in]	ESモデルデータAMBインデックス
	@param	archive			[in]	ESモデルデータを含むアーカイブ
	@param	drawflag		[in]	描画フラグ
	@param	load_model		[in]	モデル転送フラグ
									（TRUE：モデル転送行う, FALSE：モデル転送行わない）

	@note
		filename が有効な場合は filename、無効の場合はindexでテクスチャAMBファイルをambから取得します。\n
 */
// ================================================================
extern void ObjObjectAction3dESModelLoad(OBS_OBJECT_WORK *obj_work, OBS_ACTION3D_ES_WORK *obj_3des,
										 OBS_DATA_WORK *data_work, const char *filename, s32 index, void *archive, NNF_DRAWOBJ drawflag,
										 BOOL load_model=FALSE);

// =======================================================================
// ObjAction3dESModelRelease
/*!
  3D ES モデルデータ解放
  
  @param obj_3des	[io]	3DESオブジェクトワークポインタ
  
  @note
  	モデルの解放を行います。
  	ObjAction3dESModelReleaseCheck() で解放終了をチェックしてください。
 */
// =======================================================================
extern void ObjAction3dESModelRelease(OBS_ACTION3D_ES_WORK *obj_3des);

// =======================================================================
// ObjAction3dESModelReleaseCheck
/*!
  3D ES モデルデータ解放終了チェック
  
  @param obj_3des	[io]	3DESオブジェクトワークポインタ
 
  @retval TRUE	解放終了
  @retval FALSE	解放中
  
  @note
  	モデルの解放終了をチェックし、objectをメモリから解放します。
  	（データワークでobjectを共有している場合は参照カウントデクリメントを行い、
	必要であればバッファの解放を行います。）
 */
// =======================================================================
extern BOOL ObjAction3dESModelReleaseCheck(OBS_ACTION3D_ES_WORK *obj_3des);

// =======================================================================
// ObjAction3dESModelLoadToDwork
/*!
  ESモデルデータ読み込み→データワーク格納
  
  @param object_dwork	[io]	オブジェクト格納先データワーク
  @param model			[in]	モデルデータ（object_dworkロード済みならNULL可）
  @param drawflag		[in]	描画フラグ
  
  @return モデルデータ読み込み完了チェック用インデックス
          読み込み待ちが発生しない場合は「-1」を返します。
  
  @note
  モデルデータをロードして、NNオブジェクトをデータワークにセットします。
  モデルをVRAMに常駐させて共有したい場合などに利用してください。
  -1以外が返された場合はamDrawIsRegistComplete()で完了チェックを行ってください。
 */
// =======================================================================
extern Sint32 ObjAction3dESModelLoadToDwork(OBS_DATA_WORK *object_dwork, void *model, NNF_DRAWOBJ drawflag);

// =======================================================================
// ObjAction3dESModelReleaseDwork
/*!
  ESモデルデータ解放（データワーク使用）
  
  @param object_dwork	[io]	オブジェクトが格納してあるデータワーク
  
  @return モデルデータ解放完了チェック用インデックス。
          解放待ちが発生しない場合は「-1」を返します。
  
  @note
  -1以外が返された場合はObjAction3dESModelReleaseDworkCheck()で
  完了チェックを行ってください。
 */
// =======================================================================
extern Sint32 ObjAction3dESModelReleaseDwork(OBS_DATA_WORK *object_dwork);

// =======================================================================
// ObjAction3dESModelReleaseDworkCheck
/*!
  ESモデルデータ解放チェック（データワーク使用）
 
  @param object_dwork	[io]	オブジェクトが格納してあるデータワーク
  @param reg_index		[in]	モデルデータ解放完了チェック用インデックス
 
  @retval TRUE	解放完了
  @retval FALSE	解放中
  
  @note
  解放完了時にバッファ解放を行っているため、
  TRUEが返るまで毎フレーム呼び続けてください。
 */
// =======================================================================
extern BOOL ObjAction3dESModelReleaseDworkCheck(OBS_DATA_WORK *object_dwork, Sint32 reg_index);

// =======================================================================
// ObjObjectAction3dESModelSetByDwork
/*!
  登録済みESモデルセット
 
  @param obj_work		[io]	オブジェクトワーク
  @param object_dwork	[io]	NNオブジェクトデータワーク
 
  @note
  データワークで管理されているNNオブジェクトをオブジェクト3Dワークに設定します。
  事前にObjObjectAction3dESModelLoad(..., load_tex=FALSE)を呼び出しておいてください。
  オブジェクトが破棄されるときに自動的に解放処理が行われます。
 */
// =======================================================================
extern void ObjObjectAction3dESModelSetByDwork(OBS_OBJECT_WORK *obj_work, OBS_DATA_WORK *object_dwork);

// ================================================================
// ObjAction3dESEffectLoadCheck
/*!
	エフェクトデータ読み込み終了チェック

	@param	obj_3des		[in]	ESオブジェクトワークポインタ

	@return	TRUE : 読み込み終了

	@note
		データ読み込みが終了している場合は、関連フラグの設定を行います。
 */
// ================================================================
extern BOOL ObjAction3dESEffectLoadCheck(OBS_ACTION3D_ES_WORK *obj_3des);

#endif // #if OBD_USE_ACTION3D_ES


// ================================================================
// オブジェクト2Dアクション(ACE)データロード
// ================================================================
#if OBD_USE_ACTION2D_AMA
// ================================================================
// ObjAction2dAMALoad
/*!
	2Dアクションデータ読み込み

	@param	obj_2d			[in]	2DAMAオブジェクトワークポインタ
	@param	data_work		[in]	2DAMAデータワーク
	@param	filename		[in]	2DAMAデータファイル名
	@param	index			[in]	2DAMAデータAMBインデックス
	@param	archive			[in]	AMAデータを含むアーカイブ
	@param	texlist			[in]	テクスチャリスト
	@param	id				[in]	初期化するアクションorノードID
	@param	type_node		[in]	アクション生成タイプ TRUE : ノードタイプ  FALSE : アクションタイプ

	@note
 */
// ================================================================
extern void ObjAction2dAMALoad(OBS_ACTION2D_AMA_WORK *obj_2d,
									OBS_DATA_WORK *data_work, const char *filename, s32 index, void *archive,
									void *amb_tex, u32 id, BOOL type_node);

// ================================================================
// ObjObjectAction2dAMALoad
/*!
	2Dアクションデータ読み込み

	@param	obj_work		[in]	オブジェクトワークポインタ
	@param	obj_3d			[in]	3Dオブジェクトワークポインタ
									NULLの場合、オブジェワークに既に登録されているか確認し、無ければ動的に取得します。
									(ObjectExitで解放されます)

	@param	data_work		[in]	3Dモデルデータワーク
	@param	filename		[in]	2DAMAデータファイル名
	@param	index			[in]	2DAMAデータAMBインデックス
	@param	archive			[in]	AMAデータを含むアーカイブ
	@param	texlist			[in]	テクスチャリスト
	@param	id				[in]	初期化するアクションorノードID
	@param	type_node		[in]	アクション生成タイプ TRUE : ノードタイプ  FALSE : アクションタイプ

	@note
 */
// ================================================================
extern void ObjObjectAction2dAMALoad(OBS_OBJECT_WORK *obj_work, OBS_ACTION2D_AMA_WORK *obj_2d,
									OBS_DATA_WORK *data_work, const char *filename, s32 index, void *archive,
									void *amb_tex, u32 id, BOOL type_node);

// ================================================================
// ObjAction2dAMALoadSetTexlist
/*!
	テクスチャリスト設定型 2Dアクションデータ読み込み

	@param	obj_2d			[in]	2DAMAオブジェクトワークポインタ
	@param	data_work		[in]	2DAMAデータワーク
	@param	filename		[in]	2DAMAデータファイル名
	@param	index			[in]	2DAMAデータAMBインデックス
	@param	archive			[in]	AMAデータを含むアーカイブ
	@param	texlist			[in]	テクスチャリスト
	@param	id				[in]	初期化するアクションorノードID
	@param	type_node		[in]	アクション生成タイプ TRUE : ノードタイプ  FALSE : アクションタイプ

	@note
 */
// ================================================================
extern void ObjAction2dAMALoadSetTexlist(OBS_ACTION2D_AMA_WORK *obj_2d,
									OBS_DATA_WORK *data_work, const char *filename, s32 index, void *archive,
									NNS_TEXLIST *texlist, u32 id, BOOL type_node);

// ================================================================
// ObjObjectAction2dAMALoadSetTexlist
/*!
	2Dアクションデータ読み込み

	@param	obj_work		[in]	オブジェクトワークポインタ
	@param	obj_3d			[in]	3Dオブジェクトワークポインタ
									NULLの場合、オブジェワークに既に登録されているか確認し、無ければ動的に取得します。
									(ObjectExitで解放されます)

	@param	data_work		[in]	3Dモデルデータワーク
	@param	filename		[in]	2DAMAデータファイル名
	@param	index			[in]	2DAMAデータAMBインデックス
	@param	archive			[in]	AMAデータを含むアーカイブ
	@param	texlist			[in]	テクスチャリスト
	@param	id				[in]	初期化するアクションorノードID
	@param	type_node		[in]	アクション生成タイプ TRUE : ノードタイプ  FALSE : アクションタイプ

	@note
 */
// ================================================================
extern void ObjObjectAction2dAMALoadSetTexlist(OBS_OBJECT_WORK *obj_work, OBS_ACTION2D_AMA_WORK *obj_2d,
									OBS_DATA_WORK *data_work, const char *filename, s32 index, void *archive,
									NNS_TEXLIST *texlist, u32 id, BOOL type_node);

// ================================================================
// ObjAction2dAMAWorkInit
/*!
	2Dアクションワーク初期化

	@param	obj_2d			[in]	2DAMAオブジェクトワークポインタ
 */
// ================================================================
extern void ObjAction2dAMAWorkInit(OBS_ACTION2D_AMA_WORK *obj_2d);

// ================================================================
// ObjAction2dAMACreate
/*!
	2Dアクション生成

	@param	obj_2d			[in]	2DAMAオブジェクトワークポインタ

	@note
		Createする前に ObjAction2dAMALoad 等でデータを読み込んでおく必要があります。
 */
// ================================================================
extern void ObjAction2dAMACreate(OBS_ACTION2D_AMA_WORK *obj_2d);

// ================================================================
// ObjAction2dAMALoadCheck
/*!
	2Dアクションデータ読み込み終了チェック

	@param	obj_2d			[in]	2Dオブジェクトワークポインタ

	@return	TRUE : 読み込み終了

	@note
		データ読み込みが終了している場合は、関連フラグの設定を行います。
 */
// ================================================================
extern BOOL ObjAction2dAMALoadCheck(OBS_ACTION2D_AMA_WORK *obj_2d);

#endif //#if OBD_USE_ACTION2D_AMA


#if OBD_USE_ACTION2D
// ================================================================
// ObjObjectActionLoad
/*!
  アクションデータ読み込み

  @param pWork      [in] オブジェワークポインタ
  @param pObj2d     [in] 2Dワークポインタ
                         NULLの場合、オブジェワークに既に登録されているか確認し、無ければ動的に取得します。
                        （ObjectExitで解放されます）
  @param pPath      [in] データパス
  @param pData      [in] アクションデータ管理ワークポインタ
                         （NULLの場合、同じオブジェクト作成ごとにリソースを消費します）
  @param pArchive   [in] アーカイブポインタ
                          (NULLの場合、ROMからロードを行います、従ってワンカートリッジ不可)
  @param usCharSize [in] キャラサイズ
                         OBD_AUTO_CHARSIZEで自動でそのファイルの最大サイズのVRAMを取得します。

 */
// ================================================================
void ObjObjectActionLoad( OBS_OBJECT_WORK *pWork, OBS_ACTION2D_WORK *pObj2d,
                          const char* pPath, OBS_DATA_WORK* pData, void * pArchive,
                          u16 usCharSize );
#endif

#if OBD_USE_ACTION3D_NNS
// ================================================================
// ObjObjectAction3dModelLoad
/*!
  3Dアクションデータ読み込み

  @param pWork    [in] オブジェクトワークポインタ
  @param pAct     [in] 3Dアクション先頭ポインタ
                       NULLの場合、オブジェワークに既に登録されているか確認し、無ければ動的に取得します。
                      （ObjectExitで解放されます）
  @param pPath    [in] NSBMDファイルパス
  @param ulIndex  [in] モデルインデクス番号
  @param pDataMod [in] モデルデータ管理ワークポインタ（NULLの場合、同じオブジェクト作成ごとにリソースを消費します）
  @param pArchive [in] アーカイブポインタ (NULLの場合、ROMからロードを行います、従ってワンカートリッジ不可)

 */
// ================================================================
void ObjObjectAction3dModelLoad( OBS_OBJECT_WORK *pWork,
                                 OBS_ACTION3D_NNS_WORK *pObj3d,
                                 const char* pPath, u32 ulIndex,
                                 OBS_DATA_WORK* pDataMod, void * pArchive);

// ================================================================
// ObjObjectAction3dAnimeLoad
/*!
  3Dアクションデータ読み込み

  @param pWork    [in] オブジェクトワークポインタ
  @param pObj3d   [in] 3Dオブジェクト先頭ポインタ
  @param pPath    [in] NSB** ファイルパス

  @param pDataMod [in] モデルデータ管理ワークポインタ（NULLの場合、同じオブジェクト作成ごとにリソースを消費します）
  @param pArchive [in] アーカイブポインタ (NULLの場合、ROMからロードを行います、従ってワンカートリッジ不可)

 */
// ================================================================
void ObjObjectAction3dAnimeLoad( OBS_OBJECT_WORK *pWork,
                                 OBS_ACTION3D_NNS_WORK *pObj3d,
                                 const char* pPath,
                                 OBS_DATA_WORK* pDataAni, void * pArchive);
#endif

#if OBD_USE_ACTION3D_1M1S
// ================================================================
// ObjObjectAction3dModelSimpleLoad
/*!
  アニメなし3Dモデルデータ読み込み

  @param pWork    [in] オブジェクトワークポインタ
  @param pObj3d   [in] 3Dオブジェクト先頭ポインタ OBS_ACTION3D_SIMPLE_WORK
                       NULLの場合、オブジェワークに既に登録されているか確認し、無ければ動的に取得します。
                      （ObjectExitで解放されます）
  @param pPath    [in] NSBMDファイルパス
  @param ulIndex  [in] モデルインデクス番号
  @param usID     [in] シェイプID番号
  @param usID     [in] マトリクスID番号
  @param pDataMod [in] モデルデータ管理ワークポインタ（NULLの場合、同じオブジェクト作成ごとにリソースを消費します）
  @param pArchive [in] アーカイブポインタ (NULLの場合、ROMからロードを行います、従ってワンカートリッジ不可)

 */
// ================================================================
void ObjObjectAction3dModelSimpleLoad( OBS_OBJECT_WORK *pWork,
                                       OBS_ACTION3D_SIMPLE_WORK *pObj3d,
                                       const char* pPath,
                                       u32 ulIndex, u16 usID, u16 usMatID,
                                       OBS_DATA_WORK* pDataMod, void * pArchive);
#endif

#if OBD_USE_ACTION3D_SPR
// ================================================================
// ObjObjectAction3dSpriteLoad
/*!
  3Dスプライトデータ読み込み

  @param pWork       [in] オブジェクトワークポインタ
  @param pObj3d      [in] 3Dオブジェクト先頭ポインタ OBS_ACTION3D_SPRITE_WORK
                          NULLの場合、オブジェワークに既に登録されているか確認し、無ければ動的に取得します。
                         （ObjectExitで解放されます）
  @param pPath       [in] NSBMDファイルパス
  @param usCharSize  [in] 使用キャラクタサイズ
  @param usColorNum  [in] 使用色数 （0でdefaultの16色になる）
  @param pDataMod    [in] アクションデータ管理ワークポインタ（NULLの場合、同じオブジェクト作成ごとにリソースを消費します）
  @param pArchive    [in] アーカイブポインタ (NULLの場合、ROMからロードを行います、従ってワンカートリッジ不可)

 */
// ================================================================
void ObjObjectAction3dSpriteLoad( OBS_OBJECT_WORK *pWork,
                                 OBS_ACTION3D_SPRITE_WORK *pObj3d,
                                 const char* pPath,  u16 usCharSize, u16 usColorNum, 
                                 OBS_DATA_WORK* pDataMod, void * pArchive);
#endif

#if OBD_USE_ACTION3D_SS
// ================================================================
// ObjObjectActionSoftwareSpriteLoad
/*!
  ソフトウェアスプライトデータ読み込み

	@param obj_work		[in]	オブジェクトワークポインタ
	@param obj_3dss		[in]	ソフトウェアスプライトオブジェクト OBS_ACTION3D_SS_WORK
								NULLの場合、オブジェワークに既に登録されているか確認し、無ければ動的に取得します。
								(ObjObjectExitで解放されます)
	@param path			[in]	BACファイルパス
	@param char_size	[in]	使用キャラクタサイズ OBD_AUTO_CHARSIZE で自動取得
	@param color_num	[in]	使用色数 OBD_AUTO_PLTSIZE で自動取得
	@param data_work	[in]	アクションデータ管理ワークポインタ (NULLの場合、同じオブジェクト作成ごとにリソースを消費します)
	@param archive		[in]	アーカイブポインタ (NULLの場合、ROMからロードを行います、従ってワンカートリッジ不可)

 */
// ================================================================
extern void ObjObjectActionSoftwareSpriteLoad(OBS_OBJECT_WORK *obj_work, OBS_ACTION3D_SS_WORK *obj_3dss,
								const char *path,  u16 char_size, u16 color_num,
								OBS_DATA_WORK *data_work, void *archive);
#endif

#if OBD_USE_ACTION3D_POLY
// ================================================================
// ObjObjectActionPolygonLoad
/*!
  ポリゴンアクションデータ読み込み

	@param obj_work		[in]	オブジェクトワークポインタ
	@param obj_3dss		[in]	ソフトウェアスプライトオブジェクト OBS_ACTION3D_SS_WORK
								NULLの場合、オブジェワークに既に登録されているか確認し、無ければ動的に取得します。
								(ObjObjectExitで解放されます)
	@param path			[in]	PLMファイルパス
	@param data_work	[in]	アクションデータ管理ワークポインタ (NULLの場合、同じオブジェクト作成ごとにリソースを消費します)
	@param archive		[in]	アーカイブポインタ (NULLの場合、ROMからロードを行います、従ってワンカートリッジ不可)

 */
// ================================================================
extern void ObjObjectActionPolygonLoad(OBS_OBJECT_WORK *obj_work, OBS_ACTION3D_POLY_WORK *obj_3dpoly,
								const char *path,
								OBS_DATA_WORK *data_work, void *archive);
#endif

#if OBD_USE_ACTION3D_SMA
// ================================================================
// ObjObjectAction3DSmaLoad
/*!
  3D SMAデータ読み込み

	@param obj_work			[in]	オブジェクトワークポインタ
	@param obj_3dsma		[in]	3D SMAオブジェクト OBS_ACTION3D_SMA_WORK
								NULLの場合、オブジェワークに既に登録されているか確認し、無ければ動的に取得します。
								(ObjObjectExitで解放されます)
	@param path				[in]	SMM, SMG, SMP ファイルパス 拡張子なし
	@param char_size		[in]	使用キャラクタサイズ OBD_AUTO_CHARSIZE で自動取得
	@param color_num		[in]	使用色数 OBD_AUTO_PLTSIZE で自動取得
	@param cls_max			[in]	あたり情報最大数
	@param smm_data_work	[in]	SMM管理ワークポインタ (NULLの場合、同じオブジェクト作成ごとにリソースを消費します)
	@param smg_data_work	[in]	SMG管理ワークポインタ (NULLの場合、同じオブジェクト作成ごとにリソースを消費します)
	@param smp_data_work	[in]	SMP管理ワークポインタ (NULLの場合、同じオブジェクト作成ごとにリソースを消費します)
	@param smc_data_work	[in]	SMC管理ワークポインタ (NULLの場合、同じオブジェクト作成ごとにリソースを消費します)
	@param archive			[in]	アーカイブポインタ (NULLの場合、ROMからロードを行います、従ってワンカートリッジ不可)
	@param act_uncomp		[in]	解凍・転送分離用設定 TRUE : 解凍・転送分離使用
	@param tex_all_trans	[in]	テクスチャ一括転送タイプ

	@note
		path は、拡張子無しで設定します\n
		SMAデータは解凍・転送分離用設定(ActUncomp)をデータロード時に行います\n
		アクションコールバックで ObjDrawTransActUncomp を呼ぶ必要はありません
 */
// ================================================================
extern void ObjObjectAction3DSmaLoad(OBS_OBJECT_WORK *obj_work, OBS_ACTION3D_SMA_WORK *obj_3dsma,
								const char *path, u16 char_size, u16 color_num, u16 cls_max,
								OBS_DATA_WORK *smm_data_work, OBS_DATA_WORK *smg_data_work, OBS_DATA_WORK *smp_data_work,
								OBS_DATA_WORK *smc_data_work, void *archive, BOOL act_uncomp, BOOL tex_all_trans);
#endif // #if OBD_USE_ACTION3D_SMA


// ==========================================================================
// アクション 解凍・転送分離
// ==========================================================================
#if OBD_USE_ACTION2D
// ==========================================================================
// ObjObjectSetActionActUncomp
/*!
 *	アクション キャラクター 解凍・転送 分離用設定
 *
 *	@param obj_2d		[in]	アクションオブジェクトワークポインタ
 *	@param act_uncomp	[in]	解凍・転送分離ワーク
 *	@param cha_size		[in]	キャラクタサイズ(OBD_AUTO_CHARSIZE 可)
 *
 *	@note
 *		解凍領域を自動取得する場合は、\n
 *		この関数を実行する前にアクションをロードしておく必要があります\n
 *		設定したアクションは、アクションコールバック内で ObjDrawTransActUncomp を呼び出してください
 */
// ==========================================================================
extern void ObjObjectSetActionActUncomp(OBS_ACTION2D_WORK *obj_2d, OBS_ACTION_UNCOMP_WORK *act_uncomp, u32 cha_size);
#endif // #if OBD_USE_ACTION2D

#if OBD_USE_ACTION3D_SPR
// ==========================================================================
// ObjObjectSet3dSpriteActUncomp
/*!
 *	3Dスプライト キャラクター 解凍・転送 分離用設定
 *
 *	@param obj_3dspr	[in]	3Dスプライトワークポインタ
 *	@param act_uncomp	[in]	解凍・転送分離ワーク
 *	@param tex_size		[in]	テクスチャサイズ(OBD_AUTO_CHARSIZE 可)
 *
 *	@note
 *		解凍領域を自動取得する場合は、\n
 *		この関数を実行する前にアクションをロードしておく必要があります\n
 *		設定したアクションは、アクションコールバック内で ObjDrawTransActUncomp を呼び出してください
 */
// ==========================================================================
extern void ObjObjectSet3dSpriteActUncomp(OBS_ACTION3D_SPRITE_WORK *obj_3dspr, OBS_ACTION_UNCOMP_WORK *act_uncomp, u32 tex_size);
#endif // #if OBD_USE_ACTION3D_SPR

#if OBD_USE_ACTION3D_SS
// ==========================================================================
// ObjObjectSetSoftwareSpriteActUncomp
/*!
 *	ソフトウェアスプライト キャラクター 解凍・転送 分離用設定
 *
 *	@param obj_3dss		[in]	ソフトウェアスプライトオブジェクトワークポインタ
 *	@param act_uncomp	[in]	解凍・転送分離ワーク
 *	@param tex_size		[in]	テクスチャサイズ(OBD_AUTO_CHARSIZE 可)
 *
 *	@note
 *		解凍領域を自動取得する場合は、\n
 *		この関数を実行する前にアクションをロードしておく必要があります\n
 *		設定したアクションは、アクションコールバック内で ObjDrawTransActUncomp を呼び出してください
 */
// ==========================================================================
extern void ObjObjectSetSoftwareSpriteActUncomp(OBS_ACTION3D_SS_WORK *obj_3dss, OBS_ACTION_UNCOMP_WORK *act_uncomp, u32 tex_size);
#endif // #if OBD_USE_ACTION3D_SS

#if defined _DS
// ==========================================================================
// ObjObjectUpdateTransActUncomp
/*!
 *	解凍済みアクションデータの転送
 *
 *	@param	cmd			[in]	コマンドデータ
 *	@param	act			[io]	アクションデータ
 *	@param	user_data	[in]	解凍・転送分離ワーク
 *
 *	@note
 *		アクション解凍・転送分離を行う場合に、アクションコールバックから呼び出してください。\n
 *		SMAアクションは描画時に転送まで行うので設定の必要はありません
 */
// ==========================================================================
extern void ObjObjectUpdateTransActUncomp(const MTS_ACT_COMMAND *cmd, MTS_ACTION *act, OBS_ACTION_UNCOMP_WORK *act_uncomp);
#endif // #if defined _DS

// ==========================================================================
// テクスチャVRAM ダブルバッファ
// ==========================================================================
#if OBD_USE_TEX_VRAM_DB
#if OBD_USE_ACTION3D_SS
// ==========================================================================
// ObjObjectSetSoftwareSpriteActVramDB
/*!
 *	ソフトウェアスプライト テクスチャダブルバッファ用設定
 *
 *	@param obj_3dss		[in]	ソフトウェアスプライトオブジェクトワークポインタ
 *	@param act_texdb	[in]	テクスチャダブルバッファ用ワーク
 *
 *	@note
 *		VRAMを確保した後に呼び出してください
 */
// ==========================================================================
extern void ObjObjectSetSoftwareSpriteActVramDB(OBS_ACTION3D_SS_WORK *obj_3dss, OBS_ACTION_TEXVRAM_DB_WORK *act_texdb);

// ==========================================================================
// ObjObjectSoftwareSpriteLumpTransActVramDB
/*!
 *	ソフトウェアスプライト テクスチャダブルバッファ時 キャラ一括転送
 *
 *	@param act_ss		[in]	ソフトウェアスプライトアクションワーク
 *	@param act_texdb	[in]	テクスチャダブルバッファ用ワーク
 *
 *	@note
 *		逐次転送を行わないアクションのキャラを一括転送します。\n
 *		ダブルバッファがないスロットに割り当てられたアクションで実行しても問題ありません。\n
 *		act_ssには、初期化してVRAMを割り当てたアクションを指定して下さい。\n
 *		キャラの転送のみを行います。パレットも同時に転送する場合は、\n
 *		アクションフラグに MTD_ACT_FLAG_NO_REQ_PLT を設定しておいて下さい
 */
// ==========================================================================
extern void ObjObjectSoftwareSpriteLumpTransActVramDB(MTS_ACTION_SS *act_ss, u16 act_id);
#endif // #if OBD_USE_ACTION3D_SS

// ==========================================================================
// ObjObjectUpdateTransActVramDB
/*!
 *	テクスチャダブルバッファ時転送チェック
 *
 *	@param	cmd			[in]	コマンドデータ
 *	@param	act			[io]	アクションデータ
 *	@param	act_texdb	[in]	テクスチャVRAM ダブルバッファシステム用ワーク
 *
 *	@note
 *		テクスチャVRAM ダブルバッファシステムを使用する場合に、アクションコールバックから呼び出してください。
 */
// ==========================================================================
extern void ObjObjectUpdateTransActVramDB(const MTS_ACT_COMMAND *cmd, MTS_ACTION *act, OBS_ACTION_TEXVRAM_DB_WORK *act_texdb);
#endif // #if OBD_USE_TEX_VRAM_DB


// ================================================================
// 地形データ
// ================================================================
// ================================================================
// ObjObjectCollisionSet
/*!
  地形当り設定

  @param pObj     [io] オブジェクトワークポインタ
  @param pCol     [io] コリジョンワークポインタ（NULLの場合、自動的にメモリを確保する、解放も自動で行う）
  @param sOfstX   [in] オフセットX値 1:15
  @param sOfstY   [in] オフセットY値 1:15
  @param usWidth  [in] 幅 1:15  地形データをセットする場合は8ドット単位である事
  @param usHeight [in] 高さ 1:15 地形データをセットする場合は8ドット単位である事

 */
// ================================================================
void ObjObjectCollisionSet ( OBS_OBJECT_WORK* pObj, OBS_COLLISION_WORK * pCol, s16 sOfstX, s16 sOfstY, u16 usWidth, u16 usHeight);

// ================================================================
// ObjObjectCollisionDifSet
/*!
  地形当り読み込み

  @param pObj     [io] オブジェクトワークポインタ
  @param pPath    [in] ファイルパス
  @param pData    [in] 地形データ管理ワークポインタ（NULLの場合、同じオブジェクト作成ごとにリソースを消費します）
  @param pArchive [in] アーカイブポインタ (NULLの場合、ROMからロードを行います、従ってワンカートリッジ不可)

 */
// ================================================================
void ObjObjectCollisionDifSet ( OBS_OBJECT_WORK* pObj, const char* pPath, OBS_DATA_WORK* pData, void * pArchive );

// ================================================================
// ObjObjectCollisionDirSet
/*!
  地形角度読み込み

  @param pObj     [io] オブジェクトワークポインタ
  @param pPath    [in] ファイルパス
  @param pData    [in] 地形データ管理ワークポインタ（NULLの場合、同じオブジェクト作成ごとにリソースを消費します）
  @param pArchive [in] アーカイブポインタ (NULLの場合、ROMからロードを行います、従ってワンカートリッジ不可)

 */
// ================================================================
void ObjObjectCollisionDirSet ( OBS_OBJECT_WORK* pObj, const char* pPath, OBS_DATA_WORK* pData, void * pArchive );

// ================================================================
// ObjObjectCollisionAtrSet
/*!
  地形属性読み込み

  @param pObj     [io] オブジェクトワークポインタ
  @param pPath    [in] ファイルパス
  @param pData    [in] 地形データ管理ワークポインタ（NULLの場合、同じオブジェクト作成ごとにリソースを消費します）
  @param pArchive [in] アーカイブポインタ (NULLの場合、ROMからロードを行います、従ってワンカートリッジ不可)

 */
// ================================================================
void ObjObjectCollisionAtrSet ( OBS_OBJECT_WORK* pObj, const char* pPath, OBS_DATA_WORK* pData, void * pArchive );

#if	defined(__cplusplus)
} /* extern "C" */
#endif

#endif // _H_OBJBJECTLOAD
