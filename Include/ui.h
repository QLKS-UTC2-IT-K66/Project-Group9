#ifndef UI_H
#define UI_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
  #include <windows.h>
  #define SLEEP_MS(ms) Sleep(ms)
#else
  #include <unistd.h>
  #define SLEEP_MS(ms) usleep((ms)*1000)
#endif

/* -------------------------------------------------------------------
   ANSI BASE
   ------------------------------------------------------------------- */
#define RESET       "\033[0m"
#define BOLD        "\033[1m"
#define DIM         "\033[2m"
#define ITALIC      "\033[3m"
#define UNDERLINE   "\033[4m"
#define BLINK       "\033[5m"
#define REVERSE     "\033[7m"

/* -------------------------------------------------------------------
   CURSOR & SCREEN CONTROL
   ------------------------------------------------------------------- */
#define CURSOR_HIDE  "\033[?25l"
#define CURSOR_SHOW  "\033[?25h"
#define CLEAR_LINE   "\033[2K\r"

/* -------------------------------------------------------------------
   COLOR PALETTE -- Luxury Dark x Gold x Neon Accent
   ------------------------------------------------------------------- */
#define C_BORDER    "\033[38;5;75m"
#define C_BORDER2   "\033[38;5;63m"
#define C_ACCENT    "\033[38;5;87m"
#define C_DIM_LINE  "\033[38;5;237m"

#define C_GOLD      "\033[38;5;220m"
#define C_GOLD2     "\033[38;5;178m"
#define C_GOLD3     "\033[38;5;136m"
#define C_AMBER     "\033[38;5;214m"

#define C_WHITE     "\033[38;5;255m"
#define C_CREAM     "\033[38;5;230m"
#define C_LGRAY     "\033[38;5;250m"
#define C_GRAY      "\033[38;5;244m"
#define C_DGRAY     "\033[38;5;239m"

#define C_GREEN     "\033[38;5;84m"
#define C_GREEN2    "\033[38;5;120m"
#define C_RED       "\033[38;5;203m"
#define C_YELLOW    "\033[38;5;228m"
#define C_BLUE      "\033[38;5;75m"
#define C_TEAL      "\033[38;5;80m"
#define C_PINK      "\033[38;5;213m"
#define C_VIOLET    "\033[38;5;177m"

#define BG_GREEN    "\033[48;5;22m"
#define BG_GOLD     "\033[48;5;136m"
#define BG_BLUE     "\033[48;5;17m"
#define BG_RED      "\033[48;5;52m"
#define BG_DARK     "\033[48;5;234m"

/* Legacy aliases */
#define C_STEEL           C_BORDER
#define C_SKY             C_BLUE
#define C_NAVY            "\033[38;5;68m"
#define C_MINT            C_GREEN2
#define FG_BRIGHT_CYAN    C_BLUE
#define FG_BRIGHT_GREEN   C_GREEN
#define FG_BRIGHT_RED     C_RED
#define FG_BRIGHT_YELLOW  C_GOLD
#define FG_BRIGHT_BLUE    C_NAVY
#define FG_BRIGHT_WHITE   C_WHITE
#define FG_BRIGHT_MAGENTA C_VIOLET
#define FG_WHITE          C_LGRAY
#define FG_CYAN           C_TEAL
#define FG_YELLOW         C_AMBER
#define FG_RED            C_RED
#define FG_GREEN          C_GREEN
#define FG_MAGENTA        C_VIOLET

/* -------------------------------------------------------------------
   SEMANTIC STYLE MACROS
   ------------------------------------------------------------------- */
#define HEADER_STYLE    BOLD C_BORDER
#define SUCCESS_STYLE   BOLD C_GREEN
#define ERROR_STYLE     BOLD C_RED
#define WARNING_STYLE   BOLD C_YELLOW
#define INFO_STYLE      C_TEAL
#define HIGHLIGHT_STYLE BOLD C_WHITE
#define MONEY_STYLE     BOLD C_GOLD
#define MENU_NUM_STYLE  BOLD C_AMBER
#define MENU_TXT_STYLE  C_CREAM
#define DIM_STYLE       C_GRAY
#define LABEL_STYLE     C_LGRAY
#define TAG_STYLE       BOLD C_TEAL

