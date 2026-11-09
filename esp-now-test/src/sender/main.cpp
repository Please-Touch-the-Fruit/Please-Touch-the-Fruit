#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>
#include <M5Unified.h>
#include "packet.h"

uint8_t broadcastAddr[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
float counter = 0;

void setup() {
  Serial.begin(115200);
  M5.begin();
  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW init failed");
    return;
  }

  esp_now_peer_info_t peer = {};
  memcpy(peer.peer_addr, broadcastAddr, 6);
  peer.channel = 0;      // use whatever channel WiFi is currently on
  peer.encrypt = false;
  esp_now_add_peer(&peer);
}

void loop() {
  ImuPacket p = {};
  p.nodeId = NODE_ID;
  p.timestamp = millis();
  M5.update();
  if (M5.Imu.isEnabled()) {
    M5.Imu.getAccel(&p.accel[0], &p.accel[1], &p.accel[2]);
    M5.Imu.getGyro(&p.gyro[0], &p.gyro[1], &p.gyro[2]);
  } else {
    if(M5.BtnA.wasPressed()) counter++;
    p.accel[0] = counter;
    p.accel[1] = M5.BtnA.isPressed() ? 1.0f : 0.0f;
  }

  esp_err_t result = esp_now_send(broadcastAddr, (uint8_t *)&p, sizeof(p));
  if (result != ESP_OK) Serial.printf("send error: %d\n", result);

  delay(20);
}