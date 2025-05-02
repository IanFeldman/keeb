#include <avr/io.h>
#include <LUFA/Drivers/USB/USB.h>
#include <util/delay.h>
#include "device.h"
#include "nkrohid.h"

/* Initialize device hardware */
void device_init(void)
{
    /* insert delay to avoid pull up contention  */
    _delay_ms(500);

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

/* Poll for keypresses, save to keys (array of KEY_BUFFER_SIZE).
 * Return a value if keys are pressed, 0 otherwise. */
uint8_t device_poll(USB_NKRO_Report_Data_t *report)
{
    /* read ports */
    uint8_t port_b = PINB;
    uint8_t port_c = PINC;
    uint8_t port_d = PIND;
    uint8_t input = port_b & (port_c | ~PORTC_INPUT) & (port_d | ~PORTD_INPUT);

    #ifdef LEFT
    /* first check if layer key pressed */
    report->Keys[SC_TO_IDX(HID_KEYBOARD_SC_LAYER)] |= SC_TO_MSK(HID_KEYBOARD_SC_LAYER) * !(port_c & (1 << 4));
    int layer = report->Keys[SC_TO_IDX(HID_KEYBOARD_SC_LAYER)] & SC_TO_MSK(HID_KEYBOARD_SC_LAYER);
    /* seperate read required beacuse layer key can be set by uart isr */
    if (!layer)
    {
        /* port b */
        report->Keys[SC_TO_IDX(HID_KEYBOARD_SC_X)] |= SC_TO_MSK(HID_KEYBOARD_SC_X) * !(port_b & (1 << 0));
        report->Keys[SC_TO_IDX(HID_KEYBOARD_SC_W)] |= SC_TO_MSK(HID_KEYBOARD_SC_W) * !(port_b & (1 << 1));
        report->Keys[SC_TO_IDX(HID_KEYBOARD_SC_Q)] |= SC_TO_MSK(HID_KEYBOARD_SC_Q) * !(port_b & (1 << 2));
        // report->Keys[SC_TO_IDX(HID_KEYBOARD_SC__)] |= SC_TO_MSK(HID_KEYBOARD_SC__) * !(port_b & (1 << 3)); /* ESC */
        report->Keys[SC_TO_IDX(HID_KEYBOARD_SC_A)] |= SC_TO_MSK(HID_KEYBOARD_SC_A) * !(port_b & (1 << 4));
        // report->Keys[SC_TO_IDX(HID_KEYBOARD_SC__)] |= SC_TO_MSK(HID_KEYBOARD_SC__) * !(port_b & (1 << 5)); /* TAB */
        report->Keys[SC_TO_IDX(HID_KEYBOARD_SC_Z)] |= SC_TO_MSK(HID_KEYBOARD_SC_Z) * !(port_b & (1 << 6));
        // report->Keys[SC_TO_IDX(HID_KEYBOARD_SC__)] |= SC_TO_MSK(HID_KEYBOARD_SC__) * !(port_b & (1 << 7)); /* SHIFT */
        /* port c */
        report->Keys[SC_TO_IDX(HID_KEYBOARD_SC_V)] |= SC_TO_MSK(HID_KEYBOARD_SC_V) * !(port_c & (1 << 2));
        report->Keys[SC_TO_IDX(HID_KEYBOARD_SC_SPACE)] |= SC_TO_MSK(HID_KEYBOARD_SC_SPACE) * !(port_c & (1 << 5));
        /* port d */
        report->Keys[SC_TO_IDX(HID_KEYBOARD_SC_F)] |= SC_TO_MSK(HID_KEYBOARD_SC_F) * !(port_d & (1 << 0));
        report->Keys[SC_TO_IDX(HID_KEYBOARD_SC_R)] |= SC_TO_MSK(HID_KEYBOARD_SC_R) * !(port_d & (1 << 1));
        report->Keys[SC_TO_IDX(HID_KEYBOARD_SC_C)] |= SC_TO_MSK(HID_KEYBOARD_SC_C) * !(port_d & (1 << 4));
        report->Keys[SC_TO_IDX(HID_KEYBOARD_SC_D)] |= SC_TO_MSK(HID_KEYBOARD_SC_D) * !(port_d & (1 << 5));
        report->Keys[SC_TO_IDX(HID_KEYBOARD_SC_E)] |= SC_TO_MSK(HID_KEYBOARD_SC_E) * !(port_d & (1 << 6));
        report->Keys[SC_TO_IDX(HID_KEYBOARD_SC_S)] |= SC_TO_MSK(HID_KEYBOARD_SC_S) * !(port_d & (1 << 7));
    }
    else
    {
    }

    #elif RIGHT
    /* first check if layer key pressed */
    report->Keys[SC_TO_IDX(HID_KEYBOARD_SC_LAYER)] |= SC_TO_MSK(HID_KEYBOARD_SC_LAYER) * !(port_c & (1 << 5));
    int layer = report->Keys[SC_TO_IDX(HID_KEYBOARD_SC_LAYER)] & SC_TO_MSK(HID_KEYBOARD_SC_LAYER);
    /* seperate read required beacuse layer key can be set by uart isr */
    if (!layer)
    {
        /* port b */
        report->Keys[SC_TO_IDX(HID_KEYBOARD_SC_L)] |= SC_TO_MSK(HID_KEYBOARD_SC_L) * !(port_b & (1 << 0));
        report->Keys[SC_TO_IDX(HID_KEYBOARD_SC_O)] |= SC_TO_MSK(HID_KEYBOARD_SC_O) * !(port_b & (1 << 1));
        report->Keys[SC_TO_IDX(HID_KEYBOARD_SC_I)] |= SC_TO_MSK(HID_KEYBOARD_SC_I) * !(port_b & (1 << 2));
        report->Keys[SC_TO_IDX(HID_KEYBOARD_SC_K)] |= SC_TO_MSK(HID_KEYBOARD_SC_K) * !(port_b & (1 << 3));
        // report->Keys[SC_TO_IDX(HID_KEYBOARD_SC__)] |= SC_TO_MSK(HID_KEYBOARD_SC__) * !(port_b & (1 << 4)); /* COMMA */
        report->Keys[SC_TO_IDX(HID_KEYBOARD_SC_U)] |= SC_TO_MSK(HID_KEYBOARD_SC_U) * !(port_b & (1 << 5));
        report->Keys[SC_TO_IDX(HID_KEYBOARD_SC_J)] |= SC_TO_MSK(HID_KEYBOARD_SC_J) * !(port_b & (1 << 6));
        report->Keys[SC_TO_IDX(HID_KEYBOARD_SC_M)] |= SC_TO_MSK(HID_KEYBOARD_SC_M) * !(port_b & (1 << 7));
        /* port c */
        // report->Keys[SC_TO_IDX(HID_KEYBOARD_SC__)] |= SC_TO_MSK(HID_KEYBOARD_SC__) * !(port_c & (1 << 2)); /* FORWARD SLASH */
        report->Keys[SC_TO_IDX(HID_KEYBOARD_SC_SPACE)] |= SC_TO_MSK(HID_KEYBOARD_SC_SPACE) * !(port_c & (1 << 4));
        /* port d */
        // report->Keys[SC_TO_IDX(HID_KEYBOARD_SC__)] |= SC_TO_MSK(HID_KEYBOARD_SC__) * !(port_d & (1 << 0)); /* SHIFT */
        // report->Keys[SC_TO_IDX(HID_KEYBOARD_SC__)] |= SC_TO_MSK(HID_KEYBOARD_SC__) * !(port_d & (1 << 1)); /* APOSTROPHE */
        // report->Keys[SC_TO_IDX(HID_KEYBOARD_SC__)] |= SC_TO_MSK(HID_KEYBOARD_SC__) * !(port_d & (1 << 4)); /* TBD */
        // report->Keys[SC_TO_IDX(HID_KEYBOARD_SC__)] |= SC_TO_MSK(HID_KEYBOARD_SC__) * !(port_d & (1 << 5)); /* SEMICOLON */
        report->Keys[SC_TO_IDX(HID_KEYBOARD_SC_P)] |= SC_TO_MSK(HID_KEYBOARD_SC_P) * !(port_d & (1 << 6));
        // report->Keys[SC_TO_IDX(HID_KEYBOARD_SC__)] |= SC_TO_MSK(HID_KEYBOARD_SC__) * !(port_d & (1 << 7)); /* PERIOD */
    }
    else
    {
    }

    #else
    #error LEFT or RIGHT not defined
    #endif

    return ~input;
}

