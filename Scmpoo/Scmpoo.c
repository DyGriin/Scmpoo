/* Allow unsafe CRT functions on Visual C++ 2005 and higher. */
#if _MSC_VER >= 1400
#define _CRT_SECURE_NO_DEPRECATE
#endif

#define STRICT

#include <stdlib.h>
#include <time.h>
#include <Windows.h>
#include <MMSystem.h>
#include <ShellAPI.h>

#include "resources.h"

/* GetActiveWindow is made local to current thread in 32-bit Windows.
 * The function of global scope is now GetForegroundWindow. */
#ifdef _WIN32
#define GetActiveWindow GetForegroundWindow
#endif

enum
{
    kMaxPooCount = 8,
    kISubPooHwnd = kMaxPooCount,
};

enum
{
    kWinkStand = 0,
    kWinkSit,
    kWinkUp,
    kWinkDown,
    kWinkDownFront,
    kWinkSitFront,
    kWinkCount,
};

enum
{
    kHangBack = 0,
    kHangFace,
    kHangCount,
};

enum
{
    kTouchTurn = 1,
    kTouchHit = 2,
};

enum
{
    kFallHang = 0,
    kFallRun = 1,
    kFallSide = 2,
    kFallByHit = 3,     //can't set by rand
    kFallSpFront = 0,
    kFallSpSide = 1,
};

enum
{
    kActionUnknown = 0,
    kActionInfRun,
    kActionFall,
    kActionInfSleep,
    kActionFeed,
};

enum
{
    kWmPeerPoo = WM_USER,
    kWmUserUnused,
    kWmConfigCmd,

    ///
    kPeerPooBorn = 1,
    kPeerPooDead = 2,
};

enum SpriteState
{
    kStateInit = 0,
    kStateNormalIntro = 1,
    kStateNormalGForceIntro = 2,
    kStateGForceVFallIntro = 3,
    kStateSpRunIntro = 4,
    kStateSpRunning = 5,
    kStateToLongAni = 6,
    kStateRunIntro = 7,     //8, break
    kStateRunning = 8,
    //8: 8/jump/hit
    kStateJumpIntro = 9,    // >10
    kStateJumping = 10,
    //10: 10/run
    kStateWalkIntro = 11,
    kStateWalking = 12,
    kStateHandstandIntro = 13,
    kStateHandstanding = 14,
    kStateSexIntro = 15,
    kStateSexing = 16,
    kStateSleepIntro = 17,
    kStateSleepWink = 18,
    kStateSleeping = 19,
    kStateWinkIntro = 20,
    kStateWinkOpenPre = 21,
    kStateWinking = 22,
    kStateWinkOpenPost = 23,
    kStateTurnIntro = 24,
    kStateTurning = 25,
    kStateTurnEnd = 26,
    kStateHitFallIntro = 27,
    kStateHitting = 28,
    kStateHitEnd = 29,
    kStateHitIntro = 30,
    kStateHitObsIntro = 31,
    kStateHitLayIntro = 32,
    kStateHitLaying = 33,
    kStateHitRolling = 34,
    kStatePissIntro = 35,
    kStatePissTurnPre = 37,
    kStatePissPre = 38,
    kStatePissing = 39,
    kStatePissPost = 40,
    kStatePissTurnPost = 41,
    kStateToNormal = 42,
    kStateYawnIntro = 43,
    kStateYawning = 44,
    kStateBaaIntro = 45,
    kStateBaaing = 46,
    kStateSneezeIntro = 47,
    kStateSneezing = 48,
    kStateAmazedIntro = 49,
    kStateAmazing = 50,
    kStateScaryIntro = 51,
    kStateScarying = 52,
    kStateEatIntro = 53,
    kStateEating = 54,
    kStateEatObsIntro = 55,
    kStateFeedIntro = 56,
    kStateFeeding = 57,
    kStateSitIntro = 58,
    kStateSitTurn = 59,
    kStateSitting = 60,
    kStateSitEnd = 61,
    kStateBlushIntro = 62,
    kStateBlushing = 63,
    kStateRollIntro = 65,
    kStateRollTurn = 66,
    kStateRolling = 67,
    kStateRollEnd = 68,
    kStateSpinIntro = 69,
    kStateSpining = 70,
    kStateSpinStun = 71,
    kStateSpinFall = 72,
    kStateRollSideIntro = 73,
    kStateRollingSide = 74,
    kStateSlipIntro = 75,
    kStateSlipTurn = 76,
    kStateSlipClimb = 77,
    kStateSliping = 78,
    kStateSlipStun = 79,
    kStateSlipEnd = 80,
    kStateInfRunIntro = 81,
    kStateInfRunning = 82,
    kStateWndRunIntro = 85,
    kStateScrRunning = 86,
    kStateWndPtRunIntro = 87,
    kStateWndPtRunning = 88,
    kStateWndPtClimb = 89,
    kStateWndPtTurn = 90,
    kStateWndPtEnd = 91,
    kStateFreeFallIntro = 92,
    kStateFreeFallLand = 93,
    kStateHRunFallIntro = 94,
    kStateHSlowRunHsaIntro = 95,
    kStateFallIntro = 96,
    kStateVRunFallIntro = 97,
    kStateSideFallIntro = 98,
    kStateFalling = 99,
    kStateFallBounce = 100,
    kStateSideFallEnd = 101,
    kStateInsituFallIntro = 102,
    kStateInsituFallPre = 103,
    kStateFrontSpFallIntro = 104,
    kStateSideSpFallIntro = 105,
    kStateSpFallWatch = 106,
    kStateSpFallWink = 107,
    kStateSpFallGaze = 108,
    kStateSpFallJump = 109,
    kStateSpFallObsJump = 110,
    kStateSpFallPre = 111,
    kStateSpFalling = 112,
    kStateInfSleepIntro = 113,
    kStateInfSleepWink = 114,
    kStateInfSleeping = 115,
    kStateMryPassIntro = 116,    ///top
    kStateMryPassRun = 117,
    kStateMryPassSub = 118,
    kStateMryPassing = 119,
    kStateMryPassBack = 120,
    kStateMryMeetIntro = 121,    ///meet
    kStateMryMeetRun = 122,
    kStateMryMeetBlush = 123,
    kStateMryMeetFade = 124,
    kStateMryMeetEnd = 125,
    kStateMryMateIntro = 126,    ///follow
    kStateMryMateRun = 127,
    kStateUFOTakeIntro = 128,
    kStateUFOTakeRun = 129,
    kStateUFOTakeSub = 130,
    kStateUFOTakeFly = 131,
    kStateUFOTakeBeam = 132,
    kStateUFOTaking = 133,
    kStateUFOTakeLeave = 134,
    kStateUFOEtIntro = 135,
    kStateUFOEtFly = 136,
    kStateUFOEtBeam = 137,
    kStateUFOEtAppear = 138,
    kStateUFOEtLeave = 139,
    kStateUFOEtFade = 140,
    kStateUFOEnterIntro = 142,
    kStateUFOEnterFly = 143,
    kStateUFOEnterRun = 144,
    kStateUFOEnterLeave = 145,
    kStateBurnIntro = 147,  // >148
    kStateBurning = 148,
    kStateBurnBath = 149,
    kStateBurnBathOut = 150,
    kStateBurnBathLeave = 151,
    kStateMax = 152,
};

enum SpriteId
{
    kSprNone = 173,
    kAniEnd = 0,  ///guard of ani array

    kSprSleep1 = 0,
    kSprSleep2 = 1,
    kSprWalk1 = 2,
    kSprWalk0 = 3,
    kSprRun1 = 4,
    kSprRun2 = 5,
    kSprWinkOpen = 6,
    kSprWinkClosed = 8,
    kSprFrontL = 9,
    kSprFront = 10,
    kSprFrontR = 11,
    kSprBackL = 12,
    kSprBack = 13,
    kSprBackR = 14,
    kSprWalkSide1 = 15,
    kSprWalkSide2 = 16,
    kSprJumpAir = 23,
    kSprJumpDown = 24,
    kSprJumpUp = 30,
    kSprSit1 = 31,
    kSprSitFront1 = 34,
    kSprFallSide1 = 40,
    kSprFallSide2 = 41,
    kSprHangBack1 = 42,
    kSprFallenSide = 45,
    kSprFallen = 48,
    kSprFallenSit = 49,
    kSprAmazed1 = 50,
    kSprAmazed2 = 51,
    kSprSexSide1 = 52,
    kSprSexSide2 = 53,
    kSprSexFront1 = 54,
    kSprSexFront2 = 55,
    kSprScary1 = 56,
    kSprScary2 = 57,
    kSprHitRollFirst = 63,
    kSprHitFallen = 66,
    kSprHitRollLast = 70,
    kSprSpinStun = 70,
    kSprLookUp1 = 73,
    kSprClimbSide = 76,
    kSprDown1 = 78,
    kSprDownFront1 = 81,
    kSprBathClimb1 = 81,
    kSprBathClimb3 = 83,
    kSprHandst1 = 86,
    kSprHandst2 = 87,
    kSprHandstFront1 = 88,
    kSprHandstFront2 = 89,
    kSprLaySide = 93,
    kSprSpinFall = 96,
    kSprPissPre = 103,
    kSprPissReady = 104,
    kSprPiss1 = 105,
    kSprPiss2 = 106,
    kSprBlush1 = 127,
    kSprClimb1 = 131,
    kSprClimb2 = 132,
    kSprClimbSlip = 133,
    kSprBurning1 = 137,
    kSprBurning2 = 138,
    kSprBurned1 = 144,
    kSprBurned2 = 145,
    /// sub
    kSprFlowerBegin = 149,
    kSprFlowerEnd = 153,    ///never played
    kSprWifeRun1 = 154,
    kSprWifeRun2 = 155,
    kSprWifeStand = 157,
    kSprBath = 146,
    kSprUfoFirst = 158,
    kSprUfoLast = 161,
    kSprUfoOnFirst = 162,
    kSprUfoOnLast = 165,
    kSprAlien0 = 166,
    kSprAlien1 = 167,
    kSprAlien2 = 168,
};

typedef struct SpriteInfo {
    HBITMAP bitmaps[2];
    int x;
    int y;
    int width;
    int height;
} SpriteInfo;

typedef struct ResourseInfo {
    int resId;
    WORD flags;
    SpriteInfo info;
} ResourseInfo;

typedef struct WindowInfo {
    HWND hWnd;
    RECT rect;
    BYTE padding[66]; /* Unused. */
} WindowInfo;

int maxPalIndex_ = 245; /* Palette search maximum index (unused). */
ResourseInfo originalRes_[16] = { /* Resource list. */
    {IDB_SPRITE1, 1, {{NULL, NULL}, 0, 0, 0, 0}},
    {IDB_SPRITE2, 1, {{NULL, NULL}, 0, 0, 0, 0}},
    {IDB_SPRITE3, 1, {{NULL, NULL}, 0, 0, 0, 0}},
    {IDB_SPRITE4, 1, {{NULL, NULL}, 0, 0, 0, 0}},
    {IDB_SPRITE5, 1, {{NULL, NULL}, 0, 0, 0, 0}},
    {IDB_SPRITE6, 1, {{NULL, NULL}, 0, 0, 0, 0}},
    {IDB_SPRITE7, 1, {{NULL, NULL}, 0, 0, 0, 0}},
    {IDB_SPRITE8, 1, {{NULL, NULL}, 0, 0, 0, 0}},
    {IDB_SPRITE9, 1, {{NULL, NULL}, 0, 0, 0, 0}},
    {IDB_SPRITEA, 1, {{NULL, NULL}, 0, 0, 0, 0}},
    {IDB_SPRITEB, 1, {{NULL, NULL}, 0, 0, 0, 0}}
};

ResourseInfo hFlipRes_[16] = {0}; /* Resource list storing flipped images. */
WORD nextStateNormal1_[80] = { /* Normal action table (option "Gravity always on" disabled). */
    kStateWalkIntro, kStateWalkIntro, kStateRunIntro, kStateRunIntro,
    kStateWalkIntro, kStateWalkIntro, kStateRunIntro, kStateRunIntro,
    kStateWalkIntro, kStateWalkIntro, kStateRunIntro, kStateRunIntro,
    kStateWalkIntro, kStateWalkIntro, kStateRunIntro, kStateRunIntro,
    kStateWalkIntro, kStateWalkIntro, kStateRunIntro, kStateRunIntro,
    kStateWalkIntro, kStateWalkIntro, kStateRunIntro, kStateRunIntro,
    kStateWalkIntro, kStateWalkIntro, kStateRunIntro, kStateRunIntro,
    kStateWalkIntro, kStateWalkIntro, kStateRunIntro, kStateRunIntro,
    kStateWalkIntro, kStateWalkIntro, kStateRunIntro, kStateRunIntro,
    kStateWalkIntro, kStateWalkIntro, kStateRunIntro, kStateRunIntro,
    kStateWalkIntro, kStateRunIntro, kStateSleepIntro, kStateWinkIntro,
    kStateWalkIntro, kStateRunIntro, kStateSleepIntro, kStateWinkIntro,
    kStateWalkIntro, kStateRunIntro, kStateSleepIntro, kStateWinkIntro,
    kStateWalkIntro, kStateRunIntro, kStateSleepIntro, kStateWinkIntro,
    kStateWalkIntro, kStateRunIntro, kStateSleepIntro, kStateWinkIntro,
    kStateWalkIntro, kStateRunIntro, kStateSleepIntro, kStateWinkIntro,
    kStateWalkIntro, kStateRunIntro, kStateSleepIntro, kStateWinkIntro,
    kStateHandstandIntro, kStateSitIntro, kStateSexIntro, kStateBaaIntro,
    kStatePissIntro, kStateEatIntro, kStateYawnIntro, kStateSneezeIntro,
    kStateBaaIntro, kStateSneezeIntro, kStateAmazedIntro, kStateScaryIntro
};
WORD nextStateNormalGForce_[80] = { /* Normal action table (option "Gravity always on" enabled). */
    kStateWalkIntro, kStateWalkIntro, kStateRunIntro, kStateRunIntro,
    kStateWalkIntro, kStateWalkIntro, kStateRunIntro, kStateRunIntro,
    kStateWalkIntro, kStateWalkIntro, kStateRunIntro, kStateRunIntro,
    kStateWalkIntro, kStateWalkIntro, kStateRunIntro, kStateRunIntro,
    kStateWalkIntro, kStateWalkIntro, kStateRunIntro, kStateRunIntro,
    kStateWalkIntro, kStateWalkIntro, kStateRunIntro, kStateRunIntro,
    kStateWalkIntro, kStateWalkIntro, kStateRunIntro, kStateRunIntro,
    kStateWalkIntro, kStateWalkIntro, kStateRunIntro, kStateRunIntro,
    kStateWalkIntro, kStateWalkIntro, kStateRunIntro, kStateRunIntro,
    kStateWalkIntro, kStateWalkIntro, kStateRunIntro, kStateRunIntro,
    kStateWalkIntro, kStateRunIntro, kStateSleepIntro, kStateWinkIntro,
    kStateWalkIntro, kStateRunIntro, kStateSleepIntro, kStateWinkIntro,
    kStateWalkIntro, kStateRunIntro, kStateSleepIntro, kStateWinkIntro,
    kStateWalkIntro, kStateRunIntro, kStateSleepIntro, kStateWinkIntro,
    kStateWalkIntro, kStateRunIntro, kStateSleepIntro, kStateWinkIntro,
    kStateWalkIntro, kStateRunIntro, kStateSleepIntro, kStateWinkIntro,
    kStateWalkIntro, kStateRunIntro, kStateSleepIntro, kStateWinkIntro,
    kStateHandstandIntro, kStateSitIntro, kStateSexIntro, kStateRollIntro,
    kStatePissIntro, kStateEatIntro, kStateYawnIntro, kStateSlipIntro,
    kStateBaaIntro, kStateSneezeIntro, kStateAmazedIntro, kStateScaryIntro
};
WORD nextStateToLongAni_[8] = { /* Special action table. */
    kStateMryPassIntro, kStateMryMeetIntro, kStateMryMateIntro, kStateBurnIntro,
    kStateUFOTakeIntro, kStateUFOEtIntro, kStateUFOEnterIntro, kStateBurnIntro
};
int xDirection_ = 1; /* Facing direction. 1 = left, -1 = right */
int xDirectionSub_ = 1; /* Facing direction (sub). 1 = left, -1 = right */
WORD aniWinks_[6][8] = { /* Blink animations. */
    {7, 8, 7, 6, 7, 8, 7, 6},
    {32, 33, 32, 31, 32, 33, 32, 31},
    {74, 75, 74, 73, 74, 75, 74, 73},
    {79, 80, 79, 78, 79, 80, 79, 78},
    {82, 83, 82, 81, 82, 83, 82, 81},
    {35, 36, 35, 34, 35, 36, 35, 34}
};
WORD aniHangs_[2][4] = { /* Hang on window top edge animations. */
    {42, 43, 42, 44},
    {46, 47, 46, 47}
};
WORD aniHit_[20] = { /* Collision animation with obsolete height offset. */
    62, 63, 63, 64, 64, 65, 65, 66, 66, 66,
    0, 10, 17, 21, 22, 21, 17, 10, 0, 0
};
WORD aniYawn_[11] = { /* Yawn animation. */
    37, 38, 39, 39, 39, 38, 37, 3, 37, 3, 0
};
WORD aniBaa_[8] = { /* Baa animation. */
    71, 72, 71, 72, 71, 72, 3, 0
};
WORD aniSneeze_[13] = { /* Sneeze animation. */
    107, 108, 109, 109, 3, 3, 3, 110, 111, 110, 111, 3, 0
};
WORD aniAmazed_[6] = { /* Amazed animation. */
    50, 51, 50, 51, 3, 0
};
WORD aniEat_[35] = { /* Eat animation. */
    58, 150, 60, 61, 60, 61, 60, 61, 58, 151, 60, 61, 60, 61, 60, 61, 2, 58, 152, 60, 61, 60, 61, 60, 61, 58, 153, 60, 61, 60, 61, 60, 61, 3, 0
};
WORD aniBurn_[34] = { /* Burn animation. */
    134, 134, 134, 134, 134, 134, 134, 134, 135, 136, 137, 138, 137, 138, 137, 138, 137, 138, 137, 138, 139, 140, 141, 142, 143, 144, 145, 144, 145, 144, 145, 144, 145, 0
};
WORD aniRollSide_[13] = { /* Roll over animation (not used). */
    3, 93, 99, 100, 99, 100, 99, 100, 99, 100, 95, 3, 0
};
WORD aniStandUpL_[8] = { /* Get up animation (left). */
    48, 48, 48, 49, 13, 12, 3, 0
};
WORD aniStandUpR_[8] = { /* Get up animation (right). */
    48, 48, 48, 49, 13, 14, 3, 0
};
WORD aniMerryBlush_[28] = { /* Merry 2 animation. */
    130, 130, 130, 130, 130, 129, 129, 128, 128, 127, 127, 127, 6, 6, 6, 6, 7, 8, 7, 6, 7, 8, 7, 6, 6, 6, 6, 0
};
WORD aniBath_[5] = { /* Burn bathtub splash animation. */
    147, 148, 147, 146, 0
};
WORD aniBathOut_[55] = { /* Burn get out of bathtub animation. */
    169, 169, 169, 169, 169, 169, 169, 169, 170, 171, 170, 169, 170, 171,
    170, 169, 169, 169, 169, 81, 81, 81, 81, 81, 81, 81, 81, 85, 85, 85,
    85, 85, 85, 85, 85, 34, 34, 34, 34, 35, 36, 35, 34, 35, 36, 35, 34,
    34, 34, 10, 10, 9, 9, 3, 0
};
WORD aniBlush_[12] = { /* Blush animation. */
    3, 127, 128, 129, 130, 130, 130, 129, 128, 127, 127, 0
};
WORD aniRoll_[9] = { /* Roll animation. */
    119, 120, 121, 122, 123, 124, 125, 126, 0
};
WORD aniSpin_[8] = { /* Spin animation. 0-3: face, 4-7: back */
    3, 9, 10, 11, 2, 14, 13, 12
};
WORD isCursorMoved_ = 0; /* Has cursor position changed in current timer period? */
int posx_A79A = 0; /* Cursor position with respect to screen, X-coordinate */
int posy_A79C = 0; /* Cursor position with respect to screen, Y-coordinate */
WORD isMouseBtnDown_ = 0; /* Dragging Screen Mate window? */
WORD isRBtnDbClicked_ = 0; /* Destroy Screen Mate window by right double-click? */
WORD hasTimerMsgEver_ = 0; /* Unused. */
RECT lastWndRect_ = {0, 0, 0, 0}; /* Screen Mate window rectangle. */
WORD skipNextPaintMsg_ = 0; /* Not to clear window on WM_PAINT? */
POINT lastCursorPos_ = {0, 0}; /* Current cursor position. */
WORD skipNextSubPaintMsg_ = 0; /* Not to clear window on WM_PAINT? (sub) */
HBITMAP paintBmpBuf_[2] = {NULL, NULL}; /* Double buffer. */
HBITMAP bmp_A7BA = NULL; /* Sprite render target. */
HBITMAP curSpriteBmp0_ = NULL; /* Sprite colour image for current frame. */
HBITMAP curSpriteBmp1_ = NULL; /* Sprite mask image for current frame. */
int curSpriteBmpX_ = 0; /* Sprite X-coordinate on resource image for current frame. */
int curSpriteBmpY_ = 0; /* Sprite Y-coordinate on resource image for current frame. */
int lastPaintBmpX_ = 0; /* Sprite X-coordinate on resource image for previous frame. */
int lastPaintBmpY_ = 0; /* Sprite Y-coordinate on resource image for previous frame (unused). */
WORD iPaintBmp_ = 0; /* Current framebuffer index. */
WORD bool_A7D2 = 0; /* 0 to render sprite; 1 to update window. */
WORD bool_A7D4 = 0; /* Unused. */
HBITMAP lastPaintBmp_ = NULL; /* Sprite colour image for previous frame. */
int curSpriteX_ = 0; /* Screen X-coordinate for current frame. */
int curSpriteY_ = 0; /* Screen Y-coordinate for current frame. */
int curSpriteBmpW_ = 0; /* Sprite width for current frame. */
int curSpriteBmpH_ = 0; /* Sprite height for current frame. */
int paintRectX_ = 0; /* Update area rectangle X-coordinate for current frame. */
int paintRectY_ = 0; /* Update area rectangle Y-coordinate for current frame. */
int paintRectW_ = 0; /* Update area rectangle width for current frame. */
int paintRectH_ = 0; /* Update area rectangle height for current frame. */
int lastPaintRectX_ = 0; /* Update area rectangle X-coordinate for previous frame. */
int lastPaintRectY_ = 0; /* Update area rectangle Y-coordinate for previous frame. */
int lastPaintRectW_ = 0; /* Update area rectangle width for previous frame. */
int lastPaintRectH_ = 0; /* Update area rectangle height for previous frame. */
int lastPaintX_ = 0; /* Screen X-coordinate for previous frame. */
int lastPaintY_ = 0; /* Screen Y-coordinate for previous frame. */
int lastPaintBmpW_ = 0; /* Sprite width for previous frame. */
int lastPaintBmpH_ = 0; /* Sprite height for previous frame. */
WORD word_A7FA = 0; /* Current frame rectangle and previous frame rectangle have no intersecion? (unused) */
WORD bool_A7FC = 0; /* Is gravity enabled? */
WORD canHit_ = 0; /* Is collision with visible window enabled? */
int xPoo_ = 0; /* Current X-coordinate. */
int yPoo_ = 0; /* Current Y-coordinate. */
int iSprite_ = 0; /* Sprite index. */
int dyPoo_ = 0; /* Vertical speed. */
int dxPoo_ = 0; /* Horizontal speed. */
int yPooStored_ = 0; /* Y-coordinate memory. */
int xSubPoo_ = 0; /* Current X-coordinate (sub). */
int ySubPoo_ = 0; /* Current Y-coordinate (sub). */
int iSubSprite_ = 0; /* Sprite index (sub). */
HWND hPeerWnd_ = NULL; /* Active window or window to land on. */
RECT rect_A81E = {0L, 0L, 0L, 0L}; /* Rectangle of active window or window to land on. */
int iAniFrames_ = 0; /* Animation frame counter. */
int frameCount_ = 0; /* Random duration period counter. */
int randAct_ = 0; /* Random case number for action. */
WORD needWalk_ = 0; /* Unused. */
HGLOBAL hPlaySoundData_ = NULL; /* Global handle for holding WAVE resource in memory. */
int lastClockHour_ = 0; /* Current time hour. */
int chimeCount_ = 0; /* Remaining times for chime. */
DWORD lastChimeTick_ = 0; /* Tick count. */
int chimeCheckCounter_ = 0; /* Time check period counter. */
int counter_A83A = 0; /* Frame period counter. */
int targetX_ = 0; /* Target X-coordinate for window edge attachment. */
int targetY_ = 0; /* Target Y-coordinate for window edge attachment. */
WORD hasBounce_ = 0; /* Bounce when falling? */
int fallType_ = 0; /* Case number for fall action. */
int dyHitObs_ = 0; /* Collision vertical speed (unused). */
int word_A846 = 0; /* Collision spin frame counter (unused). */
WORD iAniHitFrame_ = 0; /* Collision animation frame index. */
int word_A84A = 0; /* Known instance list update period counter. */
HBITMAP paintSubBmpBuf_[2] = {NULL, NULL}; /* Double buffer (sub). */
HBITMAP bmp_A854 = NULL; /* Sprite render target (sub). */
HBITMAP curSubSpriteBmp0_ = NULL; /* Sprite colour image for current frame (sub). */
HBITMAP curSubSpriteBmp1_ = NULL; /* Sprite mask image for current frame (sub). */
HBITMAP bmp_A85A = NULL; /* Fade out processed colour image (sub). */
HBITMAP bmp_A85C = NULL; /* Fade out processed mask image (sub). */
int curSubSpriteBmpX_ = 0; /* Sprite X-coordinate on resource image for current frame (sub). */
int curSubSpriteBmpY_ = 0; /* Sprite Y-coordinate on resource image for current frame (sub). */
int lastSubSpriteBmpX_ = 0; /* Sprite X-coordinate on resource image for previous frame (sub). */
int lastSubSpriteBmpY_ = 0; /* Sprite Y-coordinate on resource image for previous frame (sub) (unused). */
WORD iPaintSubBmp_ = 0; /* Current framebuffer index (sub). */
WORD bool_A870 = 0; /* 0 to render sprite; 1 to update window (sub). */
WORD bool_A872 = 0; /* Unused. */
HBITMAP lastSubSpriteBmp0_ = NULL; /* Sprite colour image for previous frame (sub). */
int curSubSpriteX_ = 0; /* Screen X-coordinate for current frame (sub). */
int curSubSpriteY_ = 0; /* Screen Y-coordinate for current frame (sub). */
int curSubSpriteBmpW_ = 0; /* Sprite width for current frame (sub). */
int curSubSpriteBmpH_ = 0; /* Sprite height for current frame (sub). */
int paintSubRectX_ = 0; /* Update area rectangle X-coordinate for current frame (sub). */
int paintSubRectY_ = 0; /* Update area rectangle Y-coordinate for current frame (sub). */
int paintSubRectW_ = 0; /* Update area rectangle width for current frame (sub). */
int paintSubRectH_ = 0; /* Update area rectangle height for current frame (sub). */
int lastPaintSubRectX_ = 0; /* Update area rectangle X-coordinate for previous frame (sub). */
int lastPaintSubRectY_ = 0; /* Update area rectangle Y-coordinate for previous frame (sub). */
int lastPaintSubRectW_ = 0; /* Update area rectangle width for previous frame (sub). */
int lastPaintSubRectH_ = 0; /* Update area rectangle height for previous frame (sub). */
int lastSubSpriteX_ = 0; /* Screen X-coordinate for previous frame (sub). */
int lastSubSpriteY_ = 0; /* Screen Y-coordinate for previous frame (sub). */
int lastSubSpriteBmpW_ = 0; /* Sprite width for previous frame (sub). */
int lastSubSpriteBmpH_ = 0; /* Sprite height for previous frame (sub). */
WORD word_A898 = 0; /* Current frame rectangle and previous frame rectangle have no intersecion? (sub) (unused) */
WORD pooState_ = kStateInit; /* State. */
SpriteInfo sprites_[512] = {{{NULL, NULL}, 0, 0, 0, 0}}; /* Sprite list. First 256 for left-facing sprites, last 256 for right-facing sprites. */
int cursorIdleCount_ = 0; /* No mouse action consecutive period counter. */
UINT confChime_ = 0U; /* Configuration: Chime */
WORD bool_C0AE = 0; /* Screen Mate window on top of subwindow? (unused) */
HWND hPooWnd_ = NULL; /* Self instance window handle. */
HBITMAP bmp_C0B2 = NULL; /* UFO beam render target. */
HBRUSH brush_C0B4 = NULL; /* UFO beam paint colour brush. */
UINT confNoSleep_ = 0U; /* Configuration: Always moving */
HBITMAP bmp_C0B8 = NULL; /* UFO beam colour rectangle image. */
WORD word_C0BA = 0; /* Remaining no-update periods after clearing windows. */
WindowInfo otherVWnds_[32] = {{NULL, {0, 0, 0, 0}, {0}}}; /* Currently visible window list. */
WORD hasOtherPoo_ = 0; /* Prevent special actions? */
WORD confTopMost__ = 0; /* Always on top? (unused) */
int otherPooCount_ = 0; /* Known instance count. */
UINT confGForce_ = 0U; /* Configuration: Gravity always on */
HBRUSH brush_CA44 = NULL; /* UFO beam mask colour brush. */
int subPooFade_ = 0; /* Fade out frame counter. */
int otherPooCount2_ = 0; /* Unused. */
HPALETTE hPalette_ = NULL; /* Palette being used by window. */
int word_CA4C = 0; /* Unused. */
int word_CA4E = 0; /* Unused. */
int screenWidth_ = 0; /* Screen width. */
int screenHeight_ = 0; /* Screen height. */
WORD pooSleepState_ = 0; /* Temporarily holds sleep timeout action. */
WORD keepSubPoo_ = 0; /* Not to clear subwindow? */
HINSTANCE hSelfInst_ = NULL; /* Current instance. */
UINT confSound_ = 0U; /* Configuration: Cry */
int height_CA5C = 0; /* UFO beam height (sub). */
WORD word_CA5E = 0; /* Unused. */
HWND hOtherPooWnd_[kMaxPooCount+1] = {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL}; /* Known instance list. When no other instance exists, [8] is used to store subwindow handle. */
int ufoBeamHeight_ = 0; /* UFO beam height. */
int otherVWndCount_ = 0; /* Number of currently visible windows. */
WORD needSleepAfterChime_ = 0; /* Sleeping after timeout? */
WORD word_CA78 = 0; /* Unused. */
#ifdef _WIN32
HWND hOwnerWnd_ = NULL;
#endif

