#ifndef UART_H
#define UART_H

#define BUFFER_SIZE 128

void uart_init(void);
void uart_send_key(uint8_t key);
uint8_t *uart_get_buffer(int *size);

#endif /* UART_H */

