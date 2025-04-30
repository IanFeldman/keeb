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
SRC          = $(TARGET).c descriptors.c device.c uart.c $(LUFA_SRC_USB) $(LUFA_SRC_USBCLASS)
LUFA_PATH    = third-party/LUFA
CC_FLAGS     = -DUSE_LUFA_CONFIG_HEADER -Iconfig/

# Targets for left and right halves
left: CC_FLAGS += -DLEFT
left: all

right: CC_FLAGS += -DRIGHT
right: all

# Main target
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
	avrdude -c avrisp -P /dev/ttyACM0 -p m32u2 -b 19200 -U flash:w:$(TARGET).hex -F

# Program fuses
fuses:
	avrdude -c avrisp -p m32u2 -P /dev/ttyACM0 -b 19200 -U lfuse:w:0xDE:m -U hfuse:w:0xD9:m -F

