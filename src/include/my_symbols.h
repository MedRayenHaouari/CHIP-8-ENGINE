#pragma once

/* =========================================================================
 * SYMBOLS.H - UTF-8 Multi-Byte Terminal Symbols for C/C++ Command Lines
 * =========================================================================
 * 
 * Usage:
 *   printf("[" SYM_CHECK " ] Loaded successfully!\n");
 *   printf("[" SYM_CROSS " ] Operation failed.\n");
 * 
 * Note: These are multi-byte character sequences. They require a terminal
 * emulator configured to display UTF-8 text (default on almost all Linux).
 * ========================================================================= */

// --- Status Icons ---
#define SYM_CHECK            "\xE2\x9C\x94"     // ✔ (Heavy Checkmark)
#define SYM_CHECK_LIGHT      "\xE2\x9C\x93"     // ✓ (Light Checkmark)
#define SYM_CROSS            "\xE2\x9C\x96"     // ✖ (Heavy Cross)
#define SYM_CROSS_LIGHT      "\xE2\x9C\x97"     // ✗ (Light Cross)
#define SYM_WARNING          "\xE2\x9A\xA0"     // ⚠ (Warning Triangle)
#define SYM_INFO             "\xE2\x84\xB9"     // ℹ (Information Sign)
#define SYM_QUESTION         "\x3F"             // ? (Standard ASCII Question)
#define SYM_GEAR             "\xE2\x9A\x99"     // ⚙ (Gear / Settings)
#define SYM_LOCK             "\xF0\x9F\x94\x92" // 🔒 (Padlock)
#define SYM_KEY              "\xF0\x9F\x97\x9D" // 🗝 (Key)

// --- Navigation & Arrows ---
#define SYM_ARROW_RIGHT      "\xE2\x86\x92"     // →
#define SYM_ARROW_LEFT       "\xE2\x86\x90"     // ←
#define SYM_ARROW_UP         "\xE2\x86\x91"     // ↑
#define SYM_ARROW_DOWN       "\xE2\x86\x93"     // ↓
#define SYM_TRIANGLE_RIGHT   "\xE2\x96\xB6"     // ▶
#define SYM_TRIANGLE_LEFT    "\xE2\x97\x80"     // ◀
#define SYM_TRIANGLE_UP      "\xE2\x96\xB2"     // ▲
#define SYM_TRIANGLE_DOWN    "\xE2\x96\xBC"     // ▼

// --- Bullets & Lists ---
#define SYM_BULLET           "\xE2\x80\xA2"     // • (Filled Bullet)
#define SYM_BULLET_HOLLOW    "\xE2\x97\xA6"     // ◦ (Hollow Bullet)
#define SYM_DIAMOND          "\xE2\x97\x86"     // ◆ (Filled Diamond)
#define SYM_SQUARE_FILLED    "\xE2\x96\xA0"     // ■ (Filled Square)

// --- Box-Drawing Characters (Great for simple terminal UI borders) ---
#define SYM_BOX_HORIZ        "\xE2\x94\x80"     // ─ (Horizontal single line)
#define SYM_BOX_VERT         "\xE2\x94\x82"     // │ (Vertical single line)
#define SYM_BOX_TOP_LEFT     "\xE2\x94\x8C"     // ┌ (Top Left corner)
#define SYM_BOX_TOP_RIGHT    "\xE2\x94\x90"     // ┐ (Top Right corner)
#define SYM_BOX_BOT_LEFT     "\xE2\x94\x94"     // └ (Bottom Left corner)
#define SYM_BOX_BOT_RIGHT    "\xE2\x94\x98"     // ┘ (Bottom Right corner)
#define SYM_BOX_TEE_LEFT     "\xE2\x94\x9C"     // ├ (Tee Left)
#define SYM_BOX_TEE_RIGHT    "\xE2\x94\xA4"     // ┤ (Tee Right)
#define SYM_BOX_TEE_TOP      "\xE2\x94\xAC"     // ┬ (Tee Top)
#define SYM_BOX_TEE_BOT      "\xE2\x94\xB4"     // ┴ (Tee Bottom)
#define SYM_BOX_CROSS        "\xE2\x94\xBC"     // ┼ (Cross Intersection)
