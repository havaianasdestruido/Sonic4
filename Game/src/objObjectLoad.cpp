// ================================================================
/*!
  @file obObjectLoad.c
  @brief オブジェクトデータ読み込み

  @author mana
                Copyright(c) 2004 Dimps

  $Id: objObjectLoad.cpp 2 2011-04-11 05:21:26Z thamada $
  $Date:: 2011-04-11 14:21:26 +0900#$
 */
// ================================================================
/*
 * Memo
 *
 */

//----- Include Files --------------------------------------------------
#include "pch.h"
#include "objObject.h"
#include "objObjectLoad.h"
#include "efEffect.h"

//----- Macros ---------------------------------------------------------

//----- Macros Functions -----------------------------------------------

//----- Definitions ----------------------------------------------------
#if OBD_LOAD_INITIAL_DRAW
#define OBD_LOAD_INITIAL_DRAW_ONCE   (256)
#define OBD_LOAD_INITIAL_OBJECT_MAX (256 - 1)

typedef struct tag_OBS_LOAD_INITIAL_WORK {
	s32 obj_num;
	OBS_ACTION3D_NN_WORK* obj_3d[OBD_LOAD_INITIAL_OBJECT_MAX];	//	初期化するオブジェクトへの参照
	s32 es_num;
	OBS_ACTION3D_ES_WORK* obj_3des[OBD_LOAD_INITIAL_OBJECT_MAX]; // 初期化するエフェクトへの参照
} OBS_LOAD_INITIAL_WORK;
#endif // OBD_LOAD_INITIAL_DRAW

//----- External Declarations ------------------------------------------

//----- Static Declarations --------------------------------------------

//----- Global Variables -----------------------------------------------

//----- Local Variables ------------------------------------------------

static char sFile[128] ={""}; ///< ファイル名編集用、ワーク

#if OBD_LOAD_INITIAL_DRAW
static OBS_LOAD_INITIAL_WORK obj_load_initial_work = {0};
static BOOL obj_load_initial_set_flag = FALSE;
#endif 

//----- Global Functions -----------------------------------------------

#if OBD_LOAD_INITIAL_DRAW
// ==========================================================================
// ObjLoadInitDraw
/*!	カクつきの原因と思われる初回描画を先行して実行
 *
 *	@return	TRUE:描画完了 FALSE:描画中
 *
 *	@note	ObjLoad***を用いてロードされたオブジェクト/エフェクトをそのまま描画します。
 *			ObjLoadSetInitDrawFlagにて本関数での描画に登録するか否かを設定できます。
 *			登録した場合はObjLoadClearDrawにてクリアしない限り生き続けるます。
 *			オブジェクトを開放しても登録され続けるので使い方に注意してください。
 */
// ==========================================================================
BOOL ObjLoadInitDraw(void)
{
	int i;
	OBS_LOAD_INITIAL_WORK* work = &obj_load_initial_work;
	
	for (i = 0; i < work->obj_num; i++) {
#if 0
		// かくつき防止
		// ライティングをカットする場合の処理
		int count;
		int num = work->obj_3d[i]->object->nMaterial;
		NNS_MATERIAL_GLES11_DESC* desc = (NNS_MATERIAL_GLES11_DESC*)work->obj_3d[i]->object->pMatPtrList->pMaterial;
		for (count = 0; count < num; count++) {
			desc[count].fFlag = NND_MATFLAG_DISABLE_LIGHTING;
		}
#endif // 0
		ObjDrawAction3DNN(work->obj_3d[i], NULL, NULL, NULL, NULL);
	}
	for (i = 0; i < work->es_num; i++) {
		ObjDrawAction3DES(work->obj_3des[i], NULL, NULL, NULL, NULL);
	}
	return TRUE; // 一括で最適化を行わない場合はFALSEを返せるように処理を変更する。
}

// ==========================================================================
// ObjLoadClearDraw
/*!	描画命令をクリア
 */
// ==========================================================================
void ObjLoadClearDraw(void)
{
	obj_load_initial_work.obj_num = 0;
	obj_load_initial_work.es_num  = 0;
}

// ==========================================================================
// ObjLoadSetInitDrawFlag
/*!
 *	InitialDrawへの登録を行うか否かのフラグ
 *
 *	@param flag [in] TRUE:登録する FALSE:登録しない
 *
 *	@note flagがTRUEの場合はデータロード時に登録を行います。
 *		このフラグの初期値はFALSEです。
 *		
 *		この関数が呼び出された場合、強制的にObjLoadClearDrawを呼びます。
 */
// ==========================================================================
void ObjLoadSetInitDrawFlag(BOOL flag)
{
	obj_load_initial_set_flag = flag;
	ObjLoadClearDraw(); // 強制クリア
}
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
void* ObjDataLoadAmbIndex(OBS_DATA_WORK *data_work, s32 index, void *amb)
{
	void	*data = NULL;
	char	*file_id = (char *)amb;

	MTM_ASSERT(amb);

	if (strncmp(file_id + 1, "AMB", 3)) {
		MTM_ASSERT(!"objObjectLoad::ObjDataLoadAmbIndex() Error! amb file type error\n");
		return (NULL);
	}
	// 変換済みチェック
	if (*file_id != AMD_CONVERTED_MARK) {
		amBindConv((u8*)amb);
	}

	if (data_work) {
		if (data_work->pData == NULL) {
			if (amb) {
				data_work->pData = amBindGet((AMS_AMB_HEADER*)amb, index);
				data_work->num = OBD_DATA_ARCHIVE_FLAG;
				data_work->num++;
			}
		}
		else {
			data_work->num++;
		}
		return (data_work->pData);
	}
	else if (amb) {
		data = amBindGet((AMS_AMB_HEADER*)amb, index);
	}

	return (data);
}


// ================================================================
// アーカイブ ファイル読み込み
// ================================================================
#if defined _DS
// =====================================================================
// ObjDataNarcToFile
/*!
  ファイルパスを使ってアーカイブポインタからファイルポインタ取得
 
  @param pPath    [in] 読み込むファイルパス
  @param pArchive [in] アーカイブポインタ
 
 */
// =====================================================================
void* ObjDataNarcToFile( const char* pPath, void* pArchive )
{
    NNSFndArchive   nArc;
    void * ret;
    
    STD_CopyString( sFile, "obj:");
    STD_ConcatenateString( sFile, pPath);

    // アーカイブをマウント
    NNS_FndMountArchive( &nArc, "obj", pArchive );
    // アーカイブからポインタ取得
    ret = NNS_FndGetArchiveFileByName( sFile );
    // アーカイブをアンマウント
    NNS_FndUnmountArchive( &nArc );

    return ret;
}
#endif

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
void ObjNarcGetFile( OBS_DATA_WORK* pData, char * pPath,void * pArchive )
{
    NNSFndArchive   nArc;
    void* tmp;
    u32 ulSize = 0;

    MTM_ASSERT( pData );

    // 読み込み済み
    if ( pData->num ){
        return;
    }

    MTM_ASSERT( pPath );
    MTM_ASSERT( pArchive );

    // マウント
    NNS_FndMountArchive( &nArc, "obj", pArchive );

    // ファイルポインタ取得
    tmp = ObjDataLoad( pData, pPath, pArchive );
    
    MTM_ASSERT( STD_GetStringLength( pPath ) < 32 - 4);
    // サイズ取得
    STD_CopyString( sFile, "obj:");
    STD_ConcatenateString( sFile, pPath);
    
    ulSize = mtFsGetFileSize( sFile );
    
    // メモリ取得、アドレス上書き
    pData->pData = mtMemAllocMain( ulSize );
    // アーカイブ使用フラグを落とす
    pData->num &= ~OBD_DATA_ARCHIVE_FLAG;
    
    // コピー
    MI_CpuCopy32( tmp, (void*)pData->pData, ulSize );
    
    // アンマウント
    NNS_FndUnmountArchive( &nArc );
}
#endif


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
void* ObjDataSet( OBS_DATA_WORK* pWork, void * pData )
{
    MTM_ASSERT( pWork );
    MTM_ASSERT( pData );
    
    pWork->pData = pData;
    ++pWork->num;

    return pWork->pData;
}

// ================================================================
// ObjDataGetInc
/*!
  データワークからカウント付きでデータを取得
 
  @param pWork   [io] データ管理ワークポインタ（Globalなもの）
 
 */
// ================================================================
void* ObjDataGetInc( OBS_DATA_WORK* pWork )
{
    MTM_ASSERT( pWork );

	if (pWork->pData) {
		++pWork->num;
	}

    return (pWork->pData);
}

// ================================================================
// ObjDataLoad
/*!
  使用数チェックありデータ読み込み
 
  @param data_work	[io] データ管理ワークポインタ
  @param filename   [in] 読み込むファイルパス
  @param archive	[in] アーカイブポインタ
 
 */
// ================================================================
#if defined _DS
void* ObjDataLoad(OBS_DATA_WORK *data_work, const char*filename, void *archive)
{
    void * ret;

    if ( data_work == NULL ){

        MTM_ASSERT( STD_GetStringLength(filename)+4 < sizeof(sFile) );

        if ( archive )
            ret = ObjDataNarcToFile( filename, archive );
        else{
#if defined(MTD_DEBUG)  // デバッグ版
            {
                FSFile  file;
                BOOL    result;
                FS_InitFile( &file );

                // ファイルをオープンする
                result  = FS_OpenFile( &file, filename );
                if( !result ){
                    // ファイル存在しない
                    OS_Printf("ERROR! NOT FIND FILE\n");
                    return NULL;
                }
                FS_CloseFile( &file );
            }
#endif  // #endif of #if defined(MTD_DEBUG)  // デバッグ版
            ret = mtFsLoadFile( filename, MTD_FS_DEST_AUTO_ALLOC_HEAD );
        }
        return ret;
    }else{

        if ( data_work->pData == NULL ){

            MTM_ASSERT( STD_GetStringLength(filename)+4 < sizeof(sFile) );
            // データ取得後解放を行わなかったか 初期化していないか メモリ破壊あり
            MTM_ASSERT( !data_work->num );
            
            if ( archive ){
                data_work->pData = ObjDataNarcToFile( filename, archive );

                // ARCHIVEにファイルが無かった
                if ( data_work->pData == NULL )
                    return NULL;
                
                // 使用数インクリメント
                ++data_work->num;
                data_work->num |= OBD_DATA_ARCHIVE_FLAG;

            }else{
                /*
                // Dma使用時は止める
                if ( _mt_global_flag & ( MTD_GLOBAL_H_DMA_0 << MTD_DMA_NO_H_DMA_0 ))
                    _mt_global_flag_v &= (MTD_GLOBAL_H_DMA_0 << MTD_DMA_NO_H_DMA_0);
                if ( _mt_global_flag & ( MTD_GLOBAL_H_DMA_0 << MTD_DMA_NO_H_DMA_1 ))
                    _mt_global_flag_v &= (MTD_GLOBAL_H_DMA_0 << MTD_DMA_NO_H_DMA_1);
                if ( FS_DMA_NOT_USE != MTD_DMA_NO_FS )
                    // Vブランク待ち
                    SVC_WaitVBlankIntr();
                 */
                // ファイル存在チェック
#if defined(MTD_DEBUG)  // デバッグ版
                {
                    FSFile  file;
                    BOOL    result;
                    FS_InitFile( &file );

                    // ファイルをオープンする
                    result  = FS_OpenFile( &file, filename );
                    if( !result ){
                        // ファイル存在しない
                        OS_Printf("ERROR! NOT FIND FILE\n");
                        return NULL;
                    }
                    FS_CloseFile( &file );
                }
#endif  // #endif of #if defined(MTD_DEBUG)  // デバッグ版
                
                // データアドレスチェック
                data_work->pData = mtFsLoadFile( filename, MTD_FS_DEST_AUTO_ALLOC_HEAD );
                // 使用数インクリメント
                ++data_work->num;
                
                //if ( FS_DMA_NOT_USE != MTD_DMA_NO_FS )
                //    MI_WaitDma( MTD_DMA_NO_FS );
            }
        }else{
            ++data_work->num;
        }
        return data_work->pData;
    }
    return NULL;
}
#else
void* ObjDataLoad(OBS_DATA_WORK *data_work, const char*filename, void *archive)
{
	void	*data = NULL;
	char	*file_id = (char *)archive;

	MTM_ASSERT(filename);

	// ファイルタイプチェック
	if (archive && strncmp(file_id + 1, "AMB", 3)) {
		MTM_ASSERT(!"objObjectLoad::ObjDataLoad() Error! archive type error\n");
		return (NULL);
	}

	// 変換済みチェック
	if (archive && *file_id != AMD_CONVERTED_MARK) {
		amBindConv((u8*)archive);
	}

	MTM_ASSERT(strlen(filename) < sizeof(sFile));
	strcpy(sFile, filename);

	if (data_work) {
		if (data_work->pData == NULL) {
			if (archive) {
				data_work->pData = amBindSearch((AMS_AMB_HEADER*)archive, sFile, NULL);
				data_work->num = OBD_DATA_ARCHIVE_FLAG;
				data_work->num++;
			}
			else {
				// ファイル読み出し(終了復帰)
				amFsRead(sFile, &data_work->pData, 0);
				if (data_work->pData) {
					data_work->num++;
				}
			}
		}
		else {
			data_work->num++;
		}
		return (data_work->pData);
	}
	else if (archive) {
		data = amBindSearch((AMS_AMB_HEADER*)archive, sFile, NULL);
	}
	else {
		// ファイル読み出し(終了復帰)
		amFsRead(sFile, &data, 0);
	}

	return (data);
}
#endif

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
void* ObjDataLoadBB(OBS_DATA_WORK* data_work, const char *path, u16 index)
{
	void	*ret;

	MTM_ASSERT(path);

    if (data_work == NULL) {
		ret = mtBbLoadFile(path, index, MTD_FS_DEST_AUTO_ALLOC_HEAD);
		return (ret);
	}
	else {
		if (data_work->pData == NULL) {
			// データ取得後解放を行わなかったか 初期化していないか メモリ破壊あり
			MTM_ASSERT( !data_work->num );

			data_work->pData = mtBbLoadFile(path, index, MTD_FS_DEST_AUTO_ALLOC_HEAD);
			if (data_work->pData) {
				data_work->num++;
			}
		}
		else {
			data_work->num++;
		}
		return (data_work->pData);
	}
	return (NULL);
}
#endif


// ================================================================
// ObjDataRelease
/*!
  使用数チェックありデータ解放
 
  @param pWork   [io] データ管理ワークポインタ
 */
// ================================================================
void ObjDataRelease( OBS_DATA_WORK* pWork )
{
    // アーカイブ使用の場合はusNumは０
    if( pWork->num ){
        // データアドレスチェック
        if ( pWork->pData ){
            // 使用数デクリメント
            --pWork->num;

            if ( !pWork->num ){
                // 解放
                mtMemFreeMain(pWork->pData);
                pWork->pData = NULL;
            }
            if ( pWork->num == OBD_DATA_ARCHIVE_FLAG ){
                pWork->pData = NULL;
                pWork->num = 0;
            }
        }
    }
}


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
void ObjModLoadTexSetUpAndRelease( OBS_DATA_WORK* pData, char * pPath )
{
    void * pTemp;

    MTM_ASSERT(pData);

    // 使用数を増やさないようにここでチェックする
    if ( pData->pData ){
        pTemp = pData->pData;
    }else{
        // モデルデータ読み込み
        pTemp = ObjDataLoad( pData, pPath, NULL );
    }
    // テクスチャ設定
    NNS_G3dResDefaultSetup( pTemp );

    // テクスチャを除いたモデル用メモリ取得
    pData->pData = mtMemAllocMain( mtUtilGetNsbmdSizeWithoutTexPltBlock( pTemp ) );
    // テクスチャを除いたデータをコピー
    mtUtilCutTexPltBlockFromNsbmd( pTemp, pData->pData );
    // 元データを解放
    mtMemFreeMain( pTemp );
}
#endif


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
u32 ObjVramAlloc( OBS_DATA_WORK* pWork, MTE_GE2_TYPE ge_type, u32 num_cha )
{
    MTM_ASSERT( pWork != NULL);
    
    if ( pWork->pData == NULL && num_cha){
        
        // データアドレスチェック
        pWork->pData = (void*)mtVramAllocObj( ge_type, num_cha );
        
    }
    // 使用数インクリメント
    ++pWork->num;
    return (u32)pWork->pData;

}
// ================================================================
// ObjVramRelease
/*!
  使用数チェックありVRAM解放
 
  @param pWork   [io] データ管理ワークポインタ
  @param ge_type [in] 対象グラフィックスエンジンのタイプ MTE_GE2_TYPE列挙型

 */
// ================================================================
void ObjVramRelease( OBS_DATA_WORK* pWork, MTE_GE2_TYPE ge_type )
{
    MTM_ASSERT( pWork != NULL);
    
    if ( pWork->pData ){
        // 使用数デクリメント
        --pWork->num;

        if ( !pWork->num ){
            // 解放
            mtVramFreeObj( ge_type, (u32)pWork->pData );
            pWork->pData = NULL;
        }
    }
}

// ================================================================
// ObjVramAllocTex
/*!
  共用テクスチャVRAM取得
 
  @param pWork   [io] データ管理ワークポインタ（Globalなもの）
  @param num_cha [in] 確保するキャラクタ数

  @Return 取得したVRAMのアドレス

  @note
    この関数で取得したメモリは自分で解放する事\n
    ObjObjectVramAllocTexを通した場合はオブジェクトで管理され明示的に解放する必要はありません 05.03.22
 */
// ================================================================
u32 ObjVramAllocTex( OBS_DATA_WORK* pWork, u32 num_cha )
{
    MTM_ASSERT( pWork != NULL);
    
    if ( pWork->pData == NULL && num_cha){
        
        // データアドレスチェック
        pWork->pData = (void*)mtVramAllocTex( num_cha, FALSE );
        
    }
    // 使用数インクリメント
    ++pWork->num;

    // データ設定
    return (u32)pWork->pData;
}

// ================================================================
// ObjVramAllocTexPlt
/*!
  共用テクスチャパレット取得
 
  @param pWork   [io] データ管理ワークポインタ（Globalなもの）
  @param num_cha [in] 確保するパレット数

  @Return 取得したVRAMのアドレス

  @note
    この関数で取得したメモリは自分で解放する事\n
    ObjObjectVramAllocTexを通した場合はオブジェクトで管理され明示的に解放する必要はありません 05.03.22
 */
// ================================================================
u32 ObjVramAllocTexPlt( OBS_DATA_WORK* pWork, u16 num_plt )
{
    MTM_ASSERT( pWork != NULL);
    
    if ( pWork->pData == NULL && num_plt){
        // データアドレスチェック
        pWork->pData = (void*)mtVramAllocTexPlt( num_plt, FALSE);
    }
    // 使用数インクリメント
    ++pWork->num;

    // データ設定
    return (u32)pWork->pData;
}

// ================================================================
// ObjVramReleaseTex
/*!
  使用数チェックありテクスチャVRAM解放
 
  @param pWork   [io] データ管理ワークポインタ

 */
// ================================================================
void ObjVramReleaseTex(OBS_DATA_WORK* pWork)
{
    MTM_ASSERT( pWork != NULL);
    
    if ( pWork->pData ){
        // 使用数デクリメント
        --pWork->num;

        if ( !pWork->num ){
            // 解放
            mtVramFreeTex((u32)pWork->pData);
            pWork->pData = NULL;
        }
    }
}

// ================================================================
// ObjVramReleaseTexPlt
/*!
  使用数チェックありテクスチャパレット解放
 
  @param pWork   [io] データ管理ワークポインタ

 */
// ================================================================
void ObjVramReleaseTexPlt(OBS_DATA_WORK* pWork)
{
    MTM_ASSERT( pWork != NULL);
    
    if ( pWork->pData ){
        // 使用数デクリメント
        --pWork->num;

        if ( !pWork->num ){
            // 解放
            mtVramFreeTexPlt((u32)pWork->pData);
            pWork->pData = NULL;
        }
    }
}


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
void ObjActVramAllocDS( MTS_ACTION_DS *pAct, u16 usCharSize, OBS_DATA_WORK* pData )
{
    pAct->cha_addr[0] = ObjVramAlloc( pData, MTE_GE2_A, (u32)usCharSize);
    ++pData;
    pAct->cha_addr[1] = ObjVramAlloc( pData, MTE_GE2_B, (u32)usCharSize);
    
}
// ================================================================
// objActVramRelease
/*!
  アクション用共用VRAMアドレス取得

  @param pData [in] アドレス管理ワークポインタ二つ分

 */
