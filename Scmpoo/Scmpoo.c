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

enum SpriteState
{
    kStateNormal1 = 1,
    kStateRun1 = 7,     //8, break
    //8: 8/jump/hit
    kStateJump1 = 9,    //10
    //10: 10/run
    kStateWalk1 = 11,
    kStateHandstand1 = 13,
    kStateSex1 = 15,
    kStateSleep1 = 17,
    kStateWink1 = 20,
    kStateTurn1 = 24,
    kStateHit1 = 30,
    kStatePiss1 = 35,
    kStateYawn1 = 43,
    kStateBaa1 = 45,
    kStateSneeze1 = 47,
    kStateSuprise1 = 49,
    kStateScary1 = 51,
    kStateEat1 = 53,
    kStateSit1 = 58,
    kStateBlush1 = 62,
    kStateRoll1 = 65,
    kStateRotate1 = 69,
    kStateSlip1 = 75,
    kStateFall1 = 96,
    kStateMerry11 = 116,
    kStateMerry21 = 121,
    kStateMerry31 = 126,
    kStateUFO11 = 128,
    kStateUFO21 = 135,
    kStateUFO31 = 142,
    kStateBurn1 = 147,
};

typedef struct spriteinfo {
    HBITMAP bitmaps[2];
    int x;
    int y;
    int width;
    int height;
} spriteinfo;

typedef struct resourceinfo {
    int resId;
    WORD flags;
    spriteinfo info;
} resourceinfo;

typedef struct windowinfo {
    HWND window;
    RECT rect;
    BYTE padding[66]; /* Unused. */
} windowinfo;

