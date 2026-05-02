# USB WiFi Keylogger
> Academic Cybersecurity Project

A hardware USB pass-through keylogger using an Arduino Pro Micro, USB Host Shield (MAX3421E), and ESP32. Captures keystrokes from any USB keyboard, forwards them transparently to the PC, and serves them over WiFi via a browser interface.

---

## ⚠️ Disclaimer

This project was built strictly for academic and educational purposes as part of a cybersecurity capstone project. It must only be used on hardware you personally own in a controlled lab environment. Deploying this on any system without explicit written authorisation is illegal.

---

## How It Works

```
[USB Keyboard]
      │
      ▼
[USB-A Female Port]  ← built into the USB Host Shield Mini board
      │
      ▼
[USB Host Shield / MAX3421E]  ← reads raw HID reports via SPI
      │
      ▼
[Pro Micro / ATmega32U4]  ← decodes keys, forwards to PC as HID keyboard
      │
      ├──► [PC / Target Machine]  ← sees a normal keyboard, nothing suspicious
      │
      └──► [ESP32]  ← receives keystrokes over Serial, serves web dashboard
                │
                ▼
         [Your Phone/Laptop]
         WiFi: KeyloggerAP
         Browser: http://192.168.4.1
```

---

## Pictures

**Arduino Pro Micro (ATmega32U4)**

<img width="495" height="321" alt="image" src="https://github.com/user-attachments/assets/0d3a3a54-2014-419e-add2-67100fb6843f" />

**USB Host Shield Mini (MAX3421E) — Female USB port is built in**

<img width="458" height="311" alt="image" src="https://github.com/user-attachments/assets/60a337a7-e573-4ff3-b07d-f97c81d358f7" />

---

## Hardware Required

| Part | Notes |
|------|-------|
| Arduino Pro Micro (ATmega32U4) 5V/16MHz | Must be 32U4 — NOT Uno/Nano |
| USB Host Shield Mini (MAX3421E) | Already has USB-A Female port built in |
| ESP32 Dev Module | Any ESP32 with WiFi |
| Female-to-Female Dupont Jumper Wires | 10cm length ideal |
| Micro-USB to USB-A cable | Standard phone/Android cable |

> ✅ No separate USB-A Female breakout board needed — the Host Shield Mini already has one built in.

**Estimated cost: ~$14 USD (AliExpress/Temu)**

---

## Wiring

### Connection 1 — Pro Micro → USB Host Shield Mini (SPI)

| Pro Micro Pin | Host Shield Pin | Wire Colour |
|---------------|-----------------|-------------|
| VCC (5V)      | VCC             | 🔴 Red      |
| GND           | GND             | ⚫ Black    |
| Pin 10        | SS              | 🟡 Yellow   |
| Pin 16        | MOSI            | 🟠 Orange   |
| Pin 14        | MISO            | 🟢 Green    |
| Pin 15        | SCK             | 🔵 Blue     |

### Connection 2 — Pro Micro → ESP32 (Serial)

| Pro Micro Pin | ESP32 Pin      | Notes             |
|---------------|----------------|-------------------|
| 3.3V          | 3.3V           | ⚠️ NEVER use 5V  |
| GND           | GND            | Common ground     |
| Pin 8 (TX1)   | GPIO16 (RX2)   | TX → RX (crossed) |
| Pin 9 (RX1)   | GPIO17 (TX2)   | RX → TX (crossed) |

### Where to Plug

| Side   | Connection |
|--------|------------|
| INPUT  | USB Keyboard → USB-A Female port on the Host Shield Mini |
| OUTPUT | Pro Micro Micro-USB → PC USB port (standard Micro-USB cable) |

> ⚠️ ESP32 is 3.3V only. Connecting 5V will permanently damage it.

> ⚠️ TX/RX must be CROSSED: Pro Micro TX → ESP32 RX, and Pro Micro RX → ESP32 TX.

---

## Software Setup

### Step 1 — Install Arduino IDE
Download from: https://www.arduino.cc/en/software

### Step 2 — Add ESP32 Board Support
1. Go to `File > Preferences`
2. Paste into Additional Boards Manager URLs:
```
https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
```
3. Go to `Tools > Board > Boards Manager`
4. Search `ESP32` and install **ESP32 by Espressif Systems**

### Step 3 — Add Pro Micro Board Support
1. Go to `Tools > Board > Boards Manager`
2. Search `SparkFun AVR`
3. Install **SparkFun AVR Boards**

### Step 4 — Install Required Library
1. Go to `Tools > Manage Libraries`
2. Search and install: **USB Host Shield Library 2.0** by Oleg Mazurov

---

## Flashing

### Flash ESP32 First
1. Plug ESP32 into PC via its USB port
2. In Arduino IDE:
   - `Tools > Board > ESP32 Dev Module`
   - `Tools > Port >` select ESP32 COM port
3. Open `src/esp32_wifi_server/esp32_wifi_server.ino`
4. Click **Upload** and wait for `Done uploading`

### Flash Pro Micro Second
1. Plug Pro Micro into PC via Micro-USB cable
2. In Arduino IDE:
   - `Tools > Board > SparkFun Pro Micro 5V 16MHz`
   - `Tools > Port >` select Pro Micro COM port
3. Open `src/pro_micro_keylogger/pro_micro_keylogger.ino`
4. Click **Upload** and wait for `Done uploading`

> 💡 If upload fails on Pro Micro: double-tap the reset button then immediately click Upload.

---

## How to Use

1. Plug USB keyboard into the **USB-A Female port on the Host Shield Mini**
2. Plug **Pro Micro Micro-USB** into the target PC via a standard Micro-USB cable
3. PC detects it as a normal keyboard — keystrokes pass through transparently
4. On your phone or laptop:
   - Connect to WiFi: `KeyloggerAP`
   - Password: `password123`
5. Open browser → go to `http://192.168.4.1`
6. All keystrokes appear on the page, auto-refreshing every 5 seconds
7. Click **Clear Log** to wipe the current log

---

## File Structure

```
usb-keylogger/
├── README.md
├── src/
│   ├── pro_micro_keylogger/
│   │   └── pro_micro_keylogger.ino   ← Upload to Pro Micro
│   └── esp32_wifi_server/
│       └── esp32_wifi_server.ino     ← Upload to ESP32
├── docs/
│   └── WIRING.md                     ← Detailed wiring reference
└── schematics/
    └── wiring_guide.html             ← Visual wiring guide (open in browser)
```

---

## Troubleshooting

| Problem | Likely Cause | Fix |
|---------|-------------|-----|
| PC doesn't detect keyboard | Pro Micro not flashed / SPI wiring wrong | Re-flash, check pins 10/16/14/15 |
| No WiFi network appears | ESP32 not powered correctly | Check 3.3V wiring |
| Keystrokes don't appear on page | TX/RX wires swapped | Swap Pin 8 and Pin 9 connections to ESP32 |
| Upload fails on Pro Micro | Bootloader timing issue | Double-tap reset then click Upload |
| ESP32 gets hot | Connected to 5V instead of 3.3V | Move wire to 3.3V pin immediately |
| USB Host Shield not detected | SS pin wrong | Confirm Pin 10 → SS |

---

## Why NOT These Boards

| Board | Problem |
|-------|---------|
| Arduino Uno/Nano (ATmega328) | Cannot act as USB HID device to PC |
| Raspberry Pi 3/4/5 | No USB OTG — cannot present as USB device |
| Raspberry Pi Pico (RP2040) | No native USB host capability |
| ESP32 alone | No USB host or USB device |

---

## License

For academic use only. Not for deployment on systems you do not own.
