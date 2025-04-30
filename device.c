#include <avr/io.h>
#include <LUFA/Drivers/USB/USB.h>
#include <util/delay.h>
#include "device.h"
#include "nkrohid.h"

/* Initialize device hardware */
void device_init(void)
{
    /* configure ports as inputs */
    DDRB = 0x00;
    DDRC = 0x00;
    DDRD = 0x00;

    /* enable pull up resistors */
    PORTB = 0xFF;
    PORTC = 0xFF;
    PORTD = 0xFF;

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

/* Poll for keypresses, save to keys (array of KEY_BUFFER_SIZE) */
void device_poll(uint8_t *keys)
{
    uint8_t idx = 0;
    /* read ports */
    uint8_t port_b = PINB;
    uint8_t port_c = PINC;
    uint8_t port_d = PIND;

    /* check modifiers first */
    #ifdef LEFT
    /* port b */
    if (!(port_b & (1 << 0))) keys[SC_TO_IDX(HID_KEYBOARD_SC_X)] |= SC_TO_MSK(HID_KEYBOARD_SC_X);
    if (!(port_b & (1 << 1))) keys[SC_TO_IDX(HID_KEYBOARD_SC_W)] |= SC_TO_MSK(HID_KEYBOARD_SC_W);
    if (!(port_b & (1 << 2))) keys[SC_TO_IDX(HID_KEYBOARD_SC_Q)] |= SC_TO_MSK(HID_KEYBOARD_SC_Q);
    /* port c */
    /* port d */
    #elif RIGHT
    #else
    #error LEFT or RIGHT not defined
    #endif
}