// ================================================================
void ObjActVramReleaseDS( OBS_DATA_WORK* pData )
{
    ObjVramRelease( pData, MTE_GE2_A );
    ++pData;
    ObjVramRelease( pData, MTE_GE2_B );
}

// ================================================================
// オブジェクトVRAM
// ================================================================
// ================================================================
// ObjObjectVramAlloc
/*!
  オブジェクト用共用VRAMアドレス取得

  @param pWork  [in] オブジェクトポインタ
  @param usCharSize [in] キャラサイズ
  @param pData [in] アドレス管理ワークポインタ二つ分

 */
// ================================================================
void ObjObjectVramAlloc( OBS_OBJECT_WORK *pWork, u16 usCharSize, OBS_DATA_WORK* pData )
{
    if ( pWork->obj_2d ){
        pWork->obj_2d->vram_data_work = pData;
        if ( usCharSize == OBD_AUTO_CHARSIZE ){
            if ( pWork->obj_2d->bac_data_work ) {
				usCharSize = obj_get_cha_name_max_func[g_obj.vram_map_mode]( pWork->obj_2d->bac_data_work->pData );
			}
        }

        pWork->obj_2d->act_spr.cha_addr[0] = ObjVramAlloc( pData, MTE_GE2_A, (u32)usCharSize);
        ++pData;
        pWork->obj_2d->act_spr.cha_addr[1] = ObjVramAlloc( pData, MTE_GE2_B, (u32)usCharSize);
    }
}
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
// ================================================================
void ObjObjectVramAllocTex( OBS_OBJECT_WORK *pWork, u32 num_cha, u16 num_plt, OBS_DATA_WORK* pData )
{
	if (pWork->obj_3dspr) {
		OBS_ACTION3D_SPRITE_WORK	*obj_3dspr = pWork->obj_3dspr;

		// データアドレス設定
		// VRAMアドレス設定
		obj_3dspr->tex_vram_data_work = pData;
		obj_3dspr->act_3dspr.act.cha_addr = ObjVramAllocTex( pData, num_cha);
		++pData;
		obj_3dspr->plt_vram_data_work = pData;
		obj_3dspr->act_3dspr.act.plt_addr = ObjVramAllocTexPlt( pData, num_plt);
	}
#if OBD_USE_ACTION3D_SS
	else if (pWork->obj_3dss) {
		OBS_ACTION3D_SS_WORK	*obj_3dss = pWork->obj_3dss;

		// データアドレス設定
		// VRAMアドレス設定
		obj_3dss->tex_vram_data_work = pData;
		obj_3dss->act_ss.act.cha_addr = ObjVramAllocTex(pData, num_cha);
		++pData;
		obj_3dss->plt_vram_data_work = pData;
		obj_3dss->act_ss.act.plt_addr = ObjVramAllocTexPlt(pData, num_plt);
	}
#endif // #if OBD_USE_ACTION3D_SS
#if OBD_USE_ACTION3D_SMA
	else if (pWork->obj_3dsma) {
		OBS_ACTION3D_SMA_WORK	*obj_3dsma = pWork->obj_3dsma;

		// データアドレス設定
		// VRAMアドレス設定
		obj_3dsma->tex_vram_data_work = pData;
		mtSmaVramSetTexBaseAddr(pWork->obj_3dsma->act_sma, ObjVramAllocTex(pData, num_cha), num_cha);
		++pData;
		obj_3dsma->plt_vram_data_work = pData;
		mtSmaVramSetTexPltBaseAddr(pWork->obj_3dsma->act_sma, ObjVramAllocTexPlt(pData, num_plt), num_plt);
	}
#endif // #if OBD_USE_ACTION3D_SMA
}

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
void ObjObjectVramAllocOnlyTex( OBS_OBJECT_WORK *pWork, u32 num_cha, OBS_DATA_WORK* pData )
{
	if (pWork->obj_3dspr) {
		OBS_ACTION3D_SPRITE_WORK	*obj_3dspr = pWork->obj_3dspr;

		// データアドレス設定
		// VRAMアドレス設定
		obj_3dspr->tex_vram_data_work = pData;
		obj_3dspr->act_3dspr.act.cha_addr = ObjVramAllocTex( pData, num_cha);
	}
#if OBD_USE_ACTION3D_SS
	else if (pWork->obj_3dss) {
		OBS_ACTION3D_SS_WORK	*obj_3dss = pWork->obj_3dss;

		// データアドレス設定
		// VRAMアドレス設定
		obj_3dss->tex_vram_data_work = pData;
		obj_3dss->act_ss.act.cha_addr = ObjVramAllocTex(pData, num_cha);
	}
#endif // #if OBD_USE_ACTION3D_SS
#if OBD_USE_ACTION3D_SMA
	else if (pWork->obj_3dsma) {
		OBS_ACTION3D_SMA_WORK	*obj_3dsma = pWork->obj_3dsma;

		// データアドレス設定
		// VRAMアドレス設定
		obj_3dsma->tex_vram_data_work = pData;
		mtSmaVramSetTexBaseAddr(pWork->obj_3dsma->act_sma, ObjVramAllocTex(pData, num_cha), num_cha);
	}
#endif // #if OBD_USE_ACTION3D_SMA
}

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
void ObjObjectVramAllocOnlyTexPlt( OBS_OBJECT_WORK *pWork, u16 num_plt, OBS_DATA_WORK* pData )
{
	if (pWork->obj_3dspr) {
		OBS_ACTION3D_SPRITE_WORK	*obj_3dspr = pWork->obj_3dspr;

		// データアドレス設定
		// VRAMアドレス設定
		obj_3dspr->plt_vram_data_work = pData;
		obj_3dspr->act_3dspr.act.plt_addr = ObjVramAllocTexPlt( pData, num_plt);
	}
#if OBD_USE_ACTION3D_SS
	else if (pWork->obj_3dss) {
		OBS_ACTION3D_SS_WORK	*obj_3dss = pWork->obj_3dss;

		// データアドレス設定
		// VRAMアドレス設定
		obj_3dss->plt_vram_data_work = pData;
		obj_3dss->act_ss.act.plt_addr = ObjVramAllocTexPlt(pData, num_plt);
	}
#endif // #if OBD_USE_ACTION3D_SS
#if OBD_USE_ACTION3D_SMA
	else if (pWork->obj_3dsma) {
		OBS_ACTION3D_SMA_WORK	*obj_3dsma = pWork->obj_3dsma;

		// データアドレス設定
		// VRAMアドレス設定
		obj_3dsma->plt_vram_data_work = pData;
		mtSmaVramSetTexPltBaseAddr(pWork->obj_3dsma->act_sma, ObjVramAllocTexPlt(pData, num_plt), num_plt);
	}
#endif // #if OBD_USE_ACTION3D_SMA
}

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
void ObjObjectTransTexPlt( OBS_OBJECT_WORK *pWork, void *bac, u16 act_id)
{
	MTM_ASSERT(bac);

	if (pWork->obj_3dspr) {
		OBS_ACTION3D_SPRITE_WORK	*obj_3dspr = pWork->obj_3dspr;
		MTS_ACTION3D_SPRITE			act_3dspr;

		MTM_ASSERT(obj_3dspr->act_3dspr.act.plt_addr);

		mtAct3dInitStructSprite(&act_3dspr, 0, bac, act_id, MTD_ACT_FLAG_NO_CHA, 0, obj_3dspr->act_3dspr.act.plt_addr);
		mtAct3dUpdateSprite(&act_3dspr, NULL, 0);
	}
#if OBD_USE_ACTION3D_SS
	else if (pWork->obj_3dss) {
		OBS_ACTION3D_SS_WORK	*obj_3dss = pWork->obj_3dss;
		MTS_ACTION_SS			act_ss;

		MTM_ASSERT(obj_3dss->act_ss.act.plt_addr);

		mtActInitStructSS(&act_ss, bac, act_id,
        						MTD_ACT_FLAG_NO_CHA, 0, obj_3dss->act_ss.act.plt_addr, 0);
		mtActUpdateSS(&act_ss, NULL, 0);
	}
#endif // #if OBD_USE_ACTION3D_SS
#if 0
#if OBD_USE_ACTION3D_SMA
	else if (pWork->obj_3dsma) {
		OBS_ACTION3D_SMA_WORK	*obj_3dsma = pWork->obj_3dsma;

		// データアドレス設定
		// VRAMアドレス設定
		obj_3dsma->plt_vram_data_work = pData;
		mtSmaVramSetTexPltBaseAddr(pWork->obj_3dsma->act_sma, ObjVramAllocTexPlt(pData, num_plt), num_plt);
	}
#endif // #if OBD_USE_ACTION3D_SMA
#endif
}

// ================================================================
// ObjObjectVramRelease
/*!
  オブジェクト2D VRAM解放

  @param pWork [in] オブジェクトワークポインタ
 */
// ================================================================
void ObjObjectVramRelease( OBS_OBJECT_WORK * pWork )
{
    if ( pWork->obj_2d ){
        if ( pWork->obj_2d->vram_data_work ){
            // 共有VRAM開放
            ObjVramRelease( &pWork->obj_2d->vram_data_work[0], MTE_GE2_A );
            ObjVramRelease( &pWork->obj_2d->vram_data_work[1], MTE_GE2_B );
            pWork->obj_2d->act_spr.cha_addr[0] = 0;
            pWork->obj_2d->act_spr.cha_addr[1] = 0;
        }else{
            // 専用VRAM解放
            if ( pWork->obj_2d->act_spr.cha_addr[0] )
                mtVramFreeObj( MTE_GE2_A, pWork->obj_2d->act_spr.cha_addr[0]  );
            pWork->obj_2d->act_spr.cha_addr[0] = 0;
            if ( pWork->obj_2d->act_spr.cha_addr[1] )
                mtVramFreeObj( MTE_GE2_B, pWork->obj_2d->act_spr.cha_addr[1]  );
            pWork->obj_2d->act_spr.cha_addr[1] = 0;
        }

    }
    if ( pWork->obj_3dspr ){
        // 解放
#if 1
		if (pWork->obj_3dspr->tex_vram_data_work) {
            ObjVramReleaseTex(pWork->obj_3dspr->tex_vram_data_work);
            pWork->obj_3dspr->act_3dspr.act.cha_addr = NULL;
		}
		if (pWork->obj_3dspr->plt_vram_data_work) {
            ObjVramReleaseTexPlt(pWork->obj_3dspr->plt_vram_data_work);
            pWork->obj_3dspr->act_3dspr.act.plt_addr = NULL;
		}
		if (!pWork->obj_3dspr->tex_vram_data_work || !pWork->obj_3dspr->plt_vram_data_work) {
			mtAct3dReleaseStructSprite(&pWork->obj_3dspr->act_3dspr);
		}
#else
        if ( pWork->obj_3dspr->vram_data_work ){
           // ObjVramReleaseTex(&pWork->obj_3dspr->act_3dspr.a3d, &pWork->obj_3dspr->vram_data_work[0] );
           // ObjVramReleaseTex(&pWork->obj_3dspr->act_3dspr.a3d, &pWork->obj_3dspr->vram_data_work[1] );
            ObjVramReleaseTex(&pWork->obj_3dspr->vram_data_work[0]);
            ObjVramReleaseTexPlt(&pWork->obj_3dspr->vram_data_work[1]);
            pWork->obj_3dspr->act_3dspr.act.cha_addr = NULL;
            pWork->obj_3dspr->act_3dspr.act.plt_addr = NULL;
        }else{ 
            //mtAct3dReleaseStructAll( &pWork->obj_3dspr->act_3dspr.a3d );
			mtAct3dReleaseStructSprite(&pWork->obj_3dspr->act_3dspr);
		}
#endif
	}
#if OBD_USE_ACTION3D_SS
    if (pWork->obj_3dss) {
        // 解放
#if 1
		if ( pWork->obj_3dss->tex_vram_data_work ) {
			ObjVramReleaseTex(pWork->obj_3dss->tex_vram_data_work);
            pWork->obj_3dss->act_ss.act.cha_addr = NULL;
		}
		if ( pWork->obj_3dss->plt_vram_data_work ) {
            ObjVramReleaseTexPlt(pWork->obj_3dss->plt_vram_data_work);
            pWork->obj_3dss->act_ss.act.plt_addr = NULL;
		}
		if (!pWork->obj_3dss->tex_vram_data_work || !pWork->obj_3dss->plt_vram_data_work) {
			mtActReleaseStructSS(&pWork->obj_3dss->act_ss);
		}
#else
		if ( pWork->obj_3dss->vram_data_work ) {
			ObjVramReleaseTex(&pWork->obj_3dss->vram_data_work[0]);
            ObjVramReleaseTexPlt(&pWork->obj_3dss->vram_data_work[1]);
            pWork->obj_3dss->act_ss.act.cha_addr = NULL;
            pWork->obj_3dss->act_ss.act.plt_addr = NULL;
		}
		else {
			mtActReleaseStructSS(&pWork->obj_3dss->act_ss);
		}
#endif
	}
#endif // #if OBD_USE_ACTION3D_SS
#if OBD_USE_ACTION3D_SMA
    if (pWork->obj_3dsma) {
        // 解放
#if 1
		if (pWork->obj_3dsma->tex_vram_data_work) {
			ObjVramReleaseTex(pWork->obj_3dsma->tex_vram_data_work);
            pWork->obj_3dsma->act_sma->tex_vram_addr = NULL;
            pWork->obj_3dsma->act_sma->tex_vram_size = 0;
		}
		if (pWork->obj_3dsma->plt_vram_data_work) {
            ObjVramReleaseTexPlt(pWork->obj_3dsma->plt_vram_data_work);
            pWork->obj_3dsma->act_sma->plt_vram_addr = NULL;
            pWork->obj_3dsma->act_sma->plt_vram_size = 0;
		}
		if (!pWork->obj_3dsma->tex_vram_data_work || !pWork->obj_3dsma->plt_vram_data_work) {
			mtSmaVramRelease(pWork->obj_3dsma->act_sma);
		}
#else
		if (pWork->obj_3dsma->vram_data_work) {
			ObjVramReleaseTex(&pWork->obj_3dsma->vram_data_work[0]);
            ObjVramReleaseTexPlt(&pWork->obj_3dsma->vram_data_work[1]);

            pWork->obj_3dsma->act_sma->tex_vram_addr = NULL;
            pWork->obj_3dsma->act_sma->tex_vram_size = 0;
            pWork->obj_3dsma->act_sma->plt_vram_addr = NULL;
            pWork->obj_3dsma->act_sma->plt_vram_size = 0;
		}
		else {
			mtSmaVramRelease(pWork->obj_3dsma->act_sma);
		}
#endif
    }
#endif // #if OBD_USE_ACTION3D_SMA
}

// ================================================================
// パレットデータ
// ================================================================
// ================================================================
// ObjObjectPaletteLoad
/*!
  アクションからパレットデータ読み込み（ファイル読み込み済みである事）

  @param pWork [in] オブジェワークポインタ
  @param sAnimeID [in] アクションID
  @param sPltID   [in] パレットID （objPalette.h参照
 
 */
// ================================================================
void ObjObjectPaletteLoad( OBS_OBJECT_WORK *pWork, u16 usActionID, s16 sPltID )
{
    u8 ucPltNo = 0;
    if ( pWork->obj_2d ){
        if ( pWork->obj_2d->bac_data_work && pWork->obj_2d->bac_data_work->pData)
            ucPltNo = ObjPaletteLoad( pWork->obj_2d->bac_data_work->pData, usActionID, sPltID);
        else if ( pWork->obj_2d->act_spr.act.bac_addr )
            ucPltNo = ObjPaletteLoad( (void*)pWork->obj_2d->act_spr.act.bac_addr, usActionID, sPltID);
            

        pWork->obj_2d->act_spr.plt_ofst_no[0] = ucPltNo;
        pWork->obj_2d->act_spr.plt_ofst_no[1] = ucPltNo;
        pWork->obj_2d->act_spr.act.plt_ofst_no = ucPltNo;

        // グラフィックエンジンBのみのパレット
        if ( sPltID & OBD_PLT_B ){
            pWork->flag |= OBD_OBJECT_PLT_B;
        }
        pWork->obj_2d->act_spr.act.flag |= MTD_ACT_FLAG_NO_PLT;
        // pWork->obj_2d->mActB.flag |= MTD_ACT_FLAG_NO_PLT;
    }
}
// ================================================================
// ObjObjectPaletteRelease
/*!
  パレット解放

  @param pWork [in] オブジェワークポインタ
 */
// ================================================================
void ObjObjectPaletteRelease( OBS_OBJECT_WORK *pWork )
{
    if ( pWork->obj_2d ){
        // パレットアニメ終了
        if ( pWork->flag & OBD_OBJECT_PLT_ANIME )
            EfSpritePltAnimeEnd( pWork->obj_2d->act_spr.plt_ofst_no[0] );
        // パレット解放
        ObjPaletteRelease( (u8)pWork->obj_2d->act_spr.plt_ofst_no[0] );
    }
}



// ================================================================
// objActionLoad
/*!
  アクションデータ読み込み

  @param pAct  [in] アクションポインタ MTD_ACT_DS_FLAG_DISABLE_GE_AorBによって取得しないVRAMを設定する
  @param pPath [in] BACファイルパス
  @param usCharSize [in] キャラサイズ
  @param pData [in] アクションデータ管理ワークポインタ（NULLの場合、同じオブジェクト作成ごとにリソースを消費します）
  @param pArchive [in] アーカイブポインタ (NULLの場合、ROMからロードを行います、従ってワンカートリッジ不可)

  @note
  この関数で取得したメモリは自分でobjActReleaseを呼んで解放する事
  pAct->flagはあらかじめ初期化しておく事
    
 */
// ================================================================
void ObjActLoad( MTS_ACTION_DS *pAct, const char* pPath, u16 usCharSize, OBS_DATA_WORK* pData, void * pArchive)
{
    void * pBac = NULL;
    u32 ulCharAddrA = 0,ulCharAddrB = 0;

    MTM_ASSERT( pAct != NULL);
    // MTM_ASSERT( pPath != NULL);
    
    pBac = ObjDataLoad( pData, pPath, pArchive);

    if ( pBac == NULL && pArchive ){
#if defined(MTD_DEBUG)  // デバッグ版
        // ARCHIVEを指定しているのにファイルがありません
        // MTM_ASSERT(0);
        // アーカイブ
        OS_Printf("NO ARCHIVE FILE\n");
#endif        
        pBac = ObjDataLoad( pData, pPath, NULL );
    }
    if ( pBac == NULL )
        return;
    
    // VRAM取得
    if ( usCharSize ){
        if ( usCharSize == OBD_AUTO_CHARSIZE ){
            // 最大キャラサイズを取得
            usCharSize = obj_get_cha_name_max_func[g_obj.vram_map_mode]( pBac );
        }
        // 先に設定されたフラグチェック
        if ( !(pAct->flag & MTD_ACT_DS_FLAG_DISABLE_GE_A) )
            ulCharAddrA = mtVramAllocObj(MTE_GE2_A, usCharSize);
        if ( !(pAct->flag & MTD_ACT_DS_FLAG_DISABLE_GE_B) )
            ulCharAddrB = mtVramAllocObj(MTE_GE2_B, usCharSize);
    }

    
    // 初期化
    mtActInitStructDS(
        pAct, pBac, 0, pAct->flag/*| MTD_ACT_FLAG_DMA_CHA*/, MTD_ACT_FLAG_CLIP,
        MTE_CHA_VRAM_ADDRESS, ulCharAddrA,
        MTE_PLT_VRAM_ADDRESS, HW_OBJ_PLTT,
        MTE_CHA_VRAM_ADDRESS, ulCharAddrB,
        MTE_PLT_VRAM_ADDRESS, HW_DB_OBJ_PLTT, 0, 0 );
}

// ================================================================
// ObjActRelease
/*!
  アクション用共用VRAMアドレス取得

  @param pData [in] アドレス管理ワークポインタ二つ分

 */