void PASCAL sub_10(void FAR *, void FAR *);
int PASCAL sub_114(void FAR *, void FAR *, int);
void PASCAL sub_230(void FAR *, void FAR *);
#define BICOLORCOUNT(p) ((((BITMAPINFOHEADER FAR *)p)->biClrUsed == 0) ? ((DWORD)1 << ((BITMAPINFOHEADER FAR *)p)->biBitCount) : (((BITMAPINFOHEADER FAR *)p)->biClrUsed))
WORD readBmpPaletteSize(void FAR *);
WORD readBmpColorCount(void FAR *);
HPALETTE createPaletteFromBmp(HDC hdc, void FAR *bmpData);
HPALETTE createPaletteFromRgb(HDC, BYTE, BYTE, BYTE);
void palNearestCopy(HDC, HPALETTE, HPALETTE, int);
WORD getBmpColorOfFirstPixel(void FAR *bmpData);
void FAR * getBmpPixelBegin(void FAR *data);
void bmpFlipCopy(void FAR *, void FAR *, UINT flags);
int PASCAL WinMain(HINSTANCE, HINSTANCE, LPSTR, int);
void unused_1DDC(void);
LRESULT CALLBACK pooWndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK pooSubWndProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK configDlgProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK debugDlgProc(HWND, UINT, WPARAM, LPARAM);
void showSubPoo(void);
void destroySubPooWnd(void);
void putWndToTop(HWND hWnd);
void setWndOnto(HWND, HWND);
BOOL loadSprite(HDC, SpriteInfo *, int resId, int);
void releaseSprite(SpriteInfo *sprite);
void readConfig(void);
void writeProfile(LPCSTR, LPCSTR, UINT, LPCSTR);
void saveConfig(void);
BOOL initBitmaps(HWND);
void releaseWndBmp();
void setSpriteInfo(int, int, int index);
void repaintPoo(HWND);
void paint_3284(HWND hWnd);
void paint_3717(HWND hWnd);
void unused_399D(HWND, int, int, int, int);
BOOL isOtherPoo(HWND);
int calcPooHitX(int, int, int, int);
void checkOtherPoo_3B4C(HWND hWnd);
BOOL checkOtherPoo(HWND);
void notifyPooDead(HWND);
void pushOtherPoo(HWND);
void eraseOtherPoo(HWND hWnd);
void updateVWnds(void);
int getVWndHitXInfo(HWND *, int, int, int, int);
int getLandInfo(HWND *, int, int, int, int);
int getWndLandY(HWND, int, int, int, int);
void playSoundRes(int, UINT, WORD);
void stopPlaySound(void);
void asyncPlaySound(LPCSTR path);
void pooSound(int, UINT, WORD);
BOOL initResources(HDC hdc);
void releaseResources(void);
void checkPooTurn(void);
void checkPooHit(BOOL);
void aniFrameDownToNormal(void);
void processChime(void);
void setSprite(int, int, int);
void setSubSprite(int, int, int);
BOOL isLandableWnd(HWND);
void getWndRectSp(HWND, LPRECT);
void land_496F(int);
void checkPtClimbFall(void);
void procPooTouchX(int, int, int);
int getPooHitX(int, int);
void resetState(void);
void stateUpdate(void);
void postAction(int);
void debugAction(WPARAM);
void movePooWnd(int, int);
BOOL initSubWnd(HWND);
void releaseSubWnd();
void setSubSpriteInfo(int, int, int);
void repaintSubPoo(HWND);
void paint_9438(HWND);
BOOL paint_9A49(HWND);

/* Make mask bitmap image out of the first pixel (by simulating x86 assembly). */
void PASCAL sub_10(void FAR * arg_4, void FAR * arg_0)
{
#define ax (LOWORD(eax))
#define cx (LOWORD(ecx))
#define STACK_SIZE (2 * sizeof(WORD))
    BYTE var_2;
    WORD var_4;
    WORD var_6;

    HLOCAL stack = LocalAlloc(LMEM_FIXED, STACK_SIZE);

    register DWORD eax;
    register BYTE al;
    register DWORD ebx;
    register DWORD ecx;
    register WORD dx;
    register BYTE FAR * source;
    register BYTE FAR * destination;
    register BYTE * sp = (BYTE *)stack + STACK_SIZE;

    al = 0; /* xor eax, eax */
    ecx = 0; /* xor ecx, ecx */
    dx = 0; /* xor edx, edx */
    source = arg_0; /* lds si, [bp+arg_0] */
    destination = arg_4; /* les di, [bp+arg_4] */
    eax = (WORD)BICOLORCOUNT(source); /* call BICOLORCOUNT */
    sp -= sizeof(WORD); /* push ax */
    *(WORD *)sp = ax;
    *(DWORD FAR *)destination = *(DWORD FAR *)source; /* movsd; biSize */
    source += 4;
    destination += 4;
    eax = (*(DWORD FAR *)source); /* lodsd */
    source += 4;
    var_6 = LOWORD(eax); /* mov [bp+var_6], ax; var_6 = biWidth */
    *(DWORD FAR *)destination = eax; /* stosd; biWidth */
    destination += 4;
    eax = (*(DWORD FAR *)source); /* lodsd */
    source += 4;
    ecx = eax; /* mov ecx, eax; ecx = biHeight */
    *(DWORD FAR *)destination = eax; /* stosd; biHeight */
    destination += 4;
    eax = (*(DWORD FAR *)source); /* lodsd */
    source += 4;
    eax = 0x10001; /* mov eax, 10001h */
    *(DWORD FAR *)destination = eax; /* stosd; biPlanes = 1, biBitCount = 1 */
    destination += 4;
    *(DWORD FAR *)destination = *(DWORD FAR *)source; /* movsd; biCompression */
    source += 4;
    destination += 4;
    eax = (*(DWORD FAR *)source); /* lodsd; biSizeImage */
    source += 4;
    var_4 = (var_6 + 31) / 32; /* mov ax, [bp+var_6]; add ax, 31; shr ax, 5; mov [bp+var_4], ax */
    eax = var_4 * 4 * cx; /* shl ax, 2; mul cx */
    *(DWORD FAR *)destination = eax; /* stosd; biSizeImage = ceil(biWidth, 32) * 4 * biHeight */
    destination += 4;
    *(DWORD FAR *)destination = *(DWORD FAR *)source; /* movsd; biXPelsPerMeter */
    source += 4;
    destination += 4;
    *(DWORD FAR *)destination = *(DWORD FAR *)source; /* movsd; biYPelsPerMeter */
    source += 4;
    destination += 4;
    eax = (*(DWORD FAR *)source); /* lodsd */
    source += 4;
    eax = 2; /* mov eax, 2 */
    *(DWORD FAR *)destination = eax; /* stosd; biClrUsed */
    destination += 4;
    eax = (*(DWORD FAR *)source); /* lodsd */
    source += 4;
    eax = 2; /* mov eax, 2 */
    *(DWORD FAR *)destination = eax; /* stosd; biClrImportant */
    destination += 4;
    eax = *(WORD *)sp; /* pop ax */
    sp += sizeof(WORD);
    source += ax * sizeof(RGBQUAD); /* shl ax, 2; add si, ax */
    eax = 0x00FFFFFF; /* mov eax, 0FFFFFFh */
    *(DWORD FAR *)destination = eax; /* stosd */
    destination += 4;
    eax = 0; /* xor eax, eax */
    *(DWORD FAR *)destination = eax; /* stosd */
    destination += 4;
    var_2 = *source; /* mov al, [esi]; mov [bp+var_2], al; Read the first colour */
    do {
        sp -= sizeof(WORD); /* push cx */
        *(WORD *)sp = cx;
        dx = var_6; /* mov dx, [bp+var_6]; dx = biWidth */
        ecx = var_4; /* mov cx, [bp+var_4]; ceil(biWidth, 32) */
        do {
            sp -= sizeof(WORD); /* push cx */
            *(WORD *)sp = cx;
            ebx = 0; /* xor ebx, ebx */
            ecx = 8; /* mov cx, 8 */
            do {
                if ((short)dx > 0) { /* cmp dx, 0; jle short loc_F0 */
                    eax = (*(DWORD FAR *)source); /* lods dword ptr [esi] */
                    source += 4;
                    dx -= 4; /* sub dx, 4 */
                    al = LOBYTE(ax) == var_2 ? 1 : 0; /* cmp al, [bp+var_2]; setz al; */
                    ebx = ebx << 1 | (al == 0 ? 1 : 0); /* cmp al, 1; rcl ebx, 1 */
                    eax >>= 8; /* shr eax, 8 */
                    al = LOBYTE(ax) == var_2 ? 1 : 0; /* cmp al, [bp+var_2]; setz al; */
                    ebx = ebx << 1 | (al == 0 ? 1 : 0); /* cmp al, 1; rcl ebx, 1 */
                    eax >>= 8; /* shr eax, 8 */
                    al = LOBYTE(ax) == var_2 ? 1 : 0; /* cmp al, [bp+var_2]; setz al; */
                    ebx = ebx << 1 | (al == 0 ? 1 : 0); /* cmp al, 1; rcl ebx, 1 */
                    eax >>= 8; /* shr eax, 8 */
                    al = LOBYTE(ax) == var_2 ? 1 : 0; /* cmp al, [bp+var_2]; setz al; */
                    ebx = ebx << 1 | (al == 0 ? 1 : 0); /* cmp al, 1; rcl ebx, 1 */
                    eax >>= 8; /* shr eax, 8 */
                } else { /* jmp short loc_F4 */
                    ebx <<= 4; /* shl ebx, 4 */
                }
            } while (--ecx != 0); /* loop loc_A4; 8 */
            eax = (DWORD)LOBYTE(LOWORD(ebx)) << 24 | (DWORD)HIBYTE(LOWORD(ebx)) << 16 | (DWORD)LOBYTE(HIWORD(ebx)) << 8 | (DWORD)HIBYTE(HIWORD(ebx)); /* mov eax, ebx; xchg al, ah; ror eax, 16; xchg al, ah */
            *(DWORD FAR *)destination = eax; /* stos dword ptr es:[edi] */
            destination += 4;
            ecx = *(WORD *)sp; /* pop cx */
            sp += sizeof(WORD);
        } while (--ecx != 0); /* loop loc_9D; ceil(biWidth, 32) */
        ecx = *(WORD *)sp; /* pop cx */
        sp += sizeof(WORD);
    } while (--ecx != 0); /* loop loc_96; biHeight */

    LocalFree(stack);
#undef STACK_SIZE
#undef cx
#undef ax
}

/* Make mask bitmap image out of the specific colour index in the palette (by simulating x86 assembly) (unused). */
int PASCAL sub_114(void FAR * arg_6, void FAR * arg_2, int arg_0)
{
#define ax (LOWORD(eax))
#define bl (LOBYTE(LOWORD(ebx)))
#define cx (LOWORD(ecx))
#define STACK_SIZE (sizeof(WORD) + sizeof(BYTE FAR *))
    BYTE var_2;
    int var_4;
    WORD var_6;
    WORD var_8;

    HLOCAL stack = LocalAlloc(LMEM_FIXED, STACK_SIZE);

    register DWORD eax;
    register BYTE al;
    register DWORD ebx;
    register DWORD ecx;
    register WORD dx;
    register BYTE FAR * source;
    register BYTE FAR * destination;
    register BYTE * sp = (BYTE *)stack + STACK_SIZE;

    var_4 = 0; /* mov [bp+var_4], 0 */
    al = 0; /* xor eax, eax */
    ecx = 0; /* xor ecx, ecx */
    dx = 0; /* xor edx, edx */
    source = arg_2; /* lds si, [bp+arg_2] */
    destination = arg_6; /* les di, [bp+arg_6] */
    eax = (WORD)BICOLORCOUNT(source); /* call BICOLORCOUNT */
    sp -= sizeof(WORD); /* push ax */
    *(WORD *)sp = ax;
    *(DWORD FAR *)destination = *(DWORD FAR *)source; /* movsd; biSize */
    source += 4;
    destination += 4;
    eax = (*(DWORD FAR *)source); /* lodsd */
    source += 4;
    var_8 = LOWORD(eax); /* mov [bp+var_8], ax; var_8 = biWidth */
    *(DWORD FAR *)destination = eax; /* stosd; biWidth */
    destination += 4;
    eax = (*(DWORD FAR *)source); /* lodsd */
    source += 4;
    ecx = eax; /* mov ecx, eax; ecx = biHeight */
    *(DWORD FAR *)destination = eax; /* stosd; biHeight */
    destination += 4;
    eax = (*(DWORD FAR *)source); /* lodsd */
    source += 4;
    eax = 0x10001; /* mov eax, 10001h */
    *(DWORD FAR *)destination = eax; /* stosd; biPlanes = 1, biBitCount = 1 */
    destination += 4;
    *(DWORD FAR *)destination = *(DWORD FAR *)source; /* movsd; biCompression */
    source += 4;
    destination += 4;
    eax = (*(DWORD FAR *)source); /* lodsd; biSizeImage */
    source += 4;
    var_6 = (var_8 + 31) / 32; /* mov ax, [bp+var_8]; add ax, 31; shr ax, 5; mov [bp+var_6], ax */
    eax = var_6 * 4 * cx; /* shl ax, 2; mul cx */
    *(DWORD FAR *)destination = eax; /* stosd; biSizeImage = ceil(biWidth, 32) * 4 * biHeight */
    destination += 4;
    *(DWORD FAR *)destination = *(DWORD FAR *)source; /* movsd; biXPelsPerMeter */
    source += 4;
    destination += 4;
    *(DWORD FAR *)destination = *(DWORD FAR *)source; /* movsd; biYPelsPerMeter */
    source += 4;
    destination += 4;
    eax = (*(DWORD FAR *)source); /* lodsd */
    source += 4;
    eax = 2; /* mov eax, 2 */
    *(DWORD FAR *)destination = eax; /* stosd; biClrUsed */
    destination += 4;
    eax = (*(DWORD FAR *)source); /* lodsd */
    source += 4;
    eax = 2; /* mov eax, 2 */
    *(DWORD FAR *)destination = eax; /* stosd; biClrImportant */
    destination += 4;
    sp -= sizeof(BYTE FAR *); /* push esi */
    *(BYTE FAR * *)sp = source;
    source += arg_0 * sizeof(RGBQUAD); /* mov ax, [bp+arg_0]; shl ax, 2; add si, ax */
    *(DWORD FAR *)destination = *(DWORD FAR *)source; /* movsd; Mask colour */
    source += 4;
    destination += 4;
    eax = 0; /* xor eax, eax */
    *(DWORD FAR *)destination = eax; /* stosd */
    destination += 4;
    source = *(BYTE FAR * *)sp; /* pop esi */
    sp += sizeof(BYTE FAR *);
    eax = *(WORD *)sp; /* pop ax */
    sp += sizeof(WORD);
    source += ax * sizeof(RGBQUAD); /* shl ax, 2; add si, ax */
    var_2 = LOBYTE(arg_0); /* mov ax, [bp+arg_0]; mov [bp+var_2], al; The specified colour */
    do {
        sp -= sizeof(WORD); /* push cx */
        *(WORD *)sp = cx;
        dx = var_8; /* mov dx, [bp+var_8]; dx = biWidth */
        ecx = var_6; /* mov cx, [bp+var_6]; ceil(biWidth, 32) */
        do {
            sp -= sizeof(WORD); /* push cx */
            *(WORD *)sp = cx;
            ebx = 0; /* xor ebx, ebx */
            ecx = 8; /* mov cx, 8 */
            do {
                if ((short)dx > 0) { /* cmp dx, 0; jle short loc_209 */
                    eax = (*(DWORD FAR *)source); /* lods dword ptr [esi] */
                    source += 4;
                    dx -= 4; /* sub dx, 4 */
                    al = LOBYTE(ax) == var_2 ? 1 : 0; /* cmp al, [bp+var_2]; setz al; */
                    ebx = ebx << 1 | (al == 0 ? 1 : 0); /* cmp al, 1; rcl ebx, 1 */
                    eax >>= 8; /* shr eax, 8 */
                    al = LOBYTE(ax) == var_2 ? 1 : 0; /* cmp al, [bp+var_2]; setz al; */
                    ebx = ebx << 1 | (al == 0 ? 1 : 0); /* cmp al, 1; rcl ebx, 1 */
                    eax >>= 8; /* shr eax, 8 */
                    al = LOBYTE(ax) == var_2 ? 1 : 0; /* cmp al, [bp+var_2]; setz al; */
                    ebx = ebx << 1 | (al == 0 ? 1 : 0); /* cmp al, 1; rcl ebx, 1 */
                    eax >>= 8; /* shr eax, 8 */
                    al = LOBYTE(ax) == var_2 ? 1 : 0; /* cmp al, [bp+var_2]; setz al; */
                    ebx = ebx << 1 | (al == 0 ? 1 : 0); /* cmp al, 1; rcl ebx, 1 */
                    eax >>= 8; /* shr eax, 8 */
                    al = bl; /* mov al, bl */
                    al &= 0x0F; /* and al, 0Fh */
                    var_4 += al < 0x0F ? 1 : 0; /* cmp al, 0Fh; adc [bp+var_4], 0 */
                } else { /* jmp short loc_20D */
                    ebx <<= 4; /* shl ebx, 4 */
                }
            } while (--ecx != 0); /* loop loc_1B3; 8 */
            eax = (DWORD)LOBYTE(LOWORD(ebx)) << 24 | (DWORD)HIBYTE(LOWORD(ebx)) << 16 | (DWORD)LOBYTE(HIWORD(ebx)) << 8 | (DWORD)HIBYTE(HIWORD(ebx)); /* mov eax, ebx; xchg al, ah; ror eax, 16; xchg al, ah */
            *(DWORD FAR *)destination = eax; /* stos dword ptr es:[edi] */
            destination += 4;
            ecx = *(WORD *)sp; /* pop cx */
            sp += sizeof(WORD);
        } while (--ecx != 0); /* loop loc_1AC; ceil(biWidth, 32) */
        ecx = *(WORD *)sp; /* pop cx */
        sp += sizeof(WORD);
    } while (--ecx != 0); /* loop loc_1A5; biHeight */

    LocalFree(stack);

    return var_4; /* mov ax, [bp+var_4]; Number of non-transparent blocks */
#undef STACK_SIZE
#undef cx
#undef bl
#undef ax
}

/* Decompress bitmap image. */
void PASCAL sub_230(void FAR * dest, void FAR * bmpData)
{
    BYTE FAR * source = bmpData;
    BYTE FAR * destination = dest;
    BYTE FAR * originalsource = NULL;
    BYTE FAR * originaldestination = NULL;
    WORD var_2 = (WORD)((BITMAPINFOHEADER FAR *)source)->biWidth;
    DWORD ebx = var_2 + 3 & (WORD)-4;
    LONG edx = ((BITMAPINFOHEADER FAR *)source)->biHeight;
    DWORD compression = ((BITMAPINFOHEADER FAR *)source)->biCompression;
    WORD bitcount = ((BITMAPINFOHEADER FAR *)source)->biBitCount;
    DWORD counter = ((BITMAPINFOHEADER FAR *)source)->biSize;
    BYTE bytebuffer[2] = {0, 0};
    for (; counter != 0; counter -= 1) {
        *destination++ = *source++;
    }
    ((BITMAPINFOHEADER FAR *)dest)->biCompression = BI_RGB;
    counter = BICOLORCOUNT(bmpData) * 4;
    for (; counter != 0; counter -= 1) {
        *destination++ = *source++;
    }
    if (compression == BI_RGB) {
        if (bitcount == 4) {
            ((BITMAPINFOHEADER FAR *)dest)->biBitCount = 8;
            if (((BITMAPINFOHEADER FAR *)dest)->biClrUsed == 0) {
                ((BITMAPINFOHEADER FAR *)dest)->biClrUsed = 16;
            }
            compression = edx;
            var_2 = (var_2 + 1) / 2 + 3 & -4;
            do {
                originalsource = source;
                originaldestination = destination;
                counter = ebx;
                do {
                    if ((counter & 1) == 0) {
                        bytebuffer[0] = *source++;
                        bytebuffer[1] = bytebuffer[0];
                        bytebuffer[0] >>= 4;
                    } else {
                        bytebuffer[0] = bytebuffer[1];
                        bytebuffer[0] &= 0x0F;
                    }
                    *destination++ = bytebuffer[0];
                } while (--counter != 0);
                destination = originaldestination + ebx;
                source = originalsource + var_2;
            } while (--compression != 0);
        } else {
            counter = ebx * edx;
            for (; counter != 0; counter -= 1) {
                *destination++ = *source++;
            }
        }
    } else if (compression == BI_RLE4) {
        ((BITMAPINFOHEADER FAR *)dest)->biBitCount = 8;
        if (((BITMAPINFOHEADER FAR *)dest)->biClrUsed == 0) {
            ((BITMAPINFOHEADER FAR *)dest)->biClrUsed = 16;
        }
        for (;;) {
            originaldestination = destination;
            for (;;) {
                bytebuffer[0] = *source++;
                bytebuffer[1] = *source++;
                if (bytebuffer[0] == 0) {
                    if (bytebuffer[1] == 0) {
                        destination = originaldestination + ebx;
                        break;
                    } else if (bytebuffer[1] == 1) {
                        return;
                    } else if (bytebuffer[1] == 2) {
                        bytebuffer[0] = *source++;
                        bytebuffer[1] = *source++;
                        if (bytebuffer[1] != 0) {
                            do {
                                destination += ebx;
                                originaldestination += ebx;
                            } while (bytebuffer[1] != 0);
                        }
                        destination += bytebuffer[0];
                    } else {
                        counter = bytebuffer[1];
                        do {
                            bytebuffer[0] = *source++;
                            bytebuffer[1] = *source++;
                            *destination++ = bytebuffer[0] >> 4;
                            if (--counter == 0) {
                                break;
                            }
                            *destination++ = bytebuffer[0] & 0x0F;
                            if (--counter == 0) {
                                break;
                            }
                            *destination++ = bytebuffer[1] >> 4;
                            if (--counter == 0) {
                                break;
                            }
                            *destination++ = bytebuffer[1] & 0x0F;
                        } while (--counter != 0);
                    }
                } else {
                    counter = bytebuffer[0] / 2;
                    for (; counter != 0; counter -= 1) {
                        *destination++ = bytebuffer[1] >> 4;
                        *destination++ = bytebuffer[1] & 0x0F;
                    }
                    if ((bytebuffer[0] & 1) != 0) {
                        *destination++ = bytebuffer[1] >> 4;
                    }
                }
            }
        }
    } else if (compression == BI_RLE8) {
        for (;;) {
            originaldestination = destination;
            for (;;) {
                bytebuffer[0] = *source++;
                bytebuffer[1] = *source++;
                if (bytebuffer[0] == 0) {
                    if (bytebuffer[1] == 0) {
                        destination = originaldestination + ebx;
                        break;
                    } else if (bytebuffer[1] == 1) {
                        return;
                    } else if (bytebuffer[1] == 2) {
                        bytebuffer[0] = *source++;
                        bytebuffer[1] = *source++;
                        if (bytebuffer[1] != 0) {
                            do {
                                destination += ebx;
                                originaldestination += ebx;
                            } while (bytebuffer[1] != 0);
                        }
                        destination += bytebuffer[0];
                    } else {
                        counter = bytebuffer[1];
                        do {
                            *destination++ = *source++;
                        } while (--counter != 0);
                        source += bytebuffer[1] & 1;
                    }
                } else {
                    do {
                        *destination++ = bytebuffer[1];
                    } while (--bytebuffer[0] != 0);
                }
            }
        }
    }
}

/* Get palette size. */
WORD readBmpPaletteSize(void FAR * data)
{
    DWORD FAR * uData;
    WORD colorCount;
    uData = data;
    colorCount = readBmpColorCount(uData);
    if (*uData == 12) {
        return colorCount * sizeof(RGBTRIPLE);
    } else {
        return colorCount * sizeof(RGBQUAD);
    }
}

/* Get number of colours in palette. */
WORD readBmpColorCount(void FAR * data)
{
    WORD bitCount;
    BITMAPINFOHEADER FAR * pBmpInfo;
    BITMAPCOREHEADER FAR * pBmpCore;
    pBmpInfo = data;
    pBmpCore = data;
    if (pBmpInfo->biSize != 12) {
        if (pBmpInfo->biClrUsed != 0) {
            return (WORD)pBmpInfo->biClrUsed;
        }
        bitCount = pBmpInfo->biBitCount;
    } else {
        bitCount = pBmpCore->bcBitCount;
    }
    switch (bitCount) {
    case 1:
        return 2;
    case 4:
        return 16;
    case 8:
        return 256;
    default:
        return 0;
    }
}

