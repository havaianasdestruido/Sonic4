// ================================================================
/*!
    Copyright(c) 2009 Dimps CORP. All Rights Reserved. 

	@file       amEffect.cpp
	@brief      エフェクト描画ライブラリ
	@author	    Syuichi Gotou
	@date	    Date: 2009/04/03 
 */
// ================================================================

//----- Include Files --------------------------------------------------
#include "alice.h"
#include "amEffect.h"

//----- Macros ---------------------------------------------------------

//----- Definitions ----------------------------------------------------
//----- Enum Definitions -----------------------------------------------
//----- Type Definitions -----------------------------------------------
//----- Local Declarations ---------------------------------------------
static Sint32			_amEffectFrustumCulling(AMS_VECTOR* pPos, AMS_FRUSTUM* pFrustum, AMS_AME_BOUNDING* pBounding);
static void				_amEffectFinalize(AMS_AME_ECB* ecb);

static AMS_AME_RUNTIME*	_amCreateRuntimeEmitter(AMS_AME_CREATE_PARAM* param);
static AMS_AME_RUNTIME*	_amCreateRuntimeParticle(AMS_AME_CREATE_PARAM* param);
static AMS_AME_RUNTIME*	_amCreateRuntimeGroup(AMS_AME_ECB* ecb, AMS_AME_NODE* node);
static void				_amCreateEmitter(AMS_AME_CREATE_PARAM* param);
static void				_amCreateParticle(AMS_AME_CREATE_PARAM* param);
static void				_amCreateSpawnParticle(AMS_AME_RUNTIME* runtime, AMS_AME_RUNTIME_WORK* work);

static AMS_AME_RUNTIME*	_amAllocRuntime();
static void				_amFreeRuntime(AMS_AME_RUNTIME* runtime);
static AMS_AME_RUNTIME_WORK*	_amAllocRuntimeWork();
static void				_amAddEntry(AMS_AME_ECB* ecb, AMS_AME_RUNTIME* runtime);
static void				_amDelEntry(AMS_AME_ECB* ecb, AMS_AME_ENTRY* entry);

static NNE_PRIM_ALPHABLEND	_amEffectSetDrawMode(AMS_AME_RUNTIME* runtime, AMS_PARAM_DRAW_PRIMITIVE* param,
												 Sint32 NodeBlend);

static NNF_DRAWOBJ		_amEffectSetMaterial(AMS_AME_RUNTIME* runtime, Sint32* blend, Sint32 NodeBlend);


// User Functions --------------------------------------------------------------
// omni
static void				_amInitOmni(AMS_AME_CREATE_PARAM* param);
static Sint32			_amUpdateOmni(AMS_AME_RUNTIME* runtime);
static void				_amDrawOmni(AMS_AME_RUNTIME* runtime);

// directioanl
static void				_amInitDirectional(AMS_AME_CREATE_PARAM* param);
static Sint32			_amUpdateDirectional(AMS_AME_RUNTIME* runtime);
static void				_amDrawDirectional(AMS_AME_RUNTIME* runtime);

// surface
static void				_amInitSurface(AMS_AME_CREATE_PARAM* param);
static Sint32			_amUpdateSurface(AMS_AME_RUNTIME* runtime);
static void				_amDrawSurface(AMS_AME_RUNTIME* runtime);

// circle
static void				_amInitCircle(AMS_AME_CREATE_PARAM* param);
static Sint32			_amUpdateCircle(AMS_AME_RUNTIME* runtime);
static void				_amDrawCircle(AMS_AME_RUNTIME* runtime);

// simple sprite
static void				_amInitSimpleSprite(AMS_AME_CREATE_PARAM* param);
static Sint32			_amUpdateSimpleSprite(AMS_AME_RUNTIME* runtime);
static void				_amDrawSimpleSprite(AMS_AME_RUNTIME* runtime);

// sprite
static void				_amInitSprite(AMS_AME_CREATE_PARAM* param);
static Sint32			_amUpdateSprite(AMS_AME_RUNTIME* runtime);
static void				_amDrawSprite(AMS_AME_RUNTIME* runtime);

// line
static void				_amInitLine(AMS_AME_CREATE_PARAM* param);
static Sint32			_amUpdateLine(AMS_AME_RUNTIME* runtime);
static void				_amDrawLine(AMS_AME_RUNTIME* runtime);

// plane
static void				_amInitPlane(AMS_AME_CREATE_PARAM* param);
static Sint32			_amUpdatePlane(AMS_AME_RUNTIME* runtime);
static void				_amDrawPlane(AMS_AME_RUNTIME* runtime);

// model
static void				_amInitModel(AMS_AME_CREATE_PARAM* param);
static Sint32			_amUpdateModel(AMS_AME_RUNTIME* runtime);
static void				_amDrawModel(AMS_AME_RUNTIME* runtime);

// field
static void				_amApplyGravity(AMS_AME_ECB* ecb, AMS_AME_NODE* node, AMS_AME_RUNTIME_WORK* work);
static void				_amApplyUniform(AMS_AME_ECB* ecb, AMS_AME_NODE* node, AMS_AME_RUNTIME_WORK* work);
static void				_amApplyRadial(AMS_AME_ECB* ecb, AMS_AME_NODE* node, AMS_AME_RUNTIME_WORK* work);
static void				_amApplyVortex(AMS_AME_ECB* ecb, AMS_AME_NODE* node, AMS_AME_RUNTIME_WORK* work);
static void				_amApplyDrag(AMS_AME_ECB* ecb, AMS_AME_NODE* node, AMS_AME_RUNTIME_WORK* work);
static void				_amApplyNoise(AMS_AME_ECB* ecb, AMS_AME_NODE* node, AMS_AME_RUNTIME_WORK* work);


//----- External Variables ---------------------------------------------
NNS_MATRIX				_am_ef_worldViewMtx;
NNS_VECTOR				_am_ef_camPos;

//----- Global Variables -----------------------------------------------
// エントリーコントロールブロック
static AMS_AME_ECB				_am_ecb_buf[AMD_AME_ECB_SIZE];
static AMS_AME_ECB*				_am_ecb_ref[AMD_AME_ECB_SIZE];

// エントリー
static AMS_AME_ENTRY			_am_entry_buf[AMD_AME_ENTRY_SIZE];
static AMS_AME_ENTRY*			_am_entry_ref[AMD_AME_ENTRY_SIZE];

// ランタイム
static AMS_AME_RUNTIME			_am_runtime_buf[AMD_AME_RUNTIME_SIZE];
static AMS_AME_RUNTIME*			_am_runtime_ref[AMD_AME_RUNTIME_SIZE];

// ランタイムワーク
static AMS_AME_RUNTIME_WORK*	_am_work_ref[AMD_AME_WORK_SIZE];
static AMS_AME_RUNTIME_WORK		_am_work_buf[AMD_AME_WORK_SIZE];

static Sint32					_am_enable_draw;	// エフェクト描画フラグ

static float					_am_unit_time;		// 単位フレーム
static float					_am_unit_frame;		// 単位時間

static AMS_AME_ECB				_am_ecb_head;		// エントリコントロールブロックのヘッド
static AMS_AME_ECB				_am_ecb_tail;		// エントリコントロールブロックのテール
static Sint32					_am_ecb_alloc;		// 
static Sint32					_am_ecb_free;		// 
static Sint32					_am_entry_alloc;	// 
static Sint32					_am_entry_free;		// 
static Sint32					_am_runtime_alloc;	// 
static Sint32					_am_runtime_free;	// 
static Sint32					_am_work_alloc;		// 確保カウント(ランタイムワーク)
static Sint32					_am_work_free;		// 開放カウント(ランタイムワーク)

static AMS_FRUSTUM				_am_view_frustum;

#if AMD_DEBUG
static Sint32					_am_ecb_num;		// 使用中のエントリコントロールブロック数
static Sint32					_am_entry_num;		// 使用中のエントリ数
static Sint32					_am_runtime_num;	// 使用中のランタイム数
static Sint32					_am_work_num;		// 使用中のランタイムワーク数

static Sint32					_am_ecb_peak;		// 
static Sint32					_am_entry_peak;		// 
static Sint32					_am_runtime_peak;	// 
static Sint32					_am_work_peak;		// 
#endif


// エミッタ関数群
static void* _am_emitter_func[16*4] = {
	(void*)_amInitOmni,			(void*)_amUpdateOmni,			(void*)_amDrawOmni,			NULL,	// omni
	(void*)_amInitDirectional,	(void*)_amUpdateDirectional,	(void*)_amDrawDirectional,	NULL,	// directional
	(void*)_amInitSurface,		(void*)_amUpdateSurface,		(void*)_amDrawSurface,		NULL,	// surface
	(void*)_amInitCircle,		(void*)_amUpdateCircle,			(void*)_amDrawCircle,		NULL,	// circle
};

// パーティクル関数群
static void* _am_particle_func[16*4] = {
	(void*)_amInitSimpleSprite,	(void*)_amUpdateSimpleSprite,	(void*)_amDrawSimpleSprite,	NULL,	// simple sprite
	(void*)_amInitSprite,		(void*)_amUpdateSprite,			(void*)_amDrawSprite,		NULL,	// sprite
	(void*)_amInitLine,			(void*)_amUpdateLine,			(void*)_amDrawLine,			NULL,	// line
	(void*)_amInitPlane,		(void*)_amUpdatePlane,			(void*)_amDrawPlane,		NULL,	// plane
	(void*)_amInitModel,		(void*)_amUpdateModel,			(void*)_amDrawModel,		NULL,	// model
};

// ユーザーエフェクト関数群
static void* _am_field_func[16] = {
	(void*)_amApplyGravity,		// 重力フィールドの適用
	(void*)_amApplyUniform,		// 均一フィールドの適用
	(void*)_amApplyRadial,			// 放射状フィールドの適用
	(void*)_amApplyVortex,			// 渦フィールドの適用
	(void*)_amApplyDrag,			// ドラッグフィールドの適用
	(void*)_amApplyNoise,			// ノイズフィールドの適用
};


//----- Local Variables ------------------------------------------------
//----- Inline Functions -----------------------------------------------
// amRandomConeVector
/*!
	円錐座標系のランダムベクトル値の取得
	
	@param pOut	[out]	出力用
	@param s	[in]	ラジアン
*/
inline void amEffectRandomConeVector(AMS_VECTOR *pOut, float s)
{
	float c, y, r;

	c = nnCos( NNM_RADtoA32( s ) );
	y = nnRandom() * (1.0f - c) + c;	// y = rand( c, 1.0 )
	r = sqrtf( 1.0f - y * y );

	amSinCos( (nnRandom()*AMD_MATH_2PI), &s, &c );	// angle = rand( 0.0, PI*2.0 )
	amVectorSet( pOut, r*c, y, r*s );
	amVectorUnit( pOut );
}

// amRandomConeVector
/*!
	円錐座標系のランダムベクトル値の取得
	
	@param pOut	[out]	出力用
	@param s	[in]	角度（Degree）
*/
inline void amEffectRandomConeVectorDeg(AMS_VECTOR *pOut, float s)
{
	float c, y, r;

	c = nnCos( NNM_DEGtoA32( s ) );
	y = nnRandom() * (1.0f - c) + c;	// y = rand( c, 1.0 )
	r = sqrtf( 1.0f - y * y );

	amSinCos( (nnRandom()*AMD_MATH_2PI), &s, &c );	// angle = rand( 0.0, PI*2.0 )
	amVectorSet( pOut, r*c, y, r*s );
	amVectorUnit( pOut );
}

// _amConnectLinkToTail
/*!
	ヘッドへのリンク
	
	@param param0	[out] 
	@param param1	[out] 
*/
inline void _amConnectLinkToHead(AMS_AME_LIST* head, AMS_AME_LIST* list)
{
	amAssert( head );
	amAssert( list );
	list->next = head->next;
	list->prev = head->next->prev;
	head->next->prev = list;
	head->next = list;
}

// _amConnectLinkToTail
/*!
	テールへのリンク
	
	@param param0	[out] 
	@param param1	[out] 
*/
inline void _amConnectLinkToTail(AMS_AME_LIST* tail, AMS_AME_LIST* list)
{
	amAssert( tail );
	amAssert( list );
	list->prev = tail->prev;
	list->next = tail->prev->next;
	tail->prev->next = list;
	tail->prev = list;
}

//----- Global Functions -----------------------------------------------

//! amEffectSystemInit
/*!
	エフェクトシステムの初期化
*/
void amEffectSystemInit()
{
	NNS_RGBA		diffuse = { 1.0f, 1.0f, 1.0f, 1.0f};
	NNS_RGB			ambient = { 1.0f, 1.0f, 1.0f};

	nnSetPrimitive3DMaterial(&diffuse, &ambient, 1.0f);

	_am_enable_draw	= 1;
	_am_unit_frame	= AMD_AME_UNIT_FRAME;
	_am_unit_time	= AMD_AME_UNIT_TIME;

	// エントリーコントロールブロック初期化
	{
		_am_ecb_alloc	= 0;
		_am_ecb_free	= 0;
#if AMD_DEBUG
		_am_ecb_num		= 0;
		_am_ecb_peak	= 0;
#endif
		amZeroMemory( &_am_ecb_head, sizeof(AMS_AME_ECB) );
		amZeroMemory( &_am_ecb_tail, sizeof(AMS_AME_ECB) );
		_am_ecb_head.next = &_am_ecb_tail;
		_am_ecb_tail.prev = &_am_ecb_head;

		amZeroMemory( _am_ecb_buf, sizeof(_am_ecb_buf) );
		for ( int i = 0; i < AMD_AME_ECB_SIZE; i++ )
		{
			_am_ecb_ref[i] = &( _am_ecb_buf[i] );
		}
	}

	// エントリーバッファ初期化
	{
		_am_entry_alloc	= 0;
		_am_entry_free	= 0;
#if AMD_DEBUG
		_am_entry_num	= 0;
		_am_entry_peak	= 0;
#endif
		amZeroMemory( _am_entry_buf, sizeof(_am_entry_buf) );
		for ( int i = 0; i < AMD_AME_ENTRY_SIZE; i++ )
		{
			_am_entry_ref[i] = &( _am_entry_buf[i] );
		}
	}

	// ランタイムバッファ初期化
	{
		_am_runtime_alloc	= 0;
		_am_runtime_free	= 0;
#if AMD_DEBUG
		_am_runtime_num		= 0;
		_am_runtime_peak	= 0;
#endif
		amZeroMemory( _am_runtime_buf, sizeof(_am_runtime_buf) );
		for ( int i = 0; i < AMD_AME_RUNTIME_SIZE; i++ )
		{
			_am_runtime_ref[i] = &( _am_runtime_buf[i] );
		}
	}

	// ランタイムワークバッファ初期化
	{
		_am_work_alloc	= 0;
		_am_work_free	= 0;
#if AMD_DEBUG
		_am_work_num	= 0;
		_am_work_peak	= 0;
#endif
		amZeroMemory( _am_work_buf, sizeof(_am_work_buf) );
		for ( int i = 0; i < AMD_AME_WORK_SIZE; i++ )
		{
			_am_work_ref[i] = &( _am_work_buf[i] );
		}
	}
}

//! エフェクトシステムのリセット
/*!
*/
void amEffectSystemReset()
{
	amEffectSystemInit();
}

//! サーバー関数
/*!
*/
void amEffectExecute()
{
	AMS_AME_ECB* ecb;

	for ( ecb = _am_ecb_head.next; ecb != &_am_ecb_tail; ecb = ecb->next )
	{

#if AMD_DEBUG
		ecb->reserved[0] = 0;
#endif
		if ( ecb->entry_num < 0 )
		{
			_amEffectFinalize( ecb );
		}
	}
}

// amEffectRegistCustomFunc
/*!
	カスタムパーティクルの登録
	
	@param classId	[in] クラスID(AME_AME_NODE_TYPE)
	@param pParam	[in] 関数登録を行ったカスタムパラメータのポインタ

	@note
		初期化・更新・描画の関数をセットしたものを登録
*/
void amEffectRegistCustomFunc(Sint32 classId, AMS_AME_CUSTOM_PARAM* pParam)
{
	Sint32 type, index;

	type = classId & AME_AME_SUPER_CLASS_ID_MASK;

	switch ( type ) {
	case AME_AME_NODE_TYPE_EMITTER:		// エミッタ
		index = (classId & AME_AME_CLASS_ID_MASK) << 2;
		_am_emitter_func[ index+0 ]		= (void*)pParam->pInitFunc;
		_am_emitter_func[ index+1 ]		= (void*)pParam->pUpdateFunc;
		_am_emitter_func[ index+2 ]		= (void*)pParam->pDrawFunc;
		break;
	case AME_AME_NODE_TYPE_PARTICLE:	// パーティクル
		index = (classId & AME_AME_CLASS_ID_MASK) << 2;
		_am_particle_func[ index+0 ]	= (void*)pParam->pInitFunc;
		_am_particle_func[ index+1 ]	= (void*)pParam->pUpdateFunc;
		_am_particle_func[ index+2 ]	= (void*)pParam->pDrawFunc;
		break;
	case AME_AME_NODE_TYPE_FIELD:		// フィールド
		index = classId & AME_AME_CLASS_ID_MASK;
		_am_field_func[ index ]			= (void*)pParam->pFiledFunc;
	}
}

// amEffectUnregistCustomFunc
/*!
	カスタムパーティクルの登録解除
	
	@param classId	クラスID

	@note
		各種セットした関数をリセット
*/
void amEffectUnregistCustomFunc(Sint32 classId)
{
	Sint32 type, index;

	type = classId & AME_AME_SUPER_CLASS_ID_MASK;

	switch ( type ) {
	case AME_AME_NODE_TYPE_EMITTER:		// エミッタ
		index = (classId & AME_AME_CLASS_ID_MASK) << 2;
		_am_emitter_func[ index+0 ]		= NULL;
		_am_emitter_func[ index+1 ]		= NULL;
		_am_emitter_func[ index+2 ]		= NULL;
		break;
	case AME_AME_NODE_TYPE_PARTICLE:	// パーティクル
		index = (classId & AME_AME_CLASS_ID_MASK) << 2;
		_am_particle_func[ index+0 ]	= NULL;
		_am_particle_func[ index+1 ]	= NULL;
		_am_particle_func[ index+2 ]	= NULL;
		break;
	case AME_AME_NODE_TYPE_FIELD:		// フィールド
		index = classId & AME_AME_CLASS_ID_MASK;
		_am_field_func[ index ]			= NULL;
	}
}

//! オブジェクト登録
/*
	モデルパーティクルで使用するNNオブジェクトを登録します

	@param ecb    [in] エントリーコントロールブロックへのポインタ
	@param object [in] NNオブジェクト管理用
*/
void amEffectSetObject(AMS_AME_ECB* ecb, NNS_OBJECT* object, Sint32 state)
{
	amAssert( ecb );
	amAssert( object );

	ecb->pObj = object;
	ecb->drawObjState = state;
}

//! ワールドビューマトリクスの設定
/*
	ワールドビューマトリクスの設定

	@param mtx    [in] ワールドビューマトリクス(amDrawGetWorldViewMatrix())
	@note  ビルボード処理のために必要。amEffectDrawの前に設定しておくこと
	       メインスレッドのみ有効
*/
void amEffectSetWorldViewMatrix(NNS_MATRIX* mtx)
{
	amThreadCheckSafe(0, "amEffectSetWorldViewMatrix");
	amAssert( mtx );
	nnCopyMatrix(&_am_ef_worldViewMtx, mtx);
}

//! ソート用カメラ位置の設定
/*
	ワールドビューマトリクスの設定

	@param pos    [in] カメラ位置
	@note  半透明ソート処理のために必要。amEffectDrawの前に設定しておくこと
	       メインスレッドのみ有効
*/
void amEffectSetCameraPos(NNS_VECTOR* pos)
{
	amThreadCheckSafe(0, "amEffectSetCameraPos");
	amAssert( pos );
	nnCopyVector(&_am_ef_camPos, pos);
}

//! エフェクト描画のON/OFF
/*!
	@param flag [in] 描画設定
*/
void amEffectEnableDraw(Sint32 flag)
{
	_am_enable_draw = flag;
}

//! 単位時間の設定
/*!
	システムの再生速度を設定します

	@param speed [in] 速度 (デフォルトは1.0)
	@param frame_rate [in] フレームレート (デフォルトは60)

	@note
	2倍速にする場合は speed=2.0f frame_rate=60 を指定します
*/
void amEffectSetUnitTime(float speed, Sint32 frame_rate)
{
	_am_unit_frame = speed;
	_am_unit_time = speed / (float)frame_rate;
}

//! 単位時間の取得
/*!
	@note _am_unit_frame を取得します
*/
float amEffectGetUnitFrame(void)
{
	return _am_unit_frame;
}

//! ノードの検索
/*!
	指定したIDにマッチするノードを検索します

	@param node	[in] 開始位置を表すノードへのポインタ
	@param node	[in] ファイルヘッダへのポインタ
	@param id	[in] 検索対象のノードID

	@retval NULL		存在しない
	@retval NULL以外	ノードへのポインタ
*/
AMS_AME_NODE* amEffectSearchNode(AMS_AME_NODE* node, Sint32 id)
{
	amAssert( node );

	AMS_AME_NODE* temp = NULL;

	if ( node->id == id ) return node;

	if ( node->child )
	{
		temp = amEffectSearchNode( node->child, id );
		if ( temp ) return temp;
	}
	if ( node->sibling )
	{
		temp = amEffectSearchNode( node->sibling, id );
	}

	return temp;
}

AMS_AME_NODE* amEffectSearchNode(AME_HEADER* header, Sint32 id)
{
	amAssert( header );
	amAssert( header->file_id[0] == AMD_CONVERTED_MARK );

	return amEffectSearchNode( header->node, id );
}

//! エフェクトの作成
/*!
	エフェクトを作成します

	@param node [in] ノードへのポインタ
	@param header [in] ファイルヘッダへのポインタ
	@param attribute [in] ユーザー属性
	@param priority [in] 優先順位

	@retval NULL 作成失敗
	@retval NULL以外 作成成功

	@note
	ノードをパラメータとして渡した場合はバウンディング情報がコピーされません
*/
AMS_AME_ECB* amEffectCreate(AMS_AME_NODE* node, Sint32 attribute/*=0*/, Sint32 priority/*=0*/)
{
	AMS_AME_ECB* new_ecb;

	amAssert( node );

	// エントリーコントロールブロック作成
	{
		AMS_AME_ECB* ecb;

#if AMD_DEBUG
		if ( _am_ecb_num >= AMD_AME_ECB_SIZE )	// ECBサイズオーバー
			return NULL;
		++_am_ecb_num;
		if ( _am_ecb_peak < _am_ecb_num ) _am_ecb_peak = _am_ecb_num;
#endif

		new_ecb = _am_ecb_ref[ _am_ecb_alloc ];
		_am_ecb_alloc++;
		if ( _am_ecb_alloc >= AMD_AME_ECB_SIZE ) _am_ecb_alloc = 0;

		amZeroMemory( new_ecb, sizeof(AMS_AME_ECB) );
		new_ecb->attribute		= attribute;
		new_ecb->priority		= priority;
		new_ecb->transparency	= 256;
		new_ecb->size_rate		= 1.0f;
		amVectorInit( &( new_ecb->translate ) );
#if AMD_AME_ROTATE_QUAT
		amQuatInit( &( new_ecb->rotate ) );
#else
		amVectorInit( &( new_ecb->rotate ) );
#endif

		for ( ecb = _am_ecb_head.next; ecb != &_am_ecb_tail && ecb->priority <= priority; ecb = ecb->next ) ;
		ecb->prev->next = new_ecb;
		new_ecb->prev = ecb->prev;
		ecb->prev = new_ecb;
		new_ecb->next = ecb;
	}

	{
		AMS_AME_RUNTIME* new_runtime;
		AMS_AME_CREATE_PARAM param;
		AMS_VECTOR vec;

		amVectorInit( &vec );
		param.ecb				= new_ecb;
		param.runtime			= NULL;
		param.node				= node;
		param.position			= &vec;
		param.velocity			= &vec;
		param.parent_position	= &vec;
		param.parent_velocity	= &vec;

		switch ( AMD_AME_NODE_TYPE( node ) & AME_AME_SUPER_CLASS_ID_MASK )
		{
		case AME_AME_NODE_TYPE_EMITTER:
			new_runtime = _amCreateRuntimeEmitter( &param );
			new_runtime->state |= AMD_AME_STATE_ROOT;
			amAssert( new_runtime );
			break;
		case AME_AME_NODE_TYPE_PARTICLE:
			new_runtime = _amCreateRuntimeParticle( &param );
			new_runtime->state |= AMD_AME_STATE_ROOT;
			amAssert( new_runtime );
			break;
#if AMD_DEBUG
		case AME_AME_NODE_TYPE_FIELD:
			amAssert( 0 );	// フィールドはランタイムを作れません
		default:
			amAssert( 0 );	// サポートしていないノード
#endif
		}
	}
	
	new_ecb->skip_update = 1;
	return new_ecb;
}