// ================================================================
void ObjActRelease( OBS_DATA_WORK* pData, MTS_ACTION_DS *pAct )
{
    // データ解放
    ObjDataRelease( pData );
    // VRAM解放
    if ( pAct->cha_addr[0] )
        mtVramFreeObj( MTE_GE2_A, pAct->cha_addr[0]  );
    if ( pAct->cha_addr[1] )
        mtVramFreeObj( MTE_GE2_B, pAct->cha_addr[1]  );
}
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
void ObjAction3dNNModelLoad(OBS_ACTION3D_NN_WORK *obj_3d,
									OBS_DATA_WORK *data_work, const char *filename, s32 index, void *archive,
									const char *filename_tex, void *amb_tex, NNF_DRAWOBJ drawflag)
{
	s32		i;
	void	*model = NULL;
	char	*p_filename = NULL;

	MTM_ASSERT(obj_3d);
//	MTM_ASSERT(filename_tex || amb_tex);

	// 描画コマンド発行時 ステート
	obj_3d->command_state = OBD_DRAW_CMD_STATE_3DNN;	// 標準設定

	// 補間率初期化
	obj_3d->marge	= 0.0f;
	obj_3d->per		= 1.0f;

	// 標準使用ライト
	obj_3d->use_light_flag = g_obj.def_user_light_flag;

#if _PS3 | _XBOX | _PC
	// リムライト設定
	obj_3d->toon_rim_param.r = g_obj.toon_rim_param.r;
	obj_3d->toon_rim_param.g = g_obj.toon_rim_param.g;
	obj_3d->toon_rim_param.b = g_obj.toon_rim_param.b;
	// 迷彩設定
	obj_3d->toon_camouflage = g_obj.toon_camouflage;
#endif
#if _WII
	// トゥーンライト設定
	obj_3d->toon_light = g_obj.toon_light_vec;
#endif

	// ユーザーMTX初期化
	nnMakeUnitMatrix(&obj_3d->user_obj_mtx);

	// ユーザMTX_R初期化
	nnMakeUnitMatrix(&obj_3d->user_obj_mtx_r);

	// モーション速度初期化
	for (i = 0; i < OBD_ACTION3D_NN_MTN_BUF_NUM; i++) {
		obj_3d->speed[i] = 1.f;
	}
	obj_3d->mat_speed = 1.f;

	// モーションブレンド速度初期化
	obj_3d->blend_spd = OBD_ACTION3D_NN_DEF_BLEND_SPD;

	// 描画フラグ保存
	obj_3d->drawflag = drawflag;

	// 描画時設定ステータス初期化
	MI_CpuCopy8(&g_obj_draw_3dnn_draw_state, &obj_3d->draw_state, sizeof(AMS_DRAWSTATE));

	if (archive) {
		obj_3d->flag |= OBD_ACTFLAG_3D_NN_ARCHIVE;
	}

	// モデルファイル読み込み
	if (filename) {
		model = ObjDataLoad(data_work, filename, archive);

		if (archive && model == NULL) {
			// アーカイブにデータがなかった
			obj_3d->flag &= ~OBD_ACTFLAG_3D_NN_ARCHIVE;
			model = ObjDataLoad(data_work, filename, NULL);
		}
	}
	else if (archive) {
		model = ObjDataLoadAmbIndex(data_work, index, archive);
		if (model == NULL) {
			// アーカイブにデータがなかった
			obj_3d->flag &= ~OBD_ACTFLAG_3D_NN_ARCHIVE;
		}
	}
	else if (data_work) {
		model = ObjDataGetInc(data_work);
	}

	if (model == NULL) {
		amAssert(!"objObjectLoad::ObjObjectAction3dModelLoad() error! no object\n");
		return;
	}

	// モデルデータ保存
	obj_3d->model = model;

	// データワーク保存
	if (data_work) {
		obj_3d->model_data_work = data_work;
	}

	// モデル読み込み(モデルオブジェクト初期化)
	if (filename_tex) {
		MTM_ASSERT(strlen(filename_tex) < sizeof(sFile));
		strcpy(sFile, filename_tex);
		p_filename = sFile;
	}
	else {
		strcpy(sFile, "");
	}
	if (amb_tex) {
		char	*file_id = (char *)amb_tex;
		if (strncmp(file_id + 1, "AMB", 3)) {
			MTM_ASSERT(!"objObjectLoad::ObjAction3dNNModelLoad() Error! amb_tex file type error\n");
			return;
		}
		// 変換済みチェック
		if (*file_id != AMD_CONVERTED_MARK) {
			amBindConv((u8*)amb_tex);
		}
	}
	obj_3d->reg_index = amObjectLoad(&obj_3d->object, &obj_3d->texlist, &obj_3d->texlistbuf, model,
							drawflag | g_obj.load_drawflag, p_filename, (AMS_AMB_HEADER*)amb_tex);

#if OBD_LOAD_INITIAL_DRAW
	if (obj_load_initial_set_flag) {
		OBS_LOAD_INITIAL_WORK* work = &obj_load_initial_work;
		MTM_ASSERT(work->obj_num < OBD_LOAD_INITIAL_OBJECT_MAX);
		if (work->obj_num < OBD_LOAD_INITIAL_OBJECT_MAX) {
			// 使用オブジェクト登録
			work->obj_3d[work->obj_num] = obj_3d;
			++work->obj_num;
		}
	}
#endif // OBD_LOAD_INITIAL_DRAW

	obj_3d->flag |= OBD_ACTFLAG_3D_NN_REG_WAIT;
	obj_3d->flag &= ~OBD_ACTFLAG_3D_NN_REG_FINISH;
}

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
void ObjCopyAction3dNNModel(OBS_ACTION3D_NN_WORK *src_obj_3d, OBS_ACTION3D_NN_WORK *dest_obj_3d)
{
	s32		i;
	MTM_ASSERT(src_obj_3d);
	MTM_ASSERT(dest_obj_3d);

	// コピー
#if 0
	MI_CpuCopy8(src_obj_3d, dest_obj_3d, sizeof(OBS_ACTION3D_NN_WORK));
#else
	// オブジェクトデータ
	dest_obj_3d->object		= src_obj_3d->object;
	dest_obj_3d->texlist	= src_obj_3d->texlist;
	dest_obj_3d->texlistbuf	= src_obj_3d->texlistbuf;
	//dest_obj_3d->motion	= src_obj_3d->motion;
	dest_obj_3d->model				= src_obj_3d->model;
	dest_obj_3d->model_data_work	= src_obj_3d->model_data_work;

//	dest_obj_3d->mtn[i]	= src_obj_3d->mtn[i];
//	dest_obj_3d->mtn_data_work[i]	= src_obj_3d->mtn_data_work[i];
//	dest_obj_3d->mat_mtn[i]	= src_obj_3d->mat_mtn[i];
//	dest_obj_3d->mat_mtn_data_work[i]	= src_obj_3d->mat_mtn_data_work[i];

	// コマンドステート
	dest_obj_3d->command_state		= src_obj_3d->command_state;
	// 3Dオブジェクトワーク管理フラグ
	dest_obj_3d->flag				= src_obj_3d->flag;

	// 補間率初期化
	dest_obj_3d->marge	= 0.0f;
	dest_obj_3d->per	= 1.0f;

	// 標準使用ライト
	dest_obj_3d->use_light_flag = src_obj_3d->use_light_flag;

#if _PS3 | _XBOX | _PC
	// リムライト設定
	dest_obj_3d->toon_rim_param.r = src_obj_3d->toon_rim_param.r;
	dest_obj_3d->toon_rim_param.g = src_obj_3d->toon_rim_param.g;
	dest_obj_3d->toon_rim_param.b = src_obj_3d->toon_rim_param.b;
	// 迷彩設定
	dest_obj_3d->toon_camouflage = src_obj_3d->toon_camouflage;
#endif
#if _WII
	// トゥーンライト設定
	dest_obj_3d->toon_light = src_obj_3d->toon_light;
#endif
//	dest_obj_3d->act_id[i]		= src_obj_3d->act_id[i];
//	dest_obj_3d->frame[i]		= src_obj_3d->frame[i];

	// モーション速度初期化
	for (i = 0; i < OBD_ACTION3D_NN_MTN_BUF_NUM; i++) {
		dest_obj_3d->speed[i] = 1.f;
	}
	dest_obj_3d->mat_speed = 1.f;

//	dest_obj_3d->mat_act_id		= src_obj_3d->mat_act_id;
//	dest_obj_3d->mat_frame		= src_obj_3d->mat_frame;

	// ユーザーMTX初期化
	nnMakeUnitMatrix(&dest_obj_3d->user_obj_mtx);

	// ユーザMTX_R初期化
	nnMakeUnitMatrix(&dest_obj_3d->user_obj_mtx_r);

	// モーションブレンド速度初期化
	dest_obj_3d->blend_spd = OBD_ACTION3D_NN_DEF_BLEND_SPD;

	// サブオブジェクトタイプ
	dest_obj_3d->sub_obj_type		= src_obj_3d->sub_obj_type;

	// オブジェクト描画フラグ
	dest_obj_3d->drawflag	= src_obj_3d->drawflag;

	// 描画時設定ステータス初期化
	MI_CpuCopy8(&g_obj_draw_3dnn_draw_state, &dest_obj_3d->draw_state, sizeof(AMS_DRAWSTATE));

//	dest_obj_3d->user_func			= src_obj_3d->user_func;
//	dest_obj_3d->user_param			= src_obj_3d->user_param;

//	dest_obj_3d->mplt_cb_func		= src_obj_3d->mplt_cb_func;
//	dest_obj_3d->mplt_cb_param		= src_obj_3d->mplt_cb_param

//	dest_obj_3d->mtn_cb_func		= src_obj_3d->mtn_cb_func;
//	dest_obj_3d->mtn_cb_param		= src_obj_3d->mtn_cb_param;

	// その他初期化
	dest_obj_3d->reg_index	= -1;

//	dest_obj_3d->mtn_load_setting[i]		= src_obj_3d->mtn_load_setting[i];
//	dest_obj_3d->mat_mtn_load_setting[i]		= src_obj_3d->mat_mtn_load_setting[i];

#endif
}

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
void ObjObjectCopyAction3dNNModel(OBS_OBJECT_WORK *obj_work, OBS_ACTION3D_NN_WORK *src_obj_3d, OBS_ACTION3D_NN_WORK *dest_obj_3d)
{
#if 1
	MTM_ASSERT(obj_work);
	MTM_ASSERT(src_obj_3d);
	MTM_ASSERT(!obj_work->obj_3d);

	// 表示ワークチェック
	if (dest_obj_3d == NULL) {
		if (obj_work->obj_3d) {
			dest_obj_3d = obj_work->obj_3d;
		}
		else {
			dest_obj_3d = (OBS_ACTION3D_NN_WORK*)amMemAlloc(sizeof(OBS_ACTION3D_NN_WORK));
		}
		amZeroMemory(dest_obj_3d, sizeof(OBS_ACTION3D_NN_WORK));
		// 解放設定
		obj_work->flag |= OBD_OBJECT_FREE_3D;
	}

	// モデル解放無しフラグ設定
	obj_work->flag |= OBD_OBJECT_NORELEASE_3D;

	// モデルワークコピー
	ObjCopyAction3dNNModel(src_obj_3d, dest_obj_3d);

	// 3D描画ワークを設定
	obj_work->obj_3d = dest_obj_3d;

#else
	s32		i;
	MTM_ASSERT(obj_work);
	MTM_ASSERT(src_obj_3d);
	MTM_ASSERT(!obj_work->obj_3d);

	// 表示ワークチェック
	if (dest_obj_3d == NULL) {
		if (obj_work->obj_3d) {
			dest_obj_3d = obj_work->obj_3d;
		}
		else {
			dest_obj_3d = (OBS_ACTION3D_NN_WORK*)amMemAlloc(sizeof(OBS_ACTION3D_NN_WORK));
		}
		amZeroMemory(dest_obj_3d, sizeof(OBS_ACTION3D_NN_WORK));
		// 解放設定
		obj_work->flag |= OBD_OBJECT_FREE_3D;
	}

	// モデル解放無しフラグ設定
	obj_work->flag |= OBD_OBJECT_NORELEASE_3D;


	// コピー
#if 0
	MI_CpuCopy8(src_obj_3d, dest_obj_3d, sizeof(OBS_ACTION3D_NN_WORK));
#else
	// オブジェクトデータ
	dest_obj_3d->object		= src_obj_3d->object;
	dest_obj_3d->texlist	= src_obj_3d->texlist;
	dest_obj_3d->texlistbuf	= src_obj_3d->texlistbuf;
	//dest_obj_3d->motion	= src_obj_3d->motion;
	dest_obj_3d->model				= src_obj_3d->model;
	dest_obj_3d->model_data_work	= src_obj_3d->model_data_work;

//	dest_obj_3d->mtn[i]	= src_obj_3d->mtn[i];
//	dest_obj_3d->mtn_data_work[i]	= src_obj_3d->mtn_data_work[i];
//	dest_obj_3d->mat_mtn[i]	= src_obj_3d->mat_mtn[i];
//	dest_obj_3d->mat_mtn_data_work[i]	= src_obj_3d->mat_mtn_data_work[i];

	// コマンドステート
	dest_obj_3d->command_state		= src_obj_3d->command_state;
	// 3Dオブジェクトワーク管理フラグ
	dest_obj_3d->flag				= src_obj_3d->flag;

	// 補間率初期化
	dest_obj_3d->marge	= 0.0f;
	dest_obj_3d->per	= 1.0f;

	// 標準使用ライト
	dest_obj_3d->use_light_flag = src_obj_3d->use_light_flag;

//	dest_obj_3d->act_id[i]		= src_obj_3d->act_id[i];
//	dest_obj_3d->frame[i]		= src_obj_3d->frame[i];

	// モーション速度初期化
	for (i = 0; i < OBD_ACTION3D_NN_MTN_BUF_NUM; i++) {
		dest_obj_3d->speed[i] = 1.f;
	}
	dest_obj_3d->mat_speed = 1.f;

//	dest_obj_3d->mat_act_id		= src_obj_3d->mat_act_id;
//	dest_obj_3d->mat_frame		= src_obj_3d->mat_frame;

	// ユーザーMTX初期化
	nnMakeUnitMatrix(&dest_obj_3d->user_obj_mtx);

	// ユーザMTX_R初期化
	nnMakeUnitMatrix(&dest_obj_3d->user_obj_mtx_r);

	// モーションブレンド速度初期化
	dest_obj_3d->blend_spd = OBD_ACTION3D_NN_DEF_BLEND_SPD;

	// サブオブジェクトタイプ
	dest_obj_3d->sub_obj_type		= src_obj_3d->sub_obj_type;

	// オブジェクト描画フラグ
	dest_obj_3d->drawflag	= src_obj_3d->drawflag;

	// 描画時設定ステータス初期化
	MI_CpuCopy8(&g_obj_draw_3dnn_draw_state, &dest_obj_3d->draw_state, sizeof(AMS_DRAWSTATE));

//	dest_obj_3d->user_func			= src_obj_3d->user_func;
//	dest_obj_3d->user_param			= src_obj_3d->user_param;

//	dest_obj_3d->mplt_cb_func		= src_obj_3d->mplt_cb_func;
//	dest_obj_3d->mplt_cb_param		= src_obj_3d->mplt_cb_param

//	dest_obj_3d->mtn_cb_func		= src_obj_3d->mtn_cb_func;
//	dest_obj_3d->mtn_cb_param		= src_obj_3d->mtn_cb_param;

	// その他初期化
	dest_obj_3d->reg_index	= -1;

//	dest_obj_3d->mtn_load_setting[i]		= src_obj_3d->mtn_load_setting[i];
//	dest_obj_3d->mat_mtn_load_setting[i]		= src_obj_3d->mat_mtn_load_setting[i];

#endif

	// 3D描画ワークを設定
	obj_work->obj_3d = dest_obj_3d;
#endif
}

// ================================================================
// ObjObjectAction3dNNModelReleaseCopy
/*!
	3Dモデル データコピーしたモデルを解放

	@param	obj_work		[in]	オブジェクトワークポインタ
 */
