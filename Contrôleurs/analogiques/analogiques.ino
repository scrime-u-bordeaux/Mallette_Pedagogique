//#define DEBUG

int pinRead = A0;

void setup() {
  Serial.begin(9600);
  pinMode(pinRead, INPUT);
  analogReadResolution(14);
}

void loop() {
  uint16_t valRead = analogRead(pinRead);
  uint16_t valMSB = valRead >> 7;
  uint16_t valLSB = valRead & 127;

  usbMIDI.sendControlChange(1, valMSB, 1);
  usbMIDI.sendControlChange(33, valLSB, 1);
  delay(20);

#ifdef DEBUG
  Serial.print(valMSB, DEC);
  Serial.print("\t");
  Serial.print(valLSB, DEC);
  Serial.println();
  delay(50);
#endif
}
