#ifndef	CRI_AUDIO_H_INCLUDED		/* Re-definition prevention */
#define	CRI_AUDIO_H_INCLUDED
/****************************************************************************
 *
 * CRI Middleware SDK
 *
 * Copyright (c) 2005-2009 CRI Middleware Co.,LTD.
 *
 * Library  : CRI Audio
 * Module   : Library User's Header
 * File     : cri_audio.h
 * Date     : 2005-11-29
 *
 ****************************************************************************/
/*!
 *	\file		cri_audio.h
 */

#include <math.h>
#include <string.h>
#include <cri_xpt.h>
#include <cri_heap.h>
#include <cri_allocator.h>
#include <cri_error.h>
#include "cri_sound_renderer.h"
#include "cri_file_system.h"

#define CRI_AUDIO_RT_VERSION		(0x03490500)
#define CRI_AUDIO_RT_VER_NAME		"CRI Audio"
#define CRI_AUDIO_RT_VER_NUM		"3.49.06 (custom)"

#if (CRI_FS_VERSION < 0x02000000)
#define CRI_AUDIO_RT_VER_OPTION	"for CriFs1"
#else
#define CRI_AUDIO_RT_VER_OPTION	"for CriFs2"
#endif

class CriAuObj;
class CriAuPlayer;
class CriAuCueSheet;
class CriFsManager;

/*JP
 * \ingroup MDL_LIB_GLOBAL
 * \defgroup NAMESPACE_CRI_AU CriAu
 * \brief CriAuネームスペース
 * \par 説明:
 * CRI Audioライブラリ全体で使用する定数や、CRI Audioライブラリ全体に影響する関数が含まれます。
 */
/*EN
 * \ingroup MDL_LIB_GLOBAL
 * \defgroup NAMESPACE_CRI_AU CriAu
 * \brief CriAu namespace
 * \par Description:
 * This namespace includes constants/class for the CRI Audio library and functions affecting all of the CRI Audio library.
 */
namespace CriAu {
/*JP
 * \addtogroup NAMESPACE_CRI_AU
 * @{
 */
/*EN
 * \addtogroup NAMESPACE_CRI_AU
 * @{
 */
	static const CriUint32 MAX_CHANNELS=8;
	static const CriUint32 MAX_AISAC_NAME=128;
	static const CriUint32 MAX_AISAC_CONTROL_NAME=128;
	static const CriUint32 MAX_AISAC_PATTERN_NAME=128;
	static const CriUint32 MAX_VOICES=65535;

	/*JP
	 * \brief 音声データ形式
	 */
	/*EN
	 * \brief Sound data format
	 */
	enum Format {
		FORMAT_AAX=(0),						/*JP< AAXフォーマット */
											/*EN< AAX format */
	};

	/*JP
	 * \brief AISACグラフタイプ
	 */
	/*EN
	 * \brief AISAC graph type
	 */
	enum AisacGraphType {
		AISAC_GRAPH_TYPE_VOLUME=(0),		/*JP< ボリューム */
											/*EN< Volume */
		AISAC_GRAPH_TYPE_PITCH,				/*JP< ピッチ */
											/*EN< Pitch */
		AISAC_GRAPH_TYPE_DELAY,				/*JP< プリディレイ */
											/*EN< PreDelay */
		AISAC_GRAPH_TYPE_COF_LOW,			/*JP< フィルターカットオフLow */
											/*EN< Filter cutoff low */
		AISAC_GRAPH_TYPE_COF_HIGH,			/*JP< フィルターカットオフHigh */
											/*EN< Filter cutoff high */
		AISAC_GRAPH_TYPE_DRY_L,				/*JP< L ch ドライセンドレベル */
											/*EN< L ch dry send level */
		AISAC_GRAPH_TYPE_DRY_R,				/*JP< R ch ドライセンドレベル */
											/*EN< R ch dry send level */
		AISAC_GRAPH_TYPE_DRY_LS,			/*JP< Ls ch ドライセンドレベル */
											/*EN< Ls ch dry send level */
		AISAC_GRAPH_TYPE_DRY_RS,			/*JP< Rs ch ドライセンドレベル */
											/*EN< Rs ch dry send level */
		AISAC_GRAPH_TYPE_DRY_C,				/*JP< C ch ドライセンドレベル */
											/*EN< C ch dry send level */
		AISAC_GRAPH_TYPE_DRY_LFE,			/*JP< LFE ドライセンドレベル */
											/*EN< LFE dry send level */
		AISAC_GRAPH_TYPE_DRY_EX1,			/*JP< EX1 ドライセンドレベル */
											/*EN< EX1 dry send level */
		AISAC_GRAPH_TYPE_DRY_EX2,			/*JP< EX2 ドライセンドレベル */
											/*EN< EX2 dry send level */
		AISAC_GRAPH_TYPE_WET0,				/*JP< Wet0 センドレベル */
											/*EN< Wet0 send level */
		AISAC_GRAPH_TYPE_WET1,				/*JP< Wet1 センドレベル */
											/*EN< Wet1 send level */
		AISAC_GRAPH_TYPE_WET2,				/*JP< Wet2 センドレベル */
											/*EN< Wet2 send level */
		AISAC_GRAPH_TYPE_WET3,				/*JP< Wet3 センドレベル */
											/*EN< Wet3 send level */
		AISAC_GRAPH_TYPE_WET4,				/*JP< Wet4 センドレベル */
											/*EN< Wet4 send level */
		AISAC_GRAPH_TYPE_WET5,				/*JP< Wet5 センドレベル */
											/*EN< Wet5 send level */
		AISAC_GRAPH_TYPE_WET6,				/*JP< Wet6 センドレベル */
											/*EN< Wet6 send level */
		AISAC_GRAPH_TYPE_WET7,				/*JP< Wet7 センドレベル */
											/*EN< Wet7 send level */
		AISAC_GRAPH_TYPE_PAN3D_ANGLE,		/*JP< パン３D　角度 */
											/*EN< Pan3D angle */
		AISAC_GRAPH_TYPE_PAN3D_IDIST,		/*JP< パン３D　インテリア距離 */
											/*EN< Pan3D interior distance */
		AISAC_GRAPH_TYPE_PLAY_GATE,			/*JP< PLAY動作のGate(0でStop)*/
											/*EN< PLAY Gate(0=Stop) */
		AISAC_GRAPH_TYPE_PRIORITY,			/*JP< プライオリティ*/
											/*EN< PRIORITY */
		AISAC_GRAPH_TYPE_PAN3D_VOLUME,		/*JP< Pan3d ボリューム*/		//	pan3dVolume追加(tanakat 091012)
											/*EN< Pan3d volume */									
	};

	/*JP
	 * \brief ストリーミングする音声の仕様
	 */
	/*EN
	 * \brief Sound specification for streaming
	 */
	class StreamSpecSound {
	public:
		CriFsManager *fsmng;				/*JP< ファイルシステムマネージャ */
											/*EN< File system manager */
		CriAu::Format format;				/*JP< 音声データ形式 */
											/*EN< Sound data format */
		CriUint32 nch;							/*JP< チャンネル数 */
											/*EN< Number of channels */
		CriUint32 sampling_rate;				/*JP< サンプリングレート */
											/*EN< Sampling rate */

		StreamSpecSound() {
			this->fsmng = NULL;
			this->format = CriAu::FORMAT_AAX;
			this->nch = 0;
			this->sampling_rate = 0;
		};
	};

	/*JP
	 * \brief 乱数発生関数
	 * \par 説明:
	 *	乱数発生関数は0から最大値（関数登録時に指定）の間の値を返してください。
	 * \sa CriAu::SetRandomCallback
	 */
	/*EN
	 * \brief Random number generator.
	 * \par Description:
	 * A random number generator must return a value between 0 and the max number specified at the time of registering the function.
	 * \sa CriAu::SetRandomCallback
	 */
	typedef CriSint32 (*RandomCallbackFunction)(CriSint32 max);

	/*JP
	 * \brief 乱数発生関数の登録
	 * \param function 乱数発生関数
	 * \param max 発生しうる最大値（登録した乱数発生関数にも渡されます）
	 * \param err エラーコード
	 * \return	なし
	 * \par 説明:
	 *	乱数発生関数を登録します。ここで登録した乱数発生関数は、CRI Audioライブラリ内部で乱数を必要とする際に使用されます。<br>
	 *	何も登録しない場合には、C標準のrand関数を使用します。
	 * \sa CriAu::RandomCallbackFunction
	 */
	/*EN
	 * \brief Register a random number generator.
	 * \param funtion A random number generator
	 * \param max The maximum number that may be returned. Also this parameter is passed to the random number generator.
	 * \param err CriError information
	 * \par Description:
	 * This function registers a function that generates random number. The registered function is used in the CRI Audio library.<br>
	 * If any function is not set, rand() in C standard library is used.
	 * \sa CriAu::RandomCallbackFunction
	 */
	void CRIAPI SetRandomCallback(CriAu::RandomCallbackFunction, CriSint32 max, CriError &err = criErr::ErrorContainer);


	/*JP
	 * \brief ボイスリミットコールバックの種類
	 * \sa CriAu::VoiceLimitCallbackInformation
	 */
	/*EN
	 * \brief Type of voice limit callback
	 * \sa CriAu::VoiceLimitCallbackInformation
	 */
	enum VoiceLimitCallbackType {
		VOICE_LIMIT_CALLBACK_TYPE_GROUP = 0,				/*JP< VoiceLimitGroupによるリミット */
															/*EN< Limited by VoiceLimitGroup */
		VOICE_LIMIT_CALLBACK_TYPE_SOUND_RENDERER_MAX		/*JP< SoundRendererの最大発音数によるリミット */
															/*EN< Limited by maximum number of voices in SoundRenderer */
	};

	/*JP
	 * \brief ボイスリミットコールバック情報
	 */
	/*EN
	 * \brief Information of voice limit callback
	 */
	struct VoiceLimitCallbackInformation {
		VoiceLimitCallbackType type;		/*JP< ボイスリミットコールバックの種類 */
											/*EN< Type of voice limit callback */
		const CriChar8* requested_synth_name;	/*JP< リクエストしたシンセの名前 */
											/*EN< Name of a requested synthesizer */
		const CriChar8* dropped_synth_name;	/*JP< ドロップしたシンセの名前 */
											/*EN< Name of a dropped synthesizer */
	};

	/*JP
	 * \brief ボイスリミットコールバック関数
	 * \par 説明:
	 * コールバック関数が呼ばれた際、informationにボイスリミットコールバック情報が設定されています。user_objは任意に設定可能な引数です。
	 * \sa CriAu::SetVoiceLimitCallback
	 */
	/*EN
	 * \brief Voice limit callback function
	 * \par Description:
	 * When this callback is called, information of voice limit callback is set as "information". "user_obj" is available for user.(optional)
	 * \sa CriAu::SetVoiceLimitCallback
	 */
	typedef void (*VoiceLimitCallbackFunction)(const VoiceLimitCallbackInformation& information, void* user_obj);

	/*JP
	 * \brief ボイスリミットコールバック関数の登録
	 * \param function ボイスリミットコールバック関数
	 * \param user_obj 任意で指定可能なユーザー引数
	 * \param err エラーコード
	 * \return	なし
	 * \par 説明:
	 *	ボイスリミットコールバック関数を登録します。ここで登録したボイスリミットコールバック関数は、CRI Audioライブラリ内部でボイスリミットが発生した際に呼び出されます。<br>
	 * \sa CriAu::VoiceLimitCallbackFunction
	 */
	/*EN
	 * \brief Register a voice limit callback function.
	 * \param funtion A voice limit callback function
	 * \param user_obj user argument(optional)
	 * \param err CriError information
	 * \par Description:
	 * This function registers a voice limit callback function. When voice limit occurred in the CRI Audio library, the registered function is called.<br>
	 * \sa CriAu::VoiceLimitCallbackFunction
	 */
	void CRIAPI SetVoiceLimitCallback(CriAu::VoiceLimitCallbackFunction function, void* user_obj = NULL, CriError &err = criErr::ErrorContainer);

	/*JP
	 * \brief ストリーミング再生時の最小リロード時間の設定
	 * \param min_reload_time 最小リロード時間
	 * \param err エラーコード
	 * \return	なし
	 * \par 説明:
	 *	ストリーミング再生時の最小リロード時間を設定します。<br>
	 *	入力バッファの残り時間が最小リロード時間を切った場合、ロードが発生します。<br>
	 *	通常は、本関数を使用する必要はありません。
	 */
	void CRIAPI SetMinStreamReloadTime(CriFloat32 min_reload_time);

	/*JP
	 * \brief ストリーミング再生用バッファの確保
	 * \param heap ヒープハンドル
	 * \param size バッファサイズ
	 * \param err エラーコード
	 * \return	なし
	 * \par 説明:
	 * ストリーミング再生用バッファを確保します。<br>
	 * 適切なサイズは CriAuUtility::CalcStreamingMinimumBufferSize 関数により計算できます。
	 * \sa CriAuUtility::CalcStreamingMinimumBufferSize
	 * \sa CriAu::ReleaseStreamingBuffer
	 */
	void CRIAPI AllocateStreamingBuffer(CriHeap heap, CriUint32 size, CriError &err = criErr::ErrorContainer);

	/*JP
	 * \brief ストリーミング再生用バッファの解放
	 * \param heap ヒープハンドル
	 * \param size バッファサイズ
	 * \param err エラーコード
	 * \return	なし
	 * \par 説明:
	 *	ストリーミング再生用バッファを解放します。
	 * \sa CriAu::AllocateStreamingBuffer
	 */
	void CRIAPI ReleaseStreamingBuffer(CriError &err = criErr::ErrorContainer);
/*JP
 * @} NAMESPACE_CRI_AU
 */
/*EN
 * @} NAMESPACE_CRI_AU
 */
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
/*JP
 * \brief CriFsネームスペース
 * \par 説明:
 * 旧バージョンとの互換用です。
 */
/*EN
 * \brief CriFs namespace
 * \par Description:
 * For backward compatibility
 */
namespace CriFs {
	inline void CRIAPI AllocateStreamingBuffer(CriHeap heap, CriUint32 size, CriError &err = criErr::ErrorContainer)
	{
		CriAu::AllocateStreamingBuffer(heap, size, err);
	}

	inline void CRIAPI ReleaseStreamingBuffer(CriError &err = criErr::ErrorContainer)
	{
		CriAu::ReleaseStreamingBuffer(err);
	}
}
#endif


/*JP
 * \ingroup MDL_CRI_AU_SEND_LEVEL
 * \brief 出力ブロックへのセンドレベル
 * \par 説明:
 * 各バスへのセンドレベルです。<br>
 * ドライやウェットへの出力レベルを設定するために使用します。<br>
 * 値は入力に対する出力のリニア比を表します。例えば0.5を設定した場合は、出力値は入力値の半分になります。<br>
 * 設定可能な値の範囲は0.0～1.0です。0.0はまったく出力しないことを、1.0は入力をそのまま出力することを意味します。<br>
 * \sa CriAuPlayer::SetDrySendLevel(), CriAuPlayer::SetWetSendLevel(), 
 */
/*EN
 * \ingroup MDL_CRI_AU_SEND_LEVEL
 * \brief A send level value to each output block bus
 * \par Description:
 * Used to set an output level for dry output and wet output. <br>
 * A level means the linear ratio of the output to the input. For example, 
 * if you set the level to 0.5, the output becomes half of the input.<br>
 * The range of the value is from 0.0f (no output) to 1.0f (pass through).
 * \sa CriAuPlayer::SetDrySendLevel(), CriAuPlayer::SetWetSendLevel(), 
 */
class CriAuSendLevel {
/*JP
 * \addtogroup MDL_CRI_AU_SEND_LEVEL
 * @{
 */
/*EN
 * \addtogroup MDL_CRI_AU_SEND_LEVEL
 * @{
 */
public:
	static const CriUint32 MAX_OUTPUT=8;		/*JP< 最大出力数 */
											/*EN< Maximum number of output buses */
	CriAuSendLevel(void) {
		for (CriUint32 i=0; i<MAX_OUTPUT; i++)
			this->level[i] = 0.0f;
	}
	CriAuSendLevel(CriFloat32 init_val) {
		for (CriUint32 i=0; i<MAX_OUTPUT; i++)
			this->level[i] = init_val;
	}
	enum DryAssign {
		DRY_L = (0),						/*JP< 左スピーカ	*/
											/*EN< Left speaker */
		DRY_R,								/*JP< 右スピーカ	*/
											/*EN< Right speaker */
		DRY_LS,								/*JP< 左サラウンドスピーカ	*/
											/*EN< Left surround speaker */
		DRY_RS,								/*JP< 右サラウンドスピーカ	*/
											/*EN< Right surround speaker */
		DRY_C,								/*JP< 中央スピーカ	*/
											/*EN< Center speaker */
		DRY_LFE,							/*JP< 低域スピーカ	*/
											/*EN< Subwoofer speaker */
		DRY_EXT0,							/*JP< 拡張０スピーカ	*/
											/*EN< Extension-0 speaker */
		DRY_EXT1							/*JP< 拡張１スピーカ	*/
											/*EN< Extension-1 speaker */
	};
	enum WetAssign {
		WET_0 = (0),						/*JP< エフェクトブロックへの出力０(デフォルトではリバーブ)	*/
											/*EN< Effect block 0 (REVERB is assigned as a default) */
		WET_1,								/*JP< エフェクトブロックへの出力１(デフォルトではエコー)	*/
											/*EN< Effect block 1 (DELAY is assigned as a default) */
		WET_2,								/*JP< エフェクトブロックへの出力２(デフォルトでは未設定)	*/
											/*EN< Effect block 2 */
		WET_3,								/*JP< エフェクトブロックへの出力３(デフォルトでは未設定)	*/
											/*EN< Effect block 3 */
		WET_4,								/*JP< エフェクトブロックへの出力４(デフォルトでは未設定)	*/
											/*EN< Effect block 4 */
		WET_5,								/*JP< エフェクトブロックへの出力５(デフォルトでは未設定)	*/
											/*EN< Effect block 5 */
		WET_6,								/*JP< エフェクトブロックへの出力６(デフォルトでは未設定)	*/
											/*EN< Effect block 6 */
		WET_7								/*JP< エフェクトブロックへの出力７(デフォルトでは未設定)	*/
											/*EN< Effect block 7 */
	};
	CriFloat32 level[MAX_OUTPUT];				/*JP< センドレベル	*/
											/*EN< Current levels for Dry buses */
	/*JP
	 * \brief 左スピーカへセンドレベルの設定
	 * \par 説明:
	 * 左スピーカへセンドレベルを設定します。
	 */
	/*EN
 	 * \brief Set a send level to the Left speaker
	 */
	void SetLeft(CriFloat32 lvl)			{level[DRY_L] = lvl;}
	/*JP
	 * \brief 右スピーカへセンドレベルの設定
	 * \par 説明:
	 * 右スピーカへセンドレベルを設定します。
	 */
	/*EN
 	 * \brief Set a send level to the Right speaker
	 */
	void SetRight(CriFloat32 lvl)			{level[DRY_R] = lvl;}
	/*JP
	 * \brief 中央スピーカへセンドレベルの設定
	 * \par 説明:
	 * 中央スピーカへセンドレベルを設定します。
	 */
	/*EN
 	 * \brief Set a send level to the Center speaker
	 */
	void SetCenter(CriFloat32 lvl)		{level[DRY_C] = lvl;}
	/*JP
	 * \brief 左サラウンドスピーカへセンドレベルの設定
	 * \par 説明:
	 * 左サラウンドスピーカへセンドレベルを設定します。
	 */
	/*EN
 	 * \brief Set a send level to the Left surround speaker
	 */
	void SetLeftSurround(CriFloat32 lvl)		{level[DRY_LS] = lvl;}
	/*JP
	 * \brief 右サラウンドスピーカへセンドレベルの設定
	 * \par 説明:
	 * 右サラウンドスピーカへセンドレベルを設定します。
	 */
	/*EN
 	 * \brief Set a send level to the Right surround speaker
	 */
	void SetRightSurround(CriFloat32 lvl)		{level[DRY_RS] = lvl;}
	/*JP
	 * \brief 低域スピーカへセンドレベルの設定
	 * \par 説明:
	 * 低域スピーカへセンドレベルを設定します。
	 */
	/*EN
 	 * \brief Set a send level to the Subwoofer speaker
	 */
	void SetLowFrequencyEffect(CriFloat32 lvl)	{level[DRY_LFE] = lvl;}
/*JP
 * @} MDL_CRI_AU_SEND_LEVEL
 */
/*EN
 * @} MDL_CRI_AU_SEND_LEVEL
 */
};


/*JP
 * \ingroup MDL_LIB_CUE_SHEET
 * \brief キューシートハンドル
 * キューシートバイナリをロードするために使用します。
 */
/*EN
 * \ingroup MDL_LIB_CUE_SHEET
 * \brief Cue Sheet Handle
 * Cue Sheet Handles are used to load a Cue Sheet Binary (.csb) file.
 * You can <b>load</b> a Cue Sheet from a .csb file or you can just <b>start</b> reading
 * an asynchronous load of a .csb file. Or if you have already loaded a .csb file into memory,
 * you may load it from the memory.
 * In case of an asynchronous load, you need to check the handle status to determine whether it is done or
 * not.
 */
class CriAuCueSheet : public CriAllocator
{
/*JP
 * \addtogroup MDL_LIB_CUE_SHEET
 * @{
 */
/*EN
 * \addtogroup MDL_LIB_CUE_SHEET
 * @{
 */
public:
	/*JP
	 * \brief ロードステータス
	 */
	/*EN
	 * \brief Loading status
	 */
	enum LoadStatus {
		LOAD_STATUS_STOP = (0),			/*JP< 停止		*/
										/*EN< Stop (Initial state) */
		LOAD_STATUS_LOADING,			/*JP< ロード中	*/
										/*EN< Now loading */
		LOAD_STATUS_COMPLETE,			/*JP< ロード完了	*/
										/*EN< Loading Complete */
		LOAD_STATUS_ERROR				/*JP< エラー	*/
										/*EN< Error */
	};

	/*JP
	 * \brief シンセタイプ
	 */
	/*EN
	 * \brief Synthesizer type
	 */
	enum SynthType {
		SYNTH_TYPE_PRIMITIVE = 0,		/*JP< プリミティブシンセサイザ */
										/*EN< Primitive synthesizer */
		SYNTH_TYPE_COMPLEX,				/*JP< コンプレックスシンセサイザ */
										/*EN< Complex synthesizer */
	};

	/*JP
	 * \brief 波形情報
	 */
	/*EN
	 * \brief Waveform information
	 */
	struct WaveFormInfo {
		CriUint32 num_channels;		/*JP< チャンネル数 */
									/*EN< Number of Channels */
		CriUint32 sampling_rate;		/*JP< サンプリングレート */
									/*EN< Sampling rate */
		CriUint32 num_samples;			/*JP< サンプル数 */
									/*EN< Number of samples */
	};

	/*JP
	 * \brief キューシートハンドルの生成
	 * \param heap ヒープハンドル<br>
	 * \param err エラーコード<br>
	 * \par 説明:
	 * キューシートハンドルを生成します。
	 * \code
	 * CriError err;
	 * CriHeap heap = criHeap_Create(buf, sizeof(buf));
	 * // Create Cue Sheet
	 * CriAuCueSheet* cue_sheet = CriAuCueSheet::Create(heap, err);
	 * \endcode
	 */
	/*EN
	 * \brief Create a cue sheet handle
	 * \param heap A CriHeap handle
	 * \param err CriError information
	 * \return A valid cue sheet handle (CriAuCueSheet)
	 * \par Description:
	 * This function creates a CriAuCueSheet handle in the LOAD_STATUS_STOP state.
	 * Memory for the handle is allocated from the given CriHeap structure.
	 * Any memory allocation failure during this function results in NULL.
	 * Make sure to initialize and create your heap with criHeap_Initialize() and
	 * criHeap_Create() before calling this function.
	 * \code
	 * CriError err;
	 * CriHeap heap = criHeap_Create(buf, sizeof(buf));
	 * // Create Cue Sheet
	 * CriAuCueSheet* cue_sheet = CriAuCueSheet::Create(heap, err);
	 * \endcode
	 * \sa CriAuCueSheet, criHeap_Initialize(), criHeap_Create()
	 */
	static CriAuCueSheet* CRIAPI Create(CriHeap heap, CriError &err = criErr::ErrorContainer);

	/*JP
	 * \brief キューシートハンドルの削除
	 * \param err エラーコード
	 * \par 説明:
	 * キューシートハンドルを削除します。
	 * \code
	 * CriError err;
	 * CriHeap heap = criHeap_Create(buf, sizeof(buf));
	 * CriAuCueSheet* cue_sheet = CriAuCueSheet::Create(heap, err);
	 * // Destroy Cue Sheet
	 * cue_sheet->Destroy(err);
	 * \endcode
	 */
	/*EN
	 * \brief Destroy a cue sheet handle
	 * \param err CriError information
	 * \par Description:
	 * This function destroys the CriAuCueSheet handle previously created
	 * with CriAuCueSheet::Create.
	 * \code
	 * CriError err;
	 * CriHeap heap = criHeap_Create(buf, sizeof(buf));
	 * CriAuCueSheet* cue_sheet = CriAuCueSheet::Create(heap, err);
	 * // Destroy Cue Sheet
	 * cue_sheet->Destroy(err);
	 * \endcode
	 */
	virtual void Destroy(CriError &err = criErr::ErrorContainer) = 0;

