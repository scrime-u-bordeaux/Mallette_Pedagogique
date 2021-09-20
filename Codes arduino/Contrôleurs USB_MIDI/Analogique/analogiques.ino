#define DEBUG

int pinRead[9] = {A0, A1, A2, A3, A4, A5, A6, A7, A8};
uint16_t valRead[9] = {0};
uint16_t valMSB[9] = {0};
uint16_t valLSB[9] = {0};

void setup() {
  Serial.begin(9600);
  for (int i = 0; i < 9; i++) {
    pinMode(pinRead[i], INPUT);
  }
  analogReadResolution(14);
}

void loop() {
  for (int i = 0; i < 9; i++) {
    valRead[i] = analogRead(pinRead[i]);
    valMSB[i] = valRead[i] >> 7;
    valLSB[i] = valRead[i] & 127;
    usbMIDI.sendControlChange(1 + i, valMSB[i], 1);
    usbMIDI.sendControlChange(33 + i, valLSB[i], 1);
  }

  delay(20);

#ifdef DEBUG
  for (int i = 0; i < 9; i++) {
    Serial.print(valMSB[i], DEC);
    Serial.print("\t");
    Serial.print(valLSB[i], DEC);
    Serial.print("\t");
  }
  Serial.println();
#endif
}
