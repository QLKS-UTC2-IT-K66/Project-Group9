#ifndef BOOKING_H
#define BOOKING_H

#include "model.h"

void checkIn(RoomNode *head, Invoice invoices[], int *invoiceCount);
void checkOut(RoomNode *head, Invoice invoices[], int invoiceCount);
void displayBookedRooms(RoomNode *head);
void getDateInput(char *date, const char *prompt);

#endif
