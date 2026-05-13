#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef _WIN32
  #include <windows.h>
#endif
#include "model.h"
#include "ui.h"
#include "room.h"
#include "customer.h"
#include "booking.h"
#include "billing.h"
#include "service.h"
#include "membership.h"
#include "file.h"

// Biến toàn cục
static RoomNode *roomHead     = NULL;
static Invoice   invoices[MAX_INVOICES];
static int       invoiceCount = 0;

static void roomMenu(void);
static void bookingMenu(void);
static void serviceMenu_nav(void);
static void billingMenu(void);
static void dataMenu(void);
static int  readInt(void);

/* ------------------------------------------------------------
   SPLASH SCREEN  --  Animated boot sequence
   ------------------------------------------------------------ */
static void printSplash(void) {
    clearScreen();

    /* top padding */
    printf("\n\n");

    /* Outer frame -- wide gold border (62 chars width) */
    printf(BOLD C_GOLD "  +--------------------------------------------------------------+\n" RESET);
    printf(BOLD C_GOLD "  |" RESET C_DIM_LINE "                                                              " RESET BOLD C_GOLD "|\n" RESET);

    /* ASCII art logo */
    printf(BOLD C_GOLD "  |" RESET BOLD C_AMBER "            ███+   ███+███+   ███+██+     ██+  ██+            " BOLD C_GOLD "|\n" RESET);
    printf(BOLD C_GOLD "  |" RESET BOLD C_GOLD  "            ████+ ████|████+ ████|██|     ██| ██++            " BOLD C_GOLD "|\n" RESET);
    printf(BOLD C_GOLD "  |" RESET BOLD C_AMBER "            ██+████+██|██+████+██|██|     █████++             " BOLD C_GOLD "|\n" RESET);
    printf(BOLD C_GOLD "  |" RESET BOLD C_GOLD  "            ██|+██++██|██|+██++██|██|     ██+-██+             " BOLD C_GOLD "|\n" RESET);
    printf(BOLD C_GOLD "  |" RESET BOLD C_AMBER "            ██| +-+ ██|██| +-+ ██|███████+██|  ██+            " BOLD C_GOLD "|\n" RESET);
    printf(BOLD C_GOLD "  |" RESET C_GOLD2      "            +-+     +-++-+     +-++------++-+  +-+            " BOLD C_GOLD "|\n" RESET);

    /* Thin gold rule */
    printf(BOLD C_GOLD "  | " RESET C_GOLD3 "------------------------------------------------------------" RESET BOLD C_GOLD " |\n" RESET);

    printf(BOLD C_GOLD "  |" RESET BOLD C_CREAM "                 ★  T E C H H O T E L  ★                  " RESET BOLD C_GOLD "|\n" RESET);

    /* Subtitle */
    printf(BOLD C_GOLD "  |" RESET C_ACCENT   "             He thong Quan ly Khach san hien dai              " RESET BOLD C_GOLD "|\n" RESET);

    /* Team line */
    printf(BOLD C_GOLD "  |" RESET C_GRAY     "                    Nhom thuc hien: NHOM  9                   " RESET BOLD C_GOLD "|\n" RESET);

    printf(BOLD C_GOLD "  |" RESET C_DIM_LINE "                                                              " RESET BOLD C_GOLD "|\n" RESET);
    printf(BOLD C_GOLD "  +--------------------------------------------------------------+\n\n" RESET);
}

static void printBootSequence(void) {
    printf("\n");
    animateLoadingBar(C_TEAL "  Khoi dong he thong " RESET, 30, 600);
    SLEEP_MS(80);
    animateLoadingBar(C_GOLD "  Tai du lieu phong   " RESET, 30, 400);
    SLEEP_MS(80);
    animateLoadingBar(C_GREEN "  Ket noi co so du lieu" RESET, 30, 300);
    SLEEP_MS(200);
    printf("\n  " SUCCESS_STYLE "v" RESET C_CREAM "  He thong san sang!\n\n" RESET);
    SLEEP_MS(600);
}

/* ------------------------------------------------------------
   MAIN MENU
   ------------------------------------------------------------ */
