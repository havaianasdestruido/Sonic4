// ================================================================
/*!
    Copyright(c) 2009 Dimps CORP. All Rights Reserved. 

	@file       amTrail.cpp
	@brief      軌跡ライブラリ
	@author	    Syuichi Gotou
	@date	    Date: 2009/09/24 
 */
// ================================================================

//----- Include Files --------------------------------------------------
#include "alice.h"

//----- Macros ---------------------------------------------------------

#define AMD_TRAILEF_BUFSIZE			(8)
#define AMD_TRAIL_COMP_FRAME2		(6)			//!< フレーム補間軌跡サンプリングフレーム
#define AMD_TRAIL_SAMPLING_NUM		(4)			//!< スプライン補間サンプリング数

//----- Enum Definitions -----------------------------------------------

//----- Type Definitions -----------------------------------------------

// スプライン補間
typedef struct _AMTRS_FC_PARAM {
	float		m_x[AMD_TRAIL_SAMPLING_NUM];
	float		m_y[AMD_TRAIL_SAMPLING_NUM];
	float		m_z[AMD_TRAIL_SAMPLING_NUM];
	float		m_Dx[AMD_TRAIL_SAMPLING_NUM];
	float		m_Dy[AMD_TRAIL_SAMPLING_NUM];
	float		m_Dz[AMD_TRAIL_SAMPLING_NUM];
	AMS_VECTOR	m_CalcParam;
	float		m_t;
	Uint32		m_flag;
} AMTRS_FC_PARAM;

//----- Local Variables -----------------------------------------------
static AMS_TRAIL_INTERFACE	AmTrailGlobal;		//!< 軌跡管理グローバル (実体)

// 軌跡エフェクト管理用グローバル
static AMS_TRAIL_EFFECT		_amTrailEF_head;
static AMS_TRAIL_EFFECT		_amTrailEF_tail;

// 軌跡エフェクトバッファ領域
static AMS_TRAIL_EFFECT		_amTrailEF_buf[AMD_TRAILEF_BUFSIZE];
static AMS_TRAIL_EFFECT*	_amTrailEF_ref[AMD_TRAILEF_BUFSIZE];

static Sint32				_amTrailEF_alloc;	// 
static Sint32				_amTrailEF_free;	// 

#if AMD_DEBUG
static Sint32				_amTrailEF_num;		// 使用中のランタイム数
static Sint32				_amTrailEF_peak;	// 
#endif


//----- Local Functions -----------------------------------------------

// 軌跡エフェクトシステム
static void _amTrailEFDelete(AMS_TRAIL_EFFECT* pEffect);
static AMS_TRAIL_EFFECT* _amTrailEFMake(Uint16 handleId);	// 軌跡エフェクトの作成
static AMS_TRAIL_EFFECT* _amTrailEFAlloc(void);				// 軌跡エフェクトワーク領域の確保
static void				 _amTrailEFFree(AMS_TRAIL_EFFECT* runtime);		// 軌跡エフェクトワーク領域の開放
static void				 _amTrailEFDeleteEffectReal(AMS_TRAIL_EFFECT* runtime);	// 軌跡エフェクトの実削除

// 通常軌跡
static void		_amTrailInitNormal(AMS_TRAIL_EFFECT* pEffect);
static void		_amTrailFinalizeNormal(AMS_TRAIL_EFFECT* pEffect);
static Sint32	_amTrailUpdateNormal(AMS_TRAIL_EFFECT* pEffect);
static void		_amTrailDrawNormal(AMS_TRAIL_EFFECT* pEffect);
static void		_amTrailDrawPartsNormal(AMS_TRAIL_PARTS* pNow, AMS_TRAIL_PARAM* work, NNS_PRIM3D_PC* pv); // テクスチャなし
static void		_amTrailDrawPartsNormalTex(AMS_TRAIL_PARTS* pNow, AMS_TRAIL_PARAM* work, NNS_PRIM3D_PCT* pv); // テクスチャあり

static void		_amTrailAddParts(AMS_TRAIL_PARTS* pNew, AMS_TRAIL_PARAM* work);
static void     _amTrailAddPosition(AMS_TRAIL_EFFECT* pEffect, NNS_VECTOR* offset);



// スプライン補間用
static Sint32	_amTrailCalcSplinePos(NNS_VECTOR Pos[], NNS_VECTOR Dir[], AMS_TRAIL_PARTS* pNPP, AMS_TRAIL_PARTS* pNP, 
									  AMS_TRAIL_PARTS* pNow, AMS_TRAIL_PARTS* pNext, float len, Sint32 MaxComp = AMD_TRAIL_COMP_FRAME2);

static Sint32	_amTrailCalcSplinePos(NNS_VECTOR* pos, NNS_VECTOR* dir, AMTRS_FC_PARAM* FcWk,
									  float len, Sint32 MaxComp=AMD_TRAIL_COMP_FRAME2);

static void		_amTrailCalcSpline( AMTRS_FC_PARAM* param, float* P );
static float	_amTrailGetValue( AMTRS_FC_PARAM* param, float t);


//----- Global Variables ------------------------------------------------

AMS_TRAIL_INTERFACE* pTr = &AmTrailGlobal;		//!< 軌跡エフェクトグローバル

//----- Global Functions -----------------------------------------------

//{### 軌跡エフェクトシステムインターフェース
// ================================================================
// amTrailEFInitialize
/*!
	軌跡エフェクトシステム起動
	NULL ← _amTrailEF_head ⇔ _amTrailEF_tail → NULL
*/
// ================================================================
void amTrailEFInitialize(void)
{
	// 軌跡エフェクト双方向リスト初期化
	amZeroMemory( &_amTrailEF_head, sizeof( AMS_TRAIL_EFFECT ) );
	amZeroMemory( &_amTrailEF_tail, sizeof( AMS_TRAIL_EFFECT ) );
	_amTrailEF_head.pNext = &_amTrailEF_tail;
	_amTrailEF_head.pPrev = NULL;
	_amTrailEF_tail.pNext = NULL;
	_amTrailEF_tail.pPrev = &_amTrailEF_head;

	// 軌跡管理グローバル初期化
	amZeroMemory(pTr, sizeof(AMS_TRAIL_INTERFACE));

	// ランタイムバッファ初期化
	{
		_amTrailEF_alloc	= 0;
		_amTrailEF_free		= 0;
#if AMD_DEBUG
		_amTrailEF_num		= 0;
		_amTrailEF_peak		= 0;
#endif
		amZeroMemory( _amTrailEF_buf, sizeof(_amTrailEF_buf) );
		for ( int i = 0; i < AMD_TRAILEF_BUFSIZE; i++ )
		{
			_amTrailEF_ref[i] = &( _amTrailEF_buf[i] );
		}
	}
}

