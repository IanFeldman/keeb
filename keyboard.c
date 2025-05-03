/*
             LUFA Library
     Copyright (C) Dean Camera, 2021.

  dean [at] fourwalledcubicle [dot] com
           www.lufa-lib.org
*/

/*
  Copyright 2021  Dean Camera (dean [at] fourwalledcubicle [dot] com)

  Permission to use, copy, modify, distribute, and sell this
  software and its documentation for any purpose is hereby granted
  without fee, provided that the above copyright notice appear in
  all copies and that both that the copyright notice and this
  permission notice and warranty disclaimer appear in supporting
  documentation, and that the name of the author not be used in
  advertising or publicity pertaining to distribution of the
  software without specific, written prior permission.

  The author disclaims all warranties with regard to this
  software, including all implied warranties of merchantability
  and fitness.  In no event shall the author be liable for any
  special, indirect or consequential damages or any damages
  whatsoever resulting from loss of use, data or profits, whether
  in an action of contract, negligence or other tortious action,
  arising out of or in connection with the use or performance of
  this software.
*/

/*
 * Edited by Ian Feldman
 * April 2025
 */

#include <stdint.h>
#include "device.h"
#include "keyboard.h"
#include "nkrohid.h"
#include "uart.h"

/* report buffer to store incoming keys from peripheral */
volatile USB_NKRO_Report_Data_t report_buffer_g = {0};
/* keep track if this is main or peripheral */
volatile int main_unit_g = 0;
/* peripheral log if layer key pressed by main */
volatile int layer_key_press = 0;

/** Buffer to hold the previously generated Keyboard HID report, for comparison purposes inside the HID class driver. */
static uint8_t PrevKeyboardHIDReportBuffer[sizeof(USB_NKRO_Report_Data_t)];

/** LUFA HID Class driver interface configuration and state information. This structure is
 *  passed to all HID Class driver functions, so that multiple instances of the same class
 *  within a device can be differentiated from one another.
 */
USB_ClassInfo_HID_Device_t Keyboard_HID_Interface =
{
    .Config =
    {
        .InterfaceNumber = INTERFACE_ID_Keyboard,
        .ReportINEndpoint =
        {
            .Address = KEYBOARD_EPADDR,
            .Size = KEYBOARD_EPSIZE,
            .Banks = 1,
        },
        .PrevReportINBuffer = PrevKeyboardHIDReportBuffer,
        .PrevReportINBufferSize = sizeof(PrevKeyboardHIDReportBuffer),
    },
};


/** Main program entry point. This routine contains the overall program flow, including initial
 *  setup of all components and the main program loop.
 */
int main(void)
{
    SetupHardware();

    GlobalInterruptEnable();

    for (;;)
    {
        PeripheralTask();
        HID_Device_USBTask(&Keyboard_HID_Interface);
        USB_USBTask();
    }
}

/** Configures the board hardware and chip peripherals for the demo's functionality. */
void SetupHardware()
{
    /* Disable watchdog if enabled by bootloader/fuses */
    MCUSR &= ~(1 << WDRF);
    wdt_disable();

    /* Disable clock division */
    clock_prescale_set(clock_div_1);

    /* Hardware initialization */
    device_init();
    uart_init();
    USB_Init();
}

/** Peripheral main loop */
void PeripheralTask()
{
    /* skip if not peripheral */
    if (main_unit_g) return;

    /* create usb report */
    USB_NKRO_Report_Data_t KeyboardReport = {0};

    /* incorporate layer key */
    KeyboardReport.Keys[SC_TO_IDX(HID_KEYBOARD_SC_LAYER)] |=
        SC_TO_MSK(HID_KEYBOARD_SC_LAYER) * layer_key_press;

    /* poll keys */
    uint8_t input = device_poll(&KeyboardReport);

    /* send keypresses */
    if (input)
    {
        uart_send_report(KeyboardReport);
    }

    layer_key_press = 0;
}

/** Event handler for the library USB Connection event. */
void EVENT_USB_Device_Connect(void)
{
    main_unit_g = 1;
    device_blink(2);
}

/** Event handler for the library USB Disconnection event. */
void EVENT_USB_Device_Disconnect(void)
{
    main_unit_g = 0;
    device_blink(3);
}