	/*JP
	 * \brief キューシートバイナリファイルのロード
	 * \param fsmng ファイルシステムマネージャ
	 * \param filename キューシートバイナリファイルのファイル名
	 * \param err エラーコード
	 * \par 説明:
	 * キューシートバイナリファイルのロードを開始します。<br>
	 * 本関数は即時復帰関数です。ロードの完了状態を取得するには CriAuCueSheet::GetLoadStatus関数を使用してください。<br>
	 * fsmngはCPKファイルとしてパッキングされたファイルの中からロードするときに使用します。<br>
	 * CriFsManager（ファイルシステムマネージャ）の詳細については、CRI File Systemのマニュアルをご覧ください。
	 * \code
	 * //	Initialize CRI File System
	 * CriFs::Initialize(err);
	 * //	Create File System Manager
	 * CriFsManager *manager = CriFsManagerStandard::Create(heap, DATA_DIR, err);
	 * 
	 * // Create Cue Sheet
	 * CriAuCueSheet* cue_sheet=CriAuCueSheet::Create(heap, err);
	 * // Load Cue Sheet Binary File to Cue Sheet Object
	 * cue_sheet->StartLoadingCueSheetBinaryFile(manager, "tutorial.csb", err);
	 * // Wait for loading completed
	 * for (;;) {
	 * 	CriAuCueSheet::LoadStatus load_status = cue_sheet->GetLoadStatus(err);
	 * 	if ( load_status == CriAuCueSheet::LOAD_STATUS_COMPLETE ) {
	 * 		break;
	 * 	}
	 * }
	 * \endcode
	 * \sa CriAuCueSheet::GetLoadStatus()
	 * \sa CriAuCueSheet::UnloadCueSheetBinaryFile()
	 */
	/*EN
	 * \brief Start loading cue sheet binary file asynchronously
	 * \param fsmng File System Manager
	 * \param filename Cue sheet binary file name
	 * \param err CriError information
	 * \par Description:
	 * This function starts loading cue sheet binary file.<br>
	 * This function returns immediately.
	 * You can check the current loading status of the CriAuCueSheet handle by calling
	 * CriAuCueSheet::GetLoadStatus function.
	 * A .cpk file (a packed file format) is given which include a cue sheet binary file,
	 * a file system manager object which can mount the .cpk file needs to be used.<br>
	 * Please refer the CRI File System Manual for detail about CriFsManager(File System Manager).
	 * \code
	 * //	Initialize CRI File System
	 * CriFs::Initialize(err);
	 * //	Create File System Manager
	 * CriFsManager *manager = CriFsManagerStandard::Create(heap, DATA_DIR, err);
	 * 
	 * // Create Cue Sheet
	 * CriAuCueSheet* cue_sheet=CriAuCueSheet::Create(heap, err);
	 * // Load Cue Sheet Binary File to Cue Sheet Object
	 * cue_sheet->StartLoadingCueSheetBinaryFile(manager, "tutorial.csb", err);
	 * // Wait for loading completed
	 * for (;;) {
	 * 	CriAuCueSheet::LoadStatus load_status = cue_sheet->GetLoadStatus(err);
	 * 	if ( load_status == CriAuCueSheet::LOAD_STATUS_COMPLETE ) {
	 * 		break;
	 * 	}
	 * }
	 * \endcode
	 * \sa CriAuCueSheet::GetLoadStatus()
	 * \sa CriAuCueSheet::UnloadCueSheetBinaryFile()
	 *
	 */
	virtual void StartLoadingCueSheetBinaryFile(CriFsManager* fsmng, const CriChar8* filename, CriError &err = criErr::ErrorContainer) = 0;
#if (CRI_FS_VERSION >= 0x02000000)
	/*JP
	 * \brief キューシートバイナリファイルのロード
	 * \param binder バインダーハンドル
	 * \param filename キューシートバイナリファイルのファイル名
	 * \param err エラーコード
	 * \par 説明:
	 * キューシートバイナリファイルのロードを開始します。<br>
	 * 本関数は即時復帰関数です。ロードの完了状態を取得するには CriAuCueSheet::GetLoadStatus関数を使用してください。<br>
	 * binderはCPKファイルとしてパッキングされたファイルの中からロードするときに使用します。<br>
	 * CriFsBinder（バインダー）の詳細については、ファイルマジックPRO SDKに含まれるCRI File Systemのマニュアルをご覧ください。<br>
	 * binderにはNULLが指定可能で、NULL指定時はバインダーを経由せず、渡されたパスで直接ファイルシステムにアクセスします。<br>
	 * ただし、旧バージョン向け関数と同名の都合上、関数呼び出しのあいまいさ回避のために、(CriFsBinderHn)NULLのようにキャストして指定してください。
	 * \code
	 * // === Initialize CRI File System
	 * CriFsConfiguration	fs_config;
	 * void*				fs_work;
	 * CriSint32			fs_wksize;
	 * // Initialize configuration
	 * criFs_InitializeConfiguration(fs_config);
	 * // Calculate work area size
	 * criFs_CalculateWorkSize(fs_config, &fs_wksize);
	 * // Allocate work area
	 * fs_work = criHeap_AllocFix(heap, fs_wksize, "crifs_work", CRIHEAP_DEFAULT_MEM_ALIGN);
	 * // Initialize CRI File System Library
	 * criFs_Initialize(fs_config, fs_work, fs_wksize);
	 * // ===
	 * // Bind CPK file
	 * CriFsBinderHn		binder;
	 * CriFsBinderId		binder_id;
	 * CriFsBinderStatus	binder_status;
	 * CriSint32			bndr_wksize;
	 * void*				bndr_work;
	 * // Create CriFsBinder handle
	 * criFsBinder_Create(&binder);
	 * // Allocate work area for CPK binding
	 * criFsBinder_GetWorkSizeForBindCpk(NULL, DATA_DIR CPK_FILE_NAME, &bndr_wksize);
	 * bndr_work = criHeap_AllocFix(heap, bndr_wksize, "bndr_work", CRIHEAP_DEFAULT_MEM_ALIGN);
	 * // Bind CPK file
	 * criFsBinder_BindCpk(binder, NULL, DATA_DIR CPK_FILE_NAME, bndr_work, bndr_wksize, &binder_id);
     *
	 * for (;;) {
	 * 	// Check the binder status
	 * 	criFsBinder_GetStatus(binder_id, &binder_status);
	 * 	if (binder_status == CRIFSBINDER_STATUS_COMPLETE) {
	 * 		break;
	 * 	}
	 * 	criFs_ExecuteMain();
	 * }
	 * //	Create Cue Sheet
	 * CriAuCueSheet* cue_sheet=CriAuCueSheet::Create(heap, err);
	 * //	Load Cue Sheet Binary File to Cue Sheet Object
	 * cue_sheet->StartLoadingCueSheetBinaryFile(binder, "tutorial2.csb", err);
	 * // Wait for loading completed
	 * for (;;) {
	 * 	CriAuCueSheet::LoadStatus load_status = cue_sheet->GetLoadStatus(err);
	 * 	criFs_ExecuteMain();
	 * 	if (load_status == CriAuCueSheet::LOAD_STATUS_COMPLETE) {
	 * 		break;
	 * 	}
	 * }
	 * \endcode
	 * \sa CriAuCueSheet::GetLoadStatus()
	 * \sa CriAuCueSheet::UnloadCueSheetBinaryFile()
	 */
	virtual void StartLoadingCueSheetBinaryFile(CriFsBinderHn binder, const CriChar8* filename, CriError &err = criErr::ErrorContainer) = 0;
#endif
	/*JP
	 * \brief ロードステータスの取得
	 * \param err エラーコード
	 * \return	CriAuCueSheet::LoadStatus ロードステータス
	 * \par 説明:
	 * キューシートバイナリファイルのロード状態を取得します。<br>
	 * キューシートハンドルが生成されたときの状態はLOAD_STATUS_STOPです。<br>
	 * CriAuCueSheet::StartLoadingCueSheetBinaryFile関数でロードを開始すると、状態はLOAD_STATUS_LOADINGになります。<br>
	 * ロードが完了すると、状態はLOAD_STATUS_COMPLETEになります。
	 * \code
	 * CriError err;
	 * CriAuCueSheet::LoadStatus load_status = cue_sheet->GetLoadStatus(err);
	 * \endcode
	 * \sa CriAuCueSheet::StartLoadingCueSheetBinaryFile()
	 */
	/*EN
	 * \brief Retrieve the loading status
	 * \param err CriError information
	 * \return One of the CriAuCueSheet::LoadStatus enum values
	 * \par Description:
	 * A handle has LOAD_STATUS_STOP when it is created.
	 * The status will be LOAD_STATUS_LOADING after calling CriAuCueSheet::StartLoadingCueSheetBinaryFile() function.
	 * Finally, LOAD_STATUS_COMPLETE comes when the whole data processing is finished.
	 * \code
	 * CriError err;
	 * CriAuCueSheet::LoadStatus load_status = cue_sheet->GetLoadStatus(err);
	 * \endcode
	 * \sa CriAuCueSheet::StartLoadingCueSheetBinaryFile()
	 */
	virtual LoadStatus GetLoadStatus(CriError &err = criErr::ErrorContainer) = 0;
	/*JP
	 * \brief メモリからのキューシートバイナリファイルロード
	 * \param data キューシートバイナリファイルのロードされている領域へのポインタ<br>
	 * \param data_size キューシートバイナリファイルのサイズ<br>
	 * \param err エラーコード<br>
	 * \par 説明:
	 * メモリ上のキューシートバイナリファイルをロードします。<br>
	 * 与えられたデータ領域と同じサイズの領域を内部で確保し、内容をコピーします。
	 * \code
	 * CriError err;
	 * CriUint32 csbsize;
	 * CriUint8* csbdata=malloc(csbsize);
	 * load_file("file_name.csb", csbdata , csbsize);
	 * CriAuCueSheet* cue_sheet = CriAuCueSheet::Create(heap, err);
	 * load_file("file_name.csb", csbdata , csbsize);
	 * // Load Cue Sheet Binary Data to Cue Sheet Object from Memory location
	 * cue_sheet->LoadCueSheetBinaryFileFromMemory(csbdata, csbsize, err);
	 * free(csbdata);
	 * \endcode
	 * \sa CriAuCueSheet::UnloadCueSheetBinaryFile()
	 */
	/*EN
	 * \brief Load a cue sheet binary from a specified memory location
	 * \param data A pointer for the memory location where the cue sheet binary file resides
	 * \param data_size Total data size of the cue sheet binary file
	 * \param err CriError information
	 * \par Description:
	 * This function loads cue sheet binary data which residing in memory. 
	 * The API assumes that whole data in a file is contiguous in the memory.
	 * The memory of the same size as the specified data area is allocated in the API, 
	 * and the API copies the contents of data area to the allocated memory.
	 * \code
	 * CriError err;
	 * CriUint32 csbsize;
	 * CriUint8* csbdata=malloc(csbsize);
	 * load_file("file_name.csb", csbdata , csbsize);
	 * CriAuCueSheet* cue_sheet = CriAuCueSheet::Create(heap, err);
	 * load_file("file_name.csb", csbdata , csbsize);
	 * // Load Cue Sheet Binary Data to Cue Sheet Object from Memory location
	 * cue_sheet->LoadCueSheetBinaryFileFromMemory(csbdata, csbsize, err);
	 * free(csbdata);
	 * \endcode
	 * \sa CriAuCueSheet::UnloadCueSheetBinaryFile()
	 */
	virtual void LoadCueSheetBinaryFileFromMemory(const CriUint8* data, CriUint32 data_size, CriError &err = criErr::ErrorContainer) = 0;
	/*JP
	 * \brief メモリからのキューシートバイナリファイルロード
	 * \param cue_sheet_name キューシート名 (デバッグ用、ライブラリ内部では使用しません)<br>
	 * \param data キューシートバイナリファイルのロードされている領域へのポインタ<br>
	 * \param data_size キューシートバイナリファイルのサイズ<br>
	 * \param err エラーコード<br>
	 * \par 説明:
	 * メモリ上のキューシートバイナリファイルをロードします。<br>
	 * 与えられたデータ領域と同じサイズの領域を内部で確保し、内容をコピーします。<br>
	 * また、デバッグ用にキューシート名を保存します。
	 * \code
	 * CriError err;
	 * CriUint32 csbsize;
	 * CriUint8* csbdata=malloc(csbsize);
	 * load_file("file_name.csb", csbdata , csbsize);
	 * CriChar8* name = "file_name.csb";
	 * CriAuCueSheet* cue_sheet = CriAuCueSheet::Create(heap, err);
	 * // Load Cue Sheet Binary Data to Cue Sheet Object from Memory location
	 * cue_sheet->LoadCueSheetBinaryFileFromMemory(name, csbdata, csbsize, err);
	 * free(csbdata);
	 * \endcode
	 * \sa CriAuCueSheet::UnloadCueSheetBinaryFile()
	 */
	/*EN
	 * \brief Load cue sheet binary from a specified memory location
	 * \param cue_sheet_name A cue sheet name (This name is for debugging purposes, not reffered in the library.)
	 * \param data A pointer for the memory location where the cue sheet data resides
	 * \param data_size Total data size of the cue sheet binary file
	 * \param err CriError information
	 * \par Description:
	 * This function loads cue sheet binary data which exists in a specified memory location.
	 * The API assumes that whole data in a file is contiguous in the memory.
	 * The memory of the same size as the specified data area is allocated in the API.
	 * The API copies the contents of data area to the allocated memory, 
	 * and holds a name of the cue sheet for debugging.
	 * \code
	 * CriError err;
	 * CriUint32 csbsize;
	 * CriUint8* csbdata=malloc(csbsize);
	 * load_file("file_name.csb", csbdata , csbsize);
	 * CriChar8* name = "file_name.csb";
	 * CriAuCueSheet* cue_sheet = CriAuCueSheet::Create(heap, err);
	 * // Load Cue Sheet Binary Data to Cue Sheet Object from Memory location
	 * cue_sheet->LoadCueSheetBinaryFileFromMemory(name, csbdata, csbsize, err);
	 * free(csbdata);
	 * \endcode
	 * \sa CriAuCueSheet::UnloadCueSheetBinaryFile()
	 */
	virtual void LoadCueSheetBinaryFileFromMemory(const CriChar8* cue_sheet_name, const CriUint8* data, CriUint32 data_size, CriError &err = criErr::ErrorContainer) = 0;
	/*JP
	 * \brief キューシートバイナリファイルのアンロード
	 * \param err エラーコード<br>
	 * \par 説明:
	 * キューシートバイナリファイルをアンロードします。
	 * \code
	 * CriError err;
	 * CriAuCueSheet* cue_sheet = CriAuCueSheet::Create(heap, err);
	 * cue_sheet->LoadCueSheetBinaryFileFromMemory(csbdata, csbsize, err);
	 * // Unload Cue Sheet Binary Data from Cue Sheet Object
	 * cue_sheet->UnloadCueSheetBinaryFile(err);
	 * \endcode
	 */
	/*EN
	 * \brief Unload the loaded cue sheet binary file
	 * \param err CriError information
	 * \par Description:
	 * This function unloads the cue sheet binary file.
	 * \code
	 * CriError err;
	 * CriAuCueSheet* cue_sheet = CriAuCueSheet::Create(heap, err);
	 * cue_sheet->LoadCueSheetBinaryFileFromMemory(csbdata, csbsize, err);
	 * // Unload Cue Sheet Binary Data from Cue Sheet Object
	 * cue_sheet->UnloadCueSheetBinaryFile(err);
	 * \endcode
	 */
	virtual void UnloadCueSheetBinaryFile(CriError &err = criErr::ErrorContainer) = 0;

	/*JP
	 * \brief キューシートバイナリファイルのセット
	 * \param data キューシートバイナリファイルのロードされている領域へのポインタ<br>
	 * \param data_size キューシートバイナリファイルのサイズ<br>
	 * \param err エラーコード<br>
	 * \par 説明:
	 * メモリ上のキューシートバイナリファイルをセットします。<br>
	 * 与えられたデータ領域はコピーされず、ライブラリ内から直接参照されます。<br>
	 * データ領域は、UnsetCueSheetBinaryFile関数を呼ぶまで解放しないでください。<br>
	 * データ領域を直接参照する都合上、機種によっては、データ領域の確保場所について<br>
	 * 制限のある場合があります。<br>
	 * 機種固有の制限については、機種別マニュアルをご覧ください。
	 * \code
	 * CriError err;
	 * CriAuCueSheet* cue_sheet = CriAuCueSheet::Create(heap, err);
	 * CriUint32 csbsize;
	 * CriUint8* csbdata=malloc(csbsize);
	 * load_file("file_name.csb", csbdata , csbsize);
	 * CriChar8* name = "file_name.csb";
	 * // Set Cue Sheet Binary Data to Cue Sheet Object
	 * cue_sheet->SetCueSheetBinaryFile(name, csbdata, csbsize, err);
	 * \endcode
	 */
	/*EN
	 * \brief Set a cue sheet binary
	 * \param cue_sheet_name A cue sheet name (This name is for debugging, and is not used in the library.)
	 * \param data A pointer for the memory location where the cue sheet binary file exists
	 * \param data_size Total data size of the cue sheet binary file
	 * \param err CriError information
	 * \par Description:
	 * This function sets cue sheet binary data which resides in a speficied memory location.
	 * The API assumes that whole data in a file is contiguous in the memory.
	 * The pecified data area is not copied to the internal memory, 
	 * so the data area should not be freed until UnsetCueSheetBinary is called.
	 * \code
	 * CriError err;
	 * CriAuCueSheet* cue_sheet = CriAuCueSheet::Create(heap, err);
	 * CriUint32 csbsize;
	 * CriUint8* csbdata=malloc(csbsize);
	 * load_file("file_name.csb", csbdata , csbsize);
	 * CriChar8* name = "file_name.csb";
	 * // Set Cue Sheet Binary Data to Cue Sheet Object
	 * cue_sheet->SetCueSheetBinaryFile(name, csbdata, csbsize, err);
	 * \endcode
	 */
	virtual void SetCueSheetBinaryFile(const CriUint8* data, CriUint32 data_size, CriError &err = criErr::ErrorContainer) = 0;

	/*JP
	 * \brief キューシートバイナリファイルのセット
	 * \param cue_sheet_name キューシート名 (デバッグ用、ライブラリ内部では使用しません)<br>
	 * \param data キューシートバイナリファイルのロードされている領域へのポインタ<br>
	 * \param data_size キューシートバイナリファイルのサイズ<br>
	 * \param err エラーコード<br>
	 * \par 説明:
	 * メモリ上のキューシートバイナリファイルをセットします。<br>
	 * 与えられたデータ領域はコピーされず、ライブラリ内から直接参照されます。<br>
	 * データ領域は、UnsetCueSheetBinaryFile関数を呼ぶまで解放しないでください。<br>
	 * データ領域を直接参照する都合上、機種によっては、データ領域の確保場所について<br>
	 * 制限のある場合があります。<br>
	 * 機種固有の制限については、機種別マニュアルをご覧ください。
	 * \code
	 * CriError err;
	 * CriAuCueSheet* cue_sheet = CriAuCueSheet::Create(heap, err);
	 * CriUint32 csbsize;
	 * CriUint8* csbdata=malloc(csbsize);
	 * load_file("file_name.csb", csbdata , csbsize);
	 * // Set Cue Sheet Binary Data to Cue Sheet Object
	 * cue_sheet->SetCueSheetBinaryFile(csbdata, csbsize, err);
	 * \endcode
	 */
	/*EN
	 * \brief Set a cue sheet binary
	 * \param data A pointer for the memory location where the cue sheet binary file resides
	 * \param data_size Total data size of the cue sheet binary file
	 * \param err CriError information
	 * \par Description:
	 * This function sets cue sheet binary data which resides in a speficied memory location.
	 * The API assumes that whole data in a file is contiguous in the memory.
	 * The specified data area is not copied to the internal memory, 
	 * so the data area should not be freed until UnsetCueSheetBinary is called.
	 * \code
	 * CriError err;
	 * CriAuCueSheet* cue_sheet = CriAuCueSheet::Create(heap, err);
	 * CriUint32 csbsize;
	 * CriUint8* csbdata=malloc(csbsize);
	 * load_file("file_name.csb", csbdata , csbsize);
	 * // Set Cue Sheet Binary Data to Cue Sheet Object
	 * cue_sheet->SetCueSheetBinaryFile(csbdata, csbsize, err);
	 * \endcode
	 */
	virtual void SetCueSheetBinaryFile(const CriChar8* cue_sheet_name, const CriUint8* data, CriUint32 data_size, CriError &err = criErr::ErrorContainer) = 0;

	/*JP
	 * \brief キューシートバイナリファイルのアンセット
	 * \param err エラーコード<br>
	 * \par 説明:
	 * キューシートバイナリファイルをアンセットします。
	 * \code
	 * CriError err;
	 * CriAuCueSheet* cue_sheet = CriAuCueSheet::Create(heap, err);
	 * CriUint32 csbsize;
	 * CriUint8* csbdata=malloc(csbsize);
	 * load_file("file_name.csb", csbdata , csbsize);
	 * cue_sheet->SetCueSheetBinaryFile(name, csbdata, csbsize, err);
	 * // Unset Cue Sheet Binary Data of Cue Sheet Object
	 * cue_sheet->UnsetCueSheetBinaryFile(err);
	 * free(csbdata);
	 * \endcode
	 */
	/*EN
	 * \brief Unset the cue sheet binary file
	 * \param err CriError information
	 * \par Description:
	 * This function unsets the cue sheet binary file.
	 * \code
	 * CriError err;
	 * CriAuCueSheet* cue_sheet = CriAuCueSheet::Create(heap, err);
	 * CriUint32 csbsize;
	 * CriUint8* csbdata=malloc(csbsize);
	 * load_file("file_name.csb", csbdata , csbsize);
	 * cue_sheet->SetCueSheetBinaryFile(name, csbdata, csbsize, err);
	 * // Unset Cue Sheet Binary Data of Cue Sheet Object
	 * cue_sheet->UnsetCueSheetBinaryFile(err);
	 * free(csbdata);
	 * \endcode
	 */
	virtual void UnsetCueSheetBinaryFile(CriError &err = criErr::ErrorContainer) = 0;

	/*JP
	 * \brief ストリーミングファイルシステムマネージャの設定
	 * \param fsmng ファイルシステムマネージャ<br>
	 * \param path ルートフォルダのパス
	 * \param err エラーコード<br>
	 * \return なし
	 * \par 説明:
	 * ストリーミングによって再生するファイルを格納したファイルシステムマネージャを設定します。
	 * fsmngはCPKファイルとしてパッキングされたファイルからストリーミングするときに使用します。<br>
	 * pathを指定すると、ファイルをアクセスするときに、この文字列が先頭に付加されます。<br>
	 * CriFsManager（ファイルシステムマネージャ）の詳細については、CRI File Systemのマニュアルをご覧ください。
	 *
	 * \code
	 * CriError err;
	 * CriFsManagerCpk *fsmng;
	 * 
	 * fsmng = CriFsManagerCpk::Create(heap, err);
	 * fsmng->MountCpkFile("streaming_files.cpk", err);
	 * for (;;) {
	 *     if ( fsmng->IsMounted(err) == TRUE )
	 *         break;
	 * }
	 * SetStreamingFileSystemManager(fsmng, "/streaming_data/", err);
	 * \endcode
	 */
	/*EN
	 * \brief Set the Streaming File System Manager
	 * \param fsmng File System Manager<br>
	 * \param path Root folder path<br>
	 * \param err CriError information<br>
	 * \par Description:
	 * This function sets the File System Manager that contains streaming files.<br>
	 * A .cpk file (a packed file format) includes streaming files, and
	 * a file system manager object which can mount the .cpk file needs to be used.
	 * Please refer the CRI File System Manual for detail about CriFsManager(File System Manager).
 	 *
	 * \code
	 * CriError err;
	 * CriFsManagerCpk *fsmng;
	 * 
	 * fsmng = CriFsManagerCpk::Create(heap, err);
	 * fsmng->MountCpkFile("streaming_files.cpk", err);
	 * for (;;) {
	 *     if ( fsmng->IsMounted(err) == TRUE )
	 *         break;
	 * }
	 * SetStreamingFileSystemManager(fsmng, "/streaming_data/", err);
	 * \endcode
	 */
	virtual void SetStreamingFileSystemManager(CriFsManager *fsmng, const CriChar8 *path, CriError &err = criErr::ErrorContainer) = 0;
	/*JP
	 * \brief ストリーミングファイルシステムマネージャの取得
	 * \param path ルートフォルダのパスを代入するポインタ
	 * \param err エラーコード<br>
	 * \par 説明:
	 * ストリーミングに使用するファイルシステムマネージャを取得します。<br>
	 * pathには、ストリーミングファイルを格納してあるフォルダ名へのポインタが代入されます。<br>
	 *
	 * \code
	 * CriError err;
	 * const CriChar8 *path;
	 * CriFsManager *fsmnt = GetStreamingFileRootPath(&path, err);
	 * \endcode
	 *
	 */
	/*EN
	 * \brief Retrieve the Streaming File System Manager
	 * \param path Pointer to retrieve root folder path<br>
	 * \param err CriError information<br>
	 * \return Pointer to a File System Manager<br>
	 * \par Description:
	 * This function returns a pointer to a File System Manager which has streaming files.<br>
	 * A root folder path is assigned to <b>path</b>.
	 *
	 * \code
	 * CriError err;
	 * const CriChar8 *path;
	 * CriFsManager *fsmnt = GetStreamingFileRootPath(&path, err);
	 * \endcode
	 */
	virtual CriFsManager *GetStreamingFileSystemManager(const CriChar8 **path, CriError &err = criErr::ErrorContainer) = 0;
#if (CRI_FS_VERSION >= 0x02000000)
	/*JP
	 * \brief ストリーミングに使用するバインダーの設定
	 * \param binder バインダーハンドル<br>
	 * \param path ルートフォルダのパス
	 * \param err エラーコード<br>
	 * \return なし
	 * \par 説明:
	 * ストリーミングによって再生するファイルを格納したバインダーを設定します。<br>
	 * binderはCPKファイルとしてパッキングされたファイルからストリーミングするときに使用します。<br>
	 * パッキングされていないファイルをストリーミング再生する場合には、binderにNULLが指定可能です。<br>
	 * pathを指定すると、ファイルをアクセスするときに、この文字列が先頭に付加されます。pathを設定しない場合はNULLを指定してください。<br>
	 * CriFsBinder（バインダー）の詳細については、CRI File Systemのマニュアルをご覧ください。
	 *
	 * \code
	 * // Bind CPK file
	 * CriFsBinderHn		binder;
	 * CriFsBinderId		binder_id;
	 * CriFsBinderStatus	binder_status;
	 * CriSint32			bndr_wksize;
	 * void*				bndr_work;
	 * // Create CriFsBinder handle
	 * criFsBinder_Create(&binder);
	 * // Allocate work area for CPK binding
	 * criFsBinder_GetWorkSizeForBindCpk(NULL, DATA_DIR CPK_FILE_NAME, &bndr_wksize);
	 * bndr_work = criHeap_AllocFix(heap, bndr_wksize, "bndr_work", CRIHEAP_DEFAULT_MEM_ALIGN);
	 * // Bind CPK file
	 * criFsBinder_BindCpk(binder, NULL, DATA_DIR CPK_FILE_NAME, bndr_work, bndr_wksize, &binder_id);
	 *
	 * for (;;) {
	 * 	// Check the binder status
	 * 	criFsBinder_GetStatus(binder_id, &binder_status);
	 * 	if (binder_status == CRIFSBINDER_STATUS_COMPLETE) {
	 * 		break;
	 * 	}
	 * 	criFs_ExecuteMain();
	 * }
	 * //	Set File System Manager and the folder of streaming files
	 * cue_sheet->SetStreamingBinder(binder, NULL, err);
	 * \endcode
	 */
	virtual void SetStreamingBinder(CriFsBinderHn binder, const CriChar8* path, CriError &err = criErr::ErrorContainer) = 0;
	/*JP
	 * \brief ストリーミングに使用するバインダーの取得
	 * \param path ルートフォルダのパスを代入するポインタ
	 * \param err エラーコード<br>
	 * \return バインダーハンドル
	 * \par 説明:
	 * ストリーミングに使用するバインダーハンドルを取得します。<br>
	 * pathには、ストリーミングファイルを格納してあるフォルダ名へのポインタが代入されます。<br>
	 *
	 * \code
	 * CriError err;
	 * const CriChar8 *path;
	 * CriFsBinderHn binder = GetStreamingBinder(&path, err);
	 * \endcode
	 *
	 */
	virtual CriFsBinderHn GetStreamingBinder(const CriChar8 **path, CriError &err = criErr::ErrorContainer) = 0;
#endif

