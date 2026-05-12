#ifndef MEMBERSHIP_H
#define MEMBERSHIP_H

#include "model.h"

float       calculateDiscount(Customer guest, float price, DiscountSystem config, char *code);
void        displayDiscountTable(void);
void        handleMembership(RoomNode *head, Invoice invoices[], int invoiceCount);
int         validatePromoCode(const char *code, const char *validCode);
const char* getMembershipLevelName(int level);

#endif
