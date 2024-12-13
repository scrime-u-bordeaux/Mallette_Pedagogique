//#define DEBUG

int pinRead[9] = {A0, A1, A2, A3, A4, A5, A6, A7, A8};
int valRead[9] = {0};
int valMSB[9] = {0};
int valLSB[9] = {0};

void setup() {
  Serial.begin(9600);
  for (int i = 0; i < 9; i++) {
    pinMode(pinRead[i], INPUT);
  }
  analogReadResolution(14);
}

void loop() {
  for (int i = 0; i < 9; i++) {
    uint16_t valRead[i] = analogRead(pinRead[i]);
    uint16_t valMSB[i] = valRead[i] >> 7;
    uint16_t valLSB[i] = valRead[i] & 127;
    usbMIDI.sendControlChange(1 + i, valMSB, 1);
    usbMIDI.sendControlChange(33 + i, valLSB, 1);
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
