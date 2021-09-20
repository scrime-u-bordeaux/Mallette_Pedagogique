#define DEBUG

int pinRead[9] = {A0, A1, A2, A3, A4, A5, A6, A7, A8};
uint16_t valRead[9] = {0};
uint16_t valMSB[9] = {0};
uint16_t valLSB[9] = {0};
uint16_t lastMSB[9] = {0};
unsigned long previousMillis = 0;
const long interval = 20;

void setup() {
  Serial.begin(115200);
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
      usbMIDI.sendControlChange(1 + i, valMSB[i], 1);
      usbMIDI.sendControlChange(33 + i, valLSB[i], 1);
      int velocite = valMSB[i] - lastMSB[i];
      if (velocite > 25) {
        Serial.print(i);
        Serial.print(" : ");
        Serial.print(velocite);
        Serial.println();
        usbMIDI.sendNoteOn(60+i,velocite,1);
      }
      lastMSB[i] = valMSB[i];
    }
  }

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
