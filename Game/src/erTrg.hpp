// ============================================================================
/*!
	@file	erTrg.hpp
	@brief	タッチパネルアクション(トリガ)

	@author	Kouji Hokazono <kouji_hokazono@dimps.co.jp>
		Copyright(c) 2009-2010 Dimps
	$Id: erTrg.hpp 2 2011-04-11 05:21:26Z thamada $
 */
// ============================================================================
/*
 * $Log$
 */

/*!
	@page erTrgMain タッチパネルアクション(トリガ)

	@section erTrgSummary 概要
		タッチパネルアクションはタッチパネル制御の上位ライブラリです。
		タッチパネルから入力されたデータを処理し、領域別のアクションを提供します。
		
		ステートの説明
			　　　　　　　領域接触　　　　　　　　領域非接触
			　　　　　　　┌───┐　　　Out 　　┌───┐
			タッチ中　　　│　On　│　　──→　　│ Off　│
			　　　　　　　└───┘　　←──　　└───┘
			　　　　　　Down↑　│　　　　In　　　　　　　　
			　　　　　　　　│　↓Up　　　　　　　　　　　　
			　　　　　　　┌───────────────┐
			未タッチ　　　│　　　　　　　Off 　　　　　　│
			　　　　　　　└───────────────┘
			
			※正確には Off 状態はありません、ここでは On では無い状態の事を指します。
			Stand             …Onになった瞬間に発動(Down || In)
			Release           …Offになった瞬間に発動(Up || Out)
			Repeat            …On中の間に等間隔に発動
			Move              …Lock中に一定量移動すれば発動
			Over              …On中に一定量移動すれば発動
			Lock              …Downにて開始した後、未タッチに成るまで発動
			                  　タッチが継続していれば領域外でも発動
			DragAndDrop       …一度Moveが発動した後、未タッチに成るまで発動
			Click             …Downにて開始し、Upにて終了した瞬間に発動(Lock && Up)
			                  　一旦領域から離れてもDown開始Up終了なら発動
			SingleClick       …Clickが発生し、WCが発動しない事が確定した時に発動
			DoubleClick       …Clickが発生し、一定時間内にDownが発生した時に発動(DoubleClickWait && Down)
			DoubleClickWait   …Clickの後、SingleClickかDoubleClickかを判断している間中発動
			DoubleClickSecond …DoubleClickの後、未タッチに成るまで発動
 */

#pragma once
#if	defined(__cplusplus)
extern "C" {
#endif

//------ C Include Files -------------- インクルード ---------------------------******CIF*
//------ C Macro ---------------------- マクロ ---------------------------------******CMC*
//------ C External Definitions ------- グローバル変数及び関数の宣言 -----------******CED*


#if	defined(__cplusplus)
} // extern "C"
#endif

#if	defined(__cplusplus)
//------ Include ---------------------- インクルード ---------------------------******_IC*
#include "accelMpl.hpp"
#include "accelArray.hpp"
#include "accelBitset.hpp"
#include "accelCircularBuffer.hpp"
#include "accelIntrusiveList.hpp"
#include "erObject.hpp"


//------ Debug ------------------------ デバッグ -------------------------------******_DG*
#if defined(MTD_DEBUG)
#endif	//#if defined(MTD_DEBUG)
//------ Macro ------------------------ マクロ ---------------------------------******_MC*
namespace er {
//------ Class ------------------------ クラス ---------------------------------******_CL*
/*!	@class
	@brief	タッチパネルアクション(トリガ)インターフェース
		タッチパネルアクション(トリガ)のインターフェースを提供します。
 */
class ITrg : public er::object::IRelease, public er::object::IIsValid, public er::object::IUpdate, private accel::intrusive::CListNode<ITrg> {
	//-- Enum Hack -------------------- 定数宣言 -------------------------------******_EH*
protected:
private:
	//-- Public Constant -------------- 公開定数 -------------------------------******PCS*
public:
	//-- Public Variable -------------- 公開変数 -------------------------------******PVA*
	//-- Public Function -------------- 公開関数 -------------------------------******PFC*
	//-- Geter Function --------------- 取得関数 -------------------------------******PGF*
	//-- Seter Function --------------- 設定関数 -------------------------------******PSF*
	//-- Constructor And Destructor --- コンストラクタ・デストラクタ------------******_CD*
	// ============================================================================
	// ITrg::~ITrg
	/*!
		デストラクタ
	 */
	// ============================================================================
public:
	virtual ~ITrg() {};


