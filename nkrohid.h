#ifndef NKROHID_H
#define NKROHID_H

#include <LUFA/Common/Common.h>

/* 13 bytes for 104 total keys */
#define KEY_BUFFER_SIZE 13

/* Report descriptor */
#define HID_DESCRIPTOR_NKRO_KEYBOARD \
    0x05, 0x01,                 /* Usage Page (Generic Desktop)         */ \
    0x09, 0x06,                 /* Usage (Keyboard)                     */ \
    0xA1, 0x01,                 /* Collection (Application)             */ \
        0x75, 0x01,             /* Report Size (1)                      */ \
        0x95, 0x08,             /* Report Count (8)                     */ \
        0x05, 0x07,             /* Usage Page (Keyboard)                */ \
        0x19, 0xE0,             /* Usage Minimum (Modifier keys)        */ \
        0x29, 0xE7,             /* Usage Maximum                        */ \
        0x15, 0x00,             /* Logical Minimum (0)                  */ \
        0x25, 0x01,             /* Logical Maximum (1)                  */ \
        0x81, 0x02,             /* Input (Data, Variable, Absolute)     */ \
        0x75, 0x01,             /* Report Size (1)                      */ \
        0x95, 0x68,             /* Report Count (104 bits → 13 bytes)   */ \
        0x05, 0x07,             /* Usage Page (Keyboard)                */ \
        0x19, 0x00,             /* Usage Minimum (Key 0)                */ \
        0x29, 0x67,             /* Usage Maximum (Key 103)              */ \
        0x15, 0x00,             /* Logical Minimum (0)                  */ \
        0x25, 0x01,             /* Logical Maximum (1)                  */ \
        0x81, 0x02,             /* Input (Data, Variable, Absolute)     */ \
    0xC0                        /* End Collection                       */ \

/* Report */
typedef struct
{
    uint8_t Modifier;
    uint8_t Keys[KEY_BUFFER_SIZE];
} ATTR_PACKED USB_NKRO_Report_Data_t;

/* Get key buffer byte index from scancode */
#define SC_TO_IDX(scancode) (scancode >> 3)
/* Get key buffer byte mask from scancode */
#define SC_TO_MSK(scancode) (0x01 << (scancode - (SC_TO_IDX(scancode) << 3)))

#define HID_KEYBOARD_SC_LAYER 0x00

#endif /* NKROHID_H */

