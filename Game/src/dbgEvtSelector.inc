// ============================================================================
/*!
	@file	dbgEvtSelector.inc
	@brief	イベントセレクタ

	@author	Kouji Hokazono <kouji_hokazono@dimps.co.jp>
		Copyright(c) 2007-2009 Dimps
	$Id$
 */
// ============================================================================
/*
 * $Log$
 */
/*
 *	◆イベントセレクタ使用方法
 *		1.	Include 項にヘッダを追加します
 *		2.	AddMenu 項にメニュー名とイベントID、コールバックを追加します。
 *			(コールバックはNULLで構いません)
 *
 *		以上で追加終了です。
 *
 *
 *	◆オプションの制御等を行う方法
 *		1.	Static Definitions 項に関数宣言を記述します。
 *		2.	Static Functions 項に関数定義を記述します。
 *		3.	AddMenu 項の イベントインデックスの作成 の、
 *			該当イベントのコールバックに先ほど作成した関数を設定します。
 *			追加した関数内で自由に設定変更を行って下さい。
 *
 *		オプション制御用の処理定義が他のファイルに有っても問題ありません。
 *		このファイル上で広範囲の変更を行っても構いませんが、
 *		あまりにも巨大な規模になるなら別ファイルに作成する事をお勧めします。
 *
 *
 *	◆特殊機能について
 *		Option 項にイベントセレクタの挙動を変更させる項目があります。
 *		オプションの変更によって下記の事が出来ます。
 *			・フェードの不使用
 *			・初期選択イベントの設定
 *			・即時起動イベント
 *			・VRAM設定を破壊してからイベントを開始させるか
 *
 *		初期選択イベントと即時起動イベントを上手く使えば開発の効率が上がり、
 *		VRAM破壊を使用しておくと、リンク時の不具合発生率が下がります。
 */






//--------------------------------------------------------------------------------
//------ Include ---------------------- インクルード ---------------------------
#if defined(_INC_DBG_EVT_SELECTOR_INCLUDE)
//ToDo: ▼▼▼ここにヘッダを追加します▼▼▼


#include "gsMainSys.h"
#include "gsEnvironment.h"
#include "gmMain.h"
#include "dmStaffRoll.h"

#include "dbgSaveDataEditor.hpp"
#include "erDbgTrg.hpp"
#include "erDbgStartWeb.hpp"