	//-- Local Constant --------------- ローカル定数 ---------------------------******LCS*
protected:
private:
	//-- Local Variable --------------- ローカル変数 ---------------------------******LVA*
protected:
private:
	//-- Local Function --------------- ローカル関数 ---------------------------******LFC*
protected:
private:
//------------------------------------------------------------------------------**********
}; //class ITrg





































































//------ Class ------------------------ クラス ---------------------------------******_CL*
/*!	@class
	@brief	タッチパネルアクション(トリガ)状態クラス
		タッチパネルアクション(トリガ)の状態機能を提供します。
 */
class CTrgState {
	//-- Enum Hack -------------------- 定数宣言 -------------------------------******_EH*
protected:
private:
	//-- Public Constant -------------- 公開定数 -------------------------------******PCS*
public:
	//フラグ
	struct EState {
		typedef int Type;
		enum {
			On,					//<接触中(有効領域に接触している)
			Prev,				//<１フレーム前の接触状態(1f前は有効領域に接触している)
			Stand,				//<ONエッジ(有効領域に接触した瞬間)
			Release,			//<OFFエッジ(有効領域から離脱した瞬間)
			Click,				//<クリック
			SingleClick,		//<シングルクリック
			DoubleClick,		//<ダブルクリック
			Repeat,				//<リピート
			Down,				//<ダウン
			Move,				//<ムーブ
			Up,					//<アップ
			In,					//<イン(接触した状態で領域進入)
			Over,				//<オーバー(接触した状態で領域進入し、移動)
			Out,				//<アウト(接触した状態で領域離脱)
			Lock,				//<ターゲットロック中
			DragAndDrop,		//<ドラッグ&ドロップ中
			DoubleClickWait,	//<ダブルクリック待ち中
			DoubleClickSecond,	//<ダブルクリックの２回目接触中

			Max,
			None,
		};
	};


	//リピート
	struct ERepeatInterval {
		enum Type {
			First,	//<1回目の繰返間隔
			Second,	//<2回目以降の繰返間隔

			Max,
			None,
		};
	};
	typedef Sint32											TCounter;			//<カウンタ型
	typedef accel::CArray<TCounter, ERepeatInterval::Max>	TRepeatInterval;	//<リピート時間型

	//ダブルクリック
	typedef Sint32				TDoubleClickTime;		//ダブルクリック感知時間型

	//移動
	typedef accel::CArray<Sint32, 2>	TMove;			//移動量
	typedef TMove::value_type			TMoveThreshold;	//移動感知閾値型


	//-- Public Variable -------------- 公開変数 -------------------------------******PVA*
	//-- Public Function -------------- 公開関数 -------------------------------******PFC*
	// ============================================================================
	// CTrgState::Push
	/*!
		状態のプッシュ

		@param	is_on	[in]	ONか
		@param	is_edge	[in]	ONエッジ・もしくはOFFエッジか
		@param	move	[in]	移動量
	 */
	// ============================================================================
	void Push(bool is_on, bool is_edge, const TMove &move = TMove::initializer());

	// ============================================================================
	// CTrgState::operator[]
	/*!
		インデクサ

		@param	kind	[in]	値の種類

		@return 値
	 */
	// ============================================================================
	bool operator[](EState::Type kind) const;

	// ============================================================================
	// CTrgState::AddLock
	/*!
		ロックの付加
	 */
	// ============================================================================
	void AddLock();

