// ================================================================
/*!
    Copyright(c) 2009 Dimps CORP. All Rights Reserved. 

	@file       amVector.h
	@brief      ベクトルライブラリ ヘッダ
	@author	    Syuichi Gotou
	@date	    Date: 2009/04/01 
 */
// ================================================================
#ifndef _AM_VECTOR_H
#define _AM_VECTOR_H


//----- Include Files --------------------------------------------------

//----- Definitions ----------------------------------------------------

#define VEC3_COPY(d_vec, s_vec)	\
{								\
	((d_vec).x) = ((s_vec).x);	\
	((d_vec).y) = ((s_vec).y);	\
	((d_vec).z) = ((s_vec).z);	\
}

#define VEC4_COPY(d_vec, s_vec)	\
{								\
	((d_vec).x) = ((s_vec).x);	\
	((d_vec).y) = ((s_vec).y);	\
	((d_vec).z) = ((s_vec).z);	\
	((d_vec).w) = ((s_vec).w);	\
}

#define VEC4_NEG(d_vec, s_vec)	\
{								\
	((d_vec).x) = -((s_vec).x);	\
	((d_vec).y) = -((s_vec).y);	\
	((d_vec).z) = -((s_vec).z);	\
	((d_vec).w) = -((s_vec).w);	\
}



//----- Type Definitions -----------------------------------------------

//----- External Functions ---------------------------------------------

// ================================================================
/*!
	ベクトルの初期化

	@param	pVec  [o]		初期化するベクトル

	@note   pVec = <0, 0, 0, 1> で初期化する
*/
// ================================================================
inline void amVectorInit(NNS_VECTOR* pVec)
{ 
	amAssert(pVec);

	pVec->x = 0.0f;
	pVec->y = 0.0f;
	pVec->z = 0.0f;
}

// ================================================================
/*!
	ベクトルの初期化

	@param	pVec  [o]		初期化するベクトル

	@note   pVec = <0, 0, 0, 1> で初期化する
*/
// ================================================================
inline void amVectorInit(AMS_VECTOR* pVec)
{ 
	amAssert(pVec);

	pVec->x = 0.0f;
	pVec->y = 0.0f;
	pVec->z = 0.0f;
	pVec->w = 1.0f; 
}

// ================================================================
/*!
	ベクトルの要素をすべて１

	@param	pVec [o]		設定先のベクトル
*/
// ================================================================
inline void amVectorOne(AMS_VECTOR* pVec)
{
	amAssert(pVec);

	pVec->x = 1.0f;
	pVec->y = 1.0f;
	pVec->z = 1.0f;
	pVec->w = 1.0f;
}

// ================================================================
/*!
	ベクトルの要素を設定

	@param	pDst [o]		設定先のベクトル
	@param	x    [i]		設定するX 値
	@param	y    [i]		設定するY 値
	@param	z    [i]		設定するZ 値
	
	@note			pVec = <x, y, z, 1>
*/
// ================================================================
inline void amVectorSet(NNS_VECTOR* pDst, float x, float y, float z)
{
	pDst->x = x;
	pDst->y = y;
	pDst->z = z;
}

// ================================================================
/*!
	ベクトルの要素を設定

	@param	pDst [o]		設定先のベクトル
	@param	x    [i]		設定するX 値
	@param	y    [i]		設定するY 値
	
	@note			pVec = <x, y, z, 1>
*/
// ================================================================
inline void amVectorSet(NNS_VECTOR2D* pDst, float x, float y)
{
	pDst->x = x;
	pDst->y = y;
}

// ================================================================
/*!
	ベクトルの要素を設定

	@param	pDst [o]		設定先のベクトル
	@param	x    [i]		設定するX 値
	@param	y    [i]		設定するY 値
	@param	z    [i]		設定するZ 値
	
	@note			pVec = <x, y, z, 1>
*/
// ================================================================
inline void amVectorSet(AMS_VECTOR* pDst, float x, float y, float z)
{
	pDst->x = x;
	pDst->y = y;
	pDst->z = z;
	pDst->w = 1.0f;
}

// ================================================================
/*!
	ベクトルの要素を設定

	@param	pDst [o]		設定先のベクトル
	@param	x    [i]		設定するX 値
	@param	y    [i]		設定するY 値
	@param	z    [i]		設定するZ 値
	@param	w    [i]		設定するW 値
	
	@note			pVec = <x, y, z, w>
*/
// ================================================================
inline void amVectorSet(AMS_VECTOR* pDst, float x, float y, float z, float w)
{
	pDst->x = x;
	pDst->y = y;
	pDst->z = z;
	pDst->w = w;
}

// ================================================================
/*!
	ベクトルのコピー

	@param	pDst [o]		コピー先のベクトル
	@param	pSrc [i]		コピー元のベクトル
*/
// ================================================================
inline void amVectorCopy(AMS_VECTOR* pDst, const AMS_VECTOR* pSrc)
{ 
	amAssert(pDst);
	amAssert(pSrc);

	pDst->x = pSrc->x;
	pDst->y = pSrc->y;
	pDst->z = pSrc->z;
	pDst->w = pSrc->w;
}