// ================================================================
// amTrailEFUpdate
/*!
	軌跡エフェクトシステムの更新（メインスレッドで呼ぶこと）
	
	@param handleId	ハンドルID
*/
// ================================================================
void amTrailEFUpdate(Uint16 handleId)
{
	amAssert( handleId );	// 不正なハンドルID

	AMS_TRAIL_EFFECT* pEffect;
	AMS_TRAIL_PARAM* work;
	
	// 更新ループ
	for ( pEffect = _amTrailEF_head.pNext; pEffect != &_amTrailEF_tail; pEffect = pEffect->pNext )
	{
		if ( pEffect->Procedure != 0 && (pEffect->handleId & handleId) && (pEffect->Procedure != (void *)0xffffffff))
		{
			// プロシージャ実行
			pEffect->Procedure( pEffect );
			work = (AMS_TRAIL_PARAM*)pEffect->Work;

			if ( work->state & AMTRE_STATE_DELETE )
			{
				_amTrailEFDelete( pEffect );
			}
			else
			{
				// フレーム更新
				pEffect->fFrame	= pEffect->fFrame + amEffectGetUnitFrame();

				if ( (pEffect->fEndFrame > 0.0f) && (pEffect->fFrame > pEffect->fEndFrame) )
				{
					// 削除(前処理)
					_amTrailEFDelete( pEffect );
				}
			}
		}
	}

	// 削除ループ
	for ( pEffect = _amTrailEF_head.pNext; pEffect != &_amTrailEF_tail; pEffect = pEffect->pNext )
	{
		if ( (Sint32)pEffect->Procedure == -1 )
		{
			// 削除
			_amTrailEFDeleteEffectReal( pEffect );
		}
	}
}

// ================================================================
// amTrailEFDraw
/*!
	軌跡エフェクト描画（メインスレッドで呼ぶこと）
	
	@param handleId	ハンドルID
	@param texlist  テクスチャリスト
	@param state    描画ステート
*/
// ================================================================
void amTrailEFDraw(Uint16 handleId, NNS_TEXLIST* texlist, Uint32 state)
{
	amAssert( handleId );	// 不正なハンドルID

	AMS_TRAIL_EFFECT* pEffect;
	AMS_TRAIL_PARAM* work;
	
	// 描画ループ
	for ( pEffect = _amTrailEF_head.pNext; pEffect != &_amTrailEF_tail; pEffect = pEffect->pNext )
	{
		if ( pEffect->Procedure != 0 && (pEffect->handleId & handleId) && (pEffect->Procedure != (void *)0xffffffff))
		{
			pEffect->drawState = state;
			work = (AMS_TRAIL_PARAM*)pEffect->Work;
			work->texlist = texlist;
			_amTrailDrawNormal( pEffect ); // 描画
		}
	}
}

// ================================================================
// amTrailEFDeleteGroup
/*!
	軌跡エフェクトのグループ削除
	
	@param handleId	ハンドルID
	@note  プロシージャの停止およびデストラクタを実行する  
*/
// ================================================================
void amTrailEFDeleteGroup(Uint16 handleId)
{
	AMS_TRAIL_EFFECT* pEffect;

	pEffect = _amTrailEF_head.pNext;
	while ( pEffect != &_amTrailEF_tail )
	{
		if ( pEffect->handleId & handleId )
		{
			_amTrailEFDelete( pEffect );
		}

		pEffect = pEffect->pNext;
	}
}

// ================================================================
// amTrailEFOffsetPos
/*!
	軌跡エフェクトの位置オフセット
	
	@param handleId	ハンドルID
	@param offset	オフセット
	@note  関数使用後、軌跡のベース位置にもオフセット値を加算しておくこと 
*/
// ================================================================
void amTrailEFOffsetPos(Uint16 handleId, NNS_VECTOR* offset)
{
	AMS_TRAIL_EFFECT* pEffect;

	pEffect = _amTrailEF_head.pNext;
	while ( pEffect != &_amTrailEF_tail )
	{
		if ( pEffect->handleId & handleId )
		{
			_amTrailAddPosition( pEffect, offset );
		}

		pEffect = pEffect->pNext;
	}
}


//{### 軌跡登録
// ================================================================
// amTrailMakeEffect
/*!
	軌跡作成  

	@param	param     : 軌跡パラメーター
	@param	handleId  : ハンドルＩＤ
	@param  flag      : 位置データに固定小数使うかなどのフラグ
	@note param で設定していないパラメータはちゃんと０クリアーしておくこと
*/
// ================================================================
void amTrailMakeEffect(AMS_TRAIL_PARAM* param, Uint16 handleId, Sint16 flag)
{
	amAssert( pTr->trailNum != AMD_TRAIL_MAX );	// 軌跡最大数オーバー
	pTr->trailNum++;

	AMS_TRAIL_EFFECT* pEffect = _amTrailEFMake( handleId );
	amAssert( pEffect );
	if ( pEffect == NULL ) return;
	{
		pEffect->Procedure	= (AMTREffectProc)_amTrailUpdateNormal;
		pEffect->Destractor	= (AMTREffectProc)_amTrailFinalizeNormal;
		pEffect->fEndFrame	= -1.0f;
		pEffect->flag = flag;
	}

	// 軌跡パラメータ設定
	AMS_TRAIL_PARAM* work = (AMS_TRAIL_PARAM*)pEffect->Work;
	memcpy(work, param, sizeof(AMS_TRAIL_PARAM));
	work->time = work->life * amEffectGetUnitFrame();
	work->trailId = pTr->trailId;

	// 前のが残っていたら消去
	if ( pTr->trailEffect[work->trailId] ) {
    	_amTrailEFDelete( pTr->trailEffect[work->trailId] );
	}
	pTr->trailEffect[work->trailId] = pEffect;

	// 初期化処理
	_amTrailInitNormal(pEffect);

	// 次の軌跡作成に備える
	pTr->trailId++;
	if (pTr->trailId >= AMD_TRAIL_MAX ) pTr->trailId = 0;
}

