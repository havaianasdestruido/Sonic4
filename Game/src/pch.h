// プリプロセッサでリージョン定義されていないなら定義
#if !(_XBOX || _IPHONE)
#if !defined(HOG_RGN_JP) && !defined(HOG_RGN_US) && !defined(HOG_RGN_EU) && !defined(HOG_RGN_KR)

#define HOG_RGN_JP
//#define HOG_RGN_US
//#define HOG_RGN_EU
//#define HOG_RGN_KR	//HOG_RGN_USの定義も必要

#endif // !defined(HOG_RGN_JP) && !defined(HOG_RGN_US) && !defined(HOG_RGN_EU) && !defined(HOG_RGN_KR)
#endif // !(_XBOX || _IPHONE)

#include <alice.h>
#include "typedef.h"
#include "fx.h"
#include "mi.h"
#include "mt.h"
#include "mtMath.h"
#include "mtMemory.h"
#include "mtPad.h"
#include "mtTask.h"
#include "syEvtSys.h"

#if _WII
#include <dwc.h>
#include <nw4r/ut.h>
#include <nw4r/snd.h>
#include <revolution/sc.h>
#include <revolution/cx.h>
#endif // _WII

#if _PS3
#include <np.h>
#include <np/trophy.h>
#include <np/drm.h>
#include <sys/spu_initialize.h>
#include <sys/paths.h>
#include <netex/libnetctl.h>
#include <sysutil/sysutil_bgmplayback.h>
#endif // _PS3

#if _XBOX
#include "Game.spa.h"
#include <xmp.h>
#endif // _XBOX

#if _IPHONE
#include <TargetConditionals.h>
#endif

#include "ao.h"
#include "aoNetRank.h"
#include "aoPresence.h"
#include "aoAvatarAward.h"
#include "aoYsdFile.h"
#include "aoTvxFile.h"