// ================================================================
void ObjObjectAction3dNNModelReleaseCopy(OBS_OBJECT_WORK *obj_work)
{
	if (obj_work->flag & OBD_OBJECT_FREE_3D) {
		amMemFree(obj_work->obj_3d);
		obj_work->flag &= ~OBD_OBJECT_FREE_3D;
	}
	obj_work->obj_3d = NULL;
	obj_work->flag &= ~OBD_OBJECT_NORELEASE_3D;
}

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
void ObjObjectAction3dNNModelLoad(OBS_OBJECT_WORK *obj_work, OBS_ACTION3D_NN_WORK *obj_3d,
									OBS_DATA_WORK *data_work, const char *filename, s32 index, void *archive,
									const char *filename_tex, void *amb_tex, NNF_DRAWOBJ drawflag)
{
	MTM_ASSERT(obj_work);

	// 表示ワークチェック
	if (obj_3d == NULL) {
		if (obj_work->obj_3d) {
			obj_3d = obj_work->obj_3d;
		}
		else {
			obj_3d = (OBS_ACTION3D_NN_WORK*)amMemAlloc(sizeof(OBS_ACTION3D_NN_WORK));
		}
		amZeroMemory(obj_3d, sizeof(OBS_ACTION3D_NN_WORK));
		// 解放設定
		obj_work->flag |= OBD_OBJECT_FREE_3D;
	}

	// 3D描画ワークを設定
	obj_work->obj_3d = obj_3d;

	amAssert(obj_3d);

	// 3Dアクション初期化
	ObjAction3dNNModelLoad(obj_3d,
				data_work, filename, index, archive,
				filename_tex, amb_tex, drawflag);
}

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
void ObjAction3dNNModelLoadTxb(OBS_ACTION3D_NN_WORK *obj_3d,
									OBS_DATA_WORK *data_work, const char *filename, s32 index, void *archive,
									const char *filename_tex, void *amb_tex, NNF_DRAWOBJ drawflag,
									void *txb)
{
	s32		i;
	void	*model = NULL;
	char	*p_filename = NULL;

	MTM_ASSERT(obj_3d);
//	MTM_ASSERT(filename_tex || amb_tex);

	// 描画コマンド発行時 ステート
	obj_3d->command_state = OBD_DRAW_CMD_STATE_3DNN;	// 標準設定

	// 補間率初期化
	obj_3d->marge	= 0.0f;
	obj_3d->per		= 1.0f;

	// 標準使用ライト
	obj_3d->use_light_flag = g_obj.def_user_light_flag;

	// ユーザーMTX初期化
	nnMakeUnitMatrix(&obj_3d->user_obj_mtx);

	// ユーザーMTX_R初期化
	nnMakeUnitMatrix(&obj_3d->user_obj_mtx_r);

	// モーション速度初期化
	for (i = 0; i < OBD_ACTION3D_NN_MTN_BUF_NUM; i++) {
		obj_3d->speed[i] = 1.f;
	}

	// モーションブレンド速度初期化
	obj_3d->blend_spd = OBD_ACTION3D_NN_DEF_BLEND_SPD;

	// 描画フラグ保存
	obj_3d->drawflag = drawflag;

	// 描画時設定ステータス初期化
	MI_CpuCopy8(&g_obj_draw_3dnn_draw_state, &obj_3d->draw_state, sizeof(AMS_DRAWSTATE));

	if (archive) {
		obj_3d->flag |= OBD_ACTFLAG_3D_NN_ARCHIVE;
	}

	// モデルファイル読み込み
	if (filename) {
		model = ObjDataLoad(data_work, filename, archive);

		if (archive && model == NULL) {
			// アーカイブにデータがなかった
			obj_3d->flag &= ~OBD_ACTFLAG_3D_NN_ARCHIVE;
			model = ObjDataLoad(data_work, filename, NULL);
		}
	}
	else if (archive) {
		model = ObjDataLoadAmbIndex(data_work, index, archive);
		if (model == NULL) {
			// アーカイブにデータがなかった
			obj_3d->flag &= ~OBD_ACTFLAG_3D_NN_ARCHIVE;
		}
	}
	else if (data_work) {
		model = ObjDataGetInc(data_work);
	}

	if (model == NULL) {
		amAssert(!"objObjectLoad::ObjAction3dNNModelLoadTxb() error! no object\n");
		return;
	}

	// モデルデータ保存
	obj_3d->model = model;

	// データワーク保存
	if (data_work) {
		obj_3d->model_data_work = data_work;
	}

	// モデル読み込み(モデルオブジェクト初期化)
	if (filename_tex) {
		MTM_ASSERT(strlen(filename_tex) < sizeof(sFile));
		strcpy(sFile, filename_tex);
		p_filename = sFile;
	}
	else {
		strcpy(sFile, "");
	}
	if (amb_tex) {
		char	*file_id = (char *)amb_tex;
		if (strncmp(file_id + 1, "AMB", 3)) {
			MTM_ASSERT(!"objObjectLoad::ObjAction3dNNModelLoadTxb() Error! amb_tex file type error\n");
			return;
		}
		// 変換済みチェック
		if (*file_id != AMD_CONVERTED_MARK) {
			amBindConv((u8*)amb_tex);
		}
	}
#if 0
//Sint32 amObjectLoad(NNS_OBJECT **object, NNS_TEXLIST **texlist,
//		void **texlistbuf, void *buf, NNF_DRAWOBJ drawflag, char *filepath, AMS_AMB_HEADER *amb)
	{
		NNS_OBJECT		*obj_file;
		NNS_TEXFILELIST	*texfilelist;
		Sint32			num, reg_index;

		amObjectSetup(&obj_file, &texfilelist, model);

		// 追加行
		// amTxbから取得したtexfilelist
		amTxbConv((u8*)txb);
		texfilelist =  amTxbGetTexFileList(txb);

		num		= texfilelist->nTex;
		obj_3d->texlistbuf		= amMemAlloc((Uint32)nnEstimateTexlistSize(num));
		nnSetUpTexlist(&obj_3d->texlist, num, obj_3d->texlistbuf);

		reg_index	= amObjectLoad(&obj_3d->object, obj_file, drawflag | g_obj.load_drawflag);
		if ((sFile != NULL) || (amb_tex != NULL))
			obj_3d->reg_index	= amTextureLoad(obj_3d->texlist, texfilelist, sFile, (AMS_AMB_HEADER*)amb_tex);

	}
#else
	amTxbConv((u8*)txb);
	obj_3d->reg_index = amObjectLoad(&obj_3d->object, amTxbGetTexFileList(txb), &obj_3d->texlist,
						&obj_3d->texlistbuf, model, drawflag | g_obj.load_drawflag,
						p_filename, (AMS_AMB_HEADER*)amb_tex);
#endif

#if OBD_LOAD_INITIAL_DRAW
	if (obj_load_initial_set_flag) {
		OBS_LOAD_INITIAL_WORK* work = &obj_load_initial_work;
		MTM_ASSERT(work->obj_num < OBD_LOAD_INITIAL_OBJECT_MAX);
		if (work->obj_num < OBD_LOAD_INITIAL_OBJECT_MAX) {
			// 使用オブジェクト登録
			work->obj_3d[work->obj_num] = obj_3d;
			++work->obj_num;
		}
	}
#endif // OBD_LOAD_INITIAL_DRAW

	obj_3d->flag |= OBD_ACTFLAG_3D_NN_REG_WAIT;
	obj_3d->flag &= ~OBD_ACTFLAG_3D_NN_REG_FINISH;
}

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
void ObjObjectAction3dNNModelLoadTxb(OBS_OBJECT_WORK *obj_work, OBS_ACTION3D_NN_WORK *obj_3d,
									OBS_DATA_WORK *data_work, const char *filename, s32 index, void *archive,
									const char *filename_tex, void *amb_tex, NNF_DRAWOBJ drawflag, void *txb)
{
	MTM_ASSERT(obj_work);

	// 表示ワークチェック
	if (obj_3d == NULL) {
		if (obj_work->obj_3d) {
			obj_3d = obj_work->obj_3d;
		}
		else {
			obj_3d = (OBS_ACTION3D_NN_WORK*)amMemAlloc(sizeof(OBS_ACTION3D_NN_WORK));
		}
		amZeroMemory(obj_3d, sizeof(OBS_ACTION3D_NN_WORK));
		// 解放設定
		obj_work->flag |= OBD_OBJECT_FREE_3D;
	}

	// 3D描画ワークを設定
	obj_work->obj_3d = obj_3d;

	amAssert(obj_3d);

	// 3Dアクション初期化
	ObjAction3dNNModelLoadTxb(obj_3d,
				data_work, filename, index, archive,
				filename_tex, amb_tex, drawflag, txb);
}

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
void ObjAction3dNNMotionLoad(OBS_ACTION3D_NN_WORK *obj_3d, s32 reg_file_id, BOOL marge,
									OBS_DATA_WORK *data_work, const char *filename, s32 index, void *archive,
									s32 motion_num/*=AMD_MOTION_DEFAULT_MAX*/, s32 mmotion_num/*=AMD_MOTION_MATERIAL_DEFAULT_MAX*/)
{
	void				*mtn = NULL;

	MTM_ASSERT(obj_3d);
	MTM_ASSERT((u32)reg_file_id < AMD_MOTION_FILE_MAX);

	if (!(obj_3d->flag & OBD_ACTFLAG_3D_NN_REG_FINISH)) {
		// モーションデータ読み込み待機
		OBS_ACTION3D_MTN_LOAD_SETTING	*load_setting;

		obj_3d->flag |= OBD_ACTFLAG_3D_NN_REG_MTN_WAIT;

		load_setting = &obj_3d->mtn_load_setting[reg_file_id];

		MTM_ASSERT(load_setting->enable == FALSE);
		//MTM_ASSERT(strlen(filename) < OBD_ACTION3D_NN_MTN_FILENAME_LEN);

		load_setting->enable	= TRUE;
		load_setting->marge		= marge;
		load_setting->data_work	= data_work;
		amZeroMemory(load_setting->filename, OBD_ACTION3D_NN_MTN_FILENAME_LEN);
		if (filename) {
			strncpy(load_setting->filename, filename, OBD_ACTION3D_NN_MTN_FILENAME_LEN - 1);
		}
		load_setting->index		= index;
		load_setting->archive	= archive;

		return;
	}

	if (archive) {
		obj_3d->flag |= (OBD_ACTFLAG_3D_NN_ARCHIVE_MTN_0 << reg_file_id);
	}

	// モーションファイルの読み込み
	if (filename && *filename != '\0') {
		mtn = ObjDataLoad(data_work, filename, archive);
		if (archive && mtn == NULL) {
			// アーカイブにデータがなかった
			obj_3d->flag &= ~(OBD_ACTFLAG_3D_NN_ARCHIVE_MTN_0 << reg_file_id);
			mtn = ObjDataLoad(data_work, filename, NULL);
		}
	}
	else if (archive) {
		mtn = ObjDataLoadAmbIndex(data_work, index, archive);
		if (archive && mtn == NULL) {
			// アーカイブにデータがなかった
			obj_3d->flag &= ~(OBD_ACTFLAG_3D_NN_ARCHIVE_MTN_0 << reg_file_id);
		}
	}
	else if (data_work) {
		mtn = ObjDataGetInc(data_work);
	}

	if (mtn == NULL) {
		amAssert(!"objObjectLoad::ObjObjectAction3dMotionLoad() error! no motion\n");
		return;
	}

	// モーションデータ保存
	MTM_ASSERT(obj_3d->mtn[reg_file_id] == NULL);
	obj_3d->mtn[reg_file_id] = mtn;

	// データワーク保存
	if (data_work) {
		MTM_ASSERT(obj_3d->mtn_data_work[reg_file_id] == NULL);
		obj_3d->mtn_data_work[reg_file_id] = data_work;
	}

	// モーションセットアップ
	if (obj_3d->motion == NULL) {
		//obj_3d->motion = amMotionCreate(obj_3d->object, marge ? AMD_MOTION_CREATE_FLAG_MARGE : 0);
		obj_3d->motion = amMotionCreate(obj_3d->object, motion_num, mmotion_num, marge ? AMD_MOTION_CREATE_FLAG_MARGE : 0);
	}

	// mtnがambかモーション直データか判定して呼び出し
	{
		AMS_AMB_HEADER	*amb_header = (AMS_AMB_HEADER*)mtn;
		if (strncmp(&amb_header->file_id[1], "AMB", 3)) {
		// モーションデータ
			amMotionRegistFile(obj_3d->motion, reg_file_id, mtn);
		}
		else {
		// AMBファイル
			// 変換済みチェック
			if (amb_header->file_id[0] != AMD_CONVERTED_MARK) {
				amBindConv((u8*)amb_header);
			}
			amMotionRegistFile(obj_3d->motion, reg_file_id, amb_header);
		}
	}
}

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
	@param	archive			[in]	モーションデータを含むアーカイブ

	@note
		filename が有効な場合は filename、無効の場合はindexでオブジェクトファイルをambから取得します。\n
		amb_tex が無効の場合は、filename_texの読み込みが終了するまで処理をロックします。\n
		filename_tex, amb_tex のどちらかが必ず有効であるようにして下さい。
 */
// ================================================================
void ObjObjectAction3dNNMotionLoad(OBS_OBJECT_WORK *obj_work, s32 reg_file_id, BOOL marge,
									OBS_DATA_WORK *data_work, const char *filename, s32 index, void *archive,
									s32 motion_num/*=AMD_MOTION_DEFAULT_MAX*/, s32 mmotion_num/*=AMD_MOTION_MATERIAL_DEFAULT_MAX*/)
{

	OBS_ACTION3D_NN_WORK	*obj_3d;

	amAssert(obj_work);
	amAssert(obj_work->obj_3d);
	amAssert((u32)reg_file_id < AMD_MOTION_FILE_MAX);

	obj_3d = obj_work->obj_3d;

	// 3Dモーションデータ読み込み
	ObjAction3dNNMotionLoad(obj_3d, reg_file_id, marge,
							data_work, filename, index, archive,
							motion_num, mmotion_num);
}

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
void ObjAction3dNNMaterialMotionLoad(OBS_ACTION3D_NN_WORK *obj_3d, s32 reg_file_id,
									OBS_DATA_WORK *data_work, const char *filename, s32 index, void *archive,
									s32 motion_num/*=AMD_MOTION_DEFAULT_MAX*/, s32 mmotion_num/*=AMD_MOTION_MATERIAL_DEFAULT_MAX*/)
{
	void				*mtn = NULL;

	MTM_ASSERT(obj_3d);
	MTM_ASSERT((u32)reg_file_id < AMD_MOTION_FILE_MAX);

	if (!(obj_3d->flag & OBD_ACTFLAG_3D_NN_REG_FINISH)) {
		// モーションデータ読み込み待機
		OBS_ACTION3D_MTN_LOAD_SETTING	*load_setting;

		obj_3d->flag |= OBD_ACTFLAG_3D_NN_REG_MATMTN_WAIT;

		load_setting = &obj_3d->mat_mtn_load_setting[reg_file_id];

		MTM_ASSERT(load_setting->enable == FALSE);
		//MTM_ASSERT(strlen(filename) < OBD_ACTION3D_NN_MTN_FILENAME_LEN);

		load_setting->enable	= TRUE;
		load_setting->marge		= FALSE;
		load_setting->data_work	= data_work;
		amZeroMemory(load_setting->filename, OBD_ACTION3D_NN_MTN_FILENAME_LEN);
		if (filename) {
			strncpy(load_setting->filename, filename, OBD_ACTION3D_NN_MTN_FILENAME_LEN - 1);
		}
		load_setting->index		= index;
		load_setting->archive	= archive;

		return;
	}

	if (archive) {
		obj_3d->flag |= (OBD_ACTFLAG_3D_NN_ARCHIVE_MATMTN_0 << reg_file_id);
	}

	// モーションファイルの読み込み
	if (filename && *filename != '\0') {
		mtn = ObjDataLoad(data_work, filename, archive);
		if (archive && mtn == NULL) {
			// アーカイブにデータがなかった
			obj_3d->flag &= ~(OBD_ACTFLAG_3D_NN_ARCHIVE_MATMTN_0 << reg_file_id);
			mtn = ObjDataLoad(data_work, filename, NULL);
		}
	}
	else if (archive) {
		mtn = ObjDataLoadAmbIndex(data_work, index, archive);
		if (archive && mtn == NULL) {
			// アーカイブにデータがなかった
			obj_3d->flag &= ~(OBD_ACTFLAG_3D_NN_ARCHIVE_MATMTN_0 << reg_file_id);
		}
	}
	else if (data_work) {
		mtn = ObjDataGetInc(data_work);
	}

	if (mtn == NULL) {
		amAssert(!"objObjectLoad::ObjAction3dNNMaterialMotionLoad() error! no material motion\n");
		return;
	}

	// モデルデータ保存
	MTM_ASSERT(obj_3d->mat_mtn[reg_file_id] == NULL);
	obj_3d->mat_mtn[reg_file_id] = mtn;

	// データワーク保存
	if (data_work) {
		MTM_ASSERT(obj_3d->mat_mtn_data_work[reg_file_id] == NULL);
		obj_3d->mat_mtn_data_work[reg_file_id] = data_work;
	}

	// モーションセットアップ
	if (obj_3d->motion == NULL) {
		//obj_3d->motion = amMotionCreate(obj_3d->object, marge ? AMD_MOTION_CREATE_FLAG_MARGE : 0);
		obj_3d->motion = amMotionCreate(obj_3d->object, motion_num, mmotion_num, FALSE);
	}

	// mtnがambかモーション直データか判定して呼び出し
	{
		AMS_AMB_HEADER	*amb_header = (AMS_AMB_HEADER*)mtn;
		if (strncmp(&amb_header->file_id[1], "AMB", 3)) {
		// モーションデータ
			amMotionMaterialRegistFile(obj_3d->motion, reg_file_id, mtn);
		}
		else {
		// AMBファイル
			// 変換済みチェック
			if (amb_header->file_id[0] != AMD_CONVERTED_MARK) {
				amBindConv((u8*)amb_header);
			}
			amMotionMaterialRegistFile(obj_3d->motion, reg_file_id, amb_header);
		}
	}
}

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
void ObjObjectAction3dNNMaterialMotionLoad(OBS_OBJECT_WORK *obj_work, s32 reg_file_id,
									OBS_DATA_WORK *data_work, const char *filename, s32 index, void *archive,
									s32 motion_num/*=AMD_MOTION_DEFAULT_MAX*/, s32 mmotion_num/*=AMD_MOTION_MATERIAL_DEFAULT_MAX*/)
{

	OBS_ACTION3D_NN_WORK	*obj_3d;

	amAssert(obj_work);
	amAssert(obj_work->obj_3d);
	amAssert((u32)reg_file_id < AMD_MOTION_FILE_MAX);

	obj_3d = obj_work->obj_3d;

	// 3Dモーションデータ読み込み
	ObjAction3dNNMaterialMotionLoad(obj_3d, reg_file_id,
							data_work, filename, index, archive,
							motion_num, mmotion_num);
}

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
BOOL ObjAction3dNNModelLoadCheck(OBS_ACTION3D_NN_WORK *obj_3d)
{
	MTM_ASSERT(obj_3d);

	if (obj_3d->flag & OBD_ACTFLAG_3D_NN_REG_WAIT) {
		if (amDrawIsRegistComplete(obj_3d->reg_index)) {
			// 読み込み終了
			obj_3d->flag &= ~OBD_ACTFLAG_3D_NN_REG_WAIT;
			obj_3d->flag |= OBD_ACTFLAG_3D_NN_REG_FINISH;
			obj_3d->reg_index = -1;

			// 初期化後はモデルを参照しなくなるので解放しておく
			if (obj_3d->model_data_work) {
				ObjDataRelease(obj_3d->model_data_work);
				obj_3d->model_data_work = NULL;
			}
			else {
				if (obj_3d->model && !(obj_3d->flag & OBD_ACTFLAG_3D_NN_ARCHIVE)) {
#if !_WII
					mtMemFreeMain(obj_3d->model);
#endif
				}
			}
			obj_3d->flag &= ~OBD_ACTFLAG_3D_NN_ARCHIVE;
			obj_3d->model = NULL;

			// モーション読み込み待機チェック
			if (obj_3d->flag & OBD_ACTFLAG_3D_NN_REG_MTN_WAIT) {
				s32								i;
				OBS_ACTION3D_MTN_LOAD_SETTING	*load_setting;
				for (i = 0, load_setting = &obj_3d->mtn_load_setting[0]; i < AMD_MOTION_FILE_MAX; i++, load_setting++) {
					if (!load_setting->enable) {
						continue;
					}

					ObjAction3dNNMotionLoad(obj_3d, i, load_setting->marge,
									load_setting->data_work, load_setting->filename, load_setting->index, load_setting->archive);

					load_setting->enable = FALSE;
				}

				obj_3d->flag &= ~OBD_ACTFLAG_3D_NN_REG_MTN_WAIT;
			}
			// マテリアルモーション読み込み待機チェック
			if (obj_3d->flag & OBD_ACTFLAG_3D_NN_REG_MATMTN_WAIT) {
				s32								i;
				OBS_ACTION3D_MTN_LOAD_SETTING	*load_setting;
				for (i = 0, load_setting = &obj_3d->mat_mtn_load_setting[0]; i < AMD_MOTION_FILE_MAX; i++, load_setting++) {
					if (!load_setting->enable) {
						continue;
					}

					ObjAction3dNNMaterialMotionLoad(obj_3d, i,
									load_setting->data_work, load_setting->filename, load_setting->index, load_setting->archive);

					load_setting->enable = FALSE;
				}

				obj_3d->flag &= ~OBD_ACTFLAG_3D_NN_REG_MATMTN_WAIT;
			}
			return (TRUE);
		}
	}
	else if (obj_3d->flag & OBD_ACTFLAG_3D_NN_REG_FINISH) {
		return (TRUE);
	}

	return (FALSE);
}


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
void ObjAction3dNNModelRelease(OBS_ACTION3D_NN_WORK *obj_3d)
{
	obj_3d->reg_index = amObjectRelease(obj_3d->object, obj_3d->texlist);
	obj_3d->flag |= OBD_ACTFLAG_3D_NN_RELEASE_WAIT;
}

// ================================================================
// ObjAction3dNNModelReleaseCheck
/*!
	3Dモデルデータ開放終了チェック

	@param	obj_3d			[in]	3Dオブジェクトワークポインタ

	@return	TRUE : 開放終了

	@note
		オブジェクトの開放終了をチェックし、object, texlistbufを\n
		メモリから開放します。
 */
// ================================================================
BOOL ObjAction3dNNModelReleaseCheck(OBS_ACTION3D_NN_WORK *obj_3d)
{
	if (!(obj_3d->flag & (OBD_ACTFLAG_3D_NN_RELEASE_WAIT | OBD_ACTFLAG_3D_NN_REG_FINISH))) {
		return (TRUE);
	}
	if (amDrawIsRegistComplete(obj_3d->reg_index)) {
		// オブジェクト解放
		if (obj_3d->object) {
#if !_WII
			mtMemFreeMain(obj_3d->object);
#endif
			obj_3d->object = NULL;
		}
		// テクスチャリストバッファ開放
		if (obj_3d->texlistbuf) {
			mtMemFreeMain(obj_3d->texlistbuf);
			obj_3d->texlistbuf = NULL;
		}
		obj_3d->flag &= ~(OBD_ACTFLAG_3D_NN_RELEASE_WAIT | OBD_ACTFLAG_3D_NN_REG_FINISH);

		// モデルデータ開放
		if (obj_3d->model_data_work) {
			ObjDataRelease(obj_3d->model_data_work);
			obj_3d->model_data_work = NULL;
		}
		else {
			if (obj_3d->model && !(obj_3d->flag & OBD_ACTFLAG_3D_NN_ARCHIVE)) {
#if !_WII
				mtMemFreeMain(obj_3d->model);
#endif
				obj_3d->model = NULL;
			}
		}
		obj_3d->flag &= ~OBD_ACTFLAG_3D_NN_ARCHIVE;
		return (TRUE);
	}
	return (FALSE);
}

// ================================================================
// ObjAction3dNNMotionRelease
/*!
	3Dモーションデータ開放

	@param	obj_3d			[in]	3Dオブジェクトワークポインタ
 */
// ================================================================
void ObjAction3dNNMotionRelease(OBS_ACTION3D_NN_WORK *obj_3d)
{
	s32	i;

	// モーション解放
	if (obj_3d->motion) {
		amMotionDelete(obj_3d->motion);
		obj_3d->motion = NULL;
	}
	// 共用解放
	// モーション
	// マテリアルモーション
	for (i = 0; i < AMD_MOTION_FILE_MAX; i++) {
		// モーション
		if (obj_3d->mtn_data_work[i]) {
			ObjDataRelease(obj_3d->mtn_data_work[i]);
			obj_3d->mtn_data_work[i] = NULL;
		}
		else {
			if (obj_3d->mtn[i] && !(obj_3d->flag & (OBD_ACTFLAG_3D_NN_ARCHIVE_MTN_0 << i))) {
				mtMemFreeMain(obj_3d->mtn[i]);
			}
		}
		obj_3d->flag &= ~(OBD_ACTFLAG_3D_NN_ARCHIVE_MTN_0 << i);
		obj_3d->mtn[i] = NULL;

		// マテリアルモーション
		if (obj_3d->mat_mtn_data_work[i]) {
			ObjDataRelease(obj_3d->mat_mtn_data_work[i]);
			obj_3d->mat_mtn_data_work[i] = NULL;
		}
		else {
			if (obj_3d->mat_mtn[i] && !(obj_3d->flag & (OBD_ACTFLAG_3D_NN_ARCHIVE_MATMTN_0 << i))) {
				mtMemFreeMain(obj_3d->mat_mtn[i]);
			}
		}
		obj_3d->flag &= ~(OBD_ACTFLAG_3D_NN_ARCHIVE_MATMTN_0 << i);
		obj_3d->mat_mtn[i] = NULL;
	}
}