static void displayMainMenu(void) {
    int total = countRooms(roomHead);
    int avail = countAvailableRooms(roomHead);
    int occup = total - avail;

    printf("\n");

    /* -- Header -- (54 chars width) */
    printf(BOLD C_BORDER "  +------------------------------------------------------+\n" RESET);
    printf(BOLD C_BORDER "  |" RESET BOLD C_GOLD "                ★  MMLK  TechHotel  ★                 " RESET BOLD C_BORDER "|\n" RESET);
    printf(BOLD C_BORDER "  |" RESET C_ACCENT  "              He Thong Quan Ly Khach San              " RESET BOLD C_BORDER "|\n" RESET);

    /* -- Status bar -- */
    printf(BOLD C_BORDER "  +------------------------------------------------------+\n" RESET);
    printf(BOLD C_BORDER "  |  " RESET);
    printf(C_LGRAY "Phong trong " RESET SUCCESS_STYLE "%-3d" RESET, avail);
    printf(C_DGRAY "  |  " RESET);
    printf(C_LGRAY "Dang thue " RESET ERROR_STYLE "%-3d" RESET, occup);
    printf(C_DGRAY "  |  " RESET);
    printf(C_LGRAY "Tong cong " RESET HIGHLIGHT_STYLE "%-3d" RESET, total);
    printf(" " BOLD C_BORDER "|\n" RESET); // 1 space left to make exactly 54 width

    /* -- Menu items -- */
    printf(BOLD C_BORDER "  +------------------------------------------------------+\n" RESET);
    printf(BOLD C_BORDER "  |" RESET "                                                      " BOLD C_BORDER "|\n" RESET);

    struct { const char *num; const char *icon; const char *label; } items[] = {
        {"1", "[ ]", "Quan ly Phong"},
        {"2", "[ ]", "Check-in  /  Check-out"},
        {"3", "[ ]", "Quan ly Dich vu"},
        {"4", "[ ]", "Giam gia & Thanh vien"},
        {"5", "[ ]", "Thanh toan & Xuat hoa don"},
        {"6", "[ ]", "Bao cao Doanh thu"},
        {"7", "[ ]", "Luu / Tai Du lieu"},
        {"8", "[ ]", "Thong ke Phong"},
    };

    for (int i = 0; i < 8; i++) {
        printf(BOLD C_BORDER "  |" RESET "  " MENU_NUM_STYLE " %s " RESET, items[i].num);
        printf(C_DGRAY "|" RESET "  " MENU_TXT_STYLE "%-46s" RESET BOLD C_BORDER "|\n" RESET, items[i].label);
    }

    printf(BOLD C_BORDER "  |" RESET "                                                      " BOLD C_BORDER "|\n" RESET);
    printf(BOLD C_BORDER "  +------------------------------------------------------+\n" RESET);
    printf(BOLD C_BORDER "  |" RESET "  " ERROR_STYLE " 0 " RESET);
    printf(C_DGRAY "|" RESET "  " C_GRAY "%-46s" RESET BOLD C_BORDER "|\n" RESET, "Luu va Thoat chuong trinh");
    printf(BOLD C_BORDER "  +------------------------------------------------------+\n" RESET);
    printf("\n  " BOLD C_CREAM "Lua chon: " RESET);
}

/* -- Safe integer reader ----------------------------------- */
static int readInt(void) {
    int v;
    if (scanf("%d", &v) != 1) {
        while (getchar() != '\n');
        return -99;
    }
    getchar();
    return v;
}

/* ------------------------------------------------------------
   SUB-MENU HELPERS
   ------------------------------------------------------------ */
static void printSubHeader(const char *title) {
    // Width = 40 chars
    printf("\n" BOLD C_BORDER "  +----------------------------------------+\n" RESET);
    printf(BOLD C_BORDER "  |" RESET BOLD C_GOLD "%-40s" RESET BOLD C_BORDER "|\n" RESET, title);
    printf(BOLD C_BORDER "  +----------------------------------------+\n" RESET);
    printf(BOLD C_BORDER "  |" RESET "                                        " BOLD C_BORDER "|\n" RESET);
}