AMS_AME_ECB* amEffectCreate(AMS_AME_HEADER* header, Sint32 attribute/*=0*/, Sint32 priority/*=0*/)
{
	AMS_AME_ECB* new_ecb;

	amAssert( header );
	amAssert( header->file_id[0] == AMD_CONVERTED_MARK );	// ファイル未変換

	// エントリーコントロールブロック作成
	{
		AMS_AME_ECB* ecb;

#if AMD_DEBUG
		if ( _am_ecb_num >= AMD_AME_ECB_SIZE )	// ECBサイズオーバー
			return NULL;
		++_am_ecb_num;
		if ( _am_ecb_peak < _am_ecb_num ) _am_ecb_peak = _am_ecb_num;
#endif

		new_ecb = _am_ecb_ref[ _am_ecb_alloc ];
		_am_ecb_alloc++;
		if ( _am_ecb_alloc >= AMD_AME_ECB_SIZE ) _am_ecb_alloc = 0;

		amZeroMemory( new_ecb, sizeof(AMS_AME_ECB) );
		new_ecb->attribute		= attribute;
		new_ecb->priority		= priority;
		new_ecb->transparency	= 256;
		new_ecb->size_rate		= 1.0f;
		amVectorInit( &( new_ecb->translate ) );
#if AMD_AME_ROTATE_QUAT
		amQuatInit( &( new_ecb->rotate ) );
#else
		amVectorInit( &( new_ecb->rotate ) );
#endif
		memcpy( &( new_ecb->bounding ), &( header->bounding ), sizeof(AMS_AME_BOUNDING) );

		for ( ecb = _am_ecb_head.next; ecb != &_am_ecb_tail && ecb->priority <= priority; ecb = ecb->next ) ;
		ecb->prev->next = new_ecb;
		new_ecb->prev = ecb->prev;
		ecb->prev = new_ecb;
		new_ecb->next = ecb;
	}

	{
		AMS_AME_RUNTIME* new_runtime;
		AMS_AME_CREATE_PARAM param;
		AMS_VECTOR vec;

		AMS_AME_NODE* node = header->node;

		amVectorInit( &vec );

		while ( node )
		{
			param.ecb				= new_ecb;
			param.runtime			= NULL;
			param.node				= node;
			param.position			= &vec;
			param.velocity			= &vec;
			param.parent_position	= &vec;
			param.parent_velocity	= &vec;

			switch ( AMD_AME_NODE_TYPE( node ) & AME_AME_SUPER_CLASS_ID_MASK )
			{
			case AME_AME_NODE_TYPE_EMITTER:
				new_runtime = _amCreateRuntimeEmitter( &param );
				new_runtime->state |= AMD_AME_STATE_ROOT;
				amAssert( new_runtime );
				break;
			case AME_AME_NODE_TYPE_PARTICLE:
				new_runtime = _amCreateRuntimeParticle( &param );
				new_runtime->state |= AMD_AME_STATE_ROOT;
				amAssert( new_runtime );
				break;
#if AMD_DEBUG
			case AME_AME_NODE_TYPE_FIELD:
				amAssert( 0 );	// フィールドはランタイムを作れません
			default:
				amAssert( 0 );	// サポートしていないノード
#endif
			}

			node = node->sibling;
		}
	}

	new_ecb->skip_update = 1;
	return new_ecb;
}

//! エフェクトのグループ削除
/*!
	条件を満たすエフェクトを削除します

	@param attr [in] ユーザー属性
	@param flag [in] 評価フラグ
						AME_ATTR_INCLUSIVE - 一部条件
						AME_ATTR_EXCLUSIVE - 全条件

	@note
	この関数ではエントリーコントロールブロックの実体は開放されません
	実体の開放は amExecuteEffect() で行われます
*/
void amEffectDeleteGroup(Sint32 attr, Sint32 flag/*=AME_ATTR_EXCLUSIVE*/)
{
	AMS_AME_ECB* ecb;
	Sint32 grp, usr;

	grp = attr & AME_AME_ATTR_GROUP_MASK;
	usr = attr & AME_AME_ATTR_USER_MASK;
	if ( grp == 0 ) grp = AME_AME_ATTR_GROUP_ALL;

	switch ( flag )
	{
	// 一つでも満たす
	case AME_AME_ATTR_INCLUSIVE:
		for ( ecb = _am_ecb_head.next; ecb != &_am_ecb_tail; ecb = ecb->next )
		{
			if ( (ecb->attribute & grp) && (ecb->attribute & usr) != 0 )
			{
				amEffectDelete( ecb );
			}
		}
		break;

	// 全部満たす
	case AME_AME_ATTR_EXCLUSIVE:
		for ( ecb = _am_ecb_head.next; ecb != &_am_ecb_tail; ecb = ecb->next )
		{
//			if ( ((ecb->attribute & grp) & (ecb->attribute & usr)) == attr )
			if ( ((ecb->attribute & grp) | (ecb->attribute & usr)) == attr )
//			if ( ecb->attribute == attr )
			{
				amEffectDelete( ecb );
			}
		}
		break;
	}
}

//! エフェクトの終了
/*!
	エフェクトを終了します

	@param ecb [in] エントリーコントロールブロックへのポインタ

	@note
	寿命が無限に設定されているノードを強制的に終了します
*/
void amEffectKill(AMS_AME_ECB* ecb)
{
	AMS_AME_ENTRY* entry;

	amAssert( ecb );

	// 実行
	for ( entry = ecb->entry_head; entry != NULL; entry = entry->next )
	{
		AMS_AME_RUNTIME*	runtime = entry->runtime;
		AMS_AME_NODE*		node = runtime->node;

		if ( (AMD_AME_NODE_TYPE( node ) & AME_AME_SUPER_CLASS_ID_MASK) == AME_AME_NODE_TYPE_EMITTER )
		{
			if ( runtime->state & AMD_AME_STATE_KILL ) continue;

			// スポーンランタイムに削除許可フラグを設定
			if ( runtime->spawn_runtime )
			{
				runtime->spawn_runtime->state |= AMD_AME_STATE_ACCEPT_KILL;
			}

			runtime->state |= AMD_AME_STATE_KILL;

			// 子リストに削除許可フラグをセット
			{
				AMS_AME_RUNTIME* child = (AMS_AME_RUNTIME*)runtime->child_head.next;
				AMS_AME_RUNTIME* tail = (AMS_AME_RUNTIME*)&( runtime->child_tail );

				for ( ; child != tail; child = child->next )
				{
					child->state |= AMD_AME_STATE_ACCEPT_KILL;
				}
			}

			// 親ランタイムが存在する場合は、親ランタイムの子リストから削除
			if ( runtime->parent_runtime )
			{
				amEffectDisconnectLink( (AMS_AME_LIST*)runtime );
				runtime->parent_runtime->work_num--;
			}
		}
	}
}

//! エフェクトのグループ終了
/*!
	エフェクトを終了します

	@param ecb [in] エントリーコントロールブロックへのポインタ

	@note
	寿命が無限に設定されているノードを強制的に終了します
*/
void amEffectKillGroup(Sint32 attr, Sint32 flag/*=AME_ATTR_EXCLUSIVE*/)
{
	AMS_AME_ECB* ecb;
	Sint32 grp, usr;

	grp = attr & AME_AME_ATTR_GROUP_MASK;
	usr = attr & AME_AME_ATTR_USER_MASK;
	if ( grp == 0 ) grp = AME_AME_ATTR_GROUP_ALL;

	switch ( flag )
	{
	// 一つでも満たす
	case AME_AME_ATTR_INCLUSIVE:
		for ( ecb = _am_ecb_head.next; ecb != &_am_ecb_tail; ecb = ecb->next )
		{
			if ( (ecb->attribute & grp) && (ecb->attribute & usr) != 0 )
			{
				amEffectKill( ecb );
			}
		}
		break;

	// 全部満たす
	case AME_AME_ATTR_EXCLUSIVE:
		for ( ecb = _am_ecb_head.next; ecb != &_am_ecb_tail; ecb = ecb->next )
		{
			if ( (ecb->attribute & grp) && (ecb->attribute & usr) == usr )
			{
				amEffectKill( ecb );
			}
		}
		break;
	}
}

//! エフェクトの更新
/*!
	エフェクトの更新を行います

	@param ecb [in] エントリーコントロールブロックへのポインタ
*/
void amEffectUpdate(AMS_AME_ECB* ecb)
{
	AMS_AME_ENTRY* entry;

	amAssert( ecb );

	if ( ecb->entry_num <= 0 ) return;
	
	//Createしたてはすでに１コマ目なので、Updateをスキップ 
	if ( ecb->skip_update )
	{
		ecb->skip_update = 0;
	}

#if AMD_DEBUG && !AMD_AME_MULTI_UPDATE
	// 多重更新防止
	amAssert( ecb->reserved[0] == 0 );
	ecb->reserved[0]++;
#endif

	// 実行
	for ( entry = ecb->entry_head; entry != NULL; entry = entry->next )
	{
		AMS_AME_RUNTIME*	runtime = entry->runtime;
		AMS_AME_NODE*		node = runtime->node;
		Sint32				type = AMD_AME_NODE_TYPE( node );

		// ワークが無くなったら削除
		if ( (runtime->state & AMD_AME_STATE_ACCEPT_KILL) && (runtime->work_num + runtime->active_num) == 0 )
		{
			// スポーンランタイムに削除許可フラグを設定
			if ( runtime->spawn_runtime )
			{
				runtime->spawn_runtime->state |= AMD_AME_STATE_ACCEPT_KILL;
			}

			runtime->state |= AMD_AME_STATE_KILL;
			continue;
		}

		switch ( type & AME_AME_SUPER_CLASS_ID_MASK )
		{
		// エミッタ
		case AME_AME_NODE_TYPE_EMITTER:
			if ( runtime->work )
			{
				AmeUpdateFunc	update_func;
				AmeFieldFunc	field_func;

				update_func = (AmeUpdateFunc)_am_emitter_func[ ((type & AME_AME_CLASS_ID_MASK) << 2) + 1 ];
				amAssert( update_func );
				if ( runtime->work )
				{
					if ( update_func( runtime ) )
					{
						runtime->state |= AMD_AME_STATE_KILL;

						// 子リストに削除許可フラグをセット
						{
							AMS_AME_RUNTIME* child = (AMS_AME_RUNTIME*)runtime->child_head.next;
							AMS_AME_RUNTIME* tail = (AMS_AME_RUNTIME*)&( runtime->child_tail );

							for ( ; child != tail; child = child->next )
							{
								child->state |= AMD_AME_STATE_ACCEPT_KILL;
							}
						}

						// 親ランタイムが存在する場合は、親ランタイムの子リストから削除
						if ( runtime->parent_runtime )
						{
							amEffectDisconnectLink( (AMS_AME_LIST*)runtime );
							runtime->parent_runtime->work_num--;
						}

						continue;
					}

					// フィールドの適応
					{
						AMS_AME_NODE* child;

						for ( child = node->child; child != NULL; child = child->sibling )
						{
							if ( !AMD_AME_IS_FIELD( child ) ) continue;

							field_func = (AmeFieldFunc)_am_field_func[ AMD_AME_NODE_TYPE( child ) & AME_AME_CLASS_ID_MASK ];
							amAssert( field_func );

							field_func( runtime->ecb, child, runtime->work );
						}
					}
				}
			}
			break;

		// パーティクル
		case AME_AME_NODE_TYPE_PARTICLE:
			{
				AmeUpdateFunc	update_func;
				AmeFieldFunc	field_func;

				if ( runtime->work_num )
				{
					AMS_AME_RUNTIME_WORK* loop = (AMS_AME_RUNTIME_WORK*)runtime->work_head.next;
					AMS_AME_RUNTIME_WORK* tail = (AMS_AME_RUNTIME_WORK*)&( runtime->work_tail );

					while ( loop != tail )
					{
						AMS_AME_RUNTIME_WORK* work = loop;
						loop = (AMS_AME_RUNTIME_WORK*)loop->next;

						work->time += _am_unit_frame;

						// start time
						if ( work->time > 0.0f )
						{
							work->time -= _am_unit_frame;

							amEffectDisconnectLink( (AMS_AME_LIST*)work );
							_amConnectLinkToTail( &( runtime->active_tail ), (AMS_AME_LIST*)work );

							runtime->work_num--;
							runtime->active_num++;
						}
					}
				}

				update_func = (AmeUpdateFunc)_am_particle_func[ ((type & AME_AME_CLASS_ID_MASK) << 2) + 1 ];
				amAssert( update_func );
				update_func( runtime );

				// フィールドの適応
				{
					AMS_AME_RUNTIME_WORK* work = (AMS_AME_RUNTIME_WORK*)runtime->active_head.next;
					AMS_AME_RUNTIME_WORK* tail = (AMS_AME_RUNTIME_WORK*)&( runtime->active_tail );
					AMS_AME_NODE* child;

					for ( ; work != tail; work = work->next )
					{
						for ( child = node->child; child != NULL; child = child->sibling )
						{
							if ( !AMD_AME_IS_FIELD( child ) ) continue;

							field_func = (AmeFieldFunc)_am_field_func[ AMD_AME_NODE_TYPE( child ) & AME_AME_CLASS_ID_MASK ];
							amAssert( field_func );

							field_func( runtime->ecb, child, work );
						}
					}
				}
			}
			break;

#if AMD_DEBUG
		default:
			amAssert(0);
#endif
		}
	}

	// 削除
	for ( entry = ecb->entry_head; entry != NULL; entry = entry->next )
	{
		AMS_AME_RUNTIME* runtime = entry->runtime;

		if ( runtime->state & AMD_AME_STATE_KILL )
		{
			// ランタイムを開放
			_amFreeRuntime( entry->runtime );

			// エントリーから削除
			_amDelEntry( ecb, entry );
		}
	}

	// エントリーがなくなったら削除
	if ( ecb->entry_num == 0 )
	{
		amEffectDelete( ecb );
	}
}

//! エフェクトのグループ更新
/*!
	条件を満たすエフェクトの更新を行います

	@param attr [in] ユーザー属性
	@param flag [in] 評価フラグ
						AME_ATTR_INCLUSIVE - 一部条件
						AME_ATTR_EXCLUSIVE - 全条件
*/
void amEffectUpdateGroup(Sint32 attr, Sint32 flag/*=AME_ATTR_EXCLUSIVE*/)
{
	AMS_AME_ECB* ecb;
	Sint32 grp, usr;

	grp = attr & AME_AME_ATTR_GROUP_MASK;
	usr = attr & AME_AME_ATTR_USER_MASK;
	if ( grp == 0 ) grp = AME_AME_ATTR_GROUP_ALL;

	switch ( flag )
	{
	// 一つでも満たす
	case AME_AME_ATTR_INCLUSIVE:
		for ( ecb = _am_ecb_head.next; ecb != &_am_ecb_tail; ecb = ecb->next )
		{
			if ( (ecb->attribute & grp) && (ecb->attribute & usr) != 0 )
			{
				amEffectUpdate( ecb );
			}
		}
		break;

	// 全部満たす
	case AME_AME_ATTR_EXCLUSIVE:
		for ( ecb = _am_ecb_head.next; ecb != &_am_ecb_tail; ecb = ecb->next )
		{
			if ( ((ecb->attribute & grp) | (ecb->attribute & usr)) == attr )
			{
				amEffectUpdate( ecb );
			}
		}
		break;
	}
}

//! エフェクトの描画
/*!
	エフェクトの描画を行います

	@param ecb [in] エントリーコントロールブロックへのポインタ
*/
// ================================================================
/*!
	エフェクトの描画

	@param ecb  [in] エントリーコントロールブロックへのポインタ
    @param texlist  [in] ＮＮテクスチャリスト

	@note 半透明ソートがうまくいかない場合
	不透明エフェクトと半透明エフェクトをデータレベルで別にしておき、
	不透明エフェクトを描画した後に半透明エフェクトを描画すること
	また、半透明エフェクト同士の描き順によっても見た目が変化するため
	エフェクト描画タスクの優先度には注意すること。
*/
// ================================================================
void amEffectDraw(AMS_AME_ECB* ecb, NNS_TEXLIST* texlist, Uint32 state)
{
	AMS_AME_ENTRY* entry;

	amAssert( ecb );
	ecb->drawState = state;

	if ( !_am_enable_draw ) return;
	if ( ecb->entry_num <= 0 ) return;

	AMS_AME_BOUNDING* pBounding = &( ecb->bounding );

	// view frustum culling
	if ( pBounding->radius > 0.0f )
	{
		AMS_VECTOR vPos;
		if ( !_amEffectFrustumCulling( &vPos, &_am_view_frustum, pBounding ) )
			return;
	}

	for ( entry = ecb->entry_head; entry != NULL; entry = entry->next )
	{
		AMS_AME_RUNTIME*	runtime = entry->runtime;
		AmeDrawFunc			func;

		runtime->texlist = texlist;

		switch ( AMD_AME_SUPER_CLASS_ID( runtime->node ) )
		{
		// パーティクル
		case AME_AME_NODE_TYPE_PARTICLE:
			if ( runtime->active_num )
			{
				func = (AmeDrawFunc)_am_particle_func[ (AMD_AME_CLASS_ID( runtime->node ) << 2) + 2 ];
				amAssert( func );
				func( runtime );
			}
			break;

#if AMD_DEBUG
		// エミッタ
		case AME_AME_NODE_TYPE_EMITTER:
			if ( runtime->work )
			{
				func = (AmeDrawFunc)_am_emitter_func[ (AMD_AME_CLASS_ID( runtime->node ) << 2) + 2 ];
				amAssert( func );
				func( runtime );
			}
			break;

		default:
			amAssert(0);
#endif
		}
	}
}

//! エフェクトのグループ描画
/*!
	条件を満たすエフェクトの描画を行います

	@param attr [in] ユーザー属性
	@param flag [in] 評価フラグ
						AME_ATTR_INCLUSIVE - 一部条件
						AME_ATTR_EXCLUSIVE - 全条件
*/
void amEffectDrawGroup(NNS_TEXLIST* texlist, Sint32 attr, Uint32 state, Sint32 flag/*=AME_ATTR_EXCLUSIVE*/)
{
	AMS_AME_ECB* ecb;
	Sint32 grp, usr;

	if ( !_am_enable_draw ) return;

	grp = attr & AME_AME_ATTR_GROUP_MASK;
	usr = attr & AME_AME_ATTR_USER_MASK;
	if ( grp == 0 ) grp = AME_AME_ATTR_GROUP_ALL;

	switch ( flag )
	{
	// 一つでも満たす
	case AME_AME_ATTR_INCLUSIVE:
		for ( ecb = _am_ecb_head.next; ecb != &_am_ecb_tail; ecb = ecb->next )
		{
			if ( (ecb->attribute & grp) && (ecb->attribute & usr) != 0 )
			{
				amEffectDraw( ecb, texlist, state );
			}
		}
		break;

	// 全部満たす
	case AME_AME_ATTR_EXCLUSIVE:
		for ( ecb = _am_ecb_head.next; ecb != &_am_ecb_tail; ecb = ecb->next )
		{
			if ( ((ecb->attribute & grp) | (ecb->attribute & usr)) == attr )
			{
				amEffectDraw( ecb, texlist, state );
			}
		}
		break;
	}
}

//! エフェクトの平行移動の設定
/*!
	移動可能なノードのローカル平行移動成分を設定します

	@param ecb [io] エントリーコントロールブロックへのポインタ
	@param translate [in] 平行移動成分
	@note amEffectUpdate よりも前に設定しておくこと
*/
void amEffectSetTranslate(AMS_AME_ECB* ecb, AMS_VECTOR* translate)
{
	amAssert( ecb );
	amAssert( translate );

	amVectorCopy( &( ecb->translate ), translate );

	AMS_AME_ENTRY* entry;
	for ( entry = ecb->entry_head; entry != NULL; entry = entry->next )
	{
		AMS_AME_RUNTIME* runtime = entry->runtime;

		if ( (runtime->state & AMD_AME_STATE_ROOT) == 0 ) continue;
		if ( runtime->node->flag & AMD_AME_FLAG_NODE_IGNORE_TRANSLATE ) continue;

		if ( runtime->work )
		{
			amVectorAdd( &( runtime->work->position ),
				&( ((AMS_AME_NODE_OMNI*)runtime->node)->translate ), translate );
		}

		if ( runtime->work_num + runtime->active_num )
		{
			AMS_AME_RUNTIME_WORK *work, *tail;

			work = (AMS_AME_RUNTIME_WORK*)runtime->work_head.next;
			tail = (AMS_AME_RUNTIME_WORK*)&( runtime->work_tail );
			for ( ; work != tail; work = (AMS_AME_RUNTIME_WORK*)work->next )
			{
				amVectorAdd( &( work->position ),
					&( ((AMS_AME_NODE_OMNI*)runtime->node)->translate ), translate );
			}

			work = (AMS_AME_RUNTIME_WORK*)runtime->active_head.next;
			tail = (AMS_AME_RUNTIME_WORK*)&( runtime->active_tail );
			for ( ; work != tail; work = (AMS_AME_RUNTIME_WORK*)work->next )
			{
				amVectorAdd( &( work->position ),
					&( ((AMS_AME_NODE_OMNI*)runtime->node)->translate ), translate );
			}
		}
	}
}

//! エフェクトの平行移動の設定
/*!
	移動可能なノードのローカル平行移動成分を加算します

	@param ecb [io] エントリーコントロールブロックへのポインタ
	@param translate [in] 平行移動成分
	@note amEffectUpdate よりも前に設定しておくこと
*/
void amEffectTranslate(AMS_AME_ECB* ecb, AMS_VECTOR* translate)
{
	amAssert( ecb );
	amAssert( translate );

	amVectorAdd( &( ecb->translate ), translate );

	AMS_AME_ENTRY* entry;
	for ( entry = ecb->entry_head; entry != NULL; entry = entry->next )
	{
		AMS_AME_RUNTIME* runtime = entry->runtime;

		if ( (runtime->state & AMD_AME_STATE_ROOT) == 0 ) continue;
		if ( runtime->node->flag & AMD_AME_FLAG_NODE_IGNORE_TRANSLATE ) continue;

		if ( runtime->work )
		{
			amVectorAdd( &( runtime->work->position ), translate );
		}

		if ( runtime->work_num + runtime->active_num )
		{
			AMS_AME_RUNTIME_WORK *work, *tail;

			work = (AMS_AME_RUNTIME_WORK*)runtime->work_head.next;
			tail = (AMS_AME_RUNTIME_WORK*)&( runtime->work_tail );
			for ( ; work != tail; work = (AMS_AME_RUNTIME_WORK*)work->next )
			{
				amVectorAdd( &( work->position ), translate );
			}

			work = (AMS_AME_RUNTIME_WORK*)runtime->active_head.next;
			tail = (AMS_AME_RUNTIME_WORK*)&( runtime->active_tail );
			for ( ; work != tail; work = (AMS_AME_RUNTIME_WORK*)work->next )
			{
				amVectorAdd( &( work->position ), translate );
			}
		}
	}
}

//! エフェクトの回転の設定
/*!
	回転可能なノードのローカル回転成分を設定します

	@param ecb [io] エントリーコントロールブロックへのポインタ
	@param x [in] 回転成分(X軸)
	@param y [in] 回転成分(Y軸)
	@param z [in] 回転成分(Z軸)
*/
void amEffectSetRotate(AMS_AME_ECB* ecb, Angle32 x, Angle32 y, Angle32 z)
{
	amAssert( ecb );

	AMS_QUAT q;
	amQuatEulerToQuatXYZ( &q, x, y, z );
	amEffectSetRotate( ecb, &q );
}

//! エフェクトの回転の設定
/*!
	@note プレーンやモデルなどを含んだエフェクトにはこの関数を使わず、
	マトリクス演算で回転させることを推奨します。
*/
void amEffectSetRotate(AMS_AME_ECB* ecb, AMS_QUAT* q, Sint32 offset)
{
	amAssert( ecb );

	amVectorCopy( (AMS_VECTOR*)&( ecb->rotate ), (AMS_VECTOR*)q );

	AMS_AME_ENTRY* entry;
	for ( entry = ecb->entry_head; entry != NULL; entry = entry->next )
	{
		AMS_AME_RUNTIME* runtime = entry->runtime;

		if ( (runtime->state & AMD_AME_STATE_ROOT) == 0 ) continue;
		if ( runtime->node->flag & AMD_AME_FLAG_NODE_IGNORE_ROTATE ) continue;

		if ( runtime->work )
		{
			if ( offset )
			{
				amQuatMulti( &( runtime->work->rotate ), &( ((AMS_AME_NODE_OMNI*)runtime->node)->rotate ), q );
			}
			else
			{
				amVectorCopy( (AMS_VECTOR*)&( runtime->work->rotate ), (AMS_VECTOR*)q );
			}
		}

		if ( runtime->work_num + runtime->active_num )
		{
			AMS_AME_RUNTIME_WORK *work, *tail;

			work = (AMS_AME_RUNTIME_WORK*)runtime->work_head.next;
			tail = (AMS_AME_RUNTIME_WORK*)&( runtime->work_tail );
			for ( ; work != tail; work = (AMS_AME_RUNTIME_WORK*)work->next )
			{
				if ( offset )
				{
					amQuatMulti( &( work->rotate ), &( ((AMS_AME_NODE_OMNI*)runtime->node)->rotate ), q );
				}
				else
				{
					amVectorCopy( (AMS_VECTOR*)&( work->rotate ), (AMS_VECTOR*)q );
				}
			}

			work = (AMS_AME_RUNTIME_WORK*)runtime->active_head.next;
			tail = (AMS_AME_RUNTIME_WORK*)&( runtime->active_tail );
			for ( ; work != tail; work = (AMS_AME_RUNTIME_WORK*)work->next )
			{
				if ( offset )
				{
					amQuatMulti( &( work->rotate ),
						&( ((AMS_AME_NODE_OMNI*)runtime->node)->rotate ), q );
				}
				else
				{
					amVectorCopy( (AMS_VECTOR*)&( work->rotate ), (AMS_VECTOR*)q );
				}
			}

		}
	}
}

