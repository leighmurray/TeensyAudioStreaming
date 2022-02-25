
#ifndef NETWORK_MANAGER_H
#define NETWORK_MANAGER_H

#include <QNEthernet.h>
#include <QNDNSClient.h>
#include "StorageManager.h"
#include <TimeLib.h>

using namespace qindesign::network;

constexpr uint32_t kDHCPTimeout = 10000;  // 10 seconds
constexpr uint16_t kAudioPort = 4464;
constexpr char kServiceName[] = "teensy-audio-streaming"; //tas

EthernetUDP udp;
uint8_t buf[Ethernet.mtu() - 20 - 8];  // Maximum UDP payload size
                                       // 20-byte IP, 8-byte UDP header

uint8_t macAddressUSBHeader[6] = {0x04, 0xe9, 0xe5, 0x0c, 0xec, 0x21};
uint8_t macAddressNoUSBHeader[6] = {0x04, 0xe9, 0xe5, 0x11, 0x22, 0x7c};

uint8_t* serverMacAddress = macAddressNoUSBHeader;
uint8_t* clientMacAddress = macAddressUSBHeader;

struct JacktripDefaultHeader{
  uint64_t TimeStamp;
  uint16_t SeqNumber;
  uint16_t BufferSize;
  uint8_t SamplingRate;
  uint8_t BitResolution;
  uint8_t NumIncomingChannelsFromNet;
  uint8_t NumOutgoingChannelsFromNet;
};