	// ============================================================================
	// CTrgState::DelLock
	/*!
		ロックの除去
	 */
	// ============================================================================
	void DelLock();

	// ============================================================================
	// CTrgState::ResetState
	/*!
		状態の初期化

		@note
			リピート間隔・ダブルクリック感知時間・移動感知閾値は初期化されません。
	 */
	// ============================================================================
	void ResetState();


	//-- Geter Function --------------- 取得関数 -------------------------------******PGF*
	// ============================================================================
	// CTrgState::GetRepeatInterval
	/*!
		リピート間隔の取得
	 */
	// ============================================================================
	const TRepeatInterval &GetRepeatInterval() const {
		return m_repeat_interval;
	}

	// ============================================================================
	// CTrgState::GetDoubleClickTime
	/*!
		ダブルクリック感知時間の取得
	 */
	// ============================================================================
	TDoubleClickTime GetDoubleClickTime() const {
		return m_wc_time;
	}

	// ============================================================================
	// CTrgState::GetMoveThreshold
	/*!
		移動感知閾値の取得
	 */
	// ============================================================================
	TMoveThreshold GetMoveThreshold() const {
		return m_move_threshold;
	}

	// ============================================================================
	// CTrgState::GetMove
	/*!
		ムーブ量の取得

		@return	ムーブ量

		@note
			ムーブが発生していない場合は(0,0)を返します。
	 */
	// ============================================================================
	TMove GetMove() const;

	// ============================================================================
	// CTrgState::GetLastMove
	/*!
		前回のムーブ量の取得

		@return	前回のムーブ量
	 */
	// ============================================================================
	const TMove &GetLastMove() const {
		return m_move_report;
	}

	// ============================================================================
	// CTrgState::GetOver
	/*!
		オーバー量の取得

		@return	オーバー量

		@note
			ムーブが発生していない場合は(0,0)を返します。
	 */
	// ============================================================================
	TMove GetOver() const;

	// ============================================================================
	// CTrgState::GetLastOver
	/*!
		前回のオーバー量の取得

		@return	前回のオーバー量
	 */
	// ============================================================================
	const TMove &GetLastOver() const {
		return m_move_report;
	}


	//-- Seter Function --------------- 設定関数 -------------------------------******PSF*
	// ============================================================================
	// CTrgState::SetRepeatInterval
	/*!
		リピート間隔の設定
	 */
	// ============================================================================
	void SetRepeatInterval(const TRepeatInterval &repeat_interval);
	void SetRepeatInterval(TCounter first, TCounter second);
	void SetRepeatInterval() {
		SetRepeatInterval(c_repeat_interval_default);
	}

	// ============================================================================
	// CTrgState::SetDoubleClickTime
	/*!
		ダブルクリック感知時間の設定
	 */
	// ============================================================================
	void SetDoubleClickTime(TDoubleClickTime wc_time);
	void SetDoubleClickTime() {
		SetDoubleClickTime(c_wc_time_default);
	}

	// ============================================================================
	// CTrgState::SetMoveThreshold
	/*!
		移動感知閾値の設定
	 */
	// ============================================================================
	void SetMoveThreshold(TMoveThreshold move_threshold);
	void SetMoveThreshold() {
		SetMoveThreshold(c_move_threshold_default);
	}


	//-- Constructor And Destructor --- コンストラクタ・デストラクタ------------******_CD*
	// ============================================================================
	// CTrgState::CTrgState
	/*!
		デフォルトコンストラクタ
	 */
	// ============================================================================
public:
	CTrgState();


	//-- Local Constant --------------- ローカル定数 ---------------------------******LCS*
protected:
private:
	//状態
	struct BState : public EState {
		typedef accel::CBitset<Max> Type;
	};
	struct ETime {
		enum Type {
			Direct,	//<現在
			Prev,	//<前回

			Max,
			None,
		};
	};
	typedef accel::CCircularBuffer<BState::Type, ETime::Max>	TState;		//<状態型