//----- Local Functions -----------------------------------------------

// ================================================================
// amTrailEFDelete
/*!
	軌跡エフェクトの削除(前処理)
	
	@param	pEffect		削除する軌跡エフェクト
	@note	プロシージャの停止およびデストラクタを実行する  
*/
// ================================================================
void _amTrailEFDelete(AMS_TRAIL_EFFECT* pEffect)
{
	amAssert( pEffect );
	pEffect->Procedure = (AMTREffectProc) - 1;

	if ( pEffect->Destractor != 0 && (pEffect->Destractor != (void *)0xffffffff))
	{
		pEffect->Destractor( pEffect );
	}
}

// ================================================================
// _amTrailEFMake
/*!
	軌跡エフェクトの作成

	@Uint32 handle_id	ハンドルID
	@return	AMS_TRAIL_EFFECT*  軌跡エフェクトデータ
	_amTrailEF_head ⇔ EF ... EF ⇔ NewEF ⇔ _amTrailEF_tail → NULL
*/
// ================================================================
AMS_TRAIL_EFFECT* _amTrailEFMake(Uint16 handleId)
{
	amAssert( handleId ); // 不正なハンドルID

	AMS_TRAIL_EFFECT* pEffect = (AMS_TRAIL_EFFECT*)_amTrailEFAlloc();
	if ( pEffect == NULL ) return NULL;
	amZeroMemory(pEffect, sizeof(AMS_TRAIL_EFFECT));

	// Tail の前につなげる
	_amTrailEF_tail.pPrev->pNext	= pEffect;
	pEffect->pPrev					= _amTrailEF_tail.pPrev;
	_amTrailEF_tail.pPrev			= pEffect;
	pEffect->pNext					= &_amTrailEF_tail;

	pEffect->handleId				= handleId;

	return pEffect;
}

// ================================================================
/*!
	軌跡エフェクトワーク領域の確保
	@param	[o]		ランタイムワークのポインタ
*/
// ================================================================
AMS_TRAIL_EFFECT* _amTrailEFAlloc()
{
#if AMD_DEBUG
	amAssert( _amTrailEF_num < AMD_TRAILEF_BUFSIZE );	// ランタイムサイズオーバー
	_amTrailEF_num++;
	if ( _amTrailEF_peak < _amTrailEF_num )
	{
		_amTrailEF_peak = _amTrailEF_num;
	}
#endif

	AMS_TRAIL_EFFECT* new_effect = _amTrailEF_ref[ _amTrailEF_alloc ];
	++_amTrailEF_alloc;
	if ( _amTrailEF_alloc >= AMD_TRAILEF_BUFSIZE ) _amTrailEF_alloc = 0;

	amZeroMemory( new_effect, sizeof(AMS_TRAIL_EFFECT) );

	return new_effect;
}

// ================================================================
/*!
	軌跡エフェクトワーク領域の開放
	@param	[o]		ランタイムワークのポインタ
*/
// ================================================================
void _amTrailEFFree(AMS_TRAIL_EFFECT* pEffect)
{
	amAssert( pEffect );
#if AMD_DEBUG
	_amTrailEF_num--;
	amAssert( _amTrailEF_num >= 0 );
#endif

	_amTrailEF_ref[ _amTrailEF_free ] = pEffect;
	_amTrailEF_free++;
	if ( _amTrailEF_free >= AMD_TRAILEF_BUFSIZE ) _amTrailEF_free = 0;
}

// ================================================================
// _amTrailEFDeleteEffectReal
/*!
	軌跡エフェクトの削除
	
	@param pEffect	: 削除するエフェクトデータ
	@note
	リンクリストからの切断およびワークの開放を行う
*/
// ================================================================
static void _amTrailEFDeleteEffectReal(AMS_TRAIL_EFFECT* pEffect)
{
	amAssert( pEffect );

	pEffect->pPrev->pNext = pEffect->pNext;
	pEffect->pNext->pPrev = pEffect->pPrev;
	
	_amTrailEFFree(pEffect);
}

//## Local Functions
// ================================================================
/*!
	軌跡の初期化
	pHead ⇔ pNew ⇔ pTail
*/
// ================================================================
static void   _amTrailInitNormal(AMS_TRAIL_EFFECT* pEffect)
{
	amAssert( pEffect );
	AMS_TRAIL_PARAM* work = (AMS_TRAIL_PARAM*)pEffect->Work;
	AMS_TRAIL_PARTSDATA* trData = &pTr->trailData[work->trailId];
	AMS_TRAIL_PARTS* pNew = &trData->parts[0];
	AMS_TRAIL_PARTS* pTail = &trData->trailTail;
	AMS_TRAIL_PARTS* pHead = &trData->trailHead;

	// 最初の一個は、方向を持っていないため描画時には使わないように注意
	amZeroMemory( trData, sizeof(AMS_TRAIL_PARTSDATA) );
	pNew->pNext = pTail;
	pTail->pPrev = pNew;
	pNew->pPrev = pHead;
	pHead->pNext = pNew;

	if ( pEffect->flag & AMTRE_FLAG_FXPOS )
	{
		// hog専用なので注意
		pNew->pos.x = AMD_FX32_TO_FLOAT(work->trail_pos->x);
		pNew->pos.y = -AMD_FX32_TO_FLOAT(work->trail_pos->y);
		pNew->pos.z = AMD_FX32_TO_FLOAT(work->zBias);
	}
	else
	{
	    nnCopyVector( &pNew->pos,  work->trail_pos0 );
	}
	pNew->time = work->life;
	pNew->partsId = 0;
	work->trailPartsId = 1;
	work->trailPartsNum++;
}

// ================================================================
/*!
	軌跡の終了処理
*/
// ================================================================
static void   _amTrailFinalizeNormal(AMS_TRAIL_EFFECT* pEffect)
{
	amAssert( pEffect );
	AMS_TRAIL_PARAM* work = (AMS_TRAIL_PARAM*)pEffect->Work;

	if ( pTr->trailNum > 0 ) {
		pTr->trailNum--;
		if ( pTr->trailNum == 0 ) {
			pTr->trailState &= ~AMTRE_STATE_DELETE;
		}
	}
	pTr->trailEffect[work->trailId] = NULL;
}

