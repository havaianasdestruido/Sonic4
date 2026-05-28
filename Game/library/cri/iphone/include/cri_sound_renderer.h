#ifndef _CRI_SOUND_RENDERER_H_INCLUDED
#define _CRI_SOUND_RENDERER_H_INCLUDED
/****************************************************************************
 *
 * CRI Middleware SDK
 *
 * Copyright (c) 2005-2006 CRI-MW
 *
 * Library  : CRI Sound Renderer
 * Module   : Library User's Header
 * File     : cri_sound_renderer.h
 * Date     : 2006-01-22
 *
 ****************************************************************************/
/*!
 *	\file		cri_sound_renderer.h
 */

#include <cri_xpt.h>
#include <cri_allocator.h>
#include <cri_error.h>

class CriSrObjMaster;
class CriSrBus;
class CriSrEffector;

class CriSoundRenderer : public CriAllocator
{
public:
    static CriSoundRenderer* CRIAPI  Create(CriHeap heap, CriError &err = criErr::ErrorContainer);
	virtual void Destroy(CriError &err = criErr::ErrorContainer) = 0;

#ifndef DOXYGEN_SHOULD_SKIP_THIS
	enum MeasureLoadStatus {
		MEASURE_LOAD_START	= 0,
		MEASURE_LOAD_STOP	= 1
	};
	typedef void (*MeasureLoadCallback) (MeasureLoadStatus status, CriUint32 nsmpl);

	virtual void SetMeasureLoadCallback(MeasureLoadCallback cbfunc) = 0;
#endif	// DOXYGEN_SHOULD_SKIP_THIS


protected:
	CriSoundRenderer() {}
	virtual ~CriSoundRenderer() {}
};

/*JP
 * \brief 基本サウンドレンダラ
 * \ingroup MODULE_CRI_SOUND_RENDERER_LIB
 *	\par 説明:
 * 基本となるサウンドレンダラです。<br>
 * ボイスからサウンドを発生させ、フィルタやリバーブなどの処理を行います。<br>
 */
/*EN
 * \brief Basic Sound Renderer
 * \ingroup MODULE_CRI_SOUND_RENDERER_LIB
 * \par
 * A basic sound renderer class.<br>
 * This class generates sounds from voices and applies filters such as reverb effects onto the sounds.<br>
 */
class CriSoundRendererBasic : public CriSoundRenderer
{
/*JP
 * \addtogroup MODULE_CRI_SOUND_RENDERER_LIB
 * @{
 */
/*EN
 * \addtogroup MODULE_CRI_SOUND_RENDERER_LIB
 * @{
 */
public:
	
	/*JP ダウンミックスモード */
	/*EN Downmix mode */
	enum DownmixMode {
		DOWNMIX_NONE,						/*JP< ダウンミックスなし（デフォルト） */
											/*EN< No downmix (default) */
		DOWNMIX_MONO,						/*JP< モノラルにダウンミックスします。 */
											/*EN< Downmix to mono. */
		DOWNMIX_MONO_MIXED_WITH_LFE,		/*JP< LFEも含めてモノラルにダウンミックスします。 */
											/*EN< Downmix to mono mixed with the LFE channel. */
		DOWNMIX_MONO_AND_LFE,				/*JP< モノラルにダウンミックスし、LFEチャンネルはそのまま出力します。 */
											/*EN< Downmix to mono and pass through the LFE channel. */
		DOWNMIX_MONO_TO_LR,					/*JP< モノラルにダウンミックスし、結果をLチャンネル（チャンネル0番）とRチャンネル（チャンネル1番）の両方に出力します。 */
											/*EN< Downmix to mono, and output to both L(channel 0) and R(channel 1). */
		DOWNMIX_MONO_TO_LR_MIXED_WITH_LFE,	/*JP< LFEも含めてモノラルにダウンミックスし、結果をLチャンネル（チャンネル0番）とRチャンネル（チャンネル1番）の両方に出力します。 */
											/*EN< Downmix to mono mixed with the LFE channel, and output to both L(channel 0) and R(channel 1). */
		DOWNMIX_MONO_TO_LR_AND_LFE,			/*JP< モノラルにダウンミックスし、結果をLチャンネル（チャンネル0番）とRチャンネル（チャンネル1番）の両方に出力します。LFEチャンネルはそのまま出力します。 */
											/*EN< Downmix to mono, and output to both L(channel 0) and R(channel 1), and pass through the LFE channel. */
		DOWNMIX_STEREO,						/*JP< ステレオにダウンミックスします。 */
											/*EN< Downmix to stereo. */
		DOWNMIX_STEREO_MIXED_WITH_LFE,		/*JP< LFEも含めてステレオにダウンミックスします。 */
											/*EN< Downmix to stereo mixed with the LFE channel. */
		DOWNMIX_STEREO_AND_LFE				/*JP< ステレオにダウンミックスし、LFEチャンネルはそのまま出力します。 */
											/*EN< Downmix to stereo and pass through the LFE channel. */
	};


