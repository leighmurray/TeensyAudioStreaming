#include "NetworkManager.h"
#include "AudioManager.h"
#include "DisplayManager.h"

NetworkManager networkManager;
AudioManager audioManager;
DisplayManager displayManager;

void handleRemoteAudio();
void handleLocalAudio();

void setup() {
  Serial.begin(115200);
  displayManager.Setup();
  displayManager.println("Setting up Network...");
  networkManager.Setup(true);
  displayManager.println("Done!");
  displayManager.println("Setting up Audio...");
  audioManager.Setup();
  displayManager.println("Done!");
  displayManager.println("RX:");
  displayManager.println("TX:");
}

void loop() {
  handleLocalAudio();
  handleRemoteAudio();
}

void handleLocalAudio(){
  byte inputAudioBufferLeft[256];
  byte inputAudioBufferRight[256];
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
  byte outputAudioBufferLeft[256];
  byte outputAudioBufferRight[256];
  if (networkManager.receiveAudioBuffers(outputAudioBufferLeft, outputAudioBufferRight)){
    audioManager.setOutputAudioBuffers(outputAudioBufferLeft, outputAudioBufferRight);
    displayManager.ShowDataReceived();
  } else {
    displayManager.ShowNoDataReceived();
  }
}