	static const TCounter c_counter_none = TCounter(-1);	//<カウンタ無効値
	static const TRepeatInterval c_repeat_interval_default;	//リピート標準値
	static const TDoubleClickTime c_wc_time_default;		//ダブルクリック感知時間標準値
	static const TMoveThreshold c_move_threshold_default;	//移動感知閾値標準値


	//-- Local Variable --------------- ローカル変数 ---------------------------******LVA*
protected:
private:
	TState				m_state;			//<状態
	TCounter			m_counter;			//<カウンタ
	TRepeatInterval		m_repeat_interval;	//<リピート間隔
	TDoubleClickTime	m_wc_time;			//<ダブルクリック感知時間
	TMoveThreshold		m_move_threshold;	//<移動感知閾値
	TMove				m_move_accumulate;	//<移動量の蓄積
	TMove				m_move_report;		//<前回ムーブ・オーバーで報告した時の移動量


	//-- Local Function --------------- ローカル関数 ---------------------------******LFC*
protected:
private:
	void updateTime();
	void updateOnPrev(bool is_on);
	void updateEdge(bool is_edge);
	void updateRepeat();
	void updateLock();
	void updateMoveOver(const TMove &move);
	void updateClick();

	void resetMove();
	bool addMove(const TMove &move);



//------------------------------------------------------------------------------**********
}; //class CTrgState





































































//------ Class ------------------------ クラス ---------------------------------******_CL*
/*!	@class
	@brief	タッチパネルアクション(トリガ)状態クラス
		タッチパネルアクション(トリガ)の状態機能を提供します。
 */
class CTrgStateEx : public CTrgState {
	//-- Enum Hack -------------------- 定数宣言 -------------------------------******_EH*
protected:
private:
	//-- Public Constant -------------- 公開定数 -------------------------------******PCS*
public:
	typedef CTrgState TSuperClass;

	//ドラッグ速度
	typedef accel::CArray<float, 2>	TDragSpeed;			//ドラッグ速度



	//-- Public Variable -------------- 公開変数 -------------------------------******PVA*
	//-- Public Function -------------- 公開関数 -------------------------------******PFC*
	// ============================================================================
	// ::Push
	/*!
		状態のプッシュ

		@param	is_on	[in]	ONか
		@param	is_edge	[in]	ONエッジ・もしくはOFFエッジか
		@param	move	[in]	移動量
	 */
	// ============================================================================
	void Push(bool is_on, bool is_edge, const TMove &move = TMove::initializer());

	// ============================================================================
	// CTrgStateEx::ResetState
	/*!
		状態の初期化

		@note
			リピート間隔・ダブルクリック感知時間・移動感知閾値は初期化されません。
	 */
	// ============================================================================
	void ResetState();


	//-- Geter Function --------------- 取得関数 -------------------------------******PGF*
	// ============================================================================
	// CTrgStateEx::GetDragSpeed
	/*!
		ドラッグ速度の取得
	 */
	// ============================================================================
	TDragSpeed GetDragSpeed() const;


	//-- Seter Function --------------- 設定関数 -------------------------------******PSF*
	//-- Constructor And Destructor --- コンストラクタ・デストラクタ------------******_CD*
	// ============================================================================
	// CTrgStateEx::CTrgStateEx
	/*!
		デフォルトコンストラクタ
	 */
	// ============================================================================
public:
	CTrgStateEx();


	//-- Local Constant --------------- ローカル定数 ---------------------------******LCS*
protected:
private:
	static const int c_pos_history = 6;										//<移動の履歴数
	typedef accel::CCircularBuffer<TMove, c_pos_history>	TPosHistory;	//<移動の履歴


	//-- Local Variable --------------- ローカル変数 ---------------------------******LVA*
protected:
private:
	TPosHistory	m_pos_history;	//<移動の履歴


