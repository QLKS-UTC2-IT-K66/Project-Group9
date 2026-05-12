#include <stdio.h>
#include <string.h>
#include "service.h"
#include "ui.h"

Service serviceMenu[MAX_MENU] = {
    {"Giat ui",       50000,  0, 0},
    {"Thue xe may",   100000, 0, 0},
    {"An sang",       30000,  0, 0},
    {"Nuoc loc",      10000,  0, 0},
    {"Nuoc ngot",     15000,  0, 0},
    {"Bia lon",        25000,  0, 0},
    {"Massage 1h",    200000, 0, 0},
    {"Dua don san bay",300000,0, 0},
    {"",               0,     0, 0},
    {"",               0,     0, 0},
    {"",               0,     0, 0},
    {"",               0,     0, 0},
    {"",               0,     0, 0},
    {"",               0,     0, 0},
    {"",               0,     0, 0},
};
int menuSize = 8;

void showServiceMenu(void) {
    // Đã căn chuẩn bảng có độ rộng tổng là 50 ký tự
    printf(HEADER_STYLE "\n  +------------------------------------------------+\n" RESET);
    printf(HEADER_STYLE   "  |                  MENU DICH VU                  |\n" RESET);
    printf(HEADER_STYLE   "  +-------+--------------------------+-------------+\n" RESET);
    printf(HEADER_STYLE   "  | STT   | Ten dich vu              | Don gia     |\n" RESET);
    printf(HEADER_STYLE   "  +-------+--------------------------+-------------+\n" RESET);
    
    for (int i = 0; i < menuSize; i++) {
        if (strlen(serviceMenu[i].serviceName) > 0) {
            // Ép độ rộng chuẩn xác cho 3 cột
            printf("  |  " MENU_NUM_STYLE "%-3d" RESET "  | %-24s | " MONEY_STYLE "%-11.0f" RESET " |\n",
                   i + 1, serviceMenu[i].serviceName, serviceMenu[i].unitPrice);
        }
    }
    printf(HEADER_STYLE "  +-------+--------------------------+-------------+\n" RESET);
    // Dòng tùy chọn 0 được ép độ rộng 39 để nối liền qua 2 cột đầu
    printf("  |  " MENU_NUM_STYLE "%-3s" RESET "  | %-39s |\n", "0", "Khong chon / Thoat");
    printf(HEADER_STYLE "  +------------------------------------------------+\n\n" RESET);
}

void addServiceToInvoice(Invoice *inv) {
    int choice, qty;
    showServiceMenu();
    printf("  Chon dich vu [0-%d]: ", menuSize);
    if (scanf("%d", &choice) != 1) { while (getchar() != '\n'); return; }
    if (choice == 0) return;
    if (choice < 1 || choice > menuSize || strlen(serviceMenu[choice-1].serviceName) == 0) {
        printf(ERROR_STYLE "  ✗ Dich vu khong hop le!\n" RESET);
        getchar(); return;
    }
    printf("  So luong           : ");
    if (scanf("%d", &qty) != 1 || qty <= 0) {
        printf(ERROR_STYLE "  ✗ So luong phai > 0!\n" RESET);
        while (getchar() != '\n'); return;
    }
    getchar();

    if (inv->totalServiceItems >= MAX_SERVICE_USED) {
        printf(ERROR_STYLE "  ✗ Da dat so dich vu toi da!\n" RESET);
        return;
    }

    Service s = serviceMenu[choice - 1];
    s.quantity = qty;
    s.subTotal = s.unitPrice * qty;
    inv->usedServices[inv->totalServiceItems++] = s;
    inv->serviceTotal += s.subTotal;

    printf(SUCCESS_STYLE "  ✓ Da them: %s x%d = %.0f VND\n" RESET,
           s.serviceName, qty, s.subTotal);
    printDivider();
}

