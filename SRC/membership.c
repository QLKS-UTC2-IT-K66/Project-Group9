#include <stdio.h>
#include <string.h>
#include "membership.h"
#include "billing.h"
#include "room.h"
#include "booking.h"
#include "ui.h"

float calculateDiscount(Customer guest, float price, DiscountSystem config, char *code) {
    float discount = 0;
    if      (guest.membershipLevel == MEMBER_GOLD)   discount += price * config.goldDiscountRate;
    else if (guest.membershipLevel == MEMBER_SILVER)  discount += price * config.silverDiscountRate;
    if (code && strlen(code) > 0 && strcmp(code, config.couponCode) == 0)
        discount += config.cashDiscount;
    return discount;
}

const char* getMembershipLevelName(int level) {
    switch (level) {
        case MEMBER_GOLD:   return "★ Gold";
        case MEMBER_SILVER: return "◈ Silver";
        default:            return "◉ Bronze";
    }
}

int validatePromoCode(const char *code, const char *validCode) {
    if (!code || !validCode || strlen(code) == 0) return 0;
    return strcmp(code, validCode) == 0;
}

void displayDiscountTable(void) {
    printf(HEADER_STYLE "\n  +--------------------------------------------------+\n" RESET);
    printf(HEADER_STYLE   "  |             BANG CHINH SACH GIAM GIA             |\n" RESET);
    printf(HEADER_STYLE   "  +------------+-------------------------------------+\n" RESET);
    printf("  | " FG_YELLOW  "%-10s" RESET " | %-35s |\n", "◉ Bronze", "Khong giam gia");
    printf("  | " FG_WHITE   "%-10s" RESET " | %-35s |\n", "◈ Silver", "Giam  5% tren tong tien");
    printf("  | " MONEY_STYLE "%-10s" RESET " | %-35s |\n", "★ Gold", "Giam 15% tren tong tien");
    printf(HEADER_STYLE   "  +------------+-------------------------------------+\n" RESET);
    printf("  | " FG_BRIGHT_MAGENTA "%-18s" RESET " | %-27s |\n", "Ma KM: WELCOME2026", "Giam them 200,000 VND");
    printf(HEADER_STYLE   "  +--------------------------------------------------+\n\n" RESET);
}

