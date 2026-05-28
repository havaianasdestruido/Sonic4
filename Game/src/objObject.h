// ================================================================
/*!
  @file objObject.h
  @brief オブジェクト

  @author mana
                Copyright(c) 2004 Dimps

  $Id: objObject.h 2 2011-04-11 05:21:26Z thamada $
  $Date:: 2011-04-11 14:21:26 +0900#$
 */
// ================================================================
/*
 * Memo
 *
 */
/*!
  @page  obj_object obj オブジェクト
 
  @section  obj_object_plain 解説
    　 このタスクを用いて、ゲーム中の様々なオブジェクトを表現する。\n
    　 これを用いれば、生成->読み込み->表示->操作->判定->消去が容易に作成できる、バグが画一的になる\n
    　 ただし、組み込まれいてないシステムを実装する場合に手間がかかる、簡単なオブジェクトを作る時、メモリが無駄になる\n
    　 
  @section  obj_object_use_init 用法 初期化
    ・システム初期化\n
    　 まず ObjInit() を呼び出し、初期化する。\n
    　 その後、扱うシステムに合わせて、#g_obj.flag にフラグを設定する（カメラ、地形、矩形など）\n
    \n
    \n
    ・タスク作成\n
    　 作成したいオブジェクトの初期化関数で まず ObjObjectTaskInit()を行い、オブジェクトタスクを作成する\n
    　 この時グループや優先度を標準以外にしたい場合は ObjObjectTaskDetailInit() を用いる\n
    　 ObjObjectTaskInit() で作成した場合は、ObjObjectTaskNameSet() でデバッグ時のタスク名を変更できる\n
    　 ObjObjectTypeSet() でオブジェクトの種類を設定する。種類番号はユーザーが任意に設定する。必要なければしなくともよい\n
    \n
    ・タスク各設定\n
    　・拡張メモリ\n
    　　 標準以外に専用のデータを用いたい時、 \n
    　　 ObjObjecExWorkAlloc() でメモリを取得すれば、\n
    　　 タスク死亡時に自動で開放されるメモリ領域を取得できる。\n
    　　 このメモリのポインタは #OBS_OBJECT_WORK.pExWork にも保持される\n
    　　 （自動で開放させたくない場合は #OBS_OBJECT_WORK.ulFlag の #OBD_OBJECT_FREE_EX を寝かせる）\n
    \n
    　・開放処理設定\n
    　　 アクションデータを二つ使用するような特殊な描画を用いる場合など、\n
    　　 mtTaskChangeTcbDestructor() で開放関数を設定しなおす。\n
    　　 タスクポインタは #OBS_OBJECT_WORK.pTcb に設定されている\n
    　　 ここで用意する開放処理の最後に ObjObjectExit() を呼び出す事\n
    \n
    　・アクションデータロード\n
    　　 objObjectActionLoad() でファイルパスを指定してアクションデータを読み込む。\n
    　　 第2引数の #OBS_ACTION2D_WORKポインタはNULLを指定すれば、\n
    　　 自動でメモリを取得する(このメモリは自動で開放される)\n
    　　 ふたつ以上のアクションを表示したい時はユーザ側でメモリを取得し、引数にアドレスを設定する\n
    　　 第4引数の #OBS_DATA_WORK を設定すれば、既に読み込んでいるファイルを共用するようになる\n
    　　 第5引数の アーカイブポインタを設定すれば、アーカイブからファイルを引っ張るようになり、\n
    　　 ワンカートリッジで使用できるようになる。また読み込み時間も無くなる\n
    　　 キャラサイズは #OBD_AUTO_CHARSIZE を指定すれば、自動で最大サイズのVRAMを取得する\n
    　　 \n
    　　 bjObjectPaletteLoad() で使用するパレット番号を動的に取得する。\n
    　　 ObjObjectPrioritySet() でアクションの表示優先度を設定、\n
    　　 ObjObjectBgPrioritySet() でアクションのBG表示優先度を設定する。\n
    　　 設定しない場合は、標準値が設定される。\n
    \n
    　・3Dモデルデータロード\n
    　　 objObjectAction3dModelLoad() でモデルデータを読み込む。\n
    　　 第2引数の #OBS_ACTION3D_NNS_WORKポインタはNULLを指定すれば、\n
    　　 自動でメモリを取得する(このメモリは自動で開放される)\n
    　　 基本的にアクションデータのロードと同じで モデルインデックス値の指定が増えている。\n
    \n
    　　 objObjectAction3dAnimeLoad() を使ってアニメーションデータを読み込む。\n
    　　 第2引数の #OBS_ACTION3D_NNS_WORK にNULLを指定すれば、\n
    　　 objObjectAction3dModelLoad() で取得したメモリを用いる。\n
    \n
    　・3Dスプライトデータロード\n
    　　 objObjectAction3dSpriteLoad() を使ってテクスチャデータを読み込む\n
    　　 第2引数の #OBS_ACTION3D_SPRITE_WORKポインタはNULLを指定すれば、\n
    　　 自動でメモリを取得する(このメモリは自動で開放される)\n
    　　 キャラサイズ、色数には #OBD_AUTO_CHARSIZE、 #OBD_AUTO_PLTSIZE を指定すれば\n
    　　 そのアクションデータの最大サイズが設定される\n
    \n
    　・3D1M1Sデータロード\n
    　　 objObjectAction3dModelSimpleLoad() を使ってモデルデータを読み込む\n
    　　 第2引数の #OBS_ACTION3D_SIMPLE_WORKポインタはNULLを指定すれば、\n
    　　 自動でメモリを取得する(このメモリは自動で開放される)\n
    \n
    　　 この関数を、表示するシェイプ数分呼び出す。\n
    　　 この関数はNULLで複数呼び出した場合、呼び出した数だけ、メモリを保持する。\n
    \n
    　　 モデルインデックス値の他にシェイプ番号とマトリクス番号を指定する。\n
    　　 複数のシェイプで構築されるモデルを表示する場合、\n
    \n
    　・アクション解凍・転送分離式\n
    　　 キャラ(テクスチャ)圧縮タイプのアクションデータを先に解凍し、Vブランク中では\n
    　　 転送のみにする方式です。大量の圧縮タイプアクションを使用する場合に使います。\n
    　　 キャラクタ解凍用のバッファを自動取得します。\n
    \n
    　　 ObjDrawSetAction*****Uncomp() を使って解凍用ワークを取得する。\n
    　　 アクションコールバックを設定し、コールバック内でObjDrawTransActUncomp を呼び出す事\n
    　　 キャラクタサイズの指定には #OBD_AUTO_CHARSIZE の指定が可能\n
    \n
    　・地形判定設定\n
    　　 ObjObjectFieldRectSet() で 地形と判定する矩形サイズを設定する\n
    \n
    　・オブジェクト地形設定\n
    　　 objObjectCollisionSet() で、オブジェクト地形を設定する。\n
    　　 第2引数の #OBS_COLLISION_WORKポインタはNULLを指定すれば、\n
    　　 自動でメモリを取得する(このメモリは自動で開放される)\n
    　　 この後、必要であれば objObjectCollisionDifSet() objObjectCollisionDirSet() \n
    　　 objObjectCollisionAtrSet() でデータファイルを読み込む。\n
    \n
    　・落下設定\n
    　　 ObjObjectFallSet() 落下速度を設定する。\n
    　　 また #OBD_MOVE_FALLフラグ も設定される。\n
    \n
    　・当たり判定設定\n
    　　 ObjObjectRectSet() 
    　　 第2引数の #OBS_RECT_WORKポインタはNULLを指定すれば、\n
    　　 自動でメモリを取得する(このメモリは自動で開放される)\n
    　　 ゲームに合わせて当たりの設定を行う事。\n
    \n
    　・関数設定\n
    　　 #OBS_OBJECT_WORK.ppFunc \n
    　　　 ここにそのオブジェクト特有の処理を行う関数を登録する\n
    　　　 終了チェックや状態移行など\n
    　　 #OBS_OBJECT_WORK.ppIn \n
    　　　 ppFuncの前に行う処理を登録する。\n
    　　　 キー入力など\n
    　　 #OBS_OBJECT_WORK.ppLast \n
    　　　 このオブジェクトの処理の最後に行う処理関数を設定する\n
    　　　 タイマーのカウントなどに用いる\n
    　　 #OBS_OBJECT_WORK.ppOut \n
    　　　 特殊な表示を行うオブジェクトはここに表示関数を登録する\n
    　　　 設定しない場合、標準の表示処理( ObjObjectActionSummary() )が行われる。\n
    　　 #OBS_OBJECT_WORK.ppMove \n
    　　　 特殊な移動を行うオブジェクトはここに移動処理関数を登録する。\n
    　　　 設定しない場合、標準の移動処理( ObjObjectMove() )が行われる。\n
    　　 #OBS_OBJECT_WORK.ppActCall \n
    　　　 アクションコールバックを行う場合、ここに関数を登録する\n
    　　　 矩形データの取得はこれを用いずとも objObjectActionCallBack() で行われる。\n
    　　 #OBS_OBJECT_WORK.ppRec \n
    　　　 特殊な、矩形登録、オブジェクト地形登録を行う場合、ここに処理関数を登録する。\n
    　　　 設定しない場合、オブジェクト内に存在する矩形を登録する。\n
    \n
    　・サウンド設定\n
    　　 SEを扱うオブジェクトの場合 ObjObjectSoundHandleGet() でサウンドハンドルを取得する。\n
    　　 mtSndPlayArcSe でSEを再生する\n
    　　 サウンドハンドルは #OBS_OBJECT_WORK.tSe に収納される。\n
    \n
    　・親設定\n
    　　 親子関係を扱うオブジェクトは ObjObjectParentSet() で親を設定する\n
    \n
    \n
  @section  obj_object_use 用法 操作
    　・位置
    　　 #OBS_OBJECT_WORK.vPos にオブジェクトの座標を設定する。\n
    　　 カメラの設定があれば、この座標からカメラの座標を引いた位置に表示される。\n
    　　 また、#OBS_OBJECT_WORK.vPrevPos に1フレーム前の座標がセットされる\n
    \n
    　　 #OBS_OBJECT_WORK.vOffset に数値を設定すると、その値だけ表示位置がズレる。\n
    　　 この値は毎フレームクリアされる。\n
    \n
    　・表示変更\n
    　　 2Dでも3Dでも ObjObjectActionSet() を使って表示アクションを変更する\n
    　　 #OBS_OBJECT_WORK.ulDispFlag に #OBD_DISP_REPEAT を設定するとアクションが繰り返される。\n
    \n
    　・移動\n
    　　 DEFAULTの移動処理状態であれば、\n
    　　 #OBS_OBJECT_WORK.vSpd に速度を設定すればオブジェクトが移動する\n
    　　 #OBS_OBJECT_WORK.fSpdM に速度を設定した場合、\n
    　　 #OBS_OBJECT_WORK.vDir.z の角度によって移動方向が変化する。( 要 #OBD_MOVE_DIR)\n
    　　 #OBS_OBJECT_WORK.vFlow に速度を設定すると vSpdと同じようにオブジェクトが移動する\n
    　　 ただし、この速度は毎フレームクリアされる、風や、移動床などの設定で用いる。\n
    　　 #OBS_OBJECT_WORK.vMove には移動予定値が入っている。壁反射などに用いる。\n
    \n
    　　 #OBS_OBJECT_WORK.usDirSlopeや #OBS_OBJECT_WORK.fSpdSlopeに数値を設定し、\n
    　　 #OBD_MOVE_SLOPE を立てる事で坂道の追加設定が行える。\n
    　　 これを用いれば、指定以上の角度の坂道でオブジェクトが滑り落ちるようになる。\n
    \n
    　　 #OBS_OBJECT_WORK.usDirFall に数値を設定すれば、重力方向を変更できる。\n
    \n
    　　 objSpdUpSet() objSpdDownSet() objShiftSet() objDiffSet() objAlphaSet() \n
    　　 これらの関数を使って、速度操作を扱うと便利（速度以外にも使える）\n
    
    \n
    　・死亡範囲\n
    　　 カメラの位置（設定が無ければ0） ＋ #OBS_OBJECT_WORK.sViewOutOffset の値 を越える位置へ\n
    　　 オブジェクトが移動した時、自動的にそのオブジェクトは死亡する\n
    　　 範囲外で死亡させたくない場合、#OBD_OBJECT_NOCLIPを立てておく。\n
    　　 また、 #g_obj.flag に #OBD_OBJ_DEFAULT_NOCLIP を立てれば、\n
    　　 オブジェクト初期化時に #OBD_OBJECT_NOCLIPが立てられる\n
    \n
    　　 死亡判定を特殊なものにしたい場合は #OBS_OBJECT_WORK.ppViewCheck に判定関数を登録する\n
    \n
    　・乗る\n
    　　 地形オブジェクトに乗っている時、その地形オブジェクトの移動量の影響を受ける\n
    　　 #OBS_OBJECT_WORK.pRide に乗っているオブジェクトのポインタが設定される。\n
    　　 複数のオブジェクトに乗る状態は未対応\n
    \n
    　・触る、押す\n
    　　 地形オブジェクトに触れている時、そのオブジェクトのポインタを取得できる。\n
    　　 また、設定によっては、そのオブジェクトを押す事ができる\n
    　　 #OBS_OBJECT_WORK.pTouch に触れているオブジェクトのポインタが設定される。\n
    　　 押されるオブジェクトの #OBS_OBJECT_WORK.fPushMax の速度で押す事ができる\n
    　　 また押す事ができるオブジェクトは #OBD_MOVE_PUSH_COL を立てる。\n
    \n
    　・地形判定\n
    　　 ObjObjectFieldRectSet() で地形判定用矩形を設定していれば、自動で判定が行われる\n
    　　 #OBS_OBJECT_WORK.ulMoveFlag の #OBD_MOVE_COL_MASK のフラグをチェックして\n
    　　 判定結果を得る。\n
    　　 また、#OBS_OBJECT_WORK.ulColFlag に地形の属性が設定される。\n
    　　 地形判定を行いたくない場合、 #OBD_MOVE_NOCOL を立てる。\n
    　　 特殊な場合、 #OBD_MOVE_NOCOLOBJ や #OBD_MOVE_NOCOL_W で処理を軽減できる。\n
    　　 #OBD_MOVE_LIMIT_OUT を立てれば、 マップの外を壁として判定できる\n
    　　 #OBD_OBJECT_COL_MAX単位ずつ 地形判定を行う。\n
    　　 移動速度が速いものは判定が重くなるので注意。\n
    \n
    　・振動演出\n
    　　 #OBS_OBJECT_WORK.fVibTimer に数値を設定するとその間、オブジェクトが振動する\n
    　　 設定した値は自動でカウントダウンされる。\n
    \n
    　・回転\n
    　　 #OBS_OBJECT_WORK.vDir.z に数値を設定すると、オブジェクトが回転する。\n
    　　 回転値を用いるが表示を回転させたくない場合、 #OBD_DISP_NODIRを立てておく。\n
    　　 0x10000(= 0x0000) を360°として扱う\n
    \n
    　　 objRoopMove8()、 objRoopMove16() を使って指定の方向へ、じょじょに傾ける事ができる\n
    \n
    　・拡大縮小\n
    　　 #OBS_OBJECT_WORK.vScale に設定した数値に合わせてオブジェクトが拡大縮小表示される\n
    　　 初期化時に 0x1000 がセットされている。\n
    \n
    　・ヒットストップ演出\n
    　　 #OBS_OBJECT_WORK.fHitStopTimer に数値を設定するとその間、オブジェクトの処理が停止する\n
    　　 手応え感を出すのに便利\n
    　　 設定した値は自動でカウントダウンされる。\n
    \n
    　・ポーズ中、動作\n
    　　 ObjObjectPause() を使ってポーズできる。\n
    　　 オブジェクトに #OBD_OBJECT_NOPAUSE を立てておけば、ポーズ中でも\n
    　　 そのオブジェクトは処理が行われる\n
    　　 これを用いて演出などに使う。\n
    　　 ポーズの解除には ObjObjectPauseOut() を呼び出す\n
    \n
    　・タッチパネル 簡易判定\n
    　　 objTouchCheck() を用いれば、 指定の矩形がタッチされたか判定できる\n
    \n
    \n
  @section  obj_object_use_sys 用法 全体操作
    　・ポーズ\n
    　　 #g_obj.flag に #OBD_OBJ_PAUSE をセットすれば、\n
    　　 すべてのオブジェクトがポーズ処理を通るようになる\n
    　　 演出や、ポーズ機能に使用可能\n
    \n
    　・ループ
    　　 #g_obj.flag に #OBD_OBJ_LOOP をセットすれば、\n
    　　 オブジェクトが画面端（もしくはマップ端）を越えた時、位置がループする。\n
    　　 予定、、、未作成。\n
    　　 
    　・オフセット\n
    　　 ObjObjectOffsetSet() で X値、Y値を設定すれば、\n
    　　すべてのオブジェクトがその数値分ズレて表示される\n
    \n
    　・カメラ\n
    　　 ObjObjectCameraSet() でカメラの位置を設定する\n
    　　 オブジェクトの位置が画面外（X:460ドットなど）でも\n
    　　 カメラの位置を合わせれば、画面に表示される。\n
    　　 ただし、 #g_obj.flag に #OBD_OBJ_CAMERA をセットしておく事。\n
    　　 また、 #OBD_OBJ_CAMERA_STICKをセットすれば、\n
    　　 上画面と下画面がくっついた状態で処理される\n
    　　 ObjObjectCameraZSet() で奥行きを設定すれば、拡大縮小もできる。\n
    　　 #g_obj.scaleに直接 数値を設定しても拡縮は可能。\n
    \n
    　・タイマー\n
    　　 _objInit() 移行、 #g_obj.timerがカウントアップされる。\n
    　　 スロー中はゆっくりカウントされるので\n
    　　 #g_obj.flag の #BD_OBJ_TIME_MOVE フラグで、\n
    　　 そのタイムになったタイミングがどうかチェックできる\n
    　・ファイル管理\n
    　　 ObjDataAlloc() で データ管理用の構造体リストを作成\n
    　　 ObjDataFree() でリストを開放、これは objExit()内で実行される\n
    　　 管理用の #OBS_DATA_WORK を取得するには ObjDataGet() で取得する\n
    　　 ゲームシーンごとにデータIDをふったenumを用意し、そのIDに従って使用する\n
    \n
    　・オートスクロール\n
    　　 #g_obj.scroll に速度を設定すると\n
    　　 強制スクロールに対応させられる。\n
    　　 #OBD_MOVE_NO_AUTO_SCROLLを立てると強制スクロールを無視する。\n
    \n
    　・スロー対応\n
    　　ユーザが使用するタイマー関連をすべてobjTimeCountGet() を用いてカウントしていれば\n
    　　ObjObjectSpeedSet() で ゲームをスローにする事ができる。\n
    　　単純なフレーム落ちのスローとは違い、60フレームで処理するため、スローが重要になるゲームで使用できる\n
    \n
    　・ベルトアクション\n
    　　 #g_obj.flag に #OBD_OBJ_BELT をセットすれば、\n
    　　 オブジェクトのZ値がファイナルファイト系のベルトスクロールアクションのように扱われる。\n
    　　 つまり、オブジェクトが奥へ移動すると少し縦に表示がズレるようになる。\n
    　　 当たり矩形のZ値も有効になる事に注意\n
    \n
    \n
    
  @sa objObject.c objObject.h
 */
