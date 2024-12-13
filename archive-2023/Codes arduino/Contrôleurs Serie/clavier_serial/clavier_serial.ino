#include <Bounce.h>

#include <MIDI.h>
MIDI_CREATE_INSTANCE(HardwareSerial, Serial3, MIDI);

int b0[7] = {22, 0, 2, 4, 6, 8, 10};
int b1[7] = {23, 1, 3, 5, 7, 9, 11};

int bend = 0;
int lastbend = 0;

Bounce b00 = Bounce(b0[0], 10);
Bounce b01 = Bounce(b1[0], 10);
Bounce b10 = Bounce(b0[1], 10);
Bounce b11 = Bounce(b1[1], 10);
Bounce b20 = Bounce(b0[2], 10);
Bounce b21 = Bounce(b1[2], 10);
Bounce b30 = Bounce(b0[3], 10);
Bounce b31 = Bounce(b1[3], 10);
Bounce b40 = Bounce(b0[4], 10);
Bounce b41 = Bounce(b1[4], 10);
Bounce b50 = Bounce(b0[5], 10);
Bounce b51 = Bounce(b1[5], 10);
Bounce b60 = Bounce(b0[6], 10);
Bounce b61 = Bounce(b1[6], 10);

int flags[7] = {0};
int time1[7] = {0};
int velocite[7] = {0};
int top = 40000;
int bot = 1000;

void maj() {
  b00.update();
  b01.update();
  b10.update();
  b11.update();
  b20.update();
  b21.update();
  b30.update();
  b31.update();
  b40.update();
  b41.update();
  b50.update();
  b51.update();
  b60.update();
  b61.update();
}

void setup() {
  //Serial.begin(115200);
  Serial3.begin(31250);
  Serial3.setTX(20);
  MIDI.begin();
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  for (int i = 0; i < 12; i++) {
    pinMode(i, INPUT_PULLUP);
  }
  pinMode(22, INPUT_PULLUP);
  pinMode(23, INPUT_PULLUP);
  analogReadResolution(14);
}

void loop() {
  maj();

  bend = analogRead(A0);
//  bend = map(bend, 0, 16383, -8192, 8191);
//  bend = constrain(bend, -8192, 8191);
  MIDI.sendPitchBend(bend, 5);
  //Serial.println(bend);

  if (b00.fallingEdge()) {
    flags[0] = 1;
    time1[0] = micros();
  }
  if (b01.fallingEdge() && flags[0] == 1) {
    flags[0] = 0;
    velocite[0] = micros() - time1[0];
    velocite[0] = map(velocite[0], bot, top, 0, 127);
    velocite[0] = 127 - velocite[0];
    velocite[0] = constrain(velocite[0], 5, 127);
    //Serial.println(velocite[0]);
    MIDI.sendNoteOn(60, velocite[0], 5);
  }
  if (b01.fallingEdge()) {
    MIDI.sendNoteOff(60, 0, 5);
  }

  if (b10.fallingEdge()) {
    flags[1] = 1;
    time1[1] = micros();
  }
  if (b11.fallingEdge() && flags[1] == 1) {
    flags[1] = 0;
    velocite[1] = micros() - time1[1];
    velocite[1] = map(velocite[1], bot, top, 0, 127);
    velocite[1] = 127 - velocite[1];
    velocite[1] = constrain(velocite[1], 5, 127);
    //Serial.println(velocite[1]);
    MIDI.sendNoteOn(61, velocite[1], 5);
  }
  if (b11.fallingEdge()) {
    MIDI.sendNoteOff(61, 0, 5);
  }


  if (b20.fallingEdge()) {
    flags[2] = 1;
    time1[2] = micros();
  }
  if (b21.fallingEdge() && flags[2] == 1) {
    flags[2] = 0;
    velocite[2] = micros() - time1[2];
    velocite[2] = map(velocite[2], bot, top, 0, 127);
    velocite[2] = 127 - velocite[2];
    velocite[2] = constrain(velocite[2], 5, 127);
    //Serial.println(velocite[2]);
    MIDI.sendNoteOn(62, velocite[2], 5);
  }
  if (b21.fallingEdge()) {
    MIDI.sendNoteOff(62, 0, 5);
  }


  if (b30.fallingEdge()) {
    flags[3] = 1;
    time1[3] = micros();
  }
  if (b31.fallingEdge() && flags[3] == 1) {
    flags[3] = 0;
    velocite[3] = micros() - time1[3];
    velocite[3] = map(velocite[3], bot, top, 0, 127);
    velocite[3] = 127 - velocite[3];
    velocite[3] = constrain(velocite[3], 5, 127);
    //Serial.println(velocite[3]);
    MIDI.sendNoteOn(63, velocite[3], 5);
  }
  if (b31.fallingEdge()) {
    MIDI.sendNoteOff(63, 0, 5);
  }

  if (b40.fallingEdge()) {
    flags[4] = 1;
    time1[4] = micros();
  }
  if (b41.fallingEdge() && flags[4] == 1) {
    flags[4] = 0;
    velocite[4] = micros() - time1[4];
    velocite[4] = map(velocite[4], bot, top, 0, 127);
    velocite[4] = 127 - velocite[4];
    velocite[4] = constrain(velocite[4], 5, 127);
    //Serial.println(velocite[4]);
    MIDI.sendNoteOn(64, velocite[4], 5);
  }
  if (b41.fallingEdge()) {
    MIDI.sendNoteOff(64, 0, 5);
  }

  if (b50.fallingEdge()) {
    flags[5] = 1;
    time1[5] = micros();
  }
  if (b51.fallingEdge() && flags[5] == 1) {
    flags[5] = 0;
    velocite[5] = micros() - time1[5];
    velocite[5] = map(velocite[5], bot, top, 0, 127);
    velocite[5] = 127 - velocite[5];
    velocite[5] = constrain(velocite[5], 5, 127);
    //Serial.println(velocite[5]);
    MIDI.sendNoteOn(65, velocite[5], 5);
  }
  if (b51.fallingEdge()) {
    MIDI.sendNoteOff(65, 0, 5);
  }

  if (b60.fallingEdge()) {
    flags[6] = 1;
    time1[6] = micros();
  }
  if (b61.fallingEdge() && flags[6] == 1) {
    flags[6] = 0;
    velocite[6] = micros() - time1[6];
    velocite[6] = map(velocite[6], bot, top, 0, 127);
    velocite[6] = 127 - velocite[6];
    velocite[6] = constrain(velocite[6], 5, 127);
    //Serial.println(velocite[6]);
    MIDI.sendNoteOn(66, velocite[6], 5);
  }
  if (b61.fallingEdge()) {
    MIDI.sendNoteOff(66, 0, 5);
  }

  delay(20);

}
