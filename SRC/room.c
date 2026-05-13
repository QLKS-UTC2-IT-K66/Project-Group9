#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "room.h"
#include "ui.h"

RoomNode* createRoomNode(Room r) {
    RoomNode *p = (RoomNode*)malloc(sizeof(RoomNode));
    if (!p) return NULL;
    p->data = r;
    p->next = NULL;
    return p;
}

const char* getRoomTypeName(int roomType) {
    switch (roomType) {
        case TYPE_SINGLE: return "Single";
        case TYPE_DOUBLE: return "Double";
        case TYPE_VIP:    return "VIP   ";
        default:          return "N/A   ";
    }
}

static void printRoomHeader(void) {
    // Độ rộng bảng cố định là 75 ký tự '-'
    printf(HEADER_STYLE "  +-------+----------+-----------------+-------------+----------------------+\n" RESET);
    printf(HEADER_STYLE "  | Phong | Loai     | Gia/dem         | Trang thai  | Khach                |\n" RESET);
    printf(HEADER_STYLE "  +-------+----------+-----------------+-------------+----------------------+\n" RESET);
}

static void printRoomRow(Room *r) {
    // Đã điều chỉnh các string có chính xác số lượng ký tự hiển thị để canh lề khớp header
    const char *status  = (r->status == STATUS_AVAILABLE) ? SUCCESS_STYLE "Trong      " RESET
                                                          : ERROR_STYLE   "Co khach   " RESET;
    const char *guest   = (r->status == STATUS_OCCUPIED)  ? r->currentGuest.fullName : "---";
    const char *typeCol;
    switch (r->roomType) {
        case TYPE_VIP:    typeCol = MONEY_STYLE "VIP     " RESET; break;
        case TYPE_DOUBLE: typeCol = FG_CYAN     "Double  " RESET; break;
        default:          typeCol = FG_WHITE    "Single  " RESET; break;
    }
    
    printf("  | " HIGHLIGHT_STYLE "%-5d" RESET " | %s | " MONEY_STYLE "%11.0f VND" RESET " | %s | %-20.20s |\n",
           r->roomNumber, typeCol, r->basePrice, status, guest);
}

static void printRoomFooter(void) {
    printf(HEADER_STYLE "  +-------+----------+-----------------+-------------+----------------------+\n" RESET);
}

void addRoom(RoomNode **head) {
    Room r;
    memset(&r, 0, sizeof(Room));

    printf(HEADER_STYLE "\n  +--------------------------------------+\n" RESET);
    printf(HEADER_STYLE   "  |            THEM PHONG MOI            |\n" RESET);
    printf(HEADER_STYLE   "  +--------------------------------------+\n\n" RESET);

    printf("  So phong   : ");
    if (scanf("%d", &r.roomNumber) != 1) {
        printf(ERROR_STYLE "  ✗ So phong khong hop le!\n" RESET);
        while (getchar() != '\n'); return;
    }
    if (findRoom(*head, r.roomNumber) != NULL) {
        printf(ERROR_STYLE "  ✗ Phong %d da ton tai!\n" RESET, r.roomNumber);
        getchar(); return;
    }

    printf("\n  Loai phong :\n");
    printf("    " MENU_NUM_STYLE "1" RESET " → Single  – Phong don tieu chuan\n");
    printf("    " MENU_NUM_STYLE "2" RESET " → Double  – Phong doi\n");
    printf("    " MENU_NUM_STYLE "3" RESET " → " MONEY_STYLE "VIP   " RESET "  – Phong cao cap\n");
    printf("  Chọn [1-3] : ");
    if (scanf("%d", &r.roomType) != 1 || r.roomType < 1 || r.roomType > 3) {
        printf(ERROR_STYLE "  ✗ Loai phong khong hop le!\n" RESET);
        while (getchar() != '\n'); return;
    }

    printf("  Gia/dem    : ");
    if (scanf("%f", &r.basePrice) != 1 || r.basePrice <= 0) {
        printf(ERROR_STYLE "  ✗ Gia khong hop le!\n" RESET);
        while (getchar() != '\n'); return;
    }
    getchar();

    r.status = STATUS_AVAILABLE;

    RoomNode *p = createRoomNode(r);
    if (!p) { printf(ERROR_STYLE "  ✗ Khong du bo nho!\n" RESET); return; }

    if (!*head) *head = p;
    else {
        RoomNode *t = *head;
        while (t->next) t = t->next;
        t->next = p;
    }

    printf(SUCCESS_STYLE "\n  ✓ Them phong %d (%s) thanh cong!\n" RESET,
           r.roomNumber, getRoomTypeName(r.roomType));
    printDivider();
}

