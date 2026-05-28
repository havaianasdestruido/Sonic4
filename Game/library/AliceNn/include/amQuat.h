// ================================================================
/*!
    Copyright(c) 2009 Dimps CORP. All Rights Reserved. 

	@file       amQuat.h
	@brief      クォータニオンライブラリ ヘッダ
	@author	    Syuichi Gotou
	@date	    Date: 2009/04/01 
 */
// ================================================================
#ifndef _AM_QUAT_H
#define _AM_QUAT_H

//----- Definitions ----------------------------------------------------
//----- Macros ---------------------------------------------------------
#define QUAT_COPY(d_quat,s_vec)	\
{								\
	((d_quat).x) = ((s_vec).x);	\
	((d_quat).y) = ((s_vec).y);	\
	((d_quat).z) = ((s_vec).z);	\
	((d_quat).w) = ((s_vec).w);	\
}


//----- Enum Definitions -----------------------------------------------
//----- Type Definitions -----------------------------------------------
//----- External Definitions -------------------------------------------

//----- External Functions ---------------------------------------------

// クォータニオンの初期化
inline void amQuatInit(AMS_QUAT* pQuat)
{
	amAssert( pQuat );
    nnMakeUnitQuaternion( pQuat );
}

// ================================================================
/*!
	クォータニオンのコピー

	@param	pDst [o]		コピー先のクォータニオン
	@param	pSrc [i]		コピー元のクォータニオン
*/
// ================================================================
inline void amQuatCopy(AMS_QUAT* pDst, const AMS_QUAT* pSrc)
{
	amAssert( pDst );
	amAssert( pSrc );
	nnCopyQuaternion(pDst, pSrc);
}

// ================================================================
/*!
	逆クォータニオン

	@param	pDst [o]		計算先クォータニオン1
	@param	pSrc [i]		計算元クォータニオン2
*/
// ================================================================
inline void amQuatInverse(AMS_QUAT* pDst, AMS_QUAT* pSrc)
{
	amAssert( pDst );
	amAssert( pSrc );
	nnInvertQuaternion( pDst, pSrc );
}

// ================================================================
/*!
	クォータニオンの正規化

	@param	pDst [o]		計算先クォータニオン1
	@param	pSrc [i]		計算元クォータニオン2
*/
// ================================================================
inline void amQuatUnit(AMS_QUAT* pDst, const AMS_QUAT* pSrc)
{
	amAssert( pDst );
	amAssert( pSrc );
    nnNormalizeQuaternion( pDst, pSrc );
}

// ================================================================
/*!
	クォータニオンの乗算

	@param	pDst [o]		計算先クォータニオン
	@param	pQ1  [i]		計算元クォータニオン1
	@param	pQ2  [i]		計算元クォータニオン2
*/
// ================================================================
inline void amQuatMulti(AMS_QUAT* pDst, const AMS_QUAT* pQ1, const AMS_QUAT* pQ2)
{
	amAssert( pDst );
	amAssert( pQ1 );
	amAssert( pQ2 );
    nnMultiplyQuaternion(pDst, pQ1, pQ2);
}

// ================================================================
/*!
	任意軸回転クォータニオンの作成

	@param	pDst [o]		出力クォータニオンのポインタ
	@param	pV   [i]		回転軸を示すベクトルのポインタ
	@param	ang  [i]		回転する角度
*/
// ================================================================
inline void amQuatMakeRotateAxis(AMS_QUAT* pDst, AMS_VECTOR3* pV, Angle32 ang)
{
	amAssert( pDst );
	amAssert( pV );
	nnMakeRotateAxisQuaternion( pDst, pV->x, pV->y, pV->z, ang );
}

// ================================================================
/*!
	オイラー角 (Angle) -> クォータニオン 変換

	@param	pQuat [o]		XYZ軸の順に回転させるクォータニオン
	@param	ax    [i]		角度 (X-Angle)
	@param	ay    [i]		角度 (Y-Angle)
	@param	az    [i]		角度 (Z-Angle)
*/
// ================================================================
inline void amQuatEulerToQuatXYZ(AMS_QUAT* pQuat, Angle32 ax, Angle32 ay, Angle32 az)
{
	amAssert( pQuat );
	nnMakeRotateXYZQuaternion(pQuat, ax, ay, az);
}