// ================================================================
/*!
	ベクトルの加算

	@param	pDst [o]		加算結果のベクトル
	@param	pV1  [i]		加算元のベクトル
	@param	pV2  [i]		加算元のベクトル

	@note	pDst = <pV1->x + pV2->x, pV1->y + pV2->y, pV1->z + pV2->z, pV1->w>
*/
// ================================================================
inline void amVectorAdd(AMS_VECTOR* pDst, const AMS_VECTOR* pV1, const AMS_VECTOR* pV2)
{   
	amAssert(pDst);
	amAssert(pV1);
	amAssert(pV2);

	pDst->x = pV1->x + pV2->x;
	pDst->y = pV1->y + pV2->y;
	pDst->z = pV1->z + pV2->z;
	pDst->w = pV1->w;
}

// ================================================================
/*!
	ベクトルの加算

	@param	pDst [o]		加算結果のベクトル
	@param	pV1  [i]		加算元のベクトル
	@param	pV2  [i]		加算元のベクトル

	@note	pDst = <pV1->x + pV2->x, pV1->y + pV2->y, pV1->z + pV2->z, pV1->w>
*/
// ================================================================
inline void amVectorAdd(NNS_VECTOR* pDst, const AMS_VECTOR* pV1, const NNS_VECTOR* pV2)
{   
	amAssert(pDst);
	amAssert(pV1);
	amAssert(pV2);

	pDst->x = pV1->x + pV2->x;
	pDst->y = pV1->y + pV2->y;
	pDst->z = pV1->z + pV2->z;
}

// ================================================================
/*!
	ベクトルの加算

	@param	pDst [o]		加算結果のベクトル
	@param	pSrc [i]		加算元のベクトル
	@param	x    [i]		加算するX 値
	@param	y    [i]		加算するY 値
	@param	z    [i]		加算するZ 値

	@note	pDst = <pSrc->x + x, pSrc->y + y, pSrc->z + z, pSrc->w>
*/
// ================================================================
inline void amVectorAdd(AMS_VECTOR* pDst, const AMS_VECTOR* pSrc, float x, float y, float z)
{
	amAssert(pDst);
	amAssert(pSrc);

	pDst->x = pSrc->x + x;
	pDst->y = pSrc->y + y;
	pDst->z = pSrc->z + z;
	pDst->w = pSrc->w;
}

// ================================================================
/*!
	ベクトルの加算

	@param	pDst [io]	加算先のベクトル
	@param	pSrc [i]	加算元のベクトル

	@note	pDst = <pDst->x + pSrc->x, pDst->y + pSrc->y, pDst->z + pSrc->z, pDst->w>
*/
// ================================================================
inline void amVectorAdd(AMS_VECTOR* pDst, const AMS_VECTOR* pSrc)
{
	amAssert(pDst);
	amAssert(pSrc);

	pDst->x += pSrc->x;
	pDst->y += pSrc->y;
	pDst->z += pSrc->z;
}

// ================================================================
/*!
	ベクトルの加算

	@param	pDst [io]	加算先のベクトル
	@param	x    [i]		加算するX 値
	@param	y    [i]		加算するY 値
	@param	z    [i]		加算するZ 値

	@note	pDst = <pDst->x + x, pDst->y + y, pDst->z + z, pDst->w>
*/
// ================================================================
inline void amVectorAdd(AMS_VECTOR* pDst, float x, float y, float z)
{
	amAssert(pDst);

	pDst->x += x;
	pDst->y += y;
	pDst->z += z;
}

// ================================================================
/*!
	ベクトルの減算

	@param	pDst [o]		減算結果のベクトル
	@param	pV1  [i]		減算元のベクトル
	@param	pV2  [i]		減算元のベクトル

	@note	pDst = <pV1->x - pV2->x, pV1->y - pV2->y, pV1->z - pV2->z, pV1->w>
*/
// ================================================================
inline void amVectorSub(AMS_VECTOR* pDst, const AMS_VECTOR* pV1, const AMS_VECTOR* pV2)
{
	amAssert(pDst);
	amAssert(pV1);
	amAssert(pV2);

	pDst->x = pV1->x - pV2->x;
	pDst->y = pV1->y - pV2->y;
	pDst->z = pV1->z - pV2->z;
	pDst->w = pV1->w;
}

// ================================================================
/*!
	ベクトルの減算

	@param	pDst [o]		減算結果のベクトル
	@param	pSrc [i]		減算元のベクトル
	@param	x    [i]		減算するX 値
	@param	y    [i]		減算するY 値
	@param	z    [i]		減算するZ 値

	@note   pDst = <pSrc->x + x, pSrc->y + y, pSrc->z + z, pSrc->w>
*/
// ================================================================
inline void amVectorSub( AMS_VECTOR* pDst, const AMS_VECTOR* pSrc, float x, float y, float z )
{
	amAssert(pDst);
	amAssert(pSrc);

	pDst->x = pSrc->x - x;
	pDst->y = pSrc->y - y;
	pDst->z = pSrc->z - z;
	pDst->w = pSrc->w;
}