	/***
	*		Functions for tools
	***/
	/*JP
	 * \brief キューの個数の取得
	 * \param err エラーコード
	 * \par 説明:
	 * キューシート内のキューの個数を取得します。
	 * \code
	 * CriError err;
	 * CriUint32 num = cue_sheet->GetNumberOfCues(err);
	 * \endcode
	 */
	/*EN
	 * \brief Retrieve the number of cues
	 * \param err CriError information
	 * \par Description:
	 * This function returns the number of cues in the sheet.
	 * \code
	 * CriError err;
	 * CriUint32 num = cue_sheet->GetNumberOfCues(err);
	 * \endcode
	 */
	virtual CriUint32 GetNumberOfCues(CriError &err = criErr::ErrorContainer) = 0;
	/*JP
	 * \brief キュー名の取得
	 * \param no キューシート内のインデックス番号<br>
	 * \param err エラーコード<br>
	 * \par 説明:
	 * キューシート内のキュー名を取得します。
	 * \code
	 * CriError err;
	 * CriUint32 no = 5;
	 * const CriChar8* name = cue_sheet->GetCueName(no, err);
	 * \endcode
	 */
	/*EN
	 * \brief Retrieve a cue name
	 * \param no Index number in the cue sheet
	 * \param err CriError information
	 * \par Description:
	 * This function returns the name of the nth cue in the cue sheet.
	 * \code
	 * CriError err;
	 * CriUint32 no = 5;
	 * const CriChar8* name = cue_sheet->GetCueName(no, err);
	 * \endcode
	 */
	virtual const CriChar8 *GetCueName(CriUint32 no, CriError &err = criErr::ErrorContainer) = 0;
	/*JP
	 * \brief キューIDの取得
	 * \param no キューシート内のインデックス番号<br>
	 * \param err エラーコード<br>
	 * \par 説明:
	 * キューシート内のキューIDを取得します。
	 * \code
	 * CriError err;
	 * CriUint32 no = 5;
	 * CriUint32 cueid = cue_sheet->GetCueId(no, err);
	 * \endcode
	 */
	/*EN
	 * \brief Retrieve a cue ID
	 * \param no Index number in the cue sheet
	 * \param err CriError information
	 * \par Description:
	 * This function returns the ID of the nth cue in the cue sheet.
	 * \code
	 * CriError err;
	 * CriUint32 no = 5;
	 * CriUint32 cueid = cue_sheet->GetCueId(no, err);
	 * \endcode
	 */
	virtual CriUint32 GetCueId(CriUint32 no, CriError &err = criErr::ErrorContainer) = 0;
	/*JP
	 * \brief アイザックの個数の取得
	 * \param err エラーコード
	 * \par 説明:
	 * キューシートハンドル内のアイザックの個数を取得します。
	 * \code
	 * CriError err;
	 * CriUint32 num_aisacs = cue_sheet->GetNumberOfAisacs(err);
	 * \endcode
	 */
	/*EN
	 * \brief Retrieve the number of AISACs in a cue sheet handle
	 * \param err CriError information
	 * \par Description:
	 * This function returns the number of AISACs in a cue sheet handle.
	 * \code
	 * CriError err;
	 * CriUint32 num_aisacs = cue_sheet->GetNumberOfAisacs(err);
	 * \endcode
	 */
	virtual CriUint32 GetNumberOfAisacs(CriError &err = criErr::ErrorContainer) = 0;
	/*JP
	 * \brief アイザックのコントロール名の取得
	 * \param no キューシート内のアイザックのインデックス番号<br>
	 * \param err エラーコード<br>
	 * \par 説明:
	 * キューシート内のアイザックのコントロール名を取得します。
	 * \code
	 * CriError err;
	 * CriUint32 no = 2;
	 * const CriChar8* control_name = cue_sheet->GetAisacControlName(no, err);
	 * \endcode
	 */
	/*EN
	 * \brief Retrieve an AISAC control name in a cue sheet
	 * \param no Index number of the AISAC in a cue sheet
	 * \param err CriError information
	 * \par Description:
	 * This function returns the AISAC control name in a cue sheet.
	 * \code
	 * CriError err;
	 * CriUint32 no = 2;
	 * const CriChar8* control_name = cue_sheet->GetAisacControlName(no, err);
	 * \endcode
	 */
	virtual const CriChar8 *GetAisacControlName(CriUint32 no, CriError &err = criErr::ErrorContainer) = 0;
	/*JP
	 * \brief アイザックのパターン名の取得
	 * \param no キューシート内のアイザックのインデックス番号<br>
	 * \param err エラーコード<br>
	 * \par 説明:
	 * キューシート内のアイザックのパターン名を取得します。
	 * \code
	 * CriError err;
	 * CriUint32 no = 2;
	 * const CriChar8* pattern_name = cue_sheet->GetAisacPatternName(no, err);
	 * \endcode
	 */
	/*EN
	 * \brief Retrieve an AISAC pattern name in a cue sheet
	 * \param no Index number of the AISAC in a cue sheet
	 * \param err CriError information
	 * \par Description:
	 * This function returns the AISAC pattern name in a cue sheet.
	 * \code
	 * CriError err;
	 * CriUint32 no = 2;
	 * const CriChar8* pattern_name = cue_sheet->GetAisacPatternName(no, err);
	 * \endcode
	 */
	virtual const CriChar8 *GetAisacPatternName(CriUint32 no, CriError &err = criErr::ErrorContainer) = 0;
	/*JP
	 * \brief アイザックの入力値の範囲の取得
	 * \param no キューシート内のアイザックのインデックス番号<br>
	 * \param max アイザックの最大入力値<br>
	 * \param min アイザックの最小入力値<br>
	 * \param err エラーコード<br>
	 * \par 説明:
	 * キューシート内のアイザックの入力値の範囲を取得します。
	 * \code
	 * CriError err;
	 * CriUint32 no = 2;
	 * CriFloat32 max, min;
	 * cue_sheet->GetAisacControlName(no, &max, &min, err);
	 * \endcode
	 */
	/*EN
	 * \brief Retrieve AISAC input range
	 * \param no Index number of the AISAC in a cue sheet
	 * \param max Maximum value for the AISAC input
	 * \param min Minimum value for the AISAC input
	 * \param err CriError information
	 * \par Description:
	 * This function returns the range of an input variable for the AISAC.
	 * \code
	 * CriError err;
	 * CriUint32 no = 2;
	 * CriFloat32 max, min;
	 * cue_sheet->GetAisacControlName(no, &max, &min, err);
	 * \endcode
	 */
	virtual void GetAisacInputRange(CriUint32 no, CriFloat32 *max, CriFloat32 *min, CriError &err = criErr::ErrorContainer) = 0;
	/*JP
	 * \brief ユーザーデータの取得
	 * \param cue_name キュー名<br>
	 * \param err エラーコード<br>
	 * \return ユーザーデータ(文字列)
	 * \par 説明:
	 * キュー名からキューシート内のユーザーデータ(文字列)を取得します。
	 * \code
	 * CriError err;
	 * const CriChar8* cue_name = "GUN":
	 * const CriChar8* userdata = cue_sheet->GetUserData(cue_name, err);
	 * \endcode
	 */
	/*EN
	 * \brief Retrieve a user's data
	 * \param cue_name The name of the cue
	 * \param err CriError information
	 * \par Description:
	 * This function returns the user's data from a cue_name as text.
	 * \code
	 * CriError err;
	 * const CriChar8* cue_name = "GUN":
	 * const CriChar8* userdata = cue_sheet->GetUserData(cue_name, err);
	 * \endcode
	 */
	virtual const CriChar8* GetUserData(const CriChar8 *cue_name, CriError &err = criErr::ErrorContainer) = 0;
	/*JP
	 * \brief ユーザーデータの取得
	 * \param cue_id キューID<br>
	 * \param err エラーコード<br>
	 * \return ユーザーデータ(文字列)
	 * \par 説明:
	 * キューIDからキューシート内のユーザーデータ(文字列)を取得します。
	 * \code
	 * CriError err;
	 * CriUint32 cue_id = 3:
	 * const CriChar8* userdata = cue_sheet->GetUserData(cum_id, err);
	 * \endcode
	 */
	/*EN
	 * \brief Retrieve a user's data
	 * \param cue_id The id of the cue
	 * \param err CriError information
	 * \par Description:
	 * This function returns the user's data from a cue_id as text.
	 * \code
	 * CriError err;
	 * CriUint32 cue_id = 3:
	 * const CriChar8* userdata = cue_sheet->GetUserData(cum_id, err);
	 * \endcode
	 */
	virtual const CriChar8* GetUserDataById(CriUint32 cue_id, CriError &err = criErr::ErrorContainer) = 0;
	/*JP
	 * \brief キューIDの取得
	 * \param cue_name キュー名<br>
	 * \param err エラーコード<br>
	 * \return キューID
	 * \par 説明:
	 * キュー名からキューシート内のキューIDを取得します。
	 * \code
	 * CriError err;
	 * const CriChar8* cue_name = "GUN":
	 * CriUint32 cue_id = cue_sheet->GetCueId(cue_name, err);
	 * \endcode
	 */
	/*EN
	 * \brief Retrieve a Cue ID
	 * \param cue_name The name of the cue
	 * \param err CriError information
	 * \par Description:
	 * This function returns the Cue ID from a cue_name.
	 * \code
	 * CriError err;
	 * const CriChar8* cue_name = "GUN":
	 * CriUint32 cue_id = cue_sheet->GetCueId(cue_name, err);
	 * \endcode
	 */
	virtual CriUint32 GetCueId(const CriChar8* cue_name, CriError &err = criErr::ErrorContainer) = 0;
	/*JP
	 * \brief キュー名の取得
	 * \param cue_id キューID<br>
	 * \param err エラーコード<br>
	 * \return キュー名
	 * \par 説明:
	 * キューIDからキューシート内のキュー名を取得します。
	 * \code
	 * CriError err;
	 * CriUint32 cue_id = 3:
	 * const CriChar8* cue_name = cue_sheet->GetCueNameById(cue_id, err);
	 * \endcode
	 */
	/*EN
	 * \brief Retrieve a Cue Name
	 * \param cue_id The id of the cue
	 * \param err CriError information
	 * \par Description:
	 * This function returns the Cue Name from a cue_id.
	 * \code
	 * CriError err;
	 * CriUint32 cue_id = 3:
	 * const CriChar8* cue_name = cue_sheet->GetCueNameById(cue_id, err);
	 * \endcode
	 */
	virtual const CriChar8* GetCueNameById(CriUint32 cue_id, CriError &err = criErr::ErrorContainer) = 0;
	/*JP
	 * \brief アイザックコントロール名の取得
	 * \param cue_index キューシート内のインデックス<br>
	 * \param ctrl_name コントロール名の格納領域<br>
	 * \param ctrl_name_size コントロール名の格納領域の個数<br>
	 * \param err エラーコード<br>
	 * \return アイザックコントロールの数
	 * \par 説明:
	 * キューから利用できるコントロール名を取得します。<br>
	 * 返り値にコントロールの数を返します。
	 *
	 * \code
	 * #define NCTRL_NAME	(16)
	 * CriChar8 control_name_table[NCTRL_NAME][CriAu::MAX_AISAC_CONTROL_NAME];
	 * CriUint32 index=10;
	 * naisac = GetCueAisacControlByIndex(index, control_name_table, NCTRL_NAME, err);
	 * \endcode
	 *
	 */
	/*EN
	 * \brief Retrieve a Control Name List linked to the Cue
	 * \param cue_index Index Number in Cue Sheet<br>
	 * \param ctrl_name Name area table<br>
	 * \param ctrl_name_size Number of name areas<br>
	 * \param err CriError information<br>
	 * \return Number of AISAC control
	 * \par Description:
	 * This function returns the AISAC control list linked to the cue.<br>
	 *
	 * \code
	 * #define NCTRL_NAME	(16)
	 * CriChar8 control_name_table[NCTRL_NAME][CriAu::MAX_AISAC_CONTROL_NAME];
	 * CriUint32 index=10;
	 * naisac = GetCueAisacControlByIndex(index, control_name_table, NCTRL_NAME, err);
	 * \endcode
	 */
	virtual CriUint32 GetCueAisacControlByIndex(CriUint32 cue_index,
			CriChar8 (*ctrl_name)[CriAu::MAX_AISAC_CONTROL_NAME], CriUint32 ctrl_name_size, CriError &err = criErr::ErrorContainer) = 0;

	/*JP
	 * \brief ループフラグの取得
	 * \param cue_name キュー名<br>
	 * \param err エラーコード<br>
	 * \return ループフラグ
	 * \par 説明:
	 * 指定したキュー名のキューがループするかどうかを返します。<br>
	 * \code
	 * CriError err;
	 * const CriChar8* cue_name = "GUN":
	 * CriBool is_loop = cue_sheet->GetLoopFlag(cue_name, err);
	 * \endcode
	 */
	/*EN
	 * \brief Retrieve a loop flag
	 * \param cue_name The name of the cue
	 * \param err CriError information
	 * \return loop flag
	 * \par Description:
	 * This function returns whether the cue specified by cue_name will loop or not.
	 * \code
	 * CriError err;
	 * const CriChar8* cue_name = "GUN":
	 * CriBool is_loop = cue_sheet->GetLoopFlag(cue_name, err);
	 * \endcode
	 */
	virtual CriBool GetLoopFlag(const CriChar8 *cue_name, CriError &err = criErr::ErrorContainer) = 0;

	/*JP
	 * \brief ループフラグの取得
	 * \param cue_id キューID<br>
	 * \param err エラーコード<br>
	 * \return ループフラグ
	 * \par 説明:
	 * 指定したキューIDのキューがループするかどうかを返します。<br>
	 * \code
	 * CriError err;
	 * CriUint32 cue_id = 3:
	 * CriBool is_loop = cue_sheet->GetLoopFlagById(cue_id, err);
	 * \endcode
	 */
	/*EN
	 * \brief Retrieve a loop flag
	 * \param cue_id The id of the cue
	 * \param err CriError information
	 * \return loop flag
	 * \par Description:
	 * This function returns whether the cue specified by cue_id will loop or not.
	 * \code
	 * CriError err;
	 * CriUint32 cue_id = 3:
	 * CriBool is_loop = cue_sheet->GetLoopFlagById(cue_id, err);
	 * \endcode
	 */
	virtual CriBool GetLoopFlagById(CriUint32 cue_id, CriError &err = criErr::ErrorContainer) = 0;


	/*JP
	 * \brief 最大消費ボイス数の取得
	 * \param cue_name キュー名<br>
	 * \param err エラーコード<br>
	 * \return 最大消費ボイス数
	 * \par 説明:
	 * 指定したキューを再生した場合に消費されるボイスの最大数を取得します。<br>
	 * \code
	 * CriError err;
	 * const CriChar8* cue_name = "GUN":
	 * CriUint32 max_num_voices = cue_sheet->GetMaxNumberOfVoices(cue_name, err);
	 * \endcode
	 */
	/*EN
	 * \brief Retrieve the max number of voices used
	 * \param cue_name A cue name
	 * \param err CriError information
	 * \return Max number of voices used
	 * \par Description:
	 * This function returns the max number of voices used when the cue specified by cue_name is played.
	 * \code
	 * CriError err;
	 * const CriChar8* cue_name = "GUN":
	 * CriUint32 max_num_voices = cue_sheet->GetMaxNumberOfVoices(cue_name, err);
	 * \endcode
	 */
	virtual CriUint32 GetMaxNumberOfVoices(const CriChar8 *cue_name, CriError &err) = 0;

	/*JP
	 * \brief シンセ名の取得
	 * \param cue_name キュー名<br>
	 * \param err エラーコード<br>
	 * \return シンセ名
	 * \par 説明:
	 * 指定したキュー名のキューに関連付けられたシンセの名前を取得します。<br>
	 * \code
	 * CriError err;
	 * const CriChar8* cue_name = "GUN":
	 * const CriChar8* synth_name = cue_sheet->GetSynthName(cue_name, err);
	 * \endcode
	 */
	/*EN
	 * \brief Retrieve a synthesizer name
	 * \param cue_name A cue name
	 * \param err CriError information
	 * \par Description:
	 * This function returns a synthesizer name of the cue specified by cue_name.
	 * \code
	 * CriError err;
	 * const CriChar8* cue_name = "GUN":
	 * const CriChar8* synth_name = cue_sheet->GetSynthName(cue_name, err);
	 * \endcode
	 */
	virtual const CriChar8* GetSynthName(const CriChar8* cue_name, CriError &err = criErr::ErrorContainer) = 0;
	/*JP
	 * \brief シンセインデックスの取得
	 * \param cue_name キュー名<br>
	 * \param err エラーコード<br>
	 * \return シンセのインデックス番号
	 * \par 説明:
	 * 指定したキュー名のキューに関連付けられたシンセのインデックス番号を取得します。<br>
	 * \code
	 * CriError err;
	 * const CriChar8* cue_name = "GUN":
	 * CriUint32 synth_index = cue_sheet->GetSynthIndex(cue_name, err);
	 * \endcode
	 */
	/*EN
	 * \brief Retrieve a synthesizer index
	 * \param cue_name A cue name
	 * \param err CriError information
	 * \par Description:
	 * This function returns a index number of a synthesizer of the cue specified by cue_name.
	 * \code
	 * CriError err;
	 * const CriChar8* cue_name = "GUN":
	 * CriUint32 synth_index = cue_sheet->GetSynthIndex(cue_name, err);
	 * \endcode
	 */
	virtual CriUint32 GetSynthIndex(const CriChar8* cue_name, CriError &err = criErr::ErrorContainer) = 0;
	/*JP
	 * \brief シンセインデックスの取得
	 * \param synth_name シンセ名<br>
	 * \param err エラーコード<br>
	 * \return シンセのインデックス番号
	 * \par 説明:
	 * 指定したシンセ名のシンセのインデックス番号を取得します。<br>
	 * \code
	 * CriError err;
	 * const CriChar8* cue_name = "GUN":
	 * const CriChar8* synth_name = cue_sheet->GetSynthName(cue_name, err);
	 * CriUint32 synth_index = cue_sheet->GetSynthIndexFromSynthName(synth_name, err);
	 * \endcode
	 */
	/*EN
	 * \brief Retrieve a synthesizer index
	 * \param synth_name A synthesizer name
	 * \param err CriError information
	 * \par Description:
	 * This function returns a index number of the synthesizer specified by synth_name.
	 * \code
	 * CriError err;
	 * const CriChar8* cue_name = "GUN":
	 * const CriChar8* synth_name = cue_sheet->GetSynthName(cue_name, err);
	 * CriUint32 synth_index = cue_sheet->GetSynthIndexFromSynthName(synth_name, err);
	 * \endcode
	 */
	virtual CriUint32 GetSynthIndexFromSynthName(const CriChar8* synth_name, CriError &err = criErr::ErrorContainer) = 0;

	/*JP
	 * \brief シンセタイプの取得
	 * \param synth_index シンセインデックス<br>
	 * \param err エラーコード<br>
	 * \return シンセタイプ
	 * \par 説明:
	 * 指定したシンセのシンセタイプを取得します。<br>
	 * \code
	 * CriError err;
	 * const CriChar8* cue_name = "GUN":
	 * CriUint32 synth_index = cue_sheet->GetSynthIndex(cue_name, err);
	 * CriAuCueSheet::SynthType synth_type = cue_sheet->GetSynthType(synth_index, err);
	 * \endcode
	 * \sa CriAuCueSheet::SynthType
	 * \sa CriAuCueSheet::GetSynthIndex
	 */
	/*EN
	 * \brief Retrieve a synthesizer type
	 * \param synth_index A synthesizer index
	 * \param err CriError information
	 * \par Description:
	 * This function returns a type of the synthesizer specified by synth_index.
	 * \code
	 * CriError err;
	 * const CriChar8* cue_name = "GUN":
	 * CriUint32 synth_index = cue_sheet->GetSynthIndex(cue_name, err);
	 * CriAuCueSheet::SynthType synth_type = cue_sheet->GetSynthType(synth_index, err);
	 * \endcode
	 * \sa CriAuCueSheet::SynthType
	 * \sa CriAuCueSheet::GetSynthIndex
	 */
	virtual CriAuCueSheet::SynthType GetSynthType(CriUint32 synth_index, CriError &err = criErr::ErrorContainer) = 0;
	/*JP
	 * \brief 子シンセ数の取得
	 * \param synth_index シンセインデックス<br>
	 * \param err エラーコード<br>
	 * \return 子シンセ数
	 * \par 説明:
	 * 指定したシンセに含まれる子シンセの数を取得します。<br>
	 * \code
	 * CriError err;
	 * const CriChar8* cue_name = "GUN":
	 * CriUint32 synth_index = cue_sheet->GetSynthIndex(cue_name, err);
	 * CriUint32 num_child_synth = cue_sheet->GetNumberOfChildSynth(synth_index, err);
	 * for (CriUint32 i=0; i<num_child_synth; i++) {
	 * 	const CriChar8* child_synth_name = cue_sheet->GetChildSynthName(synth_index, i, err);
	 * 	CriUint32 child_synth_index = cue_sheet->GetSynthIndexFromSynthName(child_synth_name, err);
	 * }
	 * \endcode
	 * \sa CriAuCueSheet::GetSynthIndex
	 */
	/*EN
	 * \brief Retrieve the number of child synthesizers
	 * \param synth_index A synthesizer index
	 * \param err CriError information
	 * \par Description:
	 * This function returns the number of child synthesizers of the synthesizer specified by synth_index.
	 * \code
	 * CriError err;
	 * const CriChar8* cue_name = "GUN":
	 * CriUint32 synth_index = cue_sheet->GetSynthIndex(cue_name, err);
	 * CriUint32 num_child_synth = cue_sheet->GetNumberOfChildSynth(synth_index, err);
	 * for (CriUint32 i=0; i<num_child_synth; i++) {
	 * 	const CriChar8* child_synth_name = cue_sheet->GetChildSynthName(synth_index, i, err);
	 * 	CriUint32 child_synth_index = cue_sheet->GetSynthIndexFromSynthName(child_synth_name, err);
	 * }
	 * \endcode
	 * \sa CriAuCueSheet::GetSynthIndex
	 */
	virtual CriUint32 GetNumberOfChildSynth(CriUint32 synth_index, CriError &err = criErr::ErrorContainer) = 0;
	/*JP
	 * \brief 子シンセ名の取得
	 * \param synth_index シンセインデックス<br>
	 * \param index 子シンセのインデックス<br>
	 * \param err エラーコード<br>
	 * \return 子シンセ名の取得
	 * \par 説明:
	 * 指定したシンセに含まれる子シンセの名前を取得します。<br>
	 * \code
	 * CriError err;
	 * const CriChar8* cue_name = "GUN":
	 * CriUint32 synth_index = cue_sheet->GetSynthIndex(cue_name, err);
	 * CriUint32 num_child_synth = cue_sheet->GetNumberOfChildSynth(synth_index, err);
	 * for (CriUint32 i=0; i<num_child_synth; i++) {
	 * 	const CriChar8* child_synth_name = cue_sheet->GetChildSynthName(synth_index, i, err);
	 * 	CriUint32 child_synth_index = cue_sheet->GetSynthIndexFromSynthName(child_synth_name, err);
	 * }
	 * \endcode
	 * \sa CriAuCueSheet::GetSynthIndex
	 */
	/*EN
	 * \brief Retrieve a name of the child synthesizer
	 * \param synth_index A synthesizer index
	 * \param index A index of child synthesizer
	 * \param err CriError information
	 * \par Description:
	 * This function returns a name of the child synthesizer of the synthesizer specified by synth_index.
	 * \code
	 * CriError err;
	 * const CriChar8* cue_name = "GUN":
	 * CriUint32 synth_index = cue_sheet->GetSynthIndex(cue_name, err);
	 * CriUint32 num_child_synth = cue_sheet->GetNumberOfChildSynth(synth_index, err);
	 * for (CriUint32 i=0; i<num_child_synth; i++) {
	 * 	const CriChar8* child_synth_name = cue_sheet->GetChildSynthName(synth_index, i, err);
	 * 	CriUint32 child_synth_index = cue_sheet->GetSynthIndexFromSynthName(child_synth_name, err);
	 * }
	 * \endcode
	 * \sa CriAuCueSheet::GetSynthIndex
	 */
	virtual const CriChar8* GetChildSynthName(CriUint32 synth_index, CriUint32 index, CriError &err = criErr::ErrorContainer) = 0;
	/*JP
	 * \brief シンセのボリュームの取得
	 * \param synth_index シンセインデックス<br>
	 * \param err エラーコード<br>
	 * \return シンセのボリューム
	 * \par 説明:
	 * 指定したシンセのボリュームを取得します。<br>
	 * \code
	 * CriError err;
	 * const CriChar8* cue_name = "GUN":
	 * CriUint32 synth_index = cue_sheet->GetSynthIndex(cue_name, err);
	 * CriFloat32 volume = cue_sheet->GetVolume(synth_index, err);
	 * \endcode
	 * \sa CriAuCueSheet::GetSynthIndex
	 */
	/*EN
	 * \brief Retrieve a volume of a synthesizer
	 * \param synth_index A synthesizer index
	 * \param err CriError information
	 * \par Description:
	 * This function returns a volume of the synthesizer specified by synth_index.
	 * \code
	 * CriError err;
	 * const CriChar8* cue_name = "GUN":
	 * CriUint32 synth_index = cue_sheet->GetSynthIndex(cue_name, err);
	 * CriFloat32 volume = cue_sheet->GetVolume(synth_index, err);
	 * \endcode
	 * \sa CriAuCueSheet::GetSynthIndex
	 */
	virtual CriFloat32 GetVolume(CriUint32 synth_index, CriError &err = criErr::ErrorContainer) = 0;
	/*JP
	 * \brief シンセのピッチの取得
	 * \param synth_index シンセインデックス<br>
	 * \param err エラーコード<br>
	 * \return シンセのピッチ
	 * \par 説明:
	 * 指定したシンセのピッチを取得します。<br>
	 * \code
	 * CriError err;
	 * const CriChar8* cue_name = "GUN":
	 * CriUint32 synth_index = cue_sheet->GetSynthIndex(cue_name, err);
	 * CriFloat32 pitch = cue_sheet->GetPitch(synth_index, err);
	 * \endcode
	 * \sa CriAuCueSheet::GetSynthIndex
	 */
	/*EN
	 * \brief Retrieve a pitch of a synthesizer
	 * \param synth_index A synthesizer index
	 * \param err CriError information
	 * \par Description:
	 * This function returns a pitch of the synthesizer specified by synth_index.
	 * \code
	 * CriError err;
	 * const CriChar8* cue_name = "GUN":
	 * CriUint32 synth_index = cue_sheet->GetSynthIndex(cue_name, err);
	 * CriFloat32 pitch = cue_sheet->GetPitch(synth_index, err);
	 * \endcode
	 * \sa CriAuCueSheet::GetSynthIndex
	 */
	virtual CriFloat32 GetPitch(CriUint32 synth_index, CriError &err = criErr::ErrorContainer) = 0;
	/*JP
	 * \brief シンセの波形情報の取得
	 * \param synth_index シンセインデックス<br>
	 * \param err エラーコード<br>
	 * \return シンセの波形情報
	 * \par 説明:
	 * 指定したシンセの波形情報を取得します。<br>
	 * プリミティブシンセサイザの場合のみ取得できます。また、波形のサンプル数（num_samples）はCRI Audio Craft Ver.3.07.00以降で作成されたCSBでのみ有効です。<br>
	 * \code
	 * CriError err;
	 * const CriChar8* cue_name = "GUN":
	 * CriUint32 synth_index = cue_sheet->GetSynthIndex(cue_name, err);
	 * CriAuCueSheet::WaveFormInfo info;
	 * cue_sheet->GetWaveFormInfo(synth_index, info, err);
	 * \endcode
	 * \sa CriAuCueSheet::GetSynthIndex
	 * \sa CriAuCueSheet::WaveFormInfo
	 */
	/*EN
	 * \brief Retrieve a waveform information of a synthesizer
	 * \param synth_index A synthesizer index
	 * \param err CriError information
	 * \par Description:
	 * This function returns a waveform information of the synthesizer specified by synth_index.<br>
	 * This function is available only for primitive synthesizer.
	 * And num_samples is available on CSB made by CRI Audio Craft Ver.3.07.00 and newer version.
	 * \code
	 * CriError err;
	 * const CriChar8* cue_name = "GUN":
	 * CriUint32 synth_index = cue_sheet->GetSynthIndex(cue_name, err);
	 * CriAuCueSheet::WaveFormInfo info;
	 * cue_sheet->GetWaveFormInfo(synth_index, info, err);
	 * \endcode
	 * \sa CriAuCueSheet::GetSynthIndex
	 * \sa CriAuCueSheet::WaveFormInfo
	 */
	virtual void GetWaveFormInfo(CriUint32 synth_index, CriAuCueSheet::WaveFormInfo& info, CriError& err = criErr::ErrorContainer) = 0;


protected:
	CriAuCueSheet(CriHeap heap);
	virtual ~CriAuCueSheet();

	CriHeap heap;
	const CriChar8* name;

private:
	CriAuCueSheet();	// disabled
/*JP
 * @} MDL_LIB_CUE_SHEET
 */
/*EN
 * @} MDL_LIB_CUE_SHEET
 */
};


/*JP
 * \ingroup MDL_LIB_CUE
 * \brief キューハンドル
 * キューをプレーヤに設定するために使用します。
 * キューハンドルを使用することで、キューの情報を複数のプレーヤで共有することが可能になります。
 */
/*EN
 * \ingroup MDL_LIB_CUE
 * \brief Cue Handle
 * Cue Handles uses a cue for a player to set it.
 */
class CriAuCue : public CriAllocator
{
/*JP
 * \addtogroup MDL_LIB_CUE
 * @{
 */
/*EN
 * \addtogroup MDL_LIB_CUE
 * @{
 */
public:
	/*JP
	 * \brief キューハンドルの生成
	 * \param heap ヒープハンドル<br>
	 * \param cue_sheet キューシートハンドル<br>
	 * \param cue_name キュー名<br>
	 * \param err エラーコード<br>
	 * \par 説明:
	 * キューハンドルを生成します。
	 * \code
	 * CriError err;
	 * CriHeap heap = criHeap_Create(buf, sizeof(buf));
	 * CriAuCueSheet* cue_sheet = CriAuCueSheet::Create(heap, err);
	 * // Load Cue Sheet Binary File to Cue Sheet Object
 	 * // ...
	 * const CriChar8* cue_name = "GUN":
	 * // Create Cue
	 * CriAuCue* cue = CriAuCue::Create(heap, cue_sheet, cue_name, err);
	 * \endcode
	 */
	/*EN
	 * \brief Create a cue handle
	 * \param heap A CriHeap handle
	 * \param cue_sheet The cue sheet handle
	 * \param cue_name The name of the cue
	 * \param err CriError information
	 * \return A valid cue handle (CriAuCue)
	 * \par Description:
	 * This function creates a CriAuCue handle.
	 * Memory for the handle is allocated from the given CriHeap structure.
	 * Any memory allocation failure during this function results in NULL.
	 * Make sure to initialize and create your heap with criHeap_Initialize() and
	 * criHeap_Create() before calling this function.
	 * \code
	 * CriError err;
	 * CriHeap heap = criHeap_Create(buf, sizeof(buf));
	 * CriAuCueSheet* cue_sheet = CriAuCueSheet::Create(heap, err);
	 * // Load Cue Sheet Binary File to Cue Sheet Object
 	 * // ...
	 * const CriChar8* cue_name = "GUN":
	 * // Create Cue
	 * CriAuCue* cue = CriAuCue::Create(heap, cue_sheet, cue_name, err);
	 * \endcode
	 * \sa CriAuCue, criHeap_Initialize(), criHeap_Create()
	 */
	static CriAuCue* CRIAPI Create(CriHeap heap, CriAuCueSheet* cue_sheet, const CriChar8 *cue_name, CriError &err = criErr::ErrorContainer);

	/*JP
	 * \brief キューハンドルの生成
	 * \param heap ヒープハンドル<br>
	 * \param cue_sheet キューシートハンドル<br>
	 * \param cue_id キューID<br>
	 * \param err エラーコード<br>
	 * \par 説明:
	 * キューハンドルを生成します。
	 * \code
	 * CriError err;
	 * CriHeap heap = criHeap_Create(buf, sizeof(buf));
	 * CriAuCueSheet* cue_sheet = CriAuCueSheet::Create(heap, err);
	 * // Load Cue Sheet Binary File to Cue Sheet Object
 	 * // ...
	 * CriUint32 cue_id = 3:
	 * // Create Cue
	 * CriAuCue* cue = CriAuCue::CreateById(heap, cue_sheet, cue_id, err);
	 * \endcode
	 */
	/*EN
	 * \brief Create a cue handle
	 * \param heap A CriHeap handle
	 * \param cue_sheet The cue sheet handle
	 * \param cue_id The id of the cue
	 * \param err CriError information
	 * \return A valid cue handle (CriAuCue)
	 * \par Description:
	 * This function creates a CriAuCue handle.
	 * Memory for the handle is allocated from the given CriHeap structure.
	 * Any memory allocation failure during this function results in NULL.
	 * Make sure to initialize and create your heap with criHeap_Initialize() and
	 * criHeap_Create() before calling this function.
	 * \code
	 * CriError err;
	 * CriHeap heap = criHeap_Create(buf, sizeof(buf));
	 * CriAuCueSheet* cue_sheet = CriAuCueSheet::Create(heap, err);
	 * // Load Cue Sheet Binary File to Cue Sheet Object
 	 * // ...
	 * CriUint32 cue_id = 3:
	 * // Create Cue
	 * CriAuCue* cue = CriAuCue::CreateById(heap, cue_sheet, cue_id, err);
	 * \endcode
	 * \sa CriAuCue, criHeap_Initialize(), criHeap_Create()
	 */
	static CriAuCue* CRIAPI CreateById(CriHeap heap, CriAuCueSheet* cue_sheet, CriUint32 cue_id, CriError &err = criErr::ErrorContainer);

	/*JP
	 * \brief キューハンドルの削除
	 * \param err エラーコード
	 * \par 説明:
	 * キューハンドルを削除します。
	 * \code
	 * CriError err;
	 * CriHeap heap = criHeap_Create(buf, sizeof(buf));
	 * CriAuCueSheet* cue_sheet = CriAuCueSheet::Create(heap, err);
	 * // Load Cue Sheet Binary File to Cue Sheet Object
 	 * // ...
	 * const CriChar8* cue_name = "GUN":
	 * // Create Cue
	 * CriAuCue* cue = CriAuCue::Create(heap, cue_sheet, cue_name, err);
	 * // Destroy Cue
	 * cue->Destroy(err);
	 * // Destroy Cue Sheet
	 * cue_sheet->Destroy(err);
	 * \endcode
	 */
	/*EN
	 * \brief Destroy a cue sheet handle
	 * \param err CriError information
	 * \par Description:
	 * This function destroys the CriAuCueSheet handle previously created
	 * with CriAuCueSheet::Create.
	 * \code
	 * CriError err;
	 * CriHeap heap = criHeap_Create(buf, sizeof(buf));
	 * CriAuCueSheet* cue_sheet = CriAuCueSheet::Create(heap, err);
	 * // Load Cue Sheet Binary File to Cue Sheet Object
 	 * // ...
	 * const CriChar8* cue_name = "GUN":
	 * // Create Cue
	 * CriAuCue* cue = CriAuCue::Create(heap, cue_sheet, cue_name, err);
	 * // Destroy Cue
	 * cue->Destroy(err);
	 * // Destroy Cue Sheet
	 * cue_sheet->Destroy(err);
	 * \endcode
	 */
	virtual void Destroy(CriError &err = criErr::ErrorContainer) = 0;

protected:
	CriAuCue(CriHeap heap);
	virtual ~CriAuCue();

