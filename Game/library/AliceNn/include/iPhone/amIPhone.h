/*****************************************************************************/
/*      amIPhone.h                  Author : Syuichi Gotou                   */
/*            Copyright(c) 2009 Dimps CORP. All Rights Reserved.             */
/*---------------------------------------------------------------------------*/
/* iPhone関連ライブラリヘッダ                                                */
/*---------------------------------------------------------------------------*/
/* Date          Ver    Comment                                              */
/* 090629-       0.01   first version                                        */
/*****************************************************************************/

#ifndef _AM_IPHONE_H
#define _AM_IPHONE_H

/*--- Include Files (Pre Definitions) ---------------------------------------*/

/*--- Definitions -----------------------------------------------------------*/

/*--- Macros ----------------------------------------------------------------*/

/*--- Include Files (Post Definitions) --------------------------------------*/

/*--- External Valiables ----------------------------------------------------*/

/*--- External Functions ----------------------------------------------------*/

/*****************************************************************************/
/* void amIPhoneInitNN(GLint Width, GLint Height)                            */
/*---------------------------------------------------------------------------*/
/* [FUNCTION]  初期化                                                        */
/*****************************************************************************/
void amIPhoneInitNN(int Width, int Height);

/*****************************************************************************/
/* void amIPhoneExitNN(void)                                                 */
/*---------------------------------------------------------------------------*/
/* [FUNCTION]  終了                                                          */
/*****************************************************************************/
void amIPhoneExitNN(void);

/*****************************************************************************/
/* int amIPhoneMainLoop(void)                                                */
/*---------------------------------------------------------------------------*/
/* [FUNCTION]  メインループ                                                  */
/* [RETURN] 0 : 正常 /  1 : 異常あり                                         */
/*****************************************************************************/
int amIPhoneMainLoop(void);

/*!***************************************************************************
 @Function		amIPhonePVRTLoadTextureFromPointer
 @Input			pointer			Pointer to header-texture's structure
 @Modified		texName			the OpenGL ES texture name as returned by glBindTexture
 @Modified		psTextureHeader	Pointer to a PVR_Texture_Header struct. Modified to
								contain the header data of the returned texture Ignored if NULL.
 @Return		true on success
 @Description	Allows textures to be stored in C header files and loaded in.  Loads the whole texture.
				Release texture by calling PVRTReleaseTexture.
*****************************************************************************/
unsigned int amIPhonePVRTLoadTextureFromPointer(const void* pointer, GLuint *const texName, const void *psTextureHeader);

/*****************************************************************************/
/* void amIPhoneSetTextureAttribute(AMS_PARAM_LOAD_TEXTURE *param)           */
/*---------------------------------------------------------------------------*/
/* [INPUT] param : テクスチャ登録パラメータ                                  */
/* [FUNCTION]  テクスチャ属性の設定                                          */
/*****************************************************************************/
void amIPhoneSetTextureAttribute(AMS_PARAM_LOAD_TEXTURE *param);

/*****************************************************************************/
/* BOOL _amIPhoneCheckExit(void)                                             */
/*---------------------------------------------------------------------------*/
/* [RETURN] 終了判定結果                                                     */
/* [FUNCTION]  アプリケーション終了判定                                      */
/*****************************************************************************/
BOOL _amIPhoneCheckExit(void);

#endif
