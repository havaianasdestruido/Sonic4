// =======================================================================
/*!
  @file	akMath.h
  @brief 算術ライブラリ

  @author Keisuke Tanaka
 				Copyright(c) 2009 Dimps
  $Id: akMath.h 2 2011-04-11 05:21:26Z thamada $
 */
// =======================================================================
/*
 * $Log$
 */

/* 重複インクルード回避手法 */

#ifndef AK_MATH_H_
#define AK_MATH_H_

#if	defined(__cplusplus)
extern "C" {
#endif

/*------ Include Files -------------------------------------------------*/

/*------ Macros --------------------------------------------------------*/

/*------ Macro Functions -----------------------------------------------*/
// =======================================================================
// AKM_DEGtoA16
/*!
  角度を16bit整数角度に変換
  
  @param n	[in]	角度(degree)
  
  @return Angle16値
  
  @note
  計算結果が16bitから溢れてもコンパイラに依存せずに回り込みます。
 */
// =======================================================================
#define AKM_DEGtoA16(n)		((Angle16)(MTD_MATH_ANGLE_MASK & (Sint32)((n) * 182.04444f)))

// =======================================================================
// AKM_DEGtoA32
/*!
  角度を32bit整数角度に変換
  
  @param n	[in]	角度(degree)
  
  @return Angle32値
  
  @note
  計算結果が32bitから溢れた場合の動作は保証されません。（コンパイラ依存）
 */
// =======================================================================
#define AKM_DEGtoA32(n)		(NNM_DEGtoA32(n))


/*------ Definitions ---------------------------------------------------*/

/*------ External Declarations -----------------------------------------*/
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
extern void AkMathNormalizeMtx(NNS_MATRIX *dst_mtx, const NNS_MATRIX *src_mtx);

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
extern void AkMathExtractScaleMtx(NNS_MATRIX *dst_mtx, const NNS_MATRIX *src_mtx);


// =======================================================================
// AkMathInvertYZQuaternion
/*!
  クォータニオンをYZ平面で反転
  
  @param dst_quat	[out]	格納先
  @param src_quat	[in]	反転したいクォータニオン
  
  @note
  YZ平面で対称となるようにクォータニオンの回転方向を反転します。
 */
// =======================================================================
inline void AkMathInvertYZQuaternion(NNS_QUATERNION *dst_quat, const NNS_QUATERNION *src_quat)
{
	*dst_quat	= *src_quat;
	dst_quat->y	= -dst_quat->y;
	dst_quat->z	= -dst_quat->z;
}

// =======================================================================
// AkMathInvertXZQuaternion
/*!
  クォータニオンをXZ平面で反転
  
  @param dst_quat	[out]	格納先
  @param src_quat	[in]	反転したいクォータニオン
  
  @note
  XZ平面で対称となるようにクォータニオンの回転方向を反転します。
 */
// =======================================================================
inline void AkMathInvertXZQuaternion(NNS_QUATERNION *dst_quat, const NNS_QUATERNION *src_quat)
{
	*dst_quat	= *src_quat;
	dst_quat->x	= -dst_quat->x;
	dst_quat->z	= -dst_quat->z;
}

// =======================================================================
// AkMathInvertXYQuaternion
/*!
  クォータニオンをXY平面で反転
  
  @param dst_quat	[out]	格納先
  @param src_quat	[in]	反転したいクォータニオン
  
  @note
  XY平面で対称となるようにクォータニオンの回転方向を反転します。
 */
// =======================================================================
inline void AkMathInvertXYQuaternion(NNS_QUATERNION *dst_quat, const NNS_QUATERNION *src_quat)
{
	*dst_quat	= *src_quat;
	dst_quat->x	= -dst_quat->x;
	dst_quat->y	= -dst_quat->y;
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
extern void AkMathGetRandomUnitVector(NNS_VECTOR *dst_vec, Float rand_z, Angle16 rand_angle);

// =======================================================================
// AkMathRandFx
/*!
  固定小数0.0～1.0の範囲でランダムな値を取得する
  
  @return ランダム値
  
  @note
  内部でmtMathRand()を使用しています。
 */
// =======================================================================
inline fx32 AkMathRandFx(void)
{
	return mtMathRand() >> (16 - FX32_SHIFT);
}

// =======================================================================
// AkMathCountBitPopulation
/*!
  32bit2進数で1となっているビット数を取得
  
  @param bits	[in]	対象ビット列
  
  @return 1のビット数
 */
// =======================================================================
extern  Uint8 AkMathCountBitPopulation(Uint32 bits);

// =======================================================================
// AkMathDegToAngle32
/*!
  角度から32bit整数角度に変換
  
  @param deg	[in]	角度(degree)
  
  @return Angle32値
  
  @note
  DegreeをAngle32に変換します。
  結果が32bit整数最大（最小）値を超えた場合はアサートします。
  リリース版ではアサートせずに符号を維持したまま回り込ませた値を返しますが、
  floatの性質上、誤差は大きくなります（あくまでも保険動作）。
  また、このときモジュロ演算を使用しているため、マクロ版よりパフォーマンス的に不利です。
  プラットフォーム間での動作の統一を保障したい場合などに使用してください。
 */
// =======================================================================
inline Angle32 AkMathDegToAngle32(Float deg)
{
	Float result;
	
	result	= deg * 182.04444f;
	if (result > (Float)((Sint32)0x7fffffff) || result < (Float)((Sint32)0x80000000)) {
		// 
		MTM_ASSERT(!"akMath.h::AkMathDegToAngle32() Error! too large deg val");
		// PS3ではそのままAngle32にキャストしても回りこんでくれないため、
		// 剰余演算でプラットフォーム間の差異を誤魔化す
		result	= fmod(result, (Float)((Sint32)0x7fffffff));
	}
	
	return (Angle32)result;
}

// =======================================================================
// AkMathDegToAngle16
/*!
  角度から16bit整数角度に変換
  
  @param deg	[in]	角度(degree)
  
  @return Angle16値
  
  @note
  DegreeをAngle16に変換します。
  結果が16bit整数最大（最小）値を超えた場合は回り込みます。
  内部でAkMathDegToAngle32()を呼んでいます。
  プラットフォーム間での動作の統一を保障したい場合などに使用してください。
 */
// =======================================================================
inline Angle16 AkMathDegToAngle16(Float deg)
{
	return (Angle16)(MTD_MATH_ANGLE_MASK & AkMathDegToAngle32(deg));
}


// =======================================================================
// AKM_DEGtoA16_INLINE
/*!
  角度を16bit整数角度に変換（インライン版）
  
  @param n	[in]	角度(degree)
  
  @return Angle16値
  
  @note
  計算結果が16bitから溢れてもコンパイラに依存せずに回り込みます。
  計算結果が32bitから溢れるとアサートします。
 */
// =======================================================================
#define AKM_DEGtoA16_INLINE(n)		(AkMathDegToAngle16(n))

// =======================================================================
// AKM_DEGtoA32_INLINE
/*!
  角度を32bit整数角度に変換（インライン版）
  
  @param n	[in]	角度(degree)
  
  @return Angle32値
  
  @note
  計算結果が32bitから溢れるとアサートします。
 */
// =======================================================================
#define AKM_DEGtoA32_INLINE(n)		(AkMathDegToAngle32(n))


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

#if	defined(__cplusplus)
} /* extern "C" */
#endif

#endif /* AK_MATH_H_ */
