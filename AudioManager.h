#ifndef AUDIO_MANAGER_H
#define AUDIO_MANAGER_H

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
AudioInputI2S            inputDeviceI2S; //xy=352.0952377319336,372.5714702606201
//AudioInputUSB            inputDeviceUSB; //xy=356.5238265991211,285.85722398757935
AudioPlayQueue           outputBufferLeft; //xy=628.6666564941406,68
AudioPlayQueue           outputBufferRight; //xy=633.6666564941406,105
AudioMixer4              inputMixerLeft; //xy=649.380989074707,488.14288330078125
AudioMixer4              inputMixerRight; //xy=652.380989074707,571.1428833007812
AudioMixer4              loopbackMixerLeft; //xy=656.6666564941406,267
AudioMixer4              loopbackMixerRight; //xy=659.6666564941406,361
AudioRecordQueue         inputBufferLeft; //xy=831.380989074707,487.14288330078125
AudioRecordQueue         inputBufferRight; //xy=835.380989074707,571.1428833007812
AudioMixer4              monitorMixerI2SLeft; //xy=993.6666564941406,283
//AudioMixer4              monitorMixerUSBLeft; //xy=994.6666564941406,97
//AudioMixer4              monitorMixerUSBRight; //xy=997.6666564941406,182
AudioMixer4              monitorMixerI2SRight; //xy=998.6666564941406,369
//AudioOutputUSB           outputDeviceUSB; //xy=1229.6666564941406,140
AudioOutputI2S           outputDeviceI2S; //xy=1256.6666564941406,324
AudioConnection          patchCord1(inputDeviceI2S, 0, inputMixerLeft, 1);
AudioConnection          patchCord2(inputDeviceI2S, 0, loopbackMixerLeft, 1);
AudioConnection          patchCord3(inputDeviceI2S, 1, inputMixerRight, 1);
AudioConnection          patchCord4(inputDeviceI2S, 1, loopbackMixerRight, 1);
//AudioConnection          patchCord5(inputDeviceUSB, 0, inputMixerLeft, 0);
//AudioConnection          patchCord6(inputDeviceUSB, 0, loopbackMixerLeft, 0);
//AudioConnection          patchCord7(inputDeviceUSB, 1, inputMixerRight, 0);
//AudioConnection          patchCord8(inputDeviceUSB, 1, loopbackMixerRight, 0);
//AudioConnection          patchCord9(outputBufferLeft, 0, monitorMixerUSBLeft, 0);
AudioConnection          patchCord10(outputBufferLeft, 0, monitorMixerI2SLeft, 0);
//AudioConnection          patchCord11(outputBufferRight, 0, monitorMixerUSBRight, 0);
AudioConnection          patchCord12(outputBufferRight, 0, monitorMixerI2SRight, 0);
AudioConnection          patchCord13(inputMixerLeft, inputBufferLeft);
AudioConnection          patchCord14(inputMixerRight, inputBufferRight);
//AudioConnection          patchCord15(loopbackMixerLeft, 0, monitorMixerUSBLeft, 1);
AudioConnection          patchCord16(loopbackMixerLeft, 0, monitorMixerI2SLeft, 1);
AudioConnection          patchCord17(loopbackMixerRight, 0, monitorMixerI2SRight, 1);
//AudioConnection          patchCord18(loopbackMixerRight, 0, monitorMixerUSBRight, 1);
AudioConnection          patchCord19(monitorMixerI2SLeft, 0, outputDeviceI2S, 0);
//AudioConnection          patchCord20(monitorMixerUSBLeft, 0, outputDeviceUSB, 0);
//AudioConnection          patchCord21(monitorMixerUSBRight, 0, outputDeviceUSB, 1);
AudioConnection          patchCord22(monitorMixerI2SRight, 0, outputDeviceI2S, 1);
AudioControlSGTL5000     sgtl5000_1;     //xy=839.5238571166992,657.5715084075928
// GUItool: end automatically generated code


// on monitor mixers, channel 0 is output, 1 loopback
class AudioManager{

public:
  void Setup(){
    AudioMemory(1000);
    sgtl5000_1.enable();

    sgtl5000_1.inputSelect(AUDIO_INPUT_LINEIN);
    
    // max without clipping from headphone output is 0.7
    sgtl5000_1.volume(0.7);
    
    startInputBuffer();
    
  }
  
  bool getInputAudioBuffers(uint8_t localAudioBufferLeft[AUDIO_BLOCK_SAMPLES * 2], uint8_t localAudioBufferRight[AUDIO_BLOCK_SAMPLES * 2]){
    if (inputBufferLeft.available() >= 1 && inputBufferRight.available() >= 1) {

      // Fetch 1 blocks from the audio library
      memcpy(localAudioBufferLeft, inputBufferLeft.readBuffer(), AUDIO_BLOCK_SAMPLES * 2);
      inputBufferLeft.freeBuffer();
      
      memcpy(localAudioBufferRight, inputBufferRight.readBuffer(), AUDIO_BLOCK_SAMPLES * 2);
      inputBufferRight.freeBuffer();

      return true;
    } else {
      return false;
    }
  }

  bool setOutputAudioBuffers(uint8_t outputAudioBufferLeft[AUDIO_BLOCK_SAMPLES * 2], uint8_t outputAudioBufferRight[AUDIO_BLOCK_SAMPLES * 2]){
    //  look at calling buffer.play(buffer, size) if there is performance gained
    memcpy(outputBufferLeft.getBuffer(), outputAudioBufferLeft, AUDIO_BLOCK_SAMPLES * 2);
    memcpy(outputBufferRight.getBuffer(), outputAudioBufferRight, AUDIO_BLOCK_SAMPLES * 2);
    outputBufferLeft.playBuffer();
    outputBufferRight.playBuffer();
    
    return true;
  }

  void setLoopbackGain(int gainValue) {
    monitorMixerI2SLeft.gain(1, gainValue);
    monitorMixerI2SRight.gain(1, gainValue);
//    monitorMixerUSBLeft.gain(1, gainValue);
//    monitorMixerUSBRight.gain(1, gainValue);
  }

  void enableLoopback() {
    this->setLoopbackGain(1);
  }

  void disableLoopback() {
    this->setLoopbackGain(0);
  }

private:
  void startInputBuffer(){
    Serial.println("Begin Buffer");
    inputBufferLeft.begin();
    inputBufferRight.begin();
  }
};

#endif
