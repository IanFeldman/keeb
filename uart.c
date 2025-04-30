#include <avr/io.h>
#include <avr/interrupt.h>
#include "uart.h"

volatile uint8_t key_buffer_g[BUFFER_SIZE];
volatile int key_buffer_idx_g;

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

    /* initialize key buffer index */
    key_buffer_idx_g = 0;
}


/* Send a key press to main unit */
void uart_send_key(uint8_t key)
{
    /* wait for empty transmit buffer */
    while (!(UCSR1A & (1 << UDRE1)));
    /* put data in buffer */
    UDR1 = key;
}


/* Getter for key buffer and size */
uint8_t *get_buffer(int *size)
{
    *size = key_buffer_idx_g;
    key_buffer_idx_g = 0;
    return key_buffer_g;
}


/* Receive a key press */
ISR(USART1_RX_vect)
{
    /* check for data rx */
    key_buffer_g[key_buffer_idx_g++] = UDR1;

    /*
    if (UCSR1A & (1 << RXC1))
    {
        key_buffer_g[key_buffer_idx_g++] = UDR1;
    }
    */
}