//! エフェクトの回転の設定
/*!
	回転可能なノードのローカル回転成分を乗算します

	@param ecb [io] エントリーコントロールブロックへのポインタ
	@param x [in] 回転成分(X軸)
	@param y [in] 回転成分(Y軸)
	@param z [in] 回転成分(Z軸)
*/
void amEffectRotate(AMS_AME_ECB* ecb, Angle32 x, Angle32 y, Angle32 z)
{
	amAssert( ecb );

	AMS_QUAT q;
	amQuatEulerToQuatXYZ( &q, x, y, z );
	amEffectRotate( ecb, &q );
}

void amEffectRotate(AMS_AME_ECB* ecb, AMS_QUAT* q)
{
	amAssert( ecb );

	amQuatMulti( &( ecb->rotate ),
		&( ecb->rotate ), q );

	AMS_AME_ENTRY* entry;
	for ( entry = ecb->entry_head; entry != NULL; entry = entry->next )
	{
		AMS_AME_RUNTIME* runtime = entry->runtime;

		if ( (runtime->state & AMD_AME_STATE_ROOT) == 0 ) continue;
		if ( runtime->node->flag & AMD_AME_FLAG_NODE_IGNORE_ROTATE ) continue;

		if ( runtime->work )
		{
			amQuatMulti( &( runtime->work->rotate ),
				&( runtime->work->rotate ), q );
		}

		if ( runtime->work_num + runtime->active_num )
		{
			AMS_AME_RUNTIME_WORK *work, *tail;

			work = (AMS_AME_RUNTIME_WORK*)runtime->work_head.next;
			tail = (AMS_AME_RUNTIME_WORK*)&( runtime->work_tail );
			for ( ; work != tail; work = (AMS_AME_RUNTIME_WORK*)work->next )
			{
				if ( runtime->work )
				{
					amQuatMulti( &( work->rotate ),
						&( runtime->work->rotate ), q );
				}
			}

			work = (AMS_AME_RUNTIME_WORK*)runtime->active_head.next;
			tail = (AMS_AME_RUNTIME_WORK*)&( runtime->active_tail );
			for ( ; work != tail; work = (AMS_AME_RUNTIME_WORK*)work->next )
			{
				if ( runtime->work )
				{
					amQuatMulti( &( work->rotate ),
						&( runtime->work->rotate ), q );
				}
			}
		}
	}
}

//! パーティクルの作成
/*!
	@note
	カスタムパーティクル用
*/
void amEffectCreateParticle(AMS_AME_CREATE_PARAM* param)
{
	_amCreateParticle( param );
}


//--- Local functions ------------------------------------------------------

//{### ローカル関数

//! ビューフラスタムカリング
/*!
	ビューフラスタムカリングを行います。

	@param	pPos		[out]	演算後のバウンディング中心座標 (ビュー座標系)
	@param	pFrustum	[in]	フラスタム情報
	@param	pBounding	[in]	バウンディング情報

	@retval	1	視錐台内
	@retval	0	視錐台外
	@retval	-1	錐台平面と交差
*/
Sint32 _amEffectFrustumCulling(AMS_VECTOR* pPos, AMS_FRUSTUM* pFrustum, AMS_AME_BOUNDING* pBounding)
{
	UNREFERENCED_PARAMETER(pPos);
	UNREFERENCED_PARAMETER(pFrustum);
	UNREFERENCED_PARAMETER(pBounding);
#if (0)
	// transform world -> view
	amMatrixCalcPoint( pPos, &pBounding->center );
	amMatrixPush( amDrawGetWorldViewMatrix() );
	amMatrixCalcPoint( pPos, pPos );
	amMatrixPop();

	float diffNear = -pPos->z - pFrustum->nearClip + pBounding->radius;
	if ( diffNear < 0.0f ) return 0;

	float diffFar = -pPos->z - pFrustum->farClip - pBounding->radius;
	if ( diffFar > 0.0f ) return 0;

	// N dot (A - P) : P = 0
	float topFlag		= pFrustum->tbNormalY * pPos->y + pFrustum->tbNormalZ * pPos->z;
	float bottomFlag	= pFrustum->tbNormalZ * pPos->z - pFrustum->tbNormalY * pPos->y;
	float leftFlag		= pFrustum->lrNormalX * pPos->x + pFrustum->lrNormalZ * pPos->z;
	float rightFlag		= pFrustum->lrNormalZ * pPos->z - pFrustum->lrNormalX * pPos->x;
	float topOver		= topFlag * topFlag - pBounding->radius2;
	float bottomOver	= bottomFlag * bottomFlag - pBounding->radius2;
	float leftOver		= leftFlag * leftFlag - pBounding->radius2;
	float rightOver		= rightFlag * rightFlag - pBounding->radius2;

	*((int*)&topFlag)		&= ~*((int*)&topOver);
	*((int*)&bottomFlag)	&= ~*((int*)&bottomOver);
	*((int*)&leftFlag)		&= ~*((int*)&leftOver);
	*((int*)&rightFlag)		&= ~*((int*)&rightOver);

	Sint32	chkFlag = *((int*)&topFlag) | *((int*)&bottomFlag) | *((int*)&leftFlag) | *((int*)&rightFlag);
	if ( chkFlag < 0 ) return 0;
	
#if (0)
	// フラスタムとの交差判定はしない
	int overFlag =
		*((int*)&topOver) | *((int*)&bottomOver) |
		*((int*)&leftOver) | *((int*)&rightOver);
	if ( overFlag < 0 ) return -1;

	float checkRadius = pBounding->radius + pBounding->radius;
	if ( diffNear < checkRadius || -diffFar < checkRadius ) return -1;
#endif

#endif

	return 1;
}


/*!
	エントリーコントロールブロックの実削除
	
	@param ecb	[out] エントリーコントロールブロック
*/
void _amEffectFinalize(AMS_AME_ECB* ecb)
{
	AMS_AME_ENTRY* entry;

	amAssert( ecb );

	for ( entry = ecb->entry_head; entry != NULL; entry = entry->next )
	{
		AMS_AME_RUNTIME*	runtime = entry->runtime;
		AMS_AME_NODE*		node = runtime->node;

		// 
		if ( (AMD_AME_NODE_TYPE( node ) & AME_AME_SUPER_CLASS_ID_MASK) == AME_AME_NODE_TYPE_EMITTER )
		{
			// 親ランタイムが存在する場合は、親ランタイムの子リストから削除
			if ( runtime->parent_runtime )
			{
				amEffectDisconnectLink( (AMS_AME_LIST*)runtime );
				runtime->parent_runtime->work_num--;
			}
		}

		// ランタイムを開放
		_amFreeRuntime( entry->runtime );

		// エントリーリストから削除
		_amDelEntry( ecb, entry );
	}

	// エントリーコントロールブロック削除
	{
		ecb->prev->next = ecb->next;
		ecb->next->prev = ecb->prev;

#if AMD_DEBUG
		--_am_ecb_num;
		amAssert( _am_ecb_num >= 0 );
#endif

		_am_ecb_ref[ _am_ecb_free ] = ecb;
		_am_ecb_free++;
		if ( _am_ecb_free >= AMD_AME_ECB_SIZE ) _am_ecb_free = 0;
	}
}

// エミッタランタイムの作成
AMS_AME_RUNTIME* _amCreateRuntimeEmitter(AMS_AME_CREATE_PARAM* param)
{
	AMS_AME_RUNTIME* new_runtime;

	// ランタイム確保
	new_runtime = _amAllocRuntime();
	amAssert( new_runtime );

	// ランタイム初期化
	new_runtime->ecb				= param->ecb;
	new_runtime->node				= param->node;
	new_runtime->child_head.next	= &( new_runtime->child_tail );
	new_runtime->child_tail.prev	= &( new_runtime->child_head );
	new_runtime->work_head.next		= &( new_runtime->work_tail );
	new_runtime->work_tail.prev		= &( new_runtime->work_head );
	new_runtime->active_head.next	= &( new_runtime->active_tail );
	new_runtime->active_tail.prev	= &( new_runtime->active_head );

	// エントリーリストに登録
	_amAddEntry( param->ecb, new_runtime );

	// ランタイムワーク確保
	new_runtime->work = _amAllocRuntimeWork();
	amAssert( new_runtime->work );

	// ランタイムワーク初期化
	{
		AmeInitFunc func;

		param->work = new_runtime->work;

		func = (AmeInitFunc)_am_emitter_func[ (AMD_AME_NODE_TYPE( param->node ) & AME_AME_CLASS_ID_MASK) << 2 ];
		amAssert( func );

		func( param );
	}

	// 子ノードを探索
	{
		AMS_AME_NODE* child = param->node->child;
		AMS_AME_RUNTIME* runtime;

		while ( child )
		{
			if ( !AMD_AME_IS_FIELD( child ) )
			{
				runtime = _amCreateRuntimeGroup( param->ecb, child );
				amAssert( runtime );
				_amConnectLinkToTail( (AMS_AME_LIST*)&( new_runtime->child_tail ), (AMS_AME_LIST*)runtime );
				new_runtime->child_num++;
			}

			child = child->sibling;
		}
	}

	return new_runtime;
}

// パーティクルランタイムの作成
AMS_AME_RUNTIME* _amCreateRuntimeParticle(AMS_AME_CREATE_PARAM* param)
{
	AMS_AME_RUNTIME* new_runtime;

	// ランタイム確保
	new_runtime = _amAllocRuntime();
	amAssert( new_runtime );

	// ランタイム初期化
	new_runtime->ecb				= param->ecb;
	new_runtime->node				= param->node;
	new_runtime->state				= AMD_AME_STATE_ACCEPT_KILL;
	new_runtime->child_head.next	= &( new_runtime->child_tail );
	new_runtime->child_tail.prev	= &( new_runtime->child_head );
	new_runtime->work_head.next		= &( new_runtime->work_tail );
	new_runtime->work_tail.prev		= &( new_runtime->work_head );
	new_runtime->active_head.next	= &( new_runtime->active_tail );
	new_runtime->active_tail.prev	= &( new_runtime->active_head );

	// 子にパーティクルが存在する場合は、
	// スポーンランタイムを作成する
	{
		AMS_AME_NODE* child;

		for ( child = param->node->child; child != NULL; child = child->sibling )
		{
			if ( AMD_AME_IS_PARTICLE( child ) )
			{
				new_runtime->spawn_runtime = _amCreateRuntimeGroup( param->ecb, child );
				amAssert( new_runtime->spawn_runtime );
				break;
			}
		}
	}

	// エントリーリストに登録
	_amAddEntry( param->ecb, new_runtime );

	param->runtime = new_runtime;

	_amCreateParticle( param );

	return new_runtime;
}

// ランタイムグループの作成
AMS_AME_RUNTIME* _amCreateRuntimeGroup(AMS_AME_ECB* ecb, AMS_AME_NODE* node)
{
	AMS_AME_RUNTIME* new_runtime;

	// ランタイム確保
	new_runtime = _amAllocRuntime();
	amAssert( new_runtime );

	// ランタイム初期化
	new_runtime->ecb				= ecb;
	new_runtime->node				= node;
	new_runtime->child_head.next	= &( new_runtime->child_tail );
	new_runtime->child_tail.prev	= &( new_runtime->child_head );
	new_runtime->work_head.next		= &( new_runtime->work_tail );
	new_runtime->work_tail.prev		= &( new_runtime->work_head );
	new_runtime->active_head.next	= &( new_runtime->active_tail );
	new_runtime->active_tail.prev	= &( new_runtime->active_head );

	// 子にパーティクルが存在する場合は、
	// スポーンランタイムを作成する
	{
		AMS_AME_NODE* child;

		for ( child = node->child; child != NULL; child = child->sibling )
		{
			if ( AMD_AME_IS_PARTICLE( child ) )
			{
				new_runtime->spawn_runtime = _amCreateRuntimeGroup( ecb, child );
				amAssert( new_runtime->spawn_runtime );
				break;
			}
		}
	}

	// エントリーリストに登録
	_amAddEntry( ecb, new_runtime );

	return new_runtime;
}

// エミッタの作成
void _amCreateEmitter(AMS_AME_CREATE_PARAM* param)
{
	AMS_AME_RUNTIME* new_runtime;

	new_runtime = _amCreateRuntimeEmitter( param );
	amAssert( new_runtime );

	new_runtime->parent_runtime = param->runtime;

	_amConnectLinkToTail( &( new_runtime->work_tail ), (AMS_AME_LIST*)new_runtime );
	new_runtime->work_num++;

	param->runtime	= new_runtime;
	param->node		= new_runtime->node;
	param->work		= new_runtime->work;
}

// パーティクルの作成
void _amCreateParticle(AMS_AME_CREATE_PARAM* param)
{
	AMS_AME_RUNTIME_WORK* new_work;

	// ランタイムワーク確保
	new_work = _amAllocRuntimeWork();
	amAssert( new_work );

	param->work = new_work;

	// ランタイムワーク初期化
	{
		AmeInitFunc func;

		func = (AmeInitFunc)_am_particle_func[ (AMD_AME_NODE_TYPE( param->node ) & AME_AME_CLASS_ID_MASK) << 2 ];
		amAssert( func );

		func( param );
	}

	if ( new_work->time < 0.0f )
	{
		_amConnectLinkToTail( (AMS_AME_LIST*)&( param->runtime->work_tail ), (AMS_AME_LIST*)new_work );
		param->runtime->work_num++;
	}
	else
	{
		_amConnectLinkToTail( (AMS_AME_LIST*)&( param->runtime->active_tail ), (AMS_AME_LIST*)new_work );
		param->runtime->active_num++;
	}
}

// スポーンパーティクルの作成
void _amCreateSpawnParticle(AMS_AME_RUNTIME* runtime, AMS_AME_RUNTIME_WORK* work)
{
	amAssert( runtime );
	amAssert( work );

	AMS_AME_CREATE_PARAM param;
	AMS_VECTOR zero;

	amVectorInit( &zero );
	amZeroMemory( &param, sizeof(AMS_AME_CREATE_PARAM) );
	param.ecb				= runtime->ecb;
	param.runtime			= runtime->spawn_runtime;
	param.node				= runtime->spawn_runtime->node;
	param.position			= &zero;
	param.velocity			= &zero;
	param.parent_position	= &( work->position );
	param.parent_velocity	= &( work->velocity );

	if ( runtime->state & AMD_AME_STATE_ROOT )
		runtime->spawn_runtime->state |= AMD_AME_STATE_ROOT;

	_amCreateParticle( &param );

#if 1
	// フラグの継承
	if ( AMD_AME_NODE_TYPE( runtime->node ) == AMD_AME_NODE_TYPE( param.node ) )
	{
		AMS_AME_NODE* snode = (AMS_AME_NODE*)runtime->node;
		AMS_AME_NODE* dnode = (AMS_AME_NODE*)param.node;

		switch ( AMD_AME_NODE_TYPE( param.node ) )
		{
		case AME_AME_NODE_TYPE_SIMPLE_SPRITE:
			{
				AMS_AME_RUNTIME_WORK_SIMPLE_SPRITE* swork = (AMS_AME_RUNTIME_WORK_SIMPLE_SPRITE*)work;
				AMS_AME_RUNTIME_WORK_SIMPLE_SPRITE* dwork = (AMS_AME_RUNTIME_WORK_SIMPLE_SPRITE*)param.work;

				// random texture flip u
				if ( snode->flag & dnode->flag & AMD_AME_FLAG_TEXTURE_FLIP_U_RANDOM )
				{
					if ( (swork->flag ^ dwork->flag) & AMD_AME_RWFLAG_TEXTURE_FLIP_U )
					{
						dwork->flag ^= AMD_AME_RWFLAG_TEXTURE_FLIP_U;

						float tmp = dwork->st.x;
						dwork->st.x = dwork->st.z;
						dwork->st.z = tmp;
					}
				}

				// random texture flip v
				if ( snode->flag & dnode->flag & AMD_AME_FLAG_TEXTURE_FLIP_V_RANDOM )
				{
					if ( (swork->flag ^ dwork->flag) & AMD_AME_RWFLAG_TEXTURE_FLIP_V )
					{
						dwork->flag ^= AMD_AME_RWFLAG_TEXTURE_FLIP_V;

						float tmp = dwork->st.y;
						dwork->st.y = dwork->st.w;
						dwork->st.w = tmp;
					}
				}
			}
			break;

		case AME_AME_NODE_TYPE_SPRITE:
			{
				AMS_AME_RUNTIME_WORK_SPRITE* swork = (AMS_AME_RUNTIME_WORK_SPRITE*)work;
				AMS_AME_RUNTIME_WORK_SPRITE* dwork = (AMS_AME_RUNTIME_WORK_SPRITE*)param.work;

				// random twist reverse
				if ( snode->flag & dnode->flag & AMD_AME_FLAG_SPRITE_RANDOM_TWIST_REVERSE )
				{
					if ( swork->flag & AMD_AME_RWFLAG_TWIST_REVERSE )
						dwork->flag |= AMD_AME_RWFLAG_TWIST_REVERSE;
					else
						dwork->flag &= ~AMD_AME_RWFLAG_TWIST_REVERSE;
				}

				// random texture flip u
				if ( snode->flag & dnode->flag & AMD_AME_FLAG_TEXTURE_FLIP_U_RANDOM )
				{
					if ( (swork->flag ^ dwork->flag) & AMD_AME_RWFLAG_TEXTURE_FLIP_U )
					{
						dwork->flag ^= AMD_AME_RWFLAG_TEXTURE_FLIP_U;

						float tmp = dwork->st.x;
						dwork->st.x = dwork->st.z;
						dwork->st.z = tmp;
					}
				}

				// random texture flip v
				if ( snode->flag & dnode->flag & AMD_AME_FLAG_TEXTURE_FLIP_V_RANDOM )
				{
					if ( (swork->flag ^ dwork->flag) & AMD_AME_RWFLAG_TEXTURE_FLIP_V )
					{
						dwork->flag ^= AMD_AME_RWFLAG_TEXTURE_FLIP_V;

						float tmp = dwork->st.y;
						dwork->st.y = dwork->st.w;
						dwork->st.w = tmp;
					}
				}
			}
			break;

		case AME_AME_NODE_TYPE_LINE:
			{
				AMS_AME_RUNTIME_WORK_LINE* swork = (AMS_AME_RUNTIME_WORK_LINE*)work;
				AMS_AME_RUNTIME_WORK_LINE* dwork = (AMS_AME_RUNTIME_WORK_LINE*)param.work;

				// random texture flip u
				if ( snode->flag & dnode->flag & AMD_AME_FLAG_TEXTURE_FLIP_U_RANDOM )
				{
					if ( (swork->flag ^ dwork->flag) & AMD_AME_RWFLAG_TEXTURE_FLIP_U )
					{
						dwork->flag ^= AMD_AME_RWFLAG_TEXTURE_FLIP_U;

						float tmp = dwork->st.x;
						dwork->st.x = dwork->st.z;
						dwork->st.z = tmp;
					}
				}

				// random texture flip v
				if ( snode->flag & dnode->flag & AMD_AME_FLAG_TEXTURE_FLIP_V_RANDOM )
				{
					if ( (swork->flag ^ dwork->flag) & AMD_AME_RWFLAG_TEXTURE_FLIP_V )
					{
						dwork->flag ^= AMD_AME_RWFLAG_TEXTURE_FLIP_V;

						float tmp = dwork->st.y;
						dwork->st.y = dwork->st.w;
						dwork->st.w = tmp;
					}
				}
			}
			break;

		case AME_AME_NODE_TYPE_PLANE:
			{
				AMS_AME_RUNTIME_WORK_PLANE* swork = (AMS_AME_RUNTIME_WORK_PLANE*)work;
				AMS_AME_RUNTIME_WORK_PLANE* dwork = (AMS_AME_RUNTIME_WORK_PLANE*)param.work;

				// random texture flip u
				if ( snode->flag & dnode->flag & AMD_AME_FLAG_TEXTURE_FLIP_U_RANDOM )
				{
					if ( (swork->flag ^ dwork->flag) & AMD_AME_RWFLAG_TEXTURE_FLIP_U )
					{
						dwork->flag ^= AMD_AME_RWFLAG_TEXTURE_FLIP_U;

						float tmp = dwork->st.x;
						dwork->st.x = dwork->st.z;
						dwork->st.z = tmp;
					}
				}

				// random texture flip v
				if ( snode->flag & dnode->flag & AMD_AME_FLAG_TEXTURE_FLIP_V_RANDOM )
				{
					if ( (swork->flag ^ dwork->flag) & AMD_AME_RWFLAG_TEXTURE_FLIP_V )
					{
						dwork->flag ^= AMD_AME_RWFLAG_TEXTURE_FLIP_V;

						float tmp = dwork->st.y;
						dwork->st.y = dwork->st.w;
						dwork->st.w = tmp;
					}
				}
			}
			break;
		}
	}
#endif
}

// ランタイム確保
AMS_AME_RUNTIME* _amAllocRuntime()
{
#if AMD_DEBUG
	amAssert( _am_runtime_num != AMD_AME_RUNTIME_SIZE );	// ランタイムサイズオーバー
	_am_runtime_num++;
	if ( _am_runtime_peak < _am_runtime_num )
	{
		_am_runtime_peak = _am_runtime_num;
	}
#endif

	AMS_AME_RUNTIME* new_runtime = _am_runtime_ref[ _am_runtime_alloc ];
	++_am_runtime_alloc;
	if ( _am_runtime_alloc >= AMD_AME_RUNTIME_SIZE ) _am_runtime_alloc = 0;

	amZeroMemory( new_runtime, sizeof(AMS_AME_RUNTIME) );

	return new_runtime;
}

// ================================================================
/*!
	ランタイム開放
	
	@param	[o]		ランタイムワークのポインタ
*/
// ================================================================
void _amFreeRuntime(AMS_AME_RUNTIME* runtime)
{
	amAssert( runtime );

	// ランタイムワーク開放
	if ( runtime->work )
	{
		AMS_AME_RUNTIME_WORK* del_work = runtime->work;
		amEffectFreeRuntimeWork( del_work );
	}

	// 保持しているランタイムワークをすべて開放
//	if ( AME_IS_PARTICLE( runtime->node ) )
	{
		AMS_AME_RUNTIME_WORK *work, *tail;

		// ワークの先頭から終端までの開放
		work = (AMS_AME_RUNTIME_WORK*)runtime->work_head.next;
		tail = (AMS_AME_RUNTIME_WORK*)&( runtime->work_tail );
		while ( work != tail )
		{
			amEffectFreeRuntimeWork( work );
			work = work->next;
		}

		// アクティブワークの先頭から終端までの開放
		work = (AMS_AME_RUNTIME_WORK*)runtime->active_head.next;
		tail = (AMS_AME_RUNTIME_WORK*)&( runtime->active_tail );
		while ( work != tail )
		{
			amEffectFreeRuntimeWork( work );
			work = work->next;
		}
	}

#if AMD_DEBUG
	_am_runtime_num--;
	amAssert( _am_runtime_num >= 0 );
#endif

	_am_runtime_ref[ _am_runtime_free ] = runtime;
	_am_runtime_free++;
	if ( _am_runtime_free >= AMD_AME_RUNTIME_SIZE ) _am_runtime_free = 0;
}

// _amAllocRuntimeWork
/*!
	ランタイムワーク確保
	
	@return	ランタイムワークのポインタ
*/
AMS_AME_RUNTIME_WORK* _amAllocRuntimeWork()
{
	AMS_AME_RUNTIME_WORK* new_work;

#if AMD_DEBUG
	amAssert( _am_work_num != AMD_AME_WORK_SIZE );	// ランタイムワークサイズオーバー
	_am_work_num++;
	if ( _am_work_peak < _am_work_num ) _am_work_peak = _am_work_num;
#endif

	new_work = _am_work_ref[ _am_work_alloc ];
	_am_work_alloc++;
	if ( _am_work_alloc >= AMD_AME_WORK_SIZE ) _am_work_alloc = 0;

	amZeroMemory( new_work, sizeof(AMS_AME_RUNTIME_WORK) );

	return new_work;
}