	CriHeap heap;
	const CriChar8* name;

private:
	CriAuCue();	// disabled
/*JP
 * @} MDL_LIB_CUE
 */
/*EN
 * @} MDL_LIB_CUE
 */
};

/*JP
 * \ingroup MDL_LIB_AUDIO
 * \brief CRI Audio オブジェクト
 * <b>CRI Audio オブジェクト </b>は再生をコントロールするためのオブジェクトです。<br>
 * メモリを確保するときに使用するヒープハンドルと接続先のサウンドレンダラを指定して作成します。<br>
 * 音声を再生するためには、まず、キューシートハンドルを登録します。<br>
 * そして、キューを指定してPlay関数を実行するだけで、簡単にサウンドを再生できます。<br>
 * さらに、細かい制御を行うために、子供の<b>CRI Audio プレーヤ </b> (CriAuPlayer)を作成できます。<br>
 */
/*EN
 * \ingroup MDL_LIB_AUDIO
 * \brief CRI Audio Object
 * CRI Audio object manages the overall process of playing back sounds. It allocates
 * work memory blocks from a CriHeap handle, keeps several cue sheet handles in which
 * cue names to play are searched, and simply plays sounds by calling Play().
 * It requires a valid CriHeap handle to allocate memory and a sound renderer to output.
 * At first, you need to set a cue sheet handle, then call Play() to play sounds with specified
 * cues.
 *
 * You can play multiple cues easily with CRI Audio object.
 * You just create a CRI Audio object, attach some cue sheet handles,
 * and call <b>Play()</b> with cue names. <b>Stop()</b> stops all currently
 * playing sounds immediately.
 *
 * To play and stop each cue, you need to use a sub-class CriAuPlayer.
 */
class CriAuObj : public CriAllocator
{
/*JP
 * \addtogroup MDL_LIB_AUDIO
 * @{
 */
/*EN
 * \addtogroup MDL_LIB_AUDIO
 * @{
 */
public:
	static const CriUint32 DEF_MAX_CUE_SHEET=16;

	enum StopMode {
		STOP_MODE_RELEASE = 0,	/*JP< リリース時間を経て停止	*/
								/*EN< Stop after release time	*/
		STOP_MODE_IMMEDIATE		/*JP< 即時停止					*/
								/*EN< Stop immediately			*/
	};

	//	Object Operation =========================================================================
	/*JP
	 * \brief CRI Audio オブジェクトの生成
	 * \param heap ヒープハンドル<br>
	 * \param sndrndr サウンドレンダラ<br>
	 * \param name 名前<br>
	 * \param err エラーコード<br>
	 * \par 説明:
	 * CRI Audio オブジェクトを生成します。<br>
	 * キューシートを16個までアタッチできます。<br>
	 * <br>
	 * 現在、未サポートですが、nameはデバッグ時のための情報です。<br>
	 * \code
	 * CriError err;
	 * CriHeap heap;
	 * CriSoundRendererBasic* sndrndr = CriSoundRendererBasic::Create(heap, err);
	 * const CriChar8* name = "CRI Audio object";
	 * // Create CRI Audio Object
	 * CriAuObj* auobj = CriAuObj::Create(heap, sndrndr, name, err);
	 * \endcode
	 */
	/*EN
	 * \brief Create CRI Audio object
	 * \param heap A valid CriHeap handle
	 * \param sndrndr A sound renderer
	 * \param name CRI Audio object name (This is for debug use, and is not supported now.)
	 * \param err CriError information
	 * \par Description:
	 * This function creates a CRI Audio object by allocating a certain memory block
	 * from CriHeap and sets a sound renderer as an output module.<br>
	 * The maximum number of a cue sheets you can attach is 16.<br>
	 * \code
	 * CriError err;
	 * CriHeap heap;
	 * CriSoundRendererBasic* sndrndr = CriSoundRendererBasic::Create(heap, err);
	 * const CriChar8* name = "CRI Audio object";
	 * // Create CRI Audio Object
	 * CriAuObj* auobj = CriAuObj::Create(heap, sndrndr, name, err);
	 * \endcode
	 */
	static CriAuObj* CRIAPI Create(CriHeap heap, CriSoundRenderer* sndrndr, const CriChar8* name, CriError &err = criErr::ErrorContainer);
	/*JP
	 * \brief CRI Audio オブジェクトの生成
	 * \param heap ヒープハンドル<br>
	 * \param sndrndr サウンドレンダラ<br>
	 * \param name 名前<br>
	 * \param max_cue_sheet 登録できる最大キューシート数<br>
	 * \param err エラーコード<br>
	 * \par 説明:
	 * CRI Audio オブジェクトを生成します。<br>
	 * <br>
	 * 現在、未サポートですが、nameはデバッグ時のための情報です。
	 * \code
	 * CriError err;
	 * CriHeap heap;
	 * CriSoundRendererBasic* sndrndr = CriSoundRendererBasic::Create(heap, err);
	 * CriUint32 max_cue_sheet = 100;
	 * const CriChar8* name = "CRI Audio object";
	 * // Create CRI Audio object
	 * CriAuObj* auobj = CriAuObj::Create(heap, sndrndr, name, max_cue_sheet, err);
	 * \endcode
	 */
	/*EN
	 * \brief Create CRI Audio object
	 * \param heap A valid CriHeap handle
	 * \param sndrndr A sound renderer
	 * \param name CRI Audio object name (This is for a debug use, and is not supported now.)
	 * \param max_cue_sheet Maximum number of cue sheets<br>
	 * \param err CriError information
	 * \par Description:
	 * This function creates a CRI Audio object by allocating a certain memory block
	 * from CriHeap and sets a sound renderer as an output module.
	 * \code
	 * CriError err;
	 * CriHeap heap;
	 * CriSoundRendererBasic* sndrndr = CriSoundRendererBasic::Create(heap, err);
	 * CriUint32 max_cue_sheet = 100;
	 * const CriChar8* name = "CRI Audio object";
	 * // Create CRI Audio object
	 * CriAuObj* auobj = CriAuObj::Create(heap, sndrndr, name, max_cue_sheet, err);
	 * \endcode
	 */
	static CriAuObj* CRIAPI Create(CriHeap heap, CriSoundRenderer* sndrndr, const CriChar8* name, CriUint32 max_cue_sheet, CriError &err = criErr::ErrorContainer);
	/*JP
	 * \brief CRI Audio オブジェクトの実行
	 * \param	err エラーコード
	 * \par 説明:
	 * CRI Audio オブジェクトを実行します。<br>
	 * 内部状態を更新したり、再生の終了したボイスを削除したりします。
	 * \code
	 * CriError err;
	 * CriAuObj* auobj = CriAuObj::Create(heap, sndrndr, "name", err);
	 * // Main Loop	
	 * for (;;) {
	 *     if ( SyncFrame() == FALSE )
	 *         break;
	 *     // Update Status and Delete voices which are end of playback
	 *     CriAuObj::ExecuteMain(err);
	 * }
	 * \endcode
	 */
	/*EN
	 * \brief Execute CRI Audio object
	 * \param err CriError information
	 * \par Description:
	 * This function gives some CPU resources and lets CRI Audio do its task.
	 * This function updates the internal state of a CRI Audio object and does some
	 * housekeeping tasks for such resources as deleting voices after playback is complete, etc.
	 * \code
	 * CriError err;
	 * CriAuObj* auobj = CriAuObj::Create(heap, sndrndr, "name", err);
	 * // Main Loop	
	 * for (;;) {
	 *     if ( SyncFrame() == FALSE )
	 *         break;
	 *     // Update Status and Delete voices which are end of playback
	 *     CriAuObj::ExecuteMain(err);
	 * }
	 * \endcode
	 */
	static void CRIAPI ExecuteMain(CriError &err = criErr::ErrorContainer);
	/*JP
	 * \brief CRI Audio オブジェクトの削除
	 * \param	err エラーコード
	 * \par 説明:
	 * CRI Audio オブジェクトを削除します。
	 * 再生されている音声は停止します。
	 * \code
	 * CriError err;
	 * CriAuObj* auobj = CriAuObj::Create(heap, sndrndr, "name", max_cue_sheet, err);
	 * // Main Loop	
	 * for (;;) {
	 *     if ( SyncFrame() == FALSE )
	 *         break;
	 *     // Update Status and Delete voices which are end of playback
	 *     CriAuObj::ExecuteMain(err);
	 * }
	 * // Destroy CRI Audio object
	 * auobj->Destroy(err);
	 * \endcode
	 */
	/*EN
	 * \brief Destroy CRI Audio object
	 * \param err CriError information
	 * \par Description:
	 * This function stops all sounds playing via a CRI Audio object and deletes them.
	 * \code
	 * CriError err;
	 * CriAuObj* auobj = CriAuObj::Create(heap, sndrndr, "name", max_cue_sheet, err);
	 * // Main Loop	
	 * for (;;) {
	 *     if ( SyncFrame() == FALSE )
	 *         break;
	 *     // Update Status and Delete voices which are end of playback
	 *     CriAuObj::ExecuteMain(err);
	 * }
	 * // Destroy CRI Audio object
	 * auobj->Destroy(err);
	 * \endcode
	 */
	virtual void Destroy(CriError &err = criErr::ErrorContainer) = 0;
	//	Cue Sheet =================================================================================
	/*JP
	 * \brief キューシートハンドルの登録
	 * \param csht キューシートハンドル<br>
	 * \param err エラーコード<br>
	 * \par 説明:
	 * CRI Audio オブジェクトにキューシートハンドルを登録します。<br>
	 * 新しく登録されたキューシートハンドルは、CRI Audio オブジェクト内のインデックス0番に配置され、
	 * すでに登録されていたキューシートハンドルのインデックスは1ずつ増加します。
	 * CriAuObj::GetCueSheet関数を使用する場合にはこの点に注意してください。
	 * \code
	 * CriError err;
	 * CriAuCueSheet* cue_sheet = CriAuCueSheet::Create(heap, err);
	 * // Attach cue sheet to CRI Audio object
	 * auobj->AttachCueSheet(cue_sheet, err);
	 * \endcode
	 * \sa CriAuObj::DetachCueSheet
	 * \sa CriAuObj::GetCueSheet
	 * \sa CriAuObj::GetNumberOfCueSheets
	 */
	/*EN
	 * \brief Attach a cue sheet handle to CRI Audio object
	 * \param csht Cue sheet handle to be attached
	 * \param err CriError information
	 * \par Description:
	 * This function attaches a cue sheet handle to a CRI Audio object.<br>
	 * The newly-attached cue sheet handle is assigned to the index number 0 in a CRI Audio Object.
	 * And the index number of each cue sheet handle that has been already attached is incremented by one.
	 * Please note the above if you use CriAuObj::GetCueSheet function.
	 * \code
	 * CriError err;
	 * CriAuCueSheet* cue_sheet = CriAuCueSheet::Create(heap, err);
	 * // Attach cue sheet to CRI Audio object
	 * auobj->AttachCueSheet(cue_sheet, err);
	 * \endcode
	 * \sa CriAuObj::DetachCueSheet
	 * \sa CriAuObj::GetCueSheet
	 * \sa CriAuObj::GetNumberOfCueSheets
	 */
	virtual void AttachCueSheet(CriAuCueSheet* csht, CriError &err = criErr::ErrorContainer) = 0;
	/*JP
	 * \brief キューシートハンドルの登録解除
	 * \param csht キューシートハンドル<br>
	 * \param err エラーコード<br>
	 * \par 説明:
	 * CRI Audio オブジェクトからキューシートハンドルの登録を解除します。
	 * \code
	 * CriError err;
	 * CriAuCueSheet* cue_sheet = CriAuCueSheet::Create(heap, err);
	 * auobj->AttachCueSheet(cue_sheet, err);
	 * // Detach cue sheet from CRI Audio object
	 * auobj->DetachCueSheet(cue_sheet, err);
	 * \endcode
	 * \sa CriAuObj::AttachCueSheet
	 */
	/*EN
	 * \brief Detach a cue sheet handle from CRI Audio object
	 * \param csht Cue sheet handle to be detached
	 * \param err CriError information
	 * \par Description:
	 * This function detaches a cue sheet handle from a CRI Audio object.
	 * \code
	 * CriError err;
	 * CriAuCueSheet* cue_sheet = CriAuCueSheet::Create(heap, err);
	 * auobj->AttachCueSheet(cue_sheet, err);
	 * // Detach cue sheet from CRI Audio object
	 * auobj->DetachCueSheet(cue_sheet, err);
	 * \endcode
	 * \sa CriAuObj::AttachCueSheet
	 */
	virtual void DetachCueSheet(CriAuCueSheet* csht, CriError &err = criErr::ErrorContainer) = 0;
	/*JP
	 * \brief すべてのキューシートハンドルの登録解除
	 * \param err エラーコード<br>
	 * \par 説明:
	 * CRI Audio オブジェクトからすべてのキューシートハンドルの登録を解除します。
	 * \code
	 * CriError err;
	 * CriAuCueSheet* cue_sheet1 = CriAuCueSheet::Create(heap, err);
	 * CriAuCueSheet* cue_sheet2 = CriAuCueSheet::Create(heap, err);
	 * auobj->AttachCueSheet(cue_sheet1, err);
	 * auobj->AttachCueSheet(cue_sheet2, err);
	 * // Detach all cue sheet from CRI Audio object
	 * auobj->DetachAllCueSheet(err);
	 * \endcode
	 */
	/*EN
	 * \brief Detach all cue sheet handle from CRI Audio object
	 * \param err CriError information
	 * \par Description:
	 * This function detaches all cue sheet handle from a CRI Audio object.
	 * \code
	 * CriError err;
	 * CriAuCueSheet* cue_sheet1 = CriAuCueSheet::Create(heap, err);
	 * CriAuCueSheet* cue_sheet2 = CriAuCueSheet::Create(heap, err);
	 * auobj->AttachCueSheet(cue_sheet1, err);
	 * auobj->AttachCueSheet(cue_sheet2, err);
	 * // Detach all cue sheet from CRI Audio object
	 * auobj->DetachAllCueSheet(err);
	 * \endcode
	 */
	virtual void DetachAllCueSheet(CriError &err = criErr::ErrorContainer) = 0;
	/*JP
	 * \brief キューシートハンドルの取得
	 * \param index インデックス<br>
	 * \param err エラーコード<br>
	 * \return キューシートハンドル<br>
	 * \par 説明:
	 * キューシートハンドルを取得します。
	 * \code
	 * CriError err;
	 * CriUint32 index = 0;
	 * // Get cue sheet by index from CRI Audio object
	 * CriAuCueSheet* cue_sheet = auobj->GetCueSheet(index, err);
	 * \endcode
	 */
	/*EN
	 * \brief Retrieve a cue sheet handle from CRI Audio object
	 * \param index An index of cue sheets in the CRI Audio Object
	 * \param err CriError information
	 * \return Cue sheet handle
	 * \par Description:
	 * Retrieves a cue sheet handle from a CRI Audio object.
	 * \code
	 * CriError err;
	 * CriUint32 index = 0;
	 * // Get cue sheet by index from CRI Audio object
	 * CriAuCueSheet* cue_sheet = auobj->GetCueSheet(index, err);
	 * \endcode
	 */
	virtual CriAuCueSheet* GetCueSheet(CriUint32 index, CriError &err = criErr::ErrorContainer) = 0;
	/*JP
	 * \brief アタッチ済みのキューシートハンドル数の取得
	 * \param err エラーコード<br>
	 * \return アタッチ済みのキューシートハンドル数<br>
	 * \par 説明:
	 * アタッチ済みのキューシートハンドル数を取得します。
	 * \code
	 * CriError err;
	 * // Get number of cue sheet handles from CRI Audio object
	 * CriUint32 num_cue_sheets = auobj->GetNumberOfCueSheets(err);
	 * \endcode
	 */
	/*EN
	 * \brief Retrieve the number of attached cue sheet handles
	 * \param err CriError information
	 * \return Number of attached cue sheet handles
	 * \par Description:
	 * Retrieves the number of attached cue sheet handles from a CRI Audio object.
	 * \code
	 * CriError err;
	 * // Get number of cue sheet handles from CRI Audio object
	 * CriUint32 num_cue_sheets = auobj->GetNumberOfCueSheets(err);
	 * \endcode
	 */
	virtual CriUint32 GetNumberOfCueSheets(CriError &err = criErr::ErrorContainer) = 0;

	//	Play back control =========================================================================
	/*JP
	 * \brief 再生状態
	 */
	/*EN
	 * \brief Playback status
	 */
	enum PlaybackStatus {
		PLAYBACK_STATUS_STOP = (0),		/*JP< 停止		*/
										/*EN< Stoped	*/
		PLAYBACK_STATUS_PLAYING,		/*JP< 再生中	*/
										/*EN< Playing	*/
	};
	/*JP
	 * \brief Cueの単純再生
	 * \param cue_name キュー名<br>
	 * \param err エラーコード<br>
	 * \par 説明:
	 * CRI Audio オブジェクトを使用してキューを再生します。<br>
	 * サウンドデザイナの設定パラメータ値を変更することなく、再生のみを行います。
	 * ボリュームやピッチなど、パラメータを変更して再生を行うことはできません。<br>
	 * 複数のキューシートハンドルが登録されている場合、最後に登録されたものから順に各キューシートハンドル内を検索します。<br>
	 * \code
	 * CriError err;
	 * const CriChar8* cue_name = "GUN";
	 * // Play Cue
	 * auobj->Play(cue_name, err):
	 * \endcode
	 */
	/*EN
	 * \brief Play a cue
	 * \param cue_name A cue name
	 * \param err CriError information
	 * \par Description:
	 * This function starts playing a cue by specifying a cue name in cue sheets
	 * which are attached to the CRI Audio object.<br>
	 * When you set multiple cue sheets to a CRI Audio Object, the cue is searched beginning at the cue sheet that last attached.
	 * \code
	 * CriError err;
	 * const CriChar8* cue_name = "GUN";
	 * // Play Cue
	 * auobj->Play(cue_name, err):
	 * \endcode
	 */
	virtual void Play(const CriChar8 *cue_name, CriError &err = criErr::ErrorContainer) = 0;
	/*JP
	 * \brief キューIDによるCueの単純再生
	 * \param cue_id キューID<br>
	 * \param err エラーコード<br>
	 * \par 説明:
	 * キューIDを指定して再生します。<br>
	 * サウンドデザイナの設定パラメータ値を変更することなく、再生のみを行います。
	 * ボリュームやピッチなど、パラメータを変更して再生を行うことはできません。<br>
	 * 複数のキューシートハンドルが登録されている場合、最後に登録されたものから順に各キューシートハンドル内を検索します。<br>
	 * \code
	 * CriError err;
	 * CriUint32 cue_id = 10;
	 * // Play Cue
	 * auobj->PlayById(cue_id, err):
	 * \endcode
	 */
	/*EN
	 * \brief Play a cue by ID
	 * \param cue_id Cue ID
	 * \param err CriError information
	 * \par Description:
	 * This function starts playing a cue by specifying a cue ID in cue sheets
	 * which are attached to the CRI Audio object.<br>
	 * When you set multiple cue sheets to a CRI Audio Object, the cue is searched beginning at the cue sheet that last attached.
	 * \code
	 * CriError err;
	 * CriUint32 cue_id = 10;
	 * // Play Cue
	 * auobj->PlayById(cue_id, err):
	 * \endcode
	 */
	virtual void PlayById(CriUint32 cue_id, CriError &err = criErr::ErrorContainer) = 0;
	/*JP
	 * \brief 再生中のCueの停止
	 * \param err エラーコード
	 * \par 説明:
	 * CRI Audio オブジェクトで発音しているすべてのボイスを停止します。<br>
	 * リリースタイムが設定されているボイスの場合は、リリース状態になります。
	 * \code
	 * CriError err;
	 * // Stop all sounds under CRI Audio object.
	 * auobj->Stop(err):
	 * \endcode
	 */
	/*EN
	 * \brief Stop all sounds under CRI Audio object
	 * \param err CriError information
	 * \par Description:
	 * This function stops playing all cues for the CRI Audio object.
	 * If the sound has release time, the sound changes to released status.
	 * \code
	 * CriError err;
	 * // Stop all sounds under CRI Audio object.
	 * auobj->Stop(err):
	 * \endcode
	 */
	virtual void Stop(CriError &err = criErr::ErrorContainer) = 0;
	/*JP
	 * \brief 再生中のCueの停止
	 * \param stop_mode 停止方法
	 * \param err エラーコード
	 * \par 説明:
	 * CRI Audio オブジェクトで発音しているすべてのボイスを停止します。停止方法を指定できます。
	 * \code
	 * CriError err;
	 * CriAuObj::StopMode stop_mode = CriAuObj::STOP_MODE_RELEASE;
	 * // Stop all sounds under CRI Audio object.
	 * auobj->Stop(stop_mode, err):
	 * \endcode
	 */
	/*EN
	 * \brief Stop all sounds under CRI Audio object
	 * \param stop_mode Stop mode
	 * \param err CriError information
	 * \par Description:
	 * This function stops all cues playing in the CRI Audio object, using the specified stop mode.
	 * \code
	 * CriError err;
	 * CriAuObj::StopMode stop_mode = CriAuObj::STOP_MODE_RELEASE;
	 * // Stop all the sounds under the CRI Audio object.
	 * auobj->Stop(stop_mode, err):
	 * \endcode
	 */
	virtual void Stop(StopMode stop_mode, CriError &err = criErr::ErrorContainer) = 0;
	/*JP
	 * \brief 再生ステータスの取得
	 * \param	err エラーコード
	 * \par 説明:
	 * CRI Audio オブジェクトの再生ステータスを取得します。
	 * <br>
	 * \code
	 * CriError err;
	 * // Get playback status of a CRI Audio object
	 * CriAuObj::PlaybackStatus status = auobj->GetPlaybackStatus(err):
	 * \endcode
	 */
	/*EN
	 * \brief Retrieve the playback status
	 * \param err CriError information
	 * \return One of the PlaybackStatus enum values
	 * <br>
	 * \code
	 * CriError err;
	 * // Get playback status of a CRI Audio object
	 * CriAuObj::PlaybackStatus status = auobj->GetPlaybackStatus(err):
	 * \endcode
	 */
	virtual PlaybackStatus GetPlaybackStatus(CriError &err = criErr::ErrorContainer) = 0;
	/*JP
	 *	\brief 再生中のボイス数の取得
	 *	\par 説明:
	 *	\param	err エラーコード<br>
	 *	\return	ボイス数
	 *	現在再生しているボイス数を返します。
	 * \code
	 * CriError err;
	 * // Get number of voices from a CRI Audio object
	 * CriUint32 num_voices = auobj->GetNumberOfVoices(err):
	 * \endcode
	 */
	/*EN
	 * \brief Retrieve the number of voices currently playing
	 * \param err CriError information
	 * \return	Number of voices
	 * \par Description:
	 * This function returns the number of voices playing now.
	 * \code
	 * CriError err;
	 * // Get the number of voices from CRI Audio object
	 * CriUint32 num_voices = auobj->GetNumberOfVoices(err):
	 * \endcode
	 */
	virtual CriUint32 GetNumberOfVoices(CriError &err = criErr::ErrorContainer) = 0;
	/*JP
	 * \brief 全てのプレーヤに対する一時停止フラグ設定
	 * \param flag 一時停止フラグ
	 * \param err エラーコード
	 * \par 説明:
	 * CRI Audio オブジェクトで管理している全てのプレーヤに一時停止フラグを設定します。
	 * \code
	 * CriError err;
	 * // Set pause flag to all players in the CRI Audio object
	 * auobj->Pause(TURE, err):
	 * \endcode
	 */
	/*EN
	 * \brief Set a pause flag to all players in CRI Audio object
	 * \param flag Pause flag
	 * \param err CriError information
	 * \par Description:
	 * This function sets a pause flag to all players for the CRI Audio object.
	 * \code
	 * CriError err;
	 * // Set pause flag to all players in the CRI Audio object
	 * auobj->Pause(TURE, err):
	 * \endcode
	 */
	virtual void Pause(CriBool flag, CriError &err = criErr::ErrorContainer) = 0;
	//	Processing ================================================================================

#ifndef DOXYGEN_SHOULD_SKIP_THIS
	/*JP
	 * \brief サーバ関数
	 * \param	err エラーコード
	 * \par 説明:
	 * 内部状態を更新します。<br>
	 * 内部関数なので、ユーザーは使用しないでください。<br>
	 */
	/*EN
	 * \brief Execute a heartbeat function for a CRI Audio object (Internal)
	 * \param err CriError information
	 * \par Description:
	 * This function executes a heartbeat function to allocate a certain
	 * amount of a CPU time. This function updates the internal status.
	 * This function is called internally, DO NOT call this function.
	 */
	virtual void Execute(CriError &err = criErr::ErrorContainer) = 0;
#endif	// DOXYGEN_SHOULD_SKIP_THIS

protected:
	CriAuObj(CriHeap heap, CriSoundRenderer* crisr, const CriChar8* name);
	virtual ~CriAuObj(void);

	CriHeap heap;
	CriSoundRenderer* crisr;
	const CriChar8* name;

private:
	CriAuObj();	// disabled
/*JP
 * @} MDL_LIB_AUDIO
 */
/*EN
 * @} MDL_LIB_AUDIO
 */
};

/*JP
 * \ingroup MDL_LIB_PLAYER
 * \brief CRI Audioプレーヤ
 *
 * <b>CRI Audioプレーヤ </b>は、<b>CRI Audioオブジェクト </b>のサブクラスです。<br>
 * <b>CRI Audioオブジェクト </b>によって再生する場合は、サウンドデザイナが設定した
 * 再生パラメータをそのまま変更せずに再生するのに対して、
 * <b>CRI Audioプレーヤ </b>を使用すると、ボリュームやピッチ等の再生パラメータを
 * 変更（設定）して再生することが可能です。
 * 下記の再生パラメータを設定できます。<br>
 *	- ボリューム
 *	- ピッチ
 *	- フィルタパラメータ
 *	- ドライセンドレベル
 *	- ウェットセンドレベル
 *	- パンニング3D
 *	- アイザックのコントロール値
 *	- アイザックのランダム範囲
 *
 * CRI AudioプレーヤはCRI Audioオブジェクトのサブクラスであるため、
 * 親のCRI Audioオブジェクトに対してStop（再生停止）指示が発行されると
 * CRI Audioオブジェクト配下で再生中の全てのキューが再生停止します。
 */
/*EN
 * \ingroup MDL_LIB_PLAYER
 * \brief CRI Audio player
 * CRI Audio player plays sounds with certain playback parameters:
 *	- Volume
 *	- Pitch
 *	- Filter parameter
 *	- Dry send levels
 *	- Wet send levels
 *	- Panning 3D
 *	- Control value for AISAC
 *	- Random range for AISAC
 *
 * A CRI Audio player is a subclass of a CRI Audio object. 
 * Therefore all cues playing from the object stop when you call a Stop function for the CRI Audio object.
 *
 */
class CriAuPlayer : public CriAllocator
{
/*JP
 * \addtogroup MDL_LIB_PLAYER
 * @{
 */
/*EN
 * \addtogroup MDL_LIB_PLAYER
 * @{
 */
public:
	static const CriUint32 PLAYER_ID_ANY=0xffffffff;

	enum Status {
		STATUS_STOP = (0),		/*JP< 停止			*/
								/*EN< STOP			*/
		STATUS_PREP,			/*JP< 再生準備中	*/
								/*EN< PREPARING		*/
		STATUS_PLAYING,			/*JP< 再生中		*/
								/*EN< PLAYING		*/
		STATUS_PLAYEND,			/*JP< 再生終了		*/
								/*EN< PLAYEND		*/
		STATUS_ERROR			/*JP< エラー		*/
								/*EN< ERROR			*/
	};

	enum StopMode {
		STOP_MODE_RELEASE = 0,	/*JP< リリース時間を経て停止	*/
								/*EN< Stop after release time	*/
		STOP_MODE_IMMEDIATE		/*JP< 即時停止					*/
								/*EN< Stop immediately			*/
	};

	enum InvertMode {
		INVERT_MODE_NONE		= 0,	/*JP< 反転なし	*/
										/*EN< Not invert	*/
		INVERT_MODE_LEFT_RIGHT,			/*JP< 左右反転		*/
										/*EN< Invert right and left	*/
	};

	struct Parameters {
		CriFloat32 gain_volume;		/* Gain of Volume	*/
		CriFloat32 offset_volume;		/* Offset of Volume	*/
		CriFloat32 gain_pitch;			/* Gain of Pitch	*/
		CriFloat32	offset_pitch;		/* Offset of Pitch	*/
		CriFloat32 gain_predelay;		/* Gain of PreDelay	*/
		CriFloat32 offset_predelay;	/* Offset of PreDelay	*/
	};

