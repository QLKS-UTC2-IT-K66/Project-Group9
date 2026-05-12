#ifndef SERVICE_H
#define SERVICE_H

#include "model.h"

extern Service serviceMenu[MAX_MENU];
extern int     menuSize;

void  showServiceMenu(void);
void  addServiceToInvoice(Invoice *inv);
void  showUsedServices(Invoice inv);
float getTotalServicePrice(Invoice inv);
void  manageServiceMenu(void);

#endif