// ================================================================
/*!
	軌跡の更新処理
*/
// ================================================================
static Sint32 _amTrailUpdateNormal(AMS_TRAIL_EFFECT* pEffect)
{
	amAssert( pEffect );
	AMS_TRAIL_PARAM* work = (AMS_TRAIL_PARAM*)pEffect->Work;
	AMS_TRAIL_PARTSDATA* trData = &pTr->trailData[work->trailId];
	AMS_TRAIL_PARTS* pNew = &trData->parts[work->trailPartsId];
	AMS_TRAIL_PARTS* pTail = &trData->trailTail;
	AMS_TRAIL_PARTS* pHead = &trData->trailHead;
	
	if ( work->state & AMTRE_STATE_DELETE ) return 1;

	// リストが一つも連結されてない
	if (pTail->pPrev == NULL) {
    	amAssert(0);
	}
	
	// 使用リスト再連結
	if ( pNew->pNext != NULL && pNew == pHead->pNext )
	{
		pHead->pNext->pNext->pPrev = pHead;
		pHead->pNext = pHead->pNext->pNext;
	}
	
	amZeroMemory( pNew, sizeof(AMS_TRAIL_PARTS) );
	pNew->pNext = pTail; 
	pNew->pPrev = pTail->pPrev;
	pTail->pPrev = pNew;
	pNew->pPrev->pNext = pNew;

	if ( pEffect->flag & AMTRE_FLAG_FXPOS )
	{
		// hog専用なので注意
		pNew->pos.x = AMD_FX32_TO_FLOAT(work->trail_pos->x);
		pNew->pos.y = -AMD_FX32_TO_FLOAT(work->trail_pos->y);
		pNew->pos.z = AMD_FX32_TO_FLOAT(work->zBias);
	}
	else
	{
	    nnCopyVector( &pNew->pos,  work->trail_pos0 );
	}
	nnSubtractVector( &pNew->dir, &pNew->pos, &pNew->pPrev->pos );

	// 方向がない場合、軌跡の幅が無くなるので、とりあえず進行方向を入れておく
	if ( amIsZerof(pNew->dir.x) && amIsZerof(pNew->dir.y) && amIsZerof(pNew->dir.z) )
	{
		pNew->dir.x = 1.0f;
	}
//	amUnitVector( &pNew->dir );
	_amTrailAddParts( pNew, work );
	pNew->m_Flag |= AMTRE_MFLAG_PRAIMAL;

#if (0)
	// ４点スプライン補間
	if ( work->trailPartsNum >= 3 ) {
	
		NNS_VECTOR pos23[AMD_TRAIL_COMP_FRAME2];
		NNS_VECTOR dir23[AMD_TRAIL_COMP_FRAME2];
		AMS_TRAIL_PARTS* pNow = pNew->pPrev;
		float len23;
		Sint32 i, hokan23 = 0;
		AMS_TRAIL_PARTS* pNP = pNow->pPrev;

		// スプライン補間（根元）
		len23 = nnDistanceVector( &pNew->pos, &pNow->pos );
		hokan23 = _amTrailCalcSplinePos(pos23, dir23, pNP, pNow, pNew, NULL, len23);

		// リスト連結準備
		AMS_TRAIL_PARTS* pNow23 = pNew;
		
		// リスト連結（末尾の前）
		for ( i = 0; i < hokan23; i++ ) {
			pNew = &trData->parts[work->trailPartsId];
			if ( pNew->pNext != NULL && pNew == pHead->pNext )
			{
				pHead->pNext->pNext->pPrev = pHead;
				pHead->pNext = pHead->pNext->pNext;
			}
			amZeroMemory( pNew, sizeof(AMS_TRAIL_PARTS) );
			pNew->pNext = pNow23; 
			pNew->pPrev = pNow23->pPrev;
			pNow23->pPrev = pNew;
			pNew->pPrev->pNext = pNew;
			nnCopyVector( &pNew->pos, &pos23[i] );
			nnCopyVector( &pNew->dir, &dir23[i] );
//			amSubVector( &pNew->dir, &pNew->pos, &pNew->pPrev->pos );
//			amSubVector( &pNew->pNext->dir, &pNew->pNext->pos, &pNew->pos );
			_amTrailAddParts( pNew, work );
		}
	}
#endif
		
	// update frame
	work->time -= amEffectGetUnitFrame();
	if ( work->time < 0.0f )
	{
		work->time = 0.0f;
		work->state |= AMTRE_STATE_DELETE;
	}
	
	return 0;
}