#ifndef _H_OBJOBJECT
#define _H_OBJOBJECT


//----- Definitions ---------------------------------------------------------
// 使用機能設定
#define OBD_USE_ACTION3D_NN		(1)			//!< NN 3Dモデルを使用する
#if OBD_USE_ACTION3D_NN
#define OBD_USE_ACTION3D_ES		(1)			//!< EffectaStudioエフェクトを使用する
#define OBD_USE_ACTION2D_AMA	(1)			//!< 2D AMA を使用する
#endif
#define OBD_USE_ACTION3D_NNS	(0)			//!< NNS 3Dモデルを使用する
#define OBD_USE_ACTION3D_1M1S	(0)			//!< 1M1S 3Dモデルを使用する
#define OBD_USE_ACTION3D_SPR	(0)			//!< 3Dスプライトを使用する
#define OBD_USE_ACTION2D		(0)			//!< 2Dスプライトを使用する
#define OBD_USE_ACTION3D_SS		(0)			//!< ソフトウェアスプライトを使用する
#define OBD_USE_ACTION3D_POLY	(0)			//!< ポリゴンアクションを使用する
#define OBD_USE_ACTION3D_SMA	(0)			//!< SMAアクションを使用する
#define OBD_USE_TEX_VRAM_DB		(0)			//!< テクスチャVRAMダブルバッファシステムを使用する

#define OBD_USE_RECT_CHECK_DIPTH	(0)		//!< 矩形奥行きチェックあり


//----- Include Files -------------------------------------------------------
#ifndef _DS
#include <alice.h>
#include "typedef.h"
#include "fx.h"
#include "mt.h"
#include "mi.h"
#endif

#if OBD_USE_ACTION2D_AMA
#include "aoAction.h"
#include "aoTexture.h"
#endif

#include "objCollision.h"
#if OBD_USE_ACTION3D_POLY
	#include "izPolyAct.h"
#endif