/* Create palette based on given bitmap image. */
HPALETTE createPaletteFromBmp(HDC hdc, void FAR * bmpData)
{
    const BYTE FAR * ptr;
    HPALETTE hPalette;
    LOGPALETTE * pLogPalette;
    int colorCount;
    int i;
    if ((GetDeviceCaps(hdc, RASTERCAPS) & RC_PALETTE) == 0) {
        colorCount = 256;
    } else {
        colorCount = bmpData != NULL ?
                    ((BITMAPINFOHEADER FAR *)bmpData)->biClrUsed != 0 ?
                        (int)(((BITMAPINFOHEADER FAR *)bmpData)->biClrUsed) :
                        (1 << ((BITMAPINFOHEADER FAR *)bmpData)->biBitCount)
                  : GetDeviceCaps(hdc, SIZEPALETTE);
    }
    pLogPalette = (LOGPALETTE *)LocalAlloc(LPTR, (colorCount + 2) * sizeof(PALETTEENTRY));
    pLogPalette->palVersion = 0x0300;
    pLogPalette->palNumEntries = (WORD)colorCount;
    if (bmpData != NULL) {
        ptr = (BYTE FAR *)bmpData + ((BITMAPINFOHEADER FAR *)bmpData)->biSize;
        for (i = 0; i < colorCount; i += 1) {
            pLogPalette->palPalEntry[i].peRed = ptr[2];
            pLogPalette->palPalEntry[i].peGreen = ptr[1];
            pLogPalette->palPalEntry[i].peBlue = ptr[0];
            pLogPalette->palPalEntry[i].peFlags = 0;
            ptr += sizeof(PALETTEENTRY);
        }
    } else {
        GetSystemPaletteEntries(hdc, 0U, colorCount, pLogPalette->palPalEntry);
    }
    hPalette = CreatePalette(pLogPalette);
    LocalFree(pLogPalette);
    return hPalette;
}

/* Create mask palette based on given RGB values. */
HPALETTE createPaletteFromRgb(HDC hdc, BYTE r, BYTE g, BYTE b)
{
    const BYTE FAR * ptr;
    HPALETTE hPalette;
    LOGPALETTE * logPalette;
    logPalette = (LOGPALETTE *)LocalAlloc(LPTR, 12U);
    logPalette->palVersion = 0x0300;
    logPalette->palNumEntries = 1;
    logPalette->palPalEntry[0].peRed = r;
    logPalette->palPalEntry[0].peGreen = g;
    logPalette->palPalEntry[0].peBlue = b;
    logPalette->palPalEntry[0].peFlags = 0;
    ptr/* += 4 */;
    hPalette = CreatePalette(logPalette);
    LocalFree(logPalette);
    return hPalette;
}

/* Set palette entries based on another palette with nearest colours (unused). */
void palNearestCopy(HDC hdc, HPALETTE hDestPal, HPALETTE hSrcPal, int size)
{
    PALETTEENTRY * pSrcEntry;
    PALETTEENTRY * pDestEntry;
    int devPalSize;
    int i;
    COLORREF srcColor;
    COLORREF destColor;
    int iNearest;
    if ((GetDeviceCaps(hdc, RASTERCAPS) & RC_PALETTE) == 0) {
        return;
    }
    devPalSize = GetDeviceCaps(hdc, SIZEPALETTE);
    if (devPalSize == 0) {
        devPalSize = 256;
    }
    pSrcEntry = (PALETTEENTRY *)LocalAlloc(LPTR, size * sizeof(PALETTEENTRY));
    pDestEntry = (PALETTEENTRY *)LocalAlloc(LPTR, devPalSize * sizeof(PALETTEENTRY));
    GetPaletteEntries(hSrcPal, 0U, size, pSrcEntry);
    GetPaletteEntries(hDestPal, 0U, devPalSize, pDestEntry);
    for (i = 0; i < size; i += 1) {
        srcColor = *(COLORREF *)&pSrcEntry[i];
        iNearest = GetNearestPaletteIndex(hDestPal, srcColor);
        destColor = *(COLORREF *)&pDestEntry[iNearest];
        if (destColor != srcColor) {
            if (iNearest < 10 || iNearest > maxPalIndex_) {
                if (maxPalIndex_ < 10) {
                    break;
                } else {
                    iNearest = maxPalIndex_--;
                }
            }
            *(COLORREF *)&pDestEntry[iNearest] = srcColor;
        }
    }
    SetPaletteEntries(hDestPal, 0U, devPalSize, pDestEntry);
    LocalFree(pDestEntry);
    LocalFree(pSrcEntry);
}

/* Get colour index of first pixel. */
WORD getBmpColorOfFirstPixel(void FAR * bmpData)
{
    BYTE FAR * pPixel;
    BYTE color;
    pPixel = getBmpPixelBegin(bmpData);
    color = *pPixel++;
    switch (((BITMAPINFOHEADER FAR *)bmpData)->biCompression) {
    case BI_RGB:
        if (((BITMAPINFOHEADER FAR *)bmpData)->biBitCount == 4) {
            color >>= 4;
        }
        break;
    case BI_RLE8:
        if (color == 0) {
            pPixel += 1;
        }
        color = *pPixel;
        break;
    case BI_RLE4:
        if (color == 0) {
            pPixel += 1;
        }
        color = *pPixel >> 4;
        break;
    default:
        break;
    }
    return color;
}

/* Get pointer to pixel bits in bitmap image. */
void FAR * getBmpPixelBegin(void FAR * data)
{
    return (void FAR *)((BYTE FAR *)data + *(WORD FAR *)data + ((BITMAPINFOHEADER FAR *)data)->biClrUsed * sizeof(RGBQUAD));
}

/* Flip bitmap image. flags bit 0: Flip horizontally, bit 1: Flip vertically */
void bmpFlipCopy(void FAR * dest, void FAR * src, UINT flags)
{
    BYTE FAR * itrDest;
    BYTE FAR * itrSrc;
    BYTE FAR * pSrcPixel;
    int padding;
    int bytesPerLn;
    int i;
    int j;
    LONG offset;
    itrDest = dest;
    itrSrc = src;
    pSrcPixel = getBmpPixelBegin(src);
    while (itrSrc < pSrcPixel) {
        *itrDest++ = *itrSrc++;
    }
    padding = ((BITMAPINFOHEADER FAR *)src)->biBitCount == 4 ?
                (int)(-(((BITMAPINFOHEADER FAR *)src)->biWidth % 8) & 7) :
                (int)(-(((BITMAPINFOHEADER FAR *)src)->biWidth % 4) & 3);
    bytesPerLn = ((BITMAPINFOHEADER FAR *)src)->biBitCount == 4 ?
                (int)(((BITMAPINFOHEADER FAR *)src)->biWidth + padding) / 2 :
                (int)(((BITMAPINFOHEADER FAR *)src)->biWidth + padding);
    if ((flags & 2) != 0) {
        /// to to last line
        pSrcPixel += (((BITMAPINFOHEADER FAR *)src)->biHeight - 1) * bytesPerLn;
    }
    if ((flags & 1) != 0) {
        /// to last pixel of line
        offset = ((BITMAPINFOHEADER FAR *)src)->biBitCount == 4 ?
                    ((BITMAPINFOHEADER FAR *)src)->biWidth / 2 :
                    ((BITMAPINFOHEADER FAR *)src)->biWidth;
        pSrcPixel += offset - 1;
    }
    for (i = 0; i < ((BITMAPINFOHEADER FAR *)src)->biHeight; i += 1) {
        itrSrc = pSrcPixel;
        if (((BITMAPINFOHEADER FAR *)src)->biBitCount == 4) {
            if ((flags & 1) != 0) {
                for (j = 0; j < bytesPerLn; j += 1) {
                    /// swap 4bit data
                    *itrDest = *itrSrc >> 4 & 0x0F | *itrSrc << 4;
                    itrSrc -= 1;
                    itrDest += 1;
                }
            } else {
                for (j = 0; j < bytesPerLn; j += 1) {
                    *itrDest++ = *itrSrc++;
                }
            }
        } else {
            if ((flags & 1) != 0) {
                for (j = 0; j < bytesPerLn; j += 1) {
                    *itrDest++ = *itrSrc--;
                }
            } else {
                for (j = 0; j < bytesPerLn; j += 1) {
                    *itrDest++ = *itrSrc++;
                }
            }
        }
        offset = (flags & 2) != 0 ? -bytesPerLn : bytesPerLn;
        pSrcPixel += offset;
    }
}

/* WinMain function. */
int PASCAL WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    HWND hPooWnd;
    MSG msg;
    WNDCLASS wndClass;
    if (hPrevInstance == NULL) {
        wndClass.style = CS_DBLCLKS;
        wndClass.lpfnWndProc = pooWndProc;
        wndClass.cbClsExtra = 0;
        wndClass.cbWndExtra = 8;
        wndClass.hInstance = hInstance;
        wndClass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(100));
        wndClass.hCursor = LoadCursor(hInstance, MAKEINTRESOURCE(IDC_CURSOR1));
        wndClass.hbrBackground = NULL;
        wndClass.lpszMenuName = NULL;
        wndClass.lpszClassName = "ScreenMatePoo";
        if (RegisterClass(&wndClass) == 0) {
            return 0;
        }
    }
    if (hPrevInstance == NULL) {
        wndClass.style = CS_DBLCLKS;
        wndClass.lpfnWndProc = pooSubWndProc;
        wndClass.cbClsExtra = 0;
        wndClass.cbWndExtra = 0;
        wndClass.hInstance = hInstance;
        wndClass.hIcon = NULL;
        wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
        wndClass.hbrBackground = NULL;
        wndClass.lpszMenuName = NULL;
        wndClass.lpszClassName = "ScreenMatePooSub";
        if (RegisterClass(&wndClass) == 0) {
            return 0;
        }
    }
    hSelfInst_ = hInstance;
    if (FindWindow("ScreenMatePoo", "Screen Mate") != NULL) {
        hasOtherPoo_ = 1;      //isMultiInst
    }
#ifdef _WIN32
    /* In 32-bit Windows, popup window is now in the taskbar by default.
     * Additional code is needed to hide the popup window from taskbar while
     * keeping it in the Alt+Tab list.
     */
    wndClass.style = 0;
    wndClass.lpfnWndProc = DefWindowProc;
    wndClass.cbClsExtra = 0;
    wndClass.cbWndExtra = 0;
    wndClass.hInstance = hInstance;
    wndClass.hIcon = NULL;
    wndClass.hCursor = NULL;
    wndClass.hbrBackground = NULL;
    wndClass.lpszMenuName = NULL;
    wndClass.lpszClassName = "ScreenMatePooOwner";
    if (RegisterClass(&wndClass) == 0) {
        return 0;
    }
    /* Create a hidden owner top-level window to hide visible windows from taskbar. */
    hOwnerWnd_ = CreateWindowEx(0L, "ScreenMatePooOwner", NULL, 0L, 0, 0, 0, 0, NULL, NULL, hInstance, NULL);
    if (hOwnerWnd_ == NULL) {
        return 0;
    }
    /* Set the visible window to be owned by the hidden top-level window. */
    hPooWnd = CreateWindowEx(0L, "ScreenMatePoo", "Screen Mate", WS_POPUP, 0, 0, 0, 0, hOwnerWnd_, NULL, hInstance, NULL);
#else
    hPooWnd = CreateWindowEx(0L, "ScreenMatePoo", "Screen Mate", WS_POPUP, 0, 0, 0, 0, NULL, NULL, hInstance, NULL);
#endif
    if (hPooWnd == NULL) {
        return 0;
    }
    ShowWindow(hPooWnd, nShowCmd);
    UpdateWindow(hPooWnd);
    while (GetMessage(&msg, NULL, 0U, 0U)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
#ifdef _WIN32
    DestroyWindow(hOwnerWnd_);
#endif
    return (int)msg.wParam;
}

/* Set cursor position changed flag (unused). */
void unused_1DDC(void)
{
    isCursorMoved_ = 1;
}

/* Window procedure. */
LRESULT CALLBACK pooWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
    void FAR * pResData;
    HGLOBAL hResData;
    HRSRC hRes;
    void FAR * pColorData;
    int posx_10;
    int posy_12;
    RECT rect;
    char dragPath[260];
    HMENU hPopMenu;
    WORD oldTopMostConf;
    POINT cursorPos;
    UINT oldGForceConf;
    FARPROC proc;
    LPWINDOWPOS pWndPos;
#ifdef _WIN32
    HANDLE user32;
#endif
    switch (uMsg) {
    case WM_CREATE:
        if (!checkOtherPoo(hWnd)) {
            MessageBox(hWnd, "Cannot run more than 8 Screen Mate", "Screen Mate", MB_ICONHAND | MB_OK);
            return -1;
        }
#ifdef _WIN32
        /* Additional code that allows drag-and-drop across different privileges
           in Windows Vista and higher. */
        user32 = GetModuleHandle("user32.dll");
        if (user32 != NULL) {
            BOOL (WINAPI * changewindowmessagefilterex)(HWND, UINT, DWORD, LPVOID) = (BOOL (WINAPI *)(HWND, UINT, DWORD, LPVOID))GetProcAddress(user32, "ChangeWindowMessageFilterEx");
            if (changewindowmessagefilterex != NULL) {
                changewindowmessagefilterex(hWnd, 0x0049, 1, NULL); /* WM_COPYGLOBALDATA */
                changewindowmessagefilterex(hWnd, 0x004A, 1, NULL); /* WM_COPYDATA */
                changewindowmessagefilterex(hWnd, WM_DROPFILES, 1, NULL);
            }
        }
#endif
        DragAcceptFiles(hWnd, TRUE);
        hPooWnd_ = hWnd;
        readConfig();
        if (confTopMost__ != 0) {
            SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
        }
        hdc = GetDC(hWnd);
        hRes = FindResource(hSelfInst_, MAKEINTRESOURCE(101), RT_BITMAP);
        hResData = LoadResource(hSelfInst_, hRes);
        pResData = LockResource(hResData);
        pColorData = (void FAR *)((BYTE FAR *)pResData + *(WORD FAR *)pResData + readBmpPaletteSize(pResData));
        hPalette_ = createPaletteFromBmp(hdc, pResData);
        FreeResource(hResData);
        SelectPalette(hdc, hPalette_, FALSE);
        RealizePalette(hdc);
        if (!initResources(hdc)) {
            MessageBox(hWnd, "Insufficient memory", "Screen Mate", MB_ICONHAND | MB_OK);
            ReleaseDC(hWnd, hdc);
            return -1;
        }
        ReleaseDC(hWnd, hdc);
        if (!initBitmaps(hWnd)) {
            MessageBox(hWnd, "Insufficient memory", "Screen Mate", MB_ICONHAND | MB_OK);
            return -1;
        }
        SetTimer(hWnd, 1U, 108U, NULL);
        break;
    case WM_DROPFILES:
        if (hOtherPooWnd_[kISubPooHwnd] == NULL) {
            if (DragQueryFile((HDROP)wParam, 0U, dragPath, 260U) != 0U) {
                asyncPlaySound(dragPath);
                postAction(kActionFeed);
            }
        }
        DragFinish((HDROP)wParam);
        break;
    case kWmUserUnused:       ///unused msg
        switch (lParam) {
        case 0x202:
        case 0x205:
            if (bool_C0AE) {
                if (hOtherPooWnd_[kISubPooHwnd] != NULL) {
                    putWndToTop(hOtherPooWnd_[kISubPooHwnd]);
                }
                putWndToTop(hWnd);
            } else {
                putWndToTop(hWnd);
                if (hOtherPooWnd_[kISubPooHwnd] != NULL) {
                    putWndToTop(hOtherPooWnd_[kISubPooHwnd]);
                }
            }
            putWndToTop(hWnd);
            hPopMenu = CreatePopupMenu();
            AppendMenu(hPopMenu, 0U, IDM_CONFIG, "Screen Mate Settings...");
            AppendMenu(hPopMenu, 0U, IDCANCEL, "Screen Mate Exit");
            GetCursorPos(&cursorPos);
            TrackPopupMenu(hPopMenu, 0U, cursorPos.x, cursorPos.y, 0, hWnd, NULL);
            DestroyMenu(hPopMenu);
            return 0;
        default:
            break;
        }
        return 0;
    case WM_WINDOWPOSCHANGING:
        pWndPos = (LPWINDOWPOS)lParam;
        if (hasTimerMsgEver_) {
            pWndPos = (LPWINDOWPOS)lParam;
        }
        pWndPos->flags |= SWP_NOCOPYBITS;
        skipNextPaintMsg_ = 1;
        return 0;
    case WM_WINDOWPOSCHANGED:
        return 0;
    case WM_TIMER:
        if (word_C0BA != 0) {
            word_C0BA -= 1;
            return 0;
        }
        if (confNoSleep_ == 0) {
            GetCursorPos(&cursorPos);
            if (lastCursorPos_.x != cursorPos.x || lastCursorPos_.y != cursorPos.y) {
                lastCursorPos_.x = cursorPos.x;
                lastCursorPos_.y = cursorPos.y;
                isCursorMoved_ = 1;
            }
            if (needSleepAfterChime_) {
                if (isCursorMoved_) {
                    needSleepAfterChime_ = 0;
                    cursorIdleCount_ = 0;
                    postAction(0);
                }
            } else {
                if (isCursorMoved_) {
                    isCursorMoved_ = 0;
                    cursorIdleCount_ = 0;
                } else {
                    if (cursorIdleCount_++ > 300) {
                        postAction(kActionInfSleep);
                    }
                }
            }
        }
        skipNextPaintMsg_ = 0;
        hPooWnd_ = hWnd;
        stateUpdate();
        paint_3284(hWnd);
        paint_3717(hWnd);
        hasTimerMsgEver_ = 1;
        return 0;
    case kWmPeerPoo:
        if (wParam == kPeerPooBorn) {
            pushOtherPoo((HWND)lParam);
        }
        if (wParam == kPeerPooDead) {
            eraseOtherPoo((HWND)lParam);
        }
        return 0;
    case WM_PAINT:
        if (skipNextPaintMsg_) {
            skipNextPaintMsg_ = 0;
            ValidateRect(hWnd, NULL);
            return 0;
        }
        repaintPoo(hWnd);
        ValidateRect(hWnd, NULL);
        return 0;
        GetWindowRect(hWnd, &rect);
        if (lastWndRect_.top == rect.top && lastWndRect_.bottom == rect.bottom && lastWndRect_.left == rect.left && lastWndRect_.right == rect.right) {
            repaintPoo(hWnd);
            ValidateRect(hWnd, NULL);
            return 0;
        }
        GetWindowRect(hWnd, &lastWndRect_);
        ValidateRect(hWnd, NULL);
        return 0;
        break;
    case WM_QUIT:
        return 0;
    case WM_COMMAND:
        switch (wParam) {
        case IDM_CONFIG:
            SendMessage(hWnd, kWmConfigCmd, 0, 0);
            break;
        case IDCANCEL:
            DestroyWindow(hWnd);
            return 0;
            break;
        default:
            break;
        }
        break;
    case WM_LBUTTONDOWN:
    case WM_RBUTTONDOWN:
        if (hOtherPooWnd_[kISubPooHwnd] != NULL) {
            break;
        }
        if (isMouseBtnDown_) {
            break;
        }
        SetCapture(hWnd);
        GetWindowRect(hWnd, &rect);
        posx_A79A = (short)LOWORD(lParam) + rect.left;
        posy_A79C = (short)HIWORD(lParam) + rect.top;
        isMouseBtnDown_ = 1;
        postAction(1);
        paint_3284(hWnd);
        paint_3717(hWnd);
        break;
    case WM_MOUSEMOVE:
        if (!isMouseBtnDown_) {
            break;
        }
        GetWindowRect(hWnd, &rect);
        posx_10 = (short)LOWORD(lParam) + rect.left;
        posy_12 = (short)HIWORD(lParam) + rect.top;
        if (posx_A79A == posx_10 && posy_A79C == posy_12) {
            break;
        }
        movePooWnd(posx_10 - posx_A79A, posy_12 - posy_A79C);
        posx_A79A = posx_10;
        posy_A79C = posy_12;
        paint_3284(hWnd);
        paint_3717(hWnd);
        break;
    case WM_RBUTTONUP:
        if (isRBtnDbClicked_ != 0) {
            DestroyWindow(hWnd);
            break;
        }
        /// flow to bottom
    case WM_LBUTTONUP:
        if (isMouseBtnDown_) {
            GetWindowRect(hWnd, &rect);
            posx_10 = (short)LOWORD(lParam) + rect.left;
            posy_12 = (short)HIWORD(lParam) + rect.top;
            movePooWnd(posx_10 - posx_A79A, posy_12 - posy_A79C);
            postAction(0);
            if (uMsg == WM_RBUTTONUP) {
                postAction(2);
            }
            paint_3284(hWnd);
            paint_3717(hWnd);
            ReleaseCapture();
            isMouseBtnDown_ = 0;
        }
        break;
    case WM_RBUTTONDBLCLK:
        isRBtnDbClicked_ = 1;
        break;
    case WM_LBUTTONDBLCLK:
    case kWmConfigCmd:
        oldTopMostConf = confTopMost__;
        oldGForceConf = confGForce_;
        hPooWnd_ = hWnd;
        if ((HIBYTE(GetKeyState(VK_SHIFT)) & 0x80) != 0 &&
            (HIBYTE(GetKeyState(VK_CONTROL)) & 0x80) != 0) {
            proc = MakeProcInstance((FARPROC)debugDlgProc, hSelfInst_);
            DialogBox(hSelfInst_, MAKEINTRESOURCE(IDD_DEBUG), hWnd, (DLGPROC)proc);
        } else {
            proc = MakeProcInstance((FARPROC)configDlgProc, hSelfInst_);
            DialogBox(hSelfInst_, MAKEINTRESOURCE(IDD_CONFIG), hWnd, (DLGPROC)proc);
        }
        FreeProcInstance(proc);
        repaintPoo(hWnd);
        if (oldTopMostConf != confTopMost__) {
            if (confTopMost__ != 0) {
                SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
            } else {
                SetWindowPos(hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
            }
        }
        if (oldGForceConf != confGForce_ && confGForce_) {
            postAction(2);
        }
        break;
    case WM_DESTROY:
        notifyPooDead(hWnd);
        if (hOtherPooWnd_[kISubPooHwnd] != NULL) {
            destroySubPooWnd();
        }
        KillTimer(hWnd, 1U);
        stopPlaySound();
        releaseWndBmp();
        releaseResources();
        DeleteObject(hPalette_);
        DragAcceptFiles(hWnd, FALSE);
        PostQuitMessage(0);
        break;
    default:
        break;
    }
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

/* Window procedure (sub). */
LRESULT CALLBACK pooSubWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    LPWINDOWPOS wndPos;
    switch (uMsg) {
    case WM_CREATE:
        if (!initSubWnd(hWnd)) {
            DestroyWindow(hWnd);
            return 1;
        }
        SetTimer(hWnd, 1U, 108U, NULL);
        break;
    case WM_WINDOWPOSCHANGING:
        wndPos = (LPWINDOWPOS)lParam;
        wndPos->flags |= SWP_NOCOPYBITS;
        skipNextSubPaintMsg_ = 1;
        return 0;
    case WM_WINDOWPOSCHANGED:
        return 0;
    case WM_TIMER:
        skipNextSubPaintMsg_ = 0;
        paint_9438(hWnd);
        if (!paint_9A49(hWnd)) {
            hasOtherPoo_ = 1;
            resetState();
        }
        return 0;
    case WM_PAINT:
        if (skipNextSubPaintMsg_) {
            skipNextSubPaintMsg_ = 0;
            ValidateRect(hWnd, NULL);
            return 0;
        }
        repaintSubPoo(hWnd);
        ValidateRect(hWnd, NULL);
        return 0;
    case WM_ERASEBKGND:
        return 0;
    case WM_DESTROY:
        releaseSubWnd();
        KillTimer(hWnd, 1U);
        break;
    default:
        break;
    }
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

/* Configuration window callback. */
BOOL CALLBACK configDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg) {
    case WM_INITDIALOG:
        SendDlgItemMessage(hDlg, IDC_CONFIG_CHIME, BM_SETCHECK, (WPARAM)confChime_, 0);
        SendDlgItemMessage(hDlg, IDC_CONFIG_SOUND, BM_SETCHECK, (WPARAM)confSound_, 0);
        SendDlgItemMessage(hDlg, IDC_CONFIG_NOSLEEP, BM_SETCHECK, (WPARAM)confNoSleep_, 0);
        SendDlgItemMessage(hDlg, IDC_CONFIG_GFORCE, BM_SETCHECK, (WPARAM)confGForce_, 0);
        return TRUE;
    case WM_COMMAND:
        if (wParam == IDRETRY) {
            WinHelp(hDlg, "Scmpoo16.hlp", HELP_INDEX, 0L);
            return TRUE;
        }
        if (wParam == IDOK) {
            confChime_ = IsDlgButtonChecked(hDlg, IDC_CONFIG_CHIME);
            confSound_ = IsDlgButtonChecked(hDlg, IDC_CONFIG_SOUND);
            confNoSleep_ = IsDlgButtonChecked(hDlg, IDC_CONFIG_NOSLEEP);
            confGForce_ = IsDlgButtonChecked(hDlg, IDC_CONFIG_GFORCE);
            saveConfig();
        }
        if (wParam == IDABORT) {
            DestroyWindow(hPooWnd_);
        }
        if (wParam == IDOK || wParam == IDCANCEL || wParam == IDABORT) {
            EndDialog(hDlg, (int)wParam);
        }
        break;
    default:
        break;
    }
    return FALSE;
}

/* Debug window callback. */
BOOL CALLBACK debugDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg) {
    case WM_INITDIALOG:
        CheckRadioButton(hDlg, IDC_DEBUG_NORMAL, IDC_DEBUG_BURN, IDC_DEBUG_NORMAL);
        return TRUE;
    case WM_COMMAND:
        if (wParam == IDOK || wParam == IDCANCEL) {
            EndDialog(hDlg, (int)wParam);
        }
        if (wParam >= IDC_DEBUG_NORMAL && wParam <= IDC_DEBUG_ROTATE &&
            IsDlgButtonChecked(hDlg, (int)wParam)) {
            /// set Action(0~29)
            debugAction(wParam - IDC_DEBUG_NORMAL);
        }
        switch (wParam) {
        case IDC_DEBUG_UP:
            movePooWnd(0, -20);
            break;
        case IDC_DEBUG_DOWN:
            movePooWnd(0, 20);
            break;
        case IDC_DEBUG_LEFT:
            movePooWnd(-20, 0);
            break;
        case IDC_DEBUG_RIGHT:
            movePooWnd(20, 0);
            break;
        default:
            break;
        }
        break;
    default:
        break;
    }
    return FALSE;
}

/* Create subwindow. */
void showSubPoo(void)
{
    if (hOtherPooWnd_[kISubPooHwnd] != NULL) {
        return;
    }
#ifdef _WIN32
    /* Set the visible window to be owned by the hidden top-level window. */
    hOtherPooWnd_[kISubPooHwnd] =
            CreateWindowEx(0L, "ScreenMatePooSub", "ScreenMate Sub", WS_POPUP,
                           0, 0, 0, 0, hOwnerWnd_, NULL, hSelfInst_, NULL);
#else
    hOtherPooWnd_[kISubPooHwnd] =
            CreateWindowEx(0L, "ScreenMatePooSub", "ScreenMate Sub", WS_POPUP,
                           0, 0, 0, 0, NULL, NULL, hSelfInst_, NULL);
#endif
    if (hOtherPooWnd_[kISubPooHwnd] != NULL) {
        ShowWindow(hOtherPooWnd_[kISubPooHwnd], SW_SHOWNA);
        UpdateWindow(hOtherPooWnd_[kISubPooHwnd]);
    } else {
        hasOtherPoo_ = 1;
        resetState();
    }
}

/* Destroy subwindow. */
void destroySubPooWnd(void)
{
    if (hOtherPooWnd_[kISubPooHwnd] != NULL) {
        DestroyWindow(hOtherPooWnd_[kISubPooHwnd]);
        hOtherPooWnd_[kISubPooHwnd] = NULL;
    }
}