int word_9CF0 = 245; /* Palette search maximum index (unused). */
resourceinfo originalRes_[16] = { /* Resource list. */
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

resourceinfo hFlipRes_[16] = {0}; /* Resource list storing flipped images. */
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
    kStateBaa1, kStateSneeze1, kStateSuprise1, kStateScary1
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
    kStateBaa1, kStateSneeze1, kStateSuprise1, kStateScary1
};
WORD nextStateNormal6_[8] = { /* Special action table. */
    kStateMerry11, kStateMerry21, kStateMerry31, kStateBurn1,
    kStateUFO11, kStateUFO21, kStateUFO31, kStateBurn1
};
int word_A2AA = 1; /* Facing direction. 1 = left, -1 = right */
int word_A2AC = 1; /* Facing direction (sub). 1 = left, -1 = right */
WORD word_A2B4[6][8] = { /* Blink animations. */
    {7, 8, 7, 6, 7, 8, 7, 6},
    {32, 33, 32, 31, 32, 33, 32, 31},
    {74, 75, 74, 73, 74, 75, 74, 73},
    {79, 80, 79, 78, 79, 80, 79, 78},
    {82, 83, 82, 81, 82, 83, 82, 81},
    {35, 36, 35, 34, 35, 36, 35, 34}
};
WORD word_A314[2][4] = { /* Hang on window top edge animations. */
    {42, 43, 42, 44},
    {46, 47, 46, 47}
};
WORD word_A324[20] = { /* Collision animation with obsolete height offset. */
    62, 63, 63, 64, 64, 65, 65, 66, 66, 66,
    0, 10, 17, 21, 22, 21, 17, 10, 0, 0
};
WORD word_A34C[11] = { /* Yawn animation. */
    37, 38, 39, 39, 39, 38, 37, 3, 37, 3, 0
};
WORD word_A362[8] = { /* Baa animation. */
    71, 72, 71, 72, 71, 72, 3, 0
};
WORD word_A372[13] = { /* Sneeze animation. */
    107, 108, 109, 109, 3, 3, 3, 110, 111, 110, 111, 3, 0
};
WORD word_A38C[6] = { /* Amazed animation. */
    50, 51, 50, 51, 3, 0
};
WORD word_A398[35] = { /* Eat animation. */
    58, 150, 60, 61, 60, 61, 60, 61, 58, 151, 60, 61, 60, 61, 60, 61, 2, 58, 152, 60, 61, 60, 61, 60, 61, 58, 153, 60, 61, 60, 61, 60, 61, 3, 0
};
WORD word_A3DE[34] = { /* Burn animation. */
    134, 134, 134, 134, 134, 134, 134, 134, 135, 136, 137, 138, 137, 138, 137, 138, 137, 138, 137, 138, 139, 140, 141, 142, 143, 144, 145, 144, 145, 144, 145, 144, 145, 0
};
WORD word_A422[13] = { /* Roll over animation (not used). */
    3, 93, 99, 100, 99, 100, 99, 100, 99, 100, 95, 3, 0
};
WORD word_A43C[8] = { /* Get up animation (left). */
    48, 48, 48, 49, 13, 12, 3, 0
};
WORD word_A44C[8] = { /* Get up animation (right). */
    48, 48, 48, 49, 13, 14, 3, 0
};
WORD word_A45C[28] = { /* Merry 2 animation. */
    130, 130, 130, 130, 130, 129, 129, 128, 128, 127, 127, 127, 6, 6, 6, 6, 7, 8, 7, 6, 7, 8, 7, 6, 6, 6, 6, 0
};
WORD word_A494[5] = { /* Burn bathtub splash animation. */
    147, 148, 147, 146, 0
};
WORD word_A49E[55] = { /* Burn get out of bathtub animation. */
    169, 169, 169, 169, 169, 169, 169, 169, 170, 171, 170, 169, 170, 171, 170, 169, 169, 169, 169, 81, 81, 81, 81, 81, 81, 81, 81, 85, 85, 85, 85, 85, 85, 85, 85, 34, 34, 34, 34, 35, 36, 35, 34, 35, 36, 35, 34, 34, 34, 10, 10, 9, 9, 3, 0
};
WORD word_A50C[12] = { /* Blush animation. */
    3, 127, 128, 129, 130, 130, 130, 129, 128, 127, 127, 0
};
WORD word_A524[9] = { /* Roll animation. */
    119, 120, 121, 122, 123, 124, 125, 126, 0
};
WORD word_A536[8] = { /* Spin animation. 0-3: face, 4-7: back */
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
HBITMAP bmp_A7B6[2] = {NULL, NULL}; /* Double buffer. */
HBITMAP bmp_A7BA = NULL; /* Sprite render target. */
HBITMAP bmp_A7BC = NULL; /* Sprite colour image for current frame. */
HBITMAP bmp_A7BE = NULL; /* Sprite mask image for current frame. */
int posx_A7C0 = 0; /* Sprite X-coordinate on resource image for current frame. */
int posy_A7C2 = 0; /* Sprite Y-coordinate on resource image for current frame. */
int word_A7C8 = 0; /* Sprite X-coordinate on resource image for previous frame. */
int word_A7CA = 0; /* Sprite Y-coordinate on resource image for previous frame (unused). */
WORD word_A7D0 = 0; /* Current framebuffer index. */
WORD word_A7D2 = 0; /* 0 to render sprite; 1 to update window. */
WORD word_A7D4 = 0; /* Unused. */
HBITMAP word_A7D8 = NULL; /* Sprite colour image for previous frame. */
int posx_A7DA = 0; /* Screen X-coordinate for current frame. */
int posy_A7DC = 0; /* Screen Y-coordinate for current frame. */
int width_A7DE = 0; /* Sprite width for current frame. */
int height_A7E0 = 0; /* Sprite height for current frame. */
int posx_A7E2 = 0; /* Update area rectangle X-coordinate for current frame. */
int posy_A7E4 = 0; /* Update area rectangle Y-coordinate for current frame. */
int word_A7E6 = 0; /* Update area rectangle width for current frame. */
int word_A7E8 = 0; /* Update area rectangle height for current frame. */
int word_A7EA = 0; /* Update area rectangle X-coordinate for previous frame. */
int word_A7EC = 0; /* Update area rectangle Y-coordinate for previous frame. */
int word_A7EE = 0; /* Update area rectangle width for previous frame. */
int word_A7F0 = 0; /* Update area rectangle height for previous frame. */
int posx_A7F2 = 0; /* Screen X-coordinate for previous frame. */
int posy_A7F4 = 0; /* Screen Y-coordinate for previous frame. */
int word_A7F6 = 0; /* Sprite width for previous frame. */
int word_A7F8 = 0; /* Sprite height for previous frame. */
WORD word_A7FA = 0; /* Current frame rectangle and previous frame rectangle have no intersecion? (unused) */
WORD word_A7FC = 0; /* Is gravity enabled? */
WORD word_A7FE = 0; /* Is collision with visible window enabled? */
int posx_A800 = 0; /* Current X-coordinate. */
int posy_A802 = 0; /* Current Y-coordinate. */
int index_A804 = 0; /* Sprite index. */
int word_A806 = 0; /* Vertical speed. */
int word_A808 = 0; /* Horizontal speed. */
int word_A80C = 0; /* Y-coordinate memory. */
int word_A80E = 0; /* Current X-coordinate (sub). */
int word_A810 = 0; /* Current Y-coordinate (sub). */
int word_A812 = 0; /* Sprite index (sub). */
HWND word_A81C = NULL; /* Active window or window to land on. */
RECT stru_A81E = {0L, 0L, 0L, 0L}; /* Rectangle of active window or window to land on. */
int word_A826 = 0; /* Animation frame counter. */
int word_A828 = 0; /* Random duration period counter. */
int word_A82A = 0; /* Random case number for action. */
WORD word_A82C = 0; /* Unused. */
HGLOBAL hPlaySoundData_ = NULL; /* Global handle for holding WAVE resource in memory. */
int word_A830 = 0; /* Current time hour. */
int word_A832 = 0; /* Remaining times for chime. */
DWORD dword_A834 = 0; /* Tick count. */
int word_A838 = 0; /* Time check period counter. */
int word_A83A = 0; /* Frame period counter. */
int word_A83C = 0; /* Target X-coordinate for window edge attachment. */
int word_A83E = 0; /* Target Y-coordinate for window edge attachment. */
WORD word_A840 = 0; /* Bounce when falling? */
int word_A842 = 0; /* Case number for fall action. */
int word_A844 = 0; /* Collision vertical speed (unused). */
int word_A846 = 0; /* Collision spin frame counter (unused). */
WORD word_A848 = 0; /* Collision animation frame index. */
int word_A84A = 0; /* Known instance list update period counter. */
HBITMAP word_A850[2] = {NULL, NULL}; /* Double buffer (sub). */
HBITMAP word_A854 = NULL; /* Sprite render target (sub). */
HBITMAP word_A856 = NULL; /* Sprite colour image for current frame (sub). */
HBITMAP word_A858 = NULL; /* Sprite mask image for current frame (sub). */
HBITMAP word_A85A = NULL; /* Fade out processed colour image (sub). */
HBITMAP word_A85C = NULL; /* Fade out processed mask image (sub). */
int word_A85E = 0; /* Sprite X-coordinate on resource image for current frame (sub). */
int word_A860 = 0; /* Sprite Y-coordinate on resource image for current frame (sub). */
int word_A866 = 0; /* Sprite X-coordinate on resource image for previous frame (sub). */
int word_A868 = 0; /* Sprite Y-coordinate on resource image for previous frame (sub) (unused). */
WORD word_A86E = 0; /* Current framebuffer index (sub). */
WORD word_A870 = 0; /* 0 to render sprite; 1 to update window (sub). */
WORD word_A872 = 0; /* Unused. */
HBITMAP word_A876 = NULL; /* Sprite colour image for previous frame (sub). */
int word_A878 = 0; /* Screen X-coordinate for current frame (sub). */
int word_A87A = 0; /* Screen Y-coordinate for current frame (sub). */
int word_A87C = 0; /* Sprite width for current frame (sub). */
int word_A87E = 0; /* Sprite height for current frame (sub). */
int word_A880 = 0; /* Update area rectangle X-coordinate for current frame (sub). */
int word_A882 = 0; /* Update area rectangle Y-coordinate for current frame (sub). */
int word_A884 = 0; /* Update area rectangle width for current frame (sub). */
int word_A886 = 0; /* Update area rectangle height for current frame (sub). */
int word_A888 = 0; /* Update area rectangle X-coordinate for previous frame (sub). */
int word_A88A = 0; /* Update area rectangle Y-coordinate for previous frame (sub). */
int word_A88C = 0; /* Update area rectangle width for previous frame (sub). */
int word_A88E = 0; /* Update area rectangle height for previous frame (sub). */
int word_A890 = 0; /* Screen X-coordinate for previous frame (sub). */
int word_A892 = 0; /* Screen Y-coordinate for previous frame (sub). */
int word_A894 = 0; /* Sprite width for previous frame (sub). */
int word_A896 = 0; /* Sprite height for previous frame (sub). */
WORD word_A898 = 0; /* Current frame rectangle and previous frame rectangle have no intersecion? (sub) (unused) */
WORD state_A8A0 = 0; /* State. */
spriteinfo sprites_[512] = {{{NULL, NULL}, 0, 0, 0, 0}}; /* Sprite list. First 256 for left-facing sprites, last 256 for right-facing sprites. */
int word_C0A4 = 0; /* No mouse action consecutive period counter. */
UINT confChime_ = 0U; /* Configuration: Chime */
WORD word_C0AE = 0; /* Screen Mate window on top of subwindow? (unused) */
HWND hPooWnd_ = NULL; /* Self instance window handle. */
HBITMAP word_C0B2 = NULL; /* UFO beam render target. */
HBRUSH word_C0B4 = NULL; /* UFO beam paint colour brush. */
UINT confNoSleep_ = 0U; /* Configuration: Always moving */
HBITMAP word_C0B8 = NULL; /* UFO beam colour rectangle image. */
WORD word_C0BA = 0; /* Remaining no-update periods after clearing windows. */
windowinfo stru_C0BC[32] = {{NULL, {0, 0, 0, 0}, {0}}}; /* Currently visible window list. */
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
WORD word_CA56 = 0; /* Not to clear subwindow? */
HINSTANCE hSelfInst_ = NULL; /* Current instance. */
UINT confSound_ = 0U; /* Configuration: Cry */
int word_CA5C = 0; /* UFO beam height (sub). */
WORD word_CA5E = 0; /* Unused. */
HWND hOtherPooWnd_[kMaxPooCount+1] = {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL}; /* Known instance list. When no other instance exists, [8] is used to store subwindow handle. */
int word_CA72 = 0; /* UFO beam height. */
int word_CA74 = 0; /* Number of currently visible windows. */
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
BOOL loadSprite(HDC, spriteinfo *, int resId, int);
void releaseSprite(spriteinfo *sprite);
void readConfig(void);
void sub_2FB7(LPCSTR, LPCSTR, UINT, LPCSTR);
void saveConfig(void);
BOOL initBitmaps(HWND);
void releaseWndBmp();
void sub_31A8(int, int, int index);
void reset_3237(HWND);
void paint_3284(HWND hWnd);
void paint_3717(HWND hWnd);
void sub_399D(HWND, int, int, int, int);
BOOL sub_39D6(HWND);
int sub_3A36(int, int, int, int);
void checkOtherPoo_3B4C(HWND hWnd);
BOOL checkOtherPoo(HWND);
void sub_3D12(HWND);
void sub_3D5F(HWND);
void sub_3DA7(HWND hWnd);
void sub_3DF0(void);
int sub_3E7C(HWND *, int, int, int, int);
int sub_408C(HWND *, int, int, int, int);
int sub_419E(HWND, int, int, int, int);
void playSoundRes(int, UINT, WORD);
void stopPlaySound(void);
void asyncPlaySound(LPCSTR path);
void sound_42C8(int, UINT, WORD);
BOOL initResources(HDC hdc);
void releaseResources(void);
void sub_4559(void);
void sub_4614(BOOL);
void sub_46D2(void);
void sub_46F7(void);
void sub_4807(int, int, int);
void sub_488C(int, int, int);
BOOL sub_48F3(HWND);
void sub_491D(HWND, LPRECT);
void sub_496F(int);
void sub_4B3B(void);
void sub_4C21(int, int, int);
int sub_4C91(int, int);
void resetState(void);
void sub_4CF8(void);
void event_8FD7(int);
void debugAction(WPARAM);
void movePooWnd(int, int);
BOOL initSubWnd(HWND);
void releaseSubWnd();
void sub_9350(int, int, int);
void sub_93DF(HWND);
void sub_9438(HWND);
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
        reset_3237(hWnd);
        ValidateRect(hWnd, NULL);
        return 0;
        GetWindowRect(hWnd, &rect);
        if (lastWndRect_.top == rect.top && lastWndRect_.bottom == rect.bottom && lastWndRect_.left == rect.left && lastWndRect_.right == rect.right) {
            reset_3237(hWnd);
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
        if (isMouseBtnDown_ != 0) {
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
        if (isMouseBtnDown_ == 0) {
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
        if (isMouseBtnDown_ != 0) {
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
        reset_3237(hWnd);
        if (oldTopMostConf != confTopMost__) {
            if (confTopMost__ != 0) {
                SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
            } else {
                SetWindowPos(hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
            }
        }
        if (oldGForceConf != confGForce_ && confGForce_ != 0) {
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
        sub_9438(hWnd);
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
BOOL loadSprite(HDC hdc, spriteinfo * pSprite, int resId, int flag)
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
void releaseSprite(spriteinfo * sprite)
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
void sub_2FB7(LPCSTR arg_0, LPCSTR arg_4, UINT arg_8, LPCSTR arg_A)
{
    char var_28[40];
    wsprintf(var_28, "%u", arg_8);
    WritePrivateProfileString(arg_0, arg_4, var_28, arg_A);
}

/* Save configurations to file. */
void saveConfig(void)
{
    sub_2FB7("Stray", "Sound", confSound_, "scmate.ini");
    sub_2FB7("Stray", "Alarm", confChime_, "scmate.ini");
    sub_2FB7("Stray", "NoSleep", confNoSleep_, "scmate.ini");
    sub_2FB7("Stray", "GForce", confGForce_, "scmate.ini");
}

/* Initialize bitmaps. */
BOOL initBitmaps(HWND hWnd)
{
    HDC hdc;
    hdc = GetDC(hWnd);
    bmp_A7B6[0] = CreateCompatibleBitmap(hdc, 100, 100);
    if (bmp_A7B6[0] == NULL) {
        goto failed;
    }
    bmp_A7B6[1] = CreateCompatibleBitmap(hdc, 100, 100);
    if (bmp_A7B6[1] == NULL) {
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
    DeleteObject(bmp_A7B6[0]);
    DeleteObject(bmp_A7B6[1]);
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
void sub_31A8(int x, int y, int index)
{
    posx_A7DA = x;
    posy_A7DC = y;
    bmp_A7BC = sprites_[index].bitmaps[0];
    bmp_A7BE = sprites_[index].bitmaps[1];
    posx_A7C0 = sprites_[index].x;
    posy_A7C2 = sprites_[index].y;
    width_A7DE = sprites_[index].width;
    height_A7E0 = sprites_[index].height;
}

/* Clear window. */
void reset_3237(HWND arg_0)
{
    posx_A7F2 = 0;
    posy_A7F4 = 0;
    word_A7F6 = 0;
    word_A7F8 = 0;
    MoveWindow(arg_0, 0, 0, 0, 0, TRUE);
    word_A7D4 = 1;
    word_C0BA = 1;
    word_A7D8 = NULL;
}

/* Render sprite with double buffering. */
void paint_3284(HWND hWnd)
{
    HDC var_2;
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
    if (word_A7D2 != 0) {
        return;
    }
    if (posx_A7F2 == posx_A7DA && posy_A7F4 == posy_A7DC &&
            word_A7D8 == bmp_A7BC && word_A7C8 == posx_A7C0 &&
            word_CA72 == 0) {
        return;
    }
    word_A7D0 ^= 1;
    var_2 = GetDC(NULL);
    SelectPalette(var_2, hPalette_, FALSE);
    hdc_4 = CreateCompatibleDC(var_2);
    hdc_6 = CreateCompatibleDC(var_2);
    SelectPalette(hdc_6, hPalette_, FALSE);
    SelectPalette(hdc_4, hPalette_, FALSE);
    posx_16 = max(posx_A7DA, posx_A7F2);
    posy_14 = max(posy_A7DC, posy_A7F4);
    var_12 = min(width_A7DE + posx_A7DA, word_A7F6 + posx_A7F2) - posx_16;
    var_10 = min(posy_A7DC + height_A7E0, posy_A7F4 + word_A7F8) - posy_14;
    if (var_12 <= 0 || var_10 <= 0) {
        word_A7FA = 1;
        if (word_A7D4 != 0) {
            word_A7D4 = 0;
        }
        posx_A7E2 = posx_A7DA;
        posy_A7E4 = posy_A7DC;
        word_A7E6 = width_A7DE;
        word_A7E8 = height_A7E0;
        SelectObject(hdc_4, bmp_A7B6[word_A7D0]);
        BitBlt(hdc_4, 0, 0, word_A7E6, word_A7E8, var_2, posx_A7E2, posy_A7E4, SRCCOPY);
    } else {
        word_A7FA = 0;
        posx_A7E2 = min(posx_A7DA, posx_A7F2);
        posy_A7E4 = min(posy_A7DC, posy_A7F4);
        word_A7E6 = max(width_A7DE + posx_A7DA, word_A7F6 + posx_A7F2) - posx_A7E2;
        word_A7E8 = max(posy_A7DC + height_A7E0, posy_A7F4 + word_A7F8) - posy_A7E4;
        SelectObject(hdc_4, bmp_A7B6[word_A7D0]);
        BitBlt(hdc_4, 0, 0, word_A7E6, word_A7E8, var_2, posx_A7E2, posy_A7E4, SRCCOPY);
        var_1E = max(posx_A7E2, word_A7EA);
        var_1C = max(posy_A7E4, word_A7EC);
        width_1A = min(word_A7E6 + posx_A7E2, word_A7EE + word_A7EA) - var_1E;
        height_18 = min(posy_A7E4 + word_A7E8, word_A7EC + word_A7F0) - var_1C;
        posx_16 = max(0, var_1E - posx_A7E2);
        posy_14 = max(0, var_1C - posy_A7E4);
        posx_E = max(0, var_1E - word_A7EA);
        posy_C = max(0, var_1C - word_A7EC);
        if (width_1A > 0 && height_18 > 0) {
            SelectObject(hdc_6, bmp_A7B6[LOBYTE(word_A7D0) - 0xFF & 1]);
            BitBlt(hdc_4, posx_16, posy_14, width_1A, height_18, hdc_6, posx_E, posy_C, SRCCOPY);
        }
    }

    if (bmp_A7BC != NULL) {
        SelectObject(hdc_6, bmp_A7BA);
        BitBlt(hdc_6, 0, 0, word_A7E6, word_A7E8, hdc_4, 0, 0, SRCCOPY);
        posx_16 = max(0, posx_A7DA - posx_A7E2);
        posy_14 = max(0, posy_A7DC - posy_A7E4);
        if (bmp_A7BE != NULL) {
            SelectObject(hdc_4, bmp_A7BE);
            BitBlt(hdc_6, posx_16, posy_14, width_A7DE, height_A7E0, hdc_4, posx_A7C0, posy_A7C2, SRCAND);
            SelectObject(hdc_4, bmp_A7BC);
            BitBlt(hdc_6, posx_16, posy_14, width_A7DE, height_A7E0, hdc_4, posx_A7C0, posy_A7C2, SRCPAINT);
        } else {
            SelectObject(hdc_4, bmp_A7BC);
            BitBlt(hdc_6, posx_16, posy_14, width_A7DE, height_A7E0, hdc_4, posx_A7C0, posy_A7C2, SRCCOPY);
        }
        word_A7D2 = 1;
        word_CA5E = 1;
        MoveWindow(hWnd, posx_A7E2, posy_A7E4, word_A7E6, word_A7E8 + word_CA72, TRUE);
        word_CA5E = 0;
    }
    DeleteDC(hdc_4);
    DeleteDC(hdc_6);
    word_A7EA = posx_A7E2;
    word_A7EC = posy_A7E4;
    word_A7EE = word_A7E6;
    word_A7F0 = word_A7E8;
    posx_A7F2 = posx_A7DA;
    posy_A7F4 = posy_A7DC;
    word_A7F6 = width_A7DE;
    word_A7F8 = height_A7E0;
    word_A7D8 = bmp_A7BC;
    word_A7C8 = posx_A7C0;
    word_A7CA = posy_A7C2;
    ReleaseDC(NULL, var_2);
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
    if (word_A7D2 == 0) {
        return;
    }
    word_A7D2 = 0;
    hdc_2 = GetDC(hWnd);
    SelectPalette(hdc_2, hPalette_, FALSE);
    RealizePalette(hdc_2);
    hdc_4 = CreateCompatibleDC(hdc_2);
    SelectPalette(hdc_4, hPalette_, FALSE);
    SelectObject(hdc_4, bmp_A7BA);
    BitBlt(hdc_2, 0, 0, word_A7E6, word_A7E8, hdc_4, 0, 0, SRCCOPY);
    if (word_CA72 != 0) {
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
        BitBlt(hdc_E, 0, 0, 40, word_CA72, screen, posx_A7E2, posy_A7E4 + 40, SRCCOPY);
        ReleaseDC(NULL, screen);
#else
        BitBlt(var_E, 0, 0, 40, word_CA72, var_2, 0, 40, SRCCOPY);
#endif
        var_C.left = 0;
        var_C.top = 0;
        var_C.right = 40;
        var_C.bottom = word_CA72;
        SelectObject(hdc_4, word_C0B8);
        FillRect(hdc_4, &var_C, word_CA44);
        BitBlt(hdc_E, 0, 0, 40, word_CA72, hdc_4, 0, 0, SRCAND);
        FillRect(hdc_4, &var_C, word_C0B4);
        BitBlt(hdc_E, 0, 0, 40, word_CA72, hdc_4, 0, 0, SRCPAINT);
        BitBlt(hdc_2, 0, 40, 40, word_CA72, hdc_E, 0, 0, SRCCOPY);
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
int sub_3A36(int posx_a0, int posx_a2, int posy_a4, int posy_a6)
{
    int i;
    int posx_4;
    int posx_6;
    int posy_8;
    int posy_A;
    for (i = 0; i < kMaxPooCount; i += 1) {
        if (hOtherPooWnd_[i] != NULL) {
            posx_4 = (short)GetWindowWord(hOtherPooWnd_[i], 0);
            posy_8 = (short)GetWindowWord(hOtherPooWnd_[i], 2);
            posx_6 = posx_4 + 40;
            posy_A = posy_8 + 40;
            if (posx_6 == 0) {
                continue;
            }
            if (((posy_8 <= posy_a4 && posy_A > posy_a4) || (posy_8 < posy_a6 && posy_A > posy_a6)) &&
                    posx_6 > posx_a0 && posx_6 <= posx_a2 && posx_a2 > posx_a0) {
                return posx_6;
            }
            if (((posy_8 <= posy_a4 && posy_A > posy_a4) || (posy_8 < posy_a6 && posy_A > posy_a6)) &&
                    posx_4 >= posx_a2 && posx_4 < posx_a0 && posx_a2 < posx_a0) {
                return posx_4;
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
void sub_3DF0(void)
{
    HWND var_2;
    UINT var_4;
    int var_6;
    var_2 = GetDesktopWindow();
    var_4 = GW_CHILD;
    var_6 = 0;
    while ((var_2 = GetWindow(var_2, var_4)) != NULL && var_6 < 32) {
        var_4 = GW_HWNDNEXT;
        if (var_2 == hPooWnd_) {
            continue;
        }
        if ((GetWindowLong(var_2, GWL_STYLE) & WS_VISIBLE) != 0) {
            GetWindowRect(var_2, &stru_C0BC[var_6].rect);
            stru_C0BC[var_6].window = var_2;
            var_6 += 1;
        }
    }
    word_CA74 = var_6;
}

/* Find X-coordinate of possible collision with which visible window. */
int sub_3E7C(HWND * arg_0, int arg_2, int arg_4, int arg_6, int arg_8)
{
    int var_2;
    int var_4;
    RECT var_C;
    for (var_2 = 0; var_2 < word_CA74; var_2 += 1) {
        if (arg_8 > arg_6) {
            if (stru_C0BC[var_2].rect.right >= arg_6 && stru_C0BC[var_2].rect.right < arg_8 && stru_C0BC[var_2].rect.top < arg_2 && stru_C0BC[var_2].rect.bottom > arg_4) {
                for (var_4 = 0; var_4 < var_2; var_4 += 1) {
                    if (stru_C0BC[var_4].rect.top <= arg_2 && stru_C0BC[var_4].rect.bottom >= arg_4 && stru_C0BC[var_4].rect.left <= arg_6 && stru_C0BC[var_4].rect.right >= arg_8) {
                        break;
                    }
                }
                if (var_4 == var_2) {
                    if (IsWindow(stru_C0BC[var_2].window)) {
                        GetWindowRect(stru_C0BC[var_2].window, &var_C);
                        if (stru_C0BC[var_2].rect.right == var_C.right) {
                            *arg_0 = stru_C0BC[var_2].window;
                            return stru_C0BC[var_2].rect.right;
                        }
                    }
                }
            }
        } else {
            if (stru_C0BC[var_2].rect.left <= arg_6 && stru_C0BC[var_2].rect.left > arg_8 && stru_C0BC[var_2].rect.top < arg_2 && stru_C0BC[var_2].rect.bottom > arg_4) {
                for (var_4 = 0; var_4 < var_2; var_4 += 1) {
                    if (stru_C0BC[var_4].rect.top <= arg_2 && stru_C0BC[var_4].rect.bottom >= arg_4 && stru_C0BC[var_4].rect.left <= arg_6 && stru_C0BC[var_4].rect.right >= arg_8) {
                        break;
                    }
                }
                if (var_4 == var_2) {
                    if (IsWindow(stru_C0BC[var_2].window)) {
                        GetWindowRect(stru_C0BC[var_2].window, &var_C);
                        if (stru_C0BC[var_2].rect.left == var_C.left) {
                            *arg_0 = stru_C0BC[var_2].window;
                            return stru_C0BC[var_2].rect.left;
                        }
                    }
                }
            }
        }
    }
    return 0;
}

/* Find Y-coordinate of possible landing top edge of which visible window. */
int sub_408C(HWND * arg_0, int arg_2, int arg_4, int arg_6, int arg_8)
{
    int var_2;
    int var_4;
    for (var_2 = 0; var_2 < word_CA74; var_2 += 1) {
        if (stru_C0BC[var_2].rect.top <= arg_2 && stru_C0BC[var_2].rect.top > arg_4 && stru_C0BC[var_2].rect.left < arg_8 && stru_C0BC[var_2].rect.right > arg_6 && stru_C0BC[var_2].rect.top > 10) {
            for (var_4 = 0; var_4 < var_2; var_4 += 1) {
                if (stru_C0BC[var_4].rect.left <= arg_6 && stru_C0BC[var_4].rect.right >= arg_8 && stru_C0BC[var_4].rect.top <= arg_4 && stru_C0BC[var_4].rect.bottom >= arg_2) {
                    break;
                }
            }
            if (var_4 == var_2) {
                *arg_0 = stru_C0BC[var_2].window;
                return stru_C0BC[var_2].rect.top;
            }
        }
    }
    if (arg_2 >= screenHeight_ && arg_4 <= screenHeight_) {
        *arg_0 = NULL;
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
void sound_42C8(int resId, UINT flags, WORD arg_4)
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
void sub_4559(void)
{
    if (word_A2AA > 0 && posx_A800 < 0) {
        state_A8A0 = kStateTurn1;
    }
    if (word_A2AA < 0 && screenWidth_ - sprites_[index_A804].width < posx_A800) {
        state_A8A0 = kStateTurn1;
    }
    if (word_A2AA > 0 && screenWidth_ - sprites_[index_A804].width > posx_A800 && rand() % 20 == 0) {
        state_A8A0 = kStateTurn1;
    }
    if (word_A2AA < 0 && posx_A800 > 0 && rand() % 20 == 0) {
        state_A8A0 = kStateTurn1;
    }
}

/* Flag-controlled collision and turn around. */
void sub_4614(BOOL arg_0)
{
    if (word_A7FC == 0) {
        if (word_A2AA > 0 && posx_A800 < 0) {
            state_A8A0 = kStateHit1;
        }
        if (word_A2AA < 0 && screenWidth_ - sprites_[index_A804].width < posx_A800) {
            state_A8A0 = kStateHit1;
        }
    }
    if (arg_0) {
        if (word_A2AA > 0 && screenWidth_ - 80 > posx_A800 && rand() % 20 == 0) {
            state_A8A0 = kStateTurn1;
        }
        if (word_A2AA < 0 && posx_A800 > 40 && rand() % 20 == 0) {
            state_A8A0 = kStateTurn1;
        }
    }
}

/* Switch to standing sprite after certain frames. */
void sub_46D2(void)
{
    if (word_A826-- <= 0) {
        state_A8A0 = 42;
    }
}

/* Process chime. */
void sub_46F7(void)
{
    struct tm * var_2;
    time_t var_6;
    DWORD tick;
    int var_C;
    if (word_A832 != 0) {
        tick = GetTickCount();
        if (dword_A834 + 1000 < tick) {
            dword_A834 = tick;
            word_A832 -= 1;
            if (word_A832 != 0) {
                playSoundRes(IDW_BAA, 0U, 0);
            } else if (word_CA76 != 0) {
                state_A8A0 = 113;
            } else {
                state_A8A0 = kStateNormal1;
            }
        }
    } else {
        if (word_A838++ < 10) {
            return;
        }
        word_A838 = 0;
        time(&var_6);
        var_2 = localtime(&var_6);
        var_C = var_2->tm_hour % 12;
        if (var_C == 0) {
            var_C = 12;
        }
        if (var_2->tm_min == 0 && var_C != word_A830) {
            destroySubPooWnd();
            dword_A834 = 0;
            word_A830 = var_C;
            word_A832 = word_A830 + 1;
            state_A8A0 = 81;
        }
    }
}

/* Update window position and sprite to be painted. */
void sub_4807(int x, int y, int sprite)
{
    SetWindowWord(hPooWnd_, 0, (short)posx_A800);
    SetWindowWord(hPooWnd_, 2, (short)posy_A802);
    if (sprite >= 9 && sprite <= 14) {
        sub_31A8(x, y, sprite);
    } else if (word_A2AA > 0) {
        sub_31A8(x, y, sprite);
    } else {
        sub_31A8(x, y, sprite + 256);
    }
}

/* Update window position and sprite to be painted (sub). */
void sub_488C(int arg_0, int arg_2, int arg_4)
{
    if (arg_4 >= 9 && arg_4 <= 14) {
        sub_9350(arg_0, arg_2, arg_4);
    } else if (word_A2AC > 0) {
        sub_9350(arg_0, arg_2, arg_4);
    } else {
        sub_9350(arg_0, arg_2, arg_4 + 256);
    }
}

/* Return TRUE if the window handle is NULL or if the handle contains an existing window. */
BOOL sub_48F3(HWND arg_0)
{
    if (arg_0 == NULL) {
        return TRUE;
    } else {
        return IsWindow(arg_0);
    }
}

/* Get window rect. If the window handle is NULL, get a screen rect located right under the current screen. */
void sub_491D(HWND arg_0, LPRECT arg_2)
{
    if (arg_0 == NULL) {
        arg_2->left = 0;
        arg_2->right = screenWidth_;
        arg_2->top = screenHeight_;
        arg_2->bottom = screenHeight_ * 2;
    } else {
        GetWindowRect(arg_0, arg_2);
    }
}

/* Process when out of screen view or at different positions on top of visible window. */
void sub_496F(int arg_0)
{
    RECT var_8;
    if (word_A7FC == 0) {
        return;
    }
    if (word_A81C != NULL) {
        if (!sub_48F3(word_A81C)) {
            if (arg_0 == 2) {
                state_A8A0 = 94;
            } else {
                state_A8A0 = 102;
            }
            return;
        }
        sub_491D(word_A81C, &var_8);
        if (var_8.top > stru_A81E.top || posx_A800 + 40 < var_8.left || var_8.right < posx_A800) {
            if (arg_0 == 2) {
                state_A8A0 = 94;
            } else {
                state_A8A0 = 102;
            }
            return;
        }
        if (var_8.top < stru_A81E.top) {
            posy_A802 = var_8.top - sprites_[index_A804].height;
            stru_A81E.top = var_8.top;
            stru_A81E.bottom = var_8.bottom;
            stru_A81E.left = var_8.left;
            stru_A81E.right = var_8.right;
            sub_4807(posx_A800, posy_A802, index_A804);
            return;
        }
        if (arg_0 == 1) {
            if (posx_A800 + 8 < var_8.left && word_A2AA > 0) {
                state_A8A0 = 105;
                posx_A800 = var_8.left - 10;
                return;
            }
            if (posx_A800 + 32 >= var_8.right && word_A2AA < 0) {
                state_A8A0 = 105;
                posx_A800 = var_8.right - 30;
                return;
            }
            if (rand() % 20 - 1 == 0 && screenHeight_ - posy_A802 > 100) {
                state_A8A0 = 104;
                return;
            }
        }
        if (arg_0 == 2) {
            if (posx_A800 + 32 < var_8.left || posx_A800 + 8 > var_8.right) {
                state_A8A0 = 94;
                return;
            }
        }
    }
    if (sprites_[index_A804].width + posx_A800 < 0 || posx_A800 > screenWidth_) {
        state_A8A0 = 0;
        return;
    }
}

/* Process when climbing up side of a window. */
void sub_4B3B(void)
{
    RECT var_8;
    if (word_A7FC == 0) {
        return;
    }
    if (word_A81C != NULL) {
        if (!sub_48F3(word_A81C)) {
            state_A8A0 = 102;
            return;
        }
        sub_491D(word_A81C, &var_8);
        if (var_8.right < stru_A81E.right && word_A2AA > 0 || var_8.left > stru_A81E.left && word_A2AA < 0) {
            state_A8A0 = 102;
            return;
        }
        if (var_8.right > stru_A81E.right && word_A2AA > 0 || var_8.left < stru_A81E.left && word_A2AA < 0) {
            if (word_A2AA > 0) {
                posx_A800 = var_8.right + 10;
            } else {
                posx_A800 = var_8.left - 50;
            }
            sub_4807(posx_A800, posy_A802, index_A804);
            state_A8A0 = 102;
            return;
        }
    }
}

/* Detect collision with other instances, action controlled by a flag. */
void sub_4C21(int posx_0, int posx_2, int arg_4)
{
    if (posx_2 < posx_0) {
        posx_0 += 40;
        posx_2 = posx_0 - 80;
    } else {
        posx_2 = posx_0 + 80;
    }
    if (sub_3A36(posx_0, posx_2, posy_A802, posy_A802 + 40) != 0) {
        if (arg_4 == 1) {
            state_A8A0 = kStateTurn1;
        }
        if (arg_4 == 2) {
            state_A8A0 = kStateHit1;
        }
    }
}

/* Detect collision with other instances and find X-coordinate. */
int sub_4C91(int arg_0, int arg_2)
{
    if (arg_2 < arg_0) {
        arg_0 += 40;
        arg_2 = arg_0 - 80;
    } else {
        arg_2 = arg_0 + 80;
    }
    return sub_3A36(arg_0, arg_2, posy_A802, posy_A802 + 40);
}

/* Reinitialize state. */
void resetState(void)
{
    state_A8A0 = 0;
}

/* Process state change on each timer expiration. */
void sub_4CF8(void)
{
    int var_2;
    int var_4;
    HWND var_6;
    HWND var_8;
    RECT var_10;
    POINT var_14;
    if (word_A84A++ > 100) {
        checkOtherPoo_3B4C(hPooWnd_);
        word_A84A = 0;
    }
    if (confChime_ != 0) {
        sub_46F7();
    }
loc_4D33:
    switch (state_A8A0) {
    case 0:
        word_A7FC = 0;
        srand((unsigned int)GetTickCount());
        posx_A800 = -80;
        posy_A802 = -80;
        state_A8A0 = kStateNormal1;
    case kStateNormal1:
        word_A844 = 0;
        if (confGForce_ != 0U) {
            state_A8A0 = 2;
            goto loc_4D33;
        }
        word_CA72 = 0;
        destroySubPooWnd();
        if (word_CA54 != 0) {
            state_A8A0 = word_CA54;
            word_CA54 = 0;
            break;
        }
        if (rand() % 20 == 5 && word_A7FC == 0) {
            state_A8A0 = 85;
            break;
        }
        if (rand() % 40 == 5 && word_A7FC == 0 && hasOtherPoo_ == 0) {
            state_A8A0 = 4;
            break;
        }
        state_A8A0 = nextStateNormal1_[rand() % 80];
        if (posx_A800 > screenWidth_ || posx_A800 < -40 || posy_A802 < -40 || posy_A802 > screenHeight_) {
            if ((rand() & 1) == 0) {
                word_A2AA = 1;
                posx_A800 = screenWidth_ + word_CA4C;
                posy_A802 = rand() % (screenHeight_ - 64) + word_CA4E;
            } else {
                word_A2AA = -1;
                posx_A800 = -40;
                posy_A802 = rand() % (screenHeight_ - 64) + word_CA4E;
            }
            state_A8A0 = kStateWalk1;
        }
        break;
    case 2:
        word_A7FC = 1;
        word_CA72 = 0;
        destroySubPooWnd();
        if (word_CA54 != 0) {
            state_A8A0 = word_CA54;
            word_CA54 = 0;
            break;
        }
        state_A8A0 = nextStateNormal2_[rand() % 80];
        if (posx_A800 > screenWidth_ || posx_A800 < -40 || posy_A802 < -40 || posy_A802 > screenHeight_) {
            if (rand() % 10 == 0 && hasOtherPoo_ == 0) {
                state_A8A0 = 6;
                break;
            }
            word_A81C = GetActiveWindow();
            if (word_A81C == hPooWnd_ || word_A81C == hOtherPooWnd_[kISubPooHwnd] || word_A81C == NULL || sub_39D6(word_A81C)) {
                state_A8A0 = 3;
                goto loc_4D33;
            }
            sub_491D(word_A81C, &stru_A81E);
            if (stru_A81E.top < 10) {
                state_A8A0 = 3;
                goto loc_4D33;
            }
            posx_A800 = (rand() % stru_A81E.right - stru_A81E.left) / 3 + (stru_A81E.right - stru_A81E.left) / 2 + stru_A81E.left - 20;
            posy_A802 = -40;
            word_A840 = 0;
            word_A806 = 0;
            word_A808 = 0;
            word_A842 = rand() % 2;
            state_A8A0 = 92;
            if (rand() % 3 == 0) {
                state_A8A0 = 3;
                goto loc_4D33;
            }
        }
        break;
    case 3:
        word_A7FC = 1;
        posx_A800 = rand() % (screenWidth_ - 40);
        posy_A802 = -(rand() % 20 - (-40));
        word_A840 = 0;
        word_A806 = 0;
        word_A808 = 0;
        word_A842 = rand() % 2;
        if (rand() % 3 == 0) {
            putWndToTop(hPooWnd_);
        }
        state_A8A0 = 97;
        break;
    case 153:
        break;
    case 154:
        break;
    case 4:
        if (screenWidth_ / 2 - 20 > posx_A800) {
            word_A2AA = 1;
        } else {
            word_A2AA = -1;
        }
        index_A804 = 4;
        sub_4807(posx_A800, posy_A802, index_A804);
        state_A8A0 = 5;
        break;
    case 5:
        if (word_A83A++ < 1) {
            break;
        }
        word_A83A = 0;
        posx_A800 -= word_A2AA * 16;
        index_A804 = index_A804 == 4 ? 5 : 4;
        sub_4807(posx_A800, posy_A802, index_A804);
        if (posx_A800 < -40 || posx_A800 > screenWidth_) {
            state_A8A0 = 6;
        }
        break;
    case 6:
        state_A8A0 = nextStateNormal6_[rand() % 8];
        break;
    case kStateRun1:
        word_A7FE = 0;
        if ((rand() & 1) == 0) {
            word_A7FE = 1;
        }
        if (word_A7FE != 0) {
            sub_3DF0();
        }
        index_A804 = 4;
        sub_4807(posx_A800, posy_A802, index_A804);
        word_A826 = rand() % 10 + 10;
        state_A8A0 = 8;
        break;
    case 8:
        if (word_A83A++ < 1) {
            break;
        }
        word_A83A = 0;
        if (word_A7FE != 0) {
            if (word_A2AA > 0) {
                var_2 = sub_3E7C(&var_6, posy_A802, posy_A802 + 40, -(word_A2AA * 16 - posx_A800), posx_A800);
            } else {
                var_2 = sub_3E7C(&var_6, posy_A802, posy_A802 + 40, -(word_A2AA * 16 - posx_A800) + 40, posx_A800 + 40);
            }
            if (var_2 != 0) {
                if (word_A2AA > 0) {
                    posx_A800 = var_2;
                } else {
                    posx_A800 = var_2 - 40;
                }
                sub_4807(posx_A800, posy_A802, index_A804);
                state_A8A0 = kStateHit1;
                break;
            }
        }
        if (word_A82C == 0) {
            posx_A800 -= word_A2AA * 16;
        }
        index_A804 = index_A804 == 4 ? 5 : 4;
        sub_4807(posx_A800, posy_A802, index_A804);
        if (rand() % 50 == 0 && word_A7FC != 0) {
            state_A8A0 = kStateJump1;
        }
        sub_4614(TRUE);
        sub_46D2();
        sub_496F(2);
        sub_4C21(-(word_A2AA * 16 - posx_A800), word_A2AA * 16 + posx_A800, 2);
        break;
    case kStateJump1:
        word_A806 = -11;
        word_A808 = -(word_A2AA * 8);
        word_A80C = posy_A802;
        state_A8A0 = 10;
    case 10:
        posx_A800 += word_A808;
        posy_A802 += word_A806;
        word_A806 += 2;
        if (word_A806 >= -1 && word_A806 <= 1) {
            index_A804 = 23;
        } else if (word_A806 < -1) {
            index_A804 = 30;
        } else {
            index_A804 = 24;
        }
        if (word_A80C <= posy_A802) {
            posy_A802 = word_A80C;
            state_A8A0 = kStateRun1;
        }
        sub_4807(posx_A800, posy_A802, index_A804);
        sub_4614(FALSE);
        sub_4C21(word_A808 + posx_A800, posx_A800 - word_A808, 2);
        if (state_A8A0 == kStateHit1 && word_A80C != posy_A802) {
            word_A844 = posy_A802 - word_A80C;
        }
        break;
    case kStateWalk1:
        word_A7FE = 0;
        if ((word_A7FC & !(rand() & 1)) != 0) {
            word_A7FE = 1;
        }
        if (word_A7FE != 0) {
            sub_3DF0();
        }
        word_A826 = rand() % 10 + 10;
        index_A804 = 2;
        sub_4807(posx_A800, posy_A802, index_A804);
        state_A8A0 = 12;
        break;
    case 12:
        if (word_A83A++ < 1) {
            break;
        }
        word_A83A = 0;
        if (word_A7FE != 0) {
            if (word_A2AA > 0) {
                var_2 = sub_3E7C(&var_6, posy_A802, posy_A802 + 40, -(word_A2AA * 6 - posx_A800), posx_A800);
            } else {
                var_2 = sub_3E7C(&var_6, posy_A802, posy_A802 + 40, -(word_A2AA * 6 - posx_A800) + 40, posx_A800 + 40);
            }
            if (var_2 != 0) {
                if (word_A2AA > 0) {
                    posx_A800 = var_2;
                } else {
                    posx_A800 = var_2 - 40;
                }
                word_A81C = var_6;
                sub_491D(word_A81C, &stru_A81E);
                word_A83E = stru_A81E.top - 12;
                word_A7FC = 1;
                word_A83C = posx_A800;
                index_A804 = 30;
                sub_2B01(hPooWnd_, word_A81C);
                state_A8A0 = 89;
                break;
            }
        }
        if (word_A82C == 0) {
            posx_A800 -= word_A2AA * 6;
        }
        index_A804 = index_A804 == 2 ? 3 : 2;
        sub_4807(posx_A800, posy_A802, index_A804);
        sub_4559();
        sub_46D2();
        sub_496F(1);
        sub_4C21(-(word_A2AA * 6 - posx_A800), word_A2AA * 6 + posx_A800, 1);
        break;
    case kStateHandstand1:
        word_A82A = rand() % 2;
        word_A826 = rand() % 4 + 4;
        if (word_A82A != 0) {
            index_A804 = 88;
        } else {
            index_A804 = 86;
        }
        sub_4807(posx_A800, posy_A802, index_A804);
        state_A8A0 = 14;
        break;
    case 14:
        if (word_A83A++ < 3) {
            break;
        }
        word_A83A = 0;
        if (word_A82C == 0) {
            posx_A800 -= word_A2AA * 6;
        }
        if (word_A82A != 0) {
            index_A804 = index_A804 == 88 ? 89 : 88;
        } else {
            index_A804 = index_A804 == 86 ? 87 : 86;
        }
        sub_4807(posx_A800, posy_A802, index_A804);
        sub_4559();
        sub_46D2();
        sub_496F(1);
        break;
    case kStateSex1:
        word_A82A = rand() % 2;
        word_A826 = rand() % 3 + 3;
        if (word_A82A != 0) {
            index_A804 = 54;
        } else {
            index_A804 = 52;
        }
        sub_4807(posx_A800, posy_A802, index_A804);
        state_A8A0 = 16;
        break;
    case 16:
        if (word_A83A++ < 3) {
            break;
        }
        word_A83A = 0;
        if (word_A82A != 0) {
            index_A804 = index_A804 == 54 ? 55 : 54;
        } else {
            index_A804 = index_A804 == 52 ? 53 : 52;
        }
        sub_4807(posx_A800, posy_A802, index_A804);
        sub_4559();
        sub_46D2();
        sub_496F(0);
        break;
    case kStateSleep1:
        index_A804 = 6;
        sub_4807(posx_A800, posy_A802, index_A804);
        state_A8A0 = 18;
        break;
    case 18:
        if (word_A83A++ < 1) {
            break;
        }
        word_A83A = 0;
        index_A804 += 1;
        sub_4807(posx_A800, posy_A802, index_A804);
        if (index_A804 == 8) {
            index_A804 = 0;
            state_A8A0 = 19;
            word_A826 = rand() % 8 + 8;
        }
        sub_496F(0);
        break;
    case 19:
        if (word_A83A++ < 4) {
            break;
        }
        word_A83A = 0;
        index_A804 = index_A804 == 0 ? 1 : 0;
        sub_4807(posx_A800, posy_A802, index_A804);
        sub_46D2();
        sub_496F(0);
        break;
    case kStateWink1:
        word_A82A = rand() % 3;
        if (word_A82A == 0) {
            index_A804 = 6;
        } else if (word_A82A == 1) {
            index_A804 = 31;
        } else {
            index_A804 = 73;
        }
        sub_4807(posx_A800, posy_A802, index_A804);
        state_A8A0 = 21;
        word_A83A = rand() % 15 + rand() % 15;
        sub_496F(0);
        break;
    case 21:
        sub_496F(0);
        if (word_A83A-- > 0) {
            break;
        }
        state_A8A0 = 22;
        word_A826 = 0;
        break;
    case 22:
        index_A804 = word_A2B4[word_A82A][word_A826];
        sub_4807(posx_A800, posy_A802, index_A804);
        word_A826 += 1;
        if (word_A826 > 7) {
            state_A8A0 = 23;
            word_A83A = rand() % 15 + rand() % 15;
        }
        sub_496F(0);
        break;
    case 23:
        sub_496F(0);
        if (word_A83A-- > 0) {
            break;
        }
        state_A8A0 = kStateNormal1;
        break;
    case kStateTurn1:
        if (word_A83A++ < 1) {
            break;
        }
        word_A83A = 0;
        index_A804 = 3;
        sub_4807(posx_A800, posy_A802, index_A804);
        if ((rand() & 1) != 0) {
            word_A82A = 0;
        } else {
            word_A82A = 1;
        }
        state_A8A0 = 25;
        word_A826 = 0;
        break;
    case 25:
        if (word_A83A++ < 1) {
            break;
        }
        word_A83A = 0;
        if (word_A82A != 0) {
            if (word_A2AA > 0) {
                index_A804 = word_A826 + 9;
            } else {
                index_A804 = 11 - word_A826;
            }
        } else {
            if (word_A2AA > 0) {
                index_A804 = word_A826 + 12;
            } else {
                index_A804 = 14 - word_A826;
            }
        }
        sub_4807(posx_A800, posy_A802, index_A804);
        word_A826 += 1;
        if (word_A826 > 2) {
            word_A2AA = -word_A2AA;
            state_A8A0 = 26;
        }
        sub_496F(0);
        break;
    case 26:
        if (word_A83A++ < 1) {
            break;
        }
        word_A83A = 0;
        index_A804 = 3;
        sub_4807(posx_A800, posy_A802, index_A804);
        state_A8A0 = kStateNormal1;
        sub_496F(0);
        break;
    case 27:
        word_A806 = -10;
        word_A808 = word_A2AA * 8;
        word_A80C = posy_A802;
        word_A848 = 0;
        state_A8A0 = 28;
    case 28:
        posx_A800 += word_A808;
        posy_A802 += word_A806;
        word_A806 += 2;
        index_A804 = word_A324[word_A848];
        word_A848 += 1;
        sub_4807(posx_A800, posy_A802, index_A804);
        if (index_A804 == 64) {
            word_A842 = 3;
            state_A8A0 = 99;
            break;
        }
        break;
    case 29:
        word_A83A = 0;
        word_A826 = 0;
        word_A82A = 0;
        if ((rand() & 7) == 0) {
            word_A82A = 1;
        }
        if (rand() % 5 == 0) {
            word_A82A = 2;
        }
        state_A8A0 = 32;
        if (word_A82A != 0) {
            state_A8A0 = 34;
        }
        goto loc_4D33;
    case kStateHit1:
        if (word_A7FC != 0) {
            state_A8A0 = 27;
            goto loc_4D33;
        } else {
            state_A8A0 = kStateTurn1;
            goto loc_4D33;
        }
        word_A83A = 0;
        word_A826 = 0;
        word_A82A = 0;
        if ((rand() & 7) == 0) {
            word_A82A = 1;
        }
        if (rand() % 5 == 0) {
            word_A82A = 2;
        }
        state_A8A0 = 31;
    case 31:
        sub_4C21(word_A2AA * 10 + posx_A800, posx_A800, 2);
        if (state_A8A0 == kStateHit1) {
            if (word_A826 != 0) {
                word_A844 -= word_A324[word_A826 + 9];
            }
            break;
        }
        index_A804 = word_A324[word_A826];
        sub_4807(posx_A800, posy_A802 - word_A324[word_A826 + 10], index_A804);
        word_A826 += 1;
        if (word_A82A != 0 && index_A804 == 66) {
            if (word_A844 != 0) {
                posy_A802 -= word_A844;
                posx_A800 += word_A2AA * 10;
                sub_4807(posx_A800, posy_A802, index_A804);
            }
            word_A846 = 3;
            state_A8A0 = 34;
            break;
        }
        if (word_A826 > 8) {
            state_A8A0 = 32;
            break;
        }
        posx_A800 += word_A2AA * 10;
        break;
    case 32:
        sub_496F(0);
        if (word_A83A++ < 8) {
            break;
        }
        word_A83A = 0;
        word_A2AA = -word_A2AA;
        index_A804 = 93;
        sub_4807(posx_A800, posy_A802, index_A804);
        state_A8A0 = 33;
        break;
    case 33:
        sub_496F(0);
        if (word_A83A++ < 15) {
            break;
        }
        word_A83A = 0;
        state_A8A0 = kStateNormal1;
        break;
    case 34:
        posx_A800 += word_A2AA * 8;
        if (index_A804 == 70) {
            index_A804 = 63;
        } else {
            index_A804 += 1;
        }
        sub_4807(posx_A800, posy_A802, index_A804);
        if (word_A82A == 2 && index_A804 == 70) {
            state_A8A0 = kStateRotate1;
            break;
        }
        if (posx_A800 > screenWidth_ || posx_A800 < -40) {
            state_A8A0 = kStateNormal1;
        }
        sub_4C21(word_A2AA * 8 + posx_A800, -(word_A2AA * 8 - posx_A800), 2);
        if (state_A8A0 == kStateHit1) {
            if (word_A846-- > 0) {
                word_A2AA = -word_A2AA;
                state_A8A0 = 34;
            } else {
                state_A8A0 = 34;
            }
        }
        sub_496F(2);
        break;
    case kStatePiss1:
        if (word_A83A++ < 1) {
            break;
        }
        word_A83A = 0;
        index_A804 = 3;
        sub_4807(posx_A800, posy_A802, index_A804);
        state_A8A0 = 37;
        word_A826 = 0;
        break;
    case 36:
        break;
    case 37:
        if (word_A83A++ < 1) {
            break;
        }
        word_A83A = 0;
        if (word_A2AA > 0) {
            index_A804 = word_A826 + 12;
        } else {
            index_A804 = 14 - word_A826;
        }
        sub_4807(posx_A800, posy_A802, index_A804);
        word_A826 += 1;
        if (word_A826 > 1) {
            index_A804 = 103;
            state_A8A0 = 38;
        }
        sub_496F(0);
        break;
    case 38:
        if (word_A83A++ < 1) {
            break;
        }
        word_A83A = 0;
        sub_4807(posx_A800, posy_A802, index_A804);
        index_A804 += 1;
        if (index_A804 > 104) {
            word_A826 = 0;
            state_A8A0 = 39;
            break;
        }
        sub_496F(0);
        break;
    case 39:
        if (word_A826 == 0) {
            if (word_A83A++ < 10) {
                break;
            }
            word_A83A = 0;
        } else {
            if (word_A83A++ < 1) {
                break;
            }
            word_A83A = 0;
        }
        if (word_A826 <= 8 || word_A826 >= 12 && word_A826 <= 12) {
            index_A804 = index_A804 == 105 ? 106 : 105;
        } else {
            index_A804 = 104;
        }
        sub_4807(posx_A800, posy_A802, index_A804);
        if (word_A826++ > 15) {
            state_A8A0 = 40;
            index_A804 = 104;
        }
        sub_496F(0);
        break;
    case 40:
        if (word_A83A++ < 1) {
            break;
        }
        word_A83A = 0;
        sub_4807(posx_A800, posy_A802, index_A804);
        if (--index_A804 < 103) {
            word_A826 = 0;
            state_A8A0 = 41;
            break;
        }
        sub_496F(0);
        break;
    case 41:
        if (word_A83A++ < 1) {
            break;
        }
        word_A83A = 0;
        if (word_A2AA > 0) {
            index_A804 = 13 - word_A826;
        } else {
            index_A804 = word_A826 + 13;
        }
        sub_4807(posx_A800, posy_A802, index_A804);
        word_A826 += 1;
        if (word_A826 > 1) {
            state_A8A0 = 42;
        }
        sub_496F(0);
        break;
    case 42:
        sub_496F(0);
        if (word_A83A++ < 1) {
            break;
        }
        word_A83A = 0;
        index_A804 = 3;
        sub_4807(posx_A800, posy_A802, index_A804);
        state_A8A0 = kStateNormal1;
        word_A826 = 0;
        break;
    case kStateYawn1:
        sound_42C8(IDW_YAWN, 0U, 0);
        word_A826 = 0;
        state_A8A0 = 44;
    case 44:
        if (word_A83A++ < 1) {
            break;
        }
        word_A83A = 0;
        index_A804 = word_A34C[word_A826];
        word_A826 += 1;
        if (index_A804 == 0) {
            state_A8A0 = kStateNormal1;
            break;
        }
        sub_4807(posx_A800, posy_A802, index_A804);
        sub_496F(0);
        break;
    case kStateBaa1:
        sound_42C8(IDW_BAA, 0U, 0);
        word_A826 = 0;
        state_A8A0 = 46;
    case 46:
        if (word_A83A++ < 0) {
            break;
        }
        word_A83A = 0;
        index_A804 = word_A362[word_A826];
        word_A826 += 1;
        if (index_A804 == 0) {
            state_A8A0 = kStateNormal1;
            break;
        }
        sub_4807(posx_A800, posy_A802, index_A804);
        sub_496F(0);
        break;
    case kStateSneeze1:
        word_A826 = 0;
        state_A8A0 = 48;
    case 48:
        if (word_A83A++ < 0) {
            break;
        }
        word_A83A = 0;
        if (word_A826 == 2) {
            sound_42C8(IDW_SNEEZE, 0U, 0);
        }
        index_A804 = word_A372[word_A826];
        word_A826 += 1;
        if (index_A804 == 0) {
            state_A8A0 = kStateNormal1;
            break;
        }
        sub_4807(posx_A800, posy_A802, index_A804);
        sub_496F(0);
        break;
    case kStateSuprise1:
        word_A826 = 0;
        state_A8A0 = 50;
    case 50:
        if (word_A83A++ < 1) {
            break;
        }
        word_A83A = 0;
        index_A804 = word_A38C[word_A826];
        word_A826 += 1;
        if (index_A804 == 0) {
            state_A8A0 = kStateNormal1;
            break;
        }
        sub_4807(posx_A800, posy_A802, index_A804);
        sub_496F(0);
        break;
    case kStateScary1:
        word_A826 = 0;
        state_A8A0 = 52;
    case 52:
        if (word_A83A++ < 0) {
            break;
        }
        word_A83A = 0;
        index_A804 = index_A804 == 56 ? 57 : 56;
        if (word_A826++ > 30) {
            index_A804 = 3;
            state_A8A0 = kStateNormal1;
        }
        sub_4807(posx_A800, posy_A802, index_A804);
        sub_496F(0);
        break;
    case kStateEat1:
        word_A826 = 0;
        state_A8A0 = 54;
        showSubPoo();
        word_A2AC = word_A2AA;
        word_A810 = posy_A802;
        word_A812 = 149;
        if (word_A2AA > 0) {
            word_A80E = posx_A800 - 40;
        } else {
            word_A80E = posx_A800 + 40;
        }
        sub_488C(word_A80E, word_A810, word_A812);
        word_C0AE = 1;
        sub_2B01(hOtherPooWnd_[kISubPooHwnd], hPooWnd_);
        break;
    case 54:
        if (word_A83A++ < 2) {
            break;
        }
        word_A83A = 0;
        index_A804 = word_A398[word_A826];
        word_A826 += 1;
        if (index_A804 == 2) {
            posx_A800 -= word_A2AA * 8;
            sub_4807(posx_A800, posy_A802, index_A804);
            break;
        }
        if (index_A804 >= 149 && index_A804 <= 153) {
            word_A812 = index_A804;
            if (word_A812 == 153) {
                word_A812 = 173;
            }
            sub_488C(word_A80E, word_A810, word_A812);
            index_A804 = word_A398[word_A826];
            word_A826 += 1;
        }
        if (index_A804 == 0) {
            destroySubPooWnd();
            state_A8A0 = kStateNormal1;
            break;
        }
        sub_4807(posx_A800, posy_A802, index_A804);
        break;
    case 55:
        break;
        if (word_A83A++ < 1) {
            break;
        }
        word_A83A = 0;
        posx_A800 -= word_A2AA * 6;
        index_A804 = 2;
        sub_4807(posx_A800, posy_A802, index_A804);
        sub_4559();
        sub_46D2();
        sub_496F(1);
        sub_4C21(-(word_A2AA * 6 - posx_A800), word_A2AA * 6 + posx_A800, 1);
        state_A8A0 = 54;
        break;
    case 56:
        word_A826 = 0;
        state_A8A0 = 57;
    case 57:
        if (word_A83A++ < 2) {
            break;
        }
        word_A83A = 0;
        index_A804 = word_A398[word_A826];
        word_A826 += 1;
        if (index_A804 >= 149 && index_A804 <= 153) {
            index_A804 = word_A398[word_A826];
            word_A826 += 1;
        }
        if (word_A826 >= 16) {
            state_A8A0 = 42;
            break;
        }
        sub_4807(posx_A800, posy_A802, index_A804);
        sub_496F(0);
        break;
    case kStateSit1:
        if (word_A83A++ < 1) {
            break;
        }
        word_A83A = 0;
        index_A804 = 3;
        sub_4807(posx_A800, posy_A802, index_A804);
        state_A8A0 = 59;
        word_A826 = 0;
        break;
    case 59:
        if (word_A83A++ < 1) {
            break;
        }
        word_A83A = 0;
        if (word_A2AA > 0) {
            index_A804 = word_A826 + 9;
        } else {
            index_A804 = 11 - word_A826;
        }
        word_A826 += 1;
        if (word_A826 > 2) {
            index_A804 = 34;
            word_A83A = -10;
            state_A8A0 = 60;
            word_A826 = 0;
        }
        sub_4807(posx_A800, posy_A802, index_A804);
        sub_496F(0);
        break;
    case 60:
        if (word_A83A++ < 0) {
            break;
        }
        word_A83A = 0;
        index_A804 = word_A2B4[5][word_A826];
        sub_4807(posx_A800, posy_A802, index_A804);
        word_A826 += 1;
        if (word_A826 > 7) {
            word_A826 = 0;
            state_A8A0 = 61;
            word_A83A = -5;
        }
        sub_496F(0);
        break;
    case 61:
        if (word_A83A++ < 1) {
            break;
        }
        word_A83A = 0;
        if (word_A2AA > 0) {
            index_A804 = 10 - word_A826;
        } else {
            index_A804 = word_A826 + 10;
        }
        sub_4807(posx_A800, posy_A802, index_A804);
        word_A826 += 1;
        if (word_A826 > 1) {
            state_A8A0 = 42;
        }
        sub_496F(0);
        break;
    case 64:
        break;
    case kStateRoll1:
        index_A804 = 3;
        sub_4807(posx_A800, posy_A802, index_A804);
        word_A826 = 0;
        state_A8A0 = 66;
        break;
    case 66:
        if (word_A83A++ < 1) {
            break;
        }
        word_A83A = 0;
        if (word_A826 == 0) {
            if (word_A2AA > 0) {
                index_A804 = 9;
            } else {
                index_A804 = 11;
            }
        } else {
            index_A804 = 10;
        }
        sub_4807(posx_A800, posy_A802, index_A804);
        if (word_A826++ > 0) {
            state_A8A0 = 67;
            word_A828 = (rand() % 4 + 4) * 8;
            word_A826 = 0;
            break;
        }
        sub_496F(0);
        break;
    case 67:
        if (--word_A826 < 0) {
            word_A826 = 79;
        }
        posx_A800 -= word_A2AA * 8;
        index_A804 = word_A524[word_A826 % 8];
        sub_4807(posx_A800, posy_A802, index_A804);
        if (word_A2AA > 0 && posx_A800 < 0) {
            state_A8A0 = kStateHit1;
        }
        if (word_A2AA < 0 && screenWidth_ - sprites_[index_A804].width < posx_A800) {
            state_A8A0 = kStateHit1;
        }
        if (--word_A828 <= 0) {
            state_A8A0 = 68;
            word_A826 = 0;
        }
        sub_496F(2);
        sub_4C21(-(word_A2AA * 8 - posx_A800), word_A2AA * 8 + posx_A800, 2);
        break;
    case 68:
        if (word_A83A++ < 1) {
            break;
        }
        word_A83A = 0;
        if (word_A826 == 1) {
            if (word_A2AA > 0) {
                index_A804 = 9;
            } else {
                index_A804 = 11;
            }
        } else if (word_A826 == 0) {
            index_A804 = 10;
        } else {
            index_A804 = 3;
        }
        sub_4807(posx_A800, posy_A802, index_A804);
        if (word_A826++ > 1) {
            state_A8A0 = kStateNormal1;
            break;
        }
        sub_496F(0);
        break;
    case kStateBlush1:
        state_A8A0 = 63;
        word_A826 = 0;
        break;
    case 63:
        if (word_A83A++ < 2) {
            break;
        }
        word_A83A = 0;
        index_A804 = word_A50C[word_A826];
        if (index_A804 == 0) {
            state_A8A0 = kStateNormal1;
            break;
        }
        sub_4807(posx_A800, posy_A802, index_A804);
        word_A826 += 1;
        sub_496F(0);
        break;
    case kStateSlip1:
        word_A826 = rand() % 8 + 8;
        word_A828 = word_A826;
        index_A804 = 131;
        if (word_A2AA > 0) {
            index_A804 = 12;
        } else {
            index_A804 = 14;
        }
        sub_4807(posx_A800, posy_A802, index_A804);
        state_A8A0 = 76;
        break;
    case 76:
        if (word_A83A++ < 1) {
            break;
        }
        word_A83A = 0;
        index_A804 = 13;
        sub_4807(posx_A800, posy_A802, index_A804);
        state_A8A0 = 77;
        break;
    case 77:
        if (word_A83A++ < 2) {
            break;
        }
        word_A83A = 0;
        index_A804 = index_A804 == 131 ? 132 : 131;
        posy_A802 -= 8;
        sub_4807(posx_A800, posy_A802, index_A804);
        if (word_A826-- <= 0) {
            word_A826 = word_A828;
            state_A8A0 = 78;
        }
        break;
    case 78:
        index_A804 = 133;
        posy_A802 += 8;
        sub_4807(posx_A800, posy_A802, index_A804);
        if (word_A826-- <= 0) {
            state_A8A0 = 79;
        }
        break;
    case 79:
        if (word_A83A++ < 10) {
            break;
        }
        word_A83A = 0;
        state_A8A0 = 80;
        word_A826 = 3;
        break;
    case 80:
        if (word_A83A++ < 1) {
            break;
        }
        word_A83A = 0;
        if (word_A2AA > 0) {
            index_A804 = word_A43C[word_A826];
            word_A826 += 1;
        } else {
            index_A804 = word_A44C[word_A826];
            word_A826 += 1;
        }
        if (index_A804 == 0) {
            state_A8A0 = kStateNormal1;
            break;
        }
        sub_4807(posx_A800, posy_A802, index_A804);
        break;
    case kStateRotate1:
        word_A83A = 0;
        word_A826 = 0;
        state_A8A0 = 70;
    case 70:
        if (word_A2AA > 0) {
            index_A804 = word_A536[word_A826 % 8];
        } else {
            index_A804 = word_A536[(word_A826 + 4) % 8];
        }
        if (index_A804 == 2) {
            index_A804 = 3;
            if (word_A2AA > 0) {
                word_A2AA = -word_A2AA;
                sub_4807(posx_A800, posy_A802, index_A804);
                word_A2AA = -word_A2AA;
            } else {
                sub_4807(posx_A800, posy_A802, index_A804);
            }
        } else if (index_A804 == 3) {
            if (word_A2AA < 0) {
                word_A2AA = -word_A2AA;
                sub_4807(posx_A800, posy_A802, index_A804);
                word_A2AA = -word_A2AA;
            } else {
                sub_4807(posx_A800, posy_A802, index_A804);
            }
        } else {
            sub_4807(posx_A800, posy_A802, index_A804);
        }
        if (word_A826++ >= 16) {
            index_A804 = 70;
            sub_4807(posx_A800, posy_A802, index_A804);
            state_A8A0 = 71;
        }
        sub_496F(0);
        break;
    case 71:
        sub_496F(0);
        if (word_A83A++ < 14) {
            break;
        }
        word_A83A = 0;
        index_A804 = 96;
        sub_4807(posx_A800, posy_A802, index_A804);
        state_A8A0 = 72;
        break;
    case 72:
        sub_496F(0);
        if (word_A83A++ < 30) {
            break;
        }
        word_A83A = 0;
        index_A804 = 3;
        sub_4807(posx_A800, posy_A802, index_A804);
        state_A8A0 = kStateNormal1;
        break;
    case 73:
        word_A826 = 0;
        state_A8A0 = 74;
    case 74:
        if (word_A83A++ < 2) {
            break;
        }
        word_A83A = 0;
        index_A804 = word_A422[word_A826];
        word_A826 += 1;
        if (index_A804 == 0) {
            state_A8A0 = kStateNormal1;
            break;
        }
        sub_4807(posx_A800, posy_A802, index_A804);
        sub_496F(0);
        break;
    case 81:
        index_A804 = 4;
        sub_4807(posx_A800, posy_A802, index_A804);
        state_A8A0 = 82;
        break;
    case 82:
        word_A83A = 0;
        index_A804 = index_A804 == 4 ? 5 : 4;
        sub_4807(posx_A800, posy_A802, index_A804);
        break;
    case 83:
        break;
    case 84:
        state_A8A0 = 0;
        break;
    case 85:
        word_A81C = GetActiveWindow();
        if (word_A81C == hPooWnd_ || word_A81C == hOtherPooWnd_[kISubPooHwnd] || word_A81C == NULL || sub_39D6(word_A81C)) {
            state_A8A0 = kStateNormal1;
            break;
        }
        sub_491D(word_A81C, &stru_A81E);
        if (stru_A81E.top < 10) {
            state_A8A0 = kStateNormal1;
            break;
        }
        if (word_A2AA > 0 && stru_A81E.right < posx_A800 && stru_A81E.top < posy_A802 && posy_A802 + 40 < stru_A81E.bottom || word_A2AA < 0 && posx_A800 + 40 < stru_A81E.left && stru_A81E.top < posy_A802 && posy_A802 + 40 < stru_A81E.bottom) {
            state_A8A0 = 87;
            break;
        }
        word_A83C = (rand() % stru_A81E.right - stru_A81E.left) / 3 + (stru_A81E.right - stru_A81E.left) / 2 + stru_A81E.left - 20;
        word_A83E = stru_A81E.top - 40;
        if (screenWidth_ / 2 - 20 > posx_A800) {
            word_A2AA = 1;
        } else {
            word_A2AA = -1;
        }
        index_A804 = 4;
        sub_4807(posx_A800, posy_A802, index_A804);
        state_A8A0 = 86;
        break;
    case 86:
        if (word_A83A++ < 1) {
            break;
        }
        word_A83A = 0;
        posx_A800 -= word_A2AA * 16;
        index_A804 = index_A804 == 4 ? 5 : 4;
        sub_4807(posx_A800, posy_A802, index_A804);
        if (posx_A800 < -40 || posx_A800 > screenWidth_) {
            if (!sub_48F3(word_A81C)) {
                state_A8A0 = kStateNormal1;
                break;
            }
            if (rand() % 3 == 0) {
                state_A8A0 = 3;
                goto loc_4D33;
            }
            word_A840 = 0;
            state_A8A0 = 92;
            word_A7FC = 1;
            posx_A800 = word_A83C;
            posy_A802 = -40;
            word_A806 = 0;
            word_A808 = 0;
            word_A842 = rand() % 2;
            if (word_A842 != 0) {
                word_A808 = -(word_A2AA * 3);
            }
            sub_2B01(hPooWnd_, word_A81C);
        }
        break;
    case 87:
        sub_2B01(hPooWnd_, word_A81C);
        if (word_A2AA > 0) {
            word_A83C = stru_A81E.right;
            word_A83E = stru_A81E.top;
        } else {
            word_A83C = stru_A81E.left - 40;
            word_A83E = stru_A81E.top;
        }
        state_A8A0 = 88;
    case 88:
        if (word_A83A++ < 1) {
            break;
        }
        word_A83A = 0;
        posx_A800 -= word_A2AA * 16;
        index_A804 = index_A804 == 4 ? 5 : 4;
        if (word_A83C >= posx_A800 && word_A2AA > 0 || word_A83C <= posx_A800 && word_A2AA < 0) {
            if (!sub_48F3(word_A81C)) {
                state_A8A0 = kStateNormal1;
                break;
            }
            sub_491D(word_A81C, &var_10);
            if (var_10.left == stru_A81E.left && var_10.right == stru_A81E.right && var_10.top < posy_A802 && posy_A802 + 40 < var_10.bottom) {
                word_A83E = var_10.top - 12;
                word_A7FC = 1;
                posx_A800 = word_A83C;
                index_A804 = 30;
                state_A8A0 = 89;
                break;
            } else {
                state_A8A0 = kStateNormal1;
                break;
            }
        }
        sub_4807(posx_A800, posy_A802, index_A804);
        break;
    case 89:
        if (word_A83A++ < 1) {
            break;
        }
        word_A83A = 0;
        sub_4807(posx_A800, posy_A802, index_A804);
        posy_A802 -= 6;
        index_A804 = index_A804 == 15 ? 16 : 15;
        if (word_A83E >= posy_A802) {
            state_A8A0 = 90;
            break;
        }
        sub_4B3B();
        break;
    case 90:
        if (word_A83A++ < 2) {
            break;
        }
        word_A83A = 0;
        posx_A800 -= word_A2AA * 8;
        posy_A802 = word_A83E - 20;
        index_A804 = 76;
        sub_4807(posx_A800, posy_A802, index_A804);
        state_A8A0 = 91;
        break;
    case 91:
        if (word_A83A++ < 2) {
            break;
        }
        word_A83A = 0;
        posx_A800 += word_A2AA * -24;
        posy_A802 -= 8;
        index_A804 = 3;
        sub_4807(posx_A800, posy_A802, index_A804);
        state_A8A0 = kStateWalk1;
        break;
    case 92:
        word_A806 += 4;
        word_A80C = posy_A802;
        posx_A800 += word_A808;
        posy_A802 += word_A806;
        if ((var_4 = sub_419E(word_A81C, sprites_[index_A804].height + posy_A802, sprites_[index_A804].height + word_A80C, posx_A800, sprites_[index_A804].width + posx_A800)) != 0) {
            if (var_4 == -1) {
                sub_4807(posx_A800, posy_A802, index_A804);
                state_A8A0 = 0;
                break;
            }
            posy_A802 = var_4 - sprites_[index_A804].height;
            if (word_A806 < 64 && word_A840 == 0 || word_A806 < 8) {
                SetWindowPos(hPooWnd_, word_A81C, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE);
                word_A840 = 0;
                word_A826 = 0;
                state_A8A0 = 93;
                if (word_A806 < 36) {
                    index_A804 = 49;
                    word_A83A = -4;
                } else {
                    if ((rand() & 3) == 0) {
                        index_A804 = 48;
                    } else {
                        index_A804 = 42;
                    }
                    word_A83A = -12;
                }
                sub_4807(posx_A800, posy_A802, index_A804);
                break;
            } else {
                word_A806 = word_A806 * 2 / -3;
                word_A840 = 1;
            }
        }
        if (word_A842 != 0) {
            index_A804 = index_A804 == 4 ? 5 : 4;
        } else {
            index_A804 = 42;
        }
        sub_4807(posx_A800, posy_A802, index_A804);
        break;
    case 93:
        if (word_A83A++ < 1) {
            break;
        }
        word_A83A = 0;
        if (word_A842 != 0) {
            state_A8A0 = kStateWalk1;
            index_A804 = 2;
            break;
        }
        if (word_A826 == 0) {
            index_A804 = 13;
        } else if (word_A826 == 1) {
            if (word_A2AA > 0) {
                index_A804 = 12;
            } else {
                index_A804 = 14;
            }
        } else if (word_A826 == 2) {
            index_A804 = 3;
        }
        sub_4807(posx_A800, posy_A802, index_A804);
        if (word_A826++ >= 2) {
            state_A8A0 = kStateWalk1;
        }
        break;
    case 94:
        word_A7FC = 1;
        word_A806 = 0;
        word_A808 = -(word_A2AA * 8);
        word_A842 = 1;
        state_A8A0 = 99;
        goto loc_4D33;
    case 95:
        word_A7FC = 1;
        word_A806 = 0;
        word_A808 = -(word_A2AA * 3);
        word_A842 = 1;
        state_A8A0 = 99;
        goto loc_4D33;
    case kStateFall1:
        word_A7FC = 1;
        word_A806 = 0;
        word_A808 = 0;
        word_A842 = 0;
        state_A8A0 = 99;
        goto loc_4D33;
    case 97:
        word_A7FC = 1;
        word_A806 = 0;
        word_A808 = 0;
        word_A842 = 1;
        state_A8A0 = 99;
        goto loc_4D33;
    case 98:
        word_A7FC = 1;
        word_A806 = 0;
        word_A808 = 0;
        word_A842 = 2;
        state_A8A0 = 99;
        goto loc_4D33;
    case 99:
        sub_3DF0();
        word_A806 += 4;
        word_A80C = posy_A802;
        posx_A800 += word_A808;
        posy_A802 += word_A806;
        if (word_A80C > screenHeight_) {
            sub_4807(posx_A800, posy_A802, index_A804);
            state_A8A0 = 0;
            break;
        }
        if ((var_4 = sub_408C(&word_A81C, sprites_[index_A804].height + posy_A802, sprites_[index_A804].height + word_A80C, posx_A800, sprites_[index_A804].width + posx_A800)) != 0) {
            if (!sub_48F3(word_A81C)) {
                sub_4807(posx_A800, posy_A802, index_A804);
                state_A8A0 = 0;
                break;
            }
            sub_491D(word_A81C, &stru_A81E);
            posy_A802 = var_4 - sprites_[index_A804].height;
            if (word_A842 == 3) {
                index_A804 = 66;
                sub_4807(posx_A800, posy_A802, index_A804);
                state_A8A0 = 29;
                break;
            }
            if (word_A806 < 64 && word_A840 == 0 || word_A806 < 8) {
                if (word_A81C != NULL) {
                    SetWindowPos(hPooWnd_, word_A81C, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE);
                }
                word_A840 = 0;
                word_A826 = 0;
                state_A8A0 = 100;
                if (word_A806 < 36) {
                    index_A804 = 49;
                    word_A83A = -4;
                } else {
                    if ((rand() & 3) == 0) {
                        index_A804 = 48;
                    } else {
                        index_A804 = 42;
                    }
                    word_A83A = -10;
                }
                if (word_A842 == 2) {
                    if (word_A806 < 36) {
                        index_A804 = 41;
                    } else {
                        index_A804 = 45;
                    }
                }
                sub_4807(posx_A800, posy_A802, index_A804);
                break;
            } else {
                if ((rand() & 7) == 0 && word_A840 == 0) {
                    word_A840 = 0;
                    word_A826 = 0;
                    state_A8A0 = 100;
                    index_A804 = 48;
                    word_A83A = -12;
                    if (word_A842 == 2) {
                        index_A804 = 45;
                    }
                    sub_4807(posx_A800, posy_A802, index_A804);
                    break;
                }
                word_A806 = word_A806 * 2 / -3;
                word_A840 = 1;
            }
        }
        if (word_A842 == 2) {
            index_A804 = index_A804 == 40 ? 41 : 40;
        } else if (word_A842 == 1) {
            index_A804 = index_A804 == 4 ? 5 : 4;
        } else if (word_A842 == 0) {
            index_A804 = 42;
        } else {
            index_A804 = word_A324[word_A848];
            word_A848 += 1;
            if (index_A804 == 66) {
                word_A848 -= 1;
            }
        }
        if (word_A842 == 3 && sub_4C91(posx_A800, posx_A800 - word_A808) != 0) {
            word_A2AA = -word_A2AA;
            state_A8A0 = kStateHit1;
            break;
        }
        sub_4807(posx_A800, posy_A802, index_A804);
        break;
    case 100:
        if (word_A83A++ < 1) {
            break;
        }
        word_A83A = 0;
        if (word_A842 == 1) {
            state_A8A0 = kStateWalk1;
            index_A804 = 2;
            break;
        }
        if (word_A842 == 2) {
            word_A826 = 0;
            state_A8A0 = 101;
            break;
        }
        if (word_A826 == 0) {
            index_A804 = 13;
        } else if (word_A826 == 1) {
            if (word_A2AA > 0) {
                index_A804 = 12;
            } else {
                index_A804 = 14;
            }
        } else if (word_A826 == 2) {
            index_A804 = 3;
        }
        sub_4807(posx_A800, posy_A802, index_A804);
        if (word_A826++ >= 2) {
            state_A8A0 = kStateWalk1;
        }
        break;
    case 101:
        if (word_A83A++ < 1) {
            break;
        }
        word_A83A = 0;
        if (word_A826 == 0) {
            index_A804 = 31;
            word_A83A = -8;
        } else if (word_A826 == 2) {
            index_A804 = 3;
        }
        sub_4807(posx_A800, posy_A802, index_A804);
        if (word_A826++ >= 6) {
            state_A8A0 = kStateWalk1;
        }
        break;
    case 102:
        stopPlaySound();
        word_A826 = 6;
        index_A804 = 3;
        word_A82A = 0;
        if (rand() % 3 == 0) {
            word_A82A = 1;
        }
        state_A8A0 = 103;
    case 103:
        if (word_A82A != 0) {
            index_A804 = index_A804 == 50 ? 51 : 50;
        } else {
            index_A804 = index_A804 == 4 ? 5 : 4;
        }
        sub_4807(posx_A800, posy_A802, index_A804);
        if (word_A826-- <= 0) {
            state_A8A0 = 97;
        }
        break;
    case 104:
        word_A842 = 0;
        state_A8A0 = 106;
        goto loc_4D33;
    case 105:
        word_A842 = 1;
        state_A8A0 = 106;
        goto loc_4D33;
    case 106:
        if (word_A842 == 0) {
            var_14.x = posx_A800;
            var_14.y = posy_A802 + 39;
            *(HWND *)&var_10 = WindowFromPoint(var_14);
            var_14.x = posx_A800 + 39;
            var_8 = WindowFromPoint(var_14);
            if (*(HWND *)&var_10 == hPooWnd_ && var_8 == hPooWnd_) {
                putWndToTop(hPooWnd_);
            } else if (*(HWND *)&var_10 == hPooWnd_) {
                sub_2B01(hPooWnd_, var_8);
            } else {
                sub_2B01(hPooWnd_, *(HWND *)&var_10);
            }
            index_A804 = 81;
        } else {
            index_A804 = 78;
        }
        sub_4807(posx_A800, posy_A802, index_A804);
        state_A8A0 = 107;
        word_A826 = 0;
        break;
    case 107:
        index_A804 = word_A2B4[4 - word_A842][word_A826];
        sub_4807(posx_A800, posy_A802, index_A804);
        word_A826 += 1;
        if (word_A826 > 7) {
            if (word_A842 != 0) {
                if ((rand() & 1) == 0) {
                    state_A8A0 = 111;
                } else {
                    state_A8A0 = 109;
                }
            } else {
                if ((rand() & 1) == 0) {
                    state_A8A0 = 111;
                } else {
                    state_A8A0 = 108;
                }
            }
        }
        break;
    case 108:
        if (word_A83A++ < 10) {
            break;
        }
        word_A83A = 0;
        index_A804 = 3;
        sub_4807(posx_A800, posy_A802, index_A804);
        state_A8A0 = kStateNormal1;
        break;
    case 109:
        word_A808 = -(word_A2AA * 14);
        index_A804 = 23;
        posx_A800 += word_A808;
        sub_4807(posx_A800, posy_A802, index_A804);
        state_A8A0 = 95;
        word_A826 = 0;
        break;
    case 110:
        posx_A800 += word_A808;
        word_A808 += word_A2AA;
        sub_4807(posx_A800, posy_A802, index_A804);
        if (word_A826++ > 3) {
            state_A8A0 = 95;
        }
        break;
    case 111:
        if (word_A842 != 0) {
            posx_A800 += word_A2AA * -26;
            posy_A802 += 35;
            word_A2AA = -word_A2AA;
        } else {
            word_A82A = rand() % 2;
            if (word_A82A != 0) {
                posy_A802 += 36;
            } else {
                posy_A802 += 20;
            }
        }
        word_A826 = 0;
        state_A8A0 = 112;
    case 112:
        if (word_A826 == 0) {
            if (word_A83A++ < 10) {
                break;
            }
            word_A83A = 0;
        } else {
            if (word_A83A++ < 1) {
                break;
            }
            word_A83A = 0;
        }
        if (word_A842 != 0) {
            index_A804 = index_A804 == 40 ? 41 : 40;
        } else {
            index_A804 = word_A314[word_A82A][word_A826 % 4];
        }
        sub_4807(posx_A800, posy_A802, index_A804);
        word_A826 += 1;
        if (word_A826 > 12) {
            if (word_A842 != 0) {
                state_A8A0 = 98;
            } else {
                state_A8A0 = kStateFall1;
            }
        }
        break;
    case 113:
        word_CA76 = 1;
        index_A804 = 6;
        sub_4807(posx_A800, posy_A802, index_A804);
        state_A8A0 = 114;
        break;
    case 114:
        if (word_A83A++ < 1) {
            break;
        }
        word_A83A = 0;
        index_A804 += 1;
        sub_4807(posx_A800, posy_A802, index_A804);
        if (index_A804 == 8) {
            index_A804 = 0;
            state_A8A0 = 115;
        }
        sub_496F(0);
        break;
    case 115:
        sub_496F(0);
        if (word_A83A++ < 4) {
            break;
        }
        word_A83A = 0;
        index_A804 = index_A804 == 0 ? 1 : 0;
        sub_4807(posx_A800, posy_A802, index_A804);
        break;
    case kStateMerry11:
        posx_A800 = screenWidth_;
        posy_A802 = screenHeight_ * 7 / 8;
        index_A804 = 4;
        word_A2AA = 1;
        sub_4807(posx_A800, posy_A802, index_A804);
        state_A8A0 = 117;
        break;
    case 117:
        if (word_A83A++ < 1) {
            break;
        }
        word_A83A = 0;
        posx_A800 -= word_A2AA * 16;
        index_A804 = index_A804 == 4 ? 5 : 4;
        sub_4807(posx_A800, posy_A802, index_A804);
        if (screenWidth_ / 2 - 20 >= posx_A800) {
            state_A8A0 = 118;
        }
        break;
    case 118:
        showSubPoo();
        word_A2AC = -1;
        word_A80E = -40;
        word_A810 = screenHeight_ / 8;
        word_A812 = 154;
        word_A826 = 0;
        state_A8A0 = 119;
        word_C0AE = 0;
        putWndToTop(hPooWnd_);
        putWndToTop(hOtherPooWnd_[kISubPooHwnd]);
        break;
    case 119:
        if (word_A826 != 0) {
            index_A804 = word_A2B4[2][word_A826];
            sub_4807(posx_A800, posy_A802, index_A804);
            word_A826 += 1;
            if (word_A826 > 7) {
                word_A826 = 0;
            }
        } else {
            index_A804 = 73;
            sub_4807(posx_A800, posy_A802, index_A804);
            if (rand() % 20 == 0) {
                word_A826 = 1;
            }
        }
        if (word_A83A++ < 1) {
            break;
        }
        word_A83A = 0;
        word_A80E -= word_A2AC * 16;
        word_A812 = word_A812 == 154 ? 155 : 154;
        sub_488C(word_A80E, word_A810, word_A812);
        if (word_A80E > posx_A800) {
            word_A2AA = -1;
            sub_4807(posx_A800, posy_A802, index_A804);
        }
        if (word_A80E > screenWidth_) {
            destroySubPooWnd();
            state_A8A0 = 120;
        }
        break;
    case 120:
        if (word_A83A++ < 1) {
            break;
        }
        word_A83A = 0;
        posx_A800 -= word_A2AA * 16;
        index_A804 = index_A804 == 4 ? 5 : 4;
        sub_4807(posx_A800, posy_A802, index_A804);
        if (posx_A800 > screenWidth_) {
            state_A8A0 = kStateNormal1;
        }
        break;
    case kStateMerry21:
        posx_A800 = screenWidth_;
        posy_A802 = screenHeight_ * 7 / 8;
        index_A804 = 4;
        word_A2AA = 1;
        showSubPoo();
        word_A2AC = -1;
        word_A80E = -40;
        word_A810 = screenHeight_ * 7 / 8;
        word_A812 = 154;
        sub_4807(posx_A800, posy_A802, index_A804);
        sub_488C(word_A80E, word_A810, word_A812);
        state_A8A0 = 122;
        word_C0AE = 0;
        putWndToTop(hPooWnd_);
        putWndToTop(hOtherPooWnd_[kISubPooHwnd]);
        break;
    case 122:
        if (word_A83A++ < 1) {
            break;
        }
        word_A83A = 0;
        posx_A800 -= word_A2AA * 16;
        index_A804 = index_A804 == 4 ? 5 : 4;
        word_A80E -= word_A2AC * 16;
        word_A812 = word_A812 == 154 ? 155 : 154;
        if (posx_A800 - word_A80E <= 46) {
            posx_A800 = screenWidth_ / 2 + 3;
            word_A80E = screenWidth_ / 2 - 43;
            index_A804 = 3;
            word_A812 = 157;
            sub_4807(posx_A800, posy_A802, index_A804);
            sub_488C(word_A80E, word_A810, word_A812);
            word_A826 = 0;
            state_A8A0 = 123;
        } else {
            sub_4807(posx_A800, posy_A802, index_A804);
            sub_488C(word_A80E, word_A810, word_A812);
        }
        break;
    case 123:
        if (word_A83A++ < 3) {
            break;
        }
        word_A83A = 0;
        index_A804 = word_A826 + 127;
        word_A826 += 1;
        sub_4807(posx_A800, posy_A802, index_A804);
        if (word_A826 >= 4) {
            state_A8A0 = 124;
        }
        break;
    case 124:
        if (word_A83A++ < 4) {
            break;
        }
        word_A83A = 0;
        word_CA46 += 1;
        if (word_CA46 > 8) {
            destroySubPooWnd();
            word_A826 = 0;
            state_A8A0 = 125;
        }
        break;
    case 125:
        index_A804 = word_A45C[word_A826];
        word_A826 += 1;
        if (index_A804 == 0) {
            state_A8A0 = kStateNormal1;
            break;
        }
        sub_4807(posx_A800, posy_A802, index_A804);
        break;
    case kStateMerry31:
        posx_A800 = screenWidth_;
        posy_A802 = screenHeight_ * 7 / 8;
        index_A804 = 4;
        word_A2AA = 1;
        showSubPoo();
        word_A2AC = 1;
        word_A80E = screenWidth_ + 46;
        word_A810 = screenHeight_ * 7 / 8;
        word_A812 = 154;
        sub_4807(posx_A800, posy_A802, index_A804);
        sub_488C(word_A80E, word_A810, word_A812);
        state_A8A0 = 127;
        word_C0AE = 0;
        putWndToTop(hPooWnd_);
        putWndToTop(hOtherPooWnd_[kISubPooHwnd]);
        break;
    case 127:
        if (word_A83A++ < 1) {
            break;
        }
        word_A83A = 0;
        posx_A800 -= word_A2AA * 16;
        index_A804 = index_A804 == 4 ? 5 : 4;
        word_A80E -= word_A2AC * 16;
        word_A812 = word_A812 == 154 ? 155 : 154;
        if (word_A80E < -40) {
            destroySubPooWnd();
            state_A8A0 = kStateNormal1;
        } else {
            sub_4807(posx_A800, posy_A802, index_A804);
            sub_488C(word_A80E, word_A810, word_A812);
        }
        break;
    case kStateUFO11:
        posx_A800 = screenWidth_;
        posy_A802 = screenHeight_ * 7 / 8;
        index_A804 = 4;
        word_A2AA = 1;
        sub_4807(posx_A800, posy_A802, index_A804);
        state_A8A0 = 129;
        break;
    case 129:
        if (word_A83A++ < 1) {
            break;
        }
        word_A83A = 0;
        posx_A800 -= word_A2AA * 16;
        index_A804 = index_A804 == 4 ? 5 : 4;
        sub_4807(posx_A800, posy_A802, index_A804);
        if (screenWidth_ / 2 - 20 >= posx_A800) {
            state_A8A0 = 130;
        }
        break;
    case 130:
        showSubPoo();
        word_A2AC = -1;
        word_A80E = -40;
        word_A810 = screenHeight_ / 8;
        word_A812 = 158;
        word_A826 = 0;
        state_A8A0 = 131;
        word_C0AE = 0;
        putWndToTop(hPooWnd_);
        putWndToTop(hOtherPooWnd_[kISubPooHwnd]);
        break;
    case 131:
        if (word_A826 != 0) {
            index_A804 = word_A2B4[2][word_A826];
            sub_4807(posx_A800, posy_A802, index_A804);
            word_A826 += 1;
            if (word_A826 > 7) {
                word_A826 = 0;
            }
        } else {
            index_A804 = 73;
            sub_4807(posx_A800, posy_A802, index_A804);
            if (rand() % 20 == 0) {
                word_A826 = 1;
            }
        }
        word_A80E -= word_A2AC * 16;
        if (word_A812 == 161) {
            word_A812 = 158;
        } else {
            word_A812 += 1;
        }
        if (word_A80E > posx_A800) {
            word_A80E = posx_A800;
            word_A812 = 162;
            state_A8A0 = 132;
        }
        sub_488C(word_A80E, word_A810, word_A812);
        break;
    case 132:
        index_A804 = 73;
        sub_4807(posx_A800, posy_A802, index_A804);
        word_CA5C += 40;
        if (posy_A802 - word_A810 - 40 <= word_CA5C) {
            word_CA5C = posy_A802 - word_A810 - 40;
            word_CA5C -= 20;
            state_A8A0 = 133;
        }
        sub_488C(word_A80E, word_A810, word_A812);
        if (word_A83A++ < 1) {
            break;
        }
        word_A83A = 0;
        if (word_A812 == 165) {
            word_A812 = 162;
        } else {
            word_A812 += 1;
        }
        break;
    case 133:
        word_CA5C -= 20;
        if (word_CA5C <= 0) {
            word_CA5C = 0;
            posy_A802 = word_A810 + 40;
            state_A8A0 = 134;
            index_A804 = index_A804 == 4 ? 5 : 4;
            sub_4807(posx_A800, posy_A802, index_A804);
            word_A812 = 158;
            sub_488C(word_A80E, word_A810, word_A812);
            break;
        }
        sub_488C(word_A80E, word_A810, word_A812);
        index_A804 = index_A804 == 4 ? 5 : 4;
        posy_A802 -= 20;
        sub_4807(posx_A800, posy_A802, index_A804);
        if (word_A83A++ < 1) {
            break;
        }
        word_A83A = 0;
        if (word_A812 == 165) {
            word_A812 = 162;
        } else {
            word_A812 += 1;
        }
        break;
    case 134:
        posx_A800 = -80;
        sub_4807(posx_A800, posy_A802, index_A804);
        word_A80E -= word_A2AC * 16;
        if (word_A812 == 161) {
            word_A812 = 158;
        } else {
            word_A812 += 1;
        }
        if (word_A80E > screenWidth_) {
            destroySubPooWnd();
            stopPlaySound();
            state_A8A0 = kStateNormal1;
            break;
        }
        sub_488C(word_A80E, word_A810, word_A812);
        break;
    case kStateUFO21:
        word_A810 = screenHeight_ * 7 / 8;
        word_A2AA = -1;
        posx_A800 = -40;
        posy_A802 = screenHeight_ / 8;
        index_A804 = 158;
        word_A826 = 0;
        state_A8A0 = 136;
        break;
    case 136:
        posx_A800 -= word_A2AA * 16;
        if (index_A804 == 161) {
            index_A804 = 158;
        } else {
            index_A804 += 1;
        }
        if (screenWidth_ / 2 - 20 < posx_A800) {
            posx_A800 = screenWidth_ / 2 - 20;
            index_A804 = 162;
            state_A8A0 = 137;
        }
        sub_4807(posx_A800, posy_A802, index_A804);
        break;
    case 137:
        word_CA72 += 40;
        if (word_A810 - posy_A802 - 40 <= word_CA72) {
            word_CA72 = word_A810 - posy_A802 - 40;
            state_A8A0 = 138;
        }
        sub_4807(posx_A800, posy_A802, index_A804);
        if (word_A83A++ < 1) {
            break;
        }
        word_A83A = 0;
        if (index_A804 == 165) {
            index_A804 = 162;
        } else {
            index_A804 += 1;
        }
        break;
    case 138:
        if (word_A83A++ < 4) {
            break;
        }
        word_A83A = 0;
        showSubPoo();
        word_A80E = posx_A800;
        word_A812 = 167;
        sub_488C(word_A80E, word_A810, word_A812);
        sub_4807(posx_A800, posy_A802, index_A804);
        state_A8A0 = 139;
        word_C0AE = 0;
        putWndToTop(hPooWnd_);
        putWndToTop(hOtherPooWnd_[kISubPooHwnd]);
        break;
    case 139:
        if (word_CA72 != 0) {
            word_CA72 -= 40;
            if (word_CA72 <= 0) {
                index_A804 = 158;
                word_CA72 = 0;
            }
            if (index_A804 == 165) {
                index_A804 = 162;
            } else {
                index_A804 += 1;
            }
        } else {
            posx_A800 -= word_A2AA * 16;
            if (index_A804 == 161) {
                index_A804 = 158;
            } else {
                index_A804 += 1;
            }
        }
        if (posx_A800 > screenWidth_) {
            state_A8A0 = 140;
        }
        sub_4807(posx_A800, posy_A802, index_A804);
        if (word_A83A++ < 1) {
            break;
        }
        word_A83A = 0;
        word_A812 = word_A812 == 167 ? 168 : 167;
        sub_488C(word_A80E, word_A810, word_A812);
        break;
    case 140:
        word_A812 = 166;
        sub_488C(word_A80E, word_A810, word_A812);
        if (word_A83A++ < 1) {
            break;
        }
        word_A83A = 0;
        word_CA46 += 1;
        if (word_CA46 > 8) {
            destroySubPooWnd();
            stopPlaySound();
            state_A8A0 = kStateNormal1;
        }
        break;
    case 141:
        break;
    case kStateUFO31:
        posx_A800 = -80;
        posy_A802 = screenHeight_ / 8;
        sub_4807(posx_A800, posy_A802, index_A804);
        showSubPoo();
        word_A2AC = -1;
        word_A80E = -40;
        word_A810 = screenHeight_ * 7 / 8;
        word_A812 = 158;
        word_A826 = 0;
        state_A8A0 = 143;
        word_C0AE = 0;
        putWndToTop(hPooWnd_);
        putWndToTop(hOtherPooWnd_[kISubPooHwnd]);
        break;
    case 143:
        word_A80E -= word_A2AC * 16;
        if (word_A812 == 161) {
            word_A812 = 158;
        } else {
            word_A812 += 1;
        }
        if (screenHeight_ / 8 < word_A80E) {
            word_A80E = screenHeight_ / 8;
            posx_A800 = screenWidth_;
            posy_A802 = word_A810;
            index_A804 = 4;
            word_A2AA = 1;
            state_A8A0 = 144;
        }
        sub_488C(word_A80E, word_A810, word_A812);
        break;
    case 144:
        if (word_A83A++ < 1) {
            break;
        }
        word_A83A = 0;
        if (word_A812 == 161) {
            word_A812 = 158;
        } else {
            word_A812 += 1;
        }
        sub_488C(word_A80E, word_A810, word_A812);
        posx_A800 -= word_A2AA * 16;
        index_A804 = index_A804 == 4 ? 5 : 4;
        sub_4807(posx_A800, posy_A802, index_A804);
        if (word_A80E + 40 >= posx_A800) {
            posx_A800 = -80;
            sub_4807(posx_A800, posy_A802, index_A804);
            state_A8A0 = 145;
        }
        break;
    case 145:
        word_A810 -= 40;
        if (word_A812 == 161) {
            word_A812 = 158;
        } else {
            word_A812 += 1;
        }
        if (word_A810 < -40) {
            destroySubPooWnd();
            stopPlaySound();
            state_A8A0 = kStateNormal1;
            break;
        }
        sub_488C(word_A80E, word_A810, word_A812);
        break;
    case 146:
        break;
    case kStateBurn1:
        showSubPoo();
        word_CA56 = 1;
        word_A2AC = 1;
        word_A812 = 146;
        word_A826 = 0;
        posx_A800 = screenWidth_;
        posy_A802 = -40;
        word_A2AA = 1;
        word_A808 = screenWidth_ / -96;
        word_A806 = screenHeight_ / 96;
        word_A80E = word_A808 * 92 + screenWidth_;
        word_A810 = word_A806 * 92 - 20;
        state_A8A0 = 148;
        word_C0AE = 1;
        putWndToTop(hOtherPooWnd_[kISubPooHwnd]);
        putWndToTop(hPooWnd_);
    case 148:
        if (word_A83A++ < 0) {
            break;
        }
        word_A83A = 0;
        sub_488C(word_A80E, word_A810, word_A812);
        posx_A800 += word_A808;
        posy_A802 += word_A806;
        index_A804 = word_A3DE[word_A826 / 3];
        word_A826 += 1;
        if (index_A804 == 0) {
            word_A826 -= 1;
        }
        if (index_A804 == 0 || index_A804 == 144 || index_A804 == 145) {
            index_A804 = index_A804 == 144 ? 145 : 144;
        }
        if (index_A804 == 137 || index_A804 == 138) {
            index_A804 = index_A804 == 137 ? 138 : 137;
        }
        sub_4807(posx_A800, posy_A802, index_A804);
        if (word_A80E + 10 > posx_A800 || word_A810 + 20 < posy_A802) {
            word_A826 = 0;
            state_A8A0 = 149;
            index_A804 = 173;
            sub_4807(posx_A800, posy_A802, index_A804);
            break;
        }
        break;
    case 149:
        if (word_A83A++ < 1) {
            break;
        }
        word_A83A = 0;
        posx_A800 = -80;
        sub_4807(posx_A800, posy_A802, index_A804);
        word_A812 = word_A494[word_A826];
        word_A826 += 1;
        if (word_A812 == 0) {
            posx_A800 = word_A80E;
            posy_A802 = word_A810;
            word_A826 = 0;
            sound_42C8(IDW_BAA, 0U, 0);
            state_A8A0 = 150;
            break;
        }
        sub_488C(word_A80E, word_A810, word_A812);
        break;
    case 150:
        index_A804 = 169;
        index_A804 = word_A49E[word_A826];
        word_A826 += 1;
        if (index_A804 == 0) {
            index_A804 = 3;
            state_A8A0 = 151;
            break;
        }
        if (index_A804 >= 81 && index_A804 <= 83) {
            sub_4807(posx_A800, posy_A802 - 20, index_A804);
        } else {
            sub_4807(posx_A800, posy_A802, index_A804);
        }
        break;
    case 151:
        if (word_A83A++ < 1) {
            break;
        }
        word_A83A = 0;
        posx_A800 -= word_A2AA * 6;
        index_A804 = index_A804 == 2 ? 3 : 2;
        sub_4807(posx_A800, posy_A802, index_A804);
        if (posx_A800 < -40) {
            destroySubPooWnd();
            state_A8A0 = kStateNormal1;
            break;
        }
        break;
    case 152:
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
        state_A8A0 = kStateNormal1;
        if (word_A7FC != 0) {
            state_A8A0 = 97;
        }
        break;
    case 1:
        state_A8A0 = 81;
        break;
    case 2:
        state_A8A0 = 97;
        break;
    case 3:
        word_CA54 = 113;
        break;
    case 4:
        state_A8A0 = 56;
        break;
    default:
        break;
    }
}

/* Process debug window action change. */
void debugAction(WPARAM action)
{
    word_CA72 = 0;
    stopPlaySound();
    destroySubPooWnd();
    switch (action) {
    case 0:        ///Normal
        state_A8A0 = kStateNormal1;
        break;
    case 1:        ///Run
        state_A8A0 = kStateRun1;
        break;
    case 2:        ///Walk
        state_A8A0 = kStateWalk1;
        break;
    case 3:        ///Handstand
        state_A8A0 = kStateHandstand1;
        break;
    case 4:        ///Sex
        state_A8A0 = kStateSex1;
        break;
    case 5:        ///Sleep
        state_A8A0 = kStateSleep1;
        break;
    case 6:        ///Wink
        state_A8A0 = kStateWink1;
        break;
    case 7:        ///Turn
        state_A8A0 = kStateTurn1;
        break;
    case 8:        ///Hit
        state_A8A0 = kStateHit1;
        break;
    case 9:        ///Piss
        state_A8A0 = kStatePiss1;
        break;
    case 10:        ///Yawn
        state_A8A0 = kStateYawn1;
        break;
    case 11:        ///Baa
        state_A8A0 = kStateBaa1;
        break;
    case 12:        ///Suprise
        state_A8A0 = kStateSuprise1;
        break;
    case 13:        ///Scary
        state_A8A0 = kStateScary1;
        break;
    case 14:        ///Eat
        state_A8A0 = kStateEat1;
        break;
    case 15:        ///Sit
        state_A8A0 = kStateSit1;
        break;
    case 16:        ///Sneeze
        state_A8A0 = kStateSneeze1;
        break;
    case 17:        ///Burn
        state_A8A0 = kStateBurn1;
        break;
    case 18:        ///Merry1
        state_A8A0 = kStateMerry11;
        break;
    case 19:        ///Merry2
        state_A8A0 = kStateMerry21;
        break;
    case 20:        ///Merry3
        state_A8A0 = kStateMerry31;
        break;
    case 21:        ///UFO1
        state_A8A0 = kStateUFO11;
        break;
    case 22:        ///UFO2
        state_A8A0 = kStateUFO21;
        break;
    case 23:        ///UFO3
        state_A8A0 = kStateUFO31;
        break;
    case 24:        ///Roll
        state_A8A0 = kStateRoll1;
        break;
    case 25:        ///Blush
        state_A8A0 = kStateBlush1;
        break;
    case 26:        ///Slip
        state_A8A0 = kStateSlip1;
        break;
    case 27:        ///Fall
        state_A8A0 = kStateFall1;
        break;
    case 28:        ///Jump
        state_A8A0 = kStateJump1;
        break;
    case 29:        ///Rotate
        state_A8A0 = kStateRotate1;
        break;
    default:
        break;
    }
}

/* Move window by offset. */
void movePooWnd(int offsX, int offsY)
{
    posx_A800 += offsX;
    posy_A802 += offsY;
    sub_4807(posx_A800, posy_A802, index_A804);
}

/* Initialize bitmaps (sub). */
BOOL initSubWnd(HWND hWnd)
{
    HDC hdc;
    hdc = GetDC(hWnd);
    word_A850[0] = CreateCompatibleBitmap(hdc, 100, 100);
    if (word_A850[0] == NULL) {
        goto failed;
    }
    word_A850[1] = CreateCompatibleBitmap(hdc, 100, 100);
    if (word_A850[1] == NULL) {
        goto failed;
    }
    word_A854 = CreateCompatibleBitmap(hdc, 100, 100);
    if (word_A854 == NULL) {
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
    word_CA5C = 0;
    word_CA56 = 0;
    word_A890 = 0;
    word_A892 = 0;
    word_A894 = 0;
    word_A896 = 0;
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
    DeleteObject(word_A850[0]);
    DeleteObject(word_A850[1]);
    DeleteObject(word_A854);
}

/* Update window position and sprite to be actually used (sub). */
void sub_9350(int arg_0, int arg_2, int arg_4)
{
    word_A878 = arg_0;
    word_A87A = arg_2;
    word_A856 = sprites_[arg_4].bitmaps[0];
    word_A858 = sprites_[arg_4].bitmaps[1];
    word_A85E = sprites_[arg_4].x;
    word_A860 = sprites_[arg_4].y;
    word_A87C = sprites_[arg_4].width;
    word_A87E = sprites_[arg_4].height;
}

/* Clear window (sub). */
void sub_93DF(HWND arg_0)
{
    if (word_CA56 != 0) {
        return;
    }
    word_A890 = 0;
    word_A892 = 0;
    word_A894 = 0;
    word_A896 = 0;
    MoveWindow(arg_0, 0, 0, 0, 0, TRUE);
    word_A872 = 1;
    word_C0BA = 1;
    word_A876 = NULL;
}

/* Render sprite with double buffering (with fade out effect) (sub). */
void sub_9438(HWND arg_0)
{
    HDC var_2;
    HDC var_4;
    HDC var_6;
    int var_C;
    int var_E;
    int var_10;
    int var_12;
    int var_14;
    int var_16;
    int var_18;
    int var_1A;
    int var_1C;
    int var_1E;
    if (word_A870 != 0) {
        return;
    }
    if (word_A890 == word_A878 && word_A892 == word_A87A && word_A876 == word_A856 && word_A866 == word_A85E && word_CA46 == 0 && word_CA5C == 0) {
        return;
    }
    word_A86E ^= 1;
    var_2 = GetDC(NULL);
    SelectPalette(var_2, hPalette_, FALSE);
    var_4 = CreateCompatibleDC(var_2);
    var_6 = CreateCompatibleDC(var_2);
    SelectPalette(var_6, hPalette_, FALSE);
    SelectPalette(var_4, hPalette_, FALSE);
    var_16 = max(word_A878, word_A890);
    var_14 = max(word_A87A, word_A892);
    var_12 = min(word_A87C + word_A878, word_A894 + word_A890) - var_16;
    var_10 = min(word_A87A + word_A87E, word_A892 + word_A896) - var_14;
    if (var_12 <= 0 || var_10 <= 0) {
        word_A898 = 1;
        if (word_A872 != 0) {
            word_A872 = 0;
        }
        word_A880 = word_A878;
        word_A882 = word_A87A;
        word_A884 = word_A87C;
        word_A886 = word_A87E;
        SelectObject(var_4, word_A850[word_A86E]);
        BitBlt(var_4, 0, 0, word_A884, word_A886, var_2, word_A880, word_A882, SRCCOPY);
    } else {
        word_A898 = 0;
        word_A880 = min(word_A878, word_A890);
        word_A882 = min(word_A87A, word_A892);
        word_A884 = max(word_A87C + word_A878, word_A894 + word_A890) - word_A880;
        word_A886 = max(word_A87A + word_A87E, word_A892 + word_A896) - word_A882;
        SelectObject(var_4, word_A850[word_A86E]);
        BitBlt(var_4, 0, 0, word_A884, word_A886, var_2, word_A880, word_A882, SRCCOPY);
        var_1E = max(word_A880, word_A888);
        var_1C = max(word_A882, word_A88A);
        var_1A = min(word_A884 + word_A880, word_A88C + word_A888) - var_1E;
        var_18 = min(word_A882 + word_A886, word_A88A + word_A88E) - var_1C;
        var_16 = max(0, var_1E - word_A880);
        var_14 = max(0, var_1C - word_A882);
        var_E = max(0, var_1E - word_A888);
        var_C = max(0, var_1C - word_A88A);
        if (var_1A > 0 && var_18 > 0) {
            SelectObject(var_6, word_A850[LOBYTE(word_A86E) - 0xFF & 1]);
            BitBlt(var_4, var_16, var_14, var_1A, var_18, var_6, var_E, var_C, SRCCOPY);
        }
    }
    if (word_A856 != NULL) {
        SelectObject(var_6, word_A854);
        BitBlt(var_6, 0, 0, word_A884, word_A886, var_4, 0, 0, SRCCOPY);
        var_16 = max(0, word_A878 - word_A880);
        var_14 = max(0, word_A87A - word_A882);
        if (word_A858 != NULL) {
            if (word_CA46 != 0) {
                if (word_CA46 == 1) {
                    SelectObject(var_4, word_A858);
                    SelectObject(var_6, word_A85C);
                    BitBlt(var_6, 0, 0, 40, 40, var_4, word_A85E, word_A860, SRCCOPY);
                    SelectObject(var_4, word_A856);
                    SelectObject(var_6, word_A85A);
                    BitBlt(var_6, 0, 0, 40, 40, var_4, word_A85E, word_A860, SRCCOPY);
                }
                SelectObject(var_4, word_A85C);
                SelectObject(var_6, sprites_[172].bitmaps[0]);
                BitBlt(var_4, word_CA46 - 1, word_CA46 - 1, 41 - word_CA46, 40, var_6, sprites_[172].x, 0, SRCPAINT);
                SelectObject(var_4, word_A85A);
                SelectObject(var_6, sprites_[172].bitmaps[1]);
                BitBlt(var_4, word_CA46 - 1, word_CA46 - 1, 41 - word_CA46, 40, var_6, sprites_[172].x, 0, SRCAND);
                SelectObject(var_6, word_A854);
                SelectObject(var_4, word_A85C);
                BitBlt(var_6, var_16, var_14, word_A87C, word_A87E, var_4, 0, 0, SRCAND);
                SelectObject(var_4, word_A85A);
                BitBlt(var_6, var_16, var_14, word_A87C, word_A87E, var_4, 0, 0, SRCPAINT);
            } else {
                SelectObject(var_4, word_A858);
                BitBlt(var_6, var_16, var_14, word_A87C, word_A87E, var_4, word_A85E, word_A860, SRCAND);
                SelectObject(var_4, word_A856);
                BitBlt(var_6, var_16, var_14, word_A87C, word_A87E, var_4, word_A85E, word_A860, SRCPAINT);
            }
        } else {
            SelectObject(var_4, word_A856);
            BitBlt(var_6, var_16, var_14, word_A87C, word_A87E, var_4, word_A85E, word_A860, SRCCOPY);
        }
        word_A870 = 1;
        word_CA5E = 1;
        MoveWindow(arg_0, word_A880, word_A882, word_A884, word_A886 + word_CA5C, TRUE);
        word_CA5E = 0;
    }
    DeleteDC(var_4);
    DeleteDC(var_6);
    word_A888 = word_A880;
    word_A88A = word_A882;
    word_A88C = word_A884;
    word_A88E = word_A886;
    word_A890 = word_A878;
    word_A892 = word_A87A;
    word_A894 = word_A87C;
    word_A896 = word_A87E;
    word_A876 = word_A856;
    word_A866 = word_A85E;
    word_A868 = word_A860;
    ReleaseDC(NULL, var_2);
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
    if (word_A870 == 0) {
        return TRUE;
    }
    word_A870 = 0;
    var_2 = GetDC(arg_0);
    SelectPalette(var_2, hPalette_, FALSE);
    RealizePalette(var_2);
    var_4 = CreateCompatibleDC(var_2);
    SelectPalette(var_4, hPalette_, FALSE);
    SelectObject(var_4, word_A854);
    BitBlt(var_2, 0, 0, word_A884, word_A886, var_4, 0, 0, SRCCOPY);
    if (word_CA5C != 0) {
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
        BitBlt(var_E, 0, 0, 40, word_CA5C, screen, word_A880, word_A882 + 40, SRCCOPY);
        ReleaseDC(NULL, screen);
#else
        BitBlt(var_E, 0, 0, 40, word_CA5C, var_2, 0, 40, SRCCOPY);
#endif
        var_C.left = 0;
        var_C.top = 0;
        var_C.right = 40;
        var_C.bottom = word_CA5C;
        SelectObject(var_4, word_C0B8);
        FillRect(var_4, &var_C, word_CA44);
        BitBlt(var_E, 0, 0, 40, word_CA5C, var_4, 0, 0, SRCAND);
        FillRect(var_4, &var_C, word_C0B4);
        BitBlt(var_E, 0, 0, 40, word_CA5C, var_4, 0, 0, SRCPAINT);
        BitBlt(var_2, 0, 40, 40, word_CA5C, var_E, 0, 0, SRCCOPY);
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
