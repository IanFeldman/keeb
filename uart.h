#ifndef UART_H
#define UART_H

#define FRAME_START 0xAA

typedef enum
{
    AWAIT_REPORT,
    READ_MODIFIER,
    READ_KEYS,
    READ_LAYER
} interrupt_st;

void uart_init(void);
void uart_send_report(USB_NKRO_Report_Data_t *report);
void uart_send_layer_info(USB_NKRO_Report_Data_t *report);

#endif /* UART_H */