	//-- Local Function --------------- ローカル関数 ---------------------------******LFC*
protected:
private:
//------------------------------------------------------------------------------**********
}; //class CTrgStateEx





































































//------ Class ------------------------ クラス ---------------------------------******_CL*
/*!	@class
	@brief	タッチパネルアクション(トリガ)根底インターフェース
		タッチパネルアクション(トリガ)の根底インターフェースを提供します。
 */
class ITrgBase : public ITrg {
	//-- Enum Hack -------------------- 定数宣言 -------------------------------******_EH*
protected:
private:
	typedef ITrg		TSuperClass;


	//-- Public Constant -------------- 公開定数 -------------------------------******PCS*
public:
	//-- Public Variable -------------- 公開変数 -------------------------------******PVA*
	//-- Public Function -------------- 公開関数 -------------------------------******PFC*
	//-- Geter Function --------------- 取得関数 -------------------------------******PGF*
	//-- Seter Function --------------- 設定関数 -------------------------------******PSF*
	//-- Constructor And Destructor --- コンストラクタ・デストラクタ------------******_CD*
	//-- Local Constant --------------- ローカル定数 ---------------------------******LCS*
protected:
	//フラグ型
	struct BFlag {
		enum {
			Frieze,
			NoHit,

			Max,
			None,
		};
		typedef accel::CBitset<Max> Type;
	};


private:


	//-- Local Variable --------------- ローカル変数 ---------------------------******LVA*
protected:
private:
	//-- Local Function --------------- ローカル関数 ---------------------------******LFC*
protected:
private:
//------------------------------------------------------------------------------**********
}; //class CTrgBase































//------ Class ------------------------ クラス ---------------------------------******_CL*
/*!	@class
	@brief	タッチパネルアクション(トリガ)根底クラス
		タッチパネルアクション(トリガ)の根底機能を提供します。
 */
template<typename TStateType = CTrgState>
class CTrgBase : public ITrgBase {
	//-- Enum Hack -------------------- 定数宣言 -------------------------------******_EH*
protected:
	typedef accel::CArray<Sint32, 2>	TPos;	//タッチ位置


private:
	typedef ITrgBase	TSuperClass;
	typedef TStateType	TState;


	//-- Public Constant -------------- 公開定数 -------------------------------******PCS*
public:
	typedef typename TState::TRepeatInterval	TRepeatInterval;	//<リピート時間型
	typedef typename TState::TDoubleClickTime	TDoubleClickTime;	//ダブルクリック感知時間型
	typedef typename TState::TMoveThreshold		TMoveThreshold;		//移動感知閾値型


	//-- Public Variable -------------- 公開変数 -------------------------------******PVA*
	//-- Public Function -------------- 公開関数 -------------------------------******PFC*
	// =============================================================================
	// CTrgBase::Update
	/*!
		更新
	 */
	// ==========================================================================
	virtual void Update();

	// ============================================================================
	// CTrgBase::ResetState
	/*!
		状態の初期化

		@param	index	[in]	インデックス

		@note
			インデックスが省略された場合は、全て対応します。
	 */
	// ============================================================================
	void ResetState();
	void ResetState(Uint32 index);

	// ============================================================================
	// CTrgBase::AddLock
	/*!
		ロックの付加

		@param	index	[in]	インデックス
	 */
	// ============================================================================
	void AddLock();
	void AddLock(Uint32 index);

	// ============================================================================
	// CTrgBase::DelLock
	/*!
		ロックの除去

		@param	index	[in]	インデックス
	 */
	// ============================================================================
	void DelLock();
	void DelLock(Uint32 index);


	//-- Geter Function --------------- 取得関数 -------------------------------******PGF*
	// ============================================================================
	// CTrgBase::IsFrieze
	/*!
		状態の凍結確認
	 */
	// ============================================================================
	bool IsFrieze() const;

	// ============================================================================
	// CTrgBase::IsNoHit
	/*!
		無接触確認
	 */
	// ============================================================================
	bool IsNoHit() const;