class NetworkManager{
public:
  void Setup(bool useJacktripHeader = false){
    this->useJacktripHeader = useJacktripHeader;
    // Print the MAC address

    Ethernet.macAddress(mac);
    Serial.printf("MAC = %02x:%02x:%02x:%02x:%02x:%02x\n", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    
    // Initialize Ethernet, in this case with DHCP
    Serial.println("Starting Ethernet with DHCP...");
    if (!Ethernet.begin()) {
      Serial.println("Failed to start Ethernet");
      return;
    }
    if (!Ethernet.waitForLocalIP(kDHCPTimeout)) {
      Serial.println("Failed to get IP address from DHCP.");
      SetupStaticIP();
    }

    PrintIPData();
    
    // Listen on port
    udp.begin(kAudioPort);


    // start an mDNS service
    Serial.println("Starting mDNS...");
    if (!MDNS.begin(kServiceName)) {
      Serial.println("ERROR: Starting mDNS.");
    } else {
      if (!MDNS.addService("_tas", "_udp", kAudioPort)) {
        Serial.println("ERROR: Adding service.");
      } else {
        Serial.printf("Started mDNS service:\n"
                      "    Name: %s\n"
                      "    Type: _tas._udp\n"
                      "    Port: %u\n",
                      kServiceName, kAudioPort);
      }
    }
    Serial.println("Waiting for TAS messages...");

    bool initialised = false;

    // TODO: tidy this up please
    while (!initialised) {
      IPAddress savedIP(StorageManager::getRemoteIPAddress());
      Serial.printf("Please give me the remote IP address or press enter to attempt %u.%u.%u.%u:\n", savedIP[0], savedIP[1], savedIP[2], savedIP[3]);
      uint8_t count = 10;
      Serial.print("Auto-reconnect in: ");
      while(!Serial.available() && count > 0){
        if (udp.parsePacket()){
          remoteNodeIP = udp.remoteIP();
          Serial.print("Nevermind, we have incoming data from: ");
          Serial.println(remoteNodeIP);
          StorageManager::saveRemoteIPAddress(uint32_t(remoteNodeIP));
          return;
        }
        Serial.printf("%u, ", count);
        delay(1000);
        count--;
      }
      String ipString = Serial.readStringUntil('\n');
      if (!ipString.length()){
        Serial.print("Setting remote to: ");
        remoteNodeIP = savedIP;
        Serial.println(remoteNodeIP);
        initialised = true;
        // returning so that we dont "waste" a write to the EEPROM because it's limited.
        return;
      } else {
        Serial.println(ipString);
        initialised = remoteNodeIP.fromString(ipString);
      }
      
      if (!initialised){
        Serial.println("Invalid IP address.");
      } else {
        Serial.print("Thankyou. Remote IP is:");
        Serial.println(remoteNodeIP);
        StorageManager::saveRemoteIPAddress(remoteNodeIP);
      }
    }
  }

  bool sendAudioBuffers(byte audioBufferLeft[256], byte audioBufferRight[256]){
    byte audioPacket[512];
    memcpy(audioPacket, audioBufferLeft, 256);
    memcpy(&audioPacket[256], audioBufferRight, 256);

    if (useJacktripHeader == true){
      jacktripDefaultHeader.TimeStamp = (uint64_t(now()) * 1000000) + (micros()); // this is microseconds since the program started so INCORRECT
      byte audioPacketWithHeader[528];
      // copy the header to the first 16 bytes
      memcpy(&audioPacketWithHeader[0], &jacktripDefaultHeader, sizeof(JacktripDefaultHeader));
      // copy the remaining audio data to the rest of the byte array
      memcpy(&audioPacketWithHeader[16], audioPacket, 512);
      // increase the sequence for the next header
      jacktripDefaultHeader.SeqNumber++;
      return udp.send(remoteNodeIP,  kAudioPort, audioPacketWithHeader, 528);
    }
    else {
      return udp.send(remoteNodeIP,  kAudioPort, audioPacket, 512);
    }
  }

  bool receiveAudioBuffers(byte outputAudioBufferLeft[256], byte outputAudioBufferRight[256]){
    uint16_t size = udp.parsePacket();
    if (0 < size && size <= sizeof(buf)) {
      udp.read(buf, size);
      int audioDataOffset = useJacktripHeader ? 16 : 0;
      memcpy(outputAudioBufferLeft, &buf[audioDataOffset], 256);
      memcpy(outputAudioBufferRight, &buf[256+audioDataOffset], 256);
      return true;
    }
    return false;
  }


private:
  bool isStaticIP = false;
  IPAddress remoteNodeIP;
  uint8_t mac[6];
  bool useJacktripHeader = false;
  JacktripDefaultHeader jacktripDefaultHeader = {
    0,    //uint64_t TimeStamp;
    0,    //uint16_t SeqNumber;
    128,  //uint16_t BufferSize;
    2,    //uint8_t SamplingRate;
    16,   //uint8_t BitResolution;
    2,    //uint8_t NumIncomingChannelsFromNet;
    0     //uint8_t NumOutgoingChannelsFromNet;
  };
  
  bool isServer(){
    for (int i=0; i<6; i++){
      if (serverMacAddress[i] != mac[i]){
        return false;
      }
    }
    return true;
  }

  void PrintIPData(){
    IPAddress ip = Ethernet.localIP();
    Serial.printf("    Local IP     = %u.%u.%u.%u\n", ip[0], ip[1], ip[2], ip[3]);
    ip = Ethernet.subnetMask();
    Serial.printf("    Subnet mask  = %u.%u.%u.%u\n", ip[0], ip[1], ip[2], ip[3]);
    ip = Ethernet.broadcastIP();
    Serial.printf("    Broadcast IP = %u.%u.%u.%u\n", ip[0], ip[1], ip[2], ip[3]);
    ip = Ethernet.gatewayIP();
    Serial.printf("    Gateway      = %u.%u.%u.%u\n", ip[0], ip[1], ip[2], ip[3]);
    ip = Ethernet.dnsServerIP();
    Serial.printf("    DNS          = %u.%u.%u.%u\n", ip[0], ip[1], ip[2], ip[3]);
  }
  
  void SetupStaticIP(){
    Serial.println("Setting up static IP");
    IPAddress subnetMask{255, 255, 255, 0};
    IPAddress serverIP{192, 168, 1, 1};
    IPAddress clientIP{192, 168, 1, 2};
    
    if (isServer()){
      Serial.println("We're the server");
      Ethernet.begin(serverIP, subnetMask, clientIP);
    } else {
      Serial.println("We're the client");
      Ethernet.begin(clientIP, subnetMask, serverIP);
    }
    isStaticIP = true;
  }
};

#endif
