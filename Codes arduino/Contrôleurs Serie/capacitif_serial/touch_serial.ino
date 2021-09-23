#include <Bounce.h>
#include <MIDI.h>
MIDI_CREATE_INSTANCE(HardwareSerial, Serial2, MIDI);

int pin[10] = {0, 1, 3, 4, 15, 16, 17, 18, 19, 22};
int touch[10] = {0};
int lastTouch[10] = {0};
int calib[10] = {0};
int midiCC[10] = {0};

Bounce rst = Bounce(8, 10);

unsigned long previousMillis = 0;
const long interval = 40;

void setup() {
  //Serial.begin(9600);
  Serial2.begin(31250);
  MIDI.begin();
  pinMode(8, INPUT_PULLUP);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  for (int i = 0; i < 10; i++) {
    pinMode(pin[i], INPUT);
  }
  //Serial.println("Calibration...");
  calibration();
  digitalWrite(LED_BUILTIN, LOW);
  delay(1000);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);
  digitalWrite(LED_BUILTIN, LOW);
  delay(1000);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);
  digitalWrite(LED_BUILTIN, LOW);
  delay(1000);
  digitalWrite(LED_BUILTIN, HIGH);
  //delay(5000);
}

void loop() {
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    rst.update();
    for (int i = 0; i < 10; i++) {
      touch[i] = touchRead(pin[i]);
      touch[i] = touch[i] * 0.9 + lastTouch[i] * 0.1;
      lastTouch[i] = touch[i];
    }
    for (int i = 0; i < 10; i++) {
      midiCC[i] = (touch[i] - 1.1 * calib[i]) * 512 / calib[i];
      midiCC[i] = constrain( midiCC[i], 0, 127);
      MIDI.sendControlChange(101 + i, midiCC[i], 3);
      //Serial.print(midiCC[i]);
      //Serial.print("\t");
    }
    //Serial.println();

    if (rst.fallingEdge()) {
      //Serial.println("Calibration...");
      calibration();
      digitalWrite(LED_BUILTIN, LOW);
      delay(1000);
      digitalWrite(LED_BUILTIN, HIGH);
      delay(1000);
      digitalWrite(LED_BUILTIN, LOW);
      delay(1000);
      digitalWrite(LED_BUILTIN, HIGH);
      delay(1000);
      digitalWrite(LED_BUILTIN, LOW);
      delay(1000);
      digitalWrite(LED_BUILTIN, HIGH);
      //delay(5000);
    }
  }

  if (Serial2.available() >= 3) {
    byte type_channel = Serial2.read();
    if (type_channel >= 127) {
      int numero = Serial2.read();
      int velocite = Serial2.read();
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

void calibration() {
  for (int i = 0; i < 100; i++) {
    for (int j = 0; j < 10; j++) {
      calib[j] = calib[j] + touchRead(pin[j]);
    }
    delay(10);
  }
  for (int i = 0; i < 10; i++) {
    calib[i] = calib[i] / 100;
  }
}
