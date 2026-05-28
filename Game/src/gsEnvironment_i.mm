// ============================================================================
/*!
	@file	gsEnvironment_i.h
	@brief	環境別システム設定モジュール宣言(iPhone)

	@author	Kouji Hokazono <kouji_hokazono@dimps.co.jp>
		Copyright(c) 2009-2010 Dimps
	$Id$
 */
// ============================================================================
/*
 * $Log$
 */

//------ Include ---------------------- インクルード ---------------------------******_IC*
//#include <alice.h>
typedef unsigned short u16;
#include "gsEnvironment_i.h"



//------ Debug ------------------------ デバッグ -------------------------------******_DG*
#if defined(MTD_DEBUG)
#endif	//#if defined(MTD_DEBUG)
//------ Macro ------------------------ マクロ ---------------------------------******_MC*
//------ C Global Functions ----------- C グローバル関数の定義 -------------------******CGF*
// ===========================================================================
//	GsEnvGetRegionIphone
/*!
	リージョン取得

	@return リージョン
	@note
	本体に設定されているリージョンを返します。\n
*/
// ===========================================================================
GSE_REGION GsEnvGetRegionIphone(void)
{
	struct CRegionTable {
		const char	*country;
		GSE_REGION	region;
	};
	const CRegionTable c_region_table[] = {
		{"JP", GSD_REGION_JP}	//日本
	,	{"US", GSD_REGION_US}	//アメリカ合衆国
	,	{"CA", GSD_REGION_US}	//カナダ
	,	{"PM", GSD_REGION_US}	//サンピエール島・ミクロン島
	,	{"FR", GSD_REGION_EU}	//フランス
	,	{"IT", GSD_REGION_EU}	//イタリア
	,	{"DE", GSD_REGION_EU}	//ドイツ
	,	{"ES", GSD_REGION_EU}	//スペイン
	,	{"AL", GSD_REGION_EU}	//アルバニア
	,	{"AD", GSD_REGION_EU}	//アンドラ
	,	{"AZ", GSD_REGION_EU}	//アゼルバイジャン
	,	{"AT", GSD_REGION_EU}	//オーストリア
	,	{"AM", GSD_REGION_EU}	//アルメニア
	,	{"BE", GSD_REGION_EU}	//ベルギー
	,	{"BA", GSD_REGION_EU}	//ボスニア・ヘルツェゴビナ
	,	{"BG", GSD_REGION_EU}	//ブルガリア
	,	{"BY", GSD_REGION_EU}	//ベラルーシ
	,	{"HR", GSD_REGION_EU}	//クロアチア
	,	{"CZ", GSD_REGION_EU}	//チェコ
	,	{"DK", GSD_REGION_EU}	//デンマーク
	,	{"EE", GSD_REGION_EU}	//エストニア
	,	{"FO", GSD_REGION_EU}	//フェロー諸島
	,	{"FI", GSD_REGION_EU}	//フィンランド
	,	{"AX", GSD_REGION_EU}	//オーランド諸島
	,	{"GE", GSD_REGION_EU}	//グルジア
	,	{"GI", GSD_REGION_EU}	//ジブラルタル
	,	{"GR", GSD_REGION_EU}	//ギリシャ
	,	{"GL", GSD_REGION_EU}	//グリーンランド
	,	{"VA", GSD_REGION_EU}	//バチカン市国
	,	{"HU", GSD_REGION_EU}	//ハンガリー
	,	{"IS", GSD_REGION_EU}	//アイスランド
	,	{"IE", GSD_REGION_EU}	//アイルランド
	,	{"LV", GSD_REGION_EU}	//ラトビア
	,	{"LI", GSD_REGION_EU}	//リヒテンシュタイン
	,	{"LT", GSD_REGION_EU}	//リトアニア
	,	{"LU", GSD_REGION_EU}	//ルクセンブルク
	,	{"MC", GSD_REGION_EU}	//モナコ
	,	{"MD", GSD_REGION_EU}	//モルドバ
	,	{"ME", GSD_REGION_EU}	//モンテネグロ
	,	{"NL", GSD_REGION_EU}	//オランダ
	,	{"NO", GSD_REGION_EU}	//ノルウェー
	,	{"PL", GSD_REGION_EU}	//ポーランド
	,	{"PT", GSD_REGION_EU}	//ポルトガル
	,	{"RO", GSD_REGION_EU}	//ルーマニア
	,	{"SM", GSD_REGION_EU}	//サンマリノ
	,	{"RS", GSD_REGION_EU}	//セルビア
	,	{"SK", GSD_REGION_EU}	//スロバキア
	,	{"SI", GSD_REGION_EU}	//スロベニア
	,	{"SJ", GSD_REGION_EU}	//スバールバル諸島・ヤンマイエン島
	,	{"SE", GSD_REGION_EU}	//スウェーデン
	,	{"CH", GSD_REGION_EU}	//スイス
	,	{"UA", GSD_REGION_EU}	//ウクライナ
	,	{"MK", GSD_REGION_EU}	//マケドニア共和国
	,	{"GB", GSD_REGION_EU}	//イギリス
	,	{"GG", GSD_REGION_EU}	//ガーンジー島
	,	{"JE", GSD_REGION_EU}	//ジャージー島
	,	{"IM", GSD_REGION_EU}	//マン島
	};

	NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
	
	GSE_REGION result = GSD_REGION_DEF;
	NSString *ns_country = [[NSLocale currentLocale] objectForKey:NSLocaleCountryCode];
	const char *country = [ns_country cStringUsingEncoding:NSShiftJISStringEncoding];
	
	for (const CRegionTable *region = &c_region_table[0], *region_end = &c_region_table[sizeof(c_region_table)/sizeof(c_region_table[0])]; region != region_end; ++region) {
		if (0 == strcmp(region->country, country)) {
			result = region->region;
			break;
		}
	}

	[pool release];
	return result;
}

