#include <Audio.h>
#include <Wire.h>
//#include <SPI.h>
//#include <SD.h>
//#include <SerialFlash.h>
#include <Bounce.h>


//// GUItool: begin automatically generated code
//AudioSynthWaveform       waveform4;      //xy=137,444
//AudioSynthWaveform       waveform1;      //xy=142,198
//AudioSynthWaveform       waveform3;      //xy=142,361
//AudioSynthWaveform       waveform2;      //xy=144,280
//AudioFilterBiquad        biquad3;        //xy=358,364
//AudioFilterBiquad        biquad1;        //xy=364,204
//AudioFilterBiquad        biquad2;        //xy=371,301
//AudioFilterBiquad        biquad4;        //xy=389,453
//AudioMixer4              mixer1;         //xy=559,274
//AudioOutputAnalogStereo  dacs1;          //xy=719,282
//AudioConnection          patchCord1(waveform4, biquad4);
//AudioConnection          patchCord2(waveform1, biquad1);
//AudioConnection          patchCord3(waveform3, biquad3);
//AudioConnection          patchCord4(waveform2, biquad2);
//AudioConnection          patchCord5(biquad3, 0, mixer1, 2);
//AudioConnection          patchCord6(biquad1, 0, mixer1, 0);
//AudioConnection          patchCord7(biquad2, 0, mixer1, 1);
//AudioConnection          patchCord8(biquad4, 0, mixer1, 3);
//AudioConnection          patchCord9(mixer1, 0, dacs1, 0);
//AudioConnection          patchCord10(mixer1, 0, dacs1, 1);
// GUItool: begin automatically generated code
AudioSynthWaveform       waveform4;      //xy=137,444
AudioSynthWaveform       waveform1;      //xy=142,198
AudioSynthWaveform       waveform3;      //xy=142,361
AudioSynthWaveform       waveform2;      //xy=144,280
AudioFilterStateVariable filter2;        //xy=373,263
AudioFilterStateVariable filter3;        //xy=376,340
AudioFilterStateVariable filter4;        //xy=384,418
AudioFilterStateVariable filter1;        //xy=386,191
AudioMixer4              mixer1;         //xy=559,274
AudioEffectFreeverbStereo freeverbs1;     //xy=648,375
AudioOutputAnalogStereo  dacs1;          //xy=719,282
AudioConnection          patchCord1(waveform4, 0, filter4, 0);
AudioConnection          patchCord2(waveform1, 0, filter1, 0);
AudioConnection          patchCord3(waveform3, 0, filter3, 0);
AudioConnection          patchCord4(waveform2, 0, filter2, 0);
AudioConnection          patchCord5(filter2, 0, mixer1, 1);
AudioConnection          patchCord6(filter3, 0, mixer1, 2);
AudioConnection          patchCord7(filter4, 0, mixer1, 3);
AudioConnection          patchCord8(filter1, 0, mixer1, 0);
AudioConnection          patchCord9(mixer1, freeverbs1);
AudioConnection          patchCord10(freeverbs1, 0, dacs1, 0);
AudioConnection          patchCord11(freeverbs1, 1, dacs1, 1);
// GUItool: end automatically generated code
// GUItool: end automatically generated code
#include <Smoothed.h>
Smoothed <uint32_t> cs0av;
Smoothed <uint32_t> cs1av;
Smoothed <uint32_t> cs2av;
Smoothed <uint32_t> cs3av;
const int led1 = 33;
const int led2 = 34;
const int bouton = 0;
uint32_t csc;
const byte nb_cs = 16;
byte out[nb_cs] = {3, 5, 7, 9, 11, 25, 27, 29, 36, 38, 13, 15, 17, 19, 21, 23};
byte in[nb_cs] = {4, 6, 8, 10, 12, 26, 28, 30, 35, 37, 39, 14, 16, 18, 20, 22};
int calib[nb_cs];
int i;
uint32_t cs[nb_cs];

