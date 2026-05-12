#ifndef FILE_H
#define FILE_H

#include "model.h"

void saveRoomsToFile(RoomNode *head, const char *filename);
void loadRoomsFromFile(RoomNode **head, const char *filename);
void saveInvoicesToFile(Invoice invoices[], int count, const char *filename);
void loadInvoicesFromFile(Invoice invoices[], int *count, const char *filename);

#endif