// ================================================================
/*!
	ベクトルの減算

	@param	pDst [io]		減算先のベクトル
	@param	pSrc [i]		減算元のベクトル

	@note	pDst = <pDst->x - pSrc->x, pDst->y - pSrc->y, pDst->z - pSrc->z, pDst->w>
*/
// ================================================================
inline void amVectorSub(AMS_VECTOR* pDst, const AMS_VECTOR* pSrc)
{
	amAssert(pDst);
	amAssert(pSrc);

	pDst->x -= pSrc->x;
	pDst->y -= pSrc->y;
	pDst->z -= pSrc->z;
}

// ================================================================
/*!
	ベクトルの減算

	@param	pDst [io]	    減算先のベクトル
	@param	x    [i]		減算するX 値
	@param	y    [i]		減算するY 値
	@param	z    [i]		減算するZ 値

	@note	pDst = <pDst->x - x, pDst->y - y, pDst->z - z, pDst->w>
*/
// ================================================================
inline void amVectorSub(AMS_VECTOR* pDst, float x, float y, float z)
{
	amAssert(pDst);

	pDst->x -= x;
	pDst->y -= y;
	pDst->z -= z;
}

// ================================================================
/*!
	ベクトルの補間

	@param	pDst [o]		補間結果のベクトル
	@param	pV1  [i]		補間割合 0.0 時のベクトル
	@param	pV2  [i]		補間割合 1.0 時のベクトル
	@param	per  [i]		補間割合 0.0 - 1.0

	@note	pDst = pV1 * (1.0 - per) + pV2 * per
*/
// ================================================================
inline void amVectorGetInner(NNS_VECTOR* pDst, const NNS_VECTOR* pV1, const NNS_VECTOR* pV2, float per)
{
	amAssert(pDst);
	amAssert(pV1);
	amAssert(pV2);
	amAssert(per >= 0.0f && per <= 1.0f);

	float nper = 1.0f - per;
	pDst->x = pV1->x * nper + pV2->x * per;
	pDst->y = pV1->y * nper + pV2->y * per;
	pDst->z = pV1->z * nper + pV2->z * per;
}

// ================================================================
/*!
	ベクトルの補間

	@param	pDst [o]		補間結果のベクトル
	@param	pV1  [i]		補間割合 0.0 時のベクトル
	@param	pV2  [i]		補間割合 1.0 時のベクトル
	@param	per  [i]		補間割合 0.0 - 1.0

	@note	pDst = pV1 * (1.0 - per) + pV2 * per
*/
// ================================================================
inline void amVectorGetInner(AMS_VECTOR* pDst, const AMS_VECTOR* pV1, const AMS_VECTOR* pV2, float per)
{
	amAssert(pDst);
	amAssert(pV1);
	amAssert(pV2);
	amAssert(per >= 0.0f && per <= 1.0f);

	float nper = 1.0f - per;
	pDst->x = pV1->x * nper + pV2->x * per;
	pDst->y = pV1->y * nper + pV2->y * per;
	pDst->z = pV1->z * nper + pV2->z * per;
	pDst->w = pV1->w;
}

// ================================================================
/*!
	ベクトルの拡縮加算（積和演算）

	@param	pDst [o]		拡縮加算結果のベクトル
	@param	pV1  [i]		加算するベクトル
	@param	pV2  [i]		加算するベクトル
	@param	p1   [i]		加算の比率
	@param	p2   [i]		加算の比率

	@note	pDst = pV1 * p1 + pV2 * p2
*/
// ================================================================
inline void amVectorGetAverage(AMS_VECTOR* pDst, const AMS_VECTOR* pV1, const AMS_VECTOR* pV2, float p1, float p2)
{
	amAssert(pDst);
	amAssert(pV1);
	amAssert(pV2);

	pDst->x = pV1->x * p1 + pV2->x * p2;
	pDst->y = pV1->y * p1 + pV2->y * p2;
	pDst->z = pV1->z * p1 + pV2->z * p2;
	pDst->w = pV1->w;
}

// ================================================================
/*!
	ベクトルの拡縮加算（積和演算）

	@param	pDst [o]		拡縮加算結果のベクトル
	@param	pV1  [i]		加算するベクトル
	@param	pV2  [i]		加算するベクトル
	@param	p1   [i]		加算の比率
	@param	p2   [i]		加算の比率

	@note	pDst = pV1 * p1 + pV2 * p2
*/
// ================================================================
inline void amVectorGetAverage(NNS_VECTOR* pDst, const NNS_VECTOR* pV1, const NNS_VECTOR* pV2, float p1, float p2)
{
	amAssert(pDst);
	amAssert(pV1);
	amAssert(pV2);

	pDst->x = pV1->x * p1 + pV2->x * p2;
	pDst->y = pV1->y * p1 + pV2->y * p2;
	pDst->z = pV1->z * p1 + pV2->z * p2;
}

// ================================================================
/*!
	2点間の距離を求める

	@param	pV1 [i]		距離を求めるベクトル
	@param	pV2 [i]		距離を求めるベクトル

	@return	2点間の距離
*/
// ================================================================
inline float amVectorGetLength(const AMS_VECTOR* pV1, const AMS_VECTOR* pV2)
{
	amAssert(pV1);
	amAssert(pV2);

	float x = pV1->x - pV2->x;
	float y = pV1->y - pV2->y;
	float z = pV1->z - pV2->z;
	float len = amSqrt(amPow2(x) + amPow2(y) + amPow2(z));

	return len;
}