void showUsedServices(Invoice inv) {
    if (inv.totalServiceItems == 0) {
        printf(WARNING_STYLE "  ⚠ Chua su dung dich vu nao.\n" RESET);
        return;
    }
    
    // Đã căn chuẩn bảng có độ rộng tổng là 50 ký tự
    printf(HEADER_STYLE "\n  +------------------------------------------------+\n" RESET);
    printf(HEADER_STYLE   "  |               DICH VU DA SU DUNG               |\n" RESET);
    printf(HEADER_STYLE   "  +--------------------------+--------+------------+\n" RESET);
    printf(HEADER_STYLE   "  | Dich vu                  | SL     | Thanh tien |\n" RESET);
    printf(HEADER_STYLE   "  +--------------------------+--------+------------+\n" RESET);
    
    for (int i = 0; i < inv.totalServiceItems; i++) {
        Service s = inv.usedServices[i];
        printf("  | %-24s | %-6d | " MONEY_STYLE "%-10.0f" RESET " |\n",
               s.serviceName, s.quantity, s.subTotal);
    }
    
    printf(HEADER_STYLE   "  +--------------------------+--------+------------+\n" RESET);
    // Độ rộng 33s là tổng hợp chính xác khoảng trống của 2 cột (Dich vu + SL) bao gồm cả viền ở giữa
    printf("  | " BOLD "%-33s" RESET "  | " MONEY_STYLE "%-10.0f" RESET " |\n", "TONG DICH VU", inv.serviceTotal);
    printf(HEADER_STYLE   "  +-----------------------------------+------------+\n\n" RESET);
}

float getTotalServicePrice(Invoice inv) {
    return inv.serviceTotal;
}

void manageServiceMenu(void) {
    int ch;
    printf(HEADER_STYLE "\n  +--------------------------------------+\n" RESET);
    printf(HEADER_STYLE   "  |         QUAN LY MENU DICH VU         |\n" RESET);
    printf(HEADER_STYLE   "  +--------------------------------------+\n\n" RESET);
    printf("    " MENU_NUM_STYLE "1" RESET " → Xem menu hien tai\n");
    printf("    " MENU_NUM_STYLE "2" RESET " → Them dich vu moi\n");
    printf("    " MENU_NUM_STYLE "3" RESET " → Sua gia dich vu\n");
    printf("    " MENU_NUM_STYLE "0" RESET " → Quay lai\n");
    printf("  Chon: ");
    if (scanf("%d", &ch) != 1) { while (getchar() != '\n'); return; }
    getchar();

    switch (ch) {
        case 1: showServiceMenu(); break;
        case 2:
            if (menuSize >= MAX_MENU) { printf(ERROR_STYLE "  ✗ Menu day!\n" RESET); break; }
            printf("  Ten dich vu moi : ");
            fgets(serviceMenu[menuSize].serviceName, 30, stdin);
            serviceMenu[menuSize].serviceName[strcspn(serviceMenu[menuSize].serviceName, "\n")] = '\0';
            printf("  Don gia (VND)   : ");
            scanf("%f", &serviceMenu[menuSize].unitPrice);
            getchar();
            if (serviceMenu[menuSize].unitPrice > 0) {
                menuSize++;
                printf(SUCCESS_STYLE "  ✓ Da them dich vu!\n" RESET);
            } else {
                memset(&serviceMenu[menuSize], 0, sizeof(Service));
                printf(ERROR_STYLE "  ✗ Gia khong hop le!\n" RESET);
            }
            break;
        case 3: {
            int idx;
            showServiceMenu();
            printf("  Chon dich vu can sua gia: ");
            if (scanf("%d", &idx) != 1 || idx < 1 || idx > menuSize) {
                printf(ERROR_STYLE "  ✗ Khong hop le!\n" RESET);
                while (getchar() != '\n'); break;
            }
            printf("  Gia moi (VND): ");
            float newPrice;
            if (scanf("%f", &newPrice) == 1 && newPrice > 0) {
                serviceMenu[idx-1].unitPrice = newPrice;
                printf(SUCCESS_STYLE "  ✓ Da cap nhat gia!\n" RESET);
            } else {
                printf(ERROR_STYLE "  ✗ Gia khong hop le!\n" RESET);
            }
            getchar();
            break;
        }
        default: break;
    }
}