/** Event handler for the library USB Configuration Changed event. */
void EVENT_USB_Device_ConfigurationChanged(void)
{
    bool ConfigSuccess = true;

    ConfigSuccess &= HID_Device_ConfigureEndpoints(&Keyboard_HID_Interface);

    USB_Device_EnableSOFEvents();
}

/** Event handler for the library USB Control Request reception event. */
void EVENT_USB_Device_ControlRequest(void)
{
    HID_Device_ProcessControlRequest(&Keyboard_HID_Interface);
}

/** Event handler for the USB device Start Of Frame event. */
void EVENT_USB_Device_StartOfFrame(void)
{
    HID_Device_MillisecondElapsed(&Keyboard_HID_Interface);
}

/** HID class driver callback function for the creation of HID reports to the host.
 *
 *  \param[in]     HIDInterfaceInfo  Pointer to the HID class interface configuration structure being referenced
 *  \param[in,out] ReportID    Report ID requested by the host if non-zero, otherwise callback should set to the generated report ID
 *  \param[in]     ReportType  Type of the report to create, either HID_REPORT_ITEM_In or HID_REPORT_ITEM_Feature
 *  \param[out]    ReportData  Pointer to a buffer where the created report should be stored
 *  \param[out]    ReportSize  Number of bytes written in the report (or zero if no report is to be sent)
 *
 *  \return Boolean \c true to force the sending of the report, \c false to let the library determine if it needs to be sent
 */
bool CALLBACK_HID_Device_CreateHIDReport(USB_ClassInfo_HID_Device_t* const HIDInterfaceInfo,
    uint8_t* const ReportID,
    const uint8_t ReportType,
    void* ReportData,
    uint16_t* const ReportSize)
{
    /* skip if peripheral */
    if (!main_unit_g) return false;

    /* create usb report */
    USB_NKRO_Report_Data_t* KeyboardReport = (USB_NKRO_Report_Data_t*)ReportData;

    /* poll keys */
    /* get buffered keys first because it might contain layer info */
    KeyboardReport->Modifier |= report_buffer_g.Modifier;
    for (int i = 0; i < KEY_BUFFER_SIZE; i++)
    {
        KeyboardReport->Keys[i] |= report_buffer_g.Keys[i];
    }

    /* clear buffer */
    report_buffer_g.Modifier = 0x00;
    memset((uint8_t*)report_buffer_g.Keys, 0x00, KEY_BUFFER_SIZE);

    /* poll main device */
    device_poll(KeyboardReport);

    /* send layer key */
    uart_send_layer_info(*KeyboardReport);

    *ReportSize = sizeof(USB_NKRO_Report_Data_t);
    return false;
}

/** HID class driver callback function for the processing of HID reports from the host.
 *
 *  \param[in] HIDInterfaceInfo  Pointer to the HID class interface configuration structure being referenced
 *  \param[in] ReportID    Report ID of the received report from the host
 *  \param[in] ReportType  The type of report that the host has sent, either HID_REPORT_ITEM_Out or HID_REPORT_ITEM_Feature
 *  \param[in] ReportData  Pointer to a buffer where the received report has been stored
 *  \param[in] ReportSize  Size in bytes of the received HID report
 */
void CALLBACK_HID_Device_ProcessHIDReport(USB_ClassInfo_HID_Device_t* const HIDInterfaceInfo,
    const uint8_t ReportID,
    const uint8_t ReportType,
    const void* ReportData,
    const uint16_t ReportSize)
{
    /* skip if peripheral */
    if (!main_unit_g) return;
}

/* Main unit receive key state and save to global report buffer
 * Peripheral receive single byte indicating layer key press */
ISR(USART1_RX_vect)
{
    static interrupt_st state = AWAIT_REPORT;
    static int key_idx = 0;

    /* get data */
    uint8_t data = UDR1;

    /* if peripheral, any receive indicates layer key */
    if (!main_unit_g)
    {
        layer_key_press = 1;
        return;
    }

    /* if main */
    switch(state)
    {
        case AWAIT_REPORT:
            if (data == FRAME_START)
            {
                state = READ_MODIFIER;
            }
            break;
        case READ_MODIFIER:
            report_buffer_g.Modifier |= data;
            state = READ_KEYS;
            break;
        case READ_KEYS:
            report_buffer_g.Keys[key_idx++] |= data;
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