/* Place window to topmost position. */
void putWndToTop(HWND hWnd)
{
    if (confTopMost__ == 0) {
        SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE);
        SetWindowPos(hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE);
    }
}

/* Place a window on top of another. */
void setWndOnto(HWND hWnd, HWND hPeerWnd)
{
    if (confTopMost__ == 0) {
        SetWindowPos(hWnd, hPeerWnd, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE);
    }
}

/* Load resource image, generate (flipped) colour and mask images,
 * store handles into sprite info structure.
 * flag: <0 auto mask, 0 no mask, >0 mask from res */
BOOL loadSprite(HDC hdc, SpriteInfo * pSprite, int resId, int flag)
{
    void FAR * pBmpPixels;
    void FAR * pPalettePosOfFirstPixel_UNUSED;
    void FAR * pResData;
    void FAR * pBmpData;
    void FAR * pOriginBmpData;
    HGLOBAL hResData;
    HGLOBAL hBmpData;
    HGLOBAL hOriginBmpData;
    hResData = NULL;
    hBmpData = NULL;
    hOriginBmpData = NULL;
    hResData = LoadResource(hSelfInst_, FindResource(hSelfInst_, MAKEINTRESOURCE(resId), RT_BITMAP));
    pResData = LockResource(hResData);
    if (hResData == NULL) {
        return FALSE;
    }
    pSprite->x = 0;
    pSprite->y = 0;
    pSprite->width = *(int FAR *)((BYTE FAR *)pResData + 4);
    pSprite->height = *(int FAR *)((BYTE FAR *)pResData + 8);
    if (flag < 0) {
        hBmpData = GlobalAlloc(GMEM_MOVEABLE, 65535);
        if (hBmpData == NULL) {
            goto failed;
        }
        pBmpData = GlobalLock(hBmpData);
        if (flag < -1) {
            hOriginBmpData = GlobalAlloc(GMEM_MOVEABLE, 65535);
            if (hOriginBmpData == NULL) {
                goto failed;
            }
            pOriginBmpData = GlobalLock(hOriginBmpData);
            sub_230(pOriginBmpData, pResData);
            if (flag == -2) {
                bmpFlipCopy(pBmpData, pOriginBmpData, 2U);
            } else {
                bmpFlipCopy(pBmpData, pOriginBmpData, 1U);
            }
            GlobalFree(hOriginBmpData);
            hOriginBmpData = NULL;
        } else {
            sub_230(pBmpData, pResData);
        }
        pBmpPixels = (void FAR *)((BYTE FAR *)pBmpData + *(WORD FAR *)pBmpData + readBmpPaletteSize(pBmpData));
        pPalettePosOfFirstPixel_UNUSED = (void FAR *)((BYTE FAR *)pBmpData + *(WORD FAR *)pBmpData + getBmpColorOfFirstPixel(pBmpData) * sizeof(RGBQUAD));
        *(DWORD FAR *)pPalettePosOfFirstPixel_UNUSED = 0;
        pSprite->bitmaps[0] = CreateDIBitmap(hdc, pBmpData, CBM_INIT, pBmpPixels, pBmpData, DIB_RGB_COLORS);
        if (pSprite->bitmaps[0] == NULL) {
            goto failed;
        }
        sub_10(pBmpData, pBmpData);
        pBmpPixels = (void FAR *)((BYTE FAR *)pBmpData + *(WORD FAR *)pBmpData + readBmpPaletteSize(pBmpData));
        pSprite->bitmaps[1] = CreateDIBitmap(hdc, pBmpData, CBM_INIT, pBmpPixels, pBmpData, DIB_RGB_COLORS);
        if (pSprite->bitmaps[1] == NULL) {
            goto failed;
        }
        FreeResource(hResData);
        hResData = NULL;
        GlobalFree(hBmpData);
        hBmpData = NULL;
        return TRUE;
    } else {
        pBmpPixels = (void FAR *)((BYTE FAR *)pResData + *(WORD FAR *)pResData + readBmpPaletteSize(pResData));
        pSprite->bitmaps[0] = CreateDIBitmap(hdc, pResData, CBM_INIT, pBmpPixels, pResData, DIB_RGB_COLORS);
        if (pSprite->bitmaps[0] == NULL) {
            goto failed;
        }
        FreeResource(hResData);
        hResData = NULL;
        if (flag == 0) {
            pSprite->bitmaps[1] = NULL;
            return TRUE;
        }
        hResData = LoadResource(hSelfInst_, FindResource(hSelfInst_, MAKEINTRESOURCE(flag), RT_BITMAP));
        pResData = LockResource(hResData);
        if (hResData == NULL) {
            goto failed;
        }
        pSprite->x = 0;
        pSprite->y = 0;
        pSprite->width = *(int FAR *)((BYTE FAR *)pResData + 4);
        pSprite->height = *(int FAR *)((BYTE FAR *)pResData + 8);
        pBmpPixels = (void FAR *)((BYTE FAR *)pResData + *(WORD FAR *)pResData + readBmpPaletteSize(pResData));
        pSprite->bitmaps[1] = CreateDIBitmap(hdc, pResData, CBM_INIT, pBmpPixels, pResData, DIB_RGB_COLORS);
        if (pSprite->bitmaps[1] == NULL) {
            goto failed;
        }
        FreeResource(hResData);
        return TRUE;
    }
failed:
    if (hResData != NULL) {
        FreeResource(hResData);
    }
    if (hBmpData != NULL) {
        GlobalFree(hBmpData);
    }
    if (hOriginBmpData != NULL) {
        GlobalFree(hOriginBmpData);
    }
    return FALSE;
}

/* Release sprite images. */
void releaseSprite(SpriteInfo * sprite)
{
    if (sprite->bitmaps[0] != NULL) {
        DeleteObject(sprite->bitmaps[0]);
    }
    if (sprite->bitmaps[1] != NULL) {
        DeleteObject(sprite->bitmaps[1]);
    }
    sprite->bitmaps[0] = NULL;
    sprite->bitmaps[1] = NULL;
}

/* Read configuration from file. */
void readConfig(void)
{
    word_CA78 = 1;
    confTopMost__ = 0;
    confChime_ = 0U;
    confSound_ = 0U;
    confSound_ = GetPrivateProfileInt("Stray", "Sound", 0, "scmate.ini");
    confChime_ = GetPrivateProfileInt("Stray", "Alarm", 0, "scmate.ini");
    confNoSleep_ = GetPrivateProfileInt("Stray", "NoSleep", 0, "scmate.ini");
    confGForce_ = GetPrivateProfileInt("Stray", "GForce", 1, "scmate.ini");
}

/* Save individual configuration to file. */
void writeProfile(LPCSTR cls, LPCSTR key, UINT value, LPCSTR path)
{
    char str[40];
    wsprintf(str, "%u", value);
    WritePrivateProfileString(cls, key, str, path);
}

/* Save configurations to file. */
void saveConfig(void)
{
    writeProfile("Stray", "Sound", confSound_, "scmate.ini");
    writeProfile("Stray", "Alarm", confChime_, "scmate.ini");
    writeProfile("Stray", "NoSleep", confNoSleep_, "scmate.ini");
    writeProfile("Stray", "GForce", confGForce_, "scmate.ini");
}

/* Initialize bitmaps. */
BOOL initBitmaps(HWND hWnd)
{
    HDC hdc;
    hdc = GetDC(hWnd);
    paintBmpBuf_[0] = CreateCompatibleBitmap(hdc, 100, 100);
    if (paintBmpBuf_[0] == NULL) {
        goto failed;
    }
    paintBmpBuf_[1] = CreateCompatibleBitmap(hdc, 100, 100);
    if (paintBmpBuf_[1] == NULL) {
        goto failed;
    }
    bmp_A7BA = CreateCompatibleBitmap(hdc, 100, 100);
    if (bmp_A7BA == NULL) {
        goto failed;
    }
    word_CA4C = 0;
    word_CA4E = 0;
    screenWidth_ = GetSystemMetrics(SM_CXSCREEN);
    screenHeight_ = GetSystemMetrics(SM_CYSCREEN);
    ReleaseDC(hWnd, hdc);
    return TRUE;
failed:
    ReleaseDC(hWnd, hdc);
    return FALSE;
}

/* Release bitmaps. */
void releaseWndBmp()
{
    DeleteObject(paintBmpBuf_[0]);
    DeleteObject(paintBmpBuf_[1]);
    DeleteObject(bmp_A7BA);
    if (brush_C0B4 != NULL) {
        DeleteObject(brush_C0B4);
        brush_C0B4 = NULL;
    }
    if (brush_CA44 != NULL) {
        DeleteObject(brush_CA44);
        brush_CA44 = NULL;
    }
    if (bmp_C0B2 != NULL) {
        DeleteObject(bmp_C0B2);
        bmp_C0B2 = NULL;
    }
    if (bmp_C0B8 != NULL) {
        DeleteObject(bmp_C0B8);
        bmp_C0B8 = NULL;
    }
}

/* Update window position and sprite to be actually used. */
void setSpriteInfo(int x, int y, int sprite)
{
    curSpriteX_ = x;
    curSpriteY_ = y;
    curSpriteBmp0_ = sprites_[sprite].bitmaps[0];
    curSpriteBmp1_ = sprites_[sprite].bitmaps[1];
    curSpriteBmpX_ = sprites_[sprite].x;
    curSpriteBmpY_ = sprites_[sprite].y;
    curSpriteBmpW_ = sprites_[sprite].width;
    curSpriteBmpH_ = sprites_[sprite].height;
}

/* Clear window. */
void repaintPoo(HWND hWnd)
{
    lastPaintX_ = 0;
    lastPaintY_ = 0;
    lastPaintBmpW_ = 0;
    lastPaintBmpH_ = 0;
    MoveWindow(hWnd, 0, 0, 0, 0, TRUE);
    bool_A7D4 = 1;
    word_C0BA = 1;
    lastPaintBmp_ = NULL;
}

/* Render sprite with double buffering. */
void paint_3284(HWND hWnd)
{
    HDC hdcScr;
    HDC hdc_4;
    HDC hdc_6;
    int posy_C;
    int posx_E;
    int var_10;
    int var_12;
    int posy_14;
    int posx_16;
    int height_18;
    int width_1A;
    int var_1C;
    int var_1E;
    if (bool_A7D2) {
        return;
    }
    if (lastPaintX_ == curSpriteX_ && lastPaintY_ == curSpriteY_ &&
        lastPaintBmp_ == curSpriteBmp0_ && lastPaintBmpX_ == curSpriteBmpX_ &&
        ufoBeamHeight_ == 0) {
        return;
    }
    iPaintBmp_ ^= 1;
    hdcScr = GetDC(NULL);
    SelectPalette(hdcScr, hPalette_, FALSE);
    hdc_4 = CreateCompatibleDC(hdcScr);
    hdc_6 = CreateCompatibleDC(hdcScr);
    SelectPalette(hdc_6, hPalette_, FALSE);
    SelectPalette(hdc_4, hPalette_, FALSE);
    posx_16 = max(curSpriteX_, lastPaintX_);
    posy_14 = max(curSpriteY_, lastPaintY_);
    var_12 = min(curSpriteBmpW_ + curSpriteX_, lastPaintBmpW_ + lastPaintX_) - posx_16;
    var_10 = min(curSpriteY_ + curSpriteBmpH_, lastPaintY_ + lastPaintBmpH_) - posy_14;
    if (var_12 <= 0 || var_10 <= 0) {
        word_A7FA = 1;
        if (bool_A7D4) {
            bool_A7D4 = 0;
        }
        paintRectX_ = curSpriteX_;
        paintRectY_ = curSpriteY_;
        paintRectW_ = curSpriteBmpW_;
        paintRectH_ = curSpriteBmpH_;
        SelectObject(hdc_4, paintBmpBuf_[iPaintBmp_]);
        BitBlt(hdc_4, 0, 0, paintRectW_, paintRectH_,
               hdcScr, paintRectX_, paintRectY_, SRCCOPY);
    } else {
        word_A7FA = 0;
        paintRectX_ = min(curSpriteX_, lastPaintX_);
        paintRectY_ = min(curSpriteY_, lastPaintY_);
        paintRectW_ = max(curSpriteBmpW_ + curSpriteX_, lastPaintBmpW_ + lastPaintX_) - paintRectX_;
        paintRectH_ = max(curSpriteY_ + curSpriteBmpH_, lastPaintY_ + lastPaintBmpH_) - paintRectY_;
        SelectObject(hdc_4, paintBmpBuf_[iPaintBmp_]);
        BitBlt(hdc_4, 0, 0, paintRectW_, paintRectH_,
               hdcScr, paintRectX_, paintRectY_, SRCCOPY);

        var_1E = max(paintRectX_, lastPaintRectX_);
        var_1C = max(paintRectY_, lastPaintRectY_);
        width_1A = min(paintRectW_ + paintRectX_, lastPaintRectW_ + lastPaintRectX_) - var_1E;
        height_18 = min(paintRectY_ + paintRectH_, lastPaintRectY_ + lastPaintRectH_) - var_1C;
        posx_16 = max(0, var_1E - paintRectX_);
        posy_14 = max(0, var_1C - paintRectY_);
        posx_E = max(0, var_1E - lastPaintRectX_);
        posy_C = max(0, var_1C - lastPaintRectY_);
        if (width_1A > 0 && height_18 > 0) {
            SelectObject(hdc_6, paintBmpBuf_[LOBYTE(iPaintBmp_) - 0xFF & 1]);    ///???wtf
            BitBlt(hdc_4, posx_16, posy_14, width_1A, height_18,
                   hdc_6, posx_E, posy_C, SRCCOPY);
        }
    }

    if (curSpriteBmp0_ != NULL) {
        SelectObject(hdc_6, bmp_A7BA);
        BitBlt(hdc_6, 0, 0, paintRectW_, paintRectH_, hdc_4, 0, 0, SRCCOPY);
        posx_16 = max(0, curSpriteX_ - paintRectX_);
        posy_14 = max(0, curSpriteY_ - paintRectY_);
        if (curSpriteBmp1_ != NULL) {
            SelectObject(hdc_4, curSpriteBmp1_);
            BitBlt(hdc_6, posx_16, posy_14, curSpriteBmpW_, curSpriteBmpH_,
                   hdc_4, curSpriteBmpX_, curSpriteBmpY_, SRCAND);
            SelectObject(hdc_4, curSpriteBmp0_);
            BitBlt(hdc_6, posx_16, posy_14, curSpriteBmpW_, curSpriteBmpH_,
                   hdc_4, curSpriteBmpX_, curSpriteBmpY_, SRCPAINT);
        } else {
            SelectObject(hdc_4, curSpriteBmp0_);
            BitBlt(hdc_6, posx_16, posy_14, curSpriteBmpW_, curSpriteBmpH_,
                   hdc_4, curSpriteBmpX_, curSpriteBmpY_, SRCCOPY);
        }
        bool_A7D2 = 1;
        word_CA5E = 1;
        MoveWindow(hWnd, paintRectX_, paintRectY_, paintRectW_, paintRectH_ + ufoBeamHeight_, TRUE);
        word_CA5E = 0;
    }
    DeleteDC(hdc_4);
    DeleteDC(hdc_6);
    lastPaintRectX_ = paintRectX_;
    lastPaintRectY_ = paintRectY_;
    lastPaintRectW_ = paintRectW_;
    lastPaintRectH_ = paintRectH_;
    lastPaintX_ = curSpriteX_;
    lastPaintY_ = curSpriteY_;
    lastPaintBmpW_ = curSpriteBmpW_;
    lastPaintBmpH_ = curSpriteBmpH_;
    lastPaintBmp_ = curSpriteBmp0_;
    lastPaintBmpX_ = curSpriteBmpX_;
    lastPaintBmpY_ = curSpriteBmpY_;
    ReleaseDC(NULL, hdcScr);
}

/* Render UFO beam (if any) and present render targets onto window. */
void paint_3717(HWND hWnd)
{
    HDC hdc_2;
    HDC hdc_4;
    RECT var_C;
    HDC hdc_E;
#ifdef WIN32
    HDC screen;
#endif
    if (bool_A7D2 == 0) {
        return;
    }
    bool_A7D2 = 0;
    hdc_2 = GetDC(hWnd);
    SelectPalette(hdc_2, hPalette_, FALSE);
    RealizePalette(hdc_2);
    hdc_4 = CreateCompatibleDC(hdc_2);
    SelectPalette(hdc_4, hPalette_, FALSE);
    SelectObject(hdc_4, bmp_A7BA);
    BitBlt(hdc_2, 0, 0, paintRectW_, paintRectH_, hdc_4, 0, 0, SRCCOPY);
    if (ufoBeamHeight_ != 0) {
        if (bmp_C0B8 == NULL) {
            bmp_C0B8 = CreateCompatibleBitmap(hdc_2, 40, screenHeight_ * 4 / 5);
            if (bmp_C0B8 == NULL) {
                goto failed;
            }
        }
        if (bmp_C0B2 == NULL) {
            bmp_C0B2 = CreateCompatibleBitmap(hdc_2, 40, screenHeight_ * 4 / 5);
            if (bmp_C0B2 == NULL) {
                goto failed;
            }
        }
        if (brush_CA44 == NULL) {
            brush_CA44 = CreateSolidBrush(RGB(255, 255, 0));
        }
        if (brush_C0B4 == NULL) {
            brush_C0B4 = CreateSolidBrush(RGB(128, 128, 0));
        }
        hdc_E = CreateCompatibleDC(hdc_2);
        SelectObject(hdc_E, bmp_C0B2);
#ifdef _WIN32
        /* Screen contents with height of only 40 pixels can be captured from window device context on Windows 10. Capture directly from screen instead. */
        screen = GetDC(NULL);
        BitBlt(hdc_E, 0, 0, 40, ufoBeamHeight_, screen, paintRectX_, paintRectY_ + 40, SRCCOPY);
        ReleaseDC(NULL, screen);
#else
        BitBlt(var_E, 0, 0, 40, word_CA72, var_2, 0, 40, SRCCOPY);
#endif
        var_C.left = 0;
        var_C.top = 0;
        var_C.right = 40;
        var_C.bottom = ufoBeamHeight_;
        SelectObject(hdc_4, bmp_C0B8);
        FillRect(hdc_4, &var_C, brush_CA44);
        BitBlt(hdc_E, 0, 0, 40, ufoBeamHeight_, hdc_4, 0, 0, SRCAND);
        FillRect(hdc_4, &var_C, brush_C0B4);
        BitBlt(hdc_E, 0, 0, 40, ufoBeamHeight_, hdc_4, 0, 0, SRCPAINT);
        BitBlt(hdc_2, 0, 40, 40, ufoBeamHeight_, hdc_E, 0, 0, SRCCOPY);
        DeleteDC(hdc_E);
        DeleteDC(hdc_4);
    } else {
        if (brush_C0B4 != NULL) {
            DeleteObject(brush_C0B4);
            brush_C0B4 = NULL;
        }
        if (brush_CA44 != NULL) {
            DeleteObject(brush_CA44);
            brush_CA44 = NULL;
        }
        if (bmp_C0B2 != NULL) {
            DeleteObject(bmp_C0B2);
            bmp_C0B2 = NULL;
        }
        if (bmp_C0B8 != NULL) {
            DeleteObject(bmp_C0B8);
            bmp_C0B8 = NULL;
        }
        DeleteDC(hdc_4);
    }
    ReleaseDC(hWnd, hdc_2);
    return;
failed:
    ReleaseDC(hWnd, hdc_2);
}

/* Unused. */
void unused_399D(HWND hWnd, int x, int y, int w, int h)
{
    MoveWindow(hWnd, x, y, w, h, FALSE);
    MoveWindow(hWnd, 0, 0, 0, 0, TRUE);
}

/* Find if a window has a match in known instance list. */
BOOL isOtherPoo(HWND hWnd)
{
    int i;
    if (otherPooCount_ == 0) {
        return FALSE;
    }
    for (i = 0; i < kMaxPooCount; i += 1) {
        if (hOtherPooWnd_[i] == hWnd && hOtherPooWnd_[i] != NULL) {
            return TRUE;
        }
    }
    return FALSE;
}

/* Find X-coordinate of possible collision with other instances. Return zero when no collision detected. */
int calcPooHitX(int head, int tail, int top, int bot)
{
    int i;
    int peerLeft;
    int peerRight;
    int peerTop;
    int peerBot;
    for (i = 0; i < kMaxPooCount; i += 1) {
        if (hOtherPooWnd_[i] != NULL) {
            peerLeft = (short)GetWindowWord(hOtherPooWnd_[i], 0);
            peerRight = (short)GetWindowWord(hOtherPooWnd_[i], 2);
            peerTop = peerLeft + 40;
            peerBot = peerRight + 40;
            if (peerRight == 0) {
                continue;
            }
            if (((peerTop <= top && peerBot > top) || (peerTop < bot && peerBot > bot)) &&
                    peerRight > head && peerRight <= tail && tail > head) {
                return peerRight;
            }
            if (((peerTop <= top && peerBot > top) || (peerTop < bot && peerBot > bot)) &&
                    peerLeft >= tail && peerLeft < head && tail < head) {
                return peerLeft;
            }
        }
    }
    return 0;
}

/* Populate known instance list by searching for visible windows with name match. */
void checkOtherPoo_3B4C(HWND hWnd)
{
    HWND hDeskWnd;
    UINT gwCmd;
    int i;
    int wndCount;
    char deskWndTitle[64];
    for (i = 0; i < kMaxPooCount; i += 1) {
        hOtherPooWnd_[i] = NULL;
    }
    hDeskWnd = GetDesktopWindow();
    gwCmd = GW_CHILD;
    i = 0;
    wndCount = 0;
    while ((hDeskWnd = GetWindow(hDeskWnd, gwCmd)) != NULL && i < 64) {
        gwCmd = GW_HWNDNEXT;
        if (hDeskWnd == hWnd) {
            continue;
        }
        if ((GetWindowLong(hDeskWnd, GWL_STYLE) & WS_VISIBLE) != 0) {
            GetWindowText(hDeskWnd, deskWndTitle, 16);
            if (lstrcmp(deskWndTitle, "Screen Mate") == 0) {
                hOtherPooWnd_[wndCount] = hDeskWnd;
                wndCount += 1;
                if (wndCount >= kMaxPooCount) {
                    return;
                }
            }
            i += 1;
        }
    }
    otherPooCount_ = wndCount;
}

/* Populate known instance list by searching for visible windows with name match,
 * then notify other instances of self creation. */
BOOL checkOtherPoo(HWND hWnd)
{
    HWND hDeskWnd;
    UINT gwCmd;
    int i;
    int wndCount;
    char deskWndTitle[64];
    hDeskWnd = GetDesktopWindow();
    gwCmd = GW_CHILD;
    i = 0;
    wndCount = 0;
    while ((hDeskWnd = GetWindow(hDeskWnd, gwCmd)) != NULL && i < 64) {
        gwCmd = GW_HWNDNEXT;
        if (hDeskWnd == hWnd) {
            continue;
        }
        if ((GetWindowLong(hDeskWnd, GWL_STYLE) & WS_VISIBLE) != 0) {
            GetWindowText(hDeskWnd, deskWndTitle, 16);
            if (lstrcmp(deskWndTitle, "Screen Mate") == 0) {
                hOtherPooWnd_[wndCount] = hDeskWnd;
                wndCount += 1;
                if (wndCount >= kMaxPooCount) {
                    return FALSE;
                }
            }
            i += 1;
        }
    }
    otherPooCount_ = wndCount;
    otherPooCount2_ = wndCount;
    for (i = 0; i < otherPooCount_; i += 1) {
        SendMessage(hOtherPooWnd_[i], kWmPeerPoo, (WPARAM)kPeerPooBorn, (LPARAM)hWnd);
    }
    return TRUE;
}

/* Notify other instances of self destruction. */
void notifyPooDead(HWND hWnd)
{
    int i;
    for (i = 0; i < kMaxPooCount; i += 1) {
        if (hOtherPooWnd_[i] != NULL) {
            SendMessage(hOtherPooWnd_[i], kWmPeerPoo, (WPARAM)kPeerPooDead, (LPARAM)hWnd);
        }
    }
}

/* Add window into known instance list. */
void pushOtherPoo(HWND hWnd)
{
    int i;
    for (i = 0; i < kMaxPooCount; i += 1) {
        if (hOtherPooWnd_[i] == NULL) {
            otherPooCount_ += 1;
            hOtherPooWnd_[i] = hWnd;
            break;
        }
    }
}

/* Remove window from known instance list. */
void eraseOtherPoo(HWND hWnd)
{
    int i;
    for (i = 0; i < kMaxPooCount; i += 1) {
        if (hOtherPooWnd_[i] == hWnd) {
            otherPooCount_ -= 1;
            hOtherPooWnd_[i] = NULL;
            break;
        }
    }
}

/* Populate known visible window list. */
void updateVWnds(void)
{
    HWND hWnd;
    UINT gwCmd;
    int i;
    hWnd = GetDesktopWindow();
    gwCmd = GW_CHILD;
    i = 0;
    while ((hWnd = GetWindow(hWnd, gwCmd)) != NULL && i < 32) {
        gwCmd = GW_HWNDNEXT;
        if (hWnd == hPooWnd_) {
            continue;
        }
        if ((GetWindowLong(hWnd, GWL_STYLE) & WS_VISIBLE) != 0) {
            GetWindowRect(hWnd, &otherVWnds_[i].rect);
            otherVWnds_[i].hWnd = hWnd;
            i += 1;
        }
    }
    otherVWndCount_ = i;
}

/* Find X-coordinate of possible collision with which visible window. */
int getVWndHitXInfo(HWND * pHwnd, int top, int bot, int nextHead, int curHead)
{
    int i;
    int j;
    RECT rect;
    for (i = 0; i < otherVWndCount_; i += 1) {
        if (curHead > nextHead) {
            if (otherVWnds_[i].rect.right >= nextHead && otherVWnds_[i].rect.right < curHead &&
                    otherVWnds_[i].rect.top < top && otherVWnds_[i].rect.bottom > bot) {
                for (j = 0; j < i; j += 1) {
                    if (otherVWnds_[j].rect.top <= top && otherVWnds_[j].rect.bottom >= bot &&
                            otherVWnds_[j].rect.left <= nextHead && otherVWnds_[j].rect.right >= curHead) {
                        break;
                    }
                }
                if (j == i) {
                    if (IsWindow(otherVWnds_[i].hWnd)) {
                        GetWindowRect(otherVWnds_[i].hWnd, &rect);
                        if (otherVWnds_[i].rect.right == rect.right) {
                            *pHwnd = otherVWnds_[i].hWnd;
                            return otherVWnds_[i].rect.right;
                        }
                    }
                }
            }
        } else {
            if (otherVWnds_[i].rect.left <= nextHead && otherVWnds_[i].rect.left > curHead &&
                    otherVWnds_[i].rect.top < top && otherVWnds_[i].rect.bottom > bot) {
                for (j = 0; j < i; j += 1) {
                    if (otherVWnds_[j].rect.top <= top && otherVWnds_[j].rect.bottom >= bot &&
                            otherVWnds_[j].rect.left <= nextHead && otherVWnds_[j].rect.right >= curHead) {
                        break;
                    }
                }
                if (j == i) {
                    if (IsWindow(otherVWnds_[i].hWnd)) {
                        GetWindowRect(otherVWnds_[i].hWnd, &rect);
                        if (otherVWnds_[i].rect.left == rect.left) {
                            *pHwnd = otherVWnds_[i].hWnd;
                            return otherVWnds_[i].rect.left;
                        }
                    }
                }
            }
        }
    }
    return 0;
}

