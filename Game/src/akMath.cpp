// =======================================================================
/*!
  @file akMath.h
  @brief 算術ライブラリ

  @author Keisuke Tanaka
 				Copyright(c) 2009 Dimps
  $Id: akMath.cpp 2 2011-04-11 05:21:26Z thamada $
 */
// =======================================================================
/*
 * $Log$
 */

/*------ Include Files -------------------------------------------------*/
#include "pch.h"
#include "akMath.h"

/*------ Macros --------------------------------------------------------*/

/*------ Macro Functions -----------------------------------------------*/

/*------ Definitions ---------------------------------------------------*/

/*------ External Declarations -----------------------------------------*/

/*------ Static Declarations -------------------------------------------*/

/*------ Global Variables ----------------------------------------------*/

/*------ Static Variables ----------------------------------------------*/

/*------ Global Functions ----------------------------------------------*/

// =======================================================================
// AkMathNormalizeMtx
/*!
  行列を正規化
 
  @param dst_mtx	[out]	格納先
  @param src_mtx	[in]	正規化したい行列
  
  @note
  平行移動成分は失われます。
  直行化の補正を行っていないため、数値上は歪むことになります。
  したがって、この結果を保存して別の計算に再利用（何度も繰り返し乗算するなど）しないでください。
  さもなくば、誤差が積もって回転が歪みます。
  使い捨てであれば、誤差は問題になりません。
 */
// =======================================================================
void AkMathNormalizeMtx(NNS_MATRIX *dst_mtx, const NNS_MATRIX *src_mtx)
{
#if 0
	AMS_QUAT	rot_quat;
	
	// クォータニオンに変換
	nnMakeRotateMatrixQuaternion(&rot_quat, src_mtx);
	
	// クォータニオン正規化
	if (NNE_FALSE == nnNormalizeQuaternion(&rot_quat, &rot_quat)) {
		MTM_ASSERT(!"gmBossCommon.cpp::gmBsCmnNormalizeRotationMtx() Error! Failed to normalize quaternion.\n");
	}
	
	// 正規化済みクォータニオンから行列を求める
	nnMakeQuaternionMatrix(dst_mtx, &rot_quat);
#else	// ↑スケールの入っている回転行列を渡すと上手くいかないので、↓行列から直接正規化する
	Float		inv;
	NNS_VECTOR	vec_x;
	NNS_VECTOR	vec_y;
	NNS_VECTOR	vec_z;
	
	amVectorSet(&vec_x,
				NNM_MTX(*src_mtx, 0, 0),
				NNM_MTX(*src_mtx, 0, 1),
				NNM_MTX(*src_mtx, 0, 2));
	amVectorSet(&vec_y,
				NNM_MTX(*src_mtx, 1, 0),
				NNM_MTX(*src_mtx, 1, 1),
				NNM_MTX(*src_mtx, 1, 2));
	amVectorSet(&vec_z,
				NNM_MTX(*src_mtx, 2, 0),
				NNM_MTX(*src_mtx, 2, 1),
				NNM_MTX(*src_mtx, 2, 2));
	
	nnMakeUnitMatrix(dst_mtx);
	
	inv			= 1.f / nnLengthVector(&vec_x);
	NNM_MTX(*dst_mtx, 0, 0)    = vec_x.x * inv;
    NNM_MTX(*dst_mtx, 0, 1)    = vec_x.y * inv;
	NNM_MTX(*dst_mtx, 0, 2)    = vec_x.z * inv;
	
	inv			= 1.f / nnLengthVector(&vec_y);
	NNM_MTX(*dst_mtx, 1, 0)    = vec_y.x * inv;
    NNM_MTX(*dst_mtx, 1, 1)    = vec_y.y * inv;
	NNM_MTX(*dst_mtx, 1, 2)    = vec_y.z * inv;
	
	inv			= 1.f / nnLengthVector(&vec_z);
	NNM_MTX(*dst_mtx, 2, 0)    = vec_z.x * inv;
    NNM_MTX(*dst_mtx, 2, 1)    = vec_z.y * inv;
	NNM_MTX(*dst_mtx, 2, 2)    = vec_z.z * inv;
#endif
}


