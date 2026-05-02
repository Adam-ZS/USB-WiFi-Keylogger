# Wiring Reference

## Connection 1 — Pro Micro → USB Host Shield Mini (SPI)

| Pro Micro Pin | USB Host Shield Pin | Wire Colour |
|---------------|--------------------:|-------------|
| VCC (5V)      | VCC                 | Red         |
| GND           | GND                 | Black       |
| Pin 10        | SS                  | Yellow      |
| Pin 16        | MOSI                | Orange      |
| Pin 14        | MISO                | Green       |
| Pin 15        | SCK                 | Blue        |

## Connection 2 — Pro Micro → ESP32 (Serial)

| Pro Micro Pin | ESP32 Pin       | Notes              |
|---------------|----------------:|--------------------|
| 3.3V          | 3.3V            | NEVER use 5V       |
| GND           | GND             | Common ground      |
| Pin 8 (TX1)   | GPIO16 (RX2)    | TX → RX (crossed)  |
| Pin 9 (RX1)   | GPIO17 (TX2)    | RX → TX (crossed)  |

## Connection 3 — USB-A Female Breakout → USB Host Shield

| USB-A Female Pin | USB Host Shield Pin |
|------------------|--------------------:|
| Pin 1 — VBUS     | VBUS                |
| Pin 2 — D−       | D−                  |
| Pin 3 — D+       | D+                  |
| Pin 4 — GND      | GND                 |

## Where to Plug

| Side     | What                          |
|----------|-------------------------------|
| INPUT    | USB Keyboard → USB-A Female breakout board |
| OUTPUT   | Pro Micro Micro-USB → PC USB port |

## Warnings

- ESP32 is 3.3V only — never connect 5V or it will die
- TX and RX must be CROSSED between Pro Micro and ESP32
- The only soldering needed is USB-A Female breakout → USB Host Shield (4 wires)