#if	defined(__cplusplus)
extern "C" {
#endif


//----- Definitions ---------------------------------------------------------
#define OBD_OBJECT_USE_NOEXIST  (1 & _IPHONE) //!< 存在しなくする

#define OBD_TASK_COLOR			(MTD_PLT_COLOR_DARK_YELLOW)		//!< デバック用オブジェクト関連処理基本カラー
#define OBD_TASK_OBJ_COLOR		(MTD_PLT_COLOR_SKY)				//!< デバック用オブジェクト関連処理基本カラー
//


#define OBD_TASK_GROUP_SYSTEM	( 0 )								///< オブジェクトシステム標準タスクグループ番号
#define OBD_TASK_PRIO_SYSTEM	( MTD_TASK_PRIORITY_TAIL-2 )		///< オブジェクトシステム標準タスク優先
#define OBD_TASK_PRIO_OBJECT	( 0x1000 )	///< オブジェクト標準プライオリティ
#define OBD_TASK_GROUP_OBJECT	( 1 )		///< オブジェクト標準グループ
#define OBD_OBJ_PRIORITY		( 16 )		///< オブジェクト標準 表示優先度
#define OBD_OBJ_BG_PRIORITY		(  2 )		///< オブジェクト標準 BG優先度
//#define OBD_OBJ_RECT_NUM		(  6 )		///< ひとつのオブジェクトがもつ矩形ポインタの数
#define OBD_OBJ_RECT_MAX		( 32 )		///< ひとつのオブジェクトに設定する矩形情報最大数

// #define OBD_OBJ_FALL_SPD       ( 0x002a)  // オブジェクト標準落下速度
// #define OBD_OBJ_FALL_SPDMA     ( 0x0f00)  // オブジェクト標準落下最大速度

#define OBD_LCD_X				( g_obj.lcd_size[MTD_X] )	///< 画面横サイズ
#define OBD_LCD_Y				( g_obj.lcd_size[MTD_Y] )	///< 画面縦サイズ


/* イベント生成範囲 */
#if !_DS
#define OBD_OBJ_CLIP_LCD_X		(g_obj.clip_lcd_size[MTD_X])
#define OBD_OBJ_CLIP_LCD_Y		(g_obj.clip_lcd_size[MTD_Y])
#endif	// #if !_DS


/// u16のベクター型
typedef struct tag_VecU16 {
    u16 x;
    u16 y;
    u16 z;

} VecU16;

///< u32のベクター型
typedef struct tag_VecU32 {
    u32 x;
    u32 y;
    u32 z;

} VecU32;

#if defined OBD_USE_ACTION3D_NN
typedef struct tag_OBS_LIGHT {
	NNF_LIGHTTYPE	light_type;			//!< 設定するライトタイプ

	// ライトパラメータ
	union {
		NNS_LIGHT_PARALLEL		parallel;
		NNS_LIGHT_POINT			point;
		NNS_LIGHT_TARGET_SPOT	target_spot;
		NNS_LIGHT_ROTATION_SPOT	rotation_spot;
#if _WII
		NNS_LIGHT_SPECULAR_GC	specular_gc;
#endif
		u32						light_param;	// データ参照用
	};
} OBS_LIGHT;
#endif	// #if defined OBD_USE_ACTION3D_NN

/// オブジェクト設定 ユーザが改めて作る必要はない
typedef struct tag_OBS_OBJECT
{
	VecFx32	glb_scale;					///< オブジェクト拡大率 全体スケール (拡大中心等に影響)
	VecFx32	draw_scale;					///< オブジェクト拡大率 描画スケール (描画時の単純スケール)
	s16		offset[MTD_XY];				///< オブジェクト表示オフセット値
	fx32	speed;						///< オブジェクト処理速度
	fx32	scroll[MTD_XY];				///< オブジェクトオートスクロール
	fx32	depth;						///< ベルト時の奥行き
	u32		timer;						///< オブジェクトタイマー
	fx32	timer_fx;					///< オブジェクトタイマー
	u32		flag;						///< オブジェクトフラグ
	s16		lcd_size[MTD_XY];			///< 画面サイズ

#ifndef _DS
	s16		clip_lcd_size[MTD_XY];		///< イベント生成範囲用
#endif

	s32		pause_level;				///< オブジェクトポーズレベル

#if defined _DS
	fx32	camera[MTE_GE2_MAX][MTD_XY];		///< 2Dカメラ座標
#else
	float	disp_width;		///< 表示解像度
	float	disp_height;	///< 表示解像度
	fx32	camera[2/*◆暫定*/][MTD_XY];		///< 2Dカメラ座標
	fx32	clip_camera[MTD_XY];				///< クリッピング, イベント生成範囲基点
#endif
#if defined _DS
	const MTS_UTIL_CAMERA_LOOKAT	*camera3d;	///< 3Dカメラ情報
#endif
	void	(*pp3dCam)(fx32*, fx32*);			///< 3Dオブジェクトカメラ関数 fx32*, fx32* カメラ座標取得バッファアドレス
#if defined _DS
	s16		cam_scale_center[MTE_GE2_MAX][MTD_XY];	///< 拡大時描画位置拡大中心
#else
	s16		cam_scale_center[2/*◆暫定*/][MTD_XY];	///< 拡大時描画位置拡大中心
#endif
#ifndef _DS
	s32		glb_camera_id;					///< -1で無効 取得するobjCameraのID
	NNE_PROJECTION_TYPE	glb_camera_type;	///< カメラタイプ
#endif

#if OBD_USE_ACTION3D_NN
	NNF_DRAWOBJ		load_drawflag;			///< NNライブラリ用描画フラグ ロード時設定
	NNF_DRAWOBJ		drawflag;				///< NNライブラリ用描画フラグ
#if _PS3 | _XBOX | _PC
	NNS_RGB			toon_rim_param;			///< トゥーン用リムライトパラメータ
	float			toon_camouflage;		///< トゥーン用迷彩
#endif
#endif	// #if OBD_USE_ACTION3D_NN

	// ライト
#if defined _DS
	VecFx16	light_dir[4];				///< ライトベクトル
	u16		light_num;					///< ライト数
	u16		vram_map_mode;				///< キャラクタOBJのVRAMマッピングモード
#endif // #if defined _DS

#if defined OBD_USE_ACTION3D_NN
	NNS_RGB		ambient_color;				///< アンビエントカラー設定
	OBS_LIGHT	light[NNE_LIGHT_MAX];		///< 設定ライトパラメータ
	u32			def_user_light_flag;		///< 標準使用ライト(オブジェクト初期化時に設定するライト)
#if _WII
	NNS_VECTOR	toon_light_vec;				///< Wii専用 トゥーンライト
#endif // #if _WII
#endif // #if defined OBD_USE_ACTION3D_NN

    // 地形判定用設定
	s8		col_through_dot;			///< 地形すり抜け判定の高さ（ドット単位）

	// データ管理
	struct _OBS_DATA_WORK	*pData;		///< データ管理ワークバッファ
	s32		data_max;					///< データ管理ワーク最大数

	// オブジェクト管理
	struct _OBS_OBJECT_WORK	*obj_list_head;		///< オブジェクト管理リスト先頭
	struct _OBS_OBJECT_WORK	*obj_list_tail;		///< オブジェクト管理リスト末尾

	// 描画用オブジェクト管理
	struct _OBS_OBJECT_WORK	*obj_draw_list_head;		///< 描画時用オブジェクト管理リスト先頭
	struct _OBS_OBJECT_WORK	*obj_draw_list_tail;		///< 描画時用オブジェクト管理リスト末尾

	// システム関数
	void	(*ppPre)(void);									///< objMain システム前処理
	void	(*ppPost)(void);								///< objMain システム後処理
	void	(*ppDrawSort)(void);							///< 描画用登録オブジェクトソート処理
	void	(*ppCollision)(struct _OBS_OBJECT_WORK*);		///< 地形判定処理
	void	(*ppObjPre)(struct _OBS_OBJECT_WORK*);			///< ObjObjectMain 共通前処理
	void	(*ppObjPost)(struct _OBS_OBJECT_WORK*);			///< ObjObjectMain 共通後処理
	void	(*ppRegRecAuto)(struct _OBS_OBJECT_WORK*);		///< 矩形自動登録処理

	// 処理高速化用ワーク
	VecFx32	scale;						///< オブジェクト拡大率 glb_scale * draw_scale 通常参照用
	VecFx32	inv_scale;					///< scale の逆数
	VecFx32	inv_glb_scale;				///< glb_scale の逆数 glb_scale更新時に一緒に更新する事
	VecFx32	inv_draw_scale;				///< draw_scale の逆数

#if OBD_USE_TEX_VRAM_DB
	// テクスチャVRAM ダブルバッファ
	MTS_TASK_TCB	*db_tex_tcb;		///< テクスチャVRAM ダブルバッファシステム用 TCB
	GXVRamTex		db_tex_bank[2];		///< 使用するVRAMバンク (GX_VRAM_A, GX_VRAM_B, GX_VRAM_C, GX_VRAM_D)
	u16		db_tex_db_slot_flag;		///< ダブルバッファ使用するスロットフラグ
	u8		db_tex_vram_flip;			///< テクスチャダブルバッファ参照 フリップフラグ (^0x01 が 直接転送可能領域)
	u8		db_tex_slot_num;			///< テクスチャVRAM スロット数
	u32		db_tex_slot_at_lcdc[2][3];	///< テクスチャVRAM LCDCアドレス ダブルバッファ使用時最大割り当て数は3つまで
#endif // #if OBD_USE_TEX_VRAM_DB

	// デバック用
#if defined (MTD_DEBUG)
	u32				register_obj_num;	///< オブジェクト管理リスト登録中オブジェクト数
#endif // #if defined (MTD_DEBUG)
} OBS_OBJECT;

// g_obj.flag オブジェクトグローバルフラグ
#define OBD_OBJ_PAUSE        ( 1 << 0 ) ///< オブジェクトポーズ用フラグ
#define OBD_OBJ_PAUSE_START  ( 1 << 1 ) ///< オブジェクトポーズ用フラグ
//#define OBD_OBJ_LOOP         ( 1 << 2 ) ///< 端でループする
#define OBD_OBJ_CAMERA       ( 1 << 3 ) ///< カメラあり (オブジェクト毎にカメラ設定をする場合に判定)

#define OBD_OBJ_COL_BLOCK    ( 1 << 4 ) ///< ブロック地形あり
#define OBD_OBJ_COL_DIFF     ( 1 << 5 ) ///< 差分地形あり
#define OBD_OBJ_COLMAP       ( OBD_OBJ_COL_BLOCK | OBD_OBJ_COL_DIFF) ///< 当たり地形あり

#define OBD_OBJ_RECT         ( 1 << 6 ) ///< 当たり矩形登録あり
#define OBD_OBJ_RECT_D       ( 1 << 7 ) ///< 当たり矩形表示
#define OBD_OBJ_RECTF_D      ( 1 << 8 ) ///< 地形矩形表示
#define OBD_OBJ_FULL3D       ( 1 << 9 ) ///< フル3D

#define OBD_OBJ_BELT         ( 1 <<10 ) ///< PosZをベルトスクロール方式の扱いにする
#define OBD_OBJ_CAMERA_STICK ( 1 <<11 ) ///< 上下のカメラがくっついている
#define OBD_OBJ_TIME_MOVE    ( 1 <<12 ) ///< 小数点を無視してタイマーが進んだ時に立つフラグ

#define OBD_OBJ_VRAM_AB      ( 1 <<13 ) ///< 上下画面で同じようにVRAMを取得しアクションが行ききできるようにする。
#define OBD_OBJ_VRAM_B       ( 1 <<14 ) ///< OBD_OBJ_VRAM_ABが立っていない時、VRAMの取得をB側のみ行う

#define OBD_OBJ_HS_FUNC_STOP   ( 1 <<15 ) ///< ヒットストップ中、処理関数を実行しない
#define OBD_OBJ_DEFAULT_NOCLIP ( 1 <<16 ) ///< DEFAULTでNOCLIPを立てる
#define OBD_OBJ_RECT_JUSTFRAME ( 1 <<17 ) ///< 当たり矩形ソートを先に行う（判定が1フレーム遅れない）

#define OBD_OBJ_RECT_D_NOVRAM_A ( 1 <<18 ) ///< デバッグ表示用矩形スプライトのグラフィックエンジンAのVRAM取得を行わない
#define OBD_OBJ_RECT_D_NOVRAM_B ( 1 <<19 ) ///< デバッグ表示用矩形スプライトのグラフィックエンジンBのVRAM取得を行わない

#define OBD_OBJ_USE_TEX_VRAM_DB	( 1 <<20 )	//< テクスチャVRAM ダブルバッファシステム使用

#define OBD_OBJ_HS_COL_STOP   ( 1 <<21 ) ///< ヒットストップ中、地形チェックを実行しない

#define OBD_OBJ_RECT_NOUSE_DRAWSCALE	(1 << 22)	///< 矩形判定時、描画スケールを反映しない

//#define OBD_OBJ_FORCE_PAUSE        ( 1 << 22 ) ///< 強制オブジェクトポーズ用フラグ OBD_OBJECT_NOPAUSE不可ポーズ
//#define OBD_OBJ_FORCE_PAUSE_START  ( 1 << 23 ) ///< 強制オブジェクトポーズ用フラグ OBD_OBJECT_NOPAUSE不可ポーズ

//#define OBD_OBJ_ACTER_FORCE_PAUSE        ( 1 << 24 ) ///< 演出用強制オブジェクトポーズ用フラグ OBD_OBJECT_NOPAUSE不可ポーズ
//#define OBD_OBJ_ACTER_FORCE_PAUSE_START  ( 1 << 25 ) ///< 演出用強制オブジェクトポーズ用フラグ OBD_OBJECT_NOPAUSE不可ポーズ

#define OBD_OBJ_SAVE_DATAWORK	(1 << 30)		///< Exit時にデータワークを開放しない Init時に退避したデータワークをセットする
#define OBD_OBJ_EXIT_WAIT		(1 << 31)		///< オブジェクトシステム終了処理待機



// g_obj.def_user_light_flag	標準使用ライトフラグ
// 設定したライトを標準設定ライトとして、オブジェクト初期化時に設定します。
// また、復帰するライトとして使用します。
#define OBD_LIGHT_USE_FLAG_0		(1 << 0)
#define OBD_LIGHT_USE_FLAG_1		(1 << 1)
#define OBD_LIGHT_USE_FLAG_2		(1 << 2)
#define OBD_LIGHT_USE_FLAG_3		(1 << 3)
#define OBD_LIGHT_USE_FLAG_4		(1 << 4)
#define OBD_LIGHT_USE_FLAG_5		(1 << 5)
#define OBD_LIGHT_USE_FLAG_6		(1 << 6)
#define OBD_LIGHT_USE_FLAG_7		(1 << 7)




// =============================================================================
// DS用 描画オブジェクト
// =============================================================================
#if defined _DS
/// g_obj.vram_map_mode キャラクタOBJのVRAMマッピングモード
typedef enum tag_OBE_OBJ_VRAM_MMODE {
	OBD_OBJ_VRAM_MMODE_32	= 0,		///< 先頭キャラクタ境界を32バイト
	OBD_OBJ_VRAM_MMODE_64,				///< 先頭キャラクタ境界を64バイト
	OBD_OBJ_VRAM_MMODE_128,				///< 先頭キャラクタ境界を128バイト
	OBD_OBJ_VRAM_MMODE_256,				///< 先頭キャラクタ境界を256バイト

	OBD_OBJ_VRAM_MMODE_MAX
} OBE_OBJ_VRAM_MMODE;

/// アクション 特別設定タイプ
typedef enum tag_OBE_OBJ_ACTION_SP_SETTING_TYPE {
	OBE_OBJ_ACTION_SP_SETTING_TYPE_UNCOMP	= 0,	///< 圧縮キャラクター解等・転送分離
	OBE_OBJ_ACTION_SP_SETTING_TYPE_TEXVRAM_DB,		///< テクスチャVRAM ダブルバッファシステム

	OBE_OBJ_ACTION_SP_SETTING_TYPE_MAX
} OBE_OBJ_ACTION_SP_SETTING_TYPE;

/// アクション 特別設定用ワーク
typedef struct tag_OBS_ACTION_SP_SETTING_WORK {
	u32					sp_setting_type;	///< 設定情報タイプ
} OBS_ACTION_SP_SETTING_WORK;

/// 圧縮キャラクター解凍・転送分離用ワーク
typedef struct tag_OBS_ACTION_UNCOMP_WORK {
	OBS_ACTION_SP_SETTING_WORK	sp_setting;
	MTE_CHA_VRAM_TYPE	cha_vram;			///< VRAMタイプ保存
	u32					cha_addr;			///< VRAMアドレス保存
	void				*cha_uncomp;		///< キャラクター(テクスチャ)解凍用ワーク
	u32					cha_size;			///< キャラクター(テクスチャ)サイズ (バイト)
} OBS_ACTION_UNCOMP_WORK;

#if OBD_USE_TEX_VRAM_DB
/// テクスチャVRAM ダブルバッファシステム用ワーク
typedef struct tag_OBS_ACTION_TEXVRAM_DB_WORK {
	OBS_ACTION_SP_SETTING_WORK	sp_work;
	MTE_CHA_VRAM_TYPE	cha_vram;			///< VRAMタイプ保存
	u32					cha_addr;			///< VRAMアドレス保存
	u32					ofst_addr;			///< VRAMオフセットアドレス
	u16					slot_no;			///< 転送VRAMスロット
	u16					trans_flag;			///< 更新時転送済みフラグ
} OBS_ACTION_TEXVRAM_DB_WORK;
#endif // #if OBD_USE_TEX_VRAM_DB

#if OBD_USE_ACTION3D_NNS
/// オブジェクト3Dモデル表示ワーク
typedef struct tag_OBS_ACTION3D_NNS_WORK {
	MTS_ACTION3D_NNS		act_3d;										///< 3Dオブジェクト用ワーク
	void					*model;										///< モデルファイルアドレス
	void					*anime[MTE_ACT3D_NNS_ANIM_MAX];				///< アニメファイルアドレス
	struct _OBS_DATA_WORK	*model_data_work;							///< モデルファイルの共有チェックアドレス
	struct _OBS_DATA_WORK	*anime_data_work[MTE_ACT3D_NNS_ANIM_MAX];	///< アニメファイルの共有チェックアドレス
	u16						act_id;										///< アニメーションID
	u32						flag;										///< データフラグ
} OBS_ACTION3D_NNS_WORK;
#endif

#if OBD_USE_ACTION3D_1M1S
/// オブジェクト3D1M1S表示ワーク
typedef struct tag_OBS_ACTION3D_SIMPLE_WORK {
	MTS_ACTION3D_NNS_1M1S				act_s3d;			///< アニメ無し3Dオブジェクト用ワーク
	struct tag_OBS_ACTION3D_SIMPLE_WORK	*next;				///< 他パーツアドレス

	void								*model;				///< モデルファイルアドレス
	struct _OBS_DATA_WORK				*model_data_work;	///< モデルファイルの共有チェックアドレス
	u32									flag;				///< データフラグ
} OBS_ACTION3D_SIMPLE_WORK;
#endif

#if OBD_USE_ACTION3D_SPR
/// オブジェクト3Dスプライト表示ワーク
typedef struct tag_OBS_ACTION3D_SPRITE_WORK
{
    MTS_ACTION3D_SPRITE		act_3dspr;				///< 3Dスプライトオブジェクト用ワーク
    void					*bac;					///< アニメファイルの共有チェックアドレス
    struct _OBS_DATA_WORK	*bac_data_work;			///< アニメファイルの共有チェックアドレス
    struct _OBS_DATA_WORK	*tex_vram_data_work;	///< 共用VRAM アドレス tex 通常は使用しない
    struct _OBS_DATA_WORK	*plt_vram_data_work;	///< 共用VRAM アドレス tex_plt 通常は使用しない
    u32						flag;					///< データフラグ

	OBS_ACTION_UNCOMP_WORK	*act_uncomp;		///< テクスチャ解凍・転送分離用ワーク
} OBS_ACTION3D_SPRITE_WORK;
#endif

#if OBD_USE_ACTION2D
/// オブジェクト2Dスプライト表示ワーク
typedef struct tag_OBS_ACTION2D_WORK
{
	MTS_ACTION_DS			act_spr;			///< アクションワーク
	struct _OBS_DATA_WORK	*bac_data_work;		///< データファイルのアドレス
	struct _OBS_DATA_WORK	*vram_data_work;	///< 共用VRAM AB アドレス 通常は使用しない
	u32						flag;				///< データフラグ

	OBS_ACTION_UNCOMP_WORK	*act_uncomp;		///< テクスチャ解凍・転送分離用ワーク
} OBS_ACTION2D_WORK;
#endif

#if OBD_USE_ACTION3D_SS
/// オブジェクトソフトウェアスプライト表示ワーク
typedef struct tag_OBS_ACTION3D_SS_WORK {
	MTS_ACTION_SS			act_ss;
    void					*bac;				///< アニメファイルデータアドレス
    struct _OBS_DATA_WORK	*bac_data_work;		///< アニメファイルの共有チェックアドレス
    struct _OBS_DATA_WORK	*tex_vram_data_work;	///< 共用VRAM アドレス tex 通常は使用しない
    struct _OBS_DATA_WORK	*plt_vram_data_work;	///< 共用VRAM アドレス tex_plt 通常は使用しない
    u32						flag;				///< データフラグ

	OBS_ACTION_UNCOMP_WORK	*act_uncomp;		///< テクスチャ解凍・転送分離用ワーク

#if OBD_USE_TEX_VRAM_DB
	OBS_ACTION_TEXVRAM_DB_WORK	*act_texdb;		///< テクスチャVRAM ダブルバッファ用ワーク
#endif // #if OBD_USE_TEX_VRAM_DB
} OBS_ACTION3D_SS_WORK;
#endif // #if OBD_USE_ACTION3D_SS


#if OBD_USE_ACTION3D_POLY
/// オブジェクト ポリゴンアクション表示ワーク
typedef struct tag_OBS_ACTION3D_POLY_WORK {
	IZS_PLA_ACTION			act_poly;
	void					*plm;					///< アニメファイルデータアドレス
	struct _OBS_DATA_WORK	*plm_data_work;			///< アニメファイルの共有チェックアドレス
//    struct _OBS_DATA_WORK	*tex_vram_data_work;	///< 共用VRAM アドレス tex 通常は使用しない
//    struct _OBS_DATA_WORK	*plt_vram_data_work;	///< 共用VRAM アドレス tex_plt 通常は使用しない
	u32						flag;					///< データフラグ

//	OBS_ACTION_UNCOMP_WORK	*act_uncomp;		///< テクスチャ解凍・転送分離用ワーク

} OBS_ACTION3D_POLY_WORK;

#endif // #if OBD_USE_ACTION3D_POLY
#endif // #if defined _DS


// =============================================================================
// SMA
// =============================================================================
#if OBD_USE_ACTION3D_SMA
/// オブジェクト3DSMA表示ワーク
typedef struct tag_OBS_ACTION3D_SMA_WORK {
	MTS_SMA					*act_sma;
//	fx32					frame;				///< 現在の再生フレーム数
//	fx32					frame_max;			///< 現在のアクションの最大フレーム数

	void					*smm;				///< モーションデータアドレス
	void					*smg;				///< テクスチャデータアドレス
	void					*smp;				///< パレットデータアドレス
	void					*smc;				///< あたりデータアドレス
//	void					*smr;				///< ユーザーデータアドレス
	struct _OBS_DATA_WORK	*smm_data_work;		///< SMM 共有データチェックアドレス
	struct _OBS_DATA_WORK	*smg_data_work;		///< SMG 共有データチェックアドレス
	struct _OBS_DATA_WORK	*smp_data_work;		///< SMP 共有データチェックアドレス
	struct _OBS_DATA_WORK	*smc_data_work;		///< SMC 共有データチェックアドレス
//	struct _OBS_DATA_WORK	*smr_data_work;		///< SMR 共有データチェックアドレス
    struct _OBS_DATA_WORK	*tex_vram_data_work;	///< 共用VRAM アドレス tex 通常は使用しない
    struct _OBS_DATA_WORK	*plt_vram_data_work;	///< 共用VRAM アドレス tex_plt 通常は使用しない
	u32						flag;				///< データフラグ

	OBS_ACTION_UNCOMP_WORK	*act_uncomp;		///< テクスチャ解凍・転送分離用ワーク
} OBS_ACTION3D_SMA_WORK;

/// オブジェクトSMA オブジェクトシステム内擬似アクションコールバック用コマンドワーク
typedef struct tag_OBS_SMA_COMMAND {
	u32		cmd_id;			//!< コマンドID
	u32		addr;			//!< データアドレス
} OBS_SMA_COMMAND;
typedef enum tag_OBE_SMA_COMMAND_TYPE {
	OBE_SMA_COMMAND_RECT	= 0,	//!< 矩形コマンド
	OBE_SMA_COMMAND_VALUE,			//!< パラメータコマンド

	OBE_SMA_COMMAND_MAX
} OBE_SMA_COMMAND_TYPE;

/// オブジェクトSMA用 アクションコールバック関数定義
//typedef void(*MTF_ACT_CMD_CB)(const MTS_ACT_COMMAND*, MTS_ACTION*, u32);
typedef void(*OBF_SMA_CMD_CB)(const OBS_SMA_COMMAND*, MTS_SMA*, u32);
#endif // #if OBD_USE_ACTION3D_SMA


// =============================================================================
// NN描画オブジェクト PC PS3 XBOX360 Wii
// =============================================================================
#if OBD_USE_ACTION3D_NN

/* シェーダープロファイル */
#if (_WII || _IPHONE)
#define GSD_SHADER_USER_PROFILE_ID_TOON			(0)			//!< トゥーン
#define NND_DRAWOBJ_SHADER_USER_PROFILE_TOON	(0)

#else
#define GSD_SHADER_USER_PROFILE_ID_TOON			(1)			//!< トゥーン
#define NND_DRAWOBJ_SHADER_USER_PROFILE_TOON	(NND_DRAWOBJ_SHADER_USER_PROFILE(GSD_SHADER_USER_PROFILE_ID_TOON))

#endif

#define OBD_ACTION3D_NN_MTN_BUF_NUM			(2)
#define OBD_ACTION3D_NN_DEF_BLEND_SPD		(1.0f/4)
#define OBD_ACTION3D_NN_MTN_FILENAME_LEN	(64)

/// モーションロード設定ワーク
typedef struct tag_OBS_ACTION3D_MTN_LOAD_SETTING {
	BOOL					enable;				///< BOOL TRUE : 設定有効
	BOOL					marge;				///< BOOL 並列補間使用の有無
	struct _OBS_DATA_WORK	*data_work;			///< 3Dモーションデータワーク
	char					filename[OBD_ACTION3D_NN_MTN_FILENAME_LEN];		///< 3Dモーションデータファイル名
	s32						index;				///< 3DモーションデータAMBインデックス
	void					*archive;				///< モーションデータを含むアーカイブ
} OBS_ACTION3D_MTN_LOAD_SETTING;
#endif	// #if OBD_USE_ACTION3D_NN


/// オブジェクト3Dモデル表示ワーク
typedef struct tag_OBS_ACTION3D_NN_WORK {
	NNS_OBJECT				*object;									///< 3Dオブジェクト用ワーク
//	NNS_TEXFILELIST			*texfilelist;
	NNS_TEXLIST				*texlist;
	void					*texlistbuf;
	AMS_MOTION				*motion;				// ◆要統一？
	//AMS_MOTION				*mat_motion;			///< マテリアルモーション

	void					*model;					///< モデルデータ
	struct _OBS_DATA_WORK	*model_data_work;		///< モデルデータワーク

	void					*mtn[AMD_MOTION_FILE_MAX];				///< モーションデータ
	struct _OBS_DATA_WORK	*mtn_data_work[AMD_MOTION_FILE_MAX];	///< モーションデータワーク

	void					*mat_mtn[AMD_MOTION_FILE_MAX];				///< マテリアルモーションデータ
	struct _OBS_DATA_WORK	*mat_mtn_data_work[AMD_MOTION_FILE_MAX];	///< マテリアルモーションデータワーク

	u32						command_state;			///< 描画コマンド発行時のステート 標準:OBD_DRAW_CMD_STATE_3DNN

	u32						flag;									///< フラグ

	float					marge;					///< モーション並列補間率  mbuf[0] 0.0f ･･･ 1.0f mbuf[1]
	float					per;					///< モーション直列補間率  0.0f ～ 1.0f

	s32						act_id[OBD_ACTION3D_NN_MTN_BUF_NUM];			///< アクションID
	float					frame[OBD_ACTION3D_NN_MTN_BUF_NUM];				///< モーションフレーム
	float					speed[OBD_ACTION3D_NN_MTN_BUF_NUM];				///< モーション速度

	s32						mat_act_id;
	float					mat_frame;
	float					mat_speed;

	NNS_MATRIX				user_obj_mtx;			///< ユーザー計算オブジェクトMATRIX
	NNS_MATRIX				user_obj_mtx_r;			///< ユーザー計算オブジェクトMATRIX（右から乗算用）

	float					blend_spd;				///< モーションブレンド速度

	NNF_SUBOBJTYPE			sub_obj_type;			///< サブオブジェクトタイプ
	NNF_DRAWOBJ				drawflag;				///< オブジェクト描画フラグ

	AMS_DRAWSTATE			draw_state;				///< 描画時設定ステータス

	u32						use_light_flag;			///< 使用ライトフラグ OBD_LIGHT_USE_FLAG_***

#if _PS3 | _XBOX | _PC
	NNS_RGB					toon_rim_param;			///< トゥーン用リムライトパラメータ
	float					toon_camouflage;		///< トゥーン用迷彩
#endif

#if _WII
	NNS_VECTOR				toon_light;				///< トゥーンライト
#endif

	// user_func, user_param について
	//	NN描画は、描画スレッドで行われます。
	//	描画時にユーザーが追加処理を行う場合は、user_funcに処理関数を、
	//	user_paramに処理関数が使用するパラメーターバッファを設定する必要があります。
	//	user_func は一度設定するとユーザーが変更しない限り変更されません。
	//	user_param は 描画を行う度に amDrawMallocDataBuffer で取得し、値を設定する必要があります。
	//	amDrawMallocDataBuffer で取得したバッファは解放する必要はありません。
	//	描画を行った後は、user_param はNULLでクリアされます。
	//	再度描画を行う場合は、バッファを取得しなおして下さい。
	//	user_func がユーザーパラメーターを必要としない場合は、
	//	user_paramを取得する必要はありません。

	void					(*user_func)(void*);	//!< ユーザー処理関数
	void					*user_param;			//!< ユーザーパラメーター

	//! マトリックスパレットコールバック
	//  描画スレッドにて、マトリックスパレットが生成された直後に呼ばれるコールバックです。
	//  mplt_cb_funcは一度設定されると、ユーザ以外によっては変更されません。
	//  mplt_cb_param は描画を行うたびにクリアされるため、amDrawMallocDataBuffer()で取得・設定してください。
	//  マトリックスパレットにはビュー行列が掛かっているため、ワールド座標系での姿勢を取得したい場合は
	//  amDrawGetWorldViewMatrix()の逆行列を左から掛けてください。
	// CB関数の型：OBF_DRAW_3DNN_MPLT_CB_FUNC
	void	(*mplt_cb_func)(NNS_MATRIX*, const NNS_OBJECT*, void*);		//!< マトリックスパレットCB関数
	void					*mplt_cb_param;			//!< マトリックスパレットCBパラメータ
	
	//! モーションコールバック
	// メインスレッドにて、モーションが確定した後（描画前）に呼ばれるコールバックです。
	//  mtn_cb_funcは一度設定されると、ユーザ以外によっては変更されません。
	//  mtn_cb_param はメインヒープから取得したバッファを用いて問題ありません。（勝手にクリアされません。）
	//  ノードのワールド座標系での姿勢を取得したい場合は、
	//  TRSリストからマトリックスリストを生成し、さらにマトリックスリストから
	//  マトリックスパレットを生成してください。
	//  （nnCalcMatrixPaletteTRSListを使わないでください）
	//  TODO：描画・メインの両スレッドでnnCalcMatrixPaletteTRSList()を使用していると停止する。
	// CB関数の型：OBF_DRAW_3DNN_MOTION_CB_FUNC
	void	(*mtn_cb_func)(const AMS_MOTION*, const NNS_OBJECT*, void*);	//!< モーションCB関数（メインスレッド）
	void					*mtn_cb_param;			//!< モーションCBパラメータ

	//! マテリアルコールバック
	//	描画時にnnDrawObjectから呼ばれるコールバックです。
	//	マテリアルコールバック関数を設定するとnnPutMaterialCore関数(標準マテリアルコールバック)は
	//	マテリアルコールバック関数で登録した関数内部で実行しない限り実行されません。
	//	material_cb_func は一度設定するとユーザーが変更しない限り変更されません。
	//	material_cb_param は 描画を行う度に amDrawMallocDataBuffer で取得し、値を設定する必要があります。
	//	amDrawMallocDataBuffer で取得したバッファは解放する必要はありません。
	//	描画を行った後は、material_cb_param はNULLでクリアされます。
	NNE_BOOL	(*material_cb_func)(NNS_DRAWCALLBACK_VAL*, void*);	//!< マテリアルCB関数
	void		*material_cb_param;									//!< マテリアルCBパラメータ
	
//	void					*model;										///< モデルファイルアドレス
//	void					*anime[MTE_ACT3D_NNS_ANIM_MAX];				///< アニメファイルアドレス
//	struct _OBS_DATA_WORK	*model_data_work;							///< モデルファイルの共有チェックアドレス
//	struct _OBS_DATA_WORK	*anime_data_work[MTE_ACT3D_NNS_ANIM_MAX];	///< アニメファイルの共有チェックアドレス

	s32						reg_index;				///< データロードチェック用インデックス

	// モーションロード待機
	OBS_ACTION3D_MTN_LOAD_SETTING	mtn_load_setting[AMD_MOTION_FILE_MAX];
	OBS_ACTION3D_MTN_LOAD_SETTING	mat_mtn_load_setting[AMD_MOTION_FILE_MAX];	///< マテリアルモーション用
} OBS_ACTION3D_NN_WORK;


#if (OBD_USE_ACTION3D_ES)

/// オブジェクトESエフェクト表示ワーク
typedef struct tag_OBS_ACTION3D_ES_WORK {
	
	/*
	 *  直接amEffectDelete()を使ってエフェクト(ECB)を破棄しないでください。
	 */
	
	AMS_AME_ECB				*ecb;
	
	NNS_TEXLIST				*texlist;
	void					*texlistbuf;
	/// 共有テクスチャリスト データワーク 生成済みのテクスチャリストを共有する場合のみ使用
	struct _OBS_DATA_WORK	*texlist_data_work;	
	
	NNS_OBJECT				*object;				///< NNSオブジェクト
	/// 共有オブジェクト データワーク 生成済みのオブジェクトを共有する場合のみ使用
	struct _OBS_DATA_WORK	*object_data_work;
	
	void					*eff;					///< ESエフェクトデータ
	struct _OBS_DATA_WORK	*eff_data_work;			///< ESエフェクトデータワーク
	
	void					*ambtex;				///< ESテクスチャAMBデータ
	struct _OBS_DATA_WORK	*ambtex_data_work;		///< ESテクスチャAMBデータワーク
	
	void					*model;					///< ESモデルデータ
	struct _OBS_DATA_WORK	*model_data_work;		///< ESモデルデータワーク
	
	u32						flag;
	
	Uint32					command_state;			///< 描画コマンド発行時のステート 標準:OBD_DRAW_CMD_STATE_3DNN
	
	VecU16					disp_rot;				///< 表示角度（X→Y→Zの順。オブジェクト自体の角度には影響しません）
	Uint16					reserved[1];
	AMS_VECTOR				disp_ofst;				///< 表示オフセット座標（オブジェクト自体の座標には影響しません）
	
	AMS_VECTOR				dup_draw_ofst;			///< 複製描画用オフセット座標（通常描画位置からこの値分オフセットした位置に描画する）
	
	AMS_QUAT				user_dir_quat;			///< ユーザ角度クォータニオン（OBS_OBJECT_WORK::dirの右に掛けるクォータニオン）
	
	s32						user_attr;				///< ユーザー属性(AME_AME_USER_ATTRIBUTE)
	
	s32						tex_reg_index;			///< テクスチャデータロードチェック用インデックス
	s32						model_reg_index;		///< モデルデータロードチェック用インデックス
	
	Float					speed;					///< 描画更新速度
} OBS_ACTION3D_ES_WORK;

#endif /* OBD_USE_ACTION3D_ES */

// OBS_ACTION3D_NN_WORK::flag
#define OBD_ACTFLAG_3D_NN_BLEND			(1 <<  0)	///< アクション切り替え時のモーションブレンド	user r-
#define OBD_ACTFLAG_3D_NN_BG_ANM		(1 <<  1)	///< 補間反映率0のモーションバッファも再生を行う


#define OBD_ACTFLAG_3D_NN_ARCHIVE_MATMTN_0	(1 << 12)	///< 3Dマテリアルモーション ファイル0 アーカイブリソース使用
#define OBD_ACTFLAG_3D_NN_ARCHIVE_MATMTN_1	(1 << 13)	///< 3Dマテリアルモーション ファイル1 アーカイブリソース使用
#define OBD_ACTFLAG_3D_NN_ARCHIVE_MATMTN_2	(1 << 14)	///< 3Dマテリアルモーション ファイル2 アーカイブリソース使用
#define OBD_ACTFLAG_3D_NN_ARCHIVE_MATMTN_3	(1 << 15)	///< 3Dマテリアルモーション ファイル3 アーカイブリソース使用
#define OBD_ACTFLAG_3D_NN_ARCHIVE		(1 << 16)	///< モデル(アクション) アーカイブリソース使用
#define OBD_ACTFLAG_3D_NN_ARCHIVE_MTN_0	(1 << 17)	///< 3Dモーション ファイル0 アーカイブリソース使用
#define OBD_ACTFLAG_3D_NN_ARCHIVE_MTN_1	(1 << 18)	///< 3Dモーション ファイル1 アーカイブリソース使用
#define OBD_ACTFLAG_3D_NN_ARCHIVE_MTN_2	(1 << 19)	///< 3Dモーション ファイル2 アーカイブリソース使用
#define OBD_ACTFLAG_3D_NN_ARCHIVE_MTN_3	(1 << 20)	///< 3Dモーション ファイル3 アーカイブリソース使用

#define OBD_ACTFLAG_3D_NN_RELEASE_WAIT		(1 << 27)	///< 3Dモデル解放待ち
#define OBD_ACTFLAG_3D_NN_REG_MATMTN_WAIT	(1 << 28)	///< 3Dマテリアルモーションデータロード待機
#define OBD_ACTFLAG_3D_NN_REG_MTN_WAIT		(1 << 29)	///< 3Dモーションデータロード待機
#define OBD_ACTFLAG_3D_NN_REG_FINISH		(1 << 30)	///< 3Dデータロード終了
#define OBD_ACTFLAG_3D_NN_REG_WAIT			(1 << 31)	///< データロード終了チェック待ち

// OBS_ACTION3D_ES_WORK::flag
#define OBD_ACTFLAG_3D_ES_POSITION_EMITTER	(1 << 0)	///< 配置にエミッター自身の回転・平行移動を用います（オフの場合は行列による座標変換を用います）
#define OBD_ACTFLAG_3D_ES_PARTICLE_DEPEND	(1 << 1)	///< POSITION_EMITTERオンの時に、パーティクルがエミッタ座標についていくようにする（打ちっぱなしにならない）
#define OBD_ACTFLAG_3D_ES_Z_AXIS_FLIP		(1 << 2)	///< Z軸回転を利用してHフリップを行います
#define OBD_ACTFLAG_3D_ES_SCALE_BY_MTX		(1 << 3)	///< 拡大縮小に行列を用いる（POSITION_EMTの場合は正常に反映されません）
/// POSITION_EMITTERの場合にESデータの回転情報を適用する
/// （このフラグを立てなかった場合はプログラム側で設定された回転で上書きされます。
/// このフラグを立てた状態でさらにプログラム側で回転関連の設定を行うことは非推奨。
/// プログラム側で回転を設定せず、データ側で設定した回転設定そのままで表示するという使用方法を想定しています。）
#define OBD_ACTFLAG_3D_ES_EMT_USE_DATA_ROT	(1 << 4)
#define OBD_ACTFLAG_3D_ES_USER_DIR_QUAT		(1 << 5)	///< ユーザ指定のクォータニオンをOBS_OBJECT_WORK::dirの右に掛ける
#define OBD_ACTFLAG_3D_ES_DUPLICATE_DRAW	(1 << 6)	/// 複製描画OBS_ACTION3D_ES_WORK::dup_draw_ofstのオフセット位置にもう一度描画する

#define OBD_ACTFLAG_3D_ES_EFF_ARCHIVE		(1 << 16)	///< ESエフェクトアーカイブリソース使用
#define OBD_ACTFLAG_3D_ES_AMBTEX_ARCHIVE	(1 << 17)	///< ESテクスチャAMB アーカイブリソース使用
#define OBD_ACTFLAG_3D_ES_MODEL_ARCHIVE		(1 << 18)	///< ESモデル アーカイブリソース使用

#define OBD_ACTFLAG_3D_ES_REG_TEX_WAIT		(1 << 30)	///< ESテクスチャデータロード終了チェック待ち
#define OBD_ACTFLAG_3D_ES_REG_MODEL_WAIT	(1 << 31)	///< ESモデルデータロード終了チェック待ち

// OBS_ACTION*****::flag
#if 0
// OBS_ACTION3D_NNS_WORK
// OBS_ACTION3D_SIMPLE_WORK
// OBS_ACTION3D_SPRITE_WORK
// OBS_ACTION3D_SS_WORK
// OBS_ACTION3D_POLY_WORK
#define OBD_ACTFLAG_3D_ARCHIVE			( 1 << 0 )	///< モデル(アクション) アーカイブリソース使用

// OBS_ACTION3D_NNS_WORK
#define OBD_ACTFLAG_3D_ARCHIVE_CA	( 1 << 1 )	///< OBS_ACTION3D_NNS_WORK用 ジョイントアニメ アーカイブリソース使用
#define OBD_ACTFLAG_3D_ARCHIVE_MA	( 1 << 2 )	///< OBS_ACTION3D_NNS_WORK用 マテリアルアニメ アーカイブリソース使用
#define OBD_ACTFLAG_3D_ARCHIVE_TA	( 1 << 3 )	///< OBS_ACTION3D_NNS_WORK用 テクスチャパターンアニメ アーカイブリソース使用
#define OBD_ACTFLAG_3D_ARCHIVE_TP	( 1 << 4 )	///< OBS_ACTION3D_NNS_WORK用 テクスチャSRT アニメアーカイブリソース使用
#define OBD_ACTFLAG_3D_ARCHIVE_VA	( 1 << 5 )	///< OBS_ACTION3D_NNS_WORK用 ビジビリティアニメ アーカイブリソース使用

// OBS_ACTION3D_SIMPLE_WORK
#define OBD_ACTFLAG_3D_ROT_NOCOPY	( 1 << 6 )	///< OBS_ACTION3D_SIMPLE_WORK用 このパーツは回転情報をコピーしない
#define OBD_ACTFLAG_3D_NODISP		( 1 << 7 )	///< OBS_ACTION3D_SIMPLE_WORK用 このパーツは非表示

// OBS_ACTION3D_SMA_WORK
#define OBD_ACTFLAG_SMA_ARCHIVE_SMM	( 1 << 8 )	///< OBS_ACTION3D_SMA_WORK用 SMMデータ アーカイブリソース使用
#define OBD_ACTFLAG_SMA_ARCHIVE_SMG	( 1 << 9 )	///< OBS_ACTION3D_SMA_WORK用 SMGデータ アーカイブリソース使用
#define OBD_ACTFLAG_SMA_ARCHIVE_SMP	( 1 << 10 )	///< OBS_ACTION3D_SMA_WORK用 SMPデータ アーカイブリソース使用
#define OBD_ACTFLAG_SMA_ARCHIVE_SMC	( 1 << 11 )	///< OBS_ACTION3D_SMA_WORK用 SMCデータ アーカイブリソース使用

// OBS_ACTION2D_WORK
#define OBD_ACTFLAG_2D_ARCHIVE		( 1 << 12 )	///< アクション アーカイブリソース使用


// OBS_ACTION2D_WORK
// OBS_ACTION3D_SPRITE_WORK
// OBS_ACTION3D_SS_WORK
// OBS_ACTION3D_SMA_WORK
#define OBD_ACTFLAG_FREE_UNCOMP		( 1 << 16 )	///< タスク終了時に解凍・転送分離ワークを開放する

// OBS_ACTION3D_SS_WORK
#define OBD_ACTFLAG_FREE_TEXDB		( 1 << 17 )	///< タスク終了時にテクスチャVRAMダブルバッファワークを開放する

#endif


// =============================================================================
// 2D アクション(AMA) 描画オブジェクト PC PS3 XBOX360 Wii
// =============================================================================
#if OBD_USE_ACTION2D_AMA

/// オブジェクト2D表示ワーク
typedef struct tag_OBS_ACTION2D_AMA_WORK {
	u32						flag;					///< フラグ

	AOS_ACTION				*act;
	//AOS_SPRITE				*spr;

	AOS_TEXTURE				ao_tex;
	NNS_TEXLIST				*texlist;
	//void					*texlistbuf;

	void					*ama;				///< AMAデータ
	struct _OBS_DATA_WORK	*ama_data_work;		///< AMAデータワーク

	u32						act_id;				///< アクションID
	//s32						node_id;			///< アクションID
	float					frame;				///< モーションフレーム
	float					speed;				///< モーション速度


	s32						type_node;			///< ノードタイプ

	AOS_ACT_COL				color;				//!< カラー
	AOS_ACT_COL				fade;				//!< フェードカラー

} OBS_ACTION2D_AMA_WORK;


// OBS_ACTION2D_AMA_WORK::flag
#define OBD_ACTFLAG_2D_AMA_REG_TEX_FINISH	(1 << 29)	///< テクスチャ登録終了
#define OBD_ACTFLAG_2D_AMA_REG_TEX_WAIT		(1 << 30)	///< テクスチャデータロード待機
#define OBD_ACTFLAG_2D_AMA_ARCHIVE			(1 << 31)	///< AMA アーカイブリソース使用


#if 0
#define OBD_ACTFLAG_3D_NN_ARCHIVE_MATMTN_0	(1 << 12)	///< 3Dマテリアルモーション ファイル0 アーカイブリソース使用
#define OBD_ACTFLAG_3D_NN_ARCHIVE_MATMTN_1	(1 << 13)	///< 3Dマテリアルモーション ファイル1 アーカイブリソース使用
#define OBD_ACTFLAG_3D_NN_ARCHIVE_MATMTN_2	(1 << 14)	///< 3Dマテリアルモーション ファイル2 アーカイブリソース使用
#define OBD_ACTFLAG_3D_NN_ARCHIVE_MATMTN_3	(1 << 15)	///< 3Dマテリアルモーション ファイル3 アーカイブリソース使用
#define OBD_ACTFLAG_3D_NN_ARCHIVE		(1 << 16)	///< モデル(アクション) アーカイブリソース使用
#define OBD_ACTFLAG_3D_NN_ARCHIVE_MTN_0	(1 << 17)	///< 3Dモーション ファイル0 アーカイブリソース使用
#define OBD_ACTFLAG_3D_NN_ARCHIVE_MTN_1	(1 << 18)	///< 3Dモーション ファイル1 アーカイブリソース使用
#define OBD_ACTFLAG_3D_NN_ARCHIVE_MTN_2	(1 << 19)	///< 3Dモーション ファイル2 アーカイブリソース使用
#define OBD_ACTFLAG_3D_NN_ARCHIVE_MTN_3	(1 << 20)	///< 3Dモーション ファイル3 アーカイブリソース使用

#define OBD_ACTFLAG_3D_NN_RELEASE_WAIT		(1 << 27)	///< 3Dモデル解放待ち
#define OBD_ACTFLAG_3D_NN_REG_MATMTN_WAIT	(1 << 28)	///< 3Dマテリアルモーションデータロード待機
#define OBD_ACTFLAG_3D_NN_REG_MTN_WAIT		(1 << 29)	///< 3Dモーションデータロード待機
#define OBD_ACTFLAG_3D_NN_REG_FINISH		(1 << 30)	///< 3Dデータロード終了
#define OBD_ACTFLAG_3D_NN_REG_WAIT			(1 << 31)	///< データロード終了チェック待ち
#endif


#endif // #if OBD_USE_ACTION2D_AMA


/// オブジェクト地形チェック用構造体
typedef struct tag_OBS_COLLISION_OBJ {
    struct _OBS_OBJECT_WORK * obj;      ///< 登録したオブジェクトワークポインタ NULL可
    struct _OBS_OBJECT_WORK * rider_obj;    ///< 乗っているオブジェクト  NULL可
    struct _OBS_OBJECT_WORK * toucher_obj;  ///< 接触しているオブジェクト  NULL可

    VecFx32           pos;     ///< 1:19:12 親からのオフセット位置（親の設定が無ければ直座標）
    s16               ofst_x;   ///< オフセットX値 1:15
    s16               ofst_y;   ///< オフセットY値 1:15
    
    u32               flag;   ///< フラグ #OBD_COLOBJ_HFLIP など
    u16               dir;    ///< 角度
    u16               attr;   ///< 属性

    s8*               diff_data;    ///< 地形差分データ先頭アドレス
    u8*               dir_data;     ///< 地形角度データ先頭アドレス（キャラごとの角度）
    u8*               attr_data;    ///< 地形属性データ先頭アドレス（キャラごとの属性）
    u16               width;  ///< 地形幅   ドット単位
    u16               height; ///< 地形高さ ドット単位

    // 計算用データ
    VecFx32           check_pos;   ///< 計算用ワーク 1:19:12、objCollisionObjectRegistにvPosと親の座標から設定される、
    s16               check_ofst_x; ///< 計算用ワーク チェックオフセットX値 1:15
    s16               check_ofst_y; ///< 計算用ワーク チェックオフセットY値 1:15
    s32               left;        ///< 計算用ワーク
    s32               top;         ///< 計算用ワーク
    s32               right;       ///< 計算用ワーク
    s32               bottom;      ///< 計算用ワーク
    u16               check_dir;  ///< 計算用ワーク チェック角度
} OBS_COLLISION_OBJ;

/// OBS_COLLISION_OBJ::flag
#define OBD_COLOBJ_HFLIP		( 1 << 0 )	///< 左右反転してチェック
#define OBD_COLOBJ_VFLIP		( 1 << 1 )	///< 上下反転してチェック
#define OBD_COLOBJ_DIR			( 1 << 2 )	///< 角度に合わせて地形回転を行う
#define OBD_COLOBJ_DIR_FLIP		( 1 << 3 )	///< 角度をフリップ

#define OBD_COLOBJ_NOPOS_PARENT	( 1 << 4 )	///< 親の座標を無視する
#define OBD_COLOBJ_NODIR_PARENT	( 1 << 5 )	///< 親の角度を無視する
#define OBD_COLOBJ_NODIR		( 1 << 6 )	///< dirを無効にする
#define OBD_COLOBJ_NOATTR		( 1 << 7 )	///< attrを無効にする

#define OBD_COLOBJ_NOHIT		( 1 << 8 )	///< チェックを行わない

#define OBD_COLOBJ_NOFREE_DIFF_DATA	( 1 << 27 )	///< diff_data を開放しない
#define OBD_COLOBJ_NOFREE_DIR_DATA	( 1 << 28 )	///< dir_data を開放しない
#define OBD_COLOBJ_NOFREE_ATTR_DATA	( 1 << 29 )	///< attr_data を開放しない

#define OBD_COLOBJ_SYS_HFLIP	( 1 << 30 )	///< システム用 左右反転チェックフラグ
#define OBD_COLOBJ_SYS_VFLIP	( 1 << 31 )	///< システム用 上下反転チェックフラグ 







/// オブジェクト地形ワーク
typedef struct _OBS_COLLISION_WORK
{
    OBS_COLLISION_OBJ		obj_col;			///< 地形ワーク
    struct _OBS_DATA_WORK * diff_data_work;		///< 地形データファイルのアドレス
    struct _OBS_DATA_WORK * dir_data_work;		///< 角度データファイルのアドレス
    struct _OBS_DATA_WORK * attr_data_work;		///< 属性データファイルのアドレス
} OBS_COLLISION_WORK;




/// オブジェクト構造体
typedef struct _OBS_OBJECT_WORK
{
	/* システム設定部 ユーザーの変更禁止 */
	// リンク
	struct _OBS_OBJECT_WORK	*prev;		///< オブジェクト管理用リンク 前オブジェクトアドレス
	struct _OBS_OBJECT_WORK	*next;		///< オブジェクト管理用リンク 後オブジェクトアドレス

	struct _OBS_OBJECT_WORK	*draw_prev;		///< 描画管理用リンク 前オブジェクトアドレス
	struct _OBS_OBJECT_WORK	*draw_next;		///< 描画管理用リンク 後オブジェクトアドレス

    /// タスクポインタ
    MTS_TASK_TCB *tcb;					///< オブジェクトのTCBポインタ 設定必須

	s32		pause_level;				///< ポーズレベル

	/* システム設定部 ここまで */

    // オブジェクトシステム関連
    u16  obj_type;		///< ワーク情報ID

    fx32 vib_timer;			///< 振動タイマ、自動で数値は減少する。
    fx32 hitstop_timer;		///< ヒットストップタイマ、ここに値が入っている時、アニメが進まない、移動が行われない、自動で数値は減少する。
    fx32 invincible_timer;	///< 無敵タイマー、自動で数値は減少する。
    
    s16 view_out_ofst;	///< 死亡範囲オフセット 1:15 
    s16 view_out_ofst_plus[4]; ///< 死亡範囲オフセット 1:15 LEFT,TOP,RIGHT,BOTTOM

    // フラグ関連
    u32 flag;			///< オブジェクトフラグ
    u32 move_flag;		///< 移動関連フラグ
    u32 disp_flag;		///< 表示関連フラグ
	u32	gmk_flag;		///< ギミック関連フラグ
	u32	sys_flag;		///< システムフラグ

    // ワーク関連
    u32 user_flag;		///< ユーザ使用フラグ 自由に用いる
    u32 user_work;		///< ユーザ用ワーク 自由に用いる
    s32 user_timer;		///< ユーザ用タイマ 自由に用いる

    // 表示設定関連
    VecU16  dir;			///< オブジェクト角度
    VecFx32 scale;			///< 拡大率 1:19:12
    VecFx32 pos;			///< オブジェクト座標 1:19:12
    VecFx32 ofst;			///< 表示オフセット 1:19:12
    VecFx32 prev_ofst;		///< 表示オフセットコピー 1:19:12
    VecFx32 parent_ofst;	///< 親オフセット 1:19:12
    VecFx32 lock_ofst;		///< ロックオフセット 1:19:12

//	u16  dirz_buff[3];		///< オブジェクトZ角度履歴(45度問題対策用)
    // 移動関連
    VecFx32 prev_pos;		///< 前フレーム座標 1:19:12
    VecFx32 spd;			///< 移動速度 1:19:12
    VecFx32 spd_add;		///< 移動加速度 1:19:12
    // VecFx32 vSpdDown; ///< 移動減速度 1:19:12
    VecFx32 flow;			///< 自動移動速度 1:19:12
    VecFx32 move;			///< 今フレーム予定移動値 1:19:12
    fx32    spd_m;			///< マスタースピード ulMoveFlag & OBD_MOVE_DIRであれば、この値を角度に合わせてXとYに分解して移動する

    u16 dir_slope;			///< 坂道判定角度
    u16 dir_fall;			///< 重力方向角度（0x00で下）

    fx32 spd_slope;			///< 坂道加速度 1:19:12
    fx32 spd_slope_max;		///< 坂道加速度 1:19:12

    fx32 spd_fall;			///< 落下加速度 1:19:12
    fx32 spd_fall_max;		///< 落下最大速度 1:19:12

    fx32 push_max;			///< オブジェクト押し最大速度 1:19:12

    u32 col_flag;			///< 地面属性フラグ 崖やダメージ床など
    u32 col_flag_prev;		///< 地面属性フラグ 1フレーム前データ
    
    s16  field_rect[MTD_RECT];		///< 地形判定用レクト (標準地形当り判定用)
	// objDiffCollisionDirWidthCheck チェック用補正値
	s8	field_ajst_w_db_f;		///< 移動方向下 前方向チェック補正値 標準値 2
	s8	field_ajst_w_db_b;		///< 移動方向下 下方向チェック補正値 標準値 4
	s8	field_ajst_w_dl_f;		///< 移動方向左 前方向チェック補正値 標準値 2
	s8	field_ajst_w_dl_b;		///< 移動方向左 下方向チェック補正値 標準値 4
	s8	field_ajst_w_dt_f;		///< 移動方向上 前方向チェック補正値 標準値 2
	s8	field_ajst_w_dt_b;		///< 移動方向上 下方向チェック補正値 標準値 4
	s8	field_ajst_w_dr_f;		///< 移動方向右 前方向チェック補正値 標準値 2
	s8	field_ajst_w_dr_b;		///< 移動方向右 下方向チェック補正値 標準値 4
	// objDiffCollisionDirHeightCheck チェック用補正値
	s8	field_ajst_h_db_r;		///< 移動方向下 右方向チェック補正値 標準値 1
	s8	field_ajst_h_db_l;		///< 移動方向下 左方向チェック補正値 標準値 1
	s8	field_ajst_h_dl_r;		///< 移動方向左 右方向チェック補正値 標準値 1
	s8	field_ajst_h_dl_l;		///< 移動方向左 左方向チェック補正値 標準値 1
	s8	field_ajst_h_dt_r;		///< 移動方向上 右方向チェック補正値 標準値 1
	s8	field_ajst_h_dt_l;		///< 移動方向上 左方向チェック補正値 標準値 1
	s8	field_ajst_h_dr_r;		///< 移動方向右 右方向チェック補正値 標準値 2
	s8	field_ajst_h_dr_l;		///< 移動方向右 左方向チェック補正値 標準値 2

    // 関数関連
	void (*ppFunc)(struct _OBS_OBJECT_WORK*);		///< 処理関数 メイン処理
	void (*ppIn)(struct _OBS_OBJECT_WORK*);			///< 入力関数 (キー入力など) + 毎特殊処理（ヒットストップタイマーデクリメントなど）オブジェクトポーズ時の実行設定可能,
	void (*ppOut)(struct _OBS_OBJECT_WORK*);		///< 出力関数メイン 標準出力処理を設定
	void (*ppOutSub)(struct _OBS_OBJECT_WORK*);		///< 出力関数サブ ppOut に追加出力処理が必要な場合
	void (*ppMove)(struct _OBS_OBJECT_WORK*);		///< 移動関数 ヒットストップ中でも処理が行われる事に注意
	void (*ppActCall)(const void*, void*, u32);		///< アクションコールバック関数 MTF_ACT_CMD_CB, OBF_SMA_CMD_CB (コマンドワーク, アクションワーク, オブジェクトワーク)
	void (*ppRec)(struct _OBS_OBJECT_WORK*);		///< 矩形登録関数
	void (*ppLast)(struct _OBS_OBJECT_WORK*);		///< オブジェクトメイン最後処理 この関数の中ではOBD_OBJECT_TASKCLEARを立てない事、代わりにリクエストを使用する）
	void (*ppCol)(struct _OBS_OBJECT_WORK*);		///< 地形判定関数 ppCollision の代わりに実行する
	s32 (*ppViewCheck)(struct _OBS_OBJECT_WORK*);	///< 画面外チェック関数、標準でObjObjectViewOutCheckが設定される。return 0 画面内、 1画面外

	BOOL	(*ppUserRelease)(struct _OBS_OBJECT_WORK*);			///< ユーザー開放処理 return : TRUE で 開放待機処理あり
	BOOL	(*ppUserReleaseWait)(struct _OBS_OBJECT_WORK*);		///< ユーザー開放待機処理 return : TRUE で開放待機中

    struct _OBS_OBJECT_WORK *ride_obj;		///< ライドポインタ、乗っている地形オブジェクトのポインタ
    struct _OBS_OBJECT_WORK *touch_obj;		///< タッチポインタ、接触している地形オブジェクトのポインタ
    struct _OBS_OBJECT_WORK *parent_obj;	///< 親ポインタ
    struct _OBS_OBJECT_WORK *lock_obj;		///< ロック中ターゲットオブジェクト
    struct _OBS_OBJECT_WORK *locker_obj;	///< ロックしてきているオブジェクト

    /// 拡張ワークメモリ、変数が足りない場合に使用、ここに設定したメモリアドレスはタスク消去時に自動で解放されます
    void *ex_work;   

    // 表示ワークポインタ
#if OBD_USE_ACTION3D_NN
	OBS_ACTION3D_NN_WORK		*obj_3d;		///< 3Dオブジェクトデータ用
#if OBD_USE_ACTION3D_ES
	OBS_ACTION3D_ES_WORK		*obj_3des;		///< EffectaStudioエフェクトデータ
#endif /* OBD_USE_ACTION3D_ES */
#if OBD_USE_ACTION2D_AMA
	OBS_ACTION2D_AMA_WORK		*obj_2d;		///< 2Dアクションデータ用
#endif
#endif

#if defined _DS
#if OBD_USE_ACTION2D
    OBS_ACTION2D_WORK        * obj_2d;		///< 2Dアクションデータ用
#endif
#if OBD_USE_ACTION3D_NNS
    OBS_ACTION3D_NNS_WORK    * obj_3d;		///< アニメ3Dデータ用
#endif
#if OBD_USE_ACTION3D_1M1S
    OBS_ACTION3D_SIMPLE_WORK * obj_s3d;		///< モデルのみデータ用
#endif
#if OBD_USE_ACTION3D_SPR
    OBS_ACTION3D_SPRITE_WORK * obj_3dspr;	///< ビルボードデータ用
#endif
#if OBD_USE_ACTION3D_SS
	OBS_ACTION3D_SS_WORK	 * obj_3dss;	///< ソフトウェアスプライトデータ用
#endif
#if OBD_USE_ACTION3D_POLY
	OBS_ACTION3D_POLY_WORK   * obj_3dpoly;	///< ポリゴンアクションデータ用
#endif
#endif

#if OBD_USE_ACTION3D_SMA
	OBS_ACTION3D_SMA_WORK	* obj_3dsma;	///< SMAアクションデータ用
#endif // #if OBD_USE_ACTION3D_SMA

    // サウンドハンドルポインタ
#if defined _DS
    NNSSndHandle* h_snd;     ///< 汎用SEハンドル
#endif
    
    /// 地形ワークポインタ
    OBS_COLLISION_WORK * col_work;

	// OBS_TBL_WORK 関連
	struct tag_OBS_TBL_WORK * tbl_work;	///< テーブルワーク
	VecFx32 temp_ofst;					///< 一時オフセット、地形判定、矩形判定、タスク処理外の時など、posはこの値が足された状態になる 1:19:12
	VecFx32 prev_temp_ofst;				///< 一時オフセットコピー 1:19:12

	// 矩形
	u32						rect_num;			///< 登録矩形データ数
	struct _OBS_RECT_WORK	*rect_work;			///< 矩形データ(使用数分配列保持)
//	u32						rect_act_flag;		///< 矩形をアクションから設定するかどうかフラグ 32個まで

//    /// 当たりワークポインタ( #OBD_OBJ_RECT_NUM 個分)
//    struct _OBS_RECT_WORK * rect_work[OBD_OBJ_RECT_NUM];
//    u16 rect_act_flag[OBD_OBJ_RECT_NUM]; ///< 矩形をアクションから設定するかどうかフラグ
} OBS_OBJECT_WORK;

/// アクションコールバック型
typedef void (*OBF_ACT_CALLBACK)(const void*, void*, u32);

// OBS_OBJECT_WORK::flag
#define OBD_OBJECT_B                 ( 1 << 0 ) ///< このオブジェクトはB面側に居る
#define OBD_OBJECT_NOHIT             ( 1 << 1 ) ///< 当り判定登録を無視する
#define OBD_OBJECT_TASKCLEAR         ( 1 << 2 ) ///< 次フレーム、タスクをクリアする
#define OBD_OBJECT_TASKCLEAR_REQUEST ( 1 << 3 ) ///< タスククリアをリクエストする（外部からクリア時）

#define OBD_OBJECT_NOCLIP     ( 1 << 4 ) ///< 画面の外クリアチェックを無視する
#define OBD_OBJECT_NOPAUSE    ( 1 << 5 ) ///< オブジェクトポーズ時にも稼動する
#define OBD_OBJECT_NOPAUSE_IN ( 1 << 6 ) ///< ppInに登録した関数をポーズ中にも実行する（ポインタの無効チェックをppInで行うようなオブジェクトで使用する）
#define OBD_OBJECT_NOFUNC     ( 1 << 7 ) ///< メイン処理を停止する

#define OBD_OBJECT_NOWAITLOAD		(1 << 8)	///< データロード終了を待たずに処理を開始する
//#define OBD_OBJECT_ARCHIVE    		( 1 << 8 ) ///< スプライトアーカイブリソース使用
#define OBD_OBJECT_PARENT_NODIE   	( 1 << 9 ) ///< 親オブジェクトが死んでも自分は死にません
#define OBD_OBJECT_PARENT_FIX     	( 1 <<10 ) ///< 座標が親オブジェクトに付随する
#define OBD_OBJECT_PARENT_ACT_FIX 	( 1 <<11 ) ///< アクションが親オブジェクトに付随する（セットするアクションのID構成を親と同じにすること）

#define OBD_OBJECT_PLT_ANIME      	( 1 << 12 ) ///< パレットアニメあり
#define OBD_OBJECT_NO_HITSTOP   	( 1 << 13 ) ///< ヒットストップを無視する
#define OBD_OBJECT_NO_VIB       	( 1 << 14 ) ///< バイブタイマーを無視する
#define OBD_OBJECT_PLT_B        	( 1 << 15 ) ///< グラフィックエンジンBのみのパレット

#define OBD_OBJECT_NOPAUSE_REC       ( 1 << 16 ) ///< ppRecに登録した関数をポーズ中にも実行する
#define OBD_OBJECT_PARENT_FIX_NOFLIP ( 1 << 17 ) ///< OBD_OBJECT_PARENT_FIX の時に、向きをコピーしない
#define OBD_OBJECT_NOPAUSE_LAST		 ( 1 << 18 ) ///< ppLastに登録した関数をポーズ中にも実行する
#define OBD_OBJECT_PARENT_FIX_NODISP ( 1 << 19 ) ///< OBD_OBJECT_PARENT_FIX の時に、DISP設定をコピーしない

#define OBD_OBJECT_ACT_NOA    ( 1 <<20 ) ///< GE_Aの処理を行わない（VRAM取得等
#define OBD_OBJECT_ACT_NOB    ( 1 <<21 ) ///< GE_Bの処理を行わない（VRAM取得等
#define OBD_OBJECT_FREE_TBL   ( 1 <<22 ) ///< タスク終了時にテーブル行動ワークを解放する
#define OBD_OBJECT_FREE_EX    ( 1 <<23 ) ///< タスク終了時に拡張ワークを解放
#define OBD_OBJECT_FREE_COL   ( 1 <<24 ) ///< タスク終了時にオブジェクト地形ワークを解放する
#define OBD_OBJECT_FREE_HIT   ( 1 <<25 ) ///< タスク終了時に当り判定ワークを解放する

#define OBD_OBJECT_FREE_2D    ( 1 <<26 ) ///< タスク終了時に2Dワークを解放する
#define OBD_OBJECT_FREE_3D    ( 1 <<27 ) ///< タスク終了時に3Dワークを解放する
#define OBD_OBJECT_FREE_3DES  ( 1 <<28 ) ///< タスク終了時に3DESワークを解放する
#define OBD_OBJECT_NORELEASE_3D	( 1 <<29)///< タスク終了時に3Dモデル開放を行わない
//#define OBD_OBJECT_FREE_3DS   ( 1 <<28 ) ///< タスク終了時に3DSIMPLEワークを解放する
//#define OBD_OBJECT_FREE_3DSP  ( 1 <<29 ) ///< タスク終了時に3DSPRITEワークを解放する
//#define OBD_OBJECT_FREE_3DSS  ( 1 <<30 ) ///< タスク終了時に3DSSワークを解放する
//#define OBD_OBJECT_FREE_3DSMA ( 1 <<31 ) ///< タスク終了時に3DSMAワークを解放する

#if OBD_OBJECT_USE_NOEXIST
#define OBD_OBJECT_NOEXIST_ENABLE	(1 << 30)		///< OBD_OBJ_NOEXISTが有効になるようにする
#define OBD_OBJECT_NOEXIST			(1 << 31)		///< 画面外に居る際に更新類を一切行わず、存在しないように振舞う
#endif // OBD_OBJECT_USE_NOEXIST


// OBS_OBJECT_WORK::disp_flag
#define OBD_DISP_HFLIP			( 1 << 0 )	///< HFLIP して表示					user : rw
#define OBD_DISP_VFLIP			( 1 << 1 )	///< VFLIP して表示					user : rw
#define OBD_DISP_REPEAT			( 1 << 2 )	///< アニメ-ション繰り返し			user : rw
#define OBD_DISP_END			( 1 << 3 )	///< アニメーション終了				user : r-

#define OBD_DISP_STOP			( 1 << 4 ) ///< アニメーション進めない			user : r-
#define OBD_DISP_NODISP			( 1 << 5 ) ///< 表示しない						user : rw
//#define OBD_DISP_FUNC   ( 1 << 6 ) ///< ppOutオンでも標準表示を行う
#define OBD_DISP_NOMAP			( 1 << 7 ) ///< マップの位置を無視する			user : rw

#define OBD_DISP_NODIR			( 1 << 8 ) ///< 回転表示を行わない				user : rw
#define OBD_DISP_3D_PARALLEL	( 1 << 9 ) ///< カメラ向きの回転を行う			user : rw
#define OBD_DISP_3D_BLEND		( 1 <<10 ) ///< アニメーションのブレンドを行う	user : rw
#define OBD_DISP_3D_LOCK_LIGHT	( 1 <<11 ) ///< ライト０をオブジェクトの正面に向けるように固定する				user : rw

#define OBD_DISP_NOUPDATE		( 1 <<12) ///< UPDATEを行わない					user : rw
#define OBD_DISP_NOPOS			( 1 <<13) ///< 座標設定を行わない（ppFunc内などで直接表示系構造体に設定する）	user : rw
#define OBD_DISP_NOOFST			( 1 <<14) ///< 全体オフセットを無視する			user : rw
#define OBD_DISP_RECTONLY		( 1 <<15) ///< 矩形データのみを使用するアクション(Obj2dのみ影響する)			user : rw
#define OBD_DISP_NOSCALE		( 1 <<16) ///< 拡大縮小を行わない				user : rw

#define OBD_DISP_3D_SPRITE		( 1 <<17) ///< 3Dスプライトを2Dスプライトのように表示	user : rw
#define OBD_DISP_NOBELT			( 1 <<18) ///< ベルトスクロール方式を反映しない			user : rw
#define OBD_DISP_NOGLBSCALE		( 1 <<19) ///< グローバルスケールを使用しない			user : rw
#define OBD_DISP_NODRAWSCALE	( 1 <<20) ///< 描画スケールを使用しない					user : rw

#define OBD_DISP_3D_COORDINATE	( 1 <<21) ///< 3Dモデル表示時に、3D座標系(上Y+ 下Y-)で座標を設定
#define OBD_DISP_NODIRFLIP		( 1 <<22) ///< 3Dモデル表示時に、回転によるフリップ処理を行わない
#define OBD_DISP_USERMTX		( 1 <<23) ///< 描画時にユーザー計算済み行列を使用する
#define OBD_DISP_USERMTX_RIGHT	( 1 <<24) ///< 描画時にユーザー計算済み行列を使用する（オブジェクトのワールド行列に対して右から乗算）

#define OBD_DISP_MAT_END		( 1 <<25) ///< マテリアルアニメーション終了	user : r-
#define OBD_DISP_DIR2DFLIP		( 1 <<26) ///< 3Dモデル表示時に、0x0000, 0x8000 でフリップする
#define OBD_DISP_DRAWSTATE		( 1 <<27) ///< ユーザー描画ステータスで描画します(3DNN)

#if _IPHONE
#define OBD_DISP_NOCLIP			( 1 <<28) ///< クリッピングを行いません。
#if OBD_OBJECT_USE_NOEXIST
#define OBD_DISP_NOEXIST_ENABLE	(1 << 29) ///< OBD_OBJ_NOEXISTが有効になるようにする
#define OBD_DISP_NOEXIST		(1 << 30) ///< 画面外に居る際に更新類を一切行わず、存在しないように振舞う
#endif // OBD_OBJECT_USE_NOEXIST
#endif // _IPHONE

// OBS_OBJECT_WORK::move_flag
#define OBD_MOVE_UNDER		( 1 <<  0 ) ///< 下方向地面HIT
#define OBD_MOVE_OVER		( 1 <<  1 ) ///< 上方向地面HIT
#define OBD_MOVE_FRONT		( 1 <<  2 ) ///< 前方向地面HIT
#define OBD_MOVE_BACK		( 1 <<  3 ) ///< 後ろ方向地面HIT
#define OBD_MOVE_COL_MASK  ( OBD_MOVE_UNDER | OBD_MOVE_OVER | OBD_MOVE_FRONT | OBD_MOVE_BACK) ///< 地形HITフラグマスク

#define OBD_MOVE_JUMP    ( 1 <<  4 ) ///</< ジャンプフラグ (地形チェックに影響)
#define OBD_MOVE_THROUGH ( 1 <<  5 ) ///< すり抜け足場をすり抜ける（すり抜け足場を越えると自動的に消える）
#define OBD_MOVE_DIR     ( 1 <<  6 ) ///< 角度を使用する
#define OBD_MOVE_FALL    ( 1 <<  7 ) ///< 落下加速、落下計算を行う

#define OBD_MOVE_NOCOL      ( 1 <<  8 ) ///< 地形チェックを行わない
#define OBD_MOVE_NOCOLOBJ   ( 1 <<  9 ) ///< オブジェクト地形チェックを行わない
#define OBD_MOVE_NOCOL_W    ( 1 << 10 ) ///< 地形チェック、横の判定を行わない
#define OBD_MOVE_NOCOL_H    ( 1 << 11 ) ///< 地形チェック、縦の判定を行わない

#define OBD_MOVE_NOCOLFIELD ( 1 << 12 ) ///< 地形チェック、地形データとのチェックを行わない
#define OBD_MOVE_NOCOL_MASK ( OBD_MOVE_NOCOL | OBD_MOVE_NOCOLOBJ | OBD_MOVE_NOCOL_W | OBD_MOVE_NOCOL_H | OBD_MOVE_NOCOLFIELD )
#define OBD_MOVE_NOMOVE   ( 1 << 13 ) ///< 移動を行わない
#define OBD_MOVE_NOSPD    ( 1 << 14 ) ///< 壁接地時に速度クリアを行わない
#define OBD_MOVE_NOSPDM   ( 1 << 15 ) ///< SPDMASTERを使用しない

#define OBD_MOVE_COL_VIB    ( 1 << 16 ) ///< 地形チェック用（震え対策専用いじらない事）
#define OBD_MOVE_SLOPE       ( 1 << 17 ) ///< 坂道接地で加減速する
#define OBD_MOVE_SLOPE_STICK ( 1 << 18 ) ///< 坂道接地でくっつける OBD_MOVE_SLOPEを立てても速度が０なら加減速しない
#define OBD_MOVE_LIMIT_OUT   ( 1 << 19 ) ///< 画面外を壁として扱う

#define OBD_MOVE_AIRFOOT     ( 1 << 20 ) ///< 足元が空中
#define OBD_MOVE_UNDERALL    ( 1 << 21 ) ///< 足元だけ全ドットチェック
#define OBD_MOVE_UNDERPREV   ( 1 << 22 ) ///< 前フレームで地面にHITしていた

#define OBD_MOVE_DIR_SLOW    ( 1 << 23 ) ///< 地形角度設定を直値設定ではなく近似設定にする
#define OBD_MOVE_PUSH        ( 1 << 24 ) ///< 押し移動中
#define OBD_MOVE_PUSH_COL    ( 1 << 25 ) ///< 押され移動するオブジェクト

#define OBD_MOVE_NO_AUTO_SCROLL ( 1 << 26 ) ///< オートスクロールの影響を受けない
#define OBD_MOVE_NOFLOW         ( 1 << 27 ) ///< 流されない、sFlowの影響を受けない
#define OBD_MOVE_FLY            ( 1 << 28 ) ///< 床に接触しても角度が設定されない。

//#define OBD_MOVE_Z_FRONT	( 1 << 29 )		///< Z方向前側HIT
//#define OBD_MOVE_Z_BACK		( 1 << 30 )		///< Z方向後側HIT
#define OBD_MOVE_NOCLR_SPDXY ( 1 << 29 )	// 壁接地時にXY速度クリアを行わない(spd_mのみクリア:OBD_MOVE_NOSPDとの違いはspd.x/yをクリアするか否か)
#define OBD_MOVE_TOP_DIFF	(1 << 30)		// 落下中に天井判定行う。天井・床判定のめり込み許容量も２倍になります。
#define OBD_MOVE_NOBODYPUSH	( 1 << 31 )		///< 体押し合いを行わない


// OBS_OBJECT_WORK::gmk_flag ゲームによって可変
//#define OBD_GMK_NO_UPFIELD				( 1 << 0 )		///< 上側地形を無視する
//#define OBD_GMK_NO_UPFIELDPREV			( 1 << 1 )		///< 前フレーム 上側地形無視
#define OBD_GMK_NO_COLPOSADJUST			( 1 << 2 )		///< 地形チェックを行うが座標補正は行わない
#define OBD_GMK_NO_OBJCOLTHROUGH		( 1 << 3 )		///< 地形オブジェクトとのあたりをチェックする際にすり抜けない地形として扱う
#define OBD_GMK_NO_CHECK_OBJCOLTHROUGH	( 1 << 4 )		///< すり抜け可能地形オブジェクトとのあたりをチェックを行わない
#define OBD_GMK_NO_UNDER_FIELD			( 1 << 5 )		///< 下地面あたりを行わない
#define OBD_GMK_NO_UNDER_FIELDPREV		( 1 << 6 )		///< 前フレーム  下地面あたりを行わない

// OBS_OBJECT_WORK::sys_flag ゲームによって可変
#define OBD_USE_NOLANDING	(1)			// HOG トロッコ専用処理 プレイヤーのみが処理
#define OBD_SYSF_NOLANDING_UNDER		(1 << 0)		///< 下方向が地面の場合接地しない(objDiffCollisionDirHeightCheck の角度NO順にあわせる)
#define OBD_SYSF_NOLANDING_LEFT			(1 << 1)		///< 左方向が地面の場合接地しない
#define OBD_SYSF_NOLANDING_TOP			(1 << 2)		///< 上方向が地面の場合接地しない
#define OBD_SYSF_NOLANDING_RIGHT		(1 << 3)		///< 右方向が地面の場合接地しない
#define OBD_SYSF_NOLANDING_MASK			(OBD_SYSF_NOLANDING_UNDER|OBD_SYSF_NOLANDING_LEFT|\
											OBD_SYSF_NOLANDING_TOP|OBD_SYSF_NOLANDING_RIGHT)