// amFreeRuntimeWork
/*!
	ランタイムワーク開放
	
	@param param1	[out] ランタイムワークのポインタ
*/
void amEffectFreeRuntimeWork(AMS_AME_RUNTIME_WORK* work)
{
	amAssert( work );

#if AMD_DEBUG
	_am_work_num--;
	amAssert( _am_work_num >= 0 );
#endif

	_am_work_ref[ _am_work_free ] = work;
	_am_work_free++;
	if ( _am_work_free >= AMD_AME_WORK_SIZE ) _am_work_free = 0;
}

// _amAddEntry
/*!
	@param param0	[in]	入力引数0説明
	@param param1	[out]	出力ポインタ引数1説明
*/
void _amAddEntry(AMS_AME_ECB* ecb, AMS_AME_RUNTIME* runtime)
{
	amAssert( ecb );
	amAssert( runtime );

	AMS_AME_ENTRY* new_entry;

	// エントリー確保
	{
#if AMD_DEBUG
		amAssert( _am_entry_num != AMD_AME_ENTRY_SIZE );	// エントリーサイズオーバー
		_am_entry_num++;
		if ( _am_entry_peak < _am_entry_num ) _am_entry_peak = _am_entry_num;
#endif

		new_entry = _am_entry_ref[ _am_entry_alloc ];
		_am_entry_alloc++;
		if ( _am_entry_alloc >= AMD_AME_ENTRY_SIZE ) _am_entry_alloc = 0;
	}

	// エントリー初期化
	{
		new_entry->runtime = runtime;
	}

	// リストの最後に追加
	if ( ecb->entry_head == NULL )
	{
		ecb->entry_head = new_entry;
		new_entry->prev = NULL;
	}
	if ( ecb->entry_tail )
	{
		new_entry->prev = ecb->entry_tail;
		ecb->entry_tail->next = new_entry;
	}
	ecb->entry_tail = new_entry;
	new_entry->next = NULL;

	ecb->entry_num++;
}

void _amDelEntry(AMS_AME_ECB* ecb, AMS_AME_ENTRY* entry)
{
	amAssert( ecb );
	amAssert( entry );

	if ( entry->prev == NULL )
		ecb->entry_head = entry->next;
	else
		entry->prev->next = entry->next;

	if ( entry->next == NULL )
		ecb->entry_tail = entry->prev;
	else
		entry->next->prev = entry->prev;

	// エントリー開放
	{
#if AMD_DEBUG
		_am_entry_num--;
		amAssert( _am_entry_num >= 0 );
#endif

		_am_entry_ref[ _am_entry_free ] = entry;
		_am_entry_free++;
		if ( _am_entry_free >= AMD_AME_ENTRY_SIZE ) _am_entry_free = 0;
	}

	ecb->entry_num--;
}

// ================================================================
/*!
	各種描画モードを設定し、アルファブレンドするかどうかを返す

	@param runtime [input] ランタイム
	@param param   [input] プリミティブ設定情報
	@param NodeBlend   [input] ブレンドモード（AMM_BLENDで計算された値）
	@retrun NNE_PRIM_ALPHABLEND の値
*/
// ================================================================
static NNE_PRIM_ALPHABLEND _amEffectSetDrawMode(AMS_AME_RUNTIME* runtime, AMS_PARAM_DRAW_PRIMITIVE* param,
												Sint32 NodeBlend)
{
	AMS_AME_NODE* node = (AMS_AME_NODE*)runtime->node;

	NNE_PRIM_ALPHABLEND blend = NNE_PRIM_ALPHABLEND_OFF;
	param->ablend = blend;

	// αテスト
	if ( node->flag & AMD_AME_FLAG_SPRITE_ALPHA_TEST )
	{
		param->aTest = 1;
	}
	// αテストしない
	else
	{
		param->aTest = 0;
	}

	// Zマスク（Ｚバッファを更新しない）
	if( node->flag & AMD_AME_FLAG_NODE_ZMASK )
	{
		param->zMask = 1;
	}
	else
	{
		param->zMask = 0;
	}

	// Zテストしない
	if( !(node->flag & AMD_AME_FLAG_NODE_ZTEST) )
	{
		param->zTest = 0;
	}
	// Zテストする
	else
	{
		param->zTest = 1;
	}

	// αブレンド
	if ( node->flag & AMD_AME_FLAG_SPRITE_ALPHA_BLEND )
	{
		blend = NNE_PRIM_ALPHABLEND_ON;
		param->ablend = blend;
		switch ( NodeBlend )
		{
		case AMM_BLEND_NORMAL:

#if _PC | _XBOX
			param->bldSrc = NNE_BLENDMODE_SRCALPHA;
			param->bldDst = NNE_BLENDMODE_INVSRCALPHA;
			param->bldMode = NNE_BLENDOP_ADD;
#elif _PS3
            param->bldSrc = NND_BLENDFUNC_PS3_SRC_ALPHA;
			param->bldDst = NND_BLENDFUNC_PS3_ONE_MINUS_SRC_ALPHA;
			param->bldMode = NND_BLENDOP_PS3_FUNC_ADD;
#elif _WII
			param->bldSrc = GX_BL_SRCALPHA;
			param->bldDst = GX_BL_INVSRCALPHA;
			param->bldMode = GX_BM_BLEND;
#elif _IPHONE
			param->bldSrc = NND_BLENDFUNC_GL_SRC_ALPHA;
			param->bldDst = NND_BLENDFUNC_GL_ONE_MINUS_SRC_ALPHA;
			param->bldMode = NND_BLENDOP_GL_FUNC_ADD;
#endif
			break;

		case AMM_BLEND_ADD:
#if _PC | _XBOX
			param->bldSrc = NNE_BLENDMODE_SRCALPHA;
			param->bldDst = NNE_BLENDMODE_ONE;
			param->bldMode = NNE_BLENDOP_ADD;
#elif _PS3
			param->bldSrc = NND_BLENDFUNC_PS3_SRC_ALPHA;
			param->bldDst = NND_BLENDFUNC_PS3_ONE;
			param->bldMode = NND_BLENDOP_PS3_FUNC_ADD;
#elif _WII
			param->bldSrc = GX_BL_SRCALPHA;
			param->bldDst = GX_BL_ONE;
			param->bldMode = GX_BM_BLEND;
#elif _IPHONE
			param->bldSrc = NND_BLENDFUNC_GL_SRC_ALPHA;
			param->bldDst = NND_BLENDFUNC_GL_ONE;
			param->bldMode = NND_BLENDOP_GL_FUNC_ADD;
#endif
			break;

		case AMM_BLEND_SUB:
#if _PC | _XBOX
			param->bldSrc = NNE_BLENDMODE_SRCALPHA;
			param->bldDst = NNE_BLENDMODE_ONE;
			param->bldMode = NNE_BLENDOP_REVSUB;
#elif _PS3
			param->bldSrc = NND_BLENDFUNC_PS3_SRC_ALPHA;
			param->bldDst = NND_BLENDFUNC_PS3_ONE;
			param->bldMode = NND_BLENDOP_PS3_FUNC_REVERSE_SUBTRACT;
#elif _WII
			param->bldSrc = GX_BL_SRCALPHA;
			param->bldDst = GX_BL_ONE;
			param->bldMode = GX_BM_SUBTRACT;
#elif _IPHONE
			param->bldSrc = NND_BLENDFUNC_GL_SRC_ALPHA;
			param->bldDst = NND_BLENDFUNC_GL_ONE;
			param->bldMode = NND_BLENDOP_GL_FUNC_REVERSE_SUBTRACT;
#endif
			break;
		}
	}
	
	return blend;
}

// ================================================================
/*!
	各種マテリアルを設定する

	@param runtime [input] ランタイム
	@param NodeBlend   [input] ブレンドモード（AMM_BLENDで計算された値）
*/
// ================================================================
static NNF_DRAWOBJ _amEffectSetMaterial(AMS_AME_RUNTIME* runtime, Sint32* blend, Sint32 NodeBlend)
{ 
    AMS_AME_NODE* node = (AMS_AME_NODE*)runtime->node;
	NNF_DRAWOBJ draw_flag = 0;

	// αブレンド
	if ( node->flag & AMD_AME_FLAG_MODEL_ALPHA_BLEND )
	{
		draw_flag |= (NND_DRAWOBJ_IGNOREMATSPEC | NND_DRAWOBJ_MATCTRL_ALPHA /*| NND_DRAWOBJ_MATCTRL_BLEND*/);
		switch ( NodeBlend )
		{
		case AMM_BLEND_NORMAL:
			*blend = NNE_MATCTRL_BLEND_ALPHA;
			break;

		case AMM_BLEND_ADD:
			*blend = NNE_MATCTRL_BLEND_ADD;
			break;

		case AMM_BLEND_SUB:
			*blend = NNE_MATCTRL_BLEND_SUBTRACT;
			break;
		}
	}

	return draw_flag;
}


// Debug Functions -------------------------------------------------------------

#if AMD_DEBUG

#define AMD_AME_DBGMON_X	(1)
#define AMD_AME_DBGMON_Y	(38)

//! デバッグ情報表示
void amEffectDebugDisplayInfo()
{
	amPrint(  AMD_AME_DBGMON_X,AMD_AME_DBGMON_Y + 0, "[ EFFECT MONITOR ]" );
	amPrintf( AMD_AME_DBGMON_X,AMD_AME_DBGMON_Y + 1, "    ECB: %4d/%4d", _am_ecb_num, AMD_AME_ECB_SIZE);
	amPrintf( AMD_AME_DBGMON_X,AMD_AME_DBGMON_Y + 2, "  ENTRY: %4d/%4d", _am_entry_num, AMD_AME_ENTRY_SIZE);
	amPrintf( AMD_AME_DBGMON_X,AMD_AME_DBGMON_Y + 3, "RUNTIME: %4d/%4d", _am_runtime_num, AMD_AME_RUNTIME_SIZE);
	amPrintf( AMD_AME_DBGMON_X,AMD_AME_DBGMON_Y + 4, "   WORK: %4d/%4d", _am_work_num, AMD_AME_WORK_SIZE);
	amPrintf( AMD_AME_DBGMON_X,AMD_AME_DBGMON_Y + 5, "   PEEK: %4d",	 _am_runtime_peak );
}
#endif


// User Effect Functions -------------------------------------------------------
//{### ユーザーエフェクト関数
//******************************************************************************
//	_amInitOmni()
//------------------------------------------------------------------------------
//	[Function]
//		全方向エミッタの初期化
//	[Output]
//		param : 作成パラメータへのポインタ
//	[Comment]
//		注意事項など
//******************************************************************************
void _amInitOmni(AMS_AME_CREATE_PARAM* param)
{
	AMS_AME_NODE_OMNI*			node = (AMS_AME_NODE_OMNI*)param->node;
	AMS_AME_RUNTIME_WORK_OMNI*	work = (AMS_AME_RUNTIME_WORK_OMNI*)param->work;

	amAssert( AMD_AME_IS_EMITTER( node ) );

	work->time = -node->start_time;

	// position & velocity & rotate
	{
		amVectorAdd( &( work->position ), param->parent_position, param->position );
		amVectorAdd( &( work->position ), &( node->translate ) );

		amVectorScale( &( work->velocity ), param->parent_velocity, node->inheritance_rate );
		amVectorAdd( &( work->velocity ), param->velocity );

#if AMD_AME_ROTATE_QUAT
		amVectorCopy( (AMS_VECTOR*)&( work->rotate ), (AMS_VECTOR*)&( node->rotate ) );
#else
		amVectorCopy( &( work->rotate ), &( node->rotate ) );
#endif
	}

	float rate = param->ecb->size_rate;
	work->offset = node->offset * rate;
	work->offset_chaos = node->offset_chaos * rate;
}

//******************************************************************************
//	_amUpdateOmni()
//------------------------------------------------------------------------------
//	[Function]
//		全方向エミッタの更新
//	[Output]
//		runtime : ランタイムへのポインタ
//	[Return]
//		1	: 削除要求
//	[Comment]
//		注意事項など
//******************************************************************************
Sint32 _amUpdateOmni(AMS_AME_RUNTIME* runtime)
{
	AMS_AME_NODE_OMNI*			node = (AMS_AME_NODE_OMNI*)runtime->node;
	AMS_AME_RUNTIME_WORK_OMNI*	work = (AMS_AME_RUNTIME_WORK_OMNI*)runtime->work;

	amAssert( AMD_AME_IS_EMITTER( node ) );

	work->time += _am_unit_frame;

	// start time
	if ( work->time <= 0.0f )
	{
		return 0;
	}

	// life
	if ( node->life != -1.0f && work->time >= node->life )
	{
		return 1;
	}

	// position & velocity
	{
		AMS_VECTOR vel;
		amVectorScale( &vel, &( work->velocity ), _am_unit_time );
		amVectorAdd( &( work->position ), &vel );
	}

	float rate = runtime->ecb->size_rate;
	work->offset = node->offset * rate;
	work->offset_chaos = node->offset_chaos * rate;

	{
		AMS_AME_RUNTIME* child = (AMS_AME_RUNTIME*)runtime->child_head.next;
		AMS_AME_RUNTIME* tail = (AMS_AME_RUNTIME*)&( runtime->child_tail );

		for ( ; child != tail; child = child->next )
		{
			child->amount += node->frequency * _am_unit_frame;

			while ( child->amount >= 1.0f )
			{
				child->amount -= 1.0f;
				child->count++;

				if ( node->max_count != -1.0f && (child->work_num + child->active_num) < node->max_count )
				{
					AMS_AME_CREATE_PARAM param;

					AMS_VECTOR position;
					AMS_VECTOR velocity, direction;

					// direction
					amVectorRandom( &direction );

					// offset
					amVectorScale( &velocity, &direction, work->offset + work->offset_chaos * nnRandom() );
					amVectorCopy( &position, &velocity );

					// speed
					amVectorScale( &velocity, &direction, node->speed + node->speed_chaos * nnRandom() );

					param.ecb				= runtime->ecb;
					param.runtime			= child;
					param.node				= child->node;
					param.parent_position	= &( work->position );
					param.parent_velocity	= &( work->velocity );
					param.position			= &position;
					param.velocity			= &velocity;

					switch ( AMD_AME_NODE_TYPE( child->node ) & AME_AME_SUPER_CLASS_ID_MASK )
					{
					case AME_AME_NODE_TYPE_PARTICLE:	_amCreateParticle( &param );	break;
					case AME_AME_NODE_TYPE_EMITTER:		_amCreateEmitter( &param );		break;
					}
				}
			}
		}
	}

	return 0;
}

//******************************************************************************
//	_amDrawOmni()
//------------------------------------------------------------------------------
//	[Function]
//		全方向エミッタの描画
//	[Output]
//		runtime : ランタイムへのポインタ
//	[Comment]
//		リリース時は無効になります
//******************************************************************************
void _amDrawOmni(AMS_AME_RUNTIME* runtime)
{
	UNREFERENCED_PARAMETER(runtime);
#if 0
#if AMD_DEBUG
	AMS_AME_NODE_OMNI*			node = (AMS_AME_NODE_OMNI*)runtime->node;
	AMS_AME_RUNTIME_WORK_OMNI*	work = (AMS_AME_RUNTIME_WORK_OMNI*)runtime->work;

	Quad128 col = { 1.0f,1.0f,1.0f,1.0f };

	amSaveMatrix( NULL );
#if AMD_AME_ROTATE_QUAT
	amMultiQuatMatrix( &( work->rotate ), &( work->position ) );
#else
	amTranslate( &( work->position ) );
	amRotateXYZ( (float*)&( work->rotate ) );
#endif

	esuDrawSphere( node->speed, &col );
	esuDrawAxis( node->speed );

	amLoadMatrix();
#endif
#endif
}

//******************************************************************************
//	_amInitDirectional()
//------------------------------------------------------------------------------
//	[Function]
//		単方向エミッタの初期化
//	[Output]
//		param	: 作成パラメータへのポインタ
//	[Comment]
//		注意事項など
//******************************************************************************
void _amInitDirectional(AMS_AME_CREATE_PARAM* param)
{
	AMS_AME_NODE_DIRECTIONAL*			node = (AMS_AME_NODE_DIRECTIONAL*)param->node;
	AMS_AME_RUNTIME_WORK_DIRECTIONAL*	work = (AMS_AME_RUNTIME_WORK_DIRECTIONAL*)param->work;

	amAssert( AMD_AME_IS_EMITTER( node ) );

	work->time = -node->start_time;

	// position & velocity
	{
		amVectorAdd( &( work->position ), param->parent_position, param->position );
		amVectorAdd( &( work->position ), &( node->translate ) );

		amVectorScale( &( work->velocity ), param->parent_velocity, node->inheritance_rate );
		amVectorAdd( &( work->velocity ), param->velocity );

#if AMD_AME_ROTATE_QUAT
		amVectorCopy( (AMS_VECTOR*)&( work->rotate ), (AMS_VECTOR*)&( node->rotate ) );
#else
		amVectorCopy( &( work->rotate ), &( node->rotate ) );
#endif
	}

	work->spread = node->spread;
}

//******************************************************************************
//	_amUpdateDirectional()
//------------------------------------------------------------------------------
//	[Function]
//		単方向エミッタの更新
//	[Output]
//		runtime : ランタイムへのポインタ
//	[Return]
//		1	: 削除要求
//	[Comment]
//		注意事項など
//******************************************************************************
Sint32 _amUpdateDirectional(AMS_AME_RUNTIME* runtime)
{
	AMS_AME_NODE_DIRECTIONAL*			node = (AMS_AME_NODE_DIRECTIONAL*)runtime->node;
	AMS_AME_RUNTIME_WORK_DIRECTIONAL*	work = (AMS_AME_RUNTIME_WORK_DIRECTIONAL*)runtime->work;

	amAssert( AMD_AME_IS_EMITTER( node ) );

	work->time += _am_unit_frame;

	// start time
	if ( work->time <= 0.0f )
	{
		return 0;
	}

	// life
	if ( node->life != -1.0f && work->time >= node->life )
	{
		return 1;
	}

	// position & velocity
	{
		AMS_VECTOR vel;
		amVectorScale( &vel, &( work->velocity ), _am_unit_time );
		amVectorAdd( &( work->position ), &vel );
	}

	// spread
	{
		work->spread += node->spread_variation * _am_unit_time;
	}

#if AMD_AME_ROTATE_PRECALC
	NNS_MATRIX mtx;
	nnMakeUnitMatrix(&mtx);
	amMatrixPush( &mtx );
#if AMD_AME_ROTATE_QUAT
	amQuatToMatrix( NULL, &( work->rotate ), NULL );
#else // AMD_AME_ROTATE_QUAT
	amRotateXYZ( work->rotate.x, work->rotate.y, work->rotate.z );
#endif // !AMD_AME_ROTATE_QUAT
#endif // AMD_AME_ROTATE_PRECALC

	{
		AMS_AME_RUNTIME* child = (AMS_AME_RUNTIME*)runtime->child_head.next;
		AMS_AME_RUNTIME* tail = (AMS_AME_RUNTIME*)&( runtime->child_tail );

		for ( ; child != tail; child = child->next )
		{
			child->amount += node->frequency * _am_unit_frame;

			while ( child->amount >= 1.0f )
			{
				child->amount -= 1.0f;
				child->count++;

				if ( node->max_count != -1.0f && (child->work_num + child->active_num) < node->max_count )
				{
					AMS_AME_CREATE_PARAM param;

					AMS_VECTOR position;
					AMS_VECTOR velocity, direction;

					// direction
					amEffectRandomConeVectorDeg( &direction, work->spread );

#if AMD_AME_ROTATE_PRECALC
					amMatrixCalcPoint( &direction, &direction );
#else // AMD_AME_ROTATE_PRECALC
					{
#if AMD_AME_ROTATE_QUAT
						amMultiQuatVector( &direction, &direction, &( work->rotate ), NULL );
#else // AMD_AME_ROTATE_QUAT
						amSaveMatrix( &_am_unit_matrix );
						amRotateXYZ( work->rotate.x, work->rotate.y, work->rotate.z );
						amCalcPoint( &direction, &direction, );
						amLoadMatrix();
#endif // !AMD_AME_ROTATE_QUAT
					}
#endif // !AMD_AME_ROTATE_PRECALC

					// offset
					amVectorScale( &velocity, &direction, node->offset + node->offset_chaos * nnRandom() );
					amVectorCopy( &position, &velocity );

					// speed
					amVectorScale( &velocity, &direction, node->speed + node->speed_chaos * nnRandom() );

					param.ecb				= runtime->ecb;
					param.runtime			= child;
					param.node				= child->node;
					param.parent_position	= &( work->position );
					param.parent_velocity	= &( work->velocity );
					param.position			= &position;
					param.velocity			= &velocity;

					switch ( AMD_AME_NODE_TYPE( child->node ) & AME_AME_SUPER_CLASS_ID_MASK )
					{
					case AME_AME_NODE_TYPE_PARTICLE:	_amCreateParticle( &param );break;
					case AME_AME_NODE_TYPE_EMITTER:		_amCreateEmitter( &param );	break;
					}
				}
			}
		}
	}

#if AMD_AME_ROTATE_PRECALC
	amMatrixPop();
#endif // AMD_AME_ROTATE_PRECALC

	return 0;
}

//******************************************************************************
//	_amDrawDirectional()
//------------------------------------------------------------------------------
//	[Function]
//		単方向エミッタの描画
//	[Output]
//		runtime : ランタイムへのポインタ
//	[Comment]
//		リリース時は無効になります
//******************************************************************************
void _amDrawDirectional(AMS_AME_RUNTIME* runtime)
{
	UNREFERENCED_PARAMETER(runtime);
#if 0
#if AMD_DEBUG
	AMS_AME_NODE_DIRECTIONAL*			node = (AMS_AME_NODE_DIRECTIONAL*)runtime->node;
	AMS_AME_RUNTIME_WORK_DIRECTIONAL*	work = (AMS_AME_RUNTIME_WORK_DIRECTIONAL*)runtime->work;

	Quad128 col = { 1.0f,1.0f,1.0f,1.0f };

	amSaveMatrix( NULL );
#if AMD_AME_ROTATE_QUAT
	amMultiQuatMatrix( &( work->rotate ), &( work->position ) );
#else
	amTranslate( &( work->position ) );
	amRotateXYZ( (float*)&( work->rotate ) );
#endif

	esuDrawCone( node->speed * amTan( amDegToRad( node->spread ) ), node->speed, &col );
	esuDrawAxis( node->speed );

	amLoadMatrix();
#endif
#endif
}

//******************************************************************************
//	_amInitSurface()
//------------------------------------------------------------------------------
//	[Function]
//		面エミッタの初期化
//	[Output]
//		param	: 作成パラメータへのポインタ
//	[Comment]
//		注意事項など
//******************************************************************************
void _amInitSurface(AMS_AME_CREATE_PARAM* param)
{
	AMS_AME_NODE_SURFACE*			node = (AMS_AME_NODE_SURFACE*)param->node;
	AMS_AME_RUNTIME_WORK_SURFACE*	work = (AMS_AME_RUNTIME_WORK_SURFACE*)param->work;

	amAssert( AMD_AME_IS_EMITTER( node ) );

	work->time = -node->start_time;

	// position & velocity
	{
		amVectorAdd( &( work->position ), param->parent_position, param->position );
		amVectorAdd( &( work->position ), &( node->translate ) );

		amVectorScale( &( work->velocity ), param->parent_velocity, node->inheritance_rate );
		amVectorAdd( &( work->velocity ), param->velocity );

		amVectorCopy( (AMS_VECTOR*)&( work->rotate ), (AMS_VECTOR*)&( node->rotate ) );
	}

	float rate = param->ecb->size_rate;
	work->width = node->width * rate;
	work->height = node->height * rate;
	work->offset = node->offset * rate;
	work->offset_chaos = node->offset_chaos * rate;
}

