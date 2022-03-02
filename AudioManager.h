#ifndef AUDIO_MANAGER_H
#define AUDIO_MANAGER_H

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>


// GUItool: begin automatically generated code
AudioPlayQueue           loopbackBufferI2SLeft;         //xy=1436.000144958496,239.8889274597168
AudioPlayQueue           loopbackBufferUSBLeft;         //xy=1436.500144958496,200.3889274597168
AudioPlayQueue           loopbackBufferI2SRight;         //xy=1436.000144958496,332.8889274597168
AudioPlayQueue           loopbackBufferUSBRight;         //xy=1437.000144958496,293.8889274597168
AudioInputI2S            inputDeviceI2S; //xy=1597.5555744171143,511.4443984031677
AudioInputUSB            inputDeviceUSB; //xy=1600.111078262329,450.55551052093506
AudioPlayQueue           outputBufferLeft; //xy=1645.0555534362793,25.750000953674316
AudioPlayQueue           outputBufferRight; //xy=1650.4724502563477,62.25000762939453
AudioMixer4              loopbackMixerLeft;         //xy=1673.000144958496,224.8889274597168
AudioMixer4              loopbackMixerRight;         //xy=1676.000144958496,318.8889274597168
AudioMixer4              inputMixerLeft;         //xy=1810.0000019073486,448.8888397216797
AudioMixer4              inputMixerRight;         //xy=1813.3331966400146,531.1110668182373
AudioRecordQueue         inputBufferLeft;         //xy=1992.222173690796,447.7777347564697
AudioRecordQueue         inputBufferRight;         //xy=1996.6666778988308,531.1110649108887
AudioMixer4              monitorMixerI2SLeft;         //xy=2010.000144958496,240.0000286102295
AudioMixer4              monitorMixerUSBLeft;         //xy=2011.6667137145996,54.9444465637207
AudioMixer4              monitorMixerUSBRight;         //xy=2014.9166831970215,139.19444465637207
AudioMixer4              monitorMixerI2SRight;         //xy=2015.5556335449219,326.66669845581055
AudioOutputUSB           outputDeviceUSB; //xy=2246.1666831970215,97.44444465637207
AudioOutputI2S           outputDeviceI2S; //xy=2273.833122253418,281.80556869506836
AudioConnection          patchCord1(loopbackBufferI2SLeft, 0, loopbackMixerLeft, 1);
AudioConnection          patchCord2(loopbackBufferUSBLeft, 0, loopbackMixerLeft, 0);
AudioConnection          patchCord3(loopbackBufferI2SRight, 0, loopbackMixerRight, 1);
AudioConnection          patchCord4(loopbackBufferUSBRight, 0, loopbackMixerRight, 0);
AudioConnection          patchCord5(inputDeviceI2S, 0, inputMixerLeft, 1);
AudioConnection          patchCord6(inputDeviceI2S, 1, inputMixerRight, 1);
AudioConnection          patchCord7(inputDeviceUSB, 0, inputMixerLeft, 0);
AudioConnection          patchCord8(inputDeviceUSB, 1, inputMixerRight, 0);
AudioConnection          patchCord9(outputBufferLeft, 0, monitorMixerUSBLeft, 0);
AudioConnection          patchCord10(outputBufferLeft, 0, monitorMixerI2SLeft, 0);
AudioConnection          patchCord11(outputBufferRight, 0, monitorMixerUSBRight, 0);
AudioConnection          patchCord12(outputBufferRight, 0, monitorMixerI2SRight, 0);
AudioConnection          patchCord13(loopbackMixerLeft, 0, monitorMixerUSBLeft, 1);
AudioConnection          patchCord14(loopbackMixerLeft, 0, monitorMixerI2SLeft, 1);
AudioConnection          patchCord15(loopbackMixerRight, 0, monitorMixerI2SRight, 1);
AudioConnection          patchCord16(loopbackMixerRight, 0, monitorMixerUSBRight, 1);
AudioConnection          patchCord17(inputMixerLeft, inputBufferLeft);
AudioConnection          patchCord18(inputMixerRight, inputBufferRight);
AudioConnection          patchCord19(monitorMixerI2SLeft, 0, outputDeviceI2S, 0);
AudioConnection          patchCord20(monitorMixerUSBLeft, 0, outputDeviceUSB, 0);
AudioConnection          patchCord21(monitorMixerUSBRight, 0, outputDeviceUSB, 1);
AudioConnection          patchCord22(monitorMixerI2SRight, 0, outputDeviceI2S, 1);
AudioControlSGTL5000     sgtl5000_1;     //xy=1833.2222385406494,586.7777557373047
// GUItool: end automatically generated code


// on monitor mixers, channel 0 is output, 1 loopback
class AudioManager{

public:
  void Setup(){
    AudioMemory(60);
    sgtl5000_1.enable();

    sgtl5000_1.inputSelect(AUDIO_INPUT_LINEIN);
    
    // max without clipping from headphone output is 0.7
    sgtl5000_1.volume(0.7);
    
    startInputBuffer();
    
  }
  
  bool getInputAudioBuffers(uint8_t localAudioBufferLeft[256], uint8_t localAudioBufferRight[256]){
    if (inputBufferLeft.available() >= 1 && inputBufferRight.available() >= 1) {
      // Fetch 1 blocks from the audio library
      memcpy(localAudioBufferLeft, inputBufferLeft.readBuffer(), 256);
      inputBufferLeft.freeBuffer();
      
      memcpy(localAudioBufferRight, inputBufferRight.readBuffer(), 256);
      inputBufferRight.freeBuffer();

      return true;
    } else {
      return false;
    }
  }

  bool setOutputAudioBuffers(uint8_t outputAudioBufferLeft[256], uint8_t outputAudioBufferRight[256]){
    //  look at calling buffer.play(buffer, size) if there is performance gained
    memcpy(outputBufferLeft.getBuffer(), outputAudioBufferLeft, 256);
    memcpy(outputBufferRight.getBuffer(), outputAudioBufferRight, 256);
    outputBufferLeft.playBuffer();
    outputBufferRight.playBuffer();
    
    return true;
  }

  void setPlaybackAudioBuffers(uint8_t loopbackAudioBufferLeft[256], uint8_t loopbackAudioBufferRight[256]){
    memcpy(loopbackBufferUSBLeft.getBuffer(), loopbackAudioBufferLeft, 256);
    memcpy(loopbackBufferUSBRight.getBuffer(), loopbackAudioBufferRight, 256);
    loopbackBufferUSBLeft.playBuffer();
    loopbackBufferUSBRight.playBuffer();
  }

  void setLoopbackGain(int gainValue) {
    monitorMixerI2SLeft.gain(1, gainValue);
    monitorMixerI2SRight.gain(1, gainValue);
    monitorMixerUSBLeft.gain(1, gainValue);
    monitorMixerUSBRight.gain(1, gainValue);
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
