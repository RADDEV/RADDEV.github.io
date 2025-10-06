// esp32_firebase.ino
// Simple ESP32 sketch that reads /buttonState from the project's
// Firebase Realtime Database and drives a GPIO pin HIGH when the
// DB value is 1 and LOW when it's 0.
//
// Instructions:
// - Replace YOUR_SSID and YOUR_PASSWORD with your Wi‑Fi credentials.
// - (Optional) Change CONTROL_PIN if you want another GPIO.
// - Compile & upload with Arduino IDE or PlatformIO (board: ESP32).

#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>

// --- CONFIG (edit these) ---------------------------------------------------
const char* ssid = "*********";          // <-- replace with your WiFi SSID
const char* password = "********";  // <-- replace with your WiFi password
// Full DB URL for the buttonState node (from the web app)
const char* databaseUrl = "https://onebutton-3a41a-default-rtdb.europe-west1.firebasedatabase.app/buttonState.json";

// Pin that will be driven according to DB value (1 -> HIGH, 0 -> LOW)
const int CONTROL_PIN = 14; // LED/control pin set to GPIO14
// Set to true if a HIGH output should turn the LED/device ON.
// Many ESP32 dev boards have the onboard LED wired active-low; set to false in that case.
const bool ACTIVE_HIGH = true;

// Poll interval in milliseconds
const unsigned long POLL_INTERVAL_MS = 250;

// ---------------------------------------------------------------------------

int lastState = -1; // unknown
unsigned long lastPoll = 0;

void setup() {
  Serial.begin(115200);
  delay(100);
  pinMode(CONTROL_PIN, OUTPUT);
  // Ensure the pin starts in the OFF state (respect ACTIVE_HIGH polarity)
  if (ACTIVE_HIGH) digitalWrite(CONTROL_PIN, LOW); else digitalWrite(CONTROL_PIN, HIGH);

  Serial.printf("Connecting to WiFi '%s'...\n", ssid);
  WiFi.begin(ssid, password);

  unsigned long start = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - start < 20000) {
    Serial.print('.');
    delay(500);
  }
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println();
    Serial.print("Connected, IP: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println();
    Serial.println("WiFi connection failed (will keep trying in loop)");
  }
}

void loop() {
  // rate-limit polling
  if (millis() - lastPoll < POLL_INTERVAL_MS) return;
  lastPoll = millis();

  if (WiFi.status() != WL_CONNECTED) {
    // try reconnecting if needed
    WiFi.reconnect();
    return;
  }

  WiFiClientSecure *client = new WiFiClientSecure();
  // For simplicity accept any cert. For production replace with fingerprint/CA
  client->setInsecure();

  HTTPClient http;
  if (!http.begin(*client, databaseUrl)) {
    Serial.println("HTTP begin failed");
    delete client;
    return;
  }

  int httpCode = http.GET();
  if (httpCode == HTTP_CODE_OK) {
    String payload = http.getString();
    payload.trim();

    // payload from Realtime DB is typically the raw value, e.g. 0 or 1
    int newValue = -1;
    if (payload == "1" || payload == "\"1\"") newValue = 1;
    else if (payload == "0" || payload == "\"0\"") newValue = 0;
    else {
      // try parsing numeric value (safe fallback)
      newValue = payload.toInt();
      // toInt() returns 0 for non-numeric strings, so detect that case
      if (newValue == 0 && payload != "0" && payload != "\"0\"") {
        newValue = -1; // parse failed
      }
    }

    if (newValue != -1) {
      if (newValue != lastState) {
        // Map logical on/off to physical pin level according to ACTIVE_HIGH
        int pinLevel = (newValue ? (ACTIVE_HIGH ? HIGH : LOW) : (ACTIVE_HIGH ? LOW : HIGH));
        digitalWrite(CONTROL_PIN, pinLevel);
        lastState = newValue;
        Serial.printf("buttonState=%d -> pin %d set to %s (ACTIVE_HIGH=%s)\n",
                      newValue, CONTROL_PIN, newValue ? "ON" : "OFF", ACTIVE_HIGH ? "true" : "false");
      }
    } else {
      Serial.printf("Unexpected payload: '%s'\n", payload.c_str());
    }
  } else {
    Serial.printf("HTTP GET failed, code: %d\n", httpCode);
  }

  http.end();
  delete client;
}
