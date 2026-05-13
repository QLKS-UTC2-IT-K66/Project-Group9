#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "booking.h"
#include "customer.h"
#include "room.h"
#include "billing.h"
#include "ui.h"

// Nhập ngày, tháng, năm
void getDateInput(char *date, const char *prompt) {
    do {
        printf("  %s (dd/mm/yyyy): ", prompt);
        fgets(date, 20, stdin);
        date[strcspn(date, "\n")] = '\0';
        if (strlen(date) == 0)
            printf(WARNING_STYLE "  ⚠ Vui lòng nhập ngày!\n" RESET);
    } while (strlen(date) == 0);
}

// Check in khách hàng
void checkIn(RoomNode *head, Invoice invoices[], int *invoiceCount) {
    int roomNumber;
    printf(HEADER_STYLE "\n  +--------------------------------------+\n" RESET);
    printf(HEADER_STYLE   "  |         CHECK-IN KHACH HANG          |\n" RESET);
    printf(HEADER_STYLE   "  +--------------------------------------+\n\n" RESET);

    printf("  Nhap so phong: ");
    if (scanf("%d", &roomNumber) != 1) {
        printf(ERROR_STYLE "  ✗ So phong khong hop le!\n" RESET);
        while (getchar() != '\n'); return;
    }
    getchar();

    RoomNode *room = findRoom(head, roomNumber);
    if (!room) {
        printf(ERROR_STYLE "  ✗ Khong tim thay phong %d!\n" RESET, roomNumber);
        return;
    }
    if (room->data.status == STATUS_OCCUPIED) {
        printf(ERROR_STYLE "  ✗ Phong %d dang co khach!\n" RESET, roomNumber);
        return;
    }
    if (*invoiceCount >= MAX_INVOICES) {
        printf(ERROR_STYLE "  ✗ Da dat gioi han hoa don!\n" RESET);
        return;
    }

    Customer customer;
    memset(&customer, 0, sizeof(Customer));
    inputCustomer(&customer);
    if (!validateCustomer(&customer)) return;

    char checkInDate[20];
    getDateInput(checkInDate, "Ngay nhan phong");

    char expectedCheckOut[20];
    getDateInput(expectedCheckOut, "Ngay tra phong du kien");

    // Tính số ngày và tiền phòng dự kiến ngay tại check-in
    int inD, inM, inY, outD, outM, outY;
    sscanf(checkInDate,      "%d/%d/%d", &inD,  &inM,  &inY);
    sscanf(expectedCheckOut, "%d/%d/%d", &outD, &outM, &outY);
    int days = (outY - inY)*365 + (outM - inM)*30 + (outD - inD);
    if (days < 1) days = 1;

    // Assign guest to room
    room->data.currentGuest = customer;
    room->data.status        = STATUS_OCCUPIED;
    strncpy(room->data.checkInDate, checkInDate, sizeof(room->data.checkInDate)-1);

    // Tạo hóa đơn mở 
    Invoice *inv = &invoices[*invoiceCount];
    memset(inv, 0, sizeof(Invoice));
    sprintf(inv->invoiceID, "INV-%04d", *invoiceCount + 1);
    inv->targetRoomNumber = roomNumber;
    strncpy(inv->checkInDate, checkInDate, sizeof(inv->checkInDate)-1);
    strcpy(inv->checkOutDate, "---");
    inv->stayDuration = days;
    inv->roomTotal = room->data.basePrice * days;
    (*invoiceCount)++;

    printf(SUCCESS_STYLE "\n  ✓ Check-in thành công!\n" RESET);
    printf(INFO_STYLE    "  ℹ Phòng   : %d | Khách: %s\n" RESET, roomNumber, customer.fullName);
    printf(INFO_STYLE    "  ℹ Nhận    : %s | Dự kiến trả  : %s (%d ngày)\n" RESET, checkInDate, expectedCheckOut, days);
    printf(MONEY_STYLE   "  ℹ Tiền phòng dự kiến: %.0f VND\n" RESET, inv->roomTotal);
    printf(INFO_STYLE    "  ℹ Hóa đơn : %s\n" RESET, inv->invoiceID);
    printDivider();
}

