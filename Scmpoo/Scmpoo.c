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

/* GetActiveWindow is made local to current thread in 32-bit Windows. The function of global scope is now GetForegroundWindow. */
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

enum SpriteState
{
    kStateNormal1 = 1,
    kStateRun1 = 7,     //8, break
    //8: 8/jump/hit
    kStateJump1 = 9,    // >10
    kStateJumping = 10,
    //10: 10/run
    kStateWalk1 = 11,
    kStateHandstand1 = 13,
    kStateSex1 = 15,
    kStateSexing = 16,
    kStateSleep1 = 17,
    kStateWink1 = 20,
    kStateTurn1 = 24,
    kStateHit1 = 30,
    kStatePiss1 = 35,
    kStateYawn1 = 43,
    kStateYawning = 44,
    kStateBaa1 = 45,
    kStateBaaing = 46,
    kStateSneeze1 = 47,
    kStateAmazed1 = 49,
    kStateScary1 = 51,
    kStateScarying = 52,
    kStateEat1 = 53,
    kStateFeed1 = 56,
    kStateSit1 = 58,
    kStateBlush1 = 62,
    kStateBlushing = 63,
    kStateRoll1 = 65,
    kStateSpin1 = 69,
    kStateSlip1 = 75,
    kStateFall1 = 96,
    kStateMerry11 = 116,
    kStateMerry21 = 121,
    kStateMerry31 = 126,
    kStateUFO11 = 128,
    kStateUFO21 = 135,
    kStateUFO31 = 142,
    kStateBurn1 = 147,  // >148
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

int word_9CF0 = 245; /* Palette search maximum index (unused). */
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
    kStateWalk1, kStateWalk1, kStateRun1, kStateRun1,
    kStateWalk1, kStateWalk1, kStateRun1, kStateRun1,
    kStateWalk1, kStateWalk1, kStateRun1, kStateRun1,
    kStateWalk1, kStateWalk1, kStateRun1, kStateRun1,
    kStateWalk1, kStateWalk1, kStateRun1, kStateRun1,
    kStateWalk1, kStateWalk1, kStateRun1, kStateRun1,
    kStateWalk1, kStateWalk1, kStateRun1, kStateRun1,
    kStateWalk1, kStateWalk1, kStateRun1, kStateRun1,
    kStateWalk1, kStateWalk1, kStateRun1, kStateRun1,
    kStateWalk1, kStateWalk1, kStateRun1, kStateRun1,
    kStateWalk1, kStateRun1, kStateSleep1, kStateWink1,
    kStateWalk1, kStateRun1, kStateSleep1, kStateWink1,
    kStateWalk1, kStateRun1, kStateSleep1, kStateWink1,
    kStateWalk1, kStateRun1, kStateSleep1, kStateWink1,
    kStateWalk1, kStateRun1, kStateSleep1, kStateWink1,
    kStateWalk1, kStateRun1, kStateSleep1, kStateWink1,
    kStateWalk1, kStateRun1, kStateSleep1, kStateWink1,
    kStateHandstand1, kStateSit1, kStateSex1, kStateBaa1,
    kStatePiss1, kStateEat1, kStateYawn1, kStateSneeze1,
    kStateBaa1, kStateSneeze1, kStateAmazed1, kStateScary1
};
WORD nextStateNormal2_[80] = { /* Normal action table (option "Gravity always on" enabled). */
    kStateWalk1, kStateWalk1, kStateRun1, kStateRun1,
    kStateWalk1, kStateWalk1, kStateRun1, kStateRun1,
    kStateWalk1, kStateWalk1, kStateRun1, kStateRun1,
    kStateWalk1, kStateWalk1, kStateRun1, kStateRun1,
    kStateWalk1, kStateWalk1, kStateRun1, kStateRun1,
    kStateWalk1, kStateWalk1, kStateRun1, kStateRun1,
    kStateWalk1, kStateWalk1, kStateRun1, kStateRun1,
    kStateWalk1, kStateWalk1, kStateRun1, kStateRun1,
    kStateWalk1, kStateWalk1, kStateRun1, kStateRun1,
    kStateWalk1, kStateWalk1, kStateRun1, kStateRun1,
    kStateWalk1, kStateRun1, kStateSleep1, kStateWink1,
    kStateWalk1, kStateRun1, kStateSleep1, kStateWink1,
    kStateWalk1, kStateRun1, kStateSleep1, kStateWink1,
    kStateWalk1, kStateRun1, kStateSleep1, kStateWink1,
    kStateWalk1, kStateRun1, kStateSleep1, kStateWink1,
    kStateWalk1, kStateRun1, kStateSleep1, kStateWink1,
    kStateWalk1, kStateRun1, kStateSleep1, kStateWink1,
    kStateHandstand1, kStateSit1, kStateSex1, kStateRoll1,
    kStatePiss1, kStateEat1, kStateYawn1, kStateSlip1,
    kStateBaa1, kStateSneeze1, kStateAmazed1, kStateScary1
};
WORD nextStateNormal6_[8] = { /* Special action table. */
    kStateMerry11, kStateMerry21, kStateMerry31, kStateBurn1,
    kStateUFO11, kStateUFO21, kStateUFO31, kStateBurn1
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
WORD word_A798 = 0; /* Has cursor position changed in current timer period? */
int posx_A79A = 0; /* Cursor position with respect to screen, X-coordinate */
int posy_A79C = 0; /* Cursor position with respect to screen, Y-coordinate */
WORD isMouseBtnDown_ = 0; /* Dragging Screen Mate window? */
WORD isRBtnDbClicked_ = 0; /* Destroy Screen Mate window by right double-click? */
WORD word_A7A2 = 0; /* Unused. */
RECT lastWndRect_ = {0, 0, 0, 0}; /* Screen Mate window rectangle. */
WORD word_A7AC = 0; /* Not to clear window on WM_PAINT? */
POINT point_A7B0 = {0, 0}; /* Current cursor position. */
WORD word_A7B4 = 0; /* Not to clear window on WM_PAINT? (sub) */
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
WORD word_A7FE = 0; /* Is collision with visible window enabled? */
int xPoo_ = 0; /* Current X-coordinate. */
int yPoo_ = 0; /* Current Y-coordinate. */
int iSprite_ = 0; /* Sprite index. */
int dyPoo_ = 0; /* Vertical speed. */
int dxPoo_ = 0; /* Horizontal speed. */
int yPooStored_ = 0; /* Y-coordinate memory. */
int xSubPoo_ = 0; /* Current X-coordinate (sub). */
int ySubPoo_ = 0; /* Current Y-coordinate (sub). */
int iSubSprite_ = 0; /* Sprite index (sub). */
HWND hWnd_A81C = NULL; /* Active window or window to land on. */
RECT stru_A81E = {0L, 0L, 0L, 0L}; /* Rectangle of active window or window to land on. */
int iAniFrames_ = 0; /* Animation frame counter. */
int word_A828 = 0; /* Random duration period counter. */
int rand_A82A = 0; /* Random case number for action. */
WORD word_A82C = 0; /* Unused. */
HGLOBAL hPlaySoundData_ = NULL; /* Global handle for holding WAVE resource in memory. */
int lastClockHour_ = 0; /* Current time hour. */
int chimeCount_ = 0; /* Remaining times for chime. */
DWORD lastChimeTick_ = 0; /* Tick count. */
int chimeCheckCounter_ = 0; /* Time check period counter. */
int counter_A83A = 0; /* Frame period counter. */
int word_A83C = 0; /* Target X-coordinate for window edge attachment. */
int word_A83E = 0; /* Target Y-coordinate for window edge attachment. */
WORD word_A840 = 0; /* Bounce when falling? */
int fallt_A842 = 0; /* Case number for fall action. */
int dy_A844 = 0; /* Collision vertical speed (unused). */
int word_A846 = 0; /* Collision spin frame counter (unused). */
WORD index_A848 = 0; /* Collision animation frame index. */
int word_A84A = 0; /* Known instance list update period counter. */
HBITMAP paintSubBmpBuf_[2] = {NULL, NULL}; /* Double buffer (sub). */
HBITMAP bmp_A854 = NULL; /* Sprite render target (sub). */
HBITMAP curSubSpriteBmp0_ = NULL; /* Sprite colour image for current frame (sub). */
HBITMAP curSubSpriteBmp1_ = NULL; /* Sprite mask image for current frame (sub). */
HBITMAP word_A85A = NULL; /* Fade out processed colour image (sub). */
HBITMAP word_A85C = NULL; /* Fade out processed mask image (sub). */
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
WORD pooState_ = 0; /* State. */
SpriteInfo sprites_[512] = {{{NULL, NULL}, 0, 0, 0, 0}}; /* Sprite list. First 256 for left-facing sprites, last 256 for right-facing sprites. */
int word_C0A4 = 0; /* No mouse action consecutive period counter. */
UINT confChime_ = 0U; /* Configuration: Chime */
WORD word_C0AE = 0; /* Screen Mate window on top of subwindow? (unused) */
HWND hPooWnd_ = NULL; /* Self instance window handle. */
HBITMAP word_C0B2 = NULL; /* UFO beam render target. */
HBRUSH word_C0B4 = NULL; /* UFO beam paint colour brush. */
UINT confNoSleep_ = 0U; /* Configuration: Always moving */
HBITMAP word_C0B8 = NULL; /* UFO beam colour rectangle image. */
WORD word_C0BA = 0; /* Remaining no-update periods after clearing windows. */
WindowInfo otherVWnds_[32] = {{NULL, {0, 0, 0, 0}, {0}}}; /* Currently visible window list. */
WORD hasOtherPoo_ = 0; /* Prevent special actions? */
WORD confTopMost__ = 0; /* Always on top? (unused) */
int otherPooCount_ = 0; /* Known instance count. */
UINT confGForce_ = 0U; /* Configuration: Gravity always on */
HBRUSH word_CA44 = NULL; /* UFO beam mask colour brush. */
int word_CA46 = 0; /* Fade out frame counter. */
int otherPooCount2_ = 0; /* Unused. */
HPALETTE hPalette_ = NULL; /* Palette being used by window. */
int word_CA4C = 0; /* Unused. */
int word_CA4E = 0; /* Unused. */
int screenWidth_ = 0; /* Screen width. */
int screenHeight_ = 0; /* Screen height. */
WORD word_CA54 = 0; /* Temporarily holds sleep timeout action. */
WORD keepSubPoo_ = 0; /* Not to clear subwindow? */
HINSTANCE hSelfInst_ = NULL; /* Current instance. */
UINT confSound_ = 0U; /* Configuration: Cry */
int height_CA5C = 0; /* UFO beam height (sub). */
WORD word_CA5E = 0; /* Unused. */
HWND hOtherPooWnd_[kMaxPooCount+1] = {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL}; /* Known instance list. When no other instance exists, [8] is used to store subwindow handle. */
int height_CA72 = 0; /* UFO beam height. */
int otherVWndCount_ = 0; /* Number of currently visible windows. */
WORD word_CA76 = 0; /* Sleeping after timeout? */
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
HPALETTE sub_1791(HDC, BYTE, BYTE, BYTE);
void sub_17FD(HDC, HPALETTE, HPALETTE, int);
WORD getBmpColorOfFirstPixel(void FAR *bmpData);
void FAR * getBmpPixelBegin(void FAR *data);
void bmpFlipCopy(void FAR *, void FAR *, UINT flags);
int PASCAL WinMain(HINSTANCE, HINSTANCE, LPSTR, int);
void sub_1DDC(void);
LRESULT CALLBACK pooWndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK pooSubWndProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK configDlgProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK debugDlgProc(HWND, UINT, WPARAM, LPARAM);
void showSubPoo(void);
void destroySubPooWnd(void);
void putWndToTop(HWND hWnd);
void sub_2B01(HWND, HWND);
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
void sub_399D(HWND, int, int, int, int);
BOOL sub_39D6(HWND);
int calcPooCollisionX(int, int, int, int);
void checkOtherPoo_3B4C(HWND hWnd);
BOOL checkOtherPoo(HWND);
void sub_3D12(HWND);
void sub_3D5F(HWND);
void sub_3DA7(HWND hWnd);
void updateVWnds(void);
int sub_3E7C(HWND *, int, int, int, int);
int sub_408C(HWND *, int, int, int, int);
int sub_419E(HWND, int, int, int, int);
void playSoundRes(int, UINT, WORD);
void stopPlaySound(void);
void asyncPlaySound(LPCSTR path);
void pooSound(int, UINT, WORD);
BOOL initResources(HDC hdc);
void releaseResources(void);
void checkPooTurn(void);
void checkPooHit(BOOL);
void aniFrameDown42(void);
void processChime(void);
void setSprite(int, int, int);
void setSubSprite(int, int, int);
BOOL isLandableWnd(HWND);
void getWndRectSp(HWND, LPRECT);
void land_496F(int);
void sub_4B3B(void);
void procPooTouchX(int, int, int);
int getPooCollisionX(int, int);
void resetState(void);
void sub_4CF8(void);
void event_8FD7(int);
void debugAction(WPARAM);
void movePooWnd(int, int);
BOOL initSubWnd(HWND);
void releaseSubWnd();
void setSubSpriteInfo(int, int, int);
void sub_93DF(HWND);
void paint_9438(HWND);
BOOL sub_9A49(HWND);

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
HPALETTE sub_1791(HDC arg_0, BYTE arg_2, BYTE arg_3, BYTE arg_4)
{
    const BYTE FAR * var_4;
    HPALETTE var_6;
    LOGPALETTE * var_8;
    var_8 = (LOGPALETTE *)LocalAlloc(LPTR, 12U);
    var_8->palVersion = 0x0300;
    var_8->palNumEntries = 1;
    var_8->palPalEntry[0].peRed = arg_2;
    var_8->palPalEntry[0].peGreen = arg_3;
    var_8->palPalEntry[0].peBlue = arg_4;
    var_8->palPalEntry[0].peFlags = 0;
    var_4/* += 4 */;
    var_6 = CreatePalette(var_8);
    LocalFree(var_8);
    return var_6;
}

/* Set palette entries based on another palette with nearest colours (unused). */
void sub_17FD(HDC arg_0, HPALETTE arg_2, HPALETTE arg_4, int arg_6)
{
    PALETTEENTRY * var_2;
    PALETTEENTRY * var_4;
    int var_6;
    int var_8;
    COLORREF var_C;
    COLORREF var_10;
    int var_12;
    if ((GetDeviceCaps(arg_0, RASTERCAPS) & RC_PALETTE) == 0) {
        return;
    }
    var_6 = GetDeviceCaps(arg_0, SIZEPALETTE);
    if (var_6 == 0) {
        var_6 = 256;
    }
    var_2 = (PALETTEENTRY *)LocalAlloc(LPTR, arg_6 * sizeof(PALETTEENTRY));
    var_4 = (PALETTEENTRY *)LocalAlloc(LPTR, var_6 * sizeof(PALETTEENTRY));
    GetPaletteEntries(arg_4, 0U, arg_6, var_2);
    GetPaletteEntries(arg_2, 0U, var_6, var_4);
    for (var_8 = 0; var_8 < arg_6; var_8 += 1) {
        var_C = *(COLORREF *)&var_2[var_8];
        var_12 = GetNearestPaletteIndex(arg_2, var_C);
        var_10 = *(COLORREF *)&var_4[var_12];
        if (var_10 != var_C) {
            if (var_12 < 10 || var_12 > word_9CF0) {
                if (word_9CF0 < 10) {
                    break;
                } else {
                    var_12 = word_9CF0--;
                }
            }
            *(COLORREF *)&var_4[var_12] = var_C;
        }
    }
    SetPaletteEntries(arg_2, 0U, var_6, var_4);
    LocalFree(var_4);
    LocalFree(var_2);
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
void sub_1DDC(void)
{
    word_A798 = 1;
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
                event_8FD7(4);
            }
        }
        DragFinish((HDROP)wParam);
        break;
    case WM_USER + 1:
        switch (lParam) {
        case 0x202:
        case 0x205:
            if (word_C0AE != 0) {
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
            AppendMenu(hPopMenu, 0U, 101U, "Screen Mate Settings...");
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
        if (word_A7A2 != 0) {
            pWndPos = (LPWINDOWPOS)lParam;
        }
        pWndPos->flags |= SWP_NOCOPYBITS;
        word_A7AC = 1;
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
            if (point_A7B0.x != cursorPos.x || point_A7B0.y != cursorPos.y) {
                point_A7B0.x = cursorPos.x;
                point_A7B0.y = cursorPos.y;
                word_A798 = 1;
            }
            if (word_CA76 != 0) {
                if (word_A798 != 0) {
                    word_CA76 = 0;
                    word_C0A4 = 0;
                    event_8FD7(0);
                }
            } else {
                if (word_A798 != 0) {
                    word_A798 = 0;
                    word_C0A4 = 0;
                } else {
                    if (word_C0A4++ > 300) {
                        event_8FD7(3);
                    }
                }
            }
        }
        word_A7AC = 0;
        hPooWnd_ = hWnd;
        sub_4CF8();
        paint_3284(hWnd);
        paint_3717(hWnd);
        word_A7A2 = 1;
        return 0;
    case WM_USER:
        if (wParam == 1) {
            sub_3D5F((HWND)lParam);
        }
        if (wParam == 2) {
            sub_3DA7((HWND)lParam);
        }
        return 0;
    case WM_PAINT:
        if (word_A7AC != 0) {
            word_A7AC = 0;
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
        case 101U:
            SendMessage(hWnd, WM_USER + 2, 0, 0);
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
        event_8FD7(1);
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
            event_8FD7(0);
            if (uMsg == WM_RBUTTONUP) {
                event_8FD7(2);
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
    case WM_USER + 2:
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
            event_8FD7(2);
        }
        break;
    case WM_DESTROY:
        sub_3D12(hWnd);
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
        word_A7B4 = 1;
        return 0;
    case WM_WINDOWPOSCHANGED:
        return 0;
    case WM_TIMER:
        word_A7B4 = 0;
        paint_9438(hWnd);
        if (!sub_9A49(hWnd)) {
            hasOtherPoo_ = 1;
            resetState();
        }
        return 0;
    case WM_PAINT:
        if (word_A7B4 != 0) {
            word_A7B4 = 0;
            ValidateRect(hWnd, NULL);
            return 0;
        }
        sub_93DF(hWnd);
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
void sub_2B01(HWND arg_0, HWND arg_2)
{
    if (confTopMost__ == 0) {
        SetWindowPos(arg_0, arg_2, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE);
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
    if (word_C0B4 != NULL) {
        DeleteObject(word_C0B4);
        word_C0B4 = NULL;
    }
    if (word_CA44 != NULL) {
        DeleteObject(word_CA44);
        word_CA44 = NULL;
    }
    if (word_C0B2 != NULL) {
        DeleteObject(word_C0B2);
        word_C0B2 = NULL;
    }
    if (word_C0B8 != NULL) {
        DeleteObject(word_C0B8);
        word_C0B8 = NULL;
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
        height_CA72 == 0) {
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
        MoveWindow(hWnd, paintRectX_, paintRectY_, paintRectW_, paintRectH_ + height_CA72, TRUE);
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
    if (height_CA72 != 0) {
        if (word_C0B8 == NULL) {
            word_C0B8 = CreateCompatibleBitmap(hdc_2, 40, screenHeight_ * 4 / 5);
            if (word_C0B8 == NULL) {
                goto failed;
            }
        }
        if (word_C0B2 == NULL) {
            word_C0B2 = CreateCompatibleBitmap(hdc_2, 40, screenHeight_ * 4 / 5);
            if (word_C0B2 == NULL) {
                goto failed;
            }
        }
        if (word_CA44 == NULL) {
            word_CA44 = CreateSolidBrush(RGB(255, 255, 0));
        }
        if (word_C0B4 == NULL) {
            word_C0B4 = CreateSolidBrush(RGB(128, 128, 0));
        }
        hdc_E = CreateCompatibleDC(hdc_2);
        SelectObject(hdc_E, word_C0B2);
#ifdef _WIN32
        /* Screen contents with height of only 40 pixels can be captured from window device context on Windows 10. Capture directly from screen instead. */
        screen = GetDC(NULL);
        BitBlt(hdc_E, 0, 0, 40, height_CA72, screen, paintRectX_, paintRectY_ + 40, SRCCOPY);
        ReleaseDC(NULL, screen);
#else
        BitBlt(var_E, 0, 0, 40, word_CA72, var_2, 0, 40, SRCCOPY);
#endif
        var_C.left = 0;
        var_C.top = 0;
        var_C.right = 40;
        var_C.bottom = height_CA72;
        SelectObject(hdc_4, word_C0B8);
        FillRect(hdc_4, &var_C, word_CA44);
        BitBlt(hdc_E, 0, 0, 40, height_CA72, hdc_4, 0, 0, SRCAND);
        FillRect(hdc_4, &var_C, word_C0B4);
        BitBlt(hdc_E, 0, 0, 40, height_CA72, hdc_4, 0, 0, SRCPAINT);
        BitBlt(hdc_2, 0, 40, 40, height_CA72, hdc_E, 0, 0, SRCCOPY);
        DeleteDC(hdc_E);
        DeleteDC(hdc_4);
    } else {
        if (word_C0B4 != NULL) {
            DeleteObject(word_C0B4);
            word_C0B4 = NULL;
        }
        if (word_CA44 != NULL) {
            DeleteObject(word_CA44);
            word_CA44 = NULL;
        }
        if (word_C0B2 != NULL) {
            DeleteObject(word_C0B2);
            word_C0B2 = NULL;
        }
        if (word_C0B8 != NULL) {
            DeleteObject(word_C0B8);
            word_C0B8 = NULL;
        }
        DeleteDC(hdc_4);
    }
    ReleaseDC(hWnd, hdc_2);
    return;
failed:
    ReleaseDC(hWnd, hdc_2);
}

/* Unused. */
void sub_399D(HWND arg_0, int arg_2, int arg_4, int arg_6, int arg_8)
{
    MoveWindow(arg_0, arg_2, arg_4, arg_6, arg_8, FALSE);
    MoveWindow(arg_0, 0, 0, 0, 0, TRUE);
}

/* Find if a window has a match in known instance list. */
BOOL sub_39D6(HWND hWnd)
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
int calcPooCollisionX(int head, int tail, int top, int bot)
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
        SendMessage(hOtherPooWnd_[i], WM_USER, (WPARAM)1, (LPARAM)hWnd);
    }
    return TRUE;
}

/* Notify other instances of self destruction. */
void sub_3D12(HWND arg_0)
{
    int i;
    for (i = 0; i < kMaxPooCount; i += 1) {
        if (hOtherPooWnd_[i] != NULL) {
            SendMessage(hOtherPooWnd_[i], WM_USER, (WPARAM)2, (LPARAM)arg_0);
        }
    }
}

/* Add window into known instance list. */
void sub_3D5F(HWND hWnd)
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
void sub_3DA7(HWND hWnd)
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
int sub_3E7C(HWND * pHwnd, int posy_2, int posy_4, int posx_6, int posx_8)
{
    int i;
    int j;
    RECT rect;
    for (i = 0; i < otherVWndCount_; i += 1) {
        if (posx_8 > posx_6) {
            if (otherVWnds_[i].rect.right >= posx_6 && otherVWnds_[i].rect.right < posx_8 &&
                    otherVWnds_[i].rect.top < posy_2 && otherVWnds_[i].rect.bottom > posy_4) {
                for (j = 0; j < i; j += 1) {
                    if (otherVWnds_[j].rect.top <= posy_2 && otherVWnds_[j].rect.bottom >= posy_4 &&
                            otherVWnds_[j].rect.left <= posx_6 && otherVWnds_[j].rect.right >= posx_8) {
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
            if (otherVWnds_[i].rect.left <= posx_6 && otherVWnds_[i].rect.left > posx_8 &&
                    otherVWnds_[i].rect.top < posy_2 && otherVWnds_[i].rect.bottom > posy_4) {
                for (j = 0; j < i; j += 1) {
                    if (otherVWnds_[j].rect.top <= posy_2 && otherVWnds_[j].rect.bottom >= posy_4 &&
                            otherVWnds_[j].rect.left <= posx_6 && otherVWnds_[j].rect.right >= posx_8) {
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
int sub_408C(HWND * pHwnd, int posy_2, int posy_4, int posx_6, int posx_8)
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
int sub_419E(HWND arg_0, int arg_2, int arg_4, int arg_6, int arg_8)
{
    RECT var_8;
    if (IsWindow(arg_0)) {
        GetWindowRect(arg_0, &var_8);
        if (var_8.top <= arg_2 && var_8.top > arg_4 && var_8.left < arg_8 && var_8.right > arg_6) {
            return var_8.top;
        }
    }
    if (arg_2 > screenHeight_) {
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
        pooState_ = kStateTurn1;
    }
    if (xDirection_ < 0 && screenWidth_ - sprites_[iSprite_].width < xPoo_) {
        pooState_ = kStateTurn1;
    }
    if (xDirection_ > 0 && screenWidth_ - sprites_[iSprite_].width > xPoo_ && rand() % 20 == 0) {
        pooState_ = kStateTurn1;
    }
    if (xDirection_ < 0 && xPoo_ > 0 && rand() % 20 == 0) {
        pooState_ = kStateTurn1;
    }
}

/* Flag-controlled collision and turn around. */
void checkPooHit(BOOL turnCheck)
{
    if (bool_A7FC == 0) {
        if (xDirection_ > 0 && xPoo_ < 0) {
            pooState_ = kStateHit1;
        }
        if (xDirection_ < 0 && screenWidth_ - sprites_[iSprite_].width < xPoo_) {
            pooState_ = kStateHit1;
        }
    }
    if (turnCheck) {
        if (xDirection_ > 0 && screenWidth_ - 80 > xPoo_ && rand() % 20 == 0) {
            pooState_ = kStateTurn1;
        }
        if (xDirection_ < 0 && xPoo_ > 40 && rand() % 20 == 0) {
            pooState_ = kStateTurn1;
        }
    }
}

/* Switch to standing sprite after certain frames. */
void aniFrameDown42(void)
{
    if (iAniFrames_-- <= 0) {
        pooState_ = 42;
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
            } else if (word_CA76 != 0) {
                pooState_ = 113;
            } else {
                pooState_ = kStateNormal1;
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
            pooState_ = 81;
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
void land_496F(int arg_0)
{
    RECT rect;
    if (bool_A7FC == 0) {
        return;
    }
    if (hWnd_A81C != NULL) {
        if (!isLandableWnd(hWnd_A81C)) {
            if (arg_0 == 2) {
                pooState_ = 94;
            } else {
                pooState_ = 102;
            }
            return;
        }
        getWndRectSp(hWnd_A81C, &rect);
        if (rect.top > stru_A81E.top || xPoo_ + 40 < rect.left || rect.right < xPoo_) {
            if (arg_0 == 2) {
                pooState_ = 94;
            } else {
                pooState_ = 102;
            }
            return;
        }
        if (rect.top < stru_A81E.top) {
            yPoo_ = rect.top - sprites_[iSprite_].height;
            stru_A81E.top = rect.top;
            stru_A81E.bottom = rect.bottom;
            stru_A81E.left = rect.left;
            stru_A81E.right = rect.right;
            setSprite(xPoo_, yPoo_, iSprite_);
            return;
        }
        if (arg_0 == 1) {
            if (xPoo_ + 8 < rect.left && xDirection_ > 0) {
                pooState_ = 105;
                xPoo_ = rect.left - 10;
                return;
            }
            if (xPoo_ + 32 >= rect.right && xDirection_ < 0) {
                pooState_ = 105;
                xPoo_ = rect.right - 30;
                return;
            }
            if (rand() % 20 - 1 == 0 && screenHeight_ - yPoo_ > 100) {
                pooState_ = 104;
                return;
            }
        }
        if (arg_0 == 2) {
            if (xPoo_ + 32 < rect.left || xPoo_ + 8 > rect.right) {
                pooState_ = 94;
                return;
            }
        }
    }
    if (sprites_[iSprite_].width + xPoo_ < 0 || xPoo_ > screenWidth_) {
        pooState_ = 0;
        return;
    }
}

/* Process when climbing up side of a window. */
void sub_4B3B(void)
{
    RECT var_8;
    if (bool_A7FC == 0) {
        return;
    }
    if (hWnd_A81C != NULL) {
        if (!isLandableWnd(hWnd_A81C)) {
            pooState_ = 102;
            return;
        }
        getWndRectSp(hWnd_A81C, &var_8);
        if (var_8.right < stru_A81E.right && xDirection_ > 0 || var_8.left > stru_A81E.left && xDirection_ < 0) {
            pooState_ = 102;
            return;
        }
        if (var_8.right > stru_A81E.right && xDirection_ > 0 || var_8.left < stru_A81E.left && xDirection_ < 0) {
            if (xDirection_ > 0) {
                xPoo_ = var_8.right + 10;
            } else {
                xPoo_ = var_8.left - 50;
            }
            setSprite(xPoo_, yPoo_, iSprite_);
            pooState_ = 102;
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
    if (calcPooCollisionX(nextX, prevX, yPoo_, yPoo_ + 40) != 0) {
        if (action == 1) {
            pooState_ = kStateTurn1;
        }
        if (action == 2) {
            pooState_ = kStateHit1;
        }
    }
}

/* Detect collision with other instances and find X-coordinate. */
int getPooCollisionX(int nextX, int prevX)
{
    if (prevX < nextX) {
        nextX += 40;
        prevX = nextX - 80;
    } else {
        prevX = nextX + 80;
    }
    return calcPooCollisionX(nextX, prevX, yPoo_, yPoo_ + 40);
}

/* Reinitialize state. */
void resetState(void)
{
    pooState_ = 0;
}

/* Process state change on each timer expiration. */
void sub_4CF8(void)
{
    int posx_2;
    int var_4;
    HWND var_6;
    HWND var_8;
    RECT var_10;
    POINT var_14;
    if (word_A84A++ > 100) {
        checkOtherPoo_3B4C(hPooWnd_);
        word_A84A = 0;
    }
    if (confChime_) {
        processChime();
    }
fallThrough:
    switch (pooState_) {
    case 0:
        bool_A7FC = 0;
        srand((unsigned int)GetTickCount());
        xPoo_ = -80;
        yPoo_ = -80;
        pooState_ = kStateNormal1;
    case kStateNormal1:
        dy_A844 = 0;
        if (confGForce_) {
            pooState_ = 2;
            goto fallThrough;
        }
        height_CA72 = 0;
        destroySubPooWnd();
        if (word_CA54 != 0) {
            pooState_ = word_CA54;
            word_CA54 = 0;
            break;
        }
        if (rand() % 20 == 5 && bool_A7FC == 0) {
            pooState_ = 85;
            break;
        }
        if (rand() % 40 == 5 && bool_A7FC == 0 && hasOtherPoo_ == 0) {
            pooState_ = 4;
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
            pooState_ = kStateWalk1;
        }
        break;
    case 2:
        bool_A7FC = 1;
        height_CA72 = 0;
        destroySubPooWnd();
        if (word_CA54 != 0) {
            pooState_ = word_CA54;
            word_CA54 = 0;
            break;
        }
        pooState_ = nextStateNormal2_[rand() % 80];
        if (xPoo_ > screenWidth_ || xPoo_ < -40 || yPoo_ < -40 || yPoo_ > screenHeight_) {
            if (rand() % 10 == 0 && hasOtherPoo_ == 0) {
                pooState_ = 6;
                break;
            }
            hWnd_A81C = GetActiveWindow();
            if (hWnd_A81C == hPooWnd_ || hWnd_A81C == hOtherPooWnd_[kISubPooHwnd] ||
                hWnd_A81C == NULL || sub_39D6(hWnd_A81C)) {
                pooState_ = 3;
                goto fallThrough;
            }
            getWndRectSp(hWnd_A81C, &stru_A81E);
            if (stru_A81E.top < 10) {
                pooState_ = 3;
                goto fallThrough;
            }
            xPoo_ = (rand() % stru_A81E.right - stru_A81E.left) / 3 + (stru_A81E.right - stru_A81E.left) / 2 + stru_A81E.left - 20;
            yPoo_ = -40;
            word_A840 = 0;
            dyPoo_ = 0;
            dxPoo_ = 0;
            fallt_A842 = rand() % 2;
            pooState_ = 92;
            if (rand() % 3 == 0) {
                pooState_ = 3;
                goto fallThrough;
            }
        }
        break;
    case 3:
        bool_A7FC = 1;
        xPoo_ = rand() % (screenWidth_ - 40);
        yPoo_ = -(rand() % 20 - (-40));
        word_A840 = 0;
        dyPoo_ = 0;
        dxPoo_ = 0;
        fallt_A842 = rand() % 2;
        if (rand() % 3 == 0) {
            putWndToTop(hPooWnd_);
        }
        pooState_ = 97;
        break;
    case 153:
        break;
    case 154:
        break;
    case 4:
        if (screenWidth_ / 2 - 20 > xPoo_) {
            xDirection_ = 1;
        } else {
            xDirection_ = -1;
        }
        iSprite_ = kSprRun1;
        setSprite(xPoo_, yPoo_, iSprite_);
        pooState_ = 5;
        break;
    case 5:
        if (counter_A83A++ < 1) {
            break;
        }
        counter_A83A = 0;
        xPoo_ -= xDirection_ * 16;
        iSprite_ = iSprite_ == kSprRun1 ? kSprRun2 : kSprRun1;
        setSprite(xPoo_, yPoo_, iSprite_);
        if (xPoo_ < -40 || xPoo_ > screenWidth_) {
            pooState_ = 6;
        }
        break;
    case 6:
        pooState_ = nextStateNormal6_[rand() % 8];
        break;
    case kStateRun1:
        word_A7FE = 0;
        if ((rand() & 1) == 0) {
            word_A7FE = 1;
        }
        if (word_A7FE != 0) {
            updateVWnds();
        }
        iSprite_ = kSprRun1;
        setSprite(xPoo_, yPoo_, iSprite_);
        iAniFrames_ = rand() % 10 + 10;
        pooState_ = 8;
        break;
    case 8:
        if (counter_A83A++ < 1) {
            break;
        }
        counter_A83A = 0;
        if (word_A7FE != 0) {
            if (xDirection_ > 0) {
                posx_2 = sub_3E7C(&var_6, yPoo_, yPoo_ + 40, -(xDirection_ * 16 - xPoo_), xPoo_);
            } else {
                posx_2 = sub_3E7C(&var_6, yPoo_, yPoo_ + 40, -(xDirection_ * 16 - xPoo_) + 40, xPoo_ + 40);
            }
            if (posx_2 != 0) {
                if (xDirection_ > 0) {
                    xPoo_ = posx_2;
                } else {
                    xPoo_ = posx_2 - 40;
                }
                setSprite(xPoo_, yPoo_, iSprite_);
                pooState_ = kStateHit1;
                break;
            }
        }
        if (word_A82C == 0) {
            xPoo_ -= xDirection_ * 16;
        }
        iSprite_ = iSprite_ == kSprRun1 ? kSprRun2 : kSprRun1;
        setSprite(xPoo_, yPoo_, iSprite_);
        if (rand() % 50 == 0 && bool_A7FC != 0) {
            pooState_ = kStateJump1;
        }
        checkPooHit(TRUE);
        aniFrameDown42();
        land_496F(2);
        procPooTouchX(-(xDirection_ * 16 - xPoo_), xDirection_ * 16 + xPoo_, 2);
        break;
    case kStateJump1:
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
            pooState_ = kStateRun1;
        }
        setSprite(xPoo_, yPoo_, iSprite_);
        checkPooHit(FALSE);
        procPooTouchX(dxPoo_ + xPoo_, xPoo_ - dxPoo_, 2);
        if (pooState_ == kStateHit1 && yPooStored_ != yPoo_) {
            dy_A844 = yPoo_ - yPooStored_;
        }
        break;
    case kStateWalk1:
        word_A7FE = 0;
        if ((bool_A7FC & !(rand() & 1)) != 0) {
            word_A7FE = 1;
        }
        if (word_A7FE != 0) {
            updateVWnds();
        }
        iAniFrames_ = rand() % 10 + 10;
        iSprite_ = kSprWalk1;
        setSprite(xPoo_, yPoo_, iSprite_);
        pooState_ = 12;
        break;
    case 12:
        if (counter_A83A++ < 1) {
            break;
        }
        counter_A83A = 0;
        if (word_A7FE != 0) {
            if (xDirection_ > 0) {
                posx_2 = sub_3E7C(&var_6, yPoo_, yPoo_ + 40, -(xDirection_ * 6 - xPoo_), xPoo_);
            } else {
                posx_2 = sub_3E7C(&var_6, yPoo_, yPoo_ + 40, -(xDirection_ * 6 - xPoo_) + 40, xPoo_ + 40);
            }
            if (posx_2 != 0) {
                if (xDirection_ > 0) {
                    xPoo_ = posx_2;
                } else {
                    xPoo_ = posx_2 - 40;
                }
                hWnd_A81C = var_6;
                getWndRectSp(hWnd_A81C, &stru_A81E);
                word_A83E = stru_A81E.top - 12;
                bool_A7FC = 1;
                word_A83C = xPoo_;
                iSprite_ = kSprJumpUp;
                sub_2B01(hPooWnd_, hWnd_A81C);
                pooState_ = 89;
                break;
            }
        }
        if (word_A82C == 0) {
            xPoo_ -= xDirection_ * 6;
        }
        iSprite_ = iSprite_ == kSprWalk1 ? kSprWalk0 : kSprWalk1;
        setSprite(xPoo_, yPoo_, iSprite_);
        checkPooTurn();
        aniFrameDown42();
        land_496F(1);
        procPooTouchX(-(xDirection_ * 6 - xPoo_), xDirection_ * 6 + xPoo_, 1);
        break;
    case kStateHandstand1:
        rand_A82A = rand() % 2;
        iAniFrames_ = rand() % 4 + 4;
        if (rand_A82A != 0) {
            iSprite_ = kSprHandstFront1;
        } else {
            iSprite_ = kSprHandst1;
        }
        setSprite(xPoo_, yPoo_, iSprite_);
        pooState_ = 14;
        break;
    case 14:
        if (counter_A83A++ < 3) {
            break;
        }
        counter_A83A = 0;
        if (word_A82C == 0) {
            xPoo_ -= xDirection_ * 6;
        }
        if (rand_A82A != 0) {
            iSprite_ = iSprite_ == kSprHandstFront1 ? kSprHandstFront2 : kSprHandstFront1;
        } else {
            iSprite_ = iSprite_ == kSprHandst1 ? kSprHandst2 : kSprHandst1;
        }
        setSprite(xPoo_, yPoo_, iSprite_);
        checkPooTurn();
        aniFrameDown42();
        land_496F(1);
        break;
    case kStateSex1:
        rand_A82A = rand() % 2;
        iAniFrames_ = rand() % 3 + 3;
        if (rand_A82A != 0) {
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
        if (rand_A82A != 0) {
            iSprite_ = iSprite_ == kSprSexFront1 ? kSprSexFront2 : kSprSexFront1;
        } else {
            iSprite_ = iSprite_ == kSprSexSide1 ? kSprSexSide2 : kSprSexSide1;
        }
        setSprite(xPoo_, yPoo_, iSprite_);
        checkPooTurn();
        aniFrameDown42();
        land_496F(0);
        break;
    case kStateSleep1:
        iSprite_ = kSprWinkOpen;
        setSprite(xPoo_, yPoo_, iSprite_);
        pooState_ = 18;
        break;
    case 18:
        if (counter_A83A++ < 1) {
            break;
        }
        counter_A83A = 0;
        iSprite_ += 1;
        setSprite(xPoo_, yPoo_, iSprite_);
        if (iSprite_ == kSprWinkClosed) {
            iSprite_ = kSprSleep1;
            pooState_ = 19;
            iAniFrames_ = rand() % 8 + 8;
        }
        land_496F(0);
        break;
    case 19:
        if (counter_A83A++ < 4) {
            break;
        }
        counter_A83A = 0;
        iSprite_ = iSprite_ == kSprSleep1 ? kSprSleep2 : kSprSleep1;
        setSprite(xPoo_, yPoo_, iSprite_);
        aniFrameDown42();
        land_496F(0);
        break;
    case kStateWink1:
        rand_A82A = rand() % 3;
        if (rand_A82A == 0) {
            iSprite_ = kSprWinkOpen;
        } else if (rand_A82A == 1) {
            iSprite_ = kSprSit1;
        } else {
            iSprite_ = kSprLookUp1;
        }
        setSprite(xPoo_, yPoo_, iSprite_);
        pooState_ = 21;
        counter_A83A = rand() % 15 + rand() % 15;
        land_496F(0);
        break;
    case 21:
        land_496F(0);
        if (counter_A83A-- > 0) {
            break;
        }
        pooState_ = 22;
        iAniFrames_ = 0;
        break;
    case 22:
        iSprite_ = aniWinks_[rand_A82A][iAniFrames_];
        setSprite(xPoo_, yPoo_, iSprite_);
        iAniFrames_ += 1;
        if (iAniFrames_ > 7) {
            pooState_ = 23;
            counter_A83A = rand() % 15 + rand() % 15;
        }
        land_496F(0);
        break;
    case 23:
        land_496F(0);
        if (counter_A83A-- > 0) {
            break;
        }
        pooState_ = kStateNormal1;
        break;
    case kStateTurn1:
        if (counter_A83A++ < 1) {
            break;
        }
        counter_A83A = 0;
        iSprite_ = kSprWalk0;
        setSprite(xPoo_, yPoo_, iSprite_);
        if ((rand() & 1) != 0) {
            rand_A82A = 0;
        } else {
            rand_A82A = 1;
        }
        pooState_ = 25;
        iAniFrames_ = 0;
        break;
    case 25:
        if (counter_A83A++ < 1) {
            break;
        }
        counter_A83A = 0;
        if (rand_A82A != 0) {
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
            pooState_ = 26;
        }
        land_496F(0);
        break;
    case 26:
        if (counter_A83A++ < 1) {
            break;
        }
        counter_A83A = 0;
        iSprite_ = kSprWalk0;
        setSprite(xPoo_, yPoo_, iSprite_);
        pooState_ = kStateNormal1;
        land_496F(0);
        break;
    case 27:
        dyPoo_ = -10;
        dxPoo_ = xDirection_ * 8;
        yPooStored_ = yPoo_;
        index_A848 = 0;
        pooState_ = 28;
    case 28:
        xPoo_ += dxPoo_;
        yPoo_ += dyPoo_;
        dyPoo_ += 2;
        iSprite_ = aniHit_[index_A848];
        index_A848 += 1;
        setSprite(xPoo_, yPoo_, iSprite_);
        if (iSprite_ == kSprHitRollFirst + 1) {
            fallt_A842 = 3;
            pooState_ = 99;
            break;
        }
        break;
    case 29:
        counter_A83A = 0;
        iAniFrames_ = 0;
        rand_A82A = 0;
        if ((rand() & 7) == 0) {
            rand_A82A = 1;
        }
        if (rand() % 5 == 0) {
            rand_A82A = 2;
        }
        pooState_ = 32;
        if (rand_A82A != 0) {
            pooState_ = 34;
        }
        goto fallThrough;
    case kStateHit1:
        if (bool_A7FC != 0) {
            pooState_ = 27;
            goto fallThrough;
        } else {
            pooState_ = kStateTurn1;
            goto fallThrough;
        }
        counter_A83A = 0;
        iAniFrames_ = 0;
        rand_A82A = 0;
        if ((rand() & 7) == 0) {
            rand_A82A = 1;
        }
        if (rand() % 5 == 0) {
            rand_A82A = 2;
        }
        pooState_ = 31;
    case 31:
        procPooTouchX(xDirection_ * 10 + xPoo_, xPoo_, 2);
        if (pooState_ == kStateHit1) {
            if (iAniFrames_ != 0) {
                dy_A844 -= aniHit_[iAniFrames_ + 9];
            }
            break;
        }
        iSprite_ = aniHit_[iAniFrames_];
        setSprite(xPoo_, yPoo_ - aniHit_[iAniFrames_ + 10], iSprite_);
        iAniFrames_ += 1;
        if (rand_A82A != 0 && iSprite_ == kSprHitFallen) {
            if (dy_A844 != 0) {
                yPoo_ -= dy_A844;
                xPoo_ += xDirection_ * 10;
                setSprite(xPoo_, yPoo_, iSprite_);
            }
            word_A846 = 3;
            pooState_ = 34;
            break;
        }
        if (iAniFrames_ > 8) {
            pooState_ = 32;
            break;
        }
        xPoo_ += xDirection_ * 10;
        break;
    case 32:
        land_496F(0);
        if (counter_A83A++ < 8) {
            break;
        }
        counter_A83A = 0;
        xDirection_ = -xDirection_;
        iSprite_ = kSprLaySide;
        setSprite(xPoo_, yPoo_, iSprite_);
        pooState_ = 33;
        break;
    case 33:
        land_496F(0);
        if (counter_A83A++ < 15) {
            break;
        }
        counter_A83A = 0;
        pooState_ = kStateNormal1;
        break;
    case 34:
        xPoo_ += xDirection_ * 8;
        if (iSprite_ == kSprHitRollLast) {
            iSprite_ = kSprHitRollFirst;
        } else {
            iSprite_ += 1;
        }
        setSprite(xPoo_, yPoo_, iSprite_);
        if (rand_A82A == 2 && iSprite_ == kSprHitRollLast) {
            pooState_ = kStateSpin1;
            break;
        }
        if (xPoo_ > screenWidth_ || xPoo_ < -40) {
            pooState_ = kStateNormal1;
        }
        procPooTouchX(xDirection_ * 8 + xPoo_, -(xDirection_ * 8 - xPoo_), 2);
        if (pooState_ == kStateHit1) {
            if (word_A846-- > 0) {
                xDirection_ = -xDirection_;
                pooState_ = 34;
            } else {
                pooState_ = 34;
            }
        }
        land_496F(2);
        break;
    case kStatePiss1:
        if (counter_A83A++ < 1) {
            break;
        }
        counter_A83A = 0;
        iSprite_ = kSprWalk0;
        setSprite(xPoo_, yPoo_, iSprite_);
        pooState_ = 37;
        iAniFrames_ = 0;
        break;
    case 36:
        break;
    case 37:
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
            pooState_ = 38;
        }
        land_496F(0);
        break;
    case 38:
        if (counter_A83A++ < 1) {
            break;
        }
        counter_A83A = 0;
        setSprite(xPoo_, yPoo_, iSprite_);
        iSprite_ += 1;
        if (iSprite_ > kSprPissReady) {
            iAniFrames_ = 0;
            pooState_ = 39;
            break;
        }
        land_496F(0);
        break;
    case 39:
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
        if (iAniFrames_ <= 8 || iAniFrames_ >= 12 && iAniFrames_ <= 12) {
            iSprite_ = iSprite_ == kSprPiss1 ? kSprPiss2 : kSprPiss1;
        } else {
            iSprite_ = kSprPissReady;
        }
        setSprite(xPoo_, yPoo_, iSprite_);
        if (iAniFrames_++ > 15) {
            pooState_ = 40;
            iSprite_ = kSprPissReady;
        }
        land_496F(0);
        break;
    case 40:
        if (counter_A83A++ < 1) {
            break;
        }
        counter_A83A = 0;
        setSprite(xPoo_, yPoo_, iSprite_);
        if (--iSprite_ < 103) {
            iAniFrames_ = 0;
            pooState_ = 41;
            break;
        }
        land_496F(0);
        break;
    case 41:
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
            pooState_ = 42;
        }
        land_496F(0);
        break;
    case 42:
        land_496F(0);
        if (counter_A83A++ < 1) {
            break;
        }
        counter_A83A = 0;
        iSprite_ = kSprWalk0;
        setSprite(xPoo_, yPoo_, iSprite_);
        pooState_ = kStateNormal1;
        iAniFrames_ = 0;
        break;
    case kStateYawn1:
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
            pooState_ = kStateNormal1;
            break;
        }
        setSprite(xPoo_, yPoo_, iSprite_);
        land_496F(0);
        break;
    case kStateBaa1:
        pooSound(IDW_BAA, 0U, 0);
        iAniFrames_ = 0;
        pooState_ = 46;
    case kStateBaaing:
        if (counter_A83A++ < 0) {
            break;
        }
        counter_A83A = 0;
        iSprite_ = aniBaa_[iAniFrames_];
        iAniFrames_ += 1;
        if (iSprite_ == kAniEnd) {
            pooState_ = kStateNormal1;
            break;
        }
        setSprite(xPoo_, yPoo_, iSprite_);
        land_496F(0);
        break;
    case kStateSneeze1:
        iAniFrames_ = 0;
        pooState_ = 48;
    case 48:
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
            pooState_ = kStateNormal1;
            break;
        }
        setSprite(xPoo_, yPoo_, iSprite_);
        land_496F(0);
        break;
    case kStateAmazed1:
        iAniFrames_ = 0;
        pooState_ = 50;
    case 50:
        if (counter_A83A++ < 1) {
            break;
        }
        counter_A83A = 0;
        iSprite_ = aniAmazed_[iAniFrames_];
        iAniFrames_ += 1;
        if (iSprite_ == kAniEnd) {
            pooState_ = kStateNormal1;
            break;
        }
        setSprite(xPoo_, yPoo_, iSprite_);
        land_496F(0);
        break;
    case kStateScary1:
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
            pooState_ = kStateNormal1;
        }
        setSprite(xPoo_, yPoo_, iSprite_);
        land_496F(0);
        break;
    case kStateEat1:
        iAniFrames_ = 0;
        pooState_ = 54;
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
        word_C0AE = 1;
        sub_2B01(hOtherPooWnd_[kISubPooHwnd], hPooWnd_);
        break;
    case 54:
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
            pooState_ = kStateNormal1;
            break;
        }
        setSprite(xPoo_, yPoo_, iSprite_);
        break;
    case 55:
        break;
        if (counter_A83A++ < 1) {
            break;
        }
        counter_A83A = 0;
        xPoo_ -= xDirection_ * 6;
        iSprite_ = kSprWalk1;
        setSprite(xPoo_, yPoo_, iSprite_);
        checkPooTurn();
        aniFrameDown42();
        land_496F(1);
        procPooTouchX(-(xDirection_ * 6 - xPoo_), xDirection_ * 6 + xPoo_, 1);
        pooState_ = 54;
        break;
    case kStateFeed1:
        iAniFrames_ = 0;
        pooState_ = 57;
    case 57:
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
            pooState_ = 42;
            break;
        }
        setSprite(xPoo_, yPoo_, iSprite_);
        land_496F(0);
        break;
    case kStateSit1:
        if (counter_A83A++ < 1) {
            break;
        }
        counter_A83A = 0;
        iSprite_ = kSprWalk0;
        setSprite(xPoo_, yPoo_, iSprite_);
        pooState_ = 59;
        iAniFrames_ = 0;
        break;
    case 59:
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
            pooState_ = 60;
            iAniFrames_ = 0;
        }
        setSprite(xPoo_, yPoo_, iSprite_);
        land_496F(0);
        break;
    case 60:
        if (counter_A83A++ < 0) {
            break;
        }
        counter_A83A = 0;
        iSprite_ = aniWinks_[kWinkSitFront][iAniFrames_];
        setSprite(xPoo_, yPoo_, iSprite_);
        iAniFrames_ += 1;
        if (iAniFrames_ > 7) {
            iAniFrames_ = 0;
            pooState_ = 61;
            counter_A83A = -5;
        }
        land_496F(0);
        break;
    case 61:
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
            pooState_ = 42;
        }
        land_496F(0);
        break;
    case 64:
        break;
    case kStateRoll1:
        iSprite_ = kSprWalk0;
        setSprite(xPoo_, yPoo_, iSprite_);
        iAniFrames_ = 0;
        pooState_ = 66;
        break;
    case 66:
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
            pooState_ = 67;
            word_A828 = (rand() % 4 + 4) * 8;
            iAniFrames_ = 0;
            break;
        }
        land_496F(0);
        break;
    case 67:
        if (--iAniFrames_ < 0) {
            iAniFrames_ = 79;
        }
        xPoo_ -= xDirection_ * 8;
        iSprite_ = aniRoll_[iAniFrames_ % 8];
        setSprite(xPoo_, yPoo_, iSprite_);
        if (xDirection_ > 0 && xPoo_ < 0) {
            pooState_ = kStateHit1;
        }
        if (xDirection_ < 0 && screenWidth_ - sprites_[iSprite_].width < xPoo_) {
            pooState_ = kStateHit1;
        }
        if (--word_A828 <= 0) {
            pooState_ = 68;
            iAniFrames_ = 0;
        }
        land_496F(2);
        procPooTouchX(-(xDirection_ * 8 - xPoo_), xDirection_ * 8 + xPoo_, 2);
        break;
    case 68:
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
            pooState_ = kStateNormal1;
            break;
        }
        land_496F(0);
        break;
    case kStateBlush1:
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
            pooState_ = kStateNormal1;
            break;
        }
        setSprite(xPoo_, yPoo_, iSprite_);
        iAniFrames_ += 1;
        land_496F(0);
        break;
    case kStateSlip1:
        iAniFrames_ = rand() % 8 + 8;
        word_A828 = iAniFrames_;
        iSprite_ = kSprClimb1;
        if (xDirection_ > 0) {
            iSprite_ = kSprBackL;
        } else {
            iSprite_ = kSprBackR;
        }
        setSprite(xPoo_, yPoo_, iSprite_);
        pooState_ = 76;
        break;
    case 76:
        if (counter_A83A++ < 1) {
            break;
        }
        counter_A83A = 0;
        iSprite_ = kSprBack;
        setSprite(xPoo_, yPoo_, iSprite_);
        pooState_ = 77;
        break;
    case 77:
        if (counter_A83A++ < 2) {
            break;
        }
        counter_A83A = 0;
        iSprite_ = iSprite_ == kSprClimb1 ? kSprClimb2 : kSprClimb1;
        yPoo_ -= 8;
        setSprite(xPoo_, yPoo_, iSprite_);
        if (iAniFrames_-- <= 0) {
            iAniFrames_ = word_A828;
            pooState_ = 78;
        }
        break;
    case 78:
        iSprite_ = kSprClimbSlip;
        yPoo_ += 8;
        setSprite(xPoo_, yPoo_, iSprite_);
        if (iAniFrames_-- <= 0) {
            pooState_ = 79;
        }
        break;
    case 79:
        if (counter_A83A++ < 10) {
            break;
        }
        counter_A83A = 0;
        pooState_ = 80;
        iAniFrames_ = 3;
        break;
    case 80:
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
            pooState_ = kStateNormal1;
            break;
        }
        setSprite(xPoo_, yPoo_, iSprite_);
        break;
    case kStateSpin1:
        counter_A83A = 0;
        iAniFrames_ = 0;
        pooState_ = 70;
    case 70:
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
            pooState_ = 71;
        }
        land_496F(0);
        break;
    case 71:
        land_496F(0);
        if (counter_A83A++ < 14) {
            break;
        }
        counter_A83A = 0;
        iSprite_ = kSprSpinFall;
        setSprite(xPoo_, yPoo_, iSprite_);
        pooState_ = 72;
        break;
    case 72:
        land_496F(0);
        if (counter_A83A++ < 30) {
            break;
        }
        counter_A83A = 0;
        iSprite_ = kSprWalk0;
        setSprite(xPoo_, yPoo_, iSprite_);
        pooState_ = kStateNormal1;
        break;
    case 73:
        iAniFrames_ = 0;
        pooState_ = 74;
    case 74:
        if (counter_A83A++ < 2) {
            break;
        }
        counter_A83A = 0;
        iSprite_ = aniRollSide_[iAniFrames_];
        iAniFrames_ += 1;
        if (iSprite_ == kAniEnd) {
            pooState_ = kStateNormal1;
            break;
        }
        setSprite(xPoo_, yPoo_, iSprite_);
        land_496F(0);
        break;
    case 81:
        iSprite_ = kSprRun1;
        setSprite(xPoo_, yPoo_, iSprite_);
        pooState_ = 82;
        break;
    case 82:
        counter_A83A = 0;
        iSprite_ = iSprite_ == kSprRun1 ? kSprRun2 : kSprRun1;
        setSprite(xPoo_, yPoo_, iSprite_);
        break;
    case 83:
        break;
    case 84:
        pooState_ = 0;
        break;
    case 85:
        hWnd_A81C = GetActiveWindow();
        if (hWnd_A81C == hPooWnd_ || hWnd_A81C == hOtherPooWnd_[kISubPooHwnd] || hWnd_A81C == NULL || sub_39D6(hWnd_A81C)) {
            pooState_ = kStateNormal1;
            break;
        }
        getWndRectSp(hWnd_A81C, &stru_A81E);
        if (stru_A81E.top < 10) {
            pooState_ = kStateNormal1;
            break;
        }
        if (xDirection_ > 0 && stru_A81E.right < xPoo_ && stru_A81E.top < yPoo_ && yPoo_ + 40 < stru_A81E.bottom || xDirection_ < 0 && xPoo_ + 40 < stru_A81E.left && stru_A81E.top < yPoo_ && yPoo_ + 40 < stru_A81E.bottom) {
            pooState_ = 87;
            break;
        }
        word_A83C = (rand() % stru_A81E.right - stru_A81E.left) / 3 + (stru_A81E.right - stru_A81E.left) / 2 + stru_A81E.left - 20;
        word_A83E = stru_A81E.top - 40;
        if (screenWidth_ / 2 - 20 > xPoo_) {
            xDirection_ = 1;
        } else {
            xDirection_ = -1;
        }
        iSprite_ = kSprRun1;
        setSprite(xPoo_, yPoo_, iSprite_);
        pooState_ = 86;
        break;
    case 86:
        if (counter_A83A++ < 1) {
            break;
        }
        counter_A83A = 0;
        xPoo_ -= xDirection_ * 16;
        iSprite_ = iSprite_ == kSprRun1 ? kSprRun2 : kSprRun1;
        setSprite(xPoo_, yPoo_, iSprite_);
        if (xPoo_ < -40 || xPoo_ > screenWidth_) {
            if (!isLandableWnd(hWnd_A81C)) {
                pooState_ = kStateNormal1;
                break;
            }
            if (rand() % 3 == 0) {
                pooState_ = 3;
                goto fallThrough;
            }
            word_A840 = 0;
            pooState_ = 92;
            bool_A7FC = 1;
            xPoo_ = word_A83C;
            yPoo_ = -40;
            dyPoo_ = 0;
            dxPoo_ = 0;
            fallt_A842 = rand() % 2;
            if (fallt_A842 != 0) {
                dxPoo_ = -(xDirection_ * 3);
            }
            sub_2B01(hPooWnd_, hWnd_A81C);
        }
        break;
    case 87:
        sub_2B01(hPooWnd_, hWnd_A81C);
        if (xDirection_ > 0) {
            word_A83C = stru_A81E.right;
            word_A83E = stru_A81E.top;
        } else {
            word_A83C = stru_A81E.left - 40;
            word_A83E = stru_A81E.top;
        }
        pooState_ = 88;
    case 88:
        if (counter_A83A++ < 1) {
            break;
        }
        counter_A83A = 0;
        xPoo_ -= xDirection_ * 16;
        iSprite_ = iSprite_ == kSprRun1 ? kSprRun2 : kSprRun1;
        if (word_A83C >= xPoo_ && xDirection_ > 0 || word_A83C <= xPoo_ && xDirection_ < 0) {
            if (!isLandableWnd(hWnd_A81C)) {
                pooState_ = kStateNormal1;
                break;
            }
            getWndRectSp(hWnd_A81C, &var_10);
            if (var_10.left == stru_A81E.left && var_10.right == stru_A81E.right && var_10.top < yPoo_ && yPoo_ + 40 < var_10.bottom) {
                word_A83E = var_10.top - 12;
                bool_A7FC = 1;
                xPoo_ = word_A83C;
                iSprite_ = kSprJumpUp;
                pooState_ = 89;
                break;
            } else {
                pooState_ = kStateNormal1;
                break;
            }
        }
        setSprite(xPoo_, yPoo_, iSprite_);
        break;
    case 89:
        if (counter_A83A++ < 1) {
            break;
        }
        counter_A83A = 0;
        setSprite(xPoo_, yPoo_, iSprite_);
        yPoo_ -= 6;
        iSprite_ = iSprite_ == kSprWalkSide1 ? kSprWalkSide2 : kSprWalkSide1;
        if (word_A83E >= yPoo_) {
            pooState_ = 90;
            break;
        }
        sub_4B3B();
        break;
    case 90:
        if (counter_A83A++ < 2) {
            break;
        }
        counter_A83A = 0;
        xPoo_ -= xDirection_ * 8;
        yPoo_ = word_A83E - 20;
        iSprite_ = kSprClimbSide;
        setSprite(xPoo_, yPoo_, iSprite_);
        pooState_ = 91;
        break;
    case 91:
        if (counter_A83A++ < 2) {
            break;
        }
        counter_A83A = 0;
        xPoo_ += xDirection_ * -24;
        yPoo_ -= 8;
        iSprite_ = kSprWalk0;
        setSprite(xPoo_, yPoo_, iSprite_);
        pooState_ = kStateWalk1;
        break;
    case 92:
        dyPoo_ += 4;
        yPooStored_ = yPoo_;
        xPoo_ += dxPoo_;
        yPoo_ += dyPoo_;
        if ((var_4 = sub_419E(hWnd_A81C, sprites_[iSprite_].height + yPoo_, sprites_[iSprite_].height + yPooStored_,
                              xPoo_, sprites_[iSprite_].width + xPoo_)) != 0) {
            if (var_4 == -1) {
                setSprite(xPoo_, yPoo_, iSprite_);
                pooState_ = 0;
                break;
            }
            yPoo_ = var_4 - sprites_[iSprite_].height;
            if (dyPoo_ < 64 && word_A840 == 0 || dyPoo_ < 8) {
                SetWindowPos(hPooWnd_, hWnd_A81C, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE);
                word_A840 = 0;
                iAniFrames_ = 0;
                pooState_ = 93;
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
                word_A840 = 1;
            }
        }
        if (fallt_A842 != 0) {
            iSprite_ = iSprite_ == kSprRun1 ? kSprRun2 : kSprRun1;
        } else {
            iSprite_ = kSprHangBack1;
        }
        setSprite(xPoo_, yPoo_, iSprite_);
        break;
    case 93:
        if (counter_A83A++ < 1) {
            break;
        }
        counter_A83A = 0;
        if (fallt_A842 != 0) {
            pooState_ = kStateWalk1;
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
            pooState_ = kStateWalk1;
        }
        break;
    case 94:
        bool_A7FC = 1;
        dyPoo_ = 0;
        dxPoo_ = -(xDirection_ * 8);
        fallt_A842 = 1;
        pooState_ = 99;
        goto fallThrough;
    case 95:
        bool_A7FC = 1;
        dyPoo_ = 0;
        dxPoo_ = -(xDirection_ * 3);
        fallt_A842 = 1;
        pooState_ = 99;
        goto fallThrough;
    case kStateFall1:
        bool_A7FC = 1;
        dyPoo_ = 0;
        dxPoo_ = 0;
        fallt_A842 = 0;
        pooState_ = 99;
        goto fallThrough;
    case 97:
        bool_A7FC = 1;
        dyPoo_ = 0;
        dxPoo_ = 0;
        fallt_A842 = 1;
        pooState_ = 99;
        goto fallThrough;
    case 98:
        bool_A7FC = 1;
        dyPoo_ = 0;
        dxPoo_ = 0;
        fallt_A842 = 2;
        pooState_ = 99;
        goto fallThrough;
    case 99:
        updateVWnds();
        dyPoo_ += 4;
        yPooStored_ = yPoo_;
        xPoo_ += dxPoo_;
        yPoo_ += dyPoo_;
        if (yPooStored_ > screenHeight_) {
            setSprite(xPoo_, yPoo_, iSprite_);
            pooState_ = 0;
            break;
        }
        if ((var_4 = sub_408C(&hWnd_A81C, sprites_[iSprite_].height + yPoo_, sprites_[iSprite_].height + yPooStored_, xPoo_, sprites_[iSprite_].width + xPoo_)) != 0) {
            if (!isLandableWnd(hWnd_A81C)) {
                setSprite(xPoo_, yPoo_, iSprite_);
                pooState_ = 0;
                break;
            }
            getWndRectSp(hWnd_A81C, &stru_A81E);
            yPoo_ = var_4 - sprites_[iSprite_].height;
            if (fallt_A842 == 3) {
                iSprite_ = kSprHitFallen;
                setSprite(xPoo_, yPoo_, iSprite_);
                pooState_ = 29;
                break;
            }
            if (dyPoo_ < 64 && word_A840 == 0 || dyPoo_ < 8) {
                if (hWnd_A81C != NULL) {
                    SetWindowPos(hPooWnd_, hWnd_A81C, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE);
                }
                word_A840 = 0;
                iAniFrames_ = 0;
                pooState_ = 100;
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
                if (fallt_A842 == 2) {
                    if (dyPoo_ < 36) {
                        iSprite_ = kSprFallSide2;
                    } else {
                        iSprite_ = kSprFallenSide;
                    }
                }
                setSprite(xPoo_, yPoo_, iSprite_);
                break;
            } else {
                if ((rand() & 7) == 0 && word_A840 == 0) {
                    word_A840 = 0;
                    iAniFrames_ = 0;
                    pooState_ = 100;
                    iSprite_ = kSprFallen;
                    counter_A83A = -12;
                    if (fallt_A842 == 2) {
                        iSprite_ = kSprFallenSide;
                    }
                    setSprite(xPoo_, yPoo_, iSprite_);
                    break;
                }
                dyPoo_ = dyPoo_ * 2 / -3;
                word_A840 = 1;
            }
        }
        if (fallt_A842 == 2) {
            iSprite_ = iSprite_ == kSprFallSide1 ? kSprFallSide2 : kSprFallSide1;
        } else if (fallt_A842 == 1) {
            iSprite_ = iSprite_ == kSprRun1 ? kSprRun2 : kSprRun1;
        } else if (fallt_A842 == 0) {
            iSprite_ = kSprHangBack1;
        } else {
            iSprite_ = aniHit_[index_A848];
            index_A848 += 1;
            if (iSprite_ == kSprHitFallen) {
                index_A848 -= 1;
            }
        }
        if (fallt_A842 == 3 && getPooCollisionX(xPoo_, xPoo_ - dxPoo_) != 0) {
            xDirection_ = -xDirection_;
            pooState_ = kStateHit1;
            break;
        }
        setSprite(xPoo_, yPoo_, iSprite_);
        break;
    case 100:
        if (counter_A83A++ < 1) {
            break;
        }
        counter_A83A = 0;
        if (fallt_A842 == 1) {
            pooState_ = kStateWalk1;
            iSprite_ = kSprWalk1;
            break;
        }
        if (fallt_A842 == 2) {
            iAniFrames_ = 0;
            pooState_ = 101;
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
            pooState_ = kStateWalk1;
        }
        break;
    case 101:
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
            pooState_ = kStateWalk1;
        }
        break;
    case 102:
        stopPlaySound();
        iAniFrames_ = 6;
        iSprite_ = kSprWalk0;
        rand_A82A = 0;
        if (rand() % 3 == 0) {
            rand_A82A = 1;
        }
        pooState_ = 103;
    case 103:
        if (rand_A82A != 0) {
            iSprite_ = iSprite_ == kSprAmazed1 ? kSprAmazed2 : kSprAmazed1;
        } else {
            iSprite_ = iSprite_ == kSprRun1 ? kSprRun2 : kSprRun1;
        }
        setSprite(xPoo_, yPoo_, iSprite_);
        if (iAniFrames_-- <= 0) {
            pooState_ = 97;
        }
        break;
    case 104:
        fallt_A842 = 0;
        pooState_ = 106;
        goto fallThrough;
    case 105:
        fallt_A842 = 1;
        pooState_ = 106;
        goto fallThrough;
    case 106:
        if (fallt_A842 == 0) {
            var_14.x = xPoo_;
            var_14.y = yPoo_ + 39;
            *(HWND *)&var_10 = WindowFromPoint(var_14);
            var_14.x = xPoo_ + 39;
            var_8 = WindowFromPoint(var_14);
            if (*(HWND *)&var_10 == hPooWnd_ && var_8 == hPooWnd_) {
                putWndToTop(hPooWnd_);
            } else if (*(HWND *)&var_10 == hPooWnd_) {
                sub_2B01(hPooWnd_, var_8);
            } else {
                sub_2B01(hPooWnd_, *(HWND *)&var_10);
            }
            iSprite_ = kSprDownFront1;
        } else {
            iSprite_ = kSprDown1;
        }
        setSprite(xPoo_, yPoo_, iSprite_);
        pooState_ = 107;
        iAniFrames_ = 0;
        break;
    case 107:
        iSprite_ = aniWinks_[4 - fallt_A842][iAniFrames_];
        setSprite(xPoo_, yPoo_, iSprite_);
        iAniFrames_ += 1;
        if (iAniFrames_ > 7) {
            if (fallt_A842 != 0) {
                if ((rand() & 1) == 0) {
                    pooState_ = 111;
                } else {
                    pooState_ = 109;
                }
            } else {
                if ((rand() & 1) == 0) {
                    pooState_ = 111;
                } else {
                    pooState_ = 108;
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
        pooState_ = kStateNormal1;
        break;
    case 109:
        dxPoo_ = -(xDirection_ * 14);
        iSprite_ = kSprJumpAir;
        xPoo_ += dxPoo_;
        setSprite(xPoo_, yPoo_, iSprite_);
        pooState_ = 95;
        iAniFrames_ = 0;
        break;
    case 110:
        xPoo_ += dxPoo_;
        dxPoo_ += xDirection_;
        setSprite(xPoo_, yPoo_, iSprite_);
        if (iAniFrames_++ > 3) {
            pooState_ = 95;
        }
        break;
    case 111:
        if (fallt_A842 != 0) {
            xPoo_ += xDirection_ * -26;
            yPoo_ += 35;
            xDirection_ = -xDirection_;
        } else {
            rand_A82A = rand() % kHangCount;
            if (rand_A82A != 0) {
                yPoo_ += 36;
            } else {
                yPoo_ += 20;
            }
        }
        iAniFrames_ = 0;
        pooState_ = 112;
    case 112:
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
        if (fallt_A842 != 0) {
            iSprite_ = iSprite_ == kSprFallSide1 ? kSprFallSide2 : kSprFallSide1;
        } else {
            iSprite_ = aniHangs_[rand_A82A][iAniFrames_ % 4];
        }
        setSprite(xPoo_, yPoo_, iSprite_);
        iAniFrames_ += 1;
        if (iAniFrames_ > 12) {
            if (fallt_A842 != 0) {
                pooState_ = 98;
            } else {
                pooState_ = kStateFall1;
            }
        }
        break;
    case 113:
        word_CA76 = 1;
        iSprite_ = kSprWinkOpen;
        setSprite(xPoo_, yPoo_, iSprite_);
        pooState_ = 114;
        break;
    case 114:
        if (counter_A83A++ < 1) {
            break;
        }
        counter_A83A = 0;
        iSprite_ += 1;
        setSprite(xPoo_, yPoo_, iSprite_);
        if (iSprite_ == kSprWinkClosed) {
            iSprite_ = kSprSleep1;
            pooState_ = 115;
        }
        land_496F(0);
        break;
    case 115:
        land_496F(0);
        if (counter_A83A++ < 4) {
            break;
        }
        counter_A83A = 0;
        iSprite_ = iSprite_ == kSprSleep1 ? kSprSleep2 : kSprSleep1;
        setSprite(xPoo_, yPoo_, iSprite_);
        break;
    case kStateMerry11:
        xPoo_ = screenWidth_;
        yPoo_ = screenHeight_ * 7 / 8;
        iSprite_ = kSprRun1;
        xDirection_ = 1;
        setSprite(xPoo_, yPoo_, iSprite_);
        pooState_ = 117;
        break;
    case 117:
        if (counter_A83A++ < 1) {
            break;
        }
        counter_A83A = 0;
        xPoo_ -= xDirection_ * 16;
        iSprite_ = iSprite_ == kSprRun1 ? kSprRun2 : kSprRun1;
        setSprite(xPoo_, yPoo_, iSprite_);
        if (screenWidth_ / 2 - 20 >= xPoo_) {
            pooState_ = 118;
        }
        break;
    case 118:
        showSubPoo();
        xDirectionSub_ = -1;
        xSubPoo_ = -40;
        ySubPoo_ = screenHeight_ / 8;
        iSubSprite_ = kSprWifeRun1;
        iAniFrames_ = 0;
        pooState_ = 119;
        word_C0AE = 0;
        putWndToTop(hPooWnd_);
        putWndToTop(hOtherPooWnd_[kISubPooHwnd]);
        break;
    case 119:
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
            pooState_ = 120;
        }
        break;
    case 120:
        if (counter_A83A++ < 1) {
            break;
        }
        counter_A83A = 0;
        xPoo_ -= xDirection_ * 16;
        iSprite_ = iSprite_ == kSprRun1 ? kSprRun2 : kSprRun1;
        setSprite(xPoo_, yPoo_, iSprite_);
        if (xPoo_ > screenWidth_) {
            pooState_ = kStateNormal1;
        }
        break;
    case kStateMerry21:
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
        pooState_ = 122;
        word_C0AE = 0;
        putWndToTop(hPooWnd_);
        putWndToTop(hOtherPooWnd_[kISubPooHwnd]);
        break;
    case 122:
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
            pooState_ = 123;
        } else {
            setSprite(xPoo_, yPoo_, iSprite_);
            setSubSprite(xSubPoo_, ySubPoo_, iSubSprite_);
        }
        break;
    case 123:
        if (counter_A83A++ < 3) {
            break;
        }
        counter_A83A = 0;
        iSprite_ = iAniFrames_ + kSprBlush1;
        iAniFrames_ += 1;
        setSprite(xPoo_, yPoo_, iSprite_);
        if (iAniFrames_ >= 4) {
            pooState_ = 124;
        }
        break;
    case 124:
        if (counter_A83A++ < 4) {
            break;
        }
        counter_A83A = 0;
        word_CA46 += 1;
        if (word_CA46 > 8) {
            destroySubPooWnd();
            iAniFrames_ = 0;
            pooState_ = 125;
        }
        break;
    case 125:
        iSprite_ = aniMerryBlush_[iAniFrames_];
        iAniFrames_ += 1;
        if (iSprite_ == kAniEnd) {
            pooState_ = kStateNormal1;
            break;
        }
        setSprite(xPoo_, yPoo_, iSprite_);
        break;
    case kStateMerry31:
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
        pooState_ = 127;
        word_C0AE = 0;
        putWndToTop(hPooWnd_);
        putWndToTop(hOtherPooWnd_[kISubPooHwnd]);
        break;
    case 127:
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
            pooState_ = kStateNormal1;
        } else {
            setSprite(xPoo_, yPoo_, iSprite_);
            setSubSprite(xSubPoo_, ySubPoo_, iSubSprite_);
        }
        break;
    case kStateUFO11:
        xPoo_ = screenWidth_;
        yPoo_ = screenHeight_ * 7 / 8;
        iSprite_ = kSprRun1;
        xDirection_ = 1;
        setSprite(xPoo_, yPoo_, iSprite_);
        pooState_ = 129;
        break;
    case 129:
        if (counter_A83A++ < 1) {
            break;
        }
        counter_A83A = 0;
        xPoo_ -= xDirection_ * 16;
        iSprite_ = iSprite_ == kSprRun1 ? kSprRun2 : kSprRun1;
        setSprite(xPoo_, yPoo_, iSprite_);
        if (screenWidth_ / 2 - 20 >= xPoo_) {
            pooState_ = 130;
        }
        break;
    case 130:
        showSubPoo();
        xDirectionSub_ = -1;
        xSubPoo_ = -40;
        ySubPoo_ = screenHeight_ / 8;
        iSubSprite_ = kSprUfoFirst;
        iAniFrames_ = 0;
        pooState_ = 131;
        word_C0AE = 0;
        putWndToTop(hPooWnd_);
        putWndToTop(hOtherPooWnd_[kISubPooHwnd]);
        break;
    case 131:
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
            pooState_ = 132;
        }
        setSubSprite(xSubPoo_, ySubPoo_, iSubSprite_);
        break;
    case 132:
        iSprite_ = kSprLookUp1;
        setSprite(xPoo_, yPoo_, iSprite_);
        height_CA5C += 40;
        if (yPoo_ - ySubPoo_ - 40 <= height_CA5C) {
            height_CA5C = yPoo_ - ySubPoo_ - 40;
            height_CA5C -= 20;
            pooState_ = 133;
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
    case 133:
        height_CA5C -= 20;
        if (height_CA5C <= 0) {
            height_CA5C = 0;
            yPoo_ = ySubPoo_ + 40;
            pooState_ = 134;
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
    case 134:
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
            pooState_ = kStateNormal1;
            break;
        }
        setSubSprite(xSubPoo_, ySubPoo_, iSubSprite_);
        break;
    case kStateUFO21:
        ySubPoo_ = screenHeight_ * 7 / 8;
        xDirection_ = -1;
        xPoo_ = -40;
        yPoo_ = screenHeight_ / 8;
        iSprite_ = kSprUfoFirst;
        iAniFrames_ = 0;
        pooState_ = 136;
        break;
    case 136:
        xPoo_ -= xDirection_ * 16;
        if (iSprite_ == kSprUfoLast) {
            iSprite_ = kSprUfoFirst;
        } else {
            iSprite_ += 1;
        }
        if (screenWidth_ / 2 - 20 < xPoo_) {
            xPoo_ = screenWidth_ / 2 - 20;
            iSprite_ = kSprUfoOnFirst;
            pooState_ = 137;
        }
        setSprite(xPoo_, yPoo_, iSprite_);
        break;
    case 137:
        height_CA72 += 40;
        if (ySubPoo_ - yPoo_ - 40 <= height_CA72) {
            height_CA72 = ySubPoo_ - yPoo_ - 40;
            pooState_ = 138;
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
    case 138:
        if (counter_A83A++ < 4) {
            break;
        }
        counter_A83A = 0;
        showSubPoo();
        xSubPoo_ = xPoo_;
        iSubSprite_ = kSprAlien1;
        setSubSprite(xSubPoo_, ySubPoo_, iSubSprite_);
        setSprite(xPoo_, yPoo_, iSprite_);
        pooState_ = 139;
        word_C0AE = 0;
        putWndToTop(hPooWnd_);
        putWndToTop(hOtherPooWnd_[kISubPooHwnd]);
        break;
    case 139:
        if (height_CA72 != 0) {
            height_CA72 -= 40;
            if (height_CA72 <= 0) {
                iSprite_ = kSprUfoFirst;
                height_CA72 = 0;
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
            pooState_ = 140;
        }
        setSprite(xPoo_, yPoo_, iSprite_);
        if (counter_A83A++ < 1) {
            break;
        }
        counter_A83A = 0;
        iSubSprite_ = iSubSprite_ == kSprAlien1 ? kSprAlien2 : kSprAlien1;
        setSubSprite(xSubPoo_, ySubPoo_, iSubSprite_);
        break;
    case 140:
        iSubSprite_ = kSprAlien0;
        setSubSprite(xSubPoo_, ySubPoo_, iSubSprite_);
        if (counter_A83A++ < 1) {
            break;
        }
        counter_A83A = 0;
        word_CA46 += 1;
        if (word_CA46 > 8) {
            destroySubPooWnd();
            stopPlaySound();
            pooState_ = kStateNormal1;
        }
        break;
    case 141:
        break;
    case kStateUFO31:
        xPoo_ = -80;
        yPoo_ = screenHeight_ / 8;
        setSprite(xPoo_, yPoo_, iSprite_);
        showSubPoo();
        xDirectionSub_ = -1;
        xSubPoo_ = -40;
        ySubPoo_ = screenHeight_ * 7 / 8;
        iSubSprite_ = kSprUfoFirst;
        iAniFrames_ = 0;
        pooState_ = 143;
        word_C0AE = 0;
        putWndToTop(hPooWnd_);
        putWndToTop(hOtherPooWnd_[kISubPooHwnd]);
        break;
    case 143:
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
            pooState_ = 144;
        }
        setSubSprite(xSubPoo_, ySubPoo_, iSubSprite_);
        break;
    case 144:
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
            pooState_ = 145;
        }
        break;
    case 145:
        ySubPoo_ -= 40;
        if (iSubSprite_ == kSprUfoLast) {
            iSubSprite_ = kSprUfoFirst;
        } else {
            iSubSprite_ += 1;
        }
        if (ySubPoo_ < -40) {
            destroySubPooWnd();
            stopPlaySound();
            pooState_ = kStateNormal1;
            break;
        }
        setSubSprite(xSubPoo_, ySubPoo_, iSubSprite_);
        break;
    case 146:
        break;
    case kStateBurn1:
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
        word_C0AE = 1;
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
            pooState_ = kStateNormal1;
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
void event_8FD7(int arg_0)
{
    switch (arg_0) {
    case 0:
        pooState_ = kStateNormal1;
        if (bool_A7FC != 0) {
            pooState_ = 97;
        }
        break;
    case 1:
        pooState_ = 81;
        break;
    case 2:
        pooState_ = 97;
        break;
    case 3:
        word_CA54 = 113;
        break;
    case 4:
        pooState_ = kStateFeed1;
        break;
    default:
        break;
    }
}

/* Process debug window action change. */
void debugAction(WPARAM action)
{
    height_CA72 = 0;
    stopPlaySound();
    destroySubPooWnd();
    switch (action) {
    case 0:        ///Normal
        pooState_ = kStateNormal1;
        break;
    case 1:        ///Run
        pooState_ = kStateRun1;
        break;
    case 2:        ///Walk
        pooState_ = kStateWalk1;
        break;
    case 3:        ///Handstand
        pooState_ = kStateHandstand1;
        break;
    case 4:        ///Sex
        pooState_ = kStateSex1;
        break;
    case 5:        ///Sleep
        pooState_ = kStateSleep1;
        break;
    case 6:        ///Wink
        pooState_ = kStateWink1;
        break;
    case 7:        ///Turn
        pooState_ = kStateTurn1;
        break;
    case 8:        ///Hit
        pooState_ = kStateHit1;
        break;
    case 9:        ///Piss
        pooState_ = kStatePiss1;
        break;
    case 10:        ///Yawn
        pooState_ = kStateYawn1;
        break;
    case 11:        ///Baa
        pooState_ = kStateBaa1;
        break;
    case 12:        ///Amazed
        pooState_ = kStateAmazed1;
        break;
    case 13:        ///Scary
        pooState_ = kStateScary1;
        break;
    case 14:        ///Eat
        pooState_ = kStateEat1;
        break;
    case 15:        ///Sit
        pooState_ = kStateSit1;
        break;
    case 16:        ///Sneeze
        pooState_ = kStateSneeze1;
        break;
    case 17:        ///Burn
        pooState_ = kStateBurn1;
        break;
    case 18:        ///Merry1
        pooState_ = kStateMerry11;
        break;
    case 19:        ///Merry2
        pooState_ = kStateMerry21;
        break;
    case 20:        ///Merry3
        pooState_ = kStateMerry31;
        break;
    case 21:        ///UFO1
        pooState_ = kStateUFO11;
        break;
    case 22:        ///UFO2
        pooState_ = kStateUFO21;
        break;
    case 23:        ///UFO3
        pooState_ = kStateUFO31;
        break;
    case 24:        ///Roll
        pooState_ = kStateRoll1;
        break;
    case 25:        ///Blush
        pooState_ = kStateBlush1;
        break;
    case 26:        ///Slip
        pooState_ = kStateSlip1;
        break;
    case 27:        ///Fall
        pooState_ = kStateFall1;
        break;
    case 28:        ///Jump
        pooState_ = kStateJump1;
        break;
    case 29:        ///Spin
        pooState_ = kStateSpin1;
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
    word_A85A = CreateCompatibleBitmap(hdc, 40, 40);
    if (word_A85A == NULL) {
        goto failed;
    }
    word_A85C = CreateCompatibleBitmap(hdc, 40, 40);
    if (word_A85C == NULL) {
        goto failed;
    }
    word_CA4C = 0;
    word_CA4E = 0;
    screenWidth_ = GetSystemMetrics(SM_CXSCREEN);
    screenHeight_ = GetSystemMetrics(SM_CYSCREEN);
    ReleaseDC(hWnd, hdc);
    word_CA46 = 0;
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
    DeleteObject(word_A85C);
    DeleteObject(word_A85A);
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
void sub_93DF(HWND hWnd)
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
        word_CA46 == 0 && height_CA5C == 0) {
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
            if (word_CA46 != 0) {
                if (word_CA46 == 1) {
                    SelectObject(hdc_4, curSubSpriteBmp1_);
                    SelectObject(hdc_6, word_A85C);
                    BitBlt(hdc_6, 0, 0, 40, 40,
                           hdc_4, curSubSpriteBmpX_, curSubSpriteBmpY_, SRCCOPY);
                    SelectObject(hdc_4, curSubSpriteBmp0_);
                    SelectObject(hdc_6, word_A85A);
                    BitBlt(hdc_6, 0, 0, 40, 40,
                           hdc_4, curSubSpriteBmpX_, curSubSpriteBmpY_, SRCCOPY);
                }
                SelectObject(hdc_4, word_A85C);
                SelectObject(hdc_6, sprites_[172].bitmaps[0]);
                BitBlt(hdc_4, word_CA46 - 1, word_CA46 - 1, 41 - word_CA46, 40,
                       hdc_6, sprites_[172].x, 0, SRCPAINT);
                SelectObject(hdc_4, word_A85A);
                SelectObject(hdc_6, sprites_[172].bitmaps[1]);
                BitBlt(hdc_4, word_CA46 - 1, word_CA46 - 1, 41 - word_CA46, 40,
                       hdc_6, sprites_[172].x, 0, SRCAND);
                SelectObject(hdc_6, bmp_A854);
                SelectObject(hdc_4, word_A85C);
                BitBlt(hdc_6, posx_16, posy_14, curSubSpriteBmpW_, curSubSpriteBmpH_,
                       hdc_4, 0, 0, SRCAND);
                SelectObject(hdc_4, word_A85A);
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
BOOL sub_9A49(HWND arg_0)
{
    HDC var_2;
    HDC var_4;
    RECT var_C;
    HDC var_E;
#ifdef _WIN32
    HDC screen;
#endif
    if (bool_A870 == 0) {
        return TRUE;
    }
    bool_A870 = 0;
    var_2 = GetDC(arg_0);
    SelectPalette(var_2, hPalette_, FALSE);
    RealizePalette(var_2);
    var_4 = CreateCompatibleDC(var_2);
    SelectPalette(var_4, hPalette_, FALSE);
    SelectObject(var_4, bmp_A854);
    BitBlt(var_2, 0, 0, paintSubRectW_, paintSubRectH_, var_4, 0, 0, SRCCOPY);
    if (height_CA5C != 0) {
        if (word_C0B8 == NULL) {
            word_C0B8 = CreateCompatibleBitmap(var_2, 40, screenHeight_ * 4 / 5);
            if (word_C0B8 == NULL) {
                goto loc_9CC3;
            }
        }
        if (word_C0B2 == NULL) {
            word_C0B2 = CreateCompatibleBitmap(var_2, 40, screenHeight_ * 4 / 5);
            if (word_C0B2 == NULL) {
                goto loc_9CC3;
            }
        }
        if (word_CA44 == NULL) {
            word_CA44 = CreateSolidBrush(RGB(255, 255, 0));
        }
        if (word_C0B4 == NULL) {
            word_C0B4 = CreateSolidBrush(RGB(128, 128, 0));
        }
        var_E = CreateCompatibleDC(var_2);
        SelectObject(var_E, word_C0B2);
#ifdef _WIN32
        /* Screen contents with height of only 40 pixels can be captured from window device context on Windows 10. Capture directly from screen instead. */
        screen = GetDC(NULL);
        BitBlt(var_E, 0, 0, 40, height_CA5C, screen, paintSubRectX_, paintSubRectY_ + 40, SRCCOPY);
        ReleaseDC(NULL, screen);
#else
        BitBlt(var_E, 0, 0, 40, height_CA5C, var_2, 0, 40, SRCCOPY);
#endif
        var_C.left = 0;
        var_C.top = 0;
        var_C.right = 40;
        var_C.bottom = height_CA5C;
        SelectObject(var_4, word_C0B8);
        FillRect(var_4, &var_C, word_CA44);
        BitBlt(var_E, 0, 0, 40, height_CA5C, var_4, 0, 0, SRCAND);
        FillRect(var_4, &var_C, word_C0B4);
        BitBlt(var_E, 0, 0, 40, height_CA5C, var_4, 0, 0, SRCPAINT);
        BitBlt(var_2, 0, 40, 40, height_CA5C, var_E, 0, 0, SRCCOPY);
        DeleteDC(var_E);
        DeleteDC(var_4);
    } else {
        if (word_C0B4 != NULL) {
            DeleteObject(word_C0B4);
            word_C0B4 = NULL;
        }
        if (word_CA44 != NULL) {
            DeleteObject(word_CA44);
            word_CA44 = NULL;
        }
        if (word_C0B2 != NULL) {
            DeleteObject(word_C0B2);
            word_C0B2 = NULL;
        }
        if (word_C0B8 != NULL) {
            DeleteObject(word_C0B8);
            word_C0B8 = NULL;
        }
        DeleteDC(var_4);
    }
    ReleaseDC(arg_0, var_2);
    return TRUE;
loc_9CC3:
    ReleaseDC(arg_0, var_2);
    DestroyWindow(arg_0);
    return FALSE;
}
