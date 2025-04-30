#ifndef DEVICE_H
#define DEVICE_H

#define DEBUG_LED PC7

void device_init(void);
void device_blink(int count);
void device_poll(uint8_t *keys);

#endif /* DEVICE_H */