#endif //#if defined(_INC_DBG_EVT_SELECTOR_INCLUDE)
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//------ AddMenu ---------------------- メニュー追加 ---------------------------
#if defined(_INC_DBG_EVT_SELECTOR_ADD_MENU)
{
	#define	SET_CLASS(...) m_evt_idx.push_back(new __VA_ARGS__)
	#define	SET_C_CODE(...) m_evt_idx.push_back(new dbg::CEvtCb(__VA_ARGS__))
	//ToDo: ▼▼▼ここでメニューへ追加します。▼▼▼
	//SET_CLASS(dbg::CEvtBaseから派生したクラス);
	//SET_C_CODE(イベント名 , イベントID, フラグ, コールバック, コールバックデータ);

#if 0
	SET_C_CODE("TestEvent" , GSD_EVT_ID_EVT_SELECTOR);
	SET_C_CODE("TestEvent 2" , GSD_EVT_ID_NULL, 0, TEMPLATE_EvtCbFunction, NULL);
#endif

	SET_C_CODE("GameTest"		, GSD_EVT_ID_MAINGAME, 0, dbgGmGameTestCb, NULL);
//	SET_C_CODE("PauseMenuTest"	, GSD_EVT_ID_DEBUG_PAUSE_MENU, 0, dbgDefaultCb, NULL);
	SET_C_CODE("Env Setting"	, GSD_EVT_ID_DEBUG_LANG_CHANGE, 0, dbgDefaultCb, NULL);
	SET_CLASS(dbg::CSaveDataEditor("Save Data Editor"));
	SET_C_CODE("Title"			, GSD_EVT_ID_TITLE, 0, dbgDefaultCb);
	SET_C_CODE("Logo"			, GSD_EVT_ID_LOGO_SEGA, 0, dbgDefaultCb);
	SET_C_CODE("Stage Select"	, GSD_EVT_ID_MAP, 0, dbgDefaultCb, NULL);
#if _PC || _WII
	SET_C_CODE("Main Menu"		, GSD_EVT_ID_MAINMENU, 0, dbgDefaultCb, NULL);
#endif

#if _PC || _PS3 || _XBOX || _WII
	SET_C_CODE("Ranking"		, GSD_EVT_ID_RANKING, 0, dbgDefaultCb, NULL);
#endif
	SET_C_CODE("Option"			, GSD_EVT_ID_OPTION, 0, dbgDefaultCb, NULL);
	SET_C_CODE("Staff Roll"		, GSD_EVT_ID_STAFFROLL, 0, dbgDefaultCb, NULL);

	SET_C_CODE("Ending"			, GSD_EVT_ID_ENDING, 0, dbgEndingCb, NULL);

//	SET_C_CODE("AoTestSave"		, GSD_EVT_ID_AOTESTSAVE, 0, dbgDefaultCb, NULL);
#if _PS3 || _XBOX || _WII
	SET_C_CODE("System Message Viewer"	, GSD_EVT_ID_AOTESTSYSMSG, 0, dbgDefaultCb, NULL);
//	SET_C_CODE("System Message Viewer2"	, GSD_EVT_ID_AOTESTSYSMSG2, 0, dbgDefaultCb, NULL);
#endif // _PS3 || _XBOX || _WII

#if !_IPHONE
//	SET_C_CODE("AoTestTrophy"	, GSD_EVT_ID_AOTESTTROPHY, 0, dbgDefaultCb, NULL);
//	SET_C_CODE("AoTestNet"		, GSD_EVT_ID_AOTESTNET, 0, dbgDefaultCb, NULL);
//	SET_C_CODE("AoTestWait"		, GSD_EVT_ID_AOTESTWAIT, 0, dbgDefaultCb, NULL);
//	SET_C_CODE("AoTestPresence"	, GSD_EVT_ID_AOTESTPRESENCE, 0, dbgDefaultCb, NULL);
//	SET_C_CODE("AoTestExit"		, GSD_EVT_ID_AOTESTEXIT, 0, dbgDefaultCb, NULL);
	SET_C_CODE("AoTestAvatarAward"	, GSD_EVT_ID_AOTESTAVATARAWARD, 0, dbgDefaultCb, NULL);
//	SET_C_CODE("Trial Setting"	, GSD_EVT_ID_GSTRIALTEST, 0, dbgDefaultCb, NULL);
//	SET_C_CODE("Strap Image"	, GSD_EVT_ID_GSSTRAPIMAGETEST, 0, dbgDefaultCb, NULL);
#endif	// #if !_IPHONE
	SET_C_CODE("BUY SCREEN"		, GSD_EVT_ID_DBGBUYSCREEN, 0, dbgDefaultCb, NULL);
#if _IPHONE && SONIC4_TRIAL
	SET_C_CODE("MOVIE"			, GSD_EVT_ID_MOVIE, 0, dbgDefaultCb, NULL);
#endif // _IPHONE && SONIC4_TRIAL
	
#if !_IPHONE
	SET_C_CODE("SAFE FRAME"		, GSD_EVT_ID_AOTESTSAFEFRAME, 0, dbgDefaultCb, NULL);
//	SET_C_CODE("WiiFatalTest"	, GSD_EVT_ID_AOTESTWIIFATAL, 0, dbgDefaultCb, NULL);
#endif	// #if !_IPHONE

#if _IPHONE
	SET_CLASS(er::dbg::CTrg("Trg Test"));
	SET_CLASS(er::dbg::CStartWeb("Start Web Test"));
#endif //_IPHONE


	SET_C_CODE("SegaLogo"			, GSD_EVT_ID_LOGO_SEGA, 0, dbgDefaultCb);
	SET_C_CODE("SonicTeamLogo"		, GSD_EVT_ID_LOGO_SONIC, 0, dbgDefaultCb);
#if (defined(HOG_RGN_US) && !defined(HOG_RGN_KR)) && !_IPHONE
	SET_C_CODE("ESRBLogo"			, GSD_EVT_ID_LOGO_ESRB, 0, dbgDefaultCb);
#endif
#if _WII || _PC
	SET_C_CODE("WiiExpOpe"			, GSD_EVT_ID_WII_EXP_OPE, 0, dbgDefaultCb);
#endif
#if (defined(HOG_RGN_JP) && _PS3) || _PC
	SET_C_CODE("PS3Kido"			, GSD_EVT_ID_PS3_KIDO, 0, dbgDefaultCb);
#endif

#if defined GSD_DEBUG_MODEL_BUILD
	SET_C_CODE("DebugModelBuild"	, GSD_EVT_ID_DEBUG_MODEL_BUILD, 0, dbgDefaultCb);
#endif





	#undef SET_C_CODE
	#undef SET_CLASS
}
#endif //#if defined(_INC_DBG_EVT_SELECTOR_ADD_MENU)
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//------ Option ----------------------- オプション -----------------------------
#if defined(_INC_DBG_EVT_SELECTOR_OPTION)
{
	//ToDo: ▼▼▼ここでオプションを設定します。▼▼▼

	m_use_fade = false;			//<フェードの使用
	m_slct = 0;					//<初期選択イベント
	m_direct_start = u32(-1);	//<即時起動イベント(u32(-1)で無効)


	g_gs_main_sys_info.debug_flag |= GSD_DEBUG_FPS_DISP;

















}
#endif //#if defined(_INC_DBG_EVT_SELECTOR_OPTION)
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//------ RomStatus -------------------- ROM状態表記 ----------------------------
#if defined(_INC_DBG_EVT_SELECTOR_ROM_STATUS)
{
	//ToDo: ▼▼▼ここでROM状態表記を設定します。▼▼▼

	::AMS_TIMER time = getElapsedTime();
	::amTimerEnd(&time, false);
	u32 msec = static_cast<u32>(::amTimerGetMSec(&time));
	u32 min = msec / 1000 / 60 % 60;
	u32 sec = msec / 1000 % 60;
	msec = msec / 10 % 100;

	cb.Print(2, 2, "%s", "Event Selector");
	cb.Print(24, 2, "%4d:%02d:%02d", min, sec, msec);
	cb.Print(2, 3, "--------------------------------");
	int line = 4;
	cb.Print(4, line++, "RomStatus");

//ビルドバージョン
	cb.Print(6, line++, "051");

//ビルド時刻
#if 0	//有効にするとバイナリレベルで同じファイルが作成出来なくなります
	cb.Print(6, line++, "%s    %s", __DATE__, __TIME__);
#endif

//ビルドターゲット
#if _PC
	cb.Print(6, line++, "PC");
#elif _PS3
	cb.Print(6, line++, "PLAYSTATION3");
#elif _XBOX
	cb.Print(6, line++, "XBox360");
#elif _WII
	cb.Print(6, line++, "Wii");
#elif _IPHONE
	cb.Print(6, line++, "iPhone");
#else
	cb.Print(6, line++, "Other Platform");
#endif
#if defined(_DLC)
	cb.Print(6, line++, "DLC");
#elif defined(SONIC4_TRIAL)
	cb.Print(6, line++, "SONIC4_TRIAL");
#endif
#if defined(HOG_RGN_JP)
	cb.Print(6, line++, "JP");
#elif defined(HOG_RGN_US)
	#if !defined(HOG_RGN_KR)
		cb.Print(6, line++, "US");
	#else
		cb.Print(6, line++, "KR with US");
	#endif
#elif defined(HOG_RGN_EU)
	cb.Print(6, line++, "EU");
#elif defined(HOG_RGN_KR)
	cb.Print(6, line++, "KR without US");
#else
	cb.Print(6, line++, "Other Region");
#endif











}
#endif //#if defined(_INC_DBG_EVT_SELECTOR_ROM_STATUS)
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//------ Static Definitions ----------- ローカル関数宣言 -------------------------
#if defined(_INC_DBG_EVT_SELECTOR_STATIC_DEFINITIONS)
//ToDo: ▼▼▼ここにコールバック関数宣言を追加します▼▼▼

