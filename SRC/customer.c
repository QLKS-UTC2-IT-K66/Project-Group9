#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "customer.h"
#include "ui.h"

int validatePhoneNumber(char *phone) {
    size_t len = strlen(phone);
    if (len < 10 || len > 11) return 0;
    for (size_t i = 0; i < len; i++)
        if (!isdigit((unsigned char)phone[i])) return 0;
    return 1;
}

int validateIdCard(char *idCard) {
    size_t len = strlen(idCard);
    if (len < 9 || len > 12) return 0;
    for (size_t i = 0; i < len; i++)
        if (!isdigit((unsigned char)idCard[i])) return 0;
    return 1;
}

const char* getMembershipLabel(int level) {
    switch (level) {
        case MEMBER_GOLD:   return "★ Gold  ";
        case MEMBER_SILVER: return "◈ Silver";
        default:            return "◉ Bronze";
    }
}

void inputCustomer(Customer *customer) {
    printf(HEADER_STYLE "\n  +------------------------------------------+\n" RESET);
    printf(HEADER_STYLE   "  |       NHAP THONG TIN KHACH HANG          |\n" RESET);
    printf(HEADER_STYLE   "  +------------------------------------------+\n\n" RESET);

    printf(MENU_TXT_STYLE "  Họ va ten     : " RESET);
    fgets(customer->fullName, sizeof(customer->fullName), stdin);
    customer->fullName[strcspn(customer->fullName, "\n")] = '\0';

    printf(MENU_TXT_STYLE "  CMND/CCCD     : " RESET);
    fgets(customer->idCard, sizeof(customer->idCard), stdin);
    customer->idCard[strcspn(customer->idCard, "\n")] = '\0';

    printf(MENU_TXT_STYLE "  So dien thoai : " RESET);
    fgets(customer->phoneNumber, sizeof(customer->phoneNumber), stdin);
    customer->phoneNumber[strcspn(customer->phoneNumber, "\n")] = '\0';

    printf(MENU_TXT_STYLE "  Email         : " RESET);
    fgets(customer->email, sizeof(customer->email), stdin);
    customer->email[strcspn(customer->email, "\n")] = '\0';

    printf(MENU_TXT_STYLE "  Quoc tich     : " RESET);
    fgets(customer->nationality, sizeof(customer->nationality), stdin);
    customer->nationality[strcspn(customer->nationality, "\n")] = '\0';
    if (strlen(customer->nationality) == 0) strcpy(customer->nationality, "Viet Nam");

    printf(MENU_TXT_STYLE "\n  Hang thanh vien:\n" RESET);
    printf("    " MENU_NUM_STYLE "0" RESET " → " FG_BRIGHT_WHITE "Bronze" RESET " (Mac dinh)\n");
    printf("    " MENU_NUM_STYLE "1" RESET " → " FG_WHITE        "Silver" RESET " (Giam 5%%)\n");
    printf("    " MENU_NUM_STYLE "2" RESET " → " MONEY_STYLE     "Gold  " RESET " (Giam 15%%)\n");
    printf(MENU_TXT_STYLE "  Chọn hang [0-2]: " RESET);
    scanf("%d", &customer->membershipLevel);
    getchar();
    if (customer->membershipLevel < MEMBER_BRONZE || customer->membershipLevel > MEMBER_GOLD)
        customer->membershipLevel = MEMBER_BRONZE;
}

int validateCustomer(Customer *customer) {
    if (strlen(customer->fullName) == 0) {
        printf(ERROR_STYLE "  ✗ Vui long nhap họ ten!\n" RESET);
        return 0;
    }
    if (!validateIdCard(customer->idCard)) {
        printf(ERROR_STYLE "  ✗ CMND/CCCD khong hop le (9-12 chu so)!\n" RESET);
        return 0;
    }
    if (!validatePhoneNumber(customer->phoneNumber)) {
        printf(ERROR_STYLE "  ✗ So dien thoai khong hop le (10-11 chu so)!\n" RESET);
        return 0;
    }
    return 1;
}

void displayCustomer(Customer customer) {
    printf("  %-16s: " HIGHLIGHT_STYLE "%s\n" RESET, "Họ ten",       customer.fullName);
    printf("  %-16s: %s\n",                            "CMND/CCCD",   customer.idCard);
    printf("  %-16s: %s\n",                            "Dien thoai",  customer.phoneNumber);
    printf("  %-16s: %s\n",                            "Email",       customer.email);
    printf("  %-16s: %s\n",                            "Quoc tich",   customer.nationality);
    printf("  %-16s: ", "Hang thanh vien");
    switch (customer.membershipLevel) {
        case MEMBER_GOLD:   printf(MONEY_STYLE "★ Gold\n"   RESET); break;
        case MEMBER_SILVER: printf(FG_WHITE    "◈ Silver\n" RESET); break;
        default:            printf(FG_YELLOW   "◉ Bronze\n" RESET); break;
    }
}