#endif // #if OBD_USE_ACTION3D_NN



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
void ObjAction3dESEffectLoad(OBS_ACTION3D_ES_WORK *obj_3des,
							 OBS_DATA_WORK *data_work, const char *filename, s32 index, void *archive,
							 Sint32 user_attr/*=0*/, Sint32 ecb_prio/*=0*/)
{
	void	*eff	= NULL;
	
	MTM_ASSERT(obj_3des != NULL);
	MTM_ASSERT(obj_3des->eff == NULL);
	
	// 描画コマンド発行時 ステート
	obj_3des->command_state = OBD_DRAW_CMD_STATE_3DNN;	// 標準設定
	
	// エフェクトパラメータ初期化
	obj_3des->user_attr	= user_attr;
	
	// 更新速度初期化
	obj_3des->speed	= 1.0f;
	
	// アーカイブ指定チェック
	if (archive) {
		obj_3des->flag	|= OBD_ACTFLAG_3D_ES_EFF_ARCHIVE;
	}
	
	
	// ESエフェクトファイル読み込み
	if (filename) {
		eff	= ObjDataLoad(data_work, filename, archive);
		
		if ((archive) && (eff == NULL)) {
			// アーカイブにデータが無かった場合は
			obj_3des->flag	&= ~OBD_ACTFLAG_3D_ES_EFF_ARCHIVE;
			eff	= ObjDataLoad(data_work, filename, NULL);
		}
	}
	else if (archive) {
		eff	= ObjDataLoadAmbIndex(data_work, index, archive);
		if (eff == NULL) {
			// アーカイブにデータが無かった
			obj_3des->flag	&= ~OBD_ACTFLAG_3D_ES_EFF_ARCHIVE;
		}
	}
	else if (data_work) {
		eff	= ObjDataGetInc(data_work);
	}
	
	
	if (eff == NULL) {
		// ESエフェクトデータ読み込めなかった
		amAssert(!"objObjectLoad::ObjAction3dESEffectLoad() Error! no eff\n");
		return;
	}
	
	// AMEアドレス変換
	{
		AMS_AME_HEADER	*ame_header	= (AMS_AME_HEADER*)eff;
		
		MTM_ASSERT(eff);
		
		// フォーマットチェック
		if (strncmp((char*)&ame_header->file_id[1], "AME", 3)) {	// ファイルIDの1文字目はコンバート済みチェック用なので比較しない
			MTM_ASSERT(!"objObjectLoad::ObjAction3dESEffectLoad() Error! ame file type error\n");
			return;
		}
		
		// AMEアドレス変換（変換済みなら何もしない）
		amConvertAddress(eff);
	}
	
	
	// ESエフェクトデータ保存
	obj_3des->eff	= eff;
	
	// データワーク保存
	if (data_work) {
		obj_3des->eff_data_work	= data_work;
	}
	
	MTM_ASSERT(obj_3des->ecb == NULL);
	// エフェクト作成
	obj_3des->ecb	= amEffectCreate((AMS_AME_HEADER*)obj_3des->eff,
									 user_attr,
									 ecb_prio);
}

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
void ObjObjectAction3dESEffectLoad(OBS_OBJECT_WORK *obj_work, OBS_ACTION3D_ES_WORK *obj_3des,
								   OBS_DATA_WORK *data_work, const char *filename, s32 index, void *archive,
								   Sint32 user_attr/*=0*/, Sint32 ecb_prio/*=0*/)
{
	MTM_ASSERT(obj_work);
	
	// 表示ワークチェック
	if (obj_3des == NULL) {
		if (obj_work->obj_3des) {
			obj_3des	= obj_work->obj_3des;
		}
		else {
			obj_3des	= (OBS_ACTION3D_ES_WORK*)amMemAlloc(sizeof(OBS_ACTION3D_ES_WORK));
		}
		amZeroMemory(obj_3des, sizeof(OBS_ACTION3D_ES_WORK));
		// 解放設定
		obj_work->flag	|= OBD_OBJECT_FREE_3DES;
	}
	
	// 3DESエフェクト描画ワークを設定
	obj_work->obj_3des	= obj_3des;
	
	amAssert(obj_3des);
	
	// 3DESエフェクト初期化
	ObjAction3dESEffectLoad(obj_3des,
							data_work, filename, index, archive, user_attr, ecb_prio);
}

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
void ObjAction3dESEffectRelease(OBS_ACTION3D_ES_WORK *obj_3des)
{
	MTM_ASSERT(obj_3des);
	MTM_ASSERT(obj_3des->ecb);
	
	amEffectDelete(obj_3des->ecb);
	obj_3des->ecb	= NULL;
}

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
void ObjAction3dESTextureLoad(OBS_ACTION3D_ES_WORK *obj_3des,
							  OBS_DATA_WORK *data_work, const char *filename, s32 index, void *archive,
							  BOOL load_tex/*=FALSE*/)
{
	void	*amb_tex	= NULL;
	void	*txb	= NULL;
	
	MTM_ASSERT(obj_3des != NULL);
	MTM_ASSERT(obj_3des->ambtex == NULL);
	
	// アーカイブ指定チェック
	if (archive) {
		obj_3des->flag	|= OBD_ACTFLAG_3D_ES_AMBTEX_ARCHIVE;
	}
	
	/* テクスチャ読み込み */
	if (filename) {
		amb_tex	= ObjDataLoad(data_work, filename, archive);
		
		if ((archive) && (amb_tex == NULL)) {
			// アーカイブにデータが無かった場合は
			obj_3des->flag	&= ~OBD_ACTFLAG_3D_ES_AMBTEX_ARCHIVE;
			amb_tex	= ObjDataLoad(data_work, filename, NULL);
		}
	}
	else if (archive) {
		amb_tex	= ObjDataLoadAmbIndex(data_work, index, archive);
		if (amb_tex == NULL) {
			// アーカイブにデータが無かった
			obj_3des->flag	&= ~OBD_ACTFLAG_3D_ES_AMBTEX_ARCHIVE;
		}
	}
	else if (data_work) {
		amb_tex	= ObjDataGetInc(data_work);
	}
	
	
	if (amb_tex == NULL) {
		// ESエフェクトデータ読み込めなかった
		amAssert(!"objObjectLoad::ObjAction3dESTextureLoad() Error! no amb_tex\n");
		return;
	}
	
	
	// AMBアドレス変換
	{
		AMS_AMB_HEADER	*amb_header	= (AMS_AMB_HEADER*)amb_tex;
		
		MTM_ASSERT(amb_tex);
		
		// フォーマットチェック
		if (strncmp(&amb_header->file_id[1], "AMB", 3)) {	// ファイルIDの1文字目はコンバート済みチェック用なので比較しない
			MTM_ASSERT(!"objObjectLoad::ObjAction3dESTextureLoad() Error! amb_tex file type error\n");
			return;
		}
		
		// アドレス変換（変換済みなら何もしない）
		amConvertAddress(amb_tex);
	}
	
	// テクスチャAMBデータ保存
	obj_3des->ambtex	= amb_tex;
	
	// データワーク保存
	if (data_work) {
		obj_3des->ambtex_data_work	= data_work;
	}
	
	
	if (load_tex) {
		// テクスチャファイルリスト(TXB)取得
		// （テクスチャファイルリストはAMBの先頭に配置されている前提）
		txb	= amBindGet((AMS_AMB_HEADER*)amb_tex, 0);
		
		// TXBアドレス変換（変換済みなら何もしない）
		amConvertAddress(txb);
		
		
		// テクスチャリスト初期化
		{
			Uint32		tex_num;
			
			tex_num	= amTxbGetCount(txb);
			obj_3des->texlistbuf	= amMemAlloc(nnEstimateTexlistSize(tex_num));
			nnSetUpTexlist(&obj_3des->texlist,	// テクスチャリストが出力される
						   tex_num,
						   obj_3des->texlistbuf);
#if OBD_LOAD_INITIAL_DRAW
			if (obj_load_initial_set_flag) {
				OBS_LOAD_INITIAL_WORK* work = &obj_load_initial_work;
				MTM_ASSERT(work->es_num < OBD_LOAD_INITIAL_OBJECT_MAX);
				if (work->es_num < OBD_LOAD_INITIAL_OBJECT_MAX) {
					// 使用エフェクト登録
					work->obj_3des[work->es_num] = obj_3des;
					++work->es_num;
				}
			}
#endif // OBD_LOAD_INITIAL_DRAW
		}
		
		// テクスチャロード開始（コマンド発行有り）
		MTM_ASSERT(amb_tex);
		obj_3des->tex_reg_index	= amTextureLoad(obj_3des->texlist,
												amTxbGetTexFileList(txb),
												const_cast<char*>(filename),
												(AMS_AMB_HEADER*)amb_tex);
		obj_3des->flag	|= OBD_ACTFLAG_3D_ES_REG_TEX_WAIT;
	}
}


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
void ObjObjectAction3dESTextureLoad(OBS_OBJECT_WORK *obj_work, OBS_ACTION3D_ES_WORK *obj_3des,
									OBS_DATA_WORK *data_work, const char *filename, s32 index, void *archive,
									BOOL load_tex/*=FALSE*/)
{
	UNREFERENCED_PARAMETER(obj_work);
	amAssert(obj_work);
	amAssert(obj_work->obj_3des);
	
	// ESテクスチャデータの読み込み
	ObjAction3dESTextureLoad(obj_3des,
							 data_work, filename, index, archive, load_tex);
}

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
void ObjAction3dESTextureRelease(OBS_ACTION3D_ES_WORK *obj_3des)
{
	if (obj_3des->texlist_data_work) {
		// データワーク使用時
		obj_3des->tex_reg_index	=
			ObjAction3dESTextureReleaseDwork(obj_3des->texlist_data_work);
	}
	else {
		// データワーク不使用
		obj_3des->tex_reg_index	= amTextureRelease(obj_3des->texlist);
	}
	
	// 待ち必要かチェック
	if (obj_3des->tex_reg_index != -1) {
		obj_3des->flag	|= OBD_ACTFLAG_3D_ES_REG_TEX_WAIT;
	}
	else {
		// 待ち必要なければこの時点で参照をクリアしておく
		obj_3des->texlist_data_work	= NULL;
		obj_3des->texlist		= NULL;
		obj_3des->texlistbuf	= NULL;
	}
}

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
BOOL ObjAction3dESTextureReleaseCheck(OBS_ACTION3D_ES_WORK *obj_3des)
{
	if (!(obj_3des->flag & OBD_ACTFLAG_3D_ES_REG_TEX_WAIT)) {
		
		MTM_ASSERT(obj_3des->texlist_data_work == NULL);
		MTM_ASSERT(obj_3des->texlist == NULL);
		MTM_ASSERT(obj_3des->texlistbuf == NULL);
		
		// 解放済み
		return TRUE;
	}
	
	MTM_ASSERT(obj_3des->tex_reg_index != -1);
	
	if (obj_3des->texlist_data_work) {
		// データワーク使用時
		if (ObjAction3dESTextureReleaseDworkCheck(obj_3des->texlist_data_work,
												  obj_3des->tex_reg_index)) {
			obj_3des->texlist_data_work	= NULL;
			obj_3des->texlist		= NULL;
			obj_3des->texlistbuf	= NULL;
			
			obj_3des->tex_reg_index	= -1;
			obj_3des->flag	&= ~OBD_ACTFLAG_3D_ES_REG_TEX_WAIT;
			
			return TRUE;
		}
	}
	else {
		// データワーク不使用時
		if (amDrawIsRegistComplete(obj_3des->tex_reg_index)) {
			
			mtMemFreeMain(obj_3des->texlistbuf);
			obj_3des->texlist		= NULL;
			obj_3des->texlistbuf	= NULL;
			
			obj_3des->tex_reg_index	= -1;
			obj_3des->flag	&= ~OBD_ACTFLAG_3D_ES_REG_TEX_WAIT;
			
			return TRUE;
		}
	}
	
	return FALSE;
}

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
  実際のロードが発生しない場合は*texlist_bufにはNULLが設定されます。
  amDrawIsRegistComplete()で完了チェックを行ってください。
  テクスチャをVRAMに常駐させて共有したい場合などに利用してください。
 */
// =======================================================================
Sint32 ObjAction3dESTextureLoadToDwork(OBS_DATA_WORK *texlist_dwork, void *amb_tex, void **texlist_buf)
{
	void	*txb	= NULL;
	Sint32	tex_reg_index;
	NNS_TEXLIST	*texlist;
	
	MTM_ASSERT(texlist_buf);
	MTM_ASSERT(texlist_dwork);
	
	
	if (texlist_dwork->pData == NULL) {
		MTM_ASSERT(amb_tex);
		
		// AMBアドレス変換
		{
			AMS_AMB_HEADER	*amb_header	= (AMS_AMB_HEADER*)amb_tex;
			
			MTM_ASSERT(amb_tex);
			
			// フォーマットチェック
			if (strncmp(&amb_header->file_id[1], "AMB", 3)) {	// ファイルIDの1文字目はコンバート済みチェック用なので比較しない
				MTM_ASSERT(!"objObjectLoad::ObjAction3dESTextureLoadToDwork() Error! amb_tex file type error\n");
				return -1;
			}
			
			// アドレス変換（変換済みなら何もしない）
			amConvertAddress(amb_tex);
		}
		
		// テクスチャファイルリスト(TXB)取得
		// （テクスチャファイルリストはAMBの先頭に配置されている前提）
		txb	= amBindGet((AMS_AMB_HEADER*)amb_tex, 0);
		
		
		// TXBアドレス変換（変換済みなら何もしない）
		amConvertAddress(txb);
		
		
		// テクスチャリスト初期化
		{
			Uint32		tex_num;
			
			tex_num	= amTxbGetCount(txb);
			*texlist_buf	= amMemAlloc(nnEstimateTexlistSize(tex_num));
			nnSetUpTexlist(&texlist,	// テクスチャリストが出力される
						   tex_num,
						   *texlist_buf);
		}
		
		// テクスチャロード開始（コマンド発行有り）
		MTM_ASSERT(amb_tex);
		tex_reg_index	= amTextureLoad(texlist,
										amTxbGetTexFileList(txb),
										NULL,	// amb_texを指定しているのでfilepath不要
										(AMS_AMB_HEADER*)amb_tex);
		
		// データワークにセット
		ObjDataSet(texlist_dwork, (void*)texlist);
	}
	else {
		MTM_ASSERT(texlist_dwork->num > 0);
		ObjDataGetInc(texlist_dwork);
		tex_reg_index	= -1;
		
		*texlist_buf	= NULL;
	}
	
	return tex_reg_index;
}

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
Sint32 ObjAction3dESTextureReleaseDwork(OBS_DATA_WORK *texlist_dwork)
{
	Sint32	tex_reg_index	= -1;
	
	if( texlist_dwork->num ){
        // データアドレスチェック
        if ( texlist_dwork->pData ){
            // 使用数デクリメント
			--texlist_dwork->num;
			
			if ( !texlist_dwork->num ){
                // 解放
				tex_reg_index	= amTextureRelease((NNS_TEXLIST*)texlist_dwork->pData);
            }
			MTM_ASSERT(texlist_dwork->num != OBD_DATA_ARCHIVE_FLAG);
        }
    }
	
	return tex_reg_index;
}

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
BOOL ObjAction3dESTextureReleaseDworkCheck(OBS_DATA_WORK *texlist_dwork, Sint32 reg_index)
{
	MTM_ASSERT(reg_index != -1);
	
	if (amDrawIsRegistComplete(reg_index)) {
		MTM_ASSERT(texlist_dwork->num == 0);
		mtMemFreeMain(texlist_dwork->pData);
		texlist_dwork->pData	= NULL;
		
		return TRUE;
	}
	
	return FALSE;
}

// =======================================================================
// ObjObjectAction3dESTextureSetByDwork
/*!
  登録済みESテクスチャセット
 
  @param obj_work		[io]	オブジェクトワーク
  @param texlist_dwork	[io]	テクスチャリストデータワーク
 
  @note
  データワークで管理されているテクスチャリストをオブジェクト3Dワークに設定します。
  事前にObjObjectAction3dESTextureLoad(..., load_tex=FALSE)を呼び出しておいてください。
  オブジェクトが破棄されるときに自動的に解放処理が行われます。
 */