//******************************************************************************
//	_amUpdateSurface()
//------------------------------------------------------------------------------
//	[Function]
//		面エミッタの更新
//	[Output]
//		runtime : ランタイムへのポインタ
//	[Return]
//		1	: 削除要求
//	[Comment]
//		注意事項など
//******************************************************************************
Sint32 _amUpdateSurface(AMS_AME_RUNTIME* runtime)
{
	AMS_AME_NODE_SURFACE*			node = (AMS_AME_NODE_SURFACE*)runtime->node;
	AMS_AME_RUNTIME_WORK_SURFACE*	work = (AMS_AME_RUNTIME_WORK_SURFACE*)runtime->work;

	amAssert( AMD_AME_IS_EMITTER( node ) );

	work->time += _am_unit_frame;

	// start time
	if ( work->time <= 0.0f )
	{
		return 0;
	}

	// life
	if ( node->life != -1.0f && work->time >= node->life )
	{
		return 1;
	}

	// position & velocity
	{
		AMS_VECTOR vel;
		amVectorScale( &vel, &( work->velocity ), _am_unit_time );
		amVectorAdd( &( work->position ), &vel );
	}

	// width & height
	{
		float rate = runtime->ecb->size_rate;

		if ( node->width_variation || node->height_variation )
		{
			work->width += node->width_variation * _am_unit_time;
			work->height += node->height_variation * _am_unit_time;
		}
		else
		{
			work->width = node->width * rate;
			work->height = node->height * rate;
		}

		work->offset = node->offset * rate;
		work->offset_chaos = node->offset_chaos * rate;
	}

#if AMD_AME_ROTATE_PRECALC
	NNS_MATRIX mtx;
	nnMakeUnitMatrix(&mtx);
	amMatrixPush( &mtx );
#if AMD_AME_ROTATE_QUAT
	amQuatToMatrix( NULL, &( work->rotate ), NULL );
#else // AMD_AME_ROTATE_QUAT
	amRotateXYZ( work->rotate.x, work->rotate.y, work->rotate.z );
#endif // !AMD_AME_ROTATE_QUAT
#endif // AMD_AME_ROTATE_PRECALC
	
	{
		AMS_AME_RUNTIME* child = (AMS_AME_RUNTIME*)runtime->child_head.next;
		AMS_AME_RUNTIME* tail = (AMS_AME_RUNTIME*)&( runtime->child_tail );

		for ( ; child != tail; child = child->next )
		{
			child->amount += node->frequency * _am_unit_frame;

			while ( child->amount >= 1.0f )
			{
				child->amount -= 1.0f;
				child->count++;

				if ( node->max_count != -1.0f && (child->work_num + child->active_num) < node->max_count )
				{
					AMS_AME_CREATE_PARAM param;

					AMS_VECTOR position;
					AMS_VECTOR velocity, direction;

					amVectorSet( &direction, 0.0f,1.0f,0.0f );
					{
						float x, z;
						//x = work->width * amFastRandf() - (work->width * 0.5f);
						//z = work->height * amFastRandf() - (work->height * 0.5f);
						float rnd;
						rnd = nnRandom() / 2.0f + nnRandom() * 0.5f;
						x = work->width * rnd - (work->width * 0.5f);
						rnd = nnRandom() / 2.0f + nnRandom() * 0.5f;
						z = work->height* rnd - (work->height * 0.5f);

						// 外周のみに生成
						if ( node->flag & AMD_AME_FLAG_SURFACE_EDGE_ONLY )
						{
							float val = nnRandom();
							Sint32 sign = 0;

							if ( nnRandom() > 0.5f )
							{
								sign = 0x80000000;
							}

							if ( val > 0.5f )
							{
								x = work->width * 0.5f;
								*((int*)&x) = *((int*)&x) | sign;
							}
							else
							{
								z = work->height * 0.5f;
								*((int*)&z) = *((int*)&z) | sign;
							}
						}

						amVectorSet( &position, x, 0.0f, z );
					}

#if AMD_AME_ROTATE_PRECALC
					amMatrixCalcPoint( &position, &position );
					amMatrixCalcPoint( &direction, &direction );
#else // AMD_AME_ROTATE_PRECALC
					{
						amSaveMatrix( &_am_unit_matrix );
#if AMD_AME_ROTATE_QUAT
						amQuatToMatrix( NULL, &( work->rotate ), NULL );
#else // AMD_AME_ROTATE_QUAT
						amRotateXYZ( work->rotate.x, work->rotate.y, work->rotate.z );
#endif // !AMD_AME_ROTATE_QUAT
						amCalcPoint( &position, &position );
						amCalcPoint( &direction, &direction );
						amLoadMatrix();
					}
#endif // !AMD_AME_ROTATE_PRECALC

					// offset
					amVectorScale( &velocity, &direction, work->offset + work->offset_chaos * nnRandom() );
					amVectorAdd( &position, &velocity );

					// speed
					amVectorScale( &velocity, &direction, node->speed + node->speed_chaos * nnRandom() );

					param.ecb				= runtime->ecb;
					param.runtime			= child;
					param.node				= child->node;
					param.parent_position	= &( work->position );
					param.parent_velocity	= &( work->velocity );
					param.position			= &position;
					param.velocity			= &velocity;

					switch ( AMD_AME_NODE_TYPE( child->node ) & AME_AME_SUPER_CLASS_ID_MASK )
					{
					case AME_AME_NODE_TYPE_PARTICLE:	_amCreateParticle( &param ); break;
					case AME_AME_NODE_TYPE_EMITTER:		_amCreateEmitter( &param ); break;
					}
				}
			}
		}
	}

#if AMD_AME_ROTATE_PRECALC
	amMatrixPop();
#endif // AMD_AME_ROTATE_PRECALC

	return 0;
}

//******************************************************************************
//	_amDrawSurface()
//------------------------------------------------------------------------------
//	[Function]
//		面エミッタの描画
//	[Output]
//		runtime : ランタイムへのポインタ
//	[Comment]
//		リリース時は無効になります
//******************************************************************************
void _amDrawSurface(AMS_AME_RUNTIME* runtime)
{
	UNREFERENCED_PARAMETER(runtime);
#if 0
#if AMD_DEBUG
	AMS_AME_NODE_SURFACE*			node = (AMS_AME_NODE_SURFACE*)runtime->node;
	AMS_AME_RUNTIME_WORK_SURFACE*	work = (AMS_AME_RUNTIME_WORK_SURFACE*)runtime->work;

	Quad128 col = { 1.0f,1.0f,1.0f,1.0f };

	amSaveMatrix( NULL );
#if AMD_AME_ROTATE_QUAT
	amMultiQuatMatrix( &( work->rotate ), &( work->position ) );
#else
	amTranslate( &( work->position ) );
	amRotateXYZ( (float*)&( work->rotate ) );
#endif

	esuDrawPlane( node->width, node->height, &col );
	esuDrawAxis( (node->width + node->height) * 0.5f );

	amLoadMatrix();
#endif
#endif
}

//******************************************************************************
//	_amInitCircle()
//------------------------------------------------------------------------------
//	[Function]
//		円エミッタの初期化
//	[Output]
//		param	: 作成パラメータへのポインタ
//	[Comment]
//		注意事項など
//******************************************************************************
void _amInitCircle(AMS_AME_CREATE_PARAM* param)
{
	AMS_AME_NODE_CIRCLE*			node = (AMS_AME_NODE_CIRCLE*)param->node;
	AMS_AME_RUNTIME_WORK_CIRCLE*	work = (AMS_AME_RUNTIME_WORK_CIRCLE*)param->work;

	amAssert( AMD_AME_IS_EMITTER( node ) );

	work->time = -node->start_time;

	// position & velocity & rotate
	{
		amVectorAdd( &( work->position ), param->parent_position, param->position );
		amVectorAdd( &( work->position ), &( node->translate ) );

		amVectorScale( &( work->velocity ), param->parent_velocity, node->inheritance_rate );
		amVectorAdd( &( work->velocity ), param->velocity );

#if AMD_AME_ROTATE_QUAT
		amVectorCopy( (AMS_VECTOR*)&( work->rotate ), (AMS_VECTOR*)&( node->rotate ) );
#else
		amVectorCopy( &( work->rotate ), &( node->rotate ) );
#endif
	}

	float rate = param->ecb->size_rate;
	work->spread = node->spread;
	work->radius = node->radius * rate;
	work->offset = node->offset * rate;
	work->offset_chaos = node->offset_chaos * rate;
}

//******************************************************************************
//	_amUpdateCircle()
//------------------------------------------------------------------------------
//	[Function]
//		円エミッタの更新
//	[Output]
//		runtime : ランタイムへのポインタ
//	[Return]
//		1	: 削除要求
//	[Comment]
//		注意事項など
//******************************************************************************
Sint32 _amUpdateCircle(AMS_AME_RUNTIME* runtime)
{
	AMS_AME_NODE_CIRCLE*			node = (AMS_AME_NODE_CIRCLE*)runtime->node;
	AMS_AME_RUNTIME_WORK_CIRCLE*	work = (AMS_AME_RUNTIME_WORK_CIRCLE*)runtime->work;

	amAssert( AMD_AME_IS_EMITTER( node ) );

	work->time += _am_unit_frame;

	// start time
	if ( work->time <= 0.0f )
	{
		return 0;
	}

	// life
	if ( node->life != -1.0f && work->time >= node->life )
	{
		return 1;
	}

	// position & velocity
	{
		AMS_VECTOR vel;
		amVectorScale( &vel, &( work->velocity ), _am_unit_time );
		amVectorAdd( &( work->position ), &vel );
	}

	// spread & radius
	{
		float rate = runtime->ecb->size_rate;
	
		work->spread += node->spread_variation * _am_unit_time;
		if ( node->radius_variation )
		{
			work->radius += node->radius_variation * _am_unit_time;
		}
		else
		{
			work->radius = node->radius * rate;
		}

		work->offset = node->offset * rate;
		work->offset_chaos = node->offset_chaos * rate;
	}

#if AMD_AME_ROTATE_PRECALC
	NNS_MATRIX mtx;
	nnMakeUnitMatrix(&mtx);
	amMatrixPush( &mtx );
#if AMD_AME_ROTATE_QUAT
	amQuatToMatrix( NULL, &( work->rotate ), NULL );
#else // AMD_AME_ROTATE_QUAT
	amRotateXYZ( work->rotate.x, work->rotate.y, work->rotate.z );
#endif // !AMD_AME_ROTATE_QUAT
#endif // AMD_AME_ROTATE_PRECALC
	
	{
		AMS_AME_RUNTIME* child = (AMS_AME_RUNTIME*)runtime->child_head.next;
		AMS_AME_RUNTIME* tail = (AMS_AME_RUNTIME*)&( runtime->child_tail );

		for ( ; child != tail; child = child->next )
		{
			child->amount += node->frequency * _am_unit_frame;

			while ( child->amount >= 1.0f )
			{
				child->amount -= 1.0f;
				child->count++;

				if ( node->max_count != -1.0f && (child->work_num + child->active_num) < node->max_count )
				{
					AMS_VECTOR position, velocity, direction;
					AMS_AME_CREATE_PARAM param;

					amVectorSet( &direction, 0.0f,1.0f,0.0f );
					{
						float s, c, r;
						Angle32 a;

						r = work->radius;
						a = (Angle32)(nnRandom() * 10000000);
						if ( (node->flag & AMD_AME_FLAG_CIRLCE_EDGE_ONLY) == 0 )
						{
							r *= nnRandom();
						}
						else if ( node->flag & AMD_AME_FLAG_CIRLCE_EQUALLY )
						{
							Sint32 max_count;
							amFloatToInt( &max_count, node->max_count );
							a = (0xffff / max_count) * (child->count % max_count);
						}

						amSinCos( a, &s, &c );
						amVectorSet( &position, s*r, 0.0f, c*r );

						// spread
						{
							AMS_VECTOR cross;
							AMS_QUAT rot;

							amVectorOuterProduct( &cross, &direction, &position );
							amVectorUnit( &cross );

							amQuatRotAxisToQuat( &rot, &cross, NNM_DEGtoRAD( work->spread ) );
							amQuatMultiVector( &direction, &direction, &rot, NULL );
						}
					}
#if AMD_AME_ROTATE_PRECALC
					amMatrixCalcVector( &position, &position );
					amMatrixCalcVector( &direction, &direction );
#else // AMD_AME_ROTATE_PRECALC
					{
						amSaveMatrix( &_am_unit_matrix );
#if AMD_AME_ROTATE_QUAT
						amQuatToMatrix( NULL, &( work->rotate ), NULL );
#else // AMD_AME_ROTATE_QUAT
						amRotateXYZ( work->rotate.x, work->rotate.y, work->rotate.z );
#endif // !AMD_AME_ROTATE_QUAT
						amCalcVector( &position, &position );
						amCalcVector( &direction, &direction );
						amLoadMatrix();
					}
#endif // !AMD_AME_ROTATE_PRECALC

					// offset
					amVectorScale( &velocity, &direction,
						work->offset + work->offset_chaos * nnRandom() );
					amVectorAdd( &position, &velocity );

					// speed
					amVectorScale( &velocity, &direction,
						node->speed + node->speed_chaos * nnRandom() );

					param.ecb				= runtime->ecb;
					param.runtime			= child;
					param.node				= child->node;
					param.parent_position	= &( work->position );
					param.parent_velocity	= &( work->velocity );
					param.position			= &position;
					param.velocity			= &velocity;

					switch ( AMD_AME_NODE_TYPE( child->node ) & AME_AME_SUPER_CLASS_ID_MASK )
					{
					case AME_AME_NODE_TYPE_PARTICLE:	_amCreateParticle( &param ); break;
					case AME_AME_NODE_TYPE_EMITTER:		_amCreateEmitter( &param ); break;
					}
				}
			}
		}
	}

#if AMD_AME_ROTATE_PRECALC
	amMatrixPop();
#endif // AMD_AME_ROTATE_PRECALC
	
	return 0;
}

//******************************************************************************
//	_amDrawCircle()
//------------------------------------------------------------------------------
//	[Function]
//		円エミッタの描画
//	[Output]
//		runtime : ランタイムへのポインタ
//	[Comment]
//		リリース時は無効になります
//******************************************************************************
void _amDrawCircle(AMS_AME_RUNTIME* runtime)
{
	UNREFERENCED_PARAMETER(runtime);
#if 0
#if AMD_DEBUG
	AMS_AME_NODE_CIRCLE*			node = (AMS_AME_NODE_CIRCLE*)runtime->node;
	AMS_AME_RUNTIME_WORK_CIRCLE*	work = (AMS_AME_RUNTIME_WORK_CIRCLE*)runtime->work;

	Quad128 col = { 1.0f,1.0f,1.0f,1.0f };

	amSaveMatrix( NULL );
#if AMD_AME_ROTATE_QUAT
	amMultiQuatMatrix( &( work->rotate ), &( work->position ) );
#else
	amTranslate( &( work->position ) );
	amRotateXYZ( (float*)&( work->rotate ) );
#endif

	esuDrawCircle( node->radius, &col );
	esuDrawAxis( node->radius * 0.5f );

	amLoadMatrix();
#endif
#endif
}
//{### ユーザーエフェクト関数（シンプルスプライト）
//******************************************************************************
//	_amInitSimpleSprite()
//------------------------------------------------------------------------------
//	[Function]
//		シンプルスプライトの初期化
//	[Output]
//		param	: 作成パラメータへのポインタ
//	[Comment]
//		注意事項など
//******************************************************************************
void _amInitSimpleSprite(AMS_AME_CREATE_PARAM* param)
{
	AMS_AME_NODE_SPRITE*				node = (AMS_AME_NODE_SPRITE*)param->node;
	AMS_AME_RUNTIME_WORK_SIMPLE_SPRITE*	work = (AMS_AME_RUNTIME_WORK_SIMPLE_SPRITE*)param->work;

	work->time = -node->start_time;

	// color
	work->color.color = node->color_start.color;
	work->color.a =	(Uint8)((work->color.a * param->ecb->transparency) >> 8);

	// position & velocity
	{
		amVectorAdd( &( work->position ), param->parent_position, param->position );
		amVectorAdd( &( work->position ), &( node->translate ) );

		amVectorScale( &( work->velocity ), param->parent_velocity, node->inheritance_rate );
		amVectorAdd( &( work->velocity ), param->velocity );
	}

	// size
	{
		float size = node->size + node->size_chaos * nnRandom();

		work->size.x = size * node->scale_x_start;
		work->size.y = size * node->scale_y_start;
		work->size.z = size;
		work->size.w = 0.0f;
	}

	// texture animation
	if ( node->flag & AMD_AME_FLAG_TEXTURE_ANIMATION )
	{
		AMS_AME_TEX_ANIM_KEY* key;

		work->tex_time	= 0.0f;
		work->tex_no	= 0;

		// random key
		if ( node->flag & AMD_AME_FLAG_TEXTURE_RANDOM_KEY )
		{
			work->tex_no = (Sint32)(100 * nnRandom()) % node->tex_anim.key_num;
		}

		key = &( node->tex_anim.key_buf[ work->tex_no ] );
		work->st.x		= key->l;
		work->st.y		= key->t;
		work->st.z		= key->r;
		work->st.w		= key->b;
	}

	// texture cropping
	else if ( node->flag & AMD_AME_FLAG_TEXTURE_CROPPING )
	{
		work->st.x = node->cropping_l;
		work->st.y = node->cropping_t;
		work->st.z = node->cropping_r;
		work->st.w = node->cropping_b;
	}

	else
	{
		amVectorSet( (AMS_VECTOR*)&( work->st ), 0.0f,0.0f,1.0f );
	}

	// flip
	if ( (node->flag & AMD_AME_FLAG_TEXTURE_FLIP_U_FIXED) ||
		((node->flag & AMD_AME_FLAG_TEXTURE_FLIP_U_RANDOM) && (nnRandom() > 0.5f)) )
	{
		work->flag |= AMD_AME_RWFLAG_TEXTURE_FLIP_U;

		float tmp = work->st.x;
		work->st.x = work->st.z;
		work->st.z = tmp;
	}
	if ( (node->flag & AMD_AME_FLAG_TEXTURE_FLIP_V_FIXED) ||
		((node->flag & AMD_AME_FLAG_TEXTURE_FLIP_V_RANDOM) && (nnRandom() > 0.5f)) )
	{
		work->flag |= AMD_AME_RWFLAG_TEXTURE_FLIP_V;

		float tmp = work->st.y;
		work->st.y = work->st.w;
		work->st.w = tmp;
	}
}

//******************************************************************************
//	_amUpdateSimpleSprite()
//------------------------------------------------------------------------------
//	[Function]
//		シンプルスプライトの更新
//	[Output]
//		runtime : ランタイムへのポインタ
//	[Return]
//		1	: 削除要求
//	[Comment]
//		注意事項など
//******************************************************************************
Sint32 _amUpdateSimpleSprite(AMS_AME_RUNTIME* runtime)
{
	AMS_AME_NODE_SPRITE*				node = (AMS_AME_NODE_SPRITE*)runtime->node;
	AMS_AME_RUNTIME_WORK_SIMPLE_SPRITE*	work = (AMS_AME_RUNTIME_WORK_SIMPLE_SPRITE*)runtime->active_head.next;
	AMS_AME_RUNTIME_WORK_SIMPLE_SPRITE*	tail = (AMS_AME_RUNTIME_WORK_SIMPLE_SPRITE*)&( runtime->active_tail );

	amAssert( AMD_AME_IS_PARTICLE( node ) );

	Sint32 trans = runtime->ecb->transparency;

	float life, lifeInv;
	if ( node->life >= 0.0f ) {
		life	= node->life;
		lifeInv	= 1.0f / life;
	}
	else {
		life	= FLT_MAX;//3.40282347e+38F/*FLT_MAX*/;
		lifeInv	= 0.0f;
	}

	float sx, sy, ex, ey;
	{
		float rate = runtime->ecb->size_rate;
		sx = node->scale_x_start * rate;
		sy = node->scale_y_start * rate;
		ex = node->scale_x_end * rate;
		ey = node->scale_y_end * rate;
	}

	for ( ; work != tail; work = (AMS_AME_RUNTIME_WORK_SIMPLE_SPRITE*)work->next )
	{
		work->time += _am_unit_frame;

		float rate = work->time * lifeInv;

		// position & velocity
		{
			AMS_VECTOR vel;
			amVectorScale( &vel, &( work->velocity ), _am_unit_time );
			amVectorAdd( &( work->position ), &vel );
		}

		// life
		if ( work->time >= life )
		{
			// create spawn particle
			if ( runtime->spawn_runtime )
				_amCreateSpawnParticle( runtime, (AMS_AME_RUNTIME_WORK*)work );

			// delete particle
			amEffectDisconnectLink( (AMS_AME_LIST*)work );
			runtime->active_num--;
			amEffectFreeRuntimeWork( (AMS_AME_RUNTIME_WORK*)work );

			continue;
		}

		// scale
		{
			float rateInv = 1.0f - rate;
			float x = sx * rateInv + ex * rate;
			float y = sy * rateInv + ey * rate;
			work->size.x = work->size.z * x;
			work->size.y = work->size.z * y;
		}

		// color
		amEffectLerpColor( &( work->color ), &( node->color_start ), &( node->color_end ), rate );

		// global transaprency
		work->color.a =	(Uint8)((work->color.a * trans) >> 8);

		// texture animation
		if ( node->flag & AMD_AME_FLAG_TEXTURE_ANIMATION )
		{
			AMS_AME_TEX_ANIM*		anim = &( node->tex_anim );
			AMS_AME_TEX_ANIM_KEY*	key;

			if ( (work->flag & AMD_AME_RWFLAG_SKIP_ANIMATION) == 0 )
			{
				work->tex_time += _am_unit_frame;

				if ( work->tex_time >= anim->key_buf[ work->tex_no ].time )
				{
					work->tex_time	= 0.0f;
					work->tex_no	+= 1;

					if ( work->tex_no == anim->key_num )
					{
						if ( node->flag & AMD_AME_FLAG_TEXTURE_LOOP )
						{
							work->tex_no = 0;
						}
						else
						{
							work->tex_no = anim->key_num - 1;
							work->flag |= AMD_AME_RWFLAG_SKIP_ANIMATION;
						}
					}
				}
			}

			key = &( anim->key_buf[ work->tex_no ] );
			work->st.x = key->l;
			work->st.y = key->t;
			work->st.z = key->r;
			work->st.w = key->b;

			// flip
			if ( work->flag & AMD_AME_RWFLAG_TEXTURE_FLIP_U )
			{
				float tmp = work->st.x;
				work->st.x = work->st.z;
				work->st.z = tmp;
			}
			if ( work->flag & AMD_AME_RWFLAG_TEXTURE_FLIP_V )
			{
				float tmp = work->st.y;
				work->st.y = work->st.w;
				work->st.w = tmp;
			}
		}

		// uv scroll
		else if ( node->flag & AMD_AME_FLAG_TEXTURE_UV_SCROLL )
		{
			float u, v;

			u = node->scroll_u * _am_unit_time;
			v = node->scroll_v * _am_unit_time;

			// flip
			if ( work->flag & AMD_AME_RWFLAG_TEXTURE_FLIP_U ) u = -u;
			if ( work->flag & AMD_AME_RWFLAG_TEXTURE_FLIP_V ) v = -v;

			work->st.x += u;
			work->st.y += v;
			work->st.z += u;
			work->st.w += v;
		}
	}

	return 0;
}

