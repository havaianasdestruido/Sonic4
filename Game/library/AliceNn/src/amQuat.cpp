// ================================================================
/*!
    Copyright(c) 2009 Dimps CORP. All Rights Reserved. 

	@file       amQuat.cpp
	@brief      クォータニオンライブラリ
	@author	    Syuichi Gotou
	@date	    Date: 2009/04/01 
 */
// ================================================================

//----- Include Files --------------------------------------------------
#include "alice.h"

//----- Macros ---------------------------------------------------------

//----- Enum Definitions -----------------------------------------------

//----- Type Definitions -----------------------------------------------

//----- Local Functions -----------------------------------------------

// ================================================================
/*!
	クォータニオン -> オイラー角 (Radian) 変換

	@param	rx   [o]		角度 (X-Radian)
	@param	ry   [o]		角度 (Y-Radian)
	@param	rz   [o]		角度 (Z-Radian)
	@param	pQuat[i]		変換元クォータニオン
*/
// ================================================================
void amQuatToEulerXYZ(float* rx, float* ry, float* rz, const AMS_QUAT* pQuat)
{
	AMS_QUAT tquat;

	amQuatUnit(&tquat, pQuat);

	float xx = tquat.x * tquat.x;
	float xy = tquat.x * tquat.y;
	float xz = tquat.x * tquat.z;
	float xw = tquat.x * tquat.w;
	float yy = tquat.y * tquat.y;
	float yz = tquat.y * tquat.z;
	float yw = tquat.y * tquat.w;
	float zz = tquat.z * tquat.z;
	float zw = tquat.z * tquat.w;
	float sinz, cosz, sinx, cosx;

	float x = 1.0f - (2.0f * (yy + zz));
	float y = 2.0f * (xy + zw);
	float z = 2.0f * (xz - yw);
	float w = 1.0f - amPow2(z);

	(*ry) = atan2f(-z, ((w > 0.0f) ? (sqrtf(w)): (0.0f)));

	w     = sqrtf(amPow2(x) + amPow2(y));

	if (amIsZerof(w)) {
		sinz = 0.0f;
		cosz = 1.0f;
	}
	else {
		sinz = y / w;
		cosz = x / w;
	}

	float m12 = 2.0f * (xy - zw);
	float m13 = 2.0f * (xz + yw);
	float m22 = 1.0f - (2.0f * (xx + zz));
	float m23 = 2.0f * (yz - xw);

	sinx = m13 * sinz - m23 * cosz;
	cosx = m22 * cosz - m12 * sinz;

	(*rx) = atan2f(sinx, cosx);
	(*rz) = atan2f(sinz, cosz);
}

// ================================================================
/*!
	クォータニオン -> オイラー角 (Angle) 変換

	@param	ax   [o]		角度 (X-Angle)
	@param	ay   [o]		角度 (Y-Angle)
	@param	az   [o]		角度 (Z-Angle)
	@param	pQuat[i]		変換元クォータニオン
*/
// ================================================================
void amQuatToEulerXYZ(Angle32* ax, Angle32* ay, Angle32* az, const AMS_QUAT* pQuat)
{
	float rx, ry, rz;

	amQuatToEulerXYZ(&rx, &ry, &rz, pQuat);

	(*ax) = NNM_RADtoA32(rx);
	(*ay) = NNM_RADtoA32(ry);
	(*az) = NNM_RADtoA32(rz);
}

// ================================================================
/*!
	回転元ベクトルと回転先ベクトル -> クォータニオン 変換

	@param	pQuat [o]		変換先クォータニオン
	@param	pV1   [i]		回転元ベクトル
	@param	pV2   [i]		回転先ベクトル
	
	@note			回転元ベクトルから回転先ベクトルに回転させるクォータニオンを生成する
					ベクトルは正規化されている必要がある
*/
// ================================================================
void amQuatVectorToQuat(AMS_QUAT* pQuat, const AMS_VECTOR* pV1, const AMS_VECTOR* pV2)
{
	AMS_VECTOR3 c010, c020, c030, total;
	float f;

	VEC3_COPY(c010, *pV1);
	VEC3_COPY(c020, *pV2);
	nnAddVector(&total, &c010, &c020);
	f =  nnDotProductVector(&total, &total);
	f = 1.0f/sqrtf(f);//平方根の逆数
	
	nnScaleVector(&c020, &total, f);
	nnCrossProductVector(&c030, &c010, &c020);
	VEC3_COPY(*pQuat, c030);
	pQuat->w = nnDotProductVector(&c010, &c020);
}