	// ============================================================================
	// CTrgBase::GetState
	/*!
		状態取得

		@param	index	[in]	インデックス

		@note
			インデックスが省略された場合は、
			インデックスの若いロック中の値を返します。
			ロック中の値が無ければインデックスの若いオン中の値を返します。
			ロック中の値が無ければ0番目が返ります。
	 */
	// ============================================================================
	const TState &GetState() const;
	const TState &GetState(Uint32 index) const;

	// ============================================================================
	// CTrgBase::GetRepeatInterval
	/*!
		リピート間隔の取得

		@param	index	[in]	インデックス
	 */
	// ============================================================================
	const TRepeatInterval &GetRepeatInterval() const;

	// ============================================================================
	// CTrgBase::GetDoubleClickTime
	/*!
		ダブルクリック感知時間の取得
	 */
	// ============================================================================
	TDoubleClickTime GetDoubleClickTime() const;

	// ============================================================================
	// CTrgBase::GetMoveThreshold
	/*!
		移動感知閾値の取得
	 */
	// ============================================================================
	TMoveThreshold GetMoveThreshold() const;


	//-- Seter Function --------------- 設定関数 -------------------------------******PSF*
	// ============================================================================
	// CTrgBase::SetFrieze
	/*!
		状態の凍結設定
	 */
	// ============================================================================
	void SetFrieze(bool frieze);

	// ============================================================================
	// CTrgBase::SetNoHit
	/*!
		無接触設定
	 */
	// ============================================================================
	void SetNoHit(bool nohit);

	// ============================================================================
	// CTrgBase::SetRepeatInterval
	/*!
		リピート間隔の設定
	 */
	// ============================================================================
	void SetRepeatInterval(const TRepeatInterval &repeat_interval);
	void SetRepeatInterval(typename TRepeatInterval::value_type first, typename TRepeatInterval::value_type second);
	void SetRepeatInterval();

	// ============================================================================
	// CTrgBase::SetDoubleClickTime
	/*!
		ダブルクリック感知時間の設定
	 */
	// ============================================================================
	void SetDoubleClickTime(TDoubleClickTime wc_time);
	void SetDoubleClickTime();

	// ============================================================================
	// CTrgBase::SetMoveThreshold
	/*!
		移動感知閾値の設定
	 */
	// ============================================================================
	void SetMoveThreshold(TMoveThreshold move_threshold);
	void SetMoveThreshold();


	//-- Constructor And Destructor --- コンストラクタ・デストラクタ------------******_CD*
	//-- Local Constant --------------- ローカル定数 ---------------------------******LCS*
protected:
private:
	//タッチ数
#if !_IPHONE
	static const int c_touch_max = 1;
#else //!_IPHONE
	static const int c_touch_max = 5;
#endif //!_IPHONE

	//フラグ型
	using TSuperClass::BFlag;

	//状態型
	typedef accel::CArray<TState, c_touch_max> TStateArray;

	//タッチ位置型
	typedef accel::CArray<TPos,c_touch_max> TPosArray;


	//-- Local Variable --------------- ローカル変数 ---------------------------******LVA*
protected:
private:
	BFlag::Type	m_flag;		//<フラグ
	TStateArray	m_state;	//<状態
	TPosArray	m_pos;		//<前回位置



	//-- Local Function --------------- ローカル関数 ---------------------------******LFC*
protected:
	// =============================================================================
	// CTrgBase::hitTest
	/*!
		ヒットテスト

		@param	pos		[in]	タッチ位置
		@param	index	[in]	タッチインデックス

		@retval	true	ヒット
		@retval	false	未ヒット
	 */
	// ==========================================================================
	virtual bool hitTest(const TPos &pos, Uint32 index) = 0;


private:
	TState &getState();
	const TState &getState() const;
	TState &getState(Uint32 index);
	const TState &getState(Uint32 index) const;


//------------------------------------------------------------------------------**********
}; //class CTrgBase




















} //namespace er
//------ Template Include ------------- テンプレートインクルード ---------------******_IC*
#include "erTrg.tpp"





#endif //#if	defined(__cplusplus)

	// ============================================================================
	// CTrg::Function
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
	// ============================================================================