	/*JP
	 * \brief 基本サウンドレンダラの設定パラメータ
	 *	\par 説明:
	 * サウンドレンダラの設定パラメータです。<br>
	 * CriSoundRendererBasic::Create関数に指定することができます。<br>
	 */
	/*EN
	 * \brief Sound Renderer Configuration Parameters
	 * \par
	 * Configuration parameters for a sound renderer.
	 * Used with CriSoundRendererBasic::Create function.<br>
	 */
	struct ConfigParameter
	{
	public:
		static const CriUint32 DEFAULT_SRATE = 48000;
		static const CriUint32 DEFAULT_NUM_SAMPLES_OUTPUT = 128;
		static const CriUint32 DEFAULT_MAX_VOICES = 100;
		static const CriUint32 DEFAULT_MAX_VOICE_CHANNELS = 2;

		CriFloat32 srate;				/*JP< 出力データのサンプリングレート */
									/*EN< The sampling rate of an output sound */
		CriUint32 num_samples_output;	/*JP< 1回にサウンドレンダラから取得するサンプル数 */
									/*EN< The number of samples to get from a sound renderer at a time */
		CriUint32 max_voices;			/*JP< 同時最大発音数 */
									/*EN< Maximum number of voices */
		CriUint32 max_voice_channels;	/*JP< ボイスの最大チャンネル数 */
									/*EN< Maximum number of channels of a voice */

		ConfigParameter() :
			srate((CriFloat32)DEFAULT_SRATE),
			num_samples_output(DEFAULT_NUM_SAMPLES_OUTPUT),
			max_voices(DEFAULT_MAX_VOICES),
			max_voice_channels(DEFAULT_MAX_VOICE_CHANNELS)
		{
		}

		~ConfigParameter() {}
	};

