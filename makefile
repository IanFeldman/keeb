#
#             LUFA Library
#     Copyright (C) Dean Camera, 2021.
#
#  dean [at] fourwalledcubicle [dot] com
#           www.lufa-lib.org
#
# --------------------------------------
#         LUFA Project Makefile.
# --------------------------------------
#
# Edited by Ian Feldman
# April 2025

MCU          = atmega32u2
ARCH         = AVR8
BOARD        = NONE
F_CPU        = 16000000
F_USB        = $(F_CPU)
OPTIMIZATION = s
TARGET       = keyboard
SRC          = $(TARGET).c descriptors.c $(LUFA_SRC_USB) $(LUFA_SRC_USBCLASS)
LUFA_PATH    = third-party/LUFA
CC_FLAGS     = -DUSE_LUFA_CONFIG_HEADER -Iconfig/
LD_FLAGS     =

# Default target
all:

# Include LUFA-specific DMBS extension modules
DMBS_LUFA_PATH ?= $(LUFA_PATH)/Build/LUFA
include $(DMBS_LUFA_PATH)/lufa-sources.mk
include $(DMBS_LUFA_PATH)/lufa-gcc.mk

# Include common DMBS build system modules
DMBS_PATH      ?= $(LUFA_PATH)/Build/DMBS/DMBS
include $(DMBS_PATH)/gcc.mk

# Flash keyboard
flash: $(TARGET).hex
	avrdude -c stk500v1 -P /dev/ttyACM0 -p m32u2 -b 19200 -U flash:w:$(TARGET).hex

# Program fuses
fuses:
	avrdude -c stk500v1 -p m32u2 -P /dev/ttyACM0 -b 19200 -U lfuse:w:0xFF:m -U hfuse:w:0xD7:m