// ================================================================
/*!
	オイラー角 (Radian) -> クォータニオン 変換

	@param	pQuat [o]		XYZ軸の順に回転させるクォータニオン
	@param	rx    [i]		角度 (X-Radian)
	@param	ry    [i]		角度 (Y-Radian)
	@param	rz    [i]		角度 (Z-Radian)
*/
// ================================================================
inline void amQuatEulerToQuatXYZ(AMS_QUAT* pQuat, float rx, float ry, float rz)
{
	amAssert( pQuat );
	nnMakeRotateXYZQuaternion(pQuat, NNM_RADtoA32(rx), NNM_RADtoA32(ry), NNM_RADtoA32(rz));
}

// ================================================================
/*!
	オイラー角 (Radian) -> クォータニオン 変換

	@param	pQuat [o]		XYZ軸の順に回転させるクォータニオン
	@param	pRot  [i]		ベクトル (X-Radian, Y-Radian, Z-Radian)
*/
// ================================================================
inline void amQuatEulerToQuatXYZ(AMS_QUAT* pQuat, const AMS_VECTOR* pRot)
{
	amAssert( pQuat );
	amAssert( pRot );
	nnMakeRotateXYZQuaternion(pQuat, NNM_RADtoA32(pRot->x), NNM_RADtoA32(pRot->y), NNM_RADtoA32(pRot->z));
}

// ================================================================
/*!
	回転マトリクス -> クォータニオン 変換

	@param	pQuat [o]		結果を格納するクォータニオン
	@param	pMtx  [i]		角度 (X-Angle)
*/
// ================================================================
inline void amQuatMatrixToQuat(AMS_QUAT* pQuat, const AMS_MATRIX* pMtx)
{
	amAssert( pQuat );
	amAssert( pMtx );
    nnMakeRotateMatrixQuaternion( pQuat, pMtx );
}

// ================================================================
/*!
	任意クォータニオン間を補間割合で補間したクォータニオンを求める

	@param	pDst [o]		出力クォータニオンのポインタ
	@param	pQ1  [i]		入力クォータニオン1のポインタ
	@param	pQ2  [i]		入力クォータニオン2のポインタ
	@param	per  [i]		補間のパラメータ
*/
// ================================================================
inline void amQuatLerp(AMS_QUAT* pDst, const AMS_QUAT* pQ1, const AMS_QUAT* pQ2, float per)
{
    amAssert( pDst );
	amAssert( pQ1 );
	amAssert( pQ2 );
	nnLerpQuaternion( pDst, pQ1, pQ2, per );
}

// ================================================================
/*!
	任意クォータニオン間を補間割合で補間したクォータニオンを求める（正規化して返す）

	@param	pDst [o]		出力クォータニオンのポインタ
	@param	pQ1  [i]		入力クォータニオン1のポインタ
	@param	pQ2  [i]		入力クォータニオン2のポインタ
	@param	per  [i]		補間のパラメータ(0.0f～1.0f)
*/
// ================================================================
inline void amQuatUnitLerp(AMS_QUAT* pDst, const AMS_QUAT* pQ1, const AMS_QUAT* pQ2, float per)
{
	amAssert(pDst);
	amAssert( pQ1 );
	amAssert( pQ2 );
	AMS_QUAT	quat;
    amQuatLerp( &quat, pQ1, pQ2, per );
	nnNormalizeQuaternion( pDst, &quat );
}

// ================================================================
/*!
	球形線形補間を使用して、クォータニオン間を補間

	@param	pDst [o]		出力クォータニオンのポインタ
	@param	pQ1  [i]		入力クォータニオン1のポインタ
	@param	pQ2  [i]		入力クォータニオン2のポインタ
	@param	per  [i]		補間のパラメータ(0.0f～1.0f)
*/
// ================================================================
inline void amQuatSlerp(AMS_QUAT* pDst, AMS_QUAT* pQ1, AMS_QUAT* pQ2, float per)
{
	amAssert( pDst );
	amAssert( pQ1 );
	amAssert( pQ2 );
	nnSlerpQuaternion( pDst, pQ1, pQ2, per );
}