#define OBD_SYSF_NOLANDING_UNDERPREV	(1 << 4)		///< 前フレーム 下方向が地面の場合接地しない(objDiffCollisionDirHeightCheck の角度NO順にあわせる)
#define OBD_SYSF_NOLANDING_LEFTPREV		(1 << 5)		///< 前フレーム 左方向が地面の場合接地しない
#define OBD_SYSF_NOLANDING_TOPPREV		(1 << 6)		///< 前フレーム 上方向が地面の場合接地しない
#define OBD_SYSF_NOLANDING_RIGHTPREV	(1 << 7)		///< 前フレーム 右方向が地面の場合接地しない
#define OBD_SYSF_NOLANDING_PREVMASK		(OBD_SYSF_NOLANDING_UNDERPREV|OBD_SYSF_NOLANDING_LEFTPREV|\
											OBD_SYSF_NOLANDING_TOPPREV|OBD_SYSF_NOLANDING_RIGHTPREV)

// OBS_OBJECT_WORK::ulColFlag 地形処理に合わせて変更する
// 地形属性フラグ
#define OBD_COLAT_CLIFF   ( 1 << 0 ) ///< 崖
#define OBD_COLAT_THROUGH ( 1 << 1 ) ///< すり抜け足場
#define OBD_COLAT_GRAIND  ( 1 << 2 ) ///< グラインド
#define OBD_COLAT_WATER   ( 1 << 3 ) ///< 水中