/* -------------------------------------------------------------------
   PRINT HELPERS
   ------------------------------------------------------------------- */
#define printSuccess(msg)  printf(SUCCESS_STYLE "  v  " msg RESET "\n")
#define printError(msg)    printf(ERROR_STYLE   "  x  " msg RESET "\n")
#define printWarning(msg)  printf(WARNING_STYLE "  !  " msg RESET "\n")
#define printInfo(msg)     printf(INFO_STYLE    "  >  " msg RESET "\n")

/* -------------------------------------------------------------------
   BOX DRAWING CONSTANTS
   ------------------------------------------------------------------- */
#define LINE_TOP_THIN   "+-------------------------------------------------+"
#define LINE_MID_THIN   "+-------------------------------------------------+"
#define LINE_BOT_THIN   "+-------------------------------------------------+"
#define LINE_TOP_DOUBLE "+--------------------------------------------------+"
#define LINE_MID_DOUBLE "+--------------------------------------------------+"
#define LINE_BOT_DOUBLE "+--------------------------------------------------+"

/* -------------------------------------------------------------------
   INLINE UTILITIES
   ------------------------------------------------------------------- */
static inline void clearScreen(void) {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

static inline void pressEnterToContinue(void) {
    printf("\n" C_DGRAY "  ---  " C_GOLD "[ Enter" C_DGRAY " de tiep tuc ]" RESET "  ");
    fflush(stdout);
    while (getchar() != '\n');
}

static inline void printDivider(void) {
    printf(C_DIM_LINE "  ----------------------------------------------------\n" RESET);
}

static inline void printDoubleDivider(void) {
    printf(HEADER_STYLE "  ====================================================\n" RESET);
}

static inline void printThinRule(void) {
    printf(C_ACCENT "  ....................................................\n" RESET);
}

/* -------------------------------------------------------------------
   ANIMATION HELPERS
   ------------------------------------------------------------------- */
static const char *SPINNER_FRAMES[] = {"/", "-", "\\", "|"};
#define SPINNER_LEN 4

static inline void animateLoadingBar(const char *label, int width, int ms) {
    int delay = ms / width;
    if (delay < 1) delay = 1;

    printf(CURSOR_HIDE);
    printf("  %s  [", label);
    for (int i = 0; i < width; i++) printf(C_DGRAY "." RESET);
    printf("]");
    fflush(stdout);

    /* Return to bracket start */
    for (int i = 0; i < width + 1; i++) printf("\b");
    fflush(stdout);

    for (int i = 0; i < width; i++) {
        if      (i < width / 3)        printf(C_TEAL  "|" RESET);
        else if (i < 2 * width / 3)    printf(C_GOLD  "|" RESET);
        else                            printf(C_GREEN "|" RESET);
        fflush(stdout);
        SLEEP_MS(delay);
    }
    printf("] " SUCCESS_STYLE "OK" RESET "\n");
    printf(CURSOR_SHOW);
    fflush(stdout);
}

static inline void animateSpinner(const char *label, int frames, int ms_each) {
    printf(CURSOR_HIDE);
    for (int i = 0; i < frames; i++) {
        printf(CLEAR_LINE "  " C_GOLD "%s" RESET "  %s", SPINNER_FRAMES[i % SPINNER_LEN], label);
        fflush(stdout);
        SLEEP_MS(ms_each);
    }
    printf(CLEAR_LINE "  " SUCCESS_STYLE "v" RESET "  %s\n", label);
    printf(CURSOR_SHOW);
    fflush(stdout);
}

/* -------------------------------------------------------------------
   BADGE HELPERS
   ------------------------------------------------------------------- */
#define BADGE_AVAILABLE  BG_GREEN  BOLD " TRONG   " RESET
#define BADGE_OCCUPIED   BG_RED    BOLD " CO KHACH" RESET
#define BADGE_GOLD_MEM   BG_GOLD   BOLD " * GOLD  " RESET
#define BADGE_SILVER_MEM BG_BLUE   BOLD " o SILVER" RESET
#define BADGE_BRONZE_MEM BG_DARK   BOLD " . BRONZE" RESET

#endif /* UI_H */
