#ifndef CUSTOMER_H
#define CUSTOMER_H

#include "model.h"

void  inputCustomer(Customer *customer);
int   validateCustomer(Customer *customer);
void  displayCustomer(Customer customer);
int   validatePhoneNumber(char *phone);
int   validateIdCard(char *idCard);
const char* getMembershipLabel(int level);

#endif