void handleMembership(RoomNode *head, Invoice invoices[], int invoiceCount) {
    int id;
    char code[30];

    DiscountSystem config = { PROMO_CODE, PROMO_CASH_VALUE, RATE_GOLD, RATE_SILVER };

    printf(HEADER_STYLE "\n  +--------------------------------------------------+\n" RESET);
    printf(HEADER_STYLE   "  |             TINH GIAM GIA THANH VIEN             |\n" RESET);
    printf(HEADER_STYLE   "  +--------------------------------------------------+\n\n" RESET);

    displayDiscountTable();

    printf("  Nhap so phong: ");
    if (scanf("%d", &id) != 1) { while (getchar() != '\n'); return; }
    getchar();

    RoomNode *t = findRoom(head, id);
    if (!t) { printf(ERROR_STYLE "  ✗ Khong tim thay phong %d!\n" RESET, id); return; }
    if (t->data.status == STATUS_AVAILABLE) {
        printf(WARNING_STYLE "  ⚠ Phong %d dang trong!\n" RESET, id);
        return;
    }

    // Find invoice for this room
    Invoice *inv = findOpenInvoice(invoices, invoiceCount, id);
    if (!inv) {
        printf(WARNING_STYLE "  ⚠ Khong tim thay hoa don mo cho phong %d!\n" RESET, id);
        printf(INFO_STYLE    "  ℹ Hay check-in truoc khi ap dung giam gia.\n" RESET);
        return;
    }
    
    float totalAmount = inv->roomTotal + inv->serviceTotal;

    printf(INFO_STYLE "\n  Thong tin khach:\n" RESET);
    printf("    Ten   : " HIGHLIGHT_STYLE "%s\n" RESET, t->data.currentGuest.fullName);
    printf("    Hang  : ");
    switch (t->data.currentGuest.membershipLevel) {
        case MEMBER_GOLD:   printf(MONEY_STYLE "★ Gold\n"   RESET); break;
        case MEMBER_SILVER: printf(FG_WHITE    "◈ Silver\n" RESET); break;
        default:            printf(FG_YELLOW   "◉ Bronze\n" RESET); break;
    }
    printf("    Tong uoc tinh: " MONEY_STYLE "%.0f VND\n\n" RESET, totalAmount);

    printf("  Co ma khuyen mai khong? (y/N): ");
    char c = getchar(); getchar();
    memset(code, 0, sizeof(code));
    int promoValid = 0;
    if (c == 'y' || c == 'Y') {
        printf("  Nhap ma: ");
        fgets(code, sizeof(code), stdin);
        code[strcspn(code, "\n")] = '\0';
        promoValid = validatePromoCode(code, config.couponCode);
        if (promoValid)
            printf(SUCCESS_STYLE "  ✓ Ma hop le! Se duoc ap dung chinh xac khi check-out.\n" RESET);
        else
            printf(ERROR_STYLE   "  ✗ Ma khong dung.\n" RESET);
    }

    if (promoValid) {
        strncpy(inv->promoCodeUsed, code, sizeof(inv->promoCodeUsed) - 1);
        inv->promoCodeUsed[sizeof(inv->promoCodeUsed) - 1] = '\0';
    } else {
        memset(inv->promoCodeUsed, 0, sizeof(inv->promoCodeUsed));
    }

    /* --- PREVIEW (display only, local variables, not stored) --- */
    float previewDiscount = calculateDiscount(t->data.currentGuest, totalAmount, config, code);
    float previewAmount   = totalAmount - previewDiscount;
    if (previewAmount < 0) previewAmount = 0;

    printf(HEADER_STYLE "\n  +--------------------------------------------------+\n" RESET);
    printf(HEADER_STYLE   "  |           XEM TRUOC GIAM GIA (DU KIEN)           |\n" RESET);
    printf(HEADER_STYLE   "  +--------------------------------------------------+\n" RESET);
    printf("  |  %-16s: %-27d |\n", "Phong", id);
    printf("  |  %-16s: %-27.27s |\n", "Khach", t->data.currentGuest.fullName);
    printf("  |  %-16s: " MONEY_STYLE "%-23.0f VND" RESET " |\n", "Tong uoc tinh", totalAmount);

    float memberDisc = 0;
    if (t->data.currentGuest.membershipLevel == MEMBER_GOLD) {
        memberDisc = totalAmount * config.goldDiscountRate;
        printf("  |  %-16s: " FG_BRIGHT_GREEN "-%-22.0f VND" RESET " |\n", "Gold (15%)", memberDisc);
    } else if (t->data.currentGuest.membershipLevel == MEMBER_SILVER) {
        memberDisc = totalAmount * config.silverDiscountRate;
        printf("  |  %-16s: " FG_BRIGHT_GREEN "-%-22.0f VND" RESET " |\n", "Silver (5%)", memberDisc);
    }
    if (promoValid)
        printf("  |  %-16s: " FG_BRIGHT_GREEN "-%-22.0f VND" RESET " |\n", "Ma KM", config.cashDiscount);

    printf(HEADER_STYLE "  +--------------------------------------------------+\n" RESET);
    printf("  |  %-16s: " FG_BRIGHT_RED "-%-22.0f VND" RESET " |\n", "Tong giam (DK)", previewDiscount);
    printf(HEADER_STYLE "  +--------------------------------------------------+\n" RESET);
    printf("  |  " BOLD "%-16s: " MONEY_STYLE "%-23.0f VND" RESET " |\n", "DK THANH TOAN", previewAmount);
    printf(HEADER_STYLE "  +--------------------------------------------------+\n" RESET);
    printf("  | " WARNING_STYLE "%-48s" RESET " |\n", " ℹ So tien thuc te se tinh chinh xac khi check-out.");
    printf(HEADER_STYLE "  +--------------------------------------------------+\n\n" RESET);
}