// OBS_OBJECT_WORK::user_flag
#define OBD_OBJECT_USER_0	( 1 << 0 ) ///< ゲームに合わせて使用する
#define OBD_OBJECT_USER_1	( 1 << 1 ) // 
#define OBD_OBJECT_USER_2	( 1 << 2 ) // 
#define OBD_OBJECT_USER_3	( 1 << 3 ) // 
#define OBD_OBJECT_USER_4	( 1 << 4 ) // 
#define OBD_OBJECT_USER_5	( 1 << 5 ) // 
#define OBD_OBJECT_USER_6	( 1 << 6 ) // 
#define OBD_OBJECT_USER_7	( 1 << 7 ) // 

// OBS_OBJECT_WORK::dir
#define OBD_OBJECT_STANDARD_DIR  ( 0x4000 ) ///< 画面からみて左方向の角度値
#define OBD_OBJECT_LEFT_DIR      ( 0xc000 ) ///< 右方向の角度値

// OBS_OBJECT_WORK::field[]
#define OBD_LEFT	(0) ///< 矩形配列 左
#define OBD_TOP		(1) ///< 矩形配列 上
#define OBD_RIGHT	(2) ///< 矩形配列 右
#define OBD_BOTTOM	(3) ///< 矩形配列 下
#define OBD_BACK	(4) ///< 矩形配列 奥
#define OBD_FRONT	(5) ///< 矩形配列 前
#define OBD_BOX		(6)	///< 矩形配列 3D
#define OBD_WIDTH	(2) ///< 矩形配列 幅
#define OBD_HEIGHT	(3) ///< 矩形配列 高
//#define WIDTH		(2) ///< 矩形配列 幅
//#define HEIGHT		(3) ///< 矩形配列 高