//******************************************************************************
//	_amDrawSimpleSprite()
//------------------------------------------------------------------------------
//	[Function]
//		シンプルスプライトの描画
//	[Output]
//		runtime : ランタイムへのポインタ
//	[Comment]
//		回転は出来ません
//******************************************************************************
void _amDrawSimpleSprite(AMS_AME_RUNTIME* runtime)
{
	AMS_AME_NODE_SPRITE*				node = (AMS_AME_NODE_SPRITE*)runtime->node;
	AMS_AME_RUNTIME_WORK_SIMPLE_SPRITE*	work = (AMS_AME_RUNTIME_WORK_SIMPLE_SPRITE*)runtime->active_head.next;
	AMS_AME_RUNTIME_WORK_SIMPLE_SPRITE*	tail = (AMS_AME_RUNTIME_WORK_SIMPLE_SPRITE*)&( runtime->active_tail );

	// 各種描画設定
	AMS_PARAM_DRAW_PRIMITIVE param;
	memset(&param, 0, sizeof(AMS_PARAM_DRAW_PRIMITIVE));
	NNE_PRIM_ALPHABLEND blend = _amEffectSetDrawMode(runtime, &param, node->blend);

	// ビルボード
	NNS_VECTOR offset, up, right;
	float zBias = node->z_bias;
	amVectorSet(&offset, zBias * NNM_MTX(_am_ef_worldViewMtx, 2, 0), zBias * NNM_MTX(_am_ef_worldViewMtx, 2, 1), zBias * NNM_MTX(_am_ef_worldViewMtx, 2, 2));
	amVectorSet(&up, NNM_MTX(_am_ef_worldViewMtx, 1, 0), NNM_MTX(_am_ef_worldViewMtx, 1, 1), NNM_MTX(_am_ef_worldViewMtx, 1, 2));
	amVectorSet(&right, NNM_MTX(_am_ef_worldViewMtx, 0, 0), NNM_MTX(_am_ef_worldViewMtx, 0, 1), NNM_MTX(_am_ef_worldViewMtx, 0, 2));

	// テクスチャあり
	if ( node->flag & AMD_AME_FLAG_TEXTURE )
	{
		NNS_PRIM3D_PCT* pv = (NNS_PRIM3D_PCT *)amDrawMallocDataBuffer(sizeof(NNS_PRIM3D_PCT) * 6 * runtime->active_num);
		NNS_PRIM3D_PCT* head = pv; // 先頭保存
		float len = 0.0f;

		for ( ; work != tail; work = (AMS_AME_RUNTIME_WORK_SIMPLE_SPRITE*)work->next )
		{
			NNS_VECTOR pos, x, y;

#if _IPHONE
			nnScaleVector( &x, &right, work->size.y);
			nnScaleVector( &y, &up, work->size.x);
#else
			nnScaleVector( &x, &right, work->size.x );
			nnScaleVector( &y, &up, work->size.y );
#endif
			amVectorAdd( &pos, &( work->position ), &offset );

			// カメラからの距離
			len = nnDistanceVector(&pos, &_am_ef_camPos);

#if _IPHONE
			// 頂点の並びは以下のとおり（三角形が二つで四角形を形成）
			// iPhoneは強制的に回転させておく
			//  4  2ー0
			//  |＼ ＼|
			//  5－3  1
			nnSubtractVector( &pv[2].Pos, &pos, &x );
			nnAddVector( &pv[2].Pos, &pv[2].Pos, &y ); // 左上
			nnAddVector( &pv[0].Pos, &pos, &x );
			nnAddVector( &pv[0].Pos, &pv[0].Pos, &y ); // 右上
			nnSubtractVector( &pv[5].Pos, &pos, &x );
			nnSubtractVector( &pv[5].Pos, &pv[5].Pos, &y ); // 左下
			nnAddVector( &pv[1].Pos, &pos, &x );
			nnSubtractVector( &pv[1].Pos, &pv[1].Pos, &y ); // 右下
#else
			// 頂点の並びは以下のとおり（三角形が二つで四角形を形成）
			//  0-1  3
			//  |/  /|
			//  2  4-5
			nnSubtractVector( &pv[0].Pos, &pos, &x );
			nnAddVector( &pv[0].Pos, &pv[0].Pos, &y ); // 左上
			nnAddVector( &pv[1].Pos, &pos, &x );
			nnAddVector( &pv[1].Pos, &pv[1].Pos, &y ); // 右上
			nnSubtractVector( &pv[2].Pos, &pos, &x );
			nnSubtractVector( &pv[2].Pos, &pv[2].Pos, &y ); // 左下
			nnAddVector( &pv[5].Pos, &pos, &x );
			nnSubtractVector( &pv[5].Pos, &pv[5].Pos, &y ); // 右下
#endif // _IPHONE
			
			// Color
			pv[5].Col = AMD_RGBA8888(work->color.r, work->color.g, work->color.b, work->color.a);
			pv[0].Col = pv[1].Col = pv[2].Col  = pv[5].Col;

			// Texture
			pv[0].Tex.u = work->st.x; pv[0].Tex.v = work->st.y;
			pv[1].Tex.u = work->st.z; pv[1].Tex.v = work->st.y;
			pv[2].Tex.u = work->st.x; pv[2].Tex.v = work->st.w;
			pv[5].Tex.u = work->st.z; pv[5].Tex.v = work->st.w;

			pv[3] = pv[1];
			pv[4] = pv[2];

			pv += 6;
		}

		// プリミティブ描画設定
		param.format3D = NNE_PRIM3D_FMT_PCT;
		param.type = NNE_PRIM_TRIANGLE_LIST;
		param.vtxPCT3D = head;
		param.texlist = runtime->texlist;
		param.texId = node->texture_id;
		param.count = 6 * runtime->active_num;
		param.ablend = blend;
		param.sortZ = len;
		amDrawPrimitive3D(runtime->ecb->drawState, &param);
	}
	// テクスチャなし
	else
	{
		NNS_PRIM3D_PC* pv = (NNS_PRIM3D_PC *)amDrawMallocWorkBuffer(sizeof(NNS_PRIM3D_PC) * 6 * runtime->active_num);
		NNS_PRIM3D_PC* head = pv; // 先頭保存
		float len = 0.0f;

		for ( ; work != tail; work = (AMS_AME_RUNTIME_WORK_SIMPLE_SPRITE*)work->next )
		{
			NNS_VECTOR pos, x, y;
			
			nnScaleVector( &x, &right, work->size.x );
			nnScaleVector( &y, &up, work->size.y );
			amVectorAdd( &pos, &( work->position ), &offset );

			// カメラからの距離
			len = nnDistanceVector(&pos, &_am_ef_camPos);
			
#if _IPHONE
			// 頂点の並びは以下のとおり（三角形が二つで四角形を形成）
			// iPhoneは強制的に回転させておく
			//  4  2ー0
			//  |＼ ＼|
			//  5－3  1
			nnSubtractVector( &pv[2].Pos, &pos, &x );
			nnAddVector( &pv[2].Pos, &pv[2].Pos, &y ); // 左上
			nnAddVector( &pv[0].Pos, &pos, &x );
			nnAddVector( &pv[0].Pos, &pv[0].Pos, &y ); // 右上
			nnSubtractVector( &pv[5].Pos, &pos, &x );
			nnSubtractVector( &pv[5].Pos, &pv[5].Pos, &y ); // 左下
			nnAddVector( &pv[1].Pos, &pos, &x );
			nnSubtractVector( &pv[1].Pos, &pv[1].Pos, &y ); // 右下
#else
			// 頂点の並びは以下のとおり（三角形が二つで四角形を形成）
			//  0-1  3
			//  |/  /|
			//  2  4-5
			nnSubtractVector( &pv[0].Pos, &pos, &x );
			nnAddVector( &pv[0].Pos, &pv[0].Pos, &y ); // 左上
			nnAddVector( &pv[1].Pos, &pos, &x );
			nnAddVector( &pv[1].Pos, &pv[1].Pos, &y ); // 右上
			nnSubtractVector( &pv[2].Pos, &pos, &x );
			nnSubtractVector( &pv[2].Pos, &pv[2].Pos, &y ); // 左下
			nnAddVector( &pv[5].Pos, &pos, &x );
			nnSubtractVector( &pv[5].Pos, &pv[5].Pos, &y ); // 右下
#endif // _IPHONE

			// Color
			pv[5].Col = AMD_RGBA8888(work->color.r, work->color.g, work->color.b, work->color.a);
			pv[0].Col = pv[1].Col = pv[2].Col = pv[5].Col;

			pv[3] = pv[1];
			pv[4] = pv[2];

			pv += 6;
		}

		// プリミティブ描画設定
		param.format3D = NNE_PRIM3D_FMT_PC;
		param.type = NNE_PRIM_TRIANGLE_LIST;
		param.vtxPC3D = head;
		param.texlist = runtime->texlist;
		param.texId = -1;
		param.count = 6 * runtime->active_num;
		param.ablend = blend;
		param.sortZ = len;
		amDrawPrimitive3D(runtime->ecb->drawState, &param);
	}
}

//{### ユーザーエフェクト関数（スプライト）
//******************************************************************************
//	_amInitSprite()
//------------------------------------------------------------------------------
//	[Function]
//		スプライトの初期化
//	[Output]
//		param	: 作成パラメータへのポインタ
//	[Comment]
//		注意事項など
//******************************************************************************
void _amInitSprite(AMS_AME_CREATE_PARAM* param)
{
	AMS_AME_NODE_SPRITE*			node = (AMS_AME_NODE_SPRITE*)param->node;
	AMS_AME_RUNTIME_WORK_SPRITE*	work = (AMS_AME_RUNTIME_WORK_SPRITE*)param->work;

	work->time = -node->start_time;

	// color
	work->color.color = node->color_start.color;
	work->color.a =	(Uint8)((work->color.a * param->ecb->transparency) >> 8);

	// position & velocity
	{
		amVectorAdd( &( work->position ), param->parent_position, param->position );
		amVectorAdd( &( work->position ), &( node->translate ) );

		amVectorScale( &( work->velocity ), param->parent_velocity, node->inheritance_rate );
		amVectorAdd( &( work->velocity ), param->velocity );
	}

	// size
	{
		float size = node->size + node->size_chaos * nnRandom();

		work->size.x = size * node->scale_x_start;
		work->size.y = size * node->scale_y_start;
		work->size.z = size;
		work->size.w = 0.0f;
	}

	// twist
	{
		work->twist =
			node->twist_angle + node->twist_angle_chaos * nnRandom();

		// random reverse
		if ( node->flag & AMD_AME_FLAG_SPRITE_RANDOM_TWIST_REVERSE )
		{
			if ( nnRandom() > 0.5f )
				work->flag |= AMD_AME_RWFLAG_TWIST_REVERSE;
		}

		if ( work->flag & AMD_AME_RWFLAG_TWIST_REVERSE )
			work->twist_speed = -node->twist_angle_speed;
		else
			work->twist_speed = node->twist_angle_speed;
	}

	// texture animation
	if ( node->flag & AMD_AME_FLAG_TEXTURE_ANIMATION )
	{
		AMS_AME_TEX_ANIM_KEY* key;

		work->tex_time	= 0.0f;
		work->tex_no	= 0;

		// random key
		if ( node->flag & AMD_AME_FLAG_TEXTURE_RANDOM_KEY )
		{
			work->tex_no = (Sint32)(100 * nnRandom()) % node->tex_anim.key_num;
		}

		key = &( node->tex_anim.key_buf[ work->tex_no ] );
		work->st.x		= key->l;
		work->st.y		= key->t;
		work->st.z		= key->r;
		work->st.w		= key->b;
	}

	// texture cropping
	else if ( node->flag & AMD_AME_FLAG_TEXTURE_CROPPING )
	{
		work->st.x = node->cropping_l;
		work->st.y = node->cropping_t;
		work->st.z = node->cropping_r;
		work->st.w = node->cropping_b;
	}

	else
	{
		amVectorSet( (AMS_VECTOR*)&( work->st ), 0.0f,0.0f,1.0f );
	}

	// flip
	if ( (node->flag & AMD_AME_FLAG_TEXTURE_FLIP_U_FIXED) ||
		((node->flag & AMD_AME_FLAG_TEXTURE_FLIP_U_RANDOM) && (nnRandom() > 0.5f)) )
	{
		work->flag |= AMD_AME_RWFLAG_TEXTURE_FLIP_U;

		float tmp = work->st.x;
		work->st.x = work->st.z;
		work->st.z = tmp;
	}
	if ( (node->flag & AMD_AME_FLAG_TEXTURE_FLIP_V_FIXED) ||
		((node->flag & AMD_AME_FLAG_TEXTURE_FLIP_V_RANDOM) && (nnRandom() > 0.5f)) )
	{
		work->flag |= AMD_AME_RWFLAG_TEXTURE_FLIP_V;

		float tmp = work->st.y;
		work->st.y = work->st.w;
		work->st.w = tmp;
	}
}

//******************************************************************************
//	_amUpdateSprite()
//------------------------------------------------------------------------------
//	[Function]
//		スプライトの更新
//	[Output]
//		runtime : ランタイムへのポインタ
//	[Return]
//		1	: 削除要求
//	[Comment]
//		注意事項など
//******************************************************************************
Sint32 _amUpdateSprite(AMS_AME_RUNTIME* runtime)
{
	AMS_AME_NODE_SPRITE*			node = (AMS_AME_NODE_SPRITE*)runtime->node;
	AMS_AME_RUNTIME_WORK_SPRITE*	work = (AMS_AME_RUNTIME_WORK_SPRITE*)runtime->active_head.next;
	AMS_AME_RUNTIME_WORK_SPRITE*	tail = (AMS_AME_RUNTIME_WORK_SPRITE*)&( runtime->active_tail );

	amAssert( AMD_AME_IS_PARTICLE( node ) );

	Sint32 trans = runtime->ecb->transparency;

	float life, lifeInv;
	if ( node->life >= 0.0f ) {
		life	= node->life;
		lifeInv	= 1.0f / life;
	}
	else {
		life	= FLT_MAX;//3.40282347e+38F/*FLT_MAX*/;
		lifeInv	= 0.0f;
	}

	float sx, sy, ex, ey;
	{
		float rate = runtime->ecb->size_rate;
		sx = node->scale_x_start * rate;
		sy = node->scale_y_start * rate;
		ex = node->scale_x_end * rate;
		ey = node->scale_y_end * rate;
	}

	for ( ; work != tail; work = (AMS_AME_RUNTIME_WORK_SPRITE*)work->next )
	{
		work->time += _am_unit_frame;

		float rate = work->time * lifeInv;

		// position & velocity
		{
			AMS_VECTOR vel;
			amVectorScale( &vel, &( work->velocity ), _am_unit_time );
			amVectorAdd( &( work->position ), &vel );
		}

		// life
		if ( work->time >= life )
		{
			// create spawn particle
			if ( runtime->spawn_runtime )
				_amCreateSpawnParticle( runtime, (AMS_AME_RUNTIME_WORK*)work );

			// delete particle
			amEffectDisconnectLink( (AMS_AME_LIST*)work );
			runtime->active_num--;
			amEffectFreeRuntimeWork( (AMS_AME_RUNTIME_WORK*)work );

			continue;
		}

		// scale
		{
			float rateInv = 1.0f - rate;
			float x = sx * rateInv + ex * rate;
			float y = sy * rateInv + ey * rate;
			work->size.x = work->size.z * x;
			work->size.y = work->size.z * y;
		}

		// twist
		work->twist += work->twist_speed * _am_unit_time;

		// color
		amEffectLerpColor( &( work->color ), 
			&( node->color_start ), &( node->color_end ), rate );

		// global transaprency
		work->color.a =	(Uint8)((work->color.a * trans) >> 8);

		// texture animation
		if ( node->flag & AMD_AME_FLAG_TEXTURE_ANIMATION )
		{
			AMS_AME_TEX_ANIM*		anim = &( node->tex_anim );
			AMS_AME_TEX_ANIM_KEY*	key;

			if ( (work->flag & AMD_AME_RWFLAG_SKIP_ANIMATION) == 0 )
			{
				work->tex_time += _am_unit_frame;

				if ( work->tex_time >= anim->key_buf[ work->tex_no ].time )
				{
					work->tex_time	= 0.0f;
					work->tex_no	+= 1;

					if ( work->tex_no == anim->key_num )
					{
						if ( node->flag & AMD_AME_FLAG_TEXTURE_LOOP )
						{
							work->tex_no = 0;
						}
						else
						{
							work->tex_no = anim->key_num - 1;
							work->flag |= AMD_AME_RWFLAG_SKIP_ANIMATION;
						}
					}
				}
			}

			key = &( anim->key_buf[ work->tex_no ] );
			work->st.x = key->l;
			work->st.y = key->t;
			work->st.z = key->r;
			work->st.w = key->b;

			// flip
			if ( work->flag & AMD_AME_RWFLAG_TEXTURE_FLIP_U )
			{
				float tmp = work->st.x;
				work->st.x = work->st.z;
				work->st.z = tmp;
			}
			if ( work->flag & AMD_AME_RWFLAG_TEXTURE_FLIP_V )
			{
				float tmp = work->st.y;
				work->st.y = work->st.w;
				work->st.w = tmp;
			}
		}

		// uv scroll
		else if ( node->flag & AMD_AME_FLAG_TEXTURE_UV_SCROLL )
		{
			float u, v;

			u = node->scroll_u * _am_unit_time;
			v = node->scroll_v * _am_unit_time;

			// flip
			if ( work->flag & AMD_AME_RWFLAG_TEXTURE_FLIP_U ) u = -u;
			if ( work->flag & AMD_AME_RWFLAG_TEXTURE_FLIP_V ) v = -v;

			work->st.x += u;
			work->st.y += v;
			work->st.z += u;
			work->st.w += v;
		}
	}

	return 0;
}

//******************************************************************************
//	_amDrawSprite()
//------------------------------------------------------------------------------
//	[Function]
//		スプライトの描画
//	[Output]
//		runtime : ランタイムへのポインタ
//	[Comment]
//		注意事項など
//******************************************************************************
void _amDrawSprite(AMS_AME_RUNTIME* runtime)
{
	AMS_AME_NODE_SPRITE*			node = (AMS_AME_NODE_SPRITE*)runtime->node;
	AMS_AME_RUNTIME_WORK_SPRITE*	work = (AMS_AME_RUNTIME_WORK_SPRITE*)runtime->active_head.next;
	AMS_AME_RUNTIME_WORK_SPRITE*	tail = (AMS_AME_RUNTIME_WORK_SPRITE*)&( runtime->active_tail );

	// 各種描画設定
	AMS_PARAM_DRAW_PRIMITIVE param;
	memset(&param, 0, sizeof(AMS_PARAM_DRAW_PRIMITIVE));
	NNE_PRIM_ALPHABLEND blend = _amEffectSetDrawMode(runtime, &param, node->blend);

	// ビルボード
	NNS_VECTOR offset, up, right;
	float zBias = node->z_bias;
	amVectorSet(&offset, zBias * NNM_MTX(_am_ef_worldViewMtx, 2, 0), zBias * NNM_MTX(_am_ef_worldViewMtx, 2, 1), zBias * NNM_MTX(_am_ef_worldViewMtx, 2, 2));
	amVectorSet(&up, NNM_MTX(_am_ef_worldViewMtx, 1, 0), NNM_MTX(_am_ef_worldViewMtx, 1, 1), NNM_MTX(_am_ef_worldViewMtx, 1, 2));
	amVectorSet(&right, NNM_MTX(_am_ef_worldViewMtx, 0, 0), NNM_MTX(_am_ef_worldViewMtx, 0, 1), NNM_MTX(_am_ef_worldViewMtx, 0, 2));

	// テクスチャあり
	if ( node->flag & AMD_AME_FLAG_TEXTURE )
	{
		// 頂点データ格納領域取得
		NNS_PRIM3D_PCT* pv = (NNS_PRIM3D_PCT *)amDrawMallocDataBuffer(sizeof(NNS_PRIM3D_PCT) * 6 * runtime->active_num);
		NNS_PRIM3D_PCT* head = pv; // 先頭保存
		float len = 0.0f;

		for ( ; work != tail; work = (AMS_AME_RUNTIME_WORK_SPRITE*)work->next )
		{
			NNS_VECTOR pos, x, y;
			float s, c;

			// 回転、拡縮計算（simple splite とは違う部分）
			amSinCos( work->twist, &s, &c );
			amVectorGetAverage( &x, &right, &up, c, -s );
			amVectorGetAverage( &y, &right, &up, s, c );

			nnScaleVector( &x, &x, work->size.x );
			nnScaleVector( &y, &y, work->size.y );
			amVectorAdd( &pos, &( work->position ), &offset );

			// カメラからの距離
			len = nnDistanceVector(&pos, &_am_ef_camPos);

			// 頂点の並びは以下のとおり（三角形が二つで四角形を形成）
			//  0-1  3
			//  |/  /|
			//  2  4-5
			nnSubtractVector( &pv[0].Pos, &pos, &x );
			nnAddVector( &pv[0].Pos, &pv[0].Pos, &y ); // 左上
			nnAddVector( &pv[1].Pos, &pos, &x );
			nnAddVector( &pv[1].Pos, &pv[1].Pos, &y ); // 右上
			nnSubtractVector( &pv[2].Pos, &pos, &x );
			nnSubtractVector( &pv[2].Pos, &pv[2].Pos, &y ); // 左下
			nnAddVector( &pv[5].Pos, &pos, &x );
			nnSubtractVector( &pv[5].Pos, &pv[5].Pos, &y ); // 右下

			// Color
			pv[5].Col = AMD_RGBA8888(work->color.r, work->color.g, work->color.b, work->color.a);
			pv[0].Col = pv[1].Col = pv[2].Col = pv[5].Col;

			// Texture
			pv[0].Tex.u = work->st.x; pv[0].Tex.v = work->st.y;
			pv[1].Tex.u = work->st.z; pv[1].Tex.v = work->st.y;
			pv[2].Tex.u = work->st.x; pv[2].Tex.v = work->st.w;
			pv[5].Tex.u = work->st.z; pv[5].Tex.v = work->st.w;

			pv[3] = pv[1];
			pv[4] = pv[2];

			pv += 6;
		}

		// プリミティブ描画設定
		param.format3D = NNE_PRIM3D_FMT_PCT;
		param.type = NNE_PRIM_TRIANGLE_LIST;
		param.vtxPCT3D = head;
		param.texlist = runtime->texlist;
		param.texId = node->texture_id;
		param.count = 6 * runtime->active_num;
		param.ablend = blend;
		param.sortZ = len;
		amDrawPrimitive3D(runtime->ecb->drawState, &param);
	}
	// テクスチャなし
	else
	{
		// 頂点データ格納領域取得
		NNS_PRIM3D_PC* pv = (NNS_PRIM3D_PC *)amDrawMallocWorkBuffer(sizeof(NNS_PRIM3D_PC) * 6 * runtime->active_num);
		NNS_PRIM3D_PC* head = pv; // 先頭保存
		float len = 0.0f;

		for ( ; work != tail; work = (AMS_AME_RUNTIME_WORK_SPRITE*)work->next )
		{
			NNS_VECTOR pos, x, y;
			float s, c;

			// 回転、拡縮計算（simple splite とは違う部分）
			amSinCos( work->twist, &s, &c );
			amVectorGetAverage( &x, &right, &up, c, -s );
			amVectorGetAverage( &y, &right, &up, s, c );

			nnScaleVector( &x, &x, work->size.x );
			nnScaleVector( &y, &y, work->size.y );
			amVectorAdd( &pos, &( work->position ), &offset );

			// カメラからの距離
			len = nnDistanceVector(&pos, &_am_ef_camPos);

			// 頂点の並びは以下のとおり（三角形が二つで四角形を形成）
			//  0-1  3
			//  |/  /|
			//  2  4-5
			nnSubtractVector( &pv[0].Pos, &pos, &x );
			nnAddVector( &pv[0].Pos, &pv[0].Pos, &y ); // 左上
			nnAddVector( &pv[1].Pos, &pos, &x );
			nnAddVector( &pv[1].Pos, &pv[1].Pos, &y ); // 右上
			nnSubtractVector( &pv[2].Pos, &pos, &x );
			nnSubtractVector( &pv[2].Pos, &pv[2].Pos, &y ); // 左下
			nnAddVector( &pv[5].Pos, &pos, &x );
			nnSubtractVector( &pv[5].Pos, &pv[5].Pos, &y ); // 右下

			// Color
			pv[5].Col = AMD_RGBA8888(work->color.r, work->color.g, work->color.b, work->color.a);
			pv[0].Col = pv[1].Col = pv[2].Col = pv[5].Col;

			pv[3] = pv[1];
			pv[4] = pv[2];

			pv += 6;
		}

		// プリミティブ描画設定
		param.format3D = NNE_PRIM3D_FMT_PC;
		param.type = NNE_PRIM_TRIANGLE_LIST;
		param.vtxPC3D = head;
		param.texlist = runtime->texlist;
		param.texId = -1;
		param.count = 6 * runtime->active_num;
		param.ablend = blend;
		param.sortZ = len;
		amDrawPrimitive3D(runtime->ecb->drawState, &param);
	}
}

//{### ユーザーエフェクト関数（ライン）
//******************************************************************************
//	_amInitLine()
//------------------------------------------------------------------------------
//	[Function]
//		ラインの初期化
//	[Output]
//		param	: 作成パラメータへのポインタ
//	[Comment]
//		注意事項など
//******************************************************************************
void _amInitLine(AMS_AME_CREATE_PARAM* param)
{
	AMS_AME_NODE_LINE*			node = (AMS_AME_NODE_LINE*)param->node;
	AMS_AME_RUNTIME_WORK_LINE*	work = (AMS_AME_RUNTIME_WORK_LINE*)param->work;

	work->time = -node->start_time;

	work->length		= node->length_start;
	work->inside_width	= node->inside_width_start;
	work->outside_width	= node->outside_width_start;

	// color
	work->inside_color.color = node->inside_color_start.color;
	work->inside_color.a =	(Uint8)((work->inside_color.a * param->ecb->transparency) >> 8);

	work->outside_color.color = node->outside_color_start.color;
	work->outside_color.a =	(Uint8)((work->outside_color.a * param->ecb->transparency) >> 8);

	// position & velocity
	{
		amVectorAdd( &( work->position ), param->parent_position, param->position );
		amVectorAdd( &( work->position ), &( node->translate ) );

		amVectorScale( &( work->velocity ), param->parent_velocity, node->inheritance_rate );
		amVectorAdd( &( work->velocity ), param->velocity );
	}

	// texture animation
	if ( node->flag & AMD_AME_FLAG_TEXTURE_ANIMATION )
	{
		AMS_AME_TEX_ANIM_KEY* key;

		work->tex_time	= 0.0f;
		work->tex_no	= 0;

		// random key
		if ( node->flag & AMD_AME_FLAG_TEXTURE_RANDOM_KEY )
		{
			work->tex_no = (Sint32)(100 * nnRandom()) % node->tex_anim.key_num;
		}

		key = &( node->tex_anim.key_buf[ work->tex_no ] );
		work->st.x		= key->l;
		work->st.y		= key->t;
		work->st.z		= key->r;
		work->st.w		= key->b;
	}

	// texture cropping
	else if ( node->flag & AMD_AME_FLAG_TEXTURE_CROPPING )
	{
		work->st.x = node->cropping_l;
		work->st.y = node->cropping_t;
		work->st.z = node->cropping_r;
		work->st.w = node->cropping_b;
	}

	else
	{
		amVectorSet( (AMS_VECTOR*)&( work->st ), 0.0f,0.0f,1.0f );
	}

	// flip
	if ( (node->flag & AMD_AME_FLAG_TEXTURE_FLIP_U_FIXED) ||
		((node->flag & AMD_AME_FLAG_TEXTURE_FLIP_U_RANDOM) && (nnRandom() > 0.5f)) )
	{
		work->flag |= AMD_AME_RWFLAG_TEXTURE_FLIP_U;

		float tmp = work->st.x;
		work->st.x = work->st.z;
		work->st.z = tmp;
	}
	if ( (node->flag & AMD_AME_FLAG_TEXTURE_FLIP_V_FIXED) ||
		((node->flag & AMD_AME_FLAG_TEXTURE_FLIP_V_RANDOM) && (nnRandom() > 0.5f)) )
	{
		work->flag |= AMD_AME_RWFLAG_TEXTURE_FLIP_V;

		float tmp = work->st.y;
		work->st.y = work->st.w;
		work->st.w = tmp;
	}
}