	/*JP
	 * \brief 基本サウンドレンダラの生成
	 * \param heap ヒープハンドル<br>
	 * \param err エラーコード<br>
	 * \param srate 出力データのサンプリングレート<br>
	 * \param n_samples_output 1回にサウンドレンダラから取得するサンプル数<br>
	 * \par 説明:
	 * 基本サウンドレンダラを生成します。<br>
	 * サウンドレンダラの生成したサウンドデータを取り出すために、CriSoundRendererBasic::GetData関数を使用します。<br>
	 * 取り出すためのバッファは、n_samples_output サンプル分用意しなくてはなりません。<br>
	 * \code
	 * CriError err;
	 * CriHeap heap = criHeap_Create(buf, sizeof(buf));
	 * // Create CRI Sound Renderer
	 * CriSoundRendererBasic* sndrndr = CriSoundRendererBasic::Create(heap, err);
	 * \endcode
	 */
	/*EN
	 * \brief Create a basic sound renderer
	 * \param heap Heap handle
	 * \param err CriError information
	 * \param srate Sampling rate for output sound
	 * \param n_samples_output Number of samples to get from the sound renderer at a time
	 * \par
	 * This function creates a basic sound renderer.
	 * Use the CriSoundRendererBasic::GetData method to retrieve sound data made by
	 * a sound renderer. The buffer size needs to be <b>n_samples_output</b> samples.
	 * \code
	 * CriError err;
	 * CriHeap heap = criHeap_Create(buf, sizeof(buf));
	 * // Create CRI Sound Renderer
	 * CriSoundRendererBasic* sndrndr = CriSoundRendererBasic::Create(heap, err);
	 * \endcode
	 */
	static CriSoundRendererBasic* CRIAPI Create(CriHeap heap, CriError &err = criErr::ErrorContainer, CriFloat32 srate=48000.0f, CriUint32 n_samples_output=128);
	/*JP
	 * \brief 基本サウンドレンダラの生成
	 * \param heap ヒープハンドル<br>
	 * \param err エラーコード<br>
	 * \param config 設定パラメータ<br>
	 * \par 説明:
	 * 基本サウンドレンダラを生成します。<br>
	 * <b>config</b>にはサウンドレンダラの設定パラメータを指定します。
	 * \code
	 * CriError err;
	 * CriHeap heap = criHeap_Create(buf, sizeof(buf));
	 * CriSoundRendererBasic::ConfigParameter config;
	 * config.max_voices = 64;
	 * // Create CRI Sound Renderer
	 * CriSoundRendererBasic* sndrndr = CriSoundRendererBasic::Create(heap, config, err);
	 * \endcode
	 */
	/*EN
	 * \brief Create a basic sound renderer
	 * \param heap Heap handle
	 * \param err CriError information
	 * \param config Configuration parameter
	 * \par
	 * This function creates a basic sound renderer.
	 * You can specify the configuration parameters of the sound renderer via a <b>config</b> parameter.
	 * \code
	 * CriError err;
	 * CriHeap heap = criHeap_Create(buf, sizeof(buf));
	 * CriSoundRendererBasic::ConfigParameter config;
	 * config.max_voices = 64;
	 * // Create CRI Sound Renderer
	 * CriSoundRendererBasic* sndrndr = CriSoundRendererBasic::Create(heap, config, err);
	 * \endcode
	 */
	static CriSoundRendererBasic* CRIAPI Create(CriHeap heap, const CriSoundRendererBasic::ConfigParameter &config, CriError &err = criErr::ErrorContainer);
	/*JP
	 * \brief 生成されたサウンドデータの取得
	 * \param	nch チャンネル数<br>
	 * \param	nsmpl サンプル数<br>
	 * \param	pcm 出力サウンドデータ格納用のバッファ <br>
	 * \param	err エラーコード<br>
	 * \par 説明:
	 * サウンドレンダラから生成されたサウンドデータを取得します。<br>
	 * 取得したサウンドデータは、単精度の浮動小数点型のPCMデータとなります。<br>
	 * 16bitの整数型PCMにするときは、範囲をチェックして変換してください。<br>
	 * <br>
	 * 下記のようにfloat型のバッファとそのポインタを格納する配列を用意してください。<br><br>
	 * \code
	 *
	 * #define NCHANNELS 8
	 * #define NSAMPLES 128
	 * 
	 * float pcmbuf[NCHANNELS][NSAMPLES], *pcm[NCHANNELS];
	 * 
	 * for (int i=0; i<NCHANNELS; i++)
	 * 	pcm[i] = pcmbuf[i];
	 * 
	 * GetData(NCHANNELS, NSAMPLES, pcm, err);
	 * 
	 * \endcode
	 */
	/*EN
	 * \brief Retrieve sound data generated by a sound renderer
	 * \param nch Number of channels
	 * \param nsmpl Number of samples
	 * \param pcm PCM Buffer for output sound data where PCM is an array of <b>nch</b> elements
	 * and each element keeps a pointer to a string area for <b>nsmpl</b> floating point samples.
	 * \param err CriError information
	 * \par
	 * This function returns <b>nch</b> channel sound data with <b>nsmpl</b> samples for 
	 * each channel into the area pointed by <b>pcm</b>. A sample is a single precision floating point data,
	 * so you must check the range of each value when you convert it into a 16 bit integer.<br><br>
	 * \code
	 *
	 * #define NCHANNELS 8
	 * #define NSAMPLES 128
	 * 
	 * float pcmbuf[NCHANNELS][NSAMPLES], *pcm[NCHANNELS];
	 * 
	 * for (int i=0; i<NCHANNELS; i++)
	 * 	pcm[i] = pcmbuf[i];
	 * 
	 * GetData(NCHANNELS, NSAMPLES, pcm, err);
	 * 
	 * \endcode
	 */
	virtual void GetData(CriUint32 nch, CriUint32 nsmpl, CriFloat32 *pcm[], CriError &err = criErr::ErrorContainer) = 0;
	/*JP
	 * \brief エコーブロックのディレイ時間の設定
	 * \param	delay_time ディレイタイム (ミリ秒)<br>
	 * \param	err エラーコード<br>
	 * \par 説明:
	 * エコーブロックのディレイ時間を設定します。<br>
	 * \code
	 * CriError err;
	 * CriFloat32 delay_time = 500.0f;
	 * // Set delay time to the echo block
	 * sndrndr->SetEchoDelayTime(delay_time, err);
	 * \endcode
	 */
	/*EN
	 * \brief Set delay time to the echo block
	 * \param delay_time Delay time (in msec)<br>
	 * \param err CriError information
	 * \par
	 * This function sets the delay time to the echo block.
	 * \code
	 * CriError err;
	 * CriFloat32 delay_time = 500.0f;
	 * // Set delay time to the echo block
	 * sndrndr->SetEchoDelayTime(delay_time, err);
	 * \endcode
	 */
	virtual void SetEchoDelayTime(CriFloat32 delay_time, CriError &err = criErr::ErrorContainer) = 0;
	/*JP
	 * \brief エコーブロックのフィードバックゲインの設定
	 * \param	gain フィードバックゲイン<br>
	 * \param	err エラーコード<br>
	 * \par 説明:
	 * エコーブロックのフィードバックゲインを設定します。<br>
	 * \code
	 * CriError err;
	 * CriFloat32 gain = 0.3f;
	 * // Set feedback gain to the echo block
	 * sndrndr->SetEchoGain(gain, err);
	 * \endcode
	 */
	/*EN
	 * \brief Set feedback gain to the echo block
	 * \param gain Feedback gain
	 * \param err CriError information
	 * \par
	 * This function sets the delay time to the echo block.
	 * \code
	 * CriError err;
	 * CriFloat32 gain = 0.3f;
	 * // Set feedback gain to the echo block
	 * sndrndr->SetEchoGain(gain, err);
	 * \endcode
	 */
	virtual void SetEchoGain(CriFloat32 gain, CriError &err = criErr::ErrorContainer) = 0;
	/*JP
	 * \brief リバーブブロックのデフォルト値の設定
	 * \param	err エラーコード<br>
	 * \par 説明:
	 * リバーブブロックのすべてのパラメータにデフォルト値を設定します。<br>
	 * UpdateReverbParameters関数を実行すると反映されます。<br>
	 * \code
	 * CriError err;
	 * // Set default parameters to the reverb block
	 * sndrndr->SetReverbDefaultParameters(err);
	 * // Update reverb parameters
	 * sndrndr->UpdateReverbParameters(err);
	 * \endcode
	 */
	/*EN
	 * \brief Set default parameters to the reverb block
	 * \param err CriError information
	 * \par
	 * This function sets the default parameters to the reverb block.
	 * An UpdateReverbParameters() call is required to validate the values change.
	 * \code
	 * CriError err;
	 * // Set default parameters to the reverb block
	 * sndrndr->SetReverbDefaultParameters(err);
	 * // Update reverb parameters
	 * sndrndr->UpdateReverbParameters(err);
	 * \endcode
	 */
	virtual void SetReverbDefaultParameters(CriError &err = criErr::ErrorContainer) = 0;
	/*JP
	 * \brief リバーブブロックのプリディレイ時間の設定
	 * \param	delay_time プリディレイタイム (ミリ秒)<br>
	 *			err エラーコード<br>
	 * \par 説明:
	 * リバーブブロックのプリディレイ時間を設定します。<br>
	 * UpdateReverbParameters関数を実行すると反映されます。<br>
	 * \code
	 * CriError err;
	 * CriFloat32 delay_time = 5.0f;
	 * // Set pre-delay time to the reverb block
	 * sndrndr->SetReverbPredelayTime(delay_time, err);
	 * // Update reverb parameters
	 * sndrndr->UpdateReverbParameters(err);
	 * \endcode
	 */
	/*EN
	 * \brief Set pre-delay time to the reverb block
	 * \param delay_time Pre-delay time (in msec)
	 * \param err CriError information
	 * \par
	 * This function sets the pre-delay time to the echo block.
	 * An UpdateReverbParameters() call is required to validate the value change.
	 * \code
	 * CriError err;
	 * CriFloat32 delay_time = 5.0f;
	 * // Set pre-delay time to the reverb block
	 * sndrndr->SetReverbPredelayTime(delay_time, err);
	 * // Update reverb parameters
	 * sndrndr->UpdateReverbParameters(err);
	 * \endcode
	 */
	virtual void SetReverbPredelayTime(CriFloat32 delay_time, CriError &err = criErr::ErrorContainer) = 0;
	/*JP
	 * \brief リバーブブロックのルームサイズパラメータの設定
	 * \param	size_meter 部屋の大きさ (メートル)<br>
	 * \param	err エラーコード<br>
	 * \par 説明:
	 * リバーブブロックのルームサイズパラメータを設定します。<br>
	 * UpdateReverbParameters関数を実行すると反映されます。<br>
	 * \code
	 * CriError err;
	 * CriFloat32 size_meter = 24.0f;
	 * // Set a "pseudo room size" parameter to the reverb block
	 * sndrndr->SetReverbRoomSize(size_meter, err);
	 * // Update reverb parameters
	 * sndrndr->UpdateReverbParameters(err);
	 * \endcode
	 */
	/*EN
	 * \brief Set a "pseudo room size" parameter to the reverb block
	 * \param size_meter Room size (in meters)
	 * \param err CriError information
	 * \par
	 * This function sets the pseudo room size to the reverb block.
	 * An UpdateReverbParameters() call is required to validate the value change.
	 * \code
	 * CriError err;
	 * CriFloat32 size_meter = 24.0f;
	 * // Set a "pseudo room size" parameter to the reverb block
	 * sndrndr->SetReverbRoomSize(size_meter, err);
	 * // Update reverb parameters
	 * sndrndr->UpdateReverbParameters(err);
	 * \endcode
	 */
	virtual void SetReverbRoomSize(CriFloat32 size_meter, CriError &err = criErr::ErrorContainer, CriFloat32 fluctuation = 1.0f) = 0;
	/*JP
	 * \brief リバーブブロックのフィードバックフィルタの設定
	 * \param	cof_low 低域のカットオフ周波数<br>
	 * \param	cof_high 高域のカットオフ周波数<br>
	 * \param	err エラーコード<br>
	 * \par 説明:
	 * リバーブブロックのフィードバックフィルタを設定します。<br>
	 * UpdateReverbParameters関数を実行すると反映されます。<br>
	 * \code
	 * CriError err;
	 * CriFloat32 cof_low = 30.0f, cof_high = 5000.0f;
	 * // Set a cut off frequency for the feedback filter of the reverb block
	 * sndrndr->SetReverbFilterCutoffFrequency(cof_low, cof_high, err);
	 * // Update reverb parameters
	 * sndrndr->UpdateReverbParameters(err);
	 * \endcode
	 */
	/*EN
	 * \brief Set cut-off frequencies for the feedback filter of the reverb block
	 * \param cof_low Lower cut-off frequency
	 * \param cof_high Higher cut-off frequency
	 * \param err CriError information
	 * \par
	 * This function sets the cut-off frequencies for the feedback filter of the reverb block.
	 * An UpdateReverbParameters() call is required to validate the value change.
	 * \code
	 * CriError err;
	 * CriFloat32 cof_low = 30.0f, cof_high = 5000.0f;
	 * // Set a cut off frequency for the feedback filter of the reverb block
	 * sndrndr->SetReverbFilterCutoffFrequency(cof_low, cof_high, err);
	 * // Update reverb parameters
	 * sndrndr->UpdateReverbParameters(err);
	 * \endcode
	 */
	virtual void SetReverbFilterCutoffFrequency(
		CriFloat32 cof_low, CriFloat32 cof_high, CriError &err = criErr::ErrorContainer, CriFloat32 fluc_l = 1.0f, CriFloat32 fluc_h = 1.0f) = 0;
	/*JP
	 * \brief リバーブブロックの残響時間の設定
	 * \param	reverb_time_msec 残響 (ミリ秒)<br>
	 * \param	err エラーコード<br>
	 * \par 説明:
	 * リバーブブロックの残響時間を設定します。<br>
	 * UpdateReverbParameters関数を実行すると反映されます。<br>
	 * \code
	 * CriError err;
	 * CriFloat32 reverb_time_msec = 3900.0f;
	 * // Set a reverb time for the reverb block
	 * sndrndr->SetReverbDecayTime(reverb_time_msec, err);
	 * // Update reverb parameters
	 * sndrndr->UpdateReverbParameters(err);
	 * \endcode
	 */
	/*EN
	 * \brief Set a reverb time for the reverb block
	 * \param reverb_time_msec Reverb time (in msec)
	 * \param err CriError information
	 * \par
	 * This function sets the reverb time for the reverb block.
	 * An UpdateReverbParameters() call is required to validate the value change.
	 * \code
	 * CriError err;
	 * CriFloat32 reverb_time_msec = 3900.0f;
	 * // Set a reverb time for the reverb block
	 * sndrndr->SetReverbDecayTime(reverb_time_msec, err);
	 * // Update reverb parameters
	 * sndrndr->UpdateReverbParameters(err);
	 * \endcode
	 */
	virtual void SetReverbDecayTime(CriFloat32 reverb_time_msec, CriError &err = criErr::ErrorContainer) = 0;
	/*JP
	 * \brief リバーブパラメータの更新
	 * \param	err エラーコード<br>
	 * \par 説明:
	 * リバーブパラメータを更新します。<br>
	 * \code
	 * CriError err;
	 * sndrndr->SetReverbPredelayTime(5.0f, err);
	 * sndrndr->SetReverbRoomSize(24.0f, err);
	 * sndrndr->SetReverbFilterCutoffFrequency(30.0f, 5000.0f, err);
	 * sndrndr->SetReverbDecayTime(3900.0f, err);
	 * // Update reverb parameters
	 * sndrndr->UpdateReverbParameters(err);
	 * \endcode
	 */
	/*EN
	 * \brief Update reverb parameters
	 * \param err CriError information
	 * \par
	 * This function updates the reverb parameters.
	 * \code
	 * CriError err;
	 * sndrndr->SetReverbPredelayTime(5.0f, err);
	 * sndrndr->SetReverbRoomSize(24.0f, err);
	 * sndrndr->SetReverbFilterCutoffFrequency(30.0f, 5000.0f, err);
	 * sndrndr->SetReverbDecayTime(3900.0f, err);
	 * // Update reverb parameters
	 * sndrndr->UpdateReverbParameters(err);
	 * \endcode
	 */
	virtual void UpdateReverbParameters(CriError &err = criErr::ErrorContainer) = 0;
#ifndef DOXYGEN_SHOULD_SKIP_THIS
	/*JP
	 * \brief 内部用サウンドレンダラオブジェクトの取得(内部関数)<br>
	 * \return 内部用サウンドレンダラオブジェクト
	 * \par 説明:
	 * 内部用サウンドレンダラオブジェクトの取得<br>
	 */
	/*EN
	 * \brief Get an internal sound renderer object
	 * \par
	 * This function returns an internal sound renderer object.
	 */
	virtual CriSrObjMaster* GetSrObjMaster(void) = 0;
#endif
	/*JP
	 * \brief ウェット処理スイッチの設定
	 * \param	sw 処理を行うか否か（ON=1, OFF=0)<br>
	 * \param	err エラーコード<br>
	 * \par 説明:
	 * ウェット処理を行うか否かを設定します。<br>
	 * リバーブなどによるの処理負荷を無くします。<br>
	 * \code
	 * CriError err;
	 * // Set switch for wet send
	 * sndrndr->SetWetSendMasterSwitch(ON, err);
	 * \endcode
	 */
	/*EN
	 * \brief Set a Wet Send Switch
	 * \param sw Switch for wet sending to Effect Blocks<br>
	 * \param err CriError information
	 * \par
	 * This function turns on/off sending wet sound to Effect Blocks.
	 * \code
	 * CriError err;
	 * // Set switch for wet send
	 * sndrndr->SetWetSendMasterSwitch(ON, err);
	 * \endcode
	 */
	virtual void SetWetSendMasterSwitch(CriBool sw, CriError &err = criErr::ErrorContainer) = 0;
	/*JP
	 * \brief ウェット処理スイッチの取得
	 * \param	err エラーコード<br>
	 * \return ウェット処理スイッチ
	 * \par 説明:
	 * ウェット処理スイッチの状態を取得します。<br>
	 * \code
	 * CriError err;
	 * // Get switch for wet send
	 * CriBool sw = sndrndr->GetWetSendMasterSwitch(err);
	 * \endcode
	 */
	/*EN
	 * \brief Get a Wet Send Switch
	 * \param sw Switch for wet sending to Effect Blocks<br>
	 * \param err CriError information
	 * \par
	 * This function returns the status of the wet sending switch.
	 * \code
	 * CriError err;
	 * // Get switch for wet send
	 * CriBool sw = sndrndr->GetWetSendMasterSwitch(err);
	 * \endcode
	 */
	virtual CriBool GetWetSendMasterSwitch(CriError &err = criErr::ErrorContainer) = 0;
	/*JP
	 * \brief ダウンミックスモードの設定
	 * \param mode ダウンミックスモード
	 * \param err エラーコード
	 * \par 説明:
	 * ダウンミックスモードを設定します。<br>
	 * \code
	 * CriError err;
	 * CriSoundRendererBasic::DownmixMode mode = CriSoundRendererBasic::DOWNMIX_STEREO_MIXED_WITH_LFE;
	 * // Set downmix mode
	 * sndrndr->SetDownmixMode(mode, err);
	 * \endcode
	 */
	/*EN
	 * \brief Set downmix mode
	 * \param mode Downmix mode
	 * \param err CriError information
	 * \par
	 * This function sets the downmix mode.
	 * \code
	 * CriError err;
	 * CriSoundRendererBasic::DownmixMode mode = CriSoundRendererBasic::DOWNMIX_STEREO_MIXED_WITH_LFE;
	 * // Set downmix mode
	 * sndrndr->SetDownmixMode(mode, err);
	 * \endcode
	 */
	virtual void SetDownmixMode(DownmixMode mode, CriError &err = criErr::ErrorContainer) = 0;
	/*JP
	 * \brief ダウンミックスモードの取得
	 * \param err エラーコード
	 * \return ダウンミックスモード
	 * \par 説明:
	 * ダウンミックスモードを取得します。<br>
	 * \code
	 * CriError err;
	 * // Get downmix mode
	 * CriSoundRendererBasic::DownmixMode mode = sndrndr->GetDownmixMode(err);
	 * \endcode
	 */
	/*EN
	 * \brief Get downmix mode
	 * \param err CriError information
	 * \return Downmix mode
	 * \par
	 * This function returns the downmix mode.
	 * \code
	 * CriError err;
	 * // Get downmix mode
	 * CriSoundRendererBasic::DownmixMode mode = sndrndr->GetDownmixMode(err);
	 * \endcode
	 */
	virtual DownmixMode GetDownmixMode(CriError &err = criErr::ErrorContainer) = 0;

#ifndef DOXYGEN_SHOULD_SKIP_THIS
	/*JP
	 * \brief マスターエフェクタの追加（実装中）<br>
	 * \par 説明:
	 * マスターエフェクタの追加（実装中）
	 */
	virtual CriBool AddMasterEffector(CriSrEffector* effector, CriError &err = criErr::ErrorContainer) = 0;
	/*JP
	 * \brief マスターエフェクタの削除（実装中）<br>
	 * \par 説明:
	 * マスターエフェクタの削除（実装中）
	 */
	virtual void DeleteMasterEffector(CriSrEffector* effector, CriError &err = criErr::ErrorContainer) = 0;
#endif

protected:
	CriSoundRendererBasic();
	virtual ~CriSoundRendererBasic();
/*JP
 * @} MODULE_CRI_SOUND_RENDERER_LIB
 */
/*EN
 * @} MODULE_CRI_SOUND_RENDERER_LIB
 */
};