//----- Macros --------------------------------------------------------------
// ================================================================
// OBM_RAND_RANGE
/*!
  指定した値～マイナス指定値の乱数を返す、ただし指定値は二進数にした時途中に0が挟まらない事

  @param num  [in] 範囲 

 */
// ================================================================
#define OBM_RAND_RANGE( num )\
    ( ((num) ) - (mtMathRand() & (num << 1)))
// ================================================================
// OBM_RAND_RANGE
/*!
  指定した値～マイナス指定値の乱数を返す、ただし指定値は二進数にした時途中に0が挟まらない事

  @param num  [in] 範囲 

 */
// ================================================================
#define OBM_DISP_RAND_RANGE( num )\
    ( ((num) ) - (ObjDispRand() & (num << 1)))

//----- Macros Functions ----------------------------------------------------
// ================================================================
// ObjObjectAction3dSet
/*!
  アクション設定関数

  @param pObj  [in] オブジェクトワークポインタ OBS_OBJECT_WORK*
  @param mType [in] 設定するアニメ             MTE_ACTION3D_NNS_ANIM_TYPE
  @param usID  [in] 設定するアニメインデックス u16

 */
// ================================================================
#if 1
#define ObjObjectAction3dSet( pObj, mType, usID )\
        mtAct3dSetAnimNNS(&(pObj)->obj_3d->act_3d, (mType),            \
                          (pObj)->obj_3d->anime[mType], (usID));
#else
#define ObjObjectAction3dSet( pObj, mType, usID )\
    if ( mType == MTE_ACT3D_NNS_ANIM_TP )                              \
        mtAct3dSetAnimNNS(&(pObj)->obj_3d->act_3d, (mType),            \
                          (pObj)->obj_3d->anime[mType], (usID),        \
                          NNS_G3dGetTex( (NNSG3dResFileHeader*)(pObj)->obj_3d->model ) );\
    else                                                               \
        mtAct3dSetAnimNNS(&(pObj)->obj_3d->act_3d, (mType),            \
                          (pObj)->obj_3d->anime[mType], (usID), NULL );
#endif
// ================================================================
// ObjObjectAction3dModelSet
/*!
  アクション設定関数

  @param pObj  [in] オブジェクトワークポインタ OBS_OBJECT_WORK*
  @param usID  [in] 設定するモデルインデックス u16

 */
// ================================================================
#define ObjObjectAction3dModelSet( pObj, usID )\
        mtAct3dSetModelNNS(&(pObj)->obj_3d->act_3d, (pObj)->obj_3d->model, (usID) , FALSE, FALSE)