// ================================================================
/*!
	2点間の距離の2乗を求める

	@param	pV1 [i]		距離の２乗を求めるベクトル
	@param	pV2 [i]		距離の２乗を求めるベクトル

	@return 		2点間の距離の2乗
*/
// ================================================================
inline float amVectorGetLength2(const AMS_VECTOR* pV1, const AMS_VECTOR* pV2)
{
	amAssert(pV1);
	amAssert(pV2);

	float x = pV1->x - pV2->x;
	float y = pV1->y - pV2->y;
	float z = pV1->z - pV2->z;
	float len = amPow2(x) + amPow2(y) + amPow2(z);

	return len;
}

// ================================================================
/*!
	ベクトルの長さを求める

	@param	pVec [i]		長さを求めるベクトル

	@return			ベクトルの長さ
*/
// ================================================================
inline float amVectorScalor(const AMS_VECTOR* pVec)
{
	amAssert(pVec);
	float len = amSqrt(amPow2(pVec->x) + amPow2(pVec->y) + amPow2(pVec->z));
	return len;
}

// ================================================================
/*!
	ベクトルの長さの２乗を求める

	@param	pVec [i]		長さの２乗を求めるベクトル

	@return			ベクトルの長さの２乗
*/
// ================================================================
inline float amVectorScalor2(const AMS_VECTOR* pVec)
{
	amAssert(pVec);
	float len = (amPow2(pVec->x) + amPow2(pVec->y) + amPow2(pVec->z));
	return len;
}

// ================================================================
/*!
	ベクトルを任意の長さに拡縮する

	@param	pDst [o]		拡縮結果のベクトル
	@param	pSrc [i]		拡縮元のベクトル
	@param	len  [i]		ベクトルの長さ

	@return			拡縮前のベクトルの長さ
*/
// ================================================================
inline float amVectorScaleUnit(AMS_VECTOR* pDst, const AMS_VECTOR* pSrc, float len)
{
	float ret = amSqrt(amPow2(pSrc->x) + amPow2(pSrc->y) + amPow2(pSrc->z));

	amVectorCopy(pDst, pSrc);
	if (! amIsZerof(ret))
	{
		len /= ret;
		pDst->x *= len;
		pDst->y *= len;
		pDst->z *= len;
	}

	return ret;
}

// ================================================================
/*!
	ベクトルを任意の長さに拡縮する

	@param	pDst [io]		拡縮先のベクトル
	@param	len  [i]		ベクトルの長さ
	
	@return			拡縮前のベクトルの長さ
*/
// ================================================================
inline float amVectorScaleUnit(AMS_VECTOR* pDst, float len)
{
	amAssert(pDst);

	float ret = amSqrt(amPow2(pDst->x) + amPow2(pDst->y) + amPow2(pDst->z));

	if (! amIsZerof(ret))
	{
		len /= ret;
		pDst->x *= len;
		pDst->y *= len;
		pDst->z *= len;
	}

	return ret;
}

// ================================================================
/*!
	ベクトルをスカラー倍する

	@param	pDst [o]		拡縮結果のベクトル
	@param	pSrc [i]		拡縮元のベクトル
	@param	sc   [i]		スケール
*/
// ================================================================
inline void amVectorScale(AMS_VECTOR* pDst, const AMS_VECTOR* pSrc, float sc)
{
	amAssert(pDst);
	amAssert(pSrc);

	pDst->x = pSrc->x * sc;
	pDst->y = pSrc->y * sc;
	pDst->z = pSrc->z * sc;
	pDst->w = pSrc->w;
}

// ================================================================
/*!
	ベクトルをスカラー倍する

	@param	pDst [io]		拡縮先のベクトル
	@param	sc   [i]		スケール
*/
// ================================================================
inline void amVectorScale(AMS_VECTOR* pDst, float sc)
{
	amAssert(pDst);

	pDst->x *= sc;
	pDst->y *= sc;
	pDst->z *= sc;
}

// ================================================================
/*!
	ベクトルの単位ベクトル化（正規化）

	@param	pDst [o]		単位ベクトル結果のベクトル
	@param	pSrc [i]		単位ベクトル元のベクトル

	@return			正規化前のベクトルの長さ 
*/
// ================================================================
inline float amVectorUnit(NNS_VECTOR* pDst, const AMS_VECTOR* pSrc)
{
	amAssert(pDst);
	amAssert(pSrc);

	float len = amSqrt(amPow2(pSrc->x) + amPow2(pSrc->y) + amPow2(pSrc->z));

	nnCopyVector(pDst, (NNS_VECTOR*)pSrc);
	if (! amIsZerof(len))
	{
		float rlen = 1.0f / len;
		pDst->x *= rlen;
		pDst->y *= rlen;
		pDst->z *= rlen;
	}

	return len;
}

inline float amVectorUnit(AMS_VECTOR* pDst, const AMS_VECTOR* pSrc)
{
	amAssert(pDst);
	amAssert(pSrc);
	pDst->w = pSrc->w;
	return amVectorUnit((NNS_VECTOR*)pDst, pSrc);
}

