#include <avr/io.h>
#include <avr/interrupt.h>
#include "nkrohid.h"
#include "uart.h"

volatile USB_NKRO_Report_Data_t report_g;

/* Initialize uart peripheral */
void uart_init(void)
{
    /* ubrr = clock / (16 * baud) - 1 */
    uint16_t ubrr = 103;
    /* Set baud rate 9600 */
    UBRR1H = (uint8_t)(ubrr >> 8);
    UBRR1L = (uint8_t)(ubrr);
    /* enable transmitter, receiver, receiver interrupts */
    UCSR1B = (1 << TXEN1 | 1 << RXEN1 | 1 << RXCIE1);
    /* no parity, one stop bits, 8-bit data */
    UCSR1C = (3 << UCSZ10);
    /* enable global interrupts */
    SREG |= (1 << SREG_I);
}


/* Send a key press report to main unit */
void uart_send_report(USB_NKRO_Report_Data_t report)
{
    /* send modifier */
    while (!(UCSR1A & (1 << UDRE1)));
    UDR1 = report_g.Modifier;

    /* send keys */
    for (int i = 0; i < KEY_BUFFER_SIZE; i++)
    {
        /* wait for empty transmit buffer */
        while (!(UCSR1A & (1 << UDRE1)));
        /* put data in buffer */
        UDR1 = report_g.Keys[i];
    }
}


/* Incorporate report buffer into live report */
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


/* Receive key state and save to global report buffer */
ISR(USART1_RX_vect)
{
    /* if data is available */
    while (!(UCSR1A & (1 << RXC1)));
    report_g.Modifier |= UDR1;

    for (int i = 0; i < KEY_BUFFER_SIZE; i++)
    {
        while (!(UCSR1A & (1 << RXC1)));
        report_g.Keys[i] |= UDR1;
    }
}