static void printSubItem(const char *num, const char *label, const char *numStyle) {
    printf(BOLD C_BORDER "  |" RESET " %s %s " RESET C_DGRAY "|" RESET "  " MENU_TXT_STYLE "%-33s" RESET BOLD C_BORDER "|\n" RESET,
           numStyle, num, label);
}

static void printSubFooter(void) {
    printf(BOLD C_BORDER "  |" RESET "                                        " BOLD C_BORDER "|\n" RESET);
    printf(BOLD C_BORDER "  +----------------------------------------+\n" RESET);
    printf("\n  " BOLD C_CREAM "Chon: " RESET);
}

/* ------------------------------------------------------------
   SUB-MENUS
   ------------------------------------------------------------ */
static void roomMenu(void) {
    int ch;
    while (1) {
        printSubHeader("   [1]  QUAN LY PHONG");
        printSubItem("1", "Them phong moi",           MENU_NUM_STYLE);
        printSubItem("2", "Sua thong tin phong",       MENU_NUM_STYLE);
        printSubItem("3", "Xoa phong",                 MENU_NUM_STYLE);
        printSubItem("4", "Danh sach phong trong",     MENU_NUM_STYLE);
        printSubItem("5", "Tat ca phong",               MENU_NUM_STYLE);
        printSubItem("6", "Tim kiem phong",             MENU_NUM_STYLE);
        printf(BOLD C_BORDER "  +----------------------------------------+\n" RESET);
        printSubItem("0", "Quay lai menu chinh",        ERROR_STYLE);
        printSubFooter();
        ch = readInt();
        switch (ch) {
            case 1: addRoom(&roomHead);              break;
            case 2: editRoom(roomHead);              break;
            case 3: deleteRoom(&roomHead);           break;
            case 4: showAvailable(roomHead);         pressEnterToContinue(); break;
            case 5: showAllRooms(roomHead);          pressEnterToContinue(); break;
            case 6: searchRoom(roomHead);            pressEnterToContinue(); break;
            case 0: return;
            default: printf(WARNING_STYLE "\n  !  Vui long chon tu 0 den 6!\n" RESET);
        }
    }
}

static void bookingMenu(void) {
    int ch;
    while (1) {
        printSubHeader("   [2]  CHECK-IN / CHECK-OUT");
        printSubItem("1", "Check-in khach hang",        MENU_NUM_STYLE);
        printSubItem("2", "Check-out khach hang",       MENU_NUM_STYLE);
        printSubItem("3", "Phong dang co khach",        MENU_NUM_STYLE);
        printf(BOLD C_BORDER "  +----------------------------------------+\n" RESET);
        printSubItem("0", "Quay lai menu chinh",         ERROR_STYLE);
        printSubFooter();
        ch = readInt();
        switch (ch) {
            case 1: checkIn(roomHead, invoices, &invoiceCount);              break;
            case 2: checkOut(roomHead, invoices, invoiceCount);              break;
            case 3: displayBookedRooms(roomHead); pressEnterToContinue();   break;
            case 0: return;
            default: printf(WARNING_STYLE "\n  !  Khong hop le!\n" RESET);
        }
    }
}

static void serviceMenu_nav(void) {
    int roomNum, ch;
    printSubHeader("   [3]  QUAN LY DICH VU");
    printSubItem("1", "Them dich vu cho phong",     MENU_NUM_STYLE);
    printSubItem("2", "Xem dich vu cua phong",      MENU_NUM_STYLE);
    printSubItem("3", "Quan ly menu dich vu",        MENU_NUM_STYLE);
    printf(BOLD C_BORDER "  +----------------------------------------+\n" RESET);
    printSubItem("0", "Quay lai",                    ERROR_STYLE);
    printSubFooter();
    ch = readInt();
    if (ch == 0) return;
    if (ch == 3) { manageServiceMenu(); return; }
    if (ch != 1 && ch != 2) { printf(WARNING_STYLE "\n  !  Khong hop le!\n" RESET); return; }

    printf("\n  " C_LGRAY "So phong: " RESET);
    roomNum = readInt();

    RoomNode *room = findRoom(roomHead, roomNum);
    if (!room) {
        printf(ERROR_STYLE "  x  Phong %d khong ton tai!\n" RESET, roomNum);
        return;
    }
    if (room->data.status != STATUS_OCCUPIED) {
        printf(WARNING_STYLE "  !  Phong %d dang trong, chua co khach check-in!\n" RESET, roomNum);
        return;
    }

    Invoice *inv = findOpenInvoice(invoices, invoiceCount, roomNum);

    if (!inv) {
        printf(ERROR_STYLE  "  x  Khong tim thay hoa don cho phong %d!\n" RESET, roomNum);
        printf(INFO_STYLE   "  >  Hay check-in truoc khi su dung dich vu.\n" RESET);
        return;
    }
    if (ch == 1) addServiceToInvoice(inv);
    else { showUsedServices(*inv); pressEnterToContinue(); }
}