// ================================================================
/*!
	軌跡の描画処理
*/
// ================================================================
static void   _amTrailDrawNormal(AMS_TRAIL_EFFECT* pEffect)
{
	amAssert( pEffect );
	AMS_TRAIL_PARAM* work = (AMS_TRAIL_PARAM*)pEffect->Work;
	AMS_TRAIL_PARTSDATA* trData = &pTr->trailData[work->trailId];
	AMS_TRAIL_PARTS* pTail = &trData->trailTail;
	AMS_TRAIL_PARTS* pHead = &trData->trailHead;
	AMS_TRAIL_PARTS* pNow = pTail->pPrev;

	// 更新処理をされずに描画されている
	if (pTail->pPrev->pPrev == pHead) {
    	return;
	}
	amAssert(pNow->pPrev);

	if ( work->time <= 0.0f ) return;

	NNS_RGBA	color = work->startColor;
	NNS_RGBA	ptclcolor = work->ptclColor;
	NNS_VECTOR	ofst, cross, offset;
	float size, ptclsize, rate = 1.0f, len = 0.0f;

	AMS_PARAM_DRAW_PRIMITIVE param;
	memset(&param, 0, sizeof(AMS_PARAM_DRAW_PRIMITIVE));
	NNE_PRIM_ALPHABLEND blend = NNE_PRIM_ALPHABLEND_ON;
	amDrawGetPrimBlendParam((AMDRAWE_BLENDTYPE)work->blendType, &param);
	if (work->zTest )
	{
	    param.zTest = 1;
	}
	if (work->zMask)
	{
		param.zMask = 1;
	}


//	amVectorSet(&offset, work->zBias * NNM_MTX(_am_ef_worldViewMtx, 2, 0),
//		work->zBias * NNM_MTX(_am_ef_worldViewMtx, 2, 1), work->zBias * NNM_MTX(_am_ef_worldViewMtx, 2, 2));
	amVectorSet(&offset, 0.0f, 0.0f, 1.0f);

	if ( work->time < work->vanish_time )
	{
		rate = work->time / work->vanish_time;
		//rate *= rate;
	}

	work->vanish_rate = rate;
	color.a = work->startColor.a * rate;
	ptclcolor.a = work->ptclColor.a * rate;
	size = work->startSize /** rate*/;
	ptclsize = work->ptclSize /** rate*/;

	// 根元パーティクル描画
	if (work->ptclFlag && (work->ptclTexId != -1))
	{
		NNS_PRIM3D_PCT *poliData = (NNS_PRIM3D_PCT *)amDrawMallocDataBuffer(sizeof(NNS_PRIM3D_PCT) * 6);
		float hw = ptclsize * 0.5f;
		float hh = ptclsize * 0.5f;

		// カメラとの距離
		float len = nnDistanceVector(&pNow->pos, &_am_ef_camPos);

		// 頂点
		amVectorSet((NNS_VECTOR*)&poliData[0], pNow->pos.x - hw, pNow->pos.y + hh, pNow->pos.z);
		amVectorSet((NNS_VECTOR*)&poliData[1], pNow->pos.x + hw, pNow->pos.y + hh, pNow->pos.z);
		amVectorSet((NNS_VECTOR*)&poliData[2], pNow->pos.x - hw, pNow->pos.y - hh, pNow->pos.z);
		amVectorSet((NNS_VECTOR*)&poliData[5], pNow->pos.x + hw, pNow->pos.y - hh, pNow->pos.z);

		// カラー
		poliData[0].Col = AMD_FCOLTORGBA8888(ptclcolor.r, ptclcolor.g, ptclcolor.b, ptclcolor.a);
		poliData[1].Col = poliData[2].Col = poliData[5].Col = poliData[0].Col;

		// UV
		poliData[0].Tex.u = 0.0f; poliData[0].Tex.v = 0.0f;
		poliData[1].Tex.u = 1.0f; poliData[1].Tex.v = 0.0f;
		poliData[2].Tex.u = 0.0f; poliData[2].Tex.v = 1.0f;
		poliData[5].Tex.u = 1.0f; poliData[5].Tex.v = 1.0f;

		poliData[3] = poliData[1];
		poliData[4] = poliData[2];

		// プリミティブ描画設定
		param.format3D = NNE_PRIM3D_FMT_PCT;
		param.type = NNE_PRIM_TRIANGLE_LIST;
		param.vtxPCT3D = poliData;
		param.texlist = work->texlist;
		param.texId = work->ptclTexId;
		param.count = 6;
		param.ablend = blend;
		param.sortZ = len;
		amDrawPrimitive3D(pEffect->drawState, &param);
	}
	
	// 無理やり描く必要はない
	if ( work->trailPartsNum < 3 ) {
		return;
	}

	// テクスチャなし
	if ( work->texlist == NULL || work->texId == -1 )
	{
		NNS_PRIM3D_PC* pv = (NNS_PRIM3D_PC *)amDrawMallocDataBuffer(sizeof(NNS_PRIM3D_PC) * 6 * (work->trailPartsNum-1));
		NNS_PRIM3D_PC* head = pv; // 先頭保存

		// カメラからの距離
		len = nnDistanceVector(&pNow->pos, &_am_ef_camPos);

		nnCrossProductVector( &cross, &offset, &( pNow->dir ) );
		nnNormalizeVector( &cross, &cross );
		nnScaleVector( &ofst, &cross, size );
		nnAddVector( &pv[0].Pos, &( pNow->pos ), &ofst ); // 左上
		nnAddVector( &pv[1].Pos, &( pNow->pPrev->pos ), &ofst ); // 右上
		nnSubtractVector( &pv[2].Pos, &( pNow->pos ), &ofst ); // 左下
		nnSubtractVector( &pv[5].Pos, &( pNow->pPrev->pos ), &ofst ); // 右下
		
		// Color
		pv[5].Col = AMD_FCOLTORGBA8888(color.r, color.g, color.b, color.a);
		pv[0].Col = pv[1].Col = pv[2].Col = pv[5].Col;

		pv[3] = pv[1];
		pv[4] = pv[2];

		pv += 6;
		pNow = pNow->pPrev;
		work->list_no = 1;

		// 残像描画
		while ( pNow != pHead->pNext ) {
			pNow->m_Flag &= ~AMTRE_MFLAG_CHECK1;
			work->list_no++;
			_amTrailDrawPartsNormal( pNow, work, pv );
			pNow = pNow->pPrev;
			pv = pv + 6;
		}

		param.format3D = NNE_PRIM3D_FMT_PC;
		param.type = NNE_PRIM_TRIANGLE_LIST;
		param.vtxPC3D = head;
		param.texlist = work->texlist;
		param.texId = work->texId;
		param.count = 6 * (work->trailPartsNum-1);
		param.ablend = blend;
		param.sortZ = len;
		amDrawPrimitive3D(pEffect->drawState, &param);
	}
	// テクスチャあり
	else
	{
		NNS_PRIM3D_PCT* pv = (NNS_PRIM3D_PCT *)amDrawMallocDataBuffer(sizeof(NNS_PRIM3D_PCT) * 6 * (work->trailPartsNum-1));
		NNS_PRIM3D_PCT* head = pv; // 先頭保存
		float rate0 = (float)(work->trailPartsNum - 1) / (float)work->trailPartsNum;
		rate0 *= work->vanish_rate;

		// カメラからの距離
		len = nnDistanceVector(&pNow->pos, &_am_ef_camPos);

		nnCrossProductVector( &cross, &offset, &( pNow->dir ) );
		nnNormalizeVector( &cross, &cross );
		nnScaleVector( &ofst, &cross, size );
		nnAddVector( &pv[0].Pos, &( pNow->pos ), &ofst ); // 左上
		nnAddVector( &pv[1].Pos, &( pNow->pPrev->pos ), &ofst ); // 右上
		nnSubtractVector( &pv[2].Pos, &( pNow->pos ), &ofst ); // 左下
		nnSubtractVector( &pv[5].Pos, &( pNow->pPrev->pos ), &ofst ); // 右下
		
		// Color
		pv[5].Col = AMD_FCOLTORGBA8888(color.r, color.g, color.b, color.a);
		pv[0].Col = pv[1].Col = pv[2].Col = pv[5].Col;

		// テクスチャ貼られ位置確認用
#if (0)
		pv[0].Col = AMD_FCOLTORGBA8888(1.0f, 0.0f, 0.0f, color.a);
		pv[1].Col = AMD_FCOLTORGBA8888(0.0f, 1.0f, 0.0f, color.a);
		pv[2].Col = AMD_FCOLTORGBA8888(0.0f, 0.0f, 1.0f, color.a);
		pv[5].Col = AMD_FCOLTORGBA8888(1.0f, 1.0f, 0.0f, color.a);
#endif

		// Texture
		pv[0].Tex.u = 1.0f; pv[0].Tex.v = 0.0f;
	    pv[1].Tex.u = rate0; pv[1].Tex.v = 0.0f;
	    pv[2].Tex.u = 1.0f; pv[2].Tex.v = 1.0f;
	    pv[5].Tex.u = rate0; pv[5].Tex.v = 1.0f;

		pv[3] = pv[1];
		pv[4] = pv[2];

		pv += 6;
		pNow = pNow->pPrev;
		work->list_no = 1;

		// 残像描画
		while ( pNow != pHead->pNext ) {
			pNow->m_Flag &= ~AMTRE_MFLAG_CHECK1;
			work->list_no++;
			_amTrailDrawPartsNormalTex( pNow, work, pv );
			pNow = pNow->pPrev;
			pv = pv + 6;
		}

		param.format3D = NNE_PRIM3D_FMT_PCT;
		param.type = NNE_PRIM_TRIANGLE_LIST;
		param.vtxPCT3D = head;
		param.texlist = work->texlist;
		param.texId = work->texId;
		param.count = 6 * (work->trailPartsNum-1);
		param.ablend = blend;
		param.sortZ = len;
		amDrawPrimitive3D(pEffect->drawState, &param);
	}
}

