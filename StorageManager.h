#ifndef STORAGE_MANAGER_H
#define STORAGE_MANAGER_H

#include <EEPROM.h>

class StorageManager{
public:
  static bool saveRemoteIPAddress(uint32_t ip){
    uint8_t *ipSegment = (uint8_t *)(&ip);
    for (int i=0; i<4; i++){
      EEPROM.write(i, ipSegment[i]);
    }
    return true;
  }

  static uint32_t getRemoteIPAddress(){
    uint32_t ip;
    uint8_t *ipSegment = (uint8_t *)(&ip);
    for (int i=0; i<4; i++){
      ipSegment[i] = EEPROM.read(i);
    }
    return ip;
  }
};

#endif
