#include <stdio.h>
#include <string.h>
#include "billing.h"
#include "membership.h"
#include "ui.h"

// Tìm hóa đơn mở
Invoice* findOpenInvoice(Invoice invoices[], int count, int roomNumber) {
    for (int i = count - 1; i >=0; i--) {
        if (invoices[i].targetRoomNumber == roomNumber && strcmp(invoices[i].checkOutDate, "---") == 0) 
        return &invoices[i];
    }
    return NULL;
}

// Tạo hóa đơn đầy đủ
void createInvoice(Invoice *inv, Room room, int stayDuration, float discount) {
    if (stayDuration < 1) stayDuration = 1;
    inv->stayDuration  = stayDuration;
    inv->roomTotal     = room.basePrice * stayDuration;
    float total        = inv->roomTotal + inv->serviceTotal;
    inv->totalDiscount = (discount > total) ? total : discount;
    inv->netAmount     = total - inv->totalDiscount;
    if (inv->netAmount < 0) inv->netAmount = 0;
}

// Hoàn tất hóa đơn 
void finalizeInvoice(Invoice *inv, Room room, const char *actualCheckOutDate, Customer guest) {
    strncpy(inv->checkOutDate, actualCheckOutDate, sizeof(inv->checkOutDate) - 1);
    inv->checkOutDate[sizeof(inv->checkOutDate) - 1] = '\0';

    int inD, inM, inY, outD, outM, outY;
    sscanf(inv->checkInDate,  "%d/%d/%d", &inD,  &inM,  &inY);
    sscanf(inv->checkOutDate, "%d/%d/%d", &outD, &outM, &outY);
    int days = (outY - inY)*365 + (outM - inM)*30 + (outD - inD);
    if (days < 1) days = 1;
    inv->stayDuration = days;

    inv->roomTotal = room.basePrice * days;

    DiscountSystem config = { PROMO_CODE, PROMO_CASH_VALUE, RATE_GOLD, RATE_SILVER };
    float totalAmount = inv->roomTotal + inv->serviceTotal;
    inv->totalDiscount = calculateDiscount(guest, totalAmount, config, inv->promoCodeUsed);

    if (inv->totalDiscount > totalAmount) inv->totalDiscount = totalAmount;
    inv->netAmount = totalAmount - inv->totalDiscount;
    if (inv->netAmount < 0) inv->netAmount = 0;
    
    inv->guest = guest;
}

// In ra hóa đơn
static void printInvoiceBox(Invoice invoice, Customer guest,
                             void (*printer)(const char*, ...)) {
    (void)printer; 
    
    printf(HEADER_STYLE "\n  +--------------------------------------------------+\n" RESET);
    printf(HEADER_STYLE   "  |               HOA DON THANH TOAN                 |\n" RESET);
    printf(HEADER_STYLE   "  |                 MMLK TechHotel                   |\n" RESET);
    printf(HEADER_STYLE   "  +--------------------------------------------------+\n" RESET);
    
    // Cấu trúc 1 dòng: khoảng trống (2) + label (14) + ": " (2) + data (31) + khoảng trống (1) = 50 ký tự
    printf("  |  %-14s: " HIGHLIGHT_STYLE "%-31s" RESET " |\n", "Ma hoa don", invoice.invoiceID);
    printf("  |  %-14s: " HIGHLIGHT_STYLE "%-31d" RESET " |\n", "Phong so", invoice.targetRoomNumber);
    printf("  |  %-14s: %-31.31s |\n", "Ten khach", guest.fullName[0] ? guest.fullName : "---");
    printf("  |  %-14s: %-31.31s |\n", "Dien thoai", guest.phoneNumber[0] ? guest.phoneNumber : "---");
    printf("  |  %-14s: %-31s |\n", "Ngay nhan", invoice.checkInDate);
    printf("  |  %-14s: %-31s |\n", "Ngay tra", invoice.checkOutDate);
    printf("  |  %-14s: %-31d |\n", "So ngay", invoice.stayDuration);
    
    printf(HEADER_STYLE "  +--------------------------------------------------+\n" RESET);
    
    // Đối với số tiền: độ dài 31 ký tự bao gồm cả số và chữ " VND"
    printf("  |  %-14s: " MONEY_STYLE "%-27.0f VND" RESET " |\n", "Tien phong", invoice.roomTotal);
    printf("  |  %-14s: " MONEY_STYLE "%-27.0f VND" RESET " |\n", "Tien dich vu", invoice.serviceTotal);
    
    if (invoice.totalDiscount > 0) {
        printf("  |  %-14s: " FG_BRIGHT_GREEN "-%-26.0f VND" RESET " |\n", "Giam gia", invoice.totalDiscount);
    }
    
    printf(HEADER_STYLE "  +--------------------------------------------------+\n" RESET);
    printf("  |  " BOLD "%-14s: " MONEY_STYLE "%-27.0f VND" RESET " |\n", "TONG CONG", invoice.netAmount);
    printf(HEADER_STYLE "  +--------------------------------------------------+\n\n" RESET);
}