//******************************************************************************
//	_amUpdateLine()
//------------------------------------------------------------------------------
//	[Function]
//		ラインの更新
//	[Output]
//		runtime : ランタイムへのポインタ
//	[Return]
//		1	: 削除要求
//	[Comment]
//		注意事項など
//******************************************************************************
Sint32 _amUpdateLine(AMS_AME_RUNTIME* runtime)
{
	AMS_AME_NODE_LINE*			node = (AMS_AME_NODE_LINE*)runtime->node;
	AMS_AME_RUNTIME_WORK_LINE*	work = (AMS_AME_RUNTIME_WORK_LINE*)runtime->active_head.next;
	AMS_AME_RUNTIME_WORK_LINE*	tail = (AMS_AME_RUNTIME_WORK_LINE*)&( runtime->active_tail );

	amAssert( AMD_AME_IS_PARTICLE( node ) );

	Sint32 trans = runtime->ecb->transparency;

	float life, lifeInv;
	if ( node->life >= 0.0f ) {
		life	= node->life;
		lifeInv	= 1.0f / life;
	}
	else {
		life	= FLT_MAX;//3.40282347e+38F/*FLT_MAX*/;
		life	= 1e+38F;

		lifeInv	= 0.0f;
	}

	float sl, el, siw, sow, eiw, eow;
	{
		float rate = runtime->ecb->size_rate;
		sl	= node->length_start * rate;
		el	= node->length_end * rate;
		siw	= node->inside_width_start * rate;
		sow	= node->outside_width_start * rate;
		eiw	= node->inside_width_end * rate;
		eow	= node->outside_width_end * rate;
	}

	for ( ; work != tail; work = (AMS_AME_RUNTIME_WORK_LINE*)work->next )
	{
		work->time += _am_unit_frame;

		float rate = work->time * lifeInv;

		// position & velocity
		{
			AMS_VECTOR vel;
			amVectorScale( &vel, &( work->velocity ), _am_unit_time );
			amVectorAdd( &( work->position ), &vel );
		}

		// life
		if ( work->time >= life )
		{
			// create spawn particle
			if ( runtime->spawn_runtime )
				_amCreateSpawnParticle( runtime, (AMS_AME_RUNTIME_WORK*)work );

			// delete particle
			amEffectDisconnectLink( (AMS_AME_LIST*)work );
			runtime->active_num--;
			amEffectFreeRuntimeWork( (AMS_AME_RUNTIME_WORK*)work );

			continue;
		}

		{
			float rateInv = 1.0f - rate;
			// length
			work->length		= sl * rateInv + el * rate;
			// width
			work->inside_width	= siw * rateInv + eiw * rate;
			work->outside_width	= sow * rateInv + eow * rate;
		}

		// color
		amEffectLerpColor( &( work->inside_color ),
				&( node->inside_color_start ), &( node->inside_color_end ), rate );
		amEffectLerpColor( &( work->outside_color ),
				&( node->outside_color_start ), &( node->outside_color_end ), rate );

		// global transaprency
		work->inside_color.a	= (Uint8)((work->inside_color.a * trans) >> 8);
		work->outside_color.a	= (Uint8)((work->outside_color.a * trans) >> 8);

		// texture animation
		if ( node->flag & AMD_AME_FLAG_TEXTURE_ANIMATION )
		{
			AMS_AME_TEX_ANIM*		anim = &( node->tex_anim );
			AMS_AME_TEX_ANIM_KEY*	key;

			if ( (work->flag & AMD_AME_RWFLAG_SKIP_ANIMATION) == 0 )
			{
				work->tex_time += _am_unit_frame;

				if ( work->tex_time >= anim->key_buf[ work->tex_no ].time )
				{
					work->tex_time	= 0.0f;
					work->tex_no	+= 1;

					if ( work->tex_no == anim->key_num )
					{
						if ( node->flag & AMD_AME_FLAG_TEXTURE_LOOP )
						{
							work->tex_no = 0;
						}
						else
						{
							work->tex_no = anim->key_num - 1;
							work->flag |= AMD_AME_RWFLAG_SKIP_ANIMATION;
						}
					}
				}
			}

			key = &( anim->key_buf[ work->tex_no ] );
			work->st.x = key->l;
			work->st.y = key->t;
			work->st.z = key->r;
			work->st.w = key->b;

			// flip
			if ( work->flag & AMD_AME_RWFLAG_TEXTURE_FLIP_U )
			{
				float tmp = work->st.x;
				work->st.x = work->st.z;
				work->st.z = tmp;
			}
			if ( work->flag & AMD_AME_RWFLAG_TEXTURE_FLIP_V )
			{
				float tmp = work->st.y;
				work->st.y = work->st.w;
				work->st.w = tmp;
			}
		}

		// uv scroll
		else if ( node->flag & AMD_AME_FLAG_TEXTURE_UV_SCROLL )
		{
			float u, v;

			u = node->scroll_u * _am_unit_time;
			v = node->scroll_v * _am_unit_time;

			// flip
			if ( work->flag & AMD_AME_RWFLAG_TEXTURE_FLIP_U ) u = -u;
			if ( work->flag & AMD_AME_RWFLAG_TEXTURE_FLIP_V ) v = -v;

			work->st.x += u;
			work->st.y += v;
			work->st.z += u;
			work->st.w += v;
		}
	}

	return 0;
}

//******************************************************************************
//	_amDrawLine()
//------------------------------------------------------------------------------
//	[Function]
//		ラインの描画
//	[Output]
//		runtime : ランタイムへのポインタ
//	[Comment]
//		注意事項など
//******************************************************************************
void _amDrawLine(AMS_AME_RUNTIME* runtime)
{
	AMS_AME_NODE_LINE*			node = (AMS_AME_NODE_LINE*)runtime->node;
	AMS_AME_RUNTIME_WORK_LINE*	work = (AMS_AME_RUNTIME_WORK_LINE*)runtime->active_head.next;
	AMS_AME_RUNTIME_WORK_LINE*	tail = (AMS_AME_RUNTIME_WORK_LINE*)&( runtime->active_tail );

	// 各種描画設定
	AMS_PARAM_DRAW_PRIMITIVE param;
	memset(&param, 0, sizeof(AMS_PARAM_DRAW_PRIMITIVE));
	NNE_PRIM_ALPHABLEND blend = _amEffectSetDrawMode(runtime, &param, node->blend);

	// オフセット計算
	NNS_VECTOR offset, eye;
	float zBias = node->z_bias;
	amVectorSet(&offset, zBias * NNM_MTX(_am_ef_worldViewMtx, 2, 0), zBias * NNM_MTX(_am_ef_worldViewMtx, 2, 1), zBias * NNM_MTX(_am_ef_worldViewMtx, 2, 2));
	amVectorSet(&eye, NNM_MTX(_am_ef_worldViewMtx, 2, 0), NNM_MTX(_am_ef_worldViewMtx, 2, 1), NNM_MTX(_am_ef_worldViewMtx, 2, 2));

	// テクスチャあり
	if ( node->flag & AMD_AME_FLAG_TEXTURE )
	{
		// 頂点データ格納領域取得
		NNS_PRIM3D_PCT* pv = (NNS_PRIM3D_PCT *)amDrawMallocDataBuffer(sizeof(NNS_PRIM3D_PCT) * 6 * runtime->active_num);
		NNS_PRIM3D_PCT* head = pv; // 先頭保存
		float len = 0.0f;

		for ( ; work != tail; work = (AMS_AME_RUNTIME_WORK_LINE*)work->next )
		{
			NNS_VECTOR pos[2], vel, cross;
			
			// pos0 は外側エッジの中心点
			// pos1 は内側エッジの中心点
			amVectorUnit( &vel, &( work->velocity ) );
			nnScaleVector( &pos[0], &vel, work->length );
			amVectorAdd( &pos[1], &( work->position ), &offset );
			nnAddVector( &pos[0], &pos[0], &pos[1] );

			// カメラからの距離
			len = nnDistanceVector(&pos[0], &_am_ef_camPos);

			nnCrossProductVector( &cross, &vel, &eye );
			nnNormalizeVector( &cross, &cross );

			// 頂点の並びは以下のとおり（三角形が二つで四角形を形成）
			//  0------pos0------1,3（外側）
			//  |             /  |
			//  |           /    |
			//  |        /       |
			//  |     /          |
			//  |  /             |
			//  2,4----pos1------5（内側）
			nnScaleVector( &vel, &cross, work->outside_width );
			nnSubtractVector( &pv[0].Pos, &pos[0], &vel );  // 左上
			nnAddVector( &pv[1].Pos, &pos[0], &vel );       // 右上
			
			nnScaleVector( &vel, &cross, work->inside_width );
			nnSubtractVector( &pv[2].Pos, &pos[1], &vel );  // 左下
			nnAddVector( &pv[5].Pos, &pos[1], &vel );       // 右下

			// Color
			// 外側
			pv[1].Col = AMD_RGBA8888(work->outside_color.r, work->outside_color.g, work->outside_color.b, work->outside_color.a);
			pv[0].Col = pv[1].Col;

			// 内側
			pv[5].Col = AMD_RGBA8888(work->inside_color.r, work->inside_color.g, work->inside_color.b, work->inside_color.a);
			pv[2].Col  = pv[5].Col;

			// Texture
			pv[0].Tex.u = work->st.x; pv[0].Tex.v = work->st.y;
			pv[1].Tex.u = work->st.z; pv[1].Tex.v = work->st.y;
			pv[2].Tex.u = work->st.x; pv[2].Tex.v = work->st.w;
			pv[5].Tex.u = work->st.z; pv[5].Tex.v = work->st.w;
		
			pv[3] = pv[1];
			pv[4] = pv[2];

			pv += 6;
		}

		// プリミティブ描画設定
		param.format3D = NNE_PRIM3D_FMT_PCT;
		param.type = NNE_PRIM_TRIANGLE_LIST;
		param.vtxPCT3D = head;
		param.texlist = runtime->texlist;
		param.texId = node->texture_id;
		param.count = 6 * runtime->active_num;
		param.ablend = blend;
		param.sortZ = len;
		amDrawPrimitive3D(runtime->ecb->drawState, &param);
	}
	// テクスチャなし
	else
	{
		// 頂点データ格納領域取得
		NNS_PRIM3D_PC* pv = (NNS_PRIM3D_PC *)amDrawMallocWorkBuffer(sizeof(NNS_PRIM3D_PC) * 6 * runtime->active_num);
		NNS_PRIM3D_PC* head = pv; // 先頭保存
		float len = 0.0f;

		for ( ; work != tail; work = (AMS_AME_RUNTIME_WORK_LINE*)work->next )
		{	
			NNS_VECTOR pos[2], vel, cross;
			
			// pos0 は外側エッジの中心点
			// pos1 は内側エッジの中心点
			amVectorUnit( &vel, &( work->velocity ) );
			nnScaleVector( &pos[0], &vel, work->length );
			amVectorAdd( &pos[1], &( work->position ), &offset );
			nnAddVector( &pos[0], &pos[0], &pos[1] );

			// カメラからの距離
			len = nnDistanceVector(&pos[0], &_am_ef_camPos);

			nnCrossProductVector( &cross, &vel, &eye );
			nnNormalizeVector( &cross, &cross );

			// 頂点の並びは以下のとおり（三角形が二つで四角形を形成）
			//  0------pos0------1,3（外側）
			//  |             /  |
			//  |           /    |
			//  |        /       |
			//  |     /          |
			//  |  /             |
			//  2,4----pos1------5（内側）
			nnScaleVector( &vel, &cross, work->outside_width );
			nnSubtractVector( &pv[0].Pos, &pos[0], &vel );  // 左上
			nnAddVector( &pv[1].Pos, &pos[0], &vel );       // 右上
			
			nnScaleVector( &vel, &cross, work->inside_width );
			nnSubtractVector( &pv[2].Pos, &pos[1], &vel );  // 左下
			nnAddVector( &pv[5].Pos, &pos[1], &vel );       // 右下

			// Color
			// 外側
			pv[1].Col = AMD_RGBA8888(work->outside_color.r, work->outside_color.g, work->outside_color.b, work->outside_color.a);
			pv[0].Col = pv[1].Col;

			// 内側
			pv[5].Col = AMD_RGBA8888(work->inside_color.r, work->inside_color.g, work->inside_color.b, work->inside_color.a);
			pv[2].Col = pv[5].Col;

			pv[3] = pv[1];
			pv[4] = pv[2];

			pv += 6;
		}

		// プリミティブ描画設定
		param.format3D = NNE_PRIM3D_FMT_PC;
		param.type = NNE_PRIM_TRIANGLE_LIST;
		param.vtxPC3D = head;
		param.texlist = runtime->texlist;
		param.texId = -1;
		param.count = 6 * runtime->active_num;
		param.ablend = blend;
		param.sortZ = len;
		amDrawPrimitive3D(runtime->ecb->drawState, &param);
	}
}

//{### ユーザーエフェクト関数（プレーン）
//******************************************************************************
//	_amInitPlane()
//------------------------------------------------------------------------------
//	[Function]
//		プレーンの初期化
//	[Output]
//		param	: 作成パラメータへのポインタ
//******************************************************************************
void _amInitPlane(AMS_AME_CREATE_PARAM* param)
{
	AMS_AME_NODE_PLANE*			node = (AMS_AME_NODE_PLANE*)param->node;
	AMS_AME_RUNTIME_WORK_PLANE*	work = (AMS_AME_RUNTIME_WORK_PLANE*)param->work;

	work->time = -node->start_time;

	// color
	work->color.color = node->color_start.color;
	work->color.a =	(Uint8)((work->color.a * param->ecb->transparency) >> 8);

	// position & velocity
	{
		amVectorAdd( &( work->position ), param->parent_position, param->position );
		amVectorAdd( &( work->position ), &( node->translate ) );

		amVectorScale( &( work->velocity ), param->parent_velocity, node->inheritance_rate );
		amVectorAdd( &( work->velocity ), param->velocity );
	}

	// rotate
	if ( node->flag & AMD_AME_FLAG_PLANE_RANDOM_ROTATE )
	{
#if AMD_AME_ROTATE_QUAT
		AMS_VECTOR vec;
		float radian = nnRandom() * 2.0f * AMD_MATH_PI;
		amVectorRandom( &vec );
		amQuatRotAxisToQuat( &( work->rotate ), &vec, radian );
#else
		amSetVector( &( work->rotate ),
			amAng2Rad( amRand() << 1 ),
			amAng2Rad( amRand() << 1 ),
			amAng2Rad( amRand() << 1 ) );
#endif
	}
	else
	{
#if AMD_AME_ROTATE_QUAT
		amVectorCopy( (AMS_VECTOR*)&( work->rotate ), (AMS_VECTOR*)&( node->rotate ) );
#else
		amVectorCopy( &( work->rotate ), &( node->rotate ) );
#endif
	}

	// rotate axis
	if ( node->flag & AMD_AME_FLAG_PLANE_RANDOM_AXIS )
	{
#if AMD_AME_ROTATE_QUAT
		amVectorRandom( &( work->rotate_axis ) );
		work->rotate_axis.w = node->rotate_axis.w;
#else
		work->rotate_velocity.x = amAng2Rad( amRand() << 1 );
		work->rotate_velocity.y = amAng2Rad( amRand() << 1 );
		work->rotate_velocity.z = amAng2Rad( amRand() << 1 );
#endif
	}
	else
	{
#if AMD_AME_ROTATE_QUAT
		amVectorCopy( &( work->rotate_axis ), &( node->rotate_axis ) );
#else
		amSetVector( &( work->rotate_velocity ),
			node->rotate_velocity.x,
			node->rotate_velocity.y,
			node->rotate_velocity.z );
#endif
	}

	// size
	{
		float size = node->size + node->size_chaos * nnRandom();

		work->size.x = size * node->scale_x_start;
		work->size.y = size * node->scale_y_start;
		work->size.z = size;
		work->size.w = 0.0f;
	}

	// texture animation
	if ( node->flag & AMD_AME_FLAG_TEXTURE_ANIMATION )
	{
		AMS_AME_TEX_ANIM_KEY* key;

		work->tex_time	= 0.0f;
		work->tex_no	= 0;

		// random key
		if ( node->flag & AMD_AME_FLAG_TEXTURE_RANDOM_KEY )
		{
			work->tex_no = (Sint32)(100 * nnRandom()) % node->tex_anim.key_num;
		}

		key = &( node->tex_anim.key_buf[ work->tex_no ] );
		work->st.x		= key->l;
		work->st.y		= key->t;
		work->st.z		= key->r;
		work->st.w		= key->b;
	}

	// texture cropping
	else if ( node->flag & AMD_AME_FLAG_TEXTURE_CROPPING )
	{
		work->st.x = node->cropping_l;
		work->st.y = node->cropping_t;
		work->st.z = node->cropping_r;
		work->st.w = node->cropping_b;
	}
	else
	{
		amVectorSet( (AMS_VECTOR*)&( work->st ), 0.0f,0.0f,1.0f );
	}

	// flip
	if ( (node->flag & AMD_AME_FLAG_TEXTURE_FLIP_U_FIXED) ||
		((node->flag & AMD_AME_FLAG_TEXTURE_FLIP_U_RANDOM) && (nnRandom() > 0.5f)) )
	{
		work->flag |= AMD_AME_RWFLAG_TEXTURE_FLIP_U;

		float tmp = work->st.x;
		work->st.x = work->st.z;
		work->st.z = tmp;
	}
	if ( (node->flag & AMD_AME_FLAG_TEXTURE_FLIP_V_FIXED) ||
		((node->flag & AMD_AME_FLAG_TEXTURE_FLIP_V_RANDOM) && (nnRandom() > 0.5f)) )
	{
		work->flag |= AMD_AME_RWFLAG_TEXTURE_FLIP_V;

		float tmp = work->st.y;
		work->st.y = work->st.w;
		work->st.w = tmp;
	}
}

//******************************************************************************
//	_amUpdatePlane()
//------------------------------------------------------------------------------
//	[Function]
//		プレーンの更新
//	[Output]
//		runtime : ランタイムへのポインタ
//	[Return]
//		1	: 削除要求
//******************************************************************************
Sint32 _amUpdatePlane(AMS_AME_RUNTIME* runtime)
{
	AMS_AME_NODE_PLANE*			node = (AMS_AME_NODE_PLANE*)runtime->node;
	AMS_AME_RUNTIME_WORK_PLANE*	work = (AMS_AME_RUNTIME_WORK_PLANE*)runtime->active_head.next;
	AMS_AME_RUNTIME_WORK_PLANE*	tail = (AMS_AME_RUNTIME_WORK_PLANE*)&( runtime->active_tail );

	amAssert( AMD_AME_IS_PARTICLE( node ) );

	Sint32 trans = runtime->ecb->transparency;

	float life, lifeInv;
	if ( node->life >= 0.0f ) {
		life	= node->life;
		lifeInv	= 1.0f / life;
	}
	else {
		life	= FLT_MAX;//3.40282347e+38F/*FLT_MAX*/;
		lifeInv	= 0.0f;
	}

	float sx, sy, ex, ey;
	{
		float rate = runtime->ecb->size_rate;
		sx = node->scale_x_start * rate;
		sy = node->scale_y_start * rate;
		ex = node->scale_x_end * rate;
		ey = node->scale_y_end * rate;
	}

	for ( ; work != tail; work = (AMS_AME_RUNTIME_WORK_PLANE*)work->next )
	{
		work->time += _am_unit_frame;

		float rate = work->time * lifeInv;

		// position & velocity
		{
			AMS_VECTOR vel;
			amVectorScale( &vel, &( work->velocity ), _am_unit_time );
			amVectorAdd( &( work->position ), &vel );
		}

		// life
		if ( work->time >= life )
		{
			// create spawn particle
			if ( runtime->spawn_runtime )
				_amCreateSpawnParticle( runtime, (AMS_AME_RUNTIME_WORK*)work );

			// delete particle
			amEffectDisconnectLink( (AMS_AME_LIST*)work );
			runtime->active_num--;
			amEffectFreeRuntimeWork( (AMS_AME_RUNTIME_WORK*)work );

			continue;
		}

		// rotate
		{
#if AMD_AME_ROTATE_QUAT
			AMS_QUAT q;
			// rotate_axis.xyz = axis
			// rotate_axis.w   = angle
			amQuatRotAxisToQuat( &q, &( work->rotate_axis ), work->rotate_axis.w * _am_unit_time );
			amQuatMulti( &( work->rotate ), &q, &( work->rotate ) );
#else
			amScaleVector( &rot, &( node->rotate_velocity ), _am_unit_time );
			amAddVector( &( work->rotate ), &rot );
#endif
		}

		// scale
		{
			float rateInv = 1.0f - rate;
			float x = sx * rateInv + ex * rate;
			float y = sy * rateInv + ey * rate;
			work->size.x = work->size.z * x;
			work->size.y = work->size.z * y;
		}

		// color
		amEffectLerpColor( &( work->color ),
				&( node->color_start ), &( node->color_end ), rate );

		// global transaprency
		work->color.a =(Uint8)
			((work->color.a * trans) >> 8);

		// texture animation
		if ( node->flag & AMD_AME_FLAG_TEXTURE_ANIMATION )
		{
			AMS_AME_TEX_ANIM*		anim = &( node->tex_anim );
			AMS_AME_TEX_ANIM_KEY*	key;

			if ( (work->flag & AMD_AME_RWFLAG_SKIP_ANIMATION) == 0 )
			{
				work->tex_time += _am_unit_frame;

				if ( work->tex_time >= anim->key_buf[ work->tex_no ].time )
				{
					work->tex_time	= 0.0f;
					work->tex_no	+= 1;

					if ( work->tex_no == anim->key_num )
					{
						if ( node->flag & AMD_AME_FLAG_TEXTURE_LOOP )
						{
							work->tex_no = 0;
						}
						else
						{
							work->tex_no = anim->key_num - 1;
							work->flag |= AMD_AME_RWFLAG_SKIP_ANIMATION;
						}
					}
				}
			}

			key = &( anim->key_buf[ work->tex_no ] );
			work->st.x = key->l;
			work->st.y = key->t;
			work->st.z = key->r;
			work->st.w = key->b;

			// flip
			if ( work->flag & AMD_AME_RWFLAG_TEXTURE_FLIP_U )
			{
				float tmp = work->st.x;
				work->st.x = work->st.z;
				work->st.z = tmp;
			}
			if ( work->flag & AMD_AME_RWFLAG_TEXTURE_FLIP_V )
			{
				float tmp = work->st.y;
				work->st.y = work->st.w;
				work->st.w = tmp;
			}
		}

		// uv scroll
		else if ( node->flag & AMD_AME_FLAG_TEXTURE_UV_SCROLL )
		{
			float u, v;

			u = node->scroll_u * _am_unit_time;
			v = node->scroll_v * _am_unit_time;

			// flip
			if ( work->flag & AMD_AME_RWFLAG_TEXTURE_FLIP_U ) u = -u;
			if ( work->flag & AMD_AME_RWFLAG_TEXTURE_FLIP_V ) v = -v;

			work->st.x += u;
			work->st.y += v;
			work->st.z += u;
			work->st.w += v;
		}
	}

	return 0;
}