#if 0
void TEMPLATE_EvtCbFunction(CEvtCb::ECbKind::Type kind, CEvtCb &cb, void *cb_data);
#endif

void dbgDefaultCb(CEvtCb::ECbKind::Type kind, CEvtCb &cb, void *cb_data);
void dbgGmGameTestCb(CEvtCb::ECbKind::Type kind, CEvtCb &cb, void *cb_data);
void dbgEndingCb(CEvtCb::ECbKind::Type kind, CEvtCb &cb, void *cb_data);











#endif //#if defined(_INC_DBG_EVT_SELECTOR_STATIC_DEFINITIONS)
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//------ Static Functions ------------- ローカル関数定義 -------------------------
#if defined(_INC_DBG_EVT_SELECTOR_STATIC_FUNCTIONS)
//ToDo: ▼▼▼ここにコールバック関数定義を追加します▼▼▼

#if 0
// =============================================================================
// TEMPLATE_EvtCbFunction
/*!
	イベントセレクトのコールバック関数テンプレート

	@param	kind	[in]	コールバックの種類
	@param	cb		[in]	コールバック実データ
	@param	cb_data	[in]	ユーザーデータ
 */
// ==========================================================================
void TEMPLATE_EvtCbFunction(CEvtCb::ECbKind::Type kind, CEvtCb &cb, void *cb_data)
{
	static int counter = 0;

	switch (kind) {
	case CEvtCb::ECbKind::Focus:		//<選択時
		{
			cb.Print(0, 24, "%d", counter++);
			cb.Printc(0, 25, dbg::CEvtSelector::EColor::White, "%s", "DispString(Color:White)");
			cb.Printc(0, 26, dbg::CEvtSelector::EColor::Red, "%s", "DispString(Color:Red)");
			cb.Printc(0, 27, 0xFF000080, "%s", "DispString(Color:DarkRed)");
			cb.Printc(0, 28, 0xFF800000, "%s", "DispString(Color:DarkBlue)");
		}
		break;
	case CEvtCb::ECbKind::Enter:		//<決定直後
		{
			u32 buf[2] = {0, 0};
			cb.SetNextEvtArg(sizeof(buf), &buf);
		}
		break;
	case CEvtCb::ECbKind::Init:		//<初期化
	case CEvtCb::ECbKind::Exit:		//<後処理
	case CEvtCb::ECbKind::OnFocus:	//<選択された時
	case CEvtCb::ECbKind::OnBlur:	//<非選択になった時
	case CEvtCb::ECbKind::Blur:		//<非選択時
	default:
		break;
	}

	UNREFERENCED_PARAMETER(cb_data);
}
#endif