// In hóa đơn ra màn hình
void printInvoice(Invoice invoice, Customer guest) {
    printInvoiceBox(invoice, guest, NULL);
}

// Viết hóa đơn ra file 
void writeInvoiceToFile(Invoice invoice, Customer guest, const char *filename) {
    if (strcmp(invoice.checkOutDate, "---") == 0) {
        printf(WARNING_STYLE "  ⚠ Hoa don chua hoan tat (chua check-out)!\n" RESET);
        return;
    }
    FILE *f = fopen(filename, "a");
    if (!f) { printf(ERROR_STYLE "  ✗ Khong the mo file!\n" RESET); return; }

    fprintf(f, "\n------------------------------------------\n");
    fprintf(f, "           HOA DON THANH TOAN\n");
    fprintf(f, "         MMLK TechHotel\n");
    fprintf(f, "------------------------------------------\n");
    fprintf(f, "Ma HD      : %s\n", invoice.invoiceID);
    fprintf(f, "Phong      : %d\n", invoice.targetRoomNumber);
    fprintf(f, "Khach      : %s\n", guest.fullName);
    fprintf(f, "Dien thoai : %s\n", guest.phoneNumber);
    fprintf(f, "Nhan phong : %s\n", invoice.checkInDate);
    fprintf(f, "Tra phong  : %s\n", invoice.checkOutDate);
    fprintf(f, "So ngay    : %d\n", invoice.stayDuration);
    fprintf(f, "------------------------------------------\n");
    fprintf(f, "Tien phong : %.0f VND\n", invoice.roomTotal);
    fprintf(f, "Dich vu    : %.0f VND\n", invoice.serviceTotal);
    if (invoice.totalDiscount > 0)
        fprintf(f, "Giam gia   : -%.0f VND\n", invoice.totalDiscount);
    fprintf(f, "------------------------------------------\n");
    fprintf(f, "TONG CONG  : %.0f VND\n", invoice.netAmount);
    fprintf(f, "------------------------------------------\n\n");
    fclose(f);
    printf(SUCCESS_STYLE "  ✓ Da xuat hoa don ra file '%s'!\n" RESET, filename);
}

// In báo cáo doanh thu ra màn hình
void printRevenueReport(Invoice invoices[], int count) {
    float revenueByMonth[13] = {0};
    float totalRevenue = 0;
    int   invoiceByMonth[13] = {0};

    for (int i = 0; i < count; i++) {
        int d, m, y;
        if (sscanf(invoices[i].checkOutDate, "%d/%d/%d", &d, &m, &y) == 3) {
            if (m >= 1 && m <= 12) {
                revenueByMonth[m]  += invoices[i].netAmount;
                invoiceByMonth[m]++;
                totalRevenue       += invoices[i].netAmount;
            }
        }
    }

    printf(HEADER_STYLE "\n  +---------------------------------------------------+\n" RESET);
    printf(HEADER_STYLE   "  |                 BAO CAO DOANH THU                 |\n" RESET);
    printf(HEADER_STYLE   "  +-------+---------------------------+-------------+\n" RESET);
    printf(HEADER_STYLE   "  | Thang | Doanh thu (VND)           | Hoa don     |\n" RESET);
    printf(HEADER_STYLE   "  +-------+---------------------------+-------------+\n" RESET);

    int hasData = 0;
    for (int m = 1; m <= 12; m++) {
        if (revenueByMonth[m] > 0) {
            hasData = 1;
            printf("  |  %2d   | " MONEY_STYLE "%-25.0f" RESET " | %-11d |\n", m, revenueByMonth[m], invoiceByMonth[m]);
        }
    }

    if (!hasData) printf("  | " WARNING_STYLE "Chua co du lieu doanh thu." RESET "                      |\n");

    printf(HEADER_STYLE "  +-------+---------------------------+-------------+\n" RESET);
    printf("  | " BOLD "Tong doanh thu  : " MONEY_STYLE "%-29.0f" RESET " |\n", totalRevenue);
    printf("  | Tong hoa don    : %-29d |\n", count);
    printf("  | Trung binh/Don  : " MONEY_STYLE "%-29.0f" RESET " |\n",
           count > 0 ? totalRevenue / count : 0);
    printf(HEADER_STYLE "  +---------------------------------------------------+\n\n" RESET);
}