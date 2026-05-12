#ifndef BILLING_H
#define BILLING_H

#include "model.h"

void     createInvoice(Invoice *inv, Room room, int stayDuration, float discount);
void     finalizeInvoice(Invoice *inv, Room room, const char *actualCheckOutDate, Customer guest);
void     printInvoice(Invoice invoice, Customer guest);
void     writeInvoiceToFile(Invoice invoice, Customer guest, const char *filename);
void     printRevenueReport(Invoice invoices[], int count);
Invoice* findOpenInvoice(Invoice invoices[], int count, int roomNumber);

#endif