// Check out khách hàng
void checkOut(RoomNode *head, Invoice invoices[], int invoiceCount) {
    int roomNumber;
    printf(HEADER_STYLE "\n  +--------------------------------------+\n" RESET);
    printf(HEADER_STYLE   "  |        CHECK-OUT KHACH HANG          |\n" RESET);
    printf(HEADER_STYLE   "  +--------------------------------------+\n\n" RESET);

    printf("  Nhap so phong: ");
    if (scanf("%d", &roomNumber) != 1) {
        printf(ERROR_STYLE "  ✗ So phong khong hop le!\n" RESET);
        while (getchar() != '\n'); return;
    }
    getchar();

    RoomNode *room = findRoom(head, roomNumber);
    if (!room) { printf(ERROR_STYLE "  ✗ Khong tim thay phong!\n" RESET); return; }
    if (room->data.status == STATUS_AVAILABLE) {
        printf(WARNING_STYLE "  ⚠ Phong %d dang trong!\n" RESET, roomNumber);
        return;
    }

    // Tìm hóa đơn mở cho số phòng muốn check out
    Invoice *inv = findOpenInvoice(invoices, invoiceCount, roomNumber);

    if (!inv) {
        printf(WARNING_STYLE "  ⚠ Khong tim thay hoa don mo cho phong %d!\n" RESET, roomNumber);
        return;
    }

    char checkOutDate[20];
    getDateInput(checkOutDate, "Ngay tra phong");

    finalizeInvoice(inv, room->data, checkOutDate, room->data.currentGuest);

    // Giải phóng phòng về trạng thái trống sau khi check out
    room->data.status = STATUS_AVAILABLE;
    memset(&room->data.currentGuest, 0, sizeof(Customer));
    memset(room->data.checkInDate, 0, sizeof(room->data.checkInDate));

    printf(SUCCESS_STYLE "\n  ✓ Check-out phong %d thanh cong!\n" RESET, roomNumber);
    printf(INFO_STYLE    "  ℹ So ngay luu tru : %d ngay\n"  RESET, inv->stayDuration);
    printf(MONEY_STYLE   "  ℹ Tong tien phong : %.0f VND\n" RESET, inv->roomTotal);
    printf(MONEY_STYLE   "  ℹ Tong tien dich vu : %.0f VND\n" RESET, inv->serviceTotal);
    if (inv->totalDiscount > 0)
        printf(FG_BRIGHT_GREEN "  ℹ Giam gia        : -%.0f VND\n" RESET, inv->totalDiscount);
    printf(MONEY_STYLE   "  ℹ Thanh toan      : %.0f VND\n" RESET, inv->netAmount);
    printDivider();
}

// Hiển thị phòng đã đặt
void displayBookedRooms(RoomNode *head) {
    printf(HEADER_STYLE "\n  +--------------------------------------------------------------------+\n" RESET);
    printf(HEADER_STYLE   "  |                       DANH SACH PHONG DA DAT                       |\n" RESET);
    printf(HEADER_STYLE   "  +-------+----------+----------------------+-------------+------------+\n" RESET);
    printf(HEADER_STYLE   "  | Phong | Loai     | Khach                | CMND/CCCD   | Ngay vao   |\n" RESET);
    printf(HEADER_STYLE   "  +-------+----------+----------------------+-------------+------------+\n" RESET);

    int count = 0;
    while (head) {
        if (head->data.status == STATUS_OCCUPIED) {
            printf("  | " HIGHLIGHT_STYLE "%-5d" RESET " | %-8s | %-20.20s | %-11s | %-10s |\n",
                   head->data.roomNumber,
                   getRoomTypeName(head->data.roomType),
                   head->data.currentGuest.fullName,
                   head->data.currentGuest.idCard,
                   head->data.checkInDate);
            count++;
        }
        head = head->next;
    }

    printf(HEADER_STYLE "  +-------+----------+----------------------+-------------+------------+\n" RESET);
    if (count == 0) printf(WARNING_STYLE "  ⚠ Khong co phong nao dang co khach.\n" RESET);
    else            printf(INFO_STYLE    "  ℹ Co %d phong dang co khach.\n" RESET, count);
    printDivider();
}