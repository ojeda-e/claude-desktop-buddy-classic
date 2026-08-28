#include "../buddy.h"
#include "../buddy_common.h"
#include <M5StickC.h>
#include <string.h>

extern TFT_eSprite spr;

namespace puppy {

// Base dog:
//  /)---(\      ears + head
// (/ . . \)     dot eyes
//  \ (*) /      tongue

// ─── SLEEP ───  ~12s cycle, 6 poses + Z drift
static void doSleep(uint32_t t) {
  static const char* const CURL[5]    = { "            ", " /)---(\\  ", "(/ - - \\) ", " \\ (~) /  ", "            " };
  static const char* const BREATHE[5] = { "            ", " /)---(\\  ", "(/ -_- \\) ", " \\ (~) /  ", "            " };
  static const char* const EAR_F[5]   = { "            ", " v)---(\\  ", "(/ - - \\) ", " \\ (~) /  ", "            " };
  static const char* const SIDE[5]    = { "            ", "  .-~~~-.  ", " ( --zZ )  ", "  `~~~~`    ", "            " };
  static const char* const TWITCH[5]  = { "            ", " /)---(\\  ", "(/ - - \\) ", " \\ (~) /~~ ", "            " };
  static const char* const DREAM[5]   = { "            ", " /)---(\\  ", "(/ u.u \\) ", " \\ (~) /  ", "            " };

  const char* const* P[6] = { CURL, BREATHE, EAR_F, SIDE, TWITCH, DREAM };
  static const uint8_t SEQ[] = {
    0,1,0,1,0,1,4,1,
    0,1,0,1,
    3,3,3,3,
    2,2,
    1,5,1,1
  };
  uint8_t beat = (t / 5) % sizeof(SEQ);
  buddyPrintSprite(P[SEQ[beat]], 5, 0, 0xFE60);

  int p1 = (t)     % 12;
  int p2 = (t + 5) % 12;
  int p3 = (t + 9) % 12;
  buddySetColor(BUDDY_DIM);
  buddySetCursor(BUDDY_X_CENTER + 18 + p1, BUDDY_Y_OVERLAY + 18 - p1 * 2);
  buddyPrint("z");
  buddySetColor(BUDDY_WHITE);
  buddySetCursor(BUDDY_X_CENTER + 24 + p2, BUDDY_Y_OVERLAY + 14 - p2);
  buddyPrint("Z");
  buddySetColor(BUDDY_DIM);
  buddySetCursor(BUDDY_X_CENTER + 14 + p3 / 2, BUDDY_Y_OVERLAY + 8 - p3 / 2);
  buddyPrint("z");
}

// ─── IDLE ───  ~16s cycle, 10 poses
static void doIdle(uint32_t t) {
  static const char* const REST[5]    = { "            ", " /)---(\\  ", "(/ . . \\) ", " \\ (*) /  ", "            " };
  static const char* const LOOK_L[5]  = { "            ", " /)---(\\  ", "(/ . .  \\) ", " \\ (*) /  ", "            " };
  static const char* const LOOK_R[5]  = { "            ", " /)---(\\  ", "(/ . .\\)  ", " \\ (*) /  ", "            " };
  static const char* const BLINK[5]   = { "            ", " /)---(\\  ", "(/ - - \\) ", " \\ (*) /  ", "            " };
  static const char* const TILT[5]    = { "            ", " /)---(\\  ", "(/ . . \\) ", " \\ (*) /  ", " ~~         " };
  static const char* const EAR_F[5]   = { "            ", " v)---(\\  ", "(/ . . \\) ", " \\ (*) /  ", "            " };
  static const char* const TAIL_L[5]  = { "~~          ", " /)---(\\  ", "(/ . . \\) ", " \\ (*) /  ", "            " };
  static const char* const TAIL_R[5]  = { "            ", " /)---(\\  ", "(/ . . \\) ", " \\ (*) /  ", " ~~         " };
  static const char* const PANT_A[5]  = { "            ", " /)---(\\  ", "(/ . . \\) ", " \\ ( ) / ", "            " };
  static const char* const PANT_B[5]  = { "            ", " /)---(\\  ", "(/ . . \\) ", " \\ ( ) / ", "            " };

  const char* const* P[10] = { REST, LOOK_L, LOOK_R, BLINK, TILT, EAR_F, TAIL_L, TAIL_R, PANT_A, PANT_B };
  static const uint8_t SEQ[] = {
    0,0,0,3,0,1,0,2,0,
    6,7,6,7,6,7,
    0,5,0,0,
    4,4,0,
    8,9,8,9,0,
    0,3,0,
    5,5,0,0,
    7,6,7,6,
    0,0,2,0
  };
  uint8_t beat = (t / 5) % sizeof(SEQ);
  buddyPrintSprite(P[SEQ[beat]], 5, 0, 0xFE60);
}

// ─── BUSY ───  ~10s cycle, 6 poses + dot ticker
static void doBusy(uint32_t t) {
  static const char* const DIG_A[5]  = { "            ", " /)---(\\  ", "(/ v v \\) ", " \\ (_) /  ", " /_\\ /_\\  " };
  static const char* const DIG_B[5]  = { "            ", " /)---(\\  ", "(/ v v \\) ", " \\ (_) /  ", " \\_/ \\_/  " };
  static const char* const THINK[5]   = { "      ?     ", " /)---(\\  ", "(/ o o \\) ", " \\ (.) /  ", "            " };
  static const char* const CHEW[5]    = { "  o         ", " /)---(\\  ", "(/ . . \\) ", " \\ (Ww) / ", "            " };
  static const char* const EUREKA[5]  = { "      *     ", " /)===\\   ", "(/ O O \\) ", " \\ (^) /  ", "            " };
  static const char* const SHAKE[5]   = { "    ~~~     ", " /)---(\\  ", "(/ - - \\) ", " \\ (*) /  ", "            " };

  const char* const* P[6] = { DIG_A, DIG_B, THINK, CHEW, EUREKA, SHAKE };
  static const uint8_t SEQ[] = {
    0,1,0,1,0,1, 2,2, 0,1,0,1, 3,3, 2,4, 0,1,0,1,5
  };
  uint8_t beat = (t / 5) % sizeof(SEQ);
  buddyPrintSprite(P[SEQ[beat]], 5, 0, 0xFE60);

  static const char* const DOTS[] = { ".  ", ".. ", "...", " ..", "  .", "   " };
  buddySetColor(BUDDY_WHITE);
  buddySetCursor(BUDDY_X_CENTER + 22, BUDDY_Y_OVERLAY + 14);
  buddyPrint(DOTS[t % 6]);
}

// ─── ATTENTION ───  ~8s cycle, 6 poses + ! pulse
static void doAttention(uint32_t t) {
  static const char* const ALERT[5]   = { "            ", " /)===\\   ", "(/ O O \\) ", " \\ (v) /  ", "     |      " };
  static const char* const SCAN_L[5]  = { "            ", " /)===\\   ", "(/O    \\)  ", " \\ (v) /  ", "     |      " };
  static const char* const SCAN_R[5]  = { "            ", " /)===\\   ", "(/   O\\)  ", " \\ (v) /  ", "     |      " };
  static const char* const SCAN_U[5]  = { "            ", " /)===\\   ", "(/ ^ ^ \\) ", " \\ (v) /  ", "     |      " };
  static const char* const CROUCH[5]  = { "            ", "/)=====\\  ", "(/ O O \\) ", " \\ (v) /  ", "    |       " };
  static const char* const BARK[5]    = { "            ", " /)===\\   ", "(/ @ @ \\) ", " \\ (>) /  ", "     |      " };

  const char* const* P[6] = { ALERT, SCAN_L, SCAN_R, SCAN_U, CROUCH, BARK };
  static const uint8_t SEQ[] = {
    0,4,0,1,0,2,0,3, 4,4,0,1,2,0, 5,0
  };
  uint8_t beat = (t / 5) % sizeof(SEQ);
  uint8_t pose = SEQ[beat];
  int xOff = (pose == 4) ? ((t & 1) ? 1 : -1) : 0;
  buddyPrintSprite(P[pose], 5, 0, 0xFE60, xOff);

  if ((t / 2) & 1) {
    buddySetColor(BUDDY_YEL);
    buddySetCursor(BUDDY_X_CENTER - 4, BUDDY_Y_OVERLAY);
    buddyPrint("!");
  }
  if ((t / 3) & 1) {
    buddySetColor(BUDDY_YEL);
    buddySetCursor(BUDDY_X_CENTER + 4, BUDDY_Y_OVERLAY + 4);
    buddyPrint("!");
  }
}

// ─── CELEBRATE ───  ~5s cycle, 6 poses + confetti
static void doCelebrate(uint32_t t) {
  static const char* const CROUCH[5]  = { "            ", " /)---(\\  ", "(/ ^ ^ \\) ", " \\ (P) /  ", " /_____\\   " };
  static const char* const JUMP[5]    = { "  \\o/       ", " /)---(\\  ", "(/ ^ ^ \\) ", " \\ (P) /  ", " ~~         " };
  static const char* const PEAK[5]    = { "  \\*\\       ", " /)---(\\  ", "(/ * * \\) ", " \\ (P) /  ", " ~~         " };
  static const char* const SPIN_L[5]  = { "            ", " /)---(\\  ", "(/ < < \\) ", " \\ (P) / /", "~~          " };
  static const char* const SPIN_R[5]  = { "            ", " /)---(\\  ", "(/ > > \\) ", " \\ (P) /  ", " ~~         " };
  static const char* const POSE[5]    = { "    \\o/     ", " /)---(\\  ", "(/ ^ ^ \\) ", " \\ (P) /  ", " ~~         " };

  const char* const* P[6] = { CROUCH, JUMP, PEAK, SPIN_L, SPIN_R, POSE };
  static const uint8_t SEQ[] = { 0,1,2,1,0, 3,4,3,4, 0,1,2,1,0, 5,5 };
  static const int8_t Y_SHIFT[] = { 0,-3,-6,-3,0, 0,0,0,0, 0,-3,-6,-3,0, 0,0 };
  uint8_t beat = (t / 3) % sizeof(SEQ);
  buddyPrintSprite(P[SEQ[beat]], 5, Y_SHIFT[beat], 0xFE60);

  static const uint16_t cols[] = { BUDDY_YEL, BUDDY_HEART, BUDDY_CYAN, BUDDY_WHITE, BUDDY_GREEN };
  for (int i = 0; i < 6; i++) {
    int phase = (t * 2 + i * 11) % 22;
    int x = BUDDY_X_CENTER - 36 + i * 14;
    int y = BUDDY_Y_OVERLAY - 6 + phase;
    if (y > BUDDY_Y_BASE + 20 || y < 0) continue;
    buddySetColor(cols[i % 5]);
    buddySetCursor(x, y);
    buddyPrint((i + (int)(t/2)) & 1 ? "*" : ".");
  }
}

// ─── DIZZY ───  ~6s cycle, 5 poses + orbiting stars
static void doDizzy(uint32_t t) {
  static const char* const TILT_L[5]  = { "            ", "/)---(\\   ", "(/ @ @ \\) ", " \\ (~~)/  ", "~~          " };
  static const char* const TILT_R[5]  = { "            ", " /)---(\\  ", "(/ @ @ \\) ", " \\ (~~) / ", " ~~         " };
  static const char* const WOOZY[5]   = { "            ", " /)---(\\  ", "(/ x @ \\) ", " \\ (~) /  ", "            " };
  static const char* const WOOZY2[5]  = { "            ", " /)---(\\  ", "(/ @ x \\) ", " \\ (~) /  ", "~~          " };
  static const char* const SPLAT[5]   = { "            ", " /)---(\\  ", "(/ @ @ \\) ", " \\ (-) /  ", " ~          " };

  const char* const* P[5] = { TILT_L, TILT_R, WOOZY, WOOZY2, SPLAT };
  static const uint8_t SEQ[] = { 0,1,0,1, 2,3, 0,1,0,1, 4,4, 2,3 };
  static const int8_t X_SHIFT[] = { -3,3,-3,3, 0,0, -3,3,-3,3, 0,0, 0,0 };
  uint8_t beat = (t / 4) % sizeof(SEQ);
  buddyPrintSprite(P[SEQ[beat]], 5, 0, 0xFE60, X_SHIFT[beat]);

  static const int8_t OX[] = { 0, 5, 7, 5, 0, -5, -7, -5 };
  static const int8_t OY[] = { -5, -3, 0, 3, 5, 3, 0, -3 };
  uint8_t p1 = t % 8;
  uint8_t p2 = (t + 4) % 8;
  buddySetColor(BUDDY_CYAN);
  buddySetCursor(BUDDY_X_CENTER + OX[p1] - 2, BUDDY_Y_OVERLAY + 6 + OY[p1]);
  buddyPrint("*");
  buddySetColor(BUDDY_YEL);
  buddySetCursor(BUDDY_X_CENTER + OX[p2] - 2, BUDDY_Y_OVERLAY + 6 + OY[p2]);
  buddyPrint("*");
}

// ─── HEART ───  ~10s cycle, 5 poses + rising hearts
static void doHeart(uint32_t t) {
  static const char* const DREAMY[5]  = { "            ", " /)---(\\  ", "(/ ^ ^ \\) ", " \\ (u) /  ", " ~~         " };
  static const char* const BLUSH[5]   = { "            ", " /)---(\\  ", "(/#^ ^#\\)  ", " \\ (u) /  ", " ~~         " };
  static const char* const HEART_E[5] = { "            ", " /)---(\\  ","(/<3 <3\\)  ", " \\ (u) /  ", " ~~         " };
  static const char* const PURR[5]    = { "~~          ", " /)-~-\\   ", "(/ ~ ~ \\) ", " \\ (u) /  ", " ~~         " };
  static const char* const HEAD_T[5]  = { "            ", " /)---(\\  ", "(/ ^ - \\) ", " \\ (u) /  ", " ~~         " };

  const char* const* P[5] = { DREAMY, BLUSH, HEART_E, PURR, HEAD_T };
  static const uint8_t SEQ[] = {
    0,0,1,0, 2,2,0, 1,0,4, 0,0,3,3, 0,1,0,2, 1,0
  };
  static const int8_t Y_BOB[] = { 0,-1,0,-1, 0,-1,0, -1,0,0, -1,0,0,0, -1,0,-1,0, -1,0 };
  uint8_t beat = (t / 5) % sizeof(SEQ);
  buddyPrintSprite(P[SEQ[beat]], 5, Y_BOB[beat], 0xFE60);

  buddySetColor(BUDDY_HEART);
  for (int i = 0; i < 5; i++) {
    int phase = (t + i * 4) % 16;
    int y = BUDDY_Y_OVERLAY + 16 - phase;
    if (y < -2 || y > BUDDY_Y_BASE) continue;
    int x = BUDDY_X_CENTER - 20 + i * 8 + ((phase / 3) & 1) * 2 - 1;
    buddySetCursor(x, y);
    buddyPrint("v");
  }
}

}  // namespace puppy

extern const Species PUPPY_SPECIES = {
  "puppy",
  0xFE60,
  { puppy::doSleep, puppy::doIdle, puppy::doBusy, puppy::doAttention,
    puppy::doCelebrate, puppy::doDizzy, puppy::doHeart }
};