void setup()
{
  Serial.begin(115200);
  pinMode(led1, OUTPUT); pinMode(led2, OUTPUT);
  digitalWrite(led1, LOW);
  digitalWrite(led2, HIGH);
  pinMode(bouton, INPUT_PULLUP);
  for (i = 0; i < nb_cs; i++) {
    pinMode(in[i], INPUT);
    pinMode(out[i], OUTPUT);
    digitalWrite(out[i], LOW);
  }
  cs0av.begin(SMOOTHED_AVERAGE, 10); // TODO !!!! x16
  cs1av.begin(SMOOTHED_AVERAGE, 10);
  cs2av.begin(SMOOTHED_AVERAGE, 10);
  cs3av.begin(SMOOTHED_AVERAGE, 10);
  delay(500);
  Serial.print("go2");
  AudioMemory(80);
  waveform1.begin(0.8, 69, WAVEFORM_SAWTOOTH);// Tierces pure
  waveform2.begin(0.8, 86, WAVEFORM_SAWTOOTH);
  waveform3.begin(0.8, 108, WAVEFORM_SAWTOOTH);
  waveform4.begin(0.8, 135, WAVEFORM_SAWTOOTH);
  filter1.frequency(1);
  filter2.frequency(1);
  filter3.frequency(1);
  filter4.frequency(1);
  filter1.resonance(0.8);
  filter2.resonance(0.8);
  filter3.resonance(0.8);
  filter4.resonance(0.8);
  freeverbs1.roomsize(0.8);
  freeverbs1.damping(0.1);
  mixer1.gain(0, 0.9);
  mixer1.gain(1, 0.9);
  mixer1.gain(2, 0.9);
  mixer1.gain(3, 0.9);
  calibration();
}
void calibration() {
  for (i = 0; i < nb_cs; i++) {
    calib[i] = 0;
  }
  for (int n = 0; n < 100; n++) {
    for (int m = 0; m < nb_cs; m++) {
      capsens(m);
      calib[m] = calib[m] + cs[m];
    }
    delay(10);
    digitalWrite(led1, LOW);
    digitalWrite(led2, HIGH);
    delay(10);
    digitalWrite(led2, LOW);
    digitalWrite(led1, HIGH);
  }
  for (i = 0; i < nb_cs; i++) {
    calib[i] = calib[i] / 100;

  }
}
void capsens(int i) {
  csc = 0;
  noInterrupts();
  pinMode(in[i], INPUT);
  digitalWrite(out[i], HIGH);
  while (digitalReadFast(in[i]) == LOW) {
    csc = csc + 1;
  }
  interrupts();
  digitalWrite(out[i], LOW);
  pinMode(in[i], OUTPUT); digitalWrite(in[i], LOW);
  cs[i] = csc;
}
void loop()
{
  if (digitalRead(bouton) == 0) {
    calibration();
  }
  long start = millis();
  for (i = 0; i < nb_cs; i++) {
    capsens(i);
  }
  Serial.print(millis() - start);
  Serial.print("\t");
  //capsens(0);
  //capsens(1);
  //capsens(2);
  //capsens(3);

  cs0av.add(cs[0]);
  cs1av.add(cs[1]);
  cs2av.add(cs[2]);
  cs3av.add(cs[3]);

  //Serial.print(cs[0]);
  //Serial.print("\t");
  //Serial.print(cs0av.get());

  int F1 = cs0av.get() - calib[0] - 20;
  F1 = constrain(F1, 3, 250);
  F1 = sq(F1) / 6;


  int F2 = cs1av.get() - calib[1] - 20;
  F2 = constrain(F2, 3, 250);
  F2 = sq(F2) / 6;

  int F3 = cs2av.get() - calib[2] - 20;
  F3 = constrain(F3, 3, 250);
  F3 = sq(F3) / 6;

  int F4 = cs3av.get() - calib[3] - 20;
  F4 = constrain(F4, 3, 250);
  F4 = sq(F4) / 6;

  AudioNoInterrupts();
  filter1.frequency(F1);
  filter2.frequency(F2);
  filter3.frequency(F3);
  filter4.frequency(F4);
  AudioInterrupts();
  //Serial.print(F1);
  //  Serial.print("\t");
  //  Serial.print(F1);
  //  Serial.print("\t");
  //  Serial.print(F2);
  //  Serial.print("\t");
  //  Serial.print(F3);
  //  Serial.print("\t");
  //  Serial.print(F4);
  for (i = 0; i < nb_cs; i++) {
    Serial.print("\t");
    Serial.print(cs[i]);
  }
  Serial.println();
  // print sensor output 1

  //    Serial.print(total2);                  // print sensor output 2
  //    Serial.print("\t");
  //                  // print sensor output 3

  delay(10);                             // arbitrary delay to limit data to serial port
}