//******************************************************************************
//	_amDrawPlane()
//------------------------------------------------------------------------------
//	[Function]
//		プレーンの描画
//	[Output]
//		runtime : ランタイムへのポインタ
//******************************************************************************
void _amDrawPlane(AMS_AME_RUNTIME* runtime)
{
	AMS_AME_NODE_PLANE*			node = (AMS_AME_NODE_PLANE*)runtime->node;
	AMS_AME_RUNTIME_WORK_PLANE*	work = (AMS_AME_RUNTIME_WORK_PLANE*)runtime->active_head.next;
	AMS_AME_RUNTIME_WORK_PLANE*	tail = (AMS_AME_RUNTIME_WORK_PLANE*)&( runtime->active_tail );

	// 各種描画設定
	AMS_PARAM_DRAW_PRIMITIVE param;
	memset(&param, 0, sizeof(AMS_PARAM_DRAW_PRIMITIVE));
	NNE_PRIM_ALPHABLEND blend = _amEffectSetDrawMode(runtime, &param, node->blend);

	// オフセット計算
	AMS_VECTOR offset;
	float zBias = node->z_bias;
	amVectorSet(&offset, zBias * NNM_MTX(_am_ef_worldViewMtx, 2, 0),
		zBias * NNM_MTX(_am_ef_worldViewMtx, 2, 1), zBias * NNM_MTX(_am_ef_worldViewMtx, 2, 2));

	// テクスチャあり
	if ( node->flag & AMD_AME_FLAG_TEXTURE )
	{
		// 頂点データ格納領域取得
		NNS_PRIM3D_PCT* pv = (NNS_PRIM3D_PCT *)amDrawMallocDataBuffer(sizeof(NNS_PRIM3D_PCT) * 6 * runtime->active_num);
		NNS_PRIM3D_PCT* head = pv; // 先頭保存
		float len = 0.0f;

		for ( ; work != tail; work = (AMS_AME_RUNTIME_WORK_PLANE*)work->next )
		{
			AMS_VECTOR pos;
			AMS_MATRIX* m;
			float x, y;

			amMatrixPush( NULL );
			m = amMatrixGetCurrent();
			x = work->size.x;
			y = work->size.y;

#if AMD_AME_ROTATE_QUAT
			amVectorAdd( &pos, &( work->position ), &offset );
			amQuatMultiMatrix( &( work->rotate ), &pos );
#else
			nnTranslateMatrix( m, m, work->position.x, work->position.y, work->position.z );
			nnRotateXYZMatrix( m, m, NNM_DEGtoA32( work->rotate.x ),
						NNM_DEGtoA32( work->rotate.y ),
						NNM_DEGtoA32( work->rotate.z ) );
#endif
			// カメラからの距離
			len = nnDistanceVector((NNS_VECTOR*)&pos, &_am_ef_camPos);

			amVectorSet( &pv[0].Pos, -x, y, 0.0f );
			amVectorSet( &pv[1].Pos, x, y, 0.0f );
			amVectorSet( &pv[2].Pos, -x, -y, 0.0f );
			amVectorSet( &pv[5].Pos, x, -y, 0.0f );

			// 頂点の並びは以下のとおり（三角形が二つで四角形を形成）
			//  0-1  3
			//  |/  /|
			//  2  4-5

			// グローバル座標に変換
			nnTransformVector( &pv[0].Pos, m, &pv[0].Pos ); // 左上
			nnTransformVector( &pv[1].Pos, m, &pv[1].Pos ); // 右上
			nnTransformVector( &pv[2].Pos, m, &pv[2].Pos ); // 左下
			nnTransformVector( &pv[5].Pos, m, &pv[5].Pos ); // 右下

			// Color
			pv[5].Col = AMD_RGBA8888(work->color.r, work->color.g, work->color.b, work->color.a);
			pv[0].Col = pv[1].Col = pv[2].Col = pv[5].Col;

			// Texture
			pv[0].Tex.u = work->st.x; pv[0].Tex.v = work->st.y;
			pv[1].Tex.u = work->st.z; pv[1].Tex.v = work->st.y;
			pv[2].Tex.u = work->st.x; pv[2].Tex.v = work->st.w;
			pv[5].Tex.u = work->st.z; pv[5].Tex.v = work->st.w;

			pv[3] = pv[1];
			pv[4] = pv[2];

			pv += 6;

			amMatrixPop();
		}

		// プリミティブ描画設定
		param.format3D = NNE_PRIM3D_FMT_PCT;
		param.type = NNE_PRIM_TRIANGLE_LIST;
		param.vtxPCT3D = head;
		param.texlist = runtime->texlist;
		param.texId = node->texture_id;
		param.count = 6 * runtime->active_num;
		param.ablend = blend;
		param.sortZ = len;
		amDrawPrimitive3D(runtime->ecb->drawState, &param);
	}
	// テクスチャなし
	else
	{
		// 頂点データ格納領域取得
		NNS_PRIM3D_PC* pv = (NNS_PRIM3D_PC *)amDrawMallocWorkBuffer(sizeof(NNS_PRIM3D_PC) * 6 * runtime->active_num);
		NNS_PRIM3D_PC* head = pv; // 先頭保存
		float len = 0.0f;

		for ( ; work != tail; work = (AMS_AME_RUNTIME_WORK_PLANE*)work->next )
		{
			AMS_VECTOR pos;
			AMS_MATRIX* m;
			float x, y;

			amMatrixPush( NULL );
			m = amMatrixGetCurrent();
			x = work->size.x;
			y = work->size.y;

#if AMD_AME_ROTATE_QUAT
			amVectorAdd( &pos, &( work->position ), &offset );
			amQuatMultiMatrix( &( work->rotate ), &pos );
#else
			nnTranslateMatrix( m, m, work->position.x, work->position.y, work->position.z );
			nnRotateXYZMatrix( m, m, NNM_DEGtoA32( work->rotate.x ),
						NNM_DEGtoA32( work->rotate.y ),
						NNM_DEGtoA32( work->rotate.z ) );
#endif
			// カメラからの距離
			len = nnDistanceVector((NNS_VECTOR*)&pos, &_am_ef_camPos);

			amVectorSet( &pv[0].Pos, -x, y, 0.0f );
			amVectorSet( &pv[1].Pos, x, y, 0.0f );
			amVectorSet( &pv[2].Pos, -x, -y, 0.0f );
			amVectorSet( &pv[5].Pos, x, -y, 0.0f );

			// 頂点の並びは以下のとおり（三角形が二つで四角形を形成）
			//  0-1  3
			//  |/  /|
			//  2  4-5

			// グローバル座標に変換
			nnTransformVector( &pv[0].Pos, m, &pv[0].Pos ); // 左上
			nnTransformVector( &pv[1].Pos, m, &pv[1].Pos ); // 右上
			nnTransformVector( &pv[2].Pos, m, &pv[2].Pos ); // 左下
			nnTransformVector( &pv[5].Pos, m, &pv[5].Pos ); // 右下

			// Color
			pv[5].Col = AMD_RGBA8888(work->color.r, work->color.g, work->color.b, work->color.a);
			pv[0].Col = pv[1].Col = pv[2].Col = pv[5].Col;

			pv[3] = pv[1];
			pv[4] = pv[2];

			pv += 6;

			amMatrixPop();
		}

		// プリミティブ描画設定
		param.format3D = NNE_PRIM3D_FMT_PC;
		param.type = NNE_PRIM_TRIANGLE_LIST;
		param.vtxPC3D = head;
		param.texlist = runtime->texlist;
		param.texId = -1;
		param.count = 6 * runtime->active_num;
		param.ablend = blend;
		param.sortZ = len;
		amDrawPrimitive3D(runtime->ecb->drawState, &param);
    }
}

//{### ユーザーエフェクト関数（モデル）
//******************************************************************************
//	_amInitModel()
//------------------------------------------------------------------------------
//	[Function]
//		モデルの初期化
//	[Output]
//		param	: 作成パラメータへのポインタ
//	[Comment]
//		注意事項など
//******************************************************************************
void _amInitModel(AMS_AME_CREATE_PARAM* param)
{
	AMS_AME_NODE_MODEL*			node = (AMS_AME_NODE_MODEL*)param->node;
	AMS_AME_RUNTIME_WORK_MODEL*	work = (AMS_AME_RUNTIME_WORK_MODEL*)param->work;

	work->time = -node->start_time;

	amVectorCopy( &( work->scale ), &( node->scale_start ) );

	// color
	work->color.color = node->color_start.color;

	// position & velocity
	{
		amVectorAdd( &( work->position ), param->parent_position, param->position );
		amVectorAdd( &( work->position ), &( node->translate ) );

		amVectorScale( &( work->velocity ), param->parent_velocity, node->inheritance_rate );
		amVectorAdd( &( work->velocity ), param->velocity );
	}

	// rotate
	if ( node->flag & AMD_AME_FLAG_MODEL_RANDOM_ROTATE )
	{
#if AMD_AME_ROTATE_QUAT
		AMS_VECTOR vec;
		amVectorRandom( &vec );
		amQuatRotAxisToQuat( &( work->rotate ), &vec, nnRandom() * 2.0f * AMD_MATH_PI );
#else
		amSetVector( &( work->rotate ),
			amAng2Rad( amRand() << 1 ),
			amAng2Rad( amRand() << 1 ),
			amAng2Rad( amRand() << 1 ) );
#endif
	}
	else
	{
#if AMD_AME_ROTATE_QUAT
		amVectorCopy( (AMS_VECTOR*)&( work->rotate ), (AMS_VECTOR*)&( node->rotate ) );
#else
		amVectorCopy( &( work->rotate ), &( node->rotate ) );
#endif
	}

	// rotate axis
	if ( node->flag & AMD_AME_FLAG_MODEL_RANDOM_AXIS )
	{
#if AMD_AME_ROTATE_QUAT
		amVectorRandom( &( work->rotate_axis ) );
		work->rotate_axis.w = node->rotate_axis.w;
#else
		work->rotate_velocity.x = amAng2Rad( amRand() << 1 );
		work->rotate_velocity.y = amAng2Rad( amRand() << 1 );
		work->rotate_velocity.z = amAng2Rad( amRand() << 1 );
#endif
	}
	else
	{
#if AMD_AME_ROTATE_QUAT
		amVectorCopy( &( work->rotate_axis ), &( node->rotate_axis ) );
#else
		amVectorSet( &( work->rotate_velocity ),
			node->rotate_velocity.x,
			node->rotate_velocity.y,
			node->rotate_velocity.z );
#endif
	}
}

//******************************************************************************
//	_amUpdateModel()
//------------------------------------------------------------------------------
//	[Function]
//		モデルの更新
//	[Output]
//		runtime : ランタイムへのポインタ
//	[Return]
//		1	: 削除要求
//	[Comment]
//		注意事項など
//******************************************************************************
Sint32 _amUpdateModel(AMS_AME_RUNTIME* runtime)
{
	AMS_AME_NODE_MODEL*			node = (AMS_AME_NODE_MODEL*)runtime->node;
	AMS_AME_RUNTIME_WORK_MODEL*	work = (AMS_AME_RUNTIME_WORK_MODEL*)runtime->active_head.next;
	AMS_AME_RUNTIME_WORK_MODEL*	tail = (AMS_AME_RUNTIME_WORK_MODEL*)&( runtime->active_tail );

	amAssert( AMD_AME_IS_PARTICLE( node ) );

	Sint32 trans = runtime->ecb->transparency;

	float life, lifeInv;
	if ( node->life >= 0.0f ) {
		life	= node->life;
		lifeInv	= 1.0f / life;
	}
	else {
		life	= FLT_MAX;//3.40282347e+38F/*FLT_MAX*/;
		lifeInv	= 0.0f;
	}

	AMS_VECTOR s, e;
	amVectorScale( &s, &( node->scale_start ), runtime->ecb->size_rate );
	amVectorScale( &e, &( node->scale_end ), runtime->ecb->size_rate );

	for ( ; work != tail; work = (AMS_AME_RUNTIME_WORK_MODEL*)work->next )
	{
		work->time += _am_unit_frame;

		float rate = work->time * lifeInv;

		// position & velocity
		{
			AMS_VECTOR vel;
			amVectorScale( &vel, &( work->velocity ), _am_unit_time );
			amVectorAdd( &( work->position ), &vel );
		}

		// life
		if ( work->time >= life )
		{
			// create spawn particle
			if ( runtime->spawn_runtime )
				_amCreateSpawnParticle( runtime, (AMS_AME_RUNTIME_WORK*)work );

			// delete particle
			amEffectDisconnectLink( (AMS_AME_LIST*)work );
			runtime->active_num--;
			amEffectFreeRuntimeWork( (AMS_AME_RUNTIME_WORK*)work );

			continue;
		}

		// rotate
		{
#if AMD_AME_ROTATE_QUAT
			AMS_QUAT q;
			// rotate_axis.xyz = axis
			// rotate_axis.w   = angle
			amQuatRotAxisToQuat( &q, &( work->rotate_axis ), work->rotate_axis.w * _am_unit_time );
			amQuatMulti( &( work->rotate ), &( work->rotate ), &q );
#else
			amVectorScale( &rot, &( node->rotate_velocity ), _am_unit_time );
			amVectorAdd( &( work->rotate ), &rot );
#endif
		}

		// scale
		amVectorGetInner( &( work->scale ), &s, &e, rate );

		// color
		amEffectLerpColor( &( work->color ), &( node->color_start ), &( node->color_end ), rate );

		// global transaprency
		work->color.a = (Uint8)((work->color.a * trans) >> 8);

		// uv scroll
		work->scroll_u += node->scroll_u * _am_unit_time;
		work->scroll_v += node->scroll_v * _am_unit_time;
	}

	return 0;
}

//******************************************************************************
//	_amDrawModel()
//------------------------------------------------------------------------------
//	[Function]
//		モデルの描画
//	[Output]
//		runtime : ランタイムへのポインタ
//	[Comment]
//		注意事項など
//******************************************************************************
void _amDrawModel(AMS_AME_RUNTIME* runtime)
{
	AMS_AME_NODE_MODEL*	node = (AMS_AME_NODE_MODEL*)runtime->node;
	AMS_AME_ECB*		ecb = runtime->ecb;
	NNS_RGBA		    color;
	Sint32	blend = NNE_MATCTRLMODE_OFF;

    // オブジェクト設定されていない
	if (runtime->ecb->pObj == NULL) return;

	// マテリアル設定
	NNF_DRAWOBJ drawFlag = _amEffectSetMaterial(runtime, &blend, node->blend);

	// オフセット計算
	AMS_VECTOR offset;
	float zBias = node->z_bias;
	amVectorSet(&offset, zBias * NNM_MTX(_am_ef_worldViewMtx, 2, 0),
		zBias * NNM_MTX(_am_ef_worldViewMtx, 2, 1), zBias * NNM_MTX(_am_ef_worldViewMtx, 2, 2));

	AMS_AME_RUNTIME_WORK_MODEL*	work = (AMS_AME_RUNTIME_WORK_MODEL*)runtime->active_head.next;
	AMS_AME_RUNTIME_WORK_MODEL*	tail = (AMS_AME_RUNTIME_WORK_MODEL*)&( runtime->active_tail );
	for ( ; work != tail; work = (AMS_AME_RUNTIME_WORK_MODEL*)work->next )
	{
		NNS_MATRIX mtx;
		NNS_MATRIX* cm;
		float	u, v;
		
		amMatrixPush( NULL );
		
#if AMD_AME_ROTATE_QUAT
		AMS_VECTOR pos;
		amVectorAdd( &pos, &( work->position ), &offset );
		amQuatMultiMatrix( &( work->rotate ), &pos );
#else
		AMS_MATRIX* m = amMatrixGetCurrent();
		nnTranslateMatrix( m, m, work->position.x, work->position.y, work->position.z );
		nnRotateXYZMatrix( m, m, NNM_DEGtoA32( work->rotate.x ),
						NNM_DEGtoA32( work->rotate.y ),
						NNM_DEGtoA32( work->rotate.z ) );
#endif
		// UVスクロール
		u = work->scroll_u;
		v = work->scroll_v;
		drawFlag |= NND_DRAWOBJ_MATCTRL_TEXOFFSET;

		if (u >= 1.0f)
			u -= (float)((int)u);
		else while (u < 0.0f)
			u += 1.0f;

		if (v >= 1.0f)
			v -= (float)((int)v);
		else while (v < 0.0f)
			v += 1.0f;

		// 色計算
		drawFlag |= (NND_DRAWOBJ_MATCTRL_DIFFUSE | NND_DRAWOBJ_MATCTRL_AMBIENT);
		color.a = (float)(work->color.a) * AMD_MATH_S8TOFLOAT;
		color.r = (float)(work->color.r) * AMD_MATH_S8TOFLOAT;
		color.g = (float)(work->color.g) * AMD_MATH_S8TOFLOAT;
		color.b = (float)(work->color.b) * AMD_MATH_S8TOFLOAT;

		// マトリクス計算
		cm = amMatrixGetCurrent();
		memcpy( &mtx, amMatrixGetCurrent(), sizeof(NNS_MATRIX));
		nnScaleMatrix( &mtx, &mtx, work->scale.x, work->scale.y, work->scale.z );
	    nnCopyMatrix(cm, &mtx);

		amDrawObjectSetMaterial(runtime->ecb->drawObjState, ecb->pObj, runtime->texlist,
			(NNS_VECTOR *)&work->scale, color, u, v, blend, drawFlag);

		amMatrixPop();
	}
}

//{### ユーザーエフェクト関数（フィールドの適用）
//******************************************************************************
//	_amApplyGravity()
//------------------------------------------------------------------------------
//	[Function]
//		重力フィールドの適用
//	[Output]
//		ecb		: エントリーコントロールブロックへのポインタ
//		node	: ノードへのポインタ
//		work	: ランタイムワークへのポインタ
//******************************************************************************
void _amApplyGravity(AMS_AME_ECB* ecb, AMS_AME_NODE* node, AMS_AME_RUNTIME_WORK* work)
{
	amAssert( ecb );
	amAssert( node );
	amAssert( work );

	AMS_AME_NODE_GRAVITY* field = (AMS_AME_NODE_GRAVITY*)node;

	float t = _am_unit_time;
	AMS_VECTOR vec, dir;

	amVectorCopy( &dir, &( field->direction ) );

#if (1)
	// ルートの回転成分を反映
	if ( field->flag & AMD_AME_FLAG_FIELD_ROOT_ROTATE )
	{
#if AMD_AME_ROTATE_QUAT
		amQuatMultiVector( &dir, &dir, &( ecb->rotate ), NULL);
#else
		amSaveMatrix();
		amRotateXYZ( (float*)&( ecb->rotate ) );
		amCalcVector( &dir, &dir );
		amLoadMatrix();
#endif
	}
#endif

	// d = 1/2 * a * t~2
	amVectorScale( &vec, &dir, field->magnitude * t * t * 0.5f );
	amVectorAdd( &( work->position ), &vec );
	amVectorScale( &vec, &dir, field->magnitude * t );
	amVectorAdd( &( work->velocity ), &vec );
}

//******************************************************************************
//	_amApplyUniform()
//------------------------------------------------------------------------------
//	[Function]
//		均一フィールドの適用
//	[Output]
//		ecb		: エントリーコントロールブロックへのポインタ
//		node	: ノードへのポインタ
//		work	: ランタイムワークへのポインタ
//******************************************************************************
void _amApplyUniform(AMS_AME_ECB* ecb, AMS_AME_NODE* node, AMS_AME_RUNTIME_WORK* work)
{
	amAssert( ecb );
	amAssert( node );
	amAssert( work );

	AMS_AME_NODE_UNIFORM* field = (AMS_AME_NODE_UNIFORM*)node;

	float t = _am_unit_time;
	AMS_VECTOR vec;

	amVectorScale( &vec, &( field->direction ), field->magnitude * t );

#if (1)
	// ルートの回転成分を反映
	if ( field->flag & AMD_AME_FLAG_FIELD_ROOT_ROTATE )
	{
#if AMD_AME_ROTATE_QUAT
		amQuatMultiVector( &vec, &vec, &( ecb->rotate ), NULL);
#else
		amSaveMatrix();
		amRotateXYZ( (float*)&( ecb->rotate ) );
		amCalcVector( &vec, &vec );
		amLoadMatrix();
#endif
	}
#endif

	amVectorAdd( &( work->position ), &vec );
}

//******************************************************************************
//	_amApplyRadial()
//------------------------------------------------------------------------------
//	[Function]
//		放射状フィールドの適用
//	[Output]
//		ecb		: エントリーコントロールブロックへのポインタ
//		node	: ノードへのポインタ
//		work	: ランタイムワークへのポインタ
//******************************************************************************
void _amApplyRadial(AMS_AME_ECB* ecb, AMS_AME_NODE* node, AMS_AME_RUNTIME_WORK* work)
{
	amAssert( ecb );
	amAssert( node );
	amAssert( work );

	AMS_AME_NODE_RADIAL* field = (AMS_AME_NODE_RADIAL*)node;

	float t = _am_unit_time;

	float atten;
	AMS_VECTOR dir, pos;

	amVectorCopy( &pos, &( field->position ) );

#if (1)
	// ルートの平行移動成分を反映
	if ( field->flag & AMD_AME_FLAG_FIELD_ROOT_TRANSLATE )
	{
		amVectorAdd( &pos, &( ecb->translate ) );
	}
#endif

	amVectorSub( &dir, &( work->position ), &pos );
	atten = 1.0f / powf( amVectorScalor( &dir ), field->attenuation );
	amVectorScaleUnit( &dir, &dir, field->magnitude * atten * t );
	amVectorAdd( &( work->position ), &dir );
}

//******************************************************************************
//	_amApplyVortex()
//------------------------------------------------------------------------------
//	[Function]
//		渦フィールドの適用
//	[Output]
//		ecb		: エントリーコントロールブロックへのポインタ
//		node	: ノードへのポインタ
//		work	: ランタイムワークへのポインタ
//	[Comment]
//		注意事項など
//******************************************************************************
void _amApplyVortex(AMS_AME_ECB* ecb, AMS_AME_NODE* node, AMS_AME_RUNTIME_WORK* work)
{
	amAssert( ecb );
	amAssert( node );
	amAssert( work );

	AMS_AME_NODE_VORTEX* field = (AMS_AME_NODE_VORTEX*)node;

	AMS_VECTOR pos, dir, axis, cross;

	amVectorCopy( &pos, &( field->position ) );
	amVectorCopy( &axis, &( field->axis ) );

#if (1)
	// ルートの平行移動成分を反映
	if ( field->flag & AMD_AME_FLAG_FIELD_ROOT_TRANSLATE )
	{
		amVectorAdd( &pos, &( ecb->translate ) );
	}

	// ルートの回転成分を反映
	if ( field->flag & AMD_AME_FLAG_FIELD_ROOT_ROTATE )
	{
#if AMD_AME_ROTATE_QUAT
		amQuatMultiVector( &axis, &axis, &( ecb->rotate ), NULL );
#else
		amSaveMatrix();
		amRotateXYZ( (float*)&( ecb->rotate ) );
		amCalcVector( &axis, &axis );
		amLoadMatrix();
#endif
	}
#endif

	// 中心点からの差分
	amVectorSub( &dir, &( work->position ), &pos );

	amVectorOuterProduct( &cross, &axis, &dir );
	amVectorScale( &dir, &cross, _am_unit_time );
	amVectorAdd( &( work->velocity ), &dir );

	amVectorOuterProduct( &cross, &axis, &cross );
	amVectorScale( &dir, &cross, _am_unit_time );
	amVectorAdd( &( work->velocity ), &dir );
}

//******************************************************************************
//	_amApplyDrag()
//------------------------------------------------------------------------------
//	[Function]
//		ドラッグフィールドの適用
//	[Output]
//		ecb		: エントリーコントロールブロックへのポインタ
//		node	: ノードへのポインタ
//		work	: ランタイムワークへのポインタ
//******************************************************************************
void _amApplyDrag(AMS_AME_ECB* ecb, AMS_AME_NODE* node, AMS_AME_RUNTIME_WORK* work)
{
	amAssert( ecb );
	amAssert( node );
	amAssert( work );

	AMS_AME_NODE_DRAG* field = (AMS_AME_NODE_DRAG*)node;

	float t = _am_unit_time;
	AMS_VECTOR vec, dir, pos;

	amVectorCopy( &pos, &( field->position ) );

#if (1)
	// ルートの平行移動成分を反映
	if ( field->flag & AMD_AME_FLAG_FIELD_ROOT_TRANSLATE )
	{
		amVectorAdd( &pos, &( ecb->translate ) );
	}
#endif

	amVectorSub( &dir, &( work->position ), &pos );
	amVectorUnit( &dir );

	amVectorScale( &vec, &dir, field->magnitude * t * t * 0.5f );
	amVectorAdd( &( work->position ), &vec );
	amVectorScale( &vec, &dir, field->magnitude * t );
	amVectorAdd( &( work->velocity ), &vec );
}

//******************************************************************************
//	_amApplyNoise()
//------------------------------------------------------------------------------
//	[Function]
//		ノイズフィールドの適用
//	[Output]
//		ecb		: エントリーコントロールブロックへのポインタ
//		node	: ノードへのポインタ
//		work	: ランタイムワークへのポインタ
//******************************************************************************
void _amApplyNoise(AMS_AME_ECB* ecb, AMS_AME_NODE* node, AMS_AME_RUNTIME_WORK* work)
{
	UNREFERENCED_PARAMETER(ecb);
	amAssert( ecb );
	amAssert( node );
	amAssert( work );

	AMS_AME_NODE_NOISE* field = (AMS_AME_NODE_NOISE*)node;
	float m = field->magnitude * _am_unit_time;
	AMS_VECTOR vec;

	vec.x = (nnRandom() - 0.5f) * m * field->axis.x;
	vec.y = (nnRandom() - 0.5f) * m * field->axis.y;
	vec.z = (nnRandom() - 0.5f) * m * field->axis.z;

	amVectorAdd( &( work->position ), &vec );
}

// ================================================================
/*!
	エフェクトを削除します

	@param	[i]		エントリーコントロールブロックへのポインタ

	@note
	この関数ではエントリーコントロールブロックの実体は開放されません
	実体の開放は amExecuteEffect() で行われます
*/
// ================================================================
void amEffectDelete(AMS_AME_ECB* ecb)
{
	amAssert( ecb );
//	amAssert( ecb->entry_num != -1 );	// 重複呼び出し

	ecb->entry_num = -1;	// 削除フラグ
}

// End of File