/* Find Y-coordinate of possible landing top edge of which visible window. */
int getLandInfo(HWND * pHwnd, int posy_2, int posy_4, int posx_6, int posx_8)
{
    int i;
    int j;
    for (i = 0; i < otherVWndCount_; i += 1) {
        if (otherVWnds_[i].rect.top <= posy_2 && otherVWnds_[i].rect.top > posy_4 &&
                otherVWnds_[i].rect.left < posx_8 && otherVWnds_[i].rect.right > posx_6 &&
                otherVWnds_[i].rect.top > 10) {
            for (j = 0; j < i; j += 1) {
                if (otherVWnds_[j].rect.left <= posx_6 && otherVWnds_[j].rect.right >= posx_8 &&
                    otherVWnds_[j].rect.top <= posy_4 && otherVWnds_[j].rect.bottom >= posy_2) {
                    break;
                }
            }
            if (j == i) {
                *pHwnd = otherVWnds_[i].hWnd;
                return otherVWnds_[i].rect.top;
            }
        }
    }
    if (posy_2 >= screenHeight_ && posy_4 <= screenHeight_) {
        *pHwnd = NULL;
        return screenHeight_;
    }
    return 0;
}

/* Get window top Y-coordinate if it is possible to land on the window. */
int getWndLandY(HWND hWnd, int nextY, int curY, int left, int right)
{
    RECT rect;
    if (IsWindow(hWnd)) {
        GetWindowRect(hWnd, &rect);
        if (rect.top <= nextY && rect.top > curY &&
                rect.left < right && rect.right > left) {
            return rect.top;
        }
    }
    if (nextY > screenHeight_) {
        return -1;
    }
    return 0;
}

/* Play sound by resource ID and additional flags. */
void playSoundRes(int resId, UINT flags, WORD arg_4)
{
    LPCSTR lpszSoundName;
    if (hPlaySoundData_ != NULL) {
        sndPlaySound(NULL, SND_SYNC);
        GlobalUnlock(hPlaySoundData_);
        FreeResource(hPlaySoundData_);
        hPlaySoundData_ = NULL;
    }
    hPlaySoundData_ = LoadResource(hSelfInst_, FindResource(hSelfInst_, MAKEINTRESOURCE(resId), "WAVE"));
    lpszSoundName = LockResource(hPlaySoundData_);
    sndPlaySound(lpszSoundName, flags | (SND_ASYNC | SND_MEMORY));
}

/* Stop playing sound. */
void stopPlaySound(void)
{
    sndPlaySound(NULL, SND_SYNC);
}

/* Play sound by name. */
void asyncPlaySound(LPCSTR path)
{
    sndPlaySound(path, SND_ASYNC);
}

/* Play sound by resource ID and additional flags (when option "Cry" enabled). */
void pooSound(int resId, UINT flags, WORD arg_4)
{
    if (confSound_) {
        playSoundRes(resId, flags, arg_4);
    }
}

/* Generate sprites from loaded resource images. */
BOOL initResources(HDC hdc)
{
    int i;
    int j;
    for (i = 0; i < 16; i += 1) {
        if (originalRes_[i].resId == 0) {
            break;
        }
        if (!loadSprite(hdc, &originalRes_[i].info, originalRes_[i].resId, -1)) {
            return FALSE;
        }
        if (!loadSprite(hdc, &hFlipRes_[i].info, originalRes_[i].resId, -3)) {
            return FALSE;
        }
        /// what is 16, 256 mean?
        for (j = 0; j < 16; j += 1) {
            sprites_[i * 16 + j].bitmaps[0] = originalRes_[i].info.bitmaps[0];
            sprites_[i * 16 + j].bitmaps[1] = originalRes_[i].info.bitmaps[1];
            sprites_[i * 16 + j].width = 40;
            sprites_[i * 16 + j].height = 40;
            sprites_[i * 16 + j].x = j * 40;
            sprites_[i * 16 + j].y = 0;
            sprites_[i * 16 + j + 256].bitmaps[0] = hFlipRes_[i].info.bitmaps[0];
            sprites_[i * 16 + j + 256].bitmaps[1] = hFlipRes_[i].info.bitmaps[1];
            sprites_[i * 16 + j + 256].width = 40;
            sprites_[i * 16 + j + 256].height = 40;
            sprites_[i * 16 + j + 256].x = (15 - j) * 40;
            sprites_[i * 16 + j + 256].y = 0;
        }
    }
    return TRUE;
}

/* Release resource images. */
void releaseResources(void)
{
    int i;
    for (i = 0; i < 16; i += 1) {
        if (originalRes_[i].resId == 0) {
            break;
        }
        releaseSprite(&originalRes_[i].info);
        if (originalRes_[i].flags == 1) {
            releaseSprite(&hFlipRes_[i].info);
        }
    }
}

/* Turn around when approaching screen border or otherwise with 1/20 probability.  */
void checkPooTurn(void)
{
    if (xDirection_ > 0 && xPoo_ < 0) {
        pooState_ = kStateTurnIntro;
    }
    if (xDirection_ < 0 && screenWidth_ - sprites_[iSprite_].width < xPoo_) {
        pooState_ = kStateTurnIntro;
    }
    if (xDirection_ > 0 && screenWidth_ - sprites_[iSprite_].width > xPoo_ && rand() % 20 == 0) {
        pooState_ = kStateTurnIntro;
    }
    if (xDirection_ < 0 && xPoo_ > 0 && rand() % 20 == 0) {
        pooState_ = kStateTurnIntro;
    }
}

/* Flag-controlled collision and turn around. */
void checkPooHit(BOOL turnCheck)
{
    if (bool_A7FC == 0) {
        if (xDirection_ > 0 && xPoo_ < 0) {
            pooState_ = kStateHitIntro;
        }
        if (xDirection_ < 0 && screenWidth_ - sprites_[iSprite_].width < xPoo_) {
            pooState_ = kStateHitIntro;
        }
    }
    if (turnCheck) {
        if (xDirection_ > 0 && screenWidth_ - 80 > xPoo_ && rand() % 20 == 0) {
            pooState_ = kStateTurnIntro;
        }
        if (xDirection_ < 0 && xPoo_ > 40 && rand() % 20 == 0) {
            pooState_ = kStateTurnIntro;
        }
    }
}

/* Switch to standing sprite after certain frames. */
void aniFrameDownToNormal(void)
{
    if (iAniFrames_-- <= 0) {
        pooState_ = kStateToNormal;
    }
}

/* Process chime. */
void processChime(void)
{
    struct tm * tmTime;
    time_t ttime;
    DWORD tick;
    int clockHour;
    if (chimeCount_ != 0) {
        tick = GetTickCount();
        if (lastChimeTick_ + 1000 < tick) {
            lastChimeTick_ = tick;
            chimeCount_ -= 1;
            if (chimeCount_ != 0) {
                playSoundRes(IDW_BAA, 0U, 0);
            } else if (needSleepAfterChime_) {
                pooState_ = kStateInfSleepIntro;
            } else {
                pooState_ = kStateNormalIntro;
            }
        }
    } else {
        if (chimeCheckCounter_++ < 10) {
            return;
        }
        chimeCheckCounter_ = 0;
        time(&ttime);
        tmTime = localtime(&ttime);
        clockHour = tmTime->tm_hour % 12;
        if (clockHour == 0) {
            clockHour = 12;
        }
        if (tmTime->tm_min == 0 && clockHour != lastClockHour_) {
            destroySubPooWnd();
            lastChimeTick_ = 0;
            lastClockHour_ = clockHour;
            chimeCount_ = lastClockHour_ + 1;
            pooState_ = kStateInfRunIntro;
        }
    }
}

/* Update window position and sprite to be painted. */
void setSprite(int x, int y, int sprite)
{
    /// xPoo_&A802 almost == x&y
    SetWindowWord(hPooWnd_, 0, (short)xPoo_);
    SetWindowWord(hPooWnd_, 2, (short)yPoo_);
    if (sprite >= 9 && sprite <= 14) {
        setSpriteInfo(x, y, sprite);
    } else if (xDirection_ > 0) {
        setSpriteInfo(x, y, sprite);
    } else {
        setSpriteInfo(x, y, sprite + 256);
    }
}

/* Update window position and sprite to be painted (sub). */
void setSubSprite(int x, int y, int sprite)
{
    if (sprite >= 9 && sprite <= 14) {
        setSubSpriteInfo(x, y, sprite);
    } else if (xDirectionSub_ > 0) {
        setSubSpriteInfo(x, y, sprite);
    } else {
        setSubSpriteInfo(x, y, sprite + 256);
    }
}

/* Return TRUE if the window handle is NULL or if the handle contains an existing window. */
BOOL isLandableWnd(HWND hWnd)
{
    if (hWnd == NULL) {
        return TRUE;
    } else {
        return IsWindow(hWnd);
    }
}

/* Get window rect. If the window handle is NULL, get a screen rect located right under the current screen. */
void getWndRectSp(HWND hWnd, LPRECT pRect)
{
    if (hWnd == NULL) {
        pRect->left = 0;
        pRect->right = screenWidth_;
        pRect->top = screenHeight_;
        pRect->bottom = screenHeight_ * 2;
    } else {
        GetWindowRect(hWnd, pRect);
    }
}

/* Process when out of screen view or at different positions on top of visible window. */
void land_496F(int fall)
{
    RECT rect;
    if (!bool_A7FC) {
        return;
    }
    if (hPeerWnd_ != NULL) {
        if (!isLandableWnd(hPeerWnd_)) {
            if (fall == 2) {
                pooState_ = kStateHRunFallIntro;
            } else {
                pooState_ = kStateInsituFallIntro;
            }
            return;
        }
        getWndRectSp(hPeerWnd_, &rect);
        if (rect.top > rect_A81E.top || xPoo_ + 40 < rect.left || rect.right < xPoo_) {
            if (fall == 2) {
                pooState_ = kStateHRunFallIntro;
            } else {
                pooState_ = kStateInsituFallIntro;
            }
            return;
        }
        if (rect.top < rect_A81E.top) {
            yPoo_ = rect.top - sprites_[iSprite_].height;
            rect_A81E.top = rect.top;
            rect_A81E.bottom = rect.bottom;
            rect_A81E.left = rect.left;
            rect_A81E.right = rect.right;
            setSprite(xPoo_, yPoo_, iSprite_);
            return;
        }
        if (fall == 1) {
            if (xPoo_ + 8 < rect.left && xDirection_ > 0) {
                pooState_ = kStateSideSpFallIntro;
                xPoo_ = rect.left - 10;
                return;
            }
            if (xPoo_ + 32 >= rect.right && xDirection_ < 0) {
                pooState_ = kStateSideSpFallIntro;
                xPoo_ = rect.right - 30;
                return;
            }
            if (rand() % 20 - 1 == 0 && screenHeight_ - yPoo_ > 100) {
                pooState_ = kStateFrontSpFallIntro;
                return;
            }
        }
        if (fall == 2) {
            if (xPoo_ + 32 < rect.left || xPoo_ + 8 > rect.right) {
                pooState_ = kStateHRunFallIntro;
                return;
            }
        }
    }
    if (sprites_[iSprite_].width + xPoo_ < 0 || xPoo_ > screenWidth_) {
        pooState_ = kStateInit;
        return;
    }
}

/* Process when climbing up side of a window. */
void checkPtClimbFall(void)
{
    RECT rect;
    if (bool_A7FC == 0) {
        return;
    }
    if (hPeerWnd_ != NULL) {
        if (!isLandableWnd(hPeerWnd_)) {
            pooState_ = kStateInsituFallIntro;
            return;
        }
        getWndRectSp(hPeerWnd_, &rect);
        if (rect.right < rect_A81E.right && xDirection_ > 0 || rect.left > rect_A81E.left && xDirection_ < 0) {
            pooState_ = kStateInsituFallIntro;
            return;
        }
        if (rect.right > rect_A81E.right && xDirection_ > 0 || rect.left < rect_A81E.left && xDirection_ < 0) {
            if (xDirection_ > 0) {
                xPoo_ = rect.right + 10;
            } else {
                xPoo_ = rect.left - 50;
            }
            setSprite(xPoo_, yPoo_, iSprite_);
            pooState_ = kStateInsituFallIntro;
            return;
        }
    }
}

/* Detect collision with other instances, action controlled by a flag. */
void procPooTouchX(int nextX, int prevX, int action)
{
    if (prevX < nextX) {
        nextX += 40;
        prevX = nextX - 80;
    } else {
        prevX = nextX + 80;
    }
    /// TODO: replace with getPooCollisionX
    if (calcPooHitX(nextX, prevX, yPoo_, yPoo_ + 40) != 0) {
        if (action == kTouchTurn) {
            pooState_ = kStateTurnIntro;
        }
        if (action == kTouchHit) {
            pooState_ = kStateHitIntro;
        }
    }
}

/* Detect collision with other instances and find X-coordinate. */
int getPooHitX(int nextX, int prevX)
{
    if (prevX < nextX) {
        nextX += 40;
        prevX = nextX - 80;
    } else {
        prevX = nextX + 80;
    }
    return calcPooHitX(nextX, prevX, yPoo_, yPoo_ + 40);
}

/* Reinitialize state. */
void resetState(void)
{
    pooState_ = kStateInit;
}