	/*JP
	 * \brief プレーヤの生成
	 * \param auobj CRI Audio オブジェクト<br>
	 * \param err エラーコード<br>
	 * \return	CRI Audio プレーヤ
	 * \par 説明:
	 * CRI Audioプレーヤを生成します。<br>
	 * auobjに従属し、auobjに対して実行された影響を受けます。<br>
	 * 例えば、親のauobjに対して、Stop関数を実行すると、子供のプレーヤは再生を停止します。
	 * \code
	 * CriError err;
	 * CriAuObj* auobj = CriAuObj::Create(heap, sndrndr, "name", err);
	 * // Create CRI Audio Player
	 * CriAuPlayer* auply = CriAuPlayer::Create(auobj, err);
	 * \endcode
	 */
	/*EN
	 * \brief Create a player
	 * \param auobj CRI Audio object
	 * \param err CriError information
	 * \return	CRI Audio player
	 * \par Description:
	 * This function creates a CRI Audio player as a child class
	 * of the CRI Audio object <b>auobj</b>. That is, the player stops playing
	 * when <b>Stop</b> function is called for the parent, CRI Audio object <b>auobj</b>.
	 * \code
	 * CriError err;
	 * CriAuObj* auobj = CriAuObj::Create(heap, sndrndr, "name", err);
	 * // Create CRI Audio Player
	 * CriAuPlayer* auply = CriAuPlayer::Create(auobj, err);
	 * \endcode
	 */
	static CriAuPlayer* CRIAPI Create(CriAuObj* auobj, CriError &err = criErr::ErrorContainer);
	/*JP
	 * \brief CRI Audioプレーヤの削除
	 * \param	err エラーコード
	 * \par 説明:
	 * プレーヤを削除します。
	 * \code
	 * CriError err;
	 * CriAuPlayer* auply = CriAuPlayer::Create(auobj, err);
	 * // Destroy CRI Audio Player
	 * auply->Destroy(err);
	 * \endcode
	 */
	/*EN
	 * \brief Destroy the CRI Audio player
	 * \param err CriError information
	 * \par Description:
	 * This function discards the player.
	 * \code
	 * CriError err;
	 * CriAuPlayer* auply = CriAuPlayer::Create(auobj, err);
	 * // Destroy CRI Audio Player
	 * auply->Destroy(err);
	 * \endcode
	 */
	virtual void Destroy(CriError &err = criErr::ErrorContainer) = 0;
	/*JP
	 * \brief プレーヤのステータス取得
	 * \param err エラーコード
	 * \return 再生状態
	 * \par 説明:
	 * プレーヤの再生ステータスを取得します。
	 * ステータスは、作成された直後はSTOP状態です。<br>
	 * Play関数を実行した直後、PREPを経てPLAYINGになります。<br>
	 * プレーヤからは複数のサウンドを再生できますので、すべてのサウンドの再生が終了すると、
	 * PLAYEND状態になります。
	 * \code
	 * CriError err;
	 * // Get status of a player
	 * CriAuPlayer::Status status = auply->GetStatus(err);
	 * \endcode
	 */
	/*EN
	 * \brief Retrieve player status
	 * \param err CriError information
	 * \return Playback status
	 * \par Description:
	 * This function returns the player status.
	 * The player status after creation is STOP.
	 * When Play() is called, the status begins with PREP and transitions to PLAYING.
	 * A player can play more than one sound, and the status will be PLAYEND 
	 * when all sounds finish playing.
	 * \code
	 * CriError err;
	 * // Get status of a player
	 * CriAuPlayer::Status status = auply->GetStatus(err);
	 * \endcode
	 */
	virtual Status GetStatus(CriError &err = criErr::ErrorContainer) const =0;
	/*JP
	 * \brief キューの設定
	 * \param cue_name キュー名<br>
	 * \param err エラーコード<br>
	 * \par 説明:
	 * プレーヤにキューネームを指定してキューを設定します。<br>
	 * 複数のキューシートがCRI Audioオブジェクトにアタッチされている場合は、<br>
	 * 最後のアタッチされたものから順に各キューシートを検索します。<br>
	 * この関数が実行されると、キューにリンクしているシンセサイザが作成されます。
	 * \code
	 * CriError err;
	 * const CriChar8* cue_name = "GUN";
	 * // Set cue to player
	 * auply->SetCue(cue_name, err);
	 * \endcode
	 */
	/*EN
	 * \brief Set a cue to the player by Cue Name
	 * \param cue_name Cue name
	 * \param err CriError information
	 * \par Description:
	 * Set a cue by specifying the cue name.
	 * When you set multiple cue sheets to a CRI Audio Object, the cue is searched beginning at the cue sheet that last attached.
	 * A synthesizer linked to this cue is created when this function is called.
	 * \code
	 * CriError err;
	 * const CriChar8* cue_name = "GUN";
	 * // Set cue to player
	 * auply->SetCue(cue_name, err);
	 * \endcode
	 */
	virtual void SetCue(const CriChar8* cue_name, CriError &err = criErr::ErrorContainer) = 0;
	/*JP
	 * \brief 特定のキューシート内のキューの設定
	 * \param cue_name キュー名<br>
	 * \param cue_sheet キューシートハンドル<br>
	 * \param err エラーコード<br>
	 * \par 説明:
	 * プレーヤにキューネームとキューシートを指定して、キューを設定します。<br>
	 * 複数のキューシートがCRI Audioオブジェクトに設定されているときに、<br>
	 * 明示的にキューシートを指定したい場合に使用します。<br>
	 * この関数が実行されると、キューにリンクしているシンセサイザが作成されます。
	 * \code
	 * CriError err;
	 * CriAuCueSheet* cue_sheet;
	 * const CriChar8* cue_name = "GUN";
	 * // Set cue to player
	 * auply->SetCue(cue_name, cue_sheet, err);
	 * \endcode
	 */
	/*EN
	 * \brief Set a cue to the player by Cue Name and Cue Sheet
	 * \param cue_name Cue name
	 * \param cue_sheet The cue sheet handle
	 * \param err CriError information
	 * \par Description:
	 * This function set a cue by specifying the cue name and cue sheet.
	 * When you set multiple cue sheets to a CRI Audio Object, you can set the cue by your specified cue sheet.
	 * A synthesizer linked to this cue is created when this function is called.
	 * \code
	 * CriError err;
	 * CriAuCueSheet* cue_sheet;
	 * const CriChar8* cue_name = "GUN";
	 * // Set cue to player
	 * auply->SetCue(cue_name, cue_sheet, err);
	 * \endcode
	 */
	virtual void SetCue(const CriChar8* cue_name, CriAuCueSheet* cue_sheet, CriError &err = criErr::ErrorContainer) = 0;

	/*JP
	 * \brief キューハンドルを使用したキューの設定
	 * \param cue キューハンドル<br>
	 * \param err エラーコード<br>
	 * \par 説明:
	 * プレーヤにキューハンドルを指定して、キューを設定します。<br>
	 * \code
	 * CriError err;
	 * CriAuCue* gun = CriAuCue::Create("GUN");
	 * // Set cue to player
	 * auply->SetCue(gun, err);
	 * \endcode
	 */
	/*EN
	 * \brief Set a cue to the player by Cue handle
	 * \param cue Cue handle
	 * \param err CriError information
	 * \par Description:
	 * This function set a cue by specifying the cue handle.
	 * \code
	 * CriError err;
	 * CriAuCue* gun = CriAuCue::Create("GUN");
	 * // Set cue to player
	 * auply->SetCue(gun, err);
	 * \endcode
	 */
	virtual void SetCue(CriAuCue* cue, CriError &err = criErr::ErrorContainer) = 0;

	/*JP
	 * \brief キューIDによるキューの設定
	 * \param cue_id キューID<br>
	 * \param err エラーコード<br>
	 * \par 説明:
	 * プレーヤにキューIDを指定してキューを設定します。<br>
	 * 複数のキューシートがCRI Audioオブジェクトにアタッチされている場合は、<br>
	 * 最後のアタッチされたものから順に各キューシートを検索します。<br>
	 * この関数が実行されると、キューにリンクしているシンセサイザが作成されます。
	 * \code
	 * CriError err;
	 * CriAuCueSheet* cue_sheet;
	 * CriUint32 cue_id = 10;
	 * // Set cue to player
	 * auply->SetCueById(cue_id, err);
	 * \endcode
	 */
	/*EN
	 * \brief Set a cue to the player by Cue ID
	 * \param cue_id Cue ID
	 * \param err CriError information
	 * \par Description:
	 * This function set a cue by specifying the cue ID, where the ID is a certain number
	 * for the cue assigned by a sound designer.
	 * When you set multiple cue sheets to a CRI Audio Object, the cue is searched beginning at the cue sheet that last attached.
	 * A synthesizer linked to this cue is created when this function is called.
	 * \code
	 * CriError err;
	 * CriAuCueSheet* cue_sheet;
	 * CriUint32 cue_id = 10;
	 * // Set cue to player
	 * auply->SetCueById(cue_id, err);
	 * \endcode
	 */
	virtual void SetCueById(CriUint32 cue_id, CriError &err = criErr::ErrorContainer) = 0;
	/*JP
	 * \brief キューIDによるキューの設定
	 * \param cue_id キューID<br>
	 * \param cue_sheet キューシートハンドル<br>
	 * \param err エラーコード<br>
	 * \par 説明:
	 * プレーヤにキューIDを指定してキューを設定します。<br>
	 * この関数が実行されると、キューにリンクしているシンセサイザが作成されます。
	 * \code
	 * CriError err;
	 * CriAuCueSheet* cue_sheet;
	 * CriUint32 cue_id = 10;
	 * // Set cue to player
	 * auply->SetCueById(cue_id, cue_sheet, err);
	 * \endcode
	 */
	/*EN
	 * \brief Set a cue to the player by Cue ID
	 * \param cue_id Cue ID
	 * \param err CriError information
	 * \par Description:
	 * This function set a cue by specifying the cue ID and Cue Sheet, where the ID is a certain number
	 * for the cue assigned by a sound designer.
	 * A synthesizer linked to this cue is created when this function is called.
	 * \code
	 * CriError err;
	 * CriAuCueSheet* cue_sheet;
	 * CriUint32 cue_id = 10;
	 * // Set cue to player
	 * auply->SetCueById(cue_id, cue_sheet, err);
	 * \endcode
	 */
	virtual void SetCueById(CriUint32 cue_id, CriAuCueSheet* cue_sheet, CriError &err = criErr::ErrorContainer) = 0;
	/*JP
	 * \brief インデックスによるキューの設定
	 * \param idx キューシート内のインデックス番号<br>
	 * \param err エラーコード<br>
	 * \par 説明:
	 * プレーヤにキューシート内のインデックスを指定してキューを設定します。<br>
	 * この関数が実行されると、キューシートのidx番目にあるキューにリンクしているシンセサイザが作成されます。
	 * \code
	 * CriError err;
	 * CriUint32 idx = 10;
	 * // Set cue to player
	 * auply->SetCueByIndex(idx, err);
	 * \endcode
	 */
	/*EN
	 * \brief Set a cue to the player by Cue index
	 * \param idx Index in the cue sheet allocated to the player
	 * \param err CriError information
	 * \par Description:
	 * This function sets a cue by specifying an index number in the cue sheet.
	 * A synthesizer linked to the idx-th cue is created when this function is called.
	 * \code
	 * CriError err;
	 * CriUint32 idx = 10;
	 * // Set cue to player
	 * auply->SetCueByIndex(idx, err);
	 * \endcode
	 */
	virtual void SetCueByIndex(CriUint32 idx, CriError &err = criErr::ErrorContainer) = 0;
	/*JP
	 * \brief インデックスによるキューの設定
	 * \param idx キューシート内のインデックス番号<br>
	 * \param cue_sheet キューシートハンドル<br>
	 * \param err エラーコード<br>
	 * \par 説明:
	 * プレーヤにキューシート内のインデックスを指定してキューを設定します。<br>
	 * この関数が実行されると、キューシートのidx番目にあるキューにリンクしているシンセサイザが作成されます。
	 * \code
	 * CriError err;
	 * CriAuCueSheet* cue_sheet;
	 * CriUint32 idx = 10;
	 * // Set cue to player
	 * auply->SetCueByIndex(idx, cue_sheet, err);
	 * \endcode
	 */
	/*EN
	 * \brief Set a cue to the player by Cue index and Cue Sheet
	 * \param idx Index in the cue sheet allocated to the player
	 * \param err CriError information
	 * \par Description:
	 * This function sets a cue by specifying an index number in the specified cue sheet.
	 * A synthesizer linked to the idx-th cue is created when this function is called.
	 * \code
	 * CriError err;
	 * CriAuCueSheet* cue_sheet;
	 * CriUint32 idx = 10;
	 * // Set cue to player
	 * auply->SetCueByIndex(idx, cue_sheet, err);
	 * \endcode
	 */
	virtual void SetCueByIndex(CriUint32 idx, CriAuCueSheet* cue_sheet, CriError &err = criErr::ErrorContainer) = 0;
	/*JP
	 * \brief キューの解放
	 * \par 説明:
	 * プレーヤに設定されているキューを解放します。
	 * プレーヤの持つシンセサイザを破棄します。
	 * この関数の名前は将来変更される予定です。
	 * \code
	 * CriError err;
	 * // Release cue of a player
	 * auply->ReleaseCue(err);
	 * \endcode
	 */
	/*EN
	 * \brief Release cue
	 * \par Description:
	 * This function releases the cue set to the player
	 * and discards all synthesizers connected to the player.
	 * The function name will be changed in the near future.
	 * \code
	 * CriError err;
	 * // Release cue of a player
	 * auply->ReleaseCue(err);
	 * \endcode
	 */
	virtual void ReleaseCue(CriError &err = criErr::ErrorContainer) = 0;
	/*JP
	 * \brief 再生パラメータのリセット
	 * \par 説明:
	 * プレーヤに設定されているボリュームやピッチなどの再生パラメータを初期状態に戻します。
	 * \code
	 * CriError err;
	 * // Reset parameters of player
	 * auply->ResetParameters(err);
	 * \endcode
	 */
	/*EN
	 * \brief Reset playing parameters
	 * \par Description:
	 * This function resets all parameters such as volume, pitch, etc.
	 * \code
	 * CriError err;
	 * // Reset parameters of player
	 * auply->ResetParameters(err);
	 * \endcode
	 */
	virtual void ResetParameters(CriError &err = criErr::ErrorContainer) = 0;
	/*JP
	 * \brief Cueパラメータのリセット
	 * \par 説明:
	 * プレーヤに設定されているCueの再生パラメータを初期状態に戻します。
	 * \code
	 * CriError err;
	 * // Reset parameters of a cue set by a player.
	 * auply->ResetCueParameters(err);
	 * \endcode
	 */
	/*EN
	 * \brief Reset playing parameters
	 * \par Description:
	 * This function resets parameters of cue.
	 * \code
	 * CriError err;
	 * // Reset parameters of a cue set by a player.
	 * auply->ResetCueParameters(err);
	 * \endcode
	 */
	virtual void ResetCueParameters(CriError &err = criErr::ErrorContainer) = 0;
	/*JP
	 * \brief 再生の実行
	 * \param	err エラーコード
	 * \par 説明:
	 * プレーヤに設定されているシンセサイザを再生します。
	 * \code
	 * CriError err;
	 * auply->SetCue(cue_name, err);
	 * // Play sounds
	 * auply->Play(err);
	 * \endcode
	 */
	/*EN
	 * \brief Play sound
	 * \param err CriError information
	 * \par Description:
	 * This function play sounds with synthesizers connected to the player.
	 * \code
	 * CriError err;
	 * auply->SetCue(cue_name, err);
	 * // Play sounds
	 * auply->Play(err);
	 * \endcode
	 */
	virtual void Play(CriError &err = criErr::ErrorContainer) = 0;
	/*JP
	 * \brief 再生中のボイスパラメータの更新
	 * \param	err エラーコード
	 * \par 説明:
	 * 再生中のボイスパラメータを更新します。
	 * \code
	 * CriError err;
	 * auply->SetVolume(0.5f, err);
	 * // Update voice parameters
	 * auply->Update(err);
	 * \endcode
	 */
	/*EN
	 * \brief Update voice parameters while playing
	 * \param err CriError information
	 * \par Description:
	 * This function updates voice parameters of currently playing sounds.
	 * \code
	 * CriError err;
	 * auply->SetVolume(0.5f, err);
	 * // Update voice parameters
	 * auply->Update(err);
	 * \endcode
	 */
	virtual void Update(CriError &err = criErr::ErrorContainer) = 0;
	/*JP
	 * \brief 再生停止
	 * \param	err エラーコード
	 * \par 説明:
	 * プレーヤで再生中のボイスを停止します。<br>
	 * リリースタイムが設定されているボイスの場合は、リリース状態になります。
	 * \code
	 * CriError err;
	 * // Stop Playing
	 * auply->Stop(err);
	 * \endcode
	 */
	/*EN
	 * \brief Stop playing
	 * \param err CriError information
	 * \par Description:
	 * This function stops the sound which is playing via the player.
	 * If the sound has release time, the sound changes to released status.
	 * \code
	 * CriError err;
	 * // Stop Playing
	 * auply->Stop(err);
	 * \endcode
	 */
	virtual void Stop(CriError &err = criErr::ErrorContainer) = 0;
	/*JP
	 * \brief 再生停止
	 * \param	stop_mode 停止方法
	 * \param	err エラーコード
	 * \par 説明:
	 * プレーヤで再生中のボイスを停止します。停止方法を指定できます。
	 * \code
	 * CriError err;
	 * CriAuPlayer::StopMode stop_mode = CriAuPlayer::STOP_MODE_RELEASE;
	 * // Stop Playing
	 * auply->Stop(stop_mode, err);
	 * \endcode
	 * \sa CriAuPlayer::StopMode
	 */
	/*EN
	 * \brief Stop playing
	 * \param stop_mode Stop mode
	 * \param err CriError information
	 * \par Description:
	 * This function stops the sound which is playing via the player, using the specified stop mode.
	 * \code
	 * CriError err;
	 * CriAuPlayer::StopMode stop_mode = CriAuPlayer::STOP_MODE_RELEASE;
	 * // Stop Playing
	 * auply->Stop(stop_mode, err);
	 * \endcode
	 * \sa CriAuPlayer::StopMode
	 */
	virtual void Stop(StopMode stop_mode, CriError &err = criErr::ErrorContainer) = 0;
	/*JP
	 * \brief	一時停止フラグの設定
	 * \param flag 一時停止フラグ<br>
	 * \param err エラーコード<br>
	 * \return	なし
	 * \par 説明:
	 *	一時停止フラグを設定します。<BR>
	 *	TRUE : 一時停止。<br>
	 *	FALSE : 一時停止解除。<br>
	 * \code
	 * CriError err;
	 * CriBool flag = TURE;
	 * // Set flag of pause
	 * auply->Pause(flag, err);
	 * \endcode
	 */
	/*EN
	 * \brief	Set pause flag
	 * \param flag Pause flag
	 * \param err CriError information
	 * \par Description:
	 *	This function sets the pause flag to the player.<br>
	 *	TRUE: Pause/FALSE: Resume
	 * \code
	 * CriError err;
	 * CriBool flag = TURE;
	 * // Set flag of pause
	 * auply->Pause(flag, err);
	 * \endcode
	 */
	virtual void Pause(CriBool flag, CriError &err = criErr::ErrorContainer) = 0;
	/*JP
	 * \brief	一時停止状態の取得
	 * \param err エラーコード<br>
	 * \return	一時停止状態
	 * \par 説明:
	 *	一時停止状態を取得します。<BR>
	 *	TRUE : 一時停止状態。<br>
	 *	FALSE : 一時停止解除状態。<br>
	 * \code
	 * CriError err;
	 * // Get flag of pause
	 * CriBool flag = auply->IsPaused(err);
	 * \endcode
	 */
	/*EN
	 * \brief	Retrieve pause status
	 * \param err CriError information
	 * \par Description:
	 *	This function returns the pause status to the player.<br>
	 *	TRUE: Status is Pause.
	 *  FALSE: Status is Resume.
	 * \code
	 * CriError err;
	 * // Get flag of pause
	 * CriBool flag = auply->IsPaused(err);
	 * \endcode
	 */
	virtual CriBool IsPaused(CriError &err = criErr::ErrorContainer) = 0;
	/*JP
	 * \brief	ボリュームの設定
	 * \param volume ボリューム<br>
	 * \param err エラーコード<br>
	 * \return	なし
	 * \par 説明:
	 *	ボリューム値を設定します。<BR>
	 *	単位はリニアスケールです。<br>
	 *	この関数を実行しただけでは、再生中の音声は変化しません。<br>
	 *	Update関数を実行すると、実際に変更が反映されます。<br>
	 * \code
	 * CriError err;
	 * CriFloat32 volume = 0.5f;
	 * // Set volume
	 * auply->SetVolume(volume, err);
	 * // Update voice parameters
	 * auply->Update(err);
	 * \endcode
	 * \sa \ref cria_intro_what_is_voice_control
	 */
	/*EN
	 * \brief	Set volume
	 * \param volume Volume
	 * \param err CriError information
	 * \par Description:
	 *	This function sets the volume to the player.
	 *	The volume value is specified in linear scale.
	 *	This function does NOT change volume of the sound immediately.
	 *	An Update() call is required to validate the volume change.
	 * \code
	 * CriError err;
	 * CriFloat32 volume = 0.5f;
	 * // Set volume
	 * auply->SetVolume(volume, err);
	 * // Update voice parameters
	 * auply->Update(err);
	 * \endcode
	 * \sa \ref cria_intro_what_is_voice_control
	 */
	virtual void SetVolume(CriFloat32 volume, CriError &err = criErr::ErrorContainer) = 0;
	/*JP
	 * \brief	ボリュームの設定
	 * \param volume_offset ボリュームのオフセット<br>
	 * \param volume_gain ボリュームのゲイン<br>
	 * \param err エラーコード<br>
	 * \return	なし
	 * \par 説明:
	 *	ボリューム値をオフセットとゲインで設定します。<BR>
	 *	単位はリニアスケールです。<br>
	 *	この関数を実行しただけでは、再生中の音声は変化しません。<br>
	 *	Update関数を実行すると、実際に変更が反映されます。<br>
	 * \code
	 * CriError err;
	 * CriFloat32 volume_offset = 0.1f;
	 * CriFloat32 volume_gain = 0.5f;
	 * // Set volume
	 * auply->SetVolume(volume_offset, volume_gain, err);
	 * // Update voice parameters
	 * auply->Update(err);
	 * \endcode
	 * \sa \ref cria_intro_what_is_voice_control
	 */
	/*EN
	 * \brief	Set volume
	 * \param volume_offset Offset of volume
	 * \param volume_gain Gain of volume
	 * \param err CriError information
	 * \par Description:
	 *	This function sets the volume to the player using offset and gain.
	 *	The volume value is specified in linear scale.
	 *	This function does NOT change volume of the sound immediately.
	 *	An Update() call is required to validate the volume change.
	 * \code
	 * CriError err;
	 * CriFloat32 volume_offset = 0.1f;
	 * CriFloat32 volume_gain = 0.5f;
	 * // Set volume
	 * auply->SetVolume(volume_offset, volume_gain, err);
	 * // Update voice parameters
	 * auply->Update(err);
	 * \endcode
	 * \sa \ref cria_intro_what_is_voice_control
	 */
	virtual void SetVolume(CriFloat32 volume_offset, CriFloat32 volume_gain, CriError &err = criErr::ErrorContainer) = 0;
	/*JP
	 * \brief ピッチの設定
	 * \param pitch ピッチの変更値（単位：セント）<br>
	 * \param err エラーコード<br>
	 * \return	なし
	 * \par 説明:
	 *	ピッチの変更値を設定します。<BR>
	 *	単位はセントです。<br>
	 *	この関数を実行しただけでは、再生中の音声は変化しません。<br>
	 *	Update関数を実行すると、実際に変更が反映されます。<br>
	 * \code
	 * CriError err;
	 * CriFloat32 pitch = 100.0f;
	 * // Set pitch
	 * auply->SetPitch(pitch, err);
	 * // Update voice parameters
	 * auply->Update(err);
	 * \endcode
	 * \sa \ref cria_intro_what_is_voice_control
	 */
	/*EN
	 * \brief Set pitch
	 * \param pitch Pitch difference
	 * \param err CriError information
	 * \par Description:
	 *	This function sets pitch difference in CENT.
	 *	This function does NOT change pitch of the sound immediately.
	 *	An Update() call is required to validate the pitch change.
	 * \code
	 * CriError err;
	 * CriFloat32 pitch = 100.0f;
	 * // Set pitch
	 * auply->SetPitch(pitch, err);
	 * // Update voice parameters
	 * auply->Update(err);
	 * \endcode
	 * \sa \ref cria_intro_what_is_voice_control
	 */
	virtual void SetPitch(CriFloat32 pitch, CriError &err = criErr::ErrorContainer) = 0;
	/*JP
	 * \brief ピッチの設定
	 * \param pitch_offset ピッチの変更値のオフセット（単位：セント）<br>
	 * \param pitch_gain ピッチの変更値のゲイン<br>
	 * \param err エラーコード<br>
	 * \return	なし
	 * \par 説明:
	 *	ピッチの変更値をオフセットとゲインで設定します。<BR>
	 *	単位はセントです。<br>
	 *	この関数を実行しただけでは、再生中の音声は変化しません。<br>
	 *	Update関数を実行すると、実際に変更が反映されます。<br>
	 * \code
	 * CriError err;
	 * CriFloat32 pitch_offset = 100.0f;
	 * CriFloat32 pitch_gain = 0.5f;
	 * // Set pitch
	 * auply->SetPitch(pitch_offset, pitch_gain, err);
	 * // Update voice parameters
	 * auply->Update(err);
	 * \endcode
	 * \sa \ref cria_intro_what_is_voice_control
	 */
	/*EN
	 * \brief Set pitch
	 * \param pitch_offset Offset of pitch difference
	 * \param pitch_gain Gain of pitch difference
	 * \param err CriError information
	 * \par Description:
	 *	This function sets pitch difference in CENT using offset and gain.
	 *	This function does NOT change pitch of the sound immediately.
	 *	An Update() call is required to validate the pitch change.
	 * \code
	 * CriError err;
	 * CriFloat32 pitch_offset = 100.0f;
	 * CriFloat32 pitch_gain = 0.5f;
	 * // Set pitch
	 * auply->SetPitch(pitch_offset, pitch_gain, err);
	 * // Update voice parameters
	 * auply->Update(err);
	 * \endcode
	 * \sa \ref cria_intro_what_is_voice_control
	 */
	virtual void SetPitch(CriFloat32 pitch_offset, CriFloat32 pitch_gain, CriError &err = criErr::ErrorContainer) = 0;
	/*JP
	 * \brief	カットオフ周波数の設定
	 * \param cof_low 低域カットオフ周波数 (Hz)<br>
	 * \param cof_high 高域カットオフ周波数 (Hz)<br>
	 * \param err エラーコード<br>
	 * \return	なし
	 * \par 説明:
	 *	カットオフ周波数を設定します。<BR>
	 *	cof_lowからcof_highまでが通過帯域になります。<BR>
	 *	cof_lowとcof_highの両方を0.0fにするとフィルタ処理しません。<BR>
	 *  単位はヘルツです。<br>
	 *	この関数を実行しただけでは、再生中の音声は変化しません。<br>
	 *	Update関数を実行すると、実際に変更が反映されます。<br>
	 * \code
	 * CriError err;
	 * CriFloat32 cof_low = 0.0f, cof_high = 100000.0f;
	 * // Set cut-off frequency
	 * auply->SetFilterCutoffFrequency(cof_low, cof_high, err);
	 * // Update voice parameters
	 * auply->Update(err);
	 * \endcode
	 * \sa \ref cria_intro_what_is_voice_control
	 */
	/*EN
	 * \brief Set cut-off frequency
	 * \param cof_low Lower cut-off frequency (Hz)
	 * \param cof_high Higher cut-off frequency (Hz)
	 * \param err CriError information
	 * \par Description:
	 *	This function sets cut-off frequencies in Hz.
	 *	The range between cof_low and cof_high is the pass band.
	 *	If both cof_low and cof_high are set to 0.0f, the filter won't be applied.
	 *	This function does NOT change the sound immediately.
	 *	An Update() call is required to validate the change.
	 * \code
	 * CriError err;
	 * CriFloat32 cof_low = 0.0f, cof_high = 100000.0f;
	 * // Set cut-off frequency
	 * auply->SetFilterCutoffFrequency(cof_low, cof_high, err);
	 * // Update voice parameters
	 * auply->Update(err);
	 * \endcode
	 * \sa \ref cria_intro_what_is_voice_control
	 */
	virtual void SetFilterCutoffFrequency(CriFloat32 cof_low, CriFloat32 cof_high, CriError &err = criErr::ErrorContainer) = 0;
	virtual void SetFilterCutoffLow(CriFloat32 cofl_offset, CriFloat32 cofl_gain, CriError &err = criErr::ErrorContainer) = 0;
	virtual void SetFilterCutoffHigh(CriFloat32 cofh_offset, CriFloat32 cofh_gain, CriError &err = criErr::ErrorContainer) = 0;
	/*JP
	 * \brief ドライセンドレベルの設定
	 * \param lvl ドライセンドレベル<br>
	 * \param err エラーコード<br>
	 * \return	なし
	 * \par 説明:
	 *	ドライセンドレベルを設定します。<BR>
	 *	この関数を実行しただけでは、再生中の音声は変化しません。<br>
	 *	Update関数を実行すると、実際に変更が反映されます。<br>
	 * \code
	 * CriError err;
	 * CriAuSendLevel drylvl(0.0f);
	 * drylvl.level[CriAuSendLevel::DRY_C] = 1.0f;
	 * // Set dry send levels
	 * auply->SetDrySendLevel(drylvl, err);
	 * // Update voice parameters
	 * auply->Update(err);
	 * \endcode
	 * \sa CriAuSendLevel
	 * \sa \ref cria_intro_what_is_voice_control
	*/
	/*EN
	 * \brief Set dry send levels.
	 * \param lvl Dry send levels
	 * \param err CriError information
	 * \par Description:
	 *	This function sets dry send levels.
	 *	This function does NOT change the sound immediately.
	 *	An Update() call is required to validate the change.
	 * \code
	 * CriError err;
	 * CriAuSendLevel drylvl(0.0f);
	 * drylvl.level[CriAuSendLevel::DRY_C] = 1.0f;
	 * // Set dry send levels
	 * auply->SetDrySendLevel(drylvl, err);
	 * // Update voice parameters
	 * auply->Update(err);
	 * \endcode
	 * \sa CriAuSendLevel
	 * \sa \ref cria_intro_what_is_voice_control
	 */
	virtual void SetDrySendLevel(const CriAuSendLevel &lvl, CriError &err = criErr::ErrorContainer) = 0;
	/*JP
	 * \brief ドライセンドレベルの設定
	 * \param offset ドライセンドレベルのオフセット<br>
	 * \param gain ドライセンドレベルのゲイン<br>
	 * \param err エラーコード<br>
	 * \return	なし
	 * \par 説明:
	 *	ドライセンドレベルをオフセットとゲインで設定します。<BR>
	 *	この関数を実行しただけでは、再生中の音声は変化しません。<br>
	 *	Update関数を実行すると、実際に変更が反映されます。<br>
	 * \code
	 * CriError err;
	 * CriAuSendLevel drylvl_offset(0.0f);
	 * CriAuSendLevel drylvl_gain(0.0f);
	 * drylvl_offset.level[CriAuSendLevel::DRY_C] = 1.0f;
	 * // Set dry send levels
	 * auply->SetDrySendLevel(drylvl_offset, drylvl_gain, err);
	 * // Update voice parameters
	 * auply->Update(err);
	 * \endcode
	 * \sa CriAuSendLevel
	 * \sa \ref cria_intro_what_is_voice_control
	*/
	/*EN
	 * \brief Set dry send levels.
	 * \param offset Offset of Dry send levels
	 * \param gain Gain of Dry send levels
	 * \param err CriError information
	 * \par Description:
	 *	This function sets dry send levels using offset and gain.
	 *	This function does NOT change the sound immediately.
	 *	An Update() call is required to validate the change.
	 * \code
	 * CriError err;
	 * CriAuSendLevel drylvl_offset(0.0f);
	 * CriAuSendLevel drylvl_gain(0.0f);
	 * drylvl_offset.level[CriAuSendLevel::DRY_C] = 1.0f;
	 * // Set dry send levels
	 * auply->SetDrySendLevel(drylvl_offset, drylvl_gain, err);
	 * // Update voice parameters
	 * auply->Update(err);
	 * \endcode
	 * \sa CriAuSendLevel
	 * \sa \ref cria_intro_what_is_voice_control
	 */
	virtual void SetDrySendLevel(const CriAuSendLevel &offset, const CriAuSendLevel &gain, CriError &err = criErr::ErrorContainer) = 0;
	/*JP
	 * \brief ドライセンドレベルの設定
	 * \param ch チャンネル番号
	 * \param level ドライセンドレベル<br>
	 * \param err エラーコード<br>
	 * \return	なし
	 * \par 説明:
	 *	ドライセンドレベルを設定します。<BR>
	 *	この関数を実行しただけでは、再生中の音声は変化しません。<br>
	 *	Update関数を実行すると、実際に変更が反映されます。<br>
	 * \code
	 * CriError err;
	 * // Set dry send level
	 * auply->SetDrySendLevel(CriAuSendLevel::DRY_L, 0.5f, err);
	 * // Update voice parameters
	 * auply->Update(err);
	 * \endcode
	 * \sa CriAuSendLevel
	 * \sa \ref cria_intro_what_is_voice_control
	 */
	/*EN
	 * \brief Set dry send level.
	 * \param ch Channel number
	 * \param level Dry send level value
	 * \param err CriError information
	 * \par Description:
	 *	This function sets a dry send level in a linear scale.
	 *	This function does NOT change the sound immediately.
	 *	An Update() call is required to validate the change.
	 * \code
	 * CriError err;
	 * // Set dry send level
	 * auply->SetDrySendLevel(CriAuSendLevel::DRY_L, 0.5f, err);
	 * // Update voice parameters
	 * auply->Update(err);
	 * \endcode
	 * \sa CriAuSendLevel
	 * \sa \ref cria_intro_what_is_voice_control
	 */
	virtual void SetDrySendLevel(CriUint32 ch, CriFloat32 level, CriError &err = criErr::ErrorContainer) = 0;
	/*JP
	 * \brief ドライセンドレベルの設定
	 * \param ch チャンネル番号
	 * \param offset ドライセンドレベルのオフセット<br>
	 * \param gain ドライセンドレベルのゲイン<br>
	 * \param err エラーコード<br>
	 * \return	なし
	 * \par 説明:
	 *	ドライセンドレベルをオフセットとゲインで設定します。<BR>
	 *	この関数を実行しただけでは、再生中の音声は変化しません。<br>
	 *	Update関数を実行すると、実際に変更が反映されます。<br>
	 * \code
	 * CriError err;
	 * // Set dry send level
	 * auply->SetDrySendLevel(CriAuSendLevel::DRY_L, 0.5f, 0.0f, err);	
	 * // Update voice parameters
	 * auply->Update(err);
	 * \endcode
	 * \sa CriAuSendLevel
	 * \sa \ref cria_intro_what_is_voice_control
	 */
	/*EN
	 * \brief Set dry send level.
	 * \param ch Channel number
	 * \param offset Offset of Dry send level value
	 * \param gain Gain of Dry send level value
	 * \param err CriError information
	 * \par Description:
	 *	This function sets a dry send level in a linear scale using offset and gain.
	 *	This function does NOT change the sound immediately.
	 *	An Update() call is required to validate the change.
	 * \code
	 * CriError err;
	 * // Set dry send level
	 * auply->SetDrySendLevel(CriAuSendLevel::DRY_L, 0.5f, 0.0f, err);
	 * // Update voice parameters
	 * auply->Update(err);
	 * \endcode
	 * \endcode
	 * \sa CriAuSendLevel
	 * \sa \ref cria_intro_what_is_voice_control
	 */
	virtual void SetDrySendLevel(CriUint32 ch, CriFloat32 level_offset, CriFloat32 level_gain, CriError &err = criErr::ErrorContainer) = 0;
	/*JP
	 * \brief ウェットセンドレベルの設定
	 * \param line_no ライン番号
	 * \param level ウェットセンドレベル<br>
	 * \param err エラーコード<br>
	 * \return	なし
	 * \par 説明:
	 *	ウェットセンドレベルを設定します。<BR>
	 *	この関数を実行しただけでは、再生中の音声は変化しません。<br>
	 *	Update関数を実行すると、実際に変更が反映されます。<br>
	 * \code
	 * CriError err;
	 * // Set wet send level
	 * auply->SetWetSendLevel(CriAuSendLevel::WET_0, 0.5f, err);	//	Reverb
	 * // Update voice parameters
	 * auply->Update(err);
	 * \endcode
	 * \sa CriAuSendLevel
	 * \sa \ref cria_intro_what_is_voice_control
	 */
	/*EN
	 * \brief Set wet send level.
	 * \param line_no Line number
	 * \param level Wet send level value
	 * \param err CriError information
	 * \par Description:
	 *	This function sets a wet send level in a linear scale.
	 *	This function does NOT change the sound immediately.
	 *	An Update() call is required to validate the change.
	 * \code
	 * CriError err;
	 * // Set wet send level
	 * auply->SetWetSendLevel(CriAuSendLevel::WET_0, 0.5f, err);	//	Reverb
	 * // Update voice parameters
	 * auply->Update(err);
	 * \endcode
	 * \sa CriAuSendLevel
	 * \sa \ref cria_intro_what_is_voice_control
	 */
	virtual void SetWetSendLevel(CriUint32 line_no, CriFloat32 level, CriError &err = criErr::ErrorContainer) = 0;
	/*JP
	 * \brief ウェットセンドレベルの設定
	 * \param line_no ライン番号
	 * \param offset ウェットセンドレベルのオフセット<br>
	 * \param gain ウェットセンドレベルのゲイン<br>
	 * \param err エラーコード<br>
	 * \return	なし
	 * \par 説明:
	 *	ウェットセンドレベルをオフセットとゲインで設定します。<BR>
	 *	この関数を実行しただけでは、再生中の音声は変化しません。<br>
	 *	Update関数を実行すると、実際に変更が反映されます。<br>
	 * \code
	 * CriError err;
	 * // Set wet send level
	 * auply->SetWetSendLevel(CriAuSendLevel::WET_0, 0.5f, 0.0f, err);	//	Reverb
	 * // Update voice parameters
	 * auply->Update(err);
	 * \endcode
	 * \sa CriAuSendLevel
	 * \sa \ref cria_intro_what_is_voice_control
	 */
	/*EN
	 * \brief Set wet send level.
	 * \param line_no Line number
	 * \param offset Offset of Wet send level value
	 * \param gain Gain of Wet send level value
	 * \param err CriError information
	 * \par Description:
	 *	This function sets a wet send level in a linear scale using offset and gain.
	 *	This function does NOT change the sound immediately.
	 *	An Update() call is required to validate the change.
	 * \code
	 * CriError err;
	 * // Set wet send level
	 * auply->SetWetSendLevel(CriAuSendLevel::WET_0, 0.5f, 0.0f, err);	//	Reverb
	 * // Update voice parameters
	 * auply->Update(err);
	 * \endcode
	 * \endcode
	 * \sa CriAuSendLevel
	 * \sa \ref cria_intro_what_is_voice_control
	 */
	virtual void SetWetSendLevel(CriUint32 line_no, CriFloat32 level_offset, CriFloat32 level_gain, CriError &err = criErr::ErrorContainer) = 0;