// ================================================================
/*!
	球面三次補間を使用して、クォータニオン間を補間

	@param	pDst [o]		出力クォータニオンのポインタ
	@param	pQ1  [i]		入力クォータニオン1のポインタ
	@param	pQ2  [i]		入力クォータニオン2のポインタ
	@param	pQ3  [i]		入力クォータニオン3のポインタ
	@param	pQ4  [i]		入力クォータニオン4のポインタ
	@param	t    [i]		補間のパラメータ(0.0f～1.0f)
*/
// ================================================================
inline void amQuatSquad(AMS_QUAT* pDst, AMS_QUAT* pQ1, AMS_QUAT* pQ2, AMS_QUAT* pQ3, AMS_QUAT* pQ4, float t)
{
	amAssert( pDst );
	amAssert( pQ1 );
	amAssert( pQ2 );
	amAssert( pQ3 );
	amAssert( pQ4 );
	nnSquadQuaternion( pDst, pQ1, pQ2, pQ3, pQ4, t );
}

// ================================================================
/*!
	クォータニオンの要素を設定

	@param	pDst [o]		設定先のクォータニオン
	@param	x    [i]		設定するX 値
	@param	y    [i]		設定するY 値
	@param	z    [i]		設定するZ 値
	@param	w    [i]		設定するW 値
*/
// ================================================================
inline void amQuatSet(AMS_QUAT* pDst, float x, float y, float z, float w)
{
	pDst->x = x;
	pDst->y = y;
	pDst->z = z;
	pDst->w = w;
}

// ================================================================
/*!
	クォータニオン -> オイラー角 (Radian) 変換

	@param	rx   [o]		角度 (X-Radian)
	@param	ry   [o]		角度 (Y-Radian)
	@param	rz   [o]		角度 (Z-Radian)
	@param	pQuat[i]		変換元クォータニオン
*/
// ================================================================
extern void amQuatToEulerXYZ(float* rx, float* ry, float* rz, const AMS_QUAT* pQuat);

// ================================================================
/*!
	クォータニオン -> オイラー角 (Angle) 変換

	@param	ax   [o]		角度 (X-Angle)
	@param	ay   [o]		角度 (Y-Angle)
	@param	az   [o]		角度 (Z-Angle)
	@param	pQuat[i]		変換元クォータニオン
*/
// ================================================================
extern void amQuatToEulerXYZ(Angle32* ax, Angle32* ay, Angle32* az, const AMS_QUAT* pQuat);

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
extern void amQuatVectorToQuat(AMS_QUAT* pQuat, const AMS_VECTOR* pV1, const AMS_VECTOR* pV2);

// ================================================================
/*!
	回転軸と回転角度 (Radian) -> クォータニオン 変換

	@param	pQuat  [o]		変換先クォータニオン
	@param	pVec   [i]		回転軸ベクトル
	@param	radian [i]		回転角度 (Radian)
	
	@note			回転軸と回転角度からクォータニオンを生成する
	                回転軸ベクトルは正規化されている必要がある
*/
// ================================================================
extern void amQuatRotAxisToQuat(AMS_QUAT* pQuat, const AMS_VECTOR* pVec, float radian);

// ================================================================
/*!
	回転軸と回転角度 (Angle) -> クォータニオン 変換

	@param	pQuat [o]		変換先クォータニオン
	@param	pVec  [i]		回転軸ベクトル
	@param	angle [i]		回転角度 (Angle)
	
	@note			回転軸と回転角度からクォータニオンを生成する
					回転軸は正規化されている必要がある
*/
// ================================================================
inline void amQuatRotAxisToQuat(AMS_QUAT* pQuat, const AMS_VECTOR* pVec, Angle32 angle)
{
	amQuatRotAxisToQuat(pQuat, pVec, NNM_A32toRAD(angle));
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
extern void amQuatToMatrix(AMS_MATRIX* pMtx, const AMS_QUAT* pQuat, const AMS_VECTOR* pVec);


// ================================================================
/*!
	クォータニオンをカレントマトリクスに乗算

	@param	pQuat [i]		カレントマトリクスに乗算するクォータニオン
	@param	pVec  [i]		乗算するマトリクスの平行移動成分
					        NULLの場合、平行移動なし
*/
// ================================================================
extern void amQuatMultiMatrix(const AMS_QUAT* pQuat, const AMS_VECTOR* pVec);
extern void amQuatMultiMatrix(const AMS_QUAT* pQuat, const NNS_VECTOR* pVec);

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
extern void amQuatMultiVector(AMS_VECTOR* pDst, const AMS_VECTOR* pSrc, const AMS_QUAT* pQuat, const AMS_VECTOR* pVec);


#endif	// _AM_QUAT_H