/* Process state change on each timer expiration. */
void stateUpdate(void)
{
    int wndHitX;
    int landY;
    HWND hHitWnd;
    HWND hWndv_8;
    RECT rect_10;
    POINT point_14;
    if (word_A84A++ > 100) {
        checkOtherPoo_3B4C(hPooWnd_);
        word_A84A = 0;
    }
    if (confChime_) {
        processChime();
    }
fallThrough:
    switch (pooState_) {
    case kStateInit:
        bool_A7FC = 0;
        srand((unsigned int)GetTickCount());
        xPoo_ = -80;
        yPoo_ = -80;
        pooState_ = kStateNormalIntro;
    case kStateNormalIntro:
        dyHitObs_ = 0;
        if (confGForce_) {
            pooState_ = kStateNormalGForceIntro;
            goto fallThrough;
        }
        ufoBeamHeight_ = 0;
        destroySubPooWnd();
        if (pooSleepState_) {
            pooState_ = pooSleepState_;
            pooSleepState_ = 0;
            break;
        }
        if (rand() % 20 == 5 && bool_A7FC == 0) {
            pooState_ = kStateWndRunIntro;
            break;
        }
        if (rand() % 40 == 5 && bool_A7FC == 0 && hasOtherPoo_ == 0) {
            pooState_ = kStateSpRunIntro;
            break;
        }
        pooState_ = nextStateNormal1_[rand() % 80];
        if (xPoo_ > screenWidth_ || xPoo_ < -40 || yPoo_ < -40 || yPoo_ > screenHeight_) {
            if ((rand() & 1) == 0) {
                xDirection_ = 1;
                xPoo_ = screenWidth_ + word_CA4C;
                yPoo_ = rand() % (screenHeight_ - 64) + word_CA4E;
            } else {
                xDirection_ = -1;
                xPoo_ = -40;
                yPoo_ = rand() % (screenHeight_ - 64) + word_CA4E;
            }
            pooState_ = kStateWalkIntro;
        }
        break;
    case kStateNormalGForceIntro:
        bool_A7FC = 1;
        ufoBeamHeight_ = 0;
        destroySubPooWnd();
        if (pooSleepState_) {
            pooState_ = pooSleepState_;
            pooSleepState_ = 0;
            break;
        }
        pooState_ = nextStateNormalGForce_[rand() % 80];
        if (xPoo_ > screenWidth_ || xPoo_ < -40 || yPoo_ < -40 || yPoo_ > screenHeight_) {
            if (rand() % 10 == 0 && hasOtherPoo_ == 0) {
                pooState_ = kStateToLongAni;
                break;
            }
            hPeerWnd_ = GetActiveWindow();
            if (hPeerWnd_ == hPooWnd_ || hPeerWnd_ == hOtherPooWnd_[kISubPooHwnd] ||
                hPeerWnd_ == NULL || isOtherPoo(hPeerWnd_)) {
                pooState_ = kStateGForceVFallIntro;
                goto fallThrough;
            }
            getWndRectSp(hPeerWnd_, &rect_A81E);
            if (rect_A81E.top < 10) {
                pooState_ = kStateGForceVFallIntro;
                goto fallThrough;
            }
            xPoo_ = (rand() % rect_A81E.right - rect_A81E.left) / 3 + (rect_A81E.right - rect_A81E.left) / 2 + rect_A81E.left - 20;
            yPoo_ = -40;
            hasBounce_ = 0;
            dyPoo_ = 0;
            dxPoo_ = 0;
            fallType_ = rand() % 2;
            pooState_ = kStateFreeFallIntro;
            if (rand() % 3 == 0) {
                pooState_ = kStateGForceVFallIntro;
                goto fallThrough;
            }
        }
        break;
    case kStateGForceVFallIntro:
        bool_A7FC = 1;
        xPoo_ = rand() % (screenWidth_ - 40);
        yPoo_ = -(rand() % 20 - (-40));
        hasBounce_ = 0;
        dyPoo_ = 0;
        dxPoo_ = 0;
        fallType_ = rand() % 2;
        if (rand() % 3 == 0) {
            putWndToTop(hPooWnd_);
        }
        pooState_ = kStateVRunFallIntro;
        break;
    case 153:
        break;
    case 154:
        break;
    case kStateSpRunIntro:
        if (screenWidth_ / 2 - 20 > xPoo_) {
            xDirection_ = 1;
        } else {
            xDirection_ = -1;
        }
        iSprite_ = kSprRun1;
        setSprite(xPoo_, yPoo_, iSprite_);
        pooState_ = kStateSpRunning;
        break;
    case kStateSpRunning:
        if (counter_A83A++ < 1) {
            break;
        }
        counter_A83A = 0;
        xPoo_ -= xDirection_ * 16;
        iSprite_ = iSprite_ == kSprRun1 ? kSprRun2 : kSprRun1;
        setSprite(xPoo_, yPoo_, iSprite_);
        if (xPoo_ < -40 || xPoo_ > screenWidth_) {
            pooState_ = kStateToLongAni;
        }
        break;
    case kStateToLongAni:
        pooState_ = nextStateToLongAni_[rand() % 8];
        break;
    case kStateRunIntro:
        canHit_ = 0;
        if ((rand() & 1) == 0) {
            canHit_ = 1;
        }
        if (canHit_ != 0) {
            updateVWnds();
        }
        iSprite_ = kSprRun1;
        setSprite(xPoo_, yPoo_, iSprite_);
        iAniFrames_ = rand() % 10 + 10;
        pooState_ = kStateRunning;
        break;
    case kStateRunning:
        if (counter_A83A++ < 1) {
            break;
        }
        counter_A83A = 0;
        if (canHit_) {
            if (xDirection_ > 0) {
                wndHitX = getVWndHitXInfo(&hHitWnd, yPoo_, yPoo_ + 40, -(xDirection_ * 16 - xPoo_), xPoo_);
            } else {
                wndHitX = getVWndHitXInfo(&hHitWnd, yPoo_, yPoo_ + 40, -(xDirection_ * 16 - xPoo_) + 40, xPoo_ + 40);
            }
            if (wndHitX != 0) {
                if (xDirection_ > 0) {
                    xPoo_ = wndHitX;
                } else {
                    xPoo_ = wndHitX - 40;
                }
                setSprite(xPoo_, yPoo_, iSprite_);
                pooState_ = kStateHitIntro;
                break;
            }
        }
        if (needWalk_ == 0) {
            xPoo_ -= xDirection_ * 16;
        }
        iSprite_ = iSprite_ == kSprRun1 ? kSprRun2 : kSprRun1;
        setSprite(xPoo_, yPoo_, iSprite_);
        if (rand() % 50 == 0 && bool_A7FC != 0) {
            pooState_ = kStateJumpIntro;
        }
        checkPooHit(TRUE);
        aniFrameDownToNormal();
        land_496F(2);
        procPooTouchX(-(xDirection_ * 16 - xPoo_), xDirection_ * 16 + xPoo_, kTouchHit);
        break;
    case kStateJumpIntro:
        dyPoo_ = -11;
        dxPoo_ = -(xDirection_ * 8);
        yPooStored_ = yPoo_;
        pooState_ = kStateJumping;
    case kStateJumping:
        xPoo_ += dxPoo_;
        yPoo_ += dyPoo_;
        dyPoo_ += 2;
        if (dyPoo_ >= -1 && dyPoo_ <= 1) {
            iSprite_ = kSprJumpAir;
        } else if (dyPoo_ < -1) {
            iSprite_ = kSprJumpUp;
        } else {
            iSprite_ = kSprJumpDown;
        }
        if (yPooStored_ <= yPoo_) {
            yPoo_ = yPooStored_;
            pooState_ = kStateRunIntro;
        }
        setSprite(xPoo_, yPoo_, iSprite_);
        checkPooHit(FALSE);
        procPooTouchX(dxPoo_ + xPoo_, xPoo_ - dxPoo_, kTouchHit);
        if (pooState_ == kStateHitIntro && yPooStored_ != yPoo_) {
            dyHitObs_ = yPoo_ - yPooStored_;
        }
        break;
    case kStateWalkIntro:
        canHit_ = 0;
        if ((bool_A7FC & !(rand() & 1)) != 0) {
            canHit_ = 1;
        }
        if (canHit_) {
            updateVWnds();
        }
        iAniFrames_ = rand() % 10 + 10;
        iSprite_ = kSprWalk1;
        setSprite(xPoo_, yPoo_, iSprite_);
        pooState_ = kStateWalking;
        break;
    case kStateWalking:
        if (counter_A83A++ < 1) {
            break;
        }
        counter_A83A = 0;
        if (canHit_ != 0) {
            if (xDirection_ > 0) {
                wndHitX = getVWndHitXInfo(&hHitWnd, yPoo_, yPoo_ + 40, -(xDirection_ * 6 - xPoo_), xPoo_);
            } else {
                wndHitX = getVWndHitXInfo(&hHitWnd, yPoo_, yPoo_ + 40, -(xDirection_ * 6 - xPoo_) + 40, xPoo_ + 40);
            }
            if (wndHitX != 0) {
                if (xDirection_ > 0) {
                    xPoo_ = wndHitX;
                } else {
                    xPoo_ = wndHitX - 40;
                }
                hPeerWnd_ = hHitWnd;
                getWndRectSp(hPeerWnd_, &rect_A81E);
                targetY_ = rect_A81E.top - 12;
                bool_A7FC = 1;
                targetX_ = xPoo_;
                iSprite_ = kSprJumpUp;
                setWndOnto(hPooWnd_, hPeerWnd_);
                pooState_ = kStateWndPtClimb;
                break;
            }
        }
        if (needWalk_ == 0) {
            xPoo_ -= xDirection_ * 6;
        }
        iSprite_ = iSprite_ == kSprWalk1 ? kSprWalk0 : kSprWalk1;
        setSprite(xPoo_, yPoo_, iSprite_);
        checkPooTurn();
        aniFrameDownToNormal();
        land_496F(1);
        procPooTouchX(-(xDirection_ * 6 - xPoo_), xDirection_ * 6 + xPoo_, kTouchTurn);
        break;
    case kStateHandstandIntro:
        randAct_ = rand() % 2;
        iAniFrames_ = rand() % 4 + 4;
        if (randAct_ != 0) {
            iSprite_ = kSprHandstFront1;
        } else {
            iSprite_ = kSprHandst1;
        }
        setSprite(xPoo_, yPoo_, iSprite_);
        pooState_ = kStateHandstanding;
        break;
    case kStateHandstanding:
        if (counter_A83A++ < 3) {
            break;
        }
        counter_A83A = 0;
        if (needWalk_ == 0) {
            xPoo_ -= xDirection_ * 6;
        }
        if (randAct_ != 0) {
            iSprite_ = iSprite_ == kSprHandstFront1 ? kSprHandstFront2 : kSprHandstFront1;
        } else {
            iSprite_ = iSprite_ == kSprHandst1 ? kSprHandst2 : kSprHandst1;
        }
        setSprite(xPoo_, yPoo_, iSprite_);
        checkPooTurn();
        aniFrameDownToNormal();
        land_496F(1);
        break;
    case kStateSexIntro:
        randAct_ = rand() % 2;
        iAniFrames_ = rand() % 3 + 3;
        if (randAct_ != 0) {
            iSprite_ = kSprSexFront1;
        } else {
            iSprite_ = kSprSexSide1;
        }
        setSprite(xPoo_, yPoo_, iSprite_);
        pooState_ = kStateSexing;
        break;
    case kStateSexing:
        if (counter_A83A++ < 3) {
            break;
        }
        counter_A83A = 0;
        if (randAct_ != 0) {
            iSprite_ = iSprite_ == kSprSexFront1 ? kSprSexFront2 : kSprSexFront1;
        } else {
            iSprite_ = iSprite_ == kSprSexSide1 ? kSprSexSide2 : kSprSexSide1;
        }
        setSprite(xPoo_, yPoo_, iSprite_);
        checkPooTurn();
        aniFrameDownToNormal();
        land_496F(0);
        break;
    case kStateSleepIntro:
        iSprite_ = kSprWinkOpen;
        setSprite(xPoo_, yPoo_, iSprite_);
        pooState_ = kStateSleepWink;
        break;
    case kStateSleepWink:
        if (counter_A83A++ < 1) {
            break;
        }
        counter_A83A = 0;
        iSprite_ += 1;
        setSprite(xPoo_, yPoo_, iSprite_);
        if (iSprite_ == kSprWinkClosed) {
            iSprite_ = kSprSleep1;
            pooState_ = kStateSleeping;
            iAniFrames_ = rand() % 8 + 8;
        }
        land_496F(0);
        break;
    case kStateSleeping:
        if (counter_A83A++ < 4) {
            break;
        }
        counter_A83A = 0;
        iSprite_ = iSprite_ == kSprSleep1 ? kSprSleep2 : kSprSleep1;
        setSprite(xPoo_, yPoo_, iSprite_);
        aniFrameDownToNormal();
        land_496F(0);
        break;
    case kStateWinkIntro:
        randAct_ = rand() % 3;
        if (randAct_ == 0) {
            iSprite_ = kSprWinkOpen;
        } else if (randAct_ == 1) {
            iSprite_ = kSprSit1;
        } else {
            iSprite_ = kSprLookUp1;
        }
        setSprite(xPoo_, yPoo_, iSprite_);
        pooState_ = kStateWinkOpenPre;
        counter_A83A = rand() % 15 + rand() % 15;
        land_496F(0);
        break;
    case kStateWinkOpenPre:
        land_496F(0);
        if (counter_A83A-- > 0) {
            break;
        }
        pooState_ = kStateWinking;
        iAniFrames_ = 0;
        break;
    case kStateWinking:
        iSprite_ = aniWinks_[randAct_][iAniFrames_];
        setSprite(xPoo_, yPoo_, iSprite_);
        iAniFrames_ += 1;
        if (iAniFrames_ > 7) {
            pooState_ = kStateWinkOpenPost;
            counter_A83A = rand() % 15 + rand() % 15;
        }
        land_496F(0);
        break;
    case kStateWinkOpenPost:
        land_496F(0);
        if (counter_A83A-- > 0) {
            break;
        }
        pooState_ = kStateNormalIntro;
        break;
    case kStateTurnIntro:
        if (counter_A83A++ < 1) {
            break;
        }
        counter_A83A = 0;
        iSprite_ = kSprWalk0;
        setSprite(xPoo_, yPoo_, iSprite_);
        if ((rand() & 1) != 0) {
            randAct_ = 0;
        } else {
            randAct_ = 1;
        }
        pooState_ = kStateTurning;
        iAniFrames_ = 0;
        break;
    case kStateTurning:
        if (counter_A83A++ < 1) {
            break;
        }
        counter_A83A = 0;
        if (randAct_ != 0) {
            if (xDirection_ > 0) {
                iSprite_ = kSprFrontL + iAniFrames_;
            } else {
                iSprite_ = kSprFrontR - iAniFrames_;
            }
        } else {
            if (xDirection_ > 0) {
                iSprite_ = kSprBackL + iAniFrames_;
            } else {
                iSprite_ = kSprBackR - iAniFrames_;
            }
        }
        setSprite(xPoo_, yPoo_, iSprite_);
        iAniFrames_ += 1;
        if (iAniFrames_ > 2) {
            xDirection_ = -xDirection_;
            pooState_ = kStateTurnEnd;
        }
        land_496F(0);
        break;
    case kStateTurnEnd:
        if (counter_A83A++ < 1) {
            break;
        }
        counter_A83A = 0;
        iSprite_ = kSprWalk0;
        setSprite(xPoo_, yPoo_, iSprite_);
        pooState_ = kStateNormalIntro;
        land_496F(0);
        break;
    case kStateHitFallIntro:
        dyPoo_ = -10;
        dxPoo_ = xDirection_ * 8;
        yPooStored_ = yPoo_;
        iAniHitFrame_ = 0;
        pooState_ = kStateHitting;
    case kStateHitting:
        xPoo_ += dxPoo_;
        yPoo_ += dyPoo_;
        dyPoo_ += 2;
        iSprite_ = aniHit_[iAniHitFrame_];
        iAniHitFrame_ += 1;
        setSprite(xPoo_, yPoo_, iSprite_);
        if (iSprite_ == kSprHitRollFirst + 1) {
            fallType_ = kFallByHit;
            pooState_ = kStateFalling;
            break;
        }
        break;
    case kStateHitEnd:
        counter_A83A = 0;
        iAniFrames_ = 0;
        randAct_ = 0;
        if ((rand() & 7) == 0) {
            randAct_ = 1;
        }
        if (rand() % 5 == 0) {
            randAct_ = 2;
        }
        pooState_ = kStateHitLayIntro;
        if (randAct_ != 0) {
            pooState_ = kStateHitRolling;
        }
        goto fallThrough;
    case kStateHitIntro:
        if (bool_A7FC) {
            pooState_ = kStateHitFallIntro;
            goto fallThrough;
        } else {
            pooState_ = kStateTurnIntro;
            goto fallThrough;
        }
        counter_A83A = 0;
        iAniFrames_ = 0;
        randAct_ = 0;
        if ((rand() & 7) == 0) {
            randAct_ = 1;
        }
        if (rand() % 5 == 0) {
            randAct_ = 2;
        }
        pooState_ = kStateHitObsIntro;
    case kStateHitObsIntro:
        procPooTouchX(xDirection_ * 10 + xPoo_, xPoo_, kTouchHit);
        if (pooState_ == kStateHitIntro) {
            if (iAniFrames_ != 0) {
                dyHitObs_ -= aniHit_[iAniFrames_ + 9];
            }
            break;
        }
        iSprite_ = aniHit_[iAniFrames_];
        setSprite(xPoo_, yPoo_ - aniHit_[iAniFrames_ + 10], iSprite_);
        iAniFrames_ += 1;
        if (randAct_ != 0 && iSprite_ == kSprHitFallen) {
            if (dyHitObs_ != 0) {
                yPoo_ -= dyHitObs_;
                xPoo_ += xDirection_ * 10;
                setSprite(xPoo_, yPoo_, iSprite_);
            }
            word_A846 = 3;
            pooState_ = kStateHitRolling;
            break;
        }
        if (iAniFrames_ > 8) {
            pooState_ = kStateHitLayIntro;
            break;
        }
        xPoo_ += xDirection_ * 10;
        break;
    case kStateHitLayIntro:
        land_496F(0);
        if (counter_A83A++ < 8) {
            break;
        }
        counter_A83A = 0;
        xDirection_ = -xDirection_;
        iSprite_ = kSprLaySide;
        setSprite(xPoo_, yPoo_, iSprite_);
        pooState_ = kStateHitLaying;
        break;
    case kStateHitLaying:
        land_496F(0);
        if (counter_A83A++ < 15) {
            break;
        }
        counter_A83A = 0;
        pooState_ = kStateNormalIntro;
        break;
    case kStateHitRolling:
        xPoo_ += xDirection_ * 8;
        if (iSprite_ == kSprHitRollLast) {
            iSprite_ = kSprHitRollFirst;
        } else {
            iSprite_ += 1;
        }
        setSprite(xPoo_, yPoo_, iSprite_);
        if (randAct_ == 2 && iSprite_ == kSprHitRollLast) {
            pooState_ = kStateSpinIntro;
            break;
        }
        if (xPoo_ > screenWidth_ || xPoo_ < -40) {
            pooState_ = kStateNormalIntro;
        }
        procPooTouchX(xDirection_ * 8 + xPoo_, -(xDirection_ * 8 - xPoo_), kTouchHit);
        if (pooState_ == kStateHitIntro) {
            if (word_A846-- > 0) {
                xDirection_ = -xDirection_;
                pooState_ = kStateHitRolling;
            } else {
                pooState_ = kStateHitRolling;
            }
        }
        land_496F(2);
        break;
    case kStatePissIntro:
        if (counter_A83A++ < 1) {
            break;
        }
        counter_A83A = 0;
        iSprite_ = kSprWalk0;
        setSprite(xPoo_, yPoo_, iSprite_);
        pooState_ = kStatePissTurnPre;
        iAniFrames_ = 0;
        break;
    case 36:
        break;
    case kStatePissTurnPre:
        if (counter_A83A++ < 1) {
            break;
        }
        counter_A83A = 0;
        if (xDirection_ > 0) {
            iSprite_ = kSprBackL + iAniFrames_;
        } else {
            iSprite_ = kSprBackR - iAniFrames_;
        }
        setSprite(xPoo_, yPoo_, iSprite_);
        iAniFrames_ += 1;
        if (iAniFrames_ > 1) {
            iSprite_ = kSprPissPre;
            pooState_ = kStatePissPre;
        }
        land_496F(0);
        break;
    case kStatePissPre:
        if (counter_A83A++ < 1) {
            break;
        }
        counter_A83A = 0;
        setSprite(xPoo_, yPoo_, iSprite_);
        iSprite_ += 1;
        if (iSprite_ > kSprPissReady) {
            iAniFrames_ = 0;
            pooState_ = kStatePissing;
            break;
        }
        land_496F(0);
        break;
    case kStatePissing:
        if (iAniFrames_ == 0) {
            if (counter_A83A++ < 10) {
                break;
            }
            counter_A83A = 0;
        } else {
            if (counter_A83A++ < 1) {
                break;
            }
            counter_A83A = 0;
        }
        if (iAniFrames_ <= 8 || iAniFrames_ == 12) {
            iSprite_ = iSprite_ == kSprPiss1 ? kSprPiss2 : kSprPiss1;
        } else {
            iSprite_ = kSprPissReady;
        }
        setSprite(xPoo_, yPoo_, iSprite_);
        if (iAniFrames_++ > 15) {
            pooState_ = kStatePissPost;
            iSprite_ = kSprPissReady;
        }
        land_496F(0);
        break;
    case kStatePissPost:
        if (counter_A83A++ < 1) {
            break;
        }
        counter_A83A = 0;
        setSprite(xPoo_, yPoo_, iSprite_);
        if (--iSprite_ < kSprPissPre) {
            iAniFrames_ = 0;
            pooState_ = kStatePissTurnPost;
            break;
        }
        land_496F(0);
        break;
    case kStatePissTurnPost:
        if (counter_A83A++ < 1) {
            break;
        }
        counter_A83A = 0;
        if (xDirection_ > 0) {
            iSprite_ = kSprBack - iAniFrames_;
        } else {
            iSprite_ = kSprBack + iAniFrames_;
        }
        setSprite(xPoo_, yPoo_, iSprite_);
        iAniFrames_ += 1;
        if (iAniFrames_ > 1) {
            pooState_ = kStateToNormal;
        }
        land_496F(0);
        break;
    case kStateToNormal:
        land_496F(0);
        if (counter_A83A++ < 1) {
            break;
        }
        counter_A83A = 0;
        iSprite_ = kSprWalk0;
        setSprite(xPoo_, yPoo_, iSprite_);
        pooState_ = kStateNormalIntro;
        iAniFrames_ = 0;
        break;
    case kStateYawnIntro:
        pooSound(IDW_YAWN, 0U, 0);
        iAniFrames_ = 0;
        pooState_ = kStateYawning;
    case kStateYawning:
        if (counter_A83A++ < 1) {
            break;
        }
        counter_A83A = 0;
        iSprite_ = aniYawn_[iAniFrames_];
        iAniFrames_ += 1;
        if (iSprite_ == kAniEnd) {
            pooState_ = kStateNormalIntro;
            break;
        }
        setSprite(xPoo_, yPoo_, iSprite_);
        land_496F(0);
        break;
    case kStateBaaIntro:
        pooSound(IDW_BAA, 0U, 0);
        iAniFrames_ = 0;
        pooState_ = kStateBaaing;
    case kStateBaaing:
        if (counter_A83A++ < 0) {
            break;
        }
        counter_A83A = 0;
        iSprite_ = aniBaa_[iAniFrames_];
        iAniFrames_ += 1;
        if (iSprite_ == kAniEnd) {
            pooState_ = kStateNormalIntro;
            break;
        }
        setSprite(xPoo_, yPoo_, iSprite_);
        land_496F(0);
        break;
    case kStateSneezeIntro:
        iAniFrames_ = 0;
        pooState_ = kStateSneezing;
    case kStateSneezing:
        if (counter_A83A++ < 0) {
            break;
        }
        counter_A83A = 0;
        if (iAniFrames_ == 2) {
            pooSound(IDW_SNEEZE, 0U, 0);
        }
        iSprite_ = aniSneeze_[iAniFrames_];
        iAniFrames_ += 1;
        if (iSprite_ == kAniEnd) {
            pooState_ = kStateNormalIntro;
            break;
        }
        setSprite(xPoo_, yPoo_, iSprite_);
        land_496F(0);
        break;
    case kStateAmazedIntro:
        iAniFrames_ = 0;
        pooState_ = kStateAmazing;
    case kStateAmazing:
        if (counter_A83A++ < 1) {
            break;
        }
        counter_A83A = 0;
        iSprite_ = aniAmazed_[iAniFrames_];
        iAniFrames_ += 1;
        if (iSprite_ == kAniEnd) {
            pooState_ = kStateNormalIntro;
            break;
        }
        setSprite(xPoo_, yPoo_, iSprite_);
        land_496F(0);
        break;
    case kStateScaryIntro:
        iAniFrames_ = 0;
        pooState_ = kStateScarying;
    case kStateScarying:
        if (counter_A83A++ < 0) {
            break;
        }
        counter_A83A = 0;
        iSprite_ = iSprite_ == kSprScary1 ? kSprScary2 : kSprScary1;
        if (iAniFrames_++ > 30) {
            iSprite_ = kSprWalk0;
            pooState_ = kStateNormalIntro;
        }
        setSprite(xPoo_, yPoo_, iSprite_);
        land_496F(0);
        break;
    case kStateEatIntro:
        iAniFrames_ = 0;
        pooState_ = kStateEating;
        showSubPoo();
        xDirectionSub_ = xDirection_;
        ySubPoo_ = yPoo_;
        iSubSprite_ = kSprFlowerBegin;
        if (xDirection_ > 0) {
            xSubPoo_ = xPoo_ - 40;
        } else {
            xSubPoo_ = xPoo_ + 40;
        }
        setSubSprite(xSubPoo_, ySubPoo_, iSubSprite_);
        bool_C0AE = 1;
        setWndOnto(hOtherPooWnd_[kISubPooHwnd], hPooWnd_);
        break;
    case kStateEating:
        if (counter_A83A++ < 2) {
            break;
        }
        counter_A83A = 0;
        iSprite_ = aniEat_[iAniFrames_];
        iAniFrames_ += 1;
        if (iSprite_ == kSprWalk1) {
            xPoo_ -= xDirection_ * 8;
            setSprite(xPoo_, yPoo_, iSprite_);
            break;
        }
        if (iSprite_ >= kSprFlowerBegin && iSprite_ <= kSprFlowerEnd) {
            iSubSprite_ = iSprite_;
            if (iSubSprite_ == kSprFlowerEnd) {
                iSubSprite_ = kSprNone;
            }
            setSubSprite(xSubPoo_, ySubPoo_, iSubSprite_);
            iSprite_ = aniEat_[iAniFrames_];
            iAniFrames_ += 1;
        }
        if (iSprite_ == kAniEnd) {
            destroySubPooWnd();
            pooState_ = kStateNormalIntro;
            break;
        }
        setSprite(xPoo_, yPoo_, iSprite_);
        break;
    case kStateEatObsIntro:
        break;
        if (counter_A83A++ < 1) {
            break;
        }
        counter_A83A = 0;
        xPoo_ -= xDirection_ * 6;
        iSprite_ = kSprWalk1;
        setSprite(xPoo_, yPoo_, iSprite_);
        checkPooTurn();
        aniFrameDownToNormal();
        land_496F(1);
        procPooTouchX(-(xDirection_ * 6 - xPoo_), xDirection_ * 6 + xPoo_, kTouchTurn);
        pooState_ = kStateEating;
        break;
    case kStateFeedIntro:
        iAniFrames_ = 0;
        pooState_ = kStateFeeding;
    case kStateFeeding:
        if (counter_A83A++ < 2) {
            break;
        }
        counter_A83A = 0;
        iSprite_ = aniEat_[iAniFrames_];
        iAniFrames_ += 1;
        if (iSprite_ >= kSprFlowerBegin && iSprite_ <= kSprFlowerEnd) {
            iSprite_ = aniEat_[iAniFrames_];
            iAniFrames_ += 1;
        }
        if (iAniFrames_ >= 16) {
            pooState_ = kStateToNormal;
            break;
        }
        setSprite(xPoo_, yPoo_, iSprite_);
        land_496F(0);
        break;
    case kStateSitIntro:
        if (counter_A83A++ < 1) {
            break;
        }
        counter_A83A = 0;
        iSprite_ = kSprWalk0;
        setSprite(xPoo_, yPoo_, iSprite_);
        pooState_ = kStateSitTurn;
        iAniFrames_ = 0;
        break;
    case kStateSitTurn:
        if (counter_A83A++ < 1) {
            break;
        }
        counter_A83A = 0;
        if (xDirection_ > 0) {
            iSprite_ = kSprFrontL + iAniFrames_;
        } else {
            iSprite_ = kSprFrontR - iAniFrames_;
        }
        iAniFrames_ += 1;
        if (iAniFrames_ > 2) {
            iSprite_ = kSprSitFront1;
            counter_A83A = -10;
            pooState_ = kStateSitting;
            iAniFrames_ = 0;
        }
        setSprite(xPoo_, yPoo_, iSprite_);
        land_496F(0);
        break;
    case kStateSitting:
        if (counter_A83A++ < 0) {
            break;
        }
        counter_A83A = 0;
        iSprite_ = aniWinks_[kWinkSitFront][iAniFrames_];
        setSprite(xPoo_, yPoo_, iSprite_);
        iAniFrames_ += 1;
        if (iAniFrames_ > 7) {
            iAniFrames_ = 0;
            pooState_ = kStateSitEnd;
            counter_A83A = -5;
        }
        land_496F(0);
        break;
    case kStateSitEnd:
        if (counter_A83A++ < 1) {
            break;
        }
        counter_A83A = 0;
        if (xDirection_ > 0) {
            iSprite_ = kSprFront - iAniFrames_;
        } else {
            iSprite_ = kSprFront + iAniFrames_;
        }
        setSprite(xPoo_, yPoo_, iSprite_);
        iAniFrames_ += 1;
        if (iAniFrames_ > 1) {
            pooState_ = kStateToNormal;
        }
        land_496F(0);
        break;
    case 64:
        break;
    case kStateRollIntro:
        iSprite_ = kSprWalk0;
        setSprite(xPoo_, yPoo_, iSprite_);
        iAniFrames_ = 0;
        pooState_ = kStateRollTurn;
        break;
    case kStateRollTurn:
        if (counter_A83A++ < 1) {
            break;
        }
        counter_A83A = 0;
        if (iAniFrames_ == 0) {
            if (xDirection_ > 0) {
                iSprite_ = kSprFrontL;
            } else {
                iSprite_ = kSprFrontR;
            }
        } else {
            iSprite_ = kSprFront;
        }
        setSprite(xPoo_, yPoo_, iSprite_);
        if (iAniFrames_++ > 0) {
            pooState_ = kStateRolling;
            frameCount_ = (rand() % 4 + 4) * 8;
            iAniFrames_ = 0;
            break;
        }
        land_496F(0);
        break;
    case kStateRolling:
        if (--iAniFrames_ < 0) {
            iAniFrames_ = 79;
        }
        xPoo_ -= xDirection_ * 8;
        iSprite_ = aniRoll_[iAniFrames_ % 8];
        setSprite(xPoo_, yPoo_, iSprite_);
        if (xDirection_ > 0 && xPoo_ < 0) {
            pooState_ = kStateHitIntro;
        }
        if (xDirection_ < 0 && screenWidth_ - sprites_[iSprite_].width < xPoo_) {
            pooState_ = kStateHitIntro;
        }
        if (--frameCount_ <= 0) {
            pooState_ = kStateRollEnd;
            iAniFrames_ = 0;
        }
        land_496F(2);
        procPooTouchX(-(xDirection_ * 8 - xPoo_), xDirection_ * 8 + xPoo_, kTouchHit);
        break;
    case kStateRollEnd:
        if (counter_A83A++ < 1) {
            break;
        }
        counter_A83A = 0;
        if (iAniFrames_ == 1) {
            if (xDirection_ > 0) {
                iSprite_ = kSprFrontL;
            } else {
                iSprite_ = kSprFrontR;
            }
        } else if (iAniFrames_ == 0) {
            iSprite_ = kSprFront;
        } else {
            iSprite_ = kSprWalk0;
        }
        setSprite(xPoo_, yPoo_, iSprite_);
        if (iAniFrames_++ > 1) {
            pooState_ = kStateNormalIntro;
            break;
        }
        land_496F(0);
        break;
    case kStateBlushIntro:
        pooState_ = kStateBlushing;
        iAniFrames_ = 0;
        break;
    case kStateBlushing:
        if (counter_A83A++ < 2) {
            break;
        }
        counter_A83A = 0;
        iSprite_ = aniBlush_[iAniFrames_];
        if (iSprite_ == kAniEnd) {
            pooState_ = kStateNormalIntro;
            break;
        }
        setSprite(xPoo_, yPoo_, iSprite_);
        iAniFrames_ += 1;
        land_496F(0);
        break;
    case kStateSlipIntro:
        iAniFrames_ = rand() % 8 + 8;
        frameCount_ = iAniFrames_;
        iSprite_ = kSprClimb1;
        if (xDirection_ > 0) {
            iSprite_ = kSprBackL;
        } else {
            iSprite_ = kSprBackR;
        }
        setSprite(xPoo_, yPoo_, iSprite_);
        pooState_ = kStateSlipTurn;
        break;
    case kStateSlipTurn:
        if (counter_A83A++ < 1) {
            break;
        }
        counter_A83A = 0;
        iSprite_ = kSprBack;
        setSprite(xPoo_, yPoo_, iSprite_);
        pooState_ = kStateSlipClimb;
        break;
    case kStateSlipClimb:
        if (counter_A83A++ < 2) {
            break;
        }
        counter_A83A = 0;
        iSprite_ = iSprite_ == kSprClimb1 ? kSprClimb2 : kSprClimb1;
        yPoo_ -= 8;
        setSprite(xPoo_, yPoo_, iSprite_);
        if (iAniFrames_-- <= 0) {
            iAniFrames_ = frameCount_;
            pooState_ = kStateSliping;
        }
        break;
    case kStateSliping:
        iSprite_ = kSprClimbSlip;
        yPoo_ += 8;
        setSprite(xPoo_, yPoo_, iSprite_);
        if (iAniFrames_-- <= 0) {
            pooState_ = kStateSlipStun;
        }
        break;
    case kStateSlipStun:
        if (counter_A83A++ < 10) {
            break;
        }
        counter_A83A = 0;
        pooState_ = kStateSlipEnd;
        iAniFrames_ = 3;
        break;
    case kStateSlipEnd:
        if (counter_A83A++ < 1) {
            break;
        }
        counter_A83A = 0;
        if (xDirection_ > 0) {
            iSprite_ = aniStandUpL_[iAniFrames_];
            iAniFrames_ += 1;
        } else {
            iSprite_ = aniStandUpR_[iAniFrames_];
            iAniFrames_ += 1;
        }
        if (iSprite_ == kAniEnd) {
            pooState_ = kStateNormalIntro;
            break;
        }
        setSprite(xPoo_, yPoo_, iSprite_);
        break;
    case kStateSpinIntro:
        counter_A83A = 0;
        iAniFrames_ = 0;
        pooState_ = kStateSpining;
    case kStateSpining:
        if (xDirection_ > 0) {
            iSprite_ = aniSpin_[iAniFrames_ % 8];
        } else {
            iSprite_ = aniSpin_[(iAniFrames_ + 4) % 8];
        }
        if (iSprite_ == kSprWalk1) {
            iSprite_ = kSprWalk0;
            if (xDirection_ > 0) {
                xDirection_ = -xDirection_;
                setSprite(xPoo_, yPoo_, iSprite_);
                xDirection_ = -xDirection_;
            } else {
                setSprite(xPoo_, yPoo_, iSprite_);
            }
        } else if (iSprite_ == kSprWalk0) {
            if (xDirection_ < 0) {
                xDirection_ = -xDirection_;
                setSprite(xPoo_, yPoo_, iSprite_);
                xDirection_ = -xDirection_;
            } else {
                setSprite(xPoo_, yPoo_, iSprite_);
            }
        } else {
            setSprite(xPoo_, yPoo_, iSprite_);
        }
        if (iAniFrames_++ >= 16) {
            iSprite_ = kSprSpinStun;
            setSprite(xPoo_, yPoo_, iSprite_);
            pooState_ = kStateSpinStun;
        }
        land_496F(0);
        break;
    case kStateSpinStun:
        land_496F(0);
        if (counter_A83A++ < 14) {
            break;
        }
        counter_A83A = 0;
        iSprite_ = kSprSpinFall;
        setSprite(xPoo_, yPoo_, iSprite_);
        pooState_ = kStateSpinFall;
        break;
    case kStateSpinFall:
        land_496F(0);
        if (counter_A83A++ < 30) {
            break;
        }
        counter_A83A = 0;
        iSprite_ = kSprWalk0;
        setSprite(xPoo_, yPoo_, iSprite_);
        pooState_ = kStateNormalIntro;
        break;
    case kStateRollSideIntro:
        iAniFrames_ = 0;
        pooState_ = kStateRollingSide;
    case kStateRollingSide:
        if (counter_A83A++ < 2) {
            break;
        }
        counter_A83A = 0;
        iSprite_ = aniRollSide_[iAniFrames_];
        iAniFrames_ += 1;
        if (iSprite_ == kAniEnd) {
            pooState_ = kStateNormalIntro;
            break;
        }
        setSprite(xPoo_, yPoo_, iSprite_);
        land_496F(0);
        break;
    case kStateInfRunIntro:
        iSprite_ = kSprRun1;
        setSprite(xPoo_, yPoo_, iSprite_);
        pooState_ = kStateInfRunning;
        break;
    case kStateInfRunning:
        counter_A83A = 0;
        iSprite_ = iSprite_ == kSprRun1 ? kSprRun2 : kSprRun1;
        setSprite(xPoo_, yPoo_, iSprite_);
        break;
    case 83:
        break;
    case 84:
        pooState_ = kStateInit;
        break;
    case kStateWndRunIntro:
        hPeerWnd_ = GetActiveWindow();
        if (hPeerWnd_ == hPooWnd_ || hPeerWnd_ == hOtherPooWnd_[kISubPooHwnd] || hPeerWnd_ == NULL || isOtherPoo(hPeerWnd_)) {
            pooState_ = kStateNormalIntro;
            break;
        }
        getWndRectSp(hPeerWnd_, &rect_A81E);
        if (rect_A81E.top < 10) {
            pooState_ = kStateNormalIntro;
            break;
        }
        if (xDirection_ > 0 && rect_A81E.right < xPoo_ && rect_A81E.top < yPoo_ && yPoo_ + 40 < rect_A81E.bottom ||
            xDirection_ < 0 && xPoo_ + 40 < rect_A81E.left && rect_A81E.top < yPoo_ && yPoo_ + 40 < rect_A81E.bottom) {
            pooState_ = kStateWndPtRunIntro;
            break;
        }
        targetX_ = (rand() % rect_A81E.right - rect_A81E.left) / 3 + (rect_A81E.right - rect_A81E.left) / 2 + rect_A81E.left - 20;
        targetY_ = rect_A81E.top - 40;
        if (screenWidth_ / 2 - 20 > xPoo_) {
            xDirection_ = 1;
        } else {
            xDirection_ = -1;
        }
        iSprite_ = kSprRun1;
        setSprite(xPoo_, yPoo_, iSprite_);
        pooState_ = kStateScrRunning;
        break;
    case kStateScrRunning:
        if (counter_A83A++ < 1) {
            break;
        }
        counter_A83A = 0;
        xPoo_ -= xDirection_ * 16;
        iSprite_ = iSprite_ == kSprRun1 ? kSprRun2 : kSprRun1;
        setSprite(xPoo_, yPoo_, iSprite_);
        if (xPoo_ < -40 || xPoo_ > screenWidth_) {
            if (!isLandableWnd(hPeerWnd_)) {
                pooState_ = kStateNormalIntro;
                break;
            }
            if (rand() % 3 == 0) {
                pooState_ = kStateGForceVFallIntro;
                goto fallThrough;
            }
            hasBounce_ = 0;
            pooState_ = kStateFreeFallIntro;
            bool_A7FC = 1;
            xPoo_ = targetX_;
            yPoo_ = -40;
            dyPoo_ = 0;
            dxPoo_ = 0;
            fallType_ = rand() % 2;
            if (fallType_ != 0) {
                dxPoo_ = -(xDirection_ * 3);
            }
            setWndOnto(hPooWnd_, hPeerWnd_);
        }
        break;
    case kStateWndPtRunIntro:
        setWndOnto(hPooWnd_, hPeerWnd_);
        if (xDirection_ > 0) {
            targetX_ = rect_A81E.right;
            targetY_ = rect_A81E.top;
        } else {
            targetX_ = rect_A81E.left - 40;
            targetY_ = rect_A81E.top;
        }
        pooState_ = kStateWndPtRunning;
    case kStateWndPtRunning:
        if (counter_A83A++ < 1) {
            break;
        }
        counter_A83A = 0;
        xPoo_ -= xDirection_ * 16;
        iSprite_ = iSprite_ == kSprRun1 ? kSprRun2 : kSprRun1;
        if (targetX_ >= xPoo_ && xDirection_ > 0 ||
            targetX_ <= xPoo_ && xDirection_ < 0) {
            if (!isLandableWnd(hPeerWnd_)) {
                pooState_ = kStateNormalIntro;
                break;
            }
            getWndRectSp(hPeerWnd_, &rect_10);
            if (rect_10.left == rect_A81E.left && rect_10.right == rect_A81E.right &&
                rect_10.top < yPoo_ && yPoo_ + 40 < rect_10.bottom) {
                targetY_ = rect_10.top - 12;
                bool_A7FC = 1;
                xPoo_ = targetX_;
                iSprite_ = kSprJumpUp;
                pooState_ = kStateWndPtClimb;
                break;
            } else {
                pooState_ = kStateNormalIntro;
                break;
            }
        }
        setSprite(xPoo_, yPoo_, iSprite_);
        break;
    case kStateWndPtClimb:
        if (counter_A83A++ < 1) {
            break;
        }
        counter_A83A = 0;
        setSprite(xPoo_, yPoo_, iSprite_);
        yPoo_ -= 6;
        iSprite_ = iSprite_ == kSprWalkSide1 ? kSprWalkSide2 : kSprWalkSide1;
        if (targetY_ >= yPoo_) {
            pooState_ = kStateWndPtTurn;
            break;
        }
        checkPtClimbFall();
        break;
    case kStateWndPtTurn:
        if (counter_A83A++ < 2) {
            break;
        }
        counter_A83A = 0;
        xPoo_ -= xDirection_ * 8;
        yPoo_ = targetY_ - 20;
        iSprite_ = kSprClimbSide;
        setSprite(xPoo_, yPoo_, iSprite_);
        pooState_ = kStateWndPtEnd;
        break;
    case kStateWndPtEnd:
        if (counter_A83A++ < 2) {
            break;
        }
        counter_A83A = 0;
        xPoo_ += xDirection_ * -24;
        yPoo_ -= 8;
        iSprite_ = kSprWalk0;
        setSprite(xPoo_, yPoo_, iSprite_);
        pooState_ = kStateWalkIntro;
        break;
    case kStateFreeFallIntro:
        dyPoo_ += 4;
        yPooStored_ = yPoo_;
        xPoo_ += dxPoo_;
        yPoo_ += dyPoo_;
        if ((landY = getWndLandY(hPeerWnd_, sprites_[iSprite_].height + yPoo_, sprites_[iSprite_].height + yPooStored_,
                              xPoo_, sprites_[iSprite_].width + xPoo_)) != 0) {
            if (landY == -1) {
                setSprite(xPoo_, yPoo_, iSprite_);
                pooState_ = kStateInit;
                break;
            }
            yPoo_ = landY - sprites_[iSprite_].height;
            if (dyPoo_ < 64 && !hasBounce_ || dyPoo_ < 8) {
                SetWindowPos(hPooWnd_, hPeerWnd_, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE);
                hasBounce_ = 0;
                iAniFrames_ = 0;
                pooState_ = kStateFreeFallLand;
                if (dyPoo_ < 36) {
                    iSprite_ = kSprFallenSit;
                    counter_A83A = -4;
                } else {
                    if ((rand() & 3) == 0) {
                        iSprite_ = kSprFallen;
                    } else {
                        iSprite_ = kSprHangBack1;
                    }
                    counter_A83A = -12;
                }
                setSprite(xPoo_, yPoo_, iSprite_);
                break;
            } else {
                dyPoo_ = dyPoo_ * 2 / -3;
                hasBounce_ = 1;
            }
        }
        if (fallType_ != kFallHang) {
            iSprite_ = iSprite_ == kSprRun1 ? kSprRun2 : kSprRun1;
        } else {
            iSprite_ = kSprHangBack1;
        }
        setSprite(xPoo_, yPoo_, iSprite_);
        break;
    case kStateFreeFallLand:
        if (counter_A83A++ < 1) {
            break;
        }
        counter_A83A = 0;
        if (fallType_ != kFallHang) {
            pooState_ = kStateWalkIntro;
            iSprite_ = kSprWalk1;
            break;
        }
        if (iAniFrames_ == 0) {
            iSprite_ = kSprBack;
        } else if (iAniFrames_ == 1) {
            if (xDirection_ > 0) {
                iSprite_ = kSprBackL;
            } else {
                iSprite_ = kSprBackR;
            }
        } else if (iAniFrames_ == 2) {
            iSprite_ = kSprWalk0;
        }
        setSprite(xPoo_, yPoo_, iSprite_);
        if (iAniFrames_++ >= 2) {
            pooState_ = kStateWalkIntro;
        }
        break;
    case kStateHRunFallIntro:
        bool_A7FC = 1;
        dyPoo_ = 0;
        dxPoo_ = -(xDirection_ * 8);
        fallType_ = kFallRun;
        pooState_ = kStateFalling;
        goto fallThrough;
    case kStateHSlowRunHsaIntro:
        bool_A7FC = 1;
        dyPoo_ = 0;
        dxPoo_ = -(xDirection_ * 3);
        fallType_ = kFallRun;
        pooState_ = kStateFalling;
        goto fallThrough;
    case kStateFallIntro:
        bool_A7FC = 1;
        dyPoo_ = 0;
        dxPoo_ = 0;
        fallType_ = kFallHang;
        pooState_ = kStateFalling;
        goto fallThrough;
    case kStateVRunFallIntro:
        bool_A7FC = 1;
        dyPoo_ = 0;
        dxPoo_ = 0;
        fallType_ = kFallRun;
        pooState_ = kStateFalling;
        goto fallThrough;
    case kStateSideFallIntro:
        bool_A7FC = 1;
        dyPoo_ = 0;
        dxPoo_ = 0;
        fallType_ = kFallSide;
        pooState_ = kStateFalling;
        goto fallThrough;
    case kStateFalling:
        updateVWnds();
        dyPoo_ += 4;
        yPooStored_ = yPoo_;
        xPoo_ += dxPoo_;
        yPoo_ += dyPoo_;
        if (yPooStored_ > screenHeight_) {
            setSprite(xPoo_, yPoo_, iSprite_);
            pooState_ = kStateInit;
            break;
        }
        if ((landY = getLandInfo(&hPeerWnd_, sprites_[iSprite_].height + yPoo_, sprites_[iSprite_].height + yPooStored_,
                              xPoo_, sprites_[iSprite_].width + xPoo_)) != 0) {
            if (!isLandableWnd(hPeerWnd_)) {
                setSprite(xPoo_, yPoo_, iSprite_);
                pooState_ = kStateInit;
                break;
            }
            getWndRectSp(hPeerWnd_, &rect_A81E);
            yPoo_ = landY - sprites_[iSprite_].height;
            if (fallType_ == kFallByHit) {
                iSprite_ = kSprHitFallen;
                setSprite(xPoo_, yPoo_, iSprite_);
                pooState_ = kStateHitEnd;
                break;
            }
            if (dyPoo_ < 64 && !hasBounce_ || dyPoo_ < 8) {
                if (hPeerWnd_ != NULL) {
                    SetWindowPos(hPooWnd_, hPeerWnd_, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE);
                }
                hasBounce_ = 0;
                iAniFrames_ = 0;
                pooState_ = kStateFallBounce;
                if (dyPoo_ < 36) {
                    iSprite_ = kSprFallenSit;
                    counter_A83A = -4;
                } else {
                    if ((rand() & 3) == 0) {
                        iSprite_ = kSprFallen;
                    } else {
                        iSprite_ = kSprHangBack1;
                    }
                    counter_A83A = -10;
                }
                if (fallType_ == 2) {
                    if (dyPoo_ < 36) {
                        iSprite_ = kSprFallSide2;
                    } else {
                        iSprite_ = kSprFallenSide;
                    }
                }
                setSprite(xPoo_, yPoo_, iSprite_);
                break;
            } else {
                if ((rand() & 7) == 0 && !hasBounce_) {
                    hasBounce_ = 0;
                    iAniFrames_ = 0;
                    pooState_ = kStateFallBounce;
                    iSprite_ = kSprFallen;
                    counter_A83A = -12;
                    if (fallType_ == 2) {
                        iSprite_ = kSprFallenSide;
                    }
                    setSprite(xPoo_, yPoo_, iSprite_);
                    break;
                }
                dyPoo_ = dyPoo_ * 2 / -3;
                hasBounce_ = 1;
            }
        }
        if (fallType_ == kFallSide) {
            iSprite_ = iSprite_ == kSprFallSide1 ? kSprFallSide2 : kSprFallSide1;
        } else if (fallType_ == kFallRun) {
            iSprite_ = iSprite_ == kSprRun1 ? kSprRun2 : kSprRun1;
        } else if (fallType_ == kFallHang) {
            iSprite_ = kSprHangBack1;
        } else {
            iSprite_ = aniHit_[iAniHitFrame_];
            iAniHitFrame_ += 1;
            if (iSprite_ == kSprHitFallen) {
                iAniHitFrame_ -= 1;
            }
        }
        if (fallType_ == kFallByHit && getPooHitX(xPoo_, xPoo_ - dxPoo_) != 0) {
            xDirection_ = -xDirection_;
            pooState_ = kStateHitIntro;
            break;
        }
        setSprite(xPoo_, yPoo_, iSprite_);
        break;
    case kStateFallBounce:
        if (counter_A83A++ < 1) {
            break;
        }
        counter_A83A = 0;
        if (fallType_ == kFallRun) {
            pooState_ = kStateWalkIntro;
            iSprite_ = kSprWalk1;
            break;
        }
        if (fallType_ == kFallSide) {
            iAniFrames_ = 0;
            pooState_ = kStateSideFallEnd;
            break;
        }
        if (iAniFrames_ == 0) {
            iSprite_ = kSprBack;
        } else if (iAniFrames_ == 1) {
            if (xDirection_ > 0) {
                iSprite_ = kSprBackL;
            } else {
                iSprite_ = kSprBackR;
            }
        } else if (iAniFrames_ == 2) {
            iSprite_ = kSprWalk0;
        }
        setSprite(xPoo_, yPoo_, iSprite_);
        if (iAniFrames_++ >= 2) {
            pooState_ = kStateWalkIntro;
        }
        break;
    case kStateSideFallEnd:
        if (counter_A83A++ < 1) {
            break;
        }
        counter_A83A = 0;
        if (iAniFrames_ == 0) {
            iSprite_ = kSprSit1;
            counter_A83A = -8;
        } else if (iAniFrames_ == 2) {
            iSprite_ = kSprWalk0;
        }
        setSprite(xPoo_, yPoo_, iSprite_);
        if (iAniFrames_++ >= 6) {
            pooState_ = kStateWalkIntro;
        }
        break;
    case kStateInsituFallIntro:
        stopPlaySound();
        iAniFrames_ = 6;
        iSprite_ = kSprWalk0;
        randAct_ = 0;
        if (rand() % 3 == 0) {
            randAct_ = 1;
        }
        pooState_ = kStateInsituFallPre;
    case kStateInsituFallPre:
        if (randAct_ != 0) {
            iSprite_ = iSprite_ == kSprAmazed1 ? kSprAmazed2 : kSprAmazed1;
        } else {
            iSprite_ = iSprite_ == kSprRun1 ? kSprRun2 : kSprRun1;
        }
        setSprite(xPoo_, yPoo_, iSprite_);
        if (iAniFrames_-- <= 0) {
            pooState_ = kStateVRunFallIntro;
        }
        break;
    case kStateFrontSpFallIntro:
        fallType_ = kFallSpFront;
        pooState_ = kStateSpFallWatch;
        goto fallThrough;
    case kStateSideSpFallIntro:
        fallType_ = kFallSpSide;
        pooState_ = kStateSpFallWatch;
        goto fallThrough;
    case kStateSpFallWatch:
        if (fallType_ == kFallSpFront) {
            point_14.x = xPoo_;
            point_14.y = yPoo_ + 39;
            *(HWND *)&rect_10 = WindowFromPoint(point_14);
            point_14.x = xPoo_ + 39;
            hWndv_8 = WindowFromPoint(point_14);
            if (*(HWND *)&rect_10 == hPooWnd_ && hWndv_8 == hPooWnd_) {
                putWndToTop(hPooWnd_);
            } else if (*(HWND *)&rect_10 == hPooWnd_) {
                setWndOnto(hPooWnd_, hWndv_8);
            } else {
                setWndOnto(hPooWnd_, *(HWND *)&rect_10);
            }
            iSprite_ = kSprDownFront1;
        } else {
            iSprite_ = kSprDown1;
        }
        setSprite(xPoo_, yPoo_, iSprite_);
        pooState_ = kStateSpFallWink;
        iAniFrames_ = 0;
        break;
    case kStateSpFallWink:
        iSprite_ = aniWinks_[4 - fallType_][iAniFrames_];
        setSprite(xPoo_, yPoo_, iSprite_);
        iAniFrames_ += 1;
        if (iAniFrames_ > 7) {
            if (fallType_ != kFallSpFront) {
                if ((rand() & 1) == 0) {
                    pooState_ = kStateSpFallPre;
                } else {
                    pooState_ = kStateSpFallJump;
                }
            } else {
                if ((rand() & 1) == 0) {
                    pooState_ = kStateSpFallPre;
                } else {
                    pooState_ = kStateSpFallGaze;
                }
            }
        }
        break;
    case 108:
        if (counter_A83A++ < 10) {
            break;
        }
        counter_A83A = 0;
        iSprite_ = kSprWalk0;
        setSprite(xPoo_, yPoo_, iSprite_);
        pooState_ = kStateNormalIntro;
        break;
    case kStateSpFallJump:
        dxPoo_ = -(xDirection_ * 14);
        iSprite_ = kSprJumpAir;
        xPoo_ += dxPoo_;
        setSprite(xPoo_, yPoo_, iSprite_);
        pooState_ = kStateHSlowRunHsaIntro;
        iAniFrames_ = 0;
        break;
    case kStateSpFallObsJump:
        xPoo_ += dxPoo_;
        dxPoo_ += xDirection_;
        setSprite(xPoo_, yPoo_, iSprite_);
        if (iAniFrames_++ > 3) {
            pooState_ = kStateHSlowRunHsaIntro;
        }
        break;
    case kStateSpFallPre:
        if (fallType_ != kFallSpFront) {
            xPoo_ += xDirection_ * -26;
            yPoo_ += 35;
            xDirection_ = -xDirection_;
        } else {
            randAct_ = rand() % kHangCount;
            if (randAct_ != 0) {
                yPoo_ += 36;
            } else {
                yPoo_ += 20;
            }
        }
        iAniFrames_ = 0;
        pooState_ = kStateSpFalling;
    case kStateSpFalling:
        if (iAniFrames_ == 0) {
            if (counter_A83A++ < 10) {
                break;
            }
            counter_A83A = 0;
        } else {
            if (counter_A83A++ < 1) {
                break;
            }
            counter_A83A = 0;
        }
        if (fallType_ != kFallSpFront) {
            iSprite_ = iSprite_ == kSprFallSide1 ? kSprFallSide2 : kSprFallSide1;
        } else {
            iSprite_ = aniHangs_[randAct_][iAniFrames_ % 4];
        }
        setSprite(xPoo_, yPoo_, iSprite_);
        iAniFrames_ += 1;
        if (iAniFrames_ > 12) {
            if (fallType_ != kFallSpFront) {
                pooState_ = kStateSideFallIntro;
            } else {
                pooState_ = kStateFallIntro;
            }
        }
        break;
    case kStateInfSleepIntro:
        needSleepAfterChime_ = 1;
        iSprite_ = kSprWinkOpen;
        setSprite(xPoo_, yPoo_, iSprite_);
        pooState_ = kStateInfSleepWink;
        break;
    case kStateInfSleepWink:
        if (counter_A83A++ < 1) {
            break;
        }
        counter_A83A = 0;
        iSprite_ += 1;
        setSprite(xPoo_, yPoo_, iSprite_);
        if (iSprite_ == kSprWinkClosed) {
            iSprite_ = kSprSleep1;
            pooState_ = kStateInfSleeping;
        }
        land_496F(0);
        break;
    case kStateInfSleeping:
        land_496F(0);
        if (counter_A83A++ < 4) {
            break;
        }
        counter_A83A = 0;
        iSprite_ = iSprite_ == kSprSleep1 ? kSprSleep2 : kSprSleep1;
        setSprite(xPoo_, yPoo_, iSprite_);
        break;
    case kStateMryPassIntro:
        xPoo_ = screenWidth_;
        yPoo_ = screenHeight_ * 7 / 8;
        iSprite_ = kSprRun1;
        xDirection_ = 1;
        setSprite(xPoo_, yPoo_, iSprite_);
        pooState_ = kStateMryPassRun;
        break;
    case kStateMryPassRun:
        if (counter_A83A++ < 1) {
            break;
        }
        counter_A83A = 0;
        xPoo_ -= xDirection_ * 16;
        iSprite_ = iSprite_ == kSprRun1 ? kSprRun2 : kSprRun1;
        setSprite(xPoo_, yPoo_, iSprite_);
        if (screenWidth_ / 2 - 20 >= xPoo_) {
            pooState_ = kStateMryPassSub;
        }
        break;
    case kStateMryPassSub:
        showSubPoo();
        xDirectionSub_ = -1;
        xSubPoo_ = -40;
        ySubPoo_ = screenHeight_ / 8;
        iSubSprite_ = kSprWifeRun1;
        iAniFrames_ = 0;
        pooState_ = kStateMryPassing;
        bool_C0AE = 0;
        putWndToTop(hPooWnd_);
        putWndToTop(hOtherPooWnd_[kISubPooHwnd]);
        break;
    case kStateMryPassing:
        if (iAniFrames_ != 0) {
            iSprite_ = aniWinks_[kWinkUp][iAniFrames_];
            setSprite(xPoo_, yPoo_, iSprite_);
            iAniFrames_ += 1;
            if (iAniFrames_ > 7) {
                iAniFrames_ = 0;
            }
        } else {
            iSprite_ = kSprLookUp1;
            setSprite(xPoo_, yPoo_, iSprite_);
            if (rand() % 20 == 0) {
                iAniFrames_ = 1;
            }
        }
        if (counter_A83A++ < 1) {
            break;
        }
        counter_A83A = 0;
        xSubPoo_ -= xDirectionSub_ * 16;
        iSubSprite_ = iSubSprite_ == kSprWifeRun1 ? kSprWifeRun2 : kSprWifeRun1;
        setSubSprite(xSubPoo_, ySubPoo_, iSubSprite_);
        if (xSubPoo_ > xPoo_) {
            xDirection_ = -1;
            setSprite(xPoo_, yPoo_, iSprite_);
        }
        if (xSubPoo_ > screenWidth_) {
            destroySubPooWnd();
            pooState_ = kStateMryPassBack;
        }
        break;
    case kStateMryPassBack:
        if (counter_A83A++ < 1) {
            break;
        }
        counter_A83A = 0;
        xPoo_ -= xDirection_ * 16;
        iSprite_ = iSprite_ == kSprRun1 ? kSprRun2 : kSprRun1;
        setSprite(xPoo_, yPoo_, iSprite_);
        if (xPoo_ > screenWidth_) {
            pooState_ = kStateNormalIntro;
        }
        break;
    case kStateMryMeetIntro:
        xPoo_ = screenWidth_;
        yPoo_ = screenHeight_ * 7 / 8;
        iSprite_ = kSprRun1;
        xDirection_ = 1;
        showSubPoo();
        xDirectionSub_ = -1;
        xSubPoo_ = -40;
        ySubPoo_ = screenHeight_ * 7 / 8;
        iSubSprite_ = kSprWifeRun1;
        setSprite(xPoo_, yPoo_, iSprite_);
        setSubSprite(xSubPoo_, ySubPoo_, iSubSprite_);
        pooState_ = kStateMryMeetRun;
        bool_C0AE = 0;
        putWndToTop(hPooWnd_);
        putWndToTop(hOtherPooWnd_[kISubPooHwnd]);
        break;
    case kStateMryMeetRun:
        if (counter_A83A++ < 1) {
            break;
        }
        counter_A83A = 0;
        xPoo_ -= xDirection_ * 16;
        iSprite_ = iSprite_ == kSprRun1 ? kSprRun2 : kSprRun1;
        xSubPoo_ -= xDirectionSub_ * 16;
        iSubSprite_ = iSubSprite_ == kSprWifeRun1 ? kSprWifeRun2 : kSprWifeRun1;
        if (xPoo_ - xSubPoo_ <= 46) {
            xPoo_ = screenWidth_ / 2 + 3;
            xSubPoo_ = screenWidth_ / 2 - 43;
            iSprite_ = kSprWalk0;
            iSubSprite_ = kSprWifeStand;
            setSprite(xPoo_, yPoo_, iSprite_);
            setSubSprite(xSubPoo_, ySubPoo_, iSubSprite_);
            iAniFrames_ = 0;
            pooState_ = kStateMryMeetBlush;
        } else {
            setSprite(xPoo_, yPoo_, iSprite_);
            setSubSprite(xSubPoo_, ySubPoo_, iSubSprite_);
        }
        break;
    case kStateMryMeetBlush:
        if (counter_A83A++ < 3) {
            break;
        }
        counter_A83A = 0;
        iSprite_ = iAniFrames_ + kSprBlush1;
        iAniFrames_ += 1;
        setSprite(xPoo_, yPoo_, iSprite_);
        if (iAniFrames_ >= 4) {
            pooState_ = kStateMryMeetFade;
        }
        break;
    case kStateMryMeetFade:
        if (counter_A83A++ < 4) {
            break;
        }
        counter_A83A = 0;
        subPooFade_ += 1;
        if (subPooFade_ > 8) {
            destroySubPooWnd();
            iAniFrames_ = 0;
            pooState_ = kStateMryMeetEnd;
        }
        break;
    case kStateMryMeetEnd:
        iSprite_ = aniMerryBlush_[iAniFrames_];
        iAniFrames_ += 1;
        if (iSprite_ == kAniEnd) {
            pooState_ = kStateNormalIntro;
            break;
        }
        setSprite(xPoo_, yPoo_, iSprite_);
        break;
    case kStateMryMateIntro:
        xPoo_ = screenWidth_;
        yPoo_ = screenHeight_ * 7 / 8;
        iSprite_ = kSprRun1;
        xDirection_ = 1;
        showSubPoo();
        xDirectionSub_ = 1;
        xSubPoo_ = screenWidth_ + 46;
        ySubPoo_ = screenHeight_ * 7 / 8;
        iSubSprite_ = kSprWifeRun1;
        setSprite(xPoo_, yPoo_, iSprite_);
        setSubSprite(xSubPoo_, ySubPoo_, iSubSprite_);
        pooState_ = kStateMryMateRun;
        bool_C0AE = 0;
        putWndToTop(hPooWnd_);
        putWndToTop(hOtherPooWnd_[kISubPooHwnd]);
        break;
    case kStateMryMateRun:
        if (counter_A83A++ < 1) {
            break;
        }
        counter_A83A = 0;
        xPoo_ -= xDirection_ * 16;
        iSprite_ = iSprite_ == kSprRun1 ? kSprRun2 : kSprRun1;
        xSubPoo_ -= xDirectionSub_ * 16;
        iSubSprite_ = iSubSprite_ == kSprWifeRun1 ? kSprWifeRun2 : kSprWifeRun1;
        if (xSubPoo_ < -40) {
            destroySubPooWnd();
            pooState_ = kStateNormalIntro;
        } else {
            setSprite(xPoo_, yPoo_, iSprite_);
            setSubSprite(xSubPoo_, ySubPoo_, iSubSprite_);
        }
        break;
    case kStateUFOTakeIntro:
        xPoo_ = screenWidth_;
        yPoo_ = screenHeight_ * 7 / 8;
        iSprite_ = kSprRun1;
        xDirection_ = 1;
        setSprite(xPoo_, yPoo_, iSprite_);
        pooState_ = kStateUFOTakeRun;
        break;
    case kStateUFOTakeRun:
        if (counter_A83A++ < 1) {
            break;
        }
        counter_A83A = 0;
        xPoo_ -= xDirection_ * 16;
        iSprite_ = iSprite_ == kSprRun1 ? kSprRun2 : kSprRun1;
        setSprite(xPoo_, yPoo_, iSprite_);
        if (screenWidth_ / 2 - 20 >= xPoo_) {
            pooState_ = kStateUFOTakeSub;
        }
        break;
    case kStateUFOTakeSub:
        showSubPoo();
        xDirectionSub_ = -1;
        xSubPoo_ = -40;
        ySubPoo_ = screenHeight_ / 8;
        iSubSprite_ = kSprUfoFirst;
        iAniFrames_ = 0;
        pooState_ = kStateUFOTakeFly;
        bool_C0AE = 0;
        putWndToTop(hPooWnd_);
        putWndToTop(hOtherPooWnd_[kISubPooHwnd]);
        break;
    case kStateUFOTakeFly:
        if (iAniFrames_ != 0) {
            iSprite_ = aniWinks_[kWinkUp][iAniFrames_];
            setSprite(xPoo_, yPoo_, iSprite_);
            iAniFrames_ += 1;
            if (iAniFrames_ > 7) {
                iAniFrames_ = 0;
            }
        } else {
            iSprite_ = kSprLookUp1;
            setSprite(xPoo_, yPoo_, iSprite_);
            if (rand() % 20 == 0) {
                iAniFrames_ = 1;
            }
        }
        xSubPoo_ -= xDirectionSub_ * 16;
        if (iSubSprite_ == kSprUfoLast) {
            iSubSprite_ = kSprUfoFirst;
        } else {
            iSubSprite_ += 1;
        }
        if (xSubPoo_ > xPoo_) {
            xSubPoo_ = xPoo_;
            iSubSprite_ = kSprUfoOnFirst;
            pooState_ = kStateUFOTakeBeam;
        }
        setSubSprite(xSubPoo_, ySubPoo_, iSubSprite_);
        break;
    case kStateUFOTakeBeam:
        iSprite_ = kSprLookUp1;
        setSprite(xPoo_, yPoo_, iSprite_);
        height_CA5C += 40;
        if (yPoo_ - ySubPoo_ - 40 <= height_CA5C) {
            height_CA5C = yPoo_ - ySubPoo_ - 40;
            height_CA5C -= 20;
            pooState_ = kStateUFOTaking;
        }
        setSubSprite(xSubPoo_, ySubPoo_, iSubSprite_);
        if (counter_A83A++ < 1) {
            break;
        }
        counter_A83A = 0;
        if (iSubSprite_ == kSprUfoOnLast) {
            iSubSprite_ = kSprUfoOnFirst;
        } else {
            iSubSprite_ += 1;
        }
        break;
    case kStateUFOTaking:
        height_CA5C -= 20;
        if (height_CA5C <= 0) {
            height_CA5C = 0;
            yPoo_ = ySubPoo_ + 40;
            pooState_ = kStateUFOTakeLeave;
            iSprite_ = iSprite_ == kSprRun1 ? kSprRun2 : kSprRun1;
            setSprite(xPoo_, yPoo_, iSprite_);
            iSubSprite_ = kSprUfoFirst;
            setSubSprite(xSubPoo_, ySubPoo_, iSubSprite_);
            break;
        }
        setSubSprite(xSubPoo_, ySubPoo_, iSubSprite_);
        iSprite_ = iSprite_ == kSprRun1 ? kSprRun2 : kSprRun1;
        yPoo_ -= 20;
        setSprite(xPoo_, yPoo_, iSprite_);
        if (counter_A83A++ < 1) {
            break;
        }
        counter_A83A = 0;
        if (iSubSprite_ == kSprUfoOnLast) {
            iSubSprite_ = kSprUfoOnFirst;
        } else {
            iSubSprite_ += 1;
        }
        break;
    case kStateUFOTakeLeave:
        xPoo_ = -80;
        setSprite(xPoo_, yPoo_, iSprite_);
        xSubPoo_ -= xDirectionSub_ * 16;
        if (iSubSprite_ == kSprUfoLast) {
            iSubSprite_ = kSprUfoFirst;
        } else {
            iSubSprite_ += 1;
        }
        if (xSubPoo_ > screenWidth_) {
            destroySubPooWnd();
            stopPlaySound();
            pooState_ = kStateNormalIntro;
            break;
        }
        setSubSprite(xSubPoo_, ySubPoo_, iSubSprite_);
        break;
    case kStateUFOEtIntro:
        ySubPoo_ = screenHeight_ * 7 / 8;
        xDirection_ = -1;
        xPoo_ = -40;
        yPoo_ = screenHeight_ / 8;
        iSprite_ = kSprUfoFirst;
        iAniFrames_ = 0;
        pooState_ = kStateUFOEtFly;
        break;
    case kStateUFOEtFly:
        xPoo_ -= xDirection_ * 16;
        if (iSprite_ == kSprUfoLast) {
            iSprite_ = kSprUfoFirst;
        } else {
            iSprite_ += 1;
        }
        if (screenWidth_ / 2 - 20 < xPoo_) {
            xPoo_ = screenWidth_ / 2 - 20;
            iSprite_ = kSprUfoOnFirst;
            pooState_ = kStateUFOEtBeam;
        }
        setSprite(xPoo_, yPoo_, iSprite_);
        break;
    case kStateUFOEtBeam:
        ufoBeamHeight_ += 40;
        if (ySubPoo_ - yPoo_ - 40 <= ufoBeamHeight_) {
            ufoBeamHeight_ = ySubPoo_ - yPoo_ - 40;
            pooState_ = kStateUFOEtAppear;
        }
        setSprite(xPoo_, yPoo_, iSprite_);
        if (counter_A83A++ < 1) {
            break;
        }
        counter_A83A = 0;
        if (iSprite_ == kSprUfoOnLast) {
            iSprite_ = kSprUfoOnFirst;
        } else {
            iSprite_ += 1;
        }
        break;
    case kStateUFOEtAppear:
        if (counter_A83A++ < 4) {
            break;
        }
        counter_A83A = 0;
        showSubPoo();
        xSubPoo_ = xPoo_;
        iSubSprite_ = kSprAlien1;
        setSubSprite(xSubPoo_, ySubPoo_, iSubSprite_);
        setSprite(xPoo_, yPoo_, iSprite_);
        pooState_ = kStateUFOEtLeave;
        bool_C0AE = 0;
        putWndToTop(hPooWnd_);
        putWndToTop(hOtherPooWnd_[kISubPooHwnd]);
        break;
    case kStateUFOEtLeave:
        if (ufoBeamHeight_ != 0) {
            ufoBeamHeight_ -= 40;
            if (ufoBeamHeight_ <= 0) {
                iSprite_ = kSprUfoFirst;
                ufoBeamHeight_ = 0;
            }
            if (iSprite_ == kSprUfoOnLast) {
                iSprite_ = kSprUfoOnFirst;
            } else {
                iSprite_ += 1;
            }
        } else {
            xPoo_ -= xDirection_ * 16;
            if (iSprite_ == kSprUfoLast) {
                iSprite_ = kSprUfoFirst;
            } else {
                iSprite_ += 1;
            }
        }
        if (xPoo_ > screenWidth_) {
            pooState_ = kStateUFOEtFade;
        }
        setSprite(xPoo_, yPoo_, iSprite_);
        if (counter_A83A++ < 1) {
            break;
        }
        counter_A83A = 0;
        iSubSprite_ = iSubSprite_ == kSprAlien1 ? kSprAlien2 : kSprAlien1;
        setSubSprite(xSubPoo_, ySubPoo_, iSubSprite_);
        break;
    case kStateUFOEtFade:
        iSubSprite_ = kSprAlien0;
        setSubSprite(xSubPoo_, ySubPoo_, iSubSprite_);
        if (counter_A83A++ < 1) {
            break;
        }
        counter_A83A = 0;
        subPooFade_ += 1;
        if (subPooFade_ > 8) {
            destroySubPooWnd();
            stopPlaySound();
            pooState_ = kStateNormalIntro;
        }
        break;
    case 141:
        break;
    case kStateUFOEnterIntro:
        xPoo_ = -80;
        yPoo_ = screenHeight_ / 8;
        setSprite(xPoo_, yPoo_, iSprite_);
        showSubPoo();
        xDirectionSub_ = -1;
        xSubPoo_ = -40;
        ySubPoo_ = screenHeight_ * 7 / 8;
        iSubSprite_ = kSprUfoFirst;
        iAniFrames_ = 0;
        pooState_ = kStateUFOEnterFly;
        bool_C0AE = 0;
        putWndToTop(hPooWnd_);
        putWndToTop(hOtherPooWnd_[kISubPooHwnd]);
        break;
    case kStateUFOEnterFly:
        xSubPoo_ -= xDirectionSub_ * 16;
        if (iSubSprite_ == kSprUfoLast) {
            iSubSprite_ = kSprUfoFirst;
        } else {
            iSubSprite_ += 1;
        }
        if (screenHeight_ / 8 < xSubPoo_) {
            xSubPoo_ = screenHeight_ / 8;
            xPoo_ = screenWidth_;
            yPoo_ = ySubPoo_;
            iSprite_ = kSprRun1;
            xDirection_ = 1;
            pooState_ = kStateUFOEnterRun;
        }
        setSubSprite(xSubPoo_, ySubPoo_, iSubSprite_);
        break;
    case kStateUFOEnterRun:
        if (counter_A83A++ < 1) {
            break;
        }
        counter_A83A = 0;
        if (iSubSprite_ == kSprUfoLast) {
            iSubSprite_ = kSprUfoFirst;
        } else {
            iSubSprite_ += 1;
        }
        setSubSprite(xSubPoo_, ySubPoo_, iSubSprite_);
        xPoo_ -= xDirection_ * 16;
        iSprite_ = iSprite_ == kSprRun1 ? kSprRun2 : kSprRun1;
        setSprite(xPoo_, yPoo_, iSprite_);
        if (xSubPoo_ + 40 >= xPoo_) {
            xPoo_ = -80;
            setSprite(xPoo_, yPoo_, iSprite_);
            pooState_ = kStateUFOEnterLeave;
        }
        break;
    case kStateUFOEnterLeave:
        ySubPoo_ -= 40;
        if (iSubSprite_ == kSprUfoLast) {
            iSubSprite_ = kSprUfoFirst;
        } else {
            iSubSprite_ += 1;
        }
        if (ySubPoo_ < -40) {
            destroySubPooWnd();
            stopPlaySound();
            pooState_ = kStateNormalIntro;
            break;
        }
        setSubSprite(xSubPoo_, ySubPoo_, iSubSprite_);
        break;
    case 146:
        break;
    case kStateBurnIntro:
        showSubPoo();
        keepSubPoo_ = 1;
        xDirectionSub_ = 1;
        iSubSprite_ = kSprBath;
        iAniFrames_ = 0;
        xPoo_ = screenWidth_;
        yPoo_ = -40;
        xDirection_ = 1;
        dxPoo_ = screenWidth_ / -96;
        dyPoo_ = screenHeight_ / 96;
        xSubPoo_ = dxPoo_ * 92 + screenWidth_;
        ySubPoo_ = dyPoo_ * 92 - 20;
        pooState_ = kStateBurning;
        bool_C0AE = 1;
        putWndToTop(hOtherPooWnd_[kISubPooHwnd]);
        putWndToTop(hPooWnd_);
    case kStateBurning:
        if (counter_A83A++ < 0) {
            break;
        }
        counter_A83A = 0;
        setSubSprite(xSubPoo_, ySubPoo_, iSubSprite_);
        xPoo_ += dxPoo_;
        yPoo_ += dyPoo_;
        iSprite_ = aniBurn_[iAniFrames_ / 3];
        iAniFrames_ += 1;
        if (iSprite_ == kAniEnd) {
            iAniFrames_ -= 1;
        }
        if (iSprite_ == kAniEnd || iSprite_ == kSprBurned1 || iSprite_ == kSprBurned2) {
            iSprite_ = iSprite_ == kSprBurned1 ? kSprBurned2 : kSprBurned1;
        }
        if (iSprite_ == kSprBurning1 || iSprite_ == kSprBurning2) {
            iSprite_ = iSprite_ == kSprBurning1 ? kSprBurning2 : kSprBurning1;  /// higher fps for droping ani
        }
        setSprite(xPoo_, yPoo_, iSprite_);
        if (xSubPoo_ + 10 > xPoo_ || ySubPoo_ + 20 < yPoo_) { ///landing
            iAniFrames_ = 0;
            pooState_ = kStateBurnBath;
            iSprite_ = kSprNone;
            setSprite(xPoo_, yPoo_, iSprite_);
            break;
        }
        break;
    case kStateBurnBath:
        if (counter_A83A++ < 1) {
            break;
        }
        counter_A83A = 0;
        xPoo_ = -80;
        setSprite(xPoo_, yPoo_, iSprite_);
        iSubSprite_ = aniBath_[iAniFrames_];
        iAniFrames_ += 1;
        if (iSubSprite_ == kAniEnd) {
            xPoo_ = xSubPoo_;
            yPoo_ = ySubPoo_;
            iAniFrames_ = 0;
            pooSound(IDW_BAA, 0U, 0);
            pooState_ = kStateBurnBathOut;
            break;
        }
        setSubSprite(xSubPoo_, ySubPoo_, iSubSprite_);
        break;
    case kStateBurnBathOut:
        iSprite_ = aniBathOut_[iAniFrames_];
        iAniFrames_ += 1;
        if (iSprite_ == kAniEnd) {
            iSprite_ = kSprWalk0;
            pooState_ = kStateBurnBathLeave;
            break;
        }
        if (iSprite_ >= kSprBathClimb1 && iSprite_ <= kSprBathClimb3) {
            setSprite(xPoo_, yPoo_ - 20, iSprite_);
        } else {
            setSprite(xPoo_, yPoo_, iSprite_);
        }
        break;
    case kStateBurnBathLeave:
        if (counter_A83A++ < 1) {
            break;
        }
        counter_A83A = 0;
        xPoo_ -= xDirection_ * 6;
        iSprite_ = iSprite_ == kSprWalk1 ? kSprWalk0 : kSprWalk1;
        setSprite(xPoo_, yPoo_, iSprite_);
        if (xPoo_ < -40) {
            destroySubPooWnd();
            pooState_ = kStateNormalIntro;
            break;
        }
        break;
    case kStateMax:
        break;
    default:
        break;
    }
}

