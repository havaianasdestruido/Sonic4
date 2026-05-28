/*****************************************************************************/
/*      amMatrix.cpp                Author : Takashi Nakano                  */
/*            Copyright(c) 2009 Dimps CORP. All Rights Reserved.             */
/*---------------------------------------------------------------------------*/
/* マトリクスライブラリプログラム                                            */
/*---------------------------------------------------------------------------*/
/* Date          Ver    Comment                                              */
/* 090330-       0.01   first version                                        */
/*****************************************************************************/

/*--- Include Files ---------------------------------------------------------*/

#include "alice.h"


/*--- Macros ----------------------------------------------------------------*/

/*--- Definitions -----------------------------------------------------------*/

NNS_MATRIXSTACK *_amMatrixGetCurrentStack(void);


/*--- External Valiables ----------------------------------------------------*/

/*--- Global Variables ------------------------------------------------------*/

/*--- Local Variables -------------------------------------------------------*/

/*--- Global Functions ------------------------------------------------------*/

/*****************************************************************************/
/* void amMatrixPush(const NNS_MATRIX *mtx)                                  */
/*---------------------------------------------------------------------------*/
/* [INPUT]  mtx : プッシュしたいマトリクスへのポインタ                       */
/* [FUNCTION]  カレントマトリクススタックへのプッシュ                        */
/*****************************************************************************/
void amMatrixPush(const NNS_MATRIX *mtx)
{
	nnPushMatrix(_amMatrixGetCurrentStack(), mtx);
}


/*****************************************************************************/
/* void amMatrixPop(void)                                                    */
/*---------------------------------------------------------------------------*/
/* [FUNCTION]  カレントマトリクススタックへのポップ                          */
/*****************************************************************************/
void amMatrixPop(void)
{
	nnPopMatrix(_amMatrixGetCurrentStack());
}


/*****************************************************************************/
/* NNS_MATRIX *amMatrixGetCurrent(void)                                      */
/*---------------------------------------------------------------------------*/
/* [FUNCTION]  カレントマトリクススタックのカレントマトリクスの取得          */
/*****************************************************************************/
NNS_MATRIX *amMatrixGetCurrent(void)
{
	return	nnGetCurrentMatrix(_amMatrixGetCurrentStack());
}


/*****************************************************************************/
/* void amMatrixClearStack(void)                                             */
/*---------------------------------------------------------------------------*/
/* [FUNCTION]  カレントマトリクススタックのクリア                            */
/*****************************************************************************/
void amMatrixClearStack(void)
{
	nnClearMatrixStack(_amMatrixGetCurrentStack());
}


/*****************************************************************************/
/* void amMatrixCalcPoint(AMS_VECTOR* pDst, const AMS_VECTOR* pSrc)          */
/*---------------------------------------------------------------------------*/
/* [FUNCTION]  カレントマトリクスでベクトルを変換する（平行移動成分を加算）  */
/*****************************************************************************/
void amMatrixCalcPoint(AMS_VECTOR* pDst, const AMS_VECTOR* pSrc)
{
	amAssert( pDst );
	amAssert( pSrc );
    AMS_MATRIX* m;

	m = amMatrixGetCurrent();
	nnTransformVector((NNS_VECTOR*)pDst, m, (NNS_VECTOR*)pSrc);
	pDst->w = pSrc->w;
}


/*****************************************************************************/
/* void amMatrixCalcVector(AMS_VECTOR* pDst, const AMS_VECTOR* pSrc)         */
/*---------------------------------------------------------------------------*/
/* [FUNCTION]  カレントマトリクスでベクトル変換（平行移動成分は加算しない）  */
/*****************************************************************************/
void amMatrixCalcVector(AMS_VECTOR* pDst, const AMS_VECTOR* pSrc)
{
    amAssert( pDst );
	amAssert( pSrc );
    AMS_MATRIX* m;

	m = amMatrixGetCurrent();
	nnTransformNormalVector((NNS_VECTOR*)pDst, m, (NNS_VECTOR*)pSrc);
}



/*--- Local Functions -------------------------------------------------------*/

/*****************************************************************************/
/* NNS_MATRIXSTACK *_amMatrixGetCurrentStack(void)                           */
/*---------------------------------------------------------------------------*/
/* [RETURN]  カレントマトリクススタックへのポインタ                          */
/* [FUNCTION]  カレントマトリクススタックの取得                              */
/*****************************************************************************/
NNS_MATRIXSTACK *_amMatrixGetCurrentStack(void)
{
#if AMD_TASK_THREAD_NUM == 1
#if AMD_USE_DRAW_THREAD
#if !_IPHONE
	return	amThreadCheckDraw()? &_am_draw_stack: &_am_default_stack;
#else
	return	amThreadCheckDraw()? &_am_default_stack: &_am_game_stack;
#endif
#else
	return	&_am_default_stack;
#endif
#else
	AMS_THREAD_ID	thread_id = amThreadGetCurrentID();
#if AMD_USE_DRAW_THREAD
	if (thread_id == _am_draw_thread_id)
		return	&_am_draw_stack;
#endif
#if AMD_TASK_THREAD_NUM > 1
	AMS_TCB_THREAD	*thread;
	Sint32		i;
	thread		= &_am_default_taskp->tcb_thread[0];
	for (i = 0; i < AMD_TASK_THREAD_NUM; i++, thread++) {
		if (thread->thread_id == thread_id)
			return	&thread->matrix_stack;
	}
#endif
	return	&_am_default_stack;
#endif
}

