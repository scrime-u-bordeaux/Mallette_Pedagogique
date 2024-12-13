#include <Bounce.h>

int pin[10] = {0, 1, 3, 4, 15, 16, 17, 18, 19, 22};
int touch[10] = {0};
int lastTouch[10] = {0};
int calib[10] = {0};
int midiCC[10] = {0};

Bounce rst = Bounce(8, 10);

void setup() {
  Serial.begin(9600);
  pinMode(8, INPUT_PULLUP);
  for (int i = 0; i < 10; i++) {
    pinMode(pin[i],INPUT);
  }
  Serial.println("Calibration...");
  calibration();
  delay(5000);
}

bool delai(){
  delay(5000);
  return true;
}

void loop() {
  rst.update();
  for (int i = 0; i < 10; i++) {
    touch[i] = touchRead(pin[i]);
    touch[i] = touch[i] * 0.9 + lastTouch[i] * 0.1;
    lastTouch[i] = touch[i];
  }
  for (int i = 0; i < 10; i++) {
    midiCC[i] = (touch[i] - 1.1 * calib[i]) * 512 / calib[i];
    midiCC[i] = constrain( midiCC[i], 0, 127);
    usbMIDI.sendControlChange(101 + i, midiCC[i], 1);
    Serial.print(midiCC[i]);
    Serial.print("\t");
  }
  Serial.println();

  if(rst.fallingEdge()){
    Serial.println("Calibration...");
    calibration();
    delay(5000);
  }

  delay(20);
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
