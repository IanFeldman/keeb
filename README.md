# A minimal ergo keyboard
Low profile, split, ortholinear, 34-key layout staggered to my natural finger rest positions. Designed for Gateron KS-33 Red swiches and DSA keycaps.
See write-up at https://ianfeldman.dev/projects/keyboard/.
<br><br>

<img width="1860" alt="Keyboard" src="https://github.com/user-attachments/assets/0d0769d0-1ac2-4d26-b064-92777ad31b67"/>

## Features
- N-key rollover
- Efficient matrixless polling
- Lightweight and portable
- Serial communication between splits
- USB-C host connection

## Hardware Build Instructions
- Order and assemble PCB's
  - Reflow recommended for all surface mount components except SPI headers, TRRS jack, and crystal oscillator.
  - On hardware version 1.1, there are duplicate C2 and C3 traces. Only solder one C2 and one C3.
- Configure programming setup using In-system programmer (ensure shared ground)
- Power on PCB through USB-C connector
<br><br> _Note: only one PCB can be flashed at a time_

## Software Build and Flash Instructions (Linux)
```sh
# Install dependencies
sudo apt install avr-gcc

# Clone this repo
git clone https://github.com/IanFeldman/keeb.git
cd keeb

# Compile left or right half
make [left] [right]

# Update makefile targets 'fuses' and 'flash' as needed for your ISP device
# Set fuses with ISP (first time only)
make fuses

# Flash keyboard with ISP
make flash
```
