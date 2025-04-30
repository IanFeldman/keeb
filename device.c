#include <avr/io.h>
#include <util/delay.h>
#include "device.h"

/* Initialize device hardware */
void device_init(void)
{
    /* debug led */
    DDRC |= (1 << DEBUG_LED);
    PORTC &= ~(1 << DEBUG_LED);
}


/* Blink debug led */
void device_blink(int count)
{
    for (int i = 0; i < count; i++)
    {
        PORTC ^= (1 << DEBUG_LED);
        _delay_ms(50);
        PORTC ^= (1 << DEBUG_LED);
        _delay_ms(50);
    }
}