// ================================================================
// _amTrailDrawPartsNormal
/*!
	軌跡・残像描画（テクスチャなし）

	@param	pNow    : 軌跡データ
	@param	work    : 軌跡パラメータ
	@param	poliData    : ポリゴン頂点データ
*/
// ================================================================
static void _amTrailDrawPartsNormal(AMS_TRAIL_PARTS* pNow, AMS_TRAIL_PARAM* work, NNS_PRIM3D_PC* pv)
{
	float rate, len;
	NNS_RGBA color;
	NNS_PRIM3D_PC* prev = pv - 6;
	float size = work->startSize;
	    
	// 色やサイズの補間
	rate = (float)(work->trailPartsNum - work->list_no) / (float)work->trailPartsNum;
	size = work->startSize * rate + work->endSize * (1.0f - rate);
	rate *= work->vanish_rate;
	color.r = work->startColor.r * rate + work->endColor.r * (1.0f - rate);
	color.g = work->startColor.g * rate + work->endColor.g * (1.0f - rate);
	color.b = work->startColor.b * rate + work->endColor.b * (1.0f - rate);
	color.a = work->startColor.a * rate + work->endColor.a * (1.0f - rate);

	NNS_VECTOR cross, ofst, offset;

	// カメラからの距離
	len = nnDistanceVector(&pNow->pos, &_am_ef_camPos);

//	amVectorSet(&offset, work->zBias * NNM_MTX(_am_ef_worldViewMtx, 2, 0),
//			work->zBias * NNM_MTX(_am_ef_worldViewMtx, 2, 1), work->zBias * NNM_MTX(_am_ef_worldViewMtx, 2, 2));
	amVectorSet(&offset, 0.0f, 0.0f, 1.0f);

	nnCrossProductVector( &cross, &offset, &( pNow->dir ) );
	nnNormalizeVector( &cross, &cross );

	// 頂点の並びは以下のとおり（三角形が二つで四角形を形成）
	//  0-1  3
	//  |/  /|
	//  2  4-5
	// 1,3,5 のみ補間し、0,2,4は一つ前の1,3,5のデータと同じにする
	nnScaleVector( &ofst, &cross, size );
	nnAddVector( &pv[1].Pos, &( pNow->pPrev->pos ), &ofst ); // 右上
	nnSubtractVector( &pv[5].Pos, &( pNow->pPrev->pos ), &ofst ); // 右下

	pv[0] = prev[1];
	pv[4] = pv[2] = prev[5];

	pv[5].Col = AMD_FCOLTORGBA8888(color.r, color.g, color.b, color.a);
	pv[1].Col = pv[5].Col;

	pv[3] = pv[1];

#if (0)
	BTEFS_TRAIL_PARTS_STATUS* pCheckParam = pNow->pNext;
	AMS_VECTOR	CheckPos[2];				//ポジションかぶりチェック
	float		CheckLen;
	amUint16	CheckCount = 2;
	while(pCheckParam){
		if(!(pCheckParam->m_Flag & BTEFS_TWF_CHECK1))	break;
		pCheckParam = pCheckParam->pNext;
		CheckCount += 2;
	}
	CheckLen = amGetLength( &pNow->pos, &pCheckParam->pos );
	amCopyVector3((AMS_VECTOR3*)&CheckPos[0],&pv[-CheckCount].Pos);
	amCopyVector3((AMS_VECTOR3*)&CheckPos[1],&pv[-CheckCount+1].Pos);
	amSubVector(&CheckPos[0],&CheckPos[0],&( pNow->pPrev->pos ));
	amSubVector(&CheckPos[1],&CheckPos[1],&( pNow->pPrev->pos ));
	for(amSint16 i = 0;i < 2;i++){
		if(width > CheckLen || width > amScalor(&CheckPos[i])){
			amOuterProduct( &ofst, &CheckPos[i], &eye );
			ofst.z	= 0.0f;
			amUnitVector( &ofst );
			cross.z	= 0.0f;
			amUnitVector( &cross );
			if(i == 1)
				amScaleVector( &cross,&cross, (-1.0f) );
			if(amInnerProduct(&cross,&ofst) > 0.0f){
				switch(i){
					case 0: amCopyVector3((AMS_VECTOR3*)&pt[0],&pv[-CheckCount].Pos);	break;
					case 1:	amCopyVector3((AMS_VECTOR3*)&pt[1],&pv[-CheckCount+1].Pos);	break;
				}
				pNow->m_Flag |= BTEFS_TWF_CHECK1;
			}
		}
	}
#endif
}

