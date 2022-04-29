#include "NetworkManager.h"
#include "AudioManager.h"
//#include "DisplayManager.h"

// ****** Current sample rate in AudioStream.h is 48k and buffer size is 16
// ****** With sample rate at 8, the following error happens:
// ****** C:\Program Files (x86)\Arduino\hardware\teensy\avr\libraries\Audio\effect_freeverb.cpp:122:3: error: too many initializers for 'int16_t [8] {aka short int [8]}'

NetworkManager networkManager;
AudioManager audioManager;
//DisplayManager displayManager;

void handleRemoteAudio();
void handleLocalAudio();

#define LOOPBACK false
#define USE_JACKTRIP_HEADERS false

uint8_t inputAudioBufferLeft[AUDIO_BLOCK_SAMPLES * 2]; // 16 BIT DEPTH
uint8_t inputAudioBufferRight[AUDIO_BLOCK_SAMPLES * 2];
uint8_t outputAudioBufferLeft[AUDIO_BLOCK_SAMPLES * 2];
uint8_t outputAudioBufferRight[AUDIO_BLOCK_SAMPLES * 2];

const uint16_t maxOutputBufferCount = 16;
elapsedMicros microsSinceLastBuffer;

const uint16_t bufferDurationMicroSeconds = (1000000 * AUDIO_BLOCK_SAMPLES) / AUDIO_SAMPLE_RATE_EXACT *2; // *2 might be wrong

void setup() {
  delay(1000);
  Serial.print("AUDIO_SAMPLE_RATE_EXACT: ");
  Serial.println(AUDIO_SAMPLE_RATE_EXACT);
  Serial.print("AUDIO_BLOCK_SAMPLES: ");
  Serial.println(AUDIO_BLOCK_SAMPLES);
//  displayManager.Setup();
//  displayManager.print("Setup Network...\n");
  networkManager.Setup(USE_JACKTRIP_HEADERS);
//  displayManager.print("Done.\n");
//  displayManager.print("Setup Audio...\n");
  audioManager.Setup(maxOutputBufferCount);
//  displayManager.print("Done.\n");
//  displayManager.clearScreen();
  String localIPString = networkManager.getLocalIPString();
//  displayManager.print("Local IP:\n");
//  displayManager.print(localIPString);
  String remoteIPString = networkManager.getRemoteIPString();
//  displayManager.print("\nRemote IP:\n");
//  displayManager.print(remoteIPString);
  //displayManager.print("\nTX:\n");
  //displayManager.print("RX:\n");

  if (LOOPBACK == false){
//    displayManager.print("\nLoopback: No\n");
    audioManager.disableLoopback();
  } else {
//    displayManager.print("\nLoopback: Yes\n");
    audioManager.enableLoopback();
  }
}

void loop() {
  handleLocalAudio();
  handleRemoteAudio();
}

void handleLocalAudio(){
  // send all the local audio buffers we have. not sure how the ethernet library handles multiple sends at a time
  while (audioManager.getInputAudioBuffers(inputAudioBufferLeft, inputAudioBufferRight)){
    networkManager.sendAudioBuffers(inputAudioBufferLeft, inputAudioBufferRight);
    //displayManager.ShowDataSent();
  }
}

void handleRemoteAudio(){
  if (networkManager.receiveAudioBuffers(outputAudioBufferLeft, outputAudioBufferRight)){
    audioManager.setOutputAudioBuffers(outputAudioBufferLeft, outputAudioBufferRight);
    if (microsSinceLastBuffer > bufferDurationMicroSeconds) {
      Serial.println("Late buffer! Took " + String(microsSinceLastBuffer) + " microseconds. Limit is " + String(bufferDurationMicroSeconds));
      Serial.println("Audio Memory Usage: " + String(AudioMemoryUsage()));
    }
    microsSinceLastBuffer = 0;
    //displayManager.ShowDataReceived();
  } else {
    //displayManager.ShowNoDataReceived();
  }
}
