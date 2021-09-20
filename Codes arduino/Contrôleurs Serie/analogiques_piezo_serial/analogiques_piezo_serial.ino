//#define DEBUG

#include <MIDI.h>
MIDI_CREATE_INSTANCE(HardwareSerial, Serial1, MIDI);

int pinRead[9] = {A0, A1, A2, A3, A4, A5, A6, A7, A8};
uint16_t valRead[9] = {0};
uint16_t valMSB[9] = {0};
uint16_t valLSB[9] = {0};
uint16_t lastMSB[9] = {0};
unsigned long previousMillis = 0;
const long interval = 20;

void setup() {
  //Serial.begin(115200);
  Serial1.begin(31250);
  MIDI.begin();
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  for (int i = 0; i < 9; i++) {
    pinMode(pinRead[i], INPUT);
  }
  analogReadResolution(14);
}

void loop() {
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    for (int i = 0; i < 9; i++) {
      valRead[i] = analogRead(pinRead[i]);
      valMSB[i] = valRead[i] >> 7;
      valLSB[i] = valRead[i] & 127;
      MIDI.sendControlChange(1 + i, valMSB[i], 2);
      MIDI.sendControlChange(33 + i, valLSB[i], 2);
      int velocite = valMSB[i] - lastMSB[i];
      if (velocite > 25) {
        //        Serial.print(i);
        //        Serial.print(" : ");
        //        Serial.print(velocite);
        //        Serial.println();
        MIDI.sendNoteOn(61 + i, velocite, 2);
      }
      lastMSB[i] = valMSB[i];
    }

    if (Serial1.available() >= 3) {
      byte type_channel = Serial1.read();
      if (type_channel >= 127) {
        int numero = Serial1.read();
        int velocite = Serial1.read();
        int Type = type_channel >> 4;
        int channel = type_channel & B00001111 ;
        channel = channel + 1;
        //Serial.println(String("Type: ") + Type + ", Channel = " + channel + ", Numero = " + numero + ", Vélocité = " + velocite);
        if (Type == 11) { // Control Change
          MIDI.sendControlChange(numero, velocite, channel);
        }
        if (Type == 9) { // Note On
          MIDI.sendNoteOn(numero, velocite, channel);
        }
        if (Type == 8) { // Note Off
          MIDI.sendNoteOff(numero, velocite, channel);
        }
      }
    }

  }


  //#ifdef DEBUG
  //  for (int i = 0; i < 9; i++) {
  //    Serial.print(valMSB[i], DEC);
  //    Serial.print("\t");
  //    Serial.print(valLSB[i], DEC);
  //    Serial.print("\t");
  //  }
  //  Serial.println();
  //#endif
}