// ==========================================================================
// OBM_OBJECT_TASK_DETAIL_INIT
/*!
 *	オブジェクトタスク生成初期化関数
 *
  @param prio				[in]  タスクプライオリティ
  @param group				[in]  タスクグループ
  @param pause_level		[in]  タスクポーズレベル
  @param obj_pause_level	[in]  オブジェクトポーズレベル
  @param work_size			[in]  タスクワークサイズ OBS_OBJECT_WORKサイズ以上を設定
  @param name				[in]  デバッグ用TCB名(NULL可)
 *
  @return   ワークポインタ NULLで失敗
 *
 *	@note
 *		ObjObjectTaskDetailInitを呼び出します
 */
// ==========================================================================
#if defined (MTD_DEBUG)
#define OBM_OBJECT_TASK_DETAIL_INIT(priority, group, pause_level, obj_pause_level, work_size, name)	(ObjObjectTaskDetailInit(priority, group, pause_level, obj_pause_level, work_size, name))
#else
#define OBM_OBJECT_TASK_DETAIL_INIT(priority, group, pause_level, obj_pause_level, work_size, name)	(ObjObjectTaskDetailInit(priority, group, pause_level, obj_pause_level, work_size))
#endif


//----- External Variables --------------------------------------------------
extern OBS_OBJECT	g_obj;				///< グローバルデータ オブジェクトの全設定
extern const fx16 g_object_vib_tbl[];	///< 汎用振動テーブル
//extern const s8		g_object_vib_tbl[];	///< 汎用振動テーブル

#if (OBD_USE_ACTION3D_NN)
extern AMS_DRAWSTATE g_obj_draw_3dnn_draw_state;	///< 描画時設定ステータス初期化用データ
#endif

//----- External Declarations -----------------------------------------------
// ================================================================
// システム初期化
// ================================================================
// ================================================================
// ObjInit
/*!
 *	オブジェクト設定を初期化、毎フレーム処理を行うタスク生成
 *
 *	@param	group		[in]	オブジェクトシステムメイン管理処理 タスクグループ
 *	@param	prio		[in]	オブジェクトシステムメイン管理処理 タスク優先
 *	@param	lcd_size_x	[in]	オブジェクトシステム 画面Xサイズ
 *	@param	lcd_size_y	[in]	オブジェクトシステム 画面Yサイズ
 *	@param	disp_width	[in]	表示解像度X
 *	@param	disp_height	[in]	表示解像度Y
 *
 *	@note
 *		既にタスクが存在する時は一度終了します
 */
// ================================================================
extern void ObjInit(u8 group, u16 prio, u8 pause_level, s16 lcd_size_x, s16 lcd_size_y, float disp_width, float disp_height);

// ================================================================
// システム終了
// ================================================================
// ================================================================
// ObjExit
/*!
  オブジェクト終了処理

  @note
    初期化されていない場合は何も実行しません。
 */
// ================================================================
extern void ObjExit(void);

#if OBD_USE_ACTION3D_NN
// ================================================================
// ObjPreExit
/*!
  オブジェクト終了前処理

  @note
    初期化されていない場合は何も実行しません。\n
	処理の終了などの データ開放以外の終了を行います
 */
// ================================================================
extern void ObjPreExit(void);
#endif

// ================================================================
// 実行状況チェック
// ================================================================
// ================================================================
// ObjIsInit
/*!
  オブジェクトシステムが稼動しているかチェック

  @return	TRUE : 稼動中(もしくは終了処理待機中)
 */
// ================================================================
extern BOOL ObjIsInit(void);

// ================================================================
// ObjIsExitWait
/*!
  オブジェクトシステムが終了処理中かチェック

  @return	TRUE : 終了処理中
 */
// ================================================================
extern BOOL ObjIsExitWait(void);


// ================================================================
// オブジェクトポーズ
// ================================================================
// ================================================================
// ObjObjectPause
/*!
  オブジェクトポーズする

	@param	pause_level	[in]	オブジェクトポーズレベル

	@note
		オブジェクトのポーズレベルが指定したポーズレベル以下の場合ポーズになる

 */
// ================================================================
extern void ObjObjectPause(u16 pause_level);

// ================================================================
// ObjObjectPauseOut
/*!
  オブジェクトポーズを解除する

 */
// ================================================================
extern void ObjObjectPauseOut();

// ================================================================
// ObjObjectForcePause
/*!
  強制オブジェクトポーズする

 */
// ================================================================
extern void ObjObjectForcePause();

// ================================================================
// ObjObjectForcePauseOut
/*!
  強制オブジェクトポーズを解除する

 */
// ================================================================
extern void ObjObjectForcePauseOut();

// ================================================================
// ObjObjectPauseCheck
/*!
  オブジェクトポーズチェック

  @param  ulFlag [in] オブジェクトワークフラグ、無い場合は0を指定
    
  @return 0 ポーズ中では無い 非0 ポーズ中

 */
// ================================================================
extern u32 ObjObjectPauseCheck( u32 ulFlag );

// ================================================================
// データワーク
// ================================================================
// ================================================================
// ObjDataAlloc
/*!
    データファイル管理用のメモリを作成する

  @param num [in] 今ゲームで同時使用するファイルの最大値

 */
// ================================================================
extern void ObjDataAlloc( s32 num );

// ================================================================
// ObjDataGet
/*!
  データファイル管理用のメモリアドレスを取得する

  @param index [in] データ管理ID

  @return データ管理ポインタ
 */
// ================================================================
extern struct _OBS_DATA_WORK* ObjDataGet( s32 index);

// ================================================================
// ObjDataFree
/*!
  データファイル管理用のメモリを開放する
 */
// ================================================================
extern void ObjDataFree();

// ================================================================
// オブジェクトシステム設定
// ================================================================
// イベント生成範囲
// ================================================================
// ObjObjectClipLCDSet
/*!
  オブジェクト全体に適用するオフセットを設定

	@param size_x	[in]	イベント生成範囲用サイズX
	@param size_y	[in]	イベント生成範囲用サイズY

 */
// ================================================================
#ifndef _DS
extern void ObjObjectClipLCDSet(s16 size_x, s16 size_y);
#endif

// オフセット
// ================================================================
// ObjObjectOffsetSet
/*!
  オブジェクト全体に適用するオフセットを設定

  @param cX [in] オブジェクトに適用されるオフセット 1:15
  @param cY [in] 

 */
// ================================================================
extern void ObjObjectOffsetSet(s16 sX, s16 sY );

// 速度
// ================================================================
// ObjObjectSpeedSet
/*!
  オブジェクト全体に適用する処理速度を設定

  @param sSpd [in] オブジェクトに適用される処理速度 1:19:12

 */
// ================================================================
extern void ObjObjectSpeedSet(fx32 sSpd );

// ================================================================
// ObjObjectSpeedGet
/*!
  オブジェクト全体に適用する処理速度を取得

  @return   処理速度

 */
// ================================================================
extern fx32 ObjObjectSpeedGet(void);

// オートスクロール
// ================================================================
// ObjObjectScrollSet
/*!
	オブジェクト全体に適用するオートスクロール速度設定

	@param	spd_x	[in]	速度X
	@param	spd_y	[in]	速度Y
 */
// ================================================================
extern void ObjObjectScrollSet(fx32 spd_x, fx32 spd_y);

// ================================================================
// ObjObjectScrollGetX
/*!
	オブジェクト全体に適用するオートスクロール速度X取得

	@return   スクロール速度X
 */
// ================================================================
extern fx32 ObjObjectScrollGetX(void);

// ================================================================
// ObjObjectScrollGetY
/*!
	オブジェクト全体に適用するオートスクロール速度X取得

	@return   スクロール速度Y
 */
// ================================================================
extern fx32 ObjObjectScrollGetY(void);

// ベルトシステム
// ================================================================
// ObjObjectBeltSetDepth
/*!
	ベルト系描画時奥行き設定

	@param	depth	[in]	奥行き
 */
// ================================================================
extern void ObjObjectBeltSetDepth(fx32 depth);

// ================================================================
// ObjObjectBeltGetDepth
/*!
	ベルト系描画時奥行き取得

	@return	奥行き
 */
// ================================================================
extern fx32 ObjObjectBeltGetDepth(void);

// カメラ
// ================================================================
// ObjObjectCameraSet
/*!
  オブジェクト全体に適用するオフセットを設定

  @param x1 [in] カメラ1（グラフィックエンジンA）の X座標 1:19:12
  @param y1 [in] カメラ1（グラフィックエンジンA）の Y座標 1:19:12
  @param x2 [in] カメラ2（グラフィックエンジンB）の X座標 1:19:12
  @param y2 [in] カメラ2（グラフィックエンジンB）の Y座標 1:19:12

 */
// ================================================================
extern void ObjObjectCameraSet( fx32 x1, fx32 y1, fx32 x2, fx32 y2 );

#ifndef _DS
// ================================================================
// ObjObjectClipCameraSet
/*!
  クリッピング, イベント生成範囲基カメラ を設定

  @param x		[in] 基点X
  @param y		[in] 基点Y

 */
// ================================================================
extern void ObjObjectClipCameraSet(fx32 x, fx32 y);
#endif

// ================================================================
// ObjObjectCameraZSet
/*!
  オブジェクト全体に適用する拡大率を設定

  @param z [in] カメラZ距離 1:19:12

  @note
    0で標準のサイズ、0x1000で半分、-0x1000で倍角
 */
// ================================================================
extern void ObjObjectCameraZSet( fx32 z );

// ライト
#if defined _DS
// ================================================================
// ObjObjectSetLight
/*!
	ライトベクトル設定

	@param	light_id	[in] セットするライトID (0 ～ 3)
	@param	vec_x		[in] ライト方向ベクトル X成分
	@param	vec_y		[in] ライト方向ベクトル Y成分
	@param	vec_z		[in] ライト方向ベクトル Z成分

  @note
    0で標準のサイズ、0x1000で半分、-0x1000で倍角
 */
// ================================================================
extern void ObjObjectSetLightVec(u16 light_id, fx16 vec_x, fx16 vec_y, fx16 vec_z);

// ================================================================
// ObjObjectSetLightNum
/*!
	使用ライト数設定

	@param	light_num	[in] 使用するライト数 (0 ～ 4)

  @note
	0 でオブジェクトシステムではライトを設定しない
 */
// ================================================================
extern void ObjObjectSetLightNum(u16 light_num);
#endif

#if defined _DS
// OBJ VRAM マッピングモード
// ================================================================
// ObjObjectSetVramMapMode
/*!
	OBJ VRAM マッピングモード設定

	@param mmode [in] OBJ VRAM マッピングモード OBE_OBJ_VRAM_MMODE
 */
// ================================================================
extern void ObjObjectSetVramMapMode(OBE_OBJ_VRAM_MMODE mmode);
#endif

// ================================================================
// テクスチャVRAM ダブルバッファ
// ================================================================
// ================================================================
// ObjObjectSetTexDoubleBuffer
/*!
	テクスチャVRAM ダブルバッファ

	@param	bank1			[in] テクスチャ割り当てVRAMバンク1 GX_VRAM_TEX_***
	@param	bank2			[in] テクスチャ割り当てVRAMバンク2 GX_VRAM_TEX_***
	@param	db_slot_flag	[in] ダブルバッファ使用するスロットフラグ

	@note
		実行前に mtVramSetBankTex で一度初期化しておくこと
 */
// ================================================================
#if OBD_USE_TEX_VRAM_DB
extern void ObjObjectSetTexDoubleBuffer(GXVRamTex bank1, GXVRamTex bank2, u8 db_slot_flag);
#else
#define ObjObjectSetTexDoubleBuffer(bank1, bank2, db_slot_flag)
#endif	// #if OBD_USE_TEX_VRAM_DB

// ================================================================
// オブジェクト生成
// ================================================================
// ================================================================
// ObjObjectTaskInit
/*!
  オブジェクトタスク生成初期化関数

  @return   ワークポインタ NULLで失敗

  @note
    単純オブジェクト生成
 */
// ================================================================
extern OBS_OBJECT_WORK * ObjObjectTaskInit(void);

// ================================================================
// ObjObjectTaskDetailInit
/*!
  オブジェクトタスク生成初期化関数

  @param prio				[in]  タスクプライオリティ
  @param group				[in]  タスクグループ
  @param pause_level		[in]  タスクポーズレベル
  @param obj_pause_level	[in]  オブジェクトポーズレベル
  @param work_size			[in]  タスクワークサイズ OBS_OBJECT_WORKサイズ以上を設定
  @param name				[in]  デバッグ用TCB名(NULL可)
    
  @return   ワークポインタ NULLで失敗
 
 */
// ================================================================
#if defined (MTD_DEBUG)
extern OBS_OBJECT_WORK * ObjObjectTaskDetailInit( u16 prio, u8 group, u8 pause_level, u8 obj_pause_level, u32 work_size, const char *name);
#else
extern OBS_OBJECT_WORK * ObjObjectTaskDetailInit( u16 prio, u8 group, u8 pause_level, u8 obj_pause_level, u32 work_size );
#endif // #if defined (MTD_DEBUG)

// ================================================================
// ObjObjectTaskNameSet
/*!
  タスクネーム設定（設定しない場合、標準の名前のままでデバッグの時に判り難い）

  @param pObj [in]  オブジェクトポインタ
  @param name [in]  文字列データ（16文字まで）

  @note
    リリース時、自動で空マクロになる
 */
// ================================================================
#if defined(MTD_DEBUG)  // デバッグ版
extern void ObjObjectTaskNameSet( OBS_OBJECT_WORK* pObj, const char * name );
#else
#define ObjObjectTaskNameSet( obj, name)
#endif

// ================================================================
// オブジェクト管理
// ================================================================
// ================================================================
// ObjObjectRegistObject
/*!
	オブジェクトワークをオブジェクトシステムに登録

	@param	obj_work	[in] オブジェクトポインタ

	@note
		オブジェクト生成関数を自作した場合は、\n
		必ず最後にこの関数を呼んで登録して下さい。
 */
// ================================================================
extern void ObjObjectRegistObject(OBS_OBJECT_WORK *obj_work);

// ================================================================
// ObjObjectRevokeObject
/*!
	オブジェクトワークをオブジェクトシステムから削除

	@param	pWork	[in] オブジェクトポインタ
 */
// ================================================================
extern void ObjObjectRevokeObject(OBS_OBJECT_WORK *pWork);

// ================================================================
// ObjObjectClearAllObject
/*!
	登録済みオブジェクトを全クリア

	@param	pWork	[in] オブジェクトポインタ

	@note
		オブジェクト生成関数を自作した場合は、\n
		必ず最後にこの関数を呼んで登録して下さい。
 */
// ================================================================
extern void ObjObjectClearAllObject(void);

#if OBD_USE_ACTION3D_NN
// ================================================================
// ObjObjectCheckClearAllObject
/*!
	登録済みオブジェクトがすべてクリアされたかチェック

	@return	TRUE : 開放終了
 */
// ================================================================
extern BOOL ObjObjectCheckClearAllObject(void);
#endif


// ================================================================
// ObjObjectSearchRegistObject
/*!
	登録済みオブジェクトを取得

	@param	obj_work	[in] サーチ元オブジェクトワーク NULLで先頭からサーチ
	@param	obj_type	[in] 取得するオブジェクトタイプ 0xFFFFですべてHIT

	@return	HITしたオブジェクトワーク 無かった時はNULL

	@note
		前にObjObjectSearchRegistObjectで取得したワークを obj_work で与える事で\n
		続きか検索します。\n
		ex)\n
			obj_work = bjObjectSearchRegistObject(NULL, TYPE);\n
			while (obj_work) {\n
				// 処理\n
				obj_work = bjObjectSearchRegistObject(obj_work, TYPE);\n
			}
 */
// ================================================================
extern OBS_OBJECT_WORK* ObjObjectSearchRegistObject(OBS_OBJECT_WORK *obj_work, u16 obj_type);

// ================================================================
// オブジェクト各種設定
// ================================================================
// ================================================================
// ObjObjectTypeSet
/*!
  オブジェクトタイプ設定

  @param pObj   [io]  オブジェクトポインタ
  @param usType [in]  オブジェクトタイプ
    
 */
// ================================================================
extern void ObjObjectTypeSet( OBS_OBJECT_WORK* pObj, u16 usType );

// ================================================================
// ObjObjectParentSet
/*!
  オブジェクト親設定

  @param pObj   [io]  オブジェクトポインタ
  @param pObj   [in]  親オブジェクトポインタ
  @param ulFlag [in]  親子関係フラグ OBD_OBJECT_PARENT_***
    
 */
// ================================================================
extern void ObjObjectParentSet( OBS_OBJECT_WORK* pObj, OBS_OBJECT_WORK* pParent, u32 ulFlag );

// ================================================================
// ObjObjectFallSet
/*!
  地形当り設定

  @param pObj    [io] オブジェクトワークポインタ
  @param cLeft   [in] 左端値
  @param cTop    [in] 上端値
  @param cRight  [in] 右端値
  @param cBottom [in] 下端値
    
 */
// ================================================================
extern void ObjObjectFallSet ( OBS_OBJECT_WORK* pObj, fx32 fSpdFall, fx32 fSpdFallMax );

// ================================================================
// ObjObjecExWorkAlloc
/*!
  拡張メモリ取得

  @param pObj    [io] オブジェクトワークポインタ
  @param lSize   [in] 取得メモリサイズ

  @return  取得したメモリのポインタ
    
  @note
    取得したメモリのポインタはpObj->pExWorkに格納されます。
    また、objObjectExitで自動的に解放されます。
    
 */
// ================================================================
extern void* ObjObjecExWorkAlloc ( OBS_OBJECT_WORK* pObj, u32 ulSize );

#if defined _DS
// ================================================================
// ObjObjectSoundHandleGet
/*!
  サウンドハンドルポインタ取得設定

  @param pObj     [io] オブジェクトワークポインタ

  @note
    これで取得した場合、タスク終了時、自動で開放する
 */