// ================================================================
/*!
	ベクトルの単位ベクトル化(正規化)

	@param	pDst [io]		単位ベクトル先のベクトル

	@return			正規化前のベクトルの長さ
*/
// ================================================================
inline float amVectorUnit(AMS_VECTOR* pDst)
{
	amAssert(pDst);

	float len = amSqrt(amPow2(pDst->x) + amPow2(pDst->y) + amPow2(pDst->z));

	if (! amIsZerof(len))
	{
		float rlen = 1.0f / len;
		pDst->x *= rlen;
		pDst->y *= rlen;
		pDst->z *= rlen;
	}

	return len;
}

// ================================================================
/*!
	ベクトルの反転

	@param	pDst [o]		反転結果のベクトル
	@param	pSrc [i]		反転元のベクトル
*/
// ================================================================
inline void amVectorInvert(AMS_VECTOR* pDst, const AMS_VECTOR* pSrc)
{
	amAssert(pDst);
	amAssert(pSrc);

	pDst->x = -pSrc->x;
	pDst->y = -pSrc->y;
	pDst->z = -pSrc->z;
	pDst->w =  pSrc->w;
}

// ================================================================
/*!
	ベクトルの反転

	@param	pDst [o]		反転結果のベクトル
	@param	pSrc [i]		反転元のベクトル
*/
// ================================================================
inline void amVectorInvert(NNS_VECTOR* pDst, const NNS_VECTOR* pSrc)
{
	amAssert(pDst);
	amAssert(pSrc);

	pDst->x = -pSrc->x;
	pDst->y = -pSrc->y;
	pDst->z = -pSrc->z;
}

// ================================================================
/*!
	ベクトルの反転

	@param	pVec [io]	反転先のベクトル
*/
// ================================================================
inline void amVectorInvert(AMS_VECTOR* pVec)
{
	amAssert(pVec);

	pVec->x = -pVec->x;
	pVec->y = -pVec->y;
	pVec->z = -pVec->z;
}

// ================================================================
/*!
	ベクトルの反転

	@param	pVec [io]	反転先のベクトル
*/
// ================================================================
inline void amVectorInvert(NNS_VECTOR* pVec)
{
	amAssert(pVec);

	pVec->x = -pVec->x;
	pVec->y = -pVec->y;
	pVec->z = -pVec->z;
}

// ================================================================
/*!
	ベクトルの内積を求める

	@param	pV1 [i]		内積を求めるベクトル
	@param	pV2 [i]		内積を求めるベクトル

	@return			ベクトルの内積
*/
// ================================================================
inline float amVectorInnerProduct(const AMS_VECTOR* pV1, const AMS_VECTOR* pV2)
{
	amAssert(pV1);
	amAssert(pV2);
	return (pV1->x * pV2->x + pV1->y * pV2->y + pV1->z * pV2->z);
}

// ================================================================
/*!
	ベクトルの外積を求める

	@param	pDst [o]		外積結果のベクトル
	@param	pV1  [i]		外積を求めるベクトル
	@param	pV2  [i]		外積を求めるベクトル
*/
// ================================================================
inline void amVectorOuterProduct(AMS_VECTOR* pDst, const AMS_VECTOR* pV1, const AMS_VECTOR* pV2)
{
	amAssert(pDst);
	amAssert(pV1);
	amAssert(pV2);

	amVectorSet( pDst,
		pV1->y * pV2->z - pV1->z * pV2->y,
		pV1->z * pV2->x - pV1->x * pV2->z,
		pV1->x * pV2->y - pV1->y * pV2->x);
}

// ================================================================
/*!
	ベクトルの各要素毎の乗算

	@param	pDst [o]		乗算結果のベクトル
	@param	pV1  [i]		乗算元のベクトル
	@param	pV2  [i]		乗算元のベクトル
*/
// ================================================================
inline void amVectorMul(AMS_VECTOR* pDst, const AMS_VECTOR* pV1, const AMS_VECTOR* pV2)
{
	amAssert(pDst);
	amAssert(pV1);
	amAssert(pV2);

	pDst->x = pV1->x * pV2->x;
	pDst->y = pV1->y * pV2->y;
	pDst->z = pV1->z * pV2->z;
	pDst->w = pV1->w;
}

// ================================================================
/*!
	ベクトルの各要素毎の乗算

	@param	pDst [io]		乗算先のベクトル
	@param	pSrc [i]		乗算元のベクトル
*/
// ================================================================
inline void amVectorMul(AMS_VECTOR* pDst, const AMS_VECTOR* pSrc)
{
	amAssert(pDst);
	amAssert(pSrc);

	pDst->x *= pSrc->x;
	pDst->y *= pSrc->y;
	pDst->z *= pSrc->z;
}

// ================================================================
/*!
	ベクトルの各要素毎の乗算

	@param	pDst [o]		乗算結果のベクトル
	@param	pSrc [i]		乗算元のベクトル
	@param	x    [i]		乗算するX 値
	@param	y    [i]		乗算するY 値
	@param	z    [i]		乗算するZ 値

	@note			pDst = <pSrc->x * x, pSrc->y * y, pSrc->z * z, pSrc->w>
*/
// ================================================================
inline void amVectorMul(AMS_VECTOR* pDst, const AMS_VECTOR* pSrc, float x, float y, float z)
{
	amAssert(pDst);
	amAssert(pSrc);

	pDst->x = pSrc->x * x;
	pDst->y = pSrc->y * y;
	pDst->z = pSrc->z * z;
	pDst->w = pSrc->w;
}

