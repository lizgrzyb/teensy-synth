#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include <Bounce.h>

AudioSynthWaveform       waveform0, waveform1, waveform2, waveform3, waveform4, waveform5, waveform6, waveform7, waveform8 ;
AudioSynthWaveform       *waves[9] = {&waveform0, &waveform1, &waveform2, &waveform3, &waveform4, &waveform5, &waveform6, &waveform7, &waveform8};    //setup pointers to objects in memory
AudioEffectEnvelope      env0, env1, env2, env3, env4, env5, env6, env7, env8;
AudioEffectEnvelope      *envs[9] = {&env0, &env1, &env2, &env3, &env4, &env5, &env6, &env7, &env8};
AudioMixer4              mixer1;
AudioMixer4              mixer2;
AudioMixer4              mixer3;
//AudioOutputAnalog        dac1;
AudioOutputI2S           i2s1;
AudioConnection          patchCord01(waveform0, env0);
AudioConnection          patchCord02(waveform1, env1);
AudioConnection          patchCord03(waveform2, env2);
AudioConnection          patchCord04(waveform3, env3);
AudioConnection          patchCord05(waveform4, env4);
AudioConnection          patchCord06(waveform5, env5);
AudioConnection          patchCord07(waveform6, env6);
AudioConnection          patchCord08(waveform7, env7);
AudioConnection          patchCord09(waveform8, env8);
AudioConnection          patchCord10(env0, 0, mixer1, 0);
AudioConnection          patchCord11(env1, 0, mixer1, 1);
AudioConnection          patchCord12(env2, 0, mixer1, 2);
AudioConnection          patchCord13(env3, 0, mixer1, 3);
AudioConnection          patchCord14(env4, 0, mixer2, 0);
AudioConnection          patchCord15(env5, 0, mixer2, 1);
AudioConnection          patchCord16(env6, 0, mixer2, 2);
AudioConnection          patchCord17(env7, 0, mixer2, 3);
AudioConnection          patchCord18(mixer1, 0, mixer3, 0);
AudioConnection          patchCord19(mixer2, 0, mixer3, 1);
AudioConnection          patchCord20(env8, 0, mixer3, 3);
//AudioConnection          patchCord21(mixer3, 0, dac1, 0);
AudioConnection          patchCord21(mixer3, 0, i2s1, 0);
AudioConnection          patchCord22(mixer3, 0, i2s1, 1);

AudioControlSGTL5000 audioShield;

short waveType[4] = {WAVEFORM_SINE, WAVEFORM_TRIANGLE, WAVEFORM_SAWTOOTH, WAVEFORM_SQUARE};
//float notes[] = {27.5, 29.1352, 30.8677, 32.7032, 34.6478, 36.7081, 38.8909, 41.2034, 43.6535, 46.2493, 48.9994, 51.9131, 55, 58.2705, 61.7354, 65.4064, 69.2957, 73.4162, 77.7817, 82.4069, 87.3071, 92.4986, 97.9989, 103.826, 110, 116.541, 123.471, 130.813, 138.591, 146.832, 155.563, 164.814, 174.614, 184.997, 195.998, 207.652, 220, 233.082, 246.942, 261.626, 277.183, 293.665, 311.127, 329.628, 349.228, 369.994, 391.995, 415.305, 440, 466.164, 493.883, 523.251, 554.365, 587.33, 622.254, 659.255, 698.456, 739.989, 783.991, 830.609, 880, 932.328, 987.767, 1046.5, 1108.73, 1174.66, 1244.51, 1318.51, 1396.91, 1479.98, 1567.98, 1661.22, 1760, 1864.66, 1975.53, 2093, 2217.46, 2349.32, 2489.02, 2637.02, 2793.83, 2959.96, 3135.96, 3322.44, 3520, 3729.31, 3951.07};
float notesMajor[9] = {82.4069, 164.814, 184.997, 207.652, 220, 246.942, 277.183, 311.127, 329.628};
int waveOn[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
int buttonPins[9] = {0, 1, 2, 3, 4, 5, 6, 7, 8};

Bounce buttonBounce[8] = { 
  Bounce(buttonPins[0],5),
  Bounce(buttonPins[1],5),
  Bounce(buttonPins[2],5),
  Bounce(buttonPins[3],5),
  Bounce(buttonPins[4],5),
  Bounce(buttonPins[5],5),
  Bounce(buttonPins[6],5),
  Bounce(buttonPins[7],5) };

void setup() {
  Serial.begin(57600);
  for(int i=0; i<9;i++){
    pinMode(buttonPins[i], INPUT_PULLUP);
  }

  AudioMemory(20);
  //dac1.analogReference(INTERNAL);   //normal volume
  //dac1.analogReference(EXTERNAL);     //louder
  audioShield.enable();
  audioShield.volume(0.8);
  

  //setup envelope for pleasing attack/release
  for (int i = 0; i < 9; i++) {
    envs[i]->delay(0);
    envs[i]->attack(40);
    envs[i]->hold(1);
    envs[i]->decay(1);
    envs[i]->sustain(2);
    envs[i]->release(40);
  }

}

int wType = 2; //0 = Sine; 1 = Triangle; 2 = Sawtooth; 3 = Square

void loop() {

  for(int i=0; i<8; i++){
    if(buttonBounce[i].update()){
      if(buttonBounce[i].fallingEdge()){
        Serial.print("button ");Serial.print(buttonPins[i]);Serial.println(" ON");
        waves[i]->begin(0.2, notesMajor[i], waveType[wType]);
        envs[i]->noteOn();
      } else if (buttonBounce[i].risingEdge()){
        Serial.print("button ");Serial.print(buttonPins[i]);Serial.println(" OFF");
        envs[i]->noteOff();
      }
    }
  }
  
}







