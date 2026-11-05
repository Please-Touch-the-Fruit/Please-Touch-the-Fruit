#pragma once
#include <cstdint>

struct ImuPacket{
    uint8_t nodeId;
    uint32_t timestamp;
    float accel[3];
    float gyro[3];
};