	/*JP
	 * \brief パンニング3Dの角度の設定
	 * \param ang_ofst 角度のオフセット
	 * \param ang_gain 角度のゲイン
	 * \param err エラーコード<br>
	 * \return	なし
	 * \par 説明:
	 *	パンニング3Dの角度をオフセットとゲインで設定します。単位は度です。<BR>
	 *	この関数を実行しただけでは、再生中の音声は変化しません。<br>
	 *	Update関数を実行すると、実際に変更が反映されます。<br>
	 * \code
	 * CriError err;
	 * CriFloat32 ang_ofst = 180.0f, ang_gain = 0.0f;
	 * // Set panning 3D angle
	 * auply->SetPan3dAngle(ang_ofst, ang_gain, err);
	 * // Update voice parameters
	 * auply->Update(err);
	 * \endcode
	 * \sa \ref cria_intro_what_is_voice_control
	 */
	/*EN
	 * \brief Set panning 3D angle.
	 * \param ang_ofst Offset of angle
	 * \param ang_gain Gain of angle
	 * \param err CriError information
	 * \par Description:
	 *	This function sets a panning 3D angle in degrees using offset and gain.
	 *	This function does NOT change the sound immediately.
	 *	An Update() call is required to validate the change.
	 * \code
	 * CriError err;
	 * CriFloat32 ang_ofst = 180.0f, ang_gain = 0.0f;
	 * // Set panning 3D angle
	 * auply->SetPan3dAngle(ang_ofst, ang_gain, err);
	 * // Update voice parameters
	 * auply->Update(err);
	 * \endcode
	 * \sa \ref cria_intro_what_is_voice_control
	 */
	virtual void SetPan3dAngle(CriFloat32 ang_ofst, CriFloat32 ang_gain, CriError &err = criErr::ErrorContainer) = 0;
	/*JP
	 * \brief パンニング3Dの距離の設定
	 * \param idist_ofst 距離のオフセット
	 * \param idist_gain 距離のゲイン
	 * \param err エラーコード<br>
	 * \return	なし
	 * \par 説明:
	 *	インテリアパンニングを行う際の距離をオフセットとゲインで設定します。<BR>
	 *	距離は、リスナー位置を0.0、スピーカー位置を1.0として指定します。<br>
	 *	この関数を実行しただけでは、再生中の音声は変化しません。<br>
	 *	Update関数を実行すると、実際に変更が反映されます。<br>
	 * \code
	 * CriError err;
	 * CriFloat32 idist_ofst = 1.0f, idist_gain = 0.0f;
	 * // Set panning 3D distance
	 * auply->SetPan3dInteriorDistance(idist_ofst, idist_gain, err);
	 * // Update voice parameters
	 * auply->Update(err);
	 * \endcode
	 * \sa \ref cria_intro_what_is_voice_control
	 */
	/*EN
	 * \brief Set panning 3D distance.
	 * \param ang_ofst Offset of distance
	 * \param ang_gain Gain of distance
	 * \param err CriError information
	 * \par Description:
	 *	This function sets the distance used for interior panning using offset and gain.
	 *  A distance is specified between 0.0(a listener position) and 1.0(a speaker position).
	 *	This function does NOT change the sound immediately.
	 *	An Update() call is required to validate the change.
	 * \code
	 * CriError err;
	 * CriFloat32 idist_ofst = 1.0f, idist_gain = 0.0f;
	 * // Set panning 3D distance
	 * auply->SetPan3dInteriorDistance(idist_ofst, idist_gain, err);
	 * // Update voice parameters
	 * auply->Update(err);
	 * \endcode
	 * \sa \ref cria_intro_what_is_voice_control
	 */
	virtual void SetPan3dInteriorDistance(CriFloat32 idist_ofst, CriFloat32 idist_gain, CriError &err = criErr::ErrorContainer) = 0;
	/*JP
	 * \brief パンニング3Dのボリュームの設定
	 * \param ang_ofst ボリュームのオフセット
	 * \param ang_gain ボリュームのゲイン
	 * \param err エラーコード<br>
	 * \return	なし
	 * \par 説明:
	 *	パンニング3Dのボリュームをオフセットとゲインで設定します。<BR>
	 *	この関数を実行しただけでは、再生中の音声は変化しません。<br>
	 *	Update関数を実行すると、実際に変更が反映されます。<br>
	 * \code
	 * CriError err;
	 * CriFloat32 vol_ofst = 0.8f, vol_gain = 0.0f;
	 * // Set panning 3D volume
	 * auply->SetPan3dVolume(vol_ofst, vol_gain, err);
	 * // Update voice parameters
	 * auply->Update(err);
	 * \endcode
	 * \sa \ref cria_intro_what_is_voice_control
	 */
	/*EN
	 * \brief Set panning 3D volume.
	 * \param ang_ofst Offset of volume
	 * \param ang_gain Gain of volume
	 * \param err CriError information
	 * \par Description:
	 *	This function sets panning 3D volume using offset and gain.
	 *	This function does NOT change the sound immediately.
	 *	An Update() call is required to validate the change.
	 * \code
	 * CriError err;
	 * CriFloat32 vol_ofst = 0.8f, vol_gain = 0.0f;
	 * // Set panning 3D volume
	 * auply->SetPan3dVolume(vol_ofst, vol_gain, err);
	 * // Update voice parameters
	 * auply->Update(err);
	 * \endcode
	 * \sa \ref cria_intro_what_is_voice_control
	 */
	virtual void SetPan3dVolume(CriFloat32 vol_ofst, CriFloat32 vol_gain, CriError &err = criErr::ErrorContainer) = 0;
 	
 	/*JP
	 * \brief プライオリティの設定
	 * \param prioirty_offset プライオリティの変更値<br>
	 * \param err エラーコード<br>
	 * \return	なし
	 * \par 説明:
	 *	プライオリティの変更値を設定します。<BR>
	 *	この関数を実行しただけでは、再生中の音声のプライオリティは変化しません。<br>
	 *	Update関数を実行すると、実際に変更が反映されます。<br>
	 * \code
	 * CriError err;
	 * CriSint32 prioirty_offset = 10;
	 * // Set priority
	 * auply->SetPriority(prioirty_offset, err);
	 * // Update voice parameters
	 * auply->Update(err);
	 * \endcode
	 * \sa \ref cria_intro_what_is_voice_control
	 */
	/*EN
	 * \brief Set priority
	 * \param prioirty_offset Priority difference
	 * \param err CriError information
	 * \par Description:
	 *	This function sets priority difference.
	 *	This function does NOT change priority of the playing sound immediately.
	 *	An Update() call is required to validate the priority change.
	 * \code
	 * CriSint32 prioirty_offset = 10;
	 * // Set priority
	 * auply->SetPriority(prioirty_offset, err);
	 * // Update voice parameters
	 * auply->Update(err);
	 * \endcode
	 * \sa \ref cria_intro_what_is_voice_control
	 */
	virtual void SetPriority(CriSint32 prioirty_offset, CriError &err = criErr::ErrorContainer) = 0;	
	/*JP
	 * \brief アイザックのコントロール値の設定
	 * \param control_name コントロール名
	 * \param control_value コントロール値
	 * \param err エラーコード
	 * \par 説明:
	 * 指定したコントロール名を持つアイザックにコントロール値を設定します。
	 * \code
	 * CriError err;
	 * const CriChar8* control_name = "RotationSpeed";
	 * CriFloat32 control_value = 0.5f;
	 * //	Set the control value to the "RotationSpeed" AISAC of the player
	 * auply->SetAisac(control_name, control_value, err);
	 * // Update voice parameters
	 * auply->Update(err);
	 * \endcode
	 * \sa \ref cria_intro_what_is_voice_control
	 */
	/*EN
	 * \brief Set AISAC control value
	 * \param control_name A control name
	 * \param control_value A control value
	 * \param err CriError information
	 * \par Description:
	 * This function sets an control value for AISACs that have a specified control name.
	 * \code
	 * CriError err;
	 * const CriChar8* control_name = "RotationSpeed";
	 * CriFloat32 control_value = 0.5f;
	 * //	Set the control value to the "RotationSpeed" AISAC of the player
	 * auply->SetAisac(control_name, control_value, err);
	 * // Update voice parameters
	 * auply->Update(err);
	 * \endcode
	 * \sa \ref cria_intro_what_is_voice_control
	 */
	virtual void SetAisac(const CriChar8* control_name, CriFloat32 control_value, CriError &err = criErr::ErrorContainer) = 0;
	/*JP
	 * \brief アイザックのランダム範囲の設定
	 * \param offset ランダム範囲のオフセット
	 * \param gain ランダム範囲のゲイン
	 * \param err エラーコード
	 * \par 説明:
	 * 指定したコントロール名を持つアイザックのランダム範囲を設定します。<br>
	 * ランダム範囲の設定されているアイザックにコントロール値を設定した場合、以下の範囲で実際に適用される値が決定されます。<br>
	 * <br>
	 * <i>適用されるコントロール値 = (コントロール値 - ランダム範囲) ～ (コントロール値 + ランダム範囲)</i>
	 * \code
	 * CriError err;
	 * const CriChar8* control_name = "RotationSpeed";
	 * CriFloat32 offset = 0.5f, gain = 0.0f;
	 * // Set an AISAC random range to the "RotationSpeed" AISAC of the player
	 * auply->SetAisac(control_name, offset, gain, err);
	 * \endcode
	 * \sa SetAisac
	 * \sa \ref cria_intro_what_is_voice_control
	 */
	/*EN
	 * \brief Set AISAC random range
	 * \param control_name A control name
	 * \param offset An offset for random range
	 * \param gain A gain for random range
	 * \param err CriError information
	 * \par Description:
	 * This function sets a random range of control values for AISACs that have a specified control name.
	 * If you set a control value to an AISAC that has random range, a control value is decided in the range as follows.<br>
	 * <br>
	 * <i>applied_control_value = from (control_value - random_range) to (control_value + random_range)</i>
	 * \code
	 * CriError err;
	 * const CriChar8* control_name = "RotationSpeed";
	 * CriFloat32 offset = 0.5f, gain = 0.0f;
	 * // Set an AISAC random range to the "RotationSpeed" AISAC of the player
	 * auply->SetAisac(control_name, offset, gain, err);
	 * \endcode
	 * \sa SetAisac
	 * \sa \ref cria_intro_what_is_voice_control
	 */
	virtual void SetAisacRandomRange(const CriChar8 *control_name, CriFloat32 offset, CriFloat32 gain, CriError &err = criErr::ErrorContainer) = 0;
	/*JP
	 * \brief アイザックのコントロール値によるパラメータ値の取得
	 * \param control_name コントロール名
	 * \param type グラフタイプ
	 * \param err エラーコード
	 * \par 説明:
	 * 指定したコントロール名を持つアイザックの現在のコントロール値から算出されるパラメータ値を取得します。
	 * \code
	 * CriError err;
	 * const CriChar8* control_name = "RotationSpeed";
	 * //	Get the pitch value from the "RotationSpeed" AISAC of the player
	 * CriFloat32 aisac_pitch = auply->GetAisac(control_name, CriAu::AISAC_GRAPH_TYPE_PITCH, err);
	 * \endcode
	 * \sa \ref cria_intro_what_is_voice_control
	 */
	/*EN
	 * \brief Get parameter value from AISAC
	 * \param control_name A control name
	 * \param type graph type
	 * \param err CriError information
	 * \par Description:
	 * This function gets a parameter value from AISACs that have a specified control name.
	 * \code
	 * CriError err;
	 * const CriChar8* control_name = "RotationSpeed";
	 * //	Get the pitch value from the "RotationSpeed" AISAC of the player
	 * CriFloat32 aisac_pitch = auply->GetAisac(control_name, CriAu::AISAC_GRAPH_TYPE_PITCH, err);
	 * \endcode
	 * \sa \ref cria_intro_what_is_voice_control
	 */
	virtual CriFloat32 GetAisacValue(const CriChar8* control_name, CriAu::AisacGraphType type, CriError &err = criErr::ErrorContainer) = 0;
	/*JP
	 * \brief アイザックの個数の取得
	 * \param err エラーコード
	 * \par 説明:
	 * プレーヤからコントロール可能なアイザックの個数を取得します。
	 * \code
	 * CriError err;
	 * CriUint32 num_aisacs = auply->GetNumberOfAisacs(err);
	 * \endcode
	 */
	/*EN
	 * \brief Retrieve the number of AISACs in a player
	 * \param err CriError information
	 * \par Description:
	 * This function returns the number of AISACs in a player.
	 * \code
	 * CriError err;
	 * CriUint32 num_aisacs = auply->GetNumberOfAisacs(err);
	 * \endcode
	 */
	virtual CriUint32 GetNumberOfAisacs(CriError &err = criErr::ErrorContainer) = 0;
	/*JP
	 * \brief アイザックのコントロール名の取得
	 * \param no プレーヤからコントロール可能なアイザックのインデックス番号<br>
	 * \param err エラーコード<br>
	 * \par 説明:
	 * プレーヤからコントロール可能なアイザックのコントロール名を取得します。
	 * \code
	 * CriError err;
	 * CriUint32 no = 0;
	 * const CriChar8* control_name = auply->GetAisacControlName(no, err);
	 * \endcode
	 */
	/*EN
	 * \brief Retrieve an AISAC control name in a player
	 * \param no Index number of the AISAC in a player.
	 * \param err CriError information
	 * \par Description:
	 * This function returns the AISAC control name in a player.
	 * \code
	 * CriError err;
	 * CriUint32 no = 0;
	 * const CriChar8* control_name = auply->GetAisacControlName(no, err);
	 * \endcode
	 */
	virtual const CriChar8 *GetAisacControlName(CriUint32 no, CriError &err = criErr::ErrorContainer) = 0;
	/*JP
	 * \brief AISACの取り付け
	 * \param aisac_pattern_name 取り付けるAISACのパターン名
	 * \param slot_no プレーヤのAISACスロット番号
	 * \param err エラーコード
	 * \par 説明:
	 * 指定した名前のAISACをプレーヤに取り付けます。<br>
	 * AISACの検索は、プレーヤを作成したCRI Audio オブジェクトに登録されているキューシートから行われます。<br>
	 * プレーヤにはデフォルトで8個のAISACスロットが存在し、それぞれAISACを取り付けることができます。
	 * 何番目のAISACスロットにどのようなAISACを取り付けるかは、ユーザーが自由に決定します。
	 * 例えば、0番目のAISACスロットには「距離減衰」に関するAISACを取り付けることにして、
	 * シーン毎に差し替えていく、といったことができます。（距離減衰1→距離減衰2→距離減衰3…）<br>
	 * プレーヤに取り付けられたAISACは、キューに含まれるAISACと同様にコントロール可能です。
	 * プレーヤに取り付けられたAISACのコントロール値は0.0fで初期化されます。<br>
	 * すでにAISACが取り付けられているAISACスロットにもう一度AISACを取り付ける場合は、
	 * 先にCriAuPlayer::DetachAisac関数によりAISACの取り外しを行ってください。
	 * \code
	 * CriError err;
	 * const CriChar8* aisac_pattern_name = "DistanceDecay";
	 * CriUint32 slot_no = 0;
	 * // Attach the "DistanceDecay" AISAC to the player
	 * auply->AttachAisac(aisac_pattern_name, slot_no, err);
	 * \endcode
	 */
	/*EN
	 * \brief Attach AISAC to player
	 * \param aisac_pattern_name A pattern name of a AISAC to attach
	 * \param slot_no AISAC slot number of a player
	 * \param err CriError information
	 * \par Description:
	 * This function attaches an AISAC to a specified AISAC slot of a player.
	 * A search of the pattern name of an AISAC is done in the cue sheets that have been attached to
	 * the CRI Audio Object that created the player.
	 * A player has 8 AISAC slots in the default setting, and an AISAC can be attached to each slot.
	 * You can decide the type of AISAC you want to attach to any indexed AISAC slot.<br>
	 * For example, you can attach an AISAC relevant to "Distance Decay" to AISAC slot number 0.
	 * And you switch AISACs in accordance with each scene. ("Distance Decay1" -> "Distance Decay2" -> "Distance Decay3"...)<br>
	 * The AISACs that have been attached to a player can be controlled, as well as the AISACs in a cue.
	 * The control value of an AISAC to attach is initialized by 0.0f.
	 * To attach an AISAC to an AISAC slot that already has an AISAC,
	 * first detach the current AISAC using the CriAuPlayer::DetachAisac function.
	 * \code
	 * CriError err;
	 * const CriChar8* aisac_pattern_name = "DistanceDecay";
	 * CriUint32 slot_no = 0;
	 * // Attach the "DistanceDecay" AISAC to the player
	 * auply->AttachAisac(aisac_pattern_name, slot_no, err);
	 * \endcode
	 */
	virtual void AttachAisac(const CriChar8 *aisac_pattern_name, CriUint32 slot_no, CriError &err = criErr::ErrorContainer) = 0;
	/*JP
	 * \brief 特定のキューシート内のAISACの取り付け
	 * \param aisac_pattern_name 取り付けるAISACのパターン名
	 * \param slot_no プレーヤのAISACスロット番号
	 * \param cue_sheet キューシートハンドル<br>
	 * \param err エラーコード
	 * \par 説明:
	 * 特定のキューシート内の指定した名前のAISACをプレーヤに取り付けます。<br>
	 * AISACの検索は、プレーヤを作成したCRI Audio オブジェクトに登録されているキューシートから行われます。<br>
	 * プレーヤにはデフォルトで8個のAISACスロットが存在し、それぞれAISACを取り付けることができます。
	 * 何番目のAISACスロットにどのようなAISACを取り付けるかは、ユーザーが自由に決定します。
	 * 例えば、0番目のAISACスロットには「距離減衰」に関するAISACを取り付けることにして、
	 * シーン毎に差し替えていく、といったことができます。（距離減衰1→距離減衰2→距離減衰3…）<br>
	 * プレーヤに取り付けられたAISACは、キューに含まれるAISACと同様にコントロール可能です。
	 * プレーヤに取り付けられたAISACのコントロール値は0.0fで初期化されます。<br>
	 * すでにAISACが取り付けられているAISACスロットにもう一度AISACを取り付ける場合は、
	 * 先にCriAuPlayer::DetachAisac関数によりAISACの取り外しを行ってください。
	 * \code
	 * CriError err;
	 * const CriChar8* aisac_pattern_name = "DistanceDecay";
	 * CriUint32 slot_no = 0;
	 * // Attach the "DistanceDecay" AISAC to the player
	 * auply->AttachAisac(aisac_pattern_name, slot_no, err);
	 * \endcode
	 */
	/*EN
	 * \brief Attach AISAC to player by Cue Sheet
	 * \param aisac_pattern_name A pattern name of a AISAC to attach
	 * \param slot_no AISAC slot number of a player
	 * \param cue_sheet The cue sheet handle
	 * \param err CriError information
	 * \par Description:
	 * This function attaches an AISAC to a specified AISAC slot of a player by specifying the cue sheet.
	 * A search of the pattern name of an AISAC is done in the cue sheets that have been attached to
	 * the CRI Audio Object that created the player.
	 * A player has 8 AISAC slots in the default setting, and an AISAC can be attached to each slot.
	 * You can decide the type of AISAC you want to attach to any indexed AISAC slot.<br>
	 * For example, you can attach an AISAC relevant to "Distance Decay" to AISAC slot number 0.
	 * And you switch AISACs in accordance with each scene. ("Distance Decay1" -> "Distance Decay2" -> "Distance Decay3"...)<br>
	 * The AISACs that have been attached to a player can be controlled, as well as the AISACs in a cue.
	 * The control value of an AISAC to attach is initialized by 0.0f.
	 * To attach an AISAC to an AISAC slot that already has an AISAC,
	 * first detach the current AISAC using the CriAuPlayer::DetachAisac function.
	 * \code
	 * CriError err;
	 * const CriChar8* aisac_pattern_name = "DistanceDecay";
	 * CriUint32 slot_no = 0;
	 * // Attach the "DistanceDecay" AISAC to the player
	 * auply->AttachAisac(aisac_pattern_name, slot_no, err);
	 * \endcode
	 */
	virtual void AttachAisac(const CriChar8 *aisac_pattern_name, CriUint32 slot_no, CriAuCueSheet* cue_sheet, CriError &err = criErr::ErrorContainer) = 0;
	/*JP
	 * \brief AISACの取り外し
	 * \param slot_no プレーヤのAISACスロット番号
	 * \param err エラーコード
	 * \par 説明:
	 * 指定したAISACスロットからAISACを取り外します。<br>
	 * \code
	 * CriError err;
	 * CriUint32 slot_no = 0;
	 * // Detach the AISAC of slot of the player
	 * auply->DetachAisac(slot_no, err);
	 * \endcode
	 */
	/*EN
	 * \brief Detach AISAC from player
	 * \param slot_no AISAC slot number of the player
	 * \param err CriError information
	 * \par Description:
	 * This function detaches an AISAC from the specified AISAC slot of a player.
	 * \code
	 * CriError err;
	 * CriUint32 slot_no = 0;
	 * // Detach the AISAC of slot of the player
	 * auply->DetachAisac(slot_no, err);
	 * \endcode
	 */
	virtual void DetachAisac(CriUint32 slot_no, CriError &err = criErr::ErrorContainer) = 0;
	/*JP
	 * \brief キュー名の取得
	 * \param err エラーコード<br>
	 * \par 説明:
	 * プレーヤにセットされているキューの名前を取得します。
	 * \code
	 * CriError err;
	 * // Get cue name from the player
	 * const CriChar8* cue_name = auply->GetCueName(err);
	 * \endcode
	 */
	/*EN
	 * \brief Retrieve cue name
	 * \param err CriError information
	 * \par Description:
	 * This function returns the cue name of the cue set to the player.
	 * \code
	 * CriError err;
	 * // Get cue name from the player
	 * const CriChar8* cue_name = auply->GetCueName(err);
	 * \endcode
	 */
	virtual const CriChar8* GetCueName(CriError &err = criErr::ErrorContainer) = 0;
	/*JP
	 * \brief シンセサイザ名の取得
	 * \param err エラーコード<br>
	 * \par 説明:
	 * プレーヤにセットされているキューのシンセサイザ名を取得します。
	 * \code
	 * CriError err;
	 * // Get synthesizer name of the player
	 * const CriChar8* synth_name = auply->GetSynthName(err);
	 * \endcode
	 */
	/*EN
	 * \brief Retrieve synthesizer name
	 * \param err CriError information
	 * \par Description:
	 * This function returns the synthesizer name of the cue set to the player.
	 * \code
	 * CriError err;
	 * // Get synthesizer name of the player
	 * const CriChar8* synth_name = auply->GetSynthName(err);
	 * \endcode
	 */
	virtual const CriChar8* GetSynthName(CriError &err = criErr::ErrorContainer) = 0;
	/*JP
	 * \brief キューIDの取得
	 * \param err エラーコード<br>
	 * \par 説明:
	 * プレーヤにセットされているキューのIDを取得します。
	 * \code
	 * CriError err;
	 * // Get cue id of the cue which this player has
	 * CriUint32 cue_id = auply->GetCueId(err);
	 * \endcode
	 */
	/*EN
	 * \brief Retrieve cue ID
	 * \param err CriError information
	 * \par Description:
	 * This function returns the cue ID of the cue set to the player.
	 * \code
	 * CriError err;
	 * // Get cue id of the cue which this player has
	 * CriUint32 cue_id = auply->GetCueId(err);
	 * \endcode
	 */
	virtual CriUint32 GetCueId(CriError &err = criErr::ErrorContainer) = 0;
	/*JP
	 * \brief ユーザデータの取得
	 * \param err エラーコード<br>
	 * \par 説明:
	 * プレーヤにセットされているキューのユーザデータ(文字列)を取得します。
	 * \code
	 * CriError err;
	 * // Get user data of the cue which this player has
	 * const CriChar8* user_data = auply->GetUserData(err);
	 * \endcode
	 */
	/*EN
	 * \brief Retrieve user's data
	 * \param err CriError information
	 * \par Description:
	 * This function returns the user's data (a string type) of the cue set to the player.
	 * \code
	 * CriError err;
	 * // Get user data of the cue which this player has
	 * const CriChar8* user_data = auply->GetUserData(err);
	 * \endcode
	 */
	virtual const CriChar8* GetUserData(CriError &err = criErr::ErrorContainer) = 0;
	/*JP
	 * \brief 自動ループ再生モードの設定
	 * \param auto_loop_flag 自動ループフラグ
	 * \param err エラーコード
	 * \par 説明:
	 * auto_loop_flag をTRUEにすると音声全体を自動的にループ再生します。
	 * すでにループ設定されている音声には、指定できません。
	 * \code
	 * CriError err;
	 * CriBool auto_loop_flag = TRUE;
	 * // Set auto Looping mode to player
	 * auply->SetAutoLoop(auto_loop_flag, err);
	 * \endcode
	 */
	/*EN
	 * \brief Set auto Looping mode
	 * \param auto_loop_flag Auto looping flag
	 * \param err CriError information
	 * \par Description:
	 * If the auto_loop_flag is set to TRUE, the sound is played back looping automatically.
	 * If a sound has loop information, this function returns with an error.
	 * \code
	 * CriError err;
	 * CriBool auto_loop_flag = TRUE;
	 * // Set auto Looping mode to player
	 * auply->SetAutoLoop(auto_loop_flag, err);
	 * \endcode
	 */
	virtual void SetAutoLoop(CriBool auto_loop_flag, CriError &err = criErr::ErrorContainer) = 0;
	/*JP
	 * \brief 自動ループ再生モードの取得
	 * \param err エラーコード
	 * \return 自動ループ再生フラグ
	 * \par 説明:
	 * 自動ループ再生モードを返します。
	 * \code
	 * CriError err;
	 * // Get auto Looping mode of player
	 * CriBool auto_loop_flag = auply->GetAutoLoop(err);
	 * \endcode
	 */
	/*EN
	 * \brief Retrieve Auto Looping mode
	 * \param err CriError information
	 * \return Auto looping mode flag
	 * \par Description:
	 * This function returns an auto looping mode flag.
	 * \code
	 * CriError err;
	 * // Get auto Looping mode of player
	 * CriBool auto_loop_flag = auply->GetAutoLoop(err);
	 * \endcode
	 */
	virtual CriBool GetAutoLoop(CriError &err = criErr::ErrorContainer) = 0;
	/*JP
	 * \brief ソースファイルの出力チャンネル反転設定
	 * \param mode 反転モード
	 * \param err エラーコード
	 * \par 説明:
	 * ソースファイルの出力チャンネル反転を設定します。
	 * \code
	 * CriError err;
	 * // Invert right and left
	 * auply->InvertSourceChannel(INVERT_MODE_LEFT_RIGHT, err);
	 * \endcode
	 */
	/*EN
	 * \brief Invert output channel of source file
	 * \param mode Invert mode
	 * \param err CriError information
	 * \par Description:
	 * Set the output channel inversion of the source file.
	 * \code
	 * CriError err;
	 * // Invert right and left
	 * auply->InvertSourceChannel(INVERT_MODE_LEFT_RIGHT, err);
	 * \endcode
	 */
	virtual void InvertSourceChannel(InvertMode mode, CriError &err = criErr::ErrorContainer) = 0;