/* Environment-affected action change, controlled by a flag. */
void postAction(int action)
{
    switch (action) {
    case kActionUnknown:
        pooState_ = kStateNormalIntro;
        if (bool_A7FC != 0) {
            pooState_ = kStateVRunFallIntro;
        }
        break;
    case kActionInfRun:
        pooState_ = kStateInfRunIntro;
        break;
    case kActionFall:
        pooState_ = kStateVRunFallIntro;
        break;
    case kActionInfSleep:
        pooSleepState_ = kStateInfSleepIntro;
        break;
    case kActionFeed:
        pooState_ = kStateFeedIntro;
        break;
    default:
        break;
    }
}

/* Process debug window action change. */
void debugAction(WPARAM action)
{
    ufoBeamHeight_ = 0;
    stopPlaySound();
    destroySubPooWnd();
    switch (action) {
    case 0:        ///Normal
        pooState_ = kStateNormalIntro;
        break;
    case 1:        ///Run
        pooState_ = kStateRunIntro;
        break;
    case 2:        ///Walk
        pooState_ = kStateWalkIntro;
        break;
    case 3:        ///Handstand
        pooState_ = kStateHandstandIntro;
        break;
    case 4:        ///Sex
        pooState_ = kStateSexIntro;
        break;
    case 5:        ///Sleep
        pooState_ = kStateSleepIntro;
        break;
    case 6:        ///Wink
        pooState_ = kStateWinkIntro;
        break;
    case 7:        ///Turn
        pooState_ = kStateTurnIntro;
        break;
    case 8:        ///Hit
        pooState_ = kStateHitIntro;
        break;
    case 9:        ///Piss
        pooState_ = kStatePissIntro;
        break;
    case 10:        ///Yawn
        pooState_ = kStateYawnIntro;
        break;
    case 11:        ///Baa
        pooState_ = kStateBaaIntro;
        break;
    case 12:        ///Amazed
        pooState_ = kStateAmazedIntro;
        break;
    case 13:        ///Scary
        pooState_ = kStateScaryIntro;
        break;
    case 14:        ///Eat
        pooState_ = kStateEatIntro;
        break;
    case 15:        ///Sit
        pooState_ = kStateSitIntro;
        break;
    case 16:        ///Sneeze
        pooState_ = kStateSneezeIntro;
        break;
    case 17:        ///Burn
        pooState_ = kStateBurnIntro;
        break;
    case 18:        ///Merry1
        pooState_ = kStateMryPassIntro;
        break;
    case 19:        ///Merry2
        pooState_ = kStateMryMeetIntro;
        break;
    case 20:        ///Merry3
        pooState_ = kStateMryMateIntro;
        break;
    case 21:        ///UFO1
        pooState_ = kStateUFOTakeIntro;
        break;
    case 22:        ///UFO2
        pooState_ = kStateUFOEtIntro;
        break;
    case 23:        ///UFO3
        pooState_ = kStateUFOEnterIntro;
        break;
    case 24:        ///Roll
        pooState_ = kStateRollIntro;
        break;
    case 25:        ///Blush
        pooState_ = kStateBlushIntro;
        break;
    case 26:        ///Slip
        pooState_ = kStateSlipIntro;
        break;
    case 27:        ///Fall
        pooState_ = kStateFallIntro;
        break;
    case 28:        ///Jump
        pooState_ = kStateJumpIntro;
        break;
    case 29:        ///Spin
        pooState_ = kStateSpinIntro;
        break;
    default:
        break;
    }
}

