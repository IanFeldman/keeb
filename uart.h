#ifndef UART_H
#define UART_H

void uart_init(void);
void uart_send_report(USB_NKRO_Report_Data_t report);
void uart_get_report(USB_NKRO_Report_Data_t *report);

#endif /* UART_H */

