# USB WiFi Keylogger
> Academic Cybersecurity Project
A hardware USB pass-through keylogger using an Arduino Pro Micro, USB Host Shield (MAX3421E), and ESP32. Captures keystrokes from any USB keyboard, sends them to PC and serves them over WiFi via a browser interface.

---

## ⚠️ Disclaimer

This project was built strictly for academic and educational purposes as part of a cybersecurity capstone project. It must only be used on hardware you personally own in a controlled lab environment. Deploying this on any system without explicit written authorisation is illegal.

---

## How It Works

```
[USB Keyboard]
      │
      ▼
[USB-A Female Breakout]
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

## Hardware Required

| Part | Notes | Status |
|------|-------|--------|
| Arduino Pro Micro (ATmega32U4) 5V/16MHz | Must be 32U4 — NOT Uno/Nano | Required |
| USB Host Shield Mini (MAX3421E) | Mini version fits best | Required |
| ESP32 Dev Module | Any ESP32 with WiFi | Required |
| USB-A Female Breakout Board (4-pin DIP) | Keyboard plugs in here | Required |
| Female-to-Female Dupont Jumper Wires | 10cm length ideal | Required |
| Micro-USB to USB-A cable | Standard phone cable | Required |

**Estimated cost: ~$14 USD (AliExpress/Temu)**

---

## Wiring

### Pro Micro → USB Host Shield Mini (SPI)

| Pro Micro | Host Shield | Colour |
|-----------|-------------|--------|
| VCC (5V)  | VCC         | Red    |
| GND       | GND         | Black  |
| Pin 10    | SS          | Yellow |
| Pin 16    | MOSI        | Orange |
| Pin 14    | MISO        | Green  |
| Pin 15    | SCK         | Blue   |

### Pro Micro → ESP32 (Serial)

| Pro Micro     | ESP32          | Notes             |
|---------------|----------------|-------------------|
| 3.3V          | 3.3V           | NEVER use 5V      |
| GND           | GND            | Common ground     |
| Pin 8 (TX1)   | GPIO16 (RX2)   | TX → RX (crossed) |
| Pin 9 (RX1)   | GPIO17 (TX2)   | RX → TX (crossed) |

### USB-A Female Breakout → USB Host Shield

| USB-A Female  | Host Shield |
|---------------|-------------|
| Pin 1 — VBUS  | VBUS        |
| Pin 2 — D−    | D−          |
| Pin 3 — D+    | D+          |
| Pin 4 — GND   | GND         |

### Where to Plug

| Side    | Connection |
|---------|------------|
| INPUT   | USB Keyboard → USB-A Female breakout board |
| OUTPUT  | Pro Micro Micro-USB port → PC USB port (via standard Micro-USB cable) |

> ⚠️ ESP32 is 3.3V only. Never connect 5V — it will permanently damage the chip.

> ⚠️ TX/RX must be CROSSED: Pro Micro TX → ESP32 RX, Pro Micro RX → ESP32 TX.

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

### Step 4 — Install Libraries
1. Go to `Tools > Manage Libraries`
2. Search and install: **USB Host Shield Library 2.0** by Oleg Mazurov

---

## Flashing

### Flash ESP32 First
1. Plug ESP32 into PC via its USB port
2. In Arduino IDE:
   - Board: `ESP32 Dev Module`
   - Port: Select the ESP32 COM port
3. Open `src/esp32_wifi_server/esp32_wifi_server.ino`
4. Click **Upload**
5. Wait for `Done uploading`

### Flash Pro Micro Second
1. Plug Pro Micro into PC via Micro-USB cable
2. In Arduino IDE:
   - Board: `SparkFun Pro Micro 5V 16MHz`
   - Port: Select the Pro Micro COM port
3. Open `src/pro_micro_keylogger/pro_micro_keylogger.ino`
4. Click **Upload**
5. If upload fails: double-tap the reset button on Pro Micro then immediately click Upload

---

## How to Use

1. Plug USB keyboard into the **USB-A Female breakout board**
2. Plug **Pro Micro Micro-USB** into the target PC using a standard Micro-USB cable
3. PC detects it as a normal keyboard — all keystrokes pass through transparently
4. On your phone or laptop:
   - Connect to WiFi: `KeyloggerAP`
   - Password: `password123`
5. Open browser and go to: `http://192.168.4.1`
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

| Problem | Cause | Fix |
|---------|-------|-----|
| PC doesn't detect keyboard | Pro Micro not flashed / SPI wrong | Re-flash, check pins 10/16/14/15 |
| No WiFi network appears | ESP32 not powered | Check 3.3V wiring |
| Keystrokes don't appear on page | TX/RX swapped | Swap Pin8 and Pin9 connections |
| Upload fails on Pro Micro | Bootloader timing | Double-tap reset then upload |
| ESP32 gets hot | Connected to 5V | Move to 3.3V pin immediately |
| USB Host Shield not detected | SS pin wrong | Confirm Pin10 → SS |

---

## Why NOT These Boards

| Board | Problem |
|-------|---------|
| Arduino Uno/Nano (ATmega328) | Cannot act as USB HID device |
| Raspberry Pi 3/4/5 | No USB OTG — cannot be USB device to PC |
| Raspberry Pi Pico (RP2040) | No native USB host |
| ESP32 alone | No USB host or device |

---

## License

For academic use only. Not for deployment on systems you do not own.
