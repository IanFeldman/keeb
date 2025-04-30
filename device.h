#ifndef DEVICE_H
#define DEVICE_H

#include "nkrohid.h"

#define DEBUG_LED PC7

void device_init(void);
void device_blink(int count);
uint8_t device_poll(USB_NKRO_Report_Data_t *report);

#endif /* DEVICE_H */