// =============================================================================
// dbgDefaultCb
/*!
	汎用のコールバック関数

	@param	kind	[in]	コールバックの種類
	@param	cb		[in]	コールバック実データ
	@param	cb_data	[in]	ユーザーデータ
 */
// ==========================================================================
void dbgDefaultCb(CEvtCb::ECbKind::Type kind, CEvtCb &cb, void *cb_data)
{
	switch (kind) {
	case CEvtCb::ECbKind::Focus:		//<選択時
		{
#if defined(MTD_DEBUG)
			cb.Print(3, 12, "%c       : Start", GsEnvDebugGetDecideKeyChar());
#else
			cb.Print(3, 12, "-       : Start");
#endif //defined(MTD_DEBUG)

		}
		break;
	case CEvtCb::ECbKind::Enter:		//<決定直後
		break;
	case CEvtCb::ECbKind::Init:		//<初期化
	case CEvtCb::ECbKind::Exit:		//<後処理
	case CEvtCb::ECbKind::OnFocus:	//<選択された時
	case CEvtCb::ECbKind::OnBlur:	//<非選択になった時
	case CEvtCb::ECbKind::Blur:		//<非選択時
	default:
		break;
	}

	UNREFERENCED_PARAMETER(cb_data);
}



// =============================================================================
// dbgGmGameTestCb
/*!
	ゲームテスト用のコールバック関数

	@param	kind	[in]	コールバックの種類
	@param	cb		[in]	コールバック実データ
	@param	cb_data	[in]	ユーザーデータ
 */
