#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "file.h"
#include "room.h"
#include "ui.h"

// Lưu dữ liệu phòng vào file 
void saveRoomsToFile(RoomNode *head, const char *filename) {
    FILE *f = fopen(filename, "wb");
    if (!f) { printf(ERROR_STYLE "  ✗ Khong the ghi file phong!\n" RESET); return; }
    int count = 0;
    while (head) { fwrite(&head->data, sizeof(Room), 1, f); count++; head = head->next; }
    fclose(f);
    printf(SUCCESS_STYLE "  ✓ Da luu %d phong vao '%s'.\n" RESET, count, filename);
}

// Tải dữ liệu phòng từ file
void loadRoomsFromFile(RoomNode **head, const char *filename) {
    FILE *f = fopen(filename, "rb");
    if (!f) { printf(INFO_STYLE "  ℹ File '%s' chua ton tai, bat dau moi.\n" RESET, filename); return; }
    freeRoomList(head);
    Room room;
    int count = 0;
    while (fread(&room, sizeof(Room), 1, f) == 1) {
        RoomNode *node = createRoomNode(room);
        if (!node) { printf(ERROR_STYLE "  ✗ Het bo nho!\n" RESET); break; }
        if (!*head) *head = node;
        else {
            RoomNode *cur = *head;
            while (cur->next) cur = cur->next;
            cur->next = node;
        }
        count++;
    }
    fclose(f);
    printf(SUCCESS_STYLE "  ✓ Da tai %d phong tu '%s'.\n" RESET, count, filename);
}

// Lưu dữ liệu hóa đơn vào file 
void saveInvoicesToFile(Invoice invoices[], int count, const char *filename) {
    FILE *f = fopen(filename, "wb");
    if (!f) { printf(ERROR_STYLE "  ✗ Khong the ghi file hoa don!\n" RESET); return; }
    fwrite(&count, sizeof(int), 1, f);
    for (int i = 0; i < count; i++) fwrite(&invoices[i], sizeof(Invoice), 1, f);
    fclose(f);
    printf(SUCCESS_STYLE "  ✓ Da luu %d hoa don vao '%s'.\n" RESET, count, filename);
}

// Tải dữ liệu hóa đơn từ file
void loadInvoicesFromFile(Invoice invoices[], int *count, const char *filename) {
    FILE *f = fopen(filename, "rb");
    if (!f) { printf(INFO_STYLE "  ℹ File hoa don chua ton tai.\n" RESET); *count = 0; return; }
    int saved;
    if (fread(&saved, sizeof(int), 1, f) != 1) { fclose(f); *count = 0; return; }
    *count = 0;
    while (*count < saved && *count < MAX_INVOICES) {
        if (fread(&invoices[*count], sizeof(Invoice), 1, f) != 1) break;
        (*count)++;
    }
    fclose(f);
    printf(SUCCESS_STYLE "  ✓ Da tai %d hoa don tu '%s'.\n" RESET, *count, filename);
}