	// For Debugging ===========================================================
	/*JP
	 * \brief バッファリング時間の取得（デバッグ用）
	 * \param err エラーコード
	 * \return バッファリング時間
	 * \par 説明:
	 * ストリーム再生時にバッファリングされている時間を取得します。<br>
	 * プレイヤで複数の音声が再生されている場合は、それらの音声の中からある音声を特定して取得することはできません。
	 * \code
	 * CriError err;
	 * // Get buffering time
	 * CriFloat32 buffering_time = auply->GetBufferedTime(err);
	 * \endcode
	 */
	/*EN
	 * \brief Retrieve buffering time (for Debugging)
	 * \param err CriError information
	 * \return A buffering time
	 * \par Description:
	 * This function returns a remain time in a streaming buffer.
	 * If multiple voices are playing by player, this function can work for only one voice(can not specify).
	 * \code
	 * CriError err;
	 * // Get buffering time
	 * CriFloat32 buffering_time = auply->GetBufferedTime(err);
	 * \endcode
	 */
	virtual CriFloat32 GetBufferedTime(CriError &err = criErr::ErrorContainer) = 0;

	/*JP
	 * \brief 再生済みサンプル数の取得
	 * \param err エラーコード
	 * \return 再生済みサンプル数
	 * \par 説明:
	 * セットされているキューについて、再生済みのサンプル数を取得します。<br>
	 * 同じキューで複数回再生している場合は、最後に再生したものについて取得します。<br>
	 * キューがポリフォニックコンプレックスシンセサイザを含む場合は、以下の条件下でのみ正しい値が取得できます。<br>
	 * -# 子シンセの以下のパラメータがそれぞれお互いに等しい。<br>
	 *  - 素材のサンプリング周波数
	 *  - ピッチ
	 *  - プリディレイタイム
	 * -# AISACを使用して、それぞれの子シンセごとに異なるピッチ／プリディレイタイムを設定することがない。
	 * .
	 * \code
	 * CriError err;
	 * CriFloat32 played_samples = auply->GetNumPlayedSamples(err);
	 * \endcode
	 */
	/*EN
	 * \brief Retrieve the number of played samples
	 * \param err CriError information
	 * \return Number of played samples
	 * \par Description: Description:
	 * This function returns the number of played samples about the cue now set.
	 * If the cue is played several times, this function returns about the last played.<br>
	 * If the cue contains some polyphonic complex synthesizer(s), this function work correct only under the following conditions.<br>
	 * -# The following parameters of child synthesizers are equal to each other.<br>
	 *  - Sampling rate of a material.
	 *  - Pitch
	 *  - Predelay time
	 * -# A different value of pitch/predelay is not applied between each primitive synthesizer by using AISAC.
	 * .
	 * \code
	 * CriError err;
	 * CriFloat32 played_samples = auply->GetNumPlayedSamples(err);
	 * \endcode
	 */
	virtual CriUint32 GetNumPlayedSamples(CriError &err = criErr::ErrorContainer) = 0;

#ifndef DOXYGEN_SHOULD_SKIP_THIS
	/* 再生位置の取得 */
	virtual void GetPlaybackPosition(CriUint32& count, CriUint32& unit, CriError &err = criErr::ErrorContainer) const = 0;

	/* テスト実装 */
	virtual void GetTime(CriUint32& count, CriUint32& unit) const = 0;
	virtual void GetTotalTime(CriUint32& count, CriUint32& unit) const = 0;

	/*JP
	 * \brief プレーヤパラメータの設定
	 * \param params プレーヤパラメータ
	 * \param err エラーコード
	 * \par 説明:
	 * プレーヤパラメータ構造体を使用してパラメータを設定します。
	 * \code
	 *		CriAuPlayer::Parameters params = GetPlayerParameters(name, err);
	 *		//	Set player parameters with a parameter structure.
	 *		auply->SetParameters(&params, err);
	 * \endcode
	 * \sa \ref cria_intro_what_is_voice_control
	 */
	/*EN
	 * \brief Set a Player Parameters
	 * \param params player parameters
	 * \param err CriError information
	 * \par Description:
	 * This function sets a player parameters with a parameter structure.
	 * \sa \ref cria_intro_what_is_voice_control
	 */
	virtual void SetParameters(const Parameters* params, CriError &err = criErr::ErrorContainer) = 0;
#endif // DOXYGEN_SHOULD_SKIP_THIS
protected:
	CriAuPlayer();
	virtual ~CriAuPlayer();

private:
	// For R2 ==========================================================================
	/*JP
	 * \brief キューの再生＆削除（未実装）
	 * \par 説明:
	 * プレーヤ設定されているキューを再生し、プレーヤの再生ステータスがSTATUS_PLAYENDになるとプレーヤを削除します。<br>
	 * 本関数にて再生した後はプレーヤに対してアクセスすることはできません。<br>
	 * また、本関数ではループ音を再生できません<br>
	 */
	/*EN
	 * \brief Play and Destroy a cue (not available)
	 * \par Description:
	 * This function plays a cue which is bound to a CRI Audio player
	 * and destroy the player when the status reaches STATUS_PLAYEND.
	 * The player cannot be accessed after playing via this function.
	 * This function does NOT play loop sounds.
	 */
	void PlayAndDestroy(CriError &err = criErr::ErrorContainer);
	/*JP
	 * \brief Aisac3dPosの設定（未実装）
	 * \par 説明:
	 * プレーヤにAisac3dPos用のコントロールを行います。
	 */
	/*EN
	 * \brief Set Aisac3dPos (not available)
	 * \par Description:
	 * This function controls a location parameters for an Aisac3dPos.
	 */
	void SetAisac3dPos(CriFloat32 x, CriFloat32 y, CriFloat32 z, CriError &err = criErr::ErrorContainer);
/*JP
 * @} MDL_LIB_PLAYER
 */
/*EN
 * @} MDL_LIB_PLAYER
 */
};


/*JP
 * \ingroup MDL_UTILITY
 * \defgroup NAMESPACE_CRI_UTILITY CriAuUtility
 * \brief ユーティリティ
 * CRI Audioのユーティリティ関数です。<br>
 */
/*EN
 * \brief Utility
 * \defgroup NAMESPACE_CRI_UTILITY CriAuUtility
 * \ingroup MDL_UTILITY
 * Utility functions for CRI Audio
 */
namespace CriAuUtility {
/*JP
 * \ingroup MDL_UTILITY
 * \addtogroup NAMESPACE_CRI_UTILITY
 * @{
 */
/*EN
 * \ingroup MDL_UTILITY
 * \addtogroup NAMESPACE_CRI_UTILITY
 * @{
 */
	/*JP
	 * \brief ストリーム再生用バッファサイズの計算
	 * \param nstm 同時に再生するストリーミング数<br>
	 * \param stmspec ストリーミングするサウンドデータの仕様<br>
	 * \param min_buffering_time 最小バッファリング時間<br>
	 * \param err エラーコード<br>
	 * \return ストリーム再生用バッファサイズ
	 * \par 説明:
	 * ストリーム再生に必要な最小バッファサイズを計算します。<br>
	 * 第3引数は、データの読み込みを高速にするために、どのくらいバッファリングするかを指定します。<br>
	 * 例えば、10.0f を指定するとstmspec[0]の仕様をもとに10秒分のバッファ量を計算します。
	 * そして、再生に必要な最小のバッファサイズと比較し、大きい値を返します。<br>
	 * 音楽を再生しながらデータの読み込みを行う場合、stmspec[0]には音楽データの仕様を指定します。
	 * そして、第3引数に10秒を指定すると約10秒に1回の割合で音声ファイルへのアクセスを行うことになり、
	 * ストリーミング再生によるオーバーヘッドを大幅に軽減できます。<br>
	 * 48KHzのステレオ音声データの場合、10秒で約540Kバイトです。
	 * 10秒が推奨値ですが、アプリケーションの状況に応じて調整してください。
	 * この値を小さくしても音声は途切れません。
	 * 
	 * 例）48KHzのステレオを2本、48KHzのモノラル、24KHzのモノラルの４つのストリームを同時に再生する場合
	 * \code
	 *
	 * 	CriAu::StreamSpecSound stmspec[4];
	 * 	stmspec[0].nch = 2;
	 * 	stmspec[0].sampling_rate = 48000;
	 * 	stmspec[1].nch = 2;
	 * 	stmspec[1].sampling_rate = 48000;
	 * 	stmspec[2].nch = 1;
	 * 	stmspec[2].sampling_rate = 48000;
	 * 	stmspec[3].nch = 1;
	 * 	stmspec[3].sampling_rate = 24000;
	 * 
	 * 	CriUint32 bufsize = CriAuUtility::CalcStreamingMinimumBufferSize(4, stmspec, 10.0f, err);
	 * 	CriFs::AllocateStreamingBuffer(heap, bufsize);
	 * 
	 * \endcode
	 */
	/*EN
	 * \brief Calculate minimum buffer size for playback by streaming
	 * \param nstm Number of streams<br>
	 * \param stmspec Specifications for streaming sound<br>
	 * \param min_buffering_time Minimum buffering time<br>
	 * \param err CriError information
	 * \return buffer size
	 * \par Description:
 	 * This function calculates the minimum buffer size that is necessary for stream playback.<br>
	 * You can specify minimum buffering time in the third argument to read data quickly. <br>
	 * 10 seconds is the recommended value, but please adjust it depending on the application. <br>
	 * Even if you make this value small, audio playback will not be interrupted.
	 * 
	 * e.g. When you play four streams at the same time
	 * (Two stereo files of 48KHz, one monaural file of 48KHz, one monaural file of 24KHz)
	 * \code
	 *
	 * 	CriAu::StreamSpecSound stmspec[4];
	 * 	stmspec[0].nch = 2;
	 * 	stmspec[0].sampling_rate = 48000;
	 * 	stmspec[1].nch = 2;
	 * 	stmspec[1].sampling_rate = 48000;
	 * 	stmspec[2].nch = 1;
	 * 	stmspec[2].sampling_rate = 48000;
	 * 	stmspec[3].nch = 1;
	 * 	stmspec[3].sampling_rate = 24000;
	 * 
	 * 	CriUint32 bufsize = CriAuUtility::CalcStreamingMinimumBufferSize(4, stmspec, 10.0f, err);
	 * 	CriFs::AllocateStreamingBuffer(heap, bufsize);
	 * 
	 * \endcode
	 */
	CriUint32 CRIAPI CalcStreamingMinimumBufferSize(CriUint32 nstm, CriAu::StreamSpecSound stmspec[], CriFloat32 min_buffering_time, CriError &err = criErr::ErrorContainer);


	static const CriFloat32 CRIAUUTY_PI = 3.14159265358979323846f;

#ifndef DOXYGEN_SHOULD_SKIP_THIS  // 未実装のドキュメント出力を抑制
	/*JP
	 * \brief スピーカーセンドレベルの計算 (2スピーカー用)
	 * \param angle_degree 音源の角度　(左=-90, 正面=0, 右=90)<br>
	 * \param lvl_l 左スピーカードライセンドレベル<br>
	 * \param lvl_r 右スピーカードライセンドレベル<br>
	 * \return	なし
	 * \par 説明:
	 *	スピーカーへのセンドレベルを計算します。<BR>
	 *  音源の角度から、左右の２つのスピーカーへのセンドレベルを計算します。<br>
	 *	角度は、左を-90(度)、正面を0(度)、右90(度)として指定します。<br>
	 *	各スピーカーへのセンドレベルが、lvl_l, lvl_rに代入されます。<br>
	 * \sa CriAuSendLevel
	 */
	/*EN
	 * \brief Speaker send level calculation (for 2 speakers)
	 * \param angle_degree Angle of sound source (Left: 90, Front: 0, Right: -90)
	 * \param lvl_l Left speaker dry send level
	 * \param lvl_r Right speaker dry send level
	 * \par Description:
	 *	This function calculates the send levels for speakers.
	 *  Based on the specified sound source angle, two speakers' send levels are
	 * calculated into lvl_l and lvl_r.
	 * \sa CriAuSendLevel
	 */
	inline void CRIAPI CalcSendLevel2Speakers(CriFloat32 angle_degree, CriFloat32 *lvl_l, CriFloat32 *lvl_r) {
		if ( angle_degree < -90.0f )
			angle_degree = -90.0f;
		if ( angle_degree > 90.0f )
			angle_degree = 90.0f;
		*lvl_l = cosf((angle_degree+90.0f)/180.0f* CRIAUUTY_PI/2.0f);
		*lvl_r = sinf((angle_degree+90.0f)/180.0f* CRIAUUTY_PI/2.0f);
	}
	/*JP
	 * \brief スピーカーセンドレベルの計算 (2スピーカー用)
	 * \param angle_degree 音源の角度　(左=-90, 正面=0, 右=90)<br>
	 * \param lvl ドライセンドレベル<br>
	 * \return	なし
	 * \par 説明:
	 *	スピーカーへのセンドレベルを計算します。<BR>
	 *  音源の角度から、左右の２つのスピーカーへのセンドレベルを計算します。<br>
	 *	角度は、左を-90(度)、正面を0(度)、右90(度)として指定します。<br>
	 * \sa CriAuSendLevel
	 */
	/*EN
	 * \brief Speaker send level calculation (for 2 speakers)
	 * \param angle_degree Angle of sound source (Left: 90, Front: 0, Right: -90)
	 * \param lvl Dry send level
	 * \par Description:
	 *	This function calculates the send levels for speakers.
	 *  Based on the specified sound source angle, two speakers' send levels are
	 * calculated into lvl.
	 * \sa CriAuSendLevel
	 */
	inline void CRIAPI CalcSendLevel2Speakers(CriFloat32 angle_degree, CriAuSendLevel &lvl) {
		CalcSendLevel2Speakers(angle_degree, &lvl.level[CriAuSendLevel::DRY_L], &lvl.level[CriAuSendLevel::DRY_R]);
		lvl.level[CriAuSendLevel::DRY_LS] = 0.0f;
		lvl.level[CriAuSendLevel::DRY_RS] = 0.0f;
		lvl.level[CriAuSendLevel::DRY_C] = 0.0f;
		lvl.level[CriAuSendLevel::DRY_LFE] = 0.0f;
		lvl.level[CriAuSendLevel::DRY_EXT0] = 0.0f;
		lvl.level[CriAuSendLevel::DRY_EXT1] = 0.0f;
	}
	/*JP
	 * \brief スピーカーセンドレベルの計算 (4スピーカー用)
	 * \param angle_degree 音源の角度　(左=-90, 正面=0, 右=90, 後=180または-180)
	 * \param lvl_l  左スピーカードライセンドレベル<br>
	 * \param lvl_r  右スピーカードライセンドレベル<br>
	 * \param lvl_ls 左サラウンドスピーカードライセンドレベル<br>
	 * \param lvl_rs 右サラウンドスピーカードライセンドレベル<br>
	 * \return	なし
	 * \par 説明:
	 *	スピーカーへのセンドレベルを計算します。<BR>
	 *  音源の角度から、4つのスピーカーへのセンドレベルを計算します。<br>
	 *	角度は、左を-90(度)、正面を0(度)、右90(度)、後180または-180(度)として指定します。<br>
	 *	各スピーカーへのセンドレベルが、lvl_l、lvl_r、lvl_ls、lvl_rsに代入されます。<br>
	 * \sa CriAuSendLevel
	 */
	/*EN
	 * \brief Speaker send level calculation (for 4 speakers)
	 * \param angle_degree Angle of sound source (Left: 90, Front: 0, Right: -90, Rear: 180 or -180)
	 * \param lvl_l Left speaker dry send level
	 * \param lvl_r Right speaker dry send level
	 * \param lvl_ls Left surround speaker dry send level
	 * \param lvl_rs Right surround speaker dry send level
	 * \par Description:
	 *	This function calculates the send levels for four speakers
	 *  from a sound source angle. The angle is specified in degrees between
	 * -180.0 and +180.0. Modulo 360 is used if the angle is out of the range.
	 * \sa CriAuSendLevel
	 */
	inline void CRIAPI CalcSendLevel4Speakers(CriFloat32 angle_degree, CriFloat32 *lvl_l, CriFloat32 *lvl_r, CriFloat32 *lvl_ls, CriFloat32 *lvl_rs)
	{
		CriFloat32 ang2;

		for (;;) {
			if ( angle_degree > 180.0f )
				angle_degree -= 360.0f;
			else if ( angle_degree < -180.0f )
				angle_degree += 360.0f;
			else
				break;
		}
		*lvl_l = *lvl_r = *lvl_ls = *lvl_rs = 0.0f;
		if ( angle_degree >= -180.0f && angle_degree <= -120.0f ) {
			ang2 = (angle_degree - (-180.0f))/60.0f*90.0f;
			CalcSendLevel2Speakers(ang2, lvl_rs, lvl_ls);
		} else if ( angle_degree >= -120.0f && angle_degree <= -30.0f ) {
			ang2 = (angle_degree - (-75.0f))/45.0f*90.0f;
			CalcSendLevel2Speakers(ang2, lvl_ls, lvl_l);
		} else if ( angle_degree >= -30.0f && angle_degree <= 30.0f ) {
			ang2 = (angle_degree - (0.0f))/30.0f*90.0f;
			CalcSendLevel2Speakers(ang2, lvl_l, lvl_r);
		} else if ( angle_degree >= 30.0f && angle_degree <= 120.0f ) {
			ang2 = (angle_degree - 75.0f)/45.0f*90.0f;
			CalcSendLevel2Speakers(ang2, lvl_r, lvl_rs);
		} else if ( angle_degree >= 120.0f && angle_degree <= 180.0f ) {
			ang2 = (angle_degree - 180.0f)/60.0f*90.0f;
			CalcSendLevel2Speakers(ang2, lvl_rs, lvl_ls);
		}
	}
	/*JP
	 * \brief スピーカーセンドレベルの計算 (4スピーカー用)
	 * \param angle_degree 音源の角度　(左=-90, 正面=0, 右=90, 後=180または-180)
	 * \param lvl ドライセンドレベル<br>
	 * \return	なし
	 * \par 説明:
	 *	スピーカーへのセンドレベルを計算します。<BR>
	 *  音源の角度から、4つのスピーカーへのセンドレベルを計算します。<br>
	 *	角度は、左を-90(度)、正面を0(度)、右90(度)、後180または-180(度)として指定します。<br>
	 * \sa CriAuSendLevel
	 */
	/*EN
	 * \brief Speaker send level calculation (for 4 speakers)
	 * \param angle_degree Angle of sound source (Left: 90, Front: 0, Right: -90, Rear: 180 or -180)
	 * \param lvl Dry send level
	 * \par Description:
	 *	This function calculates the send levels for four speakers
	 *  from a sound source angle. The angle specified in degrees between
	 * -180.0 and +180.0. Modulo 360 is used if the angle is out of the range.
	 * \sa CriAuSendLevel
	 */
	inline void CRIAPI CalcSendLevel4Speakers(CriFloat32 angle_degree, CriAuSendLevel &lvl)
	{
		CalcSendLevel4Speakers(angle_degree,
								&lvl.level[CriAuSendLevel::DRY_L],
								&lvl.level[CriAuSendLevel::DRY_R],
								&lvl.level[CriAuSendLevel::DRY_LS],
								&lvl.level[CriAuSendLevel::DRY_RS]);
		lvl.level[CriAuSendLevel::DRY_C] = 0.0f;
		lvl.level[CriAuSendLevel::DRY_LFE] = 0.0f;
		lvl.level[CriAuSendLevel::DRY_EXT0] = 0.0f;
		lvl.level[CriAuSendLevel::DRY_EXT1] = 0.0f;
	}
	/*JP
	 * \brief スピーカーセンドレベルの計算 (4スピーカー用)
	 * \param angle_degree 音源の角度　(左=-90, 正面=0, 右=90, 後=180または-180)
	 * \param distance 音源の距離　(0.0～1.0)
	 * \param lvl_l  左スピーカードライセンドレベル<br>
	 * \param lvl_r  右スピーカードライセンドレベル<br>
	 * \param lvl_ls 左サラウンドスピーカードライセンドレベル<br>
	 * \param lvl_rs 右サラウンドスピーカードライセンドレベル<br>
	 * \return	なし
	 * \par 説明:
	 *	スピーカーへのセンドレベルをインテリアパンニングを考慮して計算します。<BR>
	 *  音源の角度と距離から、4つのスピーカーへのセンドレベルを計算します。<br>
	 *	角度は、左を-90(度)、正面を0(度)、右90(度)、後180または-180(度)として指定します。<br>
	 *	距離は、リスナー位置を0.0、スピーカー位置を1.0として指定します。<br>
	 *	各スピーカーへのセンドレベルが、lvl_l、lvl_r、lvl_ls、lvl_rsに代入されます。<br>
	 * \sa CriAuSendLevel
	 */
	/*EN
	 * \brief Speaker send level calculation (for 4 speakers)
	 * \param angle_degree Angle of sound source (Left: 90, Front: 0, Right: -90, Rear: 180 or -180)
	 * \param distance Distance of sound source (0.0-1.0)
	 * \param lvl_l Left speaker dry send level
	 * \param lvl_r Right speaker dry send level
	 * \param lvl_ls Left surround speaker dry send level
	 * \param lvl_rs Right surround speaker dry send level
	 * \par Description:
	 *	This function calculates the send levels for four speakers
	 *  from a sound source angle and a source distance with interior panning.
	 *  The angle is specified in degrees between -180.0 and +180.0. Modulo 360 is
	 *  used if the angle is out of the range.
	 *  The distance is specified between 0.0 (listener position) and 1.0 (speaker position).
	 * \sa CriAuSendLevel
	 */
	inline void CRIAPI CalcSendLevel4SpeakersWithInterior(CriFloat32 angle_degree, CriFloat32 distance, CriFloat32 *lvl_l, CriFloat32 *lvl_r, CriFloat32 *lvl_ls, CriFloat32 *lvl_rs)
	{
		CriFloat32 ang1, ang2, gain1, gain2, angi;
		CriFloat32 l1, r1, ls1, rs1;
		CriFloat32 l2, r2, ls2, rs2;

		ang1 = angle_degree;
		ang2 = ang1 + 180.0f;
		if ( ang2 > 180.0f )
			ang2 -= 360.0f;
		if ( distance > 1.0f )
			distance = 1.0f;
		if ( distance < -1.0f )
			distance = -1.0f;
//		gain1 = (distance - (-1.0f)) / 2.0f;
//		gain2 = 1.0f - gain1;
		angi = (1.0f- ((distance - (-1.0f)) / 2.0f)) * CRIAUUTY_PI/2.0f;
		gain1 = cosf(angi);
		gain2 = sinf(angi);
		CalcSendLevel4Speakers(ang1, &l1, &r1, &ls1, &rs1);
		CalcSendLevel4Speakers(ang2, &l2, &r2, &ls2, &rs2);
		*lvl_l  = l1*gain1 + l2*gain2;
		*lvl_r  = r1*gain1 + r2*gain2;
		*lvl_ls = ls1*gain1 + ls2*gain2;
		*lvl_rs = rs1*gain1 + rs2*gain2;
	}
	/*JP
	 * \brief スピーカーセンドレベルの計算 (4スピーカー用)
	 * \param angle_degree 音源の角度　(左=-90, 正面=0, 右=90, 後=180または-180)
	 * \param distance 音源の距離　(0.0～1.0)
	 * \param lvl ドライセンドレベル<br>
	 * \return	なし
	 * \par 説明:
	 *	スピーカーへのセンドレベルをインテリアパンニングを考慮して計算します。<BR>
	 *  音源の角度と距離から、4つのスピーカーへのセンドレベルを計算します。<br>
	 *	角度は、左を-90(度)、正面を0(度)、右90(度)、後180または-180(度)として指定します。<br>
	 *	距離は、リスナー位置を0.0、スピーカー位置を1.0として指定します。<br>
	 * \sa CriAuSendLevel
	 */
	/*EN
	 * \brief Speaker send level calculation (for 4 speakers)
	 * \param angle_degree Angle of sound source (Left: 90, Front: 0, Right: -90, Rear: 180 or -180)
	 * \param distance Distance of sound source (0.0-1.0)
	 * \param lvl Dry send level
	 * \par Description:
	 *	This function calculates the send levels for four speakers
	 *  from a sound source angle and a source distance with interior panning.
	 *  The angle is specified in degrees between -180.0 and +180.0. Modulo 360 is
	 *  used if the angle is out of the range.
	 *  A distance is specified between 0.0 (listener position) and 1.0 (speaker position).
	 * \sa CriAuSendLevel
	 */
	inline void CRIAPI CalcSendLevel4SpeakersWithInterior(CriFloat32 angle_degree, CriFloat32 distance, CriAuSendLevel &lvl)
	{
		CalcSendLevel4SpeakersWithInterior(angle_degree, distance,
								&lvl.level[CriAuSendLevel::DRY_L],
								&lvl.level[CriAuSendLevel::DRY_R],
								&lvl.level[CriAuSendLevel::DRY_LS],
								&lvl.level[CriAuSendLevel::DRY_RS]);
		lvl.level[CriAuSendLevel::DRY_C] = 0.0f;
		lvl.level[CriAuSendLevel::DRY_LFE] = 0.0f;
		lvl.level[CriAuSendLevel::DRY_EXT0] = 0.0f;
		lvl.level[CriAuSendLevel::DRY_EXT1] = 0.0f;
	}
	/*JP
	 * \brief スピーカーセンドレベルの計算 (5スピーカー用)
	 * \param angle_degree 音源の角度　(左=-90, 正面=0, 右=90, 後=180または-180)
	 * \param lvl_l  左スピーカードライセンドレベル<br>
	 * \param lvl_r  右スピーカードライセンドレベル<br>
	 * \param lvl_ls 左サラウンドスピーカードライセンドレベル<br>
	 * \param lvl_rs 右サラウンドスピーカードライセンドレベル<br>
	 * \param lvl_c  センタースピーカードライセンドレベル<br>
	 * \return	なし
	 * \par 説明:
	 *	スピーカーへのセンドレベルを計算します。<BR>
	 *  音源の角度から、5つのスピーカーへのセンドレベルを計算します。<br>
	 *	角度は、左を-90(度)、正面を0(度)、右90(度)、後180または-180(度)として指定します。<br>
	 *	各スピーカーへのセンドレベルが、lvl_l、lvl_r、lvl_ls、lvl_rs、lvl_cに代入されます。<br>
	 * \sa CriAuSendLevel
	 */
	/*EN
	 * \brief Speaker send level calculation (for 5 speakers)
	 * \param angle_degree Angle of sound source (Left: 90, Front: 0, Right: -90, Rear: 180 or -180)
	 * \param lvl_l Left speaker dry send level
	 * \param lvl_r Right speaker dry send level
	 * \param lvl_ls Left surround speaker dry send level
	 * \param lvl_rs Right surround speaker dry send level
	 * \param lvl_c Center speaker dry send level
	 * \par Description:
	 *	This function calculates the send levels for five speakers from a sound source angle.
	 *  The angle is specified in degrees between -180.0 and +180.0.
	 *  Modulo 360 is used if the angle is out of the range.
	 * \sa CriAuSendLevel
	 */
	inline void CRIAPI CalcSendLevel5Speakers(CriFloat32 angle_degree, CriFloat32 *lvl_l, CriFloat32 *lvl_r, CriFloat32 *lvl_ls, CriFloat32 *lvl_rs, CriFloat32 *lvl_c)
	{
		CriFloat32 ang2;

		for (;;) {
			if ( angle_degree > 180.0f )
				angle_degree -= 360.0f;
			else if ( angle_degree < -180.0f )
				angle_degree += 360.0f;
			else
				break;
		}
		*lvl_l = *lvl_r = *lvl_ls = *lvl_rs = *lvl_c = 0.0f;
		if ( angle_degree >= -180.0f && angle_degree <= -120.0f ) {
			ang2 = (angle_degree - (-180.0f))/60.0f*90.0f;
			CalcSendLevel2Speakers(ang2, lvl_rs, lvl_ls);
		} else if ( angle_degree >= -120.0f && angle_degree <= -30.0f ) {
			ang2 = (angle_degree - (-75.0f))/45.0f*90.0f;
			CalcSendLevel2Speakers(ang2, lvl_ls, lvl_l);
		} else if ( angle_degree >= -30.0f && angle_degree <= 0.0f ) {
			ang2 = (angle_degree - (-15.0f))/15.0f*90.0f;
			CalcSendLevel2Speakers(ang2, lvl_l, lvl_c);
		} else if ( angle_degree >= 0.0f && angle_degree <= 30.0f ) {
			ang2 = (angle_degree - (15.0f))/15.0f*90.0f;
			CalcSendLevel2Speakers(ang2, lvl_c, lvl_r);
		} else if ( angle_degree >= 30.0f && angle_degree <= 120.0f ) {
			ang2 = (angle_degree - 75.0f)/45.0f*90.0f;
			CalcSendLevel2Speakers(ang2, lvl_r, lvl_rs);
		} else if ( angle_degree >= 120.0f && angle_degree <= 180.0f ) {
			ang2 = (angle_degree - 180.0f)/60.0f*90.0f;
			CalcSendLevel2Speakers(ang2, lvl_rs, lvl_ls);
		}
	}
	/*JP
	 * \brief スピーカーセンドレベルの計算 (5スピーカー用)
	 * \param angle_degree 音源の角度　(左=-90, 正面=0, 右=90, 後=180または-180)
	 * \param lvl ドライセンドレベル<br>
	 * \return	なし
	 * \par 説明:
	 *	スピーカーへのセンドレベルを計算します。<BR>
	 *  音源の角度から、5つのスピーカーへのセンドレベルを計算します。<br>
	 *	角度は、左を-90(度)、正面を0(度)、右90(度)、後180または-180(度)として指定します。<br>
	 * \sa CriAuSendLevel
	 */
	/*EN
	 * \brief Speaker send level calculation (for 5 speakers)
	 * \param angle_degree Angle of sound source (Left: 90, Front: 0, Right: -90, Rear: 180 or -180)
	 * \param lvl Dry send level
	 * \par Description:
	 *  This function calculates the send levels for five speakers from a sound source angle.
	 *  The angle is specified in degrees between
	 *  -180.0 and +180.0. Modulo 360 is used if the angle is out of the range.
	 * \sa CriAuSendLevel
	 */
	inline void CRIAPI CalcSendLevel5Speakers(CriFloat32 angle_degree, CriAuSendLevel &lvl)
	{
		CalcSendLevel5Speakers(angle_degree,
								&lvl.level[CriAuSendLevel::DRY_L],
								&lvl.level[CriAuSendLevel::DRY_R],
								&lvl.level[CriAuSendLevel::DRY_LS],
								&lvl.level[CriAuSendLevel::DRY_RS],
								&lvl.level[CriAuSendLevel::DRY_C]);
		lvl.level[CriAuSendLevel::DRY_LFE] = 0.0f;
		lvl.level[CriAuSendLevel::DRY_EXT0] = 0.0f;
		lvl.level[CriAuSendLevel::DRY_EXT1] = 0.0f;
	}
	/*JP
	 * \brief スピーカーセンドレベルの計算 (5スピーカー用)
	 * \param angle_degree 音源の角度　(左=-90, 正面=0, 右=90, 後=180または-180)
	 * \param distance 音源の距離　(0.0～1.0)
	 * \param lvl_l  左スピーカードライセンドレベル<br>
	 * \param lvl_r  右スピーカードライセンドレベル<br>
	 * \param lvl_ls 左サラウンドスピーカードライセンドレベル<br>
	 * \param lvl_rs 右サラウンドスピーカードライセンドレベル<br>
	 * \param lvl_c  センタースピーカードライセンドレベル<br>
	 * \return	なし
	 * \par 説明:
	 *	スピーカーへのセンドレベルをインテリアパンニングを考慮して計算します。<BR>
	 *  音源の角度と距離から、5つのスピーカーへのセンドレベルを計算します。<br>
	 *	角度は、左を-90(度)、正面を0(度)、右90(度)、後180または-180(度)として指定します。<br>
	 *	距離は、リスナー位置を0.0、スピーカー位置を1.0として指定します。<br>
	 *	各スピーカへのセンドレベルが、lvl_l、lvl_r、lvl_ls、lvl_rs、lvl_cに代入されます。<br>
	 * \sa CriAuSendLevel
	 */
	/*EN
	 * \brief Speaker send level calculation (for 5 speakers)
	 * \param angle_degree Angle of sound source (Left: 90, Front: 0, Right: -90, Rear: 180 or -180)
	 * \param distance Distance of sound source (0.0-1.0)
	 * \param lvl_l Left speaker dry send level
	 * \param lvl_r Right speaker dry send level
	 * \param lvl_ls Left surround speaker dry send level
	 * \param lvl_rs Right surround speaker dry send level
	 * \param lvl_c Center speaker dry send level
	 * \par Description:
	 *	This function calculates the send levels for five speakers
	 *  from a sound source angle and a sound source distance with interior panning.
	 *  The angle is specified in degrees between -180.0 and +180.0. Modulo 360 is
	 *  used if the angle is out of the range.
	 *  A distance is specified  between 0.0 (listener position) and 1.0 (speaker position).
	 * \sa CriAuSendLevel
	 */
	inline void CRIAPI CalcSendLevel5SpeakersWithInterior(CriFloat32 angle_degree, CriFloat32 distance, CriFloat32 *lvl_l, CriFloat32 *lvl_r, CriFloat32 *lvl_ls, CriFloat32 *lvl_rs, CriFloat32 *lvl_c)
	{
		CriFloat32 ang1, ang2, gain1, gain2, angi;
		CriFloat32 l1, r1, ls1, rs1, c1;
		CriFloat32 l2, r2, ls2, rs2, c2;

		ang1 = angle_degree;
		ang2 = ang1 + 180.0f;
		if ( ang2 > 180.0f )
			ang2 -= 360.0f;
		if ( distance > 1.0f )
			distance = 1.0f;
		if ( distance < -1.0f )
			distance = -1.0f;
//		gain1 = (distance - (-1.0f)) / 2.0f;
//		gain2 = 1.0f - gain1;
		angi = (1.0f- ((distance - (-1.0f)) / 2.0f)) * CRIAUUTY_PI/2.0f;
		gain1 = cosf(angi);
		gain2 = sinf(angi);
		CalcSendLevel5Speakers(ang1, &l1, &r1, &ls1, &rs1, &c1);
		CalcSendLevel5Speakers(ang2, &l2, &r2, &ls2, &rs2, &c2);
		*lvl_l  = l1*gain1  + l2*gain2;
		*lvl_r  = r1*gain1  + r2*gain2;
		*lvl_ls = ls1*gain1 + ls2*gain2;
		*lvl_rs = rs1*gain1 + rs2*gain2;
		*lvl_c  = c1*gain1  + c2*gain2;
	}
	/*JP
	 * \brief スピーカーセンドレベルの計算 (5スピーカー用)
	 * \param angle_degree 音源の角度　(左=-90, 正面=0, 右=90, 後=180または-180)
	 * \param distance 音源の距離　(0.0～1.0)
	 * \param lvl ドライセンドレベル<br>
	 * \return	なし
	 * \par 説明:
	 *	スピーカーへのセンドレベルをインテリアパンニングを考慮して計算します。<BR>
	 *  音源の角度と距離から、5つのスピーカーへのセンドレベルを計算します。<br>
	 *	角度は、左を-90(度)、正面を0(度)、右90(度)、後180または-180(度)として指定します。<br>
	 *	距離は、リスナー位置を0.0、スピーカー位置を1.0として指定します。<br>
	 * \sa CriAuSendLevel
	 */
	/*EN
	 * \brief Speaker send level calculation (for 5 speakers)
	 * \param angle_degree Angle of sound source (Left: 90, Front: 0, Right: -90, Rear: 180 or -180)
	 * \param distance Distance of sound source (0.0-1.0)
	 * \param lvl Dry send level
	 * \par Description:
	 *	This function calculates the send levels for five speakers
	 *  from a sound source angle and a sound source distance with interior panning.
	 *  The angle is specified in degrees between -180.0 and +180.0. Modulo 360 is
	 *  used if the angle is out of the range.
	 *  A distance is specified  between 0.0 (listener position) and 1.0 (speaker position).
	 * \sa CriAuSendLevel
	 */
	inline void CRIAPI CalcSendLevel5SpeakersWithInterior(CriFloat32 angle_degree, CriFloat32 distance, CriAuSendLevel &lvl)
	{
		CalcSendLevel5SpeakersWithInterior(angle_degree, distance,
								&lvl.level[CriAuSendLevel::DRY_L],
								&lvl.level[CriAuSendLevel::DRY_R],
								&lvl.level[CriAuSendLevel::DRY_LS],
								&lvl.level[CriAuSendLevel::DRY_RS],
								&lvl.level[CriAuSendLevel::DRY_C]);
		lvl.level[CriAuSendLevel::DRY_LFE] = 0.0f;
		lvl.level[CriAuSendLevel::DRY_EXT0] = 0.0f;
		lvl.level[CriAuSendLevel::DRY_EXT1] = 0.0f;
	}
#endif // DOXYGEN_SHOULD_SKIP_THIS