// ================================================================
/*!
	ベクトルの各要素毎の乗算

	@param	pDst [io]	乗算先のベクトル
	@param	x    [i]		乗算するX 値
	@param	y    [i]		乗算するY 値
	@param	z    [i]		乗算するZ 値

	@note			pDst = <pDst->x * x, pDst->y * y, pDst->z * z, pDst->w>
*/
// ================================================================
inline void amVectorMul(AMS_VECTOR* pDst, float x, float y, float z)
{
	amAssert(pDst);

	pDst->x *= x;
	pDst->y *= y;
	pDst->z *= z;
}

// ================================================================
/*!
	ベクトルの各要素毎に比較し、最大値ベクトルを求める

	@param	pDst [o]		比較結果のベクトル
	@param	pV1  [i]		比較元のベクトル
	@param	pV2  [i]		比較元のベクトル
	
	@note			pDst = <max(pV1->x, pV2->x), max(pV1->y, pV2->y),
					max(pV1->z, pV2->z), pV1->w>
*/
// ================================================================
inline void amVectorMax(AMS_VECTOR* pDst, const AMS_VECTOR* pV1, const AMS_VECTOR* pV2)
{
	amAssert(pDst);
	amAssert(pV1);
	amAssert(pV2);

	pDst->x = amMax(pV1->x, pV2->x);
	pDst->y = amMax(pV1->y, pV2->y);
	pDst->z = amMax(pV1->z, pV2->z);
	pDst->w = pV1->w;
}

// ================================================================
/*!
	ベクトルの各要素毎に比較し、最大値ベクトルを求める

	@param	pDst [o]		比較結果のベクトル
	@param	pSrc [i]		比較元のベクトル
	@param	val  [i]		最大値

	@note
	pDst = <max(pSrc->x, val), max(pSrc->y, val),
			max(pSrc->z, val), pSrc->w>
*/
// ================================================================
inline void amVectorMax(AMS_VECTOR* pDst, const AMS_VECTOR* pSrc, float val)
{
	amAssert(pDst);
	amAssert(pSrc);

	pDst->x = amMax(pSrc->x, val);
	pDst->y = amMax(pSrc->y, val);
	pDst->z = amMax(pSrc->z, val);
	pDst->w = pSrc->w;
}

// ================================================================
/*!
	ベクトルの各要素毎に比較し、最大値ベクトルを求める

	@param	pDst [io]	比較先のベクトル
	@param	pSrc [i]	比較元のベクトル

	@note			pDst = <max(pDst->x, pSrc->x), max(pDst->y, pSrc->y),
					max(pDst->z, pSrc->z), pDst->w>
*/
// ================================================================
inline void amVectorMax(AMS_VECTOR* pDst, const AMS_VECTOR* pSrc)
{
	amAssert(pDst);
	amAssert(pSrc);

	pDst->x = amMax(pDst->x, pSrc->x);
	pDst->y = amMax(pDst->y, pSrc->y);
	pDst->z = amMax(pDst->z, pSrc->z);
}

// ================================================================
/*!
	ベクトルの各要素毎に比較し、最大値ベクトルを求める

	@param	pDst [io]	比較先のベクトル
	@param	val  [i]	最大値

	@note			pDst = <max(pDst->x, val), max(pDst->y, val),
					max(pDst->z, val), pDst->w>
*/
// ================================================================
inline void amVectorMax(AMS_VECTOR* pDst, float val)
{
	amAssert(pDst);

	pDst->x = amMax(pDst->x, val);
	pDst->y = amMax(pDst->y, val);
	pDst->z = amMax(pDst->z, val);
}

// ================================================================
/*!
	ベクトルの各要素毎に比較し、最小値ベクトルを求める

	@param	pDst [o]		比較結果のベクトル
	@param	pV1  [i]		比較元のベクトル
	@param	pV2  [i]		比較元のベクトル

	@note			pDst = <min(pV1->x, pV2->x), min(pV1->y, pV2->y),
					min(pV1->z, pV2->z), pV1->w>
*/
// ================================================================
inline void amVectorMin(AMS_VECTOR* pDst, const AMS_VECTOR* pV1, const AMS_VECTOR* pV2)
{
	amAssert(pDst);
	amAssert(pV1);
	amAssert(pV2);

	pDst->x = amMin(pV1->x, pV2->x);
	pDst->y = amMin(pV1->y, pV2->y);
	pDst->z = amMin(pV1->z, pV2->z);
	pDst->w = pV1->w;
}

// ================================================================
/*!
	ベクトルの各要素毎に比較し、最小値ベクトルを求める

	@param	pDst [o]		比較結果のベクトル
	@param	pSrc [i]		比較元のベクトル
	@param	val  [i]		最小値

	@note			pDst = <min(pSrc->x, val), min(pSrc->y, val),
					min(pSrc->z, val), pSrc->w>
*/
// ================================================================
inline void amVectorMin(AMS_VECTOR* pDst, const AMS_VECTOR* pSrc, float val)
{
	amAssert(pDst);
	amAssert(pSrc);

	pDst->x = amMin(pSrc->x, val);
	pDst->y = amMin(pSrc->y, val);
	pDst->z = amMin(pSrc->z, val);
	pDst->w = pSrc->w;
}