// ===========================================================================
//	GsEnvGetLanguageIphone
/*!
	言語取得

	@return 言語
	@note
	本体に設定されている言語を返します。\n
	ゲーム中で表示する言語は、全てこの関数の戻り値を元に決定して下さい。\n
*/
// ===========================================================================
GSE_LANGUAGE GsEnvGetLanguageIphone(void)
{
	struct CLanguageTable {
		const char		*lang;
		GSE_LANGUAGE	code;
	};
	const CLanguageTable c_language_table[] = {
		{"ja", GSD_LANGUAGE_JP}	//日本
	,	{"en", GSD_LANGUAGE_US}	//英語
	,	{"fr", GSD_LANGUAGE_FR}	//フランス語
	,	{"it", GSD_LANGUAGE_IT}	//イタリア語
	,	{"de", GSD_LANGUAGE_GE}	//ドイツ語
	,	{"es", GSD_LANGUAGE_SP}	//スペイン語
	};

	NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
	
	GSE_LANGUAGE result = GSD_LANGUAGE_DEF;
	NSString *ns_lang = [[NSLocale preferredLanguages] objectAtIndex:0];
	const char *lang = [ns_lang cStringUsingEncoding:NSShiftJISStringEncoding];
	
	for (const CLanguageTable *language = &c_language_table[0], *language_end = &c_language_table[sizeof(c_language_table)/sizeof(c_language_table[0])]; language != language_end; ++language) {
		if (0 == strcmp(language->lang, lang)) {
			result = language->code;
			break;
		}
	}

	[pool release];
	return result;
}




// =============================================================================
// Function
/*!
	関数の説明

	@param	org1	[io]	引数１の説明
	@param	org2	[in]	引数２の説明
	@param	org3	[out]	引数３の説明

	@return	戻り値の説明
		or
	@retval	0	正常
	@retval	!0	異常

	@exception 例外
 
	@note
		補足説明
 */
// ==========================================================================