// ================================================================
extern NNSSndHandle* ObjObjectSoundHandleGet ( OBS_OBJECT_WORK* pObj );
#endif

// ================================================================
// ObjObjectTblWorkSet
/*!
  テーブルワーク設定

  @param pObj  [io] オブジェクトワークポインタ
  @param pTbl  [io] テーブルワークポインタ（NULLの場合、自動的にメモリを確保する、解放も自動で行う）

  @note
 */
// ================================================================
#if 0 // ◆一旦カット
extern void ObjObjectTblWorkSet ( OBS_OBJECT_WORK* pObj, struct tag_OBS_TBL_WORK * pTbl );
#endif

// ================================================================
// オブジェクトメイン処理
// ================================================================
// ================================================================
// ObjObjectMain
/*!
  メイン関数
 */
// ================================================================
#if defined _DS
extern void ObjObjectMain(void);
#else
extern void ObjObjectMain(MTS_TASK_TCB *tcb);
#endif

// ================================================================
// ObjObjectExit
/*!
  オブジェクト移動

  @param pWork [in] タスクポインタ
 */
// ================================================================
extern void ObjObjectExit( MTS_TASK_TCB *pTcb );

// ================================================================
// 標準関数
// ================================================================
// ================================================================
// ObjObjectViewOutCheck
/*!
  座標画面外チェック

  @param pWork [in] オブジェクトワークポインタ

  @return 0 画面内、 1画面外
 */
// ================================================================
extern s32 ObjObjectViewOutCheck( OBS_OBJECT_WORK * pWork );

// ================================================================
// 矩形
// ================================================================
// ================================================================
// ObjObjectRectRegist
/*!
  矩形登録

  @param pWork [in] オブジェワークポインタ
  @param pRec  [in] 登録矩形ポインタ
 */
// ================================================================
extern void ObjObjectRectRegist( OBS_OBJECT_WORK *pWork, struct _OBS_RECT_WORK* pRec );

// ================================================================
// ObjObjectGetRectBuf
/*!
	当り用バッファ取得

	@param pWork			[io] オブジェクトワークポインタ
	@param rect_work		[io] 矩形ワークバッファポインタ（NULLの場合、自動的にメモリを確保する、解放も自動で行う）
	@param rect_num			[in] 登録バッファ数 pRectがNULLの場合は取得するバッファ数 最大 OBD_OBJ_RECT_MAX まで

	@note
		既にObjObjectGetRectBufで取得したバッファがある場合は先に開放する。\n
		ユーザー設定のバッファの場合は何もしないで戻る
 */
// ================================================================
extern void ObjObjectGetRectBuf(OBS_OBJECT_WORK* pWork, struct _OBS_RECT_WORK *rect_work, u16 rect_num);

// ================================================================
// ObjObjectReleaseRectBuf
/*!
	当り用バッファ開放

	@param pWork			[io] オブジェクトワークポインタ
 */
// ================================================================
extern void ObjObjectReleaseRectBuf(OBS_OBJECT_WORK* pWork);

// ================================================================
// ObjObjectSetRectWork
/*!
	当りワーク設定

	@param	pWork		[io] オブジェクトワークポインタ
	@param	rect_work	[in] 矩形ワーク

  @note ワークには基本的な攻撃、防御設定が行われるので\n
        特殊な設定のものはこの関数の後で設定する
 */
// ================================================================
extern void ObjObjectSetRectWork(OBS_OBJECT_WORK* pWork, struct _OBS_RECT_WORK *rect_work);

#if 0
// ================================================================
// ObjObjectSetRectWorkIndex
/*!
	インデックス指定当り設定

	@param	pWork		[io] オブジェクトワークポインタ
	@param	index		[in] 矩形登録番号
	@param	rect_set_act	[in] TRUE アクションデータから矩形データを取得、FALSE 自動で矩形を設定しない

  @note ワークには基本的な攻撃、防御設定が行われるので\n
        特殊な設定のものはこの関数の後で設定する\n
        ObjObjectRectBufSetで設定済みのバッファに対して情報を設定します。\n
  		indexはrect_num(ObjObjectRectBufSet指定)を超えることは出来ません。
 */
// ================================================================
extern void ObjObjectSetRectWorkIndex(OBS_OBJECT_WORK* pWork, u16 index, BOOL rect_set_act);

// ================================================================
// ObjObjectRectSet
/*!
  当り設定

  @param pObj     [io] オブジェクトワークポインタ
  @param pRect    [io] 矩形ワークポインタ（NULLの場合、自動的にメモリを確保する、解放も自動で行う）
  @param usIndex  [in] 矩形登録番号（アクションの矩形番号でもある）
  @param bAction  [in] TRUE アクションデータから矩形データを取得、FALSE 自動で矩形を設定しない

  @note ワークには基本的な攻撃、防御設定が行われるので\n
        特殊な設定のものはこの関数の後で設定する
 */
// ================================================================
void ObjObjectRectSet ( OBS_OBJECT_WORK* pObj, struct _OBS_RECT_WORK * pRect, u16 usIndex, BOOL bAction );
#endif

// ================================================================
// ObjObjectRectGet
/*!
  当りポインタ取得

  @param pObj     [io] オブジェクトワークポインタ
  @param usIndex  [in] 矩形登録番号（アクションの矩形番号でもある）

 */
// ================================================================
extern struct _OBS_RECT_WORK* ObjObjectRectGet ( OBS_OBJECT_WORK* pObj, u16 usIndex );

// ================================================================
// 画面外チェック
// ================================================================
// ================================================================
// ObjViewOutCheck
/*!
  座標画面外チェック

  @param lPosX [in] チェックする座標 1:19:12
  @param lPosY [in] 
  @param sOfst [in] 画面外オフセット 1:15
  @param sLeft   [in] 画面外オフセット 1:15
  @param sTop    [in] 画面外オフセット 1:15
  @param sRight  [in] 画面外オフセット 1:15
  @param sBottom [in] 画面外オフセット 1:15

  @return 0 画面内、 1画面外
 */
// ================================================================
extern s32 ObjViewOutCheck( s32 lPosX, s32 lPosY, s16 sOfst, s16 sLeft, s16 sTop, s16 sRight, s16 sBottom );

// ================================================================
// Utility
// ================================================================
// ================================================================
// ObjObjectSpdDirFall
/*!
  重力方向計算

  @param sSpdX     [io] 速度Xポインタ
  @param sSpdY     [io] 速度Yポインタ
  @param ucDirFall [in] 重力角度
 */
// ================================================================
extern void ObjObjectSpdDirFall( s32 *sSpdX, s32 *sSpdY, u16 ucDirFall );

// ================================================================
// ObjObjectDirFallReverseCheck
/*!
  重力反転Check

  @return 1 反転中、 0 反転以外
 */
// ================================================================
extern u32 ObjObjectDirFallReverseCheck( u16 ucDirFall );

// ================================================================
// ObjTimeCountGet
/*!
  カウント値を処理速度にあわせる関数（スロー効果などを使いたい時は全てのタイマーで使用必須）
 
  @param time [in] カウント値 1:19:12
 
  @return   現在の処理速度にあわせたカウント値
 
 */
// ================================================================
extern fx32 ObjTimeCountGet( fx32 count );

// ================================================================
// ObjTimeCountDown
/*!
  タイマーを1(0x1000)ずつカウントダウンする
 
  @param time [in] タイマー値 1:19:12
 
  @return   現在の処理速度にあわせて0x1000カウントダウンしたタイマー値
 
 */
// ================================================================
extern fx32 ObjTimeCountDown( fx32 timer );

// ================================================================
// ObjTimeCountUp
/*!
  タイマーを1(0x1000)ずつカウントアップする
 
  @param time [in] タイマー値 1:19:12
 
  @return   現在の処理速度にあわせて0x1000カウントアップしたタイマー値
 
 */
// ================================================================
extern fx32 ObjTimeCountUp( fx32 timer );

// ================================================================
// ObjTimeCountDownF
/*!
  タイマーを1.fずつカウントダウンする
 
  @param time [in] タイマー値
 
  @return   現在の処理速度にあわせて1.fカウントダウンしたタイマー値
 
 */
// ================================================================
extern float ObjTimeCountDownF(float timer);

// ================================================================
// ObjTimeCountUpF
/*!
  タイマーを1.fずつカウントアップする
 
  @param time [in] タイマー値
 
  @return   現在の処理速度にあわせて1.fカウントアップしたタイマー値
 
 */
// ================================================================
extern float ObjTimeCountUpF(float timer);

// ================================================================
// ObjMapOutCheck
/*!
  マップ外チェック

  @param lPosX [in] チェックする座標 1:19:12
  @param lPosY [in] 
  @param sOfst [in] マップ外オフセット 1:15
  @param sLeft   [in] マップ外オフセット 1:15
  @param sTop    [in] マップ外オフセット 1:15
  @param sRight  [in] マップ外オフセット 1:15
  @param sBottom [in] マップ外オフセット 1:15

  @return 0 マップ内、 1 マップ外
 */
// ================================================================
extern s32 ObjMapOutCheck( s32 lPosX, s32 lPosY, s16 sOfst, s16 sLeft, s16 sTop, s16 sRight, s16 sBottom );

#if 0
// ================================================================
// ObjObjectActDsGet
/*!
  アクションポインタ取得

  @param pObj     [io] オブジェクトワークポインタ

 */
// ================================================================
extern MTS_ACTION_DS* ObjObjectActDsGet ( OBS_OBJECT_WORK* pObj);
#endif

// ================================================================
// ObjObjectMapOutCheck
/*!
  マップ外チェック

  @param pWork [in] オブジェクトワークポインタ

  @return 0 マップ内、 1 マップ外
 */
// ================================================================
extern s32 ObjObjectMapOutCheck( OBS_OBJECT_WORK * pWork );

// ================================================================
// ObjObjectFieldRectSet
/*!
  地形当り設定

  @param pObj    [io] オブジェクトワークポインタ
  @param cLeft   [in] 左端値
  @param cTop    [in] 上端値
  @param cRight  [in] 右端値
  @param cBottom [in] 下端値
    
 */
// ================================================================
extern void ObjObjectFieldRectSet( OBS_OBJECT_WORK* pObj, s16 cLeft, s16 cTop, s16 cRight, s16 cBottom);

// ================================================================
// ObjObjectRectSet
/*!
  当り設定

  @param pObj     [io] オブジェクトワークポインタ
  @param pRect    [io] 矩形ワークポインタ（NULLの場合、自動的にメモリを確保する、解放も自動で行う）
  @param usIndex  [in] 矩形登録番号（アクションの矩形番号でもある）
  @param bAction  [in] TRUE アクションデータから矩形データを取得、FALSE 自動で矩形を設定しない

  @note ワークには基本的な攻撃、防御設定が行われるので\n
        特殊な設定のものはこの関数の後で設定する
 */
// ================================================================
extern void ObjObjectRectSet( OBS_OBJECT_WORK* pObj, struct _OBS_RECT_WORK * pRect, u16 usIndex, BOOL bAction );



// ================================================================
// ObjObjectDirFallReverseCheck
/*!
  重力反転Check

  @return 1 反転中、 0 反転以外
 */
// ================================================================
extern u32 ObjObjectDirFallReverseCheck( u16 ucDirFall );

// ================================================================
// ObjObjectCollision
/*!
  オブジェクト分解地形チェック

  @param pWork [in] オブジェワークポインタ

 */
// ================================================================
extern void ObjObjectCollision( OBS_OBJECT_WORK *pWork );

// ================================================================
// ObjObjectMove
/*!
  オブジェクト移動

  @param pWork [in] オブジェワークポインタ
 */
// ================================================================
extern void ObjObjectMove( OBS_OBJECT_WORK *pWork );



#if	defined(__cplusplus)
} /* extern "C" */
#endif


#include "objDraw.h"
#include "objRectCheck.h"
//#include "objPalette.h"
#include "objObjectLoad.h"
#include "objCollision.h"
#include "objDiffCollisionField.h"
#include "objBlockCollisionField.h"
#include "objDiffCollisionCheck.h"
#include "objDiffCollisionObject.h"
#include "objCamera.h"
#include "objUtil.h"
#include "objTblWork.h"

#endif // _H_OBJBJECT


/*
 * Revision 1.50  2005/09/23 11:17:40  use1146
 * 範囲外チェックのオフセットを各要素ずつに設定できるように対応
 *
 * Revision 1.49  2005/09/17 09:08:19  use1146
 * 振動無視フラグ追加、FLOW無視フラグ追加
 *
 * Revision 1.48  2005/08/29 10:09:49  use1146
 * サウンド位置設定関数追加
 *
 * Revision 1.47  2005/08/11 13:32:35  use1146
 * 揺れ移動関数、引き数追加
 *
 * Revision 1.46  2005/08/08 09:07:10  use1146
 * パレットアニメ、オートスクロール対応
 *
 * Revision 1.45  2005/07/26 07:41:59  use1146
 * 揺れ動き関数追加、押しY軸削除
 *
 * Revision 1.44  2005/07/20 06:30:18  use1146
 * オブジェクト押し対応
 *
 * Revision 1.43  2005/07/19 06:24:02  use1146
 * アクション優先設定関数作成
 *
 * Revision 1.42  2005/07/11 10:38:50  use1146
 * NARCからファイル取り出し関数作成、モデルファイルからテクスチャのみ解放関数作成
 *
 * Revision 1.41  2005/06/28 12:26:44  use1146
 * azAction削除
 *
 * Revision 1.40  2005/06/24 09:19:20  use1146
 * 画面外判定、2画面別々対応
 *
 * Revision 1.39  2005/06/16 05:08:05  use1146
 * 地形フラグ追加
 *
 * Revision 1.38  2005/06/10 11:09:44  use1146
 * 3Dスプライト共有テクスチャ対応
 *
 * Revision 1.37  2005/06/03 02:45:07  use1146
 * DiffSet関数追加
 *
 * Revision 1.36  2005/05/31 09:06:14  use1146
 * BELT対応
 *
 * Revision 1.35  2005/05/26 07:47:01  use1146
 * 複数シェイプ表示対応
 *
 * Revision 1.34  2005/05/19 08:37:56  use1146
 * 重力変化対応、死亡範囲可変対応
 *
 * Revision 1.33  2005/05/13 10:26:09  use1146
 * AUTOキャラサイズ追加
 *
 * Revision 1.32  2005/05/12 11:58:51  use1146
 * 速度関数変更
 *
 * Revision 1.31  2005/05/09 05:19:19  use1146
 * 座標無視フラグ追加
 *
 * Revision 1.30  2005/05/03 07:05:10  use1146
 * UNDERPREVフラグ追加
 *
 * Revision 1.29  2005/04/25 06:23:21  use1146
 * NOUPDATE対応
 *
 * Revision 1.28  2005/04/22 05:56:15  use1146
 * ランダムマクロ追加
 *
 * Revision 1.27  2005/04/20 03:36:31  use1146
 * ポーズ追加
 *
 * Revision 1.26  2005/04/18 09:16:57  use1159
 * ライトベクトルを外部から設定できるように変更
 *
 * Revision 1.25  2005/04/15 02:15:55  use1146
 * 3dアクション外部使用に対応した引数に修正
 *
 * Revision 1.24  2005/04/08 12:48:57  use1146
 * ブレンドフラグ修正
 *
 * Revision 1.23  2005/04/07 06:28:52  use1146
 * 各3Dオブジェクト描画、データ管理対応
 *
 * Revision 1.22  2005/04/06 10:00:35  use1146
 * 拡縮対応
 *
 * Revision 1.21  2005/03/29 05:30:41  use1146
 * 親ポインタ追加
 *
 * Revision 1.20  2005/03/27 14:06:31  use1159
 * グローバル変数のパブリック化
 *
 * Revision 1.19  2005/03/24 06:05:22  use1146
 * ヒットストップ対応
 *
 * Revision 1.18  2005/03/22 06:32:27  use1146
 * ＯＡＭ分割表示追加
 *
 * Revision 1.17  2005/03/18 13:03:17  use1159
 * ライト向きの設定
 *
 * Revision 1.16  2005/03/17 11:35:26  use1146
 * 角度追加
 *
 * Revision 1.15  2005/03/16 12:39:17  use1159
 * プレイヤーを３つのライトで照らすように対応
 *
 * Revision 1.14  2005/03/15 08:48:49  use1146
 * デバッグ矩形表示を一括化
 *
 * Revision 1.13  2005/03/14 03:09:31  use1159
 * ライト固定フラグの追加
 *
 * Revision 1.12  2005/03/10 11:28:06  use1146
 * 足元厳密チェックフラグ追加
 *
 * Revision 1.11  2005/03/08 11:15:08  use1146
 * RIDE対応、共有VRAM対応
 *
 * Revision 1.10  2005/03/03 13:03:06  use1159
 * オブジェクトをカメラの正面に向ける処理の大幅変更
 * 描画時のカメラのポインタを格納する変数の追加
 *
 * Revision 1.9  2005/03/02 12:02:30  use1146
 * アーカイブ、3D対応
 *
 * Revision 1.8  2005/03/01 05:32:28  use1146
 * RIDE対応
 *
 * Revision 1.7  2005/02/25 08:33:32  use1146
 * フラグ追加
 *
 * Revision 1.6  2005/02/21 09:18:28  use1146
 * 複数パーツ表示対応
 *
 * Revision 1.5  2005/02/10 10:22:21  use1146
 * 地形フラグ追加
 *
 * Revision 1.4  2005/02/09 11:34:50  use1146
 * Prevやoffsetを追加
 *
 * Revision 1.3  2005/02/03 06:38:09  use1146
 * オブジェクト機能追加
 *
 * Revision 1.2  2005/01/27 05:47:08  use1146
 * 坂対応
 *
 * Revision 1.1  2005/01/20 03:09:27  use1146
 * 登録
 *
 */