// ================================================================
/*!
	ベクトルの各要素毎に比較し、最小値ベクトルを求める

	@param	pDst [io]	比較先のベクトル
	@param	pSrc [i]	比較元のベクトル

	@note			pDst = <min(pDst->x, pSrc->x), min(pDst->y, pSrc->y),
					min(pDst->z, pSrc->z), pDst->w>
*/
// ================================================================
inline void amVectorMin(AMS_VECTOR* pDst, const AMS_VECTOR* pSrc)
{
	amAssert(pDst);
	amAssert(pSrc);

	pDst->x = amMin(pDst->x, pSrc->x);
	pDst->y = amMin(pDst->y, pSrc->y);
	pDst->z = amMin(pDst->z, pSrc->z);
}

// ================================================================
/*!
	ベクトルの各要素毎に比較し、最小値ベクトルを求める

	@param	pDst [io]	比較先のベクトル
	@param	val  [i]		最小値

	@note			pDst = <min(pDst->x, val), min(pDst->y, val),
					min(pDst->z, val), pDst->w>
*/
// ================================================================
inline void amVectorMin(AMS_VECTOR* pDst, float val)
{
	amAssert(pDst);

	pDst->x = amMin(pDst->x, val);
	pDst->y = amMin(pDst->y, val);
	pDst->z = amMin(pDst->z, val);
}

// ================================================================
/*!
	ベクトルの各要素毎に比較し、指定範囲内に収まるベクトルを求める

	@param	pDst [o]		比較結果のベクトル
	@param	pSrc [i]		比較元のベクトル
	@param	pMin [i]		最小値ベクトル
	@param	pMax [i]		最大値ベクトル

	@note			pDst = <clamp(pSrc->x, pMin->x, pMax->x),
					clamp(pSrc->y, pMin->y, pMax->y),
					clamp(pSrc->z, pMin->z, pMax->z),
					pSrc->w>
*/
// ================================================================
inline void amVectorClamp(AMS_VECTOR* pDst, const AMS_VECTOR* pSrc, const AMS_VECTOR* pMin, const AMS_VECTOR* pMax )
{
	amAssert(pDst);
	amAssert(pSrc);
	amAssert(pMin);
	amAssert(pMax);

	pDst->x = amClamp(pSrc->x, pMin->x, pMax->x);
	pDst->y = amClamp(pSrc->y, pMin->y, pMax->y);
	pDst->z = amClamp(pSrc->z, pMin->z, pMax->z);
	pDst->w = pSrc->w;
}

// ================================================================
/*!
	ベクトルの各要素毎に比較し、指定範囲内に収まるベクトルを求める

	@param	pDst [o]		比較結果のベクトル
	@param	pSrc [i]		比較元のベクトル
	@param	min  [i]		最小値
	@param	max  [i]		最大値

	@note			pDst = <clamp(pSrc->x, min, max), clamp(pSrc->y, min, max),
					clamp(pSrc->z, min, max), pSrc->w>
*/
// ================================================================
inline void amVectorClamp(AMS_VECTOR* pDst, const AMS_VECTOR* pSrc, float min, float max)
{
	amAssert(pDst);
	amAssert(pSrc);

	pDst->x = amClamp(pSrc->x, min, max);
	pDst->y = amClamp(pSrc->y, min, max);
	pDst->z = amClamp(pSrc->z, min, max);
	pDst->w = pSrc->w;
}

// ================================================================
/*!
	ベクトルの各要素毎に比較し、指定範囲内に収まるベクトルを求める

	@param	pDst [o]		比較先のベクトル
	@param	pMin [i]		最小値ベクトル
	@param	pMax [i]		最大値ベクトル

	@note			pDst = <clamp(pDst->x, pMin->x, pMax->x),
					clamp(pDst->y, pMin->y, pMax->y),
					clamp(pDst->z, pMin->z, pMax->z),
					pSrc->w>
*/
// ================================================================
inline void amVectorClamp(AMS_VECTOR* pDst, const AMS_VECTOR* pMin, const AMS_VECTOR* pMax)
{
	amAssert(pDst);
	amAssert(pMin);
	amAssert(pMax);

	pDst->x = amClamp(pDst->x, pMin->x, pMax->x);
	pDst->y = amClamp(pDst->y, pMin->y, pMax->y);
	pDst->z = amClamp(pDst->z, pMin->z, pMax->z);
}

// ================================================================
/*!
	ベクトルの各要素毎に比較し、指定範囲内に収まるベクトルを求める

	@param	pDst [io]	比較先のベクトル
	@param	min  [i]		最小値
	@param	max  [i]		最大値

	@note			pDst = <clamp(pDst->x, min, max), clamp(pDst->y, min, max),
					clamp(pDst->z, min, max), clamp(pDst->w, min, max)>
*/
// ================================================================
inline void amVectorClamp(AMS_VECTOR* pDst, float min, float max)
{
	amAssert(pDst);

	pDst->x = amClamp(pDst->x, min, max);
	pDst->y = amClamp(pDst->y, min, max);
	pDst->z = amClamp(pDst->z, min, max);
}

