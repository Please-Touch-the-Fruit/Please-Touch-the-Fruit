#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>
#include <esp_arduino_version.h>
#include "packet.h"

#if ESP_ARDUINO_VERSION_MAJOR >= 3
void onRecv(const esp_now_recv_info_t *info, const uint8_t *data, int len) {
#else
void onRecv(const uint8_t *mac, const uint8_t *data, int len) {
#endif
  if (len != sizeof(ImuPacket)) return;   // ignore anything that isn't ours
  ImuPacket p;
  memcpy(&p, data, sizeof(p));
  Serial.printf("%u,%lu,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f\n",
                p.nodeId, (unsigned long)p.timestamp,
                p.accel[0], p.accel[1], p.accel[2],
                p.gyro[0], p.gyro[1], p.gyro[2]);
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  delay(1000);
  Serial.print("Receiver MAC: ");
  Serial.println(WiFi.macAddress());

  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW init failed");
    return;
  }
  esp_now_register_recv_cb(onRecv);
}

void loop() {}