// ==========================================================================
void dbgGmGameTestCb(CEvtCb::ECbKind::Type kind, CEvtCb &cb, void *cb_data)
{
	static	s32	cursor_pos		= 0;
#if !_IPHONE
	static	s32	ctrl_type_no	= 1;
#else //!_IPHONE
	static	s32	ctrl_type_no	= 0;
#endif //!_IPHONE
	static	s32	super_sonic_free= 0;
	static	GSE_GAME_MODE	game_mode	= GSD_GAME_MODE_STORY;
	
	static	s32 zone_id = 0;
	static	s32 act_id = 0;
#if !_IPHONE
	static	s32 pad_vib = 0;
#else //!_IPHONE
	static	s32	sound_id = 0;
#endif //!_IPHONE

	const char *ctrl_type[] = {
				"NORMAL",
				"CLASSIC DOWN",
				"CLASSIC UP",
	};
	const char *stage_name[GSD_MAIN_STAGE_ID_MAX] = {
				"1", "1", "1", "1",
				"2", "2", "2", "2",
				"3", "3", "3", "3",
				"4", "4", "4", "4",
				"FINAL", "FINAL", "FINAL", "FINAL", "FINAL",
				"SS", "SS", "SS", "SS", "SS", "SS", "SS",
	};
	const char *act_name[GSD_MAIN_STAGE_ID_MAX] = {
				"1", "2", "3", "BOSS",
				"1", "2", "3", "BOSS",
				"1", "2", "3", "BOSS",
				"1", "2", "3", "BOSS",
				"1", "2", "3", "4", "5",
				"1", "2", "3", "4", "5", "6", "7",
	};
	const s32 stage_top[GSD_MAIN_ZONE_TYPE_MAX] = {
				GSD_MAIN_STAGE_ID_1_1,
				GSD_MAIN_STAGE_ID_2_1,
				GSD_MAIN_STAGE_ID_3_1,
				GSD_MAIN_STAGE_ID_4_1,
				GSD_MAIN_STAGE_ID_FINAL_1,
				GSD_MAIN_STAGE_ID_SS1,
	};
#if _IPHONE
	const char *sound_name[3] = {
		"DL001",
		"DL002",
		"DL005",
	};
#endif //_IPHONE
	const char *on_off[2] ={
		"OFF",
		"ON",
	};
	const char *mode_name[2] ={
		"SCORE",
		"TIME",
	};

	s32		x, y;

	switch (kind) {
	case CEvtCb::ECbKind::Focus:		//<選択時
		{
			//設定
			if (AoPadSomeoneRepeat(KEY_L_DOWN) >= 0) {
				cursor_pos++;
				if (cursor_pos > 5) {
					cursor_pos = 0;
				}
			}
			else if (AoPadSomeoneRepeat(KEY_L_UP) >= 0) {
				cursor_pos--;
				if (cursor_pos < 0) {
					cursor_pos = 5;
				}
			}
			cursor_pos	= MTM_MATH_CLIP(cursor_pos, 0, 5);


			switch (cursor_pos) {
			case 0:	// ZONE選択
				if (AoPadSomeoneRepeat(KEY_R_LEFT) >= 0) {
					zone_id++;
				}
				if (AoPadSomeoneRepeat(KEY_R_UP) >= 0) {
					zone_id--;
				}
				zone_id	= MTM_MATH_CLIP(zone_id, 0, GSD_MAIN_ZONE_TYPE_MAX-1);
#if _IPHONE
#if defined(HOG_PRESENT_ROM_IPHONE)
				zone_id	= MTM_MATH_CLIP(zone_id, 0, 1);
#endif //defined(HOG_PRESENT_ROM_IPHONE)
#endif //_IPHONE
				// ZONE別のアクト数クリップ
				switch (zone_id) {
					default:
					case GSD_MAIN_ZONE_TYPE_1:
					case GSD_MAIN_ZONE_TYPE_2:
					case GSD_MAIN_ZONE_TYPE_3:
					case GSD_MAIN_ZONE_TYPE_4:
						act_id	= MTM_MATH_CLIP(act_id, 0, 3);
						break;
					case GSD_MAIN_ZONE_TYPE_FINAL:
						act_id	= MTM_MATH_CLIP(act_id, 0, 4);
						break;
					case GSD_MAIN_ZONE_TYPE_SS:
						act_id	= MTM_MATH_CLIP(act_id, 0, 6);
				}
#if _IPHONE
#if defined(HOG_PRESENT_ROM_IPHONE)
				switch (zone_id) {
					case GSD_MAIN_ZONE_TYPE_2:
						act_id	= MTM_MATH_CLIP(act_id, 0, 2);
						break;
					default:
						break;
				}
#endif //defined(HOG_PRESENT_ROM_IPHONE)
#endif //_IPHONE
				break;
			case 1:	// ACT選択
				if (AoPadSomeoneRepeat(KEY_R_LEFT) >= 0) {
					act_id++;
				}
				if (AoPadSomeoneRepeat(KEY_R_UP) >= 0) {
					act_id--;
				}
				// ZONE別のアクト数クリップ
				switch (zone_id) {
					default:
					case GSD_MAIN_ZONE_TYPE_1:
					case GSD_MAIN_ZONE_TYPE_2:
					case GSD_MAIN_ZONE_TYPE_3:
					case GSD_MAIN_ZONE_TYPE_4:
						act_id	= MTM_MATH_CLIP(act_id, 0, 3);
						break;
					case GSD_MAIN_ZONE_TYPE_FINAL:
						act_id	= MTM_MATH_CLIP(act_id, 0, 4);
						break;
					case GSD_MAIN_ZONE_TYPE_SS:
						act_id	= MTM_MATH_CLIP(act_id, 0, 6);
				}
#if _IPHONE
#if defined(HOG_PRESENT_ROM_IPHONE)
				switch (zone_id) {
					case GSD_MAIN_ZONE_TYPE_2:
						act_id	= MTM_MATH_CLIP(act_id, 0, 2);
						break;
					default:
						break;
				}
#endif //defined(HOG_PRESENT_ROM_IPHONE)
#endif //_IPHONE
				break;
			case 2:	// 操作方法選択
				if (AoPadSomeoneStand(KEY_R_LEFT) >= 0) {
					if (ctrl_type_no < arrayof(ctrl_type) - 1) {
						++ctrl_type_no;
					}
				} else if (AoPadSomeoneStand(KEY_R_UP) >= 0) {
					if (0 < ctrl_type_no) {
						--ctrl_type_no;
					}
				}
				break;
			case 3:	// スーパーソニック フリー設定
				if (AoPadSomeoneStand(KEY_R_LEFT | KEY_R_UP) >= 0) {
					super_sonic_free ^= 0x01;
				}
				break;
#if !_IPHONE
			case 4: // パッド振動
				if (AoPadSomeoneStand(KEY_R_LEFT | KEY_R_UP) >= 0) {
					pad_vib ^= 0x01;
				}
				break;
#else //!_IPHONE
			case 4:	// BGM選択
				if (AoPadSomeoneRepeat(KEY_R_LEFT) >= 0) {
					sound_id++;
				}
				if (AoPadSomeoneRepeat(KEY_R_UP) >= 0) {
					sound_id--;
				}
				sound_id	= MTM_MATH_CLIP(sound_id, 0, 2);
				break;
#endif //!_IPHONE
			case 5: // パッド振動
				if (AoPadSomeoneStand(KEY_R_LEFT) >= 0) {
					game_mode = GSD_GAME_MODE_TIME_ATTACK;
				}
				else if (AoPadSomeoneStand(KEY_R_UP) >= 0) {
					game_mode = GSD_GAME_MODE_STORY;
				}
				break;
			default:
				break;
			}

			y = 12;
			x = 3;
			cb.Print(x-1,	y + cursor_pos, "*");	// カーソル表示

			// ZONE表示
			cb.Print(x, y, "ZONE : %s", stage_name[stage_top[zone_id] + act_id]);
			y++;
			// ACT表示
			cb.Print(x, y, "ACT : %s", act_name[stage_top[zone_id] + act_id]);
			y++;
			// コントロールモード
			cb.Print(x, y, "CONTROL MODE : %s", ctrl_type[ctrl_type_no]);
			y++;
			// スーパーソニック自由起動設定
			cb.Print(x, y, "SUPER SONIC START FREE : %s", on_off[super_sonic_free]);
			y++;
#if !_IPHONE
			// パッド振動
			cb.Print(x, y, "PAD VIB : %s", on_off[pad_vib]);
			y++;
#else //!_IPHONE
			// BGM選択
			cb.Print(x, y, "BGM : %s", sound_name[sound_id]);
			y++;
#endif //!_IPHONE
			// ゲームモード
			cb.Print(x, y, "GAME MODE : %s", mode_name[game_mode]);
			y++;


			// 操作説明
			y++;
#if _PC | _XBOX
			cb.Print(x, y, "XY BUTTON     : SELECT");
#elif _PS3
			cb.Print(x, y, "TRY CIR BUTTON: SELECT");
#elif _WII
			cb.Print(x, y, "AB BUTTON     : SELECT");
#else
			cb.Print(x, y, "XY BUTTON     : SELECT");
#endif
			y++;
			cb.Print(x, y, "UP DOWN       : MOVE CURSOL");
			y++;
#if _PC | _XBOX
			cb.Print(x, y, "START/A       : Start");
#elif _PS3
			cb.Print(x, y, "START/CROSS   : Start");
#elif _WII
			cb.Print(x, y, "+/1           : Start");
#else
			cb.Print(x, y, "START/A       : Start");
#endif
			y++;
		}
		break;
	case CEvtCb::ECbKind::Enter:		//<決定直後
		g_gs_main_sys_info.stage_id = (u16)(stage_top[zone_id] + act_id);
		//g_gs_main_sys_info.stage_id 
		g_gs_main_sys_info.game_flag &= ~GSD_MAINSYS_GAME_FLAG_SPECIAL_STAGE;
		// 残機数設定
		g_gs_main_sys_info.rest_player_num = 3;
		if (GSD_MAIN_STAGE_ID_SS1 <= g_gs_main_sys_info.stage_id &&
					g_gs_main_sys_info.stage_id <= GSD_MAIN_STAGE_ID_SS7) {
			g_gs_main_sys_info.char_id[0] = GSD_CHAR_ID_SP_SONIC;
			g_gs_main_sys_info.game_mode = game_mode;
			g_gs_main_sys_info.game_flag |= GSD_MAINSYS_GAME_FLAG_SPECIAL_STAGE;
		}
		else {
			g_gs_main_sys_info.char_id[0] = GSD_CHAR_ID_SONIC;
			g_gs_main_sys_info.game_mode = game_mode;
			g_gs_main_sys_info.game_flag |= GSD_MAINSYS_GAME_FLAG_7_CHAOS_EMERALD;
		}

		switch (ctrl_type_no) {
		case 1: //CLASSIC DOWN
			g_gs_main_sys_info.game_flag &= ~GSD_MAINSYS_GAME_FLAG_INPUT_IS_FLICK;
			g_gs_main_sys_info.game_flag |= GSD_MAINSYS_GAME_FLAG_INPUT_CLASSIC;
			break;
		case 2: //CLASSIC UP
			g_gs_main_sys_info.game_flag |= GSD_MAINSYS_GAME_FLAG_INPUT_IS_FLICK;
			g_gs_main_sys_info.game_flag |= GSD_MAINSYS_GAME_FLAG_INPUT_CLASSIC;
			break;
		case 0: //NORMAL
		default:
			g_gs_main_sys_info.game_flag &= ~GSD_MAINSYS_GAME_FLAG_INPUT_IS_FLICK;
			g_gs_main_sys_info.game_flag &= ~GSD_MAINSYS_GAME_FLAG_INPUT_CLASSIC;
			break;
		}

		g_gs_main_sys_info.debug_flag &= ~GSD_DEBUG_SUPER_SONIC_FREE;
		if (super_sonic_free) {
			g_gs_main_sys_info.debug_flag = GSD_DEBUG_SUPER_SONIC_FREE;
		}

#if !_IPHONE
		g_gs_main_sys_info.game_flag &= ~GSD_MAINSYS_GAME_FLAG_PAD_VIB_ENABLE;
		if (pad_vib) {
			g_gs_main_sys_info.game_flag |= GSD_MAINSYS_GAME_FLAG_PAD_VIB_ENABLE;
		}
#endif //!_IPHONE

		if (GSD_MAIN_STAGE_ID_FINAL_1 <= g_gs_main_sys_info.stage_id &&
			g_gs_main_sys_info.stage_id <= GSD_MAIN_STAGE_ID_FINAL_5) {
			
			g_gs_main_sys_info.debug_flag	&= ~GSD_DEBUG_ZONE_FINAL_CHECK_MASK;
			switch (g_gs_main_sys_info.stage_id) {
			case GSD_MAIN_STAGE_ID_FINAL_1:
				break;
			case GSD_MAIN_STAGE_ID_FINAL_2:
				break;
			case GSD_MAIN_STAGE_ID_FINAL_3:
				break;
			case GSD_MAIN_STAGE_ID_FINAL_4:
				break;
			case GSD_MAIN_STAGE_ID_FINAL_5:
				g_gs_main_sys_info.debug_flag	|= GSD_DEBUG_ZONE_FINAL_5_CHECK;
				break;
			}
			// ステージFinal_2 ～ Final_5 はステージIDとして設定しない
			g_gs_main_sys_info.stage_id	= GSD_MAIN_STAGE_ID_FINAL_1;
		}

#if !_IPHONE
		g_gs_main_sys_info.debug_flag |= GSD_DEBUG_DEBUG_DISP;
#else //!_IPHONE
		g_gs_main_sys_info.debug_flag &= ~GSD_DEBUG_DEBUG_DISP;
#endif //!_IPHONE

		GmMainGSInit();
		break;
	case CEvtCb::ECbKind::Init:		//<初期化
	case CEvtCb::ECbKind::Exit:		//<後処理
	case CEvtCb::ECbKind::OnFocus:	//<選択された時
	case CEvtCb::ECbKind::OnBlur:	//<非選択になった時
	case CEvtCb::ECbKind::Blur:		//<非選択時
	default:
		break;
	}


	UNREFERENCED_PARAMETER(cb_data);
}




