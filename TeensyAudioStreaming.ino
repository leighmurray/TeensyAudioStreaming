#include "NetworkManager.h"
#include "AudioManager.h"
#include "DisplayManager.h"

NetworkManager networkManager;
AudioManager audioManager;
DisplayManager displayManager;

void handleRemoteAudio();
void handleLocalAudio();

#define LOOPBACK true

void setup() {
  //Serial.begin(115200);
  displayManager.Setup();
  displayManager.print("Setup Network...\n");
  networkManager.Setup(true);
  displayManager.print("Done.\n");
  displayManager.print("Setup Audio...\n");
  audioManager.Setup();
  displayManager.print("Done.\n");
  displayManager.clearScreen();
  String localIPString = networkManager.getLocalIPString();
  displayManager.print("Local IP:\n");
  displayManager.print(localIPString);
  String remoteIPString = networkManager.getRemoteIPString();
  displayManager.print("\nRemote IP:\n");
  displayManager.print(remoteIPString);
  displayManager.print("\nTX:\n");
  displayManager.print("RX:\n");

  if (LOOPBACK == false){
    displayManager.print("\nLoopback Disabled\n");
    audioManager.disableLoopback();
  } else {
    displayManager.print("\nLoopback Enabled\n");
    audioManager.enableLoopback();
  }
}

void loop() {
  handleLocalAudio();
  handleRemoteAudio();
}

void handleLocalAudio(){
  uint8_t inputAudioBufferLeft[256];
  uint8_t inputAudioBufferRight[256];
  bool hasLocalAudioBuffers = audioManager.getInputAudioBuffers(inputAudioBufferLeft, inputAudioBufferRight);
 
  // if there is an audio buffer send it to the other device
  if (hasLocalAudioBuffers){
    networkManager.sendAudioBuffers(inputAudioBufferLeft, inputAudioBufferRight);
    displayManager.ShowDataSent();
  } else {
    displayManager.ShowNoDataSent();
  }
}

void handleRemoteAudio(){
  uint8_t outputAudioBufferLeft[256];
  uint8_t outputAudioBufferRight[256];
  if (networkManager.receiveAudioBuffers(outputAudioBufferLeft, outputAudioBufferRight)){
    audioManager.setOutputAudioBuffers(outputAudioBufferLeft, outputAudioBufferRight);
    displayManager.ShowDataReceived();
  } else {
    displayManager.ShowNoDataReceived();
  }
}
