


#include <cstdint>

#pragma pack(push, 1)

struct SensorData {
    uint32_t sensorId;
    uint16_t type;   //2bytes htons/ntohs
    uint32_t value;  //4bytes htonl/ntohl
};

#pragma pack(pop)