#include <ESP8266WiFi.h>

const char* ssid = "Kelompok1ShiftB";
const char* password = "admin321";
const int ledPin = 4;

unsigned long lastCheck = 0;
const unsigned long interval = 5000; // cek tiap 5 detik

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("Menghubungkan ke WiFi...");
}

void loop() {
  unsigned long now = millis();
  if (now - lastCheck >= interval) {
    lastCheck = now;
    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("Status: Terhubung");
      digitalWrite(ledPin, HIGH);
    } else {
      Serial.println("Status: Terputus, mencoba reconnect...");
      digitalWrite(ledPin, LOW);
      WiFi.disconnect();
      WiFi.begin(ssid, password);
    }
  }
}
