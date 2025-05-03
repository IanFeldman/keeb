#include <avr/io.h>
#include <avr/interrupt.h>
#include "nkrohid.h"
#include "uart.h"

/* Initialize uart peripheral */
void uart_init(void)
{
    /* ubrr = clock / (16 * baud) - 1 */
    uint16_t ubrr = 8;
    /* set baud rate 115200 */
    UBRR1H = (uint8_t)(ubrr >> 8);
    UBRR1L = (uint8_t)(ubrr);
    /* enable double speed */
    UCSR1A = (1 << U2X1);
    /* enable transmitter, receiver, receiver interrupts */
    UCSR1B = (1 << TXEN1 | 1 << RXEN1 | 1 << RXCIE1);
    /* no parity, two stop bits, 8-bit data */
    UCSR1C = (1 << USBS1) | (3 << UCSZ10);
    /* enable global interrupts */
    SREG |= (1 << SREG_I);
}


/* Send a key press report from peripheral to main unit */
void uart_send_report(USB_NKRO_Report_Data_t report)
{
    /* send frame start */
    while (!(UCSR1A & (1 << UDRE1)));
    UDR1 = FRAME_START;

    /* send modifier */
    while (!(UCSR1A & (1 << UDRE1)));
    UDR1 = report.Modifier;

    /* send keys */
    for (int i = 0; i < KEY_BUFFER_SIZE; i++)
    {
        /* wait for empty transmit buffer */
        while (!(UCSR1A & (1 << UDRE1)));
        /* put data in buffer */
        UDR1 = report.Keys[i];
    }
}


/* Main unit send layer key press */
void uart_send_layer_info(USB_NKRO_Report_Data_t report)
{
    /* only send if layer state has changed */
    static int last_layer_state = 0;

    int curr_layer_state =
        !!(report.Keys[SC_TO_IDX(HID_KEYBOARD_SC_LAYER)]
        & SC_TO_MSK(HID_KEYBOARD_SC_LAYER));

    if (curr_layer_state != last_layer_state)
    {
        while (!(UCSR1A & (1 << UDRE1)));
        UDR1 = curr_layer_state;
    }

    last_layer_state = curr_layer_state;
}