// ================================================================
// _amTrailDrawPartsNormalTex
/*!
	軌跡・残像描画（テクスチャあり）

	@param	pNow    : 軌跡データ
	@param	work    : 軌跡パラメータ
	@param	poliData    : ポリゴン頂点データ
*/
// ================================================================
static void _amTrailDrawPartsNormalTex(AMS_TRAIL_PARTS* pNow, AMS_TRAIL_PARAM* work, NNS_PRIM3D_PCT* pv)
{
	float rate, len;
	NNS_RGBA color;
	NNS_PRIM3D_PCT* prev = pv - 6;
	float size = work->startSize;

	// 色やサイズの補間
	rate = (float)(work->trailPartsNum - work->list_no) / (float)work->trailPartsNum;
	rate *= work->vanish_rate;
	size = work->startSize * rate + work->endSize * (1.0f - rate);
	color.r = work->startColor.r * rate + work->endColor.r * (1.0f - rate);
	color.g = work->startColor.g * rate + work->endColor.g * (1.0f - rate);
	color.b = work->startColor.b * rate + work->endColor.b * (1.0f - rate);
	color.a = work->startColor.a * rate + work->endColor.a * (1.0f - rate);

	NNS_VECTOR cross, ofst, offset;

	// カメラからの距離
	len = nnDistanceVector(&pNow->pos, &_am_ef_camPos);

//	amVectorSet(&offset, work->zBias * NNM_MTX(_am_ef_worldViewMtx, 2, 0),
//			work->zBias * NNM_MTX(_am_ef_worldViewMtx, 2, 1), work->zBias * NNM_MTX(_am_ef_worldViewMtx, 2, 2));
	amVectorSet(&offset, 0.0f, 0.0f, 1.0f);

	nnCrossProductVector( &cross, &offset, &( pNow->dir ) );
	nnNormalizeVector( &cross, &cross );

	// 頂点の並びは以下のとおり（三角形が二つで四角形を形成）
	//  0-1  3
	//  |/  /|
	//  2  4-5
	// 1,3,5 のみ補間し、0,2,4は一つ前の1,3,5のデータと同じにする
	nnScaleVector( &ofst, &cross, size );
	nnAddVector( &pv[1].Pos, &( pNow->pPrev->pos ), &ofst ); // 右上
	nnSubtractVector( &pv[5].Pos, &( pNow->pPrev->pos ), &ofst ); // 右下

	pv[0] = prev[1];
	pv[4] = pv[2] = prev[5];

	pv[5].Col = AMD_FCOLTORGBA8888(color.r, color.g, color.b, color.a);
	pv[1].Col = pv[5].Col;

	// Texture
	pv[1].Tex.u = rate; pv[1].Tex.v = 0.0f;
	pv[5].Tex.u = rate; pv[5].Tex.v = 1.0f;

	pv[3] = pv[1];
}

// ================================================================
// _amTrailAddParts
/*!
	軌跡制御点追加処理

	@param	pNew : 軌跡パーツ
	@param	work : 軌跡ワークデータ
*/
// ================================================================
static void _amTrailAddParts(AMS_TRAIL_PARTS* pNew, AMS_TRAIL_PARAM* work)
{
	AMS_TRAIL_PARTSDATA* trData = &pTr->trailData[work->trailId];
	AMS_TRAIL_PARTS* pHead = &trData->trailHead;
	
	pNew->time = work->life;
	pNew->partsId = work->trailPartsId;
	work->trailPartsId++;
	work->trailPartsNum++;
	
	if ( work->trailPartsNum >= work->partsNum ) {
		work->trailPartsNum = work->partsNum;
		work->trailPartsId = (Sint16)pHead->pNext->partsId;
	}
	
	if ( work->trailPartsNum >= AMD_TRAIL_PARTSMAX ) {
		work->trailPartsNum = AMD_TRAIL_PARTSMAX;
		work->trailPartsId = (Sint16)pHead->pNext->partsId;
	}
}

// ================================================================
// _amTrailAddPosition
/*!
	軌跡の位置加算

	@param	pEffect : 軌跡エフェクト
	@param	offset  : オフセット座標
*/
// ================================================================
static void   _amTrailAddPosition(AMS_TRAIL_EFFECT* pEffect, NNS_VECTOR* offset)
{
	amAssert( pEffect );
	AMS_TRAIL_PARAM* work = (AMS_TRAIL_PARAM*)pEffect->Work;
	AMS_TRAIL_PARTSDATA* trData = &pTr->trailData[work->trailId];
	AMS_TRAIL_PARTS* pTail = &trData->trailTail;
	AMS_TRAIL_PARTS* pHead = &trData->trailHead;
	AMS_TRAIL_PARTS* pNow = pTail->pPrev;

	// 更新処理をされずに描画されている
	if (pTail->pPrev->pPrev == pHead) {
    	return;
	}
	amAssert(pNow->pPrev);

	if ( work->time <= 0.0f ) return;

	// 位置加算
	while ( pNow != pHead ) {
		nnAddVector(&pNow->pos, &pNow->pos, offset);
		pNow = pNow->pPrev;
	}
}

