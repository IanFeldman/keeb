#ifndef DEVICE_H
#define DEVICE_H

#include "nkrohid.h"

#define DEBUG_LED PC7

/* input pins of portc and portd */
#define PORTC_INPUT ((1 << 2) | (1 << 4) | (1 << 5))
#define PORTD_INPUT ((1 << 0) | (1 << 1) | (1 << 4) | (1 << 5) | (1 << 6) | (1 << 7))

/* layer key position in port c */
#ifdef LEFT
    #define LAYER_POS (1 << 4)
#elif RIGHT
    #define LAYER_POS (1 << 5)
#else
    #error LEFT or RIGHT not defined
#endif

void device_init(void);
void device_blink(int count);
uint8_t device_poll(USB_NKRO_Report_Data_t *report, uint8_t extern_layer_key);

#endif /* DEVICE_H */