// ================================================================
/*!
	浮動小数点数ベクトル -> 整数ベクトル 変換 (切り上げ)

	@param	pDst [o]		変換結果のベクトル
	@param	pSrc [i]		変換元のベクトル
*/
// ================================================================
inline void amVectorCeil(AMS_VECTOR4I* pDst, const AMS_VECTOR* pSrc)
{
	amAssert(pDst);
	amAssert(pSrc);

	pDst->x = (Sint32)ceil(pSrc->x);
	pDst->y = (Sint32)ceil(pSrc->y);
	pDst->z = (Sint32)ceil(pSrc->z);
	pDst->w = (Sint32)ceil(pSrc->w);
}

// ================================================================
/*!
	浮動小数点数ベクトル -> 整数ベクトル 変換 (０方向丸め)

	@param	pDst [o]		変換結果のベクトル
	@param	pSrc [i]		変換元のベクトル
*/
// ================================================================
inline void amVectorTrunc(AMS_VECTOR4I* pDst, const AMS_VECTOR* pSrc)
{
	amAssert(pDst);
	amAssert(pSrc);

	pDst->x = (Sint32)( ( pSrc->x >= 0 ) ? floor( pSrc->x ) : -floor( -pSrc->x ) );
	pDst->y = (Sint32)( ( pSrc->y >= 0 ) ? floor( pSrc->y ) : -floor( -pSrc->y ) );
	pDst->z = (Sint32)( ( pSrc->z >= 0 ) ? floor( pSrc->z ) : -floor( -pSrc->z ) );
	pDst->w = (Sint32)( ( pSrc->w >= 0 ) ? floor( pSrc->w ) : -floor( -pSrc->w ) );
}

// ================================================================
/*!
	浮動小数点数ベクトル -> 整数ベクトル変換 (四捨五入)

	@param	pDst [o]		変換結果のベクトル
	@param	pSrc [i]		変換元のベクトル
*/
// ================================================================
inline void amVectorRound(AMS_VECTOR4I* pDst, const AMS_VECTOR* pSrc)
{
	amAssert(pDst);
	amAssert(pSrc);

	float num	= 0.0f;
	float intp	= 0.0f;
	float dec	= amModf(pSrc->x, intp);

	if (dec >= 0.5) num = intp + 1.0f;
	else if (dec < 0.5) num = intp;
	pDst->x = (Sint32)num;

	dec = amModf(pSrc->y, intp);
	if (dec >= 0.5) num = intp + 1.0f;
	else if (dec < 0.5) num = intp;
	pDst->y = (Sint32)num;

	dec = amModf(pSrc->z, intp);
	if (dec >= 0.5) num = intp + 1.0f;
	else if (dec < 0.5) num = intp;
	pDst->z = (Sint32)num;

	dec = amModf(pSrc->w, intp);
	if (dec >= 0.5) num = intp + 1.0f;
	else if (dec < 0.5) num = intp;
	pDst->w = (Sint32)num;
}

// ================================================================
/*!
	浮動小数点数ベクトル -> 整数ベクトル変換 (切り捨て)

	@param	pDst [o]		変換結果のベクトル
	@param	pSrc [i]		変換元のベクトル
*/
// ================================================================
inline void amVectorFloor(AMS_VECTOR4I* pDst, const AMS_VECTOR* pSrc)
{
	amAssert(pDst);
	amAssert(pSrc);

	pDst->x = (Sint32)floor(pSrc->x);
	pDst->y = (Sint32)floor(pSrc->y);
	pDst->z = (Sint32)floor(pSrc->z);
	pDst->w = (Sint32)floor(pSrc->w);
}

// ================================================================
/*!
	整数ベクトル -> 浮動小数点数ベクトル 変換

	@param	pDst [o]		変換結果のベクトル
	@param	pSrc [i]		変換元のベクトル
*/
// ================================================================
inline void amVectorIntToFloat(AMS_VECTOR* pDst, const AMS_VECTOR4I* pSrc)
{
	amAssert(pDst);
	amAssert(pSrc);

	pDst->x = (float)pSrc->x;
	pDst->y = (float)pSrc->y;
	pDst->z = (float)pSrc->z;
	pDst->w = (float)pSrc->w;
}

// ================================================================
/*!
	ランダム方向のベクトル取得

	@param	pDst [o]		取得先のベクトル

	@note			範囲 -1.0 <= xyz < 1.0
*/
// ================================================================
inline void amVectorRandom(AMS_VECTOR* pDst)
{
	amAssert(pDst);

	amVectorSet(
		pDst,
		nnRandom() - 0.5f,		
		nnRandom() - 0.5f,		
		nnRandom() - 0.5f
		);
	amVectorUnit(pDst);
}

// ================================================================
/*!
	ベクトルの合同を調べる

	@param	[o]		出力先
	@param	[i]		代入元
	@return			１なら合同
*/
// ================================================================
inline Uint32 amVectorCmp(const AMS_VECTOR* pV1, const AMS_VECTOR* pV2)
{
	if(pV1->x == pV2->x && pV1->y == pV2->y && pV1->z == pV2->z && pV1->w == pV2->w)	return 1;
	else																				return 0;
}


#endif	// _AM_VECTOR_H