static void billingMenu(void) {
    int ch;
    while (1) {
        printSubHeader("   [5]  THANH TOAN & HOA DON");
        printSubItem("1", "Xem hoa don theo phong",      MENU_NUM_STYLE);
        printSubItem("2", "Xem tat ca hoa don",          MENU_NUM_STYLE);
        printSubItem("3", "Xuat hoa don ra file",        MENU_NUM_STYLE);
        printf(BOLD C_BORDER "  +----------------------------------------+\n" RESET);
        printSubItem("0", "Quay lai",                     ERROR_STYLE);
        printSubFooter();
        ch = readInt();

        switch (ch) {
            case 1: {
                printf("  " C_LGRAY "So phong: " RESET);
                int roomNum = readInt();
                Invoice *inv = NULL;
                for (int i = invoiceCount - 1; i >= 0; i--){
                    if(invoices[i].targetRoomNumber == roomNum){
                        inv = &invoices[i];
                        break;
                    }
                }
                if (!inv) { printf(ERROR_STYLE "  x  Khong tim thay hoa don!\n" RESET); break; }
                /* Use guest snapshot if available (post-checkout), else read from live room */
                Customer empty; memset(&empty, 0, sizeof(Customer));
                Customer *guest;
                if (inv->guest.fullName[0]) {
                    guest = &inv->guest;                /* checked-out: use snapshot */
                } else {
                    RoomNode *rn = findRoom(roomHead, roomNum);
                    guest = rn ? &rn->data.currentGuest : &empty;  /* still checked-in */
                }
                printInvoice(*inv, *guest);
                pressEnterToContinue();
                break;
            }
            case 2:
                if (invoiceCount == 0) {
                    printf(WARNING_STYLE "  !  Chua co hoa don nao.\n" RESET);
                } else {
                    for (int i = 0; i < invoiceCount; i++) {
                        Customer empty; memset(&empty, 0, sizeof(Customer));
                        Customer *g;
                        if (invoices[i].guest.fullName[0]) {
                            g = &invoices[i].guest;   /* checked-out snapshot */
                        } else {
                            RoomNode *rn = findRoom(roomHead, invoices[i].targetRoomNumber);
                            g = rn ? &rn->data.currentGuest : &empty;
                        }
                        printInvoice(invoices[i], *g);
                    }
                    pressEnterToContinue();
                }
                break;
            case 3: {
                printf("  " C_LGRAY "So phong: " RESET);
                int roomNum = readInt();
                Invoice *inv = NULL;
                for (int i = invoiceCount - 1; i >= 0; i--){
                    if(invoices[i].targetRoomNumber == roomNum){
                        inv = &invoices[i];
                        break;
                    }
                }
                if (!inv) { printf(ERROR_STYLE "  x  Khong tim thay hoa don!\n" RESET); break; }
                /* Same snapshot logic as case 1 */
                Customer empty; memset(&empty, 0, sizeof(Customer));
                Customer *guest;
                if (inv->guest.fullName[0]) {
                    guest = &inv->guest;
                } else {
                    RoomNode *rn = findRoom(roomHead, roomNum);
                    guest = rn ? &rn->data.currentGuest : &empty;
                }
                writeInvoiceToFile(*inv, *guest, "hoadon.txt");
                break;
            }
            case 0: return;
            default: printf(WARNING_STYLE "  !  Khong hop le!\n" RESET);
        }
    }
}