// ================================================================
/*!
	回転軸と回転角度 (Radian) -> クォータニオン 変換

	@param	pQuat  [o]		変換先クォータニオン
	@param	pVect  [i]		回転軸ベクトル
	@param	radian [i]		回転角度 (Radian)
	
	@note			回転軸と回転角度からクォータニオンを生成する
	                回転軸ベクトルは正規化されている必要がある
*/
// ================================================================
void amQuatRotAxisToQuat(AMS_QUAT* pQuat, const AMS_VECTOR* pVec, float radian)
{
	float sn, cs;
	
	radian *= 0.5f;

	amSinCos(radian, &sn, &cs);

	pQuat->x = pVec->x * sn;
	pQuat->y = pVec->y * sn;
	pQuat->z = pVec->z * sn;
	pQuat->w = cs;
}

// ================================================================
/*!
	クォータニオンと平行移動成分 -> マトリクス 変換

	@param	pMtx  [o]		変換先マトリクス
							NULLの場合、カレントマトリクスに設定
	@param	pQuat [i]		マトリクスに変換するクォータニオン
	@param	pVec  [i]		マトリクスの平行移動成分
					        NULLの場合、平行移動なし
*/
// ================================================================
void amQuatToMatrix(AMS_MATRIX* pMtx, const AMS_QUAT* pQuat, const AMS_VECTOR* pVec)
{
	AMS_MATRIX* m;
	AMS_MATRIX mtx;

	if( pMtx == NULL )
	{
		m = amMatrixGetCurrent();
		nnMakeQuaternionMatrix(&mtx, pQuat);
		if( pVec != NULL ){
			nnCopyVectorMatrixTranslation( &mtx, (NNS_VECTOR*)pVec );
		}
		nnCopyMatrix(m, &mtx);
	}
	else
	{
		nnMakeQuaternionMatrix(pMtx, pQuat);
		if( pVec != NULL ){
			nnCopyVectorMatrixTranslation( &mtx, (NNS_VECTOR*)pVec );
		}
	}
}

// ================================================================
/*!
	クォータニオンをカレントマトリクスに乗算

	@param	pQuat [i]		カレントマトリクスに乗算するクォータニオン
	@param	pVec  [i]		乗算するマトリクスの平行移動成分
					        NULLの場合、平行移動なし
*/
// ================================================================
void amQuatMultiMatrix(const AMS_QUAT* pQuat, const AMS_VECTOR* pVec)
{
	amQuatMultiMatrix(pQuat, (NNS_VECTOR*)pVec);
}

void amQuatMultiMatrix(const AMS_QUAT* pQuat, const NNS_VECTOR* pVec)
{
	AMS_MATRIX m;
	AMS_MATRIX* cm = NULL;

	cm = amMatrixGetCurrent();
	nnMakeQuaternionMatrix(&m, pQuat);
	if( pVec != NULL ){
		nnCopyVectorMatrixTranslation( &m, pVec );
	}
	nnMultiplyMatrix(cm, cm, &m);
}

// ================================================================
/*!
	クォータニオンと平行移動で表される変換をベクトルに対して行う

	@param	pDst [o]		変換後ベクトル
	@param	pSrc [i]		変換元ベクトル
	@param	pQuat[i]		クォータニオン
	@param	pVec [i]		平行移動成分
					        NULLの場合、平行移動なし
*/
// ================================================================
void amQuatMultiVector(AMS_VECTOR* pDst, const AMS_VECTOR* pSrc, const AMS_QUAT* pQuat, const AMS_VECTOR* pVec)
{
	AMS_QUAT c000, c110, c120, c200;
	VEC4_COPY(c200, *pSrc);
	
	c110.x = -pQuat->x;
	c110.y = -pQuat->y;
	c110.z = -pQuat->z;
	c110.w =  pQuat->w;
	
	nnMultiplyQuaternion(&c120, pQuat, &c200);
	nnMultiplyQuaternion(&c000, &c120, &c110);
	if( pVec == NULL ){
		VEC4_COPY(*pDst, c000);
		return;
	}

	AMS_VECTOR v1, v2;
	VEC4_COPY(v1, c000);
	VEC4_COPY(v2, *pVec);
	amVectorAdd(pDst, &v1, &v2);
}