void editRoom(RoomNode *head) {
    int id;
    printf(HEADER_STYLE "\n  +--------------------------------------+\n" RESET);
    printf(HEADER_STYLE   "  |          SUA THONG TIN PHONG         |\n" RESET);
    printf(HEADER_STYLE   "  +--------------------------------------+\n\n" RESET);

    printf("  So phong can sua: ");
    if (scanf("%d", &id) != 1) { while (getchar() != '\n'); return; }
    getchar();

    while (head) {
        if (head->data.roomNumber == id) {
            printf(INFO_STYLE "\n  Thong tin hien tai:\n" RESET);
            printf("    Loai : %s  |  Gia : %.0f VND\n\n",
                   getRoomTypeName(head->data.roomType), head->data.basePrice);

            printf("  Loai moi (1=Single, 2=Double, 3=VIP): ");
            int newType;
            if (scanf("%d", &newType) != 1 || newType < 1 || newType > 3) {
                printf(ERROR_STYLE "  ✗ Loai khong hop le!\n" RESET);
                while (getchar() != '\n'); return;
            }

            printf("  Gia moi (VND)                       : ");
            float newPrice;
            if (scanf("%f", &newPrice) != 1 || newPrice <= 0) {
                printf(ERROR_STYLE "  ✗ Gia khong hop le!\n" RESET);
                while (getchar() != '\n'); return;
            }
            getchar();

            head->data.roomType  = newType;
            head->data.basePrice = newPrice;
            printf(SUCCESS_STYLE "\n  ✓ Cap nhat phong %d thanh cong!\n" RESET, id);
            printDivider();
            return;
        }
        head = head->next;
    }
    printf(ERROR_STYLE "  ✗ Khong tim thay phong %d!\n" RESET, id);
}

void deleteRoom(RoomNode **head) {
    int id;
    printf(HEADER_STYLE "\n  +--------------------------------------+\n" RESET);
    printf(HEADER_STYLE   "  |              XOA PHONG               |\n" RESET);
    printf(HEADER_STYLE   "  +--------------------------------------+\n\n" RESET);

    printf("  So phong can xoa: ");
    if (scanf("%d", &id) != 1) { while (getchar() != '\n'); return; }
    getchar();

    RoomNode *t = *head, *prev = NULL;
    while (t) {
        if (t->data.roomNumber == id) {
            if (t->data.status == STATUS_OCCUPIED) {
                printf(WARNING_STYLE "  ⚠ Phong dang co khach! Ban chac muon xoa? (y/N): " RESET);
                char c = getchar(); getchar();
                if (c != 'y' && c != 'Y') { printf(INFO_STYLE "  ℹ Huy thao tac.\n" RESET); return; }
            } else {
                printf(WARNING_STYLE "  ⚠ Xac nhan xoa phong %d? (y/N): " RESET, id);
                char c = getchar(); getchar();
                if (c != 'y' && c != 'Y') { printf(INFO_STYLE "  ℹ Huy thao tac.\n" RESET); return; }
            }
            if (!prev) *head = t->next;
            else        prev->next = t->next;
            free(t);
            printf(SUCCESS_STYLE "  ✓ Da xoa phong %d.\n" RESET, id);
            printDivider();
            return;
        }
        prev = t; t = t->next;
    }
    printf(ERROR_STYLE "  ✗ Khong tim thay phong %d!\n" RESET, id);
}

void showAvailable(RoomNode *head) {
    printf(HEADER_STYLE "\n  +---------------------------------------------------------------------------+\n" RESET);
    printf(HEADER_STYLE   "  |                           DANH SACH PHONG TRONG                           |\n" RESET);
    printf(HEADER_STYLE   "  +---------------------------------------------------------------------------+\n" RESET);

    printRoomHeader();
    int count = 0;
    while (head) {
        if (head->data.status == STATUS_AVAILABLE) {
            printRoomRow(&head->data);
            count++;
        }
        head = head->next;
    }
    printRoomFooter();

    if (count == 0)
        printf(WARNING_STYLE "  ⚠ Khong co phong trong!\n" RESET);
    else
        printf(SUCCESS_STYLE "  ✓ Co %d phong dang trong.\n" RESET, count);
    printDivider();
}

void showAllRooms(RoomNode *head) {
    printf(HEADER_STYLE "\n  +---------------------------------------------------------------------------+\n" RESET);
    printf(HEADER_STYLE   "  |                           DANH SACH TAT CA PHONG                          |\n" RESET);
    printf(HEADER_STYLE   "  +---------------------------------------------------------------------------+\n" RESET);

    printRoomHeader();
    int count = 0;
    while (head) {
        printRoomRow(&head->data);
        count++;
        head = head->next;
    }
    printRoomFooter();
    printf(INFO_STYLE "  ℹ Tong so phong: %d\n" RESET, count);
    printDivider();
}

