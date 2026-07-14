#pragma once

/* =========================================================================
 * COLORS.H - ANSI Escape Sequences for Colored Terminal Output in C/C++
 * =========================================================================
 * 
 * Usage:
 *   printf(COLOR_RED "This is red" COLOR_RESET "\n");
 *   printf(COLOR_BOLD_GREEN "This is bold green" COLOR_RESET "\n");
 * 
 * Note: Consecutive string literals are concatenated automatically by the C
 * compiler, so this has zero runtime overhead!
 * ========================================================================= */

// Reset all formatting (Crucial to prevent color "bleeding")
#define COLOR_RESET          "\033[0m"

// Text Styles
#define STYLE_BOLD           "\033[1m"
#define STYLE_DIM            "\033[2m"
#define STYLE_UNDERLINE      "\033[4m"
#define STYLE_BLINK          "\033[5m"
#define STYLE_REVERSE        "\033[7m"
#define STYLE_HIDDEN         "\033[8m"

// Standard Foreground (Text) Colors - Dimmer/Normal
#define COLOR_BLACK          "\033[0;30m"
#define COLOR_RED            "\033[0;31m"
#define COLOR_GREEN          "\033[0;32m"
#define COLOR_YELLOW         "\033[0;33m"
#define COLOR_BLUE           "\033[0;34m"
#define COLOR_MAGENTA        "\033[0;35m"
#define COLOR_CYAN           "\033[0;36m"
#define COLOR_WHITE          "\033[0;37m"

// Bold Foreground (Text) Colors
#define COLOR_BOLD_BLACK     "\033[1;30m"
#define COLOR_BOLD_RED       "\033[1;31m"
#define COLOR_BOLD_GREEN     "\033[1;32m"
#define COLOR_BOLD_YELLOW    "\033[1;33m"
#define COLOR_BOLD_BLUE      "\033[1;34m"
#define COLOR_BOLD_MAGENTA   "\033[1;35m"
#define COLOR_BOLD_CYAN      "\033[1;36m"
#define COLOR_BOLD_WHITE     "\033[1;37m"

// Underlined Foreground (Text) Colors
#define COLOR_UNDER_BLACK    "\033[4;30m"
#define COLOR_UNDER_RED      "\033[4;31m"
#define COLOR_UNDER_GREEN    "\033[4;32m"
#define COLOR_UNDER_YELLOW   "\033[4;33m"
#define COLOR_UNDER_BLUE     "\033[4;34m"
#define COLOR_UNDER_MAGENTA  "\033[4;35m"
#define COLOR_UNDER_CYAN     "\033[4;36m"
#define COLOR_UNDER_WHITE    "\033[4;37m"

// High-Intensity (Bright) Foreground Colors
#define COLOR_BRIGHT_BLACK   "\033[0;90m" // Dark Gray
#define COLOR_BRIGHT_RED     "\033[0;91m"
#define COLOR_BRIGHT_GREEN   "\033[0;92m"
#define COLOR_BRIGHT_YELLOW  "\033[0;93m"
#define COLOR_BRIGHT_BLUE    "\033[0;94m"
#define COLOR_BRIGHT_MAGENTA "\033[0;95m"
#define COLOR_BRIGHT_CYAN    "\033[0;96m"
#define COLOR_BRIGHT_WHITE   "\033[0;97m"

// Bold High-Intensity Foreground Colors
#define COLOR_B_BRIGHT_BLACK   "\033[1;90m"
#define COLOR_B_BRIGHT_RED     "\033[1;91m"
#define COLOR_B_BRIGHT_GREEN   "\033[1;92m"
#define COLOR_B_BRIGHT_YELLOW  "\033[1;93m"
#define COLOR_B_BRIGHT_BLUE    "\033[1;94m"
#define COLOR_B_BRIGHT_MAGENTA "\033[1;95m"
#define COLOR_B_BRIGHT_CYAN    "\033[1;96m"
#define COLOR_B_BRIGHT_WHITE   "\033[1;97m"

// Standard Background Colors
#define BG_BLACK             "\033[40m"
#define BG_RED               "\033[41m"
#define BG_GREEN             "\033[42m"
#define BG_YELLOW            "\033[43m"
#define BG_BLUE              "\033[44m"
#define BG_MAGENTA           "\033[45m"
#define BG_CYAN              "\033[46m"
#define BG_WHITE             "\033[47m"

// High-Intensity Background Colors
#define BG_BRIGHT_BLACK      "\033[0;100m"
#define BG_BRIGHT_RED        "\033[0;101m"
#define BG_BRIGHT_GREEN      "\033[0;102m"
#define BG_BRIGHT_YELLOW     "\033[0;103m"
#define BG_BRIGHT_BLUE       "\033[0;104m"
#define BG_BRIGHT_MAGENTA    "\033[0;105m"
#define BG_BRIGHT_CYAN       "\033[0;106m"
#define BG_BRIGHT_WHITE      "\033[0;107m"