/*--- Spline Functions ---------------------------------------------------------*/
//## Spline Functions
// ================================================================
// _amTrailCalcSplinePos
/*!
	補間点計算

	@param	Pos   : 軌跡基準点
	@param	Dir   : 軌跡の方向
	@param  pNPP  :
	@param  pNP   :
	@param  pNow  :
	@param  pNext :
	@param  len   :
	@param  MaxComp   :
*/
// ================================================================
static Sint32	_amTrailCalcSplinePos(NNS_VECTOR Pos[AMD_TRAIL_COMP_FRAME2], NNS_VECTOR Dir[AMD_TRAIL_COMP_FRAME2],
									  AMS_TRAIL_PARTS* pNPP, AMS_TRAIL_PARTS* pNP, 
									  AMS_TRAIL_PARTS* pNow, AMS_TRAIL_PARTS* pNext,
									  float len, Sint32 MaxComp)
{
	AMTRS_FC_PARAM fcWk;
	fcWk.m_flag = 0;
	{
		if(pNPP != NULL){
			fcWk.m_x[0]	= pNPP->pos.x;
			fcWk.m_y[0]	= pNPP->pos.y;
			fcWk.m_z[0]	= pNPP->pos.z;
		}else{
			fcWk.m_flag |= AMTRE_STATE_TOP;
		}
		fcWk.m_x[1]	= pNP->pos.x;
		fcWk.m_y[1]	= pNP->pos.y;
		fcWk.m_z[1]	= pNP->pos.z;
		fcWk.m_x[2]	= pNow->pos.x;
		fcWk.m_y[2]	= pNow->pos.y;
		fcWk.m_z[2]	= pNow->pos.z;
		if(pNext != NULL){
			fcWk.m_x[3]	= pNext->pos.x;
			fcWk.m_y[3]	= pNext->pos.y;
			fcWk.m_z[3]	= pNext->pos.z;
		}else{
			fcWk.m_flag |= AMTRE_STATE_END;
		}
		fcWk.m_flag |= AMTRE_STATE_SSP_SET;

		if(pNPP != NULL){
			fcWk.m_Dx[0]	= pNPP->dir.x;
			fcWk.m_Dy[0]	= pNPP->dir.y;
			fcWk.m_Dz[0]	= pNPP->dir.z;
		}else{
			fcWk.m_flag |= AMTRE_STATE_TOP;
		}
		fcWk.m_Dx[1]	= pNP->dir.x;
		fcWk.m_Dy[1]	= pNP->dir.y;
		fcWk.m_Dz[1]	= pNP->dir.z;
		fcWk.m_Dx[2]	= pNow->dir.x;
		fcWk.m_Dy[2]	= pNow->dir.y;
		fcWk.m_Dz[2]	= pNow->dir.z;
		if(pNext != NULL){
			fcWk.m_Dx[3]	= pNext->dir.x;
			fcWk.m_Dy[3]	= pNext->dir.y;
			fcWk.m_Dz[3]	= pNext->dir.z;
		}else{
			fcWk.m_flag |= AMTRE_STATE_END;
		}
		fcWk.m_flag		|= AMTRE_STATE_SSP_SET;
	}

	return _amTrailCalcSplinePos(Pos, Dir, &fcWk, len, MaxComp);
}

// ================================================================
// _amTrailCalcSplinePos
/*!
	補間点計算

	@param	pos    : 補間位置配列
	@param	dir	   : 補間向き配列
	@param	FcWk   : 補間設定データ
	@param	len    : 区間の距離
	@param	MaxComp: 最大補間点数
*/
// ================================================================
static Sint32	_amTrailCalcSplinePos(NNS_VECTOR* pos, NNS_VECTOR* dir, AMTRS_FC_PARAM* FcWk,
									  float len, Sint32 MaxComp)
{
	Sint32 hokan = (Sint32)len;
	float rate;

	hokan = amClamp(hokan, 0, MaxComp); //リミットチェック
	{	
		Sint32 i = 0;
		_amTrailCalcSpline( FcWk, FcWk->m_x); // 補間曲線算出
		for(i = 0; i < hokan; i++){
			rate = (float)(i + 1) / (float)(hokan + 1);
			pos[i].x = _amTrailGetValue( FcWk, rate );
		}

		_amTrailCalcSpline( FcWk, FcWk->m_y); // 補間曲線算出
		for(i = 0; i < hokan; i++){
			rate = (float)(i + 1) / (float)(hokan + 1);
			pos[i].y = _amTrailGetValue( FcWk, rate );
		}

		_amTrailCalcSpline( FcWk, FcWk->m_z); // 補間曲線算出
		for(i = 0; i < hokan; i++){
			rate = (float)(i + 1) / (float)(hokan + 1);
			pos[i].z = _amTrailGetValue( FcWk, rate );
		}

		_amTrailCalcSpline( FcWk, FcWk->m_Dx); // 補間曲線算出
		for(i = 0; i < hokan; i++){
			rate = (float)(i + 1) / (float)(hokan + 1);
			dir[i].x = _amTrailGetValue( FcWk, rate );
		}

		_amTrailCalcSpline( FcWk, FcWk->m_Dy); // 補間曲線算出
		for(i = 0; i < hokan; i++){
			rate = (float)(i + 1) / (float)(hokan + 1);
			dir[i].y = _amTrailGetValue( FcWk, rate );
		}

		_amTrailCalcSpline( FcWk, FcWk->m_Dz); // 補間曲線算出
		for(i = 0; i < hokan; i++){
			rate = (float)(i + 1) / (float)(hokan + 1);
			dir[i].z = _amTrailGetValue( FcWk, rate );
		}
	}

	return hokan;
}

// ================================================================
// _amTrailCalcSpline
/*!
	補間曲線の算出

	@param	param     : パラメータ
	@param  P         : 補間計算に使用する座標値配列
*/
// ================================================================
static void		_amTrailCalcSpline( AMTRS_FC_PARAM* param, float* P )
{
	amAssert( param != NULL );
	amAssert( param->m_flag & AMTRE_STATE_SSP_SET );

	float v1 = 0, v2 = 0;
	switch(param->m_flag & AMTRE_STATE_TE_MASK){
		case AMTRE_STATE_END:
			v1 = (P[2] - P[0])/1.0f;
			v2 = (P[2] - P[1])/4.0f;
			break;
		case AMTRE_STATE_TOP:
			v1 = (P[2] - P[1])/4.0f;
			v2 = (P[3] - P[1])/1.0f;
			break;
		default:
			v1 = (P[2] - P[0])/2.0f;
			v2 = (P[3] - P[1])/2.0f;
			break;
	}
	param->m_CalcParam.x = 2*P[1] - 2*P[2] + v1 + v2;
	param->m_CalcParam.y = (-3)*P[1] + 3*P[2] - 2*v1 - v2;
	param->m_CalcParam.z = v1;
	param->m_CalcParam.w = P[1];

	// 計算済みフラグのセット
	param->m_flag |= AMTRE_STATE_SSP_CALC;
}

// ================================================================
// _amTrailGetValue
/*!
	補間曲線上の任意点のデータを取得

	@param	param     : パラメータ
	@param  t         : データを取得する点
*/
// ================================================================
static float	_amTrailGetValue( AMTRS_FC_PARAM* param, float t)
{
	amAssert(param != NULL);
	float fdest = 0;

	//計算済みかチェック
	if (!(param->m_flag & AMTRE_STATE_SSP_CALC)) return fdest;

	AMS_VECTOR *Calc = &param->m_CalcParam;
	fdest = t*t*t*Calc->x + t*t*Calc->y + t*Calc->z + Calc->w;

	return fdest;
}

/*--- TCB Functions ---------------------------------------------------------*/
//## TCB Functions


