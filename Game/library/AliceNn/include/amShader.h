/*****************************************************************************/
/*      amShader.h                  Author : Takashi Nakano                  */
/*            Copyright(c) 2009 Dimps CORP. All Rights Reserved.             */
/*---------------------------------------------------------------------------*/
/* シェーダーライブラリヘッダ                                                */
/*---------------------------------------------------------------------------*/
/* Date          Ver    Comment                                              */
/* 090410-       0.01   first version                                        */
/*****************************************************************************/

#ifndef _AM_SHADER_H
#define _AM_SHADER_H

/*--- Include Files (Pre Definitions) ---------------------------------------*/

/*--- Definitions -----------------------------------------------------------*/

// プリコンパイルシェーダーを使用しない
#define AMD_SHADER_NOT_PRECOMPILED		((const char *)(-1))


/*--- Macros ----------------------------------------------------------------*/

/*--- Include Files (Post Definitions) --------------------------------------*/

/*--- External Valiables ----------------------------------------------------*/

/*--- External Functions ----------------------------------------------------*/

/*****************************************************************************/
/* void amShaderBuildStd(const char *vs_code, Sint32 vs_size,                */
/*                       const char *ps_code, Sint32 ps_size,                */
/*                           const char *code_path, const char *shader_path) */
/*---------------------------------------------------------------------------*/
/* [INPUT]  vs_code     : バーテックスシェーダーコード                       */
/*          vs_size     : バーテックスシェーダーコードサイズ                 */
/*          ps_code     : ピクセルシェーダーコード                           */
/*          ps_size     : ピクセルシェーダーコードサイズ                     */
/*          code_path   : シェーダーコードのパス                             */
/*          shader_path : コンパイル済みシェーダーのパス                     */
/* [FUNCTION]  標準シェーダーのビルド                                        */
/*****************************************************************************/
void amShaderBuildStd(const char *vs_code = NULL, Sint32 vs_size = 0,
		const char *ps_code = NULL, Sint32 ps_size = 0,
		const char *code_path = NULL,
		const char *shader_path = NULL);

/*****************************************************************************/
/* void amShaderBuildStd(AMS_AMB_HEADER *shader_amb)                         */
/*---------------------------------------------------------------------------*/
/* [INPUT]  shader_amb : コンパイル済みシェーダーファイル                    */
/* [FUNCTION]  標準シェーダーのビルド                                        */
/*****************************************************************************/
void amShaderBuildStd(AMS_AMB_HEADER *shader_amb);

/*****************************************************************************/
/* Sint32 amShaderLoadStd(void *image)                                       */
/*---------------------------------------------------------------------------*/
/* [INPUT]  image : シェーダーバイナリ                                       */
/* [RETURN]  登録完了をチェックするための登録ID                              */
/* [FUNCTION]  標準シェーダーのロード                                        */
/*****************************************************************************/
Sint32 amShaderLoadStd(void *image);

/*****************************************************************************/
/* void amShaderSetFileStd(AMS_AMB_HEADER *shader_amb)                       */
/*---------------------------------------------------------------------------*/
/* [INPUT]  shader_amb : プリコンパイルシェーダーファイル                    */
/* [FUNCTION]  コンパイル済みシェーダーファイルの設定                        */
/*****************************************************************************/
void amShaderSetFileStd(AMS_AMB_HEADER *shader_amb);

/*****************************************************************************/
/* Sint32 amShaderBuild(const char *vs_code, Sint32 vs_size,                 */
/*                      const char *ps_code, Sint32 ps_size,                 */
/*                      void **vs_shader, void **ps_shader,                  */
/*                      void *vs_param, void *ps_param,                      */
/*                      Sint32 vs_constants, Sint32 ps_constants)            */
/*---------------------------------------------------------------------------*/
/* [INPUT]  vs_code      : バーテックスシェーダーコード                      */
/*          vs_size      : バーテックスシェーダーコードサイズ                */
/*          ps_code      : ピクセルシェーダーコード                          */
/*          ps_size      : ピクセルシェーダーコードサイズ                    */
/*          vs_param     : バーテックスシェーダーパラメータ                  */
/*          ps_param     : ピクセルシェーダーパラメータ                      */
/*          vs_constants : バーテックスシェーダーパラメータ数                */
/*          ps_constants : ピクセルシェーダーパラメータ数                    */
/* [OUTPUT] vs_shader    : 頂点シェーダー                                    */
/*          ps_shader    : ピクセルシェーダー                                */
/* [RETURN]  登録完了をチェックするための登録ID                              */
/* [FUNCTION]  シェーダーのビルド（ランタイムコンパイル）                    */
/*****************************************************************************/
Sint32 amShaderBuild(const char *vs_code, Sint32 vs_size, const char *ps_code,
		Sint32 ps_size, void **vs_shader, void **ps_shader,
		void *vs_param = NULL, void *ps_param = NULL,
		Sint32 vs_constants = 0, Sint32 ps_constants = 0);

/*****************************************************************************/
/* Sint32 amShaderBuild(void *vs_image, void *ps_image,                      */
/*                      void **vs_shader, void **ps_shader,                  */
/*                      void *vs_param, void *ps_param,                      */
/*                      Sint32 vs_constants, Sint32 ps_constants)            */
/*---------------------------------------------------------------------------*/
/* [INPUT]  vs_image     : バーテックスシェーダーバイナリイメージ            */
/*          ps_image     : ピクセルシェーダーバイナリイメージ                */
/*          vs_param     : バーテックスシェーダーパラメータ                  */
/*          ps_param     : ピクセルシェーダーパラメータ                      */
/*          vs_constants : バーテックスシェーダーパラメータ数                */
/*          ps_constants : ピクセルシェーダーパラメータ数                    */
/* [OUTPUT] vs_shader    : 頂点シェーダー                                    */
/*          ps_shader    : ピクセルシェーダー                                */
/* [RETURN]  登録完了をチェックするための登録ID                              */
/* [FUNCTION]  シェーダーのビルド（オフラインコンパイル）                    */
/*****************************************************************************/
Sint32 amShaderBuild(void *vs_image, void *ps_image,
		void **vs_shader, void **ps_shader,
		void *vs_param = NULL, void *ps_param = NULL,
		Sint32 vs_constants = 0, Sint32 ps_constants = 0);

/*****************************************************************************/
/* Sint32 amShaderRelease(void *vs_shader, void *ps_shader)                  */
/*---------------------------------------------------------------------------*/
/* [INPUT]  vs_shader    : 頂点シェーダー                                    */
/*          ps_shader    : ピクセルシェーダー                                */
/* [RETURN]  登録完了をチェックするための登録ID                              */
/* [FUNCTION]  シェーダーの解放                                              */
/*****************************************************************************/
Sint32 amShaderRelease(void *vs_shader, void *ps_shader);

#endif