// =======================================================================
void ObjObjectAction3dESTextureSetByDwork(OBS_OBJECT_WORK *obj_work, OBS_DATA_WORK *texlist_dwork)
{
	OBS_ACTION3D_ES_WORK	*obj_3des	= obj_work->obj_3des;
	void	*texlistbuf;
	
	MTM_ASSERT(obj_work->obj_3des);
	MTM_ASSERT(obj_3des->texlist == NULL);
	MTM_ASSERT(obj_3des->texlist_data_work == NULL);
	
	// 事前にObjObjectAction3dESTextureLoad()しておくこと
	MTM_ASSERT(obj_3des->ambtex);
	MTM_ASSERT(obj_3des->ambtex_data_work);
	
	// データワークロード（参照カウントインクリメント）
	ObjAction3dESTextureLoadToDwork(texlist_dwork, NULL, &texlistbuf);
	MTM_ASSERT(texlistbuf == NULL);
	
	// データワークセット
	obj_3des->texlist_data_work	= texlist_dwork;
	
	// データセット
	obj_3des->texlist		= (NNS_TEXLIST*)texlist_dwork->pData;
	obj_3des->texlistbuf	= texlist_dwork->pData;
}


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
void ObjAction3dESModelLoad(OBS_ACTION3D_ES_WORK *obj_3des,
							OBS_DATA_WORK *data_work, const char *filename, s32 index, void *archive, NNF_DRAWOBJ drawflag,
							BOOL load_model/*=FALSE*/)
{
	void	*model	= NULL;
	MTM_ASSERT(obj_3des);
	MTM_ASSERT(obj_3des->model == NULL);
	MTM_ASSERT(obj_3des->ecb);
	
	// アーカイブ指定チェック
	if (archive) {
		obj_3des->flag	|= OBD_ACTFLAG_3D_ES_MODEL_ARCHIVE;
	}
	
	// ESモデルファイル読み込み
	if (filename) {
		model	= ObjDataLoad(data_work, filename, archive);
		
		if ((archive) && (model == NULL)) {
			// アーカイブにデータが無かった場合
			obj_3des->flag	&= ~OBD_ACTFLAG_3D_ES_MODEL_ARCHIVE;
			model	= ObjDataLoad(data_work, filename, NULL);
		}
	}
	else if (archive) {
		model	= ObjDataLoadAmbIndex(data_work, index, archive);
		if (model == NULL) {
			// アーカイブにデータが無かった
			obj_3des->flag	&= ~OBD_ACTFLAG_3D_ES_MODEL_ARCHIVE;
		}
	}
	else if (data_work) {
		model	= ObjDataGetInc(data_work);
	}
	
	
	if (model == NULL) {
		// ESモデルデータ読み込めなかった
		amAssert(!"objObjectLoad::ObjAction3dESModelLoad() Error! no modeln");
		return;
	}
	
	// ESモデルデータ保存
	obj_3des->model	= model;
	
	// データワーク保存
	if (data_work) {
		obj_3des->model_data_work	= data_work;
	}
	
	
	if (load_model) {
		NNS_TEXLIST	*texlist	= NULL;
		void	*texlistbuf		= NULL;
		
		// モデルロード開始（コマンド発行有り）
		obj_3des->model_reg_index	= amObjectLoad(&obj_3des->object,
												   (NNS_TEXLIST**)&texlist,
												   &texlistbuf,
												   model,
												   drawflag | g_obj.load_drawflag,
												   NULL, NULL);
		
		// テクスチャリストは別途生成するので破棄
		MTM_ASSERT(texlistbuf);
		amMemFree(texlistbuf);	// texlistの実体はtexlistbufなので、texlistはfree不要
		// TODO : ↑バッファを無駄に確保することになるので、パフォーマンス的に問題がある場合は自前でamObjectSetup()＆amObjectLoad()するように変更する。
		//        （＝テクスチャバッファの確保とテクスチャリスト生成を迂回する。）
		
		obj_3des->flag	|= OBD_ACTFLAG_3D_ES_REG_MODEL_WAIT;
		
		// NNオブジェクトを関連付け
#if _IPHONE
		amEffectSetObject(obj_3des->ecb, obj_3des->object, OBD_DRAW_CMD_STATE_3DNN_POST);
#else
		amEffectSetObject(obj_3des->ecb, obj_3des->object, OBD_DRAW_CMD_STATE_3DNN);
#endif // _IPHONE
	}
}

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
void ObjObjectAction3dESModelLoad(OBS_OBJECT_WORK *obj_work, OBS_ACTION3D_ES_WORK *obj_3des,
								  OBS_DATA_WORK *data_work, const char *filename, s32 index, void *archive, NNF_DRAWOBJ drawflag,
								  BOOL load_model/*=FALSE*/)
{
	UNREFERENCED_PARAMETER(obj_work);
	MTM_ASSERT(obj_work);
	MTM_ASSERT(obj_work->obj_3des);
	
	// ESモデルデータの読み込み
	ObjAction3dESModelLoad(obj_3des,
						   data_work, filename, index, archive, drawflag, load_model);
}

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
void ObjAction3dESModelRelease(OBS_ACTION3D_ES_WORK *obj_3des)
{
	if (obj_3des->object_data_work) {
		// データワーク使用時
		obj_3des->model_reg_index	=
			ObjAction3dESModelReleaseDwork(obj_3des->object_data_work);
	}
	else {
		// データワーク不使用
		obj_3des->model_reg_index	= amObjectRelease(obj_3des->object);
	}
	
	// 待ち必要かチェック
	if (obj_3des->model_reg_index != -1) {
		obj_3des->flag	|= OBD_ACTFLAG_3D_ES_REG_MODEL_WAIT;
	}
	else {
		// 待ち必要でなければこの時点で参照をクリアしておく
		obj_3des->object_data_work	= NULL;
		obj_3des->object			= NULL;
	}
}

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
BOOL ObjAction3dESModelReleaseCheck(OBS_ACTION3D_ES_WORK *obj_3des)
{
	if (!(obj_3des->flag & OBD_ACTFLAG_3D_ES_REG_MODEL_WAIT)) {
		
		MTM_ASSERT(obj_3des->object_data_work == NULL);
		MTM_ASSERT(obj_3des->object == NULL);
		
		// 解放済み
		return TRUE;
	}
	
	MTM_ASSERT(obj_3des->model_reg_index != -1);
	
	if (obj_3des->object_data_work) {
		// データワーク使用時
		if (ObjAction3dESModelReleaseDworkCheck(obj_3des->object_data_work,
												obj_3des->model_reg_index)) {
			obj_3des->object_data_work	= NULL;
			obj_3des->object			= NULL;
			
			obj_3des->model_reg_index	= -1;
			obj_3des->flag	&= ~OBD_ACTFLAG_3D_ES_REG_MODEL_WAIT;
			
			return TRUE;
		}
	}
	else {
		// データワーク不使用時
		if (amDrawIsRegistComplete(obj_3des->model_reg_index)) {
#if !_WII
			mtMemFreeMain(obj_3des->object);
#endif
			obj_3des->object	= NULL;
			
			obj_3des->model_reg_index	= -1;
			obj_3des->flag	&= ~OBD_ACTFLAG_3D_ES_REG_MODEL_WAIT;
			
			return TRUE;
		}
	}
	
	return FALSE;
}

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
Sint32 ObjAction3dESModelLoadToDwork(OBS_DATA_WORK *object_dwork, void *model, NNF_DRAWOBJ drawflag)
{
	Sint32	model_reg_index;
	NNS_OBJECT	*object;
	
	MTM_ASSERT(object_dwork);
	
	if (object_dwork->pData == NULL) {
		NNS_TEXLIST	*texlist	= NULL;
		void	*texlistbuf		= NULL;
		
		MTM_ASSERT(model);
		
		// モデルロード開始（コマンド発行有り）
		model_reg_index	= amObjectLoad(&object,
									   &texlist,
									   &texlistbuf,
									   model,
									   drawflag | g_obj.load_drawflag,
									   NULL, NULL);
		
		// テクスチャリストは別途生成するので破棄
		MTM_ASSERT(texlistbuf);
		amMemFree(texlistbuf);	// texlistの実体はtexlistbufなので、texlistはfree不要
		// TODO : ↑バッファを無駄に確保することになるので、パフォーマンス的に問題がある場合は自前でamObjectSetup()＆amObjectLoad()するように変更する。
		//        （＝テクスチャバッファの確保とテクスチャリスト生成を迂回する。）
		
		// データワークにセット
		ObjDataSet(object_dwork, (void*)object);
	}
	else {
		MTM_ASSERT(object_dwork->num > 0);
		ObjDataGetInc(object_dwork);
		model_reg_index	= -1;
	}
	
	return model_reg_index;
}

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
Sint32 ObjAction3dESModelReleaseDwork(OBS_DATA_WORK *object_dwork)
{
	Sint32	model_reg_index	= -1;
	
	if( object_dwork->num ){
        // データアドレスチェック
        if ( object_dwork->pData ){
            // 使用数デクリメント
			--object_dwork->num;
			
			if ( !object_dwork->num ){
                // 解放
				model_reg_index	= amObjectRelease((NNS_OBJECT*)object_dwork->pData);
            }
			MTM_ASSERT(object_dwork->num != OBD_DATA_ARCHIVE_FLAG);
        }
    }
	
	return model_reg_index;
}


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
BOOL ObjAction3dESModelReleaseDworkCheck(OBS_DATA_WORK *object_dwork, Sint32 reg_index)
{
	MTM_ASSERT(reg_index != -1);
	
	if (amDrawIsRegistComplete(reg_index)) {
		MTM_ASSERT(object_dwork->num == 0);
#if !_WII
		mtMemFreeMain(object_dwork->pData);
#endif
		object_dwork->pData	= NULL;
		
		return TRUE;
	}
	
	return FALSE;
}

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
void ObjObjectAction3dESModelSetByDwork(OBS_OBJECT_WORK *obj_work, OBS_DATA_WORK *object_dwork)
{
	OBS_ACTION3D_ES_WORK	*obj_3des	= obj_work->obj_3des;
	
	MTM_ASSERT(obj_3des);
	MTM_ASSERT(obj_3des->ecb);
	MTM_ASSERT(obj_3des->object == NULL);
	MTM_ASSERT(obj_3des->object_data_work == NULL);
	
	// 事前にObjObjectAction3dESModelLoad()しておくこと
	MTM_ASSERT(obj_3des->model);
	MTM_ASSERT(obj_3des->model_data_work);
	
	// データワークロード（参照カウントインクリメント）
	ObjAction3dESModelLoadToDwork(object_dwork, NULL, 0);
	
	// データワークセット
	obj_3des->object_data_work	= object_dwork;
	
	// データセット
	obj_3des->object	= (NNS_OBJECT*)object_dwork->pData;
	
	// NNオブジェクトを関連付け
#if _IPHONE
	amEffectSetObject(obj_3des->ecb, obj_3des->object, OBD_DRAW_CMD_STATE_3DNN_POST);
#else
	amEffectSetObject(obj_3des->ecb, obj_3des->object, OBD_DRAW_CMD_STATE_3DNN);
#endif // _IPHONE
}

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
BOOL ObjAction3dESEffectLoadCheck(OBS_ACTION3D_ES_WORK *obj_3des)
{
	BOOL	result	= TRUE;
	
	MTM_ASSERT(obj_3des);
	
	// ESテクスチャ登録終了チェック
	if (obj_3des->flag & OBD_ACTFLAG_3D_ES_REG_TEX_WAIT) {
		if (amDrawIsRegistComplete(obj_3des->tex_reg_index)) {
			obj_3des->flag	&= ~OBD_ACTFLAG_3D_ES_REG_TEX_WAIT;
			obj_3des->tex_reg_index	= -1;
		}
		else {
			result	= FALSE;
		}
	}
	
	// ESモデル登録終了チェック
	if (obj_3des->flag & OBD_ACTFLAG_3D_ES_REG_MODEL_WAIT) {
		if (amDrawIsRegistComplete(obj_3des->model_reg_index)) {
			obj_3des->flag	&= ~OBD_ACTFLAG_3D_ES_REG_MODEL_WAIT;
			obj_3des->tex_reg_index	= -1;
		}
		else {
			result	= FALSE;
		}
	}
	
	return result;
}

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
	@param	amb_tex			[in]	テクスチャAMB
	@param	id				[in]	初期化するアクションorノードID
	@param	type_node		[in]	アクション生成タイプ TRUE : ノードタイプ  FALSE : アクションタイプ

	@note
 */
// ================================================================
void ObjAction2dAMALoad(OBS_ACTION2D_AMA_WORK *obj_2d,
									OBS_DATA_WORK *data_work, const char *filename, s32 index, void *archive,
									void *amb_tex, u32 id, BOOL type_node)
{
	//s32		i;
	void	*ama = NULL;
	//char	*p_filename = NULL;

	MTM_ASSERT(obj_2d);
	MTM_ASSERT(amb_tex);

	// 2Dオブジェクトワーク初期化
	ObjAction2dAMAWorkInit(obj_2d);

	if (archive) {
		obj_2d->flag |= OBD_ACTFLAG_2D_AMA_ARCHIVE;
	}

	// AMAファイル読み込み
	if (filename) {
		ama = ObjDataLoad(data_work, filename, archive);

		if (archive && ama == NULL) {
			// アーカイブにデータがなかった
			obj_2d->flag &= ~OBD_ACTFLAG_2D_AMA_ARCHIVE;
			ama = ObjDataLoad(data_work, filename, NULL);
		}
	}
	else if (archive) {
		ama = ObjDataLoadAmbIndex(data_work, index, archive);
		if (ama == NULL) {
			// アーカイブにデータがなかった
			obj_2d->flag &= ~OBD_ACTFLAG_2D_AMA_ARCHIVE;
		}
	}
	else if (data_work) {
		ama = ObjDataGetInc(data_work);
	}

	if (ama == NULL) {
		amAssert(!"objObjectLoad::ObjAction2dACELoad() error! no ama data\n");
		return;
	}

	// AMAデータ保存
	obj_2d->ama = ama;

	// データワーク保存
	if (data_work) {
		obj_2d->ama_data_work = data_work;
	}

	// 初期化タイプ保存
	obj_2d->type_node = type_node;

	// ID保存
	obj_2d->act_id = id;

	// テクスチャ読み込み
	{
		char	*file_id = (char *)amb_tex;
		if (strncmp(file_id + 1, "AMB", 3)) {
			MTM_ASSERT(!"objObjectLoad::ObjAction2dACELoad() Error! amb_tex file type error\n");
			return;
		}
		// 変換済みチェック
		if (*file_id != AMD_CONVERTED_MARK) {
			amBindConv((u8*)amb_tex);
		}
	}
	AoTexBuild(&obj_2d->ao_tex, amb_tex);
	AoTexLoad(&obj_2d->ao_tex);

	obj_2d->flag |= OBD_ACTFLAG_2D_AMA_REG_TEX_WAIT;
	obj_2d->flag &= ~OBD_ACTFLAG_2D_AMA_REG_TEX_FINISH;
}


// ================================================================
// ObjObjectAction2dAMALoad
/*!
	2Dアクションデータ読み込み

	@param	obj_work		[in]	オブジェクトワークポインタ
	@param	obj_3d			[in]	3Dオブジェクトワークポインタ
									NULLの場合、オブジェワークに既に登録されているか確認し、無ければ動的に取得します。
									(ObjectExitで解放されます)

	@param	data_work		[in]	3Dモデルデータワーク
	@param	filename		[in]	3Dモデルデータファイル名
	@param	index			[in]	3DモデルデータAMBインデックス
	@param	archive			[in]	モデルデータを含むアーカイブ
	@param	amb_tex			[in]	テクスチャAMB
	@param	id				[in]	初期化するアクションorノードID
	@param	type_node		[in]	アクション生成タイプ TRUE : ノードタイプ  FALSE : アクションタイプ

	@note
 */
// ================================================================
void ObjObjectAction2dAMALoad(OBS_OBJECT_WORK *obj_work, OBS_ACTION2D_AMA_WORK *obj_2d,
									OBS_DATA_WORK *data_work, const char *filename, s32 index, void *archive,
									void *amb_tex, u32 id, BOOL type_node)
{
	MTM_ASSERT(obj_work);

	// 表示ワークチェック
	if (obj_2d == NULL) {
		if (obj_work->obj_2d) {
			obj_2d = obj_work->obj_2d;
		}
		else {
			obj_2d = (OBS_ACTION2D_AMA_WORK*)amMemAlloc(sizeof(OBS_ACTION2D_AMA_WORK));
		}
		amZeroMemory(obj_2d, sizeof(OBS_ACTION2D_AMA_WORK));
		// 解放設定
		obj_work->flag |= OBD_OBJECT_FREE_2D;
	}

	// 3D描画ワークを設定
	obj_work->obj_2d = obj_2d;

	amAssert(obj_2d);

	// 3Dアクション初期化
	ObjAction2dAMALoad(obj_2d,
				data_work, filename, index, archive,
				amb_tex, id, type_node);
}


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
void ObjAction2dAMALoadSetTexlist(OBS_ACTION2D_AMA_WORK *obj_2d,
									OBS_DATA_WORK *data_work, const char *filename, s32 index, void *archive,
									NNS_TEXLIST *texlist, u32 id, BOOL type_node)
{
	//s32		i;
	void	*ama = NULL;

	MTM_ASSERT(obj_2d);
	MTM_ASSERT(texlist);

	// 2Dオブジェクトワーク初期化
	ObjAction2dAMAWorkInit(obj_2d);

	if (archive) {
		obj_2d->flag |= OBD_ACTFLAG_2D_AMA_ARCHIVE;
	}

	// AMAファイル読み込み
	if (filename) {
		ama = ObjDataLoad(data_work, filename, archive);

		if (archive && ama == NULL) {
			// アーカイブにデータがなかった
			obj_2d->flag &= ~OBD_ACTFLAG_2D_AMA_ARCHIVE;
			ama = ObjDataLoad(data_work, filename, NULL);
		}
	}
	else if (archive) {
		ama = ObjDataLoadAmbIndex(data_work, index, archive);
		if (ama == NULL) {
			// アーカイブにデータがなかった
			obj_2d->flag &= ~OBD_ACTFLAG_2D_AMA_ARCHIVE;
		}
	}
	else if (data_work) {
		ama = ObjDataGetInc(data_work);
	}

	if (ama == NULL) {
		amAssert(!"objObjectLoad::ObjAction2dACELoad() error! no ama data\n");
		return;
	}

	// AMAデータ保存
	obj_2d->ama = ama;

	// データワーク保存
	if (data_work) {
		obj_2d->ama_data_work = data_work;
	}

	// 初期化タイプ保存
	obj_2d->type_node = type_node;

	// ID保存
	obj_2d->act_id = id;

	// テクスチャ設定
	obj_2d->texlist = texlist;
	obj_2d->flag |= OBD_ACTFLAG_2D_AMA_REG_TEX_FINISH;

	// AMAオブジェクト生成
	ObjAction2dAMACreate(obj_2d);
}

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
void ObjObjectAction2dAMALoadSetTexlist(OBS_OBJECT_WORK *obj_work, OBS_ACTION2D_AMA_WORK *obj_2d,
									OBS_DATA_WORK *data_work, const char *filename, s32 index, void *archive,
									NNS_TEXLIST *texlist, u32 id, BOOL type_node)
{
	MTM_ASSERT(obj_work);

	// 表示ワークチェック
	if (obj_2d == NULL) {
		if (obj_work->obj_2d) {
			obj_2d = obj_work->obj_2d;
		}
		else {
			obj_2d = (OBS_ACTION2D_AMA_WORK*)amMemAlloc(sizeof(OBS_ACTION2D_AMA_WORK));
		}
		amZeroMemory(obj_2d, sizeof(OBS_ACTION2D_AMA_WORK));
		// 解放設定
		obj_work->flag |= OBD_OBJECT_FREE_2D;
	}

	// 3D描画ワークを設定
	obj_work->obj_2d = obj_2d;

	amAssert(obj_2d);

	// 3Dアクション初期化
	ObjAction2dAMALoadSetTexlist(obj_2d,
				data_work, filename, index, archive,
				texlist, id, type_node);
}

// ================================================================
// ObjAction2dAMAWorkInit
/*!
	2Dアクションワーク初期化

	@param	obj_2d			[in]	2DAMAオブジェクトワークポインタ
 */
// ================================================================
void ObjAction2dAMAWorkInit(OBS_ACTION2D_AMA_WORK *obj_2d)
{
	MTM_ASSERT(obj_2d);

	MI_CpuClear8(obj_2d, sizeof(OBS_ACTION2D_AMA_WORK));

	obj_2d->speed	= 1.f;
	obj_2d->color.a	= 255;
	obj_2d->color.r	= 255;
	obj_2d->color.g	= 255;
	obj_2d->color.b	= 255;
	obj_2d->fade.a	= 0;
	obj_2d->fade.r	= 0;
	obj_2d->fade.g	= 0;
	obj_2d->fade.b	= 0;
}

// ================================================================
// ObjAction2dAMACreate
/*!
	2Dアクション生成

	@param	obj_2d			[in]	2DAMAオブジェクトワークポインタ

	@note
		Createする前に ObjAction2dAMALoad 等でデータを読み込んでおく必要があります。
 */
// ================================================================
void ObjAction2dAMACreate(OBS_ACTION2D_AMA_WORK *obj_2d)
{
	MTM_ASSERT(obj_2d);
	MTM_ASSERT(obj_2d->texlist);
	MTM_ASSERT(obj_2d->ama);

	AoActSetTexture(obj_2d->texlist);
	if (obj_2d->type_node) {
		obj_2d->act = AoActCreateNode(obj_2d->ama, obj_2d->act_id, 0);
	}
	else {
		obj_2d->act = AoActCreate(obj_2d->ama, obj_2d->act_id, 0);
	}
	AoActSetTexture(NULL);
}

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
BOOL ObjAction2dAMALoadCheck(OBS_ACTION2D_AMA_WORK *obj_2d)
{
	MTM_ASSERT(obj_2d);

	if (obj_2d->flag & OBD_ACTFLAG_2D_AMA_REG_TEX_WAIT) {
		if (AoTexIsLoaded(&obj_2d->ao_tex)) {
			obj_2d->texlist = AoTexGetTexList(&obj_2d->ao_tex);
			obj_2d->flag &= ~OBD_ACTFLAG_2D_AMA_REG_TEX_WAIT;
			obj_2d->flag |= OBD_ACTFLAG_2D_AMA_REG_TEX_FINISH;

			// 2Dオブジェクト生成
			ObjAction2dAMACreate(obj_2d);

			return (TRUE);
		}
	}
	else if (obj_2d->flag & OBD_ACTFLAG_2D_AMA_REG_TEX_FINISH) {
		return (TRUE);
	}
	return (FALSE);
}


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
                          u16 usCharSize )
{
    void * pBac = NULL;
    u32 ulCharAddrA = 0,ulCharAddrB = 0;

    MTM_ASSERT( pWork != NULL);
    // MTM_ASSERT( pPath != NULL);

    // 表示ワークチェック
    if ( pObj2d == NULL ){
        if ( pWork->obj_2d ){
            pObj2d = pWork->obj_2d;
        } else{
            // メモリ取得
            pObj2d = mtMemAllocMain( sizeof(OBS_ACTION2D_WORK) );
            MI_CpuClear8( pObj2d, sizeof(OBS_ACTION2D_WORK));
            // 解放設定
            pWork->flag |= OBD_OBJECT_FREE_2D;
        }
    }

    // 2Dワークポインタをオブジェクトに設定
    pWork->obj_2d = pObj2d;
    
    MTM_ASSERT( pObj2d != NULL);
    
    // 読み込み済みの時、一度解放
    {

    }

    pWork->obj_2d->flag &= ~OBD_ACTFLAG_2D_ARCHIVE;
    pWork->obj_2d->bac_data_work = pData;

    // フラグ初期化
    // オブジェクトに合わせてフラグを先行して立てる
    pWork->obj_2d->act_spr.flag = 0;
    if ( pWork->flag & OBD_OBJECT_ACT_NOA )
        pWork->obj_2d->act_spr.flag |= MTD_ACT_DS_FLAG_DISABLE_GE_A;
    if ( pWork->flag & OBD_OBJECT_ACT_NOB )
        pWork->obj_2d->act_spr.flag |= MTD_ACT_DS_FLAG_DISABLE_GE_B;
    
    // 読み込み
    ObjActLoad( &pWork->obj_2d->act_spr, pPath, usCharSize, pData, pArchive);

    // アーカイブから読み込めた場合はフラグセット
    if ( pData ){
        if ( pData->num & OBD_DATA_ARCHIVE_FLAG)
			pWork->obj_2d->flag |= OBD_ACTFLAG_2D_ARCHIVE;
    }
    
}
#endif