#ifndef DOXYGEN_SHOULD_SKIP_THIS
/*JP
 * \brief AUXハンドル
 * \ingroup MODULE_CRI_SOUND_RENDERER_LIB
 *	\par 説明:
 * サウンドレンダラへの外部入力用モジュールです。<br>
 */
/*EN
 * \brief AUX Handle
 * \ingroup MODULE_CRI_SOUND_RENDERER_LIB
 * \par
 */
class CriSrAux : public CriAllocator
{
public:
	/*JP
	 * \brief AUXコールバック関数
	 * \param	obj オブジェクト<br>
	 * \param	nch チャンネル数<br>
	 * \param	nsmpl サンプル数<br>
	 * \param	pcm 出力サウンドデータ格納用のバッファ <br>
	 * \param	smptyp サンプルタイプ<br>
	 * \par 説明:
	 * レンダラがAUXデータを必要とする場合に呼び出すコールバック関数型です。<br>
	 */
	/*EN
	 * \brief Retrieve sound data generated by a sound renderer
	 * \param obj Object for callback function
	 * \param nch Number of channels
	 * \param nsmpl Number of samples
	 * \param smptyp Type of samples
	 * \param a PCM Buffer for output sound data where PCM is an array of <b>nch</b> element
	 * and each element keeps a pointer to a string area for <b>nsmpl</b> floating point samples.
	 * \par
	 */
	enum SampleType {
		FLOAT_32   = (0),
		SINT_16   = (1),
	};
	typedef void (*CriSrAuxCallback) (void *obj, CriUint32 nch, void *sample[], CriUint32 nsmpl, SampleType smptyp);
	enum ChId {
		L   = (0),
		R   = (1),
		Ls  = (2),
		Rs  = (3),
		C   = (4),
		Lfe = (5),
		Ex1 = (6),
		Ex2 = (7),
	};
	enum Status {
		STATUS_STOP = (0),		/*JP< 停止		*/
								/*EN< STOP		*/
		STATUS_PLAYING,			/*JP< 再生中	*/
								/*EN< PLAYING		*/
	};
	/*JP
	 * \brief AUXハンドルの生成
	 * \param heap ヒープハンドル<br>
	 * \param sndrndr サウンドレンダラ<br>
	 * \param nch チャンネル数<br>
	 * \param cbfunc コールバック関数<br>
	 * \param cbobj コールバックオブジェクト<br>
	 * \param err エラーコード<br>
	 * \par 説明:
	 * AUXハンドルを生成します。
	 * \code
	 * void cbfunc(void *obj, CriUint32 nch, void *sample[], CriUint32 nsmpl, SampleType smptyp){}
	 *
	 * CriError err;
	 * CriHeap heap = criHeap_Create(buf, sizeof(buf));
	 * CriSoundRendererBasic* sndrndr = CriSoundRendererBasic::Create(heap, err);
	 * CriUint32 nch = 2;
	 * void* cbobj = NULL;
	 * // Create a AUX handle
	 * CriSrAux* aux = CriSrAux::Create(heap, sndrndr, nch, cbfunc, cbobj, err);
	 * \endcode
	 */
	/*EN
	 * \brief Create a AUX handle
	 * \param heap A CriHeap handle
	 * \param sndrndr Sound Renderer
	 * \param nch Number of channels
	 * \param cbfunc Callback Function
	 * \param cbobj A object for callback function
	 * \param err CriError information
	 * \return A valid cue sheet handle (CriAuCueSheet), or ???
	 * \par
	 * This function creates a CriSrAux handle.
	 * A memory for the handle is allocated from given CriHeap structure.
	 * Any memory allocation failure during this function results ???
	 * Make sure to initialize and create your heap with criHeap_Initialize() and
	 * criHeap_Create() before calling this function.
	 * \code
	 * void cbfunc(void *obj, CriUint32 nch, void *sample[], CriUint32 nsmpl, SampleType smptyp){}
	 *
	 * CriError err;
	 * CriHeap heap = criHeap_Create(buf, sizeof(buf));
	 * CriSoundRendererBasic* sndrndr = CriSoundRendererBasic::Create(heap, err);
	 * CriUint32 nch = 2;
	 * void* cbobj = NULL;
	 * // Create a AUX handle
	 * CriSrAux* aux = CriSrAux::Create(heap, sndrndr, nch, cbfunc, cbobj, err);
	 * \endcode
	 */
	static CriSrAux* Create(CriHeap heap, void* sndrndr, CriUint32 nch, CriSrAuxCallback cbfunc, void *cbobj, CriError &err = criErr::ErrorContainer);
	/*JP
	 * \brief AUXハンドルの削除
	 * \param err エラーコード
	 * \par 説明:
	 * AUXハンドルを削除します。
	 * \code
	 * CriError err;
	 * // Destroy a aux
	 * aux->Destroy(err);
	 * \endcode
	 */
	/*EN
	 * \brief Destroy a aux handle
	 * \param err CriError information
	 * \par
	 * This function destroys the CriSrAux handle previously created
	 * with CriSrAux::Create.
	 * \code
	 * CriError err;
	 * // Destroy a aux
	 * aux->Destroy(err);
	 * \endcode
	 */
	virtual void Destroy(CriError &err = criErr::ErrorContainer) = 0;
	/*JP
	 * \brief AUX再生処理の開始
	 * \param	err エラーコード
	 * \par 説明:
	 * AUXによる再生処理を開始します。
	 * Play関数を呼ぶとStop関数が呼ばれるまでの間、生成時に指定したコールバック関数が定期的に呼ばれます。
	 * \code
	 * CriError err;
	 * // Play a aux
	 * aux->Play(err);
	 * \endcode
	 */
	/*EN
	 */
	virtual void Play(CriError &err = criErr::ErrorContainer) = 0;
	/*JP
	 * \brief AUX再生処理の停止
	 * \param	err エラーコード
	 * \par 説明:
	 * AUXによる再生処理を停止します。
	 * \code
	 * CriError err;
	 * // Stop a aux
	 * aux->Stop(err);
	 * \endcode
	 */
	/*EN
	 */
	virtual void Stop(CriError &err = criErr::ErrorContainer) = 0;
	/*JP
	 * \brief 再生周波数の設定
	 * \param freq 周波数の変更値（単位：ヘルツ）<br>
	 * \param err エラーコード<br>
	 * \return	なし
	 * \par 説明:
	 *	再生周波数の変更値を設定します。<BR>
	 *	単位はヘルツです。<br>
	 * \code
	 * CriError err;
	 * CriFloat32 freq = 22050.0f;
	 * // Set frequency to aux
	 * aux->SetFrequency(freq, err);
	 * \endcode
	 */
	/*EN
	 * \brief Set frequency
	 * \param freq frequency difference
	 * \param err CriError information
	 * \par
	 *	Set frequency difference in Hz.
	 * \code
	 * CriError err;
	 * CriFloat32 freq = 22050.0f;
	 * // Set frequency to aux
	 * aux->SetFrequency(freq, err);
	 * \endcode
	 */
	virtual void SetFrequency(CriFloat32 freq, CriError &err = criErr::ErrorContainer) = 0;
	/*JP
	 * \brief 再生周波数の取得
	 * \param err エラーコード<br>
	 * \return	再生周波数
	 * \par 説明:
	 *	再生周波数を取得します。<BR>
	 *	単位はヘルツです。<br>
	 * \code
	 * CriError err;
	 * // Get frequency of aux
	 * CriFloat32 freq = aux->GetFrequency(err);
	 * \endcode
	 */
	/*EN
	 * \brief Get frequency
	 * \param err CriError information
	 * \return frequency
	 * \par
	 *	Get frequency in Hz.
	 * \code
	 * CriError err;
	 * // Get frequency of aux
	 * CriFloat32 freq = aux->GetFrequency(err);
	 * \endcode
	 */
	virtual CriFloat32 GetFrequency(CriError &err = criErr::ErrorContainer) = 0;
	/*JP
	 * \brief ボリュームの設定
	 * \param volume ボリューム<br>
	 * \param err エラーコード<br>
	 * \return	なし
	 * \par 説明:
	 *	ボリュームを設定します。<BR>
	 * \code
	 * CriError err;
	 * CriFloat32 volume = 0.8f;
	 * // Set volume to aux
	 * aux->SetVolume(volume, err);
	 * \endcode
	 */
	/*EN
	 * \brief Set volume
	 * \param volume Volume
	 * \param err CriError information
	 * \par
	 *	This function sets the volume to the aux.
	 *	The volume value is specified in linear scale.
	 * \code
	 * CriError err;
	 * CriFloat32 volume = 0.8f;
	 * // Set volume to aux
	 * aux->SetVolume(volume, err);
	 * \endcode
	 */
	virtual void SetVolume(CriFloat32 volume, CriError &err = criErr::ErrorContainer) = 0;
	/*JP
	 * \brief ボリュームの取得
	 * \param err エラーコード<br>
	 * \return	ボリューム
	 * \par 説明:
	 *	ボリュームを取得します。<BR>
	 * \code
	 * CriError err;
	 * // Get volume of aux
	 * CriFloat32 volume = aux->GetVolume(err);
	 * \endcode
	 */
	/*EN
	 * \brief Get volume
	 * \param err CriError information
	 * \return volume
	 * \par
	 *	Get volume.
	 * \code
	 * CriError err;
	 * // Get volume of aux
	 * CriFloat32 volume = aux->GetVolume(err);
	 * \endcode
	 */
	virtual CriFloat32 GetVolume(CriError &err = criErr::ErrorContainer) = 0;
	/*JP
	 * \brief AUXのステータス取得
	 * \param	err エラーコード
	 * \return	再生状態
	 * \par 説明:
	 * AUXのステータスを取得します。
	 * ステータスは、作成された直後はSTOP状態です。<br>
	 * Play関数を実行した直後、PLAYINGになります。<br>
	 * \code
	 * CriError err;
	 * // Get status of aux
	 * CriSrAux::Status status = aux->GetStatus(err);
	 * \endcode
	 */
	/*EN
	 * \brief Retrieve a aux status
	 * \param err CriError information
	 * \return	playback status
	 * \par
	 * This function returns the aux status.
	 * The aux status after the creation is STOP.
	 * When Play() is called, the status is PREP and becomes PLAYING.
	 * CriError err;
	 * // Get status of aux
	 * CriSrAux::Status status = aux->GetStatus(err);
	 * \endcode
	 */
	virtual Status GetStatus(CriError &err = criErr::ErrorContainer) = 0;
protected:
	CriSrAux();
	virtual ~CriSrAux();
private:
};