/* Move window by offset. */
void movePooWnd(int offsX, int offsY)
{
    xPoo_ += offsX;
    yPoo_ += offsY;
    setSprite(xPoo_, yPoo_, iSprite_);
}

/* Initialize bitmaps (sub). */
BOOL initSubWnd(HWND hWnd)
{
    HDC hdc;
    hdc = GetDC(hWnd);
    paintSubBmpBuf_[0] = CreateCompatibleBitmap(hdc, 100, 100);
    if (paintSubBmpBuf_[0] == NULL) {
        goto failed;
    }
    paintSubBmpBuf_[1] = CreateCompatibleBitmap(hdc, 100, 100);
    if (paintSubBmpBuf_[1] == NULL) {
        goto failed;
    }
    bmp_A854 = CreateCompatibleBitmap(hdc, 100, 100);
    if (bmp_A854 == NULL) {
        goto failed;
    }
    bmp_A85A = CreateCompatibleBitmap(hdc, 40, 40);
    if (bmp_A85A == NULL) {
        goto failed;
    }
    bmp_A85C = CreateCompatibleBitmap(hdc, 40, 40);
    if (bmp_A85C == NULL) {
        goto failed;
    }
    word_CA4C = 0;
    word_CA4E = 0;
    screenWidth_ = GetSystemMetrics(SM_CXSCREEN);
    screenHeight_ = GetSystemMetrics(SM_CYSCREEN);
    ReleaseDC(hWnd, hdc);
    subPooFade_ = 0;
    height_CA5C = 0;
    keepSubPoo_ = 0;
    lastSubSpriteX_ = 0;
    lastSubSpriteY_ = 0;
    lastSubSpriteBmpW_ = 0;
    lastSubSpriteBmpH_ = 0;
    return TRUE;
failed:
    ReleaseDC(hWnd, hdc);
    return FALSE;
}

/* Release bitmaps (sub). */
void releaseSubWnd()
{
    DeleteObject(bmp_A85C);
    DeleteObject(bmp_A85A);
    DeleteObject(paintSubBmpBuf_[0]);
    DeleteObject(paintSubBmpBuf_[1]);
    DeleteObject(bmp_A854);
}

/* Update window position and sprite to be actually used (sub). */
void setSubSpriteInfo(int x, int y, int sprite)
{
    curSubSpriteX_ = x;
    curSubSpriteY_ = y;
    curSubSpriteBmp0_ = sprites_[sprite].bitmaps[0];
    curSubSpriteBmp1_ = sprites_[sprite].bitmaps[1];
    curSubSpriteBmpX_ = sprites_[sprite].x;
    curSubSpriteBmpY_ = sprites_[sprite].y;
    curSubSpriteBmpW_ = sprites_[sprite].width;
    curSubSpriteBmpH_ = sprites_[sprite].height;
}

/* Clear window (sub). */
void repaintSubPoo(HWND hWnd)
{
    if (keepSubPoo_) {
        return;
    }
    lastSubSpriteX_ = 0;
    lastSubSpriteY_ = 0;
    lastSubSpriteBmpW_ = 0;
    lastSubSpriteBmpH_ = 0;
    MoveWindow(hWnd, 0, 0, 0, 0, TRUE);
    bool_A872 = 1;
    word_C0BA = 1;
    lastSubSpriteBmp0_ = NULL;
}

/* Render sprite with double buffering (with fade out effect) (sub). */
void paint_9438(HWND hWnd)
{
    HDC hdcScr;
    HDC hdc_4;
    HDC hdc_6;
    int posy_C;
    int posx_E;
    int intersectH;
    int intersectW;
    int posy_14;
    int posx_16;
    int height_18;
    int width_1A;
    int botY1;
    int rightX1;
    if (bool_A870) {
        return;
    }
    if (lastSubSpriteX_ == curSubSpriteX_ && lastSubSpriteY_ == curSubSpriteY_ &&
        lastSubSpriteBmp0_ == curSubSpriteBmp0_ && lastSubSpriteBmpX_ == curSubSpriteBmpX_ &&
        subPooFade_ == 0 && height_CA5C == 0) {
        return;
    }
    iPaintSubBmp_ ^= 1;
    hdcScr = GetDC(NULL);
    SelectPalette(hdcScr, hPalette_, FALSE);
    hdc_4 = CreateCompatibleDC(hdcScr);
    hdc_6 = CreateCompatibleDC(hdcScr);
    SelectPalette(hdc_6, hPalette_, FALSE);
    SelectPalette(hdc_4, hPalette_, FALSE);
    posx_16 = max(curSubSpriteX_, lastSubSpriteX_);
    posy_14 = max(curSubSpriteY_, lastSubSpriteY_);
    intersectW = min(curSubSpriteBmpW_ + curSubSpriteX_, lastSubSpriteBmpW_ + lastSubSpriteX_) - posx_16;
    intersectH = min(curSubSpriteY_ + curSubSpriteBmpH_, lastSubSpriteY_ + lastSubSpriteBmpH_) - posy_14;
    if (intersectW <= 0 || intersectH <= 0) {
        word_A898 = 1;
        if (bool_A872) {
            bool_A872 = 0;
        }
        paintSubRectX_ = curSubSpriteX_;
        paintSubRectY_ = curSubSpriteY_;
        paintSubRectW_ = curSubSpriteBmpW_;
        paintSubRectH_ = curSubSpriteBmpH_;
        SelectObject(hdc_4, paintSubBmpBuf_[iPaintSubBmp_]);
        BitBlt(hdc_4, 0, 0, paintSubRectW_, paintSubRectH_,
               hdcScr, paintSubRectX_, paintSubRectY_, SRCCOPY);
    } else {
        word_A898 = 0;
        paintSubRectX_ = min(curSubSpriteX_, lastSubSpriteX_);
        paintSubRectY_ = min(curSubSpriteY_, lastSubSpriteY_);
        paintSubRectW_ = max(curSubSpriteBmpW_ + curSubSpriteX_, lastSubSpriteBmpW_ + lastSubSpriteX_) - paintSubRectX_;
        paintSubRectH_ = max(curSubSpriteY_ + curSubSpriteBmpH_, lastSubSpriteY_ + lastSubSpriteBmpH_) - paintSubRectY_;
        SelectObject(hdc_4, paintSubBmpBuf_[iPaintSubBmp_]);
        BitBlt(hdc_4, 0, 0, paintSubRectW_, paintSubRectH_,
               hdcScr, paintSubRectX_, paintSubRectY_, SRCCOPY);
        rightX1 = max(paintSubRectX_, lastPaintSubRectX_);  ///LastRectX
        botY1 = max(paintSubRectY_, lastPaintSubRectY_);
        width_1A = min(paintSubRectW_ + paintSubRectX_, lastPaintSubRectW_ + lastPaintSubRectX_) - rightX1;
        height_18 = min(paintSubRectY_ + paintSubRectH_, lastPaintSubRectY_ + lastPaintSubRectH_) - botY1;
        posx_16 = max(0, rightX1 - paintSubRectX_);
        posy_14 = max(0, botY1 - paintSubRectY_);
        posx_E = max(0, rightX1 - lastPaintSubRectX_);
        posy_C = max(0, botY1 - lastPaintSubRectY_);
        if (width_1A > 0 && height_18 > 0) {
            SelectObject(hdc_6, paintSubBmpBuf_[iPaintSubBmp_ ^ 1]);
            BitBlt(hdc_4, posx_16, posy_14, width_1A, height_18,
                   hdc_6, posx_E, posy_C, SRCCOPY);
        }
    }
    if (curSubSpriteBmp0_ != NULL) {
        SelectObject(hdc_6, bmp_A854);
        BitBlt(hdc_6, 0, 0, paintSubRectW_, paintSubRectH_,
               hdc_4, 0, 0, SRCCOPY);
        posx_16 = max(0, curSubSpriteX_ - paintSubRectX_);
        posy_14 = max(0, curSubSpriteY_ - paintSubRectY_);
        if (curSubSpriteBmp1_ != NULL) {
            if (subPooFade_ != 0) {
                if (subPooFade_ == 1) {
                    SelectObject(hdc_4, curSubSpriteBmp1_);
                    SelectObject(hdc_6, bmp_A85C);
                    BitBlt(hdc_6, 0, 0, 40, 40,
                           hdc_4, curSubSpriteBmpX_, curSubSpriteBmpY_, SRCCOPY);
                    SelectObject(hdc_4, curSubSpriteBmp0_);
                    SelectObject(hdc_6, bmp_A85A);
                    BitBlt(hdc_6, 0, 0, 40, 40,
                           hdc_4, curSubSpriteBmpX_, curSubSpriteBmpY_, SRCCOPY);
                }
                SelectObject(hdc_4, bmp_A85C);
                SelectObject(hdc_6, sprites_[172].bitmaps[0]);
                BitBlt(hdc_4, subPooFade_ - 1, subPooFade_ - 1, 41 - subPooFade_, 40,
                       hdc_6, sprites_[172].x, 0, SRCPAINT);
                SelectObject(hdc_4, bmp_A85A);
                SelectObject(hdc_6, sprites_[172].bitmaps[1]);
                BitBlt(hdc_4, subPooFade_ - 1, subPooFade_ - 1, 41 - subPooFade_, 40,
                       hdc_6, sprites_[172].x, 0, SRCAND);
                SelectObject(hdc_6, bmp_A854);
                SelectObject(hdc_4, bmp_A85C);
                BitBlt(hdc_6, posx_16, posy_14, curSubSpriteBmpW_, curSubSpriteBmpH_,
                       hdc_4, 0, 0, SRCAND);
                SelectObject(hdc_4, bmp_A85A);
                BitBlt(hdc_6, posx_16, posy_14, curSubSpriteBmpW_, curSubSpriteBmpH_,
                       hdc_4, 0, 0, SRCPAINT);
            } else {
                SelectObject(hdc_4, curSubSpriteBmp1_);
                BitBlt(hdc_6, posx_16, posy_14, curSubSpriteBmpW_, curSubSpriteBmpH_,
                       hdc_4, curSubSpriteBmpX_, curSubSpriteBmpY_, SRCAND);
                SelectObject(hdc_4, curSubSpriteBmp0_);
                BitBlt(hdc_6, posx_16, posy_14, curSubSpriteBmpW_, curSubSpriteBmpH_,
                       hdc_4, curSubSpriteBmpX_, curSubSpriteBmpY_, SRCPAINT);
            }
        } else {
            SelectObject(hdc_4, curSubSpriteBmp0_);
            BitBlt(hdc_6, posx_16, posy_14, curSubSpriteBmpW_, curSubSpriteBmpH_,
                   hdc_4, curSubSpriteBmpX_, curSubSpriteBmpY_, SRCCOPY);
        }
        bool_A870 = 1;
        word_CA5E = 1;
        MoveWindow(hWnd, paintSubRectX_, paintSubRectY_, paintSubRectW_, paintSubRectH_ + height_CA5C, TRUE);
        word_CA5E = 0;
    }
    DeleteDC(hdc_4);
    DeleteDC(hdc_6);
    lastPaintSubRectX_ = paintSubRectX_;
    lastPaintSubRectY_ = paintSubRectY_;
    lastPaintSubRectW_ = paintSubRectW_;
    lastPaintSubRectH_ = paintSubRectH_;
    lastSubSpriteX_ = curSubSpriteX_;
    lastSubSpriteY_ = curSubSpriteY_;
    lastSubSpriteBmpW_ = curSubSpriteBmpW_;
    lastSubSpriteBmpH_ = curSubSpriteBmpH_;
    lastSubSpriteBmp0_ = curSubSpriteBmp0_;
    lastSubSpriteBmpX_ = curSubSpriteBmpX_;
    lastSubSpriteBmpY_ = curSubSpriteBmpY_;
    ReleaseDC(NULL, hdcScr);
}

/* Render UFO beam (if any) and present render targets onto window (sub). */
BOOL paint_9A49(HWND hWnd)
{
    HDC hdc_2;
    HDC hdc_4;
    RECT rect_C;
    HDC hdc_E;
#ifdef _WIN32
    HDC screen;
#endif
    if (bool_A870 == 0) {
        return TRUE;
    }
    bool_A870 = 0;
    hdc_2 = GetDC(hWnd);
    SelectPalette(hdc_2, hPalette_, FALSE);
    RealizePalette(hdc_2);
    hdc_4 = CreateCompatibleDC(hdc_2);
    SelectPalette(hdc_4, hPalette_, FALSE);
    SelectObject(hdc_4, bmp_A854);
    BitBlt(hdc_2, 0, 0, paintSubRectW_, paintSubRectH_, hdc_4, 0, 0, SRCCOPY);
    if (height_CA5C != 0) {
        if (bmp_C0B8 == NULL) {
            bmp_C0B8 = CreateCompatibleBitmap(hdc_2, 40, screenHeight_ * 4 / 5);
            if (bmp_C0B8 == NULL) {
                goto failed;
            }
        }
        if (bmp_C0B2 == NULL) {
            bmp_C0B2 = CreateCompatibleBitmap(hdc_2, 40, screenHeight_ * 4 / 5);
            if (bmp_C0B2 == NULL) {
                goto failed;
            }
        }
        if (brush_CA44 == NULL) {
            brush_CA44 = CreateSolidBrush(RGB(255, 255, 0));
        }
        if (brush_C0B4 == NULL) {
            brush_C0B4 = CreateSolidBrush(RGB(128, 128, 0));
        }
        hdc_E = CreateCompatibleDC(hdc_2);
        SelectObject(hdc_E, bmp_C0B2);
#ifdef _WIN32
        /* Screen contents with height of only 40 pixels can be captured from window device context on Windows 10. Capture directly from screen instead. */
        screen = GetDC(NULL);
        BitBlt(hdc_E, 0, 0, 40, height_CA5C, screen, paintSubRectX_, paintSubRectY_ + 40, SRCCOPY);
        ReleaseDC(NULL, screen);
#else
        BitBlt(var_E, 0, 0, 40, height_CA5C, var_2, 0, 40, SRCCOPY);
#endif
        rect_C.left = 0;
        rect_C.top = 0;
        rect_C.right = 40;
        rect_C.bottom = height_CA5C;
        SelectObject(hdc_4, bmp_C0B8);
        FillRect(hdc_4, &rect_C, brush_CA44);
        BitBlt(hdc_E, 0, 0, 40, height_CA5C, hdc_4, 0, 0, SRCAND);
        FillRect(hdc_4, &rect_C, brush_C0B4);
        BitBlt(hdc_E, 0, 0, 40, height_CA5C, hdc_4, 0, 0, SRCPAINT);
        BitBlt(hdc_2, 0, 40, 40, height_CA5C, hdc_E, 0, 0, SRCCOPY);
        DeleteDC(hdc_E);
        DeleteDC(hdc_4);
    } else {
        if (brush_C0B4 != NULL) {
            DeleteObject(brush_C0B4);
            brush_C0B4 = NULL;
        }
        if (brush_CA44 != NULL) {
            DeleteObject(brush_CA44);
            brush_CA44 = NULL;
        }
        if (bmp_C0B2 != NULL) {
            DeleteObject(bmp_C0B2);
            bmp_C0B2 = NULL;
        }
        if (bmp_C0B8 != NULL) {
            DeleteObject(bmp_C0B8);
            bmp_C0B8 = NULL;
        }
        DeleteDC(hdc_4);
    }
    ReleaseDC(hWnd, hdc_2);
    return TRUE;
failed:
    ReleaseDC(hWnd, hdc_2);
    DestroyWindow(hWnd);
    return FALSE;
}