#if OBD_USE_ACTION3D_NNS
// ================================================================
// ObjObjectAction3dModelLoad
/*!
  3Dモデルデータ読み込み

  @param pWork    [in] オブジェクトワークポインタ
  @param pObj3d   [in] 3Dオブジェクト先頭ポインタ
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
                                 OBS_DATA_WORK* pDataMod, void * pArchive)
{
    void * pTemp;
    MTS_ACTION3D_NNS *pAct;

    MTM_ASSERT( pWork != NULL);
    // MTM_ASSERT( pPath != NULL);

    // 表示ワークチェック
    if ( pObj3d == NULL ){
        if ( pWork->obj_3d ){
            pObj3d = pWork->obj_3d;
        } else{
            // メモリ取得
            pObj3d = mtMemAllocMain( sizeof(OBS_ACTION3D_NNS_WORK) );
            MI_CpuClear8( pObj3d, sizeof(OBS_ACTION3D_NNS_WORK));
            // 解放設定
            pWork->flag |= OBD_OBJECT_FREE_3D;
        }
    }
    
    MTM_ASSERT( pObj3d != NULL);

    // 読み込み済みの時、一度解放
    {

    }
    
    pAct= &pObj3d->act_3d;

    // オブジェクトに3Dポインタをセット
    pWork->obj_3d = pObj3d;

    // 初期化
    mtAct3dInitStructNNS( pAct, 0 );
    
    if ( pArchive )
        pObj3d->flag |= OBD_ACTFLAG_3D_ARCHIVE;

    // モデルファイル読み込み
    pTemp = ObjDataLoad( pDataMod, pPath, pArchive );

    if ( pTemp == NULL && pArchive ){
        // ARCHIVEを指定しているのにファイルがありません
        //MTM_ASSERT(0);
        
        pObj3d->flag &= ~OBD_ACTFLAG_3D_ARCHIVE;
        pTemp = ObjDataLoad( pDataMod, pPath, NULL );
    }
    if ( pTemp == NULL )
        return;

    // セット
    pObj3d->model = pTemp;
    
    if ( pDataMod ){
        pObj3d->model_data_work = pDataMod;

        // 共有データの初取得(アーカイブからの場合はusNumは32768から)
        if ( pDataMod->num == 1)
        
        // 未取得なら
        // if ( !NNS_G3dGetTex((NNSG3dResFileHeader*) pTemp) )    
            // モデル初期化
            NNS_G3dResDefaultSetup(pTemp);
        
    }

    if ( pTemp ){
        // 描画オブジェクトを初期化する
        mtAct3dSetModelNNS(pAct, pTemp, ulIndex, 0, 0 );

    }
}

// ================================================================
// ObjObjectAction3dAnimeLoad
/*!
  3Dアクションデータ読み込み

  @param pWork    [in] オブジェクトワークポインタ
  @param pObj3d   [in] 3Dオブジェクト先頭ポインタ
                       NULLの場合、オブジェワークに既に登録されているか確認し、無ければ動的に取得します。
                      （ObjectExitで解放されます）
  @param pPath    [in] NSB** ファイルパス

  @param pDataMod [in] モデルデータ管理ワークポインタ（NULLの場合、同じオブジェクト作成ごとにリソースを消費します）
  @param pArchive [in] アーカイブポインタ (NULLの場合、ROMからロードを行います、従ってワンカートリッジ不可)

 */
// ================================================================
void ObjObjectAction3dAnimeLoad( OBS_OBJECT_WORK *pWork,
                                 OBS_ACTION3D_NNS_WORK *pObj3d,
                                 const char* pPath,
                                 OBS_DATA_WORK* pDataAni, void * pArchive)
{
    MTS_ACTION3D_NNS *pAct;
    void * pTemp;
    u8 ucType = 0;
    u16 check;
    s16 sLength;
    MTM_ASSERT( pWork != NULL);
    // MTM_ASSERT( pPath != NULL);

    // 表示ワークチェック
    if ( pObj3d == NULL ){
        if ( pWork->obj_3d ){
            pObj3d = pWork->obj_3d;
        } else{
            // メモリ取得
            pObj3d = mtMemAllocMain( sizeof(OBS_ACTION3D_NNS_WORK) );
            MI_CpuClear8( pObj3d, sizeof(OBS_ACTION3D_NNS_WORK));
            // 解放設定
            pWork->flag |= OBD_OBJECT_FREE_3D;
        }
    }
    MTM_ASSERT( pObj3d != NULL);

    pAct= &pObj3d->act_3d;

    sLength = (s16)STD_GetStringLength(pPath);
    check =  pPath[sLength - 1];
    check |= pPath[sLength - 2] << 8;
    
    // ファイルタイプチェック
    switch ( check ){
    case 'ca':
        break;
    case 'ma':
        ucType = 1;
        break;
    case 'tp':
        ucType = 2;
        break;
    case 'ta':
        ucType = 3;
       break;
    case 'va':
        ucType = 4;
        break;
    default:
        // 想定外のファイル読み込み
        MTM_ASSERT( 0 );
        break;
    }
       
    
    if ( pArchive ){
        pObj3d->flag |= (OBD_ACTFLAG_3D_ARCHIVE_CA << ucType);
    }else{
        pObj3d->flag &= ~(OBD_ACTFLAG_3D_ARCHIVE_CA << ucType);
    }

    // 読み込み済みの時、一度解放
    {

    }

    // モーションファイル読み込み
    pTemp = ObjDataLoad( pDataAni, pPath, pArchive);
    
    if ( pTemp == NULL && pArchive ){
        // ARCHIVEを指定しているのにファイルがありません
        //MTM_ASSERT(0);
        
        pObj3d->flag &= ~(OBD_ACTFLAG_3D_ARCHIVE_CA << ucType);
        pTemp = ObjDataLoad( pDataAni, pPath, NULL );
    }
    // セット
    pObj3d->anime[ucType] = pTemp;

    if ( pTemp == NULL )
        return;
    
    if ( pDataAni ){
        pObj3d->anime_data_work[ucType] = pDataAni;
        // 共有データの初取得(アーカイブからの場合はusNumは32768から)
        if ( pDataAni->num == 1)
        // 未取得なら
        // if ( !NNS_G3dGetTex((NNSG3dResFileHeader*) pTemp) )    
            // モデル初期化
            NNS_G3dResDefaultSetup(pTemp);
    }
}
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
                                       OBS_DATA_WORK* pDataMod, void * pArchive)
{
    void * pTemp;
    MTS_ACTION3D_NNS_1M1S *pAct;

    MTM_ASSERT( pWork != NULL);
    //MTM_ASSERT( pPath != NULL);

    // 表示ワークチェック
    if ( pObj3d == NULL ){
        /* パーツ追加読み込み対応のためNULL時は常にメモリ取得
        if ( pWork->obj_s3d ){
            pObj3d = pWork->obj_s3d;
        } else*/{
            // メモリ取得
            pObj3d = mtMemAllocMain( sizeof(OBS_ACTION3D_SIMPLE_WORK) );
            MI_CpuClear8( pObj3d, sizeof(OBS_ACTION3D_SIMPLE_WORK));
            // 解放設定
            pWork->flag |= OBD_OBJECT_FREE_3DS;
        }
    }

    MTM_ASSERT( pObj3d != NULL);

    pAct= &pObj3d->act_s3d;

    // 読み込み済みの時、追加パーツとして読み込む
    if ( pWork->obj_s3d ){
        OBS_ACTION3D_SIMPLE_WORK *pObj3dWork = pWork->obj_s3d;
        for(;;){
            if ( pObj3dWork->next == NULL ){
                // 末尾に追加
                pObj3dWork->next = pObj3d;
                break;
            }
            // 次をチェック
            pObj3dWork = pObj3dWork->next;
        }
    }else{
        // オブジェクトに3Dポインタをセット
        pWork->obj_s3d = pObj3d;
    }
    
    // 初期化
    mtAct3dInitStructNNS1M1S( pAct, 0 );
    
    if ( pArchive )
        pObj3d->flag |= OBD_ACTFLAG_3D_ARCHIVE;

    // モデルファイル読み込み
    pTemp = ObjDataLoad( pDataMod, pPath, pArchive );

    if ( pTemp == NULL && pArchive ){
        // ARCHIVEを指定しているのにファイルがありません
        //MTM_ASSERT(0);
        
        pObj3d->flag &= ~OBD_ACTFLAG_3D_ARCHIVE;
        pTemp = ObjDataLoad( pDataMod, pPath, NULL );
    }

    if ( pTemp == NULL )
        return;
    // セット
    pObj3d->model = pTemp;
    
    if ( pDataMod ){
        pObj3d->model_data_work = pDataMod;

        // 共有データの初取得(アーカイブからの場合はusNumは32768から)
        if ( pDataMod->num == 1)
        
        // 未取得なら
        // if ( !NNS_G3dGetTex((NNSG3dResFileHeader*) pTemp) )    
            // モデル初期化
            NNS_G3dResDefaultSetup(pTemp);
        
    }

    if ( pTemp ){
        // 描画オブジェクトを初期化する
        mtAct3dSetModelNNS1M1S(pAct, pTemp, ulIndex, usMatID, usID  );

    }
}
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
                                 OBS_DATA_WORK* pDataMod, void * pArchive)
{
    // void * pTemp;
    MTS_ACTION3D_SPRITE *pAct;
    void * pBac = NULL;
    u32 ulCharAddr = 0;
    u32 ulPltAddr = 0;

    MTM_ASSERT( pWork != NULL);
    //MTM_ASSERT( pPath != NULL);

    // 表示ワークチェック
    if ( pObj3d == NULL ){
        if ( pWork->obj_3dspr ){
            pObj3d = pWork->obj_3dspr;
        } else{
            // メモリ取得
            pObj3d = mtMemAllocMain( sizeof(OBS_ACTION3D_SPRITE_WORK) );
            MI_CpuClear8( pObj3d, sizeof(OBS_ACTION3D_SPRITE_WORK));
            // 解放設定
            pWork->flag |= OBD_OBJECT_FREE_3DSP;
        }
    }

    MTM_ASSERT( pObj3d != NULL);

    // 読み込み済みの時、一度解放
    {

    }
    
    pAct= &pObj3d->act_3dspr;
    
    // オブジェクトにアクションオブジェクトワークをセット
    pWork->obj_3dspr = pObj3d;

    if ( pArchive ) {
        pObj3d->flag |= OBD_ACTFLAG_3D_ARCHIVE;	// 2Dデータと同時描画の可能性があるので、こちらのフラグを使う
	}
    
    pObj3d->bac_data_work = pDataMod;
    
    // Bacファイル読み込み
    pBac = ObjDataLoad( pDataMod, pPath, pArchive );

    if ( pBac == NULL && pArchive ){
        // ARCHIVEを指定しているのにファイルがありません
        //MTM_ASSERT(0);
        
        //pWork->flag &= ~OBD_OBJECT_ARCHIVE;
        pObj3d->flag &= ~OBD_ACTFLAG_3D_ARCHIVE;
        pBac = ObjDataLoad( pObj3d->bac_data_work, pPath, NULL );
    }
    if ( pBac == NULL ) {
#if defined (MTD_DEBUG)
		OS_TPrintf("objObjectLoad::ObjObjectAction3dSpriteLoad() Error! file not found:%s\n", pPath);
		MTM_ASSERT(0);
#endif	// #if defined (MTD_DEBUG)
        return;
    }
    pObj3d->bac = pBac;

    // Tex VRAM取得
    if ( usCharSize ){
        // サイズ自動取得
        if ( usCharSize == OBD_AUTO_CHARSIZE )
            usCharSize = (u16)mtActGetTexSizeMaxFromBac( pBac );
        ulCharAddr = mtVramAllocTex( usCharSize, FALSE );
    }
    // パレット取得
    if ( usColorNum  ){
        // 色数チェック
        if ( usColorNum == OBD_AUTO_PLTSIZE )
            usColorNum = mtActGetTexPltNumMaxFromBac(pBac);
        ulPltAddr = mtVramAllocTexPlt( usColorNum, FALSE );
    }
    
    // アクション初期化
    mtAct3dInitStructSprite( pAct, 0, pBac, 0, MTD_ACT_FLAG_DMA_CHA, ulCharAddr, ulPltAddr );

}
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
void ObjObjectActionSoftwareSpriteLoad(OBS_OBJECT_WORK *obj_work, OBS_ACTION3D_SS_WORK *obj_3dss,
								const char *path,  u16 char_size, u16 color_num,
								OBS_DATA_WORK *data_work, void *archive)
{
	MTS_ACTION_SS	*act;
	void			*bac = NULL;
	u32				char_addr = 0;
	u32				plt_addr = 0;

	MTM_ASSERT(obj_work != NULL);

	// 表示ワークチェック
	if (obj_3dss == NULL) {
		if (obj_work->obj_3dss) {
			obj_3dss = obj_work->obj_3dss;
		}
		else {
			// メモリ取得
			obj_3dss = mtMemAllocMain(sizeof(OBS_ACTION3D_SS_WORK));
			MI_CpuClear8(obj_3dss, sizeof(OBS_ACTION3D_SS_WORK));
			// 解放設定
			obj_work->flag |= OBD_OBJECT_FREE_3DSS;
		}
	}

	MTM_ASSERT(obj_3dss != NULL);

	// 読み込み済みの時、一度解放
	{

	}

	act= &obj_3dss->act_ss;

    // オブジェクトに3Dポインタをセット
    obj_work->obj_3dss = obj_3dss;

	if (archive) {
		obj_3dss->flag |= OBD_ACTFLAG_3D_ARCHIVE;	// 2Dデータと同時描画の可能性があるので、こちらのフラグを使う
	}

	obj_3dss->bac_data_work = data_work;

    // Bacファイル読み込み
	bac = ObjDataLoad(data_work, path, archive);

	if (bac == NULL && archive) {
		// ARCHIVEを指定しているのにファイルがありません
		//MTM_ASSERT(0);

        obj_3dss->flag &= ~OBD_ACTFLAG_3D_ARCHIVE;
		bac = ObjDataLoad(obj_3dss->bac_data_work, path, NULL);
	}
	if (bac == NULL) {
#if defined (MTD_DEBUG)
		OS_TPrintf("objObjectLoad::ObjObjectActionSoftwareSpriteLoad() Error! file not found:%s\n", path);
		MTM_ASSERT(0);
#endif	// #if defined (MTD_DEBUG)
		return;
	}
	obj_3dss->bac = bac;

	// Tex VRAM取得
	if (char_size) {
		// サイズ自動取得
		if (char_size == OBD_AUTO_CHARSIZE) {
			char_size = (u16)mtActGetTexSizeMaxFromBac(bac);
		}
		char_addr = mtVramAllocTex(char_size, FALSE);
    }
    // パレット取得
	if (color_num) {
		// 色数チェック
		if (color_num == OBD_AUTO_PLTSIZE) {
			color_num = mtActGetTexPltNumMaxFromBac(bac);
		}
		plt_addr = mtVramAllocTexPlt(color_num, FALSE);
	}

	// アクション初期化
	mtActInitStructSS(act, bac, 0/*act_id*/, MTD_ACT_FLAG_DMA_CHA/*flag*/, char_addr, plt_addr, 0/*sort_prio*/);
}
#endif // #if OBD_USE_ACTION3D_SS

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
void ObjObjectActionPolygonLoad(OBS_OBJECT_WORK *obj_work, OBS_ACTION3D_POLY_WORK *obj_3dpoly,
								const char *path,
								OBS_DATA_WORK *data_work, void *archive)
{
	IZS_PLA_ACTION	*act;
	void			*plm = NULL;
	MTM_ASSERT(obj_work != NULL);

	// 表示ワークチェック
	if (obj_3dpoly == NULL) {
		MTM_ASSERT(0);
		return;
#if 0
		if (obj_work->obj_3dpoly) {
			obj_3dpoly = obj_work->obj_3dpoly;
		}
		else {
			// メモリ取得
			obj_3dpoly = mtMemAllocMain(sizeof(OBS_ACTION3D_POLY_WORK));
			MI_CpuClear8(obj_3dpoly, sizeof(OBS_ACTION3D_POLY_WORK));
			// 解放設定
			//obj_work->flag |= OBD_OBJECT_FREE_3DSS;
		}
#endif
	}

	MTM_ASSERT(obj_3dpoly != NULL);

	act = &obj_3dpoly->act_poly;

	// オブジェクトに3DPOLYポインタをセット
	obj_work->obj_3dpoly = obj_3dpoly;

	if (archive) {
		obj_3dpoly->flag |= OBD_ACTFLAG_3D_ARCHIVE;	// 2Dデータと同時描画の可能性があるので、こちらのフラグを使う
	}

	obj_3dpoly->plm_data_work = data_work;

	// PLMファイル読み込み
	plm = ObjDataLoad(data_work, path, archive);

	if (plm == NULL && archive) {
		// ARCHIVEを指定しているのにファイルがありません
		//MTM_ASSERT(0);

		obj_3dpoly->flag &= ~OBD_ACTFLAG_3D_ARCHIVE;
		plm = ObjDataLoad(obj_3dpoly->plm_data_work, path, NULL);
	}
	if (plm == NULL) {
#if defined (MTD_DEBUG)
		OS_TPrintf("objObjectLoad::ObjObjectActionPolygonLoad() Error! file not found:%s\n", path);
		MTM_ASSERT(0);
#endif	// #if defined (MTD_DEBUG)
		return;
	}
	obj_3dpoly->plm = plm;

#if 0
	// Tex VRAM取得
	if (char_size) {
		// サイズ自動取得
		if (char_size == OBD_AUTO_CHARSIZE) {
			char_size = (u16)mtActGetTexSizeMaxFromBac(bac);
		}
		char_addr = mtVramAllocTex(char_size, FALSE);
    }
    // パレット取得
	if (color_num) {
		// 色数チェック
		if (color_num == OBD_AUTO_PLTSIZE) {
			color_num = mtActGetTexPltNumMaxFromBac(bac);
		}
		plt_addr = mtVramAllocTexPlt(color_num, FALSE);
	}
#endif

	// アクション初期化
	IzPolyActInitStruct(act, plm, 0/*act_id*/, 0/*flag*/, 0/*prio*/);
}
#endif // #if OBD_USE_ACTION3D_POLY

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
void ObjObjectAction3DSmaLoad(OBS_OBJECT_WORK *obj_work, OBS_ACTION3D_SMA_WORK *obj_3dsma,
								const char *path, u16 char_size, u16 color_num, u16 cls_max,
								OBS_DATA_WORK *smm_data_work, OBS_DATA_WORK *smg_data_work, OBS_DATA_WORK *smp_data_work,
								OBS_DATA_WORK *smc_data_work, void *archive, BOOL act_uncomp, BOOL tex_all_trans)
{
	MTS_SMA			*sma;
	void			*smm = NULL, *smg = NULL, *smp = NULL, *smc = NULL;
	char			path_work[512];
	s32				str_len;
	u32				char_addr = 0;
	u32				plt_addr = 0;

	MTM_ASSERT(obj_work != NULL);

	// 表示ワークチェック
	if (obj_3dsma == NULL) {
		if (obj_work->obj_3dsma) {
			obj_3dsma = obj_work->obj_3dsma;
		}
		else {
			// メモリ取得
			obj_3dsma = mtMemAllocMain(sizeof(OBS_ACTION3D_SMA_WORK));
			MI_CpuClear8(obj_3dsma, sizeof(OBS_ACTION3D_SMA_WORK));
			// 解放設定
			obj_work->flag |= OBD_OBJECT_FREE_3DSMA;
		}
	}

	MTM_ASSERT(obj_3dsma != NULL);

	// 読み込み済みの時、一度解放
	{

	}

//	sma= &obj_3dsma->act_sma;

    // オブジェクトに3Dポインタをセット
    obj_work->obj_3dsma = obj_3dsma;

	if (archive) {
		obj_3dsma->flag |= OBD_ACTFLAG_SMA_ARCHIVE_SMM | OBD_ACTFLAG_SMA_ARCHIVE_SMG | OBD_ACTFLAG_SMA_ARCHIVE_SMP | OBD_ACTFLAG_SMA_ARCHIVE_SMC;
	}

	// データワーク保存
	obj_3dsma->smm_data_work = smm_data_work;
	obj_3dsma->smg_data_work = smg_data_work;
	obj_3dsma->smp_data_work = smp_data_work;
	obj_3dsma->smc_data_work = smc_data_work;

	// ファイル読み込み
    STD_StrCpy(path_work, path);
    str_len = STD_StrLen(path);

    // SMMファイル読み込み
    STD_StrCat(path_work, ".smm");
	smm = ObjDataLoad(smm_data_work, path_work, archive);
	if (smm == NULL && archive) {
		// ARCHIVEを指定しているのにファイルがありません
        obj_3dsma->flag &= ~OBD_ACTFLAG_SMA_ARCHIVE_SMM;
		smm = ObjDataLoad(smm_data_work, path_work, NULL);
	}
	if (smm == NULL) {
#if defined (MTD_DEBUG)
		OS_TPrintf("objObjectLoad::ObjObjectAction3DSmaLoad() Error! file not found:%s\n", path_work);
		MTM_ASSERT(0);
#endif	// #if defined (MTD_DEBUG)
		return;
	}
	obj_3dsma->smm = smm;

	// SMGファイル読み込み
    path_work[str_len] = '\0';
    STD_StrCat(path_work, ".smg");
	smg = ObjDataLoad(smg_data_work, path_work, archive);
	if (smg == NULL && archive) {
		// ARCHIVEを指定しているのにファイルがありません
        obj_3dsma->flag &= ~OBD_ACTFLAG_SMA_ARCHIVE_SMG;
		smg = ObjDataLoad(smg_data_work, path_work, NULL);
	}
	if (smg == NULL) {
#if defined (MTD_DEBUG)
		OS_TPrintf("objObjectLoad::ObjObjectAction3DSmaLoad() Error! file not found:%s\n", path_work);
		MTM_ASSERT(0);
#endif	// #if defined (MTD_DEBUG)
		return;
	}
	obj_3dsma->smg = smg;

	// SMPファイル読み込み
    path_work[str_len] = '\0';
    STD_StrCat(path_work, ".smp");
	smp = ObjDataLoad(smp_data_work, path_work, archive);
	if (smp == NULL && archive) {
		// ARCHIVEを指定しているのにファイルがありません
        obj_3dsma->flag &= ~OBD_ACTFLAG_SMA_ARCHIVE_SMP;
		smp = ObjDataLoad(smp_data_work, path_work, NULL);
	}
	if (smp == NULL) {
#if defined (MTD_DEBUG)
		OS_TPrintf("objObjectLoad::ObjObjectAction3DSmaLoad() Error! file not found:%s\n", path_work);
		MTM_ASSERT(0);
#endif	// #if defined (MTD_DEBUG)
		return;
	}
	obj_3dsma->smp = smp;

	// SMCファイル読み込み
	if (cls_max) {
	    path_work[str_len] = '\0';
	    STD_StrCat(path_work, ".smc");
		smc = ObjDataLoad(smc_data_work, path_work, archive);
		if (smc == NULL && archive) {
			// ARCHIVEを指定しているのにファイルがありません
	        obj_3dsma->flag &= ~OBD_ACTFLAG_SMA_ARCHIVE_SMC;
			smc = ObjDataLoad(smc_data_work, path_work, NULL);
		}
#if defined (MTD_DEBUG)
		if (!smc) {
			OS_TPrintf("objObjectLoad::ObjObjectAction3DSmaLoad() Warning! file not found:%s\n", path_work);
		}
#endif	// #if defined (MTD_DEBUG)
		// SMCファイルはなくてもかまわない
		obj_3dsma->smc = smc;
	//	mtSmaInitObject 内でチェックしているので、データがNULLの時に0以外でも余分なワークは取得されない
	//	if (!smc) {
	//		cls_max = 0;
	//	}
	}

	// SMAオブジェクト取得
	sma = mtSmaInitObject(smm, 1/*smg_num*/, &smg, smp,
			smc, NULL/*smr*/, cls_max, 0/*atr_max*/, MTE_GE2_A,
			(u16)(MTD_SMA_FLAG_DRAW_3D | (act_uncomp ? MTD_SMA_FLAG_USE_TDEC_BUF : 0) | (tex_all_trans ? MTD_SMA_FLAG_ALL_TEX_TRANS : 0)));
	sma->alpha = 31;

	obj_3dsma->act_sma = sma;

	// Tex VRAM取得
	if (char_size) {
		// サイズ自動取得
		if (char_size == OBD_AUTO_CHARSIZE) {
			char_size = (u16)mtSmaVramGetUseTexSize(sma);
		}
		char_addr = mtVramAllocTex(char_size, FALSE);
		// アクションに設定
		mtSmaVramSetTexBaseAddr(sma, char_addr, char_size);
    }
    // パレット取得
	if (color_num) {
		// 色数チェック
		if (color_num == OBD_AUTO_PLTSIZE) {
			color_num = (u16)mtSmaVramGetUseTexPltSize(sma);
		}
		plt_addr = mtVramAllocTexPlt(color_num, FALSE);
		// アクションに設定
		mtSmaVramSetTexPltBaseAddr(sma, plt_addr, color_num);
	}

	// データ転送リクエスト発行
	if (char_size && color_num) {
		mtSmaVramTrans(sma, TRUE);
	}
}

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
void ObjObjectSetActionActUncomp(OBS_ACTION2D_WORK *obj_2d, OBS_ACTION_UNCOMP_WORK *act_uncomp, u32 cha_size)
{
	MTM_ASSERT(obj_2d);

	if (!act_uncomp) {
		act_uncomp = mtMemAllocMain(sizeof(OBS_ACTION_UNCOMP_WORK));
		obj_2d->flag |= OBD_ACTFLAG_FREE_UNCOMP;
	}

	obj_2d->act_uncomp = act_uncomp;

	if (cha_size == OBD_AUTO_CHARSIZE) {
		MTM_ASSERT(obj_2d->act_spr.act.bac_addr);
		cha_size = obj_get_cha_name_max_func[g_obj.vram_map_mode](obj_2d->act_spr.act.bac_addr);
	}

	// バイトサイズに変更
	cha_size <<= 5 + g_obj.vram_map_mode;

	// 解凍バッファ取得
	act_uncomp->cha_size = cha_size;
	act_uncomp->cha_uncomp = mtMemAllocMain(cha_size);
	MI_CpuClear8(act_uncomp->cha_uncomp, cha_size);

	// 転送先保存
	act_uncomp->cha_vram = obj_2d->act_spr.act.cha_vram;
	act_uncomp->cha_addr = obj_2d->act_spr.act.cha_addr;

	// Action側DMA転送をOFFにする
	obj_2d->act_spr.act.flag &= ~MTD_ACT_FLAG_DMA_CHA;
}
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
void ObjObjectSet3dSpriteActUncomp(OBS_ACTION3D_SPRITE_WORK *obj_3dspr, OBS_ACTION_UNCOMP_WORK *act_uncomp, u32 tex_size)
{
	MTM_ASSERT(obj_3dspr);

	if (!act_uncomp) {
		act_uncomp = mtMemAllocMain(sizeof(OBS_ACTION_UNCOMP_WORK));
		obj_3dspr->flag |= OBD_ACTFLAG_FREE_UNCOMP;
	}

	obj_3dspr->act_uncomp = act_uncomp;

	if (tex_size == OBD_AUTO_CHARSIZE) {
		MTM_ASSERT(obj_3dspr->bac);
		tex_size = (u16)mtActGetTexSizeMaxFromBac(obj_3dspr->bac);
	}

	// 解凍バッファ取得
	act_uncomp->cha_size = tex_size;
	act_uncomp->cha_uncomp = mtMemAllocMain(tex_size);
	MI_CpuClear8(act_uncomp->cha_uncomp, tex_size);

	// 転送先保存
	act_uncomp->cha_vram = obj_3dspr->act_3dspr.act.cha_vram;
	act_uncomp->cha_addr = obj_3dspr->act_3dspr.act.cha_addr;

	// Action側DMA転送をOFFにする
	obj_3dspr->act_3dspr.act.flag &= ~MTD_ACT_FLAG_DMA_CHA;
}
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
void ObjObjectSetSoftwareSpriteActUncomp(OBS_ACTION3D_SS_WORK *obj_3dss, OBS_ACTION_UNCOMP_WORK *act_uncomp, u32 tex_size)
{
	MTM_ASSERT(obj_3dss);

	if (!act_uncomp) {
		act_uncomp = mtMemAllocMain(sizeof(OBS_ACTION_UNCOMP_WORK));
		obj_3dss->flag |= OBD_ACTFLAG_FREE_UNCOMP;
	}

	obj_3dss->act_uncomp = act_uncomp;

	// タイプ設定
	act_uncomp->sp_setting.sp_setting_type = OBE_OBJ_ACTION_SP_SETTING_TYPE_UNCOMP;

	if (tex_size == OBD_AUTO_CHARSIZE) {
		MTM_ASSERT(obj_3dss->bac);
		tex_size = (u16)mtActGetTexSizeMaxFromBac(obj_3dss->bac);
	}

	// 解凍バッファ取得
	act_uncomp->cha_size = tex_size;
	act_uncomp->cha_uncomp = mtMemAllocMain(tex_size);
	MI_CpuClear8(act_uncomp->cha_uncomp, tex_size);

	// 転送先保存
	act_uncomp->cha_vram = obj_3dss->act_ss.act.cha_vram;
	act_uncomp->cha_addr = obj_3dss->act_ss.act.cha_addr;

	// Action側DMA転送をOFFにする
	obj_3dss->act_ss.act.flag &= ~MTD_ACT_FLAG_DMA_CHA;
}
#endif // #if OBD_USE_ACTION3D_SS