// =============================================================================
// dbgEndingCb
/*!
	エンディング用のコールバック関数

	@param	kind	[in]	コールバックの種類
	@param	cb		[in]	コールバック実データ
	@param	cb_data	[in]	ユーザーデータ
 */
// ==========================================================================
void dbgEndingCb(CEvtCb::ECbKind::Type kind, CEvtCb &cb, void *cb_data)
{

	static	s32	cursor_pos		= 0;
	static	s32	ctrl_type_no	= 1;
	static	s32	super_sonic_free= 0;
	static	s32 pad_vib = 0;
	static	s32 stfrl_type = 0;

	const char *ctrl_type[] = {
				"NORMAL",
				"CLASSIC DOWN",
				"CLASSIC UP",
	};
	const char *on_off[2] ={
		"OFF",
		"ON",
	};
	
	const char *stfrl_type_str[2] ={
		"FULL",
		"NOFULL",
	};


	s32		x, y;

	switch (kind) {
	case CEvtCb::ECbKind::Focus:		//<選択時
		{
			//設定
			if (AoPadSomeoneRepeat(KEY_L_DOWN) >= 0) {
				cursor_pos++;
				if (cursor_pos > 3) {
					cursor_pos = 0;
				}
			}
			else if (AoPadSomeoneRepeat(KEY_L_UP) >= 0) {
				cursor_pos--;
				if (cursor_pos < 0) {
					cursor_pos = 3;
				}
			}
			cursor_pos	= MTM_MATH_CLIP(cursor_pos, 0, 3);


			switch (cursor_pos) {
			case 0:	// 操作方法選択
				if (AoPadSomeoneStand(KEY_R_LEFT) >= 0) {
					if (ctrl_type_no < arrayof(ctrl_type) - 1) {
						++ctrl_type_no;
					}
				} else if (AoPadSomeoneStand(KEY_R_UP) >= 0) {
					if (0 < ctrl_type_no) {
						--ctrl_type_no;
					}
				}
				break;
			case 1:	// スーパーソニック フリー設定
				if (AoPadSomeoneStand(KEY_R_LEFT | KEY_R_UP) >= 0) {
					super_sonic_free ^= 0x01;
				}
				break;
			case 2: // パッド振動
				if (AoPadSomeoneStand(KEY_R_LEFT | KEY_R_UP) >= 0) {
					pad_vib ^= 0x01;
				}
				break;
			case 3: // スタッフロールタイプ設定
				if (AoPadSomeoneStand(KEY_R_LEFT | KEY_R_UP) >= 0) {
					stfrl_type ^= 0x01;
				}
				break;
			default:
				break;
			}

			y = 12;
			x = 3;
			cb.Print(x-1,	y + cursor_pos, "*");	// カーソル表示
			// コントロールモード
			cb.Print(x, y, "CONTROL MODE : %s", ctrl_type[ctrl_type_no]);
			y++;
			// スーパーソニック自由起動設定
			cb.Print(x, y, "SUPER SONIC START FREE : %s", on_off[super_sonic_free]);
			y++;
			// パッド振動
			cb.Print(x, y, "PAD VIB : %s", on_off[pad_vib]);
			y++;
			// スタッフロールタイプ設定
			cb.Print(x, y, "STAFF ROLL TYPE : %s", stfrl_type_str[stfrl_type]);
			y++;


			// 操作説明
			y++;
#if _PC | _XBOX
			cb.Print(x, y, "XY BUTTON     : SELECT");
#elif _PS3
			cb.Print(x, y, "TRY CIR BUTTON: SELECT");
#elif _WII
			cb.Print(x, y, "AB BUTTON     : SELECT");
#else
			cb.Print(x, y, "XY BUTTON     : SELECT");
#endif
			y++;
			cb.Print(x, y, "UP DOWN       : MOVE CURSOL");
			y++;
#if defined(MTD_DEBUG)
			cb.Print(x, y, "%c       : Start", GsEnvDebugGetDecideKeyChar());
#else
			cb.Print(x, y, "-       : Start");
#endif //defined(MTD_DEBUG)

			y++;
		}
		break;
	case CEvtCb::ECbKind::Enter:		//<決定直後
		g_gs_main_sys_info.stage_id		= GSD_MAIN_STAGE_ID_ENDING;
		g_gs_main_sys_info.char_id[0]	= GSD_CHAR_ID_SONIC;
		g_gs_main_sys_info.game_mode	= GSD_GAME_MODE_ENDING;

		switch (ctrl_type_no) {
		case 1: //CLASSIC DOWN
			g_gs_main_sys_info.game_flag &= ~GSD_MAINSYS_GAME_FLAG_INPUT_IS_FLICK;
			g_gs_main_sys_info.game_flag |= GSD_MAINSYS_GAME_FLAG_INPUT_CLASSIC;
			break;
		case 2: //CLASSIC UP
			g_gs_main_sys_info.game_flag |= GSD_MAINSYS_GAME_FLAG_INPUT_IS_FLICK;
			g_gs_main_sys_info.game_flag |= GSD_MAINSYS_GAME_FLAG_INPUT_CLASSIC;
			break;
		case 0: //NORMAL
		default:
			g_gs_main_sys_info.game_flag &= ~GSD_MAINSYS_GAME_FLAG_INPUT_IS_FLICK;
			g_gs_main_sys_info.game_flag &= ~GSD_MAINSYS_GAME_FLAG_INPUT_CLASSIC;
			break;
		}

		g_gs_main_sys_info.debug_flag &= ~GSD_DEBUG_SUPER_SONIC_FREE;
		g_gs_main_sys_info.game_flag &= ~GSD_MAINSYS_GAME_FLAG_7_CHAOS_EMERALD;
		if (super_sonic_free) {
			g_gs_main_sys_info.debug_flag = GSD_DEBUG_SUPER_SONIC_FREE;
			g_gs_main_sys_info.game_flag |= GSD_MAINSYS_GAME_FLAG_7_CHAOS_EMERALD;
		}

		g_gs_main_sys_info.game_flag &= ~GSD_MAINSYS_GAME_FLAG_PAD_VIB_ENABLE;
		if (pad_vib) {
			g_gs_main_sys_info.game_flag |= GSD_MAINSYS_GAME_FLAG_PAD_VIB_ENABLE;
		}

#if !_IPHONE
		g_gs_main_sys_info.debug_flag |= GSD_DEBUG_DEBUG_DISP;
#else //!_IPHONE
		g_gs_main_sys_info.debug_flag &= ~GSD_DEBUG_DEBUG_DISP;

		{
			//パッドトリガのモード切替設定
			dbg::CPadEmu &pad_emu = dbg::CPadEmu::CreateInstance();
			pad_emu.Create(dbg::CPadEmu::EMode::Game);
		}
#endif //!_IPHONE

		if (stfrl_type) {
			dm_stfrl_is_full_disp_efct = FALSE;
		}
		else {
			dm_stfrl_is_full_disp_efct = TRUE;
		}

		GmMainGSInit();
		break;
	case CEvtCb::ECbKind::Init:		//<初期化
	case CEvtCb::ECbKind::Exit:		//<後処理
	case CEvtCb::ECbKind::OnFocus:	//<選択された時
	case CEvtCb::ECbKind::OnBlur:	//<非選択になった時
	case CEvtCb::ECbKind::Blur:		//<非選択時
	default:
		break;
	}

	UNREFERENCED_PARAMETER(cb_data);
}






#endif //#if defined(_INC_DBG_EVT_SELECTOR_STATIC_FUNCTIONS)
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//------ Static Definitions ----------- ローカル関数宣言 -------------------------
#if defined(_INC_DBG_EVT_SELECTOR_STATIC_CLASSES)
//ToDo: ▼▼▼ここにクラスを追加します▼▼▼

#if 0
class TEMPLATE_CEvtFunction : public CEvtBase {}
#endif












#endif //#if defined(_INC_DBG_EVT_SELECTOR_STATIC_CLASSES)
//--------------------------------------------------------------------------------