static void dataMenu(void) {
    int ch;
    printSubHeader("   [7]  LUU / TAI DU LIEU");
    printSubItem("1", "Luu du lieu phong",           MENU_NUM_STYLE);
    printSubItem("2", "Tai du lieu phong",           MENU_NUM_STYLE);
    printSubItem("3", "Luu hoa don",                 MENU_NUM_STYLE);
    printSubItem("4", "Tai hoa don",                 MENU_NUM_STYLE);
    printSubItem("5", "Luu tat ca (khuyen dung)",    MENU_NUM_STYLE);
    printf(BOLD C_BORDER "  +----------------------------------------+\n" RESET);
    printSubItem( "0", "Quay lai",                     ERROR_STYLE);
    printSubFooter();
    ch = readInt();
    switch (ch) {
        case 1: animateSpinner("Dang luu phong...", 12, 60);
                saveRoomsToFile(roomHead, FILE_ROOMS);                       break;
        case 2: animateSpinner("Dang tai phong...", 12, 60);
                loadRoomsFromFile(&roomHead, FILE_ROOMS);                    break;
        case 3: animateSpinner("Dang luu hoa don...", 12, 60);
                saveInvoicesToFile(invoices, invoiceCount, FILE_INVOICES);   break;
        case 4: animateSpinner("Dang tai hoa don...", 12, 60);
                loadInvoicesFromFile(invoices, &invoiceCount, FILE_INVOICES); break;
        case 5:
            animateSpinner("Dang luu tat ca du lieu...", 16, 60);
            saveRoomsToFile(roomHead, FILE_ROOMS);
            saveInvoicesToFile(invoices, invoiceCount, FILE_INVOICES);
            printf(SUCCESS_STYLE "  v  Da luu tat ca du lieu!\n" RESET);
            break;
        case 0: return;
        default: printf(WARNING_STYLE "  !  Khong hop le!\n" RESET);
    }
}

/* ------------------------------------------------------------
   MAIN
   ------------------------------------------------------------ */
int main(void) {
#ifdef _WIN32
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    /* Enable ANSI escape sequences on Windows 10+ (define manually if MinGW lacks it) */
    #ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
      #define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
    #endif
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    SetConsoleMode(hOut, dwMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
#endif

    /* Splash + boot animation */
    printSplash();
    printBootSequence();

    /* Load data silently */
    loadRoomsFromFile(&roomHead, FILE_ROOMS);
    loadInvoicesFromFile(invoices, &invoiceCount, FILE_INVOICES);

    clearScreen();
    printSplash();

    int ch;
    while (1) {
        displayMainMenu();
        ch = readInt();

        switch (ch) {
            case 1: roomMenu();                                                 break;
            case 2: bookingMenu();                                              break;
            case 3: serviceMenu_nav();                                          break;
            case 4: handleMembership(roomHead, invoices, invoiceCount);
                    pressEnterToContinue();                                     break;
            case 5: billingMenu();                                              break;
            case 6: printRevenueReport(invoices, invoiceCount);
                    pressEnterToContinue();                                     break;
            case 7: dataMenu();                                                 break;
            case 8: printRoomStats(roomHead);
                    pressEnterToContinue();                                     break;
            case 0:
                printf("\n" INFO_STYLE "  >  Dang luu du lieu...\n" RESET);
                animateSpinner("Luu du lieu...", 10, 80);
                saveRoomsToFile(roomHead, FILE_ROOMS);
                saveInvoicesToFile(invoices, invoiceCount, FILE_INVOICES);
                freeRoomList(&roomHead);
                printf("\n");
                printf(BOLD C_GOLD "  +--------------------------------------+\n" RESET);
                printf(BOLD C_GOLD "  |" RESET C_CREAM "    Cam on da su dung MMLK TechHotel   " RESET BOLD C_GOLD "|\n" RESET);
                printf(BOLD C_GOLD "  |" RESET C_GOLD2 "              Hen gap lai!              " RESET BOLD C_GOLD "|\n" RESET);
                printf(BOLD C_GOLD "  +--------------------------------------+\n\n" RESET);
                return 0;
            default:
                printf(WARNING_STYLE "\n  !  Vui long chon tu 0 den 8.\n" RESET);
        }
    }
}