#if OBD_USE_ACTION3D_POLY
// 現在 OBD_USE_ACTION3D_POLY にはUncompが存在しない
#endif // #if OBD_USE_ACTION3D_POLY

#if OBD_USE_ACTION3D_SMA
#if 0
// ==========================================================================
// ObjObjectSet3DSmaActUncomp
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
void ObjObjectSet3DSmaActUncomp(OBS_ACTION3D_SMA_WORK *obj_3dsma, OBS_ACTION_UNCOMP_WORK *act_uncomp, u32 tex_size)
{
	MTM_ASSERT(0);	// 保留中
	MTM_ASSERT(obj_3dsma);

	if (!act_uncomp) {
		act_uncomp = mtMemAllocMain(sizeof(OBS_ACTION_UNCOMP_WORK));
		obj_3dsma->flag |= OBD_ACTFLAG_FREE_UNCOMP;
	}

	obj_3dsma->act_uncomp = act_uncomp;

	if (tex_size == OBD_AUTO_CHARSIZE) {
		MTM_ASSERT(obj_3dsma->act_sma);
		tex_size = (u16)mtSmaVramGetUseTexSize(obj_3dsma->act_sma);
	}

	// 解凍バッファ取得
	act_uncomp->cha_size = tex_size;
	act_uncomp->cha_uncomp = mtMemAllocMain(tex_size);
	MI_CpuClear8(act_uncomp->cha_uncomp, tex_size);

	// 転送先保存
	act_uncomp->cha_vram = MTE_CHA_VRAM_TEXTURE;
	act_uncomp->cha_addr = obj_3dsma->act_sma->tex_vram_addr;
}
#endif
#endif // #if OBD_USE_ACTION3D_SMA

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
void ObjObjectUpdateTransActUncomp(const MTS_ACT_COMMAND *cmd, MTS_ACTION *act, OBS_ACTION_UNCOMP_WORK *act_uncomp)
{
	switch (cmd->cmd_id) {
	case MTE_ACT_COMMAND_CHARACTER:
		// 解凍したデータの転送リクエスト発行
		mtChaRequestTransAddr(act_uncomp->cha_uncomp, act_uncomp->cha_size,
						act_uncomp->cha_vram, act_uncomp->cha_addr);
		break;
	}
}
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
void ObjObjectSetSoftwareSpriteActVramDB(OBS_ACTION3D_SS_WORK *obj_3dss, OBS_ACTION_TEXVRAM_DB_WORK *act_texdb)
{
	MTM_ASSERT(obj_3dss);

	if (!act_texdb) {
		act_texdb = mtMemAllocMain(sizeof(OBS_ACTION_TEXVRAM_DB_WORK));
		obj_3dss->flag |= OBD_ACTFLAG_FREE_UNCOMP;
	}

	obj_3dss->act_texdb = act_texdb;

	// タイプ設定
	act_texdb->sp_work.sp_setting_type = OBE_OBJ_ACTION_SP_SETTING_TYPE_TEXVRAM_DB;

	// VRAMアドレス保存
	act_texdb->cha_addr = obj_3dss->act_ss.act.cha_addr;
	act_texdb->cha_vram = obj_3dss->act_ss.act.cha_vram;

	// 転送VRAMスロット取得
	act_texdb->slot_no = (u16)(mtVramGetOfstFromVKeyTex(act_texdb->cha_addr) >> 17);

	// オフセットアドレス取得
	act_texdb->ofst_addr = mtVramGetOfstFromVKeyTex(act_texdb->cha_addr) - (act_texdb->slot_no << 17);

	// 現在の転送先に書き換え
//	obj_3dss->act_ss.act.cha_vram = MTE_CHA_VRAM_ADDRESS;
//	obj_3dss->act_ss.act.cha_addr = g_obj.db_tex_slot_at_lcdc[g_obj.db_tex_vram_flip ^ 0x01][act_texdb->slot_no] +
//									act_texdb->ofst_addr;
}

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
void ObjObjectSoftwareSpriteLumpTransActVramDB(MTS_ACTION_SS *act_ss, u16 act_id)
{
#if 1
	u32		act_flag;
	u32		cha_addr	= act_ss->act.cha_addr;
	u32		slot_no		= (u32)(mtVramGetOfstFromVKeyTex(cha_addr) >> 17);

	MTM_ASSERT(act_ss);

	act_flag = act_ss->act.flag;
	act_ss->act.flag |= MTD_ACT_FLAG_NO_REQ_CHA;
	if (!(act_ss->act.flag & MTD_ACT_FLAG_NO_REQ_PLT)) {
		act_ss->act.flag |= MTD_ACT_FLAG_NO_PLT;
	}

	// 転送バンクに転送(DB設定のないスロットの無い場合は接続中)
	mtActResetStructSS(act_ss, act_id);
	mtActUpdateSS(act_ss, NULL, 0);

	if (g_obj.db_tex_tcb &&
			(g_obj.db_tex_db_slot_flag & (1 << slot_no))) {
		// 接続中バンクに転送
		u32			ofst_addr	= mtVramGetOfstFromVKeyTex(cha_addr) - (slot_no << 17);
		GXVRamTex	org_tex_bank;

		// バンクをLCDCにマッピング
		org_tex_bank = GX_ResetBankForTex();

		act_ss->act.cha_vram = MTE_CHA_VRAM_ADDRESS;
		act_ss->act.cha_addr = g_obj.db_tex_slot_at_lcdc[g_obj.db_tex_vram_flip][slot_no] + ofst_addr;
		mtActRestoreSS(act_ss);

		// 復帰
		act_ss->act.cha_vram = MTE_CHA_VRAM_TEXTURE;
		act_ss->act.cha_addr = cha_addr;

		// バンク復元
		GX_SetBankForTex(org_tex_bank);
	}
#if defined (MTD_DEBUG)
	else {
		MTM_ASSERT(0);
	}
#endif // #if defined (MTD_DEBUG)

	// フラグ復帰
	act_ss->act.flag = act_flag;

#else
	u32		act_flag;

	MTM_ASSERT(act_ss);

	act_flag = act_ss->act.flag;
	act_ss->act.flag |= MTD_ACT_FLAG_NO_REQ_CHA;
	if (!(act_ss->act.flag & MTD_ACT_FLAG_NO_REQ_PLT)) {
		act_ss->act.flag |= MTD_ACT_FLAG_NO_PLT;
	}

	// 通常転送バンクに転送
	mtActResetStructSS(act_ss, act_id);
	mtActRestoreSS(act_ss);

	if (g_obj.db_tex_tcb) {
		// 接続中バンクに転送
		u32	cha_addr	= act_ss->act.cha_addr;
		u32	slot_no		= (u32)(mtVramGetOfstFromVKeyTex(cha_addr) >> 17);
		u32 ofst_addr	= mtVramGetOfstFromVKeyTex(cha_addr) - (slot_no << 17);

		//act_ss->act.cha_vram = MTE_CHA_VRAM_ADDRESS;
		act_ss->act.cha_addr = g_obj.db_tex_slot_at_lcdc[g_obj.db_tex_vram_flip][slot_no] + ofst_addr;
		mtActRestoreSS(act_ss);

		// 復帰
		//act_ss->act.cha_vram = MTE_CHA_VRAM_TEXTURE;
		act_ss->act.cha_addr = cha_addr;
	}
#if defined (MTD_DEBUG)
	else {
		MTM_ASSERT(0);
	}
#endif // #if defined (MTD_DEBUG)

	// フラグ復帰
	act_ss->act.flag = act_flag;
#endif
}
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
void ObjObjectUpdateTransActVramDB(const MTS_ACT_COMMAND *cmd, MTS_ACTION *act, OBS_ACTION_TEXVRAM_DB_WORK *act_texdb)
{
	switch (cmd->cmd_id) {
	case MTE_ACT_COMMAND_CHARACTER:
		// 転送済みフラグ設定
		act_texdb->trans_flag = TRUE;
		break;
	}
}

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
void ObjObjectCollisionSet ( OBS_OBJECT_WORK* pObj, OBS_COLLISION_WORK * pCol, s16 sOfstX, s16 sOfstY, u16 usWidth, u16 usHeight)
{
    // 地形ワークチェック
    if ( pCol == NULL ){
        if ( pObj->col_work ){
            pCol = pObj->col_work;
        } else{
            // メモリ取得
            pCol = (OBS_COLLISION_WORK*)mtMemAllocMain( sizeof(OBS_COLLISION_WORK) );
            MI_CpuClear8( pCol, sizeof(OBS_COLLISION_WORK));
            pObj->flag |= OBD_OBJECT_FREE_COL;
        }
    }
    pObj->col_work = pCol;

    // 親設定
    pCol->obj_col.obj  = pObj;
    
    // 地形キャラサイズ設定
    pCol->obj_col.ofst_x   = sOfstX;
    pCol->obj_col.ofst_y   = sOfstY;
    pCol->obj_col.width  = usWidth; 
    pCol->obj_col.height = usHeight;
}

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
void ObjObjectCollisionDifSet ( OBS_OBJECT_WORK* pObj, const char* pPath, OBS_DATA_WORK* pData, void * pArchive )
{
    if ( pObj->col_work ){
        pObj->col_work->diff_data_work = pData;
        // 地形ロード
        pObj->col_work->obj_col.diff_data = (s8*)ObjDataLoad( pData, pPath, pArchive );
    }
}

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
void ObjObjectCollisionDirSet ( OBS_OBJECT_WORK* pObj, const char* pPath, OBS_DATA_WORK* pData, void * pArchive )
{
    if ( pObj->col_work ){
        pObj->col_work->dir_data_work = pData;
        // 地形ロード
        pObj->col_work->obj_col.dir_data = (u8*)ObjDataLoad( pData, pPath, pArchive );
    }
}

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
void ObjObjectCollisionAtrSet ( OBS_OBJECT_WORK* pObj, const char* pPath, OBS_DATA_WORK* pData, void * pArchive )
{
    if ( pObj->col_work ){
        pObj->col_work->attr_data_work = pData;
        // 地形ロード
        pObj->col_work->obj_col.attr_data = (u8*)ObjDataLoad( pData, pPath, pArchive );
    }
}


//----- Local Functions ------------------------------------------------
// ================================================================
// test_func
/*!
  テスト関数
 
  @param param0 [in] 入力引数0説明
  @param param1 [out] 出力ポインタ引数1説明
  @param param2 [io] 入出力ポインタ引数2説明
 
  @return   返値説明
 
  @note
  補足説明
 */
// ================================================================