	inline CriFloat32 CRIAPI TransformFromXyToPolar(CriFloat32 x, CriFloat32 y, CriFloat32 *angle) {
		CriFloat32 dst = sqrtf( x*x + y*y );
		if ( dst == 0.0f ) {
			*angle = 0.0f;
			return 0.0f;
		}
		*angle = atan2f(x, y)/CRIAUUTY_PI*180.0f;

		return dst;
	}
/*JP
 * @} NAMESPACE_CRI_UTILITY
 */
/*EN
 * @} NAMESPACE_CRI_UTILITY
 */
}


#ifndef DOXYGEN_SHOULD_SKIP_THIS  // 未実装のドキュメント出力を抑制
/*JP
 * \brief エクストラパラメーターズハンドル
 * \ingroup MDL_LIB_EXTRAPARAMETERS
 * エクストラパラメーターズバイナリをロードするために使用します。
 */
/*EN
 * \brief Extra Parameters Handle
 * \ingroup MDL_LIB_EXTRAPARAMETERS
 * An Extra Parameters Handles are used to load a Extra Parameters Binary (.xxx) file.
 * You can <b>load</b> an Extra Parameters from a .xxx file or you can just <b>start</b> reading
 * an asynchronous reading a .xxx file. Or if you already have read a .xxx file onto memory,
 * you may load it from the memory.
 * In case asynchronous reading, you need to check the handle status whether if it is done or
 * not.
 */
class CriAuExtraParameters : public CriAllocator
{
public:
	enum LoadStatus {
		LOAD_STATUS_STOP = (0),			/*JP< 停止		*/
										/*EN< Stop (Initial state) */
		LOAD_STATUS_LOADING,			/*JP< ロード中	*/
										/*EN< Now loading */
		LOAD_STATUS_COMPLETE,			/*JP< ロード完了	*/
										/*EN< Loading Complete */
		LOAD_STATUS_ERROR				/*JP< エラー	*/
										/*EN< Error */
	};

	/*JP
	 * \brief エクストラパラメーターズハンドルの生成
	 * \param heap ヒープハンドル<br>
	 * \param err エラーコード<br>
	 * \par 説明:
	 * エクストラパラメーターズハンドルを生成します。
	 */
	/*EN
	 * \brief Create a extra parameters handle
	 * \param heap A CriHeap handle
	 * \param err CriError information
	 * \return A valid cue sheet handle (CriAuExtraParameters).
	 * \par Description:
	 * This function creates a CriAuExtraParameters handle in the LOAD_STATUS_STOP state.
	 * The Memory for the handle is allocated from the CriHeap structure that you provide.
	 * Any memory allocation failure during this function return NULL
	 * Make sure to initialize and create your heap with criHeap_Initialize() and
	 * criHeap_Create() before calling this function.
	 *
	 * \sa CriAuExtraParameters, criHeap_Initialize(), criHeap_Create()
	 */
	static CriAuExtraParameters* CRIAPI Create(CriHeap heap, CriError &err = criErr::ErrorContainer);

	/*JP
	 * \brief エクストラパラメーターズハンドルの削除
	 * \param err エラーコード
	 * \par 説明:
	 * エクストラパラメーターズハンドルを削除します。
	 */
	/*EN
	 * \brief Destroy a extra parameters handle
	 * \param err CriError information
	 * \par Description:
	 * This function destroys the CriAuExtraParameters handle previously created
	 * with CriAuExtraParameters::Create.
	 *
	 */
	virtual void Destroy(CriError &err = criErr::ErrorContainer) = 0;
	/*JP
	 * \brief メモリからのエクストラパラメーターズバイナリロード
	 * \param extraparameters_name エクストラパラメーターズ名 (未使用ですのでNULLを指定してください)<br>
	 * \param data エクストラパラメーターズデータのロードされている領域へのポインタ<br>
	 * \param dtsize エクストラパラメーターズデータサイズ<br>
	 * \param err エラーコード<br>
	 * \par 説明:
	 * メモリ上のエクストラパラメーターズバイナリをロードします。
	 */
	/*EN
	 * \brief Load a extra parameters binary from a certain memory location
	 * \param extraparameters_name A extra parameters name (Set NULL. This name is not used now.)
	 * \param data A pointer for the memory location where the extra parameters data exist.
	 * \param dtsize Total data size of the extra parameters binary file.
	 * \param err CriError information
	 * \par Description:
	 * This function loads a bunch of a extra parameters binary data which is exist
	 * a certain memory location. The API assumes that whole data in a file are continuous
	 * in the memory.
	 */
	virtual void LoadExtraParametersFileFromMemory(const CriUint8 *data, CriUint32 dtsize, CriError &err = criErr::ErrorContainer) = 0;
	virtual void LoadExtraParametersFileFromMemory(const CriChar8* extraparameters_name, const CriUint8 *data, CriUint32 dtsize, CriError &err = criErr::ErrorContainer) = 0;

	/*JP
	 * \brief プレーヤパラメータ名の取得
	 * \param no エクストラパラメーターズ内のインデックス番号<br>
	 * \param err エラーコード<br>
	 * \par 説明:
	 * エクストラパラメーターズ内のプレーヤパラメータ名を取得します。
	 */
	/*EN
	 * \brief Retrieve name of player parameter
	 * \param no Index number in the extra parameters
	 * \param err CriError information
	 * \par Description:
	 * This function returns the name of 'no'-th player parameter of the extra parameters.
	 */
	virtual const CriChar8 *GetNamePlayerParameters(CriUint32 no, CriError &err = criErr::ErrorContainer) = 0;
	/*JP
	 * \brief プレーヤパラメータデータの取得
	 * \param name プレーヤパラメータ名<br>
	 * \param err エラーコード<br>
	 * \return プレーヤパラメータデータ
	 * \par 説明:
	 * プレーヤパラメータ名からエクストラパラメーターズ内のプレーヤパラメータデータを取得します。
	 */
	/*EN
	 * \brief Retrieve a player parameter data
	 * \param name the name of the player parameter
	 * \param err CriError information
	 * \par Description:
	 * This function returns the player parameter data from name as a text.
	 */
	virtual CriAuPlayer::Parameters GetPlayerParameters(const CriChar8 *name, CriError &err = criErr::ErrorContainer) = 0;
	/*JP
	 * \brief プレーヤパラメータデータの取得
	 * \param no インデックス番号<br>
	 * \param err エラーコード<br>
	 * \return プレーヤパラメータデータ
	 * \par 説明:
	 * インデックス番号からエクストラパラメーターズ内のプレーヤパラメータデータを取得します。
	 */
	/*EN
	 * \brief Retrieve a player parameter data
	 * \param no Index number in the extra parameters
	 * \param err CriError information
	 * \par Description:
	 * This function returns the player parameter data from an index number.
	 */
	virtual CriAuPlayer::Parameters GetPlayerParameters(CriUint32 no, CriError &err = criErr::ErrorContainer) = 0;

protected:
	CriAuExtraParameters(CriHeap heap);
	virtual ~CriAuExtraParameters();

	CriHeap heap;
	CriChar8 *name;

private:
	CriAuExtraParameters();	// disabled
};
#endif // DOXYGEN_SHOULD_SKIP_THIS


#ifndef DOXYGEN_SHOULD_SKIP_THIS  // 未実装のドキュメント出力を抑制

/*JP
 * \brief 3次元ベクタ（未実装）
 * \par 説明:
 * \ingroup MDL_LIB_GLOBAL
 * (x, y, z)の要素を持つ3次元ベクタです。<br>
 */
/*EN
 * \brief 3 dimensional vector (not available)
 * \ingroup MDL_LIB_GLOBAL
 * \par Description:
 * This structure is used for indicating a 3D vector such as a position with (x, y, z).
 */
class CriAu3dVector {
public:
	CriFloat32 x;				/*JP< x座標値	*/
							/*EN< x coordinate */
	CriFloat32 y;				/*JP< y座標値	*/
							/*EN< y coordinate */
	CriFloat32 z;				/*JP< z座標値	*/
							/*EN< z coordinate */
};

/*JP
 * \brief ソース（未実装）
 * \ingroup MDL_LIB_GLOBAL
 * \par 説明:
 * 3D空間用のソースパラメータです。
 */
/*EN
 * \brief Source (not available)
 * \ingroup MDL_LIB_GLOBAL
 * \par Description:
 * Sound source parameters for 3D space
 */
class CriAu3dSource {
public:
	CriAu3dVector position;			/*JP< 位置ベクトルの座標値						*/
									/*EN< Location of sound source					*/
	CriAu3dVector velocity;			/*JP< ベロシティ ベクトルの座標値				*/
									/*EN< Velocity of sound source					*/
	CriFloat32 inside_cone_angle;		/*JP< 内側の指向角								*/
									/*EN< Angle of inside cone						*/
	CriFloat32 outside_cone_angle;		/*JP< 外側の指向角								*/
									/*EN< Angle of outside cone						*/
	CriAu3dVector cone_orientation;	/*JP< サウンド コーンの向きベクトル				*/
									/*EN< Orientation of sound cone					*/
	CriFloat32 cone_outside_volume;	/*JP< サウンド バッファのコーン外部ボリューム	*/
									/*EN< Outside volume of sound projection cone	*/
	CriFloat32 min_distance;			/*JP< 最小距離値 								*/
									/*EN< Minimum distance							*/
	CriFloat32 max_distance;			/*JP< 最大距離値								*/
									/*EN< Maximum distance							*/
};


/*JP
 * \brief リスナー（未実装）
 * \ingroup MDL_LIB_GLOBAL
 * \par 説明:
 * 3D空間用のリスナーパラメータです。
 */
/*EN
 * \brief Listener (not available)
 * \ingroup MDL_LIB_GLOBAL
 * \par Description:
 * Listener parameter for 3D space.
 */
class CriAu3dListener {
public:
	CriAu3dVector position;			/*JP< 位置ベクトルの座標値			*/
									/*EN< Location of a listener		*/
	CriAu3dVector velocity;			/*JP< ベロシティ ベクトルの座標値	*/
									/*EN< Velocity vector				*/
	CriAu3dVector orient_front;		/*JP< 姿勢の前方ベクトル			*/
									/*EN< Orientation for front 		*/
	CriAu3dVector orient_top;		/*JP< 姿勢の上方ベクトル			*/
									/*EN< Orientation for top			*/
	CriFloat32 distance_factor;		/*JP< 距離係数						*/
									/*EN< Distance factor				*/
	CriFloat32 rolloff_factor;			/*JP< ロールオフ係数				*/
									/*EN< Roll off factor				*/
	CriFloat32 doppler_factor;			/*JP< ドップラー係数				*/
									/*EN< Doppler factor				*/

	CriFloat32 CalcDistance(const CriAu3dSource &source, CriError &err = criErr::ErrorContainer);
	CriFloat32 CalcAngle(const CriAu3dSource &source, CriError &err = criErr::ErrorContainer);
	/*JP
	 * \brief ドップラーピッチの計算
	 * \par 説明:
	 * リスナーとソースの位置と速度からソースのドップラー効果によるピッチを計算します。
	 * \sa CriAuSendLevel
	 */
	/*EN
	 * \brief Calculate Doppler pitch
	 * \par Description:
	 * This function calculates Doppler pitch from coordinates for listener and source.
	 * \sa CriAuSendLevel
	 *
	 *	CriAu3dListener listener;
	 *	CriAu3dSource sorce;
	 *	listener.position.x =   10.0f, listener.position.y =  20.0f,listener.position.z =  30.0f; 
	 *	listener.velocity.x =    3.0f, listener.velocity.y =  -5.0f,listener.velocity.z = -10.0f; 
	 *	source.position.x   =  100.0f, source.position.y   = 120.0f,source.position.z   = -50.0f; 
	 *	source.velocity.x   =   10.0f, source.velocity.y   =   3.0f,source.velocity.z   =  -5.0f; 
	 *
	 *	CriFloat32 pitch = listener.CalcDopplerPitch(source, err);
	 *	auply->SetPitch(pitch);
	 *
	 */
	CriFloat32 CalcDopplerPitch(const CriAu3dSource &source, CriError &err = criErr::ErrorContainer);
	/*JP
	 * \brief ドライセンドレベルの計算
	 * \par 説明:
	 * リスナーとソースの位置からソースのドライセンドレベルを計算します。
	 * \sa CriAuSendLevel
	 */
	/*EN
	 * \brief Calculation of dry send levels (not available)
	 * \par Description:
	 * This function calculates dry send levels from coordinates for listener and source.
	 * \sa CriAuSendLevel
	 */
	void CalcDrySendLevel(const CriAu3dSource &source, CriAuSendLevel &lvl, CriError &err = criErr::ErrorContainer);
	CriFloat32 CalcVolume(const CriAu3dSource &source, CriError &err = criErr::ErrorContainer);
};

class CriAuSeamlessController : public CriAllocator
{
public:
	static CriAuSeamlessController* Create(CriHeap heap, CriError &err = criErr::ErrorContainer);
	virtual void Destroy(CriError &err = criErr::ErrorContainer) = 0;

	virtual void AttachToPlayer(CriAuPlayer* player, CriError &err = criErr::ErrorContainer) = 0;
	virtual void DetachFromPlayer(CriError &err = criErr::ErrorContainer) = 0;

	virtual void Execute(CriError &err = criErr::ErrorContainer) = 0;

	virtual CriUint32 RegisterCue(const CriChar8* cue_name, CriError &err = criErr::ErrorContainer) = 0;
	virtual CriUint32 RegisterCue(const CriChar8* cue_name, CriAuCueSheet* cue_sheet, CriError &err = criErr::ErrorContainer) = 0;
	virtual CriUint32 RegisterCueById(CriUint32 cue_id, CriError &err = criErr::ErrorContainer) = 0;
	virtual CriUint32 RegisterCueById(CriUint32 cue_id, CriAuCueSheet* cue_sheet, CriError &err = criErr::ErrorContainer) = 0;
	virtual CriUint32 RegisterCueByIndex(CriUint32 index, CriError &err = criErr::ErrorContainer) = 0;
	virtual CriUint32 RegisterCueByIndex(CriUint32 index, CriAuCueSheet* cue_sheet, CriError &err = criErr::ErrorContainer) = 0;

	virtual CriBool CancelRegisteredCue(CriUint32 register_id, CriError &err = criErr::ErrorContainer) = 0;

	virtual void ResetRegisteredCues(CriError &err = criErr::ErrorContainer) = 0;
	virtual CriSint32 GetNumberOfRegisteredCues(CriError &err = criErr::ErrorContainer) = 0;

	virtual void Play(CriError &err = criErr::ErrorContainer) = 0;
	virtual void Stop(CriAuPlayer::StopMode stop_mode, CriError &err = criErr::ErrorContainer) = 0;
	virtual void ReleaseLinking(CriError &err = criErr::ErrorContainer) = 0;

	virtual void SetPreparationTime(CriFloat32 preparation_time_sec, CriError &err = criErr::ErrorContainer) = 0;

	virtual void Pause(CriBool flag, CriError &err = criErr::ErrorContainer) = 0;
	virtual CriBool IsPaused(CriError &err = criErr::ErrorContainer) = 0;

protected:
	CriAuSeamlessController();
	virtual ~CriAuSeamlessController();
};

class CriAuSeamlessControllerMulti : public CriAllocator
{
public:
	static const CriUint32 MAX_CUES = 8;

	static CriAuSeamlessControllerMulti* Create(CriHeap heap, CriError &err = criErr::ErrorContainer);
	virtual void Destroy(CriError &err = criErr::ErrorContainer) = 0;

	virtual void AttachPlayers(CriAuPlayer* players[], CriUint32 num_players, CriError &err = criErr::ErrorContainer) = 0;
	virtual void DetachPlayers(CriError &err = criErr::ErrorContainer) = 0;

	virtual void Execute(CriError &err = criErr::ErrorContainer) = 0;

	virtual CriUint32 RegisterCueSet(const CriChar8* cue_names[], CriSint32 num_cues, CriError &err = criErr::ErrorContainer) = 0;
	virtual CriUint32 RegisterCueSet(const CriChar8* cue_name[], CriSint32 num_cues, CriAuCueSheet* cue_sheet, CriError &err = criErr::ErrorContainer) = 0;
	virtual CriUint32 RegisterCueSetById(CriUint32 cue_id[], CriSint32 num_cues, CriError &err = criErr::ErrorContainer) = 0;
	virtual CriUint32 RegisterCueSetById(CriUint32 cue_id[], CriSint32 num_cues, CriAuCueSheet* cue_sheet, CriError &err = criErr::ErrorContainer) = 0;
	virtual CriUint32 RegisterCueSetByIndex(CriUint32 index[], CriSint32 num_cues, CriError &err = criErr::ErrorContainer) = 0;
	virtual CriUint32 RegisterCueSetByIndex(CriUint32 index[], CriSint32 num_cues, CriAuCueSheet* cue_sheet, CriError &err = criErr::ErrorContainer) = 0;

	virtual CriBool CancelRegisteredCueSet(CriUint32 register_id, CriError &err = criErr::ErrorContainer) = 0;

	virtual void ResetRegisteredCueSets(CriError &err = criErr::ErrorContainer) = 0;
	virtual CriSint32 GetNumberOfRegisteredCueSets(CriError &err = criErr::ErrorContainer) = 0;
	virtual CriUint32 GetActiveCueSetId(CriError &err = criErr::ErrorContainer) = 0;

	virtual void Play(CriError &err = criErr::ErrorContainer) = 0;
	virtual void Stop(CriAuPlayer::StopMode stop_mode, CriError &err = criErr::ErrorContainer) = 0;
	virtual void ReleaseLinking(CriError &err = criErr::ErrorContainer) = 0;

	virtual void SetPreparationTime(CriFloat32 preparation_time_sec, CriError &err = criErr::ErrorContainer) = 0;

	virtual void Pause(CriBool flag, CriError &err = criErr::ErrorContainer) = 0;
	virtual CriBool IsPaused(CriError &err = criErr::ErrorContainer) = 0;

protected:
	CriAuSeamlessControllerMulti();
	virtual ~CriAuSeamlessControllerMulti();
};
#endif // DOXYGEN_SHOULD_SKIP_THIS

#endif	/* CRI_AUDIO_H_INCLUDED */