// =======================================================================
// AkMathExtractScaleMtx
/*!
  スケールマトリクスを取り出す
  
  @param dst_mtx	[out]	格納先
  @param src_mtx	[in]	スケールを取り出したい行列
  
  @note
  指定した行列のスケール成分を取り出してスケールマトリクスを返します。
 */
// =======================================================================
void AkMathExtractScaleMtx(NNS_MATRIX *dst_mtx, const NNS_MATRIX *src_mtx)
{
	Float	scale_vec[MTD_XYZ];
	
	for (Sint32 i = 0; i < MTD_XYZ; ++i) {
		NNS_VECTOR	vec;
		amVectorSet(&vec,
					NNM_MTX(*src_mtx, i, 0),
					NNM_MTX(*src_mtx, i, 1),
					NNM_MTX(*src_mtx, i, 2));
		scale_vec[i]	= nnLengthVector(&vec);
	}
	
	nnMakeScaleMatrix(dst_mtx,
					  scale_vec[MTD_X],
					  scale_vec[MTD_Y],
					  scale_vec[MTD_Z]);
}

// =======================================================================
// AkMathGetRandomUnitVector
/*!
  ランダムな単位ベクトルを取得
 
  @param dst_vec	[out]	格納先
  @param rand_z		[in]	ランダムなZ値（-1 <= rand_z <= 1）
  @param rand_angle	[in]	ランダムな角度（0deg <= rand_angle <= 360deg）
  
  @note
  ランダムな方向の単位ベクトルを取得します。
  内部で乱数は生成しません。
  パラメータとしてrand_z, radn_angleに適切な範囲の乱数を渡してください。
 */
// =======================================================================
void AkMathGetRandomUnitVector(NNS_VECTOR *dst_vec, Float rand_z, Angle16 rand_angle)
{
	MTM_ASSERT(dst_vec);
	MTM_ASSERT(rand_z >= -1.f && rand_z <= 1.f);
	
	dst_vec->x	= nnSqrt(1.f - (rand_z * rand_z)) * nnCos(rand_angle);
	dst_vec->y	= nnSqrt(1.f - (rand_z * rand_z)) * nnSin(rand_angle);
	dst_vec->z	= rand_z;
}

// =======================================================================
// AkMathCountBitPopulation
/*!
  32bit2進数で1となっているビット数を取得
  
  @param bits	[in]	対象ビット列
  
  @return 1のビット数
 */
// =======================================================================
Uint8 AkMathCountBitPopulation(Uint32 bits)
{
	// ビット列を2bit毎の1の数に変換 e.g. 1011 -> 10,11 -> 01,10(=1,2)
	bits	= bits - ((bits >> 1) & 0x55555555);
	
	// 「2bit毎の1の数」を隣の「2bit毎の1の数」と足して「4bit毎の1の数」の羅列に変換
	bits	= (bits & 0x33333333) + ((bits >> 2) & 0x33333333);
	
	// 「4bit毎の1の数」を隣の「4bit毎の1の数」と足して、「8bit毎の1の数」の羅列に変換
	// （マスクせずに足しても、それぞれ4bitを超えることはないので、
	// 足したあとにマスクして結果を得る）
	bits	= (bits + (bits >> 4)) & 0x0f0f0f0f;
	
	// 「8bit毎の1の数」を隣の「8bit毎の1の数」と足して、「16bit毎の1の数」の羅列に変換
	// （マスクしないことによって生じるゴミは無視）
	bits	= bits + (bits >> 8);
	
	// 「16bit毎の1の数」を隣の「16bit毎の1の数」と足して、「32bit毎の1の数」の羅列に変換
	// （マスクしないことによって生じるゴミは無視）
	bits	= bits + (bits >> 16);
	
	// 最下位6bitに結果が格納される（マスクしてゴミの部分をカットする）
	return (Uint8)(bits & 0x3f);
}


/*------ Static Functions ----------------------------------------------*/
// =======================================================================
// test_func
/*!
  関数機能
 
  @param param0 [in] 入力引数0説明
  @param param1 [out] 出力ポインタ引数1説明
  @param param2 [io] 入出力ポインタ引数2説明
 
  @return 返値説明
 
  @note
  補足説明
 */
// =======================================================================