void searchRoom(RoomNode *head) {
    int id;
    printf(HEADER_STYLE "\n  +--------------------------------------+\n" RESET);
    printf(HEADER_STYLE   "  |            TIM KIEM PHONG            |\n" RESET);
    printf(HEADER_STYLE   "  +--------------------------------------+\n\n" RESET);
    printf("  Nhap so phong: ");
    if (scanf("%d", &id) != 1) { while (getchar() != '\n'); return; }
    getchar();

    RoomNode *found = findRoom(head, id);
    if (!found) {
        printf(ERROR_STYLE "  ✗ Khong tim thay phong %d!\n" RESET, id);
        return;
    }
    Room *r = &found->data;
    printf(HEADER_STYLE "\n  +------------------------------------+\n" RESET);
    printf(HEADER_STYLE   "  |          CHI TIET PHONG %-4d       |\n" RESET, r->roomNumber);
    printf(HEADER_STYLE   "  +------------------------------------+\n\n" RESET);
    printf("  Loai phong  : %s\n", getRoomTypeName(r->roomType));
    printf("  Gia/dem     : " MONEY_STYLE "%.0f VND\n" RESET, r->basePrice);
    printf("  Trang thai  : ");
    if (r->status == STATUS_AVAILABLE)
        printf(SUCCESS_STYLE "Trong\n" RESET);
    else {
        printf(ERROR_STYLE "Da co khach\n\n" RESET);
        printf(HEADER_STYLE "  --- Thong tin khach ---\n" RESET);
        printf("  Ten         : " HIGHLIGHT_STYLE "%s\n" RESET, r->currentGuest.fullName);
        printf("  CMND/CCCD   : %s\n",  r->currentGuest.idCard);
        printf("  Dien thoai  : %s\n",  r->currentGuest.phoneNumber);
        printf("  Ngay nhan   : %s\n",  r->checkInDate);
    }
    printDivider();
}

RoomNode* findRoom(RoomNode *head, int roomNumber) {
    while (head) {
        if (head->data.roomNumber == roomNumber) return head;
        head = head->next;
    }
    return NULL;
}

int countRooms(RoomNode *head) {
    int c = 0;
    while (head) { c++; head = head->next; }
    return c;
}

int countAvailableRooms(RoomNode *head) {
    int c = 0;
    while (head) { if (head->data.status == STATUS_AVAILABLE) c++; head = head->next; }
    return c;
}

void printRoomStats(RoomNode *head) {
    int total    = countRooms(head);
    int avail    = countAvailableRooms(head);
    int occupied = total - avail;
    int single = 0, dbl = 0, vip = 0;
    RoomNode *cur = head;
    while (cur) {
        if (cur->data.roomType == TYPE_SINGLE) single++;
        else if (cur->data.roomType == TYPE_DOUBLE) dbl++;
        else if (cur->data.roomType == TYPE_VIP)    vip++;
        cur = cur->next;
    }
    float rate = (total > 0) ? (float)occupied / total * 100 : 0;

    // Căn chuẩn border 40 ký tự
    printf(HEADER_STYLE "\n  +----------------------------------------+\n" RESET);
    printf(HEADER_STYLE   "  |          THONG KE PHONG NHANH          |\n" RESET);
    printf(HEADER_STYLE   "  +----------------------------------------+\n" RESET);
    printf("  | Tong phong : " HIGHLIGHT_STYLE "%-4d" RESET " | Single : %-7d |\n", total, single);
    printf("  | " SUCCESS_STYLE "Trong      : %-4d" RESET " | Double : %-7d |\n", avail, dbl);
    printf("  | " ERROR_STYLE "Co khach   : %-4d" RESET " | " MONEY_STYLE "VIP    : %-7d" RESET " |\n", occupied, vip);
    printf(HEADER_STYLE   "  +----------------------------------------+\n" RESET);
    printf("  | Ti le lap day: " MONEY_STYLE "%-5.1f%%" RESET "               |\n", rate);

    int bars = (int)(rate / 5);
    printf("  | [");
    for (int i = 0; i < 20; i++) {
        if (i < bars) printf(BG_GREEN " " RESET);
        else           printf(DIM_STYLE "▒" RESET);
    }
    printf("]             |\n");
    printf(HEADER_STYLE   "  +----------------------------------------+\n\n" RESET);
}

void freeRoomList(RoomNode **head) {
    RoomNode *cur = *head;
    while (cur) { RoomNode *t = cur; cur = cur->next; free(t); }
    *head = NULL;
}
