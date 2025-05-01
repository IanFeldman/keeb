#include <avr/io.h>
#include <avr/interrupt.h>
#include "nkrohid.h"
#include "uart.h"

/* Only used by main unit */
volatile USB_NKRO_Report_Data_t report_g;

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

    /* intialize report to zeros */
    report_g.Modifier = 0x00;
    memset((uint8_t *)report_g.Keys, 0x00, KEY_BUFFER_SIZE);
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


/* Main unit incorporate report buffer into current report */
void uart_get_report(USB_NKRO_Report_Data_t *report)
{
    report->Modifier |= report_g.Modifier;
    for (int i = 0; i < KEY_BUFFER_SIZE; i++)
    {
        report->Keys[i] |= report_g.Keys[i];
    }

    /* clear report buffer */
    report_g.Modifier = 0x00;
    memset((uint8_t *)report_g.Keys, 0x00, KEY_BUFFER_SIZE);
}


/* Main unit receive key state and save to global report buffer */
ISR(USART1_RX_vect)
{
    static interrupt_st state = AWAIT_REPORT;
    static int key_idx = 0;

    /* get data */
    uint8_t data = UDR1;

    switch(state)
    {
        case AWAIT_REPORT:
            if (data == FRAME_START)
            {
                state = READ_MODIFIER;
            }
            break;
        case READ_MODIFIER:
            report_g.Modifier |= data;
            state = READ_KEYS;
            break;
        case READ_KEYS:
            report_g.Keys[key_idx++] |= data;
            if (key_idx >= KEY_BUFFER_SIZE)
            {
                state = AWAIT_REPORT;
                key_idx = 0;
            }
            break;
        default:
            break;
    }
}

