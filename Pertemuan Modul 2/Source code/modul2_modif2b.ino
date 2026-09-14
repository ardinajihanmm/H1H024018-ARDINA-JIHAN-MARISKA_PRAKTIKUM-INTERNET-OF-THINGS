#include <ESP8266WiFi.h>

const char* sta_ssid = "Kelompok1ShiftB";
const char* sta_password = "admin321";

const char* ap_ssid = "ESP8266_AccessPoint";
const char* ap_password = "12345678";

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_AP_STA);

  WiFi.softAP(ap_ssid, ap_password);
  Serial.print("AP IP Address   : ");
  Serial.println(WiFi.softAPIP());

  WiFi.begin(sta_ssid, sta_password);
  Serial.print("Menghubungkan ke WiFi rumah");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("STA IP Address  : ");
  Serial.println(WiFi.localIP());
}

void loop() {
  int jumlahClient = WiFi.softAPgetStationNum();
  Serial.print("Client pada AP: ");
  Serial.print(jumlahClient);
  Serial.print(" | Status STA: ");
  Serial.println(WiFi.status() == WL_CONNECTED ? "Terhubung" : "Terputus");
  delay(5000);
}