/*JP
 * \brief エフェクタインターフェイス
 * \ingroup MDL_LIB_SOUND_RENDERER
 * \par 説明:
 */
/*EN
 * \brief
 * \ingroup MDL_LIB_SOUND_RENDERER
 */
class CriSrEffector : public CriAllocator
{
public:
	virtual void Destroy(void) = 0;

	virtual CriSrBus* GetInputBus() = 0;
	virtual CriSrBus* GetOutputBus() = 0;

protected:
	CriSrEffector() {}
	virtual ~CriSrEffector() {}

private:
	// 使用不可メンバ
	CriSrEffector(const CriSrEffector&);
	void operator=(const CriSrEffector&);
};

/*JP
 * \brief パラグラフィックイコライザ
 * \ingroup MDL_LIB_SOUND_RENDERER
 * \par 説明:
 */
/*EN
 * \brief
 * \ingroup MDL_LIB_SOUND_RENDERER
 */
class CriSrParagraphicEqualizer : public CriSrEffector
{
public:
	enum FilterType {
		FILTER_TYPE_LOW_SHELF,
		FILTER_TYPE_HIGH_SHELF,
		FILTER_TYPE_PEAKING
	};

	struct ConfigParameter {
		CriUint32 num_bands;
		CriFloat32 sampling_frequency;
	};

	static CriSrParagraphicEqualizer* Create(CriHeap heap, const ConfigParameter& config);
	virtual void Destroy(void) = 0;

	virtual CriSrBus* GetInputBus() = 0;
	virtual CriSrBus* GetOutputBus() = 0;

	virtual void SetParameter(CriUint32 band_index, FilterType ftype, CriFloat32 freq0, CriFloat32 q_coef, CriFloat32 db_gain) = 0;
	virtual void GetParameter(CriUint32 band_index, FilterType& ftype, CriFloat32& freq0, CriFloat32& q_coef, CriFloat32& db_gain) const = 0;

protected:
	CriSrParagraphicEqualizer() {}
	virtual ~CriSrParagraphicEqualizer() {}

private:
	// 使用不可メンバ
	CriSrParagraphicEqualizer(const CriSrParagraphicEqualizer&);
	void operator=(const CriSrParagraphicEqualizer&);
};

#endif	// DOXYGEN_SHOULD_SKIP_THIS

#endif //	_CRI_SOUND_RENDERER_H_INCLUDED

